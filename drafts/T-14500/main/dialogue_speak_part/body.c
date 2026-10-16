s32 dialogue_speak_part(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
u8 *arg0;
u8 arg1;
u8 arg2;
DialogueVoice8 ***arg3;
u16 arg4;
u16 arg5;
u8 arg6;
{
    s16 temp_s1;
    s16 var_v1;
    DialogueWork *dialogue;
    s16 sp286;
    s16 var_a1_2;
    u8 *sp27C;
    u8 *p;
    u8 sp7C[0x200];
    u8 sp74[8];
    u8 sp68[12];
    DialogueBytes16 sp58;
    /* FAKE: observed 0x290-byte frame extent; T-10730. */
    s32 pad[1];

    dialogue = (DialogueWork *)D_80122CD0;
    sp58 = D_800CA36C;
    /* FAKE: line grouping preserves as1 scheduling; T-10730. */
    if (arg1 != 0xFF) { strcpy(sp7C, arg0); }
    dialogue->unk_20 = 0;
    D_800CA2A8 = 0;
    temp_s1 = (dialogue->unk_58 * 0x10) + 0x30;
    /* FAKE: byte view shares the 0xFF constant in v1; T-10730. */
    switch (*(u8 *)&arg1) {
    case 0x7:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        if (sp7C[0] == 0x3E) {
            strncpy(sp74, &sp7C[1], 4);
            sp74[4] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x78, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "", "", 0);
        } else if (sp7C[0] == 0x2D) {
            strncpy(sp74, &sp7C[1], 6);
            sp74[6] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x80, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[7], 0, "", "", 0);
        } else if (sp7C[0] == 0x2B) {
            strncpy(sp74, &sp7C[1], 2);
            sp74[2] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x70, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[3], 0, "", "", 0);
        } else if (sp7C[0] == 0x3C) {
            strncpy(sp74, &sp7C[1], 4);
            sp74[4] = 0;
            sprintf(sp68, "%s ", sp74);
            set_kanji_string(-0x78, temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "（", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) &sp7C[5], 0, "）", "", 0);
        } else if (strcmp(sp7C, "そして…") == 0) {
            temp_s1 += 0x10;
            dialogue_render(-0x28, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ガチャ") == 0) || (strcmp(sp7C, "カチャ") == 0) || (strcmp(sp7C, "ガラッ") == 0) || (strcmp(sp7C, "ごそごそ") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else if ((strcmp(sp7C, "ジロジロ") == 0) || (strcmp(sp7C, "ドン") == 0)) {
            dialogue_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
        } else if (strcmp(sp7C, "ピンポーン") == 0) {
            temp_s1 += 0x10;
            dialogue_render(-0x28, (s32) temp_s1, 2, (s32) sp7C, 0, "", "", 0);
        } else {
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        dialogue->unk_14 = 1;
        break;
    case 0x10:
        dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x11:
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0x9:
        if (sp7C[0] != 0) {
            if (strcmp(sp7C, "ドン") == 0) {
                dialogue_render(-0x80, (s32) temp_s1, 1, (s32) sp7C, 0, "", "", 0);
            }
            dialogue_render(-0x80, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        }
        break;
    case 0x0:
        sp286 = strlen(sp7C);
        if ((sp7C[sp286 - 1] == 0x6E) && (sp7C[sp286 - 2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                sp7C[sp286 - 2] = 0;
                sp286 = strlen(D_800CA16C);
                sprintf(sp68, "%s ", D_800CA16C);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(D_800CA16C);
            sprintf(sp68, "%s ", D_800CA16C);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "『 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xE:
        sp286 = strlen(D_800CA16C);
        sprintf(sp68, "%s ", D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
        break;
    case 0x1:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && ((*(u8 *)&D_800E6280.unk_03C != 0) && ((sp7C[sp286 - 1] != 0x6E) || (sp7C[sp286 - 2] != 0x6E) || (dialogue->unk_0C == 0)))) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(D_800CA188);
                sprintf(sp68, "%s ", D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xF:
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        sp286 = strlen(D_800CA188);
        sprintf(sp68, "%s ", D_800CA188);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_48 = 1;
        break;
    case 0xC:
        D_800CA2C4 = 0;
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                sp286 = strlen(D_800CA188);
                sprintf(sp68, "%s ", D_800CA188);
                set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_30 = 1;
            }
        } else {
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「 ", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_30 = 1;
        }
        break;
    case 0xD:
        sp286 = strlen(sp7C);
        if (((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) && (*(u8 *)&D_800E6280.unk_03C != 0) && (D_800CA2C4 == 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
            dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x12:
        strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {

                if (D_800CA2C4 == 0) {
                    if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                            func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, sp58.value[D_800E6280.unk_F5F]);
                        func_80044750(0x300);
                    }
                    if (D_800CA2AC < 0x400) {
                        if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                            D_800CA2A8 = 1;
                            func_80081128();
                            if (D_800CA2A8 == 1) {
                                return 0;
                            }
                        }
                    } else {
                        D_800CA2AC = 0;
                        func_8007B640();
                        func_80044750(0x74);
                        return 0x64;
                    }
                    dialogue->unk_20 = 1;
                    D_800CA2AC = 0;
                }
            }
        }
        set_kanji_string(-0x56, temp_s1, arg6, "　 ", 0);
        dialogue_render(-0x48, (s32) D_800CA2C0, (s32) arg6, (s32) sp7C, 0, "", "", 0);
        dialogue->unk_30 = 1;
        break;
    case 0xB:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
            }
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                set_kanji_string((s16) (-0x68 - ((s16) strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("藤崎") * 4)), temp_s1, arg6, "藤崎", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xA:
        sp286 = strlen(sp7C);
        if ((arg3[arg4][arg5]->unk_00 != 0) || (arg3[arg4][arg5]->unk_04 != 0)) { /* FAKE: same-line branches preserve scheduling, T-10730. */ if (*(u8 *)&D_800E6280.unk_03C != 0) {
                if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                    func_80046290(arg3[arg4][arg5]->unk_00, arg3[arg4][arg5]->unk_04, 0xD);
                    func_80044750(0x300);
                }
                if (D_800CA2AC < 0x400) {
                    if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                        D_800CA2A8 = 1;
                        func_80081128();
                        if (D_800CA2A8 == 1) {
                            return 0;
                        }
                    }
                } else {
                    D_800CA2AC = 0;
                    func_8007B640();
                    func_80044750(0x74);
                    return 0x64;
                }
                dialogue->unk_20 = 1;
                D_800CA2AC = 0;
            }
        }
        p = &sp7C[sp286];
        if ((p[-1] == 0x6E) && (p[-2] == 0x6E)) {
            if (dialogue->unk_0C == 0) {
                p[-2] = 0U;
                set_kanji_string((s16) (-0x68 - ((s16) strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
                set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
                dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
                dialogue->unk_14 = 1;
            }
        } else {
            set_kanji_string((s16) (-0x68 - ((s16) strlen("好雄") * 4)), temp_s1, arg6, "好雄", 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0x4:
    case 0x6:
        sp286 = strlen(D_800CA16C);
        sprintf(sp68, "%s ", D_800CA16C);
        set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
        set_kanji_string(-0x56, temp_s1, arg6, "（ ", 0);
        dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "", 0);
        dialogue->unk_14 = 1;
        break;
    case 0x3:
        if (arg2 != 0xFF) {
            junban_init();
            make_three_select(sp7C);
        } else {
            D_800CA2A4[0] = 0;
            D_800CA2A4[1] = 1;
            D_800CA2A4[2] = 2;
        }
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x8:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "』", "『", 1));
        break;
    case 0x13:
        D_800CA2A4[0] = 0;
        D_800CA2A4[1] = 1;
        k_disp_start(dialogue_render(-0x72, (s32) temp_s1, (s32) arg6, (s32) sp7C, 0, "）", "（", 1));
        break;
    case 0x2:
    case 0x5:
        if ((((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2] != 0) && (*(u8 *)&D_800E6280.unk_03C != 0)) {
            if ((D_800CA2AC++ == 0) && (dialogue->unk_14 == 0)) {
                func_80046290(((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2], ((s32 *)arg3[arg4][arg5])[dialogue->unk_0C * 2 + 1], sp58.value[D_800E6280.unk_F5F]);
                func_80044750(0x300);
            }
            if (D_800CA2AC < 0x400) {
                if (!(func_800460EC() & 4) && dialogue->unk_14 == 0) {
                    D_800CA2A8 = 1;
                    func_80081128();
                    if (D_800CA2A8 == 1) {
                        return 0;
                    }
                }
            } else {
                D_800CA2AC = 0;
                func_8007B640();
                func_80044750(0x74);
                return 0x64;
            }
            dialogue->unk_20 = 1;
            D_800CA2AC = 0;
        }
        sp286 = 0;
        var_a1_2 = 0;
        for (var_v1 = 0; ; var_v1 += 2) {
            if (sp7C[var_v1] == 0x20 || sp7C[var_v1] == 0x6D ||
                sp7C[var_v1] == 0x4D || sp7C[var_v1] == 0x41 ||
                sp7C[var_v1] == 0x57 || sp7C[var_v1] == 0x4F ||
                sp7C[var_v1] == 0x46 || sp7C[var_v1] == 0x59 ||
                sp7C[var_v1] == 0x70 || sp7C[var_v1] == 0x47 ||
                sp7C[var_v1] == 0x50 || sp7C[var_v1] == 0x53) {
                var_v1--;
            } else if (sp7C[var_v1] == 0x71 || sp7C[var_v1] == 0) {
                sp7C[var_v1] = 0;
                p = &sp7C[sp286];
                var_a1_2++;
                if (arg2 < var_a1_2) {
                    break;
                }
                sp286 = var_v1 + 1;
                var_v1--;
            }
        }
        if (*p != 0x6E) {
            sp27C = p;
            sp286 = strlen(D_800CA188);
            sprintf(sp68, "%s ", D_800CA188);
            set_kanji_string((s16) (-0x68 - (sp286 * 4)), temp_s1, arg6, sp68, 0);
            set_kanji_string(-0x56, temp_s1, arg6, "「", 0);
            dialogue_render(-0x48, (s32) temp_s1, (s32) arg6, (s32) sp27C, 0, "", "", 0);
            dialogue->unk_14 = 1;
        }
        break;
    case 0xFF:
        func_8004284C();
        break;
    default:
        func_80046500();
        break;
    }
    return 0;
}
