void func_80144094(void) {
    s32 pad[2];
    MATRIX m;
    SVECTOR v;
    s32 sxy[13][9];
    s32 ang;
    s32 i;
    s16 *q;
    s32 idx;
    POLY_FT4 *poly;
    s32 depth;
    s32 flag;
    s32 j;
    TcTex4 ut;
    TcTex4 vt;

    ut = D_8015F5F8;
    vt = D_8015F5FC;
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    v.vy = D_8015F5F0.b;
    ang = 0;
    for (i = 0; i < 9; i++) {
        if (i == 8) {
            v.vz = ang;
        } else {
            v.vz = ang + D_8015F5F0.c;
        }
        for (j = 0; j < 13; j++) {
            v.vx = 0x3000 - j * 0x800 + D_8015F5F0.a;
            func_800AB170(&v, &sxy[j][i], &depth, &flag);
        }
        ang -= 0x1000;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 12; j++) {
            q = (s16 *)&sxy[j][i];
            if (q[20] >= 0 && q[2] < 0x201) {
                if (q[1] >= D_800E6280.unk_018[D_8011ECA0] - 0x200 && D_800E6280.unk_018[D_8011ECA0] + 0x400 >= q[3]) {
                    poly = func_800490F0(0x28, D_8011ECA0);
                    func_8009F040(poly);
                    poly->r0 = 0x80;
                    poly->g0 = 0x80;
                    poly->b0 = 0x80;
                    poly->tpage = func_8009ECB0(1, 0, 0x200, 0x100);
                    poly->clut = func_8009ED74(0, 0x1ED);
                    idx = (j + i + D_8015F5F0.d) & 7;
                    if (idx >= 4) {
                        idx = 0;
                    }
                    poly->u0 = ut.b[idx];
                    poly->v0 = vt.b[idx];
                    poly->u1 = ut.b[idx] + 0x3F;
                    poly->v1 = vt.b[idx];
                    poly->u2 = ut.b[idx];
                    poly->v2 = vt.b[idx] + 0x3F;
                    poly->u3 = ut.b[idx] + 0x3F;
                    poly->v3 = vt.b[idx] + 0x3F;
                    poly->x0 = q[0];
                    poly->y0 = q[1];
                    poly->x1 = q[2];
                    poly->y1 = q[3];
                    poly->x2 = q[18];
                    poly->y2 = q[19];
                    poly->x3 = q[20];
                    poly->y3 = q[21];
                    func_8009EED0(D_800E8CA0 + (D_8011ECA0 << 10) + 0x3F8, poly);
                }
            }
        }
    }
}
