---
id: T-9720
title: "Wave 9: list 2"
status: Done
assignee: wave-9 batch agent 2
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match as many functions as practical from wave9-list-2.txt within its 34 owned C files. Follow the shared wave9-brief.md, including a quick draft pass, ranked deep attempts and at least four hours of work.

## Acceptance criteria

- [x] Keep only verified byte-matching C; restore unresolved drafts to INCLUDE_ASM.
- [x] Attempt near misses with source/type/control-flow iteration and bounded permuter runs.
- [x] Record matching count/bytes, unresolved shapes and tooling limitations.
- [x] Final Docker ninja passes all 27 checksums, headers and globals checks; prototype branch guard passes.
- [x] Inline standards and brief review resolves findings before Done.

## Notes

Owned files and ranked 132-function list: /private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad/wave9-list-2.txt.
Scratch: the sibling scratchpad/w9-2/ directory. No commits, generated asm edits, tooling changes, or raw-source writes.

## Comments

- 2026-10-11: Read wiki index, shared brief, standards, local instructions, workflow and register-allocation clusters. Starting baseline configuration and first-pass drafts at approximately 15:00 UTC.

- Preparation: strSetDefDecEnv's 0x30-byte output has four leading words, two eight-byte RECTs at +0x10/+0x18, an index word at +0x20, another RECT at +0x24 and a flag at +0x2C. The raw m2c draft's void-pointer member accesses need a real local view; assembly establishes every used width. DATE func_8014AD9C will use the shared MATRIX layout rather than an opaque word payload.

- normal_date_three_select_init: the twelve scalar halfword locals are four three-halfword tables passed to menu_set, at +0x34/+0x3C/+0x44/+0x4C. Plan to replace scalar address tricks with four s16[3] locals, chained equal values and measured declaration order; inspect frame size independently of each table's extent.

- Baseline Docker configure/ninja completed all 27 unit SHA1 checks, with headers/globals stamps. The ordered 132-function first draft/build/diff pass is running. Its early errors distinguish missing scalar/table declarations from incorrect void-pointer/aggregate views; unresolved C is restored after each attempt.

- GYOZI func_8013A4E8: selector bytes are at work+0x68D in four-byte entries; the copied source D_801474B4 is exactly one aligned word (lw/sw $at), and the flag word D_800F5718 is girl[8].unk_10. Added the minimum word-copy type in the overlay header for typed experiments.

- Tooling bug: tools/permute.py's `_worker` launch leaves tools/ first on sys.path, so multiprocessing imports tools/queue.py instead of Python's queue and fails importing Empty/Full. The initial RPG_BAT run did not search and is excluded from effort/evidence. Scratch-only workaround preloads the standard-library queue in a python -c worker, then calls the unchanged setup/run/verify helpers. No tools edits.

- Deep pass: GYOZI `func_8013A4E8`, variant bitfield-u8, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- Deep pass: RPG_BAT `func_8013F350`, variant permuter-score-50, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- GYOZI func_8013A4E8 linked proof: build/ovl/GYOZI.bin SHA1 43f92523116e7b6cd772d4397712a52eeda89703 equals the original overlay target. 268 bytes gained. No FAKE operations in this repair.

- RPG_BAT func_8013F350 (160 bytes) passes fresh resolved MATCH and the RPG_BAT unit SHA1. The 300-second working permuter stopped at score 50; applying its function body and resolving aliases proves a real match. `value ^ 0` retains the target's redundant store and the final dead `if (value) {}` retains v0 on implicit-return paths; these are marked FAKE, T-9720. Running gain: 2 functions / 428 bytes.

- Ordered first pass complete: all 132 m2c attempts captured. Outcomes: 29 draft repairs, 79 compile errors, 7 GameState mapping repairs, 17 genuine resolved diffs. Most errors are void-pointer member selectors or absent table views, not evidence of a compiler gap. Ranked diffs begin with ETC func_801451D4 (4), ETC func_80144540 (7), SHOUGATU func_8013A2F8 (13). Two deeper aggregate/permuter repairs are independently retained and verified.

- Small-draft repair: TEL D_801444A0 is a signed-halfword table indexed by the status nibble; ETC D_8015018C is a signed word selecting fade direction. Appended minimum declarations to the overlay headers. TT's +0x3AA/+0x3AC loads establish a pair of unsigned-halfword phase fields; the packet frame byte is at +0x52.

