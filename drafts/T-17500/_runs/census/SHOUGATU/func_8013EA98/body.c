void func_8013EA98(void) {
    ShoShorts7 sp40;
    ShoShorts7 sp30;
    s16 temp_t8_3;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 var_a0;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s32 temp_v0_2;
    s32 var_v1;
    u32 temp_hi;
    u32 temp_t7;
    u32 temp_t8_2;
    u8 temp_t8;

    sp40 = D_8014610C;
    sp30 = D_8014611C;
    var_a0 = 0;
    var_v0 = 0;
    do {
        temp_v0 = var_v0 + 1;
        var_a0 += ((u32) (D_800E6280.unk_1BC[var_v0].unk_0C.w << 0x1E) >> 0x1F) != 0;
        var_v0 = temp_v0;
    } while (temp_v0 < 0xB);
    var_v0_2 = 0;
loop_3:
    temp_hi = (u32) (D_800E7378 + var_v0_2) % 10U;
    var_v0_2 += 1;
    temp_t8 = temp_hi + 1;
    D_800E6280.unk_F5F = temp_t8;
    temp_t7 = (u32) (D_800E6280.unk_1BC[temp_t8 & 0xFF].unk_0C.w << 0x1E) >> 0x1F;
    if ((temp_t7 == 0) && ((u32) (temp_t8 & 0xFF) >= 9U)) {
block_12:
        goto loop_3;
    }
    if ((D_800E6280.unk_F5F == 5) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 8)) {
        goto block_12;
    }
    if ((D_800E6280.unk_F5F == 6) && (((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 9)) {
        goto block_12;
    }
    if (D_80146054 == D_800E6280.unk_F5F) {
        goto block_12;
    }
    if ((temp_t7 == 0) && (var_a0 >= 3)) {
        goto block_12;
    }
    if (((D_80145FE4 == 7) || (D_80145FE4 == 9)) && (D_80146054 != D_80145FE4)) {
        D_800E6280.unk_F5F = D_80145FE4;
    }
    D_80146058 = D_800E6280.unk_F5F;
    D_80145F2C = 0;
    temp_v0_2 = get_g_zyotai_s(D_800E6280.unk_F5F, 0xA, 5, 8);
    temp_t8_2 = temp_v0_2 & 0x7F;
    if ((D_800E6280.unk_F5F == 6) && (temp_t8_2 >= 2U)) {
        D_80145F2C += 1;
    }
    if ((temp_v0_2 & 0x7F) == 4) {
        D_80145F2C += 1;
    }
    func_8013EE30();
    func_800847B8(D_800E6280.unk_F5F);
    if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.w & 2)) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 2;
        D_800E6280.unk_720 = D_800E6280.unk_1108;
        D_800E6280.unk_721 = D_800E6280.unk_1109;
        D_800E6280.unk_722 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0);
        func_80042908(5);
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
        var_v0_3 = 0;
loop_25:
        var_v1 = var_v0_3 * 2;
        temp_v0_3 = var_v0_3 + 1;
        if (D_800E6280.unk_114.unk_02 >= sp40.v[var_v0_3]) {
            temp_t8_3 = temp_v0_3;
            var_v0_3 = temp_t8_3;
            if (temp_t8_3 >= 7) {
                var_v1 = temp_t8_3 * 2;
            } else {
                goto loop_25;
            }
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += sp30.v[var_v1 / 2];
        func_80085B3C(4, 0);
        func_8004284C();
    }
    check_para_limit();
}
