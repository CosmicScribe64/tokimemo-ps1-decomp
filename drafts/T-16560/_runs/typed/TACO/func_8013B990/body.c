void func_8013B990(void) {
    s32 temp_v1;

    func_8013C1F4();
    func_8013C440();
    if (((D_800E6280.unk_1109 != 0xC) || (D_8015EDCC == 0)) && ((D_800E6280.unk_1109 != 0x16) || ((u8) D_8015EDCC < 2U))) {
        if (D_800E6280.unk_1109 == 0x20) {
            if ((u8) D_8015EDCC < 3U) {
                goto block_8;
            }
            return;
        }
block_8:
        func_8013BAC4((s32) D_8015E270, 1, 0);
        func_8013BAC4((s32) D_8015E4F0, 4, 0);
        func_8013BAC4((s32) D_8015E3B0, 1, 1);
        func_8013BAC4((s32) D_8015E630, 4, 1);
        temp_v1 = (((s32) D_8015EDF0 / 10) * 0xA) + 1;
        D_8015EDF0 = temp_v1;
        if (temp_v1 >= 0x989680) {
            D_8015EDF0 = 0x98967F;
        }
    }
}
