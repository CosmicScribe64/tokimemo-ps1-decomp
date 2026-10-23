"""T-25604 scratch: compile a body in the whole unit with an as1 shim that saves ugen's binasm.
    python3 gc.py BODY.c OUT_STEM [--rev1]   -> OUT_STEM.o, OUT_STEM.G, OUT_STEM.T (in Docker, repo root)"""
import sys, os
from pathlib import Path
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, 'tools'); sys.path.insert(0, HERE)
import cc, funcloc, funcdiff_verify as proof, score2
sys.path.insert(0, os.path.join(HERE, "..", "fresh-r1")); import rev1
c = funcloc.locate(score2.NAME, "NAME_ENT")
stem = sys.argv[2]
text = score2.unit_text(c, Path(sys.argv[1]).read_text())
if '--rev1' in sys.argv: text = rev1.apply(text)
v = Path(stem + '.variant.c'); v.write_text(text)
pre = proof.command(["gcc", "-E", "-P", "-Iinclude", "-I" + os.path.dirname(c.src), "-D__sgi", "-DNON_MATCHING",
                     "-D__attribute__(x)=", "-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)",
                     "-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)", str(v)])
src = Path(stem + '.c'); src.write_text(pre)
cc.SHIMS['as1'] = os.path.join(HERE, 'as1save.py')
os.environ['T25604_G'] = stem + '.G'
cc.compile_ido(str(src), stem + '.o', '5.3')
