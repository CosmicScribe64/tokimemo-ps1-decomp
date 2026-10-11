---
id: T-9530
title: "Wave 8: list 3"
status: Done
assignee: w8-3
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Work through the 136 functions in wave8-list-3.txt, within the 36 owned C files.

## Acceptance criteria

- [x] Every listed function matched or time-boxed and restored to INCLUDE_ASM.
- [x] Matches verified with Docker funcdiff and all 27 linked-unit SHA1 checks.
- [x] Shared declarations and GameState access checks pass.
- [x] Inline review against CODING_STANDARDS.md completed; findings resolved.

## Notes

Follow the shared wave-8 brief. No commits, merges, pushes, tooling edits, or unowned C edits.

## Comments

2026-10-11: Read the wiki index, workflow, standards, wave-8 brief, and T-9410 register-allocation findings. Claimed list 3.

## Results

9 functions / 4,040 bytes adopted. All 136 functions received a fresh, explicit-path m2c attempt in list order; recoverable drafts were probed with the current Docker compiler. Attempts were bounded (90-second draft/compiler limits; 60-second permutation runs on selected near misses). Non-matches remain INCLUDE_ASM; no failed C or trial padding was retained.

| Unit/file | Function | Bytes |
|---|---|---:|
| GYOZI/8013D470 | `func_8013D764` | 288 |
| TAIIKU/80133C80 | `func_801345A4` | 344 |
| KANGEI/80137B90 | `func_80138D40` | 252 |
| TAIIKU/80133C80 | `func_80134D94` | 516 |
| TACO/80151C30 | `func_80153774` | 812 |
| GYOZI/8013C3B0 | `func_8013CB18` | 452 |
| RPG_BAT/80141C30 | `func_80141C70` | 456 |
| RPG_BAT/80141C30 | `func_80141E38` | 456 |
| RPG_BAT/8013E300 | `func_8013E300` | 464 |

Eleven observed register retries were appended to [[data/t0018-cases]]. Other time boxes need record/table/type recovery or broader C/control-flow changes; they are not labelled impossible or attributed to T-0018 without evidence. Own-data/layout work requiring config YAML edits is outside this batch's allowed edits.

Scratch evidence: the shared scratchpad `w8-3/` directory, including `probes-refined-0.jsonl`, `probes-45.jsonl`, `probes-75.jsonl`, `probes-105.jsonl`, `custom-*.jsonl`, and the three `permute-*.log` files. The JSONL records contain draft text, status, compiler diagnostics, and resolved instruction differences.

Tooling findings: m2c omitted argument 3 (`500`) of RPG_BAT `func_8014B738` despite its three-parameter matched definition. The ENDING permutation search emitted internal AST type-resolution failures on implicit renamed callees, but continued and stopped at its limit. Scoped duplicate/near-duplicate scans found no applicable copies. They classify matches from assembly directories and still printed the baseline 4734 after configure, while current source progress is 4743.

## Attempt manifest

Each non-match below was time-boxed. Category totals: T-0018 retry: 11; Draft record/table/control recovery: 58; C/code generation difference: 28; Type/declaration recovery: 30; Matched: 9.

