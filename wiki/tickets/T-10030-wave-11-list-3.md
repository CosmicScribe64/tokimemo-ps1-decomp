---
id: T-10030
title: "Wave 11: list 3"
status: Done
assignee: Codex wave-11 agent 3
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Match as many as possible of the 132 assigned functions (52,900 bytes) in the 53 owned C files.
Follow the shared wave-11 brief: initial drafts, ranked deep attempts, type
repair and bounded permutation searches. Spend at least four hours.

## Acceptance criteria

- [x] Verify retained C with resolved function diffs and unit SHA1 checks.
- [x] Run the final incremental build, header/global guards and prototype check.
- [x] Record inline review against CODING_STANDARDS.md and resolve findings.
- [x] Leave scoped, uncommitted edits for the orchestrator.

## Notes

Work list and brief: shared scratchpad `wave11-list-3.txt` and `wave11-brief.md`.
Experiment output stays in `scratchpad/w11-3/`. No toolchain or build-rule edits.

## Comments

- 2026-10-11: Read the wiki index, shared brief, standards, workflow and allocator
  guidance; claimed the batch. Initial worktree has no build directory.

- Verified ETC `func_80146AC0`, 128 bytes: recovered a 0x40-byte work-memory view with two six-word arrays. Plain s32 loop matches; resolved funcdiff and full incremental ninja pass. No FAKE construct.
- Deep source probes: model initializer, 192 compiling variants, best seven
  resolved edits; checksum, 144 named-bound variants (best 41) and 144
  repeated-bound variants (best eight). Five-minute permuters started for both.
- ETC clear type repair: the initial 120 array-override trials failed because
  the shared scalar declaration is not override-guarded; these are not negative
  codegen evidence. A file-local struct view compiled and matched on its first
  natural loop trial, then passed real verification.
- First pass main `initModelingData_init`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80147F78`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80140B20`: ERROR (0 differing instruction lines).
- First pass main `func_8004636C`: ERROR (0 differing instruction lines).
- First pass TT `func_8013D2E0`: ERROR (0 differing instruction lines).
- First pass TT `func_8013F878`: ERROR (0 differing instruction lines).
- First pass main `func_80055A38`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80145EA4`: ERROR (0 differing instruction lines).
- First pass main `_sys_default_tpage_set`: ERROR (0 differing instruction lines).
- First pass BUNKA_SD `func_80134418`: ERROR (0 differing instruction lines).
- First pass main `load_tpage_buf_lock`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_801368B8`: ERROR (0 differing instruction lines).
- First pass main `func_80058F9C`: ERROR (0 differing instruction lines).
- First pass BUNKA_SD `func_80133D54`: ERROR (0 differing instruction lines).
- First pass ETC `func_8013E094`: ERROR (0 differing instruction lines).
- First pass TT `func_8013D3C0`: ERROR (0 differing instruction lines).
- First pass main `func_80045318`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80137228`: ERROR (0 differing instruction lines).
- First pass main `func_8006D6E0`: ERROR (0 differing instruction lines).
- First pass KANGEI `func_801370A0`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80136DA4`: ERROR (0 differing instruction lines).
- First pass ETC `func_80143758`: ERROR (0 differing instruction lines).
- Model and checksum five-minute permuters completed without a match. Recorded
  two measured T-0018 rows with reproducible scratch drafts. Original assembly
  remains in the real sources.
- Historical pass recompiles 46 prior candidates against current headers;
  scores and errors are in `scratchpad/w11-3/history-results.jsonl`.
