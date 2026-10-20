#!/usr/bin/env python3
"""Tests for tools/wiki.py with a synthetic wiki (no game data).

Run: tools/docker.sh python3 tools/test_wiki.py
"""
import os
import tempfile
import unittest

import wiki


def put(root, rel, text):
    p = os.path.join(root, rel)
    os.makedirs(os.path.dirname(p), exist_ok=True)
    with open(p, "w") as f:
        f.write(text)


def get(root, rel):
    with open(os.path.join(root, rel)) as f:
        return f.read()


LOG = """---
type: log
---

# Log

Append-only.

## [2026-10-01] ticket | old one
body 1

## [2026-10-02] ticket | old two
body 2
```
## [2026-10-02] not a heading inside code
```

## [2026-10-19] ticket | new
body 3
"""


class WikiTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name

    def tearDown(self):
        self.tmp.cleanup()

    def test_rotate_log_moves_old_entries_verbatim(self):
        put(self.root, "wiki/log.md", LOG)
        wiki.rotate_log(self.root, 7, today=wiki.datetime.date(2026, 10, 20))
        active = get(self.root, "wiki/log.md")
        self.assertIn("## [2026-10-19] ticket | new", active)
        self.assertNotIn("old one", active)
        self.assertIn("Append-only.", active)
        arch = get(self.root, "wiki/log/2026-W40.md")
        self.assertIn("## [2026-10-01] ticket | old one\nbody 1", arch)
        self.assertIn("## [2026-10-02] not a heading inside code", arch)
        before = sum(1 for ln in LOG.split("\n") if ln.startswith("## [") and "inside" not in ln)
        after = sum(1 for p in ("wiki/log.md", "wiki/log/2026-W40.md") for ln in get(self.root, p).split("\n")
                    if ln.startswith("## [") and "inside" not in ln)
        self.assertEqual(before, after)

    def test_rotate_log_appends_to_existing_week(self):
        put(self.root, "wiki/log.md", LOG)
        wiki.rotate_log(self.root, 7, today=wiki.datetime.date(2026, 10, 20))
        put(self.root, "wiki/log.md", "# Log\n\n## [2026-10-03] ticket | late\nx\n")
        wiki.rotate_log(self.root, 7, today=wiki.datetime.date(2026, 10, 20))
        arch = get(self.root, "wiki/log/2026-W40.md")
        self.assertLess(arch.index("old two"), arch.index("late"))

    def test_rotate_notes_keeps_newest_and_chunks(self):
        secs = "".join("## T-%d section\n%s\n\n" % (i, "x" * 100) for i in range(10))
        put(self.root, "wiki/matching-notes.md", "---\ntype: notes\n---\n\n# Matching notes\n\n" + secs)
        wiki.rotate_notes(self.root, keep_bytes=250, chunk_bytes=400)
        active = get(self.root, "wiki/matching-notes.md")
        self.assertIn("## T-9 section", active)
        self.assertNotIn("## T-0 section", active)
        parts = sorted(os.listdir(os.path.join(self.root, "wiki/notes")))
        self.assertGreater(len(parts), 1)
        text = "".join(get(self.root, "wiki/notes/" + p) for p in parts) + active
        for i in range(10):
            self.assertEqual(text.count("## T-%d section" % i), 1)

    def test_catalog_and_lint(self):
        put(self.root, "config/symbol_addrs.txt", "draw_thing = 0x80040000; // type:func\n")
        put(self.root, "wiki/tickets/T-0001-a.md", "---\nid: T-0001\ntitle: First\nstatus: Done\n---\n")
        put(self.root, "wiki/tickets/T-0002-b.md", "---\nid: T-0002\ntitle: Second\nstatus: Backlog\n---\n")
        put(self.root, "wiki/kanban.md", "## Backlog\n\n- [ ] [[tickets/T-0002-b|T-0002 Second]]\n\n## Done\n\n- [ ] [[tickets/T-0001-a|T-0001 First]]\n")
        put(self.root, "wiki/matching-notes.md", "# Notes\n\n## T-0001 thing\nfunc_8004A1A0 and `draw_thing` [[kanban]]\n")
        wiki.catalog(self.root)
        cat = get(self.root, "wiki/tickets/catalog.md")
        self.assertIn("## Done (1)", cat)
        self.assertIn("[[tickets/T-0002-b|T-0002]] Second", cat)
        self.assertIn("[[notes/functions-main]]", get(self.root, "wiki/notes/functions.md"))
        self.assertIn("`func_8004A1A0`: [[matching-notes#T-0001 thing|T-0001]]", get(self.root, "wiki/notes/functions-main.md"))
        self.assertIn("`draw_thing`", get(self.root, "wiki/notes/functions-named.md"))
        self.assertEqual(wiki.lint(self.root, 10 ** 6), 0)
        put(self.root, "wiki/overview.md", "see [[missing-page]]\n")
        self.assertEqual(wiki.lint(self.root, 10 ** 6), 1)


if __name__ == "__main__":
    unittest.main()
