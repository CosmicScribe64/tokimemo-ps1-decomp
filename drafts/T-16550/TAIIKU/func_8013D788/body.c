void func_8013D788(void) {
    s16 var_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_at;
    s32 var_at_2;
    s32 var_v1;

    temp_v1 = D_800E6375 & 0xF;
    var_v0 = D_800E638E;
    if ((temp_v1 & 1) || (var_at = var_v0 < 0xA0, ((temp_v1 & 2) != 0))) {
        var_v0 = (s16) ((s16) D_800E638E >> 1);
        var_at = var_v0 < 0xA0;
    }
    if (var_at == 0) {
        var_v1 = 0x18000;
    } else if (var_v0 >= 0x64) {
        var_v1 = 0x16000;
    } else if (var_v0 >= 0x4B) {
        var_v1 = 0x14000;
    } else {
        var_v1 = 0x10000;
        if (var_v0 >= 0x32) {
            var_v1 = 0x12000;
        }
    }
    if (D_80149940 == 2) {
        if (var_v0 >= 0x190) {
            var_v1 = 0x18000;
        } else if (var_v0 >= 0x12C) {
            var_v1 = 0x16000;
        } else if (var_v0 >= 0xC8) {
            var_v1 = 0x14000;
        } else {
            var_v1 = 0x10000;
            if (var_v0 >= 0x64) {
                var_v1 = 0x12000;
            }
        }
    }
    if (D_801499E0 >= 0xB) {
        if (D_8014996C > 0) {
            var_a0 = D_80149960 + 0x2000;
        } else if (D_80149960 >= 0x10001) {
            var_a0 = D_80149960 - 0x4000;
        } else {
            var_a0 = D_80149960 - 0x1800;
        }
        if ((var_a0 >= 0xE001) && (D_80149998 <= 0)) {
            var_a0 -= 0x4000;
            var_at_2 = var_v1 < var_a0;
            if (var_a0 < 0xE000) {
                var_a0 = 0xE000;
                goto block_30;
            }
        } else {
block_30:
            var_at_2 = var_v1 < var_a0;
        }
        if (var_at_2 != 0) {
            var_a0 = var_v1;
        }
        if (var_a0 < 0) {
            var_a0 = 0;
        }
        D_801499E0 = 0;
        D_80149960 = var_a0;
    }
    D_80149964 = (s32) (D_80149964 - D_80149960);
    if (*(s16 *)((unsigned char *)(&D_80149964) + (2)) < 0) {
        *(s16 *)((unsigned char *)(&D_80149964) + (2)) = 0x80;
    }
    func_8013D9B4();
    func_8004B19C(D_80149960, 0, -0x64);
    D_801499C0.word += D_80149960;
}
