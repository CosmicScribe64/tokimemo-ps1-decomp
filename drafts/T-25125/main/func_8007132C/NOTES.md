# T-25125: main func_8007132C (998 words + 75-word jump table)

Not matched. `src/main/8006CB30.c` keeps `INCLUDE_ASM` (no guard: every body is far above 30 rows).
Rows below are `funcdiff --resolve --version c` +/- lines on the whole TU.

## Start state

Best known body: T-25052 `late-bounds-unsigned-switch-ff` (regenerated from `drafts/T-25052/probe.py` into
`t25052-best.c`): 950 rows, 991/998 words. First divergence at word 121: uopt keeps the address constant
0x800E6280 in `$a3` for most of the function (regdiff: priority 0.17, 21 uses); the original never keeps it in
a uopt register (it is materialized by ugen with `lui/addiu` where needed and reused across fall-through branches).

## Files

- `t25052-best.c`: start body (950 rows).
- `v1.c`: two semantic fixes (937 rows): `u32 t` (the original tests `unk_0F4.h >> 12` with `sltiu`), and the
  rand condition has a redundant test: `b3 < 7 || (b3 >= 7 && unk_042 >= 0x2DB && b3 < 9)` (`sltiu at,v0,7; bnez`
  twice in the original).
- `best-v4.c`: v1 with every `unk_71C` access through `*(u8 *)&` (FAKE): 738 rows, lowest measured.
- `v4b.c`: best-v4 plus `unk_0C.f.b16 = 1` for `b[2] |= 1` (bit-field store spends the ugen slot the original
  spends, so `lbu t5 / ori t8` at word 161): the first 192 words match exactly, 748 rows.
- `v17.c`: the walk form the original has (see below), 996 rows: the base constant is allocated again.
- `lead-exp6.c`: v17 with the schedule reads after the prefix through an absolute address (FAKE): base unallocated,
  first 192 words exact, the walk has the original's structure, 825 rows.
- v17.c and lead-exp6.c need a struct view in `GsHalf` (include/main_api.h, reverted in the tree):
  `/* 0x00 */ struct { u8 lo; u8 hi; } s;` (used as `unk_0F4.s.hi & 0xF`).

## What was measured (traced uopt, `regdiff -v` plus R9SAVE/R9UNIT lines of draft.log)

Priority of a constant range in this IDO: `totalsave / (units + reaching blocks)`, with
`totalsave = sum(uses * freq) - sum(freq of units that need a register load)`; a reload in a loop-first block with
a predecessor in the range and no outside `ijp/xjp` predecessor is free. Checked on v17's piece: 29 uses (icon loop
header counts 10) - 25 reload freq (two 0xFF-block loop latches at 10 each, 5 single blocks) = 4, printed 4/29.
The whole range (34 blocks) is coloured only after splitting; the piece that gets `$a3` needs `totalsave > 0`.

Uses of the 0x800E6280 constant (each removal measured as -1 on totalsave):
`((ScheduleClubFlags *)&unk_54C[0])->b29` (use `unk_54C_0.active`, same code), `unk_0F4.b[1]` (an array in a
union is an ILOD; `unk_0F4.s.hi` is a direct load, same code), `unk_1BC[11].unk_0C.f.f9` (bit-field read; `.u << 20
>> 29` counts the same). Direct members (`unk_0F4.h`, `unk_56C[77]`, `unk_F68.b[0]`) do not count.

Walk (the opcode chain after the first loop), facts from the original:
- p is a variable in `$a1` holding `base + idx * 4` (no +0x69C), read at `0x69C/0x69D(a1)`: a GameState-typed
  record pointer, `p = (GameState *)((u8 *)&D_800E6280 + D_800E6280.unk_71C * 4)`, fields `p->unk_69C[0].unk_xx`.
- Case 0x45 reads `p->unk_01` through `$a1` but reloads the index for `unk_69C[idx].unk_02`: the switch-time p is
  not the same expression as the IXA reads, which rules out `&D_800E6280.unk_69C[idx]` forms (v13b/v14 reuse it).
- After `idx += 1` the original forwards the narrowed store (`addiu t1,t6,1; andi t2,t1,0xFF; sll t3,t2,2; sb t1`).
  With the record-base form this needs `D_800E6280.unk_71C++` (v17); `+= 1` makes `cvt(idx)` a uopt range
  (prio 2.9) that reloads after every store (v2). Byte-view stores give a pointer induction (`addiu a1,a1,4`).

## Negatives added

- Record-base forms `(s32 *)&D + idx`, `&((s32 *)&D)[idx]`, `idx * 4 + (u8 *)&D` with `+= 1`: same as v2.
- Byte views only on stores, or only inside p's expression: no change of the base priority.
- Plain `for (i = 0; i < 11; i++)` instead of the T-25052 late-bound loops: constant 11 takes `$a3` from word 26
  (1056 rows); its piece is +8 (three latches at 10, two reloads at 10). The late-bound FAKE is still needed.
- `i < 0xE` for the icon loop, `.f.rest` for `b[3]`: no change.
- `op = (p = E)->...` for the first walk assignment: identical code.

## First divergence left

With the base unallocated (lead-exp6.c, or best-v4/v4b), the first difference is word 193: the walk's registers.
The original: E = `idx*4 + base` in ugen `t8`, op in `$v1`, constant 0x25 in `$v0`, p moved to `$a1` in the
`bne` delay slot. The draft: p's expression copy-propagated into a uopt range (`$v1`), op `$v0`, 0x25 `$a0`.
Putting a byte view of the index inside p's expression (has_ilod) keeps p a variable in `$a1` but drops the `t8`
temporary. Measured on a throwaway control where the icon-loop compare read a wrong field (only to take its
10-weight use off the base constant): 820 rows, and 827 with the byte view in p's expression.

## Next hypothesis

The original reaches the schedule array (`unk_69C[idx]` IXA reads: walk op reads, the 0xFF test, the icon loop,
cases 0x44/0x45/0x48) through something that is not the 0x800E6280 address constant, while p, the 0x44-byte table
and the girl table use it. Candidates to test, in order: the schedule as its own aggregate at 0x800E691C (a second
symbol for that range, or a `GsRec69C *` global/pointer the original copy-propagates), then a different spelling
of the switch-time and walk reads. Measure each by the R9 piece totalsave of the 0x800E6280 constant (must be <= 0)
on top of v17.c, then fix the walk colours (op in v1, 0x25 in v0, p in a1 with E in t8).

## Helper scripts (drafts/T-25125/tools, run from the repo root in Docker)

- `sbs.py [OUT]`: compiles the guarded TU and writes an opcode-aligned side-by-side of func_8007132C against the
  target (`#` structure, `r` register rows) to `build/t25125/sbs.txt` (mkdir it first).
- `udump.py`: dumps the input ucode of the last `regdiff.py` trace to `build/t25125/ucode.txt`.
- `bbmap.py BIT [LR]`: after `regdiff.py -v` and `udump.py`, prints the R9SAVE/R9UNIT lines of a range with the
  source line of each uopt block (from the trace flow graph labels). The traced uopt was copied from
  `build/scratch/t25012/r9-uopt/variants/trace` of the main checkout into `$TOKIMEMO_SCRATCH/r9-uopt/variants/`.
