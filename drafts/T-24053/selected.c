void func_8013A1D4(void) {
    s32 temp_v1;
    u8 temp_a0;

    func_801386C4();
    temp_v1 = D_800E6280.unk_F5F;
    if (temp_v1 == 2) {
        temp_a0 = get_g_zyotai_h(temp_v1);
        if ((D_800CA2E0 == 7) && (D_80122CDC != 0)) {
            func_80083440(temp_a0);
        }
        if (D_800CA2E0 == 6) {
            if (D_80122CDC == 0) {
                func_80083440(temp_a0);
            }
        }
    }
}
