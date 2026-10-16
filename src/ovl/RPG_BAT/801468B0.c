#define RPG_BAT_OVERRIDE_func_8013E7C0 /* implicit int view: the call's $v0 moves the D_8015EDE0 web of func_80146C30 to $v1 (T-17104) */
#define RPG_BAT_OVERRIDE_func_8013E97C /* implicit int view, same reason (T-17104) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013E7C0();
s32 func_8013E97C();

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801468B0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "？？？「拙僧にまかせなされ。");
        func_8014B738(D_8015E8F0, 1, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x05000045, 0x0700004E, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8014698C(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "虚無僧「迷える大鹿よ、");
        func_800AE0F0(D_8015E850, "怒りを静め給え。");
        func_8014B738(D_8015E8F0, 2, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1D00009C, 0x1D0000A2, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1D0000A3, 0x1E0000A9, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80146AB4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "虚無僧「はしたないところを");
        func_800AE0F0(D_8015E850, "お見せしてしまった。");
        func_800AE0F0(D_8015E878, "では、拙僧はこれにて。");
        func_8014B738(D_8015E8F0, 3, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1D0000AA, 0x1F0000AE, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0D0000B2, 0x0D0000B7, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x1000008A, 0x05000092, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(0);
        return;
    case 7:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

void func_80146C30(void) {
    s32 i;

    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　　　スタンピート　　　　　　");
        func_8013E97C(0x38, 0, 0);
        func_8013E97C(0x39, 0, 0);
        ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[153].unk_01 = 4;
        func_8013E7C0(0x38, 3, 5, 1);
        func_8013E7C0(0x39, 1, 5, 1);
        func_8013F15C(0x508, 1, 2);
        func_8014EF3C();
        break;
    case 1:
        if (!(((DisplayActor44 *)D_8011ECD0)[152].unk_02 & 1)) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
        }
        func_8014F080(0x32);
        break;
    case 2:
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E7C0(0x39, 0xFF, 1, 0);
        func_8013E97C(0x3B, 0, 0);
        func_8013E7C0(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    case 3:
        func_8014F178(1);
        break;
    case 4:
        D_8015EDE0 += 6;
        func_8013F15C(0x507, 0, 1);
        func_8013E97C(0x3B, (s32) (D_8015EDE0 * 0x90) / 100, 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x3B, 0x90, 0);
            func_8014EF3C();
        }
        break;
    case 5:
        func_8013E7C0(0x30, 0xA, 1, 1);
        func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        D_8015ED98 = 0x1000;
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        func_8014F080(0x1E);
        break;
    case 7:
        D_8015EDE0 += 6;
        func_8013E7C0(0x3B, 0, 1, 1);
        func_8013E7C0(0x30, 4, 1, 1);
        func_8013E97C(0x3B, 0x90 - ((s32) (D_8015EDE0 * 0x90) / 100), 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013E97C(0x3B, 0, 0);
            func_8014EF3C();
        }
        break;
    case 8:
        func_8014F080(0xA);
        break;
    case 9:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「だめだ！　体が動かない！");
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(0);
        break;
    case 11:
        func_801468B0();
        break;
    case 12:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「？！");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        break;
    case 13:
        func_8014F178(0);
        break;
    case 14:
        D_8015EDE0 += 1;
        if (D_8015EDE0 < 0x3D) {
            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x30, 0xFF, 1, 0);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 0);
                }
            } else {
                func_8013E7C0(0x30, 0xFF, 1, 1);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 1);
                }
            }
        }
        func_8013E97C(0x28, 0, 0);
        if (D_8015EDE0 >= 0x3D) {
            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x28, 0, 1, 1);
            } else {
                func_8013E7C0(0x28, 0, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x78) {
            func_8014EF3C();
        }
        break;
    case 15:
        D_8015EE18 ^= 0x10000000;
        func_800AE0F0(D_8015E828, "正義の虚無僧推参！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 16:
        func_8014F178(1);
        break;
    case 17:
        func_8014698C();
        break;
    case 18:
        func_800AE0F0(D_8015E828, "　　　　　現世忘却の呪文　　　　　");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 19:
        func_8014F178(1);
        break;
    case 20:
        func_8013E7C0(0x28, 1, 5, 1);
        func_8014EF3C();
        break;
    case 21:
        func_8014F0F8(0x28);
        break;
    case 22:
        ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 6;
        func_8013E7C0(0x28, 2, 1, 1);
        D_8015ECEC = func_8013E9A8(0x400);
        D_8015ECF0 = func_8013E9A8(0x400) + 0x400;
        D_8015ECF4 = func_8013E9A8(0x400) + 0x800;
        func_8013E7C0(0x29, func_8013E9A8(2) + 3, 1, 1);
        func_8013E7C0(0x2A, func_8013E9A8(2) + 3, 1, 1);
        func_8013E7C0(0x2B, func_8013E9A8(2) + 3, 1, 1);
        func_8013E97C(0x29, 0x10, (func_800A0070(D_8015ECEC) >> 8) + 0x20);
        func_8013E97C(0x2A, 0x10, (func_800A0070(D_8015ECF0) >> 8) - 0x10);
        func_8013E97C(0x2B, 0x10, (func_800A0070(D_8015ECF4) >> 8) - 0x40);
        func_8013F1C4(0x0C00003A, 0x06000062, 9);
        func_8013F15C(0x509, 1, 3);
        func_8014EF3C();
        break;
    case 23:
        D_8015EDE0 += 1;
        func_8013E97C(0x29, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECEC) >> 8) + 0x20);
        func_8013E97C(0x2A, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECF0) >> 8) - 0x10);
        func_8013E97C(0x2B, 0x10 - ((s32) D_8015EDE0 / 2), (func_800A0070(D_8015ECF4) >> 8) - 0x40);
        /* FAKE: D_8015ECEC..D_8015ECF4 are three separate words in the header (8014A660.c clears them one by one);
         * the original walks them as an array, and a 3-trip index loop stays rolled. T-17104 */
        for (i = 0; i < 3; i++) {
            (&D_8015ECEC)[i] += 0x28;
        }
        if (D_8015EDE0 >= 0x221) {
            func_8013E7C0(0x28, 0, 1, 1);
            func_8013F15C(0x507, 1, 4);
            func_8014EF3C();
        }
        if ((func_800460EC() == 2) && (D_8015EDE0 >= 0x221)) {
            func_8013E7C0(0x28, 0, 1, 1);
            func_8013F15C(0x507, 1, 4);
            func_8014EF3C();
        }
        break;
    case 24:
        ((DisplayActor44 *)D_8011ECD0)[155].unk_03 = 0x81;
        D_8015EDE0 += 6;
        func_8013E97C(0x3B, -D_8015EDE0, 0);
        if (D_8015EDE0 >= 0x89) {
            func_8014EF3C();
        }
        break;
    case 25:
        func_800AE0F0(D_8015E828, "鹿・グレートは去っていった。");
        k_disp_switch(D_8015EDE8, 0);
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 26:
        func_8014F178(1);
        break;
    case 27:
        func_80146AB4();
        break;
    case 28:
        D_8015EDE0 += 1;
        if (!(D_8015EDE0 & 1)) {
            func_8013E7C0(0x28, 0, 1, 1);
        } else {
            func_8013E7C0(0x28, 0, 1, 0);
        }
        if (D_8015EDE0 >= 0x5B) {
            D_8015EE18 ^= 0x10000000;
            func_8013E7C0(0x30, 0, 1, 1);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8014EF3C();
        }
        break;
    case 29:
        func_8014F080(0x1E);
        break;
    case 30:
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、正義の虚無僧");
        func_800AE0F0(D_8015EA30, "経験値　：８４１ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：２０４５ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 31:
        func_8014F080(0x3C);
        break;
    case 32:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 33:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 34:
        func_8014F178(0);
        break;
    case 35:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
