? func_80085ED0();                                  /* extern */
s32 func_80085EEC();                                /* extern */
? func_800862D0();                                  /* extern */
s32 func_800862F0();                                /* extern */
? func_80086350();                                  /* extern */
s32 func_80086370();                                /* extern */
extern ? D_800E62B8;
extern s16 D_800E643E;
extern ? D_800E6558;
extern u8 D_800E738A;
extern s16 D_80120962;
extern s16 D_801209A6;
extern s16 D_801209EA;
extern s16 D_80120A2E;
extern u8 D_801491E1;
extern s8 D_8014920B;
extern u16 D_80149214;
extern ? D_801494C4;

void func_80134F98(void) {
    s32 sp60;
    ? sp34;
    u8 *sp30;
    ? *var_v0_4;
    ? *var_v0_5;
    s16 temp_t0;
    s16 temp_t0_2;
    s16 temp_t1;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t6;
    s16 temp_t6_2;
    s16 temp_t7_2;
    s16 temp_t8;
    s32 temp_t7;
    s32 var_v1;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v0_3;

    M2C_MEMCPY_ALIGNED(&sp34, &D_801494C4, 0x24);
    *(s32 *)((unsigned char *)(&sp34) + (36)) = (s32) *(s32 *)((unsigned char *)(&D_801494C4) + (36));
    *(s32 *)((unsigned char *)(&sp34 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801494C4 + 0x24) + (4));
    func_80085F0C(0, 0xF, 0, 0x70, 0x280);
    func_80085F0C(0, 0xF, 0x70, 0x80, 0x80);
    switch (D_800E738A) {
    case 0:
        func_80044750(0x60B);
        func_80086350();
        func_8004284C();
        break;
    case 1:
        if (func_80086370() != 0) {
            if ((*D_801491E0 == 1) || (D_801491E1 == 1)) {
                func_8004500C(1, 0x205);
            } else {
                func_8004500C(1, 0x206);
            }
            D_8014920B = 0;
            D_80149214 = 0;
            func_8004284C();
        }
        break;
    case 2:
        D_80149214 += 1;
        func_80138800();
        var_v0 = D_8011ECD0;
        do {
            temp_t0 = *(s16 *)((unsigned char *)var_v0 + (6566));
            var_v0 += 0x44;
            *(s16 *)((unsigned char *)var_v0 + (6498)) = (s16) (temp_t0 - 1);
        } while ((u32) var_v0 < (u32) D_8011EDE0);
        var_v0_2 = D_8011ECD0;
        do {
            temp_t6 = *(s16 *)((unsigned char *)var_v0_2 + (7314));
            var_v0_2 += 0x44;
            *(s16 *)((unsigned char *)var_v0_2 + (7246)) = (s16) (temp_t6 - 1);
        } while ((u32) var_v0_2 < (u32) D_8011EDE0);
        if ((D_80120962 < -0x9F) || (D_801209A6 < -0x9F) || (D_801209EA < -0x9F) || (D_80120A2E < -0x9F)) {
            var_v1 = 0;
            if (D_80122E94 == 1) {
                func_80138950();
            } else {
                var_v0_3 = D_801491E0;
                do {
                    if (*var_v0_3 == 1) {
                        sp60 = var_v1;
                        sp30 = var_v0_3;
                        func_8013384C(*(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (52)), var_v1 & 0xFF);
                    }
                    var_v1 += 1;
                    var_v0_3 += 1;
                } while (var_v1 != 4);
            }
            func_80085ED0();
            func_8004284C();
        }
        break;
    case 3:
        func_80138800();
        if (D_80122E94 == 1) {
            if (D_80120ABA < -0x58) {
                D_80120ABA += 4;
            } else if (D_80120ABA == -0x58) {
                func_80044750(0x50D);
                D_80120ABA = -0x54;
            }
        }
        if (func_80085EEC() >= 0x79) {
            func_8004284C();
        }
        break;
    case 4:
        if (func_800460EC() == 2) {
            func_80085ED0();
            func_8004284C();
        }
        break;
    case 5:
        if (func_80085EEC() >= 0x79) {
            func_800862D0();
            func_8004284C();
        }
        break;
    case 6:
        if (func_800862F0() != 0) {
            if ((u8) D_801491DC < 0xBU) {
                D_800E6280.unk_1BC[D_801491DC].unk_06 += 2;
            } else {
                temp_t7 = *(s32 *)((unsigned char *)(sp + (D_801491EC * 4)) + (52));
                D_800E6280.unk_1BC[temp_t7].unk_06 += 1;
            }
            if (*D_801491E0 == 1) {
                D_800E643E += 6;
                var_v0_4 = &D_800E62B8;
                do {
                    temp_t7_2 = *(s16 *)((unsigned char *)var_v0_4 + (502));
                    temp_t1 = *(s16 *)((unsigned char *)var_v0_4 + (558));
                    temp_t3 = *(s16 *)((unsigned char *)var_v0_4 + (614));
                    temp_t8 = *(s16 *)((unsigned char *)var_v0_4 + (446));
                    var_v0_4 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_4 + (390)) = (s16) (temp_t3 + 6);
                    *(s16 *)((unsigned char *)var_v0_4 + (334)) = (s16) (temp_t1 + 6);
                    *(s16 *)((unsigned char *)var_v0_4 + (278)) = (s16) (temp_t7_2 + 6);
                    *(s16 *)((unsigned char *)var_v0_4 + (222)) = (s16) (temp_t8 + 6);
                } while (var_v0_4 != &D_800E6558);
            } else if (D_801491E1 == 1) {
                D_800E643E += 3;
                var_v0_5 = &D_800E62B8;
                do {
                    temp_t6_2 = *(s16 *)((unsigned char *)var_v0_5 + (502));
                    temp_t2 = *(s16 *)((unsigned char *)var_v0_5 + (558));
                    temp_t4 = *(s16 *)((unsigned char *)var_v0_5 + (614));
                    temp_t0_2 = *(s16 *)((unsigned char *)var_v0_5 + (446));
                    var_v0_5 += 0xE0;
                    *(s16 *)((unsigned char *)var_v0_5 + (390)) = (s16) (temp_t4 + 3);
                    *(s16 *)((unsigned char *)var_v0_5 + (334)) = (s16) (temp_t2 + 3);
                    *(s16 *)((unsigned char *)var_v0_5 + (278)) = (s16) (temp_t6_2 + 3);
                    *(s16 *)((unsigned char *)var_v0_5 + (222)) = (s16) (temp_t0_2 + 3);
                } while (var_v0_5 != &D_800E6558);
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
