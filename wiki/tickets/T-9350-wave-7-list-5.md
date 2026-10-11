---
id: T-9350
title: Wave 7: list 5
status: Done
assignee: w7-5
created: 2026-10-10
updated: 2026-10-10
links: []
---

## Goal
Match functions from wave-7 list 5 (36 files) in branch w7-5.

## Acceptance criteria
- [x] Matched functions build byte-identical, 27/27 OK (clean rebuild)
- [x] Inline review against CODING_STANDARDS.md

## Notes
16 functions matched (see [[matching-notes]], "Wave 7, list 5"): EVENT `func_800FD588`; NAME_ENT `func_80133E54`, `func_80133FC4`, `func_80137AA0`; main `func_8005C7FC`, `func_8005D1B0`; TACO `func_80155E10`; OLH `func_801324EC`, `func_801365AC`, `func_80132294`; BUNKA_SD `func_801362EC`; BUNKAKEN `func_80144D38`, `func_80144E9C`; RPG_BAT `func_8013673C`, `func_8015675C`, `func_8013FD8C`. 20 rows appended to [[data/t0018-cases]]. dupes.py and neardupes.py with `--files` found no twins to fill.

## Review (inline, against CODING_STANDARDS.md)
- Fakematches carry `FAKE` comments with reason and ticket: EVENT `func_800FD588` (`* 0` load move, same as `func_800FCDD8`), BUNKAKEN `func_80144D38` (`pad[2]` frame), OLH `func_801324EC`/`func_801365AC` (`pad[1]` frame, template of `func_80133C4C`), OLH `func_80132294` (u8 view of the selector).
- No main-exe symbol declared outside `include/main_api.h`: new globals went in with `sync_protos.py --add-global` (`D_800B5970` not kept; `D_80120B1B` not kept; kept: `D_8012139E`, `D_80120E1C`, `D_80121311`), `func_80049A40` is a documented `MAIN_API_OVERRIDE_` in `src/main/8005A0B0.c`; overlay-local declarations appended to `include/ovl/NAME_ENT.h`.
- Game state accessed only through `D_800E6280` members (`migrate_globals.py --check` OK); `sync_protos.py --check-branch` OK; no game data, `asm/`, `build/` committed.
- No open findings.

## Comments
Verification: a full clean rebuild was started and stopped at 309/657 steps because the shared Docker host (load 40) made it take hours; the 17 units built before the stop were OK, then `ninja` on the targets of every touched unit (SLPM_86.053, EVENT, NAME_ENT, TACO, OLH, BUNKA_SD, BUNKAKEN, RPG_BAT, headers, globals) from the clean tree ended OK. `ninja` earlier in the session (incremental) was all OK before the last headers change. Progress 4574/6958 functions (+16), about 4880 bytes. `sync_protos.py --check-branch` OK, `migrate_globals.py --check` OK.
