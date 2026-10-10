#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_801326F0);

void func_8013279C(void) {
    s32 t;

    D_800B3D60 = 0;
    t = dec_bg_cd_read(0x3FC6, 0);
    if (t == D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (t == 1 - D_800B5938) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_8013281C);

void func_801328A8(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_801328D0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_801329F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_80132A48);

void func_80132AA0(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC657, 0xC623, 0xDA9F, 0xDA4F, 0xDA3D);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_80132B08);

void func_80132B88(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80132BB0(void) {
    func_80046318(0x11, 0x80197000, 0xAEEC);
    func_80132000();
    func_8004284C();
}

void func_80132BEC(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8008585C();
    message_window_init();
    hizuke_init();
    func_80084E4C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_80132C60);

void func_80132D84(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80132DAC(void) {
    bg_read_sub2(0x4335);
    func_8004284C();
}
