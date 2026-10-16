#include "common.h"
#include "ovl/ETC.h"

/* ETC view of shared work memory: the two six-word arrays are also walked
 * by func_801465F8. Other overlays use the first word as a scalar. T-10030. */
typedef struct EtcWork40 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 pad_04[0xC];
    /* 0x10 */ s32 unk_10[6];
    /* 0x28 */ s32 unk_28[6];
} EtcWork40; /* size 0x40 */

/* NON_MATCHING: T-10850, 14 differing listing lines; see the ticket scratch evidence. */
#ifdef NON_MATCHING
/* FAKE: unused padding array preserves the frame layout. T-10850 */
s32 func_80146400(void)
{
    s32 x;
    s32 pad[3];
    s32 *p;
    s32 a;
    s32 r;
    s32 d;
    switch (D_800E6280.unk_03E)
    {
        case 0x5F:
            x = 0;
            break;

        case 0x60:
            x = 3;
            break;

        case 0x61:
            x = 6;
            break;

        default:
            x = 6;
            break;

    }

    if (D_800E6280.unk_03F != 2)
    {
        if (D_800E6280.unk_03F != 7)
        {
            if (D_800E6280.unk_03F == 0xC)
            {
                x += 1;
            }
        }
    }
    else
    {
        x -= 1;
    }
    p = &D_801506F8[x];
    func_800AE0B0("補正 %d ", *p);
    r = func_800AE0D0() & 7;
    a = *p;
    d = r + a;
    d -= 8;
    if (d > 0)
    {
        return a;
    }
    /* FAKE: zero expression preserves the comparison live range. T-10850 */
    return (d > 0) * 0;
}
#else
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146400);
#endif

s32 func_801464D4(s32 mode) {
    u32 i;
    GameState *g;

    g = &D_800E6280;
    for (i = 0; i < 256; i++) {
        if (g->unk_75E[i].unk_04 == 0) {
            break;
        }
        if (g->unk_03E == g->unk_75E[i].unk_04 &&
            g->unk_03F == g->unk_75E[i].unk_05) {
            if (mode == 8 && g->unk_75E[i].unk_02 == 2 &&
                g->unk_75E[i].unk_01 == 0x3B) {
                return 9;
            }
            if (mode == 0xB && g->unk_75E[i].unk_02 == 0xB &&
                g->unk_75E[i].unk_01 == 0xF) {
                return 1;
            }
            if (g->unk_75E[i].unk_02 == 0 && g->unk_75E[i].unk_01 == 0xF) {
                if (mode == 0xF) {
                    return 1;
                }
                if (mode == g->unk_75E[i].unk_00) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void func_801465F8(void) {
    s32 *p;
    s32 y;

    switch (D_800E6280.unk_1109) {
    case 0:
        func_80146B40();
        break;
    case 1:
        func_80146714();
        break;
    case 2:
        func_80148078();
        break;
    }
    func_80066C08(1);
    func_80064F48();
    func_80064DEC();
    func_800646CC();
    func_8006BA40();
    func_80065B0C(1);
    if (D_800E6280.unk_041 == 5) {
        func_8006764C(0);
    } else {
        func_80067870();
    }
    func_800578F4(0);
    p = &D_801230D0;
    for (y = 0x28; y != 0x58; y += 8) {
        func_8004B19C(p[10], 0x40, y);
        func_8004B19C(p[4], 0x50, y);
        p++;
    }
}
void func_80146714(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_041) {
    case 0:
        func_80147650();
        return;
    case 1:
        func_801476DC();
        return;
    case 2:
        func_80147768();
        return;
    case 3:
        func_801477F4();
        return;
    case 4:
        func_80147880();
        return;
    case 5:
        func_80147F7C();
        /* fallthrough */
    default:
        return;
    }
}

void func_801467C4(void) {
    s32 var_s0;

    func_8004E58C();
    func_8006612C((s32) "期末テスト");
    func_80041584();
    func_80048E78();
    func_80048390();
    func_8006509C();
    func_80064FA4(1);
    func_8006CE84();
    for (var_s0 = 0; var_s0 != 14; var_s0++) {
        func_80065048(var_s0, 0);
    }
    func_800654D0(*(u8 *)&D_800E6280.unk_F6D);
    func_8006BD6C(1);
    func_800649D4();
    func_800639D8();
    func_800639D8();
    if (D_800E6280.unk_041 == 0) {
        func_80044890(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    } else if (((EtcStateFlagsView *)&D_800E6280)->records[D_800E6280.unk_71C].unk_00 != 0x40) {
        func_80044890(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
    }
    func_8004284C();
}

void func_801468FC(void) {
    s32 r2;
    s32 r;

    if (D_800E6280.unk_041 == 0 || *(u8 *) &D_800E6280.unk_66C[10 + D_800E6280.unk_71C] != 0x40) {
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0x205);
            r = func_8005742C(0x3FA4, 0);
            func_80057390(0);
            if (r != -1) {
                if (r == 0 || r == 1) {
                    func_80057418(0, r);
                    func_80057390(0x80);
                    func_80042940(3);
                }
            } else {
                func_8004284C();
            }
        }
    } else {
        if (func_8004481C() == 0) {
            func_8004500C(1, 0x205);
        }
        r2 = func_8005742C(0x3FA4, 0);
        if (r2 != -1) {
            if (r2 == 0 || r2 == 1) {
                func_80057418(0, r2);
                func_800578F4(0);
                func_80057390(0x80);
                func_80042940(3);
            }
        } else {
            func_8004284C();
        }
    }
}

void func_80146A38(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(0x3FA4, 0);
        } else {
            set_dec_bri(0x80);
            func_8004284C();
            func_800578F4(0);
        }
    }
}

void func_80146AC0(void) {
    s32 i;

    if (D_800E6280.unk_041 == 0) {
        for (i = 0; i < 6; i++) {
            ((EtcWork40 *)&D_801230D0)->unk_10[i] = 0;
            ((EtcWork40 *)&D_801230D0)->unk_28[i] = 0;
        }
    }
    func_80042808();
}

s32 func_80146B40(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801467C4();
        break;
    case 1:
        func_801468FC();
        break;
    case 2:
        func_80146A38();
        break;
    case 3:
        func_80146AC0();
        break;
    }
}

