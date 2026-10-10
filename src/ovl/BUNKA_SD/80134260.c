#include "common.h"
#include "ovl/BUNKA_SD.h"

void func_80134260(void) {
    func_8008585C();
    func_80048E78();
    k_reset(1);
    tpage_buf_clear();
    D_800E62B6 = 0;
    D_800E62B8 = 0;
    D_800E62B7 = 0;
    if (D_80122EB8 == 1) {
        back_clear_switch(0);
    } else {
        back_clear_switch(1);
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    hizuke_disp_switch(0);
    func_80041584();
    D_8013C700 = get_k_speed();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_80134314);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_80134418);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_801344A8);
