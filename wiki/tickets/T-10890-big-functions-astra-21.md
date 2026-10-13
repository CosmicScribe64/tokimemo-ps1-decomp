---
id: T-10890
title: Big functions (astra) 21
status: In Progress
assignee: Codex
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Match the assigned large functions in NAME_ENT/80132000, TAIIKU/80138950,
main/80062CD0, and TAIIKU/80133C80. Recover reusable types and idioms.

## Acceptance criteria

- [ ] Verify retained matches with resolved funcdiff and unit checksums.
- [ ] Keep useful near misses guarded and document regdiff findings.
- [ ] Record shared types and idioms in headers and workflow.
- [ ] Run final header/global checks and inline standards review.

## Notes

Work begins 2026-10-13 12:44 UTC. Four-hour minimum requested; largest first.
All tooling runs through Docker, scratch under scratchpad/a21. No Git writes,
tool/config changes, or edits to other agents' C files.

## Comments

- Read shared wave-11 brief, register cheatsheet, standards and workflow.

- Baseline: first configure/build completed all 27 checksums. Drafting the 57
  assigned targets in isolated scratch; starting the 3,316-byte initializer.
- Initial type recovery: DisplayActor44 in include/game.h, stride 0x44;
  parameter sprite id +0x18, position halfwords +0x26/+0x2A, update halfword
  +0x2E and update byte +0x43. Canonical actor pool remains D_8011ECD0.

- Match 1: parameter_show_init, 3,316 bytes. Full-file guarded C and private
  linked bytes verified; now active C. regdiff run on each chain-order and
  scheduling candidate. FAKE frame homes and same-line stores marked explicitly.
- A proposed D_80120888 declaration collided with EVENT's overlay-local table
  at the same address. Removed the declaration; NAME_ENT reads its actor field
  through DisplayActor44 instead, without touching EVENT's file.

- Cleanup replaced all three provisional unused frame homes with useful locals:
  separate loop index, raw parameter value, digit result and snapshot destination
  pointer. The scratch compile and regdiff remain exact, with no padding FAKE.
  Only the explicitly marked same-line scheduling dependency remains.

- Character picker recovery: NameKanaPages44 (17 pointers), D_8014CDB0
  (45 pointers), loaded u16 count and text-pointer tables. Initial typed
  picker draft is 534/564 instructions; further selector/lifetime work needed.
- func_80062DBC is now instruction-for-instruction except frame offsets
  (26 listing lines). Preload component coordinates and sizes; use the second
  coordinate/size set for part 7 as well as part 3. Reusing the origin locals
  removes the register mismatches. Grouped local-pair layouts are being tested.
- DisplayImage78 models the observed eight parts after an 8-byte header;
  DisplayImagePart0E names the seven halfword fields of each part.

- Checkpoint 1: all 27 binary checksums pass with active parameter_show_init.
- func_80066C08: recovered column-major 10x8 mask, SDK SPRT usage, packed
  DisplayColorPair4, four input palettes and one output palette. Explicit
  case 0 restores the six-entry switch. Exact count alone is insufficient:
  register pressure, source scheduling and palette-loop induction still differ.
- The 600-second func_80062DBC permuter search had no score-zero candidate.

- Match 2: func_80062DBC, 1,856 bytes. Moving origin_x/origin_y declarations
  before w1/h1 fixes the frame without padding or argument reuse. Renamed the
  origin locals and verified literal linked bytes again. Both active matches
  (5,172 bytes total) pass tools/funcdiff.py --resolve.

- Match 3: func_80066C08. Verified 1,968 literal linked code bytes; the
  work-list size is 1,992 including its 24-byte jump table. No dummy padding:
  four real coordinate/palette-index locals plus declaration order fix the
  248-byte frame. Blend local and weight assignment order fix every register.

- Match 4: func_80066334, 1,784 linked bytes. Canonical signed counter,
  full-width drawing-call override, x/y call locals, and FAKE-marked paired
  work-field writes on one line. Removed the obsolete func_8004E970 declaration
  in favour of the existing renamed get_k_work definition to resolve the alias
  conflict reported by check_headers.
- Match 5: NAME_ENT func_80136808, 2,712 linked bytes. All cursor reads and
  writes now use DisplayActor44 consistently; this resolves the load ordering
  and the one missing instruction. No new fake needed. Its 600-second
  permuter search found no exact candidate before the aggregate fix.
- Recovered TAIIKU's camera word prefix and eleven-entry result table types.
  The two current TAIIKU drafts still need structural/frame work.

- TAIIKU func_801346FC: local table copies now match frame and all but ten
  listing lines. Inlining both delta-table indices removes two register islands;
  moving the scroll accumulator update before the actor decrement fixes the
  temporary order. Remaining raw decrement uses v1 instead of v0. A timed
  permuter and explicit lifetime variants are running; all misses use regdiff.

- Checkpoint 3: all 27 checksums pass with all five active matches and shared
  header updates. Started the next selection initializer; recovered its 11x8
  byte flag table, two eleven-halfword tables and one eleven-word table.

- Retained func_801346FC as a ten-line guarded draft. Its one unused word
  preserves table offsets after index inlining and is explicitly marked FAKE.
  A version using a real coordinate home is twenty lines away, saved in
  scratch/a21/taiiku-346fc-home-4.c. The 600-second permuter found no match.
- Added TAIIKU motion-helper declarations from their existing definitions;
  func_8013A6FC is the next 1,536-byte work-list target (1,472 code + tables).

- Retained func_8013A6FC guarded at twenty differing listing lines (five
  instruction-order swaps); all registers and frame size agree. Removed its
  unused local without changing the result. The 300-second permuter did not
  match it. Removed the far rotated-grid draft from the source; all its
  camera/perspective experiments remain in scratch/a21.
- NAME_ENT func_801324AC's actor initialization and five-iteration flag loop
  are structurally recovered; current draft has exact instruction count.

- Match 6: NAME_ENT func_801324AC, 1,420 linked bytes. Reconstruct the
  five-iteration flag loop and share the base-plus-index actor expression
  with the initialization loop; otherwise its pointer takes v0 rather than
  s0. Precede the constant field stores with the resource-pointer assignment.
  Removed the unused pointer without a frame change. Re-indexing and the
  one-line flag loop are explicitly FAKE-marked scheduling dependencies.
