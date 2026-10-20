---
id: T-24600
title: Finish remaining game-code matching
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]"]
---

## Goal

Complete the remaining game-code matching requested by the user. SDK assembly remains excluded by project policy.

## Acceptance criteria

- [ ] All 6,952 game functions use verified default C.
- [ ] All 27 executable checksums and declaration guards pass in Docker.
- [ ] Changes pass the coding-standards review.

## Notes

Starting census: Docker progress reports 6,816/6,952 functions and 2,091,568/2,276,920 bytes. Existing conveyor ownership records reserve most remaining source files; initial experiments use isolated scratch files without modifying those sources.

## Comments

- 2026-10-20: Read the wiki, playbook, local orchestration instructions and coding standards. Docker access works with sandbox escalation. Investigate MAIN func_8006C988's join-value narrowing and pointer allocation in scratch first; historical grids and the s32-cur variant are already documented.
- 2026-10-20: Initial 25 explicit join-value/type/control-flow probes and 20 increment/base-lifetime probes found no match. Full-TU verification of the best candidate retains the same 69 instructions and register/scheduling mismatch. Traced uopt identifies unconstrained ranges 83 (induction pointer) and 84 (PRE-loaded byte) allocated in bit order; their priorities are 15.25 and 20, so priority-only changes do not explain the swap. Next probe changes address-expression construction, following the established PRE-seeding mechanism.
- 2026-10-20 checkpoint: 84 MAIN scratch probes in total and 29 GEKO func_80133024 probes produced no match. Evidence and limitations recorded in [[matching-notes#T-24600: record-scan allocation and GEKO day-load probes]]. Production source unchanged; completion acceptance criteria remain unmet. Ticket stays In Progress, not Done.
- 2026-10-20 continuation: User explicitly requires continued work until completion. Persistent goal activated. MAIN func_80047560 grouping probes improved isolated score but full-TU output still differs; no match claim. Switched to the latest T-24501 get_fnt reconstruction (reproduced full-TU cost 23, 258/259 words) to recover concrete pointer-local, frame and exit differences.
- 2026-10-20: T-24601 get_fnt is Done and committed as d7c7d1a97: +1,036 verified C bytes, 135 functions remain. Follow-up save-helper experiments separate frame padding from retained-pointer lifetimes; initial explicit pointer forms do not improve the guarded source. Production remains unchanged for that helper.
- 2026-10-20: T-24602 GEKO func_80144198 is Done: +216 verified C bytes. A date-result array closes the scalar live-range conflict; minimized initial stores/test are marked FAKE. Full build, fresh 27 hashes, independent standards/spec reviews pass. Current remaining count: 134 functions.
- 2026-10-20: User authorized fan-out. GEKO, NAME_ENT and TACO agents work disjoint source lanes while root continues MAIN. T-24603 TACO func_80153B38 adds 832 verified bytes; full build, fresh 27 hashes and both independent reviews pass. Three functions / 2,084 bytes completed in this continuation; 133 remain.
- 2026-10-20: T-24604 MAIN hizuke_init is Done: +1,048 verified bytes. Both unsigned entry reads and one joined source-line pair are minimized and marked FAKE; no pass/header changes. Full build, fresh 27 hashes, declaration guard and both independent reviews pass. This continuation has completed four functions / 3,132 bytes; 132 remain. Matching continues in four lanes.
- 2026-10-20: T-24605 GEKO func_801420E4 is Done: +1,256 verified bytes. Typed ranking reconstruction preserves signed sums, tie order, narrowing and initializer calls; minimized frame/allocation/scheduling controls marked FAKE. Full build, fresh27 hashes, declaration guards and both reviews pass. Five functions /4,388 bytes completed in this continuation;131 remain. Four lanes continue.
