#!/usr/bin/env python3
"""Unsigned-load conversion pass for IDO 5.3 (T-1321).

The original game code was built by a MIPS ucode compiler that keeps a global
`u8`/`u16` variable in one register across basic blocks and calls (a compare
chain on the global, then the reload after a call goes into the same
register, `lui v1; lbu v1,%lo(D)(v1)`). IDO 5.3 does not, and the reason is
one ucode instruction: its front end (cfe) writes every load of an unsigned
8- or 16-bit variable as `LOD L` followed by `CVT J<-L` (widen to int).
uopt then treats `cvt(D)` as an expression of its own: the compare chain uses
the expression (a CSE temporary, usually `$v0`), and the variable `D` itself
is left with too few uses to be worth a register, so its later loads get
fresh ugen temporaries. A signed `s8`/`s16` global, whose load needs no
`CVT`, is already promoted by IDO exactly like the original does (measured,
wiki/matching-notes.md, "Unsigned-load conversion pass (T-1321)").

The rule
--------
For every global (memory type S) variable that the procedure accesses only
as unsigned and narrower than a word, the `CVT J<-L` that directly follows a
`LOD` of it is removed before uopt runs. uopt then sees plain variable uses
and allocates registers for the variable as it does for an `int` or signed
global. Two consequences of the removal are undone, so that nothing else
changes:

  * operand order of `==`/`!=`: uopt puts a plain variable first when the
    left operand is not one (uoptinput, "swap == and !="). With the CVT the
    load was not a plain variable. The pass reorders the two operands in the
    ucode so that uopt's result is the order it would have chosen with the
    CVT; when no input order gives that result (the expression would have
    to come before the variable), that one CVT is kept;
  * signedness: without the CVT uopt types the load by its use (int) and
    would emit `lb`/`lh`. After uopt, every load (`LOD`, `RLOD`) of a
    rewritten variable gets its unsigned type back, so ugen emits
    `lbu`/`lhu` as before.

Constant operands (`LDC`, `LDA`) do not take part in the reordering: ugen
emits the compare the same way for either order.

Layer
-----
IDO runs cfe -> uopt -> ugen -> as1. tools/cc.py points IDO's USR_LIB at a
directory whose `uopt` is a shim for this file: it rewrites uopt's input
ucode, runs the real uopt, and rewrites uopt's output. The ucode is IDO's
binary ucode (big-endian 32-bit words; record layout from the ido-decomp
project's ucode.h and libu/uini.c). Anything the pass does not understand
(an unknown opcode, an operand it cannot move, a truncated file) aborts with
PassError; the build fails and nothing is guessed.

Usage as the uopt shim:  cvt_pass.py --uopt /opt/ido/5.3/uopt <uopt args...>
"""
import os
import struct
import subprocess
import sys
import tempfile

# Uopcode enum (ucode.h), in order.
OPS = """abs add adj aent and aos asym bgn bgnb bsub cg1 cg2 chkh chkl chkn chkt
cia clab clbd comm csym ctrl cubd cup cvt cvtl dec def dif div dup end
endb ent eof equ esym fill fjp fsym geq grt gsym hsym icuf idx iequ igeq
igrt ijp ilda ildv ileq iles ilod inc ineq init inn int ior isld isst istr
istv ixa lab lbd lbdy lbgn lca lda ldap ldc ldef ldsp lend leq les lex
lnot loc lod lsym ltrm max min mod mov movv mpmv mpy mst mus neg neq
nop not odd optn par pdef pmov pop regs rem ret rlda rldc rlod rnd rpar
rstr sdef sgs shl shr sign sqr sqrt ssym step stp str stsp sub swp tjp
tpeq tpge tpgt tple tplt tpne typ ubd ujp unal uni vreg xjp xor xpar mtag
alia ildi isti irld irst ldrc msym rcuf ksym osym irlv irsv""".split()
OP = {name: i for i, name in enumerate(OPS)}

# Record length in words (uini.c instlength; default 2).
_LEN4 = """adj bgn cia clab ctrl comm cup cvt def dif int uni ent aent iequ igeq igrt
ileq iles ineq fill ildv ilod inn istv istr lab ldef lca ldc lod mov mus optn
par pdef regs rlda rldc rlod rnd rpar rstr sdef asym hsym sgs str swp csym esym
fsym gsym lsym icuf isld isst pmov mpmv vreg typ ssym rcuf ldrc msym ksym osym
ildi isti irld irst""".split()
LENGTH = {OP[n]: 4 for n in _LEN4}
LENGTH.update({OP["init"]: 6, OP["ilda"]: 6, OP["lda"]: 6, OP["xjp"]: 8})
HASCONST = {OP[n] for n in "cia comm init lca ldc rldc ssym".split()}

