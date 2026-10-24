"""Add or drop a MAIN_API_OVERRIDE_<N> in a C file so the file keeps the view it had under the placeholder P.
Runs inside Docker from the repo root. Library + CLI:
  ovr.py try FILE [SYMBOL...|ALL]   add overrides for the changed views of FILE (default: every non-benign one),
                                compile the object, compare it with the baseline, then drop each override that is not needed
  ovr.py cleanup                 unwrap declarations in main_api.h that no file overrides (also drops old stale wrappers)
Inputs, made at the base commit and not committed: drafts/T-27003/snap0.json (`sync_protos.py --snapshot`),
drafts/T-27003/head/ (`git archive HEAD src`), .base_build/ (the .o files of the base build, see objdiff.py).
"""
import json, os, re, subprocess, sys
sys.path.insert(0, "tools")
import sync_protos as sp

PRE = open("drafts/T-27003/main_api.pre.h", encoding="latin-1").read().split("\n")
S0 = json.load(open("drafts/T-27003/snap0.json"))
REN = {}
for t in ("obin", "sdk"):
    for l in open("drafts/T-27003/%s.tsv" % t):
        if not l.startswith("#") and l.strip():
            f = l.split(); REN[f[2]] = f[3]
INV = {n: p for p, n in REN.items()}

def rd(p): return open(p, encoding="latin-1").read()
def wr(p, t): open(p, "w", encoding="latin-1").write(t)

def old_decl(p, n):
    """Declaration of the placeholder in the old main_api.h with the new name, or None."""
    hits = [l for l in PRE if l[:1].isalpha() and re.search(r"(?<![\w])%s(?![\w])" % re.escape(p), re.sub(r"/\*.*?\*/", " ", l))
            and re.sub(r"/\*.*?\*/", " ", l).rstrip().endswith(";") and "(" in l]
    if len(hits) != 1:
        return None
    d = re.sub(r"\s*/\*.*\*/\s*$", "", hits[0].rstrip())
    return re.sub(r"(?<![\w])%s(?![\w])" % re.escape(p), n, d)

def head_tokens(path):
    """Identifiers of the file as it was at HEAD (drafts/T-27003/head, from git archive)."""
    f = os.path.join("drafts/T-27003/head", path)
    return sp.tokens(rd(f)) if os.path.exists(f) else set()

_API = {}
def api_type(n):
    if not _API:
        m = sp.Model("include")
        _API.update({k: v[0] for k, v in m.api.items()})
    return _API.get(n)

def view_changes(path, snap1=None):
    """[(unit, name, kind, old view, new view, class)] for the names of the table that the file uses: the view it
    had under the placeholder (or under the name, when it never used the placeholder) against the view it has now."""
    s0 = dict(S0[path])
    toks = head_tokens(path)
    out = []
    for p, n in REN.items():
        if p not in toks and n not in toks:
            continue
        ta = s0.get(p) if p in toks else s0.get(n)
        tb = api_type(n)
        if ta == tb:
            continue
        for u, name, kind, a, b, cls in sp.view_changes({path: {n: ta} if ta else {}}, {path: {n: tb} if tb else {}}):
            out.append((u, name, kind, a, b, cls))
    return out

def add_override(path, n, ta, tb, decl):
    text = rd(path)
    lines = text.split("\n")
    if any(re.match(r"\s*#\s*define\s+MAIN_API_OVERRIDE_%s\b" % n, l) for l in lines):
        return False
    first = next(i for i, l in enumerate(lines) if re.match(r"\s*#\s*include", l))
    defs = [i for i, l in enumerate(lines[:first]) if re.match(r"\s*#\s*define\s+MAIN_API_OVERRIDE_", l)]
    at = (defs[-1] + 1) if defs else first
    why = ("implicit declaration, as matched (main_api.h: %s)" % sp.short(tb)) if ta is None else \
          ("matched with %s (main_api.h: %s)" % (sp.short(ta), sp.short(tb) if tb else "implicit"))
    lines.insert(at, "#define MAIN_API_OVERRIDE_%s /* %s */" % (n, why))
    if decl:
        last = max(i for i, l in enumerate(lines) if re.match(r"\s*#\s*include", l))
        lines[last + 1:last + 1] = ["", decl]
    wr(path, "\n".join(lines))
    return True

