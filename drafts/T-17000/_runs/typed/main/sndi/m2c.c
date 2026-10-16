void sndi(u8 *arg0, s32 arg1, s32 arg2) {
    u8 spD4;
    u8 spAC;
    u8 sp84;
    u8 sp5C;
    u8 sp48;
    s32 sp40;
    s32 sp3C;
    s32 sp34;                                       /* compiler-managed */
    u8 *sp30;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t9;
    s32 var_v0;
    s32 var_v0_2;
    u8 *var_t9;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;

    strncpy((s32) &spD4, (s32) arg0, 0x40);
    D_800E6280.unk_F62 = 0;
    temp_t6 = arg2 & 0xF;
    sp3C = 0;
    if (temp_t6 == 0xF) {
        var_v0 = 0;
        if (spD4 != 0) {
            var_v1 = &spD4;
loop_3:
            temp_t9 = var_v0 & 1;
            var_v0 += 1;
            if (*var_v1 == 0x50) {
                if (temp_t9 == 0) {
                    *var_v1 = 0;
                    sp3C = 1;
                    sp34 = temp_t6;
                    sp30 = var_v1;
                    strcpy(&sp84, &spD4);
                    get_p_name((s32) &sp48, sp34);
                    strcat(&sp84, &sp48);
                    sp40 = 0;
                    strcat(&sp84, sp30 + 1);
                    var_v0 = sp40;
                } else {
                    goto block_7;
                }
            } else {
block_7:
                var_v1 = &(&spD4)[var_v0];
                if ((&spD4)[var_v0] == 0) {
                    var_v0 = 0;
                } else {
                    goto loop_3;
                }
            }
        }
        sp3C = 0;
        if (sp3C == 0) {
            sp40 = var_v0;
            strcpy(&sp84, &spD4);
            var_v0 = sp40;
        }
        if (sp84 != 0) {
            var_v1_2 = &(&sp84)[var_v0];
loop_13:
            temp_t7 = var_v0 & 1;
            var_v0 += 1;
            if (*var_v1_2 == 0x47) {
                var_t9 = &(&sp84)[var_v0];
                if (temp_t7 == 0) {
                    *var_v1_2 = 0;
                    sp3C = 1;
                    sp34 = var_v1_2;
                    strcpy(&sp5C, &sp84);
                    get_g_name(&sp48, (s32) D_800E6280.unk_F5F);
                    strcat(&sp5C, &sp48);
                    strcat(&sp5C, sp34 + 1);
                } else {
                    goto block_17;
                }
            } else {
                var_t9 = &(&sp84)[var_v0];
block_17:
                var_v1_2 = &(&sp84)[var_v0];
                if (*var_t9 != 0) {
                    goto loop_13;
                }
            }
        }
        if (sp3C == 0) {
            strcpy(&sp5C, &sp84);
        }
    } else {
        var_v0_2 = 0;
        if (spD4 != 0) {
            var_v1_3 = &spD4;
loop_22:
            temp_t5 = var_v0_2 & 1;
            var_v0_2 += 1;
            if (*var_v1_3 == 0x50) {
                if (temp_t5 == 0) {
                    *var_v1_3 = 0;
                    sp3C = 1;
                    sp30 = var_v1_3;
                    strcpy(&sp5C, &spD4);
                    get_p_name((s32) &sp48, (s32) D_800E6280.unk_F5F);
                    strcat(&sp5C, &sp48);
                    strcat(&sp5C, sp30 + 1);
                } else {
                    goto block_26;
                }
            } else {
block_26:
                var_v1_3 = &(&spD4)[var_v0_2];
                if ((&spD4)[var_v0_2] != 0) {
                    goto loop_22;
                }
            }
        }
        if (sp3C == 0) {
            strcpy(&sp5C, &spD4);
        }
    }
    switch (arg1) {                                 /* irregular */
    case 0:
        if (arg2 == 0xF) {
            strcpy(&spAC, "『");
            strcat(&spAC, &sp5C);
        } else if (arg2 == 0x1F) {
            strcpy(&spAC, "（");
            strcat(&spAC, &sp5C);
        } else if (arg2 == 0x2F) {
            strcpy(&spAC, &sp5C);
        } else {
            strcpy(&spAC, "「");
            strcat(&spAC, &sp5C);
        }
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x32, 0U, &spAC, 0);
            return;
        }
        set_kanji_string(-0x56, 0x32, 0U, &spAC, 0);
        return;
    case 1:
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x42, 0U, &sp5C, 0);
            return;
        }
        set_kanji_string(-0x48, 0x42, 0U, &sp5C, 0);
        return;
    default:
    case 2:
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x52, 0U, &sp5C, 0);
            return;
        }
        set_kanji_string(-0x48, 0x52, 0U, &sp5C, 0);
        return;
    }
}
