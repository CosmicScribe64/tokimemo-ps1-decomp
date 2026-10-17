---
id: T-9940
title: "Wave 10: list 4"
status: Done
assignee: w10-4
created: 2026-10-11
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match functions in wave10-list-4.txt, restricted to the 47 assigned C files.
Follow the shared wave-10 brief: fresh draft/build/diff pass, then sustained
source iteration and bounded permuter searches on near misses.

## Acceptance criteria

- [ ] At least four hours of matching work or deep attempts on every function.
- [ ] Retained matches verified by resolved function diffs and all 27 unit SHA1s.
- [ ] Shared declarations and GameState migration checks pass.
- [ ] Inline standards/spec review completed; no commits or merges.

## Notes

Started 2026-10-11 20:19 UTC. Scratch artifacts live in the shared scratchpad's
`w10-4/` directory. No tools or build configuration changes are permitted.

## Comments

- Read the shared brief, wiki index, standards, workflow, register-shape notes,
  and allocator trace evidence before implementation.

- Initial MouseState type repair plus a single return and assigning `result = -1`
  before the four output stores matches resolved words in a scratch compilation.
  Original-source integration and full verification are pending. Fresh drafts and
  776 unique prior candidates (132 assigned functions) are retained in scratch.

- Verified initial matches: main `MouseState` (144 bytes), OPTION
  `func_8013A550` (180 bytes). Both real-file `funcdiff --resolve` calls pass,
  and main/OPTION SHA1s pass after integration. Baseline configure/ninja passed
  all 27 SHA1s. Neither match needs a FAKE marker. Details in [[matching-notes]].

- TT `func_8014BB30` (324 bytes) now passes real resolved diff and full ninja
  validation. Its existing TtActor78 view fixes multiplication evaluation order;
  appended TtActorTail10 describes the halfword/byte at +0x5E/+0x60. The scoped
  dupes and neardupes passes added no functions. Current verified gain: 3/648.
- TT `func_8014CC04`: replacing the old byte-buffer arithmetic with
  TtDrawBuffer80B8 and its ordering array fixes the last swapped `addu` operands.
  The TILE packet variant also matches scratch words; integration awaits the
  first-pass source loop. The TtCore30 color view changes register allocation,
  so retain the existing declared byte-array view for color reads.

- Fourth verified match: TT `func_8014CC04` (440 bytes), with TILE and the
  existing typed ordering table. Full incremental ninja passes after all fresh
  drafts were restored. Current gain: 4 functions / 1,088 bytes. Fresh pass
  completed for the list; no additional direct m2c wins. Compilation failures
  and resolved differences are saved in `firstpass.jsonl`, with repaired and
  earlier-draft results in the per-function scratch directories.
- Fifth verified function diff: TACO `func_80141290` (296 bytes). Corrected
  typed actor/record strides and wrote the second position before reset fields.
  Reordering the five real locals removed all dead m2c spill aliases while
  preserving the exact frame and spills. No FAKE needed. Full unit verification
  follows in `five-build.log`. Running total: 5 functions / 1,384 bytes.
- Full incremental `ninja -j4` after match five succeeds, including
  `build/ovl/TACO.bin: OK` and the globals/header gates.
- TT `func_80133AD0` reaches zero scratch differences with a typed 0x96
  prefix, u16 K&R parameter/index, and two-step bit-index construction.
  Added the justified per-file unprototyped `func_80044750` override;
  generating its main_api guard and verifying the whole object next.
- TT `func_80133AD0` (108 bytes) passes real resolved funcdiff. The three
  neighboring matches checked (`func_80133970`, `func_80133A60`,
  `func_80133B3C`) remain exact with the file-local override. The generated
  main_api diff is just its two-line guard. Six function diffs now total
  1,492 bytes; full rebuild after the shared header change is running.
- Full rebuild after the shared prototype guard passes all 27 binary checksums
  (`six-build.log`, 521 build steps). Six verified matches / 1,492 bytes.
  Re-running duplicate/near-duplicate matching with the same explicit 47-file
  scope. All source experiments remain isolated in the owned scratch directory.
- Sustained deep pass: OPTION 80137E38 down to four instruction edits (one
  pointer spill slot), DATE 80150C78 to 16, ETC 8014354C to 13, main
  8006BC28 to 17 after recovering the switch. TACO 8013A580 has only temporary
  spill offsets wrong after removing dead aliases. TT 8014B7A8 remains at
  12 edits after type/range/reuse trials. Details and scratch paths are in
  the matching-notes section and per-function results journals.
- Second scoped duplicate/near-duplicate run adds no matches. Its sole exact
  candidate, EVENT 8011B5A8, fails verification and is restored by the tool.
- BUNKASAI `func_8015B828` (216 bytes) passes real resolved funcdiff after
  recovering its 0x40 local table and declaring both result/index before the
  table. No padding or FAKE. This resolves the earlier T-8040/T-9710 blocker;
  full binary validation follows with the next match.
- EVENT `func_80118DAC` (404 bytes) reaches exact scratch words using a
  0xE7-byte table wrapper and a real local for the table row index. The two
  signed-halfword counters precede the row and table. Source/header retained;
  the missing main-exe halfword is being added through sync_protos.

2026-10-17: Closing as superseded. The Codex run hit its usage limit with 8 functions merged (MouseState, OPTION func_8013A550, TT func_8014BB30, func_8014CC04 and func_80133AD0, TACO func_80141290, BUNKASAI func_8015B828, EVENT func_80118DAC; about 2,100 bytes), all still C on main. Its near misses OPTION func_80137E38, DATE func_80150C78, TACO func_8013A580 and TT func_8014B7A8 have matched since, and the rest is in the T-14500 census. Review: one finding, the unmarked goto loops in EVENT func_80118DAC, moved to a followup; the func_80044750 override carries its reason.

Followup opened: [[tickets/T-21000-event-goto-loops-and-seq-views|T-21000]].
