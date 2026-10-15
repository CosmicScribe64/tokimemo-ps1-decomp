s32 func_801103A0(void) {
    EventStateView *g;
    u8 sp2B;
    u8 sp29;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_at;

    g = (EventStateView *)&D_800B0860;
    temp_v0 = func_8002328C(g->unk_EE6);
    sp2B = temp_v0 & 0x7F;
    sp29 = temp_v0 & 0x80;
    temp_v0_2 = func_8002336C(g->unk_EE6);
    if (g->unk_EE6 == 0) {
        if ((D_801243B0 == 1) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x0] == 0) && (g->unk_1296 == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->unk_5CA[0x0] = 1;
            return 0;
        }
        if ((D_801243B0 == 0x10) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (g->unk_5CA[0x1] == 0) && (g->unk_1A4[g->unk_EE6].pad2E[2] != 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->unk_5CA[0x1] = 1;
            return 0;
        }
        if ((D_801243B0 == 0) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if (sp2B < 2U) {
                if (temp_v0_2 == 0) {
                    if ((g->unk_5CA[0x2] == 0) && (g->unk_1295 == 1)) {
                        if (g->unk_1296 == 1) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->unk_5CA[0x2] = 1;
                            return 0;
                        }
                        goto block_26;
                    }
                    goto block_27;
                }
                goto block_26;
            }
            goto block_27;
        }
block_26:
block_27:
        if ((D_801243B0 == 0xC) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (sp2B == 0) && (temp_v0_2 == 0) && (sp29 != 0) && (g->unk_5CA[0x3] == 0) && (D_800EECBC == 2) && (g->unk_1295 == 1) && (g->unk_1296 == 0x23)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x3] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 1) {
        if ((D_801243B0 == 0xC) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && ((u8) g->unk_5CA[0x7] == 0) && (D_800EECBC == 0) && (D_80125304 != 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0x23)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U) && (g->unk_5CA[0x8] == 0)) {
            if (sp2B < 2U) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->unk_5CA[0x8] = 1;
                    return 0;
                }
                goto block_57;
            }
            goto block_58;
        }
block_57:
block_58:
        if ((D_801243B0 == 8) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if (g->unk_5CA[0x9] != 0) {
                if (g->unk_5CA[0x9] == 1) {
                    if (!((CharFlags *)&D_800B1AE4)->b0) {
                        goto block_64;
                    }
                    goto block_68;
                }
                goto block_67;
            }
block_64:
            if ((g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
                D_800EECB0 = 4;
                func_80011E28(0x60);
                g->unk_5CA[0x9] = 1;
                return 0;
            }
            goto block_67;
        }
block_67:
block_68:
        if ((D_801243B0 == 0xE) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0xA] == 0) && (D_80125314 == 6) && (g->unk_1295 == 1) && (g->unk_1296 == 0x11)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0xA] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 2) {
        if ((D_801243B0 == 4) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0xD] == 0) && ((u8) g->unk_035 >= 0x60U) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0xD] = 1;
            return 0;
        }
        if ((D_801243B0 == 0xA) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U) && (g->unk_5CA[0xE] == 0) && (g->unk_035 == 0x61) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->unk_5CA[0xE] = 1;
            return 0;
        }
        if ((D_801243B0 == 9) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU) && (g->unk_5CA[0xF] == 0) && ((u8) g->unk_035 >= 0x60U) && (D_80125300 != 0) && (D_800EECBC == 1) && (g->unk_1295 == 1) && (g->unk_1296 == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->unk_5CA[0xF] = 1;
            return 0;
        }
        if ((D_801243B0 == 2) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (D_80125324 == 2) && (g->unk_1295 == 1) && (g->unk_1296 == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x10] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 3) {
        if ((D_801243B0 == 0xB) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x13] == 0) && (g->unk_035 == 0x61) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0x13] = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
            if (g->unk_5CA[0x14] == 0) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->unk_5CA[0x14] = 1;
                    return 0;
                }
                goto block_128;
            }
            goto block_129;
        }
block_128:
block_129:
        if ((D_801243B0 == 7) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU) && (g->unk_5CA[0x15] == 0)) {
            if ((D_801252F4 == 1) && (g->unk_1295 == 1)) {
                if (g->unk_1296 == 7) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->unk_5CA[0x15] = 1;
                    return 0;
                }
                goto block_137;
            }
            goto block_138;
        }
