void func_801424A8(s32 arg0, s32 arg1) {
    s32 pad[2]; /* FAKE: existing sibling frame homes, T-19024. */
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    Tc14 *rec;
    POLY_FT4 *poly;
    s32 side;
    s32 o;
    s32 depth;
    s32 flag;
    s32 j;
    s32 step;
    u8 tbl[2];
    s32 k;

    *(u16 *)tbl = D_8015F590;
    if (arg1 >= 0x80) {
        step = 0x800;
    } else {
        step = (0x18000 - (arg1 << 9)) / 256;
    }
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    rec = &D_8015E770[arg0];
    v.vz = rec->unk4;
    for (o = 0; o < 2; o++) {
        v.vy = rec->unk2 + o * step - step / 2;
        for (j = 0; j < 2; j++) {
            v.vx = rec->unk0 + j * step - step / 2;
            k = j * 2 + o;
            func_800AB170(&v, &pt[k], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            side = (rec->unk12 >> 2) & 1;
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(0x50, 0x1E7);
            /* FAKE: inherited single-line UV macro expansion, T-19024. */
            poly->u0 = tbl[side], poly->v0 = 0, poly->u1 = tbl[side] + 0xF, poly->v1 = 0, poly->u2 = tbl[side], poly->v2 = 0xF, poly->u3 = tbl[side] + 0xF, poly->v3 = 0xF;
            poly->x0 = ((s16 *)pt)[0];
            poly->y0 = ((s16 *)pt)[1];
            poly->x1 = ((s16 *)pt)[2];
            poly->y1 = ((s16 *)pt)[3];
            poly->x2 = ((s16 *)pt)[4];
            poly->y2 = ((s16 *)pt)[5];
            poly->x3 = ((s16 *)pt)[6];
            poly->y3 = ((s16 *)pt)[7];
            func_8009EED0((D_8011ECA0 << 10) + 0x10 + D_800E8CA0, poly);
        }
    }
}