| # | Owned file | Function | Outcome / remaining work |
|---:|---|---|---|
| 1 | EVENT/800F9680 | `func_800FA194` | T-0018 retry |
| 2 | EVENT/800F9680 | `func_800FA0D0` | T-0018 retry |
| 3 | ENDING/801396E0 | `func_80139B48` | Draft record/table/control recovery |
| 4 | TT/80148DB0 | `func_80149B20` | C/code generation difference |
| 5 | TAIIKU/80133C80 | `func_80136B18` | T-0018 retry |
| 6 | GEKO/80142760 | `func_80144198` | Type/declaration recovery |
| 7 | SHUGAKU/80132000 | `func_801322F0` | T-0018 retry |
| 8 | EVENT/800F9680 | `func_800FAA04` | Draft record/table/control recovery |
| 9 | TT/80148DB0 | `func_80149400` | Draft record/table/control recovery |
| 10 | BUNKA_SD/80134260 | `func_80134418` | C/code generation difference |
| 11 | TAIIKU/80133C80 | `func_801368B8` | Type/declaration recovery |
| 12 | EVENT/800F9680 | `func_800FA25C` | Type/declaration recovery |
| 13 | TT/80148DB0 | `func_8014A814` | T-0018 retry |
| 14 | GYOZI/80136220 | `func_80137188` | Type/declaration recovery |
| 15 | GYOZI/80136220 | `func_80136488` | Type/declaration recovery |
| 16 | SHUGAKU/80132000 | `func_80132BCC` | C/code generation difference |
| 17 | TAIIKU/80133C80 | `func_80137228` | C/code generation difference |
| 18 | ENDING/801396E0 | `func_80139EC4` | T-0018 retry |
| 19 | TAIIKU/80133C80 | `func_80136DA4` | Type/declaration recovery |
| 20 | TACO/8015D0D0 | `func_8015D0D0` | Type/declaration recovery |
| 21 | TT/80148DB0 | `func_80149EA0` | C/code generation difference |
| 22 | TT/80144D70 | `func_80145370` | T-0018 retry |
| 23 | ENDING/80133C10 | `func_80134B18` | C/code generation difference |
| 24 | TAIIKU/80133C80 | `func_80138680` | Draft record/table/control recovery |
| 25 | TT/8013E290 | `func_8013E498` | Draft record/table/control recovery |
| 26 | TT/801487D0 | `func_801487D0` | Draft record/table/control recovery |
| 27 | EVENT/800F9680 | `func_800FA6A0` | C/code generation difference |
| 28 | TAIIKU/80133C80 | `func_80138800` | T-0018 retry |
| 29 | TAIIKU/80133C80 | `func_80137090` | Draft record/table/control recovery |
| 30 | TAIIKU/80133C80 | `func_80135DF8` | Draft record/table/control recovery |
| 31 | TT/8013E850 | `func_8013EC64` | Draft record/table/control recovery |
| 32 | RPG_BAT/80144010 | `func_80144480` | C/code generation difference |
| 33 | TAIIKU/80133C80 | `func_80137440` | Type/declaration recovery |
| 34 | 8004E500 | `func_8004F4F0` | Draft record/table/control recovery |
| 35 | TT/80148DB0 | `func_801498C8` | C/code generation difference |
| 36 | ENDING/801396E0 | `func_8013B510` | T-0018 retry |
| 37 | TAIIKU/80133C80 | `func_80135D24` | Draft record/table/control recovery |
| 38 | BUNKA_SD/80132000 | `func_80132708` | C/code generation difference |
| 39 | TT/80148DB0 | `func_80149108` | C/code generation difference |
| 40 | TT/80148DB0 | `func_80148E7C` | Draft record/table/control recovery |
| 41 | TT/80148DB0 | `func_80149704` | Draft record/table/control recovery |
| 42 | TACO/80151C30 | `func_801522B0` | T-0018 retry |
| 43 | TAIIKU/80133C80 | `func_80137C44` | Draft record/table/control recovery |
| 44 | ENDING/801396E0 | `func_80139F3C` | C/code generation difference |
| 45 | SHOUGATU/801330C0 | `func_8013315C` | Draft record/table/control recovery |
| 46 | TT/80148DB0 | `func_801491F0` | Draft record/table/control recovery |
| 47 | TT/80148DB0 | `func_8014AB48` | C/code generation difference |
| 48 | TACO/80151C30 | `func_80152808` | Type/declaration recovery |
| 49 | TACO/80139170 | `func_8013A580` | Type/declaration recovery |
| 50 | 8004C3F0 | `srn_set` | Draft record/table/control recovery |
| 51 | TAIIKU/80133C80 | `func_80135480` | Draft record/table/control recovery |
| 52 | DATE/8014CC40 | `func_8014DCCC` | Draft record/table/control recovery |
| 53 | TAIIKU/80133C80 | `func_80136EC0` | Draft record/table/control recovery |
| 54 | GYOZI/80136220 | `func_801362A0` | Draft record/table/control recovery |
| 55 | TAIIKU/80133C80 | `func_80135830` | Draft record/table/control recovery |
| 56 | TACO/801480B0 | `func_80148B58` | Type/declaration recovery |
| 57 | TAIIKU/80133C80 | `func_80137EA8` | Draft record/table/control recovery |
| 58 | TT/80145E60 | `func_80146000` | Draft record/table/control recovery |
| 59 | EVENT/800F9680 | `func_800F9730` | Draft record/table/control recovery |
| 60 | 80049FF0 | `func_80049FF0` | C/code generation difference |
| 61 | 8004E500 | `load_string` | Draft record/table/control recovery |
| 62 | TAIIKU/80133C80 | `func_801365C8` | Draft record/table/control recovery |
| 63 | SHOUGATU/801330C0 | `func_80133760` | Type/declaration recovery |
| 64 | GYOZI/8013D470 | `func_8013D764` | Matched |
| 65 | TT/80148DB0 | `func_80149BB0` | C/code generation difference |
| 66 | TACO/80137A60 | `func_80137D00` | Type/declaration recovery |
| 67 | TACO/80151C30 | `func_80152C60` | C/code generation difference |
| 68 | ENDING/801396E0 | `func_8013A004` | Draft record/table/control recovery |
| 69 | TAIIKU/80133C80 | `func_801345A4` | Matched |
| 70 | KANGEI/80137B90 | `func_80138D40` | Matched |
| 71 | TACO/801480B0 | `func_801483B0` | Type/declaration recovery |
| 72 | TAIIKU/80133C80 | `func_80135AD0` | Draft record/table/control recovery |
| 73 | BUNKA_SD/80132000 | `func_801322EC` | Type/declaration recovery |
| 74 | GYOZI/80134520 | `func_801347D4` | C/code generation difference |
| 75 | TACO/801480B0 | `func_801487A4` | Draft record/table/control recovery |
| 76 | TT/80148DB0 | `func_801494D8` | Draft record/table/control recovery |
| 77 | SHUGAKU/80132000 | `func_8013243C` | C/code generation difference |
| 78 | TACO/80133870 | `func_801342CC` | Type/declaration recovery |
| 79 | TACO/801480B0 | `func_80148184` | Type/declaration recovery |
| 80 | TT/8013E290 | `func_8013E6A0` | Draft record/table/control recovery |
| 81 | TACO/801480B0 | `func_801484E4` | Type/declaration recovery |
| 82 | TAIIKU/80133C80 | `func_80137784` | Draft record/table/control recovery |
| 83 | TACO/801480B0 | `func_80148D6C` | C/code generation difference |
| 84 | TACO/80139170 | `func_8013AD64` | Type/declaration recovery |
| 85 | 8004E500 | `func_8004E58C` | Type/declaration recovery |
| 86 | TAIIKU/80133C80 | `func_80134D94` | Matched |
| 87 | TACO/80133870 | `func_801345D4` | Type/declaration recovery |
| 88 | TACO/801430B0 | `func_801430D0` | Draft record/table/control recovery |
| 89 | 80049FF0 | `func_8004A1A0` | Draft record/table/control recovery |
| 90 | TACO/80151C30 | `func_80153774` | Matched |
| 91 | TACO/80139170 | `func_8013920C` | T-0018 retry |
| 92 | TAIIKU/80133C80 | `func_80137544` | C/code generation difference |
| 93 | 8004C3F0 | `get_fnt` | Draft record/table/control recovery |
| 94 | SHOUGATU/801330C0 | `func_80133B44` | Draft record/table/control recovery |
| 95 | TACO/80151C30 | `func_80151D1C` | Type/declaration recovery |
| 96 | GYOZI/8013C3B0 | `func_8013CB18` | Matched |
| 97 | TACO/801430B0 | `func_801432F0` | Draft record/table/control recovery |
| 98 | TACO/80139170 | `func_8013A790` | Draft record/table/control recovery |
| 99 | 8004C3F0 | `mod_trans_vram` | Draft record/table/control recovery |
| 100 | TACO/80151C30 | `func_801523D0` | C/code generation difference |
| 101 | TACO/80151C30 | `func_80152DB4` | C/code generation difference |
| 102 | TACO/801447D0 | `func_801449D8` | Draft record/table/control recovery |
| 103 | 8004C3F0 | `srn_vram_set` | Draft record/table/control recovery |
| 104 | BUNKA_SD/80132000 | `func_80132148` | Type/declaration recovery |
| 105 | RPG_BAT/80144010 | `func_80144010` | Draft record/table/control recovery |
| 106 | TAIIKU/80133C80 | `func_80133CFC` | Draft record/table/control recovery |
| 107 | 8004C3F0 | `mod_set` | Type/declaration recovery |
| 108 | TT/80145E60 | `func_80146348` | Draft record/table/control recovery |
| 109 | TACO/801447D0 | `func_80145DC4` | Type/declaration recovery |
| 110 | 80049FF0 | `func_8004A414` | Draft record/table/control recovery |
| 111 | 80049FF0 | `func_8004A814` | C/code generation difference |
| 112 | TACO/801447D0 | `func_8014616C` | Type/declaration recovery |
| 113 | TT/80144D70 | `func_80144D70` | Draft record/table/control recovery |
| 114 | TACO/801447D0 | `func_80146754` | C/code generation difference |
| 115 | 8004E500 | `disp_string` | C/code generation difference |
| 116 | TAIIKU/80133C80 | `func_80138224` | Type/declaration recovery |
| 117 | TACO/80139170 | `func_80139424` | Draft record/table/control recovery |
| 118 | SHUGAKU/80133430 | `func_801337FC` | C/code generation difference |
| 119 | TACO/801447D0 | `func_80145650` | Draft record/table/control recovery |
| 120 | SHUGAKU/80132000 | `func_8013282C` | C/code generation difference |
| 121 | TACO/80139170 | `func_80139BCC` | Type/declaration recovery |
| 122 | DATE/8014CC40 | `func_8014D298` | Draft record/table/control recovery |
| 123 | TACO/80139170 | `func_80139FB8` | Type/declaration recovery |
| 124 | KANGEI/80137B90 | `func_80138F88` | Type/declaration recovery |
| 125 | TAIIKU/80133C80 | `func_80135F3C` | Draft record/table/control recovery |
| 126 | RPG_BAT/80141C30 | `func_80141C70` | Matched |
| 127 | RPG_BAT/80141C30 | `func_80141E38` | Matched |
| 128 | TAIIKU/80133C80 | `func_801346FC` | Draft record/table/control recovery |
| 129 | RPG_BAT/8013E300 | `func_8013E300` | Matched |
| 130 | BUNKA_SD/80132000 | `func_80132DFC` | C/code generation difference |
| 131 | TACO/801447D0 | `func_80144E3C` | Draft record/table/control recovery |
| 132 | TT/8013E850 | `func_8013E850` | Draft record/table/control recovery |
| 133 | BUNKA_SD/80132000 | `func_80132FF0` | Draft record/table/control recovery |
| 134 | 8004C3F0 | `mod_set_gpu` | Draft record/table/control recovery |
| 135 | TT/80144D70 | `func_801455C8` | Draft record/table/control recovery |
| 136 | TT/80148DB0 | `func_8014A2B4` | Draft record/table/control recovery |

