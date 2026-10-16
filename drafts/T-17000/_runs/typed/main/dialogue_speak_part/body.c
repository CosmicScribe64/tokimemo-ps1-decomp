s32 dialogue_speak_part(u8 *arg0, u8 arg1, u8 arg2, s32 arg3, u16 arg4, u16 arg5, u8 arg6) {
    s16 sp286;
    u8 *sp27C;
    s32 sp83[1];
    s32 sp81[1];
    s32 sp7F[1];
    s32 sp7D[1];
    u8 sp7C;
    s8 sp7A;
    s8 sp78;
    s8 sp76;
    s32 sp74[1];
    u8 sp68;
    ? sp58.value[0];
    s16 temp_s1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_a1_2;
    s16 var_v1;
    s32 *temp_t0;
    s32 *temp_t0_2;
    s32 *temp_t0_3;
    s32 *temp_t0_4;
    s32 *temp_t0_5;
    s32 *temp_t0_6;
    s32 *temp_t0_7;
    s32 *temp_t0_8;
    s32 *temp_t0_9;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_v1_18;
    s32 var_a1;
    u8 *temp_a0;
    u8 *temp_v0_10;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 *temp_v0_7;
    u8 *temp_v1_4;
    u8 temp_v0_9;
    void *temp_v0_8;
    void *temp_v1;
    void *temp_v1_10;
    void *temp_v1_11;
    void *temp_v1_12;
    void *temp_v1_13;
    void *temp_v1_14;
    void *temp_v1_15;
    void *temp_v1_16;
    void *temp_v1_17;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_5;
    void *temp_v1_6;
    void *temp_v1_7;
    void *temp_v1_8;
    void *temp_v1_9;


    sp58 = D_800CA36C;
    if (arg1 != 0xFF) {
        strcpy(&sp7C, arg0);
    }
    D_80122CF0 = 0;
    D_800CA2A8 = 0;
    temp_s1 = (D_80122D28 * 0x10) + 0x30;
    switch (arg1) {                                 /* irregular */
    case 0x7:
        temp_t0 = arg3 + (arg4 * 4);
        temp_t8 = arg5 * 4;
        temp_v1 = *(*temp_t0 + temp_t8);
        if (((temp_v1->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1 + (4)) != 0)) && (D_800E6280.unk0 != 0)) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v1_2 = *(*temp_t0 + temp_t8);
                func_80046290(temp_v1_2->unk0, *(s32 *)((unsigned char *)temp_v1_2 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {

                    } else {
                        goto block_20;
                    }
                } else {
block_20:
                    D_80122CF0 = 1;
                    D_800CA2AC = 0;
                    goto block_21;
                }
                goto block_231;
            }
            D_800CA2AC = 0;
            func_8007B640();
            func_80044750(0x74);
            return 0x64;
        }
block_21:
        if (sp7C == 0x3E) {
            strncpy((s32) &sp74, (s32) &sp7D, 4);
            sp78 = 0;
            sprintf((s32) &sp68, (s32) "%s ", (s32) &sp74);
            set_kanji_string(-0x78, temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp81, 0, &D_800B2008, &D_800B200C, 0);
        } else if (sp7C == 0x2D) {
            strncpy((s32) &sp74, (s32) &sp7D, 6);
            sp7A = 0;
            sprintf((s32) &sp68, (s32) "%s ", (s32) &sp74);
            set_kanji_string(-0x80, temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp83, 0, &D_800B2018, &D_800B201C, 0);
        } else if (sp7C == 0x2B) {
            strncpy((s32) &sp74, (s32) &sp7D, 2);
            sp76 = 0;
            sprintf((s32) &sp68, (s32) "%s ", (s32) &sp74);
            set_kanji_string(-0x70, temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7F, 0, &D_800B2028, &D_800B202C, 0);
        } else if (sp7C == 0x3C) {
            strncpy((s32) &sp74, (s32) &sp7D, 4);
            sp78 = 0;
            sprintf((s32) &sp68, (s32) "%s ", (s32) &sp74);
            set_kanji_string(-0x78, temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "（", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp81, 0, "）", &D_800B203C, 0);
        } else if (strcmp(&sp7C, "そして…") == 0) {
            dialogue_render(-0x28, (s32) (s16) (temp_s1 + 0x10), (s32) arg6, (s32) &sp7C, 0, &D_800B204C, &D_800B2050, 0);
        } else if ((strcmp(&sp7C, "ガチャ") == 0) || (strcmp(&sp7C, "カチャ") == 0) || (strcmp(&sp7C, "ガラッ") == 0) || (strcmp(&sp7C, "ごそごそ") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 2, (s32) &sp7C, 0, &D_800B2078, &D_800B207C, 0);
        } else if ((strcmp(&sp7C, "ジロジロ") == 0) || (strcmp(&sp7C, "ドン") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 1, (s32) &sp7C, 0, &D_800B2094, &D_800B2098, 0);
        } else if (strcmp(&sp7C, "ピンポーン") == 0) {
            dialogue_render(-0x28, (s32) (s16) (temp_s1 + 0x10), 2, (s32) &sp7C, 0, &D_800B20A8, &D_800B20AC, 0);
        } else {
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B20B0, &D_800B20B4, 0);
        }
        D_80122CE4 = 1;
