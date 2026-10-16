void func_8007AF0C(void) {
    s32 sp44;
    s32 sp2C[1];
    s16 temp_a0;
    s16 temp_a3;
    s16 temp_v0;
    s16 var_a3;

    if ((func_80079524() == 0) && (func_80079554() == 0) && (D_80125D4C != 2)) {
        func_8007B7E0();
    }
    if (D_80125D10.unk_00 & 0x10000000) {
        if (func_80079578() != 0) {
            func_8008FF5C(0, 0);
        }
    } else {
        temp_a0 = *(D_80125E20 + (((u16) D_80125D4A << 5) + (D_80125D3A * 2)));
        func_8008FF5C((s32) temp_a0, (s32) temp_a0);
    }
    temp_v0 = func_800900A0(*(D_80125DA0 + (((u16) D_80125D4A << 6) + (D_80125D3A * 4))), (s32) (s16) D_80125D4C);
    D_80125D38 = temp_v0;
    if (temp_v0 == -1) {
        func_80059B04(((D_80125D4C << 8) & 0xFF00) | 0x01040000 | ((u16) D_80125D4A & 0xFF));
        func_8007AD6C();
        return;
    }
    D_80125D14 = D_80125D14 & ~2 & ~1 & ~2;
    D_80125D10.unk_00 = (D_80125D10.unk_00 | 0x100) & ~0x1000 & 0xFFEFFFFF;
    D_80125D30 = D_80125D32;
    func_80090270((s32) (s16) D_80125D4C, (s32) &sp2C);
    temp_a3 = sp44 - 0x19;
    var_a3 = temp_a3;
    if (D_80125D10.unk_00 & 0x20) {
        var_a3 = temp_a3 - 0x18;
    }
    D_80125D32 = var_a3;
    D_80125D30 = var_a3;
    func_8008B750(0x7F, 0x7F);
    func_8008C720((s32) D_80125D38, (s32) (s16) D_80125D30, (s32) (s16) D_80125D30);
    func_80090350((s32) D_80125D38, 1, 1);
}
