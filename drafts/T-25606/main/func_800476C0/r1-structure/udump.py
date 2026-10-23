#!/usr/bin/env python3
"""usage (docker, repo root): python3 udump.py BODY.c OUT.txt
T-25606 r1-structure experiment: compiles with BODY.c for func_800476C0 through the normal pipeline and
writes ugen's input ucode (uopt output, after the cvt pass) as text: opcode, dtype, mtype, words."""
import sys, os, subprocess, tempfile, shutil
from pathlib import Path
sys.path.insert(0, "/work/tools")
if len(sys.argv) > 1 and sys.argv[1] == "--ugen":
    real = sys.argv[2]; args = sys.argv[3:]
    i = 0
    while i < len(args):
        if args[i] == "-o": i += 2; continue
        if args[i].startswith("-"):
            i += 2 if args[i] in ("-G", "-t", "-temp") else 1; continue
        if args[i].endswith(".B") or args[i].endswith(".O") or os.path.exists(args[i]):
            shutil.copy(args[i], os.environ["UDUMP_OUT"]); break
        i += 1
    os.execvp("python3", ["python3", "/work/tools/ugen_pass.py", "--ugen", real] + args)
import funcdiff as fd, permute, cc, ugen_pass as up
NAME = "func_800476C0"
body_file, out = sys.argv[1:3]
c = fd.locate(NAME, "main")
text = Path(c.src).read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
if inc in text:
    a = text.index(inc)
    text = text[:a] + "u8 func_8004901C(void);\n" + Path(body_file).read_text().rstrip("\n") + "\n" + text[a + len(inc):]
else:
    a = text.index("#ifdef NON_MATCHING\nu8 func_8004901C(void);\nvoid func_800476C0")
    b = text.index(inc + "#endif\n", a) + len(inc) + len("#endif\n")
    text = text[:a] + "u8 func_8004901C(void);\n" + Path(body_file).read_text().rstrip("\n") + "\n" + text[b:]
d = Path(tempfile.mkdtemp(dir="/tmp"))
(d / "cand.c").write_text(text)
(d / "base.c").write_text(permute.build_base(".", str(d / "cand.c"), NAME))
tmp = tempfile.mkdtemp(prefix="idolib")
os.environ["UDUMP_OUT"] = str(d / "u.B")
env = cc.ido_frame_env("5.3", tmp, {"ugen": os.path.abspath(__file__)})
flags = ["-I" + os.path.abspath("include") if f == "-Iinclude" else f for f in cc.IDO_CFLAGS]
r = subprocess.run(["/opt/ido/5.3/cc"] + flags + ["-o", str(d / "base.o"), "base.c"], cwd=str(d), env=env,
                   capture_output=True, text=True)
if r.returncode:
    sys.exit(r.stdout + r.stderr)
ins = up.parse_ucode(open(d / "u.B", "rb").read())
lines = []
for x in ins:
    lines.append("%-5s dt%-2d mt%d %s" % (up.OPS[x.opc], x.dtype, x.mtype, " ".join("%x" % w for w in x.words[1:])))
Path(out).write_text("\n".join(lines) + "\n")
print(len(lines))
