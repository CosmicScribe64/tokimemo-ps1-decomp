#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80135AE0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_80135B4C();
        return;
    case 2:
        func_80135D90();
        return;
    case 3:
        func_80135E80();
        return;
    }
}

void func_80135B4C(void) {

    switch (D_8015EE00.unk_04) {
    case 0:
        func_8013E7C0(0x1D, 5, 1, 1);
        func_8013F15C(0x512, 1, 0);
        func_8014F230();
    default:
        break;
    case 1:
        func_8014F350(0x14);
        break;
    case 2:
        func_8013E7C0(0x1D, 4, 1, 1);
        func_8013F15C(0x512, 1, 0);
        func_8014F230();
        break;
    case 3:
        func_8014F350(0x14);
        break;
    case 4:
        func_8013E7C0(0x1D, 5, 1, 1);
        func_8013F15C(0x512, 1, 0);
        func_8014F230();
        break;
    case 5:
        func_8014F350(0x14);
        break;
    case 6:
        func_8013E7C0(0x1D, 4, 1, 1);
        func_8013F15C(0x512, 1, 0);
        func_8014F230();
        break;
    case 7:
        func_8014F350(0x14);
        break;
    case 8:
        func_800AE0F0(D_8015E904, "美樹原「きゃっ！");
        func_8013F1C4(0x1F000087, 0x18000089, 0);
        func_8014B738((s32) D_8015E9CC, 1, 60);
        func_8014F230();
        break;
    case 9:
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8014F230();
        break;
    case 10:
        D_8015EE00.unk_0C += 1;
        func_8013E97C(0x1D, D_8015EE00.unk_0C, 0);
        if (D_8015EE00.unk_0C >= 0x41) {
            func_8014F230();
        }
        break;
    case 11:
        func_8014F448(0);
        break;
    case 12:
        func_8014F350(0x14);
        break;
    case 13:
        func_8014F278();
        D_8015EDF0 = 0x40000000;
        break;
    }
}

void func_80135D90(void) {
    switch (D_8015EE00.unk_04) {
    case 0:
        func_8013E7C0(0x1D, 1, 1, 1);
        func_8014F230();
        return;
    case 1:
        D_8015EE00.unk_0C += 1;
        func_8013E97C(0x1D, 0x40 - D_8015EE00.unk_0C, 0);
        if (D_8015EE00.unk_0C >= 0x41) {
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0, 1, 1);
            func_8014F230();
            return;
        }
        return;
    case 2:
        func_8014F350(0x14);
        return;
    case 3:
        func_8014F278();
        break;
    }
}

void func_80135E80(void) {

    switch (D_8015EE00.unk_04) {
    case 0:
        func_8013E7C0(0x1D, 1, 1, 1);
        func_8014F230();
    default:
        break;
    case 1:
        D_8015EE00.unk_0C += 1;
        func_8013E97C(0x1D, 0x40 - D_8015EE00.unk_0C, 0);
        if (D_8015EE00.unk_0C >= 0x41) {
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0, 1, 1);
            func_8014F230();
        }
        break;
    case 2:
        func_8014F350(0x14);
        break;
    case 3:
        func_800AE0F0(D_8015E904, "美樹原「はっ！");
        func_8013F1C4(0x1F00008A, 0x1900008C, 0);
        func_8014B738((s32) D_8015E9CC, 1, 70);
        func_8014F230();
        break;
    case 4:
        func_8014F350(0xA);
        break;
    case 5:
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8014F230();
        break;
    case 6:
        D_8015EE00.unk_0C += 1;
        func_8013E97C(0x1D, D_8015EE00.unk_0C, 0);
        if (D_8015EE00.unk_0C >= 0x41) {
            func_8014F230();
        }
        break;
    case 7:
        func_8014F448(0);
        break;
    case 8:
        func_8014F350(0x14);
        break;
    case 9:
        func_8014F278();
        D_8015EDF0 = 0x40000000;
        break;
    }
}
