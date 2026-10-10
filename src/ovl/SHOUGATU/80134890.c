#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80134890(void) {
    D_80143DB0 = 0x801DABE0;
    D_80143DB4 = 0x801DB684;
    D_80143DB8 = 0x801DABE8;
    D_80143DBC = 0x801DB6A0;
    D_80143DC0 = 0x801DAC48;
    D_80143DC4 = 0x801DB6CC;
    D_80143DC8 = *(s16 *)0x801DB6E0;
    D_80143DCC = *(s16 *)0x801DB6E4;
    D_80143DD0 = 0x801C0000;
    D_80143DD4 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_80134930);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_80134A54);

void func_80134C34(void) {
    if ((D_80143B20 == 0) && (D_800E6280.unk_75E[D_800E6280.unk_F5E - 1].unk_02 != 5)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 5);
        } else {
            func_80085B3C(5, 2);
        }
    }
    bg_read_sub2(0x4200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_80134CB4);

void func_80134F10(void) {
    func_80044750(0x201);
    func_8004284C();
}

void func_80134F38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80134F60(void) {
    if ((D_800E6280.unk_F5F == 2) && (D_80143B20 == 0)) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80134FAC(void) {
    if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
        D_80143B00 += 2;
    } else if (D_80143DD8 != 0) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_80135014(void) {
    if (D_80143DD8 == 0) {
        D_80143B00 += 2;
    } else if ((D_80143DD8 != 3) && (D_80143DD8 != 4)) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_8013507C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_801350F8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_801359FC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134890", func_80135B10);

void func_80135B54(void) {
    func_80046318(0x37, 0x801C0000, 0xBD5E);
    func_8004284C();
}
