---
id: T-12100
title: Match DATE movie and effect functions
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[build-system]]"]
---

## Goal

Byte-matching pass on the twelve assigned DATE functions, prioritizing the large movie/effect family and reusable type/data ownership repairs. Owned C files: src/ovl/DATE/801461B0.c, 8014A1B0.c, and 8014CC40.c.

## Acceptance criteria

- [x] Every promotion has literal linked-byte and DATE SHA1 verification.
- [x] All assigned functions attempted; only ticketed near misses below about 30 rows retained.
- [x] Final configure/full build passes 27 checksums, headers, globals and prototype checks.
- [ ] Standards review before Done.

## Notes

Baseline Docker configure and ninja -j2 pass 27/27 checksums, headers and globals. Evidence: build/scratch/baseline.log. Worktree cx10 only; cx3 and main are frozen/read-only. No toolchain changes, host dependencies, merge or push.

The existing 489D0 draft has a documented data-ownership/shared-LUI blocker. Unlike its earlier batch, this package permits changes to config; investigate the established 8015CF80..8015CFD0 data range first. External matching research was read for scratch-only AS1 diagnostics and equal-score/full-file candidate verification; production proof remains unchanged.

## Comments

- 2026-10-15: package claimed after successful baseline.

- Matched `DATE:func_801489D0`: 2,068 literal linked bytes, production DATE SHA1 and header/global checks pass. Added the established 8015CF80..8015CFD0 data island, first verifying its assembly pieces unchanged, then defined the four known s16[3] coordinate/velocity arrays in the owning C file. Their zero initializers and aligned padding reproduce the original bytes. Shared LUI initialization now matches without any function-body trick. Existing DateMovieScene50 layout and array declarations were reused. Logs: build/scratch/data-island.log, island-build.log, 489d0-before.log, 489d0-owned.log, 489d0-build.log and 489d0-final.log.

- Matched `DATE:func_8014E3E8`: 2,000 literal linked bytes and production DATE SHA1, headers/globals pass. Recovered a natural initialized s32[4][2] colour table and indexed pair pointer; repeated timer remainder expressions reproduce the original frame without extra phase locals. The initializer owns the original 32-byte D_8015E510 data, replacing its INCLUDE_RODATA. No FAKE shaping. The normal funcdiff relocation normalizer reports two rows for anonymous `.data+0x2F0`; the existing exact verifier called directly on a frozen production object confirms all 2,000 bytes (source hashes stable). Evidence: build/scratch/e3e8-build.log, e3e8-final.log, e3e8-literal.log; scratch harness literal_proof.py. No production verifier changes.

- `4718C` bounded recovery: repaired byte-vs-record pointer strides, GameState array arithmetic, actual zero return, MovieWork and Rec24 views; unified m2c locals and tested direct state accesses. Still 700+ diff rows with timer-tail placement, branch layout and register lifetime differences (frame 48 vs target64). Restored production fallback; useful typed recovery retained only in ignored build/scratch/func_8014718C-recovered.c. Logs 4718c-{fixed,unify,natural}.log. This is a substantial control-flow reconstruction task, not a scheduling near miss.

- `47958` / `462B0`: typed Rec24 indexing, corrected GameState byte-array addressing, MovieWork aliases, real RECT locals, loop-variable and sprite-pointer unification tested. 47958 also tested s32 colour arithmetic and explicit zero return; 462B0 tested word-sized selector preservation. Both remain over 1,000 diff rows with oversized frames and large control-flow/lifetime differences. Restored assembly fallbacks and all temporary prototype changes. Recovered candidates and logs are in build/scratch/func_80147958-recovered.c, func_801462B0-recovered.c, 47958-{typed,unified}.log, 462b0-{typed,unified}.log. No distant draft retained in production.

- Matched `DATE:func_8014A4DC`: 820 literal linked bytes, production DATE SHA1, headers/globals. Recovered destination pointer array D_8015DE34[3] in its existing data island, repaired first buffer loop (start 0x80180000, increment 0x4000) and byte stride of the second, unified loop locals, and used the actual unsigned halfword switch view. Separate assignment source lines reproduce the original load/store order. Added existing void callee prototypes AF88/B080 to header for recovered call context; existing consumer uses array element zero. No FAKE shaping. Evidence: build/scratch/a4dc-{context,unify,array,lines,build,final}.log. Package total 4,888 bytes / three matches.

- `EF1C`: tested existing shared POLY_G4 packet type, one pointer versus two, u8 versus u32 RGB locals, RGB source/store order and grouping. Best remains roughly90 differing instruction rows, beyond retained-guard threshold; restored fallback. Typed candidates/evidence in build/scratch/ef1c-original.c and ef1c-{typed2,group,bytes,rgb}.log. A broad scratch restore selector was caught in diff inspection; the affected file was backed up and restored from the already verified 57d6788b commit, preserving E3E8. No accidental deletion remains.

