void func_80135FBC(s32 arg0, s32 arg1) {
    POLY_FT4 *poly;
    s32 p[4][2];
    s32 u0;
    s32 u1;
    s32 u2;
    s32 u3;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 ofs;

    p[0][0] = D_801392AC[arg0][arg1].unk_00.h[1] + arg0 * 25 - 25;
    p[1][0] = D_801392AC[arg0 + 1][arg1].unk_00.h[1] + arg0 * 25 - 1;
    p[2][0] = D_801392AC[arg0][arg1 + 1].unk_00.h[1] + arg0 * 25 - 25;
    p[3][0] = D_801392AC[arg0 + 1][arg1 + 1].unk_00.h[1] + arg0 * 25 - 1;
    p[0][1] = D_801392AC[arg0][arg1].unk_04.h[1] + arg1 * 25 - 25;
    p[1][1] = D_801392AC[arg0 + 1][arg1].unk_04.h[1] + arg1 * 25 - 25;
    p[2][1] = D_801392AC[arg0][arg1 + 1].unk_04.h[1] + arg1 * 25 - 1;
    p[3][1] = D_801392AC[arg0 + 1][arg1 + 1].unk_04.h[1] + arg1 * 25 - 1;
    u2 = u0 = arg0 * 25 + D_801392A4 + 0x27;
    u3 = u1 = arg0 * 25 + D_801392A4 + 0x3F;
    v1 = v0 = arg1 * 25 + D_801392A8 + 0x27;
    v3 = v2 = arg1 * 25 + D_801392A8 + 0x3F;
    while (u0 > 0xC0) {
        u0 -= 0x80;
        u1 -= 0x80;
        u2 -= 0x80;
        u3 -= 0x80;
    }
    while (v0 > 0xC0) {
        v0 -= 0x80;
        v1 -= 0x80;
        v2 -= 0x80;
        v3 -= 0x80;
    }
    poly = func_800490F0(0x28, D_8011ECA0);
    func_8009F040(poly);
    func_8009EFB0(poly, 0);
    func_8009EF84(poly, 1);
    poly->r0 = D_8013921C;
    poly->g0 = D_8013921C;
    poly->b0 = D_8013921C;
    poly->clut = func_8009ED74(0, 0x1FF);
    poly->tpage = func_8009ECB0(1, 4, 0x1C0, 0);
    ofs = -D_8011ECA0 * 0xF0 + 0xF0;
    poly->x0 = p[0][0];
    poly->y0 = p[0][1] + ofs;
    poly->x1 = p[1][0] + 1;
    poly->y1 = p[1][1] + ofs;
    poly->x2 = p[2][0];
    poly->y2 = p[2][1] + ofs + 1;
    poly->x3 = p[3][0] + 1;
    poly->y3 = p[3][1] + ofs + 1;
    poly->u1 = u1 + 1;
    poly->v2 = v2 + 1;
    poly->u3 = u3 + 1;
    poly->v3 = v3 + 1;
    poly->u2 = u2;
    poly->v1 = v1;
    poly->v0 = v0;
    poly->u0 = u0;
    func_8009EED0(D_8011ECA0 * 0x400 + 0x14 + D_800E8CA0, poly);
}
