#include "common.h"
#include "ovl/TACO.h"

void func_801480B0(void) {
    if ((u32)D_800E6280.unk_1100 < 0x60 && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0((s32)"1ST BOSS", -0x30, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80148184();
        break;
    case 1:
        func_80148308();
        break;
    case 2:
        func_801483B0();
        break;
    case 3:
        func_801484E4();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_80148184);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_80148308);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_801483B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_801484E4);

void func_8014866C(void) {
    D_80122760 = -0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x1E;
    D_8012276C = 0xE0;
    D_8012276D = 0xE0;
    D_8012276E = 0xE0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x28;
    D_80122774 = 0x32;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0xA;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x30;
    D_8012278D = 0x30;
    D_8012278E = 0x30;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_801487A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_80148B58);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_80148D6C);
