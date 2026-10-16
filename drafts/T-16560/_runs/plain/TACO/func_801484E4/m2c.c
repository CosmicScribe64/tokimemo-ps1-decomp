extern s32 D_800E7384;
extern ? D_801270B0;
extern ? D_80127270;

void func_801484E4(void) {
    s32 sp2C;
    ? *var_v0;
    s32 var_t0;
    u8 temp_t1;
    u8 temp_t9;

    var_t0 = D_800E7384;
    if (D_800E7384 < 0x78) {
        if (((s32) D_800E7384 % 60) == 0) {
            sp2C = var_t0;
            func_801335A0(0, 0x508);
        }
        sp2C = var_t0;
        func_80147E80(0);
    }
    if (var_t0 == 0x64) {
        D_80127090.unk0 = 0x80000000;
        var_v0 = &D_801270B0;
        D_801270A0.unk0 = 0x80000000;
        do {
            var_v0 += 0x40;
            *(s32 *)((unsigned char *)var_v0 + (-64)) = 0x80000000;
            *(s32 *)((unsigned char *)var_v0 + (-48)) = 0x80000000;
            *(s32 *)((unsigned char *)var_v0 + (-32)) = 0x80000000;
            *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x80000000;
        } while (var_v0 != &D_80127270);
    }
    if (var_t0 < 0x78) {
        func_80153E78(1, 0xE0U, 0xE0U, 0xE0U);
    } else if ((var_t0 >= 0x78) && (var_t0 < 0xB0)) {
        temp_t9 = (0x2C0 - (var_t0 * 4)) & 0xFF;
        func_80153E78(1, temp_t9, temp_t9 & 0xFF, temp_t9 & 0xFF);
    } else if ((var_t0 >= 0xB0) && (var_t0 < 0xF0)) {
        temp_t1 = ((var_t0 * 4) - 0x2C0) & 0xFF;
        func_80153E78(2, temp_t1, temp_t1 & 0xFF, temp_t1 & 0xFF);
    } else if (var_t0 == 0xF0) {
        func_80143730(0x80162000, 0xE);
        func_80143730(0x80172000, 0xC);
        func_80042908(0x15);
    }
    D_800E7384 += 1;
}
