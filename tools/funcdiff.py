#!/usr/bin/env python3
"""Per-function diff of the built object against the original object.

Compares `objdump -dr` of the built object of the source file that holds each function
(build/src/main/<addr>.o, found by searching src/main/*.c; T-0012) with the same object under
expected/ (a copy of the all-INCLUDE_ASM build; see wiki/decompile-workflow.md) and
prints MATCH or a short diff for each named function. Instruction
addresses and absolute branch targets are stripped, so only instructions,
relocations and symbol-relative targets are compared.

Usage (in Docker): python3 tools/funcdiff.py [--built OBJ] func_80042400 [func_...]
--built compares another object (e.g. a scratch compile or an overlay object)
instead of the per-file object; --expected names the original-side object (default
expected/<same path as built>, e.g. a copy of build/ovl/<NAME>/<NAME>.o).
Exit code 1 if any function differs.
"""
import difflib
import glob
import re
import subprocess
import sys

SRC_GLOB = "src/main/*.c"


def object_of(name):
    """Built object path of the src/main file that mentions `name`, or None."""
    pat = re.compile(r"\b%s\b" % re.escape(name))
    for c in sorted(glob.glob(SRC_GLOB)):
        if pat.search(open(c).read()):
            return "build/" + c[:-2] + ".o"
    return None


def functions(obj, wanted):
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", obj], check=True,
                         stdout=subprocess.PIPE, text=True).stdout
    funcs, cur = {}, None
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <(\w+)>:", line)
        if m and re.match(r"^(L[0-9A-Fa-f]{8}|\.?L\w+|jtbl_\w+)$", m.group(1)):
            continue  # internal label: stay inside the current function
        if m:
            cur = m.group(1)
            funcs[cur] = []
        elif cur and line.strip() and not line.startswith("Disassembly"):
            line = re.sub(r"^\s*[0-9a-f]+:\s*", "", line)
            # branch/jump targets: drop the absolute address, keep <sym+off>
            line = re.sub(r"\b[0-9a-f]+ (<[^>]+>)", r"\1", line)
            funcs[cur].append(re.sub(r"\s+", " ", line.strip()))
    return {k: funcs.get(k) for k in wanted}


def main(names):
    built = expected = None
    while names[:1] in (["--built"], ["--expected"]):
        if names[0] == "--built":
            built = names[1]
        else:
            expected = names[1]
        names = names[2:]
    if not names:
        sys.exit(__doc__)
    bad = 0
    for n in names:
        b = built or object_of(n)
        if b is None:
            print("%s: not found in %s" % (n, SRC_GLOB))
            bad += 1
            continue
        e = expected or "expected/" + b
        got, want = functions(b, [n])[n], functions(e, [n])[n]
        if got is None or want is None:
            print("%s: missing in %s" % (n, "built" if got is None else "expected"))
            bad += 1
        elif got == want:
            print("%s: MATCH" % n)
        else:
            bad += 1
            print("%s: DIFF (- expected, + built)" % n)
            for d in difflib.unified_diff(want, got, lineterm="", n=1):
                if not d.startswith(("---", "+++")):
                    print("  " + d)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
