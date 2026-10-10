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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80135AE0", func_80135B4C);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80135AE0", func_80135E80);
