#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80146D60);

void func_80146E3C(void) {
    D_80122760 = 0x14;
    D_80122764 = 0x64;
    D_80122768 = -0x64;
    D_8012276C = 0xB0;
    D_8012276D = 0xB0;
    D_8012276E = 0xB0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x14;
    D_80122774 = -0x64;
    D_80122778 = 0x64;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0x80;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = 0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x60;
    D_8012278D = 0x60;
    D_8012278E = 0x60;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80146F74);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_801471D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_801472D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147400);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147444);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147488);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147674);

void func_801478F8(s32 arg0, s16 *arg1) {
    TcPos *p;

    p = &D_80128880[arg0];
    p->x = arg1[3];
    p->z = arg1[4];
    arg1 += 5;
    p->y = arg1[0];
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147928);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147B24);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147C98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147E80);
