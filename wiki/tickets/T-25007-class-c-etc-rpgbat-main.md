---
id: T-25007
title: "Class C: ETC func_80146D4C, RPG_BAT func_8014BFE4, main func_8005938C"
status: Done
assignee: agent c-m6
created: 2026-10-20
updated: 2026-10-20
links: [[[plan-remaining]], [[playbook]], [[matching-notes]], src/ovl/ETC/80146400.c, src/ovl/RPG_BAT/8014ADD0.c, src/main/80058D20.c]
---

## Goal

Match ETC `func_80146D4C` (1076 B), RPG_BAT `func_8014BFE4` (892 B) and main `func_8005938C` (764 B), class C of [[plan-remaining]]. Owned files: `src/ovl/ETC/80146400.c`, `src/ovl/RPG_BAT/8014ADD0.c`, `src/main/80058D20.c`.

## Acceptance criteria

- [x] Each function matched in plain C, or guarded under about 30 rows with a NON_MATCHING note, or INCLUDE_ASM with the draft in `drafts/T-25007/`
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch
- [x] `## T-25007` section in [[matching-notes]]

## Notes

Branch c-m6 from w2-good.

## Comments

### 2026-10-20 (agent c-m6)

- ETC `func_80146D4C`: guarded, 12 rows (from 104). Levers: one `ret` variable for ret/score/grade, `GameState *g`, a GsByteNibbles view for the flag byte, an empty `if (g->unk_041) {}` (permuter) that makes every unk_041 read `a3`, and the total through `score`. Left: last block's ugen temporaries one lower.
- RPG_BAT `func_8014BFE4`: INCLUDE_ASM, best is still the T-17110 hand draft (128 rows); notes in `drafts/T-25007/RPG_BAT/func_8014BFE4/NOTES.md`.
- main `func_8005938C`: INCLUDE_ASM, best is still the T-17017 closer draft (124 rows, MATRIX-copy ring slots); notes in `drafts/T-25007/main/func_8005938C/NOTES.md`.
- Clean build 27/27; check_headers, migrate_globals --check, sync_protos --check-branch pass. Notes: [[matching-notes]] T-25007; playbook recipe "Promoted global left uncoloured".
- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): ETC func_80146D4C guard 12 rows; RPG_BAT func_8014BFE4 and main func_8005938C drafts and notes. FAKE comments ticketed; no inline asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Done.
