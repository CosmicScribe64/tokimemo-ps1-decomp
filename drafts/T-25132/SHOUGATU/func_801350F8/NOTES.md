# T-25132: SHOUGATU func_801350F8 (588 words, 2308 bytes)

Omikuji scene step handler (switch on the scene step `D_800E6280.unk_1104.w`). `body.c` here is the body
that is in the `NON_MATCHING` guard of `src/ovl/SHOUGATU/80134890.c`.

## Start state

INCLUDE_ASM. Best earlier body: `drafts/T-24029/SHOUGATU/func_801350F8/body.c`, 656 aligned edits
(`side.py` score) / 672 funcdiff rows in the current tree; it also needed a `void func_801359FC(void);`
prototype (now in `include/ovl/SHOUGATU.h`).

## What closed (656 -> 2), in order

1. Control flow and return values (656 -> 601 -> 521). Cases 1, 7, 9 are `break`, the button tail is
   after the switch, `default: func_8004284C(); break;`; cases return `0`, the two button exits
   return `1`, the end of the function falls off (K&R int). Case 4 must be `if (...) { ... } else
   { ... } return 0;`: a `return 0` in the same uopt block as the last use of the `&D_8011ECD0` base
   makes the base range interfere with `$v0` and the whole base range goes to `$v1` (b1 vs b4/b5).
   IDO's jump-to-jump retargeting then gives the action path its own `b end; move v0,zero`.
2. One declared scalar (521 -> 511). The original frame holds one 4-byte scalar above the strings
   (`sp84` at 0x84); without it every array sits 4 bytes high.
3. Case 6 order (-> 509): `D_800E6280.unk_1104.w++;` before `D_80122CE4 = 1;` in the then-block
   (as1 keeps the store order; the step increment fills the `lbu` delay).
4. Case 5 timer byte (-> 503). Two causes:
   - `unk_110D` read as a promoted global gave a second register: uopt CSEs the widened copy
     `cvt(G)` used by `== 0` and `== 0x1E` into `$v1` while the promoted byte sits in `$a0`.
     Reading it through `GameState *g = &D_800E6280` (ILOD/ISTR, as in DATE `func_8013AF1C`)
     gives one register `$v1`, as in the original.
   - The `D_80120696`, `D_801206DA`, `D_8012067A` stores/loads must be the same symbol
     (`D_8011ECD0` records 97/98/96): with separate symbols as1 hoists the `lh` above the stores and
     the `xori`/`addiu` land in other slots.
5. All record fields of `D_8011ECD0` records 96..99 (`D_80120652` .. `D_80120734`) written through
   `((DisplayActor44 *)D_8011ECD0)[n].unk_xx` (-> 501, then with `g` replacing the `step` local -> 487
   and only case 8 left).
6. Case 8 index `sp38.s[(s32)D_80143DD8]` (487 -> 2). With a `u8` index cfe multiplies the raw byte
   (`mpy L`), so uopt promotes `D_80143DD8` and keeps the widened copy for the tests in a second
   register (`t0` + `move v1,t0`), which also moved `&sp38` to `t1` and shifted every prologue
   temporary. With the `(s32)` index every use reads one CSE of `cvt(lod G)`: `$v1`, and `&sp38`
   is `t0` again.

## Rows now: 2 (one swapped pair)

Case 8 head: original `ori t7,t4,0x80; lui v1,%hi(D_80143DD8); sb t7,%lo(D_80120653)(at); lbu v1`,
draft `ori; sb t7; lui v1; lbu v1`. Same instructions and registers. ugen emits the D_80143DD8 load
with statement 3 (`unk_18 = sel + 0xD`); as1's local list scheduler takes the `sb` first (it is
earlier in the ugen order; the two seem to tie).

## Negatives (measured, current tree)

- Case 8: any separate-symbol/array spelling of the four head stores (16 combinations: no change);
  `D_80120650` base for the first store; head statements on one line (4 groupings); statement order
  (3 orders, 8-14 rows); `(s32)`/`0xD +`/`13U` on the `+ 0xD`; `(s32)` on the tests; `(u32)`/`(s16)`
  index (worse); one raw-u8 index left (483); a `u8`/`s32` `sel` local (frame/prologue change, 437+);
  an empty `if (D_80143DD8) {}` after the calls (no change).
- Case 5: `D_800E738D` as its own byte symbol (no change); `if (G == 0x1E) ... G++` (511);
  `s32`/`u8` local copies of the timer (505-515); a flag local for `G++ == 0x1E` (524).
- `return;` without value everywhere (561) and an explicit final `return 0` (610).

## Next hypothesis

Make ugen emit the `D_80143DD8` load at the case 8 head (as a promoted-global `rlod` would) while
keeping one register, or check whether the original defined `D_80143DD8` in this file: the object's
data range is `80143DB0..80143EB0` (`config/objects/SHOUGATU.txt`, `data 80134890 ... owner`), so a
data island (`tools/data_island.py SHOUGATU/80134890`, then C definitions) may change as1's
treatment of the load; untested.