block_231:
        return 0;
    case 0x10:
        dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B20B8, &D_800B20BC, 0);
        D_80122D00 = 1;
        goto block_231;
    case 0x11:
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) &sp7C, 0, &D_800B20C0, &D_800B20C4, 0);
        D_80122D00 = 1;
        goto block_231;
    case 0x9:
        if (sp7C != 0) {
            if (strcmp(&sp7C, "ドン") == 0) {
                dialogue_render(-0x80, (s32) temp_s1, 1, (s32) &sp7C, 0, &D_800B20D0, &D_800B20D4, 0);
            }
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B20D8, &D_800B20DC, 0);
        }
        goto block_231;
    case 0x0:
        temp_v0 = strlen(&sp7C);
        sp286 = temp_v0;
        if ((*(u8 *)((unsigned char *)(&(&sp7C)[temp_v0]) + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)(&(&sp7C)[temp_v0]) + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(s8 *)((unsigned char *)(&(&sp7C)[sp286]) + (-2)) = 0;
                sp286 = strlen(D_800CA16C);
                sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA16C);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B20E8, &D_800B20EC, 0);
                D_80122CE4 = 1;
            }
        } else {
            sp286 = strlen(D_800CA16C);
            sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA16C);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B20F8, &D_800B20FC, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0xE:
        sp286 = strlen(D_800CA16C);
        sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, "）", &D_800B210C, 0);
        goto block_231;
    case 0x1:
        temp_v0_2 = strlen(&sp7C);
        sp286 = temp_v0_2;
        temp_t0_2 = arg3 + (arg4 * 4);
        temp_t7 = arg5 * 4;
        temp_v1_3 = *(*temp_t0_2 + temp_t7);
        if ((temp_v1_3->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_3 + (4)) != 0)) {
            temp_v1_4 = &(&sp7C)[temp_v0_2];
            if ((D_800E6280.unk0 != 0) && ((*(u8 *)((unsigned char *)temp_v1_4 + (-1)) != 0x6E) || (*(u8 *)((unsigned char *)temp_v1_4 + (-2)) != 0x6E) || (D_80122CDC == 0))) {
                if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                    temp_v1_5 = *(*temp_t0_2 + temp_t7);
                    func_80046290(temp_v1_5->unk0, *(s32 *)((unsigned char *)temp_v1_5 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {

                        } else {
                            goto block_71;
                        }
                    } else {
block_71:
                        D_80122CF0 = 1;
                        D_800CA2AC = 0;
                        goto block_72;
                    }
                    goto block_231;
                }
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
        }
block_72:
        temp_v0_3 = &(&sp7C)[sp286];
        if ((*(u8 *)((unsigned char *)temp_v0_3 + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)temp_v0_3 + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(u8 *)((unsigned char *)temp_v0_3 + (-2)) = 0U;
                sp286 = strlen(D_800CA188);
                sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2118, &D_800B211C, 0);
                D_80122CE4 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2128, &D_800B212C, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0xF:
        temp_t0_3 = arg3 + (arg4 * 4);
        temp_t9 = arg5 * 4;
        temp_v1_6 = *(*temp_t0_3 + temp_t9);
        if (((temp_v1_6->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_6 + (4)) != 0)) && (D_800E6280.unk0 != 0)) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v1_7 = *(*temp_t0_3 + temp_t9);
                func_80046290(temp_v1_7->unk0, *(s32 *)((unsigned char *)temp_v1_7 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {

                    } else {
                        goto block_90;
                    }
                } else {
block_90:
                    D_80122CF0 = 1;
                    D_800CA2AC = 0;
                    goto block_91;
                }
                goto block_231;
            }
            D_800CA2AC = 0;
            func_8007B640();
            func_80044750(0x74);
            return 0x64;
        }
