# scan.py G-files...: find loops whose preheader defines the limit (bne operand) before the iv start
import sys
sys.path.insert(0, "/work/tools")
import frame_pass as fp
names = {v: k[1:] for k, v in fp.Z.items()}
BR = {fp.Z[n] for n in ("zbne", "zbltu", "zblt", "zbgtu", "zbgt", "zbeq")}
for g in sys.argv[1:]:
    try: recs = fp.parse(open(g, "rb").read())
    except Exception as e: continue
    proc = None; labels = {}; ins = []
    for r, p in recs:
        if r.instr == 1 or r.instr == fp.I_IENT:  # isym/ient
            pass
        if r.instr == fp.I_IENT: proc = r.w[0]; ins = []; labels = {}
        if r.instr == fp.I_ILABEL: labels[r.w[0]] = len(ins); ins.append(("L", r.w[0], None))
        if r.instr == fp.I_IOCODE: ins.append(("I", r, None))
        if r.instr == fp.I_IEND and proc is not None:
            for k, (t, r2, _) in enumerate(ins):
                if t != "I" or r2.op not in BR: continue
                lab = r2.w[0]
                if lab not in labels or labels[lab] >= k: continue
                top = labels[lab]
                # preheader: records before top until previous label
                j = top - 1; pre = []
                while j >= 0 and ins[j][0] == "I": pre.insert(0, ins[j][1]); j -= 1
                defs = {}
                for q, x in enumerate(pre):
                    if names.get(x.op) in ("la", "addu", "move") or x.op == 41: defs[x.reg1] = (q, names.get(x.op, x.op), x.imm, x.w[0])
                a, b = r2.reg1, r2.reg2
                if a in defs and defs[a][1] == "la" and defs[a][0] == len(pre) - 1 and len(pre) > 2:
                    print(g.split("/")[-1], proc, "IVLAST", names.get(r2.op), defs[a][:3], " ".join("%s:%d" % (names.get(x.op, x.op), x.reg1) for x in pre[-8:]))
                if a not in defs and b in defs and defs[b][1] == "la":
                    print(g.split("/")[-1], proc, "NOSTART", a, defs[b], [ (names.get(x.op, x.op), x.reg1) for x in pre][-8:])
                if a in defs and b in defs and defs[b][1] == "la" and defs[a][0] > defs[b][0]:
                    print(g.split("/")[-1], proc, "ODD", defs[a], defs[b], [ (names.get(x.op, x.op), x.reg1) for x in pre][-12:])
                if a in defs and b in defs and defs[a][1] == "la" and defs[b][1] == "la":
                    order = "LIMIT-FIRST" if defs[b][0] < defs[a][0] else "start-first"
                    print(g.split("/")[-1], proc, names.get(r2.op), order, defs[a][:3], defs[b][:3], len(pre), " ".join("%s:%d" % (names.get(x.op, x.op), x.reg1) for x in pre[-10:]))
            proc = None
