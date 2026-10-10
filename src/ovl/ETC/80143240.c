#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143240);

void func_80143334(void) {
    D_80150144 = 0;
    func_80143758();
    if (D_8015014C == 1) {
        draw2d3d(1, 1);
        func_80061710();
        func_80061790();
        func_800618B0();
        func_8014354C();
        func_80042808();
        return;
    }
    func_80044774(0);
    func_80046290(0x07000001, 0x02000E0C, 0xF);
    func_80044750(0x300);
    func_8014407C();
    func_80042808();
    func_80042940(0xFF);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801433E4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_8014354C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143644);

void func_8014369C(void) {
    func_80044774(0);
    func_80046290(0x07000001, 0x02000E0C, 0xF);
    func_80044750(0x300);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801436E4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143758);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_8014387C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143968);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143A88);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143F24);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80143FE0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_8014407C);

void func_801441DC(s32 arg0, s32 arg1) {
    u8 v;
    u8 *p;

    if (arg1 > 0x80) {
        v = 0x80;
    } else if (arg1 < 0) {
        v = 0;
    } else {
        v = arg1;
    }
    p = (u8 *)&D_801217D0 + arg0 * 36;
    p[0x16] = v;
    p[0x15] = v;
    p[0x14] = v;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144224);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144540);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801446E4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144768);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801447B0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144A38);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144AC0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144CC4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144DF0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144F74);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801451D4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145318);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801455B0);

void func_80145960(void) {
    switch (D_800E738A) {
    case 0x0:
        func_801447B0();
        break;
    case 0x1:
        func_80144AC0();
        break;
    case 0x2:
        func_80144DF0();
        break;
    case 0x3:
        func_80144F74();
        break;
    case 0xA0:
        func_801455B0();
        break;
    case 0xC0:
        func_80144CC4();
        break;
    case 0xF0:
        func_80145318();
        break;
    case 0xFF:
        func_801451D4();
        break;
    }
}

void func_80145A40(void) {
    if (D_800E7380 != 0) {
        func_80066C08(0);
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145A6C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145AF8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80145C00);

void func_80145F74(void) {
    if (D_800E738A == 0) {
        func_80145C00();
    }
}

void func_80145FA0(void) {
    back_clear_switch(1);
    if (D_800E738D != 0 && (u32)D_800E7384 < 0xFF) {
        func_8004AC18(0xFF - D_800E7384);
    }
}

void func_80145FF0(void) {
    back_clear_switch(1);
    switch (D_800E738D) {
    case 3:
        break;
    case 0:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 1:
        if (func_800460EC() & 2) {
            D_800E738D += 1;
        }
        break;
    case 2:
        func_80046290(0x1B000000, 0x01000004, 0xE);
        func_80044750(0x300);
        D_800E738D += 1;
        break;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801460C4);

void func_8014618C(void) {
    func_80041584();
    func_80048E78();
    func_8009C5E0(0);
    func_8004111C();
    if (D_800E73A4 != 0) {
        func_80042940(3);
        return;
    }
    if (D_800E7204 & 0x860) {
        func_80042940(1);
        return;
    }
    func_80042940(2);
}

void func_80146214(void) {
    if (*(u16 *)D_800E6280 == 0x100) {
        func_800590CC(0);
    }
    func_80042878(0xC4);
    func_8009C5E0(1);
}

void func_80146254(void) {
    if (*(u16 *)D_800E6280 == 0x100) {
        func_800590CC(0);
    }
    func_80042878(0x91);
    func_8009C5E0(1);
}

void func_80146294(void) {
    if (*(u16 *)D_800E6280 == 0x100) {
        func_800590CC(0);
    }
    D_800E71DF += 0x10;
    func_80042878(0x12);
    func_8009C5E0(1);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_801462E8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_8014636C);
