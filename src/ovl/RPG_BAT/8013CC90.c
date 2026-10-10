#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013CC90);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D0D4);

void func_8013D1D0(void) {
    if (!(D_8015EDB4 & 1)) {
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
    D_8015EDB4++;
    if (D_8015EDB4 >= 0x3C) {
        func_8014EBA8();
    }
}

void func_8013D290(void) {
    if (!(D_8015EDB4 & 1)) {
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
    D_8015EDB4++;
    if (D_8015EDB4 >= 0x3D) {
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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D418);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013CC90", func_8013D59C);
