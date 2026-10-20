---
id: T-24526
title: Reconstruct NAME_ENT introduction state-machine regions
status: In Progress
assignee: Codex Sol 6.1 medium
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-20008-large-name-introduction]]", "[[tickets/T-24515-name-ent]]"]
---

## Goal

Own NAME_ENT func_80143580 in src/ovl/NAME_ENT/80142A80.c, assigned by root. Reconstruct actual type/control/dataflow gaps by semantic regions using existing SDK/game aggregates; do not treat the835-row archive as all-register or run broad declaration/scheduling grids.

## Acceptance criteria

- [ ] Full-TU source controls identify and repair real opcode/type/control gaps; concise replayable checkpoint if incomplete.
- [ ] Literal default C/linked proof, all27 hashes and declaration/global/prototype guards for any promoted match.
- [ ] Standards review and synchronized ticket/board/log; production unchanged unless proven.

## Notes

History T-20008/T-24515 and full canonical source audited before implementation. Full-TU1078 words/835 rows against1077 target;79 opcode-shape hunks include moved instructions,313 register pairs and24 immediate-pair differences. Frame88/table64 versus target90/6C. Fresh semantic candidates: step7 explicit old-byte fade/increment/test; step5 s16 old_x truncates a decremented value that original passes as a full word. Palette-copy condition and nested-fill expression forms also need regional audit. Docker-only tooling, existing image, no shared header changes.

## Findings

- Step5 old_x must be a full word: target computes lh value minus1, stores its low half and passes the untruncated word to the next call. s32 old_x removes two real sign-extension instructions; full-TU1076 words/835 raw rows.
- Step7 explicit old-byte fade then increment/test removes the extra Boolean xori (1077 words). Raw1674 rows reflects the known equal-length positional scorer discontinuity, not a sudden semantic regression; register allocation remains different. Kept as a separate causal control.
- Step13 palette-copy exact endpoint index!=128 removes the extra sltu latch versus index<128. Combined with word old_x gives1075 words/820 raw rows.
- A meaningful depth local computed before index++ at the top of the three-record initialization loop naturally restores original frame0x90/table0x6C with no pad. Current depth-first source1075 words/864 raw rows; store/constant/selector allocation still broad. Private full-TU sources and comparisons live under build/T-24515/func_80143580/t24526-*; no production/header changes or literal match claimed.

## Checkpoint

Source-only body/declarations/NOTES/full-TU replay under drafts/T-24526/NAME_ENT/func_80143580. Isolated step7 old-byte local1074/883 does not fix color allocation; unsigned state/counter split1075/956 and direct global state flow1070/1003 both rejected. Current trace identifies separate selector conversion/index webs but does not explain original coalescing; no further width/global-state grid. Regional opcode/type/control findings and remaining gaps are recorded in packet NOTES. Root requested freeze then idle; this larger matching investigation remains open, with no production changes or new coverage.
