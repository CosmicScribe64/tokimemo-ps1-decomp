#include "ovl/OMIMAI.h"

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801331B0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80133424);

void func_801334AC(void) {
    if ((D_800E71DF == 4) && (D_800CA148 == 5)) {
        normal_date_speak_1line();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80133500);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801335C4);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_8013364C);

void func_801336CC(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801336F4(void) {
    func_80046318(6, 0x80197000, 0xAF33);
    func_801331B0();
    func_8004284C();
}

void func_80133730(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    func_80084E4C();
    func_801337EC();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801337EC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_8013388C);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80134030);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_801340E4);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80134384);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801331B0", func_80134628);
