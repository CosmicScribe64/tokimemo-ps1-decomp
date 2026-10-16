extern ? D_800CA280;
extern u8 D_800E6377;
extern s32 D_800E68EC;
extern s32 D_800E68F0;
extern s32 D_800E68F4;
extern s32 D_800E68F8;
extern s32 D_800E68FC;
extern s32 D_800E6900;
extern s32 D_800E6904;
extern s32 D_800E6908;
extern s32 D_800E690C;
extern s32 D_800E6910;
extern u8 D_800E71DF;
static ? D_8013C0EC;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C0F0;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1E4;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1E8;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1EC;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1F0;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1F4;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1F8;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C1FC;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C200;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C204;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C208;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C20C;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C210;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C214;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C218;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C21C;                                /* unable to generate initializer: unknown type; const */
static ? D_8013C220;                                /* unable to generate initializer: unknown type; const */

void func_8013A2E8(void) {
    s32 sp5C;
    u8 sp5B;
    s16 sp58;
    u8 sp57;
    ? sp3C;
    ? *temp_t7;
    s16 temp_t3;
    s16 temp_t3_2;
    s16 temp_t6_2;
    s16 temp_t6_3;
    s16 temp_t6_4;
    s16 temp_t6_5;
    s16 temp_v0;
    s16 var_a0;
    s16 var_t0;
    s16 var_t0_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 temp_a3;
    s32 var_a3;
    s32 var_t0_3;
    s32 var_t4;
    s32 var_t4_2;
    s32 var_v0;
    u32 temp_t6;
    u32 var_a0_2;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 var_a1;
    u8 var_t1;

    temp_t7 = "M残念ながら 駄目だったが、";
    sp3C.unk0 = (s32) temp_t7->unk0;
    *(s32 *)((unsigned char *)(&sp3C) + (4)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (4));
    *(s32 *)((unsigned char *)(&sp3C) + (8)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (8));
    *(s32 *)((unsigned char *)(&sp3C) + (12)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (12));
    *(s32 *)((unsigned char *)(&sp3C) + (16)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (16));
    *(s32 *)((unsigned char *)(&sp3C) + (20)) = (s32) *(s32 *)((unsigned char *)temp_t7 + (20));
    var_a0 = 0;
    var_a1 = 0xFF;
    var_v1 = 0;
    *(? *)((unsigned char *)(&sp3C) + (23)) = (unaligned s32) M2C_LWL(*(? *)((unsigned char *)temp_t7 + (26)));
    var_t4 = 0 * 4;
    do {
        temp_v0 = *(s16 *)((unsigned char *)(&D_800E6280 + var_t4) + (258));
        if (var_a0 < temp_v0) {
            var_a0 = temp_v0;
            var_a1 = var_v1 & 0xFF;
        }
        var_v1 += 1;
        var_t4 = var_v1 * 4;
    } while (var_v1 < 7);
    sp58 = var_a0;
    sp5B = var_a1;
    func_801396E0();
    D_80122CD0 = dialogue_render(-0x48, 0x48, 0, **D_8013CAF0, 0, &D_8013C0EC, &D_8013C0F0, 0);
    func_8013B640();
    if (D_800E6377 & 1) {
        func_800AE0F0(&D_800CA280, "一流大学");
    } else if (D_800E6377 & 2) {
        func_800AE0F0(&D_800CA280, "二流大学");
    } else if (D_800E6377 & 4) {
        func_800AE0F0(&D_800CA280, "三流大学");
    } else if (D_800E6377 & 8) {
        func_800AE0F0(&D_800CA280, "一流企業");
    } else if (D_800E6377 & 0x10) {
        func_800AE0F0(&D_800CA280, "二流企業");
    } else if (D_800E6377 & 0x20) {
        func_800AE0F0(&D_800CA280, "三流企業");
    } else if ((u32) ((u32) (D_800E68EC << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "小説家は");
    } else if ((u32) ((u32) (D_800E68F0 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "俳優は");
    } else if ((u32) ((u32) (D_800E68F4 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "製薬会社は");
    } else if ((u32) ((u32) (D_800E68F8 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "有名ゲーム会社は ");
    } else if ((u32) ((u32) (D_800E68FC << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "画家は");
    } else if ((u32) ((u32) (D_800E6900 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "音楽家は");
    } else if ((u32) ((u32) (D_800E6904 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "プロ野球選手は");
    } else if ((u32) ((u32) (D_800E6908 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "Ｊリーガーは");
    } else if ((u32) ((u32) (D_800E690C << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "プロテニスプレイヤーは ");
    } else if ((u32) ((u32) (D_800E6910 << 5) >> 0x1E) >= 3U) {
        func_800AE0F0(&D_800CA280, "水泳の実業団は");
    } else {
        func_800AE0F0(&D_800CA280, "プロバスケプレイヤーは ");
    }
    var_a0_2 = 0;
    var_t0 = 0;
    var_v1_2 = 0;
    do {
        if (var_v1_2 == 7) {
            temp_t6 = (u8) *(u8 *)((unsigned char *)(&D_800E6280 + var_v1_2) + (309)) >> 1;
            if (var_a0_2 < temp_t6) {
                var_a0_2 = temp_t6 & 0xFF;
                goto block_46;
            }
        } else if (var_v1_2 == 6) {
            temp_v0_2 = *(u8 *)((unsigned char *)(&D_800E6280 + var_v1_2) + (309));
            if (var_a0_2 < (u32) ((u32) (temp_v0_2 * 3) >> 2)) {
                var_a0_2 = (temp_v0_2 >> 1) & 0xFF;
                var_t0 = var_v1_2;
            }
        } else {
            temp_v0_3 = *(u8 *)((unsigned char *)(&D_800E6280 + var_v1_2) + (309));
            if (var_a0_2 < temp_v0_3) {
                var_a0_2 = temp_v0_3 & 0xFF;
block_46:
                var_t0 = var_v1_2;
            }
        }
        var_v1_2 += 1;
    } while (var_v1_2 < 8);
    temp_t3 = D_800CA2C0 + 0x10;
    D_800CA2C0 = temp_t3;
    D_80122CD0 = dialogue_render(-0x48, (s32) temp_t3, 0, *(s32 *)((unsigned char *)(&(*D_8013CAF0)[var_t0]) + (4)), 0, &D_8013C1E4, &D_8013C1E8, 0);
    func_8013B640();
    temp_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (36));
    temp_t3_2 = D_800CA2C0 + 0x10;
    D_800CA2C0 = temp_t3_2;
    sp5C = temp_a3;
    D_80122CD0 = dialogue_render(-0x48, (s32) temp_t3_2, 0, temp_a3, 0, &D_8013C1EC, &D_8013C1F0, 0);
    func_8013B640();
    var_a3 = temp_a3;
    var_t0_2 = 0xFF;
    var_v1_3 = 0;
    var_v0 = 0 * 4;
loop_49:
    if (((u32) ((u32) (*(s32 *)((unsigned char *)(&D_800E6280 + var_v0) + (1644)) << 5) >> 0x1E) >= 3U) && (D_800E6377 & 0x40) && !(D_800E6377 & 0x80)) {
        var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0 + var_v0) + (40));
        var_t1 = var_v1_3 & 0xFF;
        var_t0_2 = var_v1_3;
    } else {
        var_v1_3 += 1;
        var_v0 = var_v1_3 * 4;
        if (var_v1_3 >= 0xB) {
            var_t1 = sp57;
        } else {
            goto loop_49;
        }
    }
    temp_t6_2 = D_800CA2C0 + 0x10;
    D_800CA2C0 = temp_t6_2;
    if (var_t0_2 == 0xFF) {
        if (D_800E6377 & 0x80) {
            if (D_800E6377 & 0x40) {
                D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_2, 0, (s32) &sp3C, 0, &D_8013C1F4, &D_8013C1F8, 0);
                func_8013B640();
            } else {
                D_80122CD0 = dialogue_render(-0x48, (s32) D_800CA2C0, 0, *(s32 *)((unsigned char *)(*D_8013CAF0) + (108)), 0, &D_8013C1FC, &D_8013C200, 0);
                func_8013B640();
            }
            if (sp58 < 0x78) {
                if ((D_800E6377 & 1) || (D_800E6377 & 2) || (D_800E6377 & 4)) {
                    var_t1 = 0xC;
                    var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (128));
                } else {
                    var_t1 = 0xB;
                    var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (132));
                }
            } else if ((sp5B == 0) || (sp5B == 1) || (sp5B == 2) || (sp5B == 3)) {
                var_t1 = 0xD;
                var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (112));
            } else if (sp5B == 4) {
                var_t1 = 0xE;
                var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (116));
            } else if (sp5B == 5) {
                var_t1 = 0xF;
                var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (120));
            } else {
                var_t1 = 0x10;
                var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (124));
            }
        } else if (D_800E6377 & 1) {
            var_t1 = 0x14;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (84));
        } else if (D_800E6377 & 2) {
            var_t1 = 0x15;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (88));
        } else if (D_800E6377 & 4) {
            var_t1 = 0x16;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (92));
        } else if (D_800E6377 & 8) {
            var_t1 = 0x11;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (96));
        } else if (D_800E6377 & 0x10) {
            var_t1 = 0x12;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (100));
        } else {
            var_t1 = 0x13;
            var_a3 = *(s32 *)((unsigned char *)(*D_8013CAF0) + (104));
        }
    }
    sp57 = var_t1;
    D_80122CD0 = dialogue_render(-0x48, (s32) D_800CA2C0, 0, var_a3, 0, &D_8013C204, &D_8013C208, 0);
    func_8013B640();
    switch (D_800E71DF) {
    case 0:
        if (var_t1 == 0x14) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x15) || (var_t1 == 0x16) || (var_t1 == 0xD)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAC4;
        break;
    case 1:
        if (var_t1 == 0x14) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x15) || (var_t1 == 0x16) || (var_t1 == 0xD)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAD8;
        break;
    case 2:
        if (var_t1 == 0x11) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x12) || (var_t1 == 0x13) || (var_t1 == 0xE) || (var_t1 == 0xF) || (var_t1 == 0x10)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAC8;
        break;
    case 3:
        if (var_t1 == 4) {
            var_t0_3 = 0;
        } else if ((var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 3;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 4;
            }
        }
        D_800CA160 = D_8013CAD0;
        break;
    case 4:
        if (var_t1 == 0xD) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x14) || (var_t1 == 0x15) || (var_t1 == 0x16)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAE8;
        break;
    case 5:
        if (var_t1 == 0x15) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x14) || (var_t1 == 0x16) || (var_t1 == 0xD)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CADC;
        break;
    case 6:
        if (var_t1 == 9) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 3;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 4;
            }
        }
        var_t4_2 = D_8013CAE0;
