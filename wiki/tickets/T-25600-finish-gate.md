---
id: T-25600
title: Finish gate - loop the last functions until 100%
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-24
links: [[[tickets/T-25500-stubborn-functions]], [[tickets/T-25400-wave-11-until-matched]], [[tickets/T-25300-wave-10-second-pass]], [[status]], [[playbook]]]
---

## Goal

User decision (2026-10-23): replace the separate wave conveyors with one gate that keeps every remaining game function under work until none is left in assembly. Waves 10 and 11 start no new work (their running passes finish and merge); the stubborn-function workflow finishes SHOUGATU. The gate then takes each function as its file frees up and gives it the treatment that matched TAIIKU func_80137784: independent lenses in separate worktrees (clean-room, compiler trace, block-by-block structure for big functions, long search, toolchain model under CODING_STANDARDS.md 7a), a synthesis pass, two adversarial reviews, a fix pass for review findings, then merge. A function still unmatched goes around again with the last round's findings, up to three gate rounds.

## Acceptance criteria

- [ ] No game INCLUDE_ASM left; clean build 27/27; `tools/progress.py` reports 6,952/6,952.
- [ ] Every match passed both reviews and the literal default-C checks; any tools/ or config/ change reviewed by the orchestrator and the user before merge.
- [ ] Anything left after three rounds is reported with its blocker.

## Notes

Local picker `.claude/orchestrator/bin/gate_next.py` (fewest rounds, largest first; waits while a wave worktree or a hold holds the file). Two functions at a time, usage gate as before, every agent call wrapped. Nine functions at the start:

- [[tickets/T-25601-gate-ETC-func_8013FFAC|T-25601]] ETC:func_8013FFAC (545 words, `src/ovl/ETC/8013FF80.c`)
- [[tickets/T-25602-gate-GEKO-func_80133024|T-25602]] GEKO:func_80133024 (71 words, `src/ovl/GEKO/801323E0.c`)
- [[tickets/T-25603-gate-GEKO-func_80142C9C|T-25603]] GEKO:func_80142C9C (926 words, `src/ovl/GEKO/80142760.c`)
- [[tickets/T-25604-gate-NAME_ENT-func_80143580|T-25604]] NAME_ENT:func_80143580 (1098 words, `src/ovl/NAME_ENT/80142A80.c`)
- [[tickets/T-25605-gate-SHOUGATU-func_8013EA98|T-25605]] SHOUGATU:func_8013EA98 (231 words, `src/ovl/SHOUGATU/8013E330.c`)
- [[tickets/T-25606-gate-main-func_800476C0|T-25606]] main:func_800476C0 (850 words, `src/main/80047550.c`)
- [[tickets/T-25607-gate-main-func_80067870|T-25607]] main:func_80067870 (239 words, `src/main/800674B0.c`)
- [[tickets/T-25608-gate-main-func_8006B0C8|T-25608]] main:func_8006B0C8 (318 words, `src/main/800674B0.c`)
- [[tickets/T-25609-gate-main-func_8007132C|T-25609]] main:func_8007132C (1074 words, `src/main/8006CB30.c`)

## Comments
- 2026-10-24 (orchestrator): phase 1 complete: 6,952/6,952 game functions, 2,276,920/2,276,920 bytes. The gate and its fresh-start runs matched main func_800476C0, NAME_ENT func_80143580 and main func_8006B0C8 (with the T-25608 ugen_pass rule; the IDO 4.1 rule follow-up is T-26000).
