#include "common.h"
#include "ovl/ETC.h"

s32 func_80143240(void) {
    s32 i;
    u8 *p;

    if (D_800E7D11[0x13] == 0 && D_800E6280.unk_53C[0] == 0) {
        return 0;
    }
    i = 1;
    /* FAKE: `do {` on the line of the pointer init; as1 schedules by source line and only this layout orders the lui/addiu pair like the original. T-8030 */
    p = D_800E7D11; do {
        if (p[0x14] == 0 && D_800E6280.unk_53C[i] == 0) {
            return 0;
        }
        if (p[0x15] == 0 && D_800E6280.unk_53C[i + 1] == 0) {
            return 0;
        }
        if (p[0x16] == 0 && D_800E6280.unk_53C[i + 2] == 0) {
            return 0;
        }
        if (p[0x17] == 0 && D_800E6280.unk_53C[i + 3] == 0) {
            return 0;
        }
        i += 4;
        p += 4;
    } while (i != 0xD);
    return 1;
}

void func_80143334(void) {
    D_80150144 = 0;
    func_80143758();
    if (D_8015014C[0] == 1) {
        draw2d3d(1, 1);
        func_80061710();
        func_80061790();
        func_800618B0();
        func_8014354C();
        func_80042808();
        return;
    }
    func_80044774(0);
    func_80046290(0x07000001, 0x02000E0C, 0xF);
    func_80044750(0x300);
    func_8014407C();
    func_80042808();
    func_80042940(0xFF);
}

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} EtcVec2; /* VECTOR layout */

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
} EtcMat2; /* MATRIX layout, size 0x20 */

void func_801433E4(void) {
    s32 pad1[2]; /* FAKE: two unused 8-byte locals (above m1 and below scale) give the original frame 0x88, as in func_8013FA7C; real source unknown. T-9110 */
    EtcMat2 m1;
    EtcMat2 m2;
    EtcVec2 scale;
    s32 pad2[2];

    m1 = *(EtcMat2 *)D_80125C60;
    m2 = *(EtcMat2 *)D_80125C60;
    func_800A09D0(&D_8015013C, &m1);
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    func_800A0F4C(&m2, &scale);
    func_800A0C64(&m1, &m2);
    m1.t[0] = D_80150130;
    m1.t[1] = D_80150134;
    m1.t[2] = D_80150138;
    *(EtcMat2 *)D_801227A4 = m1;
    D_801227A0->unk0 = 0;
}

/* bulk_m2c declarations (T-11190) */
extern TaiikuBig *D_80122644;
extern s16 D_80150140;
/* end bulk_m2c declarations */
void func_8014354C(void) {
    TaiikuBig *var_s1;
    u32 *var_s0;

    func_8009B3C0(0x801F9004);
    var_s1 = D_801227A0;
    var_s0 = D_80122640;
    do {
        func_8009B430(0x801F900C, (u8 *) var_s0, 0);
        *(TaiikuBig **)((unsigned char *)var_s0 + (4)) = var_s1;
        var_s1 += 0x50;
        var_s0 += 0x10;
        *(s32 *)((unsigned char *)var_s0 + (-16)) = 0x80000000;
    } while (var_s1 != D_80122CA0);
    func_8009B430(0x801F900C, (u8 *) D_80122640, 0);
    D_80122644 = D_801227A0;
    *D_80122640 = 0;
    D_80150130 = -0x60;
    D_80150134 = 0x20;
    D_80150138 = -0x300;
    D_80150140 = 0;
    D_8015013C.vx = 0;
    D_8015013C.vy = 0;
}

void func_80143644(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80142B80();
        break;
    case 1:
        func_80143334();
        break;
    }
    func_80142AE8();
}
void func_8014369C(void) {
    func_80044774(0);
    func_80046290(0x07000001, 0x02000E0C, 0xF);
    func_80044750(0x300);
    func_8004284C();
}

void func_801436E4(s32 arg0) {
    load_tpage_buf_lock(5, 0x801E8000 - (arg0 << 15), 2, arg0);
    load_tpage_buf_lock(6, 0x801EA000 - (arg0 << 15), 2);
    load_tpage_buf_lock(7, 0x801EC000 - (arg0 << 15), 2);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143758);

