#include "common.h"
#include "ovl/TACO.h"

s32 func_80137590(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013760C();
        break;
    case 1:
        func_8013788C();
        break;
    }
    if (D_800E6280.unk_10F8 & 0x10) {
        func_801432F0("DEMO PLAY", -0x40, 0, 2);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137590", func_8013760C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137590", func_8013788C);
