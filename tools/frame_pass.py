#!/usr/bin/env python3
"""Frame-layout emulation pass for IDO 5.3 (T-0016).

The original game code was built by a MIPS ucode compiler whose stack frames
are 16 bytes larger than IDO 5.3's. This module models that one deterministic
difference and is applied to every IDO-compiled function by tools/cc.py.
It is a toolchain emulation pass in the sense of CODING_STANDARDS.md: it has
no per-function switches, annotations or lists, and the C source stays
ordinary. A real copy of the original compiler would replace it without any
change to the C code (T-0100).

The rule (evidence: wiki/matching-notes.md, "Frame layout emulation")
---------------------------------------------------------------------
For every procedure that has a stack frame (frame size F > 0 in `.frame`):

  * the frame grows by 16 bytes: F' = F + 16;
  * a 16-byte unused hole is inserted at offset H, and every stack-pointer
    relative offset >= H moves up by 16 (locals, spill temporaries, address-of
    -local computations, and the incoming-argument homes above the frame);
  * H is the end of the register-save block when the procedure saves $ra
    (non-leaf procedures, and leaf procedures that use $ra as a register), so
    the saves and the outgoing-argument area stay where IDO puts them and the
    hole sits between the saves and the locals;
  * H is 0 when the procedure does not save $ra (leaf procedures), so the
    whole frame, saves included, moves up and the hole is at the bottom.

Procedures without a frame (F == 0) are untouched.

Layer
-----
IDO runs cfe -> uopt -> ugen -> as1. ugen hands as1 a binary assembly stream
("binasm", 16-byte records, include/indy/cmplrs/binasm.h of the ido-decomp
project). as1 needs that stream rather than ugen's text listing (`cc -S`
output): the listing drops the vreg records and assembling it gives different
code. This module is run as a stand-in for the `as1` executable (tools/cc.py
points IDO's USR_LIB at a directory whose `as1` is a shim for this file): it
reads the binasm file, rewrites the frame, writes a new file and execs the real
as1. Because it works on records, it sees `.frame`/`.mask` exactly and needs no
instruction decoding.

Handled: memory operations based on $sp, `addu/addiu rd,$sp,imm`, frame
allocation and release, `.frame`, `.mask`, and `addu rd,$sp,rs` (variable
index into a local array) with the register's later uses within the same basic
block. Anything else that reads $sp, a register derived from it in a way the
pass cannot follow, a frame-pointer frame, saved FP registers, or an unknown
record layout aborts with PassError (the build fails; nothing is guessed).

Usage as the as1 shim:  frame_pass.py --as1 /opt/ido/5.3/as1 <as1 args...>
"""
import os
import struct
import sys
import tempfile

FRAME_EXTRA = 16

# itype values (binasm.h)
I_IOPTION = 47
I_IFRAME = 43
I_IMASK = 38
I_IFMASK = 39
I_IENT = 27
I_IEND = 24
I_IOCODE = 23
I_ILABEL = 0
I_ILAB = 36
I_IASCII = 5
I_IASCIIZ = 6
I_IFILE = 12
REJECTED_RECORDS = (I_IFILE,)

# format values (binasm.h)
F_FROB, F_FRA, F_FRI, F_FRRR, F_FRRI, F_FRR, F_FA, F_FR = range(8)
F_FRRL, F_FRL, F_FL, F_FORRR, F_FRIL, F_FI, F_FOA, F_FRRRR = range(8, 16)

# asmcodes (binasm.h numbers in the enum comments)
Z = dict(
    zadd=1, zaddu=2, zb=4, zbeq=13, zbge=14, zbgeu=15, zbgez=16, zbgt=17,
    zbgtu=18, zbgtz=19, zble=20, zbleu=21, zblez=22, zblt=23, zbltu=24,
    zbltz=25, zbne=26, zbreak=27, zj=34, zjal=35, zla=36, zlb=37, zlbu=38,
    zlh=39, zlhu=40, zlw=42, zjr=43, zlwc1=44, zlwc2=45, zmove=49,
    zjalr=50, zswc1=51, zswc2=52, zsb=70, zsh=76, zsubu=86, zsw=87,
    zsll=79, zlwl=91, zlwr=92, zswl=93, zswr=94, zld=108, zsd=109, zlui=205,
    zulw=206, zulh=207, zulhu=208, zusw=209, zush=210, zaddi=211,
    zaddiu=212, zbeqz=220, zbnez=221, zlwu=304, zsc=262,
)
LOADS = {Z[n] for n in ("zlb", "zlbu", "zlh", "zlhu", "zlw", "zlwl", "zlwr",
                        "zlwc1", "zlwc2", "zld", "zulw", "zulh", "zulhu",
                        "zlwu")}
