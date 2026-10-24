import sys, os, re, glob, json
sys.path.insert(0, "/work/tools")
import ugen_pass as up
OP = up.OP
res = []
for cwd in sorted(glob.glob("/work/build/ugen_trace/*.cwd")):
    src = open(cwd).read().strip()
    if not src: continue
    insns = up.parse_ucode(open(cwd[:-4] + ".O", "rb").read())
    lines = open("/work/" + src, encoding="utf-8", errors="replace").read().split("\n")
    for s, e in up._procedures(insns):
        if not any(insns[k].opc == OP["mov"] for k in range(s, e)): continue
        name = None
        for k in range(s, min(s + 4, e)):
            if insns[k].opc == OP["comm"]:
                name = insns[k].extra.split(b"\0")[0].decode(); break
        state, cur_line, events, movs = "B", 0, [("entry", 0)], []
        detail = []; last_istr = None; last_cmp = None
        for k in range(s + 1, e):
            opc = insns[k].opc
            if opc == OP["loc"]: cur_line = insns[k].words[1]
            if opc in (OP["istr"], OP["istv"]):
                state = "F"; events.append(("istr", cur_line)); last_istr = cur_line
            elif opc in (OP["fjp"], OP["tjp"]) and up._is_register_compare_jump(insns, s, k):
                if state != "B": events.append(("cmp", cur_line))
                state = "B"; last_cmp = cur_line
            elif opc == OP["mov"]:
                movs.append((state, cur_line))
                detail.append((state, cur_line, last_istr, last_cmp))
        # function body in source
        a = None
        for i, l in enumerate(lines):
            if re.match(r"^[A-Za-z_].*\b%s\(.*\{\s*$" % re.escape(name), l) or re.match(r"^[A-Za-z_].*\b%s\(" % re.escape(name), l) and "{" in l:
                a = i; break
        fakes = []
        if a is not None:
            b = a
            while b < len(lines) and lines[b] != "}": b += 1
            fakes = [i + 1 for i in range(a, b) if "FAKE" in lines[i]]
            rng = (a + 1, b + 1)
        else:
            rng = None
        res.append(dict(detail=detail, src=src, name=name, movs=movs, events=events, fakes=fakes, rng=rng))
json.dump(res, open("/work/build/census.json", "w"))
print(len(res), "procedures with Umov")
