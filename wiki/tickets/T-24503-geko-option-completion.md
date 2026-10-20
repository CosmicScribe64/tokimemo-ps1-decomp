---
id: T-24503
title: Match GEKO and OPTION remaining functions
status: In Progress
assignee: codex local GEKO OPTION
created: 2026-10-20
updated: 2026-10-20
links: ["[[matching-notes]]", "[[playbook]]"]
---

## Goal

Complete remaining GEKO and OPTION matching work during the contributor's pause. Start with GEKO func_80133024 and func_80134808 close guards; record precise handoff results.

## Acceptance criteria

- [ ] Literal default C matches verified in Docker.
- [ ] Full 27 binary checksums and header/global/prototype checks pass.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Scope reassigned from ETC before source edits. Worktree /private/tmp/tokimemo-t24503 at 2c7293275; branch codex/t24503-etc retained. No GitHub messages or issue changes authorized.

## Comments

- 2026-10-20: Read index, status, playbook, standards and T-24302 histories. GEKO func_80133024 has five remaining rows; prior types, order and empty-test attempts noted.
- 2026-10-20: Docker setup reuses tokimemo-decomp-arm64 with the source checkout mounted read-only at /reference and its disc data linked locally; no bulk build/disc copies. viewsweep: func_80133024 cost10 unchanged (6 compiles), func_80134808 cost30 unchanged (10), func_80132B24 cost61 unchanged (2).
- 2026-10-20: func_80133024 focused tests improved cost10 to4 (five instruction rows to two): an empty table-element test before the true-arm call puts month in a3. regdiff finds equal-priority10 global day and local day, with the local allocated first; only day-address base a1 instead of a2 remains. Further empty-test combinations and first-use tests did not improve. Scratch candidates/scripts retained under ignored build/t24503-*.
- 2026-10-20: func_80134808 explicit `(s16)next; if(i) {}` loop tail reduces cost30 to18; spelling its second equality `bestState << 0 == (u8)state` reduces to14. Further focused comparisons in progress.
- 2026-10-20: GEKO func_80134808 promoted to default C; Docker funcdiff --resolve --version default reports MATCH, linked bytes equal target (432 bytes). Empty latch test and distinct equality wrappers fix narrowing/CSE; unsigned word view of D_80122D40 fixes compare order; final result mask fixes ring numbering. All matching controls marked FAKE. No header changes.
- 2026-10-20: GEKO func_80133308 candidate also reaches zero and is promoted; literal default funcdiff MATCH (656 bytes). Final verification and focused commit preparation in progress.
- 2026-10-20 verification: full Docker ninja and incremental rebuild pass. Explicit hashes of build/SLPM_86.053.bin and all 26 build/ovl/*.bin match the pinned configs (27/27). check_headers and migrate_globals --check pass; sync_protos --check-branch reports zero new disagreements, 15 known. A first manual checksum command used overlay source filenames from the wrong directory; corrected explicit built-path hashes all pass.
- 2026-10-20 parent review against CODING_STANDARDS.md and task scope: no blocking findings on func_80134808 or func_80133308; FAKE explanations sufficient, C89 and fallback removal checked. Requested whitespace correction retained the matching loop-tail source line. Ticket stays In Progress while remaining functions are investigated.
