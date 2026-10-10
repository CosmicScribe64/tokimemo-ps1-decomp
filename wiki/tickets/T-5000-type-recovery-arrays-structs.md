---
id: T-5000
title: "Type recovery: arrays and structs from access patterns"
status: Done
assignee: r3-types
created: 2026-10-10
updated: 2026-10-10
links: ["[[data-types]]", "[[matching-notes]]", "[[decompile-workflow]]", "tools/type_recovery.py", "tools/test_type_recovery.py", "config/symbol_addrs_types.txt"]
---

## Goal

About 110 `FAKE` markers in `src/`; many reach later globals through the first symbol (`(&D_X)[n]`, `*(&D_X + k)`) so that IDO keeps a later load after an earlier store. The D_800E643E case (commit 2e84b1c) showed that the real array declaration gives the same bytes without the trick. Hypothesis: many neighbouring `D_` globals are fields of one struct or array, and IDO keeps source order for accesses through one base symbol.

## Acceptance criteria

- [x] `tools/type_recovery.py` clusters globals from the original asm of all functions (shared %hi/%lo base, index stride, loops, globals accessed together) and proposes struct/array types with confidence; unit tests on synthetic asm.
- [x] Every "first symbol" FAKE and every load-hoist note checked against the proposals; high-confidence ones converted to real aggregate declarations, FAKE comments removed, bytes unchanged.
- [x] 20 load-hoist blocked functions retried with the recovered types; count reported.
- [x] `wiki/data-types.md`, CODING_STANDARDS rule, `wiki/decompile-workflow.md` step.
- [x] Clean build 27/27 OK, headers OK.

## Notes

Results in [[data-types]]. `tools/type_recovery.py`: 1724 proposals (654 high, 559 medium, 511 low); high ones absorb 11,593 symbols. 33 first-symbol `FAKE`s removed (117 to 84). 20 load-hoist functions retried: 7 match with the recovered types, 3 more only through a base whose type is not recovered (left as asm), 5 have the load order fixed but another gap, 5 are not one-object cases. Clean build 27/27, headers OK, `ninja progress` 3438 to 3445 of 6958. Files touched outside `src/`/`include/`: `configure.py` (main_names.ld also reads `config/symbol_addrs_types.txt`), CODING_STANDARDS section 8, wiki.

## Comments

2026-10-10 inline review against CODING_STANDARDS.md (r3-types): matches verified by the clean build (`rm -rf asm build`, configure, ninja: 27/27 sha1 OK, `headers OK`); no NON_MATCHING added; C89 only (one K&R definition was tried for `func_80148924` and not kept); new types documented with offsets and sizes, fields `unk_XX`/`pad`; main-exe types and declarations only in `include/main_api.h`, the RPG_BAT type in its overlay header; the one remaining FAKE in `func_80153E90` has an updated comment; no new FAKE; tool is Python 3, Docker-run, with a docstring and 13 unit tests on synthetic asm; no game data or asm staged; commits carry T-5000. Findings fixed during review: `Rec24` base moved from `D_801217AC` to `D_801217D0` (ENDING mismatch), `RpgRec18` as a struct (array mismatch). No open findings.
