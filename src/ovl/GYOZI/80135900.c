#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135900", func_80135900);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135900", func_80135988);

void func_80135A20(void) {
    if (D_80145F60 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135A5C(void) {
    if (D_8012E66C != 0) {
        func_8008E2E0();
        return;
    }
    func_8008E280();
}

void func_80135A98(void) {
    func_80090960(5, 0);
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80135ACC(void) {
    if ((D_800F62CF != 5) || (D_80145F60 != 0) || (D_80145EC8 == 0)) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void func_80135B30(void) {
    if (D_80145F60 != 0) {
        D_800F647A += 4;
    }
    func_8004DE1C();
}

void func_80135B70(void) {
    if (D_80145EC8 == 0) {
        func_8004DE1C();
        return;
    }
    func_801343A4();
}

void func_80135BAC(void) {
    if ((D_800F62CF == 5) && (D_80145EC8 == 0)) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135C00(void) {
    if (D_80145EC8 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135900", func_80135C3C);