## Review findings and fixes

- Removed the obsolete KANGEI `func_80138D40` NON_MATCHING alternative after its array-access version matched.
- Marked the redundant byte-lvalue cast in GYOZI `func_8013D764` FAKE: it keeps the byte store and call argument in a0; original spelling unknown. This is the only new fakematch.
- No trial padding, unused locals, pointer tricks from failed probes, or prototype overrides were retained.

## Verification and inline review

2026-10-11: reviewed the uncommitted diff against HEAD with the local `.claude/skills/code-review/SKILL.md` standards/spec axes, inline as required by the shared wave-8 brief.

Standards: all findings resolved. C89 declarations, fixed-width types, symbol placeholders, existing aggregate/bit-field views, source order, and generated main API declarations checked. The one new FAKE is labelled and tracked above. No SDK types, compiler passes, tools, config YAML, raw sources, unowned C, or staged files changed. Repeated scene switches and unknown names are preserved for byte matching, as the project standards require.

Spec: all 136 entries matched or explicitly time-boxed, with nine retained functions / 4,040 bytes. Failed candidates stay INCLUDE_ASM; scratch drafts remain outside the worktree. Owned-file duplicate and near-duplicate scans found no applicable copies. No commits, merges, pushes, or reviewer sub-agents.

Final Docker incremental ninja succeeds. All 27 current binary SHA1 values were then checked against config hashes: 27/27 OK, including actual string and jump-table contents. All nine functions pass resolved funcdiff; the two review-touched functions were rechecked after corrections. `build/headers.ok` and `build/globals.ok` exist. `sync_protos.py --check-branch`: zero new disagreements (17 known baseline); `migrate_globals.py --check`: globals OK. `git diff --check` passes; staging is empty.

