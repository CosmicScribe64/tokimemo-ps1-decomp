#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_801476E4;

void func_80142760(void) {
    D_80147630 = 0x80197558;
    D_80147634 = 0x80197F60;
    D_80147638 = 0x80198830;
    D_8014763C = 0x801991A8;
    D_80147640 = 0x80199C00;
    D_80147644 = 0x8019A598;
    D_80147648 = 0x8019B0D4;
    D_8014764C = 0x8019BAEC;
    D_80147650 = 0x8019C4FC;
    D_80147654 = 0x8019CF64;
    D_80147658 = 0x8019D970;
    D_8014765C = 0x8019DF94;
    D_80147660 = 0x8019E5E4;
    D_80147664 = 0x80197624;
    D_80147668 = 0x80198024;
    D_8014766C = 0x801988FC;
    D_80147670 = 0x8019926C;
    D_80147674 = 0x80199CCC;
    D_80147678 = 0x8019A664;
    D_8014767C = 0x8019B1A0;
    D_80147680 = 0x8019BBB8;
    D_80147684 = 0x8019C5C8;
    D_80147688 = 0x8019D030;
    D_8014768C = 0x8019DA3C;
    D_80147690 = 0x8019DFBC;
    D_80147694 = 0x8019E6B0;
    D_80147698 = 0x801979D0;
    D_8014769C = 0x801983B0;
    D_801476A0 = 0x80198CA8;
    D_801476A4 = 0x801995F8;
    D_801476A8 = 0x8019A078;
    D_801476AC = 0x8019AA2C;
    D_801476B0 = 0x8019B54C;
    D_801476B4 = 0x8019BF64;
    D_801476B8 = 0x8019C974;
    D_801476BC = 0x8019D3DC;
    D_801476C0 = 0x8019DE04;
    D_801476C4 = 0x8019E05C;
    D_801476C8 = 0x8019EA5C;
}

