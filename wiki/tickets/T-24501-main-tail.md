---
id: T-24501
title: Match MAIN remaining functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[playbook]]", "[[matching-notes]]"]
---

## Goal

Match remaining MAIN functions with plausible C and preserve all 27 executable checksums.

## Acceptance criteria

- [ ] Literal default C proof for every promoted function.
- [ ] Full 27 executable checksums and header/prototype checks pass.
- [ ] Code review against CODING_STANDARDS.md before Done.

## Notes

Isolated worktree `/private/tmp/tokimemo-t24501`, baseline `2c7293275`. First target `func_8006C988` in `src/main/800674B0.c`: 11-row pointer/value ordering tail from T-24301.

## Comments

### 2026-10-20 investigation

- `func_8006C988`: guarded baseline reproduced; targeted shared-value lifetime, index-before-load, and source grouping did not beat the prior `s32 cur` 10-row tail. Production unchanged.
- `func_8006509C` (1,076 bytes): existing draft full-TU resolved edit cost 99 (`funcdiff.diff_score`, insert/delete 1, substitution 2); pointer/local order, typed index, and count hoist variants did not improve. Moving the second loop initializer before its entry test reduced cost to 95 but remains too far to guard.
- `func_800654D0` (1,072 bytes): clean reconstruction from the original assembly replaces the raw m2c unrolled loops with indexed loops over four `s16[16]` menu arrays and actors 80+/64+. Initial cost 216; typed pair indexing avoids CSE of `i * 2` into the source scale; scalar-after-arrays declaration puts arrays at original offsets. Further structural work in progress. Scratch files stay untracked.

### First verified match

`main:func_800654D0`: `funcdiff --resolve --version default` reports `MATCH (linked function bytes equal target (1072 bytes))`. `tools/check_headers.py` reports `headers OK`; `tools/sync_protos.py --check-branch` reports zero new disagreements and `branch check OK` (14 known baseline cases). Plain C replaces its INCLUDE_ASM. Two FAKE comments mark the preserved unused scalar and pre-call index zero that reproduce the original frame and saved index; details in [[matching-notes]]. Full 27-unit build pending before handoff.

Full verification completed: fresh `ninja -j 6` exited zero, with 27 checksum `OK` lines in `.scratch/t24501/full27.log`. Parent reviewed the 43-line source diff against CODING_STANDARDS.md and accepted the typed actor/pair-array views, loops, and both marked FAKE comments with no outstanding source finding. No shared headers or production compiler/tooling changes. Focused match commit is ready for integration while additional MAIN work remains In Progress.
