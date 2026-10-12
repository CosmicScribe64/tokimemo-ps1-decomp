---
id: T-10020
title: "Wave 11: list 2"
status: Done
assignee: Codex w11-2
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[register-allocation-wave7]]"]
---

## Goal

Match as many of the 158 assigned functions (54,604 bytes, 53 owned files) as possible, with at least four hours of work and a deep pass after initial drafting.

## Acceptance criteria

- [x] Draft, build and resolve function differences; iterate on near misses.
- [x] Keep verified matches only; preserve useful failed drafts in scratch.
- [x] Docker ninja passes all 27 units, header/global guards and prototype branch check.
- [x] Inline coding-standards review completed; no commits or out-of-scope edits.

## Notes

Shared brief and list: scratchpad/wave11-brief.md and scratchpad/wave11-list-2.txt in the orchestrator's private scratch root. Experiments: scratchpad/w11-2/. Follow the brief's source ownership and Docker isolation rules. Initial worktree is clean; configure/build outputs are absent.

## Comments

- 2026-10-11: Read wiki index, shared brief, standards, local instructions, workflow and allocator evidence. Starting first pass, followed by ranked deep iterations and short permuter searches.

- Initial Docker configure/ninja completed with all 27 unit SHA1 checks OK. Scratch first-pass results distinguish compile failures from actual resolved differences. Manual Work80125D10 repair and narrow-parameter tests reduced main func_8007B2B4 to ten differing instruction lines; GYOZI func_80137F48 local-range trials are in progress. No new match claimed yet.

- Verified gain 1: main func_8007B2B4, 164 bytes. K&R u16 parameter preserves the entry mask and the existing unprototyped two-argument caller. The dead `if (1)` around the selector and its shared source line with the flag update are explicitly FAKE: they select v0 and the original ORI schedule. Real `funcdiff --resolve --unit main` MATCH; subsequent Docker ninja passed all 27 SHA1 checks. No header changes were needed. TT func_80134178's five-minute permutation did not improve its two-line scheduling miss.

- Verified gain 2: TT func_80149108, 232 bytes (batch total 2 / 396). Existing TtState58 supplies the real 0x58 record stride and member widths. An empty index-bound test after the state switch changes the saved-register allocation without emitting instructions; it is explicitly marked FAKE. Real resolved funcdiff MATCH and TT overlay SHA1 pass. Forty-eight loop/limit variants and typed/void cleanup checks precede the real-file verification. Other near misses remain scratch-only.

- Verified gains 3–4: DATE func_8013E3C4 (292 bytes) and ETC func_8014BF2C (236 bytes); batch total 4 / 924. DATE uses a documented FAKE narrow constant/cast for branch ordering and stack home, and writes the independent girl field before D_80122D20. ETC uses a GameState pointer for its selector and the s32 fallthrough form; no dummy statements remain. Both real funcdiffs MATCH and DATE/ETC SHA1 pass. Inline review also replaced TT's integer-to-pointer call casts with u8 pointer casts; that function and checksum remain exact.

- Verified gain 5: TAIIKU func_801482BC, 180 bytes; batch total 5 / 1,104. Appended the documented 0x1C slot type and moved the pre-existing duplicate byte-view declarations of D_8014A400 into the overlay header. The reset uses slot index zero and real TaiikuController fields; no FAKE is required. Typed cleanup passes real resolved funcdiff, and the subsequent dependent-object rebuild passes all 27 unit SHA1 checks. Scoped dupes is being rerun at the checkpoint.

- Verified gain 6: TACO func_80148B58, 532 bytes; batch total 6 / 1,636. Reusing the loop counter and indexing the constructor table restore register allocation and the call delay slot. Existing TcActor fields replace all tail offsets. Real resolved funcdiff MATCH and all 27 unit SHA1 checks pass. The five-match scoped-dupes checkpoint tried one owned EVENT candidate and retained no additional match.

- Verified gains 7–8: EN_NICHI func_801337E0 (268 bytes) and DATE func_80139DD0 (364 bytes), total 8 / 2,268. EN_NICHI uses EnObj fields, precomputed squared radius, direct nested comparisons and a documented FAKE shared setup line. DATE removes the m2c shared-goto temporary and uses ordinary switch-case updates, with no trick. Both real funcdiffs MATCH and all 27 unit checksums pass. Six bounded retry rows have been appended to T-0018.

- Verified gain 9: main func_800836A0 (360 bytes), total 9 / 2,628. Explicit zero returns preserve both early failure paths; the success path falls through. A documented FAKE byte cast before the seven-bit mask supplies the target temporary range. Accessing the byte clear and both flag words through D_8011ECD0 restores their order. Real funcdiff and all 27 checksums pass. sync_protos --fix changed no declaration views; its unrelated six-declaration reorder was edited back.

- Verified gains 10–11: ETC func_8014AF28 (424 bytes) and TAIIKU func_80147A6C (168 bytes), total 11 / 3,220. ETC uses an indexed view of the nine contiguous GsRec0FC parameter records; the natural loop is exact without a FAKE. TAIIKU uses the recovered TaiikuSlot1C; a single documented empty pointer test preserves its register range. Real resolved funcdiffs MATCH. Gain 10 checkpoint passes all 27 checksums; scoped dupes retained no new function.

- Verified gain 12: TT func_80133D14, 284 bytes, total 12 / 3,504. Recovered TtActor68 fields and declaration order object/source/angle match the initializer naturally. Corrected the existing helper func_80133BE0 to s32 fallthrough because its caller consumes v0; the helper remains an exact match. Both resolved diffs, TT SHA1 and globals guard pass.

