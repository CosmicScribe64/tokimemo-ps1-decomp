/* NON_MATCHING: T-10320, IDO hoists the loop-invariant &D_80155B36 into $s8 (the original keeps lui/addiu in the loop and hoists 12 into $s7), 26 lines */
void func_8013A3E0(void *arg0, u8 *arg1, s32 arg2) {
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *p;
    s32 i;
    s32 j;
    s32 k;

    a = D_80158A74;
    b = D_80158A64;
    c = D_80158A8C;
    if (a[0x52] == 0 && a[0] != 0 && func_80145508(a + 0x14, D_80155B30, arg0, arg1) != 0) {
        func_80144094();
        *(u16 *)(a + 4) = 0x60;
        *(s16 *)(a + 10) = 0;
        *(s16 *)(a + 8) = 0;
        *(s16 *)(a + 6) = 0;
        a[15] = 0;
        a[14] = 0;
        a[13] = 0;
    }
    p = D_80158A80;
    i = 0;
    do {
        if (p[0] != 0 && *(u16 *)(p + 4) == 0x40 && func_80145508(p + 0x14, &D_80155B36, arg0, arg1) != 0) {
            b[0x25] = b[0x25] + 1;
            if (b[0x24] != 0 && !(b[0x25] & 3)) {
                func_80133D14(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x20, 0xA0);
            }
            func_80133AD0(0x508);
            *(u16 *)(p + 4) = 0x80;
            *(s16 *)(p + 10) = 0;
            *(s16 *)(p + 8) = 0;
            *(s16 *)(p + 6) = 0;
            p[15] = 0;
            p[14] = 0;
            p[13] = 0;
            if (arg2 >= 0) {
                if (i >= 6) {
                    *(s16 *)(c + arg2 * 2 + 0x2D8) -= 1;
                } else if (*(u16 *)(p + 2) == 9) {
                    *(s16 *)(c + arg2 * 2 + 0x2D8) -= 4;
                } else {
                    *(s16 *)(c + arg2 * 2 + 0x2D8) -= 2;
                }
                j = arg2 * 2;
                if (arg2 >= 2) {
                    j = arg2 * 2 + 2;
                }
                c[j * 0x14 + 0x42] = 1;
                if (*(s16 *)(c + arg2 * 2 + 0x2D8) < 0) {
                    func_80133AD0(0x50A);
                    c[arg2 + 0x343] = 1;
                }
            }
        }
        i++;
        p += 0x54;
    } while (i != 12);
}
