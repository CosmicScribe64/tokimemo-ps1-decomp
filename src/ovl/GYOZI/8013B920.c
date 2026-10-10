#include "common.h"
#include "ovl/GYOZI.h"

void func_8013B920(void) {
    switch (D_801474B8) {
    case 0:
        func_8013B980();
        return;
    case 1:
        func_8013B9FC();
        return;
    default:
        func_8013BA84();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013B980);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013B9FC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013BA84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013BB0C);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013B920.rodata", D_80145B80);

void func_8013BBFC(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145B80);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013BC40);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013B920", func_8013BCB4);

void func_8013BD28(void) {
    func_8008A0D4(0x4011);
    func_8004DE1C();
}
