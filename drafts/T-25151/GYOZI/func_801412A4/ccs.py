# cc -S listing of the NON_MATCHING TU; prints the function's ugen listing
import sys, os, re, subprocess, tempfile
sys.path.insert(0, '/work/tools'); os.chdir('/work')
import cc, funcdiff_verify as fv
src = sys.argv[1]; fn = sys.argv[2]
text = fv.preprocess_draft(src, True)
text = "\n".join(l for l in text.split("\n") if not l.startswith("INCLUDE_ASM") and not l.startswith("INCLUDE_RODATA"))
tmp = tempfile.mkdtemp()
c = os.path.join(tmp, "t.c"); open(c, "w").write(text)
envd = tempfile.mkdtemp()
env = cc.ido_frame_env("5.3", envd)
flags = [f for f in cc.IDO_CFLAGS if f != "-c"]
r = subprocess.run(["/opt/ido/5.3/cc", "-S"] + flags + ["-o", os.path.join(tmp, "t.s"), c], env=env, cwd=tmp, capture_output=True, text=True)
if r.returncode: print(r.stderr[-2000:]); sys.exit(1)
s = open(os.path.join(tmp, "t.s")).read()
m = re.search(r'\n\s*\.ent\s+%s\b.*?\.end\s+%s' % (fn, fn), s, re.S)
print(m.group(0) if m else "not found")