void func_8014387C(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 j;

    j = arg0 + 0x60;
    p = D_8011ECD0 + arg0 * 0x44;
    p[0x1982] = 0x40;
    p[0x1983] = 0xA4;
    p[0x1981] = 0;
    *(s32 *)(p + 0x19B8) = 0;
    *(s32 *)(p + 0x198C) = 0x801F86EC;
    *(s32 *)(p + 0x1990) = 0x801F897C;
    *(s32 *)(p + 0x19B4) = 0x801F86A4;
    p[0x1984] = 8;
    *(s16 *)(p + 0x1998) = arg1;
    p[0x19C3] = 0xF;
    p[0x1985] = 0x80;
    *(s16 *)(p + 0x19A6) = arg2 - 0xA0;
    *(s16 *)(p + 0x19AA) = 0x8C;
    *(s16 *)(p + 0x19AE) = 0;
    *(s16 *)(p + 0x19B2) = -1;
    p[0x1987] = 0x80;
    if (arg1 == 0xA3) {
        D_8011ECD0[j * 0x44 + 0x43] = 0xE;
    }
    p = D_8011ECD0 + j * 0x44;
    if (*(s16 *)(p + 0x18) >= 0xF1) {
        p[3] = 0x34;
    } else {
        p[3] = 0xB4;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143968);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143A88);

/* NON_MATCHING: T-10330, 23 lines: c and the Rec24 pointer sit in v1/v0 (original a1/v1, the original reads D_800E62BA as its own scalar). */
#ifdef NON_MATCHING
/* NON_MATCHING: T-10720, register assignment only: the promoted D_800E6280.unk_03A lands in v1 (plus a copy in a1) and the pointer in v0, the original has the value in a1 and the pointer in v1 with lui v0 for D_80150138 hoisted; the loop is written over D_80121818 so the end folds to a single lui/addiu, about 25 lines */
void func_80143F24(void) {
    s32 i;

    func_80142AE8();
    if (D_800E6280.unk_03A != 0) {
        for (i = 0; i < 3; i++) {
            ((u8 *)&D_80121818)[i * 0x24 + 0x16] = D_800E6280.unk_03A;
            ((u8 *)&D_80121818)[i * 0x24 + 0x15] = D_800E6280.unk_03A;
            ((u8 *)&D_80121818)[i * 0x24 + 0x14] = D_800E6280.unk_03A;
        }
        D_800E6280.unk_03A -= 1;
    }
    D_80150138 -= 0xC8;
    if (D_80150138 < -0x7530) {
        func_80059048();
        if (D_800E6280.unk_F5F >= 0xD) {
            main_state_set(0x12);
        } else {
            main_state_set(0xC3);
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143F24);
#endif

void func_80143FE0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014369C();
        break;
    case 1:
        func_80143A88();
        break;
    case 2:
        func_80143F24();
        break;
    case 0xFF:
        func_80144224();
        break;
    }
    func_801433E4();
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_8014407C);

void func_801441DC(s32 arg0, s32 arg1) {
    u8 v;
    u8 *p;

    if (arg1 > 0x80) {
        v = 0x80;
    } else if (arg1 < 0) {
        v = 0;
    } else {
        v = arg1;
    }
    p = (u8 *)D_801217D0 + arg0 * 36;
    p[0x16] = v;
    p[0x15] = v;
    p[0x14] = v;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144224);

s32 func_80144540(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        tpage_buf_clear();
        draw2d3d(1, 0);
        k_speed_set(0);
        D_800E6280.unk_F6F = 3;
        func_80048EB8(0);
        func_80048DAC(1);
        func_80041584();
        *(s32 *) &D_800E6280.unk_1095[3] = 0x9F;
        func_8006BC28(0);
        back_clear_switch(1);
        func_8004E58C();
        set_kanji_string(-0x90, -0x40, 0xF, "今回のプレイをセーブしますか？", 0);
        set_kanji_string(-0x18, 4, 0xF, D_801500F0, 0);
        set_kanji_string(0x3C, 4, 1, func_80140F50(), 0);
        set_kanji_string(-0x18, 0x14, 0xF, D_8015010C, 0);
        set_kanji_string(0x3C, 0x14, 2, func_80140F94(), 0);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        data_save_load_class_init();
        D_80120653 |= 0x20;
        D_800E6280.unk_110D += 1;
        D_800E6280.unk_03A = 0;
        break;
    case 1:
        if (D_800E6280.unk_03A++ > 0x80) {
            func_8004284C();
        }
        break;
    }
}

void func_801446E4(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_8004E58C();
        func_8004E788(-0x90, -0x40, 0xF, "メモリーカードをチェックしています", 0);
        func_8004E884(1);
        D_800E8BEE = 0;
        func_80053CE0();
        func_80042808();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(2);
    }
}

void func_80144768(void) {
    if (D_800E6280.unk_03A-- < 8U) {
        D_800E6280.unk_03A = 0;
        func_80053D10();
        main_state_set(0x11);
    }
}

