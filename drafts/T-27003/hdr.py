"""Small header edits for the T-27003 alias merge (one operation per call, from the repo root).
  hdr.py wrap NAME            wrap NAME's declaration in include/main_api.h in #ifndef MAIN_API_OVERRIDE_NAME
  hdr.py unwrap NAME          remove that wrapper
  hdr.py type NAME 'DECL'     replace NAME's declaration line (keeps a trailing comment unless DECL has one)
  hdr.py define FILE NAME 'REASON' ['DECL']   add `#define MAIN_API_OVERRIDE_NAME /* REASON */` before the first
                              include of FILE and, with DECL, the declaration after its last include
"""
import re, sys
API = "include/main_api.h"

def find(lines, name):
    hits = [i for i, l in enumerate(lines) if l[:1].isalpha() and re.search(r"(?<![\w])%s(?![\w])" % re.escape(name), re.sub(r"/\*.*?\*/", " ", l))
            and re.sub(r"/\*.*?\*/", " ", l).rstrip().endswith(";") and "(" in l]
    if len(hits) != 1:
        sys.exit("%s: %d declarations %s" % (name, len(hits), hits))
    return hits[0]

def rd(p): return open(p, encoding="latin-1").read().split("\n")
def wr(p, lines): open(p, "w", encoding="latin-1").write("\n".join(lines))

def wrapped(lines, i, name):
    return lines[i - 1].strip() == "#ifndef MAIN_API_OVERRIDE_%s" % name

def main(a):
    op = a[0]
    if op in ("wrap", "unwrap", "type"):
        lines = rd(API)
        i = find(lines, a[1])
        if op == "wrap":
            if not wrapped(lines, i, a[1]):
                lines[i:i + 1] = ["#ifndef MAIN_API_OVERRIDE_" + a[1], lines[i], "#endif"]
        elif op == "unwrap":
            if wrapped(lines, i, a[1]):
                assert lines[i + 1].strip() == "#endif"
                lines[i - 1:i + 2] = [lines[i]]
        else:
            m = re.search(r"(\s*/\*.*\*/)\s*$", lines[i])
            lines[i] = a[2] + ("" if "/*" in a[2] or not m else m.group(1))
        wr(API, lines)
    elif op == "define":
        path, name, reason = a[1:4]
        lines = rd(path)
        if not any(re.match(r"\s*#\s*define\s+MAIN_API_OVERRIDE_%s\b" % name, l) for l in lines):
            first = next(i for i, l in enumerate(lines) if re.match(r'\s*#\s*include', l))
            defs = [i for i, l in enumerate(lines[:first]) if re.match(r"\s*#\s*define\s+MAIN_API_OVERRIDE_", l)]
            at = (defs[-1] + 1) if defs else first
            lines.insert(at, "#define MAIN_API_OVERRIDE_%s /* %s */" % (name, reason))
        if len(a) > 4:
            last = max(i for i, l in enumerate(lines) if re.match(r'\s*#\s*include', l))
            decl = a[4]
            if decl not in lines:
                # after the last include, and after a blank line when there is one
                lines.insert(last + 1, "")
                lines.insert(last + 2, decl)
        wr(path, lines)

main(sys.argv[1:])
