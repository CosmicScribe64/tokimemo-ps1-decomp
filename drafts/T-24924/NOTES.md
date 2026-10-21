# T-24924: ENDING:func_801330E4 (796 B) - closer notes

Status: not matched. Best body: `drafts/T-24924/ENDING/func_801330E4/body.c`
(92 diff lines, down from the inherited 114). `funcdiff --resolve` diff archived in
`drafts/T-24924/ENDING/func_801330E4/funcdiff.txt`. `src/ovl/ENDING/80133030.c` is back
to `INCLUDE_ASM`.

## Root cause 1: stack frame — FIXED

Target `asm/ovl/ENDING/nonmatchings/ENDING/80133030/func_801330E4.s`, read in full:

| item | target |
|---|---|
| frame | `addiu sp,sp,-0xB0` (176) |
| `$ra` home | `0x14` |
| `chance` (`EdShorts18`, 0x24 = 36 B) | `0x30..0x54` |
| block copy of `chance = D_8013C3B0` | 3 iterations of 3 words, end pointer `t0 = t7 + 0x24`; **9 words, 36 bytes** (not 6, not 40: `EdShorts18` is 18 shorts) |
| `level` spill around `jal rand` | `sh/lh 0x54(sp)` |
| `points[16]` (32 B) | `0x58..0x78` |
| `rank[32]` (32 B) | `0x78..0x98` |
| free hole below `chance` | `0x18..0x30` (24 B, same hole the draft has) |
| area above `rank` | `0x98..0xB0` (24 B) |

The draft's declaration order (`rank, points, level, chance, best, i, n, p`) put `rank`
at `0x88`, `points` at `0x68`, `chance` at `0x40` and the frame at `0xA8`. Layout rule
measured by compiling variants (frame / `chance` offset):

| scalars declared **before** `rank` | frame | `chance` |
|---|---|---|
| 4 | `0xA8` (168) | `0x34` |
| 5 | `0xA8` | `0x30` |
| 6 | `0xB0` (176) | `0x34` |
| 7 | **`0xB0`** | **`0x30`** |

So: locals are placed **top-down in declaration order** (first declared = highest
address), arrays sit at the top of the local area unless scalars are declared before
them, the 24-byte hole at `0x18..0x30` is fixed, and the frame steps +8 for every two
extra leading scalar slots. Seven leading scalars reproduce the target exactly. The
declarations that do it (best body):

```
s32 pad3; s32 pad2; s32 pad1;      /* FAKE (T-24924): the target has 3 more slots above rank than the 4 known scalars */
s16 best; s16 i; u32 n; s16 *p;
u8 rank[32]; s16 points[16]; s16 level; EdShorts18 chance;
```

`level` must stay between `points` and `chance` (`0x54`), `chance` last. With this the
body matches rows 0-83 byte for byte; every `$sp` offset and the frame agree. Rows
0-83 (prologue copy, the four `unk_03E..041` stores, `func_80053418`, the `points[0]`
test, the `best` loop) are clean.

## Root cause 2: level block (rows 84-106) — improved, not closed

Target rows 84-106:

```
lui t2,6838 / lbu t2          ; A = unk_56C[0x4C]
lui a0,66B0 / lw a0           ; f9 word
lui t8,683A / lbu t8          ; B = unk_56C[0x4E]
sltiu t3,t2,1 / xori t3,t3,1 / sll t4,t3,1     ; (A == 0 ^ 1) * 2
sll v0,a0,20 / srl t1,v0,29                    ; n = f9 in a ugen temporary
sltiu t0,t8,1 / xori t0,t0,1 / sll t7,t0,1     ; (B == 0 ^ 1) * 2
addu t9,t1,t4 / addu v1,t9,t7                  ; (n + A2) + B2 -> level in $v1
sll t6,v1,16 / sra v1,t6,16 / slti at,v1,18 / bnez at
or v0,t1,zero                                  ; <-- copy of n into $v0, in the bnez delay slot
li v1,17 / beqz v0                             ; n != 0 test
```

The target keeps the f9 extraction as a **ugen ring temporary** (`t1`) for the sum and
copy-assigns it to `n` in `$v0` *after* the sum, so the copy lands in the `bnez` delay
slot. Every C spelling tried made uopt coalesce the extraction with `n` (the sum then
uses `$v0`/`$v1` directly), and when ugen did emit the `move` (`cc -S` listing) as1
folded it into `beqz` and filled the slot with the next `lui` instead.

Variants measured (all with the fixed declarations; rows):

1. draft `n = f9; level = n + A2 + B2;` -> 100
2. `level = A2 + f9 + B2; n = f9;` -> 92
3. `level = A2 + (n = f9) + B2;` -> 98
4. `level = A2 + f9 + B2;` clamp; `n = f9;` after the clamp -> 98 (ugen emits `move v0,v1`, as1 removes it)
5. `level = f9 + A2 + B2;` clamp; `n = f9;` -> 98
6. **no `n` variable**, `if (f9 != 0 && unk_56C[0x4D] == 0)`, `level = f9 + A2 + B2` -> 92
7. **best archived**: as 6 with `level = A2 + f9 + B2` -> 92 (closest shape: `level` in `$v1`,
   `beqz v0`, `sh v1` spill; only the copy and the A/B temporary numbers differ)

`regdiff` could not run: the traced uopt is not built (`TOKIMEMO_SCRATCH` not mounted,
`/scratch/r9-uopt/variants/trace` missing).

Remaining diff lines by block (92 total, `funcdiff.txt`):
- rows 84-106: the level block above (A/B temp numbers `t2`/`t8` vs `t9`/`t2`, the missing
  `or v0,t1,zero`, sum operand order, load order A,n,B vs A,B,n).
- rows 113-114: `sh/lh` of `level` at `0x56` instead of `0x54` (our s16 home is 2-byte
  aligned; target writes the 4-byte-aligned slot).
- rows 116-127: chance index chain (`sll/addu/lh`) one to three ugen temporaries off,
  follows from the level temp.
- rows 156-161, 182-194: constant/address temporary numbers (`t7/t8/t9`, `t0/t1`) in the
  tail call sequence; no structural difference.

## Next hypothesis

1. Identify the 3 real leading scalars. Candidates worth trying in the 3 pad slots (in this
   order, one at a time, checking that frame/offsets stay): a `u32 f9val`/`s32 temp` holding
   the extraction (then `n = temp`), a second loop/local counter, an `s16 max`/`s16 idx`.
   A pointer local (`s16 *q`) and a `GameState *g` are also cheap to test; the pads are only
   standing in for slots, not for code.
2. Get the copy to survive: the sum's f9 read must stay a ring temporary while the `n`
   assignment is a separate range. Try spelling the second read differently from the sum's
   (e.g. sum reads `.f.f9`, the `n` read uses the word view `(unk_0C.w << 20) >> 29`, or a
   byte/halfword view of the same field) so uopt cannot coalesce them; also try `s32 n` and
   moving the assignment after the clamp (variant 4) with the different spelling.
3. The `0x54` vs `0x56` spill should be re-checked only after the level register is `$v1`
   with the copy present; if it survives, look for a declared 4-byte local in the original
   whose slot is `0x54` (it may be one of the 3 unknown scalars, declared between `chance`
   and `points`).
