---
id: T-10010
title: "Wave 11: list 1"
status: Done
assignee: Codex w11-1
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Match functions from wave11-list-1.txt within the 54 owned C files, following the shared wave-11 brief. Work for at least four hours; session started 2026-10-12 00:54 UTC (2026-10-11 local).

## Acceptance criteria

- [x] Quick draft/build/diff pass, followed by ranked deep attempts.
- [x] All retained matches verified in Docker; final 27/27 checksum build and header/global/prototype checks pass.
- [x] Inline CODING_STANDARDS.md review complete; changes left uncommitted.

## Notes

Scratch artifacts: scratchpad/w11-1/ beneath the shared scratchpad. List contains 130 functions, 51,880 bytes. Only owned source files and explicitly permitted declarations/wiki files may change.

## Comments

- Initial checkout is clean. No build.ninja exists; configure before drafting.

- First verified gain: SHOUGATU func_801341E4, 168 bytes. Resolved real-file funcdiff MATCH; incremental ninja passes SHOUGATU and restored TAIIKU checksums with globals OK. FAKE comment documents the narrow return cast, dead raw-byte test, and u16 compare constant. Other 25 units retain their passing baseline artifacts. Scratch proof: narrow-probes/, lifetime-probes/, compare-probes/; real proof: shou341-real.log, first-match-build.log.

- Quick pass complete for all 130 listed functions (initial + resumed quick.jsonl records), with every attempt drafting, building and diffing or retaining the recorded compiler failure. Raw m2c drafts frequently need aggregate repair or missing declaration widths; those failures are not claimed as deep attempts. A separate scratch scan and 50 wave-10 recovery probes rank useful seeds. Copied all 130 scoped candidates from the existing farm queue into this agent's scratch only; the shared farm is unchanged.

- Second verified gain: ENDING func_80139F3C, 200 bytes (368 bytes total). Global brightness updates keep the original a1 range. The existing unprototyped func_80084C98 alias and FAKE-marked comma/empty-test idioms reproduce the loop conversion and final color-shift allocation. Resolved real-file funcdiff MATCH; incremental ninja verifies ENDING checksum and globals. Proof: ending-fade-real.log, second-match-build.log.

- Third real-file resolved match: NAME_ENT func_80135A98, 284 bytes (652 bytes total). Direct selector accesses and an explicit fourth switch case restore the original branch layout. Three overlay-local globals appended to NAME_ENT.h; no FAKE construct needed. The header change triggers the full incremental checksum rebuild.

- Fourth verified gain: NAME_ENT func_80133D94, 192 bytes (844 bytes total). Direct column-global updates replace the saved draft's artificial parameter and dead test, yielding the original a0 lifetime and final branch-delay constant. No FAKE idiom remains. Real resolved diff MATCH and NAME_ENT checksum/global check pass. The scoped duplicate sweep across all 54 owned paths reports zero applicable plans.

- Fifth/sixth verified gains: TAIIKU func_8014430C (344 bytes) and DATE2 func_801328D0 (288 bytes). Six functions total 1,476 bytes. Both pass real resolved diffs and the six-match full incremental rebuild passes all 27 checksums. TAIIKU reuses TaiikuPosition and direct speed updates; DATE2 restores two post-increment tests. Global declaration generated via sync_protos; overlay-only type/prototype additions are appended.

- Seventh real-file resolved match: TACO func_8013546C (240 bytes). Direct button-state and selection-global accesses remove the saved draft's unnecessary local copies; the final C needs no FAKE idiom. Added an overlay prototype for an earlier caller. Seven functions total 1,716 bytes; checksum rebuild is running.

- Eighth real-file resolved match: DATE2 func_801338F8 (256 bytes), bringing the total to 1,972 bytes. Byte status locals, direct word accumulators, and FAKE-marked s32 fall-through reproduce the original code. The full seven-match rebuild passes all 27 checksums; a fresh incremental build checks the eighth edit's final state.

- Ninth real-file resolved match: SHOUGATU func_80136340 (228 bytes), total 2,200 bytes. An indirect byte view prevents unwanted promotion of the mode flag; a u32 girl copy restores the second comparison's operand order. Both choices are FAKE-marked. The eighth-edit incremental build passes; the ninth overlay checksum is running.

- Tenth through twelfth real-file matches: SHOUGATU func_8013644C (324 bytes), TAIIKU func_801449A0 (416 bytes), and TACO func_8013D348 (172 bytes). Total 3,112 bytes. The first restores the timer's post-increment and uses structured if/else; the second promotes the existing NON_MATCHING draft with the indexed queue scan and permuter's `(i * 3) * 12` flag-store stride; the third uses a u32 indexed loop plus documented s32 fall-through. All three resolved diffs match; combined checksum build is running.

