# T-25318: main func_8007132C (second pass): matched

Matched (3992 bytes, default C in `src/main/8006CB30.c`, commits `T-25318: match main:func_8007132C` and the
FAKE-comment follow-up). No draft body is kept here: the source file has the final body.

## Start state

First pass T-25125: best 738 rows (`drafts/T-25125/main/func_8007132C/best-v4.c`, byte views on every index access).
Its v17 walk form (record pointer `p = (GameState *)((u8 *)&D_800E6280 + idx * 4)`, `op`/`t` locals) had the
target's structure but 996 rows: the 0x800E6280 base was coloured (`$a3`). Its next hypothesis was a second
aggregate or symbol for the schedule at 0x800E691C.

## What closed it (rows by funcdiff --resolve --version c)

1. Base priority. In the traced uopt (`R9SAVE` of the base constant, `bbmap.py`), every read through a
   copy-propagated `p` counts as one more use of the base in its block. Removing the `op`/`t` reassignments inside
   the 0x25 and 0x32 blocks and writing the tail tests (0x33 and 4, 0xFA) as direct reads took the split piece from
   4/29 to 0/29; the base is then unallocated (935 rows, W5). Direct tail reads also keep the target's forwarded
   read after each increment (PRE inserts it), 698 rows (W9).
2. No record pointer and no opcode local in the walk at all (W15): every walk instruction then matches up to
   colours. A `GsRec69C *p` instead of the GameState-typed pointer outside the walk: 635. No `op` at the 0xFB
   check (op assigned after the join, before the switch): 594.
3. `D_800E6280.unk_54C_0.active` and `.unk_25` bit-field reads instead of `.w & 0x20000000` and `(w << 3) >> 28`:
   these fix the ugen temporaries of the appointment block and everything after it, 306.
4. The 0xFF branch's hour value in its own local (as `op` with `6U` compares to keep four locals): 292.
5. No `p` at all (all reads direct): the whole function up to word 561 is exact, 459.
6. `f.b17 = 1` instead of `b[2] |= 2` in the 0xFB loop (the same slot effect as T-25125's `f.b16 = 1`): 171,
   only case 0x45 left.
7. Case 0x45 second field: the target reloads the index, the draft reused the record address in `$a1`. An `(s16)`
   or `(s32)` cast of the index (FAKE) gives the reload: 20 rows; `(u16)`, `(u32)`, `(u8)` do not.
8. `unk_725 != unk_724` operand order (ugen temporaries of the two loads): 16 rows.
9. A separate `u32 hour` local again (now the fourth local): the v1 spill slot moves from 0x34 to 0x30, MATCH.

## FAKEs left (each marked in the source)

- T-25052 calendar byte views `(*(u8 *)&D_800E6280.unk_03E)` etc.: plain fields give 901 rows.
- T-25052 late-bound bounds `11 + ((u32)i < 0)` in the three counter loops: plain `for` loops give 92 rows.
- T-25318 `(s32)` index cast in case 0x45.
- The `unk_0F4.s.hi` GsHalf view of T-25125 is not needed: `unk_0F4.b[1]` matches.

## Negatives (second pass)

- Spellings of the schedule base (`(GsRec69C *)&D.unk_69C`, `(u8 *)&D + 0x69C`, a base at `unk_69C[0].unk_02`):
  uopt canonicalises them to the same address constant, no change.
- `++D.unk_71C` inside the pointer expression, `i = 0` hoisted before the switch, byte views only in the pointer
  definitions (alone or with the direct walk): no gain or worse.
- Indirect or volatile store to D_80122CD4 in case 0x45: does not make uopt reload the index.
