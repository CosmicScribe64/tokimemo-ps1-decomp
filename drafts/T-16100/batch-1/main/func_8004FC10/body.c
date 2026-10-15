void func_8004FC10(s32 arg0) {
    GameState *sp4C;
    void *sp30;
    s16 var_t8;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_t6;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_a3;
    s32 var_t1;
    s32 var_v0;
    s32 var_v0_4;
    s32 var_v0_6;
    s32 var_v1;
    s8 var_v1_2;
    u8 var_v1_3;
    u8 var_v1_4;
    u8 *temp_a2;
    u8 *temp_a2_2;
    u8 *temp_v0_2;
    u8 *var_a2;
    u8 *var_a2_2;
    u8 *var_a2_3;
    u8 *var_t6;
    u8 *var_t7;
    u8 *var_v0_2;
    u8 *var_v0_3;
    u8 *var_v0_5;
    u8 *var_v0_7;

    var_t1 = 0;
    var_a3 = 0x10;
    if (D_800E7208 & 0x100) {
        D_800E7312 += 1;
        *(s8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4243)) = -1;
        return;
    }
    temp_t6 = D_800E7312 & 3;
    switch (temp_t6) {
    default:
        temp_v0 = D_800E7208 & 0xF000;
        if (temp_t6 != 3) {

        } else {
            var_v1 = -1;
            switch (temp_v0) {
            case 0x1000:
                var_v0 = get_near_menu(1, -1, arg0);
block_88:
                var_v1 = var_v0;
                break;
            case 0x4000:
                var_v0 = get_near_menu(1, 1, arg0);
                goto block_88;
            case 0x2000:
                var_v0 = get_near_menu(0, 1, arg0);
                goto block_88;
            case 0x8000:
                var_v0 = get_near_menu(0, -1, arg0);
                goto block_88;
            }
            var_a2 = ((u8 *)&D_800E6280) + arg0;
            if (var_v1 != -1) {
                *(s8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4243)) = (s8) var_v1;
            }
            var_v1_2 = *(s8 *)((unsigned char *)var_a2 + (4243));
            if (var_v1_2 >= (s32) *(u8 *)((unsigned char *)var_a2 + (4240))) {
                sp30 = var_a2;
                *(s8 *)((unsigned char *)var_a2 + (4243)) = func_80050324(arg0, 1);
                var_v1_2 = *(s8 *)((unsigned char *)var_a2 + (4243));
            }
            if (var_v1_2 < 0) {
                sp30 = var_a2;
                *(s8 *)((unsigned char *)var_a2 + (4243)) = func_80050324(arg0, 0);
                var_v1_2 = *(s8 *)((unsigned char *)var_a2 + (4243));
            }
            temp_a1 = arg0 * 8;
            temp_v0_2 = ((u8 *)&D_800E6280) + (var_v1_2 * 0x10) + temp_a1;
            D_8011ECF6 = (*(u8 *)((unsigned char *)temp_v0_2 + (3988)) + *(s16 *)((unsigned char *)temp_v0_2 + (3984))) - 4;
            var_t8 = *(s16 *)((unsigned char *)(((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2 + (4243)) * 0x10) + temp_a1) + (3986)) + 8;
block_96:
            D_8011ECFA = var_t8;
        }
        break;
    case 0:
        sp4C = ((u8 *)&D_800E6280);
        func_8006BA40();
        temp_a2 = ((u8 *)&D_800E6280) + arg0;
        var_v1_3 = 0;
        if (*(u8 *)((unsigned char *)temp_a2 + (4240)) != 0) {
            var_v0_2 = (arg0 * 8) + ((u8 *)&D_800E6280);
loop_9:
            if (*(u8 *)((unsigned char *)var_v0_2 + (3990)) != 0) {
                *(u8 *)((unsigned char *)temp_a2 + (4243)) = var_v1_3;
            } else {
                var_v1_3 += 1;
                var_v0_2 += 0x10;
                if (var_v1_3 >= (u8) *(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4240))) {

                } else {
                    goto loop_9;
                }
            }
        }
        break;
    case 1:
        temp_v0_3 = D_800E7208 & 0xF000;
        if (D_800E7200 & 0xF) {
            var_a2_2 = ((u8 *)&D_800E6280) + arg0;
        } else {
            var_a2_2 = ((u8 *)&D_800E6280) + arg0;
            switch (temp_v0_3) {
            default:
                var_a2_2 = ((u8 *)&D_800E6280) + arg0;
                break;
            case 0x1000:
            case 0x8000:
                var_t1 = -1;
                *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_2 + (4243)) - 1);
                break;
            case 0x2000:
            case 0x4000:
                var_a2_2 = ((u8 *)&D_800E6280) + arg0;
                var_t1 = 1;
                *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_2 + (4243)) + 1);
                break;
            }
        }
        temp_a1_2 = arg0 * 8;
        if (*(s8 *)((unsigned char *)var_a2_2 + (4243)) >= (s32) *(u8 *)((unsigned char *)var_a2_2 + (4240))) {
            *(s8 *)((unsigned char *)var_a2_2 + (4243)) = 0;
        }
        if (*(s8 *)((unsigned char *)var_a2_2 + (4243)) < 0) {
            *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(u8 *)((unsigned char *)var_a2_2 + (4240)) - 1);
        }
        var_v0_3 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_2 + (4243)) * 0x10) + temp_a1_2;
        if (*(u8 *)((unsigned char *)var_v0_3 + (3991)) == 1) {
            var_v0_4 = 0x10 == 0;
loop_28:
            var_a3 -= 1;
            if (var_v0_4 != 0) {
                var_t6 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_2 + (4243)) * 0x10);
            } else {
                if (var_t1 == 1) {
                    *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_2 + (4243)) + 1);
                }
                if (var_t1 == -1) {
                    *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_2 + (4243)) - 1);
                }
                if (*(s8 *)((unsigned char *)var_a2_2 + (4243)) >= (s32) *(u8 *)((unsigned char *)var_a2_2 + (4240))) {
                    *(s8 *)((unsigned char *)var_a2_2 + (4243)) = 0;
                }
                if (*(s8 *)((unsigned char *)var_a2_2 + (4243)) < 0) {
                    *(s8 *)((unsigned char *)var_a2_2 + (4243)) = (s8) (*(u8 *)((unsigned char *)var_a2_2 + (4240)) - 1);
                }
                var_v0_4 = var_a3 == 0;
                if (*(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4243)) * 0x10) + temp_a1_2) + (3991)) != 1) {
                    var_t6 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_2 + (4243)) * 0x10);
                } else {
                    goto loop_28;
                }
            }
            var_v0_3 = var_t6 + temp_a1_2;
        }
        D_8011ECF6 = (*(u8 *)((unsigned char *)var_v0_3 + (3988)) + *(s16 *)((unsigned char *)var_v0_3 + (3984))) - 4;
        var_t8 = *(s16 *)((unsigned char *)(((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_2 + (4243)) * 0x10) + temp_a1_2) + (3986)) + 8;
        goto block_96;
    case 2:
        if (D_800E7200 & 4) {
            var_a2_3 = ((u8 *)&D_800E6280) + arg0;
            if (D_800E7208 & 4) {
                var_a1 = arg0 * 8;
                *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) D_800E6280.unk_1095[arg0];
                var_t7 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_3 + (4243)) * 0x10);
                goto block_71;
            }
            temp_v0_4 = D_800E7208 & 0xF000;
            if (temp_v0_4 != 0x1000) {
                var_a2_3 = ((u8 *)&D_800E6280) + arg0;
                if ((temp_v0_4 != 0x2000) && (temp_v0_4 != 0x4000)) {
                    var_a2_3 = ((u8 *)&D_800E6280) + arg0;
                    if (temp_v0_4 != 0x8000) {
                        var_a2_3 = ((u8 *)&D_800E6280) + arg0;
                    } else {
                        goto block_52;
                    }
                } else {
                    var_t1 = 1;
                    *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_3 + (4243)) + 1);
                }
            } else {
                var_a2_3 = ((u8 *)&D_800E6280) + arg0;
block_52:
                var_t1 = -1;
                *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_3 + (4243)) - 1);
            }
            var_a1 = arg0 * 8;
            if (*(s8 *)((unsigned char *)var_a2_3 + (4243)) >= (s32) *(u8 *)((unsigned char *)var_a2_3 + (4240))) {
                *(s8 *)((unsigned char *)var_a2_3 + (4243)) = 0;
            }
            if (*(s8 *)((unsigned char *)var_a2_3 + (4243)) < 0) {
                *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(u8 *)((unsigned char *)var_a2_3 + (4240)) - 1);
            }
            var_v0_5 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_3 + (4243)) * 0x10) + var_a1;
            if (*(u8 *)((unsigned char *)var_v0_5 + (3991)) == 1) {
                var_v0_6 = 0x10 == 0;
loop_59:
                var_a3 -= 1;
                if (var_v0_6 != 0) {
                    var_t7 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_3 + (4243)) * 0x10);
                } else {
                    if (var_t1 == 1) {
                        *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_3 + (4243)) + 1);
                    }
                    if (var_t1 == -1) {
                        *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(s8 *)((unsigned char *)var_a2_3 + (4243)) - 1);
                    }
                    if (*(s8 *)((unsigned char *)var_a2_3 + (4243)) >= (s32) *(u8 *)((unsigned char *)var_a2_3 + (4240))) {
                        *(s8 *)((unsigned char *)var_a2_3 + (4243)) = 0;
                    }
                    if (*(s8 *)((unsigned char *)var_a2_3 + (4243)) < 0) {
                        *(s8 *)((unsigned char *)var_a2_3 + (4243)) = (s8) (*(u8 *)((unsigned char *)var_a2_3 + (4240)) - 1);
                    }
                    var_v0_6 = var_a3 == 0;
                    if (*(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4243)) * 0x10) + var_a1) + (3991)) != 1) {
                        var_t7 = ((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_3 + (4243)) * 0x10);
                    } else {
                        goto loop_59;
                    }
                }
