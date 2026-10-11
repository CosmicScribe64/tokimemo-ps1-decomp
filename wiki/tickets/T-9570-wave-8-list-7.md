---
id: T-9570
title: "Wave 8: list 7"
status: Done
assignee: w8-7
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Current result

Done after the deep second pass: 14 matched functions, 4,628 bytes; 12 functions and 3,788 bytes gained beyond the first two. All 129 assigned functions have attempts recorded, all 36 aggregate rejects were reconstructed and retried, and the deep ledger records valid comparisons for 62 functions. More than three hours of deep work completed. Changes remain uncommitted.

Final progress: `grand total   4748/6958   734240/2279368  32.2%`.

## Goal

Time-box the 129 functions in wave8-list-7.txt, preserving byte identity and file ownership.

## Acceptance criteria

- [x] Work list completed or every remaining function time-boxed.
- [x] Kept C verified with funcdiff and all 27 unit checksums.
- [x] Header/global checks and inline standards review pass.

## Notes

Follow wave8-brief.md. No commits, merges, tool changes, or reviewer sub-agents.

## Comments

2026-10-11: Read wiki index, shared brief, standards, workflow, local instructions and wave-7 register allocation findings. Baseline build started.

Initial scoped duplicate scan: 0 applicable plans. First 12 drafts attempted with bounded ANSI/K&R variants; rejected candidates restored. Type-repair follow-ups and remaining list continue. Experimental full-build failure was in the GYOZI candidate; the default checksums will be rerun after restoration or verification.

Positions 1–55 have first-pass outcomes recorded in scratchpad/w8-7/results-1.jsonl through results-3.jsonl. Additional ANSI/K&R/color-local variants of GYOZI:func_80142A8C and input-parameter/return/goto variants of OPTION:func_8013A550 were rejected. OPTION permuter: 60-second limit, best score 560, no verified score-zero candidate. Both remain INCLUDE_ASM.

Matched `KANGEI:func_80134BC4` (492 bytes); real object `funcdiff --resolve` passed.

All 129 list entries now have bounded first-pass outcomes (128 JSON records plus the separately handled GYOZI:func_80142A8C). First-pass total: one verified match, 91 rejected build/diff candidates, 36 drafts needing larger type/copy repair. Final follow-ups, scoped near-duplicate check, regression checks and review remain.

Matched `TT:func_8014BF54` (348 bytes), relocation-resolved object verification passed.

Four T-0018 rows appended after consulting the wave-7 clusters: TAIIKU:func_80147A6C, GYOZI:func_8013AC38, SHOUGATU:func_80137DE4, BUNKASAI:func_8013B190. Scoped near-duplicate report: zero fillable copies. Tooling limitation: register_shapes.py takes no function argument and its full inventory fails on missing historical VALEN:func_80133670 asm; no tooling edits made.

## Inline review

Standards axis: reviewed the uncommitted diff against CODING_STANDARDS.md. The two C replacements retain their original names and address order, use C89 and fixed-width types, introduce no SDK types/prototypes, overrides, padding, NON_MATCHING blocks or fakematches. TT uses the existing byte-pointer view of its opaque slot, matching neighboring functions. Main/overlay declarations and raw sources are unchanged. All rejected files are restored; only the two owned C files remain changed.

Spec axis: every one of the 129 list entries has an outcome in scratchpad/w8-7/, including the separately handled top GYOZI function. Work stayed in the 35 owned C files during experiments, with final C retained only in KANGEI/80134120 and TT/8014AF00. No tools/configs changed, and no commits, merges, pushes, sub-agents or Docker cleanup occurred. The final plain ninja run exits zero with 27/27 SHA1 OK; build/headers.ok and build/globals.ok are present. Fresh real funcdiff checks pass for both kept functions after local-name cleanup. git diff --check passes. No unresolved review findings.

## Final result

- Matched KANGEI:func_80134BC4 (492 bytes) and TT:func_8014BF54 (348 bytes): 2 functions, 840 bytes.
- All remaining 127 functions time-boxed and left INCLUDE_ASM; no experimental edits retained.
- Full Docker ninja: exit 0, 27 SHA1 OK lines. Header/global stamps present; fresh sync_protos --check-branch: 0 new disagreements, 17 known, exit 0; migrate_globals --check: globals OK. Both final funcdiff checks pass. Inline review has no unresolved findings.
- Progress: `grand total   4736/6958   730452/2279368  32.0%`.
- Four T-0018 rows added. Largest other blocker: missing aggregate/local-copy types in 36 first-pass drafts. OPTION epilogue layout and the handwritten TACO rotation helper also remain open.
- Tool issue: register_shapes inventory fails on the missing historical VALEN func_80133670 asm row; the brief's per-function argument is unsupported. No tools changed.
- Done after inline standards/spec review. Changes remain uncommitted for the orchestrator.