- Twelve-match checksum checkpoint passes the changed SHOUGATU, TAIIKU and TACO units; all other unit artifacts retain the full 27/27 passing checkpoint. The TAIIKU unused-word audit confirms it is necessary: removing q moves the table from stack +0x48 to +0x4C (two diff rows). No unnecessary padding was removed.

- Thirteenth/fourteenth real-file matches: TACO func_8013CE78 (352 bytes) and func_8013D3F4 (308 bytes), total 3,772 bytes. Both restore indexed loops; the actor loop needs the correct s32 score-table width and source addition order, while the particle loop needs direct byte post-increment tests. Unused locals were removed and rechecked. Both resolved diffs match; the fourteen-match checksum build is running.

- Fifteenth/sixteenth real-file matches: RPG_BAT func_801453B4 (308 bytes) and SHOUGATU func_80138758 (272 bytes), total 4,352 bytes. Aggregate weight accesses and the documented comma-return schedule fix the selector. A RECT plus packed nibble/byte flag fields fix the setup; its unused scalar frame home is documented. Both resolved diffs match. The fourteen-match rebuild passes all 27 units; the sixteen-match rebuild is running.

- Seventeenth real-file match: GYOZI func_80135518 (272 bytes), total 4,624 bytes. Correct early returns, the existing aggregate flag, and a byte previous-selection local remove all differences. An overlay forward declaration resolves the real-file implicit-int/void conflict. Sixteen-match full rebuild passes 27/27; duplicate sweep and next checkpoint running.

- Eighteenth real-file match: TT func_80132810 (888 bytes), total 5,512 bytes. Typed 0x10/0x64 records, a u16 scroll view, initialization order, and tile-before-grid pointer increments reproduce the original unrolled loop. No FAKE idiom is required. Reusing the older signed core field changed its increment load to lh, so the proven unsigned view is retained. Seventeen-match full rebuild passes 27/27; eighteen-match build running.

- Nineteenth through twenty-second real-file matches: OPTION func_80132624 (152 bytes), RPG_BAT func_80144480 (360 bytes), TT func_8013A610 and func_8013A710 (256 bytes each). Total 6,536 bytes. All pass real resolved diffs. OPTION restores the typed 16-entry grid; RPG restores nested switches with documented integer fall-through; TT shares a scene view and the existing position aggregate, with documented scalar homes. The 18-match build passes 27/27; combined 22-match build running.

- Twenty-two-match full incremental checkpoint passes all 27 checksums plus header/global checks. No tool/config files have changed. Near misses remain scratch-only.

- Two further cleaned scratch matches are being placed for real verification: TACO func_80155C64 (428 bytes), with documented xor-zero and byte-stride operand-order idioms; main func_8005B2BC (224 bytes), using an ordinary s32 table of two-word entries and selected-girl assignment in the third call argument. Five missing main globals are generated with sync_protos; the one TACO-local byte is appended to its overlay header. Expected total after verification: 24 functions / 7,188 bytes.

- Real-file funcdiff confirms both #23/#24 and the paired main func_8005B1A8 (#25, 276 bytes). Total 25 functions / 7,464 bytes. The latter restores the observed 1/0 returns; generated main_api now declares the actual s32(void) definition. Two more main data declarations are generated. Its two unused word homes are FAKE-marked and controlled by successful two-word versus failed one-word/no-word scratch comparisons. Full incremental build and another scoped duplicate sweep are running.

