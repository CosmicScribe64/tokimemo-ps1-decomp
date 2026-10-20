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

### Second verified match

`main:make_color_bar16`: literal default `funcdiff --resolve --version default` reports `MATCH (linked function bytes equal target (1540 bytes))`. Plain signed-channel interpolation through the existing `Pix2Out` replaces the 1,540-byte asm body; no FAKE or shared declarations. Typed signed channels, direct half-count loop expressions, alpha field order, and declaration order close the raw m2c draft. Details and discarded paths are in [[matching-notes]]. Header check passes; final prototype and 27-unit hash verification pending.

Parent source review accepted the interpolation body with no findings but found the missing shared declaration. The canonical symbol is `make_color_bar16 = 0x8004BBA8` (`config/symbol_addrs_obin.txt`), with no prototype or aliased declaration in `main_api.h`. Added its exact definition prototype there with parent approval; callers are NAME_ENT `80139740.c` and SHUGAKU `80135430.c`. Pre-header explicit 27/27 hashes passed; rebuilding and verifying callers after the shared declaration.

Final pre-header prototype branch check passed with `0 new disagreement(s), 15 known`; no new prototype disagreement was introduced. All 27 image hashes were explicitly recomputed as expected. Shared declaration validation is running after that review fix.

Post-declaration verification completed: fresh affected rebuild `ninja -j 8` exited zero, then `funcdiff --resolve --version default main:make_color_bar16` again proved all 1,540 linked bytes. Explicit SHA1 recomputation passed 27/27 images, including both caller units NAME_ENT and SHUGAKU. Header check passed; prototype branch check reports zero new disagreements (15 known). Parent source review had no body findings, and its only declaration finding is resolved by the canonical prototype.

### Bounded post-match repairs

Archived four readable, non-production drafts under `drafts/T-24501/main/`: CD dispatcher `func_800455C4` (resolved edit cost 439), `palette_load_vram` (clean 419, padded experiment 394), `make_color_bar` (330), and `_sprite_set_light_effect1` (374). Additional typed menu `func_80076000` remains at 214 after 11 bounded view compiles; existing `func_80054E6C` guard remains at 84. Exact hypotheses and remaining structural misses are in [[matching-notes]]. Production source and headers are unchanged during this phase; this diagnostic checkpoint needs no executable rebuild. Continue with smaller untried helpers rather than repeating documented row grids.

### Third match: palette helper

`menu_bar_color` (84 bytes) now passes literal default linked proof. `D_800E3680[16] = {0}` owns exactly 0x40 initialized data bytes through a YAML island at 0xA2E80; previous raw range and next owner `80059710` retain their boundaries. A marked redundant nibble mask restores the last five register differences. Parent approved the missing exact canonical prototype in `main_api.h`; post-header full build, 27 hashes and branch checks are running. Parent code review is pending. Source/config details and boundary proof are in [[matching-notes]].

Post-header verification completed: fresh affected 520-step build exited zero; explicit SHA1 recomputation passed all 27 executable images, including all four call sites' units. Literal default proof again reports all 84 linked bytes equal target. Header check passes; prototype branch check reports zero new disagreements (15 known). Parent reviewed the exact source/config/header diff and linked data boundaries against CODING_STANDARDS.md, with no blocking findings. Removed the cosmetic blank after the FAKE comment; the final source was recompiled during literal proof. Focused match is ready for integration; broader ticket remains In Progress.

Bounded status-display reconstruction `func_8004443C` remains asm. Clean array/RECT scratch cost 109; alternate declaration order 92 but wrong stack homes. Setup-order, scalar-gap and grouped-record hypotheses do not fix broad register/home differences. Clean draft archived with exact requirements in `drafts/T-24501/main/func_8004443C.c`; no production/header change. See [[matching-notes]].

Bounded ring alias follow-up `func_80075C84`: local struct views over keys, position or both leave resolved cost 154 and missing reload unchanged. Integer-base and position-relative diagnostic views are worse (204/191). No promotion or shared edit; details in [[matching-notes]]. Same data-ownership history scan finds `InitMouse` already fixed and `sndisp` still structurally blocked.
