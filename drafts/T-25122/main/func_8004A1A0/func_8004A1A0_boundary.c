/* T-25122 boundary draft of main func_8004A1A0: every uopt colour matches the original, but 159/157 words
 * (82 +/- lines): after the boundary uopt emits no `unal` for map, so as1 gets no .noalias(a0,sp)/(a0,v0)
 * and keeps two slot reloads and the header order. See NOTES.md. */
void func_8004A1A0(s32 *tex, s32 slot, s32 count, s32 w, s32 h, s32 clut, s32 x, s32 y, s32 flag, s32 tp) {
    SpriteSlot24 *bg;
    SpriteMap10 *map;
    s32 row;
    s32 col;
    u8 ch;

    ch = 0x78;
    bg = &((SpriteSlot24 *)&D_801220D0)[slot];
    map = &D_80122120[slot];
    bg->unk_00 = 0x1000000;
    bg->unk_04 = x;
    bg->unk_06 = y;
    bg->unk_08 = w << 7;
    bg->unk_0A = h * ch;
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
    /* FAKE: T-25122, uopt block boundary (folded after uopt); puts 0x1000 in $v1, map in $a0. */
    do {
    } while ((u32)slot < 0);
    bg->unk_20 = 0;
    map->cellw = 0x80;
    map->cellh = ch;
    map->ncellw = w;
    map->base = D_80122140[slot];
    map->index = D_80122540[slot].v;
    map->ncellh = h;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            load_tpage_buf_lock(tp + col + row * 32, tex[row * w + col], 1);
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
            if (row * w + col <= 0x40) {
                continue;
            }
            printf("BG_ERROR:p_line.c \n");
        }
    }
    D_800E6280.unk_10EC[slot] = flag;
}
