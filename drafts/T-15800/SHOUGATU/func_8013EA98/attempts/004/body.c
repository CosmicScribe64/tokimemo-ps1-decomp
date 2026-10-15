void func_8013EA98(void) {
    s16 sp40[7];
    s32 sp30[4];
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

    *(s32 *)sp40 = (s32) D_8014610C;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014610C) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014610C) + (8));
    *(u16 *)((unsigned char *)(&sp40[0]) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_8014610C) + (12));
    (*(s32 *)&sp30) = (s32) D_8014611C;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014611C) + (8));
    var_a0 = 0;
    var_v0 = 0;
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014611C) + (4));
    *(u16 *)((unsigned char *)(&sp30) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_8014611C) + (12));
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
    D_800E71DF = temp_t8;
    temp_t7 = (u32) (D_800E6280.unk_1BC[temp_t8 & 0xFF].unk_0C.w << 0x1E) >> 0x1F;
    if ((temp_t7 == 0) && ((u32) (temp_t8 & 0xFF) >= 9U)) {
block_12:
        goto loop_3;
    }
    if ((D_800E71DF == 5) && (((u8) D_800E644A >> 4) == 8)) {
        goto block_12;
    }
    if ((D_800E71DF == 6) && (((u8) D_800E644A >> 4) == 9)) {
        goto block_12;
    }
    if (D_80146054 == D_800E71DF) {
        goto block_12;
    }
    if ((temp_t7 == 0) && (var_a0 >= 3)) {
        goto block_12;
    }
    if (((D_80145FE4 == 7) || (D_80145FE4 == 9)) && (D_80146054 != D_80145FE4)) {
        D_800E71DF = D_80145FE4;
    }
    D_80146058 = D_800E71DF;
    D_80145F2C = 0;
    temp_v0_2 = get_g_zyotai_s(D_800E71DF, 0xA, 5, 8);
    temp_t8_2 = temp_v0_2 & 0x7F;
    if ((D_800E71DF == 6) && (temp_t8_2 >= 2U)) {
        D_80145F2C += 1;
    }
    if ((temp_v0_2 & 0x7F) == 4) {
        D_80145F2C += 1;
    }
    func_8013EE30();
    func_800847B8(D_800E71DF);
    if (!(D_800E6280.unk_1BC[D_800E71DF].unk_0C.w & 2)) {
        D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 2;
        D_800E69A0 = D_800E7388;
        D_800E69A1 = D_800E7389;
        D_800E69A2 = 0x18;
        D_80122D48 = 3;
        func_80085B3C(4, 0);
        func_80042908(5);
    } else {
        D_800E6280.unk_1BC[D_800E71DF].unk_06 += 4;
        var_v0_3 = 0;
loop_25:
        var_v1 = var_v0_3 * 2;
        temp_v0_3 = var_v0_3 + 1;
        if (D_800E6396 >= sp40[var_v0_3]) {
            temp_t8_3 = temp_v0_3;
            var_v0_3 = temp_t8_3;
            if (temp_t8_3 >= 7) {
                var_v1 = temp_t8_3 * 2;
            } else {
                goto loop_25;
            }
        }
        D_800E6280.unk_1BC[D_800E71DF].unk_02 += ((s16 *)sp30)[var_v1 / 2];
        func_80085B3C(4, 0);
        func_8004284C();
    }
    check_para_limit();
}