# Data types (ucode.h enum Datatype) and memory types (enum Memtype).
DTYPES = "ACFGHIJKLMNPQRSWXZ"
DT_J, DT_L = DTYPES.index("J"), DTYPES.index("L")
STRING_DTYPES = {DTYPES.index(c) for c in "MQRSX"}
MT_S = 4

# Expression operators: operands popped (uini.c stack_pop) for ops that push
# a value. Only these may appear inside an operand that the pass reorders;
# all are free of side effects.
EXPR_POP = dict(
    abs=1, add=2, adj=1, And=2, cvt=1, cvtl=1, dec=1, div=2, equ=2, geq=2,
    grt=2, ilod=1, inc=1, ior=2, ixa=2, lda=0, ldc=0, leq=2, les=2, lnot=1,
    lod=0, max=2, min=2, mod=2, mpy=2, neg=1, neq=2, Not=1, rem=2, shl=2,
    shr=2, sub=2, xor=2)
EXPR_POP = {OP[k.lower()]: v for k, v in EXPR_POP.items()}
CONSTS = {OP["ldc"], OP["lda"]}
VOLATILE_ATTR = 1


class PassError(Exception):
    """The pass met something it cannot transform exactly."""


class Insn:
    """One ucode record: its 32-bit words and any trailing string bytes."""

    __slots__ = ("words", "extra")

    def __init__(self, words, extra=b""):
        self.words = list(words)
        self.extra = extra

    @property
    def opc(self):
        return self.words[0] >> 24

    @property
    def dtype(self):
        return (self.words[0] >> 16) & 0x1F

    @property
    def mtype(self):
        return (self.words[0] >> 21) & 7

    @property
    def lexlev(self):
        return self.words[0] & 0xFFFF

    def set_dtype(self, dtype):
        self.words[0] = (self.words[0] & ~(0x1F << 16)) | (dtype << 16)

    def location(self):
        """(memtype, block, offset, length) of a LOD/STR/RLOD record."""
        if self.opc == OP["rlod"]:
            return (self.mtype, self.words[1], self.words[3], self.words[2])
        return (self.mtype, self.words[1], self.words[3], self.words[2])


def parse(data):
    """List of Insn for a binary ucode file; raises PassError if it is malformed."""
    if len(data) % 4:
        raise PassError("ucode size is not a multiple of 4")
    n = len(data) // 4
    words = struct.unpack(">%dI" % n, data)
    out, p = [], 0
    while p < n:
        opc = words[p] >> 24
        if opc >= len(OPS):
            raise PassError("unknown ucode opcode %d at word %d" % (opc, p))
        length = LENGTH.get(opc, 2)
        if opc in HASCONST:
            length += 2
        if p + length > n:
            raise PassError("truncated ucode record at word %d" % p)
        rec = words[p:p + length]
        p += length
        extra = b""
        if opc in HASCONST:
            dtype = (rec[0] >> 16) & 0x1F
            if dtype in STRING_DTYPES or opc == OP["comm"]:
                nwords = (rec[LENGTH.get(opc, 2)] + 3) // 4
                nwords += nwords & 1
                if p + nwords > n:
                    raise PassError("truncated ucode string at word %d" % p)
                extra = data[p * 4:(p + nwords) * 4]
                p += nwords
        out.append(Insn(rec, extra))
    return out


def serialize(insns):
    return b"".join(struct.pack(">%dI" % len(i.words), *i.words) + i.extra for i in insns)


def _unsigned_narrow_globals(insns):
    """Locations of S variables narrower than a word that are only loaded/stored as unsigned."""
    types = {}
    for i in insns:
        if i.opc in (OP["lod"], OP["str"]) and i.mtype == MT_S:
            types.setdefault(i.location(), set()).add(i.dtype)
    return {loc for loc, ts in types.items() if ts == {DT_L} and loc[3] < 4}


def _widenings(insns, unsigned):
    """Indices of the CVT J<-L records that directly follow a LOD of a location in `unsigned`."""
    found = set()
    for k in range(1, len(insns)):
        cvt, lod = insns[k], insns[k - 1]
        if (cvt.opc == OP["cvt"] and cvt.dtype == DT_J and (cvt.words[2] >> 24) == DT_L
                and lod.opc == OP["lod"] and lod.dtype == DT_L and lod.mtype == MT_S
                and lod.location() in unsigned):
            found.add(k)
    return found


