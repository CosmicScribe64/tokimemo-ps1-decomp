#include "common.h"
#include "ovl/RPG_BAT.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EBA8;
void func_8013D0D4(void);
/* end bulk_m2c declarations */
void func_8013CC90(void) {
    s32 temp_t7;
    s32 temp_t8;
    s32 var_v1;
    s32 var_v1_2;

    func_8014BB80();
    if (func_801445E8(0) < D_8015ED9C) {
        D_8015ED98 |= 0x10000;
        D_8015ED9C = 0;
    }
    if (!(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF) && !(D_8015EDF0 & 0xFF) && (((u32) D_8015ED98 >> 0x18) != 0)) {
        func_8013D030();
        temp_t8 = D_8015ED98 & 0xFF000000;
        switch (temp_t8) {
        default:
            var_v1 = D_8015EBFC.unk_34;
            break;
        case 0x1000000:
            D_8015ED98 = 1;
            if (D_8015EB98 == 0x80000000) {
                D_8015EE00.unk_00 += 0x258;
            }
            var_v1 = D_8015EBFC.unk_34 + func_8013E9A8(0xF) + 0x1E;
            break;
        case 0x2000000:
            D_8015ED98 = 0x100;
            var_v1 = D_8015EBFC.unk_34 + func_8013E9A8(0xF) + 0x14;
            break;
        case 0x4000000:
            D_8015ED98 = 2;
            D_8015EBA8 = 0;
            var_v1 = D_8015EBFC.unk_00 - 0x64;
            break;
        }
        if (var_v1 >= 0xAB) {
            var_v1 = 0xAA;
        }
        D_8015EBFC.unk_34 = var_v1;
        if ((var_v1 >= 0x64) && (D_8015EBA4 == 1)) {
            D_8015EBA8 = 1;
        }
    }
    temp_t7 = D_8015ED98 & 0x1FF;
    var_v1_2 = temp_t7;
    switch (temp_t7) {
    case 0x100:
        func_8013AD50();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x104:
        func_8013AD50();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x1:
        func_8013AF1C();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    case 0x2:
        func_8013D0D4();
        var_v1_2 = D_8015ED98 & 0x1FF;
        break;
    }
    if ((var_v1_2 == 0) && !(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF) && !(D_8015EE1C & 0xFF) && (!(D_800E6280.unk_F80 & 4) || !(D_800E6280.unk_F80 & 8) || !(D_8015EB98 & 0xF0000))) {
        func_8013D030();
        var_v1_2 = D_8015ED98 & 0x1FF;
    }
    if (var_v1_2 == 0) {
        if (!(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF)) {
            if (D_8015EB98 & 0xF0000) {
                if ((D_800E6280.unk_F80 & 4) && (D_800E6280.unk_F80 & 8)) {
                    if (((DisplayActor44 *)D_8011ECD0)[144].unk_16 != 2) {
                        func_8013E7C0(0x30, 2, 1, 1);
                    }
                } else {
                    func_8013D030();
                }
            }
        }
    }
}

void func_8013D030(void) {
    if (D_8015ED94 == 0) {
        func_8013E7C0(0x30, 7, 1, 1);
        return;
    }
    /* FAKE: absolute address instead of D_8015ED98; the original's lui and lw use different registers. Real source unknown. T-4010 */
    if (*(s32 *)0x8015ED98 & 0x100) {
        func_8013E7C0(0x30, 3, 1, 1);
        return;
    }
    func_8013E7C0(0x30, 0, 1, 1);
    if (D_8015ED94 < 0xA) {
        func_8013E7C0(0x30, 4, 1, 1);
    }
}

void func_8013D0D4(void) {
    switch (D_8015EBA0) {
    case 0:
        func_801368B0();
        return;
    case 1:
        func_801505B8();
        return;
    case 2:
        func_801421E0();
        return;
    case 3:
        func_8013A650();
        return;
    case 4:
        func_80154430();
        return;
    case 5:
        func_80134D30();
        return;
    case 6:
        func_8015A1F0();
        return;
    case 7:
        func_801484E0();
        return;
    case 8:
        func_80155CC0();
        return;
    case 9:
        func_8014A660();
        return;
    case 10:
        func_801321D0();
        return;
    case 11:
        func_801498C0();
        return;
    }
}