Progress: `grand total   4743/6958   733632/2279368  32.2%`. Evidence logs in scratch `w8-3/`: `resolved-batch.log`, `final-build.log`, `final-guards.log`, `review-build.log`, `final-proof.log`, `dupes-regenerated.log`, `neardupes-initial.log`.

## Second deep pass

2026-10-11 09:15:09 UTC: user requested a deeper pass after committing the first nine matches (c4792c1). Reopened the ticket. Spend at least three hours on substantive iteration or exhaust the remaining list; start with ranked resolved diffs, spend roughly 10–15 minutes per candidate, run roughly five-minute permutation searches on close cases, and recover missing aggregate/local-copy types. Leave new changes uncommitted. First-pass results above are historical, not the second-pass outcome.

- [x] Rank remaining compilable drafts by resolved differing-line count.
- [x] Iterate on close candidates with asm/C comparisons and recorded experiments.
- [x] Recover missing aggregate/local-copy types and retry prioritized candidates within the three-hour pass.
- [x] Reach three hours of work (12:15:09 UTC) or exhaust the list.
- [x] Verify retained matches, run final guards, and complete inline review.

### Deep-pass experiment journal

- 09:17–09:28 UTC, SHUGAKU `func_801322F0`: 56 signedness/current-local/control-flow variants, 48 two-constant variants, 96 single narrow-constant/cast/declaration-order variants, plus a 300-second permutation run. Best remains four resolved instruction-line edits (two branch operand reversals). Narrow locals can replace the artificial padding while preserving the frame, but do not fix the branches. No source change retained.
- 09:21–09:31 UTC, SHUGAKU `func_80132BCC`: literal-6 branch proof, 64 typed-selector/control-flow forms, 12 function-pointer/unprototyped-view probes and 32 call-cast/operand-order probes, plus a 300-second permutation run. Best remains ten edits; the header's narrow callee argument adds a mask, and even an unprototyped scratch view keeps the selector in v0 with extra moves. No header override or function-pointer trick retained.
- ENDING `func_8013B510`: recovered the existing `Entry8` from neighbouring matched main functions. First trial lacked `game.h` (compile failures, not codegen evidence); corrected trials use typed members and canonical header-declared callee names. Counter/declaration-order/increment/position forms and a 300-second permutation search are in progress.

