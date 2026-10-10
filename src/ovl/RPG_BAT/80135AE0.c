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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80135AE0", func_80135D90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80135AE0", func_80135E80);
