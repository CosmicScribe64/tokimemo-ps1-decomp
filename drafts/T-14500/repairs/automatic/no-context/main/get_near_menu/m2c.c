s32 abs(s32, s16, ?);                               /* extern */
extern ? D_800E6280;

s32 get_near_menu(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp80;
    s32 sp7C;
    s16 *var_v0_2;
    s16 *var_v0_3;
    s16 *var_v0_4;
    s16 *var_v0_5;
    s16 *var_v0_6;
    s16 *var_v0_7;
    s16 *var_v0_8;
    s16 *var_v0_9;
    s16 temp_a1_5;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s16 temp_v1_5;
    s16 temp_v1_6;
    s16 var_s6;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a1_3;
    s32 temp_a1_4;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s1_3;
    s32 temp_s1_4;
    s32 temp_s1_5;
    s32 var_s2;
    s32 var_s4;
    s32 var_v0;
    u8 temp_v0;
    u8 var_s7;
    void *temp_s5;
    void *temp_v0_2;
    void *temp_v0_3;
    void *var_s0;
    void *var_s0_2;
    void *var_s0_3;
    void *var_s0_4;
    void *var_s0_5;

    temp_s5 = &D_800E6280 + arg2;
    temp_v0 = *(u8 *)((unsigned char *)temp_s5 + (4240));
    var_s7 = 0x10;
    var_s6 = 0x140;
    if (temp_v0 < 0x10U) {
        var_s7 = temp_v0;
    }
    switch (arg0) {                                 /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        var_s2 = 0;
        if ((s32) var_s7 > 0) {
            temp_a1 = var_s7 & 3;
            if (temp_a1 != 0) {
                temp_s1 = arg2 * 8;
                var_s0 = &D_800E6280 + (0 * 0x10) + temp_s1;
                var_v0_2 = &(&sp88[0])[0];
                do {
                    var_s2 += 1;
                    *var_v0_2 = *(s16 *)((unsigned char *)var_s0 + (3984)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1) + (3984));
                    var_v0_2 += 2;
                    var_s0 += 0x10;
                } while (temp_a1 != var_s2);
                if (var_s2 != var_s7) {
                    goto block_10;
                }
            } else {
block_10:
                temp_s1_2 = arg2 * 8;
                var_s0_2 = &D_800E6280 + (var_s2 * 0x10) + temp_s1_2;
                var_v0_3 = &(&sp88[0])[var_s2];
                do {
                    var_v0_3->unk0 = *(s16 *)((unsigned char *)var_s0_2 + (3984)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_2) + (3984));
                    *(s16 *)((unsigned char *)var_v0_3 + (2)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (4000)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_2) + (3984)));
                    *(s16 *)((unsigned char *)var_v0_3 + (4)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (4016)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_2) + (3984)));
                    var_v0_3 += 8;
                    *(s16 *)((unsigned char *)var_v0_3 + (-2)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (4032)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_2) + (3984)));
                    var_s0_2 += 0x40;
                } while (var_v0_3 != &(&sp88[0])[var_s7]);
            }
        default:                                    /* switch 1 */
