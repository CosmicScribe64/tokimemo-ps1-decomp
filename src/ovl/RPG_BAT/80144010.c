#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80144010", func_80144010);

/* FAKE: integer fall-through preserves the selector/result registers. T-10010. */
s32 func_80144480(void) {
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x10000:
        D_8015EBAC = 0x10;
        break;
    case 0x20000:
        D_8015EBAC = 0x11;
        break;
    case 0x40000:
        D_8015EBAC = 0x12;
        break;
    case 0x100000:
        if (D_8015ED98 & 1) {
            switch (func_8013E9A8(15)) {
            case 0:
                D_8015EBAC = 1;
                break;
            case 1:
                D_8015EBAC = 2;
                break;
            default:
                D_8015EBAC = 0;
                break;
            }
        } else {
            switch (func_8013E9A8(4)) {
            case 0:
                D_8015EBAC = 1;
                break;
            case 1:
                D_8015EBAC = 2;
                break;
            default:
                D_8015EBAC = 0;
                break;
            }
        }
        break;
    case 0x200000:
    case 0x400000:
        D_8015EBAC = 3;
        break;
    case 0x8000000:
        D_8015EBAC = 4;
        break;
    case 0x80000000:
        D_8015EBAC = 5;
        break;
    }
}

s32 func_801445E8(s32 arg0) {
    if (arg0 == 0) {
        return 0x78;
    }
    if (D_8015EDC4 & 0x80000000) {
        return func_8013EA00(0x1E, 0x1E) + 0x1E;
    }
    return func_8013EA00(0x28, 0x1E) + 0x3C;
}

s32 func_80144640(void) {
    switch (D_8015EDC4 & ~0xFFFF) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return 0x14A;
    case 0x8000000:
        return 0x168;
    case 0x80000000:
        return D_8015EBDC + 0x96;
    case 0x100000:
        return D_8015EBDC + 0x96;
    case 0x200000:
        return D_8015EBDC + 0x96;
    case 0x400000:
        return D_8015EBDC + 0x96;
    }
}

s32 func_801446F8(void) {
    switch (D_8015EB9C) {
    case 1:
        return 10;
    case 0:
        return 200;
    case 7:
        return 240;
    case 9:
        return 260;
    case 6:
        return 260;
    case 8:
        return 260;
    case 10:
        return 260;
    case 4:
        return 300;
    case 2:
        return 300;
    case 3:
        return 300;
    case 5:
        return 300;
    case 13:
        if (D_8015EDEC < 10) {
            return 110;
        }
        return 170;
    case 14:
        return 0x7FFFFFFF;
    }
}

s32 func_801447B8(void) {
    if (D_8015EC1C == 1) {
        return 1;
    }
    return 0;
}
