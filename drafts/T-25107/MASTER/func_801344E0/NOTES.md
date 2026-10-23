# T-25107: MASTER func_801344E0 (2,912 bytes), matched

Start: plain `INCLUDE_ASM`. Best earlier body: `drafts/T-24519/func_801344E0.c`
(the T-24048 body with the clone and point loops in the indexed form of
`func_801363F4`). Under a guard it needs `void func_80137BF4(void);`, which
this package adds to `include/ovl/MASTER.h`. Measured whole-unit: 171 rows. The
first divergence was row 289, the end pointer of the first 0x44-byte record
copy in case 4: target `addiu t5,t3,60`, candidate `t6` (one ring slot high).

## Cause and fix

This is the block-copy state miss that closed `func_801363F4` in the same
package (see `drafts/T-25107/MASTER/func_801363F4/NOTES.md`). Case 4 of this
function has the same record setup, so both fixes transfer unchanged:

1. FAKE (T-25107): the never-true arm
   `if ((u32)D_8013C578 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_8013C578 < 1); }`
   just before the first record copy. `tools/ugen_pass.py` then picks state F
   for both copies (record copy and the `[i] = [1]` copy loop).
2. The point loop stores x before y:
   `D_8013C58C[i].x = value - 0xA0; D_8013C58C[i].y = 0x3C;`.

Result: 0 rows, default build MATCH (`compared default build`).

## Measured negatives

- Arm only, point loop still y before x: 5 rows, first at row 386
  (`li t2,60` / `sh t2,-2(v1)`), the same 5 rows as `func_801363F4` had.

## Next

ENDING `func_80134270` has the same recorded symptom (T-19001, "F after
global byte stores"). Try the same arm before its first copy after checking
which of its copies need state F.
