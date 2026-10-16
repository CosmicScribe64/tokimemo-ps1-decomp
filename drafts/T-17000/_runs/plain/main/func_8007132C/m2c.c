extern ? D_800E62B8;
extern u8 D_800E62BD;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E62C2;
extern u16 D_800E6374;
extern u8 D_800E6375;
extern ? D_800E6442;
extern ? D_800E6446;
extern ? D_800E6448;
extern ? D_800E64B0;
extern ? D_800E64E8;
extern s32 D_800E66B0;
extern u8 D_800E66B3;
extern ? D_800E67CC;
extern u8 D_800E6800;
extern u8 D_800E6839;
extern u8 D_800E683E;
extern ? D_800E6918;
extern u8 D_800E691C;
extern ? D_800E691D;
extern ? D_800E691E;
extern u8 D_800E699C;
extern u8 D_800E699D;
extern u8 D_800E699E;
extern u8 D_800E699F;
extern u8 D_800E69A4;
extern u8 D_800E69A5;
extern u8 D_800E71DF;
extern u8 D_800E71E8;

void func_8007132C(void) {
    GameState *sp30;
    ? *var_v1;
    GameState *var_v1_2;
    GameState *var_v1_5;
    GameState *var_v1_6;
    GameState *var_v1_8;
    GameState *var_v1_9;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t3;
    s32 temp_t5;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_a0;
    s32 var_at;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_v0;
    s8 var_s0;
    u32 temp_t3_3;
    u8 temp_t0_2;
    u8 temp_t1;
    u8 temp_t1_2;
    u8 temp_t3_2;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_v0_4;
    u8 var_v0_2;
    u8 var_v1_3;
    u8 var_v1_4;
    u8 var_v1_7;
    void *temp_t8;
    void *var_a1;
    void *var_a1_2;

    func_80071280();
    func_80071038();
    if (((u8) D_800E62BE >= 0x62U) && ((u8) D_800E62BF >= 3U) && (D_800E62C0 != 0)) {
        func_8004284C();
        return;
    }
    if (((u8) D_800E62BE >= 0x62U) && (D_800E62BF == 2) && (D_800E62C0 == 0x1A)) {
        var_v0 = 0;
        if (D_800E683E == 0) {
            var_v1 = &D_800E62B8;
            do {
                temp_t3 = *(s32 *)((unsigned char *)var_v1 + (456));
                var_v1 += 0x38;
                if (temp_t3 & 2) {
                    var_v0 += 1;
                }
            } while (var_v1 != &D_800E64B0);
            if (var_v0 >= 6) {
                D_800E699C -= 1;
                D_80122CD4 = 0xD;
                D_80122CD0 = 0;
                main_state_set(0x5D);
                return;
            }
            goto block_14;
        }
    }
block_14:
    if (((u8) D_800E62BE >= 0x62U) && (D_800E62BF == 2) && (D_800E62C0 == 0x1C) && (func_80071110() != 0) && (D_800E6800 == 0)) {
        D_80122CD4 = 2;
        D_80122CD0 = 7;
        D_800E6800 += 1;
        main_state_set(0x5D);
        return;
    }
    func_8006D138();
    func_8006D00C();
    if (((D_800E62BF != 0xC) || (D_800E62C0 != 0x18)) && (D_800E699D == 0) && (D_800E62C1 != 6) && (D_800E6280.unk_044[0].unk_00[D_800E62C0] == 0) && (D_800E6280.unk_69C[D_800E699C].unk_00 == 0xFB) && (D_800E691C != 0x40)) {
        var_s0 = 0;
        var_v1_2 = &D_800E6280;
        if (*(u8 *)((unsigned char *)((D_800E699C * 4) + &D_800E6280) + (1688)) == 0) {
loop_28:
            if ((var_v1_2->unk_1BC[0].unk_0C.w & 2) && (var_v1_2->unk_1BC[0].unk_0A >= 0x4C) && !(var_v1_2->unk_1BC[0].unk_0C.h[1] & 1)) {
                var_v1_2->unk_1BC[0].unk_0C.b[2] |= 1;
                if (D_800E699C != 0) {
                    D_800E699C -= 1;
                }
                D_800E699D += 1;
                *(&D_800E691D + (D_800E699C * 4)) = var_s0;
                main_state_set(0x59);
                return;
            }
            var_s0 += 1;
            var_v1_2 += 0x38;
            if (var_s0 == 0xB) {
                goto block_35;
            }
            goto loop_28;
        }
    }
block_35:
    temp_t8 = (D_800E699C * 4) + &D_800E6280;
    var_v1_3 = D_800E6280.unk_69C[D_800E699C].unk_00;
    var_a1 = temp_t8;
    if ((D_800E691C == 0x25) && (*(u8 *)((unsigned char *)temp_t8 + (1688)) == 0x25)) {
        temp_t1 = D_800E699C + 1;
        if (var_v1_3 == 0x32) {
            temp_t2 = temp_t1 & 0xFF;
            D_800E699C = temp_t1;
            var_a1 = (temp_t2 * 4) + &D_800E6280;
            var_v1_3 = D_800E6280.unk_69C[temp_t2].unk_00;
        }
        if (var_v1_3 == 0x33) {
            temp_t7 = D_800E699C + 1;
            temp_t0 = temp_t7 & 0xFF;
            D_800E699C = temp_t7;
            var_a1 = (temp_t0 * 4) + &D_800E6280;
            var_v1_3 = D_800E6280.unk_69C[temp_t0].unk_00;
        }
    }
    if (var_v1_3 == 0x32) {
        var_v0_2 = *(u8 *)((unsigned char *)var_a1 + (1693));
        if ((var_v0_2 == 0) && (D_800E699E & 1)) {
            temp_t3_2 = D_800E699C + 1;
            temp_t5 = temp_t3_2 & 0xFF;
            var_a1 = (temp_t5 * 4) + &D_800E6280;
            D_800E699C = temp_t3_2;
            var_v0_2 = D_800E6280.unk_69C[temp_t5].unk_01;
        }
        if ((var_v0_2 != 0) && (D_800E699E & 2)) {
            temp_t6 = D_800E699C + 1;
            var_a1 = ((temp_t6 & 0xFF) * 4) + &D_800E6280;
            D_800E699C = temp_t6;
        }
    }
    var_v1_4 = *(u8 *)((unsigned char *)var_a1 + (1692));
    if ((var_v1_4 == 0x33) && (D_800E699E & 4)) {
        temp_t0_2 = D_800E699C + 1;
        D_800E699C = temp_t0_2;
        var_v1_4 = *(&D_800E691C + ((temp_t0_2 & 0xFF) * 4));
    }
    if (var_v1_4 == 0xFA) {
        temp_t1_2 = D_800E699C + 1;
        D_800E699C = temp_t1_2;
        var_v1_4 = *(&D_800E691C + ((temp_t1_2 & 0xFF) * 4));
    }
    if ((var_v1_4 == 0xFB) || (var_v1_4 == 0xFF)) {
        func_8006B0C8();
        if (appointment_find_today((s32) D_800E62BE, (s32) D_800E62BF, (s32) D_800E62C0) > 0) {
            if (get_g_zyotai_s((s32) ((u32) (D_800E67CC.unk0 * 8) >> 0x1C)) == 4) {
                *(u8 *)((unsigned char *)(&D_800E67CC) + (3)) = (u8) (*(u8 *)((unsigned char *)(&D_800E67CC) + (3)) & 0xFFDF);
                D_800E699F = 1;
                main_state_set(0x43);
                func_80042908(2);
                return;
            }
            if (D_800E69A4 != D_800E69A5) {
                *(u8 *)((unsigned char *)(&D_800E67CC) + (3)) = (u8) (*(u8 *)((unsigned char *)(&D_800E67CC) + (3)) & 0xFFDF);
                D_800E699F = 1;
                main_state_set(0x43);
                func_80042908(0);
                return;
            }
            if (D_800E67CC.unk0 & 0x20000000) {
                *(u8 *)((unsigned char *)(&D_800E67CC) + (3)) = (u8) (*(u8 *)((unsigned char *)(&D_800E67CC) + (3)) & 0xFFDF);
                D_800E699F = 1;
                main_state_set(0x43);
                func_80042908(1);
                return;
            }
            goto block_62;
        }
    }
block_62:
    if (*(&D_800E691C + (D_800E699C * 4)) == 0xFF) {
        temp_t3_3 = (u16) D_800E6374 >> 0xC;
        if ((temp_t3_3 >= 6U) && (temp_t3_3 < 0xBU)) {
            var_at = temp_t3_3 < 6U;
            if (((u32) (D_800E62C0 + 6) / 7U) == 3) {
                var_at = temp_t3_3 < 6U;
                if (((u8) D_800E71E8 >> 4) != 4) {
                    main_state_set(0x43);
                    func_80042908(8);
                    return;
                }
            }
            goto block_69;
        }
        var_at = temp_t3_3 < 6U;
block_69:
        if ((var_at != 0) && (D_800E62BF == 0xA) && (((D_800E62BE == 0x5F) && (D_800E62C0 == 8)) || ((D_800E62BE == 0x60) && (D_800E62C0 == 6)) || ((D_800E62BE == 0x61) && (D_800E62C0 == 5)))) {
            main_state_set(0x43);
            func_80042908(9);
            return;
        }
        if ((D_800E62BE == 0x5F) && (D_800E62BF == 0xA)) {
            var_s0_2 = 0;
            if (D_800E62C0 == 0xF) {
                var_v1_5 = &D_800E6280;
loop_82:
                temp_v0 = var_v1_5->unk_1BC[0].unk_0C.w;
                if ((temp_v0 & 2) && (temp_v0 & 0x1000)) {
                    sp30 = var_v1_5;
                    if (get_g_zyotai_s(var_s0_2) != 4) {
                        sp30 = var_v1_5;
                        if (func_8006B014(var_s0_2) == 0) {
                            D_800E699F = 1;
                            main_state_set(0x43);
                            func_80042908(6);
                            return;
                        }
                    }
                }
                var_s0_2 += 1;
                var_v1_5 += 0x38;
                if (var_s0_2 == 0xB) {
                    goto block_88;
                }
                goto loop_82;
            }
        }
block_88:
        if ((D_800E62BE == 0x60) && (D_800E62BF == 0xA)) {
            var_s0_3 = 0;
            if (D_800E62C0 == 0xD) {
                var_v1_6 = &D_800E6280;
loop_92:
                temp_v0_2 = var_v1_6->unk_1BC[0].unk_0C.w;
                if ((temp_v0_2 & 2) && (temp_v0_2 & 0x2000)) {
                    sp30 = var_v1_6;
                    if (get_g_zyotai_s(var_s0_3) != 4) {
                        sp30 = var_v1_6;
                        if (func_8006B014(var_s0_3) == 0) {
                            D_800E699F = 1;
                            main_state_set(0x43);
                            func_80042908(7);
                            return;
                        }
                    }
                }
                var_s0_3 += 1;
                var_v1_6 += 0x38;
                if (var_s0_3 == 0xB) {
                    goto block_98;
                }
                goto loop_92;
            }
        }
        goto block_98;
    }
block_98:
    var_a1_2 = (D_800E699C * 4) + &D_800E6280;
    var_v1_7 = D_800E6280.unk_69C[D_800E699C].unk_00;
    var_a0 = 0;
    if (var_v1_7 == 0xFB) {
        var_v1_8 = &D_800E6280;
        do {
            temp_v0_3 = var_v1_8->unk_1BC[0].unk_0C.w;
            if ((temp_v0_3 & 2) && (var_v1_8->unk_1BC[0].unk_0A >= 0x3D) && !(temp_v0_3 & 0x20000)) {
                var_a0 += 1;
                var_v1_8->unk_1BC[0].unk_0C.b[2] |= 2;
            }
            var_v1_8 += 0x38;
        } while ((u32) var_v1_8 < (u32) &D_800E64E8);
        if (var_a0 != 0) {
            main_state_set(0x44);
            return;
        }
        if (D_800E699F == 0) {
            if ((D_800E62BE == 0x61) && (D_800E62BF == 7) && (D_800E62C0 == 0x1A)) {
                D_800E699F = 1;
                main_state_set(0x43);
                func_80042908(5);
                return;
            }
            if ((D_800E62BD == 0) && (D_800E62BE != 0x62)) {
                var_v1_9 = &D_800E6280;
loop_115:
                if (var_v1_9->unk_1BC[0].unk_0C.w & 2) {
                    var_v1_9 += 0x38;
                    if (var_v1_9->unk_1BC[0].unk_0A >= 0x5B) {
                        func_80044750(0x50E);
                        D_800E699F = 1;
                        main_state_set(0x43);
                        func_80042908(4);
                        return;
                    }
                    goto block_119;
                }
                var_v1_9 += 0x38;
block_119:
                if (var_v1_9 == &D_800E64E8) {
                    goto block_120;
                }
                goto loop_115;
            }
block_120:
            if ((((u32) (D_800E66B0 << 0x14) >> 0x1D) != 0) && (D_800E6839 == 0) && (((u8) D_800E66B3 < 7U) || (((u8) D_800E66B3 >= 7U) && ((u16) D_800E62C2 >= 0x2DBU) && ((u8) D_800E66B3 < 9U))) && !(rand() & 0x7F)) {
                D_800E699F = 1;
                main_state_set(0x43);
                func_80042908(3);
                return;
            }
            goto block_128;
        }
block_128:
        D_800E699F = 0;
        D_800E699C += 1;
        func_80072338();
        var_a1_2 = (D_800E699C * 4) + &D_800E6280;
        var_v1_7 = D_800E6280.unk_69C[D_800E699C].unk_00;
        goto block_129;
    }
block_129:
    switch (var_v1_7) {                             /* irregular */
    case 0x0:
        var_s0_4 = 0;
        do {
            if (var_s0_4 != *(&D_800E691D + (D_800E699C * 4))) {
                icon_can_use_set(var_s0_4, 0);
            } else {
                icon_can_use_set(var_s0_4, 1);
            }
            var_s0_4 += 1;
        } while (var_s0_4 != 0xE);
        main_state_set(0x40);
        return;
    case 0x10:
        main_state_set(0x46);
        return;
    case 0xFE:
        main_state_set(0x38);
        return;
    case 0xFD:
        func_8006CF80();
        main_state_set(0x39);
        return;
    case 0x20:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        main_state_set(0x72);
        return;
    case 0x24:
        func_8006CF80();
        bzero(D_80122DD0, 0x100);
        if ((u32) ((u16) D_800E6374 >> 0xC) < 6U) {
            main_state_set(0x70);
            return;
        }
        main_state_set(0x71);
        return;
    case 0x28:
        func_8006CF80();
        main_state_set(0x42);
        return;
    case 0x2C:
        func_8006CF80();
        main_state_set(0x41);
        return;
    case 0x31:
        main_state_set(0x54);
        return;
    case 0x30:
        main_state_set(0x55);
        return;
    case 0x40:
        func_8006CF80();
        main_state_set(0x35);
        return;
    case 0x41:
        func_8006CF80();
        main_state_set(0x51);
        return;
    case 0x42:
        main_state_set(0x5F);
        return;
    case 0x43:
        func_8006CF80();
        main_state_set(0x36);
        return;
    case 0x44:
        D_800E699D += 1;
        if ((D_800E6375 & 0xF) == 3) {
            func_80072B5C(0);
            return;
        }
        temp_v0_4 = *(&D_800E6918 + (D_800E699C * 4));
        if (temp_v0_4 == 0x28) {
            func_80072B5C(0);
            return;
        }
        if (temp_v0_4 == 0x47) {
            func_80072B5C(0);
            return;
        }
        if (!(*(&D_800E6448 + (*(u8 *)((unsigned char *)var_a1_2 + (1693)) * 0x38)) & 2)) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x59);
        return;
    case 0xFC:
        main_state_set(0x32);
        return;
    case 0x32:
        main_state_set(0x57);
        return;
    case 0x33:
        main_state_set(0x56);
        return;
    case 0x25:
        main_state_set(0x74);
        return;
    case 0x26:
        main_state_set(0x75);
        return;
    case 0x27:
        main_state_set(0x76);
        return;
    case 0x46:
        D_800E699D += 1;
        if (D_800E67CC.unk0 & 0x20000000) {
            func_80072B5C(0);
            return;
        }
        main_state_set(0x5A);
        return;
    case 0x45:
        D_80122CD4 = (s32) *(u8 *)((unsigned char *)var_a1_2 + (1693));
        D_80122CD0 = (s32) *(&D_800E691E + (D_800E699C * 4));
        main_state_set(0x5D);
        return;
    case 0x47:
        D_800E699D += 1;
        main_state_set(0x61);
        return;
    case 0x48:
        D_800E71DF = *(u8 *)((unsigned char *)var_a1_2 + (1693));
        *(&D_800E6442 + (D_800E71DF * 0x38)) = 0x32;
        *(&D_800E6446 + (D_800E71DF * 0x38)) = 0;
        D_80122CD0 = (s32) *(&D_800E691E + (D_800E699C * 4));
        main_state_set(0x5B);
        return;
    case 0x49:
        main_state_set(0x58);
        return;
    case 0x4A:
        main_state_set(0x37);
        return;
    default:
        main_state_set(0x34);
        return;
    }
}
