---
id: T-10870
title: Big functions 5 (astra)
status: Done
assignee: Codex a19
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Match the assigned big-function list in main/8007C030, DATE/80132000,
and EVENT/8010FE10, largest first. Recover reusable layouts and idioms.

## Acceptance criteria

- [x] At least four hours of deep matching work, roughly one hour per large handler.
- [x] Verify active matches with funcdiff and complete unit checksums.
- [x] Retain useful near misses under NON_MATCHING with ticketed explanations.
- [x] Document types in shared headers and findings in the workflow.
- [x] Complete prototype/global guards and inline standards review.

## Notes

Started 2026-10-13 10:42 UTC. Shared brief and register cheat sheet read.
Only the three assigned C files and authorized declarations/wiki files may
change. No commits, branch operations, tooling edits, or reviewer subagents.

## Comments

- Initial tree is clean. Prior T-10730 layouts and scratch seeds are relevant;
  new-tool literal-byte checks will decide whether those seeds can be retained.
- Initial dialogue comparison: 2,168 words, ten positional scheduling misses
  (two four-word rotations around `li a0,-40`, one swapped address load).
  Full-file regdiff confirms no independent allocator miss. Physical-line
  sweep is isolated in scratchpad/a19; current repository draft unchanged.
- Baseline full build: all 27 checksums OK. Baseline progress is 5,226/6,958
  functions and 1,011,424/2,279,368 bytes (44.4%). Dialogue's 180-second
  permuter pass found no improvement. Dispatcher regdiff isolates two
  commutative byte-row address sums; nine straightforward index variants
  did not resolve them.
- First integrated match: read_bustup_uniform, 980 literal text bytes verified
  by funcdiff (list/progress span includes its jump table). Ordinary fourteen
  seasonal tests plus girl-14 override; no FAKE needed. Generated prototypes
  retained only for this new public function and the owned set_tarao_sprt.
- Dialogue case-1 condition recovery removes its address-load scheduling
  swap. Retained guarded source now has eight differing positions (sixteen
  displayed +/- lines), entirely the two incremented-Y calls.
- set_tarao_bg and the existing set_tarao_sprt both pass full-file regdiff
  after recovering the helper's K&R definition and a parity-offset local.
  Literal integration checks are running; source uses no new FAKE.
- set_tarao_bg now passes literal funcdiff (764 bytes); its K&R helper
  remains exact (264 bytes). Full build: 27/27 OK.
