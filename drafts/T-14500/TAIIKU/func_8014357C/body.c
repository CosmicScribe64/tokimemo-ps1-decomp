void func_8014357C(void) {
    s16 var_v0;
    s32 temp_v1;
    s32 var_v0_2;

    *D_8014A120 = 0;
    D_8014A13D = 0;
    *(s8 *)((unsigned char *)(&D_8014A13D) + (-1)) = 0;
    D_8014A140 = 0;
    *(s32 *)((unsigned char *)(&D_8014A140) + (4)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A140) + (8)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A140) + (12)) = 0;
    D_8014A150 = 0;
    *(s32 *)((unsigned char *)(&D_8014A150) + (8)) = 1;
    *(s32 *)((unsigned char *)(&D_8014A150) + (12)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A150) + (6)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A150) + (4)) = 0;
    D_8014A13E = 0;
    D_8014A121 = 0;
    D_8014A161 = 0;
    *(s8 *)((unsigned char *)(&D_8014A161) + (-1)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A161) + (3)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A161) + (7)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A161) + (11)) = 0;
    D_8014A17C = 1;
    *(s16 *)((unsigned char *)(&D_8014A17C) + (-6)) = 8;
    *(s32 *)((unsigned char *)(&D_8014A17C) + (-12)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A17C) + (-8)) = 0;
    D_8014A180 = 0;
    D_8014A17A = 0;
    *(s16 *)((unsigned char *)(&D_8014A17A) + (-2)) = 0;
    D_8014A162 = 0;
    D_8014A122 = 0;
    D_8014A185 = 0;
    *(s8 *)((unsigned char *)(&D_8014A185) + (-1)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A185) + (3)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A185) + (7)) = 0;
    D_8014A19A = 0x10;
    *(s32 *)((unsigned char *)(&D_8014A19A) + (-10)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A19A) + (-6)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A19A) + (-2)) = 0;
    D_8014A1A0 = 1;
    *(s32 *)((unsigned char *)(&D_8014A1A0) + (4)) = 0;
    D_8014A19E = 0;
    *(s16 *)((unsigned char *)(&D_8014A19E) + (-2)) = 0;
    D_8014A186 = 0;
    D_8014A123 = 0;
    D_8014A1A9 = 0;
    *(s8 *)((unsigned char *)(&D_8014A1A9) + (-1)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A1A9) + (3)) = 0;
    D_8014A1BE = 0x18;
    *(s32 *)((unsigned char *)(&D_8014A1BE) + (-14)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A1BE) + (-10)) = 0;
    *(s32 *)((unsigned char *)(&D_8014A1BE) + (-6)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A1BE) + (-2)) = 0;
    D_8014A1C4 = 1;
    *(s32 *)((unsigned char *)(&D_8014A1C4) + (4)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A1C4) + (-2)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A1C4) + (-4)) = 0;
    D_8014A1AA = 0;
    D_8014A152 = -0xA0;
    D_8014A1D0 = 0;
    D_8014A1CC.w = 0;
    D_8014A118 = 0;
    temp_v1 = D_800E6375 & 0xF;
    var_v0 = D_800E638E;
    (*(s32 *)D_8014A130) = 0x7E0000;
    *(s32 *)((unsigned char *)D_8014A130 + (4)) = 0x7F0000;
    *(s32 *)((unsigned char *)D_8014A130 + (8)) = 0x7D0000;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0 = (s16) (var_v0 >> 1);
    }
    if (var_v0 >= 0xB4) {
        D_8014A124 = 6;
        var_v0_2 = 0x20000;
    } else if (var_v0 >= 0x64) {
        D_8014A124 = 7;
        var_v0_2 = 0x1E000;
    } else if (var_v0 >= 0x41) {
        D_8014A124 = 8;
        var_v0_2 = 0x1C000;
    } else if (var_v0 >= 0x1E) {
        D_8014A124 = 9;
        var_v0_2 = 0x1A000;
    } else {
        D_8014A124 = 0xA;
        var_v0_2 = 0x18000;
    }
    if ((u8) D_8014A110 != 0) {
        var_v0_2 -= 0x2000;
    }
    D_8014A128 = var_v0_2 >> 3;
    D_8014A12C = var_v0_2;
    func_800AE120(D_800E7374);
}
