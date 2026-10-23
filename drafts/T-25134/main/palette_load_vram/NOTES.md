# T-25134: main palette_load_vram (2,024 bytes): matched

Start: `INCLUDE_ASM`. The best body was the T-24501 draft (`drafts/T-24501/main/palette_load_vram.c`, edit cost 419, frame 0x50). It walked the records with a separate `GameState *g` stepped by 8 bytes, plus a `base` local for the loop test.

Measured with a scratch harness: the `permute.py setup` base, `tools/cc.py`, and an objdump row diff with and without register names. Then confirmed with `funcdiff --resolve`.

| step | change | rows (exact / without register names) |
|---|---|---|
| T-24501 body | none | 554 / 32 |
| 1 | drop `g`, index the records `base->unk_1328[index]` as matched `csr_load_vram` does (uopt makes the stepped pointer `$t1`; `base` stays a memory local stored from `$t6`) | 22 / 19 |
| 2 | compute `y = (unk_04 & 31) + 0x1E0` before `first = unk_07 << 7` (ugen order: t8, t9 for y, then t7 for the shift) | 18 / 18 (only stack offsets) |
| 3 | two more locals after `j` (frame 0x70 -> 0x78; homes base 0x74, x 0x72, y 0x70, width 0x6E, index 0x68; the `$t1` save stays at 0x30) | 0 (funcdiff MATCH, 2,024 bytes) |

Final form: a second counter `k` for the second loops of cases 2 and 4, and `u16 height = 1` for the `LoadSquare` height. Both are marked FAKE because the real locals are unknown. `GsRec1328.unk_00` is now `u32 *` (its only C user is this function). The two forms in step 3 and the pointer field type compile to the same bytes.

Why there are no saved registers: uopt charges `clamp(0.25 * blocks, 4, 60)` for the first use of a callee-saved register. With this many blocks, saving around the one call (index in `$ra` at 0x68, `$t1` at 0x30) is cheaper. The draft already did this. The frame and the two orders were what was left.

Negatives (measured once):
- Dropping `default:` from the empty `case 1` gives 500 words (422 rows). Keep `case 1: default: break;` between cases 0 and 2.
- The `D_800E6280.unk_1428` and `base->unk_1428` entry tests compile the same. So do `first + 0` and `first`, and `(u8)` on the loop test.
