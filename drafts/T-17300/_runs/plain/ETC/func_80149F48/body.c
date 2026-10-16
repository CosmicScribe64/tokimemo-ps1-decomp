void func_80149F48(void) {
    s32 sp28;
    s16 var_a1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_hi;
    s32 var_v1;
    u32 temp_t0;
    u32 temp_t4;
    u32 temp_t6;
    u32 var_v0;
    u32 var_v0_2;
    u32 var_v0_3;

    temp_a0 = D_80150E94;
    temp_hi = temp_a0 % 3;
    var_a1 = 0;
    switch (temp_hi) {
    case 0:
        temp_t6 = (u16) D_800E68EC >> 4;
        var_v0 = temp_t6;
        if (((s32) temp_t6 > 0) && ((s32) temp_t6 >= 0x15)) {
            var_v0 = 0x14;
        }
        var_v1 = D_800E6382 + var_v0;
        if (D_800E6375 & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    case 1:
        temp_t0 = (u16) D_800E68F4 >> 4;
        var_v0_2 = temp_t0;
        if (((s32) temp_t0 > 0) && ((s32) temp_t0 >= 0x15)) {
            var_v0_2 = 0x14;
        }
        var_v1 = D_800E6386 + var_v0_2;
        if (D_800E6375 & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    default:
        sp28 = 0;
        func_800AE0B0("DEFAUT:", 0);
        var_a1 = 0;
        /* fallthrough */
    case 2:
        temp_t4 = (u16) D_800E68FC >> 4;
        var_v0_3 = temp_t4;
        if (((s32) temp_t4 > 0) && ((s32) temp_t4 >= 0x15)) {
            var_v0_3 = 0x14;
        }
        var_v1 = D_800E638A + var_v0_3;
        if (D_800E6375 & 0xF) {
            var_v1 = var_v1 / 2;
        }
        break;
    }
    temp_a0_2 = temp_a0 + 1;
    if (D_80150E98 == 1) {
        var_v1 += 0x26;
    }
    if (D_80150E98 == 2) {
        var_v1 += 0x36;
    }
    D_80150E94 = temp_a0_2;
    if (var_v1 < (&D_80150EA4)[(s32) (D_80150EA0 + temp_a0_2) % 12]) {
        var_a1 = 1;
    }
    if (var_a1 == 0) {
        D_80150E9C += 1;
    }
    D_80120652 = 5;
    D_80120658 = 0;
    D_80120666 = var_a1;
    D_80120668 = 0;
    D_800E738D += 1;
}
