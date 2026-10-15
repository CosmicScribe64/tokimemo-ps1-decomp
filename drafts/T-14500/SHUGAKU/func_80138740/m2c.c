extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern s32 D_800E6378;
extern s32 D_800E644C;
extern s8 D_800E69A1;
extern s8 D_800E69A2;
extern u8 D_800E69DD;
extern u8 D_800E71DF;
extern u8 D_800E7389;

void func_80138740(void) {
    s32 temp_v0;

    temp_v0 = func_80051A68(D_800E71DF);
    if ((D_800E62BF == (D_800E644C & 0xF)) && (D_800E62C0 == ((u32) (D_800E644C << 0x17) >> 0x1B))) {
        D_800E69A1 = D_800E7389 + 1;
        D_800E69A2 = 0;
        D_800E69DD = D_800E71DF;
        D_800E71DF = 0;
        func_80042908(6);
        return;
    }
    if (((D_800E6378 & 0xF) == D_800E62BF) && (((u32) (D_800E6378 << 0x17) >> 0x1B) == D_800E62C0)) {
        D_800E69A1 = D_800E7389 + 1;
        D_800E69A2 = 0;
        D_800E69DD = D_800E71DF;
        func_80042908(7);
        return;
    }
    if ((D_800E7389 == 4) && (D_800E71DF == 2) && (D_800CA2CC == 2)) {
        D_800E69A1 = D_800E7389 + 1;
        D_800E69A2 = 0;
        func_80042908(8);
        return;
    }
    if ((D_800E7389 == 2) && (D_800E71DF == 5) && ((u32) (temp_v0 & 0x7F) < 2U)) {
        D_800E69A1 = D_800E7389 + 1;
        D_800E69A2 = 0;
        func_80042908(0xB);
        return;
    }
    func_8004284C();
}
