#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801421E0", func_801421E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801421E0", func_80142544);

void func_801426B0(void) {
    s32 t;

    if (D_8015EDB0.unk_04 == 0) {
        func_8013E810(0x2B, 4, 1, 1);
        func_8013E810(0x2C, 0x13, 1, 1);
        func_8013E97C(0x2C, 0x110, 0x90);
    }
    func_8013E97C(0x2B, 0x110, D_8015EDB0.unk_04 * 4 - 0xC0);
    t = (D_8015EDB0.unk_04 << 14) / 292;
    D_8012121A = t;
    D_8012121C = t;
    if (D_8015EDB0.unk_04 * 4 - 0xC0 >= 0x55) {
        func_8013E810(0x2B, 0x15, 1, 1);
    }
    D_8015EDB0.unk_04 += 1;
    if (D_8015EDB0.unk_04 * 4 - 0xC0 >= 0x65) {
        func_8013E7C0(0x2B, 6, 1, 1);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8013E97C(0x2B, 0, 0);
        func_8014EBA8();
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801421E0", func_801427E4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801421E0", func_80142A30);

void func_80142CB8(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E97C(0x2A, 0, 0);
        func_8013E7C0(0x2A, 0xF, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        if (!(D_8012117A & 1)) {
            func_8013F0F4(0x505, 0xF, 5);
            func_8013E7C0(0x2A, 0x10, 5, 1);
            D_8015EDB0.unk_04 += 1;
            func_8014EBD0();
            return;
        }
        return;
    case 2:
        if (D_8015EDB0.unk_04 < 0x32) {
            func_8013F0F4(0x505, 0xF, 5);
            if (!(D_8012117A & 1)) {
                func_8013E7C0(0x2A, 0x10, 5, 1);
                D_8015EDB0.unk_04 += 1;
                return;
            }
        } else {
            func_8013E7C0(0x2A, 0xFF, 1, 0);
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801421E0", func_80142DF4);
