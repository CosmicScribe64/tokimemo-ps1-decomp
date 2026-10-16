s32 func_800460FC(void) {
    u8 sp46;
    u8 sp45;
    u8 sp44;
    u32 sp3C;
    u8 sp34;
    s32 temp_s0;
    s32 var_s0;
    s32 var_v0;

    D_801255D0.bytes[2] = 0;
    D_801255D0.bytes[1] = 0;
    D_801255D0.bytes[0] = 0;
    D_801255D0.bytes[3] = 0;
    sp3C = D_801255D0.word;
    if (func_80087954(1, &sp34) != 0) {
        var_s0 = 1;
        if (func_80087C64(0x10, 0, (s32) &D_801255D0) == 0) {
loop_2:
            if (var_s0 >= 0x10) {
                if (func_80046094() == 0x1B) {
                    D_8012512B = -1;
                    if (D_80125128 != 0) {
                        D_80125128 -= 1;
                    } else {
                        D_80125128 = 0x2F;
                    }
                }
                D_801255D0.word = sp3C;
            } else {
                var_s0 += 1;
                if (func_80087C64(0x10, 0, (s32) &D_801255D0) == 0) {
                    goto loop_2;
                }
            }
        }
    }
    sp44 = D_801255D0.bytes[0];
    sp45 = D_801255D0.bytes[1];
    sp46 = D_801255D0.bytes[2];
    temp_s0 = func_80087FF0((s32) &sp44);
    if (func_80046684((s32) D_801255D0.bytes[0]) >= 0x4B) {
        return 0;
    }
    if (func_80046684((s32) D_801255D0.bytes[1]) >= 0x3C) {
        return 0;
    }
    if (func_80046684((s32) D_801255D0.bytes[2]) >= 0x4B) {
        return 0;
    }
    var_v0 = 0;
    if (temp_s0 > 0) {
        var_v0 = temp_s0;
    }
    return var_v0;
}
