---
id: T-24400
title: Reorganize the wiki so important information is easy to find
status: Done
assignee: claude-orchestrator
created: 2026-10-20
updated: 2026-10-20
links: ["[[index]]", "[[status]]", "[[playbook]]", "[[SCHEMA]]"]
---

## Goal

User request 2026-10-20: the wiki had grown hard to search (index.md 135 KB with 654 ticket lines, log.md 1.9 MB, matching-notes.md 1.7 MB, recipes buried in 400 KB of reference pages).

## Done

- `tools/wiki.py` (tests `tools/test_wiki.py`): `rotate-log` (log entries older than 7 days move verbatim to `log/YYYY-Www.md`), `rotate-notes` (oldest matching-notes sections move verbatim to `notes/matching-NN.md` until the active page is under 150 KB), `catalog` (`tickets/catalog.md`, `notes/sections.md`, `notes/functions*.md`), `lint` (broken links, ticket status vs board, large pages).
- [[index]] is a one-screen hub by topic; the old index is [[archive/index-2026-10-20]]; tools list moved to [[tooling]].
- New [[status]] (progress, what is left by unit, active work) and [[playbook]] (the matching recipes that closed matches, by symptom).
- Superseded status pages moved to `archive/`; the 21 per-overlay result pages moved to `families/` (links by page name still resolve).
- Fixed the broken links lint found (17).
- AGENTS.md rules 1-2 and [[SCHEMA]] describe the new layout; the orchestrator's merge script runs rotate and catalog after each merge.

## Acceptance criteria

- [x] index fits on one screen and points to status, playbook and the function lookup
- [x] append-only pages rotate without editing or deleting entries
- [x] `tools/wiki.py lint` reports no broken links or status mismatches

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): tools/wiki.py is stdlib-only with a docstring and tests (tools/test_wiki.py, 4 tests); rotation verified lossless (4,772 log entries before and after; every notes section in exactly one file); lint 0 broken links, 0 status mismatches; clean 27/27 merge. Done.
