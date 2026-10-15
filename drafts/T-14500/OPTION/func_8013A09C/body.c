void func_8013A09C(void) {
    s32 temp_v0;

    switch (D_8013D3B4) {
    case 1:
        temp_v0 = ((u8) D_8013D3A0 % 63U) * 4;
        func_80044890(1, 0xBF98, 0xBF79, *(&D_8013E340 + temp_v0), *(&D_8013E244 + temp_v0), *(&D_8013E148 + temp_v0));
        D_8013D3B4 += 1;
        return;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B4 = 0;
            func_800AE0F0(D_8013D3CC, (((u8) D_8013D3A0 % 63U) * 0x10) + &D_8013D968);
            func_800AE0F0(D_8013D3E0, (((u8) D_8013D3DC % 58U) * 0x10) + &D_8013D3F8);
        }
        return;
    }
}
