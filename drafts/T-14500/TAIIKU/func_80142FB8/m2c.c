? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
s32 func_80086370();                                /* extern */
extern ? D_800E62B8;
extern s16 D_800E643E;
extern s16 D_800E6442;
extern s16 D_800E6476;
extern s16 D_800E647A;
extern ? D_800E6558;
extern u8 D_800E738A;
extern u8 D_8014A121;

void func_80142FB8(void) {
    u8 *sp3C;
    ? *var_v0_3;
    ? *var_v0_4;
    s16 temp_t0;
    s16 temp_t0_2;
    s16 temp_t2;
    s16 temp_t2_2;
    s16 temp_t4;
    s16 temp_t4_2;
    s16 temp_t8;
    s16 temp_t8_2;
    s16 var_t4;
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_v0;
    u8 *var_v0_2;

    func_80085F0C(0, 0xF, 0, 0xF0, 0x280);
    switch (D_800E738A) {
    case 0:
        func_80086350();
        func_8004284C();
        break;
    case 1:
        if (func_80086370() != 0) {
            if ((*D_8014A120 == 1) || (D_8014A121 == 1)) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            func_80085ED0();
            func_8004284C();
        }
        break;
    case 2:
        if (func_80085EEC() >= 0x3D) {
            func_80145044();
            func_80085ED0();
            func_8004284C();
        }
        break;
    case 3:
        if (func_80085EEC() >= 0x3D) {
            func_801450C0();
            var_s0 = 0;
            if ((u8) D_8014A110 != 0) {
                var_v0 = D_8014A120;
                var_s0_2 = 0;
                do {
                    if (*var_v0 == 1) {
                        sp3C = var_v0;
                        func_8013384C(1, var_s0_2 & 0xFF);
                    }
                    var_s0_2 += 1;
                    var_v0 += 1;
                } while (var_s0_2 != 4);
            } else {
                var_v0_2 = D_8014A120;
                do {
                    if (*var_v0_2 == 1) {
                        sp3C = var_v0_2;
                        func_8013384C(0, var_s0 & 0xFF);
                    }
                    var_s0 += 1;
                    var_v0_2 += 1;
                } while (var_s0 != 4);
            }
            func_80085ED0();
            D_8014A11C = 0;
            func_8004284C();
        }
        break;
    case 4:
        D_8014A11C += 1;
        func_80145148();
        if (func_80085EEC() >= 0xF1) {
            func_800862D0();
            func_8004284C();
        }
        break;
    case 5:
        if (func_800862F0() != 0) {
            if (*D_8014A120 == 1) {
                D_800E643E += 4;
                var_v0_3 = &D_800E62B8;
                do {
                    temp_t0 = *(s16 *)((unsigned char *)var_v0_3 + (502));
                    temp_t2 = *(s16 *)((unsigned char *)var_v0_3 + (558));
                    temp_t4 = *(s16 *)((unsigned char *)var_v0_3 + (614));
                    temp_t8 = *(s16 *)((unsigned char *)var_v0_3 + (446));
                    var_v0_3 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t4 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t2 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t0 + 4);
                    *(s16 *)((unsigned char *)var_v0_3 + (222)) = (s16) (temp_t8 + 4);
                } while (var_v0_3 != &D_800E6558);
                if ((u8) D_8014A110 != 0) {
                    D_800E6476 += 2;
                    D_800E647A += 0xA;
                } else {
                    D_800E643E += 2;
                    var_t4 = D_800E6442 + 0xA;
                    goto block_38;
                }
            } else if (D_8014A121 == 1) {
                D_800E643E += 2;
                var_v0_4 = &D_800E62B8;
                do {
                    temp_t0_2 = *(s16 *)((unsigned char *)var_v0_4 + (502));
                    temp_t2_2 = *(s16 *)((unsigned char *)var_v0_4 + (558));
                    temp_t4_2 = *(s16 *)((unsigned char *)var_v0_4 + (614));
                    temp_t8_2 = *(s16 *)((unsigned char *)var_v0_4 + (446));
                    var_v0_4 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_4 + (390)) = (s16) (temp_t4_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_4 + (334)) = (s16) (temp_t2_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_4 + (278)) = (s16) (temp_t0_2 + 2);
                    *(s16 *)((unsigned char *)var_v0_4 + (222)) = (s16) (temp_t8_2 + 2);
                } while (var_v0_4 != &D_800E6558);
                if ((u8) D_8014A110 != 0) {
                    D_800E6476 += 1;
                    D_800E647A += 5;
                } else {
                    D_800E643E += 1;
                    var_t4 = D_800E6442 + 5;
block_38:
                    D_800E6442 = var_t4;
                }
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