- Verified gain 13: EN_NICHI func_8013275C, 216 bytes, total 13 / 3,720. A real 12-byte definition covers the ten status bytes plus two bytes of trailing padding. Splitting it into separate 10- and 2-byte C arrays added alignment padding and failed the unit checksum; the single extent restores all 27 unit checksums. The reset reuses index in the global reset and shares the word-pointer initialization/do line, both documented FAKE code-generation constraints. Both the new function and the earlier func_801337E0 still resolve exactly.

- Five further bounded retry rows appended after reading register_shapes classifications: TACO func_8015B1B8 / func_80152808, TT func_80134178, main func_8007A868 / func_800422C8. Total T-0018 rows added: 11. Existing historical near misses are distinguished from current measurements.

- Verified gain 14: main SD_CalcCDAve, 264 bytes, total 14 / 3,984. Correct u16 start/count, signed halfword samples and indexed channel reads remove the register-pair swap. Explicit result masks preserve redundant target ANDIs and are marked FAKE. Removed unused draft locals, reformatted normally, and recompiled the cleaned body to zero; real resolved funcdiff, main SHA1 and globals guard pass. The thirteen-match prototype branch guard also passed with zero new disagreements (17 known).

- Verified checkpoint 17: total 17 functions / 4,488 bytes. Owned-file extension: TACO func_80152764 (164 bytes) matches with the correctly typed s16 resource pointer D_80160444; SHUGAKU func_80139400 (152 bytes) uses a documented FAKE local s32 selector to preserve v0; DATE func_8013DEF8 (188 bytes) matches after moving the independent girl-field store before D_80122D20. The latter two routines have ordinary control flow and unchanged strings. All resolved comparisons and all 27 unit SHA1s pass.

- Four final bounded retry rows added after reading register_shapes: TACO func_80137D00, TT func_80149400, EVENT func_80110234 and SHUGAKU func_80139054. Total T-0018 rows added: 15. TACO func_8015BF70 remains a mixed lifetime/control-flow draft (50 lines after typed animation-pair recovery, 168 declaration/control variants and a 300-second search), so it is not presented as a pure allocator blocker.

- Verified checkpoint 23: total 23 functions / 5,476 bytes. Six further owned-file functions match: DATE func_8013BFF8 and EVENT func_801189B4 (172 bytes each) are natural switch dispatchers; DATE func_80138314 / func_801385D8 (168 / 164 bytes) copy real 11- and 12-entry tables of paired s32 arguments into typed local aggregates; EVENT func_80117304 (140 bytes) copies two byte-table rows through the existing word address views, with integer addition before the pointer cast preserving ADDU operand order; EN_NICHI func_801320C0 (172 bytes) needs forward void declarations for its already matched local callees. No extra dummy statements are retained in these six. Jump-table data is verified by the unit SHA1s, and byte counts exclude table words. All resolved comparisons and all 27 unit SHA1s pass.

- Whole-unit review caught a collateral SHUGAKU difference after the callback conversion: only two bytes at unit offset 0x6E14, the BNE operand order in existing func_80138DD8. Commuting its comparison and changing local widths did not help; that existing source remains unchanged. A documented FAKE empty if (1) after the new callback-table copy preserves the compilation context and restores both exact function comparisons plus the unit checksum. Final 27-unit verification includes this correction.

- Verified final dispatcher checkpoint: 26 functions / 6,008 bytes. OPTION func_80138A8C (192 bytes) uses GameState.unk_1104.w and a forward void declaration; DATE func_8014A1E4 (156 bytes) reads its signed-storage selector through a u16 memory view and declares four existing void callees; SHUGAKU func_80138B20 (184 bytes) copies the real 71-callback aggregate, with a documented FAKE wrapper adding the observed unused 0x10-byte extent to reproduce the 0x158 frame. All body instructions and stack locations match. The wrapper does not claim a recovered source purpose or change compiler flags. Cleaned full-object comparisons, real resolved funcdiffs and all 27 unit SHA1s pass.

- Final inline review against CODING_STANDARDS: PASS, no unresolved findings. Reviewed all retained C, appended overlay declarations/types, both existing-helper/context effects, and the twelve-byte data extent. C89, fixed-width fields with documented layouts, canonical SDK/game views, explicitly ticketed FAKE constructs, header ownership, source ownership, and unchanged raw/tool/config files all satisfy the checklist. The callback's extra local extent and empty compilation-context branch are explicitly FAKE; the existing neighboring source stays unchanged. All 27 final unit SHA1s pass, including jump-table/data verification; the prototype guard reports zero new disagreements (17 known), globals pass, and final incremental ninja has no work. Whitespace and append-only wiki/header checks pass. No commit, staging, merge, push, or host dependency installation was performed.

- Tooling evidence: permute.py can return success and describe a 300-second search even when the base compilation fails immediately; invalid-base runs were excluded from bounded-search claims. dupes.py uses generated asm directory classification, so it still counted 4,982 C functions after progress counted 5,005. A scratch-only source-state wrapper refreshed the 26 newly matched definitions, counted 5,008, and found zero scoped copy plans. Repository tools remain unchanged.

- Final context refresh rebuilt saved near-miss drafts for 20 functions against the finished source; none became exact. Final result: 26 functions / 6,008 bytes; 15 T-0018 rows. `grand total   5008/6958   842948/2279368  37.0%`. More than four hours elapsed before closure at 2026-10-12 04:53:51 UTC. Changes remain uncommitted on w11-2.