- SD_GetCDLevel now passes literal funcdiff --resolve (736 bytes). Added
  the shared CdLevelWork prefix, generated helper prototypes and the
  explained status-result override. Final explicit halfword masks fix
  four ugen register differences. Three new functions, 2,480 progress bytes
  (the work list's uniform span also includes 60 bytes of jump-table data).
- Third integrated checkpoint: all 27 unit checksums OK after SD_GetCDLevel.
  Owned-file duplicate pass found no additional copies.
- Formatter research recovers the exact instruction count and, with direct
  output indexing, the 760-byte frame. Remaining control-flow/temporary
  differences are broad; the five-minute permuter produced no valid match.
- DATE func_8013272C is independently verified at 5,768 literal bytes and
  the original whole-overlay SHA1. Its existing void prototype needs an
  append-only exception; the user's approval question remains pending.
- DATE func_80140374 now has a 261-word particle-record draft with eight
  register/operand differences. It also needs a corrected s32 return view;
  research remains in scratch until the prototype restriction is resolved.
- Follow-up particle work reduced that DATE draft to three operand swaps.
  Appended DateParticleView44 and D_8015BD40[20] to the shared DATE header.
- Main func_8007D8AC now has all 697 original instructions and the original
  104-byte frame. Natural loop/coordinate cleanup leaves four register
  differences plus a spill offset; an indirect case-1 word view removes
  the register differences. The five-minute permuter confirmed that view.
- DATE shop research corrected an uninitialized GameState pointer in the
  older scratch seed. Explicit byte-stride indexing through an initialized
  state pointer restores repeated multu operations that typed table indexing
  had collapsed. Further register/control-flow work remains.
- EVENT func_8011B190 matches all 1016 literal bytes. Shared particle/effect
  layouts, a corrected no-argument call, mask/palette first-use ordering,
  and RGB chain direction removed the remaining differences. Full build
  passes 27/27 checksums; cumulative batch gain is four / 3496 bytes.
- DATE func_80138934 and EVENT func_80115C18 pass resolved literal checks
  at 1740 and 1780 bytes. Integer row offset before table-pointer addition
  fixes both handlers without allocation-only scaffolding. Cumulative
  gain: six functions / 7016 bytes. DATE ride pointers are shared in DATE.h.
- Both museum handlers now match: DATE func_80134C64, 1320 literal bytes;
  EVENT func_80112BEC, 1284 bytes. Consistent three-halfword chain order at
  every site and a real EVENT girl switch resolve the remaining differences.
- DATE func_8013F638 matches 1044 literal bytes with the recovered EVENT
  particle initializer shape. Nine active matches / 10664 bytes total.
- Inline review caught an allocation-only output-mask comment lacking its
  FAKE prefix; fixed it. Replaced EVENT ride's separate output accesses
  with the new EventResultRow6 view and simplified DATE museum address
  expressions. All three affected overlay handlers still match via regdiff.
- Added two T-0018 rows for DATE particle drafts (40374 and 3FD6C), after
  checking the register-shape inventory. Their best source remains in the
  isolated a19 scratch directory; neither is counted as a match.

- Review checkpoint: all 27 checksums pass, sync_protos --check-branch
  reports zero new disagreements, globals check passes, owned dupes applies
  zero changes. Grand total: 5235/6958 functions, 1022088/2279368 bytes,
  44.8%. Source/header review found no remaining standards violations.

- EVENT func_8011BE00 is the tenth match, 796 bytes. The cleaned signed
  drift temporary, indexed fields and for loop match all 199 instructions.
  Added scale fields and the twenty-word coordinate table to EVENT.h.

## Inline review

Reviewed the uncommitted diff against HEAD, CODING_STANDARDS.md sections
1-13, CONTRIBUTING.md and the shared wave-11 brief. Used the code-review
skill's standards/spec axes inline because the brief prohibits reviewer
subagents. No git mutations or unowned source/tool/config edits occurred.

- Standards: C89 declarations, fixed-width fields and documented offsets;
  shared headers own the types; generated main declarations and explained
  overrides; three guarded main drafts retain matching assembly fallbacks.
  Allocation-only masks/constants/views are marked FAKE and ticketed.
  Resolved the mask-comment and result-row consistency findings.
- Spec: ten new matches / 11460 literal bytes, recovered shared types and
  reusable idioms, two T-0018 rows. The large DATE match stays in scratch
  pending the requested append-only exception; it is excluded from counts.
  No source exceeding the near-miss limit was retained. Final elapsed-time
  gate and final whole-build/literal checks remain before Done.
- Known tool limitation: m2c invents stale argument-register values at
  several no-argument calls (and a second argument for func_800469F4).
  Removing those arguments fixes otherwise widespread allocator misses.

## Matched functions

| Unit | Function | Literal bytes |
| --- | --- | ---: |
| main | read_bustup_uniform | 980 |
| main | set_tarao_bg | 764 |
| main | SD_GetCDLevel | 736 |
| DATE | func_80138934 | 1740 |
| DATE | func_80134C64 | 1320 |
| DATE | func_8013F638 | 1044 |
| EVENT | func_80115C18 | 1780 |
| EVENT | func_80112BEC | 1284 |
| EVENT | func_8011B190 | 1016 |
| EVENT | func_8011BE00 | 796 |
| Total | 10 new matches | 11460 |

The existing set_tarao_sprt helper remains byte-exact after changing its
definition to K&R. Work-list spans may also contain strings or jump tables;
the table above uses literal text/progress bytes.

## Handoff

All scratch paths below are relative to the shared scratchpad/a19 directory.

- Three retained main drafts: 7EF48 has eight scheduling positions
  (16 +/- lines); 82764 has five positions (10 lines); 7D8AC has two spill
  accesses (4 lines), with its original frame and all registers correct.
- date-pending-body.c / date-pending-full.c: the 5768-byte DATE 3272C match,
  also verified against the whole DATE SHA1. Existing void prototype needs
  the pending append-only exception before integration.
- date-particle-drift.c: DATE 40374, three operand-order differences;
  existing void prototype also conflicts with its observed s32 result.
- date-motion-const-s16-left.c: DATE 3FD6C, seventeen positions; its older
  local D_8015BCB4 declaration should use the shared ten-element declaration.
- best-probes.json / probes.jsonl record traced results; the work tree
  includes only reviewed matches and the three small guarded main gaps.
- Shared headers: CdLevelWork; DateParticleView44; EventParticleView44;
  EventEffectTables; EventResultRow6; DateEventChoice6; ride/museum/count
  table declarations and coordinate/velocity arrays.

Final scratch resolution: date-particle-final-left-s16.c matches all 261
instructions of DATE 40374. Put `s16 two` on the left of both selector
comparisons and the last inequality; this fixes all three operand orders.
The prototype exception still blocks integration, so it is not counted.
The two added T-0018 rows comprise one remaining allocator case and this
case resolved in scratch. The previously listed three-miss draft is
superseded by this exact draft.

## Final validation

Completed 2026-10-13 14:42:18 UTC after 4.0 hours.
All ten new functions pass resolved literal byte checks. The final full
build reports 27/27 checksums OK; headers and globals pass. Prototype
branch check reports zero new disagreements (17 known). Owned duplicate
scan applies zero changes. The final incremental ninja succeeds.

Grand total: 5236/6958 functions, 1022884/2279368 bytes, 44.9%.
Added two T-0018 rows. Inline standards and scope review has no unresolved
findings. Ticket, board and index moved to Done together. Work remains
uncommitted; only the three assigned C files, authorized headers and wiki
pages changed. Pending prototype-exception drafts are excluded from gains.
