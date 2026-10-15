#include "common.h"
#include "ovl/DATE2.h"

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_8013B600;

void func_80137D50(void) {
    D_8013B540 = 0x801B00A4;
    D_8013B544 = 0x801B0238;
    D_8013B548 = 0x801B03C8;
    D_8013B54C = 0x801B0558;
    D_8013B550 = 0x801B0704;
    D_8013B554 = 0x801B0878;
    D_8013B558 = 0x801B0A20;
    D_8013B55C = 0x801B0B9C;
    D_8013B560 = 0x801B0D88;
    D_8013B564 = 0x801B0F24;
    D_8013B568 = 0x801B106C;
    D_8013B56C = 0x801B00CC;
    D_8013B570 = 0x801B0260;
    D_8013B574 = 0x801B03F0;
    D_8013B578 = 0x801B0580;
    D_8013B57C = 0x801B072C;
    D_8013B580 = 0x801B08A0;
    D_8013B584 = 0x801B0A48;
    D_8013B588 = 0x801B0BC4;
    D_8013B58C = 0x801B0DB0;
    D_8013B590 = 0x801B0F4C;
    D_8013B594 = 0x801B1084;
    D_8013B598 = 0x801B016C;
    D_8013B59C = 0x801B0300;
    D_8013B5A0 = 0x801B0490;
    D_8013B5A4 = 0x801B0620;
    D_8013B5A8 = 0x801B07CC;
    D_8013B5AC = 0x801B0940;
    D_8013B5B0 = 0x801B0AE8;
    D_8013B5B4 = 0x801B0C64;
    D_8013B5B8 = 0x801B0E50;
    D_8013B5BC = 0x801B0FEC;
    D_8013B5C0 = 0x801B10E4;
}

void func_80137F64(void) {
    func_80083808();
    func_80138048();
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}

void func_80137FCC(void) {
    D_800CA134 = &D_8013B5F8;
    D_800CA138 = &D_8013B5FC;
    D_800CA13C = D_8013B5EC;
    D_800CA140 = D_8013B5F0;
    D_800CA144 = D_8013B5F4;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80138048(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_8013B600;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x40);
}

void func_801380C4(void) {
    cd_read_sectors(2, 0x801E8000, 0xBD02);
    func_80138100();
    func_8004284C();
}

void func_80138100(void) {
    D_8013B5D4 = 0x801E80D0;
    D_8013B5D8 = 0x801E82C0;
    D_8013B5DC = 0x801E8444;
    D_8013B5E0 = 0x801E8700;
}

void func_80138144(void) {
    func_80044750(0x202);
    D_80122D20 = 0;
    func_8004284C();
}

void func_80138170(void) {
    if (D_80122CDC != 0) {
        func_80083440(2);
    }
    func_8004284C();
}

void func_801381A4(void) {
    main_state_set(0x50);
    func_80042908(1);
    func_80042940(0x11);
}

void func_801381D4(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF39);
    func_80137D50();
    func_8004284C();
}

void func_8013820C(void) {

    *D_800CA234 = 0;
    D_800CA236 = 0;
    D_800CA234[2] = 0;
    *D_800CA224 = 1;
    D_800CA224[1] = 1;
    D_800CA224[2] = 1;
    func_8004E9F4(0);
    func_8006612C((s32) "ＢＯＸ内");
    D_80122CDC = 0;
    D_8013B5CC = 0;
    D_8013B5D0 = 0;
    func_80137D50();
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013B5EC = D_8013B544;
        D_8013B5F0 = D_8013B570;
        D_8013B5F4 = D_8013B59C;
        break;
    case 1:
        D_8013B5EC = D_8013B554;
        D_8013B5F0 = D_8013B580;
        D_8013B5F4 = D_8013B5AC;
        break;
    case 2:
        D_8013B5EC = D_8013B548;
        D_8013B5F0 = D_8013B574;
        D_8013B5F4 = D_8013B5A0;
        break;
    case 3:
        D_8013B5EC = D_8013B54C;
        D_8013B5F0 = D_8013B578;
        D_8013B5F4 = D_8013B5A4;
        break;
    case 4:
        D_8013B5EC = D_8013B564;
        D_8013B5F0 = D_8013B590;
        D_8013B5F4 = D_8013B5BC;
        break;
    case 5:
        D_8013B5EC = D_8013B558;
        D_8013B5F0 = D_8013B584;
        D_8013B5F4 = D_8013B5B0;
        break;
    case 6:
        D_8013B5EC = D_8013B55C;
        D_8013B5F0 = D_8013B588;
        D_8013B5F4 = D_8013B5B4;
        break;
    case 7:
        D_8013B5EC = D_8013B550;
        D_8013B5F0 = D_8013B57C;
        D_8013B5F4 = D_8013B5A8;
        break;
    case 8:
        D_8013B5EC = D_8013B540;
        D_8013B5F0 = D_8013B56C;
        D_8013B5F4 = D_8013B598;
        break;
    case 9:
        D_8013B5EC = D_8013B560;
        D_8013B5F0 = D_8013B58C;
        D_8013B5F4 = D_8013B5B8;
        break;
    case 10:
        D_8013B5EC = D_8013B568;
        D_8013B5F0 = D_8013B594;
        D_8013B5F4 = D_8013B5C0;
        break;
    }
    D_8013B5F8 = 0;
    D_8013B5FC = 0;
    func_8004284C();
}


