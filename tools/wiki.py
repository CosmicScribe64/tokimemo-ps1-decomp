#!/usr/bin/env python3
"""Wiki upkeep: rotate the two append-only pages, generate the catalogs, and lint (T-24400).

The wiki has two append-only pages that grow without bound: wiki/log.md (dated entries) and
wiki/matching-notes.md (one section per ticket). Agents keep appending to them as before; this
tool moves old entries out verbatim, so the active pages stay small and nothing is edited:

  rotate-log    entries of wiki/log.md older than --keep-days, then the oldest until the page is under
                --keep-bytes, go to wiki/log/YYYY-Www.md (ISO week of the entry date) in their original order
  rotate-notes  the oldest sections of wiki/matching-notes.md go to wiki/notes/matching-NN.md
                chunks (about --chunk-bytes each) until the active page is under --keep-bytes
  catalog       writes wiki/tickets/catalog.md (every ticket by status), wiki/notes/sections.md
                (every notes section and the file holding it) and wiki/notes/functions.md (every
                function named in the notes, with the sections that mention it)
  lint          broken [[wikilinks]], ticket status vs kanban column, pages over --max-bytes;
                exit status 1 when it finds a broken link or a status mismatch

Usage: python3 tools/wiki.py [--root REPO] rotate-log|rotate-notes|catalog|lint [options]
No game data is read: only wiki/ and config/symbol_addrs*.txt (function names).
"""
import argparse
import datetime
import glob
import os
import re
import sys

LOG_HEAD = re.compile(r"^## \[(\d{4}-\d{2}-\d{2})\]")
SECTION = re.compile(r"^## ")
LINK = re.compile(r"\[\[([^\]|#]+)(?:#[^\]|]*)?(?:\|[^\]]*)?\]\]")
FUNC_HEX = re.compile(r"\b(func_[0-9A-F]{8})\b")
MAX_REFS = 8
COLS = ["Backlog", "Ready", "In Progress", "In Review", "Done"]


def read(p):
    with open(p, encoding="utf-8", errors="replace") as f:
        return f.read()


def write(p, s):
    os.makedirs(os.path.dirname(p) or ".", exist_ok=True)
    with open(p, "w", encoding="utf-8") as f:
        f.write(s)


def split_blocks(text, head):
    """Split text into (preamble, [blocks]); a block starts at a line matching `head`."""
    lines = text.split("\n")
    pre, blocks, cur = [], [], None
    in_code = False
    for ln in lines:
        if ln.startswith("```"):
            in_code = not in_code
        if not in_code and head.match(ln):
            if cur is not None:
                blocks.append("\n".join(cur))
            cur = [ln]
        elif cur is None:
            pre.append(ln)
        else:
            cur.append(ln)
    if cur is not None:
        blocks.append("\n".join(cur))
    return "\n".join(pre), blocks


def norm(block):
    return block.rstrip("\n") + "\n"


def archive_page(path, title, note):
    if os.path.exists(path):
        return read(path).rstrip("\n") + "\n"
    return "---\ntype: archive\n---\n\n# %s\n\n%s\n" % (title, note)


def rotate_log(root, keep_days, today=None, keep_bytes=200000):
    wiki = os.path.join(root, "wiki")
    lp = os.path.join(wiki, "log.md")
    pre, blocks = split_blocks(read(lp), LOG_HEAD)
    today = today or datetime.date.today()
    cutoff = today - datetime.timedelta(days=keep_days)
    keep, moved = [], {}
    for b in blocks:
        d = datetime.date.fromisoformat(LOG_HEAD.match(b).group(1))
        if d < cutoff:
            y, w, _ = d.isocalendar()
            moved.setdefault("%d-W%02d" % (y, w), []).append(b)
        else:
            keep.append(b)
    while keep and sum(len(b) for b in keep) > keep_bytes:  # busy weeks: also cap the active page by size
        b = keep.pop(0)
        y, w, _ = datetime.date.fromisoformat(LOG_HEAD.match(b).group(1)).isocalendar()
        moved.setdefault("%d-W%02d" % (y, w), []).append(b)
    if not moved:
        print("rotate-log: nothing older than %s" % cutoff)
        return 0
    for wk, bs in moved.items():
        ap = os.path.join(wiki, "log", wk + ".md")
        s = archive_page(ap, "Log %s" % wk, "Entries moved verbatim from [[log]] by `tools/wiki.py rotate-log`; append-only.")
        write(ap, s + "\n" + "\n".join(norm(b) for b in bs))
    write(lp, pre.rstrip("\n") + "\n\n" + "\n".join(norm(b) for b in keep))
    print("rotate-log: moved %d entries into %d weekly pages, kept %d" % (sum(map(len, moved.values())), len(moved), len(keep)))
    return 0