- Deep pass: TT `func_8013923C`, variant compound-u16, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- TT func_8013923C (100 bytes): local typed pointer keeps the record address across the branch; `phase = work->phase += work->step` and `phase = work->phase = 0` reproduce the target narrowing and stores. Fresh resolved MATCH and build/ovl/TT.bin SHA1 OK. Running gain: 3 functions / 528 bytes.

- Second permuter bug: setup hangs in tools/permute.py:200's strip_functions re.findall for SHOUGATU/TEL source, before the time limit begins. A 15-second faulthandler diagnostic confirms the regex stack. Interrupted only our two containers (SIGINT restored sources). Scratch-only bounded ANSI function-discovery override will allow the searches; repository tools stay unchanged.

- Deep pass: TEL `func_80139E3C`, variant permuter-best, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- TEL func_80139E3C (84 bytes) now passes the fresh resolved diff: s32 implicit return retains the selector in v0; a local Rec38 pointer models the selected girl, and repeating the table expression for the subtraction restores the strength-reduction temporary order. The 300-second permuter score is 10, but the real relocation-resolved comparison is MATCH. Linked unit check is running. Mark the repeated expression and implicit-return allocation FAKE before final review. Running candidate gain: 4 functions / 612 bytes.

- TEL linked proof: build/ovl/TEL.bin SHA1 OK. Formatting, renamed local index/delta and the required FAKE comment preserve a fresh resolved MATCH. Confirmed gain: 4 functions / 612 bytes.

- TT collision type recovery: matched func_80145508 reads signed halfwords +0/+2 and unsigned byte half extents +4/+5. Data labels B30/B36 and C64/C6A are pairs of six-byte entries. Appended TtCollision6 and minimum arrays; new caller drafts will use element 1 rather than duplicate scalar aliases.

- Deep pass: SHOUGATU `func_8013A2F8`, variant D_80122EBC3, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- SHOUGATU func_8013A2F8 (260 bytes) now passes resolved MATCH. Bit 2 uses a byte bit-field, and the selector assignment of 5 is the actual call argument. A dead triple test of unrelated D_80122EBC preserves the target `li a0; lui at; jal; sb` scheduling without promoting the final copied source D_80144DEC into v0. IDO removes all dummy reads/tests; marked FAKE and tracked here. Unit check follows formatting. Running candidate gain: 5 functions / 872 bytes.

- SHOUGATU linked/formatted proof: fresh resolved MATCH and build/ovl/SHOUGATU.bin SHA1 OK. Confirmed gain: 5 functions / 872 bytes. A full-tree proof will run once the current draft sweeps restore their source snapshots (still within the 5–10-gain batch interval).

- ETC selector cases remain assembly. func_80144540's +0x1098 store must be a word; after that repair, selector/local/dead-test forms and 300-second permutation leave only the missing v0/v1 copy. func_801451D4 has four lines in the case-2/case-3 operands after 19 source forms and a 300-second permutation. Existing overlay void prototypes are append-only; incompatible s32 trials are excluded as compiler evidence. register_shapes was run; appended two T-0018 evidence rows.

- Owned-file duplicate scan completed for the four restored files containing GYOZI/RPG_BAT/TEL/SHOUGATU gains: 0 applicable plans, 3 skipped. No source copies added. TT cleanup removes the unused m2c temp and renames phase/step; fresh resolved MATCH remains.

- DATE func_8014AD9C: real MATRIX/typed record repair reaches four stack-slot differences (saved object pointer +0x28 vs +0x30), including 300-second search and extensive declaration/layout trials. Restored to assembly. BUNKAKEN func_80140F60 reaches two selector-load scheduling lines; unit SHA1 fails, so the candidate is restored. MDEC trials retain a full RECT; misleading scalar-address candidates with dropped stores are excluded.
- EVENT func_80108A24: sync_protos --add-global adds D_800EB042/D_800EB044 as s16, based on target lh/sh. Main declarations remain generated and centralized.

- Deep pass: EVENT `func_80108A24`, variant s32temp_v0 = D_800EB042 += 0x80;
    D_800EB044 += 0x80;False, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- EVENT func_80108A24 (180 bytes): compound halfword assignment plus implicit s32 return preserves the target v0 narrowing. Marked FAKE for the return allocation; all callers ignore it. Removed the unused bit-field trial type. Formatted fresh resolved MATCH and EVENT SHA1 OK. Running gain: 6 functions / 1,052 bytes.

