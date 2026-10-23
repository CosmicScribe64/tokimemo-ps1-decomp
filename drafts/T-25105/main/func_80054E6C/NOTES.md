# T-25105: main func_80054E6C (src/main/80053650.c)

Result: MATCH, 340 bytes (86 words), literal default C. Clean rebuild 27/27.

## Start state

NON_MATCHING guard from T-24123/T-24703/T-24711: 22 resolved diff lines (about 19 rows), all in the
unk_CC8 copy. The guard wrote that copy as a 4-trip loop with four hand-written byte stores per trip,
and stored the club value through the reused counter (`i = h >> 12; club = i`). The first divergence
was the loop head order and the byte loads: the target loads byte 0 last (1, 2, 3, 0 into t0, t1, t2, t9)
and puts its store in the delay slot.

## What closed it

1. The load order 1, 2, 3, 0 is IDO's own unroll-by-four shape (copy 0 last when the body is on its
   own line; see decompile-workflow, "Loops: unrolled or not"). The original is a 16-trip byte loop:
   `for (i = 0; i < 16; i++) dst[i] = src[i];`. Written that way it first stayed rolled, because the
   counter was assigned again after the loop (the club store). Storing the club value directly lets
   IDO unroll it: MATCH under the old guard's other FAKE edits.
2. Removing the old FAKE edits one at a time (each measured with full-TU funcdiff):
   - `if (i) {}` (T-24123): not needed.
   - struct-cast bases `((Work800E7D10 *)((u8 *)&D_800E7D10 + slot))->year[0]`: the plain fields
     `D_800E7D10.year[slot]` etc. match once all of them are plain (mixing gave 130 rows).
   - nickname `(slot << 4)` shift (T-24711): the plain `nickname[slot]` is CSE'd with the loop's flat
     `slot * 16 + i` and spilled (53 rows). The loop indexed as a 16x16 row array, `unk_CC8[slot][i]`,
     keeps the two apart and matches. The header now declares `u8 unk_CC8[16][16]` plus
     `u8 unk_DC8[0x100]`, the same split SaveImageView has (unk_CD8 / unk_DD8). The NAME_ENT users
     (`w->unk_CC8[w->selected_slot * 16 + i]`, four reads) become `[w->selected_slot][i]` and still
     build to the same bytes; func_800552DC uses unk_DC8.
   - slot byte view `*(u8 *)&D_800E7D36` (T-24703): the plain global load gives 97 rows (raw byte not in
     s0, the slot and slot * 8 bases not saved across the memcpy calls). Any indirect load works.
     The natural one is the work-pointer idiom of the matched NAME_ENT code:
     `Work800E7D10 *w = &D_800E7D10; GameState *g = &D_800E6280;` with every access through w and g.
     Pointer only for the slot read: matches with pad[15], but the mixed style is unlikely. All
     accesses through w only: 16 rows (loop-head order). w and g for everything: MATCH with pad[14].
3. Frame: the two pointer locals take 8 of the 64 extra frame bytes; 56 bytes remain unexplained and
   stay as a FAKE `s32 pad[14]`. Declaration order of pad, w, g, i, slot does not change the bytes.
   `slot = w->selected_slot & 0xF` in one statement and the two-statement form both match.

## Negatives added

- Plain `D_800E7D36` (with & 0xF, % 16, two statements), `D_800E7D10.selected_slot`, `(&D_800E7D10)->`:
  97-136 rows.
- Flat loop index `slot * 16 + i`, `i + slot * 16`, `16 * slot + i` with a plain nickname: 39-53 rows.
- Work pointer for the slot read only, no pad: 12 rows (frame only).

## Left

FAKE `pad[14]`: 56 bytes of the original 0x80 frame (spills at 0x30/0x34, saves at 0x18/0x1C). No
reference to them survives in the code, so the real local is unknown. The metric copy keeps the
documented word view: the target copies each 4-byte record with lw/sw; the canonical record types are
halfword aligned.
