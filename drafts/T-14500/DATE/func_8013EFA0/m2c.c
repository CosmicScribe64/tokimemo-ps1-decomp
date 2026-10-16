extern ? D_800CA280;
extern u8 D_800E69A4;
extern u8 D_800E69A5;
extern u8 D_800E69A6;
extern u8 D_800E69A7;
extern u8 D_800E71DF;
extern u8 D_800E738A;
extern ? date_place_names;

void func_8013EFA0(void) {
    ? sp2C;

    M2C_MEMCPY_ALIGNED(&sp2C, &date_place_names, 0x2D4);
    *(u16 *)((unsigned char *)(&sp2C + 0x2D0) + (4)) = (u16) *(u16 *)((unsigned char *)(&date_place_names + 0x2D0) + (4));
    func_80065F34(1);
    D_80122CD4 = (s32) D_800E71DF;
    if ((func_80051A68(D_800E71DF) & 0x7F) == 4) {
        D_8015BF3C = 8;
        D_800E738A += 0x24;
        return;
    }
    if (((D_800E69A4 == D_800E69A5) && (date_arrival == 1)) || (D_800E69A6 == 0xFF) || ((D_800E69A5 == D_800E69A6) && (D_800E69A7 == 0xFF)) || ((D_800E69A5 == D_800E69A6) && (D_800E69A5 == D_800E69A7))) {
        D_800E738A += 0x11;
        return;
    }
    if (D_800E69A4 == D_800E69A6) {
        func_800AE0F0(D_800CA25C, (D_800E69A5 * 0x21) + &sp2C);
    } else {
        func_800AE0F0(D_800CA25C, (D_800E69A6 * 0x21) + &sp2C);
    }
    if (D_800E69A7 != 0xFF) {
        D_8015BF3C = 3;
        if (D_800E69A4 == D_800E69A7) {
            func_800AE0F0(&D_800CA280, (D_800E69A5 * 0x21) + &sp2C);
        } else {
            func_800AE0F0(&D_800CA280, (D_800E69A7 * 0x21) + &sp2C);
        }
    }
    func_8004284C();
}
