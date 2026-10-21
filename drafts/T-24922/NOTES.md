# T-24922 GEKO:func_801367EC

Best body: `drafts/T-24922/GEKO/func_801367EC/body.c` (GameState fields:
`D_800E6280.unk_0F4.h`, `.unk_71E`, `.unk_F5F`). Full diff: `funcdiff.txt`.

State: **44 diff rows, no register rows.** Restored `INCLUDE_ASM` (guards are only
kept under ~30 rows).

## What the diff is

Two as1 global-filler picks; every row is placement or a branch offset.

1. Head, 8 rows. The `bnez t6` (`D_80145088`) delay slot:
   - original: `lui t8, %hi(D_800E6374)` (switch selector block, weight 4),
     then `lbu t7 D_80145090`, `nop`, `bnez t7` with `lui a0, %hi(D_800CA19C)`
     in its slot;
   - built: `lui t1, %hi(D_80145074)` (first instruction of the outer `else`
     block) lands in the `bnez t6` slot, so the selector `lui t8` lands in the
     `bnez t7` slot and the simple block keeps its `lui a0` before the branch.
2. Switch tail, 36 rows. The selector `lhu t8` load delay and the range-check
   `beqz` slot:
   - original: `lhu t8; nop; srl t9; sltiu; beqz at, default` with `sll t9` in
     the branch slot; the default arm keeps its own
     `lui a0 %hi(D_800CA19C)` + `lui a1; addiu a1`;
   - built: as1 takes the default block's `lui a0` into the `lhu` delay slot and
     its `addiu a0` into the `beqz` slot, which moves the default block 8 bytes
     earlier and shifts the join by 8 bytes: all 11 `b .L80136A04` targets and
     the `b .L80136B14` targets differ by exactly two words.

The pre-flight "defect 3" (`D_800CA134`/`D_800CA138` types, final
`func_80085AC0`) is not in this function: the asm tail is `func_8004284C` and
both globals are already `u8 *` in `include/main_api.h`. Ignore it.

## Tried

- baseline migrated draft: 44 rows.
- selector computed before `if (D_80145090 == 0)` (the `func_80136C20` sibling
  idiom, `girl = ...` before the inner test): uopt sinks the load back, built
  stream byte-identical, 44 rows.
- `default:` arm moved first in the switch: 64 rows (layout and scheduling
  change), reverted.
- `default:` last (the natural form) is the 44-row body above.

## What is left / next step

Both picks are block-number/weight decisions, not source-line order; the
sibling `func_80136C20` has the same head pick and is still the 4-row
T-24203/T-24904 near miss. Next run should trace this exact build with the
T-24900 as1 trace (`drafts/T-24900-as1/`) and print the block numbers/weights
of the entry, outer-else, selector and default blocks, then look for a lever
that makes the entry block prefer the selector block, and one that makes the
default block's `lui`/`addiu` not movable. A plain `volatile` on `D_800CA19C`
is not usable: the simple block's `lui a0` in the original *is* moved (into the
`bnez t7` slot), so pinning every use changes the head the other way.
