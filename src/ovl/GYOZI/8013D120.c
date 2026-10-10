#include "common.h"
#include "ovl/GYOZI.h"

void func_8013D120(void) {
    if (D_801474B8 == 0) {
        func_8013D15C();
        return;
    }
    func_8013D20C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D120", func_8013D15C);

void func_8013D1E4(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D120", func_8013D20C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D120", func_8013D294);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D120.rodata", D_80145C00);

void func_8013D3BC(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C00);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D120.rodata", D_80145C0C);

void func_8013D400(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C0C);
    func_8004DE1C();
}

void func_8013D448(void) {
    func_8008A0D4(0x4276);
    func_8004DE1C();
}