- First pass TT `func_8013D150`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013AF78`: ERROR (0 differing instruction lines).
- First pass TACO `func_801472D4`: ERROR (0 differing instruction lines).
- First pass TACO `func_8015435C`: ERROR (0 differing instruction lines).
- First pass DATE `func_8015A640`: ERROR (0 differing instruction lines).
- First pass DATE `func_8015A7E0`: ERROR (0 differing instruction lines).
- First pass TACO `func_8015D0D0`: ERROR (0 differing instruction lines).
- First pass TACO `func_80154230`: ERROR (0 differing instruction lines).
- First pass VALEN `func_80133670`: ERROR (0 differing instruction lines).
- First pass TT `func_80145370`: ERROR (0 differing instruction lines).
- First pass TEL `func_8013BE38`: DIFF (107 differing instruction lines).
- First pass TACO `func_80147B24`: ERROR (0 differing instruction lines).
- First pass ENDING `func_80134B18`: DIFF (40 differing instruction lines).
- First pass main `func_80076DA0`: ERROR (0 differing instruction lines).
- First pass TACO `func_801546FC`: ERROR (0 differing instruction lines).
- First pass TT `func_8013E498`: ERROR (0 differing instruction lines).
- First pass GEKO `func_80140040`: ERROR (0 differing instruction lines).
- First pass TACO `func_80146D60`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80138800`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80137090`: ERROR (0 differing instruction lines).
- First pass TACO `func_80154820`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80135DF8`: ERROR (0 differing instruction lines).
- First pass main `func_80065964`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013DAC4`: DIFF (27 differing instruction lines).
- First pass TAIIKU `func_80137440`: ERROR (0 differing instruction lines).
- First pass main `func_80047560`: DIFF (105 differing instruction lines).
- First pass ETC `func_8014407C`: ERROR (0 differing instruction lines).
- First pass main `yuukou_down`: ERROR (0 differing instruction lines).
- First pass SHOUGATU `func_801359FC`: ERROR (0 differing instruction lines).
- First pass ETC `func_80143968`: ERROR (0 differing instruction lines).
- First pass main `func_8004B590`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_801398EC`: ERROR (0 differing instruction lines).
- First pass main `func_8006EA40`: ERROR (0 differing instruction lines).
- First pass main `func_800754C0`: ERROR (0 differing instruction lines).
- First pass GYOZI `func_8013E638`: DIFF (62 differing instruction lines).
- First pass ETC `func_8014354C`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80147A80`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80149144`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80132970`: ERROR (0 differing instruction lines).
- First pass ETC `func_80146400`: ERROR (0 differing instruction lines).
- First pass main `func_80041F48`: ERROR (0 differing instruction lines).
- Further bounded searches: RPG_BAT slot setup, 72 source variants (12 edits)
  plus 300-second permuter, no improvement; NAME_ENT fade, 72 source variants
  (20 edits) plus 300-second permuter, no zero. Two-record checksum
  `func_80055A38`: 264 loop/index/order variants reduce 52 to eight edits, then
  300-second permuter has no improvement. Best drafts remain in scratch.
- First pass NAME_ENT `func_80145FB0`: ERROR (0 differing instruction lines).
- First pass ETC `func_80143F24`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80137C44`: ERROR (0 differing instruction lines).
- First pass RPG_BAT `func_8013E54C`: ERROR (0 differing instruction lines).
- First pass main `func_80042CB0`: ERROR (0 differing instruction lines).
- First pass KANGEI `func_80136970`: ERROR (0 differing instruction lines).

