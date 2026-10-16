void func_80139F8C(void) {
    s32 temp_t6;

    temp_t6 = D_800E6280.unk_F88 & 0x60;
    if (temp_t6 != 0x20) {
        if (temp_t6 == 0x40) {
            D_8013D3A8 -= 1;
        }
    } else {
        D_8013D3A8 += 1;
    }
    if ((u8) D_8013D3A8 >= 0x11U) {
        func_80044750((D_8013D3A8 | 0x1200) & 0xFFFF);
    }
}