- Main func_80050324 passes real resolved diff (#26, 228 bytes), total 26 / 7,692 bytes. Shared-index forward/reverse loops and the two-column GsRecF90 view reproduce the original; no FAKE idiom is needed. The preceding 25-match full rebuild passes 27/27 checksums, headers and globals, and the scoped duplicate sweep applies zero plans.

- Review cleanup: func_80050324 now accesses counts/current indices through the documented MenuSelection5 view, and its real resolved comparison still passes. DATE func_801511B4's integer scratch zero cannot be retained under the append-only overlay-declaration scope because its established prototype is void; assembly is restored, and void variants remain four rows away. Main func_80053418 reaches a clean scratch zero with a real 20x20 byte lookup table, two sixteen-byte buffers, and natural return of the final callee's result. Real verification is running; no added FAKE is needed for that function.

- Main func_80053418 passes its real resolved diff (#27, 276 bytes), total 27 / 7,968 bytes. The new central declaration is u8 D_800B41B0[20][20], exactly spanning the lookup rows before the existing D_800B4341 table. Returning the final callee result preserves the existing s32 convention without a fall-through trick. Review is conservatively renaming the new TT scene's unclassified selectors to offset-based unk_XX fields; names/whitespace only, with real rechecks.

- Twenty-eighth real-file match: ENDING func_8013A004 (740 bytes), total 28 / 8,708 bytes. Replacing invalid draft memcpy placeholders and individual aliases with the existing Rec24 array gives an exact match directly. Five ordinary record assignments preserve the copy loops; no declaration changes or FAKE idioms are introduced. Real resolved diff passes; incremental checksum checkpoint is running. The preceding 27-function checkpoint passed every checksum, prototype/global guards and scope checks.

- Final review follow-up: the new ENDING initializer passes its checksum. The earlier fade must retain the established scalar brightness alias: direct Rec24 field and field-pointer views each leave 32 resolved rows by promoting the value to v0 instead of a1. Added a FAKE comment documenting that proven view constraint; no duplicate declaration is added. The late EVENT loop is four rows away and OPTION grid recovery remains farther away, both scratch-only.

## Final retained results

| Unit | Functions | Text bytes |
| --- | ---: | ---: |
| DATE2 | 2 | 544 |
| ENDING | 2 | 940 |
| GYOZI | 1 | 272 |
| NAME_ENT | 2 | 476 |
| OPTION | 1 | 152 |
| RPG_BAT | 2 | 668 |
| SHOUGATU | 4 | 992 |
| TACO | 5 | 1,500 |
| TAIIKU | 2 | 760 |
| TT | 3 | 1,400 |
| main | 4 | 1,004 |
| **Total** | **28** | **8,708** |

`ninja progress`: `grand total   5010/6958   845648/2279368  37.1%`.

Verification artifacts under scratchpad/w11-1/: `final28-build.log` (incremental ninja), `final28-artifacts.log` (all 27 final binary hashes plus direct header check), `final28-protos.log` (zero new disagreements, 17 known), `final28-globals.log` (globals OK), `final28-dupes.log` (zero plans), and `final28-scope.log` (owned paths and append-only overlay headers). `git diff --check` passes. Every retained function also has a real resolved funcdiff proof.

## Standards review

Inline review of the uncommitted diff against c55a3b8c564cd5c58d1eb7a7e8f987e72d4c165f, using CODING_STANDARDS.md, CONTRIBUTING.md, AGENTS.md and the code-review skill's standards axis. The shared brief explicitly requires inline review, so no reviewer agents were used. No unresolved findings.

- Menu-selection accesses now use the documented five-byte aggregate view; real diff still matches.
- New TT scene members with unconfirmed semantics use offset-based names; both affected functions were reverified.
- ENDING's scalar brightness view is explicitly FAKE-marked after direct field/pointer alternatives fail by 32 rows. Other register/frame idioms carry reasons and T-10010 references.
- Retained C is C89 with fixed-width types, existing SDK types and documented aggregate layouts. Main declarations are generator-owned; overlay additions are append-only. No new unguarded nonmatching code remains. Byte-matching requirements override generic duplicate-code/name smell heuristics.

## Spec review

Against wave11-brief.md and wave11-list-1.txt: zero unresolved implementation or scope findings. The full 130-function quick pass was followed by ranked deep attempts, bounded permutations and aggregate repairs. All 28 retained matches belong to the owned files. No tools/config/YAML changes, commits, host dependency installs, shared-farm edits or other-agent container actions occurred. The session ran from 2026-10-12 00:54:19 UTC to 2026-10-12 04:54:25 UTC (4h 0m), satisfying the four-hour minimum.

## Handoff

- One T-0018 row added: SHOUGATU func_80136078, two callback-comparison operand reversals (four resolved rows) after full aggregate/frame repair and a 300-second permutation.
- Top non-T-0018 blocker: TAIIKU/80138950 needs its own data island (80149940-80149FA0, 60 pieces). The dry run is recorded in `taiiku-island-dry.log`; applying it would change forbidden YAML, so this is left for integration.
- Tooling bug: m2c collapses distinct LO16 store targets sharing one HI16 into repeated stores to the first symbol. The raw TAIIKU assembly disproves those draft stores; no tool patch is included.
- DATE func_801511B4 has a scratch-zero integer candidate but an established void overlay prototype. It remains assembly under append-only header scope.
- Late EVENT func_800FAA04 is four resolved scheduling rows away in `schedule-probes/`; its bounded permutation resolves to eight rows. OPTION func_80132384's aggregate draft remains farther away. Both are scratch-only.

Completion: all review findings resolved; final second EVENT permutation still leaves four resolved rows, so it remains assembly. Changes are intentionally uncommitted for the orchestrator.
