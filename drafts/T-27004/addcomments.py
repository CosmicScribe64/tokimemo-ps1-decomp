"""Add the one-line English comments of core.tsv `comment` rows above the C definitions.
usage: addcomments.py ROOT comments.tsv   (prints what it did; writes the files)"""
import re, sys, glob, os
root, table = sys.argv[1], sys.argv[2]
rows = []
for line in open(table, encoding="latin-1"):
    p = line.rstrip("\n").split("\t")
    if len(p) >= 4 and p[0] == "comment":
        rows.append((p[2], p[3], p[1]))
files = sorted(glob.glob(os.path.join(root, "src/main/*.c")))
texts = {f: open(f, encoding="latin-1").read() for f in files}
done, missing, multi = [], [], []
edits = {}
for name, text, scope in rows:
    if scope != "main":
        missing.append((name, "scope " + scope))
        continue
    rx = re.compile(r"^(?!\s)(?:[A-Za-z_][\w\s\*]*?[\s\*])%s\s*\([^;{()]*\)[ \t]*(?:\{|$)" % re.escape(name), re.M)
    hits = []
    for f, t in texts.items():
        for m in rx.finditer(t):
            hits.append((f, m.start()))
    hits = [h for h in hits if ("/* %s */\n" % text.strip().rstrip(".")) != texts[h[0]][max(0, h[1] - len(text.strip().rstrip(".")) - 9):h[1]][-(len(text.strip().rstrip(".")) + 9):]]
    if len(hits) == 1:
        edits.setdefault(hits[0][0], []).append((hits[0][1], name, text))
        done.append(name)
    elif not hits:
        missing.append((name, "no C definition"))
    else:
        multi.append((name, hits))
for f, es in edits.items():
    t = texts[f]
    for pos, name, text in sorted(es, reverse=True):
        c = text.strip().rstrip(".")
        assert "*/" not in c and c.isascii(), name
        t = t[:pos] + "/* %s */\n" % c + t[pos:]
    open(f, "w", encoding="latin-1", newline="").write(t)
print("commented %d, missing %d, multi %d" % (len(done), len(missing), len(multi)))
for m in missing: print("MISSING", *m)
for m in multi: print("MULTI", m[0], [os.path.basename(h[0]) for h in m[1]])