block_137:
block_138:
        if ((D_801243B0 == 6) && ((var_at = sp2B < 2U, (g->unk_036 == 0xC)) || (var_at = sp2B < 2U, (g->unk_036 == 1)) || (var_at = sp2B < 2U, (g->unk_036 == 2))) && (var_at != 0) && (temp_v0_2 == 0) && (g->unk_5CA[0x16] == 0) && ((u8) g->unk_035 >= 0x61U) && (g->unk_1295 == 1) && (g->unk_1296 == 0xB)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x16] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 4) {
        if ((D_801243B0 == 1) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_5CA[0x19] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0x19] = 1;
            return 0;
        }
        if ((D_801243B0 == 0x10) && ((u8) g->unk_036 >= 6U)) {
            if (((u8) g->unk_036 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->unk_5CA[0x1A] == 0) {
                        if ((g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->unk_5CA[0x1A] = 1;
                            return 0;
                        }
                        goto block_170;
                    }
                    goto block_171;
                }
                goto block_170;
            }
            goto block_171;
        }
block_170:
block_171:
        if ((D_801243B0 == 0xD) && ((u8) D_8012532C == 0) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU) && (g->unk_5CA[0x1B] == 0) && ((u8) D_8012531C < 2U) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->unk_5CA[0x1B] = 1;
            return 0;
        }
        if ((D_801243B0 == 2) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x1C] == 0) && (D_800EECBC == 1) && (g->unk_1295 == 1) && (g->unk_1296 == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x1C] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 5) {
        if ((D_801243B0 == 5) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x20] == 0) && (g->unk_035 == 0x61) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->unk_5CA[0x20] = 1;
            return 0;
        }
        if ((D_801243B0 == 3) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (g->unk_5CA[0x21] == 0) && ((u8) g->unk_035 >= 0x61U) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            g->unk_5CA[0x21] = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if ((g->unk_5CA[0x22] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0xD)) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->unk_5CA[0x22] = 1;
                    return 0;
                }
                goto block_218;
            }
            goto block_219;
        }
block_218:
block_219:
        if ((D_801243B0 == 0x14) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x23] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->unk_5CA[0x23] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 6) {
        if ((D_801243B0 == 7) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x26] == 0) && (D_801252F8 != 0) && (g->unk_1295 == 1) && (g->unk_1296 == 7)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0x26] = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_5CA[0x27] == 0) && (g->unk_0FE >= 0x64) && (g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->unk_5CA[0x27] = 1;
            return 0;
        }
        if ((D_801243B0 == 5) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if (g->unk_5CA[0x28] == 0) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 9)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->unk_5CA[0x28] = 1;
                    return 0;
                }
                goto block_254;
            }
            goto block_255;
        }
block_254:
block_255:
        if ((D_801243B0 == 0xA) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x29] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x29] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 7) {
        if ((D_801243B0 == 2) && (D_800EECBC == 0) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x2C] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0xF)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0x2C] = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) g->unk_036 >= 6U)) {
            if ((u8) g->unk_036 < 9U) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    g->unk_5CA[0x2D] = 1;
                    return 0;
                }
                goto block_278;
            }
            goto block_279;
        }
block_278:
block_279:
        if ((D_801243B0 == 0xC) && (D_80125320 == 1) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if ((g->unk_5CA[0x2E] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0x38)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->unk_5CA[0x2E] = 1;
                    return 0;
                }
                goto block_290;
            }
            goto block_291;
        }
block_290:
block_291:
        if ((D_801243B0 == 0x14) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x2F] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x2F] = 1;
            return 0;
        }
        if ((g->unk_5CA[0x30] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_1295 == 0) && (g->unk_1296 == 0x29)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->unk_5CA[0x30] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 8) {
        if ((D_801243B0 == 9) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x33] == 0) && ((D_80125300 == 0) || (D_800EECBC == 0)) && (g->unk_1295 == 1) && (g->unk_1296 == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            g->unk_5CA[0x33] = 1;
            return 0;
        }
        if ((D_801243B0 == 0xC) && ((u8) D_80125310 == 1) && ((u8) g->unk_036 >= 6U)) {
            if (((u8) g->unk_036 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->unk_5CA[0x34] == 0) {
                        if ((g->unk_1295 == 1) && (g->unk_1296 == 0x3C)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            g->unk_5CA[0x34] = 1;
                            return 0;
                        }
                        goto block_330;
                    }
                    goto block_331;
                }
                goto block_330;
            }
            goto block_331;
        }
block_330:
block_331:
        if ((D_801243B0 == 0xF) && (D_80125318 == 0xA) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if (g->unk_5CA[0x35] == 0) {
                if ((g->unk_1295 == 1) && (g->unk_1296 == 0x11)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    g->unk_5CA[0x35] = 1;
                    return 0;
                }
                goto block_339;
            }
            goto block_340;
        }
