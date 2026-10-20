void func_8004A414(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    GameState *g;
    POLY_GT4 *poly;
    s32 x;
    s32 y;
    s32 random_x;

    arg3 &= 0x7F;
    arg4 &= 0x7F;
    if (arg3 < 8) {
        arg3 = 8;
    }
    if (arg4 < 8) {
        arg4 = 8;
    }
    for (x = arg0 - arg2; x < arg0 + arg2; x += arg3) {
        g = &D_800E6280;
        {
            y = (arg1 - arg2);
            if (y < (arg1 + arg2)) {
                do {
                    if ((arg0 - x) * (arg0 - x) + (arg1 - y) * (arg1 - y) < arg2 * arg2) {
                        poly = (POLY_GT4 *)GetWorkBase(sizeof(POLY_GT4), D_8011ECA0);
                        func_8009F068(poly);
                        SetSemiTrans(poly, 0);
                        poly->x0 = (s16) ((g->unk_014[D_8011ECA0] + (s16)x) - (arg3 / 2));
                        poly->y0 = (s16) ((g->unk_018[D_8011ECA0] + (s16)y) - (arg4 / 2));
                        poly->x1 = (s16) (((g->unk_014[D_8011ECA0] + (s16)x) - (arg3 / 2)) + (s16)arg3);
                        poly->y1 = (s16) ((g->unk_018[D_8011ECA0] + (s16)y) - (arg4 / 2));
                        poly->x2 = (s16) ((g->unk_014[D_8011ECA0] + (s16)x) - (arg3 / 2));
                        poly->y2 = (s16) (((g->unk_018[D_8011ECA0] + (s16)y) - (arg4 / 2)) + (s16)arg4);
                        poly->x3 = (s16) (((g->unk_014[D_8011ECA0] + (s16)x) - (arg3 / 2)) + (s16)arg3);
                        poly->v3 = 0x60;
                        poly->v2 = 0x60;
                        poly->y3 = (s16) (((g->unk_018[D_8011ECA0] + (s16)y) - (arg4 / 2)) + (s16)arg4);
                        poly->u3 = 0xD0;
                        poly->u2 = 0xC0;
                        poly->v1 = 0x50;
                        poly->u1 = 0xD0;
                        poly->v0 = 0x50;
                        poly->u0 = 0xC0;
                        poly->b0 = 0x80;
                        poly->g0 = 0x80;
                        poly->r0 = 0x80;
                        poly->b1 = 0x80;
                        poly->g1 = 0x80;
                        poly->r1 = 0x80;
                        poly->b2 = 0x80;
                        poly->g2 = 0x80;
                        poly->r2 = 0x80;
                        poly->b3 = 0x80;
                        poly->g3 = 0x80;
                        poly->r3 = 0x80;
                        poly->tpage = func_8009ECB0(0, 0, 0x3C0, 0x100);
                        random_x = rand();
                        poly->clut = func_8009ED74((random_x & 8) + g->unk_014[D_8011ECA0] + (s16)x, (rand() & 8) + (s16)y + g->unk_018[D_8011ECA0]);
                        AddPrim(&D_800E8CA0[(D_8011ECA0 << 10) + (arg5 * 4)], poly);
                    }
                    y += arg4;
                } while (y < (arg1 + arg2));
            }
        }
    }
}
