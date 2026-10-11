---
id: T-9540
title: "Wave 8: list 4"
status: Done
assignee: w8-4
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match the 176 functions assigned to wave-8 list 4 within its 36 owned C files.

## Acceptance criteria

- [x] Deep second pass: sustained experiments and five-minute near-miss permutation searches.
- [x] Retained C passes resolved function diffs and the 27 unit SHA1 checks.
- [x] Header and GameState checks pass; inline standards review recorded.

## Notes

Follow the shared wave-8 brief. Leave changes uncommitted for the orchestrator.

## Deep-pass result

- Matched **10 functions, 1628 bytes**; all retained definitions pass fresh resolved funcdiff and their unit SHA1 checks. Remaining drafts are reverted.
- Deep pass began at 09:07:54 UTC; final checks completed at 12:27:26 UTC. Close candidates received sustained control-flow/type/layout experiments and 300-second permutation searches.
- Retried more than 36 aggregate/local-copy drafts using neighboring matched functions; only verified table, flag and draw-record views remain.
- Final incremental Docker ninja: **27/27 SHA1 OK**, with headers/globals stamps present. Exact-owned dupes --apply --check: zero plans/copies.
- Final branch check: zero new disagreements, seventeen known. GameState check and git diff --check pass.
- Progress: `grand total   4744/6958   731240/2279368  32.1%`.
- Nine T-0018 rows added across both passes; four are resolved by this work and preserved as history.
- Other blocker: InitMouse has no owned data range for the shared-$at pointer table. Tooling bug: m2c collapses its second table-element store onto element zero; raw permuter score alone also missed the initializer's resolved match.
- Evidence: scratchpad/w8-4/deep-final-ninja.log, deep-final-evidence.log, deep-final-dupes.log, two-real.log and last-clean-real.log. Changes remain uncommitted.

## Historical first-pass result

- Matched: **0 functions, 0 bytes**; all 176 assigned functions time-boxed.
- Three T-0018 rows added, with measured temporary/constant symptoms and bounded retries.
- Owned-files duplicate check: zero plans, zero copies.
- Final Docker ninja: **27/27 SHA1 OK**; headers/globals stamps present.
- `sync_protos.py --check-branch`: zero new disagreements, 17 known; `migrate_globals.py --check`: OK.
- `ninja progress`: `grand total   4734/6958   729612/2279368  32.0%`.
- Concrete other blocker: InitMouse needs an owned data range for its shared-$at pointer table; none is assigned to 80059710.
- Tooling observations: register_shapes has no positional-function CLI; m2c's InitMouse draft collapses the second table-element store onto element 0.
- Evidence: the shared scratchpad's w8-4 directory contains drafts.jsonl, comparison/retry/permuter logs and final-checks.log.

## Comments

- 2026-10-11: Read wiki index, workflow, standards, shared brief and wave-7 register clusters. Started baseline Docker configure/build; asm was absent initially.
- 2026-10-11: Completed the ordered 176-function draft/comparison pass, repaired byte-buffer and GameState views where possible, ran three bounded permuter attempts and five additional source-idiom retries. No candidate survived resolved verification; all experimental source and header changes were restored. Added three bounded-recheck T-0018 rows; final duplicate/build/branch checks and inline review remain.
- 2026-10-11: Exact-owned-files dupes --apply --check produced zero plans and zero copies. Moved to In Review for inline standards/spec review and final verification. No source, header, config or tooling edits remain.
- 2026-10-11: Inline code/spec review against CODING_STANDARDS.md and the shared brief completed with no unresolved findings. Confirmed all source/header/config/tool experiments restored, no raw or generated assets added, old wiki rows/log entries preserved, three new rows describe measured symptoms, ticket/board/index synchronized and git diff --check clean. Final Docker checks pass as recorded above. Moved to Done; changes remain uncommitted for the orchestrator.

