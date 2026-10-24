#!/usr/bin/env python3
"""Count what is still number-named in the decompilation (T-25900 survey, read-only).

Per unit (main exe game code, the SDK region, each overlay) it prints:
  funcs      functions (splat .s files of the unit's C files), placeholders func_XXXXXXXX
  data       data symbols splat defines in the unit (dlabel), placeholders D_XXXXXXXX / jtbl_
  c_D        distinct D_XXXXXXXX names the unit's C files use
  fields     struct/union members in the unit's header (main: game.h + main_api.h), unk_XX ones
  types      typedef names in the unit's header and C files; address- or shape-named ones
  locals     m2c-style local names in the unit's C (var_*, temp_*, spXX, argN, phi_*)
  hex        hex literals >= 0x10 in function bodies of the unit's C
  files      C files (all named by address today)

Needs a finished build (asm/). Run from the repo root:
  tools/docker.sh python3 drafts/T-25900/count_names.py [--tsv]
"""
import os
import re
import sys

sys.path.insert(0, "tools")
import srcscan  # noqa: E402

PH_FUNC = re.compile(r"^func_[0-9A-F]{8}$")
PH_DATA = re.compile(r"^(D_|jtbl_)[0-9A-F]{8}$")
GLABEL = re.compile(r"^\s*glabel\s+(\w+)", re.M)
DLABEL = re.compile(r"^\s*(?:dlabel|jlabel)\s+(\w+)", re.M)
D_NAME = re.compile(r"\bD_[0-9A-F]{8}\b")
COMMENT = re.compile(r"/\*.*?\*/", re.S)
STRING = re.compile(r'"(?:\\.|[^"\\\n])*"')
TYPEDEF_END = re.compile(r"^\s*}\s*([A-Za-z_]\w*)\s*;", re.M)
MEMBER = re.compile(r"^\s*(?:/\*[^*]*\*/\s*)?(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+|union\s+)*"
                    r"[A-Za-z_]\w*[\s*]+([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?::\s*\d+\s*)?;")
LOCAL = re.compile(r"\b(?:var_[a-z0-9_]+|temp_[a-z0-9_]+|phi_[a-z0-9_]+|saved_reg_\w+|sp[0-9A-F]{1,4}|arg[0-9])\b")
HEX = re.compile(r"\b0x([0-9A-Fa-f]+)\b")
SHAPE = re.compile(r"(?:[0-9A-F]{3,}|[0-9]+x[0-9]+|[0-9]+$|^Rec[0-9A-F]|^Unk|^Gs(?:Rec|Word|Half))")


def read(p):
    with open(p, errors="replace") as f:
        return f.read()


def s_files(dirs):
    out = []
    for d in dirs:
        if os.path.isdir(d):
            for dp, _dn, fs in os.walk(d):
                out += [f[:-2] for f in fs if f.endswith(".s")]
    return out


def struct_members(text):
    """Member names inside struct/union bodies (brace depth > 0 after struct/union)."""
    text = STRING.sub('""', COMMENT.sub(lambda m: m.group() if re.match(r"/\*\s*0x", m.group()) else " ", text))
    names, depth, in_agg = [], 0, []
    for line in text.splitlines():
        opens, closes = line.count("{"), line.count("}")
        if re.search(r"\b(struct|union)\b[^;(]*\{", line):
            in_agg.append(depth + 1)
        elif opens and in_agg and depth >= in_agg[-1]:
            pass
        if in_agg and depth >= in_agg[-1] and not opens:
            m = MEMBER.match(line)
            if m:
                names.append(m.group(1))
        depth += opens - closes
        while in_agg and depth < in_agg[-1]:
            in_agg.pop()
    return names


def bodies(text):
    """Text inside top-level function bodies (depth >= 1 after a ')' line), comments and strings removed."""
    text = STRING.sub('""', COMMENT.sub(" ", text))
    out, depth, start = [], 0, None
    for i, ch in enumerate(text):
        if ch == "{":
            if depth == 0:
                head = text[max(0, i - 200):i]
                start = i if re.search(r"\)\s*$", head) else None
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0 and start is not None:
                out.append(text[start:i])
                start = None
    return "".join(out)


def unit_row(unit):
    cfs = srcscan.unit_c_files(unit)
    funcs = s_files([str(c.nonmatchings) for c in cfs] + [str(c.matchings) for c in cfs])
    if unit == "main":
        data_dirs, headers = ["asm/data/main"], ["include/game.h", "include/main_api.h"]
        data_text = ""
        for dp, _dn, fs in os.walk("asm/data"):
            if dp.startswith("asm/data/main") or dp == "asm/data":
                for f in fs:
                    if f.endswith(".s") and not f.startswith("lib"):
                        data_text += read(os.path.join(dp, f))
    else:
        headers = ["include/ovl/%s.h" % unit]
        data_text = ""
        d = "asm/ovl/%s/data" % unit
        for dp, _dn, fs in os.walk(d):
            for f in fs:
                if f.endswith(".s"):
                    data_text += read(os.path.join(dp, f))
    data = set(DLABEL.findall(data_text))
    ctext = "".join(read(str(c.src)) for c in cfs if os.path.exists(str(c.src)))
    htext = "".join(read(h) for h in headers if os.path.exists(h))
    members = struct_members(htext) + struct_members(ctext)
    types = set(TYPEDEF_END.findall(COMMENT.sub(" ", htext + ctext)))
    body = bodies(ctext)
    hexes = [h for h in HEX.findall(body) if int(h, 16) >= 0x10]
    return {
        "unit": unit,
        "funcs": len(funcs), "funcs_ph": sum(1 for f in funcs if PH_FUNC.match(f)),
        "data": len(data), "data_ph": sum(1 for d in data if PH_DATA.match(d)),
        "c_D": len(set(D_NAME.findall(COMMENT.sub(" ", ctext)))),
        "fields": len(members), "fields_unk": sum(1 for m in members if m.startswith(("unk", "pad", "field_"))),
        "types": len(types), "types_shape": sum(1 for t in types if SHAPE.search(t)),
        "locals": len(set(LOCAL.findall(body))), "locals_occ": len(LOCAL.findall(body)),
        "hex": len(hexes),
        "files": len(cfs),
    }


def sdk_row():
    names = []
    for f in sorted(os.listdir("asm")):
        if f.endswith(".s"):
            names += GLABEL.findall(read(os.path.join("asm", f)))
    return {"unit": "SDK", "funcs": len(names), "funcs_ph": sum(1 for n in names if PH_FUNC.match(n))}


COLS = ["funcs", "funcs_ph", "data", "data_ph", "c_D", "fields", "fields_unk", "types", "types_shape",
        "locals", "locals_occ", "hex", "files"]


def main(argv):
    rows = [unit_row("main")] + [unit_row(u) for u in srcscan.overlay_names()]
    tot = {"unit": "TOTAL"}
    for c in COLS:
        tot[c] = sum(r.get(c, 0) for r in rows)
    rows.append(tot)
    sdk = sdk_row()
    sep = "\t" if "--tsv" in argv else " "
    print(sep.join(["unit"] + COLS))
    for r in rows:
        print(sep.join([r["unit"]] + [str(r.get(c, "")) for c in COLS]))
    print("SDK asm functions %d, placeholders %d" % (sdk["funcs"], sdk["funcs_ph"]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
