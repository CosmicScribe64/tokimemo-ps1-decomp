#include "common.h"
#include "ovl/RPG_BAT.h"

/* The club index is kept in the global D_8015EBA0 throughout: a local for it
 * is coloured before the loop constants and shifts every register (T-17111). */
void func_80144010(void) {
    s32 i;
    s32 sum;
    s32 v;

    sum = 0;
    D_8015EBA0 = 0xFF;
    for (i = 0; i < 12; i++) {
        if (((ScheduleClubFlags *)&D_800E6280.unk_66C[i])->unk_25 == 3) {
            D_8015EBA0 = i;
        }
        sum += D_800E6280.unk_66C[i].unk_00;
    }
    if (D_8015EBA0 == 0xB) {
        if (sum == 0) {
            D_8015EBA0 = 0xB;
        } else {
            D_8015EBA0 = 0xFF;
        }
    }
    if (D_8015EBA0 == 0xFF) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
        D_8015EBFC.unk_34 = 0;
    } else {
        D_8015EBA4 = 1;
        D_8015EBA8 = 1;
        D_8015EBFC.unk_34 = 0x64;
    }
    D_8015EC34 = 0;
    if (D_8015EB98 & 0x800F0000) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
    }
    if ((D_8015EB98 & 0xF0000) == 0 && D_8015EB9C == 0xD) {
        D_8015EB9C = 9;
    }
    if ((D_8015EB98 & 0xF0000) == 0 && D_8015EB9C == 0xE) {
        D_8015EB9C = 9;
    }
    if ((D_8015EB98 & 0xF0000) != 0 && D_8015EB9C != 0xD) {
        D_8015EB9C = 0xE;
    }
    if (D_8015EB98 & 0x80000000) {
        D_8015EB9C = 2;
    }
    if (D_8015EB98 & 0x08000000) {
        D_8015EB9C = 0xE;
    }
    if (D_8015EB98 != 0x08000000) {
        D_8015ED9C = 0xFFFF;
    } else {
        D_8015ED9C = 0;
        D_8015ED98 = 0x1000;
    }
    if (D_800E6280.unk_0FC.unk_02 == 0) {
        v = 0xA;
    } else {
        v = D_800E6280.unk_0FC.unk_02 * 0xA;
    }
    D_8015EDA0 = v;
    D_8015ED94 = v;
    switch (D_8015EB9C) {
    case 0:
        D_8015EDEC = 0x100;
        break;
    case 1:
        D_8015EDEC = 0x63;
        break;
    case 2:
        D_8015EDEC = 0x138;
        break;
    case 3:
        D_8015EDEC = 0x130;
        break;
    case 4:
        D_8015EDEC = 0xBD;
        break;
    case 5:
        D_8015EDEC = 0x12A;
        break;
    case 6:
        D_8015EDEC = 0x756;
        break;
    case 7:
        D_8015EDEC = 0x115;
        break;
    case 8:
        D_8015EDEC = 0x16D;
        break;
    case 9:
        D_8015EDEC = 0xAD;
        break;
    case 10:
        D_8015EDEC = 0x109;
        break;
    case 13:
        D_8015EDEC = 0x23D;
        break;
    case 14:
        D_8015EDEC = 0x7FFFFFFF;
        break;
    }
    D_8015EDF8 = D_8015EDEC;
    D_8015EDC4 = D_8015EB98;
    if (D_8015EB98 == 0x08000000) {
        D_8015EDC4 = 0x08000002;
    }
    switch (D_8015EB98) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        D_8015EDC0 = 1;
        D_8015EDCC = 1;
        break;
    case 0x8000000:
        D_8015EDC0 = 0x1388;
        D_8015EDCC = 0x1388;
        break;
    case 0x80000000:
        D_8015EDC0 = 0x7FFFFFFF;
        D_8015EDCC = 0x7FFFFFFF;
        D_8015EBD8 = 0x3E8;
        break;
    case 0x100000:
        D_8015EDC0 = 0x10000;
        D_8015EDCC = 0x10000;
        if (func_801447B8() != 0) {
            D_8015EBD8 = 0x3E8;
            D_8015EBC0 = 0x1388;
            D_8015EBC4 = 0x1388;
            D_8015EBC8 = 0x1388;
        } else {
            D_8015EBD8 = 0x5DC;
            D_8015EBC0 = 0x270F;
            D_8015EBC4 = 0x270F;
            D_8015EBC8 = 0x270F;
        }
        break;
    }
    switch (D_8015EB98) {
    case 0x10000:
        D_8015EE18 = 0x10000075;
        break;
    case 0x20000:
        D_8015EE18 = 0x10000232;
        break;
    case 0x40000:
        D_8015EE18 = 0x1000012A;
        break;
    default:
        D_8015EE18 = 0x7FFFFFFF;
        break;
    }
}

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