def drop_override(path, n, decl):
    lines = rd(path).split("\n")
    out = [l for l in lines if not re.match(r"\s*#\s*define\s+MAIN_API_OVERRIDE_%s\b" % n, l)]
    if decl and decl in out:
        i = out.index(decl)
        del out[i]
        if i > 0 and out[i - 1] == "" and (i >= len(out) or out[i] == ""):
            del out[i - 1]
    wr(path, "\n".join(out))

API = "include/main_api.h"

def wrap(n):
    """Wrap the declaration of n in main_api.h in #ifndef MAIN_API_OVERRIDE_n (idempotent)."""
    lines = rd(API).split("\n")
    hits = [i for i, l in enumerate(lines) if l[:1].isalpha() and re.search(r"(?<![\w])%s(?![\w])" % re.escape(n), re.sub(r"/\*.*?\*/", " ", l))
            and re.sub(r"/\*.*?\*/", " ", l).rstrip().endswith(";") and "(" in l]
    assert len(hits) == 1, (n, hits)
    i = hits[0]
    if lines[i - 1].strip() != "#ifndef MAIN_API_OVERRIDE_%s" % n:
        lines[i:i + 1] = ["#ifndef MAIN_API_OVERRIDE_" + n, lines[i], "#endif"]
        wr(API, "\n".join(lines))

def unwrap_unused():
    """Remove the wrapper of every symbol that no file or header overrides any more."""
    used = set()
    for root in ("src", "include"):
        for d, _s, fs in os.walk(root):
            for f in fs:
                if f.endswith((".c", ".h")) and os.path.join(d, f) != API:
                    used |= set(re.findall(r"#\s*define\s+MAIN_API_OVERRIDE_(\w+)", rd(os.path.join(d, f))))
    lines = rd(API).split("\n")
    out, i, n_un = [], 0, 0
    while i < len(lines):
        m = re.match(r"#ifndef MAIN_API_OVERRIDE_(\w+)$", lines[i])
        if m and m.group(1) not in used and i + 2 < len(lines) and lines[i + 2].strip() == "#endif" and lines[i + 1][:1].isalpha():
            out.append(lines[i + 1]); i += 3; n_un += 1
        else:
            out.append(lines[i]); i += 1
    wr(API, "\n".join(out))
    return n_un

def obj_of(path):
    if path.startswith("src/main/"):
        return "build/" + path[:-2] + ".o"
    u = path.split("/")[2]
    return "build/ovl/%s/%s.o" % (u, path[:-2])

def matches(path):
    r = subprocess.run(["ninja", "-j2", obj_of(path)], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if r.returncode:
        return "compile error: " + r.stdout.decode("latin-1")[-300:]
    r = subprocess.run(["python3", "drafts/T-27003/objdiff.py", os.path.basename(path)[:-2]], stdout=subprocess.PIPE)
    out = r.stdout.decode()
    return None if (" differ" in out and ("0 differ" in out or obj_of(path)[len("build/"):] not in out)) else "differs"

def main(path, only, snap1="drafts/T-27003/snap1.json"):
    s1 = json.load(open(snap1))
    ch = [c for c in view_changes(path, s1) if (True if only == {"ALL"} else c[1] in only if only else c[5] != "benign")]
    added = []
    for unit, n, kind, ta, tb, cls in ch:
        p = INV.get(n, n)
        decl = old_decl(p, n) if ta is not None else None
        if ta is not None and decl is None:
            print("  %s: no unique old declaration of %s; skipped" % (n, p)); continue
        if add_override(path, n, ta, tb, decl):
            wrap(n)
            added.append((n, ta, tb, decl))
    print("%s: added %s" % (path, [a[0] for a in added]))
    res = matches(path)
    print("  with all: %s" % (res or "match"))
    if res:
        return
    for n, ta, tb, decl in list(added):
        before = rd(path)
        drop_override(path, n, decl)
        res = matches(path)
        if res:
            wr(path, before)
            print("  keep %s" % n)
        else:
            print("  drop %s (not needed)" % n)

if __name__ == "__main__":
    if sys.argv[1] == "try":
        main(sys.argv[2], set(sys.argv[3:]))
    elif sys.argv[1] == "cleanup":
        print("unwrapped", unwrap_unused())
