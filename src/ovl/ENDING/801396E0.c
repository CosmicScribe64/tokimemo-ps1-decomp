#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"
#include "psx_types.h"

void func_801396E0(void) {
    D_8013CAC0[0][0] = 0x80180308;
    D_8013CAC0[0][1] = 0x80180728;
    D_8013CAC0[0][2] = 0x80180BCC;
    D_8013CAC0[0][3] = 0x80180ED4;
    D_8013CAC0[0][4] = 0x801812E4;
    D_8013CAC0[0][5] = 0x80181648;
    D_8013CAC0[0][6] = 0x80181A84;
    D_8013CAC0[0][7] = 0x80181ED0;
    D_8013CAC0[0][8] = 0x80182258;
    D_8013CAC0[0][9] = 0x80182684;
    D_8013CAC0[0][10] = 0x80182AB4;
    D_8013CAC0[0][11] = 0x80182C74;
    D_8013CAC0[0][12] = 0x801835E4;
    D_8013CAC0[0][13] = 0x80183CC4;
    D_8013CAC0[0][14] = 0x80184008;
    D_8013CAC0[1][0] = 0x80180314;
    D_8013CAC0[1][1] = 0x80180734;
    D_8013CAC0[1][2] = 0x80180BD8;
    D_8013CAC0[1][3] = 0x80180EE0;
    D_8013CAC0[1][4] = 0x801812F0;
    D_8013CAC0[1][5] = 0x80181654;
    D_8013CAC0[1][6] = 0x80181A90;
    D_8013CAC0[1][7] = 0x80181EDC;
    D_8013CAC0[1][8] = 0x80182264;
    D_8013CAC0[1][9] = 0x80182690;
    D_8013CAC0[1][10] = 0x80182AC0;
    D_8013CAC0[1][11] = 0x80182C80;
    D_8013CAC0[1][12] = 0x8018360C;
    D_8013CAC0[1][13] = 0x80183CD4;
    D_8013CAC0[1][14] = 0x80184014;
    D_8013CAC0[2][0] = 0x801803DC;
    D_8013CAC0[2][1] = 0x801807FC;
    D_8013CAC0[2][2] = 0x80180CA0;
    D_8013CAC0[2][3] = 0x80180FA8;
    D_8013CAC0[2][4] = 0x801813B8;
    D_8013CAC0[2][5] = 0x8018171C;
    D_8013CAC0[2][6] = 0x80181B58;
    D_8013CAC0[2][7] = 0x80181FA4;
    D_8013CAC0[2][8] = 0x8018232C;
    D_8013CAC0[2][9] = 0x80182758;
    D_8013CAC0[2][10] = 0x80182B88;
    D_8013CAC0[2][11] = 0x80182D48;
    D_8013CAC0[2][12] = 0x801839E4;
    D_8013CAC0[2][13] = 0x80183DB8;
    D_8013CAC0[2][14] = 0x801840DC;
}

void func_801399B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl14 tbl;

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

/* FAKE: the explicit "& 0xFF" masks and one source line per colour trio, as in the matched main sibling
 * _sprite_set_box_shade_tarao (T-17001; one colour instead of two): the masks leave the shifts in ugen
 * temporaries (t6, t7) and as1 schedules the stores by source line. The real source is unknown. T-20930 */
void func_80139B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    GameState *gs;
    POLY_G4 *p;

    p = (POLY_G4 *)GetWorkBase(0x24, D_8011ECA0);
    func_8009F054(p);
    SetSemiTrans(p, arg6);
    gs = &D_800E6280;
    p->r0 = (arg4 >> 16) & 0xFF; p->g0 = (arg4 >> 8) & 0xFF; p->b0 = arg4 & 0xFF;
    p->r1 = (arg4 >> 16) & 0xFF; p->g1 = (arg4 >> 8) & 0xFF; p->b1 = arg4 & 0xFF;
    p->r2 = (arg4 >> 16) & 0xFF; p->g2 = (arg4 >> 8) & 0xFF; p->b2 = arg4 & 0xFF;
    p->r3 = (arg4 >> 16) & 0xFF; p->g3 = (arg4 >> 8) & 0xFF; p->b3 = arg4 & 0xFF;
    p->x0 = gs->unk_014[D_8011ECA0] + arg0;
    p->y0 = gs->unk_018[D_8011ECA0] + arg1;
    p->x1 = gs->unk_014[D_8011ECA0] + arg0 + arg2;
    p->y1 = gs->unk_018[D_8011ECA0] + arg1;
    p->x2 = gs->unk_014[D_8011ECA0] + arg0;
    p->y2 = gs->unk_018[D_8011ECA0] + arg1 + arg3;
    p->x3 = gs->unk_014[D_8011ECA0] + arg0 + arg2;
    p->y3 = gs->unk_018[D_8011ECA0] + arg1 + arg3;
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg5 * 4, (s32)p);
}

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

