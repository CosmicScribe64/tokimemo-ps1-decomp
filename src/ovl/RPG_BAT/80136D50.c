#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80136D50(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "虹野「がんばってっ！");
        func_8014B738(D_8015E9CC, 1, 0x82);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x06000032, 0x08000034, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F230();
        return;
    }
}

void func_80136E2C(void) {
    func_80137204();
    switch (D_8015EE04) {
    case 0:
        func_80143DA8(0x1D, 0, 0, -0x20, -0x30, 0x64);
    default:
        break;
    case 1:
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8014F230();
        break;
    case 2:
        if (D_80120E1C == 8) {
            func_8013E54C(5);
            func_801477E0();
            func_8014F230();
        }
        break;
    case 3:
        func_80136D50();
        break;
    case 4:
        func_8014F3C8(0x1D);
        break;
    case 5:
        switch (D_8015EE0C) {
        case 0:
            func_8013F15C(0x511, 1, 0);
            func_80137148(4, 6);
            break;
        case 1:
            func_80137148(5, 7);
            break;
        case 5:
            func_80137148(6, 7);
            break;
        case 6:
            func_80137148(7, 6);
            break;
        case 9:
            func_80137148(8, 7);
            break;
        case 13:
            func_80137148(9, 7);
            break;
        case 14:
            func_80137148(0xA, 6);
            break;
        case 20:
            func_80137148(0xB, 7);
            break;
        case 22:
            func_80137148(0xC, 7);
            break;
        case 26:
            func_80137148(0xD, 6);
            break;
        case 27:
            func_80137148(0xE, 7);
            break;
        case 32:
            func_80137148(0xF, 7);
            break;
        case 40:
            func_8014F230();
            break;
        }
        D_8015EE0C += 1;
        break;
    case 6:
        func_8013EA60(0, 0x21, D_8015EBB0, 0);
        func_8013F15C(0x502, 1, 0);
        func_8014F230();
        break;
    case 7:
        func_8014F350(0x28);
        break;
    case 8:
        func_8013E7C0(0x1D, 2, 1, 1);
        func_8014F230();
        break;
    case 9:
        func_80143DA8(0x1D, -0x20, -0x30, 0, 0, 0x64);
        break;
    case 10:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8013E54C(0xFF);
        func_8014F230();
        break;
    case 11:
        func_8014F350(0x14);
        break;
    case 12:
        func_8014F278();
        break;
    }
}

void func_80137148(s32 arg0, s32 arg1) {
    if (arg1 == 7) {
        if (!(D_8015EC74 & 1)) {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
        } else {
            func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x58);
        }
        D_8015EC74 += 1;
    } else {
        func_8013E97C(arg0, func_8013E9A8(0x10) + 0x108, 0x40);
    }
    func_8013E810(arg0, arg1, 5, 1);
}

void func_80137204(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        *(s16 *)(var_s0 + 0x1ABA) = (s16) (*(s16 *)(var_s0 + 0x1ABA) + 1);
        if (!(*(u8 *)(var_s0 + 0x1A92) & 1)) {
            func_8013E7C0(var_s1 + 4, 0xFF, 1, 0);
        }
        var_s1 += 1;
        var_s0 += 0x44;
    } while (var_s1 != 0xC);
}
