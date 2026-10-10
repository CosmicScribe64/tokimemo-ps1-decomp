#include "common.h"
#include "ovl/DATE2.h"

typedef struct {
    void (*f[31])();
} FnTbl31; /* size 0x7C */
extern FnTbl31 D_8013A440;

void func_801326F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl31 tbl;

    tbl = D_8013A440;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if (D_800E7389 == 0 && (u8)D_800E738A < 0x13U) {
        func_80066C08(1);
    }
}

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

s32 func_8013281C(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800573AC();
        func_8004284C();
        D_800E738A -= 3;
        return 0;
    }
    func_8005751C(0);
    func_8004284C();
}

void func_801328A8(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/801326F0", func_801328D0);

void func_801329F0(void) {
    func_80132670();
    if (D_800E7384++ == 0) {
        if (D_8013A42C == 1) {
            func_80044750(0x60A);
        }
    }
}
void func_80132A48(void) {
    func_80132670();
    if (D_800E7384++ == 0) {
        if (D_8013A42C == 1) {
            func_80044750(0x60B);
        }
    }
}
void func_80132AA0(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC657, 0xC623, 0xDA9F, 0xDA4F, 0xDA3D);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132B08(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

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
