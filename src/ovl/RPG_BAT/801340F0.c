#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801340F0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_8013415C();
        return;
    case 2:
        func_80134570();
        return;
    case 3:
        func_80134984();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_8013415C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_80134570);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_80134984);
