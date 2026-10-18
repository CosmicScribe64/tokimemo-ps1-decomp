/* T-24046 source-only checkpoint: cost 504; no C match. */
void srn_tpage_show(u8 arg0, u8 arg1, u8 arg2, RECT *arg3, s32 arg4, s32 arg5, s16 arg6, s16 arg7, s32 arg8) {
    GameState *g = &D_800E6280;
    POLY_FT4 *poly;
    s32 divisor;
    s32 pageWidth;

    /* arg5 is an unused ABI slot; target clut/depth inputs are arguments 6, 7 and 8. */
    switch (arg1) {
    case 0:
        divisor = 4;
        pageWidth = 0x40;
        break;
    case 1:
        divisor = 2;
        pageWidth = 0x80;
        break;
    case 2:
        divisor = 1;
        pageWidth = 0x100;
        break;
    }
    poly = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = (u8) g->unk_03A;
    poly->g0 = (u8) g->unk_03A;
    poly->b0 = (u8) g->unk_03A;
    poly->u1 = 0xFF;
    poly->v0 = ((arg0 & 0x20) * 4);
    poly->v1 = ((arg0 & 0x20) * 4);
    poly->u2 = 0;
    poly->u0 = 0;
    poly->u3 = 0xFF;
    poly->v2 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->v3 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->x0 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x);
    poly->y0 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x1 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0xFF);
    poly->y1 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x2 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x);
    poly->y2 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->x3 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0xFF);
    poly->y3 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->tpage = func_8009ECB0((s32) arg1, (arg2 & 3), (((arg0 & 0xF) << 6) + ((s32) (arg4 & 0x7F00) / divisor)), (((arg0 & 0x10) * 0x10) + ((arg0 & 0x20) * 4)));
    poly->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], poly);
    poly = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = (u8) g->unk_03A;
    poly->g0 = (u8) g->unk_03A;
    poly->b0 = (u8) g->unk_03A;
    poly->v1 = 0;
    poly->u1 = (((arg4 / 256) * 4) + 4);
    poly->u0 = (((arg4 / 256) * 4) + 3);
    poly->u2 = (((arg4 / 256) * 4) + 3);
    poly->v0 = 0;
    poly->u3 = (((arg4 / 256) * 4) + 4);
    poly->v2 = (s8) arg3->h;
    poly->v3 = (s8) arg3->h;
    poly->x0 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x);
    poly->y0 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x1 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 1);
    poly->y1 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x2 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x);
    poly->y2 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->x3 = (s16) ((g->unk_014[D_8011ECA0] - (arg4 & 0xFF)) + arg3->x + 0x100);
    poly->y3 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->tpage = func_8009ECB0(((s32) arg1), (arg2 & 3), 0x3C0, 0);
    poly->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4) - 4], poly);
    poly = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = (u8) g->unk_03A;
    poly->g0 = (u8) g->unk_03A;
    poly->b0 = (u8) g->unk_03A;
    poly->u1 = 0xFF;
    poly->u2 = 0;
    poly->u0 = 0;
    poly->v1 = (s8) ((arg0 & 0x20) * 4);
    poly->v0 = (s8) ((arg0 & 0x20) * 4);
    poly->u3 = 0xFF;
    poly->v2 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->v3 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->x0 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x100);
    poly->y0 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x1 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x1FF);
    poly->y1 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x2 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x100);
    poly->y2 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->x3 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x1FF);
    poly->y3 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->tpage = func_8009ECB0(((s32) arg1), (arg2 & 3), (((arg0 & 0xF) << 6) + ((s32) (arg4 & 0x7F00) / divisor)) + pageWidth, (((arg0 & 0x10) * 0x10) + ((arg0 & 0x20) * 4)));
    poly->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], poly);
    poly = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = (u8) g->unk_03A;
    poly->g0 = (u8) g->unk_03A;
    poly->b0 = (u8) g->unk_03A;
    poly->u1 = (((arg4 / 256) * 4) + 8);
    poly->u0 = (((arg4 / 256) * 4) + 7);
    poly->u2 = (((arg4 / 256) * 4) + 7);
    poly->v1 = 0;
    poly->v0 = 0;
    poly->u3 = (((arg4 / 256) * 4) + 8);
    poly->v2 = (s8) arg3->h;
    poly->v3 = (s8) arg3->h;
    poly->x0 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x100);
    poly->y0 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x1 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x101);
    poly->y1 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x2 = (s16) (g->unk_014[D_8011ECA0] + (0xFF - (arg4 & 0xFF)) + arg3->x + 0x100);
    poly->y2 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->x3 = (s16) ((g->unk_014[D_8011ECA0] - (arg4 & 0xFF)) + arg3->x + 0x200);
    poly->y3 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->tpage = func_8009ECB0(((s32) arg1), (arg2 & 3), 0x3C0, 0);
    poly->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], poly);
    poly = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(poly);
    if (arg2 != 0) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    SetShadeTex(poly, 0);
    poly->r0 = (u8) g->unk_03A;
    poly->g0 = (u8) g->unk_03A;
    poly->b0 = (u8) g->unk_03A;
    poly->u1 = 0xFF;
    poly->u2 = 0;
    poly->u0 = 0;
    poly->v1 = (s8) ((arg0 & 0x20) * 4);
    poly->v0 = (s8) ((arg0 & 0x20) * 4);
    poly->v2 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->u3 = 0xFF;
    poly->v3 = (s8) (((arg0 & 0x20) * 4) + arg3->h);
    poly->x0 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x200);
    poly->y0 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x1 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x2FF);
    poly->y1 = (s16) (g->unk_018[D_8011ECA0] + arg3->y);
    poly->x2 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x200);
    poly->y2 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->x3 = (s16) (g->unk_014[D_8011ECA0] + (-(arg4 & 0xFF)) + arg3->x + 0x2FF);
    poly->y3 = (s16) (g->unk_018[D_8011ECA0] + arg3->y + arg3->h);
    poly->tpage = func_8009ECB0(((s32) arg1), (arg2 & 3), (((arg0 & 0xF) << 6) + ((s32) (arg4 & 0x7F00) / divisor)) + (pageWidth * 2), (((arg0 & 0x10) * 0x10) + ((arg0 & 0x20) * 4)));
    poly->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], poly);
}
