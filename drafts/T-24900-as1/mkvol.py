"""mkvol.py SRC.c HEADER TAG SYM... : write var/TAG.h (HEADER with SYMs volatile) and var/TAG.c (SRC including it)."""
import sys, re
src, header, tag, syms = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4:]
h = open(header).read()
for x in syms:
    pat = re.compile(r"^extern (?!volatile)([\w ]+?) %s(\[[^\]]*\])?;" % x, re.M)
    h, n = pat.subn(lambda m: "extern volatile %s %s%s;" % (m.group(1), x, m.group(2) or ""), h)
    if n != 1: print("WARN", x, n)
open("build/as1tr/var/%s.h" % tag, "w").write(h)
s = open(src).read()
inc = '#include "%s"' % header.split("include/")[1]
assert inc in s, inc
open("build/as1tr/var/%s.c" % tag, "w").write(s.replace(inc, '#include "../build/as1tr/var/%s.h"' % tag))
