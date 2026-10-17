---
id: T-12090
title: Match cx9 BUNKA_SD functions
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: []
---

## Goal

Deep matching pass over ten assigned BUNKA_SD functions, prioritizing verified byte gains.

## Acceptance criteria

- [x] Exact linked and overlay checksum proof for every match.
- [x] Retain only measured guards within about thirty differing rows.
- [x] Full configure/build and prototype verification.
- [ ] Independent standards review before Done.

## Notes

Owned files: src/ovl/BUNKA_SD/80132000.c, 80134260.c and 801388D0.c. No main or other worktree edits. Deadline 21:45 UTC.

## Comments

2026-10-15: Read wiki index, project/local instructions, coding standards, workflow and register cheat sheet. Searched historical evidence for all ten functions. Baseline Docker configure and ninja -j2 pass 27/27 SHA1 checks, headers and globals. Prior small fade loops have known allocation gaps; prioritize the 1164/1604-byte functions and their setup family after checking the old score-zero dispatcher.

### func_80132428: MATCH, 448 text bytes

Canonical GameState selector/timer access and six missing overlay forward declarations make the old score-zero dispatcher compile in its complete translation unit. Plain C, no FAKE. Exact production linked comparison and BUNKA_SD SHA1 pass; headers/globals pass. Worklist size512 includes the64-byte jump table.

### func_80132FF0: MATCH, 532 text bytes

Restored explicit case4/default from the five-entry jump table; canonical player-name arrays and strcat declaration replace old draft aliases. Full linked text MATCH and BUNKA_SD SHA1 pass, with headers/globals OK. FAKE: 24-byte name-buffer capacity was chosen to reproduce observed stack homes; the original source capacity is unknown. Worklist552 includes20-byte jump table. Cumulative980 matched text bytes plus84 table bytes. Added overlay prototypes and typed display-data declarations used by ongoing larger-family reconstruction.

### func_80132940: MATCH, 1144 text bytes

Reused the palette/setup loop index, restored case4/default from the five-entry jump table, and represented the intermediate frame number as u16 before subtracting2 into the record halfword. The narrow unsigned temporary preserves the original arithmetic/update order without extra instructions. Cleaned raw offsets to DisplayActor44 and canonical calls. No FAKE. Exact production linked comparison and BUNKA_SD checksum pass, with headers/globals OK. Cumulative2124 text bytes plus104 table bytes.

### Larger-target scheduling diagnostic

func_80138D48 reaches6 rows with typed DisplayActor44 access and the texture assignment before counter/index updates; all238 instructions otherwise agree. A private copy of frame_pass applies the unchanged transform, then invokes native as1 with -R -xbbdbg8. Stock and diagnostic full object files compare byte-identical, and the captured native log contains8958 lines including the reached loop DAG. Direct -Wa flags failed because frame_pass interpreted the numeric8 argument as another input; no production tool was edited. At the remaining discrepancy, loop-bound constant6, index halfword store and -120 materialization all have two-cycle priorities; current as1 picks original emission order. Bounded line grouping, condition width and simple loop forms did not yet close it. Automated wider stores improved raw scores but changed semantics and were rejected.

### func_80138D48: MATCH, 952 bytes

Typed DisplayActor44 records, a32-bit frame induction variable and unsigned byte stores repair the saved draft. Assigning the image metadata before the counter/index updates fixes the large scheduling gap; putting the index increment in the for clause fixes the final constant scheduling. Keep i=2 before frame=15, outside the for initializer, to preserve load order. Removed unused m2c locals and raw offsets. No FAKE. Exact linked comparison and BUNKA_SD checksum pass; headers/globals OK. Cumulative3076 verified text bytes plus104 jump-table bytes.

### func_80133204: MATCH, 828 text bytes

Restored the fifth switch entry, replaced the shared m2c goto with separate if/else assignments, and used typed word-table indexing for voice records. Natural string-table indexing preserves the match after removing the synthetic byte-offset temporary. Exact production linked comparison and BUNKA_SD checksum pass, headers/globals OK; no FAKE. Cumulative3904 text bytes plus124 table bytes (worklist848 includes20 rodata bytes; linked text includes8 trailing padding bytes).

### func_801388D0: MATCH, 268 bytes