s32 func_80146BC4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        cd_read_sectors(9U, 0x801E0000, *(s32 *)&D_80150720[-(func_80066A84() * 4)]);
        D_800E6280.unk_110D = 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8007C6A8(1 - func_80066A84());
            func_8004284C();
        }
        break;
    }
}

void func_80146C68(s32 arg0) {
    func_80048F64(0x60);
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    D_80120650[1] = 1;
    D_80120650[2] = 4;
    *(s32 *)&D_80120650[0x38] = 0x01000000;
    D_80120650[3] = 0x84;
    *(u8 **)&D_80120650[0xC] = D_80125CA8;
    *(u8 **)&D_80120650[0x10] = D_80125CAC;
    *(u8 **)&D_80120650[0x34] = D_80125CB0;
    D_80120650[5] = 2;
    D_80120650[6] = 1;
    D_80120650[0x43] = 0x10;
    *(s16 *)&D_80120650[0x16] = arg0;
    *(s16 *)&D_80120650[0x26] = -0x50;
    *(s16 *)&D_80120650[0x2A] = -0x40;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146D4C);

void func_8014716C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    func_80050DFC(D_800E6280.unk_0D4);
    func_8004E884(2);
    func_80050E8C("第一日目の試験科目は 語学だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_801471F4(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    func_80050DFC(D_800E6280.unk_0D4);
    func_8004E884(2);
    func_80050E8C("第二日目の試験科目は 数学だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_8014727C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    func_80050DFC(D_800E6280.unk_0D4);
    func_8004E884(2);
    func_80050E8C("第三日目の試験科目は 理科だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_80147304(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    func_80050DFC(D_800E6280.unk_0D4);
    func_8004E884(2);
    func_80050E8C("第四日目の試験科目は 社会だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_8014738C(void) {
    func_8004E58C();
    func_8006612C("期末試験");
    func_80050DFC(D_800E6280.unk_0D4);
    func_8004E884(2);
    func_80050E8C("第五日目の試験科目は 美術だ。）", 0, 0x1F);
    if (func_80146D4C() < 2) {
        func_80146C68(1);
    } else {
        func_80146C68(0);
    }
    func_8004284C();
}

void func_80147414(void) {
    s16 *mode; /* pointer local: the switch reads the flag through it, as the permuter-found form that matches (T-6050) */
    if (D_800E6280.unk_1104.w == 0x20) {
        D_80120652 |= 1;
    }
    if (!(D_80120652 & 1) && (D_80120668 != 0)) {
        if (D_80120658 == 0) {
            mode = &D_80120666;
            switch ((*mode) & 1) {
            case 0:
                func_80044750(0x601);
                break;
            case 1:
                func_80044750(0x602);
                break;
            }
        }
        if (D_80120658++ >= 0x11) {
            func_8004284C();
        }
    }
    k_disp_inc();
}

void func_801474FC(void) {
    if (D_800E6280.unk_110D == 0) {
        k_reset(1);
        gnsx(D_800E6280.unk_0D4);
        switch (D_801230E0[D_800E6280.unk_041]) {
        case 0:
            sndisp("もうだめだー。）", 0, 0x1F);
            break;
        case 1:
            sndisp("ちょっとだめだな。）", 0, 0x1F);
            break;
        case 2:
            sndisp("まあまあのできだな。）", 0, 0x1F);
            break;
        case 3:
            sndisp("いいできだ。）", 0, 0x1F);
            break;
        case 4:
            sndisp("バッチリだ。）", 0, 0x1F);
            break;
        }
        D_800E6280.unk_110D += 1;
    } else {
        k_disp_inc();
        if (check_end_k() != 0) {
            func_8004284C();
        }
    }
}

void func_80147630(void) {
    func_80042808();
}

void func_80147650(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014716C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_801476DC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_801471F4();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_80147768(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014727C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_801477F4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_80147304();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

void func_80147880(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80146BC4();
        return;
    case 1:
        func_8014738C();
        return;
    case 2:
        func_80147414();
        return;
    case 3:
        func_801474FC();
        return;
    case 4:
        func_80147630();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014790C);

/* Bits 0 and 1 of the low byte of GameState +0xF6 (T-7010). */
typedef struct {
    u8 f0 : 1;
    u8 f1 : 1;
    u8 rest : 6;
} GsBits0F6;

void func_80147D74(void) {
    func_80148BC8();
    D_800E6280.unk_0F6.b[0] &= 0xFFFD;
    if (D_800E6280.unk_1104.u >= 0x79U) {
        func_80050DFC(D_800E6280.unk_0D4);
        if (D_801230F4 >= 3) {
            func_80050E8C("３科目以上赤点だった。 明日から、一週間補習だぁ。 勉強しかできないー！）", 0, 0x1F);
            func_8004500C(1, 0x203);
            ((GsBits0F6 *)&D_800E6280.unk_0F6.b[0])->f0 = 1;
            D_800E6280.unk_044[0].unk_20[D_800E6280.unk_040 + 1] = 0x17;
            ((GsBits0F6 *)&D_800E6280.unk_0F6.b[0])->f1 = 1;
        } else if (D_801506F0 < 2) {
            func_80050E8C("やったー、一番だ！ これで試験の帝王だな。）", 0, 0x1F);
            func_8004500C(1, 0x204);
        } else if (D_801506F4 != 0) {
            func_80050E8C("今回の試験は終了した。）", 0, 0x1F);
            ((GsBits0F6 *)&D_800E6280.unk_0F6.b[0])->f1 = 1;
            func_8004500C(1, 0x200);
        } else {
            func_80050E8C("今回の試験は終了した。）", 0, 0x1F);
            func_8004500C(1, 0x201);
        }
        func_8004EA98();
        func_8004EAD4(0);
        func_8006BD6C(0);
        func_8004284C();
    }
    func_80148E98();
    func_80148EE8();
}

void func_80147F0C(void) {
    if (((D_800E6280.unk_F88 & 0x20) && (u32)D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F88 & 0x40)) {
        func_8006BD6C(1);
        func_80042808();
    }
    func_80148E98();
    func_80148EE8();
    func_80148BC8();
}

s32 func_80147F7C(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014790C();
        break;
    case 1:
        func_80147D74();
        break;
    case 2:
        func_80147F0C();
        break;
    }
}

void func_80147FFC(void) {
    parameter_show_init();
    parameter_show_init();
    func_80048E78();
    func_8004E58C();
    func_8004284C();
}

void func_8014803C(void) {
    if (D_800E6280.unk_041 == 5) {
        func_8004500C(0, 0);
    }
    func_80072B5C(0);
}

void func_80148078(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80147FFC();
        break;
    case 1:
        func_8014803C();
        break;
    }
    func_800646CC();
}
/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_800E6476;
extern s16 D_800E64AE;
extern s16 D_80150668;
extern s16 D_8015066A;
extern s16 D_80150682;
extern s32 D_800E6328;
extern s32 D_800E64E8;
extern s32 D_800E7378;
extern s32 D_8012310C;
extern s32 D_8015066C;
extern s32 D_80150684;
extern s32 D_80150688;
extern s32 D_801506BC;
extern s32 D_801506C0;
extern s32 D_801506C4;
extern u8 D_80150728;
extern u8 D_80150738;
/* end bulk_m2c declarations */
void func_801480D0(void) {
    s32 spBC;
    s32 spAC;
    s32 spA4;
    s32 spA0[1];
    s32 sp70[1];
    s32 sp6C;
    s32 sp68;
    u8 *sp54;
    s32 *var_s0_4;
    s32 *var_s1_5;
    s32 *var_s1_6;
    s32 *var_s1_7;
    s32 *var_v0;
    s32 *var_v0_3;
    s16 *temp_a1;
    s16 *var_s0;
    s16 *var_s0_2;
    s16 *var_s0_3;
    s16 *var_s0_6;
    s16 *var_s0_8;
    s16 *var_s0_9;
    s16 temp_a2_2;
    s16 temp_a2_3;
    s16 temp_a2_5;
    s16 temp_t3_3;
    s16 temp_t4;
    s16 temp_t4_4;
    s16 temp_t6_4;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t8_5;
    s16 temp_t9;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_3;
    s16 var_a0_4;
    s32 *temp_a2_4;
    s32 *temp_v1_6;
    s32 *var_a1;
    s32 *var_s0_5;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 *var_s1_4;
    s32 *var_s1_8;
    s32 *var_s1_9;
    s32 *var_v0_2;
    s32 temp_a2;
    s32 temp_a2_6;
    s32 temp_a3;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_t0;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t4_2;
    s32 temp_t4_3;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1_10;
    s32 temp_v1_4;
    s32 temp_v1_5;
    s32 temp_v1_7;
    s32 temp_v1_8;
    s32 temp_v1_9;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_at;
    s32 var_s0_7;
    s32 var_s2;
    s32 var_s2_10;
    s32 var_s2_2;
    s32 var_s2_3;
    s32 var_s2_4;
    s32 var_s2_5;
    s32 var_s2_6;
    s32 var_s2_7;
    s32 var_s2_8;
    s32 var_s2_9;
    s32 var_s3_2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t0_3;
    u8 *var_s3;
    u8 *temp_v1_11;

    func_800AE120(D_800E7378);
    var_s3 = &D_80150728;
    var_s0 = &D_80150668;
    sp54 = &D_80150738;
    var_s2 = 0;
    do {
        temp_s1 = func_800AE0D0();
        spBC = (((func_800AE0D0() & 1) * 2) - 1) * (temp_s1 % (s32) *sp54);
        temp_s1_2 = func_800AE0D0();
        *var_s0 = ((*var_s3 + ((((func_800AE0D0() & 1) * 2) - 1) * (temp_s1_2 % (s32) *sp54))) * 5) + spBC;
        temp_v0 = func_801464D4(var_s2);
        if (temp_v0 != 1) {
            if (temp_v0 == 9) {
                *var_s0 += func_800AE0D0() % 250;
            }
        } else {
            *var_s0 += func_800AE0D0() % 50;
        }
        if (*var_s0 >= 0x1F5) {
            *var_s0 = 0x1F4;
        }
        func_800AE0B0("[%d:%d]\n", var_s2, *var_s0);
        var_s2 += 1;
        sp54 += 1;
        var_s0 += 2;
        var_s3 += 1;
    } while (var_s2 != 0xD);
    D_80150682 = (s16) D_8012310C;
    var_a0 = 0;
    do {
        var_a3 = 0;
        var_t0 = 1;
loop_10:
        var_s2_2 = var_t0;
        if (var_t0 < 0xE) {
            temp_a2 = (0xE - var_t0) & 3;
            temp_a1 = &(&D_80150668)[var_a3];
            if (temp_a2 != 0) {
                var_s0_2 = &(&D_80150668)[var_s2_2];
                do {
                    temp_a2_2 = *var_s0_2;
                    var_s2_2 += 1;
                    if (temp_a2_2 == *temp_a1) {
                        var_a0 += 1;
                        *var_s0_2 = temp_a2_2 - 1;
                    }
                    var_s0_2 += 2;
                } while ((temp_a2 + var_t0) != var_s2_2);
                if (var_s2_2 != 0xE) {
                    goto block_17;
                }
            } else {
block_17:
                var_s0_3 = &(&D_80150668)[var_s2_2];
                do {
                    temp_a2_3 = (*(s32 *)var_s0_3);
                    if (temp_a2_3 == *temp_a1) {
                        (*(s32 *)var_s0_3) = temp_a2_3 - 1;
                        var_a0 += 1;
                    }
                    temp_v1 = *(s16 *)((unsigned char *)var_s0_3 + (2));
                    if (temp_v1 == *temp_a1) {
                        *(s16 *)((unsigned char *)var_s0_3 + (2)) = (s16) (temp_v1 - 1);
                        var_a0 += 1;
                    }
                    temp_v1_2 = *(s16 *)((unsigned char *)var_s0_3 + (4));
                    if (temp_v1_2 == *temp_a1) {
                        *(s16 *)((unsigned char *)var_s0_3 + (4)) = (s16) (temp_v1_2 - 1);
                        var_a0 += 1;
                    }
                    temp_v1_3 = *(s16 *)((unsigned char *)var_s0_3 + (6));
                    if (temp_v1_3 == *temp_a1) {
                        var_a0 += 1;
                        *(s16 *)((unsigned char *)var_s0_3 + (6)) = (s16) (temp_v1_3 - 1);
                    }
                    var_s0_3 += 8;
                } while (var_s0_3 != &D_80150684);
            }
        }
        var_a3 = var_t0;
        var_t0 += 1;
        if (var_a3 < 0xE) {
            goto loop_10;
        }
        var_a0 = 0;
    } while (var_a0 != 0);
    var_s0_4 = &D_8015066C;
    var_v0 = &sp70;
    sp6C = (s32) D_8015066A;
    sp68 = (s32) D_80150668;
    do {
        temp_t4 = *(s16 *)((unsigned char *)var_s0_4 + (2));
        temp_t9 = *(s16 *)((unsigned char *)var_s0_4 + (4));
        temp_t8 = *(s16 *)((unsigned char *)var_s0_4 + (6));
        temp_t7 = (*(s32 *)var_s0_4);
        var_v0 += 0x10;
        var_s0_4 += 8;
        *(s32 *)((unsigned char *)var_v0 + (-12)) = (s32) temp_t4;
        *(s32 *)((unsigned char *)var_v0 + (-8)) = (s32) temp_t9;
        *(s32 *)((unsigned char *)var_v0 + (-4)) = (s32) temp_t8;
        *(s32 *)((unsigned char *)var_v0 + (-16)) = (s32) temp_t7;
    } while (var_v0 != &spA0);
    var_a1 = &D_80150684;
    var_s3_2 = 0;
    do {
        var_a0_2 = sp68;
        var_s2_3 = 1;
        var_v0_2 = &sp6C;
loop_33:
        temp_v1_4 = *var_v0_2;
        if (var_a0_2 < temp_v1_4) {
            var_a0_2 = temp_v1_4;
            var_s3_2 = var_s2_3;
        }
        var_s2_3 += 1;
        var_v0_2 += 4;
        if (var_s2_3 < 0xE) {
            goto loop_33;
        }
        var_a1 += 4;
        (&sp68)[var_s3_2] = 0;
        *(s32 *)((unsigned char *)var_a1 + (-4)) = var_s3_2;
        var_s3_2 = 0;
    } while ((u32) var_a1 < (u32) &D_801506BC);
    func_800AE0B0("順位計算\n ", var_a1, &sp68, &D_801506BC);
    var_s0_5 = &D_80150684;
    var_s2_4 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_4, *var_s0_5);
        var_s2_4 += 1;
        var_s0_5 += 4;
    } while (var_s2_4 < 0xE);
    func_800AE0B0("順位作成\n ");
    var_t0_2 = func_800AE0D0() & 3;
    var_s1 = &D_801506BC;
    var_s0_6 = &D_80150668;
    var_s2_5 = 0;
    do {
        temp_lo = (s32) (*var_s0_6 << 0xC) / 1000;
        spAC = var_t0_2;
        spA4 = temp_lo;
        temp_v0_2 = func_800A0140(temp_lo);
        temp_v1_5 = ((s32) (temp_v0_2 * 0xC8) / 4096) + var_t0_2 + 0xC8;
        *var_s1 = temp_v1_5;
        func_800AE0B0("[%d:%d %d %d] \n", var_s2_5, spA4, temp_v0_2, temp_v1_5);
        var_s2_5 += 1;
        var_s0_6 += 2;
        var_s1 += 4;
    } while (var_s2_5 != 0xE);
    var_a0_3 = 0;
    do {
        var_a3_2 = 0;
        var_t0_3 = 1;
loop_43:
        var_s2_6 = var_t0_3;
        if (var_t0_3 < 0xE) {
            temp_a2_4 = &(&D_801506BC)[var_a3_2];
            if ((0xE - var_t0_3) & 1) {
                temp_v1_6 = &(&D_801506BC)[var_t0_3];
                temp_v0_3 = *temp_v1_6;
                var_s2_6 = var_t0_3 + 1;
                if (temp_v0_3 == *temp_a2_4) {
                    var_a0_3 += 1;
                    if ((&D_80150668)[var_t0_3] < (&D_80150668)[var_a3_2]) {
                        *temp_v1_6 = temp_v0_3 + 1;
                    } else {
                        *temp_v1_6 = temp_v0_3 - 1;
                    }
                }
                if (var_s2_6 != 0xE) {
                    goto block_50;
                }
            } else {
block_50:
                var_s1_2 = &(&D_801506BC)[var_s2_6];
                do {
                    temp_v1_7 = (*(s32 *)var_s1_2);
                    if (temp_v1_7 == *temp_a2_4) {
                        var_a0_3 += 1;
                        if ((&D_80150668)[var_s2_6].unk0 < (&D_80150668)[var_a3_2]) {
                            (*(s32 *)var_s1_2) = temp_v1_7 + 1;
                        } else {
                            (*(s32 *)var_s1_2) = temp_v1_7 - 1;
                        }
                    }
                    temp_v1_8 = *(s32 *)((unsigned char *)var_s1_2 + (4));
                    if (temp_v1_8 == *temp_a2_4) {
                        var_a0_3 += 1;
                        if (*(s16 *)((unsigned char *)(&(&D_80150668)[var_s2_6]) + (2)) < (&D_80150668)[var_a3_2]) {
                            *(s32 *)((unsigned char *)var_s1_2 + (4)) = (s32) (temp_v1_8 + 1);
                        } else {
                            *(s32 *)((unsigned char *)var_s1_2 + (4)) = (s32) (temp_v1_8 - 1);
                        }
                    }
                    var_s2_6 += 2;
                    var_s1_2 += 8;
                } while (var_s2_6 != 0xE);
            }
        }
        var_a3_2 = var_t0_3;
        var_t0_3 += 1;
        if (var_a3_2 < 0xE) {
            goto loop_43;
        }
        var_a0_3 = 0;
    } while (var_a0_3 != 0);
    func_800AE0B0("順位\x83\\ート\n ", &D_80150668);
    var_s1_3 = &D_801506BC;
    var_s2_7 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_7, *var_s1_3);
        var_s2_7 += 1;
        var_s1_3 += 4;
    } while (var_s2_7 < 0xE);
    var_s0_7 = 0x3E8;
    var_s1_4 = &D_801506BC;
    do {
        temp_v1_9 = *var_s1_4;
        var_s1_4 += 4;
        var_at = (u32) var_s1_4 < (u32) &D_801506F4;
        if (temp_v1_9 < var_s0_7) {
            var_s0_7 = temp_v1_9;
            var_at = (u32) var_s1_4 < (u32) &D_801506F4;
        }
    } while (var_at != 0);
    if (var_s0_7 >= 9) {
        var_s1_5 = &D_801506C4;
        temp_v1_10 = ((func_800AE0D0() % (s32) (var_s0_7 - 8)) + var_s0_7) - 8;
        D_801506BC -= temp_v1_10;
        D_801506C0 -= temp_v1_10;
        do {
            temp_t3 = *(s32 *)((unsigned char *)var_s1_5 + (4));
            temp_t4_2 = *(s32 *)((unsigned char *)var_s1_5 + (8));
            temp_t6 = *(s32 *)((unsigned char *)var_s1_5 + (12));
            temp_t7_2 = (*(s32 *)var_s1_5) - temp_v1_10;
            var_s1_5 += 0x10;
            *(s32 *)((unsigned char *)var_s1_5 + (-4)) = (s32) (temp_t6 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-8)) = (s32) (temp_t4_2 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-12)) = (s32) (temp_t3 - temp_v1_10);
            *(s32 *)((unsigned char *)var_s1_5 + (-16)) = temp_t7_2;
        } while (var_s1_5 != &D_801506F4);
    } else if (var_s0_7 <= 0) {
        temp_v0_4 = 1 - var_s0_7;
        D_801506BC += temp_v0_4;
        D_801506C0 += temp_v0_4;
        var_s1_6 = &D_801506C4;
        do {
            temp_t7_3 = *(s32 *)((unsigned char *)var_s1_6 + (4));
            temp_t9_2 = *(s32 *)((unsigned char *)var_s1_6 + (8));
            temp_t5 = *(s32 *)((unsigned char *)var_s1_6 + (12));
            temp_t8_2 = (*(s32 *)var_s1_6) + temp_v0_4;
            var_s1_6 += 0x10;
            *(s32 *)((unsigned char *)var_s1_6 + (-4)) = (s32) (temp_t5 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-8)) = (s32) (temp_t9_2 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-12)) = (s32) (temp_t7_3 + temp_v0_4);
            *(s32 *)((unsigned char *)var_s1_6 + (-16)) = temp_t8_2;
        } while (var_s1_6 != &D_801506F4);
    }
    var_a0_4 = D_80150668;
    var_s0_8 = &D_8015066A;
    var_s2_8 = 1;
    do {
        temp_a2_5 = *var_s0_8;
        if (var_a0_4 < temp_a2_5) {
            var_a0_4 = temp_a2_5;
            var_s3_2 = var_s2_8;
        }
        var_s2_8 += 1;
        var_s0_8 += 2;
    } while (var_s2_8 < 0xE);
    if (var_a0_4 >= 0x1D2) {
        temp_t0 = (&D_801506BC)[var_s3_2] - 1;
        D_801506BC -= temp_t0;
        D_801506C0 -= temp_t0;
        var_s1_7 = &D_801506C4;
        do {
            temp_t8_3 = *(s32 *)((unsigned char *)var_s1_7 + (4));
            temp_t3_2 = *(s32 *)((unsigned char *)var_s1_7 + (8));
            temp_t4_3 = *(s32 *)((unsigned char *)var_s1_7 + (12));
            temp_t6_2 = (*(s32 *)var_s1_7) - temp_t0;
            var_s1_7 += 0x10;
            *(s32 *)((unsigned char *)var_s1_7 + (-4)) = (s32) (temp_t4_3 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-8)) = (s32) (temp_t3_2 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-12)) = (s32) (temp_t8_3 - temp_t0);
            *(s32 *)((unsigned char *)var_s1_7 + (-16)) = temp_t6_2;
        } while (var_s1_7 != &D_801506F4);
    }
    func_800AE0B0("順位確定\n ");
    var_s1_8 = &D_801506BC;
    var_s2_9 = 0;
    do {
        func_800AE0B0("[%d:%d] \n", var_s2_9, *var_s1_8);
        var_s2_9 += 1;
        var_s1_8 += 4;
    } while (var_s2_9 < 0xE);
    var_s2_10 = 0;
    if ((D_80150684 == 0xD) && (D_80150688 == 0)) {
        temp_t8_4 = D_801506BC;
        if (temp_t8_4 < D_801506F0) {
            D_801506BC = D_801506F0;
            D_801506F0 = temp_t8_4;
        }
    }
    temp_a2_6 = D_801506F0;
    if ((D_80150684 == 0xD) && (temp_a2_6 <= 0)) {
        temp_a3 = 1 - temp_a2_6;
        D_801506BC += temp_a3;
        var_s1_9 = &D_801506C0;
        D_801506F0 = 1;
        do {
            temp_t6_3 = *(s32 *)((unsigned char *)var_s1_9 + (4));
            temp_t7_4 = *(s32 *)((unsigned char *)var_s1_9 + (8));
            temp_t9_3 = *(s32 *)((unsigned char *)var_s1_9 + (12));
            temp_t5_2 = (*(s32 *)var_s1_9) + temp_a3;
            var_s1_9 += 0x10;
            *(s32 *)((unsigned char *)var_s1_9 + (-4)) = (s32) (temp_t9_3 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-8)) = (s32) (temp_t7_4 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-12)) = (s32) (temp_t6_3 + temp_a3);
            *(s32 *)((unsigned char *)var_s1_9 + (-16)) = temp_t5_2;
        } while (var_s1_9 != &D_801506F0);
    }
    var_s0_9 = &D_80150668;
    do {
        temp_v1_11 = &D_800E6280 + var_s2_10;
        if (*var_s0_9 < D_80150682) {
            if (*(u8 *)((unsigned char *)temp_v1_11 + (1588)) == 0) {
                D_800E6280.unk_1BC[var_s2_10].unk_02 += 0xB;
                *(u8 *)((unsigned char *)temp_v1_11 + (1588)) = 1U;
            } else {
                D_800E6280.unk_1BC[var_s2_10].unk_02 += 2;
            }
        }
        var_s2_10 += 1;
        var_s0_9 += 2;
    } while (var_s2_10 < 0xB);
    D_801506F0 = temp_a2_6;
    if (temp_a2_6 == 1) {
        D_800E643E += 2;
        D_800E6476 += 2;
        D_800E64AE += 2;
        var_v0_3 = &D_800E6328;
        do {
            temp_t8_5 = *(s16 *)((unsigned char *)var_v0_3 + (502));
            temp_t3_3 = *(s16 *)((unsigned char *)var_v0_3 + (558));
            temp_t4_4 = *(s16 *)((unsigned char *)var_v0_3 + (614));
            temp_t6_4 = *(s16 *)((unsigned char *)var_v0_3 + (446)) + 2;
            var_v0_3 += 0xE0;
            *(s16 *)((unsigned char *)var_v0_3 + (390)) = (s16) (temp_t4_4 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (334)) = (s16) (temp_t3_3 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (278)) = (s16) (temp_t8_5 + 2);
            *(s16 *)((unsigned char *)var_v0_3 + (222)) = temp_t6_4;
        } while (var_v0_3 != &D_800E64E8);
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80148BC8);

void func_80148E98(void) {
    func_80049A40(-0xA0, -0x68, 0x140, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void func_80148EE8(void) {
    func_80049A40(-0x41, -0x70, 0x84, 0x12, 4, 0x30E00, 0);
    dtd_on(4);
}

void func_80148F34(void) {
    RECT rect;

    rect.x = 0x100;
    rect.y = 0x1F0;
    rect.w = 0x10;
    rect.h = 2;
    func_8009C884(&rect, &D_80150754);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80148F7C);
