#include "common.h"
#include "ovl/GEKO.h"

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80134D10);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80134F84);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801351F8);

void func_80135340(void) {
    s32 var_v0;

    D_800B3D60 = 0;
    if ((D_80145074 != 0) || (D_800E699E & 0x20)) {
        if (((u8) D_800E62BF >= 3U) && ((u8) D_800E62BF < 6U)) {
            var_v0 = dec_bg_cd_read(0x3FE9, 0);
        } else if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 9U)) {
            var_v0 = dec_bg_cd_read(0x3FF2, 0);
        } else if (((u8) D_800E62BF >= 9U) && ((u8) D_800E62BF < 0xCU)) {
            var_v0 = dec_bg_cd_read(0x3FD7, 0);
        } else {
            var_v0 = dec_bg_cd_read(0x3FE0, 0);
        }
    } else if (((u8) D_800E62BF >= 3U) && ((u8) D_800E62BF < 6U)) {
        var_v0 = dec_bg_cd_read(0x3FBE, 0);
    } else if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 9U)) {
        var_v0 = dec_bg_cd_read(0x3FC6, 0);
    } else if (((u8) D_800E62BF >= 9U) && ((u8) D_800E62BF < 0xCU)) {
        var_v0 = dec_bg_cd_read(0x3FAD, 0);
    } else {
        var_v0 = dec_bg_cd_read(0x3FB6, 0);
    }
    if (var_v0 == *D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (var_v0 == (1 - *D_800B5938)) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801354E4);

void func_80135570(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

void func_80135598(void) {
    func_80042940(1);
}

void func_801355B8(void) {
    D_80145060 = 0x23;
    D_80145064 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801355E8);

void func_80135894(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80134F84();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801358CC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80135A6C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80135AC8);

void func_80135BC4(void) {
    D_800B3D60 = 0;
    if (D_800E691C == 0x40) {
        func_8004500C(0, 0x200);
    }
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80135C48);

void func_80135CC8(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80134D10();
    func_8004284C();
}

void func_80135D00(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    func_8008585C();
    func_80084E4C();
    D_800E7322 = 0;
    D_800E7368 = 1;
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    D_8014507C = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80135DC0);

void func_80136694(void) {
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

void func_801366DC(void) {
    if (D_80145088 == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80136714);

void func_801367C4(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801367EC);

void func_80136B2C(void) {
    D_800CA134 = &D_80145060;
    D_800CA138 = &D_80145064;
    D_800CA13C = D_80145048;
    D_800CA140 = D_8014504C;
    D_800CA144 = D_80145050;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80136BA8(void) {
    D_800CA134 = &D_80145060;
    D_800CA138 = &D_80145064;
    D_800CA13C = D_80145048;
    D_800CA140 = D_8014504C;
    D_800CA144 = D_80145050;
    func_80082764(0xFF, 1, 0);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80136C20);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80136E7C);

void func_80136EE4(void) {
    if (D_80145088 == 0) {
        func_80136B2C();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80136F20);

void func_80136FB8(void) {
    if (D_80145088 == 1) {
        func_8004284C();
        return;
    }
    normal_date_girl_in();
}

void func_80136FF8(void) {
    if (D_80145088 == 1) {
        normal_date_girl_in();
        return;
    }
    D_80145060 = 4;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137040);

void func_801370EC(void) {
    D_80145060 = (D_80122CDC * 2) + 0xD;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137120);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801372D4);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_801374A8);

void func_80137560(void) {
    func_80042940(0x31);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137580);

void func_80137630(void) {
    D_80145060 = D_80145068;
    D_80145048 = D_80145054;
    D_8014504C = D_80145058;
    D_80145050 = D_8014505C;
    func_80137894();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137694);

void func_80137844(void) {
    if (D_80145088 == 1) {
        D_80145070 = 1;
        func_8007C8A4();
        return;
    }
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137894);

void func_80137954(void) {
    if (D_8014508C == 0) {
        func_8004284C();
        return;
    }
    normal_date_bg_out();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80137990);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_8013882C);

void func_80138A80(void) {
    D_800CA134 = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = D_80146274;
    D_800CA140 = D_80146278;
    D_800CA144 = D_8014627C;
    func_80082764(0xFF, 1, 0);
}

void func_80138AF8(void) {
    D_800CA134 = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = D_80146274;
    D_800CA140 = D_80146278;
    D_800CA144 = D_8014627C;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138B74);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138BF0);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138CDC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138D64);

void func_80138DE4(void) {
    func_8004500C(1, 0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138E10);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134D10", func_80138F7C);

void func_80139128(void) {
    _sprite_set_light_effect1(0x120, -0xF8, 0xA0, 0x3C, -0xA0, 0x3C, 9, 0x808080, 0);
    _sprite_set_light_effect1(0x120, -0xF8, 0x40, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0);
    _sprite_set_light_effect1(0x120, -0xF8, 0x10, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x40);
    _sprite_set_light_effect1(0x120, -0xF8, -0x1C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x63);
    _sprite_set_light_effect1(0x120, -0xF8, -0x50, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x24);
    _sprite_set_light_effect1(0x120, -0xF8, -0x8C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x73);
    _sprite_set_light_effect1(0x120, -0xF8, -0xF0, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x11);
    _sprite_set_light_effect1(0x120, -0xF8, -0x140, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x50);
    _sprite_set_light_effect1(0x120, -0xF8, -0x30, 0x3C, 0, 0xB4, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
    _sprite_set_light_effect1(-0x140, -0x118, -0x142, -0x141, -0xA2, -0x79, 9, 0x808080, 0);
    dtd_on_tpage(0, 0, 9, 1, 0);
}