def _operand(insns, end):
    """Index range [start, end] of the side-effect-free expression that ends at `end`, or None."""
    need, k = 1, end
    while k >= 0:
        i = insns[k]
        pops = EXPR_POP.get(i.opc)
        if pops is None:
            return None
        if i.opc in (OP["lod"], OP["ilod"]) and i.lexlev & VOLATILE_ATTR:
            return None
        need += pops - 1
        if need == 0:
            return (k, end)
        k -= 1
    return None


def pre(insns):
    """Remove the widenings (in place on a copy). Returns (new insns, rewritten locations)."""
    unsigned = _unsigned_narrow_globals(insns)
    strip = _widenings(insns, unsigned)
    if not strip:
        return list(insns), set()
    keep = set()
    swaps = {}   # start of the first operand -> (first range, second range)
    for k, i in enumerate(insns):
        if i.opc not in (OP["equ"], OP["neq"]):
            continue
        b = _operand(insns, k - 1)
        a = _operand(insns, b[0] - 1) if b else None
        involved = [j for j in strip if (b and b[0] <= j <= b[1]) or (a and a[0] <= j <= a[1])]
        if not involved:
            continue
        if a is None or b is None:
            raise PassError("cannot delimit the operands of %s at record %d" % (OPS[i.opc], k))

        def single(r, op):
            return r[0] == r[1] and insns[r[0]].opc == op

        def plain_with_cvt(r):     # a plain variable for uopt while the CVT is there
            return single(r, OP["lod"])

        def plain_without_cvt(r):  # ... and after the CVT is removed
            return plain_with_cvt(r) or (r[1] == r[0] + 1 and r[1] in strip
                                         and insns[r[0]].opc == OP["lod"])

        if single(a, OP["ldc"]) or single(a, OP["lda"]) or single(b, OP["ldc"]) or single(b, OP["lda"]):
            continue
        want = (a, b) if plain_with_cvt(a) else (b, a)
        if plain_without_cvt(want[0]):
            give = want
        elif not plain_without_cvt(want[1]):
            give = (want[1], want[0])
        else:
            keep.add(want[1][1])
            give = (want[1], want[0])
        if give != (a, b):
            swaps[a[0]] = (a, b)
    out, k = [], 0
    while k < len(insns):
        if k in swaps:
            a, b = swaps[k]
            order = list(range(b[0], b[1] + 1)) + list(range(a[0], a[1] + 1))
            k = b[1] + 1
        else:
            order = [k]
            k += 1
        out.extend(insns[j] for j in order if j not in strip or j in keep)
    return out, {insns[j - 1].location() for j in strip if j not in keep}


def post(insns, rewritten):
    """Give loads of the rewritten locations their unsigned type back (in place)."""
    for i in insns:
        if i.opc in (OP["lod"], OP["rlod"]) and i.dtype == DT_J and i.location() in rewritten:
            i.set_dtype(DT_L)
    return insns


VALUED = {"-G", "-t", "-l", "-f", "-i", "-p", "-varref", "-Olimit", "-loopunroll",
          "-unrolllimit", "-regr", "-rege"}


def run_uopt(real, argv):
    """Rewrite uopt's input, run the real uopt, rewrite its output. Returns uopt's exit code."""
    files, i = [], 0
    while i < len(argv):
        if argv[i] in VALUED:
            i += 2
        elif argv[i].startswith("-"):
            i += 1
        else:
            files.append(i)
            i += 1
    if len(files) < 2:
        raise PassError("uopt arguments name no input and output ucode")
    inp, outp = files[0], files[1]
    with open(argv[inp], "rb") as f:
        insns, rewritten = pre(parse(f.read()))
    fd, tmp = tempfile.mkstemp(prefix="cvtpass", suffix=".B")
    with os.fdopen(fd, "wb") as f:
        f.write(serialize(insns))
    args = list(argv)
    args[inp] = tmp
    try:
        rc = subprocess.call([real] + args)
    finally:
        os.unlink(tmp)
    if rc == 0 and rewritten:
        with open(argv[outp], "rb") as f:
            out = post(parse(f.read()), rewritten)
        with open(argv[outp], "wb") as f:
            f.write(serialize(out))
    return rc


def main(argv):
    if len(argv) < 3 or argv[1] != "--uopt":
        sys.exit(__doc__)
    try:
        sys.exit(run_uopt(argv[2], argv[3:]))
    except PassError as e:
        sys.exit("cvt_pass.py: %s" % e)


if __name__ == "__main__":
    main(sys.argv)