block_91:
        sp286 = strlen(D_800CA188);
        sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2138, &D_800B213C, 0);
        D_80122D18 = 1;
        goto block_231;
    case 0xC:
        D_800CA2C4 = 0;
        sp286 = strlen(&sp7C);
        temp_t0_4 = arg3 + (arg4 * 4);
        temp_t7_2 = arg5 * 4;
        temp_v1_8 = *(*temp_t0_4 + temp_t7_2);
        if (((temp_v1_8->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_8 + (4)) != 0)) && (D_800E6280.unk0 != 0)) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v1_9 = *(*temp_t0_4 + temp_t7_2);
                func_80046290(temp_v1_9->unk0, *(s32 *)((unsigned char *)temp_v1_9 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {

                    } else {
                        goto block_105;
                    }
                } else {
block_105:
                    D_80122CF0 = 1;
                    D_800CA2AC = 0;
                    goto block_106;
                }
                goto block_231;
            }
            D_800CA2AC = 0;
            func_8007B640();
            func_80044750(0x74);
            return 0x64;
        }
block_106:
        temp_v0_4 = &(&sp7C)[sp286];
        if ((*(u8 *)((unsigned char *)temp_v0_4 + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)temp_v0_4 + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(u8 *)((unsigned char *)temp_v0_4 + (-2)) = 0U;
                sp286 = strlen(D_800CA188);
                sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2148, &D_800B214C, 0);
                D_80122D00 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2158, &D_800B215C, 0);
            D_80122D00 = 1;
        }
        goto block_231;
    case 0xD:
        sp286 = strlen(&sp7C);
        temp_t0_5 = arg3 + (arg4 * 4);
        temp_t7_3 = arg5 * 4;
        temp_v1_10 = *(*temp_t0_5 + temp_t7_3);
        if (((temp_v1_10->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_10 + (4)) != 0)) && (D_800E6280.unk0 != 0) && (D_800CA2C4 == 0)) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v1_11 = *(*temp_t0_5 + temp_t7_3);
                func_80046290(temp_v1_11->unk0, *(s32 *)((unsigned char *)temp_v1_11 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {

                    } else {
                        goto block_125;
                    }
                } else {
block_125:
                    D_80122CF0 = 1;
                    D_800CA2AC = 0;
                    goto block_126;
                }
                goto block_231;
            }
            D_800CA2AC = 0;
            func_8007B640();
            func_80044750(0x74);
            return 0x64;
        }
