---
id: T-20900
title: "Cross-unit shared-type unification"
status: In Progress
assignee: claude-opus (ty-shared)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]"]
---

## Goal

The per-unit types-first passes (T-16550, T-16560, T-17000 .. T-18800) could only add to `include/game.h`, so the
shared tables have many parallel per-overlay views. Start with the 0x44-byte display-record table `D_8011ECD0`
(and the pools of the same record at `D_800E9620` in EVENT and `D_80129F40` in GYOZI): find the views of the same
table, define one shared record type in `include/game.h` from the union of all users' accesses (widths from the asm
loads and stores), and convert the users where the build stays byte-identical. Where a field's width or signedness
really differs between users, keep a documented view. Then do the same for other tables with three or more
per-overlay duplicate types. Measure the effect by rescoring the drafts of unmatched functions that use the
unified tables (`tools/redraft.py`).

## Acceptance criteria

- [ ] One shared 0x44 record type in `include/game.h`; per-overlay duplicates converted or documented as views.
- [ ] Other tables with three or more duplicate types handled the same way.
- [ ] Every matched function byte-identical (clean build 27/27); `check_headers`, `migrate_globals --check`,
      `sync_protos --check-branch` pass.
- [ ] Draft score changes measured and recorded.
- [ ] [[data-types]] and the family pages document the types.

## Notes

Off limits: `src/ovl/TT/*` (contributor, issue #2) and main `dialogue_render` in `src/main/8007C030.c` (issue #45).

## Comments
