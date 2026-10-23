#!/usr/bin/env python3
"""usage (docker, repo root): python3 gdump.py BODY.c OUT.txt
T-25606 r1-structure experiment: compiles src/main/80047550.c with BODY.c for func_800476C0 through the
normal pipeline and dumps ugen's binasm records (the as1 input) of func_800476C0 as text: op, regs, imm.
The shim copies the .G file and then runs the normal frame-pass as1 shim; nothing in the build changes."""
import sys, os, subprocess, tempfile, shutil
from pathlib import Path
sys.path.insert(0, "tools")
if len(sys.argv) > 1 and sys.argv[1] == "--as1":
    real = sys.argv[2]; args = sys.argv[3:]
    i = 0
    while i < len(args):
        if args[i] == "-t":
            shutil.copy(args[i + 1], os.environ["GDUMP_OUT"] + ".T")
            i += 2
        elif args[i] in ("-G", "-o", "-temp"):
            i += 2
        elif args[i].startswith("-"):
            i += 1
        else:
            shutil.copy(args[i], os.environ["GDUMP_OUT"])
            open(os.environ["GDUMP_OUT"] + ".args", "w").write(repr(args) + "\n" + repr(args[i]))
            i += 1
    os.execvp("python3", ["python3", "/work/tools/frame_pass.py", "--as1", real] + args)
import funcdiff as fd, permute, cc, frame_pass as fp
NAME = "func_800476C0"
body_file, out = sys.argv[1:3]
c = fd.locate(NAME, "main")
text = Path(c.src).read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
a = text.index(inc)
text = text[:a] + "u8 func_8004901C(void);\n" + Path(body_file).read_text().rstrip("\n") + "\n" + text[a + len(inc):]
d = Path(tempfile.mkdtemp(dir="/tmp"))
(d / "cand.c").write_text(text)
(d / "base.c").write_text(permute.build_base(".", str(d / "cand.c"), NAME))
tmp = tempfile.mkdtemp(prefix="idolib")
os.environ["GDUMP_OUT"] = str(d / "u.G")
env = cc.ido_frame_env("5.3", tmp, {"as1": os.path.abspath(__file__)})
flags = ["-I" + os.path.abspath("include") if f == "-Iinclude" else f for f in cc.IDO_CFLAGS]
r = subprocess.run(["/opt/ido/5.3/cc"] + flags + ["-o", str(d / "base.o"), "base.c"], cwd=str(d), env=env,
                   capture_output=True, text=True)
if r.returncode:
    sys.exit(r.stdout + r.stderr)
data = open(d / "u.G", "rb").read()
shutil.copy(str(d / "u.G"), out + ".G")
shutil.copy(str(d / "u.G.args"), out + ".args")
shutil.copy(str(d / "u.G.T"), out + ".T")
print("G bytes", len(data))
recs = fp.parse(data)
import collections; print(collections.Counter(r.instr for r, _ in recs).most_common(8))
names = {v: k[1:] for k, v in fp.Z.items()}
rn = lambda r: "-" if r == fp.REG_NONE else "$%d" % r
lines, on = [], False
for rec, pay in recs:
    if rec.instr == fp.I_IENT: on = True
    if on:
        if rec.instr == fp.I_IOCODE:
            lines.append("%-6s %s %s %s %d" % (names.get(rec.op, "op%d" % rec.op), rn(rec.reg1), rn(rec.reg2), rn(rec.reg3), rec.imm))
        elif rec.instr in (fp.I_ILABEL, fp.I_ILAB):
            lines.append("L%d:" % rec.w[0])
        else:
            lines.append(".i%d %d %d %d" % (rec.instr, rec.w[0], rec.w[2], rec.w[3]))
    if rec.instr == fp.I_IEND and on: break
Path(out).write_text("\n".join(lines) + "\n")
print(len(lines))
