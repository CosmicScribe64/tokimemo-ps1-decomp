#include "common.h"
#include "ovl/KANGEI.h"

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80137B90);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80137E04);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138078);

void func_8013812C(void) {
    D_80139AC4 = 1;
    func_80042940(0);
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138158);

void func_80138264(void) {
    D_8013A2A8 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_8013828C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138320);

void func_8013836C(void) {
    if (((u32)(D_800E6378 << 0x17) >> 0x1B) == D_800E62C0 && (D_800E6378 & 0xF) == D_800E62BF) {
        D_800E738A += 0xC;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801383D0);

void func_8013844C(void) {
    D_80139AC0 = 0;
    func_80133C84();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138470);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_8013852C);

void func_801385E0(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
    D_80139ADC = 3;
    func_80139540();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138640);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801386C4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138728);

void func_801387B8(void) {
    func_80083440(D_80122CDC + 1);
    D_80139ADC = (D_80122CDC * 2) + 0xD;
    func_8004284C();
}

void func_801387FC(void) {
    if (D_80122CDC != 0) {
        D_80139AC0 = 1;
        D_800E738A += 0x39;
        return;
    }
    D_80139ADC += 1;
    func_8004284C();
}

void func_8013885C(void) {
    D_80139ADC = 0x22;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138884);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801388E4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138944);

void func_801389F4(void) {
    D_800E71DF = D_8013A2B0;
    func_801392D4();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138A34);

void func_80138B28(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138B90);

void func_80138C18(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    back_clear_switch(1);
    func_80048EB8(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    D_800E7322 = 0;
    D_800E7368 = 1;
    D_800B593C = 0;
    func_8008585C();
    parameter_disp_switch(0);
    parameter_disp_switch(0);
    icon_disp_switch(0);
    addr_init_bustup();
    func_80084E4C();
    D_80139AC0 = 0;
    func_8004284C();
}

void func_80138CD0(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80137E04();
    func_8004284C();
}

void func_80138D08(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80137B90();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138D40);

void func_80138E3C(void) {
    bg_read_sub2(0x42A9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138E64);

void func_80138F54(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138F88);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801392D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80139540);
