void func_80134F98(void) {
    typedef struct { s32 w[9]; } M2cBlock24; /* m2c memcpy of 36 bytes */
    s32 sp60;
    s32 sp34[11];
    u8 *sp30;
    s32 *var_v0_4;
    s32 *var_v0_5;
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

    *(M2cBlock24 *)(&sp34) = *(M2cBlock24 *)(&D_801494C4);
    *(s32 *)((unsigned char *)(&sp34) + (36)) = (s32) *(s32 *)((unsigned char *)(&D_801494C4) + (36));
    *(s32 *)((unsigned char *)((u8 *)sp34 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)((u8 *)&D_801494C4 + 0x24) + (4));
    Scroll(0, 0xF, 0, 0x70, 0x280);
    Scroll(0, 0xF, 0x70, 0x80, 0x80);
    switch (D_800E738A) {
    case 0:
        func_80044750(0x60B);
        Fade_In_Init();
        func_8004284C();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
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
                        func_8013384C(sp34[D_801491EC], var_v1 & 0xFF);
                    }
                    var_v1 += 1;
                    var_v0_3 += 1;
                } while (var_v1 != 4);
            }
            Vblnk_Timer_Init();
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
        if (Vblnk_Timer() >= 0x79) {
            func_8004284C();
        }
        break;
    case 4:
        if (func_800460EC() == 2) {
            Vblnk_Timer_Init();
            func_8004284C();
        }
        break;
    case 5:
        if (Vblnk_Timer() >= 0x79) {
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 6:
        if (Fade_Out_Main() != 0) {
            if ((u8) D_801491DC < 0xBU) {
                D_800E6280.unk_1BC[D_801491DC].unk_06 += 2;
            } else {
                temp_t7 = sp34[D_801491EC];
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