STORES = {Z[n] for n in ("zsb", "zsh", "zsw", "zswl", "zswr", "zswc1",
                         "zswc2", "zsd", "zusw", "zush", "zsc")}
MEM_OPS = LOADS | STORES
# add-like ops that carry an address through unchanged
ADD_RRR = {Z["zadd"], Z["zaddu"]}
ADD_RRI = {Z["zadd"], Z["zaddu"], Z["zaddi"], Z["zaddiu"], Z["zla"]}
# ops whose reg1 is not written
NO_DEF = STORES | {Z[n] for n in (
    "zb", "zbeq", "zbge", "zbgeu", "zbgez", "zbgt", "zbgtu", "zbgtz", "zble",
    "zbleu", "zblez", "zblt", "zbltu", "zbltz", "zbne", "zbeqz", "zbnez",
    "zj", "zjr", "zbreak")}
# ops where a tainted register may be read without affecting frame offsets
READ_OK = NO_DEF | {Z["zjalr"]}

SP, RA = 29, 31
REG_NONE = 72


class PassError(Exception):
    """The pass met something it cannot transform exactly."""


def s32(x):
    return x - (1 << 32) if x & 0x80000000 else x


class Rec:
    """One 16-byte binasm record (four big-endian words)."""

    def __init__(self, words):
        self.w = list(words)

    @property
    def instr(self):
        return (self.w[1] >> 16) & 0x3F

    @property
    def op(self):
        return (self.w[1] >> 1) & 0x1FF

    @property
    def form(self):
        return (self.w[2] >> 14) & 0xF

    @property
    def reg1(self):
        return (self.w[2] >> 25) & 0x7F

    @property
    def reg2(self):
        return (self.w[2] >> 18) & 0x7F

    @property
    def reg3(self):
        return (self.w[2] >> 7) & 0x7F

    @property
    def imm(self):
        return s32(self.w[3])

    def set_imm(self, v):
        if not -0x8000 <= v <= 0x7FFF:
            raise PassError("immediate %d out of 16-bit range" % v)
        self.w[3] = v & 0xFFFFFFFF


