#include "common.h"
#include "ovl/KANGEI.h"

void func_801355F0(void) {
    D_80139D10 = 0x801D4074;
    D_80139D14 = 0x801D4078;
    D_80139D18 = 0x801D4088;
    D_80139D1C = *(s16 *)0x801D4094;
}

void func_80135634(void) {
    D_80139D20 = 0x801D4074;
    D_80139D24 = 0x801D4078;
    D_80139D28 = 0x801D4088;
    D_80139D2C = *(s16 *)0x801D4094;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135678);

void func_801357A4(void) {
    D_800E71DF = D_800E69DD;
    func_8004284C();
}

void func_801357D0(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    func_8004284C();
}

void func_80135808(void) {
    func_80044750(0x24);
    func_80044750(0x502);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135838);

void func_80135AC0(void) {
    func_8004284C();
}

void func_80135AE0(void) {
    func_80042808();
    func_80042808();
    func_80042808();
    func_80042808();
}

INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/801355F0.rodata", D_801398B0);

void func_80135B18(void) {
    bg_read_sub2(0x42A9);
    func_800AE0F0(D_800CA19C, &D_801398B0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135B54);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135C18);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135C78);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/801355F0", func_80135CE0);

void func_80135E90(void) {
    if (D_800E71DF == 0xA) {
        func_80046318(9, 0x801D0000, 0xBCC8);
        func_801355F0();
    } else {
        func_80046318(9, 0x801D0000, 0xBD95);
        func_80135634();
    }
    func_8004500C(1, 0);
    func_8004284C();
}