block_20:
            var_s2 = 0;
        }
        break;
    case 1:                                         /* switch 1 */
        var_s2 = 0;
        if ((s32) var_s7 > 0) {
            temp_a1_2 = var_s7 & 3;
            if (temp_a1_2 != 0) {
                temp_s1_3 = arg2 * 8;
                var_s0_3 = &D_800E6280 + (0 * 0x10) + temp_s1_3;
                var_v0_4 = &(&sp88[0])[0];
                do {
                    var_s2 += 1;
                    *var_v0_4 = *(s16 *)((unsigned char *)var_s0_3 + (3986)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_3) + (3986));
                    var_v0_4 += 2;
                    var_s0_3 += 0x10;
                } while (temp_a1_2 != var_s2);
                if (var_s2 != var_s7) {
                    goto block_18;
                }
            } else {
block_18:
                temp_s1_4 = arg2 * 8;
                var_s0_4 = &D_800E6280 + (var_s2 * 0x10) + temp_s1_4;
                var_v0_5 = &(&sp88[0])[var_s2];
                do {
                    var_v0_5->unk0 = *(s16 *)((unsigned char *)var_s0_4 + (3986)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_4) + (3986));
                    *(s16 *)((unsigned char *)var_v0_5 + (2)) = (s16) (*(s16 *)((unsigned char *)var_s0_4 + (4002)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_4) + (3986)));
                    *(s16 *)((unsigned char *)var_v0_5 + (4)) = (s16) (*(s16 *)((unsigned char *)var_s0_4 + (4018)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_4) + (3986)));
                    var_v0_5 += 8;
                    *(s16 *)((unsigned char *)var_v0_5 + (-2)) = (s16) (*(s16 *)((unsigned char *)var_s0_4 + (4034)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_4) + (3986)));
                    var_s0_4 += 0x40;
                } while (var_v0_5 != &(&sp88[0])[var_s7]);
            }
            goto block_20;
        }
        break;
    }
    temp_s1_5 = arg2 * 8;
    switch (arg1) {                                 /* switch 2; irregular */
    case -1:                                        /* switch 2 */
        temp_a1_3 = var_s7 & 3;
        if ((s32) var_s7 > 0) {
            if (temp_a1_3 != 0) {
                var_v0_6 = &(&sp88[0])[0];
                do {
                    temp_v1 = *var_v0_6;
                    var_s2 += 1;
                    if (temp_v1 > 0) {
                        *var_v0_6 = 0;
                    } else {
                        *var_v0_6 = -temp_v1;
                    }
                    var_v0_6 += 2;
                } while (temp_a1_3 != var_s2);
                if (var_s2 != var_s7) {
                    goto block_32;
                }
            } else {
block_32:
                var_v0_7 = &(&sp88[0])[var_s2];
                do {
                    temp_v1_2 = var_v0_7->unk0;
                    if (temp_v1_2 > 0) {
                        var_v0_7->unk0 = 0;
                    } else {
                        var_v0_7->unk0 = -temp_v1_2;
                    }
                    temp_v1_3 = *(s16 *)((unsigned char *)var_v0_7 + (2));
                    if (temp_v1_3 > 0) {
                        *(s16 *)((unsigned char *)var_v0_7 + (2)) = 0;
                    } else {
                        *(s16 *)((unsigned char *)var_v0_7 + (2)) = (s16) -temp_v1_3;
                    }
                    temp_v1_4 = *(s16 *)((unsigned char *)var_v0_7 + (4));
                    if (temp_v1_4 > 0) {
                        *(s16 *)((unsigned char *)var_v0_7 + (4)) = 0;
                    } else {
                        *(s16 *)((unsigned char *)var_v0_7 + (4)) = (s16) -temp_v1_4;
                    }
                    temp_v1_5 = *(s16 *)((unsigned char *)var_v0_7 + (6));
                    if (temp_v1_5 > 0) {
                        *(s16 *)((unsigned char *)var_v0_7 + (6)) = 0;
                    } else {
                        *(s16 *)((unsigned char *)var_v0_7 + (6)) = (s16) -temp_v1_5;
                    }
                    var_v0_7 += 8;
                } while (var_v0_7 != &(&sp88[0])[var_s7]);
            }
block_64:
            var_s2 = 0;
        }
        break;
    case 1:                                         /* switch 2 */
        temp_a1_4 = var_s7 & 3;
        if ((s32) var_s7 > 0) {
            if (temp_a1_4 != 0) {
                var_v0_8 = &(&sp88[0])[0];
                do {
                    var_s2 += 1;
                    if (*var_v0_8 < 0) {
                        *var_v0_8 = 0;
                    }
                    var_v0_8 += 2;
                } while (temp_a1_4 != var_s2);
                if (var_s2 != var_s7) {
                    goto block_54;
                }
            } else {
block_54:
                var_v0_9 = &(&sp88[0])[var_s2];
                do {
                    if (var_v0_9->unk0 < 0) {
                        var_v0_9->unk0 = 0;
                    }
                    if (*(s16 *)((unsigned char *)var_v0_9 + (2)) < 0) {
                        *(s16 *)((unsigned char *)var_v0_9 + (2)) = 0;
                    }
                    if (*(s16 *)((unsigned char *)var_v0_9 + (4)) < 0) {
                        *(s16 *)((unsigned char *)var_v0_9 + (4)) = 0;
                    }
                    if (*(s16 *)((unsigned char *)var_v0_9 + (6)) < 0) {
                        *(s16 *)((unsigned char *)var_v0_9 + (6)) = 0;
                    }
                    var_v0_9 += 8;
                } while (var_v0_9 != &(&sp88[0])[var_s7]);
            }
            goto block_64;
        }
        break;
    }
    var_s4 = -1;
    if ((s32) var_s7 > 0) {
        var_s0_5 = temp_s1_5 + &D_800E6280;
        do {
            if (*(u8 *)((unsigned char *)var_s0_5 + (3991)) == 0) {
                temp_v1_6 = *(s16 *)((unsigned char *)(sp + (var_s2 * 2)) + (136));
                if (temp_v1_6 != 0) {
                    if ((arg0 == 0) && (temp_v0_2 = &D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5, (*(s16 *)((unsigned char *)var_s0_5 + (3986)) == *(s16 *)((unsigned char *)temp_v0_2 + (3986))))) {
                        if (var_s6 > 0) {
                            var_s6 = -0x140;
                        }
                        if (var_s6 < -abs(*(s16 *)((unsigned char *)var_s0_5 + (3984)) - *(s16 *)((unsigned char *)temp_v0_2 + (3984)))) {
                            var_s6 = abs(*(s16 *)((unsigned char *)var_s0_5 + (3984)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3984))) * -1;
                            goto block_91;
                        }
                    } else if ((arg0 == 1) && (temp_v0_3 = &D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5, (*(s16 *)((unsigned char *)var_s0_5 + (3984)) == *(s16 *)((unsigned char *)temp_v0_3 + (3984))))) {
                        temp_a1_5 = *(s16 *)((unsigned char *)temp_v0_3 + (3986));
                        if (var_s6 > 0) {
                            var_s6 = -0x140;
                        }
                        if (var_s6 < -abs(*(s16 *)((unsigned char *)var_s0_5 + (3986)) - temp_a1_5, temp_a1_5, 1)) {
                            var_s6 = abs(*(s16 *)((unsigned char *)var_s0_5 + (3986)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3986))) * -1;
                            goto block_91;
                        }
                    } else if (var_s6 > 0) {
                        if (temp_v1_6 < var_s6) {
                            var_s6 = temp_v1_6;
                            goto block_91;
                        }
                        if (var_s6 == temp_v1_6) {
                            switch (arg0) {         /* switch 3; irregular */
                            case 0:                 /* switch 3 */
                                sp7C = abs(*(s16 *)((unsigned char *)(&D_800E6280 + (var_s4 * 0x10) + temp_s1_5) + (3986)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3986)));
                                if (abs(*(s16 *)((unsigned char *)var_s0_5 + (3986)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3986))) < sp7C) {
block_91:
                                    var_s4 = var_s2;
                                }
                                break;
                            case 1:                 /* switch 3 */
                                sp80 = abs(*(s16 *)((unsigned char *)(&D_800E6280 + (var_s4 * 0x10) + temp_s1_5) + (3984)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3984)));
                                if (abs(*(s16 *)((unsigned char *)var_s0_5 + (3984)) - *(s16 *)((unsigned char *)(&D_800E6280 + (*(s8 *)((unsigned char *)temp_s5 + (4243)) * 0x10) + temp_s1_5) + (3984))) < sp80) {
                                    goto block_91;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            var_s2 += 1;
            var_s0_5 += 0x10;
        } while (var_s2 != var_s7);
    }
    var_v0 = -1;
    if (var_s4 >= 0) {
        var_v0 = var_s4;
    }
    return var_v0;
}
