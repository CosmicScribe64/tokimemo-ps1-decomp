s32 func_801103A0(void) {
    u8 sp2B;
    u8 sp29;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_at;

    temp_v0 = func_8002328C(D_800B1746);
    sp2B = temp_v0 & 0x7F;
    sp29 = temp_v0 & 0x80;
    temp_v0_2 = func_8002336C(D_800B1746);
    if (D_800B1746 == 0) {
        if ((D_801243B0 == 1) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E2A == 0) && (D_800B1AF6 == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            D_800B0E2A = 1;
            return 0;
        }
        if ((D_801243B0 == 0x10) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (D_800B0E2B == 0) && (((EventStateView *)&D_800B0860)->unk_1A4[D_800B1746].pad2E[2] != 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            D_800B0E2B = 1;
            return 0;
        }
        if ((D_801243B0 == 0) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if (sp2B < 2U) {
                if (temp_v0_2 == 0) {
                    if ((D_800B0E2C == 0) && (D_800B1AF5 == 1)) {
                        if (D_800B1AF6 == 1) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            D_800B0E2C = 1;
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
        if ((D_801243B0 == 0xC) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (sp2B == 0) && (temp_v0_2 == 0) && (sp29 != 0) && (D_800B0E2D == 0) && (D_800EECBC == 2) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0x23)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E2D = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 1) {
        if ((D_801243B0 == 0xC) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && ((u8) D_800B0E31 == 0) && (D_800EECBC == 0) && (D_80125304 != 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0x23)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U) && (D_800B0E32 == 0)) {
            if (sp2B < 2U) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    D_800B0E32 = 1;
                    return 0;
                }
                goto block_57;
            }
            goto block_58;
        }
block_57:
block_58:
        if ((D_801243B0 == 8) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if (D_800B0E33 != 0) {
                if (D_800B0E33 == 1) {
                    if (!((CharFlags *)&D_800B1AE4)->b0) {
                        goto block_64;
                    }
                    goto block_68;
                }
                goto block_67;
            }
block_64:
            if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
                D_800EECB0 = 4;
                func_80011E28(0x60);
                D_800B0E33 = 1;
                return 0;
            }
            goto block_67;
        }
block_67:
block_68:
        if ((D_801243B0 == 0xE) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E34 == 0) && (D_80125314 == 6) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0x11)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E34 = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 2) {
        if ((D_801243B0 == 4) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E37 == 0) && ((u8) D_800B0895 >= 0x60U) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E37 = 1;
            return 0;
        }
        if ((D_801243B0 == 0xA) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U) && (D_800B0E38 == 0) && (D_800B0895 == 0x61) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            D_800B0E38 = 1;
            return 0;
        }
        if ((D_801243B0 == 9) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU) && (D_800B0E39 == 0) && ((u8) D_800B0895 >= 0x60U) && (D_80125300 != 0) && (D_800EECBC == 1) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xD)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            D_800B0E39 = 1;
            return 0;
        }
        if ((D_801243B0 == 2) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_80125324 == 2) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E3A = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 3) {
        if ((D_801243B0 == 0xB) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E3D == 0) && (D_800B0895 == 0x61) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E3D = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U)) {
            if (D_800B0E3E == 0) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    D_800B0E3E = 1;
                    return 0;
                }
                goto block_128;
            }
            goto block_129;
        }
block_128:
block_129:
        if ((D_801243B0 == 7) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU) && (D_800B0E3F == 0)) {
            if ((D_801252F4 == 1) && (D_800B1AF5 == 1)) {
                if (D_800B1AF6 == 7) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    D_800B0E3F = 1;
                    return 0;
                }
                goto block_137;
            }
            goto block_138;
        }
