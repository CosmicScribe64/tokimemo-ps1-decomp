---
id: T-20007
title: Recover the large dialogue renderer
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-17
links: ["[[main-family-results]]", "[[matching-notes]]", "[[ref-english-patch]]"]
---

## Goal

Match main dialogue_render (5,588 bytes, src/main/8007C030.c), which expands dialogue text/name tokens and calls the kanji renderer. Claim opened before implementation: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/45. Other functions in this file and TT are outside the claim.

## Acceptance criteria

- [x] Recover plausible buffers, loop locals and text expansion control flow from original assembly.
- [x] Any production C passes actual default linked proof and full binary/header/global/prototype checks.
- [x] Document the source/type findings, review against CODING_STANDARDS.md and submit verified work.

## Notes

Branch codex/large-scene-handlers starts at main 5df5fdc6. Size-ranked survey finds this is the fourth-largest remaining game function outside TT in the archived census and current ASM fallbacks. The larger dialogue_speak_part has a documented front-end/as1 scheduling obstacle after extensive previous probes; it is deliberately excluded. Prior renderer drafts have many scalar buffers, duplicated register temporaries and unresolved three-byte copies. Work from the original, keep probes ignored and run toolchains only in Docker. Do not merge moving main repeatedly; maintainer handles integration.

## Comments

PR #44 remains open without feedback at this start. Its GYOZI changes are not part of this branch.

Initial scratch controls: repaired census buffers plus real three-byte auto-array initializers compile, but still differ by 1,751 rows. Merging only per-branch copy counters worsens the register layout; narrow parameters alone do not repair it. The original frame is 0x2F8 (760 bytes), versus the archived draft around 0x3F0 (1,008 bytes); many m2c temporaries are separate C locals. Five original lwl/swl pairs copy three-byte local punctuation initializers. Reconstructing shared scanner/copy indices, 512/64/24-byte buffers and the two original expansion loops as structured C before tuning allocation.

Structured source reduces the diagnostic score to 1,232 rows and reaches the original 760-byte frame; ordinary compound updates/subscripts improve it to 1,208, and declaration placement reaches 1,196 (forms/layout logs). Array/local offsets and parser block placement still differ, so this is not a match. The original final-line setup stores the raw arg1 + row sum at sp+0x68 while separately narrowing the drawing Y at sp+0x64. The archive narrowed both, and the first structured draft inherited that error. Corrected the scratch semantics to retain the full sum, and testing explicit per-token branches plus a common result variable before further register/layout work.

The shared narrow-index advance block is important: keeping a single token condition and explicit common advance restores the original 1,397-instruction count (shared.log), whereas a split per-token goto chain adds unwanted loads. Current best is 1,090 differing rows, dominated by an 8-byte frame/offset shift and repeated operand/temporary choices. A common return reuses the finished raw-Y scratch word rather than adding a dummy local. A normalized instruction-shape diagnostic distinguishes these layout/register differences from semantic/control-flow differences; it is not a byte-match proof. Coalescing the archived m2c temporaries preserves too many artificial addresses and is worse (1,736 rows).

Breakthrough: remove the explicit line-cursor local and let ordinary array-index expressions supply the shared pointer. This reaches the original 760-byte frame and 1,397 instructions at 982 rows. Splitting each narrow output-index update into out_index += strlen(name); out_index -= 2; restores an otherwise missing ugen temporary; as1 removes the redundant narrowing while retaining the original later temporary numbers. The full-unit score drops to 358 rows (split.log). This is an ordinary two-statement update, not an added no-op or forced register. Local offsets and a few operand/library-view differences remain. Maintainer confirms exclusive function ownership in issue #45; PR #44 is now merged after independent 27/27 and linked verification.

Further controls: a word/scalar placement change lowers the score to 312 rows, but moves a referenced line-state word above the arrays, so it is not a final layout. Keep the simpler 358-row indexed source as the next probe base. Reversing source compare operands is canonicalized away. A void strncpy alias is 12 rows worse than the implicit/s32 view; integer-address views do not fix the repeated pointer-add operand order. The corrected raw/narrow Y distinction is a dataflow fact: the final next-Y global is written as a halfword, so no user-visible arithmetic bug is asserted. Source sequencing and scalar layout are still being recovered; no compiler-gap claim is made.

2026-10-17 continuation: moving each name-copy counter initialization after its strcpy/sprintf setup reduces the clean diagnostic from 358 to 330 rows. Keeping the three related narrow-index updates on one physical source line further reduces it to 242 rows; this is a measured scheduler-sensitive source layout and must be marked FAKE if retained. An unsigned index view does not change the source-pointer operand order; signed casts of the byte token worsen code generation. Still 1,397 instructions and a 760-byte frame, with most remaining differences in stack homes and repeated commutative operands. No production C or match claim yet.