void func_801447B0(void) {
    GameState *gs = &D_800E6280;

    switch (gs->unk_110D) {
    case 0:
        if (func_80053F04() != 0) {
            if (gs->unk_1115 == 1) {
                func_80053D10();
                gs->unk_110D += 1;
            } else if (gs->unk_1115 == 0xF1) {
                func_80053D10();
                func_80042940(0xA0);
            } else if (gs->unk_1115 == 0xFF) {
                func_80053D10();
                func_80042940(0xFF);
            }
        } else if ((u32) gs->unk_1120 >= 0x201U) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 1:
        func_80053CE0();
        gs->unk_110D += 1;
        break;
    case 2:
        if (func_80054140() == 2) {
            gs->unk_110D += 1;
        } else if ((u32) gs->unk_1120 >= 0x201U) {
            func_80042940(0xFF);
        }
        break;
    case 3:
        if ((u8) D_800E8BEE != 0) {
            func_80053D10();
            func_80053CE0();
            gs->unk_110D += 1;
        } else {
            func_80053D10();
            func_8004E58C();
            set_kanji_string(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            k_disp_start(1);
            func_8004284C();
        }
        break;
    case 4:
        if (func_80054284() == 3) {
            func_800552DC();
            func_8004E58C();
            set_kanji_string(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            k_disp_start(1);
            func_8004284C();
            func_80053D10();
        } else if (((u32) gs->unk_1120 >= 0x201U) || (gs->unk_1115 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    }
    k_disp_inc();
}

s32 func_80144A38(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_03A = 0;
        func_80059E00();
        break;
    case 1:
        func_80059BC0();
        break;
    case 2:
        func_80144540();
        break;
    case 3:
        func_801446E4();
        break;
    }
}

/* Port of the main clone func_8005C5BC (T-17300); only the state codes differ. */
void func_80144AC0(void) {
    s32 i;
    s32 d;
    FlagsWork24 *work;
    s16 x[16];
    s16 y[16];
    s16 w[16];
    s16 h[16];

    func_8004E58C();
    func_80066104("データセーブ");
    d = 5;
    for (i = 0; i < 14; i++) {
        x[i] = (i / d) * 32 - 124;
        y[i] = (i % d) * 16 - 64;
        w[i] = 32;
        h[i] = 16;
        set_kanji_string(x[i], y[i], 15, D_800B32CC[i], 0);
    }
    menu_set(1, 14, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    k_sub_reset_point_set();
    work = (FlagsWork24 *)&D_800E7D10;
    for (i = 1; i != 15; i++) {
        if (work->unk_04[i] == 1) {
            break;
        }
    }
    if ((u8) D_800E8BEE != 0) {
        D_800E6280.unk_1094 = D_800E7D36 - 1;
        D_800E6280.unk_1118 = (s8) D_800E7D36;
        func_80042940(2);
        func_800696DC(0, 0);
        func_8005C7C0();
    } else if (i == 0xF) {
        func_8005C7C0();
        func_80042940(2);
    } else {
        D_800E6280.unk_1118 = i;
        func_80042940(0xC0);
    }
    func_8005C7FC();
}

void func_80144CC4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1120 >= 0x201U || D_800E6280.unk_1115 == 0xD0) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_80042940(2);
            func_800696DC(0, 0);
            func_8005C7C0();
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042940(0xFF);
        }
        break;
    }
    func_8005C7FC();
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144DF0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144F74);

s32 func_801451D4(void) {
    /* FAKE: scalar selector view keeps the selector in v1 for every compare. T-11350 */
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_80053D10();
        func_80044750(0x502);
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, (s32) "メモリーカードを認識できません", 0);
        k_sub_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x80, 0x32, 1, (s32) "メモリーカードを差し込み口１に", 0);
        set_kanji_string(-0x80, 0x42, 1, (s32) "セットし、ボタンを押してください。", 0);
        k_sub_disp_start(2);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x860) {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145318);

void func_801455B0(void) {
    u8 temp;
    temp = D_800E6280.unk_110D;
    switch (temp) {
    case 0:
        func_8004E58C();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004E58C();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "初期化していいですか？", 0);
        set_kanji_string(-0x90, -0x12, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x3C, -0x12, 1, func_80140F50(), 0);
        set_kanji_string(-0x90, -4, 0, (s32) D_8015010C, 0);
        set_kanji_string(-0x3C, -4, 2, func_80140F94(), 0);
        k_disp_start(5);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            if (func_80054108() == 1) {
                D_800E6280.unk_110D = 0x10;
            } else {
                func_80053CE0();
                func_80042940(0);
            }
        } else if (D_800E6280.unk_F88 & 0x40) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_8004E58C();
        set_kanji_string(-0x80, -0x3C, 0xF, (s32) "別のメモリーカードを", 0);
        set_kanji_string(-0x80, -0x2E, 1, (s32) "差し込み口１", 0);
        set_kanji_string(-0x80, -0x2E, 0xF, (s32) "　　　　　　にセットして、", 0);
        set_kanji_string(-0x80, -0x20, 0xF, (s32) "　ボタンを押してください。", 0);
        set_kanji_string(-0x80, -0x20, 1, func_80140F50(), 0);
        k_disp_start(5);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    case 16:
        if (func_80054694(0) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    case 17:
        D_800E7D36 = 0;
        *D_800E7D14 = 1;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 18:
        if (func_80054388() == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    case 19:
        func_80053CE0();
        func_80042940(0);
        break;
    }
}