- Six-gain checkpoint: full Docker ninja passed all 27 binary SHA1 checks, CHECK HEADERS and CHECK GLOBALS after all trial sources restored. RPG_BAT formatting still passes fresh resolved MATCH; all retained units rebuilt after generated API additions. Owned-file duplicate plan reports zero plans (13 skipped). No changes to tooling/config/raw data.

- Deep pass: main `normal_date_girl_suddenin`, variant s32False, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- Main normal_date_girl_suddenin (112 bytes) passes fresh resolved MATCH: independent scalar-address byte chains plus implicit s32 return retain the target v1/v0 constant copy. The void form stays at seven differences. Source comment/linked main proof pending completion of other trials in the same file. Candidate gain: 7 functions / 1,164 bytes.

- Main sudden-in verified after cleanup: fresh resolved MATCH and build/SLPM_86.053.bin SHA1 OK. Both implicit-return and byte-array allocation details are documented FAKE, T-9720. Running verified gain: 7 functions / 1,164 bytes.

- Deep pass: TAIIKU `func_80140810`, variant literal source halfwords, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- TAIIKU func_80140810 (640 bytes) passes resolved MATCH after fixing all fourteen signed-halfword input addresses. Initial direct-symbol trial was blocked by a concurrent incompatible ETC return trial and is excluded; source-address-literal form matches. Rechecking direct symbols and final linked TAIIKU SHA1 after all interfering sources restored. Candidate gain: 8 functions / 1,804 bytes.

- TAIIKU initializer linked proof: the direct named-source variant changes code; restored the matching asset-address view, with a T-3300/T-9720 explanation. Final resolved MATCH and TAIIKU SHA1 OK. Running verified gain: 8 functions / 1,804 bytes.

- Further near-miss iteration: DATE's paired local pointers increase differences to 18; restored. K-scroll calculation scheduling, a 300-second search and declaration-order trials reach two lines, solely branch operand order. No additional match claimed. ETC func_80143F24 negative selectors repaired as Rec24 entries 2–4 color stores, with a single local GameState byte load being tested.

- Deep pass: main `check_k_scroll`, variant (u32)temp_v1 == 0x30U, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- main check_k_scroll (268 bytes) retains the exact branch with unsigned equality on its signed-halfword temporary. Declaration order places the saved Entry8 pointer at sp+0x48. The chained next-index assignment and comparison are documented FAKE. Formatted fresh resolved MATCH and full Docker ninja passed all 27 SHA1 checks, with header/global validation intact. Verified gain: 9 functions / 2,072 bytes.
- RPG_BAT initializer: appended twelve missing s32 value declarations at target lw width, to support the typed 0x44-record deep repair. Main induction aliases were added using sync_protos.

- Further type repairs: RPG_BAT func_8013E54C natural twelve-record loop requires field base +0x1980, not the target's unrolled +0x1870. Corrected source still differs (best 220 lines); restored. Move-place return-value/API generation trial reaches 43 lines, then restores both source and API. ETC func_80143968 compiles with typed 0x44 records and byte coordinates (best 110 lines); restored. TT func_801356C8 and main func_800850D4 typed trials continue.

- GYOZI func_801347D4 reads a word flag at work+0x1B8 and advances its biased work cursor by 0x38 bytes. Added the genuine void forward declaration for func_80134938, already defined later in the owned file; typed flag/pointer trials now compile. main func_800850D4 uses a +0x1980 0x44-byte draw record; word assets, byte flags and signed halfword coordinates have explicit local layout. No gain claimed from these ongoing probes.

- Additional deep repairs restored: main draw initializer 69 lines, menu character buffer 98, GYOZI selection 66. TT trig loop 50 lines; another 300-second search found no improved outputs. DATE pointer-array layouts worsen the near miss. ETC album copy compiles with the existing byte-only GsRec75E record and reaches 35 lines; avoids m2c's unsupported unaligned-scalar syntax.

- BUNKAKEN closest repaired draft has a working 300-second search, then another fresh-root search with canonical GameState relocation expressions; neither improves the two-line load-order case. Generated target normalization is independently proven byte-identical after linking (304 bytes, SHA1 4c5cc1dabaa1700c37b89c66ba9d4e0dea829c7f). The actual base scores are 90/60; 925 progress output was stale from older candidates. register_shapes identifies the existing selector/compare-chain rows. Added one refinement row; this batch adds 3 T-0018 rows total.

