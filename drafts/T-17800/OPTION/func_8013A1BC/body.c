void func_8013A1BC(void) {
    u8 t = D_8013D3B0;

    switch (t) {
    case 1:
        func_80044890(0, D_8013D880[D_8013D3DC % 58], D_8013D798[D_8013D3DC % 58], D_8013E634[D_8013D3A4 % 63],
                      D_8013E538[D_8013D3A4 % 63], D_8013E43C[D_8013D3A4 % 63]);
        D_8013D3B0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B0 = 0;
            func_800AE0F0(D_8013D3BC, D_8013DD58[D_8013D3A4 % 63]);
            func_800AE0F0(D_8013D3E0, D_8013D3F8[D_8013D3DC % 58]);
        }
        break;
    }
}
