import sys, re, glob, os, collections, json
sys.path.insert(0, "tools")
import symtab
t = symtab.SymbolTable(".")
byaddr = collections.defaultdict(list)
for l in t.owned_lines("main"):
    if symtab.placeholder_addr(l.name) is None:
        byaddr[l.addr].append(l)
PH = re.compile(r"\b(func|D|jtbl)_([0-9A-F]{8})\b")
files = [p for r in ("src","include") for p in glob.glob(r+"/**/*", recursive=True) if os.path.isfile(p) and p.endswith((".c",".h"))]
uses = collections.Counter(); filesby = collections.defaultdict(set)
for p in files:
    txt = open(p, encoding="latin-1").read()
    for m in PH.finditer(txt):
        a = int(m.group(2), 16)
        if a in byaddr:
            k=(m.group(0), a)
            uses[k]+=1; filesby[k].add(p)
print(len(uses), "placeholders;", sum(uses.values()), "uses;", len(set(f for s in filesby.values() for f in s)), "files")
rows=[]
for (ph,a),n in sorted(uses.items(), key=lambda kv: kv[0][1]):
    rows.append((ph,a,n,sorted(filesby[(ph,a)]),[(l.name,l.path.split('/')[-1],l.comment) for l in byaddr[a]]))
json.dump(rows, open(sys.argv[1],"w"))
cc=collections.Counter(); cu=collections.Counter()
for r in rows:
    k=tuple(sorted(set(x[1] for x in r[4]))); cc[k]+=1; cu[k]+=r[2]
print(cc); print(cu)
