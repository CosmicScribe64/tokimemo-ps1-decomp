import sys
# usage: setbody.py FILE FUNC BODYFILE : replace the body of the guarded FUNC (between '#ifdef NON_MATCHING' and '#else')
src, func, body = sys.argv[1:4]
L = open(src).read().split("\n")
inc = [i for i, l in enumerate(L) if l.startswith("INCLUDE_ASM(") and l.rstrip().endswith(", %s);" % func)]
assert len(inc) == 1, inc
i = inc[0]
assert L[i-1] == "#else", L[i-1]
j = i - 1
k = j
while L[k] != "#ifdef NON_MATCHING":
    k -= 1
newbody = open(body).read().rstrip("\n").split("\n")
L[k+1:j] = newbody
open(src, "w").write("\n".join(L))
print("ok", func, len(newbody))
