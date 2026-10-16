s32 func_80136794(void) {
    s32 temp_v0;

    temp_v0 = get_g_zyotai_s(D_800E71DF, &D_800E71DF);
    if ((D_800E62BF == (D_800E6378 & 0xF)) && (D_800E62C0 == ((u32) (D_800E6378 << 0x17) >> 0x1B))) {
        if ((u32) (temp_v0 & 0x7F) < 2U) {
            D_800E69A2 = D_800E738A + 1;
            D_800E699E = (D_800E699E & 0xFFF7) | 4;
            D_800E71DF = D_800E69DD;
            func_800847B8(D_800E69DD);
            func_80042908(4);
            return 0;
        }
        D_800E699E |= 8;
        goto block_5;
    }
block_5:
    return func_8004284C();
}
