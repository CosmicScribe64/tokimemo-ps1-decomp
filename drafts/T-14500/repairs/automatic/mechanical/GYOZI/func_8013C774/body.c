void func_8013C774(void) {
    s32 sp34[16];
    typedef struct { s32 w[36]; } M2cBlock90; /* m2c memcpy of 144 bytes */
    *(M2cBlock90 *)(&sp34[0]) = *(M2cBlock90 *)(&D_80147C6C);
    (&sp34[0])[D_800F647A]();
    if (D_800F62F0 & 0x100) {
        if (((u8) D_800F53DF >= 6U) && ((u8) D_800F53DF < 9U)) {
            func_8004EE18(D_80147AE0, 0x11, 1, 0, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 0, 0);
        }
        func_800BCDF0("S\n");
    }
    if (D_800F62F0 & 0x800) {
        if (((u8) D_800F53DF >= 6U) && ((u8) D_800F53DF < 9U)) {
            func_8004EE18(D_80147AE0, 0x11, 1, 2, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 2, 0);
        }
        func_800BCDF0("T\n");
    }
}
