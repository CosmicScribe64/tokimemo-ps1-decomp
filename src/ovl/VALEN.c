#include "ovl/VALEN.h"

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132000);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132274);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132348);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_801323D0);

void func_8013244C(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_80044750(0x201);
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_801324B4);

void func_8013253C(void) {
    func_80046318(7, 0x80197000, 0xAFAC);
    func_80132000();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132578);

void func_801326B8(void) {
    bg_read_sub2(0x4045);
    func_80085B3C(6, 0);
    func_8004284C();
}

void func_801326EC(void) {
    bg_read_sub2(0x4122);
    func_8004284C();
}

void func_80132714(void) {
    D_80134498 = (D_80134498 + (D_800E62BE * 4)) - 0x180;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132760);

void func_801327E8(void) {
    func_800847B8(9);
    func_80132348();
}

void func_80132810(void) {
    D_800E71DF = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132834);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80132D0C);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133088);

void func_801334DC(void) {
    D_80134520 += 1;
    func_80042940(1);
}

void func_80133510(void) {
    if (strcmp(D_800CA19C, D_800CA1DC) == 0) {
        func_8004284C();
    }
    strcpy(D_800CA19C, D_800CA1DC);
    func_8004284C();
}

void func_80133568(void) {
    if ((D_80134544 == 1) && (D_800E71DF == 9)) {
        D_800E738A += 6;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_801335C0);

void func_80133610(void) {
    func_800847B8(0);
    func_80062CD0(0x544A);
    func_8004284C();
}

void func_80133640(void) {
    func_800847B8(9);
    func_80062CD0(0x64D0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133670);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133728);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_8013387C);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_801339D0);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133A4C);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133AA8);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133B6C);

void func_80133C44(void) {
    bg_read_sub2(0x414C);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133C70);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80133EE4);

void func_80133F58(void) {
    if (D_800E71DF == 9) {
        func_8004284C();
        return;
    }
    normal_date_girl_in();
}

void func_80133F98(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80133C70();
    func_8004284C();
}

void func_80133FD0(void) {
    D_8013448C = D_80134400;
    D_80134490 = D_80134434;
    D_80134494 = D_80134468;
    D_80134498 = 0xC;
    D_8013449C = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN", func_80134030);

void func_801342E8(void) {
    func_80042908(D_800E69A1);
    func_80042940(D_800E69A2);
}