block_137:
block_138:
        if ((D_801243B0 == 6) && ((var_at = sp2B < 2U, (D_800B0896 == 0xC)) || (var_at = sp2B < 2U, (D_800B0896 == 1)) || (var_at = sp2B < 2U, (D_800B0896 == 2))) && (var_at != 0) && (temp_v0_2 == 0) && (D_800B0E40 == 0) && ((u8) D_800B0895 >= 0x61U) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xB)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E40 = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 4) {
        if ((D_801243B0 == 1) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B0E43 == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E43 = 1;
            return 0;
        }
        if ((D_801243B0 == 0x10) && ((u8) D_800B0896 >= 6U)) {
            if (((u8) D_800B0896 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (D_800B0E44 == 0) {
                        if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            D_800B0E44 = 1;
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
        if ((D_801243B0 == 0xD) && ((u8) D_8012532C == 0) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU) && (D_800B0E45 == 0) && ((u8) D_8012531C < 2U) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            D_800B0E45 = 1;
            return 0;
        }
        if ((D_801243B0 == 2) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E46 == 0) && (D_800EECBC == 1) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xE)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E46 = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 5) {
        if ((D_801243B0 == 5) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E4A == 0) && (D_800B0895 == 0x61) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            D_800B0E4A = 1;
            return 0;
        }
        if ((D_801243B0 == 3) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U) && (sp2B < 3U) && ((u32) temp_v0_2 < 2U) && (D_800B0E4B == 0) && ((u8) D_800B0895 >= 0x61U) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 4;
            func_80011E28(0x60);
            D_800B0E4B = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if ((D_800B0E4C == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xD)) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    D_800B0E4C = 1;
                    return 0;
                }
                goto block_218;
            }
            goto block_219;
        }
block_218:
block_219:
        if ((D_801243B0 == 0x14) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E4D == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            D_800B0E4D = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 6) {
        if ((D_801243B0 == 7) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E50 == 0) && (D_801252F8 != 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 7)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E50 = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) D_800B0896 >= 6U) && ((u8) D_800B0896 < 9U) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B0E51 == 0) && (D_800B095E >= 0x64) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            D_800B0E51 = 1;
            return 0;
        }
        if ((D_801243B0 == 5) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if (D_800B0E52 == 0) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    D_800B0E52 = 1;
                    return 0;
                }
                goto block_254;
            }
            goto block_255;
        }
block_254:
block_255:
        if ((D_801243B0 == 0xA) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E53 == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E53 = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 7) {
        if ((D_801243B0 == 2) && (D_800EECBC == 0) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E56 == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xF)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E56 = 1;
            return 0;
        }
        if ((D_801243B0 == 0x11) && ((u8) D_800B0896 >= 6U)) {
            if ((u8) D_800B0896 < 9U) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0xA)) {
                    D_800EECB0 = 3;
                    func_80011E28(0x60);
                    D_800B0E57 = 1;
                    return 0;
                }
                goto block_278;
            }
            goto block_279;
        }
block_278:
block_279:
        if ((D_801243B0 == 0xC) && (D_80125320 == 1) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if ((D_800B0E58 == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0)) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0x38)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    D_800B0E58 = 1;
                    return 0;
                }
                goto block_290;
            }
            goto block_291;
        }