void func_8013D1D0(void) {
    if (!(D_8015EDB0.unk_04 & 1)) {
        D_80121423 |= 0x80;
        if (D_8015EB9C != 0xE) {
            D_80120E07 |= 0x80;
        }
    } else {
        D_80121423 &= ~0x80;
        if (D_8015EB9C != 0xE) {
            D_80120E07 &= ~0x80;
        }
    }
    D_8015EDB0.unk_04++;
    if (D_8015EDB0.unk_04 >= 0x3C) {
        func_8014EBA8();
    }
}

void func_8013D290(void) {
    if (!(D_8015EDB0.unk_04 & 1)) {
        D_80121423 |= 0x80;
        if (D_8015EB9C != 0xE) {
            D_80120E07 |= 0x80;
        }
    } else {
        D_80121423 &= ~0x80;
        if (D_8015EB9C != 0xE) {
            D_80120E07 &= ~0x80;
        }
    }
    D_8015EDB0.unk_04++;
    if (D_8015EDB0.unk_04 >= 0x3D) {
        func_8014EBA8();
    }
}

void func_8013D350(void) {
    if (D_8015EDC4 & 0x100000) {
        func_8013EA60(0, 0, D_8015EBB0, 1);
        func_8013EA60(1, 1, D_8015EBB4, 1);
        func_8013EA60(2, 2, D_8015EBB8, 1);
        return;
    }
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

void func_8013D3E4(void) {
    func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
}

void func_8013D418(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E97C(0x30, 0, 0);
        func_8013E7C0(0x30, 1, 1, 1);
        func_8014EBD0();
        return;
    case 1:
        func_8013E97C(0x30, -D_8015EDB0.unk_08, 0);
        D_8015EDB0.unk_08 += 2;
        if (D_8015EDB0.unk_08 >= 0x21) {
            func_8013E7C0(0x30, 0, 1, 0);
            func_8013E97C(0x34, 0, 0);
            if (D_8015EDC4 & 0x600000) {
                D_80121426 = 0x82;
                func_8013E7C0(0x34, 1, 5, 1);
            } else {
                D_80121426 = 0x82;
                func_8013E7C0(0x34, 0, 5, 1);
            }
            func_8014EBD0();
            return;
        }
        return;
    case 2:
        D_8015EDB0.unk_08 += 1;
        if (D_8015EDB0.unk_08 == 1) {
            func_8013F15C(0x503, 1, 0);
        }
        func_8014EDFC(0x34);
        return;
    case 3:
        D_80121426 = 1;
        func_8014EBA8();
        break;
    }
}

void func_8013D59C(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        if (D_8015EDC4 & 0x600000) {
            func_8013E7C0(0x34, 1, 0x21, 1);
            D_80121426 = 0x82;
            D_80121438 = 0xF;
        } else {
            func_8013E7C0(0x34, 0, 0x21, 1);
            D_80121426 = 0x82;
            D_80121438 = 0xF;
        }
        func_8013E97C(0x34, 0, 0);
        func_8014EBD0();
        return;
    case 1:
        func_8014EDFC(0x34);
        return;
    case 2:
        D_80121426 = 1;
        func_8013E7C0(0x34, 0, 1, 0);
        func_8013E7C0(0x30, 0, 1, 1);
        func_8014EBD0();
        return;
    case 3:
        func_8014ED80(0x14);
        return;
    case 4:
        func_8013E7C0(0x30, 2, 1, 1);
        func_8014EBD0();
        return;
    case 5:
        func_8013E97C(0x30, D_8015EDB0.unk_04 - 0x20, 0);
        D_8015EDB0.unk_04 += 2;
        if (D_8015EDB0.unk_04 >= 0x21) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8014EBD0();
            return;
        }
    default:
        return;
    case 6:
        func_8014ED80(0x14);
        return;
    case 7:
        func_8014EBA8();
        break;
    }
}
