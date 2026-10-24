"""Compare the allocated sections of the .o files under build/ with .base_build/ (names of symbols ignored).
usage: objdiff.py [build_dir] ; prints the objects whose bytes differ, or that are missing."""
import os, struct, sys
base = ".base_build"
new = "build"
only = sys.argv[1:]

def sections(path):
    d = open(path, "rb").read()
    if d[:4] != b"\x7fELF":
        return None
    le = d[5] == 1
    e = "<" if le else ">"
    shoff, = struct.unpack_from(e + "I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(e + "HHH", d, 0x2E)
    secs = []
    for i in range(shnum):
        n, t, f, a, off, size = struct.unpack_from(e + "IIIIII", d, shoff + i * shentsize)
        secs.append((n, t, f, off, size))
    stroff = secs[shstrndx][3]
    out = {}
    for n, t, f, off, size in secs:
        name = d[stroff + n:d.index(b"\0", stroff + n)].decode()
        if f & 2:   # SHF_ALLOC
            out[name] = d[off:off + size] if t != 8 else ("bss", size)
    return out

bad = []
n = 0
for root, _dirs, files in os.walk(base):
    for fn in files:
        if not fn.endswith(".o"):
            continue
        b = os.path.join(root, fn)
        if only and not any(o in b for o in only):
            continue
        p = os.path.join(new, os.path.relpath(b, base))
        n += 1
        if not os.path.exists(p):
            bad.append((p, "missing")); continue
        if sections(b) != sections(p):
            bad.append((p, "differs"))
print("%d objects compared, %d differ" % (n, len(bad)))
for p, w in sorted(bad):
    print(w, p)
