/* T-25006 draft, 76 rows (funcdiff --resolve; frame 0x70 and the spill homes of row, cells and index match).
 * Needs the BG slot types of include/ovl/OPTION.h (SpriteCell8, SpriteMap10, SpriteIndex80 and the three
 * externs) moved to a shared main header before a promotion.
 * Found: `u8 ch = 0x78` gives the original `li t0,0x78; multu s4,t0` shared with the cellh store (a literal
 * strength-reduces to shifts; `h * (map->cellh = 0x78)` adds an andi); no cells/index locals (they become the
 * spilled loop-invariant bases at sp+0x58/0x54); unk_14 as `(u32)D_80122120 + slot * 0x10` keeps the second
 * address add the original has.
 * Left: the header schedule and constant registers (the original keeps 0x80 in $a2, 0x1000 in $v1 and map
 * in $a0; here map takes $a2 and the constants are ugen temporaries), w and the row*0x78 counter swap
 * $s5/$s6, the tp counter and the index pointer swap $s3/$s4. */
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
    bg->unk_14 = (u32)D_80122120 + slot * 0x10;
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