Second-pass retained matches (actual resolved diffs and unit SHA1 verified): SHUGAKU `func_80132BCC` (80 bytes, FAKE comma argument); ENDING `func_80139EC4` (120 bytes, FAKE comma argument plus existing unprototyped callee view); TAIIKU `func_80135D24` (212 bytes, recovered `TaiRandomTable`, separate column-call assignment, no FAKE). Current gain: 3 functions / 412 bytes.

`func_80132BCC` above initially appeared timed out, then the permutation output passed the resolved comparison despite its nonzero scoring-tool score. The retained source is the subsequently verified result. This is evidence to resolve and recompile best outputs, rather than treating a nonzero permutation score as definitive failure.

ENDING scroll `func_8013B510`, 09:26–09:38 UTC: 144 valid record/counter/declaration-order forms, 108 increment/position forms, 48 comma-placement forms, 30 call-type/cast forms and 300 seconds of permutation. Narrowing the wide counter explicitly reduces the best to six edits, all mask-result register differences; no source retained. Permutation candidates were rechecked with the required game-type include restored.

TACO `func_801522B0`, 09:39–09:50 UTC: predicate signedness, replacing the synthesized boolean with the actual final range test, 180 increment/layout/declaration variants, 56 dead-test probes, and 300 seconds of permutation. Best six edits: final byte-load/add/store use t2/t3 instead of t3/t4. Remains assembly.

GYOZI `func_801362A0`, 09:43–10:02 UTC: reconstructed the 30-entry callback table, varied declaration order, scalar widths and pointer/array expressions, and ran two 300-second permutation searches. Removing unnecessary pointer/index locals leaves one byte index declared before the table and matches naturally. The signed header flag needs an explicit unsigned view. Main callback address declarations were generated with `sync_protos.py --fix --only ovl/GYOZI.h`. Actual resolved diff and GYOZI SHA1 pass: fourth second-pass match, +284 bytes (cumulative 4 / 696).

The TACO `func_8013920C` best fell from 18 to two resolved edits through a scalar address view; a second 300-second permutation search found a resolved match despite a nonzero raw score. Minimizing its scheduling layout and checking the full unit are in progress.

TACO `func_8013920C`, 09:47–10:11 UTC: scalar views and load/control-flow experiments, 144 + 72 + 72 structured probes, two 300-second permutation searches, and 90 layout-minimization probes. The final match joins only `func_80059048(); previous = *(s32 *)&D_8015EDF8;` on one line, with a FAKE explanation for both address view and scheduling. Removing the cast restores 18 edits. Actual resolved diff and TACO SHA1 pass: fifth second-pass match, +392 bytes (cumulative 5 / 1,088).

TAIIKU `func_80138680`, 10:08–10:21 UTC: reconstructed the 16- and 44-byte copied word arrays and the 0x24/0x44-stride lookup records, consulted neighbouring matched `func_801388E4`, then varied local reuse, declaration order, copy initialization, K&R spelling and expressions. One reused selector/result local removes 146 differing lines; key-before-selector declaration order fixes the last stack slot. Use the real `D_80149208` record base, not a separate interior-symbol array view. A 300-second permutation run was also started on the four-edit form. Final C needs no FAKE. Actual resolved funcdiff and TAIIKU SHA1 pass: sixth second-pass match, +384 bytes (cumulative 6 / 1,472).