block_295:
        D_800CA160 = var_t4_2;
        break;
    case 7:
        if (var_t1 == 0xF) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x14) || (var_t1 == 0x15) || (var_t1 == 0x16) || (var_t1 == 0xD) || (var_t1 == 0xC)) {
            var_t0_3 = 3;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 4;
            }
        }
        D_800CA160 = D_8013CAD4;
        break;
    case 8:
        if (var_t1 == 0xB) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x14) || (var_t1 == 0x15) || (var_t1 == 0x16) || (var_t1 == 0xD)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 0xF;
            }
        }
        D_800CA160 = D_8013CAC0;
        break;
    case 9:
        if (var_t1 == 0x12) {
            var_t0_3 = 0;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if ((var_t1 == 0x11) || (var_t1 == 0x13) || (var_t1 == 0xE) || (var_t1 == 0xF) || (var_t1 == 0x10)) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 1;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAE4;
        break;
    case 10:
        if ((var_t1 == 0x14) || (var_t1 == 0x15) || (var_t1 == 0x16)) {
            var_t0_3 = 0;
        } else if ((var_t1 == 0x11) || (var_t1 == 0x12) || (var_t1 == 0x13)) {
            var_t0_3 = 1;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if (var_t1 == 0xF) {
            var_t0_3 = 3;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 4;
        } else {
            var_t0_3 = 7;
            if (var_t1 == 0xB) {
                var_t0_3 = 5;
            }
        }
        D_800CA160 = D_8013CAF4;
        break;
    case 11:
        if ((var_t1 == 0) || (var_t1 == 1) || (var_t1 == 2) || (var_t1 == 3) || (var_t1 == 6) || (var_t1 == 7) || (var_t1 == 8) || (var_t1 == 9)) {
            var_t0_3 = 1;
        } else if ((var_t1 == 4) || (var_t1 == 5) || (var_t1 == 0xA)) {
            var_t0_3 = 2;
        } else if (var_t1 == 0xC) {
            var_t0_3 = 3;
        } else {
            var_t0_3 = 0;
            if (var_t1 == 0xB) {
                var_t0_3 = 4;
            }
        }
        D_800CA160 = D_8013CAF8;
        break;
    case 12:
        var_t0_3 = 0;
        D_800CA160 = D_8013CACC;
        break;
    default:
        var_t4_2 = D_8013CAEC;
        var_t0_3 = 0;
        goto block_295;
    }
    temp_t6_3 = D_800CA2C0 + 0x10;
    D_800CA2C0 = temp_t6_3;
    D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_3, 0, *(*D_800CA160 + (var_t0_3 * 4)), 0, &D_8013C20C, &D_8013C210, 0);
    func_8013B640();
    if ((D_800E71DF != 0xC) && (D_800E71DF != 0xD)) {
        temp_t6_4 = D_800CA2C0 + 0x10;
        D_800CA2C0 = temp_t6_4;
        D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_4, 0, *(s32 *)((unsigned char *)(*D_800CA160) + (24)), 0, &D_8013C214, &D_8013C218, 0);
        func_8013B640();
        temp_t6_5 = D_800CA2C0 + 0x10;
        D_800CA2C0 = temp_t6_5;
        D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_5, 0, *(s32 *)((unsigned char *)(*D_8013CAF0) + (136)), 0, &D_8013C21C, &D_8013C220, 0);
        func_8013B640();
    }
    D_800CA2C0 += 0x20;
    func_8004284C();
}
