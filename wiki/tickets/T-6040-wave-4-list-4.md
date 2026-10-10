---
id: T-6040
title: Wave 4: list 4
status: In Progress
assignee: agent (worktree w4-4)
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[game-state]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave-4 work list 4 (185 functions, 41996 bytes in 36 C files: main 80059B40, 8005A0B0, 80062CD0, 80079B10, 8007C030, 80085E30 and overlays BUNKAKEN, BUNKA_SD, DATE, DATE2, EN_NICHI, ETC, EVENT, GEKO, GYOZI, NAME_ENT, RPG_BAT, SHOUGATU, TACO, TT, VALEN), best first, then continue from the byte queue inside the same files.

## Acceptance criteria

- [ ] functions matched, each verified with `funcdiff.py --resolve` and a sha1-OK build
- [ ] blocked T-0018 shapes reverted to INCLUDE_ASM and recorded in `wiki/data/t0018-cases.md`
- [ ] `sync_protos.py --check-branch` and `migrate_globals.py --check` clean
- [ ] clean rebuild 27/27 OK, inline code review recorded below

## Notes

## Comments
