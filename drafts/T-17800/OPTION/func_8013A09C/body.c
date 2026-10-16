void func_8013A09C(void) {
    u8 t = D_8013D3B4;

    switch (t) {
    case 1:
        func_80044890(1, 0xBF98, 0xBF79, D_8013E340[D_8013D3A0 % 63], D_8013E244[D_8013D3A0 % 63],
                      D_8013E148[D_8013D3A0 % 63]);
        D_8013D3B4 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B4 = 0;
            func_800AE0F0(D_8013D3CC, D_8013D968[D_8013D3A0 % 63]);
            func_800AE0F0(D_8013D3E0, D_8013D3F8[D_8013D3DC % 58]);
        }
        break;
    }
}
