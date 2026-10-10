#include "common.h"
#include "ovl/TACO.h"

void func_80148FE0(void) {
    if (((u32) D_800E6280.unk_1100 < 0x60U) && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0("2ST BOSS", -0x30, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801490B4();
        break;
    case 1:
        func_8014927C();
        break;
    case 2:
        func_801493D4();
        break;
    case 3:
        func_80149538();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801490B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_8014927C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801493D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149538);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801497EC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149B7C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149D1C);
