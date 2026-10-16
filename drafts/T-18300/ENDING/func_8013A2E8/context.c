#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_801396E0(void) {
    D_8013CAC0 = 0x80180308;
    D_8013CAC4 = 0x80180728;
    D_8013CAC8 = 0x80180BCC;
    D_8013CACC = 0x80180ED4;
    D_8013CAD0 = 0x801812E4;
    D_8013CAD4 = 0x80181648;
    D_8013CAD8 = 0x80181A84;
    D_8013CADC = 0x80181ED0;
    D_8013CAE0 = 0x80182258;
    D_8013CAE4 = 0x80182684;
    D_8013CAE8 = 0x80182AB4;
    D_8013CAEC = 0x80182C74;
    D_8013CAF0 = (s32 **) 0x801835E4;
    D_8013CAF4 = 0x80183CC4;
    D_8013CAF8 = 0x80184008;
    D_8013CAFC = 0x80180314;
    D_8013CB00 = 0x80180734;
    D_8013CB04 = 0x80180BD8;
    D_8013CB08 = 0x80180EE0;
    D_8013CB0C = 0x801812F0;
    D_8013CB10 = 0x80181654;
    D_8013CB14 = 0x80181A90;
    D_8013CB18 = 0x80181EDC;
    D_8013CB1C = 0x80182264;
    D_8013CB20 = 0x80182690;
    D_8013CB24 = 0x80182AC0;
    D_8013CB28 = 0x80182C80;
    D_8013CB2C = 0x8018360C;
    D_8013CB30 = 0x80183CD4;
    D_8013CB34 = 0x80184014;
    D_8013CB38 = 0x801803DC;
    D_8013CB3C = 0x801807FC;
    D_8013CB40 = 0x80180CA0;
    D_8013CB44 = 0x80180FA8;
    D_8013CB48 = 0x801813B8;
    D_8013CB4C = 0x8018171C;
    D_8013CB50 = 0x80181B58;
    D_8013CB54 = 0x80181FA4;
    D_8013CB58 = 0x8018232C;
    D_8013CB5C = 0x80182758;
    D_8013CB60 = 0x80182B88;
    D_8013CB64 = 0x80182D48;
    D_8013CB68 = 0x801839E4;
    D_8013CB6C = 0x80183DB8;
    D_8013CB70 = 0x801840DC;
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_8013CB74;

void func_801399B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_8013CB74;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122CD4 != 0) {
        func_80139AA0(0x404040);
    }
}

void func_80139A58(void) {
    if (D_800E6280.unk_F5F == 0xD) {
        func_80042908(7);
        func_80042940(0xF);
        return;
    }
    func_80042908(8);
}

void func_80139AA0(s32 arg0) {
    func_80139B48(-0x64, -0x50, 0xC8, 0x10, arg0, 0, 1);
    func_80139B48(-0x64, 0x40, 0xC8, 0x10, arg0, 0, 1);
    dtd_on(0);
    func_80139B48(-0x64, -0x40, 0xC8, 0x80, arg0, 0xB, 1);
    dtd_on(0xB);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139B48);

void func_80139D24(void) {
    cd_read_sectors(9, 0x80180000, 0xAE18);
    func_8004284C();
}

void func_80139D54(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    func_80041584();
    func_8004E58C();
    func_8004E500(3);
    D_80122CE4 = 0;
    D_80122CD4 = 0;
    func_8004284C();
}

void func_80139DA8(void) {
    if (D_800E6280.unk_F5F != 0xD) {
        func_800462C8(9, 0x80162000, 0x497C);
    } else {
        func_800462C8(0xA, 0x80162000, 0x4985);
    }
    func_8004284C();
}

s32 func_80139E08(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800AE0B0("DEC_BS header Error \n");
        func_800AE0B0("%x %x\n", ((u32 *)D_80125C04)[0], ((u32 *)D_80125C04)[1]);
        D_800B5920[2] = 0;
        D_800B5920[3] = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_800536AC(0x140, 0x100, 0x140, 0xF0);
    func_8004284C();
}

