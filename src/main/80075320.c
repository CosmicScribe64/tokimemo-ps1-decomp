#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075320);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075468);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800754C0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800755B4);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075A64);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075AF8);

void func_80075BE8(void) {
    if (D_800E738A == 0) {
        func_80075C24();
    }
    func_80066334();
    func_80065B0C(0);
}

void func_80075C24(void) {
    k_disp_inc();
    func_8006BA40();
    if (func_80076000() == 0 && func_80075C84() == 0 && func_8005B0F4() == 0) {
        func_8005B1A8();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075C84);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80075FA0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076000);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800764AC);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076630);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_800767A4);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076B48);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076C7C);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076DA0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076EC0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076F48);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80076FDC);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077330);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007739C);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077694);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077900);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077BE0);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077C50);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077CA8);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_80077E30);

void magazine_exit(void) {
    func_8006D138();
    hizuke_disp_switch(1);
    func_80042908(0);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel);

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel_init);

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel_call);

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_tel_exit);

INCLUDE_ASM("asm/nonmatchings/main/80075320", telephone_class_init);

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_club_init);

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007866C);

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_club_exit);

void holiday_taibu_club_exit(void) {
    func_80048E78();
    if (D_800E7208 & 0x60) {
        func_80042908(0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", holiday_club);

void holiday_club_join_exit(void) {
    func_80048E78();
    icon_disp_switch(1);
    func_80042908(5);
}

INCLUDE_ASM("asm/nonmatchings/main/80075320", func_8007894C);