- User steering follow-up: largest remaining literal spans are 47958=4,044, 462B0=3,804, 4718C=1,996 bytes (worklist sizes included adjacent tables). Reopened 47958 for deeper reconstruction. Direct indexed Rec24 fields in natural loops recover original register allocation, reducing the candidate from 1,029 to about1,013 instructions versus1,010 target. GameState pointer access for the state word coalesces callback restores back into original a3; direct selector accesses preserve the target's scratch registers. Scalar state alias diagnostic did not help and was removed. Remaining differences are fade narrowing, timer/state scheduling and local layout; checkpoints/logs build/scratch/47958-deep1..7, stateptr, recordptr. None promoted.

- Deep 47958 follow-up: all 1,010 instructions now align; best candidate differs in four register-only rows (two fade addiu/andi pairs). Removed redundant inner returns in cases23/40 to recover the original branch layout. Reordered genuine locals and added movie/header pointer declarations to recover the96-byte frame; header declaration is currently unused and must be labelled FAKE if retained. Reversing the case42 comparison restores branch operand order. Keeping the case40 call, state restoration and comparison on one source line restores the original AS1 load schedule; ordinary separated lines differ by2–3 rows. Existing DateMovieHeader8 direct field casts preserve the old header loads. Evidence: build/scratch/g2-callandafter.log, g2-typedheader.log, 47958-seven-reg.log. No bytes claimed/promoted yet.

- Matched `DATE:func_8014D298`: 740 literal linked bytes, production DATE SHA1 and header/global checks pass. Reused DateParticleView44 for the68-byte local and all three record copies; the genuine record-pointer declaration before the scene local recovers its0x30 stack address. A diagnostic unused padding declaration also matched, but was discarded in favor of the useful pointer. Recovered GameState field accesses replace old scalar aliases. No FAKE shaping. Evidence: build/scratch/d298-{typed,natural,build,final}.log. Package total5,628 bytes / four matches.

2026-10-17: Code review against CODING_STANDARDS.md done. The four matches (func_801489D0 2,068 B, func_8014E3E8 2,000 B, func_8014A4DC 820 B, func_8014D298 740 B) are on main as C89; the FAKE comments name T-12100 with reasons, and its near-miss guards were resolved by later work (T-17201 matched func_80147958; AF88, A8FC and EF1C are ticketed under T-17208 and T-17203). func_801462B0 stays assembly and is distant. No findings. Moved to Done.

## Final matching pass

Verified coverage is **5,628 bytes in four functions**: 489D0 (2,068), E3E8 (2,000), A4DC (820), D298 (740). All four have production DATE checksum and literal linked-byte evidence; none uses FAKE shaping.

The retained new guards are 47958 (four instruction rows) and AD9C (five instruction rows). The former matches the full 1,010-instruction structure; remaining fade addiu/andi pairs use ugen temporaries instead of destructively updating the source register. Its unknown non-void return behavior, unused frame slot and callback source grouping are explicitly FAKE. Width, signed-promotion, repeated-expression, liveness, indexed/global views and 500 optimizer-guided candidates did not close the gap. AD9C uses SDK MATRIX and SVECTOR, correct 12-byte motion and 8-byte angle strides, and exact instruction/register order; only two saved-pointer slots differ by 4 bytes. Natural declaration-order and genuine-pointer-local probes failed; no padding retained. Logs: 47958-autotweak.log, g2-callandafter.log, ad9c-vector.log and ad9c-layout0..5.log.

Existing AF88 and DCCC guards received bounded follow-up. DCCC's byte-field pointer fixes its frame and table offset, leaving 16 register/operand rows; 235 guided candidates made no further improvement. AF88 remains a saved-constant/end-pointer allocation mismatch. Evidence: dccc-ptr-first.log, dccc-autotweak.log, af88-baseline.log, af88-permute.log.

462B0 was reconstructed to the exact 950-instruction count, with natural indexed loops, a shared animation tail and four-way substate switch. Saved-state promotion, fade narrowing, timer registers and local placement still differ substantially. A8FC received corrected pointer strides, shared counters, chained RGB updates, natural object-hide iteration and return-type diagnostics; it remains distant. Both are restored to assembly; latest candidates are ignored scratch files func_801462B0-final-distant.c and func_8014A8FC-final-distant.c. Their earlier checkpoints preserve alternate hypotheses. Together with the previously recorded 4718C and EF1C attempts, all 12 assigned functions were attempted.

Final handoff validation: fresh configure and Docker ninja -j2 passed all 27 binary SHA1 checks, headers, globals and symbols. Branch prototype check reports zero new conflicts (15 known baseline conflicts). Four retained guards: 47958 (4 rows), AD9C (5), DCCC (16), AF88 (17). All larger unmatched drafts use assembly fallbacks. Evidence: build/scratch/final-build.log, final-protos.log and *-retained.log. Awaiting standards review; no new matches in this final guard-only checkpoint.