void func_80145960(void) {
    u8 sel = D_800E6280.unk_110A; /* FAKE: shared global, yet the original selector is in $v0; cause unknown (T-5010) */

    switch (sel) {
    case 0x0:
        func_801447B0();
        break;
    case 0x1:
        func_80144AC0();
        break;
    case 0x2:
        func_80144DF0();
        break;
    case 0x3:
        func_80144F74();
        break;
    case 0xA0:
        func_801455B0();
        break;
    case 0xC0:
        func_80144CC4();
        break;
    case 0xF0:
        func_80145318();
        break;
    case 0xFF:
        func_801451D4();
        break;
    }
}

void func_80145A40(void) {
    if (D_800E6280.unk_1100 != 0) {
        func_80066C08(0);
    }
}

void func_80145A6C(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80144A38();
        break;
    case 1:
        func_80145960();
        break;
    case 2:
        func_80144768();
        break;
    }
    func_80145A40();
    func_80145AF8();
}
/* FAKE: the extra pointer homes retain the frame, and the intensity assignment
 * inside the call keeps its spill in the jal delay slot. The implicit return
 * preserves the original return-zero early exit and final call value. T-9720. */
s32 func_80145AF8(void)
{
    u8 *brightness;
    u8 *brightness_alias;
    s32 intensity;
    s32 color;
    brightness = &D_800E6280.unk_03A;
    if ((D_800E6280.unk_1109 == 0) && (((u8) D_800E6280.unk_110A) < 2U))
    {
        return 0;
    }
    brightness_alias = &(*brightness);
    func_80049A40(-0xA0, -0x57, 0x11B, 0x7D, 2, (((s32) ((intensity = *brightness_alias) * 3)) / 2) * 0x10101, 2);
    func_8004955C(2);
    color = (intensity / 3) * 0x40401;
    func_80049A40(-0x85, 0x2E, 0x10A, 0x34, 2, color, 0x81);
    func_800494BC(0, 0, 2, 0, 0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145C00);

void func_80145F74(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80145C00();
    }
}

void func_80145FA0(void) {
    back_clear_switch(1);
    if (D_800E6280.unk_110D != 0 && (u32)D_800E6280.unk_1104.w < 0xFF) {
        func_8004AC18(0xFF - D_800E6280.unk_1104.w);
    }
}

void func_80145FF0(void) {
    back_clear_switch(1);
    switch (D_800E6280.unk_110D) {
    case 3:
        break;
    case 0:
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 1:
        if (func_800460EC() & 2) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_80046290(0x1B000000, 0x01000004, 0xE);
        func_80044750(0x300);
        D_800E6280.unk_110D += 1;
        break;
    }
}

void func_801460C4(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80145FA0();
        break;
    case 1:
        func_80145FF0();
        break;
    }
    /* (u32): the original compares the s32 counters unsigned (sltiu); T-1321 */
    if ((u32) D_800E6280.unk_1100 < 0xFFU) {
        func_8004AC18(0xFF - D_800E6280.unk_1100);
    }
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042808();
    } else if ((u32) D_800E6280.unk_1104.w >= 0x709U) {
        func_80042808();
    }
}
void func_8014618C(void) {
    func_80041584();
    func_80048E78();
    func_8009C5E0(0);
    func_8004111C();
    if (D_800E6280.unk_1124 != 0) {
        func_80042940(3);
        return;
    }
    if (D_800E6280.unk_F84 & 0x860) {
        func_80042940(1);
        return;
    }
    func_80042940(2);
}

void func_80146214(void) {
    if (D_800E6280.unk_000 == 0x100) {
        func_800590CC(0);
    }
    main_state_set(0xC4);
    func_8009C5E0(1);
}

void func_80146254(void) {
    if (D_800E6280.unk_000 == 0x100) {
        func_800590CC(0);
    }
    main_state_set(0x91);
    func_8009C5E0(1);
}

void func_80146294(void) {
    if (D_800E6280.unk_000 == 0x100) {
        func_800590CC(0);
    }
    D_800E6280.unk_F5F += 0x10;
    main_state_set(0x12);
    func_8009C5E0(1);
}

s32 func_801462E8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014618C();
        break;
    case 1:
        func_80146214();
        break;
    case 2:
        func_80146254();
        break;
    case 3:
        func_80146294();
        break;
    }
}

void func_8014636C(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80145F74();
        break;
    case 1:
        func_801460C4();
        break;
    case 2:
        func_801462E8();
        break;
    default:
        func_80046500();
        break;
    }
}