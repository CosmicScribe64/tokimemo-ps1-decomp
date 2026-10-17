---
id: T-20008
title: Recover the large NAME_ENT introduction handler
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[matching-notes]]", "[[ref-english-patch]]"]
---

## Goal

Recover NAME_ENT func_80143580 (4,308 bytes) in src/ovl/NAME_ENT/80142A80.c. This submission is a compiling reconstruction checkpoint in drafts/T-20008; matching remains unfinished and production stays ASM. Claim #50 was opened before implementation: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/50. Neighboring functions, TT and DATE2 are outside this scope.

## Acceptance criteria

- [x] Recover the tile-address work array, scene steps and record/palette loops from original assembly.
- [x] Archive a compiling full-unit draft, reproducible view sweep and explicit remaining differences.
- [x] Verify the default fallback, all 27 binary hashes and declaration/global/prototype checks.
- [x] Review against CODING_STANDARDS.md and prepare a scoped archive PR.

## Remaining matching work

- [ ] Resolve the instruction/register differences and verify any future production C against actual linked bytes. The archived C is not matched; zero new verified C bytes are claimed.

## Notes

Branch codex/large-scene-followup starts at upstream d73328b8 after PR #46 merged and the maintainer independently confirmed the dialogue renderer's 5,588-byte MATCH and 27/27 hashes. DATE2 is claimed in issue #49; leave it untouched. Current target is ASM with an archived malformed m2c draft (1,168 rows in the older survey).

The opening original loop writes eight tile addresses at sp+0x6C in a 0x90-byte frame. The archived draft mistakes an addiu relocation low half for a pointer symbol and loses the 0x4000 increment; typed-pointer increments and Rec24/GameState strides are also malformed. Reconstruct these source loops and use existing aggregate types before tuning registers.

Initial semantic reconstruction compiles in the full original translation unit (1,079 diagnostic rows). Corrected eight 0x4000-spaced tile addresses, three Rec24 records, the 128-word palette copy and byte color fields in Rec24. The archived D_80121808 word declaration would emit a wrong-width store; use record[1].unk_14 and the other real fields. The first structured loop is unrolled by IDO and the frame is 0xA0 rather than 0x90; test pointer-address induction and remove unused/derived locals before scheduling work. Production remains ASM.

Assembly audit found two additional archived-draft semantic errors: the horizontal thresholds in steps 1 and 8 compare the value before decrementing (post-decrement tests), and the step-4 helper path reloads the scene selector at 0x80143850 before falling through to step 5. The missing reload wrongly kept the pre-call selector alive across the helper and restored an obsolete step. Scratch candidates now recover those facts before register tuning. A shared loop index brings the frame and tile array back to 0x90 and sp+0x6C; remaining save/register and loop-shape differences are still under investigation. Maintainer confirms exclusive function ownership in issue #50.

Further controls: ordinary shared-counter nested for loops recover the original rolled initial tile loop, while a plain do-while is unrolled by four. Pointer/integer induction views alone do not fix it. Different fade/selector widths and direct/global-pointer views do not resolve the remaining register web. The best older semantic candidate is 941 rows; corrected post-decrement/reload candidates remain around 1,000. Do not choose a lower diagnostic score over those assembly-confirmed semantics. Building the documented pinned IDO uopt tracer in ignored Docker scratch and generating a fresh current-context m2c draft to diagnose the saved-register decision; no production source/header changes.

Allocator diagnostics are available in ignored build/T-20008/compiler/variants/trace, built from the documented pinned IDO revision; stock toolchain and flags are unchanged. The return-view sweep tests 39 combinations with no untested variants and improves the best typed candidate from 849 to 797 rows with s32 func_8004A1A0(). Uniform unsigned flag views reduce duplicate 0x01000000/0x80000000 ranges; these are scratch type controls, not adopted header changes. Pointer/byte palette walkers, phase-local byte snapshots and an escaped local-work-record control have not matched. A consistent aligned-distance ranking avoids the scorer discontinuity when candidate/target instruction counts happen to coincide; best semantic candidates still have substantial register/scheduling differences. No new verified C bytes yet.

Consistent alignment changes the diagnostic ranking: some equal-length candidates looked much worse because row-by-row output shifted, while candidates with different lengths used an aligned diff. The current runner records aligned edit distance and raw diff rows separately. Unsigned resource-call addresses separate loop-bound/call constant ranges and remove the extra saved register in the best structural control. The remaining sprite-loop sll/sra operations identify another draft type mistake: x/y are word induction counters feeding halfword record stores. Testing s32 coordinates and direct post-update byte arguments next. No padding or per-function compiler options are introduced.

The latest controls recover the original 0x90-byte frame, ra save at sp+0x2C and tile table at sp+0x6C without a saved s0. Scoped unsigned flag stores compile like the broad unsigned Rec24/F4 control, avoiding a shared type change. Ordinary indexed tile fills eliminate the artificially cached array-base range; reusing the scene-selector local for phase counters and computing depth in the field assignment also recovers the shared 0x01000000 range. Best consistent aligned distance improves from 413 to 378 instructions, still nonmatching. Direct/global color update forms and first-argument/own return-view controls do not close the remaining gap. These are source-shape findings, not a measured compiler limitation or a claim of new matched bytes.

## Final archive and validation

The cleaned archive uses the real SpriteSlot24 work-area fields, Rec24 sprite fields and GameState fields. Remove unused locals instead of retaining them as hidden frame padding. The resulting canonical-header archive is 1,078 instruction rows / 835 resolved diff rows / 448 aligned edits; the diagnostic s32 func_8004A1A0() view is 799 rows / 430 aligned edits. The view sweep completed 39 compiles with zero untested variants. Earlier scalar/pointer controls reached lower diagnostic scores but are not the archived source or production matches. The cleaned frame is 0x88/table sp+0x64; original 0x90/sp+0x6C remains a matching task.

Docker validation: ninja completed 605 steps including header/global guards; 27/27 independent binary SHA1 checks passed; sync_protos.py --check-branch reports 0 new / 15 known disagreements. Actual default funcdiff reports MATCH for 4,308 bytes through the unchanged ASM fallback, not through the archived C. No production source/header/compiler/build changes are submitted.

Self-review against CODING_STANDARDS.md: C89 and fixed-width types; existing aggregates and canonical symbols; scratch declarations only for missing functions; every retained allocator/redundant-loop source control is marked FAKE with T-20008; no generated asm, comparison instruction listings, game assets, binaries, compiler artifacts or one-off probe scripts included. README and result.json distinguish canonical C, diagnostic view and default ASM results. Ticket/kanban synchronized to In Review; maintainer review remains pending.

## Comments

2026-10-17, maintainer review (PR #51): draft checkpoint merged; no findings (archive files hold C and notes only, no instruction listings; FAKE controls name T-20008). Matching continues under claim #50, so the ticket goes back to In Progress until a match or a stop.
