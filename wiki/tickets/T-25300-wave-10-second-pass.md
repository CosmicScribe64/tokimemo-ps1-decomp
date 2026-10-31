---
id: T-25300
title: Wave 10 - second pass on every function left after waves 8 and 9
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-24
links: `[tickets/T-25100-wave-8-finish`, [[tickets/T-25200-wave-9-neighbor]], [[status]], [[playbook]]]
---

## Goal

User request "Finish the game decomp" (continued). Every game function still INCLUDE_ASM after its wave 8 or wave 9 pass gets a second deep Opus pass that starts from the first pass's NOTES.md, best body and next hypothesis, plus a third fresh-eyes pass when the guard is 25 rows or fewer. Replaces the wave 9 neighbor run, which stopped on 2026-10-23 when one job-picker agent returned no structured result.

## Acceptance criteria

- [ ] Every file below has run (or was fully matched before its turn) and its ticket is Done.
- [ ] Every match passed the literal default-C checks, 27/27 sha1, header/global/prototype checks.

## Notes

GEKO stays with diman8 (issue #65). Files start as soon as no wave 8 or wave 9 worktree holds them (local `.claude/orchestrator/bin/w10_next.py`, under `state/git.lock`). Four workers, usage gate as in wave 8. Every agent call is wrapped so one bad result cannot stop the run.

- [[tickets/T-25301-w10-NAME_ENT-80139740|T-25301]] `src/ovl/NAME_ENT/80139740.c`: func_8013AF78 (first pass T-25153)
- [[tickets/T-25302-w10-main-80049FF0|T-25302]] `src/main/80049FF0.c`: func_8004A1A0 (first pass T-25122)
- [[tickets/T-25303-w10-main-800563F0|T-25303]] `src/main/800563F0.c`: strCallback (first pass T-25118)
- [[tickets/T-25304-w10-main-8004C3F0|T-25304]] `src/main/8004C3F0.c`: mod_trans_vram (first pass T-25104)
- [[tickets/T-25305-w10-SHOUGATU-8013E330|T-25305]] `src/ovl/SHOUGATU/8013E330.c`: func_8013EA98 (first pass T-25143)
- [[tickets/T-25306-w10-BUNKA_SD-80134540|T-25306]] `src/ovl/BUNKA_SD/80134540.c`: func_80134D94 (first pass T-25142)
- [[tickets/T-25307-w10-main-8004F870|T-25307]] `src/main/8004F870.c`: func_80052F68 (first pass T-25101)
- [[tickets/T-25308-w10-RPG_BAT-80154430|T-25308]] `src/ovl/RPG_BAT/80154430.c`: func_80154DD8 (first pass T-25114)
- [[tickets/T-25309-w10-TAIIKU-80133C80|T-25309]] `src/ovl/TAIIKU/80133C80.c`: func_80137784 (first pass T-25106)
- [[tickets/T-25310-w10-main-80053650|T-25310]] `src/main/80053650.c`: func_80055AFC (first pass T-25105)
- [[tickets/T-25311-w10-ENDING-80133C10|T-25311]] `src/ovl/ENDING/80133C10.c`: func_80134270 (first pass T-25135)
- [[tickets/T-25312-w10-TACO-801447D0|T-25312]] `src/ovl/TACO/801447D0.c`: func_80145650 (first pass T-25136)
- [[tickets/T-25313-w10-main-80043510|T-25313]] `src/main/80043510.c`: palette_load_vram (first pass T-25134)
- [[tickets/T-25314-w10-ETC-8013FF80|T-25314]] `src/ovl/ETC/8013FF80.c`: func_8013FFAC (first pass T-25128)
- [[tickets/T-25315-w10-SHOUGATU-80134890|T-25315]] `src/ovl/SHOUGATU/80134890.c`: func_801350F8 (first pass T-25132)
- [[tickets/T-25316-w10-TEL-8013CCD0|T-25316]] `src/ovl/TEL/8013CCD0.c`: func_8013D88C (first pass T-25133)
- [[tickets/T-25317-w10-main-80047550|T-25317]] `src/main/80047550.c`: func_800476C0 (first pass T-25116)
- [[tickets/T-25318-w10-main-8006CB30|T-25318]] `src/main/8006CB30.c`: func_8007132C (first pass T-25125)
- [[tickets/T-25319-w10-NAME_ENT-80142A80|T-25319]] `src/ovl/NAME_ENT/80142A80.c`: func_80143580 (first pass T-25131)
- [[tickets/T-25320-w10-main-800674B0|T-25320]] `src/main/800674B0.c`: func_80067870, func_8006B0C8 (first pass T-25103)

## Comments
- 2026-10-24 (orchestrator): wave 10 finished (run wf_e8f9963e-531): second passes matched strCallback, mod_trans_vram, func_8004A1A0, NAME_ENT func_8013AF78, RPG_BAT func_80154DD8, BUNKA_SD func_80134D94, func_80052F68, ENDING func_80134270, func_80055AFC, ETC func_8013FFAC and main func_8007132C; the rest went to wave 11, the stubborn-function attack and the finish gate. Phase 1 complete.
