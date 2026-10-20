# VALEN func_80133088 source checkpoint

**Not matched. No coverage.** Production retains the matching INCLUDE_ASM fallback installed by 7a6290955. This is the correct location/background function, based on T-19016, not the structurally stale stats-update guard removed earlier.

The exact annotated body.c compiles with full-file aligned resolved edit cost **14** against the immutable 1,108-byte target. Replace only the target INCLUDE_ASM in a scratch copy of src/ovl/VALEN/80132760.c, then use tools/order_search.py make_text_scorer(resolve=True, non_matching=False). The scratch extern D_80122D48 must gain a canonical declaration before any future production promotion.

## Causal reconstruction

- Same-context T-19016 baseline189; explicit forward labels173 (retains the first two case branches and inline default store).
- An unsigned-byte cast only on the player seed override restores the exact original move/mask/move sequence and later temporary numbers:127. Casting/replacing the initial selector globally instead worsens227.
- A GameState pointer only from location_ready through the middle conditions prevents the unwanted early selected-girl promotion into a0:54. The initial selector prefix stays direct.
- Removing the old single-use cur scalar makes no further difference. Separate C89 scopes recover the 0x30 frame but leave the id spill at sp+0x2C:50.
- A documented FAKE union shares the disjoint selector and pointer phase word. It restores both frame0x30 and id spill sp+0x28:14. The active member is written before every use; the selector is dead when the pointer phase begins. Annotated source was recompiled and still measures14.

## Exact remaining differences

The initial third dispatch at VRAM0x801330F8 (function+0x70) originally branches on equality with2 to0x8013314C, followed at0x80133100 (+0x78) by the unconditional jump to0x80133164. The draft branches on inequality to0x80133164 and then jumps to0x8013314C. Its two target blocks retain the correct order and instruction counts. This is a remaining source/branch topology problem, not an asserted compiler limit.

Two strcpy calls differ in argument/id scheduling, with all other instructions and spill positions aligned:

- Gate-text call at0x801331F8: original source-address high setup precedes id0x4032 and its spill, with source-address low setup in the call delay slot. Current id comes first, then both source-address halves; the id spill fills the delay slot.
- Classroom-text call at0x8013329C: original source-address high setup precedes id0x4055 and its spill, followed by the source-address low setup. Current id comes first, then both source-address halves and the spill. The destination-address delay slot already agrees.

Together these yield14 aligned insert/delete edits. No instruction listing or target words included.

## Bounded negative probes

A nested one-case third switch leaves173 before later improvements; adding an explicit case3/default alias to the14 form worsens21. Joining only the two id assignments and copy calls onto single source lines leaves14. Putting those id assignments in destination comma expressions leaves14. Block-local named string-pointer ranges before those assignments also leave14. Neither integer nor actual pointer return views of the strcpy helper improve14; an already-started bounded13-single return/prototype pass also found no improvement. No view changes retained. No broad permutation, declaration-order grid, compiler flags or toolchain patches.

Default production source and all headers are unchanged by this checkpoint. Parent review is recorded in T-24523 before pinning.
