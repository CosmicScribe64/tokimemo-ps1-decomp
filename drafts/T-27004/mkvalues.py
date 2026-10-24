"""Write one values file per const group of drafts/T-27004/consts.tsv (NAME VALUE [comment])."""
import re, sys, os
src, outdir = sys.argv[1], sys.argv[2]
groups = {}
for line in open(src, encoding="latin-1"):
    p = line.rstrip("\n").split("\t")
    if len(p) < 4 or p[0] != "const":
        continue
    groups.setdefault(p[1], []).append((p[3], p[2], p[4] if len(p) > 4 else ""))

def comment(note):
    hyp = note.upper().startswith("HYPOTHESIS")
    c = note
    if hyp:
        c = re.sub(r"^HYPOTHESIS:?\s*", "", c, flags=re.I)
        # drop a parenthesised lead like "name (reading ...)" and keep a short clause
    c = c.split(";")[0].strip()
    c = re.sub(r"\*/", "", c)
    if re.search(r"(func|D|jtbl)_[0-9A-Fa-f]{8}", c) or "'" in c:
        c = ""
    if len(c) > 60:
        c = ""
    c = c.encode("ascii", "ignore").decode()
    if hyp:
        return "HYPOTHESIS: " + c if c else "HYPOTHESIS"
    return c

for g, rows in groups.items():
    cs = [comment(n) for _a, _b, n in rows]
    dup = {c for c in cs if cs.count(c) > 1 and not c.startswith("HYPOTHESIS")}
    with open(os.path.join(outdir, "values_%s.txt" % g), "w") as f:
        for name, val, note in rows:
            m = re.match(r"bit (\d+)$", val)
            if m:
                val = "0x%08X" % (1 << int(m.group(1)))
            c = comment(note)
            if c in dup or re.search(r"\bentry \d+$", c):
                c = ""
            f.write(("%s %s %s\n" % (name, val, c)) if c else ("%s %s\n" % (name, val)))
print({g: len(r) for g, r in groups.items()})