## Continuation audit

2026-10-11: Re-read the exact assigned list after the continuation request. Its 167 lines comprise 129 function entries, 35 file declarations and 3 comments. Compared every entry in order against the seven batch-result files and separate top-function experiments: no unattempted functions remain. The 36 type-repair outcomes are bounded m2c attempts; the other rejected drafts were compiled/diffed, with near candidates receiving additional experiments. The TT follow-up converted one initial rejection into the second match. No new C edits or commits in this audit.

Evidence files below are under scratchpad/w8-7/.

| Order | Function | Final outcome | Attempt evidence |
| --- | --- | --- | --- |
| 1 | GYOZI:func_80142A8C | Time-boxed: ANSI/K&R, color/local order and primitive-view variants differ | `top-final.jsonl` |
| 2 | main:menu_bar_color | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 3 | TAIIKU:func_80147A6C | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 4 | OPTION:func_8013A550 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 5 | TAIIKU:func_801482BC | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 6 | GYOZI:func_8013AC38 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 7 | TAIIKU:func_80146994 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 8 | main:strSetDefDecEnv | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 9 | main:menu_check_2 | Time-boxed: aggregate/local-copy type repair | results-1.jsonl |
| 10 | TT:func_8014C5F8 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 11 | main:func_80050324 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 12 | main:menu_check_1 | Time-boxed: compile/code-generation mismatch | results-1.jsonl |
| 13 | TAIIKU:func_80147120 | Time-boxed: aggregate/local-copy type repair | results-2.jsonl |
| 14 | TAIIKU:func_801485C4 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 15 | main:menu_bar_show | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 16 | TAIIKU:func_80147210 | Time-boxed: aggregate/local-copy type repair | results-2.jsonl |
| 17 | main:menu_set | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 18 | TAIIKU:func_80147928 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 19 | TACO:func_8015CF30 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 20 | TT:func_8014CC04 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 21 | SHOUGATU:func_80137DE4 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 22 | TT:func_8014CFEC | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 23 | TT:func_8014BF54 | Matched; fresh relocation-resolved verification passed | results-2.jsonl |
| 24 | TACO:func_801511C0 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 25 | TACO:func_80138964 | Time-boxed: aggregate/local-copy type repair | results-2.jsonl |
| 26 | DATE2:func_80136794 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 27 | TT:func_8014B088 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 28 | TAIIKU:func_80146A60 | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 29 | TAIIKU:func_8014666C | Time-boxed: aggregate/local-copy type repair | results-2.jsonl |
| 30 | TT:func_8014CDBC | Time-boxed: compile/code-generation mismatch | results-2.jsonl |
| 31 | main:func_80052060 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 32 | main:x_taku_menu_set | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 33 | BUNKASAI:func_8013B190 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 34 | GYOZI:func_801428EC | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 35 | TT:func_8014BB30 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 36 | TAIIKU:func_80147318 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 37 | DATE2:func_801338F8 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 38 | TAIIKU:func_80148370 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 39 | main:strNextVlc | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 40 | main:menu_girl_taku_set | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 41 | main:func_80053418 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 42 | TAIIKU:func_80146D18 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 43 | KANGEI:func_80135004 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 44 | TACO:func_8014F820 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 45 | RPG_BAT:func_8014C360 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 46 | RPG_BAT:func_8013BBF8 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 47 | main:func_8006190C | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 48 | TAIIKU:func_8014607C | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 49 | RPG_BAT:func_8014AB84 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 50 | TACO:func_80138B2C | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 51 | TAIIKU:func_80147B14 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 52 | TAIIKU:func_80146338 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 53 | BUNKAKEN:func_80140F60 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 54 | SHOUGATU:func_8013A688 | Time-boxed: compile/code-generation mismatch | results-3.jsonl |
| 55 | main:func_80052F68 | Time-boxed: aggregate/local-copy type repair | results-3.jsonl |
| 56 | main:strCallback | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 57 | GEKO:func_80138E10 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 58 | RPG_BAT:func_8014AC74 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 59 | main:strNext | Time-boxed: aggregate/local-copy type repair | results-4.jsonl |
| 60 | RPG_BAT:func_8013C32C | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 61 | main:sndisp | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 62 | GEKO:func_80133AA0 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 63 | DATE:func_80144720 | Time-boxed: aggregate/local-copy type repair | results-4.jsonl |
| 64 | TT:func_8014B7A8 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 65 | GEKO:func_801355E8 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 66 | DATE2:func_80136A90 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 67 | DATE2:func_80136860 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 68 | TACO:func_8014F5F0 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 69 | main:xa_wait | Time-boxed: aggregate/local-copy type repair | results-4.jsonl |
| 70 | DATE2:func_801330D4 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 71 | TACO:func_8014A480 | Time-boxed: aggregate/local-copy type repair | results-4.jsonl |
| 72 | GEKO:func_801358CC | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 73 | TT:func_8014B274 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 74 | SHOUGATU:func_80137E54 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 75 | SHOUGATU:func_8013288C | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 76 | BUNKA_SD:func_80135F38 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 77 | GYOZI:func_80139E4C | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 78 | main:func_80062210 | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 79 | TT:func_801410A0 | Time-boxed: aggregate/local-copy type repair | results-4.jsonl |
| 80 | main:func_8005715C | Time-boxed: compile/code-generation mismatch | results-4.jsonl |
| 81 | KANGEI:func_80134724 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 82 | TAIIKU:func_80147F58 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 83 | BUNKA_SD:func_80135CD4 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 84 | GEKO:func_80137120 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 85 | GEKO:func_801372D4 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 86 | TT:func_8014C84C | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 87 | TAIIKU:func_80145370 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 88 | main:func_80062948 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 89 | TACO:func_8014E5F8 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 90 | SHOUGATU:func_801326F8 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 91 | GYOZI:func_801421B4 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 92 | RPG_BAT:func_8014ADD0 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 93 | RPG_BAT:func_8014BFE4 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 94 | RPG_BAT:func_80149C5C | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 95 | BUNKA_SD:func_80137620 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 96 | BUNKA_SD:func_80136578 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 97 | main:get_h_tokimeki | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 98 | TAIIKU:func_80147594 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 99 | TACO:func_8014B8B4 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 100 | RPG_BAT:func_80152384 | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 101 | RPG_BAT:func_8014A3DC | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 102 | TT:func_8014C2F8 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 103 | RPG_BAT:func_8014B294 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 104 | BUNKA_SD:func_801373D0 | Time-boxed: compile/code-generation mismatch | results-5.jsonl |
| 105 | main:func_80061A3C | Time-boxed: aggregate/local-copy type repair | results-5.jsonl |
| 106 | TT:func_8014C680 | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 107 | BUNKA_SD:func_80136F3C | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 108 | main:func_80056BA8 | Time-boxed: aggregate/local-copy type repair | results-6.jsonl |
| 109 | RPG_BAT:func_80139E88 | Time-boxed: aggregate/local-copy type repair | results-6.jsonl |
| 110 | RPG_BAT:func_8014D380 | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 111 | RPG_BAT:func_8014B79C | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 112 | RPG_BAT:func_80136E2C | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 113 | GYOZI:func_80141ACC | Time-boxed: aggregate/local-copy type repair | results-6.jsonl |
| 114 | RPG_BAT:func_8013B5B0 | Time-boxed: aggregate/local-copy type repair | results-6.jsonl |
| 115 | RPG_BAT:func_80138170 | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 116 | TACO:func_8014CB24 | Time-boxed: aggregate/local-copy type repair | results-6.jsonl |
| 117 | RPG_BAT:func_801498C0 | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 118 | main:get_near_menu | Time-boxed: compile/code-generation mismatch | results-6.jsonl |
| 119 | KANGEI:func_80134BC4 | Matched; fresh relocation-resolved verification passed | results-6.jsonl |
| 120 | RPG_BAT:func_8013A034 | Time-boxed: aggregate/local-copy type repair | results-7.jsonl |
| 121 | main:func_8004FC10 | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 122 | TACO:func_8014AB50 | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 123 | GYOZI:func_80138C9C | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 124 | RPG_BAT:func_8014BB80 | Time-boxed: aggregate/local-copy type repair | results-7.jsonl |
| 125 | main:sndi | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 126 | TACO:func_8014FFDC | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 127 | OPTION:func_8013BFF8 | Time-boxed: aggregate/local-copy type repair | results-7.jsonl |
| 128 | main:get_p_name | Time-boxed: compile/code-generation mismatch | results-7.jsonl |
| 129 | KANGEI:func_8013454C | Time-boxed: aggregate/local-copy type repair | results-7.jsonl |

