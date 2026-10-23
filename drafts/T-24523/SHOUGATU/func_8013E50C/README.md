# SHOUGATU func_8013E50C corrected source checkpoint

**Not matched. No coverage.** This belongs to the correct function and algorithm family. The archived source corrects one concrete field error in the old guard: the random-bit test reads GameState.unk_10F8, address0x800E7378, as in the original; the old unk_0F8 read address0x800E6378.

The immutable target spans file0xC50C..0xC6E3, VRAM0x8013E50C..0x8013E6E3,472 bytes. Generated assembly code bytes within that function span equal the immutable target. Its opening copies two seven-halfword local tables, clears the special flag, tests girl9 status/condition plus a random bit, selects the player and applies a threshold-based stat change. All five calls agree with the guarded source: func_80051A68, func_800847B8, func_80084D3C, func_80085B3C and func_8004284C. The related GYOZI func_80140E0C lacks the local copies and one helper, so it is a388-byte algorithm relative rather than an exact twin.

Fresh same-context full-unit resolved edit cost is152 for both the old guard and this corrected body. Fresh object diffs show the random load changes from0x800E6378 (field offset248) to0x800E7378 (offset4344). That load already differs from the original in its registers, so an unchanged aligned edit count does not imply the address correction had no effect. No stale object or failed compile was scored. No extra source variants tested.

Remaining differences are broad: local table homes at sp+0x48/0x38 versus original0x40/0x30, shifted temporary sequence and a missing promoted zero live range. The existing FAKE base-plus-stride helper argument is retained with its T-24137 explanation. No new header, prototype view or matching control is introduced, and no compiler-limit claim is made.

To reproduce, replace only func_8013E50C in a scratch copy of src/ovl/SHOUGATU/8013E330.c with body.c and use tools/order_search.py make_text_scorer(resolve=True, non_matching=False). Source-only checkpoint; default production uses matching INCLUDE_ASM. No listings, target words, assets or executables included.