- Verified main `_sys_default_tpage_set`, 300 bytes: five-iteration loop over four GameState array slices, plus a word load through the existing byte-buffer declaration. Resolved funcdiff and full incremental ninja pass; no FAKE construct. Batch total: two functions / 428 bytes.
- First pass TT `func_801330D8`: ERROR (0 differing instruction lines).
- First pass TT `func_8013D508`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_801469D0`: ERROR (0 differing instruction lines).
- First pass SHOUGATU `func_80134A54`: ERROR (0 differing instruction lines).
- First pass main `func_80054E6C`: ERROR (0 differing instruction lines).
- First pass main `col2sepia`: ERROR (0 differing instruction lines).
- First pass main `func_80042AC8`: ERROR (0 differing instruction lines).
- First pass TACO `func_8013A580`: ERROR (0 differing instruction lines).
- First pass TT `func_8013D73C`: ERROR (0 differing instruction lines).
- First pass main `func_8006F218`: ERROR (0 differing instruction lines).
- First pass main `func_80065F34`: ERROR (0 differing instruction lines).
- First pass main `srn_set`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80135480`: ERROR (0 differing instruction lines).
- First pass TACO `func_801471D0`: ERROR (0 differing instruction lines).
- First pass TACO `func_80147928`: ERROR (0 differing instruction lines).
- First pass SHUGAKU `func_801388D8`: DIFF (86 differing instruction lines).
- First pass TT `func_8013F9E0`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013F0DC`: DIFF (34 differing instruction lines).
- First pass NAME_ENT `func_801455E4`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80136EC0`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013FB8C`: DIFF (27 differing instruction lines).
- First pass main `func_8006EEC4`: ERROR (0 differing instruction lines).
- First pass TT `func_801356C8`: ERROR (0 differing instruction lines).

- Verified SHOUGATU `func_80134A54`, 480 bytes: typed word-array reads of the pointer-valued globals and s32 return with zero on failed validation. Successful scene advance retains the original K&R fall-through return. Clean multiline source passes resolved diff and full incremental ninja. Total three functions / 908 bytes.
- First pass TT `func_8013CC24`: ERROR (0 differing instruction lines).
- First pass main `func_8006EB64`: ERROR (0 differing instruction lines).
- First pass KANGEI `func_80135678`: ERROR (0 differing instruction lines).
- First pass KANGEI `func_80135004`: DIFF (44 differing instruction lines).
- First pass TAIIKU `func_80135830`: ERROR (0 differing instruction lines).
- First pass RPG_BAT `func_8014C360`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_80137EA8`: ERROR (0 differing instruction lines).
- First pass main `func_80063668`: ERROR (0 differing instruction lines).
- First pass RPG_BAT `func_8014AB84`: ERROR (0 differing instruction lines).
- First pass DATE `func_801560E0`: ERROR (0 differing instruction lines).
- First pass main `func_800419FC`: ERROR (0 differing instruction lines).
- First pass main `func_80049FF0`: ERROR (0 differing instruction lines).
- First pass TACO `func_80146F74`: ERROR (0 differing instruction lines).
- First pass main `geko_judge`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_80149B08`: ERROR (0 differing instruction lines).
- First pass TAIIKU `func_801365C8`: ERROR (0 differing instruction lines).
- First pass TACO `func_80147674`: ERROR (0 differing instruction lines).
- First pass main `func_800590CC`: DIFF (40 differing instruction lines).
- First pass GYOZI `func_8013DC2C`: ERROR (0 differing instruction lines).
- First pass SHOUGATU `func_8013D820`: DIFF (60 differing instruction lines).
- First pass main `search_tpage`: ERROR (0 differing instruction lines).
- First pass main `search_tpage8`: ERROR (0 differing instruction lines).
- First pass SHOUGATU `func_80133760`: ERROR (0 differing instruction lines).
- First pass main `func_80041C2C`: ERROR (0 differing instruction lines).
- First pass TT `func_80135870`: ERROR (0 differing instruction lines).
- First pass TT `func_801336F8`: ERROR (0 differing instruction lines).
- First pass main `schedule_girls_gakko`: DIFF (74 differing instruction lines).
- First pass main `func_800764AC`: ERROR (0 differing instruction lines).
- First pass main `func_800591D8`: ERROR (0 differing instruction lines).
- First pass SHOUGATU `func_8013D92C`: DIFF (85 differing instruction lines).
- First pass main `make_color_bar`: ERROR (0 differing instruction lines).
- First pass ETC `func_80149F48`: ERROR (0 differing instruction lines).
- First pass main `func_80046754`: ERROR (0 differing instruction lines).
- First pass RPG_BAT `func_8015461C`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013A9C8`: ERROR (0 differing instruction lines).
- First pass NAME_ENT `func_8013BE18`: DIFF (70 differing instruction lines).
- First pass NAME_ENT `func_8013F454`: DIFF (23 differing instruction lines).
- First pass NAME_ENT `func_8013D4CC`: DIFF (33 differing instruction lines).
- First pass NAME_ENT `func_801473AC`: ERROR (0 differing instruction lines).
- First-pass queue completed through entry 132. The already verified ETC clear
  helper was skipped; all other entries have m2c and real build/diff results.
  Scratch-only memory-width repair is also complete. Continue ranked deep work.
