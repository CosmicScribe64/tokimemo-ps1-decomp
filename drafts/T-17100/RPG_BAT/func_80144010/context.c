#include "common.h"
#include "ovl/RPG_BAT.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EBA8;
extern s32 D_8015EBD8;
s32 func_801447B8();
/* end bulk_m2c declarations */
void func_80144010(void) {
    s32 *var_v1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a3;
    s32 var_v0;
    s32 var_v0_2;
    u16 temp_t9;
    u32 var_a0_2;

    var_a0 = 0;
    var_a3 = 0xFF;
    var_v1 = &D_800E6280;
    var_v0 = 0;
    do {
        if (((u32) (*(s32 *)((unsigned char *)var_v1 + (1644)) << 5) >> 0x1E) == 3) {
            var_a3 = var_v0;
        }
        temp_t9 = (u16) *(s32 *)((unsigned char *)var_v1 + (1644));
        var_v0 += 1;
        var_v1 += 1;
        var_a0 += temp_t9;
    } while (var_v0 != 0xC);
    if (var_a3 == 0xB) {
        var_a3 = 0xFF;
        if (var_a0 == 0) {
            var_a3 = 0xB;
        }
    }
    D_8015EBA0 = var_a3;
    if (var_a3 == 0xFF) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
        D_8015EBFC.unk_34 = 0;
    } else {
        D_8015EBA4 = 1;
        D_8015EBA8 = 1;
        D_8015EBFC.unk_34 = 0x64;
    }
    temp_v1 = D_8015EB98;
    D_8015EBFC.unk_38 = 0;
    if (temp_v1 & 0x800F0000) {
        D_8015EBA4 = 0;
        D_8015EBA8 = 0;
    }
    temp_v0 = temp_v1 & 0xF0000;
    if ((temp_v0 == 0) && (D_8015EB9C == 0xD)) {
        D_8015EB9C = 9;
    }
    var_a0_2 = D_8015EB9C;
    if ((temp_v0 == 0) && (var_a0_2 == 0xE)) {
        var_a0_2 = 9;
    }
    if ((temp_v0 != 0) && (var_a0_2 != 0xD)) {
        var_a0_2 = 0xE;
    }
    if (temp_v1 & 0x80000000) {
        var_a0_2 = 2;
    }
    if (temp_v1 & 0x08000000) {
        var_a0_2 = 0xE;
    }
    if (temp_v1 != 0x08000000) {
        D_8015ED9C = 0xFFFF;
    } else {
        D_8015ED9C = 0;
        D_8015ED98 = 0x1000;
    }
    if (D_800E6280.unk_0FC.unk_02 == 0) {
        var_v0_2 = 0xA;
    } else {
        var_v0_2 = D_800E6280.unk_0FC.unk_02 * 0xA;
    }
    D_8015EDA0 = var_v0_2;
    D_8015ED94 = var_v0_2;
    D_8015EB9C = var_a0_2;
    switch (var_a0_2) {
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
    D_8015EDC4 = temp_v1;
    if (temp_v1 == 0x08000000) {
        D_8015EDC4 = 0x08000002;
    }
    switch (temp_v1) {
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
        if (func_801447B8(0x7FFFFFFF, D_800E6280.unk_0FC.unk_02, 0x08000000, 0x80000000) != 0) {
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
    switch (temp_v1) {
    default:
        D_8015EE18 = 0x7FFFFFFF;
        return;
    case 0x10000:
        D_8015EE18 = 0x10000075;
        return;
    case 0x20000:
        D_8015EE18 = 0x10000232;
        return;
    case 0x40000:
        D_8015EE18 = 0x1000012A;
        return;
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
