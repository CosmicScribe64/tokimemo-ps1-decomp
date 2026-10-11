---
id: T-9730
title: "Wave 9: list 3"
status: Done
assignee: w9-3
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match as many functions as possible in wave9-list-3.txt, restricted to its 35 owned C files. Follow the shared wave-9 brief: first-pass drafts, then ranked deep attempts and type repairs; work at least four hours or deeply attempt every function.

## Acceptance criteria

- [x] Retained C verified with Docker funcdiff and binary checksums.
- [x] Failed drafts restored to INCLUDE_ASM; new allocation cases recorded.
- [x] Main prototypes and GameState migration checks pass.
- [x] Final incremental ninja passes all 27 checksums and header/global guards.
- [x] Inline review against CODING_STANDARDS.md completed.

## Notes

Assigned list: 146 functions / 96,724 bytes across 35 files. Scratch: /private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad/w9-3/.
No commits or tooling/config YAML changes; leave edits for the orchestrator.

## Comments

2026-10-11: Read wiki index, shared wave-9 brief, local instructions, coding standards, workflow, register-allocation notes, and relevant matching history. Started baseline configure/build through tools/docker.sh at approximately 15:00 UTC.

First 20 quick trials: no retained matches. Nine compiled bodies show real resolved instruction differences; the rest need type/field repair. Saved every draft and build/diff result in scratchpad/w9-3/firstpass-2.jsonl. Closest initial code is func_8007B2B4 (17 changed diff lines), then audio reset and NAME_ENT fade. Failed edits and temporary declarations restored.

First-pass match: NAME_ENT:func_8013FE40, 344 bytes; resolved funcdiff and unit SHA1 pass.

Draft generation completed for all 146 functions. The first retained C is NAME_ENT:func_8013FE40 (344 bytes), verified with resolved funcdiff and build/ovl/NAME_ENT.ok. The quick pass continues; temporary edits to the current trial are expected until that function is accepted or restored.

Quick pass completed for all 146 assigned functions: 1 retained match (344 bytes), 39 compiled real diffs, and 106 drafts needing declaration/aggregate repair. Saved all m2c bodies and outcomes in scratchpad/w9-3. A full incremental ninja build passed all 27 unit checksums after restoring failed trials. Beginning ranked manual iterations and five-minute permuter runs.

Deep audio dispatcher: 31 manual parameter/return/selector/aggregate variants and a working 300-second permuter run did not close the $v0/$v1 selector gap (best raw score 110). Restored INCLUDE_ASM. Gradient line: typed prefix and K&R/s32 coordinate/volatile-stack-byte variants have the original 0x30 frame; remaining differences concern colour parameter promotion and scheduling. A 300-second search reached raw 395, no match retained.

Deep-pass match: main:func_80048DD0; s32 counter, i < count && i < 160; resolved diff and unit SHA1 pass.

Deep-pass match: BUNKA_SD:func_801322EC; void 10 record flags, counter 0, positive outer mask; resolved diff and unit SHA1 pass.

Persistent allocation gaps recorded for main func_8007B2B4, main func_800422C8 and BUNKAKEN func_801428D0 after typed manual variants and bounded searches. register_shapes identifies selector/compare-chain, pointer/base/CSE, and selector/compare-chain clusters respectively. Failed bodies and trial declarations restored. Remaining pending functions will only receive rows after their final attempt fails.

Deep-pass match: RPG_BAT:func_8013B2F0; s32 decrement; FAKE dead counter test preserves v1 promotion; resolved diff and unit SHA1 pass.

RPG_BAT func_8013B2F0 retained (580 bytes) after eight return/decrement forms and the bounded permuter's verified zero. The cleaned direct-decrement body needs the marked empty counter test to keep v1 promotion. Preliminary whitespace review found m2c's trailing spaces on switch/case labels; trim them and re-verify during the final review pass.

Deep-pass match: BUNKA_SD:func_80132148; void background script addresses, explicit fifth case True; resolved diff and unit SHA1 pass.

Matching pass time-boxed at 19:37 UTC after approximately 4 hours 37 minutes. Retained five functions / 1,828 bytes. Final brightness prototype/include repair compiles but remains 34 lines after a 300-second search (raw 335); restored. Eighteen event-local/shared-goto/natural-if/switch record-zero variants improve 44 to 31 lines but remain nonmatching; all source/declarations restored. Twelve T-0018 symptom rows appended. Beginning final source cleanup, fresh diffs, scoped duplicate scans and standards review.


## Final result and inline review

2026-10-11, 19:43 UTC: completed approximately 4 hours 43 minutes of matching and verification. All 146 assigned functions received m2c/build/diff quick trials; ranked deep attempts included manual control/type/promotion repairs and bounded permutation. Failed C and declarations are restored. Retained gains:

| Unit | Function | Bytes | Evidence |
|---|---|---:|---|
| NAME_ENT | `func_8013FE40` | 344 | fresh resolved MATCH; NAME_ENT SHA1 |
| main | `func_80048DD0` | 168 | fresh resolved MATCH; main SHA1 |
| BUNKA_SD | `func_801322EC` | 316 | fresh resolved MATCH; BUNKA_SD SHA1 |
| RPG_BAT | `func_8013B2F0` | 580 | fresh resolved MATCH; RPG_BAT SHA1 |
| BUNKA_SD | `func_80132148` | 420 | fresh resolved MATCH; BUNKA_SD SHA1 and five-entry jump table |

Total: **5 functions / 1,828 bytes**. Final `ninja progress`: `grand total   4875/6958   778488/2279368  34.2%` (baseline 4870/6958, 776660/2279368). Twelve T-0018 case rows appended. Scoped dupes/neardupes apply zero candidates, repeated after refreshing the four changed units' generated assembly classification.

Inline review against every CODING_STANDARDS.md checklist item is complete, with no outstanding findings. C89/fixed-width code remains in original function order; unmatched functions retain INCLUDE_ASM, no NON_MATCHING blocks are added. Existing SDK/GameState types and views are preserved. New address-only BUNKA script arrays and pointers have one declaration home in its guarded overlay header; main_api.h is unchanged after rejected trials. No speculative aggregate types, local main forwards, flags, toolchain passes, symbol/config changes, or debug code are retained. No copyrighted assets are staged, and no commits, merges, pushes, host installs, or shared Docker cleanup were performed.

One marked fakematch remains: RPG_BAT `func_8013B2F0` uses an s32 fall-through view and an empty counter test in the first case 1 to preserve `$v1` allocation; original source is unknown. The comment states FAKE, reason and T-9730. Whitespace findings and the incomplete explanatory comment are resolved and reverified; git diff --check is clean.

Final Docker proof: sync_protos --check-branch passes (zero new disagreements, 17 known); migrate_globals --check passes; full incremental ninja (no -k) succeeds. All 27 original binaries independently pass sha1sum -c; build/headers.ok and build/globals.ok exist. Fresh resolved diffs pass all five gains after cleanup and generated-split refresh. Logs are in scratchpad/w9-3/final-source-check-refreshed.log and final-verification.log.

Top non-T-0018 blockers: original loop induction/unrolling (bank table/record scan and texture-page setup) and unrecovered aggregates in the larger drafts. Record-zero's event-local and duplicated natural clamp repairs still leave mixed control/allocation differences, so it is not logged as a pure allocation case.

Tooling reports for the orchestrator: permute.py -j 2 lets tools/queue.py shadow standard-library queue in multiprocessing workers; its --time limit also excludes definition-stripping setup, which hung on the large NAME_ENT object. Scratch-only wrappers preload standard queue and use bounded definition parsing; shared tools remain unchanged.
