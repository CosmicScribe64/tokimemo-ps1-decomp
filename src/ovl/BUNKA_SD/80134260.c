#include "common.h"
#include "ovl/BUNKA_SD.h"

void func_80134260(void) {
    func_8008585C();
    func_80048E78();
    k_reset(1);
    tpage_buf_clear();
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    D_800E6280.unk_037 = 0;
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

s32 func_80134314(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else {
        func_80048390();
        func_80048E78();
        if (D_80122EB8 == 1) {
            func_800573AC();
            func_800438F0(1);
        }
        func_8006492C(1);
        func_8004EBEC(*(u8 *)&D_8013C700);
        func_80042908(0x10);
        func_80044750(0x200);
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_80134418);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_801344A8);
