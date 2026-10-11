---
id: T-9370
title: Wave 7 list 7
status: Done
assignee: wave-7 agent 7
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-7 list 7 (36 files, 149 functions).

## Acceptance criteria
- [x] Matches build byte-identical, 27/27 OK (clean rebuild at the end)
- [x] Inline review against CODING_STANDARDS.md (see Comments)

## Notes
Result: 19 functions matched, 6,780 bytes (see [[matching-notes]] "Wave 7, list 7 (T-9370)"); 24 rows appended to [[data/t0018-cases]]; ninja 27/27 OK, headers OK, globals OK, `--check-branch` OK, progress 4577/6958. Method: m2c draft, `funcdiff.py --resolve`, then `permute.py` on every near miss (it found `*a = *b = 0`, `(P + idx)->f` and `^ 0`).

Fakematches added (all marked `FAKE` in the source):
- TACO `func_80148308`: write-back `D_800E6280.unk_1104.u = t;` before the call (store in the `jal` delay slot).
- TACO `func_80151F94` (`s32 pad[15]`), `func_80153068` (`s32 pad[15]`, `gap[1]`): unused arrays for the original's larger frame.
- RPG_BAT `func_80157CCC`: `func_8013E9A8(6) ^ 0` (permuter) for the switch-temporary copy.

## Comments
Inline review (CODING_STANDARDS.md, checklist section 12): every new declaration is in `include/main_api.h` (`sync_protos.py --add-global`) or `include/ovl/<NAME>.h` (appended); no duplicate main-exe prototype; fakematches carry `FAKE` with reason and ticket; strings are written as literals; no `INCLUDE_RODATA` used; no game data or `asm/` committed. No open findings.
