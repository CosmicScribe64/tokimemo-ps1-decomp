extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern s32 D_800E6378;
extern s32 D_800E644C;
extern u8 D_800E699E;
extern s8 D_800E69A1;
extern s8 D_800E69A2;
extern u8 D_800E69DD;
extern u8 D_800E71DF;

void func_80134C98(void) {
    if ((D_800E62BF == (D_800E644C & 0xF)) && (D_800E62C0 == ((u32) (D_800E644C << 0x17) >> 0x1B))) {
        D_800E69A1 = 5;
        D_800E69A2 = 0;
        D_800E69DD = D_800E71DF;
        D_800E71DF = 0;
        func_80042908(6);
        return;
    }
    if (((D_800E6378 & 0xF) == D_800E62BF) && (((u32) (D_800E6378 << 0x17) >> 0x1B) == D_800E62C0)) {
        D_800E69A1 = 5;
        D_800E69A2 = 0;
        D_800E69DD = D_800E71DF;
        D_800E699E |= 8;
        func_80042908(7);
        return;
    }
    func_8004284C();
}
