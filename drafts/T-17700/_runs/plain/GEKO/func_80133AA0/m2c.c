extern u8 D_800E71DF;
extern s32 D_800E7384;

void func_80133AA0(void) {
    u8 sp2B;
    s16 temp_a0;

    sp2B = func_80051B48(D_800E71DF);
    func_801322E8();
    temp_a0 = D_80144C40;
    if ((temp_a0 == 2) && (D_800E71DF == 8) && (D_80144C50 == 1)) {
        if (D_800E7384++ == 1) {
            func_80083440(3U);
        }
    }
    if ((temp_a0 == 4) && (D_800E71DF == 8)) {
        if (D_80144C50 == 1) {
            if (D_800E7384++ == 1) {
                func_80083440(sp2B);
            }
        }
    }
    if ((temp_a0 == 1) || (temp_a0 == 5)) {
        if (D_800E71DF == 0xA) {
            if (D_80144C50 == 1) {
                if (D_800E7384++ == 1) {
                    func_80083440(2U);
                }
            }
        }
    }
    if ((temp_a0 == 3) && (D_800E71DF == 0xA)) {
        if (D_80144C50 == 1) {
            if (D_800E7384++ == 1) {
                func_80083440(3U);
            }
        }
    }
}
