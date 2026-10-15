extern s32 D_80120740;
extern s32 D_80120744;
extern s32 D_80120748;
extern s32 D_8012074C;

void func_801360B0(void) {
    s16 temp_t1;
    s16 temp_t1_2;
    s32 temp_t5;
    s32 temp_t9;
    u32 var_v1;

    if ((D_80120732 == 5) && !(D_8012071F & 0x80)) {
        temp_t1 = D_8013C2E8 + 1;
        D_8013C2E8 = temp_t1;
        if (temp_t1 >= 0x29) {
            func_80044750(0x500);
            D_8013C2E8 = 0;
            D_80120732 = 6;
            D_80120734 = 0;
            D_80120724 = 0;
            D_8012071F |= 0x80;
            D_8012071E = 1;
            D_80120748 = 0x64000;
            D_8012074C = 0x12000;
        }
    }
    if ((D_8012071F & 0x80) && (D_80120732 == 6)) {
        temp_t5 = D_80120748 - 0x1800;
        D_80120748 = temp_t5;
        D_80120740 += temp_t5;
        temp_t9 = D_8012074C - 0x600;
        D_8012074C = temp_t9;
        D_80120744 += temp_t9;
        if (D_80120742 < -0xA0) {
            D_8012071F &= 0xFF7F;
        }
    }
    if (D_80120742 < -0xA0) {
        D_800B5BD4 = 9;
        if (D_8013C2E8 == 0x64) {
            func_80135B68();
        }
        if (D_8013C2E8 == 0xA) {
            func_80044750(0x502);
        }
        if ((D_801206F0 == 1) && (D_801206E0 == 0x21)) {
            func_80135BB8();
        }
        temp_t1_2 = D_8013C2E8 + 1;
        D_8013C2E8 = temp_t1_2;
        if (temp_t1_2 >= 0xA6) {
            D_8013C2E8 = 0xA5;
        }
        var_v1 = D_8013C2E8 + 0x80;
        if (var_v1 >= 0x100U) {
            var_v1 = 0xFF;
        }
        if (var_v1 == 0xFF) {
            if (D_800B593C == 0) {
                var_v1 = 0;
            } else {
                var_v1 = (D_800B593C * 2) - 1;
            }
        }
        func_80046AC8(0x25, 0, 0x1FF, (s32) (s16) ((u32) (D_8013C2E8 * D_8013C2E8) >> 1), -0xA0, -0x78, (var_v1 << 0x10) | (var_v1 << 8) | var_v1 | 0x10000000, 0xA);
        func_800494BC(0, 0, 0xA, 0, 0);
    }
}