def rotate_notes(root, keep_bytes, chunk_bytes):
    wiki = os.path.join(root, "wiki")
    np_ = os.path.join(wiki, "matching-notes.md")
    pre, blocks = split_blocks(read(np_), SECTION)
    total = sum(len(b) for b in blocks)
    moving = []
    while blocks and total > keep_bytes:
        b = blocks.pop(0)
        total -= len(b)
        moving.append(b)
    if not moving:
        print("rotate-notes: matching-notes.md is under %d bytes" % keep_bytes)
        return 0
    chunks = sorted(glob.glob(os.path.join(wiki, "notes", "matching-[0-9][0-9].md")))
    n = int(re.search(r"(\d\d)\.md$", chunks[-1]).group(1)) if chunks else 1
    cur = os.path.join(wiki, "notes", "matching-%02d.md" % n)
    for b in moving:
        s = archive_page(cur, "Matching notes, part %d" % n,
                         "Sections moved verbatim from [[matching-notes]] by `tools/wiki.py rotate-notes`, oldest first. "
                         "Find a function in [[notes/functions]] and a section in [[notes/sections]].")
        if len(s) > chunk_bytes and "\n## " in s:
            n += 1
            cur = os.path.join(wiki, "notes", "matching-%02d.md" % n)
            s = archive_page(cur, "Matching notes, part %d" % n,
                             "Sections moved verbatim from [[matching-notes]] by `tools/wiki.py rotate-notes`, oldest first. "
                             "Find a function in [[notes/functions]] and a section in [[notes/sections]].")
        write(cur, s + "\n" + norm(b))
    write(np_, pre.rstrip("\n") + "\n\n" + "\n".join(norm(b) for b in blocks))
    print("rotate-notes: moved %d sections, kept %d (%d bytes)" % (len(moving), len(blocks), total))
    return 0


def tickets(root):
    out = {}
    for f in glob.glob(os.path.join(root, "wiki", "tickets", "T-*.md")):
        s = read(f)[:3000]
        tid = re.search(r"^id:\s*(\S+)", s, re.M)
        st = re.search(r"^status:\s*([^#\n]+)", s, re.M)
        ti = re.search(r"^title:\s*(.+)$", s, re.M)
        base = os.path.basename(f)[:-3]
        tid = tid.group(1) if tid else "-".join(base.split("-")[:2])
        out[tid] = dict(slug=base, status=(st.group(1).strip() if st else "Backlog"),
                        title=(ti.group(1).strip().strip('"') if ti else base))
    return out


def tnum(t):
    return int(re.sub(r"\D", "", t.split("-")[1]) or 0)


def function_names(root):
    names = set()
    for f in glob.glob(os.path.join(root, "config", "symbol_addrs*.txt")):
        for ln in read(f).split("\n"):
            m = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=.*type:func", ln)
            if m and len(m.group(1)) > 3:
                names.add(m.group(1))
    return names


def anchor(h):
    return re.sub(r"[\[\]|#^]", "", h).strip()


def catalog(root):
    wiki = os.path.join(root, "wiki")
    ts = tickets(root)
    lines = ["---", "type: catalog", "---", "", "# Ticket catalog", "",
             "Generated by `tools/wiki.py catalog` from ticket frontmatter; do not edit. Board: [[kanban]].", ""]
    for col in COLS + sorted({t["status"] for t in ts.values()} - set(COLS)):
        rows = sorted((k for k, t in ts.items() if t["status"] == col), key=tnum, reverse=(col == "Done"))
        if not rows:
            continue
        lines += ["## %s (%d)" % (col, len(rows)), ""]
        lines += ["- [[tickets/%s|%s]] %s" % (ts[k]["slug"], k, ts[k]["title"]) for k in rows]
        lines.append("")
    write(os.path.join(wiki, "tickets", "catalog.md"), "\n".join(lines))

    files = [os.path.join(wiki, "matching-notes.md")] + sorted(glob.glob(os.path.join(wiki, "notes", "matching-[0-9][0-9].md")))
    names = function_names(root)
    word = re.compile(r"\b([A-Za-z_][A-Za-z0-9_]{3,})\b")
    sections, funcs = [], {}
    for f in files:
        page = os.path.relpath(f, wiki)[:-3]
        _, blocks = split_blocks(read(f), SECTION)
        for b in blocks:
            h = b.split("\n", 1)[0][3:].strip()
            tk = re.search(r"T-\d+[A-Z]?", h)
            ref = "[[%s#%s|%s]]" % (page, anchor(h), tk.group(0) if tk else anchor(h)[:40])
            sections.append((page, ref))
            found = set(FUNC_HEX.findall(b)) | {w for w in word.findall(b) if w in names}
            for fn in found:
                funcs.setdefault(fn, []).append(ref)
    sl = ["---", "type: catalog", "---", "", "# Matching-notes sections", "",
          "Generated by `tools/wiki.py catalog`; newest last. Active page: [[matching-notes]]. Functions: [[notes/functions]].", ""]
    for page in [os.path.relpath(f, wiki)[:-3] for f in files[1:]] + ["matching-notes"]:
        rows = [r for p, r in sections if p == page]
        if rows:
            sl += ["## %s" % page, ""] + ["- " + r for r in rows] + [""]
    write(os.path.join(wiki, "notes", "sections.md"), "\n".join(sl))
    groups = [("functions-main", "main executable (`func_8004`..`func_800B`)", lambda f: f.startswith("func_800")),
              ("functions-overlays", "overlays (`func_8010`.. and up; one name can exist in several overlays)", lambda f: f.startswith("func_80") and not f.startswith("func_800")),
              ("functions-named", "named functions (from `config/symbol_addrs*.txt`)", lambda f: not f.startswith("func_"))]
    hub = ["---", "type: catalog", "---", "", "# Functions in the matching notes", "",
           "Generated by `tools/wiki.py catalog`: every function named in [[matching-notes]] or its archived parts, "
           "with links to the sections that mention it (newest last, at most %d per function; the link text is the section's ticket). "
           "Search the right page for the name:" % MAX_REFS, ""]
    for page, what, sel in groups:
        names = sorted(f for f in funcs if sel(f))
        hub.append("- [[notes/%s]] - %s, %d functions" % (page, what, len(names)))
        fl = ["---", "type: catalog", "---", "", "# Functions in the matching notes: %s" % what, "",
              "Generated by `tools/wiki.py catalog`; see [[notes/functions]].", ""]
        for fn in names:
            refs = list(dict.fromkeys(funcs[fn]))
            more = " (+%d older, see [[notes/sections]])" % (len(refs) - MAX_REFS) if len(refs) > MAX_REFS else ""
            fl.append("- `%s`: %s%s" % (fn, "; ".join(refs[-MAX_REFS:]), more))
        write(os.path.join(wiki, "notes", page + ".md"), "\n".join(fl) + "\n")
    write(os.path.join(wiki, "notes", "functions.md"), "\n".join(hub) + "\n")
    print("catalog: %d tickets, %d sections, %d functions" % (len(ts), len(sections), len(funcs)))
    return 0