- Historical collector caveat: some wave-10 `body` values were paths rather
  than inline C. The first recompile correctly recorded syntax errors for
  these; GYOZI `func_8013E638` is now retried from its actual saved source.
- Second deep pass: VALEN `func_80133670` reduced to four differing lines
  (the first girl load uses the argument register as its address base). A
  five-minute permuter did not improve it. NAME_ENT selector-family searches
  are running separately for five candidates.
- ETC `func_8014354C`: corrected historical draft's element/byte stride
  confusion (0x50-byte coordinate records, 0x10-byte objects) and removed
  the spurious fourth argument. Best scratch draft is 18 differing lines;
  remaining differences include folded constant address setup and scheduling.
- KANGEI `func_80135004`: replacing four scalar halfwords with an actual RECT
  is being checked against the original frame; the raw scalar draft wrongly
  eliminates three stores. Main video-mode and ENDING fade source variants
  are also in progress.
- TAIIKU `func_801368B8`: raw m2c conflates adjacent store destinations.
  A 0x24-stride aggregate loop reproduces the counter widths/offsets and
  reduces the draft to 24 lines. The remaining pattern is shared-$at and
  scheduling. The object has an owned data range, but no data island in this
  worktree; scratch own-data definitions are being tested before any decision.
- TACO `func_8013A580`: the five-minute permuter found a self-copy that changes
  spill allocation. Manual probes reduced the draft to six differing lines
  (one spill slot). These self-copy/padding probes are diagnostic fakematches;
  none has been installed or counted.
- Header-return repair recovered compiling drafts for TAIIKU `func_80136DA4`
  (40 lines) and main `func_80049FF0` (78). Other failed drafts still need real
  aggregate/declaration recovery; they are not negative matching evidence.

- Verified TACO `func_8015435C`, 224 bytes: adjacent pointer-table bounds, u16 second parameter, and named u32 command constants. The constants fix the comparison operand order without a FAKE. Clean pointer-table source passes resolved funcdiff and the full incremental Docker build. Total four functions / 1132 bytes.

- Verified main `func_80041F48`, 272 bytes: natural indexed loops, a 0x100 byte-block view spanning GameState +0x56C through +0x66B, and GsWord views for four-byte accesses. Explicit u8-pointer arithmetic prevented strength reduction in the raw drafts; aggregate indexing gives the original loop pointers. Resolved diff and full incremental Docker ninja pass. Total five functions / 1404 bytes.

- Formatting review for `func_80041F48`: a conventional multiline while loop for the final 16-byte clear retains the exact store ordering. All loops now use normal multiline bodies; no source-line FAKE is needed. Real resolved diff and incremental build rechecked.
- Extended the type-repair pass to packed RGB5551 pairs in `col2sepia` and
  `make_color_bar`; both now have compilable bitfield drafts. Initial
  differences remain substantial, so they are scratch-only.
- Main `func_80076DA0` is down to five differing lines: one extra global
  address setup inside the three-entry history loop, with the event-record
  writes matching. Testing common-base aggregate views before classifying it.
- TAIIKU `func_80137440` now compiles after replacing the undeclared end
  symbol with the three-record extent. Its global reload and local lifetime
  require further work; no draft has been installed.
- Recovered typed TT TILE/ordering-table drafts for `func_8013D2E0` and
  `func_8013D3C0`, reusing shared packet and render-buffer types. Caching the
  core pointer removes spurious reloads; the fade helper reaches 24 differing
  lines, chiefly red/green allocation and store scheduling. A 288-variant
  declaration/type/layout search did not resolve them.