- 2026-10-11 09:08 UTC: Reopened after user review found the first pass insufficiently deep. Second pass ranks resolved differing lines, repairs aggregate types, and runs for at least three hours unless the list is exhausted. First-pass results above are historical.

- Deep pass gain: SHUGAKU func_80138DD8, 104 bytes. A u8 local for the last compared byte fixes operand order. Resolved diff MATCH and SHUGAKU SHA1 OK; multiline cleanup being verified.

- Deep pass gain: EVENT func_80102118, 172 bytes. Reused neighboring FnTbl39 and declared an index before the copied table. funcdiff --resolve MATCH and EVENT SHA1 OK. Running total: 2 functions, 276 bytes.

- Deep pass gain: OPTION func_8013357C, 108 bytes. An s32-return counted loop retains the otherwise dead 0x60 initialization in v0; no explicit return is added. Resolved diff MATCH and OPTION SHA1 OK. Running total: 3 functions, 384 bytes.

- Deep pass gain: main func_800853FC, 156 bytes. Scoped promoted-index view of the existing brightness alias and a u8 selector loaded after the fade call. Resolved diff MATCH and main executable SHA1 OK. Running total: 4 functions, 540 bytes.

- Deep pass gain: main func_80085368, 148 bytes. Reapplied and reverified after adding the existing brightness alias's linker address. Resolved diff MATCH and main executable SHA1 OK. Running total: 5 functions, 688 bytes.

- Deep pass gain: main func_8008585C, 140 bytes. Corrected the word-table indexing and used a signed halfword loop counter. Resolved diff MATCH and main executable SHA1 OK. Running total: 6 functions, 828 bytes. The scalar-backed array view needs its final FAKE annotation/type-view cleanup.

- Deep pass gain: SHUGAKU func_80139D40, 332 bytes. Defined the 68-entry copied function table, used u8 index/kind locals, and removed m2c's spurious second indirect-call argument. Resolved diff MATCH and SHUGAKU SHA1 OK. Running total: 7 functions, 1160 bytes.

- Deep pass gain: OPTION func_80136F3C, 260 bytes. Recovered a minimum word/flag aggregate view at D_800E7D10; the struct member index recovers the original peeled-loop base and +0x14 displacement. Resolved diff MATCH and OPTION SHA1 OK. Running total: 8 functions, 1420 bytes.

- Deep pass gain: normal_date_girl_in_init, 112 bytes. A marked FAKE s32/no-return definition and alias of a local zero retain the missing v0 initialization. The raw-score-20 permuter output has zero resolved differences; fixed-width version passed real funcdiff and main executable SHA1. Generated canonical prototype now matches the definition.

- Deep pass gain: SHOUGATU func_8013AE0C, 96 bytes. A marked FAKE dead raw-byte test plus reversed compared operands closes the last two lines. Resolved diff MATCH and SHOUGATU SHA1 OK. Running total: 10 functions, 1628 bytes.

- 2026-10-11: Deep pass ran from 09:07:54 UTC through 12:15 UTC and final verification continues. Moved to In Review. Inline review covers all ten new definitions, three documented aggregate/table layouts, scoped main-API views, return artifacts and the existing-address alias. One duplicate FAKE comment needs removal before the final resolved comparisons; all failed source/header experiments are restored. Final full-unit checks remain.

- 2026-10-11 12:27 UTC: Inline review against CODING_STANDARDS.md and the shared brief completed. Resolved the duplicated comment and the new EVENT row's file typo. Verified C89/block-top declarations, fixed widths, documented struct offsets/sizes, unchanged SDK declarations, explicit FAKE reasons/ticket references, generated canonical prototypes with justified scoped views, and address-order replacements. All ten fresh resolved comparisons and all 27 unit SHA1 checks pass; header/global/branch guards are clean against the known baseline. Changed files are limited to owned C, permitted declarations/symbol alias and this ticket's wiki records; no tooling/raw/generated assets are added and no git mutations were made. No unresolved findings. Moved to Done.