Continuation finish checks: incremental Docker ninja exits 0 (`no work to do`); the prior full build remains 27/27 SHA1 OK. Header/global stamps present. Fresh sync_protos --check-branch passes with 0 new disagreements (17 known); migrate_globals --check passes. Fresh progress remains `grand total   4736/6958   730452/2279368  32.0%`. Only documentation changed during this audit; prior inline C review remains applicable. Status stays Done; edits remain uncommitted.

## Deep second pass

Started 2026-10-11 09:07:34 UTC. Reopened following the requested deeper experiments: rank resolved diffs, 10–15-minute candidate iterations, 300-second permuter near misses, then aggregate/local-copy type recovery. Prior ledger records shallow first-pass outcomes and does not satisfy this deeper pass. Minimum duration three hours unless list exhausted.

Deep-pass match `RPG_BAT:func_8013C32C`: real funcdiff and unit SHA1 passed. Variant: direct global increment and compare.

Deep-pass ranking: 25 first-pass drafts reached resolved diffs; TT func_8014BB30 (13 lines), SHOUGATU func_80137DE4 (14), BUNKASAI func_8013B190 (21), GYOZI func_8013AC38 (30), RPG_BAT func_8013C32C (30), RPG_BAT func_80138170 (33) are first. The already matched TT function is removed from the retry queue. TT shifted-sign control flow reduces its mismatch to eight lines; SHOUGATU's old-style called-function view removes the extraneous ANDI. Both receive 300-second permutation searches after source experiments. RPG_BAT func_8013C32C matched immediately with the documented direct-global idiom and verified unit SHA1.

