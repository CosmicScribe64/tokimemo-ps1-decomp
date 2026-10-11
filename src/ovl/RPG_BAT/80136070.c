#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80136070", func_80136070);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80136070", func_801361AC);

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
