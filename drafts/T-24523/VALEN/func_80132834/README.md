# VALEN func_80132834 typed source checkpoint

**Not matched. No coverage.** Correct source reconstruction from the immutable 1240-byte target, informed by GYOZI func_80138C9C and the earlier T-14500/T-16100 VALEN decompiler checkpoints. The old production guard described a different sequence of operations; its historical near-match metric does not describe this target.

Full-unit aligned resolved edit cost is381. Replace only this function in a scratch copy of src/ovl/VALEN/80132760.c with body.c and use tools/order_search.py make_text_scorer(resolve=True, non_matching=False). No shared header or prototype view needed.

The target spans file0x834..0xD0B, VRAM0x80132834..0x80132D0B, with next function0x80132D0C. It clears current/count/mode/special, tests girl9 flag and signed-halfword condition, sets that girl's affinity to50, computes a strict top-three ranking, gathers candidates, rotates their order and continues through reorder traversal without breaks. The shared gather/rotation/reorder structure has normalized original opcode similarity0.60215 to GYOZI38C9C; it is a related algorithm rather than an exact twin.

Types follow target accesses: girl9 condition at GameState+0x3B6, affinity store+0x3BA, player condition+0x1BE, random word+0x10F8; CharFlags b1/b8 match the observed word test and byte update. Three rank values have signed-halfword homes sp+0x50/0x52/0x54, and the gathered candidate list begins sp+0x5C. The source's list capacity13 is inherited from the sibling foundation and fits the observed remaining frame space; its exact original capacity is unknown and is marked FAKE. This body produces frame0x70 instead of target0x78, promotes the ranking values differently, and retains broad register/scheduling differences. It is useful semantic source, not a near-match or compiler-limit claim.

One prior probe applied assignment-only *1 wrappers to the old, wrong guard:484 baseline to491 modified. Those results do not test the correct target's ranking tail. No wrapper retained and no additional width/declaration grid run. Any further identity probe must begin from this correct foundation, preserving signed-halfword ranking semantics and the third assignment's shared comparison value.

No assembly listing, instruction words, executable or asset is included. Production retains the original matching INCLUDE_ASM fallback after separate stale-guard cleanup. Parent review is recorded in T-24523.
