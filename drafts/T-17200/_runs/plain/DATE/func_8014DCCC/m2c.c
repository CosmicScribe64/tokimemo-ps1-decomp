extern ? D_8007D878;
extern ? D_8007EC9C;
extern u8 D_800E738A;

void func_8014DCCC(void) {
    ? (*temp_v0)(?);
    s32 var_at;
    u8 temp_v1;

    M2C_MEMCPY_ALIGNED(&sp30[0], &D_8015E3C4, 0x9C);
    temp_v0 = (&sp30[0])[D_800E738A];
    if (&D_8007EC9C == temp_v0) {
        D_80120653 |= 0x80;
    }
    if (&D_8007D878 == temp_v0) {
        D_80120653 &= 0xFF7F;
    }
    temp_v0(0x80);
    if (D_80122D04 != 0) {
        func_8014E010();
    }
    temp_v1 = D_800E738A;
    var_at = temp_v1 < 0x19U;
    if (temp_v1 == 0xE) {
        func_8014E108();
        var_at = (u8) D_800E738A < 0x19U;
    }
    if (var_at == 0) {
        if (temp_v1 < 0x24U) {
            func_80140D38();
        }
    }
}
