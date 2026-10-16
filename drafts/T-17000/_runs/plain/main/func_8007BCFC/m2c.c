extern s16 D_80125D30;
extern s16 D_80125D32;
extern s16 D_80125D34;
extern s16 D_80125D36;

void func_8007BCFC(s32 arg0) {
    s8 sp2F;
    s8 sp2E;
    s8 sp2D;
    s8 sp2C;
    u8 sp2B;
    s16 temp_t1;
    s16 temp_t9;
    u8 var_v1;

    if ((arg0 & 0xFFFF) == 1) {
        func_80090E40();
        sp2C = 0x4F;
        sp2E = 0x4F;
        sp2D = 0x4F;
        sp2F = 0x4F;
        D_80125D10.unk_00 = (D_80125D10.unk_00 & ~0x10) | 0x20;
        var_v1 = 0x18;
    } else {
        func_80090E50();
        sp2C = 0x7F;
        sp2E = 0x7F;
        sp2F = 0;
        sp2D = 0;
        D_80125D10.unk_00 = (D_80125D10.unk_00 & ~0x20) | 0x10;
        var_v1 = 0;
    }
    sp2B = var_v1;
    func_80087DC8((s32) &sp2C);
    temp_t9 = 0x50 - sp2B;
    D_80125D30 = temp_t9;
    temp_t1 = 0x5A - sp2B;
    D_80125D34 = temp_t1;
    D_80125D32 = temp_t9;
    D_80125D36 = temp_t1;
}