block_126:
        temp_v0_5 = &(&sp7C)[sp286];
        if ((*(u8 *)((unsigned char *)temp_v0_5 + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)temp_v0_5 + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(u8 *)((unsigned char *)temp_v0_5 + (-2)) = 0U;
                dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) &sp7C, 0, &D_800B2160, &D_800B2164, 0);
                D_80122CE4 = 1;
            }
        } else {
            set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
            dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) &sp7C, 0, &D_800B216C, &D_800B2170, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0x12:
        strlen(&sp7C);
        temp_t0_6 = arg3 + (arg4 * 4);
        temp_t8_2 = arg5 * 4;
        temp_v1_12 = *(*temp_t0_6 + temp_t8_2);
        if ((temp_v1_12->unk0 != 0) || (var_a1 = temp_s1 << 0x10, (*(s32 *)((unsigned char *)temp_v1_12 + (4)) != 0))) {
            var_a1 = temp_s1 << 0x10;
            if (D_800E6280.unk0 != 0) {
                var_a1 = temp_s1 << 0x10;
                if (D_800CA2C4 == 0) {
                    if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                        temp_v1_13 = *(*temp_t0_6 + temp_t8_2);
                        func_80046290(temp_v1_13->unk0, *(s32 *)((unsigned char *)temp_v1_13 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                        func_80044750(0x300);
                    }
                    if (D_800CA2AC < 0x400) {
                        if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                            D_800CA2A8 = 1;
                            func_80081128();
                            if (D_800CA2A8 == 1) {

                            } else {
                                goto block_145;
                            }
                        } else {
block_145:
                            D_80122CF0 = 1;
                            D_800CA2AC = 0;
                            var_a1 = temp_s1 << 0x10;
                            goto block_146;
                        }
                        goto block_231;
                    }
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
            }
        }
block_146:
        set_kanji_string(-0x56, (s16) (var_a1 >> 0x10), arg6, "　 ", 0);
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) &sp7C, 0, &D_800B2178, &D_800B217C, 0);
        D_80122D00 = 1;
        goto block_231;
    case 0xB:
        sp286 = strlen(&sp7C);
        temp_t0_7 = arg3 + (arg4 * 4);
        temp_t6 = arg5 * 4;
        temp_v1_14 = *(*temp_t0_7 + temp_t6);
        if ((temp_v1_14->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_14 + (4)) != 0)) {
            if (D_800E6280.unk0 != 0) {
                if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                    temp_v1_15 = *(*temp_t0_7 + temp_t6);
                    func_80046290(temp_v1_15->unk0, *(s32 *)((unsigned char *)temp_v1_15 + (4)), 0);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {

                        } else {
                            goto block_160;
                        }
                    } else {
block_160:
                        D_80122CF0 = 1;
                        D_800CA2AC = 0;
                        goto block_161;
                    }
                    goto block_231;
                }
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            goto block_162;
        }
