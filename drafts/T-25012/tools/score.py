# usage: score.py UNIT:func real_src body1.c [body2.c ...]  (bodies replace the INCLUDE_ASM line, guarded; prints diff-line counts)
import sys, re, tempfile, difflib, shutil
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff, funcdiff_verify as proof
unit, name = sys.argv[1].split(":")
real = Path(sys.argv[2])
c = funcdiff.locate(name, unit)
e, _ = funcdiff.expected_object(c, False, stale=[])
want = funcdiff.functions(e, [name])[name]
def norm(l): return re.sub(r"<[^>]*>", "<>", re.sub(r"R_MIPS_\w+ \S+", "", l))
w = [norm(x) for x in want if not x.startswith("R_MIPS")]
for body in sys.argv[3:]:
    if body.endswith(".unit"):
        t = Path(body).read_text()
    else:
      t = real.read_text()
      m = re.search(r'INCLUDE_ASM\("[^"]*", %s\);\n' % name, t)
      t = t[:m.start()] + "#ifdef NON_MATCHING\n" + Path(body).read_text() + "\n#else\n" + m.group(0) + "#endif\n" + t[m.end():]
    d = Path(tempfile.mkdtemp(dir="build/scratch/t25012"))
    s = d / "src.c"
    s.write_text(t)
    try:
        obj, _ = proof.compile_unit(c, d, source=str(s))
    except Exception as ex:
        print(body, "ERR", str(ex)[-200:]); continue
    got = funcdiff.functions(obj, [name])[name]
    g = [norm(x) for x in got if not x.startswith("R_MIPS")]
    import os
    P=int(os.environ.get("PREFIX","0")) or None
    sm = difflib.SequenceMatcher(None, w[:P], g[:P], autojunk=False)
    n = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal")
    regs=r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-7]|k[01]|gp|sp|fp|ra)\b"
    def st(l): return re.sub(r"-?\d+\(","#(",re.sub(regs,"r",re.sub(r"^[0-9a-f]{8} ","",l)))
    sm2 = difflib.SequenceMatcher(None, [st(x) for x in w[:P]], [st(x) for x in g[:P]], autojunk=False)
    n2 = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm2.get_opcodes() if tag != "equal")
    print(body, "struct", n2, end=" ")
    print("want", len(w), "got", len(g), "diff", n, flush=True)
    shutil.rmtree(d, ignore_errors=True)