Deep-pass match `SHOUGATU:func_80137DE4`: real funcdiff and unit SHA1 passed. Variant: cleanup dead=if ((D_80144E0C && D_80144E0C) != 0) { } zero=True.

Deep trials now include TT signed-shift and direct multiplication casts (two 300-second searches, best eight differing lines), BUNKASAI bit-field/control-flow/local-type variants plus 300 seconds (best permuter score 305), RPG_BAT func_80138170's omitted third argument fixed plus selector variants (first 300-second run reaches score 25), and KANGEI func_80135004 real RECT/one/girl locals and declaration orders (300 seconds, score 100). Rejected searches are restored; retained gains remain verified. Large-type retries are now using a recovered TaiikuRunner and Lim3 local copy rather than refusing m2c's unknown fields.

Deep-pass match `TAIIKU:func_80146994`: real funcdiff and unit SHA1 passed. Variant: complete controller aggregate and high-half position.

Deep-pass match `SHOUGATU:func_80137E54`: real funcdiff and unit SHA1 passed. Variant: byteviewu8 chain3.

Deep-pass match `GYOZI:func_8013AC38`: real funcdiff and unit SHA1 passed. Variant: byteviewu8 chain3.

Deep-pass match `KANGEI:func_8013454C`: real funcdiff and unit SHA1 passed. Variant: pad1 tableview0 copyorder0.

Deep-pass match `KANGEI:func_8013454C`: real funcdiff and unit SHA1 passed. Variant: cleanarray1 scalarselector1.

Deep-pass match `TT:func_8014B088`: real funcdiff and unit SHA1 passed. Variant: dimensionu32 use0 line0.

Deep type retries also cover main menu selection and object records, TACO signed timer and 0x88 actor/0x50 object views, TAIIKU runners/controller/camera, RPG_BAT action and reward tables, KANGEI local tables, SHOUGATU character flags, BUNKA_SD pointer/coordinate tables and OPTION 16-entry menu records. Experiments repair pointer strides and missing call arguments before type/register searches. KANGEI copied-table cleanup is a verified gain without FAKE padding. Current retained total: eight functions, 2,836 bytes.

Retained total after TT's unsigned dimension fix: nine functions, 3,108 bytes. The previous eight-function total is historical. Copy/type recovery continues for GYOZI's six 0x44 records, RPG_BAT's four 0x10 tables and four packed three-halfword arguments. Invalid DATE/XA aliases have been repaired before comparisons; their earlier compile failures are not evidence of an irreducible compiler gap.

Deep-pass match `SHOUGATU:func_8013A688`: real funcdiff and unit SHA1 passed. Variant: clean overlay verification.