def lint(root, max_bytes):
    wiki = os.path.join(root, "wiki")
    pages = {}
    for f in glob.glob(os.path.join(wiki, "**", "*.md"), recursive=True):
        rel = os.path.relpath(f, wiki)[:-3]
        pages[rel] = f
    base = {}
    for rel in pages:
        base.setdefault(os.path.basename(rel), []).append(rel)
    broken, big = [], []
    for rel, f in pages.items():
        s = read(f)
        if len(s.encode("utf-8")) > max_bytes and not rel.startswith(("log/", "notes/")):
            big.append((rel, len(s.encode("utf-8"))))
        if rel.startswith("log/") or rel in ("log", "tickets/_template"):
            continue
        for m in LINK.finditer(re.sub(r"`[^`\n]*`", "", s)):
            t = m.group(1).strip().lstrip("[")
            if os.path.exists(os.path.join(wiki, t)):
                continue
            if t.endswith(".md"):
                t = t[:-3]
            if t not in pages and t not in base:
                broken.append((rel, t))
    kan = read(os.path.join(wiki, "kanban.md")) if "kanban" in pages else ""
    col, where = None, {}
    for ln in kan.split("\n"):
        m = re.match(r"^## (.+?)\s*$", ln)
        if m:
            col = m.group(1)
        m = re.match(r"^- \[[ x]\] \[\[tickets/(T-[0-9A-Z]+)", ln)
        if m and col:
            where[m.group(1)] = col
    mism = [(k, t["status"], where.get(k)) for k, t in tickets(root).items() if where.get(k) != t["status"]]
    for rel, t in sorted(broken)[:200]:
        print("broken link: %s -> [[%s]]" % (rel, t))
    for k, s, c in sorted(mism, key=lambda x: tnum(x[0]))[:200]:
        print("status mismatch: %s frontmatter %s, kanban %s" % (k, s, c))
    for rel, n in sorted(big, key=lambda x: -x[1]):
        print("large page: %s (%d bytes > %d)" % (rel, n, max_bytes))
    print("lint: %d broken links, %d status mismatches, %d large pages" % (len(broken), len(mism), len(big)))
    return 1 if broken or mism else 0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--root", default=".")
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("rotate-log")
    r.add_argument("--keep-days", type=int, default=7)
    r.add_argument("--today")
    r.add_argument("--keep-bytes", type=int, default=200000)
    r = sub.add_parser("rotate-notes")
    r.add_argument("--keep-bytes", type=int, default=150000)
    r.add_argument("--chunk-bytes", type=int, default=250000)
    sub.add_parser("catalog")
    r = sub.add_parser("lint")
    r.add_argument("--max-bytes", type=int, default=250000)
    a = ap.parse_args(argv)
    if a.cmd == "rotate-log":
        return rotate_log(a.root, a.keep_days, datetime.date.fromisoformat(a.today) if a.today else None, a.keep_bytes)
    if a.cmd == "rotate-notes":
        return rotate_notes(a.root, a.keep_bytes, a.chunk_bytes)
    if a.cmd == "catalog":
        return catalog(a.root)
    return lint(a.root, a.max_bytes)


if __name__ == "__main__":
    sys.exit(main())
