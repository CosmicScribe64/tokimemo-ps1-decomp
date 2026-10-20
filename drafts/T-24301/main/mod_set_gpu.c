/* T-24301 draft (autotweak best), ~120 rows; needs psx_types.h. mode is stack-resident (stored at sp+0x78 in
 * every case), p at sp+0x6C; the frame wants 0xA8. */
void mod_set_gpu(s32 idx, s32 frame, s32 x, s32 y, s32 cx, s32 cy, s32 pri) {
    u8 *p;
    s32 mode;
    SPRT_8 *s;
    u32 row;
    u32 col;
    u32 *data;
    u32 v;
    u32 uv;
    u32 page;
    u32 clut;
    u32 tpage;
    u32 last;
    s32 bright;

    bright = func_8004901C();
    p = D_801217A0 + idx * 0x14;
    switch (p[8]) {
    case 0:
        mode = 1;
        break;
    case 1:
        mode = 2;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 1;
        break;
    case 4:
        mode = 1;
        break;
    case 5:
        mode = 1;
        break;
    default:
        mode = 1;
        break;
    }
    last = 0xFFFF;
    data = *(u32 **)(p + 4) + p[0xD] * frame * p[0xC];
    for (row = 0; row < p[0xD]; row++) {
        for (col = 0; col < p[0xC]; col++) {
            v = *data;
            uv = ((*data) & 0xFF0000) >> 16;
            page = (v & 0xFF000000) >> 24;
            clut = (v & 0xFF00) >> 8;
            s = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F07C(s);
            SetSemiTrans(s, 0);
            s->x0 = x + 0xA0 + col * 8;
            s->y0 = y + row * 8 + 0x168 - D_8011ECA0 * 240;
            s->v0 = ((uv & 0xF0) >> 4) * 8;
            s->u0 = (uv & 0xF) * 8;
            s->b0 = bright;
            s->g0 = bright;
            s->r0 = bright;
            tpage = search_tpage_multi((*(s32 **)p)[page], mode);
            if (last != tpage) {
                if (last != 0xFFFF) {
                    goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
                }
                last = tpage & 0xFFFF;
            }
            s->clut = func_8009ED74(cx + clut * 16, cy);
            AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + pri * 4 + 4, s);
            data++;
        }
    }
    goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
}
