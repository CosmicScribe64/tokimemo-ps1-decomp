#include "ovl/OMIMAI.h"

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132000);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801320A0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801322E4);

void func_80132398(void) {
    if ((D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E6378 & 0xF))) {
        func_80042808();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801323F8);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801324BC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132544);

void func_801325C4(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801325EC(void) {
    func_80046318(4, 0x80197000, 0xAF3F);
    func_801320A0();
    func_80044750(0x603);
    func_8004284C();
}

void func_80132630(void) {
    func_8004E58C();
    k_reset(1);
    addr_init_bustup();
    func_80084E4C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132670);

void func_80132788(void) {
    bg_read_sub2(0x4144);
    func_80085B3C(0xF, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801327BC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_8013285C);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132AD0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132D44);

void func_80132DC0(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80132AD0();
    func_8004284C();
}

void func_80132DF8(void) {
    func_80085B3C(0xD, 0x25);
    D_800E699E |= 4;
    D_800CA160 = D_80134A8C;
    D_800CA164 = D_80134ABC;
    D_800CA168 = D_80134AEC;
    D_800CA148 = 2;
    D_800CA14C = 0;
    func_8004284C();
}

void func_80132E78(void) {
    func_80132EB0();
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80132EB0);

void func_80133120(void) {
    func_80083808();
    if (D_800E7389 == 0) {
        func_80133424();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_OMIMAI_801331A4);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801331B0);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80133424);

void func_801334AC(void) {
    if ((D_800E71DF == 4) && (D_800CA148 == 5)) {
        normal_date_speak_1line();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80133500);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801335C4);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_8013364C);

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

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801337EC);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_8013388C);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80134030);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_801340E4);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80134384);

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI", func_80134628);
