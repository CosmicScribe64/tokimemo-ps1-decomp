/* T-25010 draft of main func_8004A1A0 (628 B), from the T-25006 draft (76 rows). The BG slot types and the
 * three tables are now in include/main_api.h (moved from OPTION.h), so this compiles in src/main/80049FF0.c.
 * Found: `bg->unk_14 = (u32)(D_80122120 + slot);` gives the original's second `sll a1,4` (slot * 16 computed
 * twice, not CSE'd with map) and the original instruction count (157 = 157; the T-25006 form is one short).
 * Left (all uopt colouring): the original keeps 0x1000 in $v1, map in $a0 (tex moved to $s3 first) and 0x80
 * in $a2, 0x78 in $t0; here map takes $a2 and the constants other registers, and the header order follows.
 * In the loops w and row * 0x78 swap $s5/$s6, the tp counter and the index pointer swap $s3/$s4.
 * Tried without gain (T-25010): u8/s16 locals for 0x80 and 0x1000 (same code: the constants are already
 * ranges), chained `a = b = k` stores (reload of the field), map as `D_80122120[slot].` everywhere,
 * `map =` inside the unk_14 store, map/bg declaration order, map assigned late, `row * ch` for the v store,
 * `col + tp`, row/col order, `(slot << 4)`, `(u32)slot * 0x10`. autotweak 300 s (empty tests and line
 * swaps, exact 151 -> 121 lines, all FAKE) and permute 600 s (4863 -> 3992, formatting and a chained zero
 * store only) found nothing near a guard. */
void func_8004A1A0(s32 *tex, s32 slot, s32 count, s32 w, s32 h, s32 clut, s32 x, s32 y, s32 flag, s32 tp) {
    SpriteSlot24 *bg;
    SpriteMap10 *map;
    s32 row;
    s32 col;
    u8 ch;

    ch = 0x78;
    bg = &((SpriteSlot24 *)&D_801220D0)[slot];
    map = &D_80122120[slot];
    bg->unk_04 = x;
    bg->unk_00 = 0x1000000;
    bg->unk_0A = h * ch;
    bg->unk_08 = w << 7;
    bg->unk_14 = (u32)(D_80122120 + slot);
    bg->unk_1C = 0x1000;
    bg->unk_1E = 0x1000;
    bg->unk_0C = 0;
    bg->unk_0E = 0;
    bg->unk_10 = 0x80;
    bg->unk_11 = 0x80;
    bg->unk_12 = 0x80;
    bg->unk_18 = 0;
    bg->unk_1A = 0;
    bg->unk_20 = 0;
    map->cellw = 0x80;
    map->cellh = ch;
    map->ncellw = w;
    map->base = D_80122140[slot];
    map->index = D_80122540[slot].v;
    map->ncellh = h;
    bg->unk_06 = y;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            load_tpage_buf_lock(tp + row * 32 + col, tex[row * w + col], 1);
        }
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            D_80122540[slot].v[row * w + col] = row * w + col;
            D_80122140[slot][row * w + col].u = 0;
            D_80122140[slot][row * w + col].v = row * 0x78;
            D_80122140[slot][row * w + col].tpage = tp + col;
            D_80122140[slot][row * w + col].cba = func_8009ED74(0, clut);
            D_80122140[slot][row * w + col].flag = 0;
            if (row * w + col > 0x40) {
                printf("BG_ERROR:p_line.c \n");
            }
        }
    }
    D_800E6280.unk_10EC[slot] = flag;
}