Deep type-repair negatives are retained only in scratch: TT `func_8013E498` (09:49–10:18, recovered state/actor/word-byte union views, local pointers, flag and constant-chain variants; best valid form 95 edits, rejected an uninitialized-pointer 59-edit form); ENDING `func_80139B48` (10:02–10:16, SDK POLY_G4 fields, widths, direct expressions, copy locals, declaration order and layout, 36 + 108 probes; best 138 edits). TT `func_80145370` (10:05–10:18) remains twenty edits after 108 type/constant/record forms, 60 K&R/formal/return/comma forms and 300 seconds of permutation. Its only differences are the phantom constant registers. No failed body is in src.

EVENT `func_800FA6A0`, 10:27–10:38 UTC: 89 callback-table/index/type/declaration-order probes reach a match. Reuse a byte index, and compare table entries directly; a named handler local changes branch operand and temporary allocation. Cleanup probes show that the unused handler declaration is unnecessary, so remove it. Generated the opaque main callback declaration and appended the forward declaration of existing void `func_800FA850`, required by the real full-file build. Actual resolved funcdiff and EVENT SHA1 pass. +228 bytes, seventh second-pass match.

TAIIKU `func_80136B18`, 10:21–10:40 UTC: 72 local record/word/halfword views and 61 aggregate/alias/layout probes recover a union: D_80149208+0x20 is a word, and +0x22 is its signed high halfword. A local record pointer reads/clamps the halfword, while word updates use the constant record view. This reproduces the original independent word relocations, hoisted record base and post-clamp reloads without FAKE or a header override. Extended the existing new TaiSlot24 view rather than adding another overlapping record type. Actual resolved funcdiff and TAIIKU SHA1 pass. +156 bytes, eighth second-pass match; cumulative 8 / 1,856.

Checkpoint verification: Docker ninja passed all 27 unit SHA1 checks after six gains; both new units also pass. Real verification builds now run sequentially after a concurrent Ninja deps-log race. A manual split for fresh duplicate classification initially omitted rodata/data-piece regeneration, which was corrected by running the full split postprocessing before rebuilding. Owned-file duplicate scans applied zero copies. Their reported inventory includes stale matching/nonmatching asm copies after split; funcdiff explicitly filters those stale files. The exploratory SDK POLY_G4 header addition was discarded because its helper did not match.

Additional sustained negatives: DATE `func_8014DCCC` (09:43–10:34, seven manual helpers and two 300-second searches, best four edits: two branch operand swaps); TAIIKU `func_80135DF8` (09:43–09:55 and 10:26–10:28, 192 table/record probes, 72 constant probes and 54 row-pointer probes, best valid ordinary index form 24 edits); TAIIKU `func_80138800` (10:14–10:22, 108 local reuse/layout/expression forms and 300-second search, best 22 edits). All remain assembly.

TAIIKU `func_801368B8` (10:35–10:49) has a recovered 0x24-byte counter record and corrected store destinations. 108 + 72 probes leave 26 differing lines from shared `$at` bases and scheduling. Its object has no owned data range, so the own-data remedy cannot be applied within the batch's permitted edits. ENDING `func_80134B18` (10:38–10:53) received four width/declaration/control-flow grids (108 + 108 + 54 + 108 probes) and 300 seconds of permutation; best 34 edits. TT `func_8014A814` (10:41–10:54) received 96 record/return/layout/order probes and 300 seconds of permutation; best 44 edits, still the five phantom constant registers. No failed source retained.

In progress from 10:46–10:55: ENDING `func_80139F3C` brightness reload repair, TAIIKU `func_80137090` copied word/halfword tables and actor fields, TT `func_80149108` typed state record, and both EVENT brightness loops. The EVENT pointer-and-chain form reduces `func_800FA194` to four edits (store order); the TT typed state field removes a duplicate constant and reduces `func_80149108` to twelve edits.

EVENT `func_800FA194` (200 bytes), 10:54–11:00: the existing main `Rec24` view, a local record-base pointer, and the chain `unk_14 = unk_15 = unk_16 = brightness` fix both register allocation and original reverse store order. Removed unused draft locals and restored ordinary formatting; real resolved comparison and EVENT SHA1 pass. Ninth second-pass gain (cumulative 9 / 2,056).

EVENT `func_800FA0D0` (196 bytes): the same record-and-chain remedy leaves six edits. Explicit signed-word locals `before` and `after` for the callback index preserve the original word stack slot and reload order. Scratch and actual full-file resolved comparisons pass; full-unit SHA1 checking is in progress. A 300-second search was also started on its prior six-edit form.

The EVENT `func_800FA0D0` full-unit SHA1 passed: tenth gain, cumulative 10 / 2,252. The permutation search independently found a matching unsigned-word saved-index form; the retained signed before/after locals are the clearer manually verified form.