block_290:
block_291:
        if ((D_801243B0 == 0x14) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E59 == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E59 = 1;
            return 0;
        }
        if ((D_800B0E5A == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B1AF5 == 0) && (D_800B1AF6 == 0x29)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            D_800B0E5A = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 8) {
        if ((D_801243B0 == 9) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E5D == 0) && ((D_80125300 == 0) || (D_800EECBC == 0)) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0xD)) {
            D_800EECB0 = 2;
            func_80011E28(0x60);
            D_800B0E5D = 1;
            return 0;
        }
        if ((D_801243B0 == 0xC) && ((u8) D_80125310 == 1) && ((u8) D_800B0896 >= 6U)) {
            if (((u8) D_800B0896 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (D_800B0E5E == 0) {
                        if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0x3C)) {
                            D_800EECB0 = 3;
                            func_80011E28(0x60);
                            D_800B0E5E = 1;
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
        if ((D_801243B0 == 0xF) && (D_80125318 == 0xA) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if (D_800B0E5F == 0) {
                if ((D_800B1AF5 == 1) && (D_800B1AF6 == 0x11)) {
                    D_800EECB0 = 4;
                    func_80011E28(0x60);
                    D_800B0E5F = 1;
                    return 0;
                }
                goto block_339;
            }
            goto block_340;
        }
block_339:
block_340:
        if ((D_801243B0 == 0x13) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E60 == 0) && (((EventStateView *)&D_800B0860)->unk_1A4[D_800B1746].pad2E[4] != 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 5;
            func_80011E28(0x60);
            D_800B0E60 = 1;
            return 0;
        }
        if (D_801243B0 == 8) {
            if (!((D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 10)) ||
                  (D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 8)) ||
                  (D_800B0895 = (D_800B0896 == 12 && D_800B0897 == 7)))) {
                goto block_448;
            }
            if ((sp2B < 3U) && ((EventStateView *)&D_800B0860)->unk_0E4.f.b1 && (D_800B1AF5 == 1) && (D_800B1AF6 == 0)) {
                D_800EECB0 = 6;
                func_80011E28(0x60);
                D_800B0E61 = 1;
                return 0;
            }
            goto block_448;
        }
        goto block_448;
    }
    if (D_800B1746 == 9) {
        if ((D_801243B0 == 0xC) && (D_80125328 != 0) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E64 == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 0x1B)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            D_800B0E64 = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) D_800B0896 >= 6U)) {
            if (((u8) D_800B0896 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if (D_800B0E65 == 0) {
                        if ((D_800B1AF5 == 0) && (D_800B1AF6 == 0x29)) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            D_800B0E65 = 1;
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
        if ((D_801243B0 == 4) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU)) {
            if ((D_800B0E66 == 0) && (D_800B1AF5 == 1)) {
                if (D_800B1AF6 == 9) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    D_800B0E66 = 1;
                    return 0;
                }
                goto block_391;
            }
            goto block_392;
        }
block_391:
block_392:
        if ((D_801243B0 == 0x13) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (D_800B0E67 == 0) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B095E >= 0x50) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            D_800B0E67 = 1;
            return 0;
        }
        goto block_448;
    }
    if (D_800B1746 == 0xA) {
        if ((D_801243B0 == 0xC) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && (D_800B0E6B == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 3;
            func_80011E28(0x60);
            D_800B0E6B = 1;
            return 0;
        }
        if ((D_801243B0 == 1) && ((u8) D_800B0896 >= 6U)) {
            if (((u8) D_800B0896 < 9U) && (sp2B == 0) && (sp29 != 0)) {
                if (temp_v0_2 == 0) {
                    if ((D_800B0E6C == 0) && (D_800B1AF5 == 1)) {
                        if (D_800B1AF6 == 1) {
                            D_800EECB0 = 4;
                            func_80011E28(0x60);
                            D_800B0E6C = 1;
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
        if ((D_801243B0 == 0xD) && ((u8) D_800B0896 >= 9U) && ((u8) D_800B0896 < 0xCU) && (D_800B0E6D == 0)) {
            if (((u8) D_8012531C >= 2U) && (D_800B1AF5 == 1)) {
                if (D_800B1AF6 == 8) {
                    D_800EECB0 = 5;
                    func_80011E28(0x60);
                    D_800B0E6D = 1;
                    return 0;
                }
                goto block_431;
            }
            goto block_432;
        }
block_431:
block_432:
        if ((D_801243B0 == 0x14) && ((D_800B0896 == 0xC) || (D_800B0896 == 1) || (D_800B0896 == 2)) && (sp2B == 0) && (sp29 != 0) && (temp_v0_2 == 0) && (D_800B0E6E == 0) && (D_800B1AF5 == 1) && (D_800B1AF6 == 8)) {
            D_800EECB0 = 6;
            func_80011E28(0x60);
            D_800B0E6E = 1;
            return 0;
        }
        goto block_448;
    }
    if ((D_801243B0 == 0xB) && (((EventStateView *)&D_800B0860)->unk_1A4[1].pad23[3] == 2) && (D_800B1AF5 == 1) && (D_800B1AF6 == 9)) {
        func_80011E28(0xD1);
        return 0;
    }
block_448:
    if (((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && ((u8) D_800B0E70 >= 2U)) {
        if (D_800B0E72 == 0) {
            if ((D_800B1AF5 == 0) && (D_800B1AF6 == 0x29)) {
                D_800EECB0 = 3;
                D_800B0F28 = 0x50;
                D_800B0F29 = 0;
                D_800B0F2A = 0xC;
                D_800EECB4 = 0xB;
                func_80011E28(0x60);
                D_800B0E72 = 1;
                return 0;
            }
            goto block_455;
        }
        goto block_456;
    }
block_455:
block_456:
    if ((D_801243B0 == 0x11) && ((u8) D_800B0896 >= 3U) && ((u8) D_800B0896 < 6U) && ((u8) D_800B0E70 >= 2U) && (D_800B0E74 == 0) && (D_800B1AF5 == 0) && (D_800B1AF6 == 0x29)) {
        D_800EECB0 = 5;
        D_800B0F28 = 0x50;
        D_800B0F29 = 0;
        D_800B0F2A = 0xC;
        D_800EECB4 = 0xB;
        func_80011E28(0x60);
        D_800B0E74 = 1;
        return 0;
    }
    func_80011DFC();
    return 1;
}
