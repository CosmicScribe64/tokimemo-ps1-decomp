#!/usr/bin/env python3
"""Unit-specific type rewrites of m2c drafts (T-20800): the ones the types-first passes wrote for one
overlay's shapes, ported from drafts/T-16560 .. T-18800/redraft.py. Each takes the `Env` of
tools/redraft_rewrites.py and the body, returns the body, and does nothing unless its pattern or its
type is there, so the list can run on every unit; the compiler decides whether a candidate is better.

  rect-view     (T-18800) the main-exe words D_800B5920[4] as two RECTs: `((RECT *)D_800B5920)[0].w`.
  menu-arrays   (T-17800) the four row arrays of a menu_set call as s16 arrays or one Arrs4x8.
  matrix-copy   (T-16560) eight word copies of a whole MATRIX from D_80129C40, D_80125C60, a coordinate
                object or a stack matrix: one struct copy.
  tcpos-args    (T-16560) calls that pass TcPos by value: m2c splits each into two words and loses the source.
  rect-local    (T-18000) `(RECT *) &sp2C` over four s16 locals is one RECT local.
  club-bits, record-copy, reroll  (T-18600) MASTER: the club bit-field word of GameState, 0x44-byte
                sprite record copies, and a constant loop over records IDO unrolled by four.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import migrate_globals as mg  # noqa: E402

ID = r"[A-Za-z_]\w*"

# ------------------------------------------------------------------------------------------ rect-view, rect-local

RECT_FIELDS = ("x", "y", "w", "h")


def rect_view(env, body):
    """(T-18800) the main-exe words D_800B5920[4] are two RECTs to DATE2 (every access is a halfword
    store); DATE's matched code writes `((RECT *)D_800B5920)[0].w`. m2c prints the splat labels."""
    def lab(m):
        k, r = divmod(int(m.group(1), 16) - 0x20, 8)
        return "((RECT *)D_800B5920)[%d].%s" % (k, RECT_FIELDS[r // 2]) if r % 2 == 0 else m.group(0)
    body = re.sub(r"\bD_800B59(2[2-9A-F])\b", lab, body)
    return re.sub(r"\*D_800B5920\b(?!\s*\[)", "((RECT *)D_800B5920)[0].x", body)


RECT_LOCALS = ((0, "x"), (2, "y"), (4, "w"), (6, "h"))


def rect_locals(env, body):
    """T-18000: `(RECT *) &sp2C` with s16 locals sp2C, sp2E, sp30, sp32 is one RECT local."""
    for base in sorted(set(re.findall(r"\(RECT \*\) ?&(sp[0-9A-F]+)\b", body))):
        n = int(base[2:], 16)
        names = {"sp%X" % (n + d): f for d, f in RECT_LOCALS}
        decl = re.compile(r"^[ \t]*(?:s16|u16|s32|\?) (sp[0-9A-F]+);[ \t]*\n", re.M)
        found = [d for d in decl.finditer(body) if d.group(1) in names]
        if base not in [d.group(1) for d in found]:
            continue
        for d in reversed(found):
            body = body[:d.start()] + body[d.end():]
        body = re.sub(r"\(RECT \*\) ?&%s\b" % base, "&@R@%s" % base, body)
        for name, f in names.items():
            body = re.sub(r"(?<![@\w])%s\b(?!\.)" % name, "%s.%s" % (base, f), body)
        body = body.replace("&@R@%s" % base, "&%s" % base)
        body = re.sub(r"(\)\s*\{\n)", r"\g<1>    RECT %s;\n" % base, body, count=1)
    return body


# ------------------------------------------------------------------------------------------ menu-arrays (T-17800)

MENU_RE = re.compile(r"\b(?:menu_set|func_8004F870)\(([^,()]+), ([^,()]+), ((?:&sp[0-9A-F]+(?:\[0\])?(?:, )?){4})\)")


def menu_arrays(env, body):
    """The four row arrays of a menu_set call as s16 arrays (or one Arrs4x8 filled by func_801325A0)."""
    # m2c already typed the local from func_801325A0's prototype: write the rows, not their first elements
    for sp in re.findall(r"^\s+Arrs4x8 (sp[0-9A-F]+);$", body, re.M):
        body = re.sub(r"&%s\.([abcd])\[0\]" % sp, r"%s.\1" % sp, body)
        body = re.sub(r"(\b(?:menu_set|func_8004F870)\([^,()]+, [^,()]+, )&%s, " % sp, r"\g<1>%s.a, " % sp, body)
        body = re.sub(r"(?:\(s16\) )?\b%s\b(?![.\[])(?=[,;)])" % sp, "%s.a[0]" % sp, body)
        body = body.replace("func_801325A0(&%s.a[0])" % sp, "func_801325A0(&%s)" % sp)
        body = body.replace("Arrs4x8 %s.a[0];" % sp, "Arrs4x8 %s;" % sp)
        # m2c reads row a as structs (`.a[i].unk0`) and the other rows as byte offsets from it
        body = re.sub(r"\b%s\.a\[([^\[\]]+)\]\.unk0\b" % sp, r"%s.a[\1]" % sp, body)
        body = re.sub(r"\*\(s16 \*\)\(\(unsigned char \*\)\(&%s\.a\[([^\[\]]+)\]\) \+ \((16|32|48)\)\)" % sp,
                      lambda k, sp=sp: "%s.%s[%s]" % (sp, "xbcd"[int(k.group(2)) // 16], k.group(1)), body)
    m = MENU_RE.search(body)
    if not m:
        return body
    names = re.findall(r"&(sp[0-9A-F]+)", m.group(3))
    offs = [int(n[2:], 16) for n in names]
    gaps = {offs[i] - offs[i + 1] for i in range(3)}
    if len(gaps) != 1:
        return body
    gap = gaps.pop()
    decl = re.compile(r"^\s+(?:\?|\w+) \*?(%s)(?:\[\d+\])*;\n" % "|".join(names), re.M)
    head = re.search(r"\)\s*\{\n", body)
    if head is None:
        return body
    fill = re.search(r"\bfunc_801325A0\((?:\([^()]*\) )?&%s(?:\[0\])?\)" % names[0], body)
    if gap == -0x10 and fill:
        sp = names[0]
        body = decl.sub("", body)
        head = re.search(r"\)\s*\{\n", body)
        body = body[:head.end()] + "    Arrs4x8 %s;\n" % sp + body[head.end():]
        body = re.sub(r"\bfunc_801325A0\((?:\([^()]*\) )?&%s(?:\[0\])?\)" % sp, lambda k: "func_801325A0(&%s)" % sp, body)
        body = MENU_RE.sub(lambda k: "func_8004F870(%s, %s, %s.a, %s.b, %s.c, %s.d)" % (
            k.group(1), k.group(2), sp, sp, sp, sp), body, count=1)
        return body
    if gap <= 0 or gap % 2:
        return body
    n = gap // 2
    body = decl.sub("", body)
    head = re.search(r"\)\s*\{\n", body)
    body = body[:head.end()] + "".join("    s16 %s[%d];\n" % (sp, n) for sp in names) + body[head.end():]
    for sp in names:
        body = re.sub(r"&%s\[0\]|&%s\b(?!\[)" % (sp, sp), sp, body)
        # m2c wrote the first element as the scalar itself
        body = re.sub(r"^(\s+)%s = " % sp, r"\g<1>%s[0] = " % sp, body, flags=re.M)
        body = re.sub(r"\((s16|s32)\) %s\b(?!\[)" % sp, r"%s[0]" % sp, body)
        # pointers that walk an array: m2c steps them in bytes, pointer-step turns that into elements
        for var in re.findall(r"^\s+(\w+) = %s;$" % sp, body, re.M):
            body = re.sub(r"^(\s+)(?:\?|\w+) \*%s;$" % var, r"\1s16 *%s;" % var, body, count=1, flags=re.M)
    return body


# ------------------------------------------------------------------------------------------ matrix-copy (T-16560)

MATRIX_GLOBALS = {"D_80129C40": "%s", "D_80125C60": "*(MATRIX *)%s"}
OBJ50_FIELDS = {"unk0": 0, "unk18": 0x18, "unk1C": 0x1C, "unk20": 0x20}


def word_ref(expr):
    """(base, offset) of a word access as m2c and the bulk cleanup print it, or None.
    base is a variable name, or '*' + pointer name for an access through a pointer."""
    e = expr.strip()
    while e.startswith("(s32) "):
        e = e[6:].strip()
    pats = (
        (r"^\(\*\(s32 \*\)&(%s)\)$" % ID, lambda m: (m.group(1), 0)),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)\(&(%s)\) \+ \((\d+)\)\)$" % ID, lambda m: (m.group(1), int(m.group(2)))),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)(%s) \+ \((\d+)\)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), int(m.group(2)))),
        (r"^\(\*\(s32 \*\)(%s)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), 0)),
        (r"^(%s)\.t\[(\d)\]$" % ID, lambda m: (m.group(1), 0x14 + 4 * int(m.group(2)))),
        (r"^(%s)\.unk0$" % ID, lambda m: (m.group(1), 0)),
        (r"^(%s)->(unk0|unk18|unk1C|unk20)$" % ID, lambda m: ("*" + m.group(1), OBJ50_FIELDS[m.group(2)])),
        (r"^(D_80129C40)$", lambda m: (m.group(1), 0)),
    )
    for pat, fn in pats:
        m = re.match(pat, e)
        if m:
            return fn(m)
    return None


def matrix_lvalue(base, off, body):
    if not base.startswith("*"):
        if base in MATRIX_GLOBALS and off == 0:
            return MATRIX_GLOBALS[base] % base
        return base if off == 0 else None
    ptr = base[1:]
    if off == 4 and re.search(r"^\s*TcObj50 \*%s;" % re.escape(ptr), body, re.M):
        return "*(MATRIX *)%s->unk4" % ptr
    return "*(MATRIX *)%s" % ptr if off == 0 else "*(MATRIX *)((u8 *)%s + %d)" % (ptr, off)


def matrix_copies(env, body):
    """Eight word copies that move a whole MATRIX (from D_80129C40, D_80125C60, a coordinate object
    or a stack matrix), in any order and interleaved with other statements: one struct copy."""
    lines = body.split("\n")
    groups = {}
    for i, l in enumerate(lines):
        m = re.match(r"^(\s*)(.+?) = (.+);\s*$", l)
        if not m:
            continue
        d, s_ = word_ref(m.group(2)), word_ref(m.group(3))
        if d is None or s_ is None or d[0] == s_[0]:
            continue
        key = (d[0], s_[0], d[1] - s_[1])
        groups.setdefault(key, []).append((i, s_[1]))
    retype = set()
    for (dbase, sbase, delta), items in groups.items():
        offs = sorted(o for _, o in items)
        if len(items) != 8 or len(set(offs)) != 8:
            continue
        start = min(offs)
        if offs != list(range(start, start + 32, 4)):
            continue
        dst = matrix_lvalue(dbase, start + delta, body)
        src = matrix_lvalue(sbase, start, body)
        if dst is None or src is None:
            continue
        first = min(i for i, _ in items)
        indent = re.match(r"^(\s*)", lines[first]).group(1)
        lines[first] = "%s%s = %s;" % (indent, dst, src)
        for i, _ in items:
            if i != first:
                lines[i] = None
        for b in (dbase, sbase):
            if not b.startswith("*") and b not in MATRIX_GLOBALS:
                retype.add(b)
    out = [l for l in lines if l is not None]
    text = "\n".join(out)
    for name in retype:
        text = re.sub(r"^(\s*)(?:s32|\?) %s(?:\[\d*\])?;" % re.escape(name), r"\1MATRIX %s;" % name, text,
                      flags=re.M)
    for g in MATRIX_GLOBALS:
        text = re.sub(r"\b%s\.unk0\b" % g, "(*(s32 *)&%s)" % g, text)
    return text


# ------------------------------------------------------------------------------------------ tcpos-args (T-16560)

TCPOS_CALLS = {"func_8015ACCC": (2, 4, 6),       # arg-word index of each TcPos parameter
               "func_8013D1E0": (0, 2)}          # no prototype: m2c prints subroutine_arg0..3
ASM_INS = re.compile(r"^\s*/\*[^*]*\*/\s+(\w+)\s+(.*?)\s*$")


def asm_lines(asm_text):
    out = []
    for line in asm_text.splitlines():
        m = ASM_INS.match(line)
        if m:
            out.append((m.group(1), [a.strip() for a in m.group(2).split(",")]))
        elif re.match(r"^\s*\.L\w+:", line):
            out.append(("label", []))
    return out


def tcpos_source(ins, j, home):
    """C expression for the TcPos copied to sp+home before the call at ins[j], or None."""
    reg = off = base = None
    for k in range(min(j + 1, len(ins) - 1), max(j - 80, 0), -1):     # the delay slot first
        op, a = ins[k]
        if k < j and (op in ("jal", "jalr") or (op == "label" and base is None)):
            return None
        if reg is None:
            if op == "swr" and len(a) == 2 and a[1] == "0x%X($sp)" % home:
                reg = a[0]
        elif base is None:
            if op == "lwr" and a[0] == reg:
                m = re.match(r"(-?0x[0-9A-Fa-f]+|\d+)?\((\$\w+)\)$", a[1])
                if not m:
                    return None
                off, base = int(m.group(1) or "0", 0), m.group(2)
        elif a and a[0] == base and op not in ("sw", "sh", "sb", "swl", "swr"):
            if op == "addiu" and len(a) == 3 and a[1] == "$sp":
                return "sp%X" % (int(a[2], 0) + off)
            m = re.match(r"%lo\((D_[0-9A-F]{8})\)", a[2]) if op == "addiu" and len(a) == 3 else None
            if m and off == 0:
                return m.group(1)
            if op == "lw" and a[1].startswith("%lo(D_8015EDB4)") and off in (0x74, 0x7C):
                return "*(TcPos *)&D_8015EDB4->unk%X" % off
            return ("reg", base[1:], off)
    return None


def reg_variable(body, reg):
    """The draft's variable for register `reg` when m2c named exactly one (temp_s0 or var_s0)."""
    names = [n for n in ("temp_" + reg, "var_" + reg) if re.search(r"\b%s\b" % n, body)]
    return names[0] if len(names) == 1 else None


def split_args(text, start):
    """(args, end) of the call whose '(' is at text[start]."""
    depth, args, cur, i = 0, [], start + 1, start
    while i < len(text):
        c = text[i]
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
            if depth == 0:
                args.append(text[cur:i])
                return args, i
        elif c == "," and depth == 1:
            args.append(text[cur:i])
            cur = i + 1
        i += 1
    return None, None


def tcpos_args(env, body, asm_text):
    """Calls passing TcPos by value: m2c splits each into two words (`/* TcPos+0x0 */ X, /* TcPos+0x4 */ Y`)
    and loses the source of the register words. The asm names each copy's source; stack locals at
    spN..spN+6 become one `TcPos spN`."""
    if not asm_text or "TcPos" not in env.types:
        return body
    ins = asm_lines(asm_text)
    locals_ = set()
    for func, words in TCPOS_CALLS.items():
        sites = [j for j, (op, a) in enumerate(ins) if op == "jal" and a and a[0] == func]
        calls = [m.start() for m in re.finditer(r"\b%s\(" % func, body)]
        if not sites or len(sites) != len(calls):
            continue
        new, last = [], 0
        for site, pos in zip(sites, calls):
            open_ = pos + len(func)
            args, end = split_args(body, open_)
            if args is None:
                return body
            srcs = [tcpos_source(ins, site, 4 * w) for w in words]
            for k, src in enumerate(srcs):
                if isinstance(src, tuple):
                    var = reg_variable(body, src[1])
                    srcs[k] = None if var is None else "*(TcPos *)((u8 *)%s + %d)" % (var, src[2])
            marks = [i for i, x in enumerate(args) if "TcPos+0x0" in x]
            if not marks and len(args) >= 2 * len(words) and all(
                    re.sub(r"/\*.*?\*/", "", args[k]).strip() == "subroutine_arg%d" % k for k in range(2 * len(words))):
                marks = [2 * k for k in range(len(words))]
            if None in srcs or len(marks) != len(words):
                new.append(body[last:end + 1])
                last = end + 1
                continue
            plain = [re.sub(r"/\*.*?\*/", "", x).strip() for x in args]
            out, i, n = [], 0, 0
            while i < len(plain):
                if i in marks:
                    s = srcs[n]
                    n += 1
                    if re.match(r"^sp[0-9A-F]+$", s):
                        locals_.add(int(s[2:], 16))
                        s = "@TCPOS%s@" % s
                    out.append(s)
                    i += 2
                else:
                    out.append(plain[i])
                    i += 1
            new.append(body[last:open_] + "(" + ", ".join(out) + ")")
            last = end + 1
        new.append(body[last:])
        body = "".join(new)
    decl = re.compile(r"^[ \t]*(s16|u16|s32|u32|\?) (sp[0-9A-F]+)(\[\d*\])?;[ \t]*\n", re.M)
    for n in sorted(locals_):
        names = {"sp%X" % (n + d): f for d, f in ((0, "x"), (2, "y"), (4, "z"), (6, "unk6"))}
        found = [m for m in decl.finditer(body) if m.group(2) in names]
        kinds = {m.group(2): m.group(1) for m in found}
        for m in reversed(found):
            body = body[:m.start()] + body[m.end():]
        body = re.sub(r"\{\n", "{\n    TcPos @TCPOSsp%X@;\n" % n, body, count=1)
        for name, field in names.items():
            if name not in kinds or kinds[name] not in ("s16", "u16"):
                continue
            body = re.sub(r"(?<!@TCPOS)\b%s\b(?!@)" % name, "sp%X.%s" % (n, field), body)
    return re.sub(r"@TCPOS(sp[0-9A-F]+)@", r"\1", body)


# ------------------------------------------------------------------------------------------ MASTER (T-18600)

CLUB_RE = re.compile(r"\(u32\) \(\(u32\) \(\(s32\) (D_800E6280\.unk_66C\[[^\[\]]+\]) << 5\) >> 0x1E\)")


def club_bits(env, body):
    """T-18600: field-symbols writes GameState word +0x684 as the GsRec66C record `D_800E6280.unk_66C[6]`, which
    C cannot shift; bits 25..26 are ScheduleClubFlags.unk_25 (MASTER's MasterClubBits.count until T-20900, as the
    matched func_801344E0 neighbours read them), any other word read is `*(s32 *)&...`."""
    if "ScheduleClubFlags" not in env.types:
        return body
    body = CLUB_RE.sub(lambda m: "((ScheduleClubFlags *)&%s)->unk_25" % m.group(1), body)
    return re.sub(r"\((?:s32|u32)\) (D_800E6280\.unk_66C\[[^\[\]]+\])(?![.\[])", lambda m: "*(s32 *)&" + m.group(1), body)


# record arrays a do-while walks: the C base expression, the record type, its size, and the absolute byte offset of
# record 0 (the GameState records are measured from D_800E6280, the sprites from D_80120650)
ROLL_BASES = {"sprite": ("((DisplayFixActor44 *)D_80120650)", "DisplayFixActor44", 0x44, 0),
              "girls": ("D_800E6280.unk_1BC", "Rec38", 0x38, 0x1BC)}
ROLL_START = (
    (r"&\(\(DisplayFixActor44 \*\)D_80120650\)\[(\d+)\]", "sprite", lambda m: int(m.group(1)) * 0x44),
    (r"\(?&\(\(DisplayFixActor44 \*\)D_80120650\)\[(\d+)\]\)?", "sprite", lambda m: int(m.group(1)) * 0x44),
    (r"&D_800E6280\.unk_([0-9A-F]{3})", "girls", lambda m: int(m.group(1), 16)),
    (r"&D_800E6280\.unk_1BC\[(\d+)\]\.unk_([0-9A-F]{2})", "girls",
     lambda m: 0x1BC + int(m.group(1)) * 0x38 + int(m.group(2), 16)),
)


def roll_fields(env, tname):
    out = {}
    for off, size, path, lt in mg.walk_layout(env.types[tname]):
        if lt.kind in ("scalar", "ptr"):
            out.setdefault(path, (off, size))
    return out


def roll_addr(expr):
    for pat, kind, fn in ROLL_START:
        m = re.fullmatch(pat, expr.strip())
        if m:
            return kind, fn(m)
    return None


def roll_access(expr, var, fields, size):
    """byte offset of `expr` relative to the walking pointer `var`, or None."""
    e = expr.strip()
    m = re.fullmatch(r"\*\((?:u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)%s \+ \((-?\d+)\)\)" % var, e)
    if m:
        return int(m.group(1))
    m = re.fullmatch(r"%s\[(-?\d+)\](\.\w+)" % var, e) or re.fullmatch(r"%s->()(\w+)" % var, e)
    if m:
        k = int(m.group(1)) if m.group(1) else 0
        f = m.group(2) if m.group(2).startswith(".") else "." + m.group(2)
        if f in fields:
            return k * size + fields[f][0]
    return None


def roll_norm(val, width):
    """(op, value) of a field update with the constant reduced to the field width (`& 0xFF7F` is `&= ~0x80`)."""
    op, c = val
    c = c.strip()
    try:
        n = ~int(c[1:], 0) if c.startswith("~") else int(c, 0)
    except ValueError:
        return val
    return op, n & ((1 << (8 * width)) - 1)


def reroll(env, body):
    """T-18600: IDO unrolls a constant loop over records by four; m2c prints the copy (a pointer walk with
    temporaries and byte offsets). Rewrite `p = &REC[a]; do { ...; p += 4; ... } while (p != &REC[b]);` as
    `for (i = a; i < b; i++) { REC[i].f op= c; }` when every record of an iteration gets the same field updates;
    for the sprite records also fold the peeled records just before the walk into the loop."""
    if "do {" not in body:
        return body
    lines = body.split("\n")
    out, i, used = [], 0, False
    while i < len(lines):
        m = re.match(r"^(\s+)(\w+) = (.+);$", lines[i])
        if not (m and i + 1 < len(lines) and lines[i + 1] == m.group(1) + "do {"):
            out.append(lines[i])
            i += 1
            continue
        ind, var, start = m.groups()
        got = roll_addr(start)
        end_i = None
        for j in range(i + 2, len(lines)):
            if lines[j].startswith(ind + "} while ("):
                end_i = j
                break
            if lines[j].startswith(ind) and not lines[j].startswith(ind + "    "):
                break
        if got is None or end_i is None or ROLL_BASES[got[0]][1] not in env.types:
            out.append(lines[i])
            i += 1
            continue
        kind, start_abs = got
        base, tname, size, rec0 = ROLL_BASES[kind]
        fields = roll_fields(env, tname)
        names = {}
        for k, v in fields.items():
            names.setdefault(v[0], k)
        wm = re.fullmatch(r"\} while \(%s (?:!=|<) \(?(.+?)\)?\);" % var,
                          lines[end_i].strip().replace("(u32) ", "")) if end_i is not None else None
        end = roll_addr(wm.group(1)) if wm else None
        ok = end is not None and end[0] == kind
        temps, delta, ops, step = {}, 0, {}, None
        for line in lines[i + 2:end_i] if ok else []:
            t = line.strip()
            sm = re.fullmatch(r"%s ([-+])= (0x[0-9A-Fa-f]+|\d+);" % var, t)
            if sm:
                n = int(sm.group(2), 0) * (-1 if sm.group(1) == "-" else 1)
                n *= 1 if kind == "girls" else size
                delta += n
                step = n if step is None else step
                continue
            am = re.fullmatch(r"(.+?) ([-+|&^]?)= (.+);", t)
            if not am:
                ok = False
                break
            lhs, op, rhs = am.groups()
            if re.fullmatch(r"\w+", lhs) and not op:
                off = roll_access(rhs, var, fields, size)
                if off is None:
                    ok = False
                    break
                temps[lhs] = start_abs + delta + off
                continue
            off = roll_access(lhs, var, fields, size)
            if off is None:
                ok = False
                break
            dst = start_abs + delta + off
            rhs = re.sub(r"^\((?:u8|s8|u16|s16|s32|u32)\) ", "", rhs).strip()
            if rhs.startswith("(") and rhs.endswith(")"):
                rhs = rhs[1:-1]
            if op:
                val = (op, rhs)
            else:
                vm = re.fullmatch(r"(.+?) ([-+|&^]) (~?(?:0x[0-9A-Fa-f]+|\d+))", rhs)
                if vm and (temps.get(vm.group(1)) == dst or roll_access(vm.group(1), var, fields, size) is not None
                           and start_abs + delta + roll_access(vm.group(1), var, fields, size) == dst):
                    val = (vm.group(2), vm.group(3))
                elif re.fullmatch(r"-?(?:0x[0-9A-Fa-f]+|\d+)U?", rhs):
                    val = ("", rhs.rstrip("U"))
                else:
                    ok = False
                    break
            rel = dst - rec0
            ops.setdefault(rel // size, {})[rel % size] = val
        recs = sorted(ops)
        if ok and recs and step and step % size == 0:
            per = step // size
            first = recs[0]
            ok = (len(recs) == per and recs == list(range(first, first + per))
                  and all(ops[r] == ops[first] for r in recs) and (end[1] - start_abs) % step == 0)
        else:
            ok = False
        if not ok:
            out.append(lines[i])
            i += 1
            continue
        last = first + (end[1] - start_abs) // step * per
        fops = ops[first]
        # the peeled records written just before the walk (sprites only)
        if kind == "sprite":
            while first > 0:
                k = first - 1
                lvs = {}
                for off, (op, c) in fops.items():
                    f = names.get(off, "")
                    lvs[("%s[%d]%s" % (base, k, f)) if k else ("%s->%s" % (base, f[1:]))] = (off, op, c)
                n = len(lvs)
                prev = [x.strip() for x in out[-n:]] if len(out) >= n else []
                seen = set()
                for x in prev:
                    pm = re.fullmatch(r"(.+?) ([-+|&^]?)= (.+);", x)
                    if not pm or pm.group(1) not in lvs:
                        break
                    off, op, c = lvs[pm.group(1)]
                    width = fields[names[off]][1]
                    rhs = re.sub(r"^\((?:u8|s8|u16|s16|s32|u32)\) ", "", pm.group(3)).strip()
                    if rhs.startswith("(") and rhs.endswith(")"):
                        rhs = rhs[1:-1]
                    if pm.group(2):
                        got = (pm.group(2), rhs)
                    else:
                        vm = re.fullmatch(r"(.+?) ([-+|&^]) (~?(?:0x[0-9A-Fa-f]+|\d+))", rhs)
                        got = (vm.group(2), vm.group(3)) if vm and vm.group(1) == pm.group(1) else ("", rhs.rstrip("U"))
                    if roll_norm(got, width) != roll_norm((op, c), width):
                        break
                    seen.add(pm.group(1))
                if len(seen) != n:
                    break
                del out[-n:]
                first = k
        stmts = []
        for off, (op, c) in sorted(fops.items(), key=lambda kv: kv[0]):
            f = names.get(off)
            if f is None:
                stmts = None
                break
            stmts.append("%s    %s[i]%s %s= %s;" % (ind, base, f, op, c) if op else
                         "%s    %s[i]%s = %s;" % (ind, base, f, c))
        if stmts is None:
            out.append(lines[i])
            i += 1
            continue
        out.append("%sfor (i = %d; i < %d; i++) {" % (ind, first, last))
        out.extend(stmts)
        out.append(ind + "}")
        used = True
        i = end_i + 1
    body = "\n".join(out)
    if used and not re.search(r"^\s+s32 i;$", body, re.M):
        body = re.sub(r"^(.*\)\s*\{\n)", r"\g<1>    s32 i;\n", body, count=1)
    # drop declarations of locals the rewrite left unused
    for name in re.findall(r"^\s+[\w ]+?\*?\s*(\w+);(?:\s*/\*.*\*/)?$", body, re.M):
        if len(re.findall(r"\b%s\b" % name, body)) == 1:
            body = re.sub(r"^\s+[\w ]+?\*?\s*%s;(?:\s*/\*.*\*/)?\n" % name, "", body, count=1, flags=re.M)
    return body


SPRITE_COPY_RE = re.compile(r"\*\(M2cBlock44 \*\)\((.+?)\) = \*\(M2cBlock44 \*\)\((.+?)\);")


def record_copies(env, body):
    """T-18600: m2c's 0x44-byte block copy between sprite records is one DisplayFixActor44 assignment."""
    if "DisplayFixActor44" not in env.types:
        return body

    def side(e):
        e = e.strip()
        m = re.fullmatch(r"&(.+)", e)
        return m.group(1) if m else "*(DisplayFixActor44 *)%s" % e if re.fullmatch(r"\w+", e) else None

    def repl(m):
        a, b = side(m.group(1)), side(m.group(2))
        return "%s = %s;" % (a, b) if a and b else m.group(0)
    new = SPRITE_COPY_RE.sub(repl, body)
    if "{" in new and "M2cBlock44" not in new.split("{", 1)[1].replace("typedef struct { s32 w[17]; } M2cBlock44;", ""):
        new = re.sub(r"^\s+typedef struct \{ s32 w\[17\]; \} M2cBlock44;.*\n", "", new, flags=re.M)
    return new
