#include "common.h"
#include "ovl/KANGEI.h"

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132090);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132214);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132290);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132354);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801323A8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132430);

void func_801324B0(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801324D8(void) {
    func_80046318(3, 0x80197000, 0xAF3C);
    func_80132090();
    func_8004284C();
}

void func_80132514(void) {
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
    func_801325D0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801325D0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_8013260C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801327DC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132C24);