void func_8013850C(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        func_8013856C();
        return;
    case 1:
        func_80138A3C();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013856C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_801385CC();
        return;
    case 1:
        func_801388C0();
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801385CC);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7208;
extern s32 D_800E7384;
extern s32 D_8013B5E4;
extern s8 D_800E738D;
/* end bulk_m2c declarations */
void func_801388C0(void) {
    s32 var_t0;
    s32 var_v0_2;
    u8 temp_t7;
    u8 var_v0;

    menu_check(0, D_8011ECF6, D_8011ECFA);
    func_8004FC10(0);
    menu_bar_show(0);
    var_v0 = D_800B3CFC;
    if ((var_v0 != 0) && ((u16) D_8013B5D0 != var_v0)) {
        func_80044750(0x601);
        temp_t7 = (D_8013B5CC + 1) & 0xFF;
        D_8013B5CC = temp_t7;
        if (temp_t7 >= 3U) {
            D_8013B5CC = 1;
        }
        D_800E738D = 0;
        var_v0 = 0;
        D_800B3CF8 = 0;
    }
    D_8013B5D0 = (s16) var_v0;
    D_800B3CFC = var_v0;
    if (D_800E7208 & 0x600060) {
        var_v0_2 = 0;
        var_t0 = 0 * 0x10;
loop_7:
        if ((*(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + var_t0) + (3990)) != 0) && (*(&D_8013B5E4 + (var_v0_2 * 2)) != 0)) {
            D_80122CDC = var_v0_2;
            D_8013B5C4 = (u8) var_v0_2;
            D_80122D30 = 0;
            D_8011ECD3 &= 0xFF7F;
            k_reset(1);
            D_800E7384 += 1;
            D_800E738D = 0;
            return;
        }
        var_v0_2 = (s32) (s16) (var_v0_2 + 1);
        var_t0 = var_v0_2 * 0x10;
        if (var_v0_2 >= 4) {

        } else {
            goto loop_7;
        }
    }
}

s32 func_80138A3C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80138A90();
        break;
    case 1:
        func_80138BEC();
        break;
    }
}

/* FAKE: leading byte fixes copy offsets; integer fallthrough retains allocation. T-9550 */
s32 func_80138A90(void) {
    u8 pad[1];
    Date2LoadPairs4 loads;
    Date2LoadPair first;
    Date2LoadPair second;
    loads = D_8013B688;
    first = D_8013B6A8;
    second = D_8013B6B0;
    D_80122D20 = 1;
    func_80044750(0xBF);
    if (D_8013B5CC < 2U || D_80122CDC == 1 || D_80122CDC == 2) {
        func_80046290(loads.pair[D_80122CDC].unk_00, loads.pair[D_80122CDC].unk_04, 15);
    } else if (D_80122CDC == 0) {
        func_80046290(first.unk_00, first.unk_04, 15);
    } else {
        func_80046290(second.unk_00, second.unk_04, 15);
    }
    func_80044750(0x300);
    D_8013B5C8 = 0;
    D_800E6280.unk_110D++;
}