void func_80139EC4(void) {
    s16 i;

    D_80121874 += 4;
    if (D_80121874 >= 0x80) {
        D_80121874 = 0x80;
        func_8004284C();
    }
    for (i = 4; i < 0xA; i++) {
        /* FAKE: the comma expression preserves the brightness register.
         * Use the existing unprototyped callee view. T-9530. */
        func_80084C98(i, (0, D_80121874));
    }
}

void func_80139F3C(void) {
    s16 i;
    s32 shade;
    /* FAKE: keep the existing scalar brightness view for a1 promotion;
     * Rec24 field/pointer views use v0 and do not match. T-10010. */
    if (D_80121874 == 128) {
        func_80044750(0xD1);
        D_80122CD4 = 0;
    }
    D_80121874 -= 4;
    if (D_80121874 < 5U) {
        func_8004284C();
        D_80121874 = 0;
    }
    for (i = 4; i < 10; i++) {
        /* FAKE: comma expression retains the brightness register, as in
         * the adjacent fade-in routine. T-10010. */
        func_80084C98(i, (0, D_80121874));
    }
    shade = (((u32)(D_80121874 << 6)) >> 7) & 0xFF;
    /* FAKE: the empty test preserves the final shift register. T-10010. */
    if (1) {
    }
    func_80139AA0((shade << 16) | (shade << 8) | shade);
}

void func_8013A004(void) {
    D_801217D0[4].unk_00 = 0x02000000;
    D_801217D0[4].unk_08 = 0xC0;
    D_801217D0[4].unk_0A = 0x80;
    D_801217D0[4].unk_0E = 0;
    D_801217D0[4].unk_0F = 0x38;
    D_801217D0[4].unk_10 = 0;
    D_801217D0[4].unk_0C = 0x15;
    D_801217D0[4].unk_12 = 0x1F1;
    D_801217D0[4].unk_14 = 0;
    D_801217D0[4].unk_15 = 0;
    D_801217D0[4].unk_16 = 0;
    D_801217D0[4].unk_18 = 0;
    D_801217D0[4].unk_1A = 0;
    D_801217D0[4].unk_1C = 0x1000;
    D_801217D0[4].unk_1E = 0x1000;
    D_801217D0[4].unk_20 = 0;
    D_801217D0[4].unk_04 = -0xA0;
    D_801217D0[4].unk_06 = -0x40;
    D_800E6280.unk_10A5[4] = 0xD;
    D_801217D0[5] = D_801217D0[4];
    D_801217D0[5].unk_08 = 0x80;
    D_801217D0[5].unk_0C = 0x18;
    D_801217D0[5].unk_04 = 0x20;
    D_800E6280.unk_10A5[5] = 0xD;
    D_801217D0[6] = D_801217D0[4];
    D_801217D0[6].unk_0F = 0;
    D_801217D0[6].unk_0A = 0x38;
    D_801217D0[6].unk_06 = -0x78;
    D_800E6280.unk_10A5[6] = 1;
    D_801217D0[7] = D_801217D0[5];
    D_801217D0[7].unk_0F = 0;
    D_801217D0[7].unk_0A = 0x38;
    D_801217D0[7].unk_06 = -0x78;
    D_800E6280.unk_10A5[7] = 1;
    D_801217D0[8] = D_801217D0[4];
    D_801217D0[8].unk_06 = 0x40;
    D_801217D0[8].unk_0F = 0xB8;
    D_801217D0[8].unk_0A = 0x38;
    D_800E6280.unk_10A5[8] = 1;
    D_801217D0[9] = D_801217D0[5];
    D_801217D0[9].unk_06 = 0x40;
    D_801217D0[9].unk_0F = 0xB8;
    D_801217D0[9].unk_0A = 0x38;
    D_800E6280.unk_10A5[9] = 1;
    func_80044750(0x201);
    func_8004284C();
}

