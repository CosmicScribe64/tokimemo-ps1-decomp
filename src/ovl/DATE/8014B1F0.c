#include "common.h"
#include "ovl/DATE.h"

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014B1F0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014C08C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014C20C);

void func_8014C550(void) {
    D_800CA134 = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = D_8015E208;
    D_800CA140 = D_8015E20C;
    D_800CA144 = D_8015E210;
    func_80082764(0xFF, 1, 0);
}

void func_8014C5C8(void) {
    D_800CA134 = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = D_8015E208;
    D_800CA140 = D_8015E20C;
    D_800CA144 = D_8015E210;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014C644);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014C6CC);

void func_8014C754(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_8014C7D4(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014B1F0", func_8014C7FC);

void func_8014C9A8(void) {
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
