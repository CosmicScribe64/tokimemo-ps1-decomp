#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133870);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133980);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133AF4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133C08);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133E98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801342CC);

void func_80134450(void) {
    func_8004435C(0x100, 0x1E0, 0x100, 0x18, 0x801B8000);
    func_8004435C(0, 0x1F0, 0x100, 0x10, 0x801C0000);
    func_80143730(0x801C8000, 8);
    func_80143730(0x801C8000, 0xA);
    func_80143730(0x801C8000, 0xC);
    func_80143730(0x801C8000, 0xE);
    func_80143730(0x801C8000, 0x18);
    func_80143730(0x801C8000, 0x1A);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80134500);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801345D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_8013474C);

void func_801347A8(void) {
    s32 i;

    func_80059048();
    for (i = 0; i < 0x40; i++) {
        D_80127080[i].unk0 = 0x80000000;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801347F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80134884);