block_339:
block_340:
        if ((D_801243B0 == 0x13) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x36] == 0) && (g->unk_1A4[g->unk_EE6].pad2E[4] != 0) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            g->unk_5CA[0x36] = 1;
            return 0;
        }
        if (D_801243B0 == 8) {
            if (!((g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 10)) ||
                  (g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 8)) ||
                  (g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 7)))) {
                goto block_448;
            }
            if ((sp2B < 3U) && g->unk_0E4.f.b1 && (g->unk_1295 == 1) && (g->unk_1296 == 0)) {
                D_800EECB0 = 6;
                func_80011E28(0x60);
                g->unk_5CA[0x37] = 1;
                return 0;
            }
            goto block_448;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 9) {
        if ((D_801243B0 == 0xC) && (D_80125328 != 0) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x3A] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 0x1B)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->unk_5CA[0x3A] = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) g->unk_036 >= 6U)) {
            if (((u8) g->unk_036 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (g->unk_5CA[0x3B] == 0) {
                        if ((g->unk_1295 == 0) && (g->unk_1296 == 0x29)) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            g->unk_5CA[0x3B] = 1;
                            return 0;
                        }
                        goto block_383;
                    }
                    goto block_384;
                }
                goto block_383;
            }
            goto block_384;
        }
block_383:
block_384:
        if ((D_801243B0 == 4) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            if ((g->unk_5CA[0x3C] == 0) && (g->unk_1295 == 1)) {
                if (g->unk_1296 == 9) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->unk_5CA[0x3C] = 1;
                    return 0;
                }
                goto block_391;
            }
            goto block_392;
        }
block_391:
block_392:
        if ((D_801243B0 == 0x13) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (g->unk_5CA[0x3D] == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_0FE >= 0x50) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->unk_5CA[0x3D] = 1;
            return 0;
        }
        goto block_448;
    }
    if (g->unk_EE6 == 0xA) {
        if ((D_801243B0 == 0xC) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && (g->unk_5CA[0x41] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            g->unk_5CA[0x41] = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) g->unk_036 >= 6U)) {
            if (((u8) g->unk_036 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if ((g->unk_5CA[0x42] == 0) && (g->unk_1295 == 1)) {
                        if (g->unk_1296 == 1) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            g->unk_5CA[0x42] = 1;
                            return 0;
                        }
                        goto block_422;
                    }
                    goto block_423;
                }
                goto block_422;
            }
            goto block_423;
        }
block_422:
block_423:
        if ((D_801243B0 == 0xD) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU) && (g->unk_5CA[0x43] == 0)) {
            if (((u8) D_8012531C >= 2U) && (g->unk_1295 == 1)) {
                if (g->unk_1296 == 8) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    g->unk_5CA[0x43] = 1;
                    return 0;
                }
                goto block_431;
            }
            goto block_432;
        }
block_431:
block_432:
        if ((D_801243B0 == 0x14) && ((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (g->unk_5CA[0x44] == 0) && (g->unk_1295 == 1) && (g->unk_1296 == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            g->unk_5CA[0x44] = 1;
            return 0;
        }
        goto block_448;
    }
    if ((D_801243B0 == 0xB) && (g->unk_1A4[1].pad23[3] == 2) && (g->unk_1295 == 1) && (g->unk_1296 == 9)) {
        func_80011E28(0xD1);
        return 0;
    }
block_448:
    if (((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && ((u8) g->unk_5CA[0x46] >= 2U)) {
        if (g->unk_5CA[0x48] == 0) {
            if ((g->unk_1295 == 0) && (g->unk_1296 == 0x29)) {
                D_800EECB0 = 3;
                D_800B0F28 = 0x50;
                D_800B0F29 = 0;
                D_800B0F2A = 0xC;
                D_800EECB4 = 0xB;
                func_80011E28(0x60);
                g->unk_5CA[0x48] = 1;
                return 0;
            }
            goto block_455;
        }
        goto block_456;
    }
block_455:
block_456:
    if ((D_801243B0 == 0x11) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U) && ((u8) g->unk_5CA[0x46] >= 2U) && (g->unk_5CA[0x4A] == 0) && (g->unk_1295 == 0) && (g->unk_1296 == 0x29)) {
        D_800EECB0 = 5;
        D_800B0F28 = 0x50;
        D_800B0F29 = 0;
        D_800B0F2A = 0xC;
        D_800EECB4 = 0xB;
        func_80011E28(0x60);
        g->unk_5CA[0x4A] = 1;
        return 0;
    }
    func_80011DFC();
    return 1;
}
