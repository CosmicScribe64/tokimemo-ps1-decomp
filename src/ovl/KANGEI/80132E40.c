#include "common.h"
#include "ovl/KANGEI.h"

void func_80132E40(void) {
    D_80139A50 = 0x801C1400;
    D_80139A54 = 0x801C1D6C;
    D_80139A58 = 0x801CA47C;
}

void func_80132E74(void) {
    D_80139A5C = 0x80199CD4;
    D_80139A60 = 0x8019EE30;
    D_80139A64 = 0x801A422C;
    D_80139A68 = 0x801A82AC;
    D_80139A6C = 0x801AB394;
    D_80139A70 = 0x801AF5FC;
    D_80139A74 = 0x801B1C9C;
    D_80139A78 = 0x801B3370;
    D_80139A7C = 0x80199EF4;
    D_80139A80 = 0x8019F078;
    D_80139A84 = 0x801A447C;
    D_80139A88 = 0x801A8408;
    D_80139A8C = 0x801AB4E0;
    D_80139A90 = 0x801AF824;
    D_80139A94 = 0x801B1D10;
    D_80139A98 = 0x801B3478;
    D_80139A9C = 0x8019BC24;
    D_80139AA0 = 0x801A0FD8;
    D_80139AA4 = 0x801A63F8;
    D_80139AA8 = 0x801A96B8;
    D_80139AAC = 0x801AC800;
    D_80139AB0 = 0x801B17AC;
    D_80139AB4 = 0x801B2090;
    D_80139AB8 = 0x801B41F4;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80132FF8);

void func_801335A0(void) {
    D_800E6280.unk_71C += 1;
    D_80139AC4 = 0;
    D_80139AC8 = 0;
    func_80072338();
    hizuke_init();
    func_80042808();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_801335F0);

void func_80133734(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    func_8004284C();
}

void func_801337B4(void) {
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80133834(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801338B4(void) {
    if (D_800E6280.unk_1104.w == 0) {
        func_8004500C(0, 0);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_801338EC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A14);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A54);
