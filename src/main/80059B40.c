#include "common.h"
#include "game.h"

void func_80059B40(void) {
    switch (D_800E6280.unk_110A) {                           /* irregular */
    case 0:
        func_80059E00();
        return;
    case 1:
        func_80059BC0();
        return;
    case 2:
        func_8004ADE4();
        func_8004284C();
        return;
    default:
        func_80042808();
        return;
    }
}

void func_80059BC0(void) {
    func_80059BE8();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059BE8);

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059E00);

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059EF0);

void func_8005A06C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80059B40();
        return;
    }
    func_80042878(0x12);
}