- TACO `func_801472D4` now has a natural nested-loop MATRIX draft, 28 differing
  lines. Its matrix home is eight bytes low despite the correct total frame;
  inline-copy temporary allocation also differs. Testing actual loaded-value
  locals before considering this a frame/layout blocker.
- NAME_ENT `func_8013AF78`, main `func_8006EA40`, and KANGEI `func_80135678`
  now compile with recovered local arrays/table copies. They remain scratch
  candidates pending control-flow, frame, and register work.
- The scratch symbol-repair sweep recovered additional compilable drafts, but
  inferred declarations and raw offset repairs are diagnostic only. They have
  not been added to headers or installed as matches.

- Added 17 further bounded blocker rows after scoped register_shapes checks;
  19 T-0018 rows total so far. These record observed source/compiler differences,
  not proof that no C source can match. Assembly remains the fallback.
- Both `search_tpage` and `search_tpage8` reach four differing lines after
  reconstructing their loops from matched `search_tpage_multi`. Integer-address
  construction preserves the original table-base lifetime and reloads after
  slot writes; the typed-array base folds those operations. Two comparisons
  still have reversed operands after signedness/pointer/expression variants.
  This near draft is scratch-only and would need a FAKE explanation if retained.
- Inline review replaced the SHOUGATU validation helper's verbose offset
  dereferences with ordinary word-array indexing. Scratch and real resolved
  comparisons still match. No new function counted for this cleanup.
- Duplicate-tool caveat: `dupes.load_funcs` classifies matching status from
  generated asm directory placement, so its report still counted 4,982 matched
  functions after the five new C definitions. `ninja progress` correctly reports
  4,987. A direct fingerprint audit of the five gains against current owned
  INCLUDE_ASM targets found no exact or ALU-immediate twin. No tool was edited.

- Verified TT `func_8013F878`, 272 bytes: the scene holds ten 0x14-byte
  animation records at +0x40 and a u16 timer at +0x364. Ordinary field accesses
  recover the original duplicate timer loads and byte-value promotion across
  each clamp. Scratch formatting and real resolved comparisons match. No FAKE;
  six functions / 1676 bytes total.

- Verified NAME_ENT `func_801455E4`, 380 bytes, in the real object. Typed actor
  records and signed fixed-point halves reduce the body to one setup-order
  mismatch. A marked FAKE joins pointer initialization to the do-loop opening
  line; cleaner loop/declaration/K&R forms were tested first. Two endpoint
  globals were added by `sync_protos.py --add-global`. Seven functions / 2056
  bytes total; one new FAKE. Header-triggered incremental build in progress.
- TT `func_8013F9E0` reaches six differing lines after reconstructing the
  packet, coordinate-array, and flag-array scene layout. Its full control flow
  and stores match; three setup instructions are reordered. Bounded source
  variants and a 600-second permuter are being tested in scratch.

- Verified KANGEI `func_80135678`, 300 bytes. The 32-entry function-pointer
  table is copied as a 0x80-byte aggregate; its declaration was appended to the
  overlay header. A documented two-byte control view preserves the reload
  between neighbouring writes. A marked FAKE keeps the early condition and
  store on one line, resolving timer/girl address-load order. Real resolved
  comparison passes. Eight functions / 2356 bytes, two new FAKEs.

- Verified main `search_tpage` and `search_tpage8`, 616 bytes each. Inlining
  the page-table expression instead of caching a `page` scalar fixes the first
  comparison's operand order; putting the slot expression first fixes the
  second. This also makes the ordinary typed record-array base match, so the
  scratch integer-address construction is unnecessary. Clean multiline source,
  no FAKE. Real resolved comparisons pass.