block_161:
block_162:
        temp_v0_6 = &(&sp7C)[sp286];
        if ((*(u8 *)((unsigned char *)temp_v0_6 + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)temp_v0_6 + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(u8 *)((unsigned char *)temp_v0_6 + (-2)) = 0U;
                set_kanji_string((s16) (-0x68 - (strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B2194, &D_800B2198, 0);
                D_80122CE4 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - (strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B21B0, &D_800B21B4, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0xA:
        sp286 = strlen(&sp7C);
        temp_t0_8 = arg3 + (arg4 * 4);
        temp_t7_4 = arg5 * 4;
        temp_v1_16 = *(*temp_t0_8 + temp_t7_4);
        if ((temp_v1_16->unk0 != 0) || (*(s32 *)((unsigned char *)temp_v1_16 + (4)) != 0)) {
            if (D_800E6280.unk0 != 0) {
                if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                    temp_v1_17 = *(*temp_t0_8 + temp_t7_4);
                    func_80046290(temp_v1_17->unk0, *(s32 *)((unsigned char *)temp_v1_17 + (4)), 0xD);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {

                        } else {
                            goto block_180;
                        }
                    } else {
block_180:
                        D_80122CF0 = 1;
                        D_800CA2AC = 0;
                        goto block_181;
                    }
                    goto block_231;
                }
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            goto block_182;
        }
block_181:
block_182:
        temp_v0_7 = &(&sp7C)[sp286];
        if ((*(u8 *)((unsigned char *)temp_v0_7 + (-1)) == 0x6E) && (*(u8 *)((unsigned char *)temp_v0_7 + (-2)) == 0x6E)) {
            if (D_80122CDC == 0) {
                *(u8 *)((unsigned char *)temp_v0_7 + (-2)) = 0U;
                set_kanji_string((s16) (-0x68 - (strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B21CC, &D_800B21D0, 0);
                D_80122CE4 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - (strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, &D_800B21E8, &D_800B21EC, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0x4:
    case 0x6:
        sp286 = strlen(D_800CA16C);
        sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, "）", &D_800B21FC, 0);
        D_80122CE4 = 1;
        goto block_231;
    case 0x3:
        if (arg2 != 0xFF) {
            junban_init();
            make_three_select((s32) &sp7C);
        } else {
            D_800CA2A4->unk0 = 0;
            D_800CA2A4->unk0 = 1;
            D_800CA2A4->unk0 = 2;
        }
        dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, "』", "『", 1);
        k_disp_start(M2C_ERROR(/* Read from unset register $v0 */));
        goto block_231;
    case 0x8:
        D_800CA2A4->unk0 = 0;
        D_800CA2A4[1] = 1;
        dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, "』", "『", 1);
        k_disp_start(M2C_ERROR(/* Read from unset register $v0 */));
        goto block_231;
    case 0x13:
        D_800CA2A4->unk0 = 0;
        D_800CA2A4[1] = 1;
        dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) &sp7C, 0, "）", "（", 1);
        k_disp_start(M2C_ERROR(/* Read from unset register $v0 */));
        goto block_231;
    case 0x2:
    case 0x5:
        temp_t0_9 = arg3 + (arg4 * 4);
        temp_t9_2 = arg5 * 4;
        temp_v1_18 = D_80122CDC * 8;
        if ((*(*(*temp_t0_9 + temp_t9_2) + temp_v1_18) != 0) && ((u8) D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_8 = *(*temp_t0_9 + temp_t9_2) + temp_v1_18;
                func_80046290(temp_v0_8->unk0, *(s32 *)((unsigned char *)temp_v0_8 + (4)), (s32) *(sp58.value + D_800E6280.unk_F5F));
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {

                    } else {
                        goto block_206;
                    }
                } else {
block_206:
                    D_80122CF0 = 1;
                    D_800CA2AC = 0;
                    goto block_207;
                }
                goto block_231;
            }
            D_800CA2AC = 0;
            func_8007B640();
            func_80044750(0x74);
            return 0x64;
        }
block_207:
        sp286 = 0;
        var_a1_2 = 0;
        var_v1 = 0;
loop_208:
        temp_a0 = &(&sp7C)[var_v1];
        temp_v0_9 = *temp_a0;
        if (temp_v0_9 == 0x20) {
block_220:
            var_v1 -= 1;
block_226:
            var_v1 += 2;
            goto loop_208;
        }
        if (temp_v0_9 == 0x6D) {
            goto block_220;
        }
        if (temp_v0_9 == 0x4D) {
            goto block_220;
        }
        if (temp_v0_9 == 0x41) {
            goto block_220;
        }
        if (temp_v0_9 == 0x57) {
            goto block_220;
        }
        if (temp_v0_9 == 0x4F) {
            goto block_220;
        }
        if (temp_v0_9 == 0x46) {
            goto block_220;
        }
        if (temp_v0_9 == 0x59) {
            goto block_220;
        }
        if (temp_v0_9 == 0x70) {
            goto block_220;
        }
        if (temp_v0_9 == 0x47) {
            goto block_220;
        }
        if (temp_v0_9 == 0x50) {
            goto block_220;
        }
        if (temp_v0_9 == 0x53) {
            goto block_220;
        }
        if ((temp_v0_9 != 0x71) && (temp_v0_9 != 0)) {
            goto block_226;
        }
        *temp_a0 = 0;
        var_a1_2 += 1;
        temp_v0_10 = &(&sp7C)[sp286];
        if (arg2 >= (u32) var_a1_2) {
            sp286 = var_v1 + 1;
            var_v1 -= 1;
            goto block_226;
        }
        if (*temp_v0_10 != 0x6E) {
            sp27C = temp_v0_10;
            sp286 = strlen(D_800CA188);
            sprintf((s32) &sp68, (s32) "%s ", (s32) D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, &sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp27C, 0, &D_800B2220, &D_800B2224, 0);
            D_80122CE4 = 1;
        }
        goto block_231;
    case 0xFF:
        func_8004284C();
        goto block_231;
    default:
        func_80046500();
        goto block_231;
    }
}
