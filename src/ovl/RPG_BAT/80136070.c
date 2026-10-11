#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80136070(void) {
    switch (*D_8015ED8C) {
    case 0:
        func_800AE0F0(D_8015E904, "早乙女「えーん");
        func_800AE0F0(D_8015E92C, "えーん（うそなき）");
        func_8014B738((s32) D_8015E9CC, 2, 0xB4);
        *D_8015ED8C += 1;
        return;
    case 1:
        func_8013F1C4(0x0700002C, 0x07000034, 0);
        *D_8015ED8C += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013E7C0(0x1E, 7, 5, 1);
        func_8013F1C4(0x04000029, 0x0600002E, 1);
        *D_8015ED8C += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014F230();
        /* fallthrough */
    default:
        return;
    }
}

void func_801361AC(void) {
    switch (*D_8015ED8C) {
    case 0:
        if (D_8015EDC4 & 0x100000) {
            func_800AE0F0(D_8015E904, "不良達「これはいかん！");
            func_800AE0F0(D_8015E92C, "女子供を泣かせては、男がすたる。");
        } else {
            func_800AE0F0(D_8015E904, "番長「いっか〜ん！");
            func_800AE0F0(D_8015E92C, "女子供を泣かせては、番長の名折れ！");
        }
        func_8014B738((s32) D_8015E9CC, 2, 0xC8);
        *D_8015ED8C += 1;
        return;
    case 1:
        if (D_8015EDC4 & 0x100000) {
            func_8013F1C4(0x1D000014, 0x05000017, 0);
        } else {
            func_8013F1C4(0x1E00008C, 0x1B000093, 0);
        }
        *D_8015ED8C += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        if (D_8015EDC4 & 0x100000) {
            func_8013F1C4(0x0E00004E, 0x0C000057, 1);
        } else {
            func_8013F1C4(0x04000012, 0x04000020, 1);
        }
        *D_8015ED8C += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014F230();
        /* fallthrough */
    default:
        return;
    }
}

void func_80136374(void) {
    switch (D_8015EE04) {
    case 0:
        D_8015EE0C += 1;
        if (D_8015EE0C % 20 < 0xA) {
            func_8013E7C0(0x1D, 5, 1, 1);
        } else {
            func_8013E7C0(0x1D, 0, 1, 1);
        }
        if (D_8015EE0C >= 0x3B) {
            func_8014F230();
        }
        break;
    case 1:
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8014F230();
        break;
    case 2:
        func_80136070();
        break;
    case 3:
        func_8014F3C8(0x1E);
        break;
    case 4:
        func_801361AC();
        break;
    case 5:
        func_8013E7C0(0x1E, 7, 0x21, 1);
        D_80120E60 = 7;
        D_80120E4E = 0x83;
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8014F230();
        break;
    case 6:
        if (func_8013E90C(0x1E) != 0) {
            func_8013E7C0(0x1E, 0xFF, 1, 0);
            D_80120E4E = 1;
            func_8014F230();
        }
        break;
    case 7:
        func_8014F350(0x14);
        break;
    case 8:
        func_8014F278();
        break;
    }
}