Further source recovery reaches 120 rows: direct text[scan] accesses in the initial punctuation scan restore all eight cached-token compare operand orders. A signed-byte pointer view at each tail-copy expression restores the 22 repeated pointer-add operand orders; declaration-only signedness does not, so that expression cast is a FAKE candidate if retained. Moving meaningful locals aligns the line/name/text buffers and row home; removing the explicit drawing-Y local lets the shared call expression recover both original Y spill homes, reaching 106 rows. Remaining differences are the three-byte opening/punctuation local homes, the line-source save, and two short scheduling sequences. Opening-buffer extents 4 and 8 grow the frame and worsen the result; no oversized buffer retained. Default source is still unchanged.

Large-function breakthrough: the saved line source is a compiler-generated common expression, not an explicit pointer local. Preserve the line-start index and give the substitution copy loop its own s16 copy index; repeated (text + i) expressions then recover sp+0x60, with the drawing/raw-Y expressions at sp+0x64/0x68. This reduces 80 diagnostic rows to 10 without padding, forced registers, or the previously tested signed-pointer casts. Initializing the two expansion indices in each for head (rather than before the parser) restores the last five hoisted-address scheduling words. All 1,397 instructions now agree in the full-unit scratch comparison (loop_init/0); actual linked verification and full checks remain mandatory. The grouped narrow updates still matter (ungrouping is 98 rows) and will carry FAKE comments.

## Recovered renderer behavior

The 512-byte input copy is scanned as two-byte glyphs interleaved with one-byte controls. Space and q split lines; the expansion loops substitute the following tokens in their original order:

| Token | Replacement |
| --- | --- |
| P | D_800CA17C |
| S | D_800CA23C |
| G | girl_name_buf |
| p | D_800CA16C |
| Y | name buffer initialized to the fixed companion name |
| F | name buffer formatted from D_800CA16C and D_800CA174 |
| m | name buffer copied from D_800CA174 |
| O | D_800CA248 |
| W | D_800CA250 |
| A | D_800CA25C |
| M | D_800CA280 |

Each substitution copies its replacement and the remaining source tail into the 64-byte line buffer, then adjusts the two short indices and line length. The 24-byte name temporary is shared by the formatted-name cases. Prefix/suffix drawing, the opening full-width parenthesis X adjustment, the punctuation-only flag and the next-line Y update retain their original control flow. The two expansion copies remain separate because that is how the original instructions are laid out.

Production integration uses shared s32 dialogue_render() with callee-side K&R narrowing, removing this file's old dialogue_render override. D_800CA280 and sprintf are declared once in main_api.h. The SDK's linked symbol is strncpy; the old func_800AE090 name is an overlay address alias and does not link by name in main. Declare the SDK alias with the existing shared void view, then retain this file's measured s32 strncpy() view through an explicit override. The void view changes the length result from v1 to v0 (12 rows). Both this view and the grouped narrow updates are marked FAKE and tracked here. No pointer cast, buffer padding, forced register, toolchain change or ASM edit is retained.

## Self-review against CODING_STANDARDS.md

Reviewed the scoped source/header diff: C89 declarations and comments, fixed-width locals, original function position, SDK/game types and existing symbol names preserved. Only dialogue_render replaces ASM; the sibling guarded dialogue_speak_part and other functions are unchanged. No raw/ASM/generated files or game assets are included. All new main symbols have a single shared declaration; the old dialogue_render file override is removed after trying the shared view. The measured strncpy override and both grouped-update comments identify T-20007 and their reason. No oversized buffer, dummy local, no-op, register forcing or compiler flag change remains. All validation below passes; submit for upstream review with the ticket In Review.

## Final validation (2026-10-17)

Docker evidence in ignored build/T-20007:

- full-build.log: ninja -j8 completes all 526 steps, including headers OK and globals OK.
- checksums.log: main plus all 26 overlays match immutable configured SHA1s (27/27).
- protos.log: sync_protos.py --check-branch reports 0 new disagreements, 15 known; branch check OK. No baseline exception was added.
- final-proof.log: funcdiff.py --resolve --version default main:dialogue_render reports MATCH, linked function bytes equal target (5,588 bytes).
- git diff --check passes; only source/header/wiki files are included.

Final pre-submission fetch is origin/main 2de889b8; dialogue_render is still INCLUDE_ASM there. Branch remains based on 5df5fdc6 with no main merge, per maintainer coordination. Upstream review is pending; do not mark Done until it is accepted.

## Submission

PR #46: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/46 (open, attached to the Codex chat). GitHub reports integration conflicts against newer main; per existing maintainer coordination, do not repeatedly merge moving main into this branch. The claimed renderer is still ASM upstream and the verified branch is unchanged. No immediate review comments or hosted checks were present at submission. Ticket remains In Review for maintainer integration/review.
