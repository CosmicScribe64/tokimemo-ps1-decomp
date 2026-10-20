s32 func_80153B38(buf, arg1, col, r)
POLY_G3 **buf;
s32 *arg1;
u32 col;
s32 r;
{
    SVECTOR v[42];
    s32 i;
    u8 cr;
    u8 cg;
    u8 cb;
    u8 dr;
    u8 dg;
    u8 db;
    POLY_G3 **p;

    for (i = 0; i < 42; i++) {
        if (i == 0) {
            v[0].vx = 0x100;
            v[0].vy = 0x78;
            v[0].vz = 0;
        } else {
            v[i].vx = func_800A0140(i * 0x9000 / 0x168) * r / 4096 + 0x100;
            v[i].vy = func_800A0070(i * 0x9000 / 0x168) * r / 4096 + 0x78;
            v[i].vz = 0;
        }
    }
    p = buf;
    for (i = 0; i != 40; i++) {
        *p = (POLY_G3 *)GetWorkBase(sizeof(POLY_G3), D_8011ECA0);
        func_8009F004(*p);
        SetSemiTrans(*p, 1);
        SetShadeTex(*p, 1);
        if (D_8011ECA0 == 0) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy + 0xF0;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy + 0xF0;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy + 0xF0;
        }
        if (D_8011ECA0 == 1) {
            (*p)->x0 = v[i + 1].vx;
            (*p)->y0 = v[i + 1].vy;
            (*p)->x1 = v[0].vx;
            (*p)->y1 = v[0].vy;
            (*p)->x2 = v[i + 2].vx;
            (*p)->y2 = v[i + 2].vy;
        }
        cr = (col >> 16) & 0xFF;
        cg = (col >> 8) & 0xFF;
        cb = col & 0xFF;
        dr = (cr & 0xFF) - 0x25;
        dg = (cg & 0xFF) - 0x25;
        db = (cb & 0xFF) - 0x25;
        (*p)->r0 = dr;
        (*p)->g0 = dg;
        (*p)->b0 = db;
        (*p)->r1 = cr;
        (*p)->g1 = cg;
        (*p)->b1 = cb;
        (*p)->r2 = dr;
        (*p)->g2 = dg;
        (*p)->b2 = db;
        AddPrim(D_8011ECA0 * 0x400 + 0x10 + D_800E8CA0, (s32)*p);
        p++;
    }
}

