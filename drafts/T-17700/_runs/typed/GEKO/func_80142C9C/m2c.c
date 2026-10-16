extern ? D_800CA280;
extern ? D_801478D4;

void func_80142C9C(void) {
    u8 sp553;
    u8 sp552;
    u8 sp551;
    u8 sp550;
    u8 sp529;
    s16 sp526;
    ? sp454;
    ? sp43C;
    s16 sp422;
    ? sp39C;
    ? sp318;
    ? sp300;
    ? sp2E8;
    ? sp74;
    ? sp4C;
    ? sp41;
    ? sp34;
    ? *var_t8_2;
    GameState *var_t7;
    s16 var_t8;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 temp_t7;
    s32 var_a3;
    s32 var_at;
    s32 var_t8_3;
    s8 *temp_a1;
    u32 temp_a1_2;
    u32 temp_t8;
    u32 var_t3_2;
    u8 *temp_v0_4;
    u8 *var_t9;
    u8 *var_t9_2;
    u8 temp_t9;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 var_t3;
    u8 var_v1;
    void *temp_a0;
    void *temp_a0_2;

    M2C_MEMCPY_ALIGNED(&sp454, &D_80147764, 0xD0);
    *(u8 *)((unsigned char *)(&sp454 + 0xCC) + (4)) = (u8) *(u8 *)((unsigned char *)(&D_80147764.v[0xCC]) + (4));
    sp420[0].unk0 = (s32) D_80147838.unk0;
    *(s32 *)((unsigned char *)(&sp420[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147838) + (4));
    *(s32 *)((unsigned char *)(&sp420[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147838) + (8));
    *(s32 *)((unsigned char *)(&sp420[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147838) + (12));
    *(s32 *)((unsigned char *)(&sp420[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147838) + (16));
    *(s32 *)((unsigned char *)(&sp420[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80147838) + (20));
    M2C_MEMCPY_ALIGNED(&sp39C, &D_80147850, 0x84);
    M2C_MEMCPY_ALIGNED(&sp318, &D_801478D4, 0x84);
    sp300.unk0 = (s32) D_80147958.unk0;
    *(s32 *)((unsigned char *)(&sp300) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147958) + (8));
    *(s32 *)((unsigned char *)(&sp300) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147958) + (4));
    *(s32 *)((unsigned char *)(&sp300) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147958) + (16));
    *(u8 *)((unsigned char *)(&sp300) + (20)) = (u8) D_80147958.v[0x14];
    *(s32 *)((unsigned char *)(&sp300) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147958) + (12));
    sp2E8.unk0 = (s32) D_80147970.unk0;
    *(s32 *)((unsigned char *)(&sp2E8) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147970) + (8));
    *(s32 *)((unsigned char *)(&sp2E8) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147970) + (4));
    *(s32 *)((unsigned char *)(&sp2E8) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147970) + (16));
    *(u8 *)((unsigned char *)(&sp2E8) + (20)) = (u8) D_80147970.v[0x14];
    *(s32 *)((unsigned char *)(&sp2E8) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147970) + (12));
    M2C_MEMCPY_ALIGNED(&sp74, &D_80147988, 0x270);
    *(? *)((unsigned char *)(&sp74 + 0x270) + (-1)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)(&D_80147988.v[0x270]) + (2)));
    M2C_MEMCPY_ALIGNED(&sp4C, &D_80147BFC, 0x24);
    *(u8 *)((unsigned char *)(&sp4C) + (36)) = (u8) *(u8 *)((unsigned char *)(&D_80147BFC) + (36));
    sp34.unk0 = (s32) D_80147C24.unk0;
    *(s32 *)((unsigned char *)(&sp34) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147C24) + (8));
    *(s32 *)((unsigned char *)(&sp34) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147C24) + (4));
    *(s32 *)((unsigned char *)(&sp34) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147C24) + (16));
    *(u8 *)((unsigned char *)(&sp34) + (20)) = (u8) D_80147C24.v[0x14];
    *(s32 *)((unsigned char *)(&sp34) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147C24) + (12));
    if (D_800E6280.unk_03E == 0x60) {
        sp422 = 0x1D;
    }
    var_v1 = 0;
    if ((u32) *(s16 *)((unsigned char *)(&(&sp420[0])[D_800E6280.unk_03F]) + (-2)) < (u32) ((D_800E6280.unk_040 - D_800E6280.unk_041) + 6)) {
        var_v1 = 1;
    }
    sp552 = var_v1;
    sp550 = func_80144198(0);
    sp551 = func_80144198(1);
    sp553 = func_80144198(2);
    D_800E6280.unk_F5F = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
    func_80085B3C(9, 0);
    if (sp550 == 0x5F) {
        if (sp551 >= 4U) {
            var_at = sp551 < 6U;
            if (sp551 < 6U) {
                sp526 = 0;
            } else {
                goto block_9;
            }
        } else {
            var_at = sp551 < 6U;
block_9:
            if ((var_at == 0) && (sp551 < 9U)) {
                sp526 = 1;
            } else {
                if (sp551 >= 9U) {
                    var_t8 = 2;
                    if (sp551 < 0xCU) {
                        goto block_45;
                    }
                }
                sp526 = 3;
            }
        }
    } else if (sp550 == 0x60) {
        if ((sp551 != 0) && (sp551 < 3U)) {
            sp526 = 3;
        } else if ((sp551 >= 3U) && (sp551 < 6U)) {
            sp526 = 4;
        } else {
            if (sp551 >= 6U) {
                var_t8 = 5;
                if (sp551 < 9U) {
                    goto block_45;
                }
            }
            if ((sp551 >= 9U) && (sp551 < 0xCU)) {
                sp526 = 6;
            } else {
                sp526 = 7;
            }
        }
    } else {
        var_t8 = 0xB;
        if (sp550 == 0x61) {
            if ((sp551 != 0) && (sp551 < 3U)) {
                sp526 = 7;
            } else {
                if (sp551 >= 3U) {
                    var_t8 = 8;
                    if (sp551 < 6U) {
                        goto block_45;
                    }
                }
                if ((sp551 >= 6U) && (sp551 < 9U)) {
                    sp526 = 9;
                } else if ((sp551 >= 9U) && (sp551 < 0xCU)) {
                    sp526 = 0xA;
                } else {
                    sp526 = 0xB;
                }
            }
        } else {
block_45:
            sp526 = var_t8;
        }
    }
    var_t3 = 0;
    var_v1_2 = 0;
    var_t7 = &D_800E6280;
    do {
        temp_a1 = var_v1_2 + &sp43C;
        *temp_a1 = 0;
        if (var_t7->unk_120[0] == 1) {
            temp_v0 = *(u8 *)((unsigned char *)(sp + var_v1_2) + (768));
            if ((sp550 >= temp_v0) && ((sp550 != temp_v0) || (sp551 >= (u8) *(u8 *)((unsigned char *)(sp + var_v1_2) + (744)))) && ((var_v1_2 != 0xE) || !(D_800E6280.unk_74C[D_800E6280.unk_F5F] & 1)) && ((var_v1_2 != 0xF) || !(D_800E6280.unk_74C[D_800E6280.unk_F5F] & 2)) && ((var_v1_2 != 7) || !(D_800E6280.unk_74C[D_800E6280.unk_F5F] & 4)) && ((var_v1_2 != 0xE) || (*(u8 *)((unsigned char *)(sp + ((D_800E6280.unk_F5F * 0xC) + sp526)) + (924)) != 0)) && ((var_v1_2 != 0xF) || (*(u8 *)((unsigned char *)(sp + ((D_800E6280.unk_F5F * 0xC) + sp526)) + (792)) != 0)) && ((var_v1_2 != 7) || (((sp550 != 0x61) || (sp551 < 9U) || (sp551 >= 0xCU) || (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0xC] == 0)) && ((sp550 != 0x61) || (sp551 < 3U) || (sp551 >= 6U) || (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0xD] == 0)) && ((sp550 != 0x5F) || (sp551 < 9U) || (sp551 >= 0xCU) || (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0xE] == 0)))) && (var_v1_2 != 0) && (var_v1_2 != 1) && (var_v1_2 != 0x12) && ((var_v1_2 != 0xD) || ((sp551 != 2) && (sp551 != 0xC))) && ((var_v1_2 != 0xE) || (sp550 != 0x5F) || (sp551 >= 6U))) {
                if ((var_v1_2 == 0x10) || (var_v1_2 == 0x11)) {
                    if ((*(u8 *)((unsigned char *)(&D_800E6280 + (sp552 << 6) + sp553) + (68)) == 2) || (sp551 == 7)) {
                        goto block_88;
                    }
                } else {
block_88:
                    if ((((var_v1_2 != 0x13) && (var_v1_2 != 0x14)) || (*(u8 *)((unsigned char *)(&D_800E6280 + (sp552 << 6) + sp553) + (68)) == 2) || (sp551 == 0xC)) && (var_v1_2 != *(u8 *)((unsigned char *)(sp + D_800E6280.unk_72C[D_800E6280.unk_F5F]) + (76))) && ((temp_t7 = D_800E6280.unk_54C[0].w, (D_800E6280.unk_F5F != ((u32) (temp_t7 * 8) >> 0x1C))) || (var_v1_2 != (temp_t7 & 0x1F))) && ((D_800E6280.unk_F5F != 1) || (((var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && (var_v1_2 != 0x10) && (var_v1_2 != 0x11) && (var_v1_2 != 0x14))) && ((D_800E6280.unk_F5F != 2) || (var_v1_2 != 0x14)) && ((D_800E6280.unk_F5F != 3) || ((var_v1_2 != 0x10) && (var_v1_2 != 0x11))) && ((D_800E6280.unk_F5F != 4) || (var_v1_2 != 7)) && ((D_800E6280.unk_F5F != 5) || (var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && ((D_800E6280.unk_F5F != 9) || (((var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && (var_v1_2 != 0x14))) && ((D_800E6280.unk_F5F != 0xA) || ((var_v1_2 != 7) && (var_v1_2 != 0xB)))) {
                        temp_v0_2 = *(u8 *)((unsigned char *)(sp + var_v1_2) + (52));
                        temp_a0 = (D_800E6280.unk_F5F * 0x13) + &sp454;
                        if ((sp551 == 1) || (sp551 == 3) || (sp551 == 6) || (var_t9 = temp_a0 + temp_v0_2, (sp551 == 0xB))) {
                            var_t9 = temp_a0 + temp_v0_2;
                            if (var_v1_2 == 0xD) {
                                var_t9 = temp_a0 + ((temp_v0_2 + 1) & 0xFF);
                            }
                        }
                        *temp_a1 = 1;
                        var_t3 = (var_t3 + *var_t9) & 0xFF;
                    }
                }
            }
        }
        var_v1_2 += 1;
        var_t7 = &D_800E6280 + var_v1_2;
    } while (var_v1_2 < 0x15);
    sp529 = var_t3;
    func_800AE120((s32) D_800E6280.unk_10F8);
    var_t3_2 = 0;
    var_a3 = 2;
    var_v1_3 = 0;
    temp_a1_2 = (func_800AE0D0() % var_t3) & 0xFF;
    var_t8_2 = &subroutine_arg0;
loop_136:
    if (*(u8 *)((unsigned char *)var_t8_2 + (1084)) != 0) {
        temp_v0_3 = *(&sp34 + var_v1_3);
        temp_a0_2 = (D_800E6280.unk_F5F * 0x13) + &sp454;
        if ((sp551 == 1) || (sp551 == 3) || (sp551 == 6) || (var_t9_2 = temp_a0_2 + temp_v0_3, (sp551 == 0xB))) {
            var_t9_2 = temp_a0_2 + temp_v0_3;
            if (var_v1_3 == 0xD) {
                var_t9_2 = temp_a0_2 + ((temp_v0_3 + 1) & 0xFF);
            }
        }
        temp_t8 = (var_t3_2 + *var_t9_2) & 0xFF;
        var_t3_2 = temp_t8;
        if (temp_t8 >= temp_a1_2) {
            var_a3 = var_v1_3 & 0xFF;
        } else {
            goto block_145;
        }
    } else {
block_145:
        var_v1_3 += 1;
        var_t8_2 = sp + var_v1_3;
        if (var_v1_3 < 0x15) {
            goto loop_136;
        }
    }
    temp_v0_4 = var_a3 + &sp34;
    D_801476E0 = *temp_v0_4;
    if (sp551 != 1) {
        if ((sp551 != 3) && (sp551 != 6)) {
            if (sp551 == 0xB) {
                goto block_150;
            }
        } else {
            goto block_151;
        }
    } else {
block_150:
block_151:
        if (temp_v0_4 == &sp41) {
            D_801476E0 += 1;
        }
    }
    func_80144270();
    func_8004E9F4(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_801476D8 = 0;
    D_801476DC = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80142760();
    temp_t9 = D_800E6280.unk_F5F;
    switch (temp_t9) {
    case 0:
        D_801476CC = D_80147634;
        D_801476D0 = D_80147668;
        D_801476D4 = D_8014769C;
        break;
    case 1:
        D_801476CC = D_80147644;
        var_t8_3 = D_801476AC;
        D_801476D0 = D_80147678;
block_167:
        D_801476D4 = var_t8_3;
        break;
    case 2:
        D_801476CC = D_80147638;
        D_801476D0 = D_8014766C;
        D_801476D4 = D_801476A0;
        break;
    case 3:
        D_801476CC = D_8014763C;
        D_801476D0 = D_80147670;
        D_801476D4 = D_801476A4;
        break;
    case 4:
        D_801476CC = D_80147658;
        D_801476D0 = D_8014768C;
        D_801476D4 = D_801476C0;
        break;
    case 5:
        D_801476CC = D_80147648;
        var_t8_3 = D_801476B0;
        D_801476D0 = D_8014767C;
        goto block_167;
    case 6:
        D_801476CC = D_80147650;
        D_801476D0 = D_80147684;
        D_801476D4 = D_801476B8;
        if ((u32) (func_80051A68(D_800E6280.unk_F5F) & 0x7F) < 2U) {
            D_801476CC = D_8014764C;
            var_t8_3 = D_801476B4;
            D_801476D0 = D_80147680;
            goto block_167;
        }
        break;
    case 7:
        D_801476CC = D_80147640;
        D_801476D0 = D_80147674;
        D_801476D4 = D_801476A8;
        break;
    case 8:
        D_801476CC = D_80147630;
        D_801476D0 = D_80147664;
        D_801476D4 = D_80147698;
        break;
    case 9:
        D_801476CC = D_80147654;
        D_801476D0 = D_80147688;
        D_801476D4 = D_801476BC;
        break;
    case 10:
        D_801476CC = D_80147660;
        var_t8_3 = D_801476C8;
        D_801476D0 = D_80147694;
        goto block_167;
    }
    func_800AE0F0(&D_800CA280, (D_801476E0 * 0x21) + &sp74);
    func_8004284C();
}
