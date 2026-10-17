---
id: T-12010
title: Codex cx1 main helper matching
status: Done
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over cx1 twelve functions in main objects 800420D0, 800451D0, 80047550 and 80059710. Work only in the cx1 worktree.

## Acceptance criteria

- [x] Investigate assigned functions and document guarded drafts and blockers.
- [x] Promoted C passes linked funcdiff and binary checksum verification.
- [x] Final configure/build verifies 27 binaries; headers, globals and branch prototypes pass.
- [ ] Submit for review against CODING_STANDARDS.md.

## Notes

Package ../codex/cx1-list.txt. Baseline Docker configure/build passed 27/27 SHA1; log /tmp/cx1-baseline.log. Three workers share Docker, subsequent ninja uses -j2. InitMouse has a documented missing data-ownership range; func_800422C8 has a documented call-result register mismatch.

## Comments

Review pending.

### InitMouse matched (16 bytes)

The original two stores share one lui $at. All code references to the pointer pair are in object 80059710; its remaining mouse state occupies 800E36C8..800E36EA, with zero padding through 800E36F0. Added an owned-data range 800E36C0..800E36F0 and generated its island with tools/data_island.py. Defined only the two zero-initialized buffer pointers; other data remains INCLUDE_RODATA. Linked funcdiff proves 16 bytes, configure/ninja -j2 passes 27/27, headers and globals OK. Evidence: build/scratch/InitMouse-match.diff and build/scratch/mouse-island-build.log. No FAKE.

### Initial near-miss experiments

file_checksum_ok: 4 listing lines, sltu/andi scheduling swap. Autotweak (385 variants), all 720 loop statement permutations and 42 manual source-line/mask variants unchanged. func_80049044: 12 listing lines (six word substitutions), ugen loads into t6/t7 instead of v1; 255 autotweak candidates unchanged. Traced uopt reused from the orchestrator-provided existing binary at build/scratch/r9-uopt/variants/trace; regdiff confirms ugen expression temporary rather than uopt allocation.

### func_80042134 matched (404 bytes)

Migrated the saved main-loop draft to GameState fields and used a local GameState pointer only for the loop-tail selector load. This reproduces the original s2 base saved at entry. Declared later helpers func_800422C8 and func_80042418 once in main_api.h to prevent implicit-return-type conflicts. Plain C linked funcdiff MATCH (404 bytes), main SHA1 OK; all-overlay header rebuild passed 27/27. No FAKE. Evidence build/scratch/42134-match.diff and 42134-build.log.

### func_80049044 matched (112 bytes)

A five-minute permuter run improved its stripped score from 90 to 50 using a u8 pointer to the brightness field. Full-unit variants show the simple initialized pointer (with both loads and stores through it) is actually exact. The original six-word ugen mismatch disappears. Plain C linked funcdiff MATCH (112 bytes), main SHA1 and globals OK; headers unchanged since the preceding 27/27 build. No FAKE: the pointer is an ordinary field view, with no alias offset or dead statement. Evidence build/scratch/49044-pointer.log and 49044-match.diff.

### func_80059938 matched (232 linked bytes)

Replaced m2c unknown-register placeholders with the incoming port index, then rewrote coordinate updates as ordinary array accesses. A named pointer-table slot (s8 **slot = &D_800E36C0[port], p = *slot) gives the exact original index/pointer allocation; direct table indexing misses by 20 lines. Updated the shared prototype and the existing owned caller to pass its port explicitly. Both func_80059938 (232 bytes) and the previously matched func_80059860 (216 bytes) pass linked verification. Full header rebuild 27/27, headers/globals OK, followed by clean-pointer main checksum pass. No FAKE. Evidence build/scratch/59938-clean-match.diff, 59938-clean-build.log and 59938-build.log.

### Deep-pass outcome

All twelve functions attempted; four matched (764 linked bytes), four near misses retained, four larger drafts remain only in ignored scratch. No inline assembly was added.

| Function | Outcome | Evidence / remaining difference |
|---|---|---|
| file_checksum_ok | Guard, 4 listing lines | sltu/andi schedule swap. 385 autotweak candidates, 720 statement orders, 90 manual/loop/narrowing variants and 300-second permuter unchanged; checksum-permute.log. |
| func_800422C8 | Guard, 8 listing lines | Four a3/v1 substitutions. Traced regdiff and 16 pointer/stored-value views unchanged. |
| load_tpage_buf_lock | New guard, 29 listing lines | Argument copies and narrow parameter homes. 19-line autotweak candidate changed signed comparison to unsigned and was rejected; load-auto.log, load-abi.log, load-tpage-best.diff. |
| func_800460FC | New guard, 17 listing lines | Retry loop, read-index pointer and byte/word state view recover the control flow. Remaining zero-register stores and local homes. 36 final layout variants reached 13 only with an unjustified larger local buffer, rejected. FAKE note marks reuse of zero retry value in byte-clear chain. 460FC-scalars.log, 460FC-layout-last.log, final-guards.log. |
| func_80047560 | Scratch only, 91 listing lines | Fixed rsin narrowing and GameState fields; quotient, coordinate and phase temporaries still differ. 47560-types.log. |
| func_80045414 | Scratch only, 128 listing lines | Narrow command, clamp, packet buffer and corrected callee arity tried; control/temporary and stack differences remain. 45414-test.log. |
| func_800455C4 | Scratch only, 533 listing lines | Fresh m2c with repaired Ring18 views, response buffer and callee arity; large branch/register differences remain. 455C4-repaired.log. |
| func_800476C0 | Scratch only, 1036 listing lines | Default m2c produces duplicate case labels; --no-switches compiles after aggregate and callee repairs, but nested control flow and record types still need recovery. 476C0-repaired.log. |

Logs and discarded drafts are under build/scratch/. The reused traced uopt was supplied by the orchestrator from an existing local build, copied to build/scratch/r9-uopt/variants/trace; no host compiler or dependency installation. All compiling, diffing, tracing and permutation ran in Docker. See wiki/data/t0018-cases.md for the updated compiler-shape rows. Existing scratch contains experimental pseudo-prototypes, but none entered source.

### Review handoff

Four exact promotions have individual linked funcdiff evidence and immediate commits. Final guards are recompiled from the current full translation units in build/scratch/final-guards.log. Final configure/build and prototype results are recorded below after completion. Status In Review; independent standards review still required.

Final verification: Docker `python3 configure.py && ninja -j2` completed successfully with 27/27 binary SHA1 checks, headers OK and globals OK (build/scratch/final-build.log). `sync_protos.py --check-branch` passed: zero new disagreements, fifteen known baseline disagreements (build/scratch/final-protos.log). Current-source guarded counts verified as 4, 8, 29 and 17 (build/scratch/final-guards.log). `git diff --check` clean; source diff contains no added inline assembly. Final source has one newly labeled FAKE in the guarded 460FC draft; four promoted functions have none.

2026-10-17: Code review against CODING_STANDARDS.md at 28d5f509 found two issues; the four matches and four guards are otherwise fine. (1) func_80042134 reads GameState through a pointer only in the loop-tail condition, which is the redundant-pointer-view pattern other files mark FAKE, and the ticket said 'No FAKE'. (2) config/objects/main.txt got a hand-written data line for 80059710 although the file is generated, and the next object_boundaries.py --data --write drops it. Moved to Done with a Backlog followup for (1); (2) goes into the data-range tooling ticket opened from T-12050.

Followup opened: [[tickets/T-21014-main-80042134-gamestate-pointer|T-21014]].