A bounded direct transfer from neighboring matched state machines: s32 no-return keeps the byte-assignment result live, plain +=1 uses the canonical byte conversion, and removing the spurious func_80048390 argument matches the original nop delay slot. Exact linked and overlay SHA1 pass with headers/globals OK. No FAKE. Six matches total4172 text bytes plus124 table bytes.

2026-10-17: Code review against CODING_STANDARDS.md at w2-int 28d5f509: no findings. The six matches (func_80132428, func_80132FF0, func_80132940, func_80138D48, func_80133204, func_801388D0) are plain C apart from the marked FAKE name buffer in func_80132FF0, and the two guards (func_80134418, func_8013987C) name T-12090 and a row count. The unmatched rest of BUNKA_SD (func_80139290, func_801344A8 and the two guards) sits in the unstarted conveyor packages w3-bunka-sd-p02 and p03 (see bunka-sd-family-results). Moved to Done.

## T-12090: cx9 BUNKA_SD byte-first pass

Six verified matches, **4172 text bytes**, plus124 bytes of jump tables. All ten targets attempted. Every match has exact production linked proof and passing BUNKA_SD SHA1, with immediate commits. Final package verification is recorded in [[tickets/T-12090-codex-cx9]].

| Function | Linked bytes | Source pattern |
|---|---:|---|
| func_80132428 |448| Canonical GameState fields and missing overlay forward declarations repair the old stripped score-zero dispatcher. |
| func_80132FF0 |532| Explicit case4 restores the five-entry jump table. FAKE:24-byte name-buffer capacity reproduces original stack homes; source capacity unknown. |
| func_80132940 |1144| Reused palette/setup index; explicit case4; u16 intermediate frame index before subtract2; typed DisplayActor44. |
| func_80138D48 |952| Unsigned record bytes and32-bit frame induction; image metadata assignment before index updates; for-clause increment, with i=2 then frame=15 initialized before the loop. |
| func_80133204 |828| Explicit fifth switch case, typed word-table pointer arithmetic, separate if/else assignments instead of m2c shared goto. Linked size includes8 trailing padding bytes. |
| func_801388D0 |268| Neighboring s32 no-return state-machine idiom, canonical byte +=1, and no spurious argument to func_80048390. |

Retained guards: func_8013987C14 differing listing rows (pointer setup and missing clear-zero live range); func_8013441826 rows (base/selector allocation and shared0x80 constant). Saved func_801344A8 remains57 rows and stays assembly. No new FAKE beyond the disclosed name buffer.

Largest remaining target func_80139290 is1516 text bytes plus88 table bytes. Repaired canonical records, natural independent case stores instead of two m2c shared gotos, and a timer local improve509→89 rows; repeating the delta expression instead of its local reaches87. Regdiff reports377 versus378 instructions: target copies the switch result v1→a1, then allocates the character byte in t0; the draft coalesces that copy and takes a1 for the character, shifting later mask values. Explicit copies, signed/unsigned views, assignment-in-switch, first-case splitting, bounded empty tests, and return liveness do not close it. Broad candidate stays scratch-only. These are measured failures, not proof that matching C is impossible.

Native AS1 diagnostics were calibrated on the952-byte near miss. A scratch copy of frame_pass applies the unchanged transform and injects `-R -xbbdbg 8` only at real as1 invocation; complete stock and traced objects are byte-identical. Captured native output has8958 lines and a reached loop DAG. Direct `-Wa` forwarding fails the production frame wrapper's numeric-argument parser, so do not add it to production flags. The remaining six rows were equal-priority scheduling nodes; natural for-loop increment placement closed them. Retained evidence in cx9 `build/scratch/as1-stock.o`, `as1-trace.o`, `as1-native.log`, `38d-clean.c` and `38d-final.c`. No production tool changes or diagnostic patches are part of the match.

### Final verification

Fresh Docker configure and ninja -j2 pass all 27 SHA1 checks, headers and globals after deleting build .ok stamps. sync_protos --check-branch reports 0 new disagreements, 15 known; branch check OK. git diff --check passes. Logs: build/scratch/cx9-final.log and cx9-protos.log. All ten targets attempted; six matches, two retained guards, two assembly fallbacks. Awaiting independent review.
