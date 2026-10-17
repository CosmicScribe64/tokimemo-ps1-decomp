void func_801374C0(u8 *arg0) {
    u8 *a;
    u8 *p;
    s32 j;
    s32 n;
    /* FAKE: unused homes (pa/pm) place pt at sp+0x68 and the spilled q, i at sp+0x54/0x58 in the 152-byte frame. T-11000. */
    s32 pa[7];
    s16 pt[2];
    s32 pm[3];
    s32 i;
    u8 *q;

    a = D_80158A74; i = 0; q = arg0; do {
        pt[0] = *(s16 *)(q + 0x1E);
        pt[1] = *(s16 *)(q + 0x9E);
        if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, (u8 *)pt, D_80155C64) != 0) {
            func_80144094();
            *(u16 *)(a + 4) = 0x60;
            *(s16 *)(a + 10) = 0;
            *(s16 *)(a + 8) = 0;
            *(s16 *)(a + 6) = 0;
            a[13] = a[14] = a[15] = 0;
        }
        p = D_80158A80;
        for (j = 0, n = 12; j != n; j++, p += 0x54) {
            if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, (u8 *)pt, D_80155C64) != 0) {
                *(u16 *)(p + 4) = 0x80;
                *(s16 *)(p + 10) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 6) = 0;
                p[15] = 0;
                p[14] = 0;
                p[13] = 0;
            }
        }
        i += 4;
        q += 4;
    } while (i != 0x20);
}