Deep-pass match `BUNKA_SD:func_80137620`: real funcdiff and unit SHA1 passed. Variant: coordinatebasefirst0.

Deep-pass match `TAIIKU:func_801485C4`: real funcdiff and unit SHA1 passed. Variant: typedcamera decl('s32 index;', 's32 chosen;', 'u8 *p;') initialization1.

Deep-pass match `DATE:func_80144720`: real funcdiff and unit SHA1 passed. Variant: struct flags0 byte1 line0.

Deep-pass match `main:strNextVlc`: real funcdiff and unit SHA1 passed. Variant: clean main verification; byte-pointer interface and implicit strNext.

## Deep-pass experiment ledger (2026-10-11)

Pass start: 09:07:34 UTC; final type experiment finished after 12:16 UTC (more than three hours). The initial 129-entry ledger remains above as historical first-pass evidence. All 36 aggregate/local-copy rejects received recovered types and at least one valid resolved instruction comparison. Compiler errors were repaired and are excluded from the comparison counts below. Near candidates received repeated manual families and five-minute permutation searches. Unverified and rejected C was restored.

The table is ordered by the best resolved differing-line count. “0” records a function comparison, including provisional matches recovered only later; the retained list and final checksum verification determine the actual result. Body counts are distinct source candidates across the scratch logs, not a claim of independent semantic implementations.

| Function | Best differing lines | Distinct C candidates | Permuter run |
| --- | ---: | ---: | --- |
| `BUNKA_SD:func_80137620` | 0 | 16 | yes |
| `DATE:func_80144720` | 0 | 19 | yes |
| `GYOZI:func_8013AC38` | 0 | 4 | no |
| `KANGEI:func_8013454C` | 0 | 6 | no |
| `RPG_BAT:func_8013C32C` | 0 | 1 | no |
| `SHOUGATU:func_80137DE4` | 0 | 25 | yes |
| `SHOUGATU:func_80137E54` | 0 | 4 | no |
| `SHOUGATU:func_8013A688` | 0 | 8 | yes |
| `TAIIKU:func_80146994` | 0 | 9 | yes |
| `TAIIKU:func_801485C4` | 0 | 48 | yes |
| `TT:func_8014B088` | 0 | 26 | no |
| `main:strNextVlc` | 0 | 13 | yes |
| `SHOUGATU:func_801326F8` | 2 | 30 | yes |
| `RPG_BAT:func_80138170` | 8 | 85 | yes |
| `TT:func_8014BB30` | 8 | 75 | yes |
| `TT:func_8014C5F8` | 14 | 64 | yes |
| `BUNKASAI:func_8013B190` | 15 | 39 | yes |
| `main:strSetDefDecEnv` | 21 | 24 | yes |
| `TACO:func_801511C0` | 22 | 46 | yes |
| `KANGEI:func_80135004` | 28 | 106 | yes |
| `main:func_80052060` | 28 | 17 | yes |
| `TAIIKU:func_80147A6C` | 32 | 16 | yes |
| `TAIIKU:func_801482BC` | 32 | 12 | yes |
| `main:menu_check_2` | 32 | 23 | yes |
| `GEKO:func_80138E10` | 35 | 12 | yes |
| `BUNKAKEN:func_80140F60` | 37 | 36 | yes |
| `RPG_BAT:func_8014AB84` | 42 | 34 | no |
| `DATE2:func_80136A90` | 46 | 66 | no |
| `main:func_8006190C` | 47 | 8 | no |
| `RPG_BAT:func_80152384` | 56 | 8 | no |
| `TAIIKU:func_8014666C` | 56 | 4 | no |
| `main:menu_bar_show` | 59 | 16 | no |
| `GYOZI:func_80142A8C` | 62 | 20 | no |
| `DATE2:func_801338F8` | 70 | 7 | no |
| `TAIIKU:func_80147210` | 70 | 6 | no |
| `main:strNext` | 70 | 8 | no |
| `OPTION:func_8013BFF8` | 78 | 12 | no |
| `TAIIKU:func_80147120` | 81 | 120 | no |
| `main:xa_wait` | 93 | 8 | no |
| `RPG_BAT:func_8013B5B0` | 103 | 4 | no |
| `main:menu_check_1` | 104 | 16 | no |
| `TACO:func_80138964` | 106 | 11 | no |
| `RPG_BAT:func_80139E88` | 113 | 13 | no |
| `RPG_BAT:func_8013A034` | 138 | 4 | no |
| `TAIIKU:func_8014607C` | 147 | 1 | no |
| `KANGEI:func_80134724` | 150 | 7 | no |
| `TACO:func_8014F820` | 164 | 12 | no |
| `TAIIKU:func_80145370` | 164 | 2 | no |
| `RPG_BAT:func_8013BBF8` | 165 | 6 | no |
| `TAIIKU:func_80147F58` | 166 | 2 | no |
| `RPG_BAT:func_8014BB80` | 175 | 8 | no |
| `main:func_80056BA8` | 239 | 4 | no |
| `main:func_80061A3C` | 243 | 1 | no |
| `TT:func_801410A0` | 254 | 4 | no |
| `TACO:func_8014A480` | 257 | 8 | no |
| `main:func_80062948` | 264 | 4 | no |
| `TAIIKU:func_80146D18` | 272 | 2 | no |
| `GYOZI:func_80141ACC` | 422 | 4 | no |
| `main:func_80052F68` | 488 | 4 | no |
| `TAIIKU:func_80147B14` | 534 | 4 | no |
| `TACO:func_8014CB24` | 600 | 4 | no |
| `main:get_h_tokimeki` | 858 | 8 | no |