- Late deep pass: main history func_80054E6C compiles with typed save windows, three actual memcpy calls and byte/word induction, but still differs (best 91). Restored. Album func_80141ABC's 300-second search improves 35 to 27 lines; subsequent end-pointer/copy forms do not match. Career func_8014A360's phantom m2c arguments are removed in scratch; 300-second search reaches 16 lines and control-flow iteration continues.
- Review cleanup: TAIIKU literal load form now explicitly FAKE; unused RPG induction API aliases removed using sync_protos' helper. No retained function is changed by that cleanup.

- EN_NICHI func_80137FEC: recovering two true local table copies (12 signed halfwords, 33 callbacks) and a typed work-window selector/flag view. The target decrements its descending index after the callback, unlike the draft. Minimum overlay declarations appended; six local declaration orders under test.

- Deep pass: EN_NICHI `func_80137FEC`, variant init0, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- EN_NICHI func_80137FEC (476 bytes): outside=0 before record calculation fixes the final a1/a2 swap. Plain typed copies and local reuse yield fresh resolved MATCH; formatted/renamed source remains MATCH. Local reuse/callback comma scheduling marked FAKE. Candidate gain: 10 functions / 2,548 bytes; linked proof running.
- Career remains two lines off solely in final LUI ordering after source views/control-flow and two working 300-second searches; restored. Album pointer initialization-order sweep misses; restored. ETC draw func_80145AF8 compiles after its draft is corrected to the existing s32 return view and return-zero early exit; best eight lines.

- Linked EN_NICHI proof passes build/ovl/EN_NICHI.bin SHA1. Confirmed gain: 10 / 2,548 bytes.
- TT func_80138B90: new typed 0x14 animation records and work-side halfword arrays; source-coordinate table declared at target lh width. Natural loop/type/pointer trials are running; current phase-update gain is preserved.

- TT initializer restored: natural four-record loop 142 lines, paired typed/byte-stride form 131, cached work pointer 126. No gain claimed. The existing verified phase updater is preserved.

- Deep pass: ETC `func_80145AF8`, variant 20, fresh `funcdiff --resolve` MATCH; unit SHA1 verification pending batch cleanup.

- ETC func_80145AF8 (264 bytes): a working 300-second search fixes the frame and reaches four lines; the intensity assignment inside the first color argument fixes the spill's jal delay slot. Fresh resolved MATCH survives local renaming and s32 color. Allocation/implicit-return choices explicitly FAKE. Candidate total 11 / 2,812 bytes; linked/full proof pending.
- Prototype guard before this final gain: branch check OK, 0 new disagreements, 17 known baseline. GameState migration guard: globals OK. Final guards will run on the complete retained source tree.

## Final retained functions

| Unit | Function | Bytes | Matching note |
|---|---|---:|---|
| GYOZI | func_8013A4E8 | 268 | Typed word copy, byte bit-field and selector table. |
| RPG_BAT | func_8013F350 | 160 | FAKE redundant store and implicit-return allocation. |
| TT | func_8013923C | 100 | Typed phase/step fields and compound narrowing. |
| TEL | func_80139E3C | 84 | FAKE repeated table expression and implicit return. |
| SHOUGATU | func_8013A2F8 | 260 | Byte bit-field; FAKE dead scheduling test. |
| EVENT | func_80108A24 | 180 | Halfword phase; FAKE implicit-return allocation. |
| main | normal_date_girl_suddenin | 112 | FAKE byte chains and implicit return. |
| TAIIKU | func_80140810 | 640 | Correct fourteen source loads; FAKE literal bases. |
| main | check_k_scroll | 268 | Entry8 view; FAKE chained index/comparison. |
| EN_NICHI | func_80137FEC | 476 | Typed local tables; FAKE reuse/comma scheduling. |
| ETC | func_80145AF8 | 264 | FAKE pointer homes and call-embedded intensity spill. |
| **Total** | **11 functions** | **2812** | Final full-tree proof: all 27 SHA1 checks OK. |

- Final scoped dupes --apply --check across all 34 owned paths: 0 plans, 0 applied, 13 skipped. No probe sources remain; the ten changed C files contain exactly these eleven retained replacements.
- Final GameState guard: globals OK. Full Docker ninja and the final prototype branch guard are running against the stable retained tree.

