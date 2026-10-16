void func_8013E6A4(void) {
    u8 sp2B;

    sp2B = get_g_zyotai_h(D_800E6280.unk_F5F);
    normal_date_speak();
    D_800E6280.unk_1104.w += 1;
    if ((D_800E6280.unk_1104.w ^ 1) == 0) {
        if (D_800CA14C == 2) {
            func_80083440((s32) sp2B);
        }
    }
}
