---
id: T-0700
title: Overlay batch A: RENSYU, OMIMAI, VALEN, MASTER
status: In Progress   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal
Decompile functions in `src/ovl/RENSYU.c`, `OMIMAI.c`, `VALEN.c`, `MASTER.c` (IDO 5.3 + frame pass), smallest first, about 50 matches. Branch ovl-batch-a.

## Acceptance criteria
- [ ] About 50 functions matched, all 27 sha1 checks OK
- [ ] Hard patterns noted in [[matching-notes]] when new
- [ ] Code review gate passed

## Notes
Per-overlay headers in `include/ovl/<NAME>.h`. Main-exe symbols declared with old names where needed to link (list below, for rename at merge).

## Comments
