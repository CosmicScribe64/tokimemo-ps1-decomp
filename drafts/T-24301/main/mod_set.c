/* T-24301 draft, ~220 rows: the local is a GsSPRITE (Rec24 layout) at sp+0xB0 in a 0xE8 frame; `mode`
 * lives in its home slot sp+0x70 (reloaded before each search_tpage_multi call) and the six switch arms
 * stay separate in the original. */
void mod_set(s32 idx, s32 frame, s32 x0, s32 y0, s32 cx0, s32 cy, u16 pri) {
    u8 *p;
    s32 mode;
    Rec24 sp;
    u32 row;
    u32 col;
    u32 *data;
    u32 v;
    u32 uv;
    u32 clut;
    s32 x;
    s32 bright;

    bright = func_8004901C();
    p = D_801217A0 + idx * 0x14;
    mode = 1;
    switch (p[8]) {
    case 0:
        sp.unk_00 = 0x01000000;
        mode = 1;
        break;
    case 1:
        mode = 2;
        sp.unk_00 = 0;
        mode = 2;
        break;
    case 2:
        mode = 2;
        sp.unk_00 = 0;
        mode = 2;
        break;
    case 3:
        mode = 1;
        sp.unk_00 = 0x01000000;
        mode = 1;
        break;
    case 4:
        mode = 1;
        sp.unk_00 = 0x01000000;
        mode = 1;
        break;
    case 5:
        mode = 1;
        sp.unk_00 = 0x01000000;
        mode = 1;
        break;
    default:
        sp.unk_00 = 0x01000000;
        break;
    }
    sp.unk_18 = 0;
    sp.unk_1A = 0;
    sp.unk_14 = bright;
    sp.unk_15 = bright;
    sp.unk_16 = bright;
    sp.unk_20 = 0;
    sp.unk_1C = 0x1000;
    sp.unk_1E = 0x1000;
    sp.unk_08 = p[0xE];
    sp.unk_0A = p[0xF];
    data = *(u32 **)(p + 4) + p[0xD] * frame * p[0xC];
    for (row = 0; row < p[0xD]; row++) {
        for (col = 0, x = x0; col < p[0xC]; col++, x += p[0xE]) {
            sp.unk_04 = x;
            sp.unk_06 = row * p[0xE] + y0;
            v = *data;
            uv = (v & 0xFF0000) >> 16;
            clut = (v & 0xFF00) >> 8;
            sp.unk_0C = search_tpage_multi((*(s32 **)p)[(v & 0xFF000000) >> 24], mode);
            if (p[0xE] == 8) {
                sp.unk_0E = (uv & 0xF) * 8;
                sp.unk_0F = ((uv & 0xF0) >> 4) * 8;
            } else if (p[0xE] == 0x10) {
                sp.unk_0E = (uv & 7) * 0x10;
                sp.unk_0F = ((uv & 0xF8) >> 4) * 0x10;
            } else {
                sp.unk_0F = ((uv & 0xF0) >> 4) * 8;
                sp.unk_0E = (uv & 0xF) * 8;
            }
            sp.unk_10 = cx0 + clut * 0x10;
            sp.unk_12 = cy;
            func_80098B80(&sp, (u8 *)&D_800E8C70 + D_8011ECA0 * 0x14, pri);
            data++;
        }
    }
}