void func_801429D4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_801476E4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80142A5C(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_80142B20(void) {
    u8 *p = &D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;

    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[*p], D_800B36C8[*p], D_800B3708[*p]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80142BB8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80142C38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80142C60(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAF23);
    func_80142760();
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
typedef struct { u8 b[209]; } UnalignedBytes209; /* size 0xD1 */
extern UnalignedBytes209 D_80147764;
typedef struct { u8 b[24]; } UnalignedBytes24; /* size 0x18 */
extern UnalignedBytes24 D_80147838;
typedef struct { u8 b[132]; } UnalignedBytes132; /* size 0x84 */
extern UnalignedBytes132 D_80147850;
extern UnalignedBytes132 D_801478D4;
typedef struct { u8 b[21]; } UnalignedBytes21; /* size 0x15 */
extern UnalignedBytes21 D_80147958;
extern UnalignedBytes21 D_80147970;
typedef struct { u8 b[627]; } UnalignedBytes627; /* size 0x273 */
extern UnalignedBytes627 D_80147988;
typedef struct { u8 b[37]; } UnalignedBytes37; /* size 0x25 */
extern UnalignedBytes37 D_80147BFC;
extern UnalignedBytes21 D_80147C24;
extern s32 D_800CA280;
extern s32 D_800E67CC;
extern s32 D_800E7368;
extern s32 D_800E7378;
extern s8 D_800E62BA;
extern s8 D_800E7322;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u8 D_800E699C;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80142C9C(void) {
    UnalignedBytes24 sp420;
    u8 sp553;
    u8 sp552;
    u8 sp551;
    u8 sp550;
    u8 sp529;
    s16 sp526;
    UnalignedBytes209 sp454;
    u8 sp43C[21];
    UnalignedBytes132 sp39C;
    UnalignedBytes132 sp318;
    UnalignedBytes21 sp300;
    UnalignedBytes21 sp2E8;
    UnalignedBytes627 sp74;
    UnalignedBytes37 sp4C;
    UnalignedBytes21 sp34;
    u8 *var_t8_2;
    u8 *var_t7;
    s16 var_t8;
    s16 var_v1_2;
    s16 var_v1_3;
    s32 var_a3;
    s32 var_at;
    s8 *temp_a1;
    u32 temp_a1_2;
    u32 temp_t8;
    u32 var_t3_2;
    u8 *temp_v0_4;
    u8 *var_t9;
    u8 *var_t9_2;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 var_t3;
    u8 var_v1;
    u8 *temp_a0;
    u8 *temp_a0_2;

    sp454 = D_80147764;
    sp420 = D_80147838;
    sp39C = D_80147850;
    sp318 = D_801478D4;
    sp300 = D_80147958;
    sp2E8 = D_80147970;
    sp74 = D_80147988;
    sp4C = D_80147BFC;
    sp34 = D_80147C24;
    if (D_800E62BE == 0x60) {
        *(s16 *)&sp420.b[2] = 0x1D;
    }
    var_v1 = 0;
    if ((u32) *(s16 *)((unsigned char *)((sp420.b + D_800E62BF * 2)) + (-2)) < (u32) ((D_800E62C0 - D_800E62C1) + 6)) {
        var_v1 = 1;
    }
    sp552 = var_v1;
    sp550 = func_80144198(0);
    sp551 = func_80144198(1);
    sp553 = func_80144198(2);
    D_800E71DF = D_800E6280.unk_69C[D_800E699C].unk_01;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
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
    var_t7 = (u8 *)&D_800E6280;
    do {
        temp_a1 = var_v1_2 + sp43C;
        *temp_a1 = 0;
        if (var_t7[0x120] == 1) {
            temp_v0 = sp300.b[var_v1_2];
            if ((sp550 >= temp_v0) && ((sp550 != temp_v0) || (sp551 >= (u8) sp2E8.b[var_v1_2])) && ((var_v1_2 != 0xE) || !(D_800E6280.unk_74C[D_800E71DF] & 1)) && ((var_v1_2 != 0xF) || !(D_800E6280.unk_74C[D_800E71DF] & 2)) && ((var_v1_2 != 7) || !(D_800E6280.unk_74C[D_800E71DF] & 4)) && ((var_v1_2 != 0xE) || (sp39C.b[((D_800E71DF * 0xC) + sp526)] != 0)) && ((var_v1_2 != 0xF) || (sp318.b[((D_800E71DF * 0xC) + sp526)] != 0)) && ((var_v1_2 != 7) || (((sp550 != 0x61) || (sp551 < 9U) || (sp551 >= 0xCU) || (D_800E6280.unk_1BC[D_800E71DF].unk_14[0xC] == 0)) && ((sp550 != 0x61) || (sp551 < 3U) || (sp551 >= 6U) || (D_800E6280.unk_1BC[D_800E71DF].unk_14[0xD] == 0)) && ((sp550 != 0x5F) || (sp551 < 9U) || (sp551 >= 0xCU) || (D_800E6280.unk_1BC[D_800E71DF].unk_14[0xE] == 0)))) && (var_v1_2 != 0) && (var_v1_2 != 1) && (var_v1_2 != 0x12) && ((var_v1_2 != 0xD) || ((sp551 != 2) && (sp551 != 0xC))) && ((var_v1_2 != 0xE) || (sp550 != 0x5F) || (sp551 >= 6U))) {
                if ((var_v1_2 == 0x10) || (var_v1_2 == 0x11)) {
                    if ((*(u8 *)((unsigned char *)((u8 *)&D_800E6280 + (sp552 << 6) + sp553) + (68)) == 2) || (sp551 == 7)) {
                        goto block_88;
                    }
                } else {
block_88:
                    if ((((var_v1_2 != 0x13) && (var_v1_2 != 0x14)) || (*(u8 *)((unsigned char *)((u8 *)&D_800E6280 + (sp552 << 6) + sp553) + (68)) == 2) || (sp551 == 0xC)) && (var_v1_2 != sp4C.b[D_800E6280.unk_72C[D_800E71DF]]) && ((D_800E71DF != ((u32) (D_800E67CC * 8) >> 0x1C)) || (var_v1_2 != (D_800E67CC & 0x1F))) && ((D_800E71DF != 1) || (((var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && (var_v1_2 != 0x10) && (var_v1_2 != 0x11) && (var_v1_2 != 0x14))) && ((D_800E71DF != 2) || (var_v1_2 != 0x14)) && ((D_800E71DF != 3) || ((var_v1_2 != 0x10) && (var_v1_2 != 0x11))) && ((D_800E71DF != 4) || (var_v1_2 != 7)) && ((D_800E71DF != 5) || (var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && ((D_800E71DF != 9) || (((var_v1_2 != 0xD) || ((sp551 != 1) && (sp551 != 3) && (sp551 != 6) && (sp551 != 0xB))) && (var_v1_2 != 0x14))) && ((D_800E71DF != 0xA) || ((var_v1_2 != 7) && (var_v1_2 != 0xB)))) {
                        temp_v0_2 = sp34.b[var_v1_2];
                        temp_a0 = (D_800E71DF * 0x13) + sp454.b;
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
        var_t7 = (u8 *)&D_800E6280 + var_v1_2;
    } while (var_v1_2 < 0x15);
    sp529 = var_t3;
    func_800AE120(D_800E7378);
    var_t3_2 = 0;
    var_a3 = 2;
    var_v1_3 = 0;
    temp_a1_2 = (func_800AE0D0() % var_t3) & 0xFF;
    var_t8_2 = sp43C;
loop_136:
    if (*var_t8_2 != 0) {
        temp_v0_3 = *(sp34.b + var_v1_3);
        temp_a0_2 = (D_800E71DF * 0x13) + sp454.b;
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
        var_t8_2 = sp43C + var_v1_3;
        if (var_v1_3 < 0x15) {
            goto loop_136;
        }
    }
    temp_v0_4 = var_a3 + sp34.b;
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
        if (temp_v0_4 == (sp34.b + 13)) {
            D_801476E0 += 1;
        }
    }
    func_80144270();
    k_reset(1);
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_801476D8 = 0;
    D_801476DC = 0;
    func_800847B8(D_800E71DF);
    func_80142760();
    switch (D_800E71DF) {
    case 0:
        D_801476CC = D_80147634;
        D_801476D0 = D_80147668;
        D_801476D4 = D_8014769C;
        break;
    case 1:
        D_801476CC = D_80147644;
        D_801476D0 = D_80147678;
        D_801476D4 = D_801476AC;
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
        D_801476D0 = D_8014767C;
        D_801476D4 = D_801476B0;
        break;
    case 6:
        D_801476CC = D_80147650;
        D_801476D0 = D_80147684;
        D_801476D4 = D_801476B8;
        if ((u32) (get_g_zyotai_s(D_800E71DF) & 0x7F) < 2U) {
            D_801476CC = D_8014764C;
            D_801476D0 = D_80147680;
            D_801476D4 = D_801476B4;
            break;
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
        D_801476D0 = D_80147694;
        D_801476D4 = D_801476C8;
        break;
    }
    func_800AE0F0(&D_800CA280, (D_801476E0 * 0x21) + sp74.b);
    func_8004284C();
}

void func_80143AE4(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80143B0C(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80143B88(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(0xFF, 1, 0);
}

void func_80143C00(void) {
    if (D_80122CDC != 0) {
        D_801476D8 = 0x17;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 0xF;
        func_80042940(0x18);
    } else {
        D_801476D8 += D_801476E0;
        func_8004284C();
    }
    check_para_limit();
}

void func_80143C94(void) {
    D_801476D8 = 0x14;
    func_8004284C();
}

void func_80143CBC(void) {
    s16 i;
    u8 a;
    u8 b;

    if (D_80122CDC != 0) {
        if (D_800E6280.unk_F5F == 2) {
            D_801476D8 = 0x18;
        } else {
            D_801476D8 = 0x17;
        }
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 0x14;
        func_80042940(0x18);
    } else {
        D_801476D8 = 0x15;
        for (i = 0; i < 0xB; i++) {
            if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_F5F != i) {
                a = D_800E6280.unk_1BC[i].unk_02 / 20;
                b = func_800AE0D0() % (u32)(a + 1);
                if ((u8)(func_80051A68(i) & 0x7F) < 2) {
                    D_800E6280.unk_1BC[i].unk_0A = D_800E6280.unk_1BC[i].unk_0A + b + 6;
                } else {
                    D_800E6280.unk_1BC[i].unk_0A = D_800E6280.unk_1BC[i].unk_0A + b + 4;
                }
            }
        }
        func_8004284C();
    }
    func_80084D3C();
}

void func_80143EA4(void) {
    if (D_80122CDC != 0) {
        if (D_800E6280.unk_F5F == 2 || D_800E6280.unk_F5F == 7 || D_800E6280.unk_F5F == 9 || D_800E6280.unk_F5F == 0xA) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80143F48);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80144198);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80142760", func_80144270);
