# T-25108: main func_8006509C (1076 B), matched

Start: `INCLUDE_ASM`. Best earlier body `drafts/T-24301/main/func_8006509C.c` (`GameState *g`, u8 views, the
15-pair copy as an index loop). Full-unit resolved score 99 (funcdiff `diff_score`, 99 diff lines).
T-24501 measured 95 by moving the second loop's zero.

The match needed no FAKE. Plain C: `for` loops through `g`, and one counter reuse in case 2.

## Steps (scratch harness, whole-unit compile, score = funcdiff diff_score)

| change | insns | score |
|---|---|---|
| T-24301 draft, as is | 268/269 | 99 |
| `i = 0;` at the join between the two loops, not after loop 1 and inside if 2 (uopt PRE puts it on the loop-1 exit edge, as in the target) | 268 | 93 |
| first-half record stores: `unk_14 = D_800C9A54` right after `unk_34`, as in the second half (the target loads D_800C9A54 there) | 268 | 89 |
| case 2 scan as an index loop `for (j = 0; j < 15; j++)` on its own counter, copy still on `i` | 269 | 56 |
| case 2 copy on the count variable: `for (n = 0; n < 15; n++)` | 269 | **0** |

After the match, the cleanup went one step at a time, and each step kept score 0:
- dropped the unused `s16 *p`;
- every F6C/F6D read and the 0xF store go through `g` (no `*(u8 *)&` views, no `(u8)` switch cast);
- the four `i = 0; if (F6D) do ... while` loops become `for (i = 0; i < g->unk_F6D; i++)`;
- `s32 i` in place of `u32 i`.

## Why the last step closed 56 rows

The registers of the strength-reduced copy pointers are decided across the cases. With the copy on `i`,
its expressions `D_800B5AE4[i][k]` and `((s16 *)D_80125C10)[i * 2]` are the same uopt bits as in
case 0 (and the destination as in case 1). One range then spans several cases, so it takes one colour
everywhere: the target has dst v0 / src v1 in cases 0 and 1 but src v0 / dst v1 in the case 2 copy. The
same sharing gave n its high priority and the scan end constant `a0` in place of `v1`. With the copy on `n`,
every case has its own ranges and colours like the target.

## Negatives (measured)

- With the u8 views kept: absolute `D_800E6280.unk_F6C` in the first test (209), absolute F6D in any one
  entry test (123 to 193). The views were standing in for reads through `g`: with every access
  through `g`, the views are not needed.
- Removing both `i = 0` resets (a bare `do` loop 2): 238.
- `unk_14` after `unk_04` 91, after `unk_05`/`unk_16` 93.
- Case 2 scan and copy on the same counter (`i` 79, `j` 39), scan on `i` with copy on `j` 16.
- `(s16 (*)[2])` destination spelling in cases 0/1 or in the copy: 68. Struct copy of a 4-byte pair
  in cases 0/1: 172; in the copy: 244.

## Other change

`include/main_api.h`: K&R declaration `void func_800654D0();`. The definition follows func_8006509C in the
file, and without a declaration the implicit-int call clashes with the `void` definition. The prototype
`void func_800654D0(s32 n);` was tried first and broke the OPTION.bin checksum (OPTION calls it as
`func_800654D0(g->unk_F6D = 0xE)`), so the declaration has no parameter list.
