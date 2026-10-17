/* T-17013 best draft, 49 differing rows (funcdiff --resolve). Needs #include "psx_types.h" and s32 return in main_api.h; x/c registers swapped and the entry load is v0 instead of the t8 temporary. */
s32 func_8004B358(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 pad0;
    s32 x;
    SPRT *p;
    u8 *s;
    u8 c;
    s32 flag;
    s32 tpx;
    s32 tpy;

    if (D_800B3D80 < 4U) {
        return 0;
    }
    if (D_800B3220 == 0) {
        return 0;
    }
    s = (u8 *)arg0;
    if (*s != 0) {
        x = arg1;
        tpx = (arg3 & 8) * 2 + 0x100;
        tpy = (arg3 & 7) + 0x1F8;
        c = *s;
        do {
            flag = c & 0x20;
            c = c & 0x1F;
            p = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
            func_8009F0A4(p);
            SetSemiTrans(p, 0);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->x0 = D_800E6280.unk_014[D_8011ECA0] + x;
            p->y0 = D_800E6280.unk_018[D_8011ECA0] + arg2;
            if (flag != 0) {
                x += 6;
            } else {
                x += 8;
            }
            if (flag != 0) {
                p->u0 = D_800B3D84[0x14] + c * 8 - 8;
                p->v0 = D_800B3D84[0x15];
            } else {
                p->u0 = D_800B3D84[0x14] + c * 8 - 8;
                p->v0 = D_800B3D84[0x15] - 0x10;
            }
            if (flag != 0) {
                p->w = 6;
                p->h = 0xC;
            } else {
                p->w = 8;
                p->h = 8;
            }
            p->clut = func_8009ED74(tpx, tpy);
            AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
            s++;
            c = *s;
        } while (*s != 0);
    }
    dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
    return 0;
}
