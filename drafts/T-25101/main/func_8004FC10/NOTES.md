# T-25101: main func_8004FC10 (menu cursor step, 1812 bytes)

Result: matched as default C (commit "T-25101: match main:func_8004FC10"). `funcdiff --resolve --version c` and `--version default` both print MATCH (1812 bytes), with no INCLUDE_ASM left for the function. No FAKE.

## Start state

INCLUDE_ASM. No usable draft: the 752-row cx5 draft (T-12050) was archived and is not in the tree, and T-24702 lists only an m2c draft with 91 unknown-type tokens. The drafts named in the brief (T-25055, T-25024, T-25012, T-24905, T-24503) only call the function. The body was written fresh from the asm, using the views of the matched siblings in the same file (`MenuSelection5` at +0x1090, `GsRecF90 (*)[2]` at +0xF90, as in `menu_check_1` and `get_near_menu`).

## What the function does

`menu` is 0 or 1. Bit 0x100 of the pad word (`unk_F88`) bumps the mode byte `unk_1092` and clears the cursor. Otherwise `switch (unk_1092 & 3)`:
- 0: `func_8006BA40()`, then the first entry with `unk_06` set becomes the cursor.
- 1: unless `unk_F80 & 0xF`, the d-pad steps the cursor (0x1000/0x8000 back, 0x2000/0x4000 forward). The cursor wraps and skips disabled entries (`unk_07 == 1`, at most 16 steps), then sets the pointer position `D_8011ECF6/FA`.
- 2: if `unk_F80 & 4`, the same step (case order forward then back), or the saved choice `unk_1095[menu]` when `unk_F88 & 4`. Otherwise it calls `func_8006BA40()` and the last entry with `unk_06` becomes the cursor.
- 3: `get_near_menu` per direction (source order up, down, right, left), then out-of-range repair through `func_80050324`.
Every path except the 0x100 one ends in the pointer clamps (x to [-150, 140], y to [-100, 100]).

## Steps (side-by-side rows of `build/fc10/sbs.py`; funcdiff counts are about twice as large)

1. First typed draft: 73 rows (funcdiff 140). Structure, loop shapes, the `g` copy in t2 with its 0x4C spill, the case-1 constant in t4 and both spills were already right. Left: a1/a2/a3 swapped (pad word, cursor base, step counter, `menu << 3`), `sltu` for the wrap tests, `bnez` in place of the target's `beqz; b join` in case 1, load order of `unk_00`/`unk_04`, and the frame (0x58 against 0x50).
2. Three changes together gave 10 rows, all frame:
   - `CUR >= (s32)COUNT`: IDO runs in -cckr, so the u8 count promotes to unsigned and the s8 cursor test became `sltu`. The target has `slt`.
   - `rec.unk_00 + rec.unk_04 - 4`: the target loads `unk_04` first.
   - case 1 as `if (g->unk_F80 & 0xF) { } else { switch ... }`: the target jumps to the wrap tests from the empty arm (`beqz t6, switch; ...; b wrap; addu a2,t5,s0`).
3. One counter `s32 i` for the scan loops and the `get_near_menu` result (no separate `near`): the frame is 0x50, the t2 home is 0x4C and the a2 spill is 0x30. 0 rows.

Removal controls on the final body: the old sum order gives 6 rows, no `(s32)` gives 5, the inverted `if (!(...))` gives 52, and a separate `near` local gives 10 (frame only).

## Notes for siblings

- `g = &D_800E6280` as a local gives the target's mix: fixed fields absolute, in-loop bases through t5, loop latches through the `g` copy in t2. Spelling with direct globals was not needed.
- The u8 count needs `(s32)` wherever the target compares signed (`slt`). The u32 scan (`sltu`) comes from the same promotion with an `s32 i`.
