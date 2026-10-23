# T-25119: main func_80061A3C (1216 bytes, 304 words)

Result: matched as default C in `src/main/80061710.c` (commit "T-25119: match main:func_80061A3C"). `funcdiff --resolve --version default` prints MATCH; the file has no `INCLUDE_ASM` left.

## Start state

Plain `INCLUDE_ASM`. Earlier work: only m2c drafts (T-17000 typed draft, score 264; T-9570 list 7, 243). T-17010 did not start it. The m2c draft had wrong constants (`&D_80184000` for the first texture page, which is 0x80180000), a word view of `D_800E6280 + 0x1098`, and pointer walks.

## Steps (rows are `funcdiff --resolve --version c`, whole unit)

1. Hand-written C from the asm: a `switch` on `unk_110D` (compare chain), case loops over one shared `s32 i`, `tbl[8]` of texture addresses, `((DisplayActor44 *)D_8011ECD0)[96]` field stores, a 13-element copy loop. 60 rows.
2. Copy loop: the original has the first element as a separate store (direct `%lo` addresses) and a 12-trip loop unrolled by four, whose source pointer starts on element 1 (offsets 0..6) and whose destination pointer starts 4 bytes before the element (offsets +4..+16). That is `(&D_801230D0)[i + 1] = D_800B5A40[i]` for i = 1..12, the same work-area spelling func_80062948 uses for its 13 positions. 42 rows, then 38 with the frame fix.
3. Frame: 0x88 with `tbl` at sp+0x60 needs two scalars above the array (`i`, `p`) and two unused words below it: `s32 pad[2];` after `tbl` (FAKE). Without it: 8 rows (frame 0x80, tbl at 0x58).
4. Copy-loop colours (34 rows): the original has source v0, destination v1, end a0; the draft had v1, a0, v0. regdiff: the source pointer range is allocated first and skips v0. Control: making `func_8006190C` void (experiment only) gave exactly the original colours, so the int call's return register is reserved in the block after the call. The C cannot change that type (`func_8006190C` returns 0 on its early exit, and cfe rejects a void view of a function defined in the same file). `if (func_8006190C()) { }` (FAKE) ends the block after the call; uopt drops the empty test and the pointers take v0/v1: 38 to 4 rows.
5. Case 1 argument order (4 rows): the original sets `li a0,1` before `lui a1,0x801f` for `cd_read_sectors(1, 0x801F0000, 0xBE13)`. A minimal repro (case 1 alone) has the original order; adding case 2's `memcpy(p, 0x801F0000, 0x800)` reverses it, so one shared integer constant 0x801F0000 is the cause. `(void *)0x801F0000` (also `0x801F0000U` or `(u8 *)`) for the memcpy source closes it: MATCH. `p` is a `u8 *` walker.

## Negatives

- Natural 13-trip copy loop (`for (i = 0; i < 13; i++)`): IDO's remainder iteration goes through the pointers (`sll t7,zero,1; addu v0,t7,t8`), 52 rows. A separate counter for it (`j`, or reusing `p`) makes uopt unroll the 8-entry table loop too (86 rows): the loops must share `i`.
- `(&D_801230D4)[i]` for i = 1..12, or `[i + 1]` on both sides for i = 0..11: wrong pointer offsets (38 rows).
- Block boundary attempts that do not change the colours: `while` loop, `for (; i < 13; i++)` with `i = 1` before the peel, `i != 13`, `i = func_8006190C();`, `p = func_8006190C();` (all 38).
- Pads above `tbl` instead of below: frame right, tbl at 0x58.

## Removal controls on the final body

`if (func_8006190C()) {}` removed: 34 rows (copy-loop colours). `(void *)` cast removed: 4 rows (case 1 order). `pad[2]` removed: 8 rows.
