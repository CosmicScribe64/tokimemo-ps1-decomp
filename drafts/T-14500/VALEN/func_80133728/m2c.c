u32 func_80052648(u8);                              /* extern */
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern s32 D_800E6378;
extern s16 D_800E643E;
extern u8 D_800E69A1;
extern s8 D_800E69A2;
extern u8 D_800E71DF;
extern u8 D_800E7389;
extern u8 D_800E738A;

void func_80133728(void) {
    if ((((u32) (D_800E6378 << 0x17) >> 0x1B) == D_800E62C0) && ((D_800E6378 & 0xF) == D_800E62BF) && (((func_80052648(D_800E71DF) >= 0x46U) && (D_800E71DF != 0)) || ((D_800E643E >= 0x46) && (D_800E71DF == 0))) && ((D_800E71DF == D_80134540) || (D_800E71DF == D_80134541) || (D_800E71DF == D_80134542))) {
        D_800E69A1 = D_800E7389;
        D_800E69A2 = D_800E738A + 1;
        func_80042908(3);
        return;
    }
    if (((func_80052648(D_800E71DF) >= 0x46U) && (D_800E71DF != 0)) || ((D_800E643E >= 0x46) && (D_800E71DF == 0))) {
        func_80132348();
        return;
    }
    func_8004284C();
}
