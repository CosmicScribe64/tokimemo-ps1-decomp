#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132000);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132214);

void func_80132334(void) {
    D_800CA134 = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    func_80082764(D_80122CDC, 1, 0);
}

void func_801323B0(void) {
    D_800CA134 = &D_8013A430;
    D_800CA138 = &D_8013A434;
    D_800CA13C = D_8013A384;
    D_800CA140 = D_8013A3B0;
    D_800CA144 = D_8013A3DC;
    func_80082764(D_80122CDC, 1, 0);
}

void func_8013242C(void) {
    D_800CA134 = &D_8013A438;
    D_800CA138 = &D_8013A43C;
    D_800CA13C = D_8013A410;
    D_800CA140 = D_8013A414;
    D_800CA144 = D_8013A418;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801324A8);

void func_8013257C(void) {
    D_800CA134 = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    func_80082764(0xFF, 1, 0);
}

void func_801325F4(void) {
    D_800CA134 = &D_8013A428;
    D_800CA138 = &D_8013A42C;
    D_800CA13C = D_8013A404;
    D_800CA140 = D_8013A408;
    D_800CA144 = D_8013A40C;
    func_80082764(D_80122CDC, 1, 2);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132670);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801326F0);

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

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013281C);

void func_801328A8(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801328D0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801329F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132A48);

void func_80132AA0(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC657, 0xC623, 0xDA9F, 0xDA4F, 0xDA3D);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132B08);

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

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132C60);

void func_80132D84(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80132DAC(void) {
    bg_read_sub2(0x4335);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132DE0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132E5C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132EC8);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132F48);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80132FB0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013301C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133088);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801330D4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133258);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133494);

void func_8013357C(void) {
    func_80062CD0(0x6C8C);
    func_8004284C();
}

void func_801335A4(void) {
    D_800E71DF = 0;
    get_p_name(&D_800CA17C, 0);
    func_80062CD0(0x55DF);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801335E4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133620);

void func_80133720(void) {
    if (D_80122CDC != 0) {
        func_80133620();
        D_8013A4C4 |= 1 << (s8)D_8013A4C0;
    } else {
        func_801335A4();
    }
    func_80133258();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133784);

void func_80133884(void) {
    if (D_8013A4C0 == 2) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_801338C4(void) {
    D_8013A4C0 += 1;
    func_80042940(0x1B);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801338F8);

void func_801339F8(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133A6C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80133AF0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801348F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801350A4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80135DA4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80135E50);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801365DC);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136660);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801366E8);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136794);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136860);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136A90);

void func_80136B7C(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136BA4);

void func_80136CEC(void) {
    D_8013A428 = 0x17;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136D14);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136D50);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80136FC4);

void func_80137064(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80136D50();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013709C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801370E4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137360);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801375D4);

void func_8013765C(void) {
    func_80042908(2);
    func_80042940(D_800E69A2);
}

void func_8013768C(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80137360();
    func_8004284C();
}

void func_801376C4(void) {
    D_80122CDC = D_8013A818;
    func_8004284C();
}

void func_801376F0(void) {
    D_8013A818 = D_80122CDC;
    D_800CA148 = 0;
    D_800CA14C = 0;
    D_800CA160 = D_8013A774;
    D_800CA164 = D_8013A7A8;
    D_800CA168 = D_8013A7DC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013775C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013780C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801378A4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137948);

void func_801379CC(void) {
    D_800CA148 = 0x1D;
    D_800CA160 = D_8013A80C;
    D_800CA164 = D_8013A810;
    D_800CA168 = D_8013A814;
    func_80137A2C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137A2C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137AF0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137B94);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80137D50);

void func_80137F64(void) {
    func_80083808();
    func_80138048();
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
    func_80047560();
}

void func_80137FCC(void) {
    D_800CA134 = &D_8013B5F8;
    D_800CA138 = &D_8013B5FC;
    D_800CA13C = D_8013B5EC;
    D_800CA140 = D_8013B5F0;
    D_800CA144 = D_8013B5F4;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138048);

void func_801380C4(void) {
    func_80046318(2, 0x801E8000, 0xBD02);
    func_80138100();
    func_8004284C();
}

void func_80138100(void) {
    D_8013B5D4 = 0x801E80D0;
    D_8013B5D8 = 0x801E82C0;
    D_8013B5DC = 0x801E8444;
    D_8013B5E0 = 0x801E8700;
}

void func_80138144(void) {
    func_80044750(0x202);
    D_80122D20 = 0;
    func_8004284C();
}

void func_80138170(void) {
    if (D_80122CDC != 0) {
        func_80083440(2);
    }
    func_8004284C();
}

void func_801381A4(void) {
    func_80042878(0x50);
    func_80042908(1);
    func_80042940(0x11);
}

void func_801381D4(void) {
    func_80046318(3, 0x801B0000, 0xAF39);
    func_80137D50();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013820C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013850C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_8013856C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801385CC);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801388C0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138A3C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138A90);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138BEC);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138E0C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_80138F8C);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801391A0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801393B4);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2", func_801395C8);