def parse(data):
    """Split a binasm file into [(Rec, payload_bytes)], payload kept verbatim."""
    if len(data) % 16:
        raise PassError("binasm size %d is not a multiple of 16" % len(data))
    recs, i = [], 0
    while i < len(data):
        rec = Rec(struct.unpack(">4I", data[i:i + 16]))
        i += 16
        payload = b""
        if rec.instr in (I_IASCII, I_IASCIIZ):
            n = -(-rec.w[2] // 16) * 16
            payload = data[i:i + n]
            i += n
        elif rec.instr in REJECTED_RECORDS:
            raise PassError("unsupported record type %d" % rec.instr)
        elif rec.instr == I_IOPTION and (rec.w[1] >> 14) & 3 not in (1, 2):
            raise PassError("unsupported .option record")
        if i > len(data):
            raise PassError("truncated binasm payload")
        recs.append((rec, payload))
    return recs


def serialize(recs):
    out = []
    for rec, payload in recs:
        out.append(struct.pack(">4I", *rec.w))
        out.append(payload)
    return b"".join(out)


def procedures(recs):
    """Yield (start, end) index ranges, ient .. iend inclusive."""
    start = None
    for idx, (rec, _) in enumerate(recs):
        if rec.instr == I_IENT:
            if start is not None:
                raise PassError("nested procedure")
            start = idx
        elif rec.instr == I_IEND and start is not None:
            yield start, idx
            start = None
    if start is not None:
        raise PassError("procedure without end")


def transform_proc(recs, start, end):
    """Rewrite one procedure in place. Returns True if it had a frame."""
    body = [r for r, _ in recs[start:end + 1]]
    frames = [r for r in body if r.instr == I_IFRAME]
    masks = [r for r in body if r.instr == I_IMASK]
    fmasks = [r for r in body if r.instr == I_IFMASK]
    if len(frames) != 1 or len(masks) > 1:
        raise PassError("expected one .frame and at most one .mask")
    frame = frames[0]
    size = s32(frame.w[2])
    framereg = (frame.w[3] >> 25) & 0x7F
    if size == 0:
        return False
    if framereg != SP:
        raise PassError("frame register %d is not $sp" % framereg)
    if fmasks and (fmasks[0].w[2] or fmasks[0].w[3]):
        raise PassError("saved floating point registers are not supported")
    mask = masks[0] if masks else None
    mask_bits = mask.w[2] if mask else 0
    if mask_bits & (1 << RA):
        hole = size + s32(mask.w[3]) + 4      # end of the register-save block
        if not 0 < hole <= size:
            raise PassError("save block end %d outside frame %d" % (hole, size))
        saves_move = False
    else:
        hole = 0
        saves_move = True

    tainted = {}      # register -> still in the basic block of its definition
    n_alloc = 0

    def shift(rec):
        off = rec.imm
        if off < 0:
            raise PassError("negative $sp offset %d" % off)
        if off >= hole:
            rec.set_imm(off + FRAME_EXTRA)

    for rec in body:
        ins = rec.instr
        if ins in (I_ILABEL, I_ILAB):
            for r in tainted:
                tainted[r] = False
            continue
        if ins != I_IOCODE:
            continue
        op, form = rec.op, rec.form
        r1, r2 = rec.reg1, rec.reg2
        uses_sp = SP in (r1, r2) or (form in (F_FRRR, F_FRRRR) and
                                     SP in (rec.reg3,))
        # frame allocation / release: subu/addu $sp,$sp,F  (fri or frri)
        if op in (Z["zsubu"], Z["zaddu"]) and r1 == SP and (
                form == F_FRI or (form == F_FRRI and r2 == SP)):
            if rec.imm != size:
                raise PassError("$sp adjusted by %d, frame is %d" % (rec.imm, size))
            rec.set_imm(size + FRAME_EXTRA)
            n_alloc += 1
            continue
        if form in (F_FRA, F_FA, F_FRL, F_FRRL, F_FL) and uses_sp:
            raise PassError("symbolic operand using $sp")
        # memory operations
        if form == F_FROB and op in MEM_OPS:
            base = r2
            if op in STORES and r1 in tainted:
                raise PassError("$sp-derived register $%d stored as a value" % r1)
            if base == SP:
                shift(rec)
            elif base in tainted:
                if not tainted[base]:
                    raise PassError("$sp-derived register $%d used after a label" % base)
                shift(rec)
            if op in LOADS and r1 in tainted and r1 != base:
                del tainted[r1]
            elif op in LOADS and r1 == base and base in tainted:
                del tainted[base]
            continue
        # $sp or derived register read as a source
        srcs = []
        if form == F_FRRR:
            srcs = [r2, rec.reg3]
        elif form == F_FRRI:
            srcs = [r2]
        elif form == F_FRR:
            srcs = [r2]
        carriers = [s for s in srcs if s == SP or s in tainted]
        if carriers:
            for s in carriers:
                if s != SP and not tainted[s]:
                    raise PassError("$sp-derived register $%d used after a label" % s)
            if form == F_FRRI and op in ADD_RRI and len(carriers) == 1:
                shift(rec)               # address of a local / element
                tainted.pop(r1, None)
            elif form == F_FRRR and op in ADD_RRR:
                tainted[r1] = True
            elif form == F_FRR and op == Z["zmove"]:
                tainted[r1] = True
            else:
                raise PassError("op %d reads $sp or a derived register" % op)
            if r1 == SP:
                raise PassError("unexpected write to $sp")
            continue
        if uses_sp and not (op in READ_OK):
            raise PassError("op %d uses $sp in an unsupported form" % op)
        # plain definition kills any taint on the destination
        if op not in NO_DEF and form not in (F_FI, F_FOA) and r1 in tainted:
            del tainted[r1]
        if op == Z["zjalr"] or op in NO_DEF:
            continue

    if n_alloc < 2:
        raise PassError("frame of %d bytes lacks matching allocate/release" % size)
    frame.w[2] = (size + FRAME_EXTRA) & 0xFFFFFFFF
    if mask is not None and not saves_move:
        mask.w[3] = (s32(mask.w[3]) - FRAME_EXTRA) & 0xFFFFFFFF
    return True


def transform(data):
    recs = parse(data)
    for start, end in procedures(recs):
        transform_proc(recs, start, end)
    return serialize(recs)


def run_as1(real, argv):
    """Rewrite the binasm input named in as1's argv and exec the real as1."""
    i, inp, valued = 0, None, ("-G", "-o", "-t", "-temp")
    while i < len(argv):
        if argv[i] in valued:
            i += 2
        elif argv[i].startswith("-"):
            i += 1
        else:
            if inp is not None:
                raise PassError("more than one input file in as1 arguments")
            inp = i
            i += 1
    if inp is None:
        raise PassError("no input file in as1 arguments")
    with open(argv[inp], "rb") as f:
        data = f.read()
    new = transform(data)
    fd, tmp = tempfile.mkstemp(prefix="framepass", suffix=".G")
    with os.fdopen(fd, "wb") as f:
        f.write(new)
    args = list(argv)
    args[inp] = tmp
    try:
        rc = os.spawnv(os.P_WAIT, real, [real] + args)
    finally:
        os.unlink(tmp)
    return rc


def main(argv):
    if len(argv) < 3 or argv[1] != "--as1":
        sys.exit(__doc__)
    try:
        sys.exit(run_as1(argv[2], argv[3:]))
    except PassError as e:
        sys.exit("frame_pass.py: %s" % e)


if __name__ == "__main__":
    main(sys.argv)
