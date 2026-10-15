void func_801388D8(void) {
    s32 temp_v0;

    temp_v0 = get_g_zyotai_s(D_800E71DF);
    if ((D_800E62BF == (D_800E644C & 0xF)) && (D_800E62C0 == ((u32) (D_800E644C << 0x17) >> 0x1B))) {
        if ((D_800E71DF == 4) && ((u32) (temp_v0 & 0x7F) < 2U)) {
            D_800E69A1 = D_800E7389;
            D_800E69A2 = D_800E738A + 1;
        } else {
            D_800E69A1 = D_800E7389 + 1;
            D_800E69A2 = 0;
        }
        D_800E69DD = D_800E71DF;
        D_800E71DF = 0;
        func_80042908(6);
        return;
    }
    if (((D_800E6378 & 0xF) == D_800E62BF) && (((u32) (D_800E6378 << 0x17) >> 0x1B) == D_800E62C0)) {
        if ((D_800E71DF == 4) && ((u32) (temp_v0 & 0x7F) < 2U)) {
            D_800E69A1 = D_800E7389;
            D_800E69A2 = D_800E738A + 1;
        } else {
            D_800E69A1 = D_800E7389 + 1;
            D_800E69A2 = 0;
        }
        D_800E69DD = D_800E71DF;
        D_800E71DF = D_800E71DF;
        func_80042908(7);
        return;
    }
    func_8004284C();
}
