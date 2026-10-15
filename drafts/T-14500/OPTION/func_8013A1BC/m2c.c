extern u8 D_8013D3DC;
extern ? D_8013D3F8;
extern ? D_8013D798;
extern ? D_8013D880;
extern ? D_8013DD58;
extern ? D_8013E43C;
extern ? D_8013E538;
extern ? D_8013E634;

void func_8013A1BC(void) {
    s32 temp_v0;
    s32 temp_v1;

    switch (D_8013D3B0) {                           /* irregular */
    case 1:
        temp_v1 = ((u8) D_8013D3DC % 58U) * 4;
        temp_v0 = ((u8) D_8013D3A4 % 63U) * 4;
        func_80044890(0, *(&D_8013D880 + temp_v1), *(&D_8013D798 + temp_v1), *(&D_8013E634 + temp_v0), *(&D_8013E538 + temp_v0), *(&D_8013E43C + temp_v0));
        D_8013D3B0 += 1;
        return;
    case 2:
        if (func_80044E8C() == 1) {
            D_8013D3B0 = 0;
            func_800AE0F0(D_8013D3BC, (((u8) D_8013D3A4 % 63U) * 0x10) + &D_8013DD58);
            func_800AE0F0(D_8013D3E0, (((u8) D_8013D3DC % 58U) * 0x10) + &D_8013D3F8);
        }
        return;
    }
}