- Verified RPG_BAT `func_8015461C`, 324 bytes. A six-record loop through the
  existing `RpgFlagRecord44` type produces the original peel-two/unroll-four
  sequence, including the computed two-record base. Removed the draft's
  spurious arguments to the no-argument helper. Appended the overlay global and
  function declarations; the forward declaration resolves an earlier implicit
  int use in the same file. Real resolved comparison passes. Eleven functions /
  3912 bytes total, two new FAKEs.
- Inline review found only minor whitespace/layout-comment cleanup and the
  missing RPG forward declaration above; those are fixed. Current-source real
  resolved comparisons were repeated for the retained functions. The full
  review and final build/checks remain to be completed before Done.
- Eleven-match incremental build passes all 27 binary SHA1 checks. Final
  prototype guard reports zero new disagreements (17 known) and branch check
  OK; global migration check passes. Progress: 4993/6958 functions,
  840852/2279368 bytes (36.9%). Scoped duplicate/near-duplicate tools apply no
  changes, and the current-source fingerprint audit finds no missed owned twin.
- Verified TAIIKU `func_80137440`, 260 bytes. Typed record traversal and a
  timer reload between flag stores match the body. Two marked FAKE constructs
  retain indirect timer reads and the pointer-initialization/do-loop source
  line. Direct scalar reads and separate-line initialization were checked;
  both introduce differences. Real resolved comparison passes. Total: twelve
  functions / 4172 bytes, three functions carrying new FAKE annotations.
- Final checksum-only incremental build passes 27/27, with header and global
  guards passing. Prototype check again reports zero new/17 known disagreements
  and branch check OK. Scope audit confirms all twelve gains are assigned
  functions and there are no tools, configuration or raw-source edits.
- Added three further bounded T-0018 rows for the TT scene setup, TT TILE
  fade helper and TACO MATRIX helper (22 rows so far). Later ETC score-table
  and NAME_ENT averaging repairs improved their drafts but did not match;
  they remain private scratch candidates with assembly in the real source.
- Inline code review completed against every CODING_STANDARDS.md checklist
  item and the shared brief, comparing the working changes with starting HEAD
  c55a3b8c564cd5c58d1eb7a7e8f987e72d4c165f. All retained functions have real
  resolved verification and all 27 binary checks pass. Reviewed C89 syntax,
  fixed-width types, aggregate offsets and extents, existing SDK type reuse,
  canonical/append-only declarations, function placement and source ownership.
  Three functions carry four explicit FAKE annotations, each explained and
  ticketed; all failed drafts remain assembly-backed. The formatting/layout
  comments and RPG forward-declaration findings were resolved and verified.
  No unresolved code-review findings, generated-asm edits, raw-data edits,
  tool/config changes, host installs or Git mutations. In Review while the
  final bounded searches and the requested four-hour period finish.
- Final schedule comparison variants also fail to match; added its bounded
  scheduling/frame row, bringing the batch to 23 T-0018 rows. The top other
  blocker remains TACO `func_8013A580` (local frame versus spill-home layout).
  All unresolved functions retain their original INCLUDE_ASM fallback.
- Final allocator trace compares the TAIIKU indirect timer read with the
  otherwise identical scalar form: the latter introduces a separately
  profitable global range (priority 0.5, v0), while the timer local is a0.
  The indirect view makes the global range unprofitable and verifies exactly;
  scalar form has 13 edits. Recorded the bounded explanation in matching notes.
- The final 300-second scalar-timer permuter has no score-zero candidate, so
  the reviewed indirect-read FAKE remains. The broad queue completed 34 bounded
  attempts before its checkpoint stop; all owned searches have ended. Final
  retained result is twelve functions / 4172 bytes, with 23 new T-0018 rows.
  Progress: `grand total   4994/6958   841112/2279368  36.9%`.
  Append-only audit passes for the shared wiki pages and overlay headers.
  No uncommitted changes outside the authorized source/header/wiki scope.
- Completed after approximately 00:53–04:55 UTC on October 12 (October 11
  locally), exceeding the requested four hours. Inline review and all required
  checks pass; ticket moved to Done. Changes are left uncommitted for merge.