/* T-18300: the census repair (drafts/T-14500/repairs-unaligned), with its declarations; dialogue_render now comes from ENDING.h. */
/* size 0x1B */
/* T-18300: the census repair (drafts/T-14500/repairs-unaligned), with its declarations; dialogue_render now comes from ENDING.h. */
typedef struct { u8 b[27]; } UnalignedBytes27; /* size 0x1B */
extern UnalignedBytes27 D_8013C0D0;
extern s32 D_800CA280;
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
extern s32 D_8013C0EC;
extern s32 D_8013C0F0;
extern s32 D_8013C1E4;
extern s32 D_8013C1E8;
extern s32 D_8013C1EC;
extern s32 D_8013C1F0;
extern s32 D_8013C1F4;
extern s32 D_8013C1F8;
extern s32 D_8013C1FC;
extern s32 D_8013C200;
extern s32 D_8013C204;
extern s32 D_8013C208;
extern s32 D_8013C20C;
extern s32 D_8013C210;
extern s32 D_8013C214;
extern s32 D_8013C218;
extern s32 D_8013C21C;
extern s32 D_8013C220;
extern u8 D_800E6377;
extern u8 D_800E71DF;
void func_8013B640(void);
void func_8013A2E8(void) {
    s32 sp5C;
    u8 sp5B;
    s16 sp58;
    u8 sp57;
    UnalignedBytes27 sp3C;
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

    sp3C = D_8013C0D0;
    var_a0 = 0;
    var_a1 = 0xFF;
    var_v1 = 0;
    var_t4 = 0 * 4;
    do {
        temp_v0 = *(s16 *)((unsigned char *)((u8 *)&D_800E6280 + var_t4) + (258));
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
    D_80122CD0 = dialogue_render(-0x48, 0x48, 0, *(s32 *)*(s32 *)D_8013CAF0, 0, &D_8013C0EC, &D_8013C0F0, 0);
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
            temp_t6 = (u8) *(u8 *)((unsigned char *)((u8 *)&D_800E6280 + var_v1_2) + (309)) >> 1;
            if (var_a0_2 < temp_t6) {
                var_a0_2 = temp_t6 & 0xFF;
                goto block_46;
            }
        } else if (var_v1_2 == 6) {
            temp_v0_2 = *(u8 *)((unsigned char *)((u8 *)&D_800E6280 + var_v1_2) + (309));
            if (var_a0_2 < (u32) ((u32) (temp_v0_2 * 3) >> 2)) {
                var_a0_2 = (temp_v0_2 >> 1) & 0xFF;
                var_t0 = var_v1_2;
            }
        } else {
            temp_v0_3 = *(u8 *)((unsigned char *)((u8 *)&D_800E6280 + var_v1_2) + (309));
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
    D_80122CD0 = dialogue_render(-0x48, (s32) temp_t3, 0, *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0 + (var_t0 * 4)) + (4)), 0, &D_8013C1E4, &D_8013C1E8, 0);
    func_8013B640();
    temp_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (36));
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
    if (((u32) ((u32) (*(s32 *)((unsigned char *)((u8 *)&D_800E6280 + var_v0) + (1644)) << 5) >> 0x1E) >= 3U) && (D_800E6377 & 0x40) && !(D_800E6377 & 0x80)) {
        var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0 + var_v0) + (40));
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
                D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_2, 0, (s32) sp3C.b, 0, &D_8013C1F4, &D_8013C1F8, 0);
                func_8013B640();
            } else {
                D_80122CD0 = dialogue_render(-0x48, (s32) D_800CA2C0, 0, *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (108)), 0, &D_8013C1FC, &D_8013C200, 0);
                func_8013B640();
            }
            if (sp58 < 0x78) {
                if ((D_800E6377 & 1) || (D_800E6377 & 2) || (D_800E6377 & 4)) {
                    var_t1 = 0xC;
                    var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (128));
                } else {
                    var_t1 = 0xB;
                    var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (132));
                }
            } else if ((sp5B == 0) || (sp5B == 1) || (sp5B == 2) || (sp5B == 3)) {
                var_t1 = 0xD;
                var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (112));
            } else if (sp5B == 4) {
                var_t1 = 0xE;
                var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (116));
            } else if (sp5B == 5) {
                var_t1 = 0xF;
                var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (120));
            } else {
                var_t1 = 0x10;
                var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (124));
            }
        } else if (D_800E6377 & 1) {
            var_t1 = 0x14;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (84));
        } else if (D_800E6377 & 2) {
            var_t1 = 0x15;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (88));
        } else if (D_800E6377 & 4) {
            var_t1 = 0x16;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (92));
        } else if (D_800E6377 & 8) {
            var_t1 = 0x11;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (96));
        } else if (D_800E6377 & 0x10) {
            var_t1 = 0x12;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (100));
        } else {
            var_t1 = 0x13;
            var_a3 = *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (104));
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
    D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_3, 0, *(s32 *)(*(s32 *)D_800CA160 + (var_t0_3 * 4)), 0, &D_8013C20C, &D_8013C210, 0);
    func_8013B640();
    if ((D_800E71DF != 0xC) && (D_800E71DF != 0xD)) {
        temp_t6_4 = D_800CA2C0 + 0x10;
        D_800CA2C0 = temp_t6_4;
        D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_4, 0, *(s32 *)((unsigned char *)(*(s32 *)D_800CA160) + (24)), 0, &D_8013C214, &D_8013C218, 0);
        func_8013B640();
        temp_t6_5 = D_800CA2C0 + 0x10;
        D_800CA2C0 = temp_t6_5;
        D_80122CD0 = dialogue_render(-0x48, (s32) temp_t6_5, 0, *(s32 *)((unsigned char *)(*(s32 *)D_8013CAF0) + (136)), 0, &D_8013C21C, &D_8013C220, 0);
        func_8013B640();
    }
    D_800CA2C0 += 0x20;
    func_8004284C();
}