void func_80138BEC(void) {
    u8 t;

    if ((func_80046094() == 9) && (D_8013B5C8 >= 0x79)) {
        func_8004284C();
        D_80122CDC = 0;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 2;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 1;
        func_80084D3C();
    } else if ((D_800E6280.unk_F88 & 0x400040) && (D_8013B5C8 >= 0x79)) {
        t = func_80051A68(D_800E6280.unk_F5F) & 0x7F;
        D_8013B5F8 = D_8013B5F8 + (t == 2) + ((t == 3) * 2) + 1;
        if (D_800E6280.unk_F5F == 0xA) {
            D_8013B5F8 = 2;
        }
        func_8004284C();
        func_8004E9F4(1);
        func_80044750(0x74);
        D_80122CDC = 1;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 1;
        func_80084D3C();
    }
    if (func_800460EC() & 4) {
        D_8013B5C8 += 1;
    }
    switch (D_8013B5C4) {
    case 2:
        func_80138E0C();
        break;
    case 0:
        func_80138F8C();
        break;
    case 3:
        func_801391A0();
        break;
    default:
        func_801393B4();
        break;
    }
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void func_80138E0C(void) {
    s16 index;
    s16 selected;
    Date2Threshold30 thresholds;
    u8 pad[8];

    thresholds = D_8013B6B8;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 30);
    if (D_8013B5C8 == 200) {
        func_8004E788(-116, 54, 0, (s32)((u8 **)D_8013B5D4)[0], 0);
        func_8004E884(func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5D4)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D4)[1], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        func_8004E9F4(1);
        func_801395C8((s16)(selected | 0));
    }
    if (D_8013B5C8 == 3540) func_8004E9F4(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void func_80138F8C(void) {
    s16 index;
    s16 selected;
    Date2Threshold51 thresholds;
    u8 pad[4];
    thresholds = D_8013B6F4;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 51);
    if (D_8013B5C8 == 1500) {
        func_8004E788(-116, 54, 0, (s32)((u8 **)D_8013B5D8)[0], 0);
        func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5D8)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D8)[1], 0);
        func_8004E884(func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5D8)[0]) * 7 + func_800AE0E0(((u8 **)D_8013B5D8)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5D8)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        func_8004E9F4(1);
        func_801395C8((s16)(selected | 0));
    }
    if (D_8013B5C8 == 6364) func_8004E9F4(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void func_801391A0(void) {
    s16 index;
    s16 selected;
    Date2Threshold39 thresholds;
    u8 pad[4];
    thresholds = D_8013B75C;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 39);
    if (D_8013B5C8 == 1200) {
        func_8004E788(-116, 54, 0, (s32)((u8 **)D_8013B5DC)[0], 0);
        func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5DC)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5DC)[1], 0);
        func_8004E884(func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5DC)[0]) * 7 + func_800AE0E0(((u8 **)D_8013B5DC)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5DC)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        func_8004E9F4(1);
        func_801395C8((s16)(selected | 0));
    }
    if (D_8013B5C8 == 5400) func_8004E9F4(1);
}

/* FAKE: padding fixes table offset; OR-zero preserves conversion; string pointers retain scalar views. T-9550 */
void func_801393B4(void) {
    s16 index;
    s16 selected;
    Date2Threshold81 thresholds;
    u8 pad[4];
    thresholds = D_8013B7AC;
    selected = 0xFF;
    index = 0;
    do {
        if (D_8013B5C8 >= thresholds.values[index]) selected = index;
        index++;
    } while (index < 81);
    if (D_8013B5C8 == 30) {
        func_8004E788(-116, 54, 0, (s32)((u8 **)D_8013B5E0)[0], 0);
        func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5E0)[0]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5E0)[1], 0);
        func_8004E884(func_8004E788((s16)(func_800AE0E0(((u8 **)D_8013B5E0)[0]) * 7 + func_800AE0E0(((u8 **)D_8013B5E0)[1]) * 7 - 116), 54, 0, (s32)((u8 **)D_8013B5E0)[2], 0));
    }
    if (selected != 0xFF && D_8013B5C8 == thresholds.values[selected]) {
        func_8004E9F4(1);
        func_801395C8((s16)(selected | 0));
    }
    if (D_8013B5C8 == 4620) func_8004E9F4(1);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137D50", func_801395C8);
