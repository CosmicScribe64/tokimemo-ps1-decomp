extern ? D_800E6446;
extern ? D_800E6448;
extern u8 D_800E71DF;

void func_80133598(void) {
    u8 sp2B;
    s16 temp_v0;
    s32 temp_v1;
    u32 temp_a0;
    u8 temp_a1;

    temp_v1 = func_80051A68(D_800E71DF) & 0x7F;
    temp_a0 = temp_v1 & 0xFF;
    temp_a1 = temp_v1 & 0xFF;
    if ((temp_a0 == 4) || (D_80144C58 != 0) || ((D_800E71DF == 6) && (temp_a0 < 2U))) {
        sp2B = temp_a1;
        func_80044750(0x24);
        func_80044750(0x507);
    }
    if (((u8) D_80144C54 != 0) && ((D_80144C50 == 0) || (*(&D_800E6448 + (D_800E71DF * 0x38)) & 1))) {
        func_8004284C();
        return;
    }
    if (((u8) D_80144C54 == 0) && (D_800E71DF == 6) && (temp_a1 < 2U)) {
        func_8004284C();
        return;
    }
    if ((u8) D_80144C54 == 0) {
        if ((temp_a1 < 2U) && (D_80144C58 == 0)) {
            D_80144C30 = D_80144BC8;
            D_80144C34 = D_80144BF8;
            D_80144C38 = D_80144C28;
            temp_v0 = ((s16) *(&D_800E6446 + (D_800E71DF * 0x38)) / 20) + 1;
            D_80144C3C = temp_v0;
            if (temp_v0 >= 6) {
                D_80144C3C = 5;
            }
        }
        func_8004284C();
        return;
    }
    func_80042808();
}