block_71:
                var_v0_5 = var_t7 + var_a1;
            }
            D_8011ECF6 = (*(u8 *)((unsigned char *)var_v0_5 + (3988)) + *(s16 *)((unsigned char *)var_v0_5 + (3984))) - 4;
            D_8011ECFA = *(s16 *)((unsigned char *)(((u8 *)&D_800E6280) + (*(s8 *)((unsigned char *)var_a2_3 + (4243)) * 0x10) + var_a1) + (3986)) + 8;
        } else {
            sp4C = ((u8 *)&D_800E6280);
            func_8006BA40();
            temp_a2_2 = ((u8 *)&D_800E6280) + arg0;
            var_v1_4 = 0;
            if (*(u8 *)((unsigned char *)temp_a2_2 + (4240)) != 0) {
                var_v0_7 = (arg0 * 8) + ((u8 *)&D_800E6280);
                do {
                    if (*(u8 *)((unsigned char *)var_v0_7 + (3990)) != 0) {
                        *(u8 *)((unsigned char *)temp_a2_2 + (4243)) = var_v1_4;
                    }
                    var_v1_4 += 1;
                    var_v0_7 += 0x10;
                } while (var_v1_4 < (u8) *(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + arg0) + (4240)));
            }
        }
        break;
    }
    if (D_8011ECF6 >= 0x8D) {
        D_8011ECF6 = 0x8C;
    }
    if (D_8011ECF6 < -0x96) {
        D_8011ECF6 = -0x96;
    }
    if (D_8011ECFA < -0x64) {
        D_8011ECFA = -0x64;
    }
    if (D_8011ECFA >= 0x65) {
        D_8011ECFA = 0x64;
    }
}