BUNKA_SD `func_80132DFC` (500 bytes), 10:59–11:07: add the missing explicit case 4 alongside default, and use `GameState.unk_F80` for the flag. This changes the draft's branch chain into the original five-entry jump table and removes 80 differing lines. The remaining object diff is an unsupported `R_MIPS_PC16` local-label relocation. Built an all-original-assembly reference object in scratch, retained the original floating rodata includes, linked it with the unchanged unit objects, and verified its SHA1 equals the rebuilt unit (`173ac942ad4efbbbb176f8522fc7d273540169b0`). `funcdiff --resolve --built <built ELF> --expected <original-reference ELF> --no-build` reports MATCH after linking. The ordinary unit SHA1 also passes. Eleven gains / 2,752 bytes. A 300-second permutation search runs on the adopted body; its scratch score is not the final linked proof.

SHOUGATU `func_8013315C` (172 bytes), 11:06–11:11: restore the pre-increment comparison (`GameState.unk_1104.w++ == 0`), pointer-to-byte-array and pointer-to-word-array indexing, and the documented s32/no-return idiom. The m2c draft had incorrectly compared the incremented value and invented negative aggregate members. Removed all three unused draft locals and used ordinary formatting. Actual resolved diff and SHOUGATU SHA1 pass. Twelve second-pass gains / 2,924 bytes.

Checkpoint at twelve gains: the complete Docker Ninja build passed all 27 unit SHA1 checks. Fresh EVENT, BUNKA_SD and SHOUGATU splits included both rodata/data piece postprocessing. Owned-file duplicate application found zero plans; near-duplicate report found no fillable unit. Failed experiments remain exclusively in scratch.

Further sustained negatives: TT `func_80149108` (10:54–11:12, 288 + 192 scalar/record/control-flow probes and 300 seconds of permutation) remains twelve edits from constant order. GEKO `func_80144198` (11:04–11:19, recovered twelve-halfword calendar table, 624 year/month/selector/K&R forms) remains 61 edits, including a frame mismatch. TAIIKU `func_80136DA4` (11:04–11:19, 72 + 90 scalar/pointer/record/return forms) remains forty edits from the first global-load register and later scheduling. BUNKA_SD `func_80134418` (11:06–11:20, recovered color records, 144 + 108 forms and 300 seconds of permutation) remains fourteen edits. No source retained.

ENDING `func_80139F3C` (10:46–11:26) received 72 reload/type forms, 72 single-brightness reuse forms, two 300-second searches and 108 expression/return/layout forms. A single byte variable reduces the valid draft from 37 to eighteen edits; permutation reaches ten, with brightness held in v0 instead of a1. TAIIKU `func_80137090` (10:49–11:26) received copied-table and actor types, 64 + 72 + 120 valid forms and 54 corrected global-view probes; best 26 edits, still copy-slot and height/flag register differences. Scratch header-macro failures before fixing the harness include span are compile failures, not matching evidence.

TT `func_80149400` and `func_8013EC64` (from 11:17): recovered the 0x26-byte control record and halfword input tables; 72 valid forms each, then 96 further alias/local/return forms on `func_80149400`. Best ordinary forms are 43 and fifty edits respectively; volatile scratch views are not retained. New aggregate attempts from 11:27: EVENT `func_800FAA04` uses the existing main Rec24; TAIIKU `func_80137440` uses a 0x44-byte motion record. The latter starts thirteen edits away and receives a 300-second search alongside type/expression/layout experiments. `func_80137228` is being repaired against its actual overlapping 0x24-byte word/high-halfword record and 0x44-byte actor stride.

Latest type-repair timeboxes: EVENT `func_800FAA04` (11:27–11:42) received 144 + 108 + 108 record/halfword/division/increment/layout forms; best valid form 53 edits, with promoted constant/division and increment scheduling differences. Some deliberately diagnostic random/divisor reuse forms alter semantics and were rejected. TAIIKU `func_80137228` (11:32–11:45) received 192 corrected typed-record forms and 192 local-reuse/initialization/condition forms; best 76 edits after reordering initializers. The first missing-main-symbol grid is compiler-failure evidence only. TAIIKU `func_80137440` (11:27–11:49) received 144 + 120 + 90 forms, 24 overwritten-parameter probes, and two 300-second searches; best nine edits. No prototype/parameter experiment retained.

BUNKA_SD `func_80132708` (11:38–11:51) received 162 counter/view/explicit-case/layout forms and 120 offset/shift/increment forms; best 51 edits. GYOZI `func_801347D4` (11:39–11:56) received 128 existing-record/flag-bit forms, 96 mask/base/operand forms, 72 counter/loop/base forms and 300 seconds of permutation. A byte result, counter-before-pointer init and corrected sum operand order reduce it to 26 edits. Main `func_80049FF0` (11:41–11:54) received 72 SDK-line/K&R/color/operand forms and 72 parameter-declaration/address-view/local-color forms; best 58 edits. SDK type stayed scratch-only. These failed bodies remain assembly. Added only two previously absent T-0018 rows (TAIIKU motion and GYOZI pointer/base); retained historical rows and appended their newly matched resolutions.

