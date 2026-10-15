void func_80134724(void) {
    s16 var_a2;
    s16 var_v0;
    s32 var_at;
    s32 var_v1;
    u16 temp_a0;
    u32 temp_t6;
    u32 temp_t7;

    temp_t6 = get_g_zyotai_s(D_800E71DF) & 0x7F;
    if (temp_t6 < 2U) {
        var_v1 = 0;
    } else if (temp_t6 == 2) {
        var_v1 = 1;
    } else {
        var_v1 = 3;
        if (temp_t6 == 3) {
            var_v1 = 2;
        }
    }
    temp_t7 = (u16) D_800E6374 >> 0xC;
    temp_a0 = *(&D_800E68EC + (temp_t7 * 4));
    var_v0 = (s16) (temp_a0 / 50U);
    if (temp_a0 >= 0x64U) {
        var_v0 = 2;
    }
    var_at = temp_a0 < 0x164U;
    if (temp_a0 >= 0xC8U) {
        var_v0 = 3;
        var_at = temp_a0 < 0x164U;
    }
    if (var_at == 0) {
        var_v0 = 4;
    }
    D_80139AE0 = 0;
    switch (D_800E71DF) {
    case 0:
        var_a2 = (s16) temp_t7;
        if (temp_t7 == 9) {
            var_a2 = 8;
            if (temp_a0 >= 0x164U) {
                var_v0 = 5;
            }
        }
        if (temp_t7 == 0xA) {
            var_a2 = 8;
            if (temp_a0 >= 0x164U) {
                var_v0 = 6;
            }
        }
        D_80139ADC = (var_a2 * 0x14) + (var_v0 * 4) + var_v1 + 3;
        if ((D_800E62BE == 0x61) && (temp_t7 == 6)) {
            D_80139ADC = var_v1 + 0xBF;
        }
        break;
    case 1:
        D_80139ADC = (temp_t7 * 0x14) + (var_v0 * 4) + var_v1 + 8;
        break;
    case 2:
        D_80139ADC = ((temp_t7 * 0x14) + (var_v0 * 4) + var_v1) - 0x20;
        break;
    case 3:
        D_80139ADC = ((temp_t7 * 0x14) + (var_v0 * 4) + var_v1) - 0x48;
        break;
    case 4:
        D_80139ADC = ((temp_t7 * 0x18) + (var_v0 * 4) + var_v1) - 0x89;
        if ((D_800E62BE == 0x61) && (((u16) D_800E6374 >> 0xC) == 6)) {
            D_80139ADC = var_v1 + 0x1B;
        }
        break;
    case 5:
        D_80139ADC = (var_v0 * 4) + var_v1 + 7;
        break;
    case 6:
        D_80139ADC = (var_v0 * 4) + var_v1 + 7;
        break;
    default:
        D_80139ADC = (var_v0 * 4) + var_v1 + 3;
        break;
    }
    func_801349D4();
    func_8004284C();
}
