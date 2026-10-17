---
id: T-20006
title: Match two GYOZI scene callbacks
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[event-family-results]]", "[[matching-notes]]"]
---

## Goal

Match func_80141944 (336 bytes, src/ovl/GYOZI/801418B0.c) and func_80145384 (372 bytes, src/ovl/GYOZI/80143AD0.c). Claim opened before implementation: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/43.

## Acceptance criteria

- [x] Actual default linked proofs for both installed functions (708 bytes).
- [x] Full build, 27 binary SHA1 checks and header/global/prototype guards.
- [x] Local standards review and documentation complete.
- [x] Focused PR for verified work: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/44.

## Notes

Branch codex/gyozi-scene-callbacks starts at main d27b6916. Only these two functions are claimed, not other functions in their files or TT. Maintainer handles moving-main/wiki integration. Archived m2c drafts have invalid scalar callback calls; existing GzCallbacks68/BC and GyoziRec44 should model the table copies and display fields. Compare against the original before installing any source; scratch outputs remain ignored and all compiler tools run in Docker.

## Comments

PRs #37, #40 and #42 are merged. Maintainer independently verified their linked matches and all 27 hashes; no outstanding review request remains on those contributions.

Full-unit diagnostic results: func_80141944 reaches zero resolved instruction differences with a meaningful s32 index before GzCallbacks68, s32 fall-through return, direct ++ on the existing GyoziRec44 counter and fresh global selector checks. Void controls remain 18+ rows different. func_80145384 reaches two rows (one lb/lbu pair), using the same index/table/counter form with GzCallbacksBC. The unsigned byte read from the first record is being tested explicitly. No actual linked match is claimed yet; source remains ASM.

Both source bodies now have zero resolved differences with ordinary field accesses (plain.log). The remaining lb/lbu pair was a wrong signed field type: original reads D_801317EB and GyoziRec44.unk_07 with lbu. Their shared u8 types replace the initial signed types; plain source assignments now need no byte-view casts. The earlier unsigned cast on the loaded signed field did not fix the instruction, while an unsigned memory view did; correcting the field type gives the same zero score naturally. No other function body is edited. Installed the two candidates and shared s32 declarations for actual default linked verification. Void callee declarations and the word flag remain shared in GYOZI.h.

First actual default linked proof passed for both installed functions: 336 + 372 = 708 bytes (default-proof.log). Added the explicit shared s32 func_801428EC(void) declaration matching its previously implicit-int no-argument call. The final whole build, all-binary hash validation, prototype guard and fresh linked proofs are running after that final declaration/comment cleanup. No new FAKE, local override, volatile access, frame padding, compiler pass or literal workaround is used.

Local self-review against CODING_STANDARDS.md: new C is C89 with fixed-width locals, existing aggregate/SDK types, original symbols and function order. All new declarations live once in the overlay header. The index local has a real use and follows the measured T-3330 stack-table pattern; plain shared return types were tested before any override. Original lbu/lw widths support the data declarations. No other C body, raw input, generated assembly, binary, compiler option or pass is edited. No new FAKE, dummy statement/local, pointer-view workaround or NON_MATCHING body is retained. Full validation remains required before moving to In Review.

Build scheduling note: the initial whole-tree ninja -j2 run was intentionally interrupted after its EVENT binary checksum passed, solely to resume the remaining jobs with ninja -j8. Compiler flags and all source inputs are unchanged; initial output is retained in full-build-initial.log. The resumed full default build and explicit 27-binary checks determine acceptance.

Final verification on branch base d27b6916, all Docker: full default build PASS (full-build-initial.log and resumed full-build.log); explicit checksums.log reports 27/27 binary SHA1 checks passed, including GYOZI and all existing functions affected by the shared header; protos.log reports 0 new / 15 known disagreements and branch check OK. final-proof.log reports fresh actual default linked MATCH for func_80141944 (336 bytes) and func_80145384 (372 bytes). Source and header are unchanged after these checks. Local standards review has no unresolved findings. Moved to In Review; maintainer acceptance is pending. Latest fetched main 41b3fdf9 still has both functions as INCLUDE_ASM, so no duplicate matches are submitted.