TT `func_8014AB48` (from 11:51) repairs the 0x54-byte record, unsigned state/timer halfwords, direct pre-decrement tests, and explicit u32 loop bound. 144 initial forms reduce 77 to 36 edits; 96 expression/control forms reach two. Further bound placement/width/layout probes reach a match when `limit = 12` is the `for` initializer. Two permutation searches were run, the second independently resolves a matching score-zero scratch output. Removed both unused decrement locals and replaced provisional fields with offset names; the cleaned scratch result still matches. Real full-file and unit verification is in progress.

Inline review correction: guarded the appended overlay type blocks and documented the random table's member offset. All 27 unit SHA1 checks pass after the header changes. Rebuilt the BUNKA_SD linked original reference using its preserved original assembly with attached strings/table, since the new split's matching asm omits those payloads; reference SHA1 still agrees and linked funcdiff still reports MATCH. No generated asm was edited.

### Second-pass final results and review

The minimum three-hour work interval elapsed at 12:15:09 UTC (start 09:15:09). The pass stops on that time criterion, not exhaustion of all remaining drafts. Thirteen additional functions / 3,344 bytes are verified and left uncommitted. Including the committed first pass, this batch has 22 matches / 7,384 bytes. Failed bodies and unsuccessful type/prototype experiments remain exclusively in scratch; their source functions still use INCLUDE_ASM.

| Unit | Function | Bytes |
|---|---|---:|
| SHUGAKU | `func_80132BCC` | 80 |
| ENDING | `func_80139EC4` | 120 |
| TAIIKU | `func_80135D24` | 212 |
| GYOZI | `func_801362A0` | 284 |
| TACO | `func_8013920C` | 392 |
| TAIIKU | `func_80138680` | 384 |
| EVENT | `func_800FA6A0` | 228 |
| TAIIKU | `func_80136B18` | 156 |
| EVENT | `func_800FA194` | 200 |
| EVENT | `func_800FA0D0` | 196 |
| BUNKA_SD | `func_80132DFC` | 500 |
| SHOUGATU | `func_8013315C` | 172 |
| TT | `func_8014AB48` | 420 |

TT's final full-file resolved comparison reports MATCH (`deep-verify-AB48-sequential.log`). The final sequential Docker Ninja build completed all 521 actions with **27/27 SHA1 OK**, including TT, strings and jump-table bytes (`deep-final-sequential-build.log`); independent checks also pass (`deep-final-sha1-13.log`). Eleven earlier ordinary resolved matches are in `deep-final-resolved.log`; the BUNKA_SD linked proof is in `deep-final-linked-2.log`. `sync_protos.py --check-branch` reports zero new disagreements (17 known baseline), and `migrate_globals.py --check` reports globals OK. Ninja checked headers and globals. Progress: `grand total   4756/6958   736976/2279368  32.3%`.

Inline code review against every applicable CODING_STANDARDS.md checklist item is complete. The added C is C89, uses fixed-width members, documented layouts and original symbol names, and replaces only owned INCLUDE_ASM bodies in address order. Main declarations were generated in main_api.h; overlay additions are appended and guarded. SDK types, tools, configs, generated assembly and unowned code are unchanged. Three retained FAKE idioms (SHUGAKU comma, ENDING comma, TACO scalar view/source-line scheduling) are marked and tracked above. Review findings concerning guards, the random table offset comment, unused TT locals, and TT byte-pointer call views were resolved before verification. No unresolved findings. `git diff --check` passes and staging is empty. Two new T-0018 rows were appended; prior rows retain their historical evidence with an appended resolution note. Ticket and kanban return to Done after this review.

The final verification was deliberately sequential after overlapping Ninja invocations raced on the dependency log. No source workaround was retained. Remaining non-T-0018 blocker: TAIIKU `func_801368B8` needs shared `$at` data ownership, but its object has no owned data range. Tooling issue: object-level funcdiff cannot resolve the local-label PC16 relocation in BUNKA_SD; the independently checksummed original-reference linked comparison verifies the match.

Final exact-owned-file `dupes.py --apply --files` scan found zero plans and applied zero copies (`deep-final-dupes-owned-13.log`). Header/global stamp files exist, the final whitespace check passes, and staging remains empty. No Git write operation was performed.