- Eleven-gain checkpoint: full Docker ninja -j4 completes 522 actions, all 27 SHA1 checks OK, including ETC's final gain. Final branch guard reports 0 new disagreements / 17 known and branch check OK; GameState check globals OK. Header/global stamps intact.
- ninja progress grand line: grand total 4881/6958 779472/2279368 34.2%.
- Last typed select_girl_main attempt repairs the F90 pair selector and signed-halfword selection table; isolated API snapshot is restored on a miss. Final tree will be checked again afterward.

- Final selection trial restored: corrected F90/table views reach 40 lines, no match. The temporary generated D_800CA330 declaration is removed by restoring this trial's API snapshot. Eleven gains unchanged; final restored-tree checks running.
- Inline review inspected all eleven replacements and appended declarations against CODING_STANDARDS: C89 declarations/comments; fixed-width locals; true copied extents/field offsets; SDK types for unsuccessful geometry repairs; canonical GameState and existing work-table views; centralized generated main declarations; documented FAKE choices; no unresolved drafts, SDK redefinitions, inline asm, volatile hacks, build-pass/config edits, raw-source writes or staged data. Final header/prototype/build results remain the review gate.

- Status moved to In Review with its kanban card/index entry. Final restored-tree guards pass (headers OK, globals OK, branch check OK / 0 new disagreements); full binary verification is completing. Standards review remains inline as the brief requires.

## Tooling reports for the orchestrator

1. Permuter worker import collision: tools/queue.py shadows the standard-library queue when the worker starts with tools/ first on sys.path; multiprocessing cannot import Empty/Full. The initial failed-worker run is excluded from search evidence. Working searches preload stdlib queue before entering the unchanged tools module; workaround only in scratch.
2. Permuter setup regex: strip_functions at tools/permute.py:200 hangs on the owned SHOUGATU/TEL translation units before the search timer starts. A 15-second faulthandler diagnostic identifies re.findall; only our containers were interrupted. Scratch-only bounded function discovery lets the normal setup/run/verify code execute.
3. m2c draft errors: reused HI16 bases lose distinct later asset source addresses in TAIIKU func_80140810 and collapse junban_init's three byte destinations into one. ETC func_8014A360 also receives phantom arguments to the already matched no-argument func_8014A2C4. Each corrected draft was checked against target asm; no repository tooling was edited.

Search output directories preserve older candidates: the misleading BUNKAKEN score 925 came from stale output, not from the current base. Fresh roots and the resolved function/unit checks are the authoritative evidence.

## Inline review result

PASS against CODING_STANDARDS.md and the wave-9 brief. All eleven retained functions have fresh resolved MATCH evidence and linked binary proof; final restored-tree Docker ninja completes 523 actions with 27/27 SHA1 OK. Explicit check_headers: headers OK. migrate_globals --check: globals OK. sync_protos --check-branch: branch check OK, zero new disagreements / 17 known baseline. Ownership/replacement audit: ten owned C files, eleven replacements, 2812 work-list bytes. git diff --check is clean.

Resolved review findings: mark TAIIKU literal load allocation FAKE, remove unused RPG induction aliases, use fixed-width s32 color in ETC, preserve actual table extents/callback ordering in EN_NICHI, and restore every unsuccessful source/API trial. Nine allocation/scheduling matches carry FAKE reasons and T-9720; no unresolved findings remain. No commit/stash/reset/checkout, reviewer agents, tooling/build-config changes, raw-source modifications or staged game data.

- Final non-T-0018 blocker: ETC func_8014A360, two real final-LUI ordering lines after the initial flag/result and control-flow repairs. DATE func_8014AD9C is next at four stack-slot lines. All candidates remain scratch-only/INCLUDE_ASM; closest follow-ups recorded in matching-notes.

- Completed after the four-hour minimum: approximately 15:00–19:01 UTC on 2026-10-11. Ordered first pass covers all 132 listed functions; ranked source/type/control-flow/permuter iterations and final reviews yield 11 retained functions / 2,812 bytes. Three T-0018 evidence rows added. Final progress: grand total 4881/6958 779472/2279368 34.2%.
- Status/card/index moved to Done after inline review passed and every finding was resolved. Changes are uncommitted for the orchestrator; no merge/push/commit performed.
