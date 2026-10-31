---
id: T-25400
title: Wave 11 - repeated passes until every game function matches
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-24
links: `[tickets/T-25300-wave-10-second-pass`, [[status]], [[playbook]]]
---

## Goal

User request (2026-10-23): reach 100% without waiting for the issue #65 claim to finish. Wave 11 runs next to wave 10 ([[tickets/T-25300-wave-10-second-pass]]). It works the GEKO functions now (diman8 was told on issue #65; their PRs stay welcome and whichever match lands first wins), then picks up each file wave 10 leaves unmatched as soon as wave 10 has merged it. Every file gets up to three wave 11 passes, each a deep Opus pass that starts from all earlier notes, plus a fresh-eyes pass when the guard is 25 rows or fewer.

## Acceptance criteria

- [ ] No game INCLUDE_ASM left, or each file left has used its three passes with notes naming the first divergence and the next hypothesis.
- [ ] Every match passed the literal default-C checks, 27/27 sha1, header/global/prototype checks.

## Notes

Job picker: local `.claude/orchestrator/bin/w11_next.py` (GEKO first, then fewest passes, then smallest; waits while wave 10 holds a file). Worktrees `w11-UNIT-FILE-pN`, one per pass. Three workers, usage gate as before, every agent call wrapped.

- [[tickets/T-25401-w11-GEKO-801323E0|T-25401]] `src/ovl/GEKO/801323E0.c`: func_80132B24, func_80133024
- [[tickets/T-25402-w11-GEKO-801338F0|T-25402]] `src/ovl/GEKO/801338F0.c`: func_80133C50
- [[tickets/T-25403-w11-GEKO-80142760|T-25403]] `src/ovl/GEKO/80142760.c`: func_80142C9C
- [[tickets/T-25404-w11-main-80047550|T-25404]] `src/main/80047550.c`: func_800476C0
- [[tickets/T-25405-w11-main-80053650|T-25405]] `src/main/80053650.c`: func_80055AFC
- [[tickets/T-25406-w11-main-800674B0|T-25406]] `src/main/800674B0.c`: func_80067870, func_8006B0C8
- [[tickets/T-25407-w11-main-8006CB30|T-25407]] `src/main/8006CB30.c`: func_8007132C
- [[tickets/T-25408-w11-ENDING-80133C10|T-25408]] `src/ovl/ENDING/80133C10.c`: func_80134270
- [[tickets/T-25409-w11-ETC-8013FF80|T-25409]] `src/ovl/ETC/8013FF80.c`: func_8013FFAC
- [[tickets/T-25410-w11-NAME_ENT-80142A80|T-25410]] `src/ovl/NAME_ENT/80142A80.c`: func_80143580
- [[tickets/T-25411-w11-SHOUGATU-8013E330|T-25411]] `src/ovl/SHOUGATU/8013E330.c`: func_8013EA98
- [[tickets/T-25412-w11-TAIIKU-80133C80|T-25412]] `src/ovl/TAIIKU/80133C80.c`: func_80137784

## Comments
- 2026-10-24 (orchestrator): wave 11 stopped when the finish gate took over; it matched GEKO func_80133024, func_80142C9C (pass 2) and main func_80067870 (closer). Phase 1 complete.