/* NON_MATCHING: T-23002, 12 differing lines (6 rows): the original fills the load-delay slot after `lbu a0` of the first flag test with the `lui a1` of the first string (arm 1 then reads `lui a0; addiu a0; jal; addiu a1`), here the slot stays a nop and the arm keeps `lui a1` (viewsweep, autotweak, 240 s of permuter, local and pointer forms of the first call: no gain). Binasm probes: ugen's chain is the plain order, and as1 builds the original's slot and moves `andi t8` from this same chain once about 240 instructions or the last 14 calls after the join are deleted, so the cause is a whole-function as1 limit, not the chain's C */
#ifdef NON_MATCHING
s32 func_8013B640(void); /* the callers read its (unset) $v0: it blocks $v0 in the next block (selector in $a0) */

void func_8013A2E8(void) {
    u32 top;
    s16 i;
    s16 idx;
    u8 sel;
    s32 pad; /* FAKE (T-23002): unused word, the frame has 16 bytes above msg and the declared locals fill 13 */
    s32 msg;
    u8 bestGirl;
    s16 best;
    u8 kind;
    u8 name[27] = "M残念ながら 駄目だったが、";

    best = 0;
    bestGirl = 0xFF;
    for (i = 0; i < 7; i++) {
        /* FAKE (T-23002): the seven GsRec0FC records at +0x100 are separate members; indexed until their aggregate is known */
        if (best < (&D_800E6280.unk_100)[i].unk_02) {
            best = (&D_800E6280.unk_100)[i].unk_02;
            bestGirl = i;
        }
    }
    func_801396E0();
    D_80122CD0 = dialogue_render(-0x48, 0x48, 0, (*D_8013CAF0)[0], 0, "", "", 0);
    func_8013B640();
    if (D_800E6280.unk_0F6.b[1] & 1) {
        func_800AE0F0(D_800CA280, "一流大学");
    } else if (D_800E6280.unk_0F6.b[1] & 2) {
        func_800AE0F0(D_800CA280, "二流大学");
    } else if (D_800E6280.unk_0F6.b[1] & 4) {
        func_800AE0F0(D_800CA280, "三流大学");
    } else if (D_800E6280.unk_0F6.b[1] & 8) {
        func_800AE0F0(D_800CA280, "一流企業");
    } else if (D_800E6280.unk_0F6.b[1] & 0x10) {
        func_800AE0F0(D_800CA280, "二流企業");
    } else if (D_800E6280.unk_0F6.b[1] & 0x20) {
        func_800AE0F0(D_800CA280, "三流企業");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[0])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "小説家は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[1])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "俳優は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[2])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "製薬会社は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[3])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "有名ゲーム会社は ");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[4])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "画家は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[5])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "音楽家は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[6])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "プロ野球選手は");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[7])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "Ｊリーガーは");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[8])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "プロテニスプレイヤーは ");
    } else if (((ScheduleClubFlags *)&D_800E6280.unk_66C[9])->unk_25 >= 3) {
        func_800AE0F0(D_800CA280, "水泳の実業団は");
    } else {
        func_800AE0F0(D_800CA280, "プロバスケプレイヤーは ");
    }
    top = 0;
    idx = 0;
    for (i = 0; i < 8; i++) {
        if (i == 7) {
            if (top < (D_800E6280.unk_120[0x15 + i] >> 1)) {
                top = (D_800E6280.unk_120[0x15 + i] >> 1) & 0xFF;
                idx = i;
            }
        } else if (i == 6) {
            if (top < (D_800E6280.unk_120[0x15 + i] * 3 >> 2)) {
                top = (D_800E6280.unk_120[0x15 + i] >> 1) & 0xFF;
                idx = i;
            }
        } else {
            if (top < D_800E6280.unk_120[0x15 + i]) {
                top = D_800E6280.unk_120[0x15 + i] & 0xFF;
                idx = i;
            }
        }
    }
    msg = (*D_8013CAF0)[idx + 1];
    D_800CA2C0 += 0x10;
    D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    msg = (*D_8013CAF0)[9];
    D_800CA2C0 += 0x10;
    D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    idx = 0xFF;
    for (i = 0; i < 11; i++) {
        if (((ScheduleClubFlags *)&D_800E6280.unk_66C[i])->unk_25 >= 3 && (D_800E6280.unk_0F6.b[1] & 0x40) && !(D_800E6280.unk_0F6.b[1] & 0x80)) {
            msg = (*D_8013CAF0)[10 + i];
            kind = i;
            idx = i;
            break;
        }
    }
    D_800CA2C0 += 0x10;
    if (idx == 0xFF) {
        if (D_800E6280.unk_0F6.b[1] & 0x80) {
            if (D_800E6280.unk_0F6.b[1] & 0x40) {
                D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, (s32)name, 0, "", "", 0);
                func_8013B640();
            } else {
                msg = (*D_8013CAF0)[27];
                D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
                func_8013B640();
            }
            if (best < 0x78) {
                if ((D_800E6280.unk_0F6.b[1] & 1) || (D_800E6280.unk_0F6.b[1] & 2) || (D_800E6280.unk_0F6.b[1] & 4)) {
                    kind = 0xC;
                    msg = (*D_8013CAF0)[32];
                } else {
                    kind = 0xB;
                    msg = (*D_8013CAF0)[33];
                }
            } else if (bestGirl == 0 || bestGirl == 1 || bestGirl == 2 || bestGirl == 3) {
                kind = 0xD;
                msg = (*D_8013CAF0)[28];
            } else if (bestGirl == 4) {
                kind = 0xE;
                msg = (*D_8013CAF0)[29];
            } else if (bestGirl == 5) {
                kind = 0xF;
                msg = (*D_8013CAF0)[30];
            } else {
                kind = 0x10;
                msg = (*D_8013CAF0)[31];
            }
        } else if (D_800E6280.unk_0F6.b[1] & 1) {
            kind = 0x14;
            msg = (*D_8013CAF0)[21];
        } else if (D_800E6280.unk_0F6.b[1] & 2) {
            kind = 0x15;
            msg = (*D_8013CAF0)[22];
        } else if (D_800E6280.unk_0F6.b[1] & 4) {
            kind = 0x16;
            msg = (*D_8013CAF0)[23];
        } else if (D_800E6280.unk_0F6.b[1] & 8) {
            kind = 0x11;
            msg = (*D_8013CAF0)[24];
        } else if (D_800E6280.unk_0F6.b[1] & 0x10) {
            kind = 0x12;
            msg = (*D_8013CAF0)[25];
        } else {
            kind = 0x13;
            msg = (*D_8013CAF0)[26];
        }
    }
    D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    sel = D_800E6280.unk_F5F;
    switch (sel) {
    case 0:
        if (kind == 0x14) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][1];
        break;
    case 1:
        if (kind == 0x14) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][6];
        break;
    case 2:
        if (kind == 0x11) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x12 || kind == 0x13 || kind == 0xE || kind == 0xF || kind == 0x10) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][2];
        break;
    case 3:
        if (kind == 4) {
            idx = 0;
        } else if (kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][4];
        break;
    case 4:
        if (kind == 0xD) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][10];
        break;
    case 5:
        if (kind == 0x15) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][7];
        break;
    case 6:
        if (kind == 9) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][8];
        break;
    case 7:
        if (kind == 0xF) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16 || kind == 0xD || kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][5];
        break;
    case 8:
        if (kind == 0xB) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 0xF;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][0];
        break;
    case 9:
        if (kind == 0x12) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x11 || kind == 0x13 || kind == 0xE || kind == 0xF || kind == 0x10) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        D_800CA160 = D_8013CAC0[0][9];
        break;
    case 10:
        if (kind == 0x14 || kind == 0x15 || kind == 0x16) {
            idx = 0;
        } else if (kind == 0x11 || kind == 0x12 || kind == 0x13) {
            idx = 1;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xF) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 7;
        }
        D_800CA160 = D_8013CAC0[0][13];
        break;
    case 11:
        if (kind == 0 || kind == 1 || kind == 2 || kind == 3 || kind == 6 || kind == 7 || kind == 8 || kind == 9) {
            idx = 1;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 0;
        }
        D_800CA160 = D_8013CAC0[0][14];
        break;
    case 12:
        idx = 0;
        D_800CA160 = D_8013CAC0[0][3];
        break;
    default:
        D_800CA160 = D_8013CAC0[0][11];
        idx = 0;
        break;
    }
    msg = (*(s32 **)D_800CA160)[idx];
    D_800CA2C0 += 0x10;
    D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    sel = D_800E6280.unk_F5F;
    if (sel != 0xC && sel != 0xD) {
        msg = (*(s32 **)D_800CA160)[6];
        D_800CA2C0 += 0x10;
        D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
        func_8013B640();
        msg = (*D_8013CAF0)[34];
        D_800CA2C0 += 0x10;
        D_80122CD0 = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
        func_8013B640();
    }
    D_800CA2C0 += 0x20;
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A2E8);
#endif

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

/* FAKE (T-23002): s32 with no return, so the callers (func_8013A2E8) see a live $v0 after the call; the real return type is unknown. */
s32 func_8013B640(void) {
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