/* FAKE (T-9780): the s32 fall-through preserves the original timer result register. */
s32 func_8013B510(void) {
    s16 timer;
    s16 y;
    u32 max;
    u32 count;
    Entry8 *entry;

    if (D_800E6280.unk_10F8 & 3) {
        return 0;
    }
    max = func_8004ECF0() & 0xFF;
    count = 0;
    do {
        entry = (Entry8 *)func_8004E970(count);
        entry->unk_02--;
        y = entry->unk_02;
        count++;
        if (y >= 81 || y < -80) {
            entry->unk_07 = 0;
        } else {
            entry->unk_07 = 1;
        }
        /* FAKE (T-9780): normalize in the condition to retain the delay-slot copy. */
    } while (max >= (count = count & 0xFF));
    timer = --D_800CA2C0;
    if (timer < -64) {
        func_8004284C();
    }
}

s32 func_8013B5E4(void) {
    if ((u32)D_800E6280.unk_1104.w++ < 0x80U) {
        return 0;
    }
    func_8004E884(D_80122CD0);
    D_80122CD4 = 1;
    func_8004284C();
}

void func_8013B640(void) {
    s32 temp_v0;
    s32 var_s0;
    u32 temp_s1;
    u8 *temp_v0_2;

    temp_v0 = get_now_k();
    temp_s1 = temp_v0 & 0xFF;
    get_k_work(temp_v0);
    var_s0 = 0;
    if (temp_s1 != 0) {
        do {
            temp_v0_2 = get_k_work(var_s0);
            var_s0 = (s32) (s16) (var_s0 + 1);
            *(s8 *)((unsigned char *)temp_v0_2 + (7)) = 0;
        } while ((u32) var_s0 < temp_s1);
    }
}