Aggregate coverage: 36/36 valid comparisons. Missing: none.

## Final retained changes (deep second pass)

Fourteen functions, 4,628 bytes total, including the original two (840 bytes); the second pass adds twelve functions and 3,788 bytes. All 129 list functions were attempted in the first pass, 62 functions received valid comparisons in the deep pass, and all 36 first-pass aggregate/local-copy rejects were reconstructed and retried. Deep work lasted more than three hours (09:07:34–after 12:16 UTC). Every unsuccessful source draft was restored; scratch evidence remains outside tracked source.

| Unit | Function | Bytes |
| --- | --- | ---: |
| KANGEI | func_80134BC4 | 492 |
| TT | func_8014BF54 | 348 |
| RPG_BAT | func_8013C32C | 676 |
| SHOUGATU | func_80137DE4 | 112 |
| TAIIKU | func_80146994 | 204 |
| GYOZI | func_8013AC38 | 248 |
| SHOUGATU | func_80137E54 | 284 |
| KANGEI | func_8013454C | 472 |
| TT | func_8014B088 | 272 |
| SHOUGATU | func_8013A688 | 296 |
| BUNKA_SD | func_80137620 | 436 |
| TAIIKU | func_801485C4 | 180 |
| DATE | func_80144720 | 436 |
| main | strNextVlc | 172 |

Five append-only T-0018 rows were added. Two historical rows subsequently resolved (GYOZI's byte chain and SHOUGATU's selector, the latter with marked FAKE idioms); no historical row was deleted. Largest non-T-0018 limitation: large reconstructed copy/table functions still differ in control flow, loop unrolling and frame layout. No SDK surrogate buffer or non-matching experimental type was retained.

Tool reports: the normal funcdiff resolver cannot resolve the low absolute D_7FFFFF constant in the movie timeout. strNextVlc was verified with `funcdiff --resolve --expected` using an expected object made directly from original executable bytes, followed by the clean whole-main SHA1. register_shapes does not support the brief's positional function argument, and its inventory fails on the historical missing VALEN func_80133670 asm. No tools or generated asm were changed.

Inline standards review: retained code is C89; all recovered types document offsets and sizes and live in game/overlay headers; main D_80122CE8 was added only by sync_protos. The old-style SHOUGATU caller override and both FAKE selector idioms are explained and ticketed. Removed unused BUNKA_SD m2c locals, checked source order and INCLUDE_ASM restoration, and confirmed only owned C files changed. No SDK types were redefined, no dependencies installed on the host, and no commits, staging, merges, pushes, sub-agents or unrelated container actions occurred. Final verification results follow below before the ticket closes.

Final verification and inline review (2026-10-11): Docker Ninja exited 0 with all 27 SHA1 checks OK, build/headers.ok and build/globals.ok present. Fresh resolved comparisons pass for all 14 retained functions; strNextVlc uses the strict original-byte expected object described above. The final incremental Ninja exited 0 with no work remaining. Scoped dupes --apply found 0 plans and applied 0 copies. sync_protos --check-branch reports 0 new disagreements and 17 known; migrate_globals --check reports globals OK; git diff --check passes. All inline CODING_STANDARDS checklist findings are resolved. Ticket and kanban moved to Done; no commits or staging.
