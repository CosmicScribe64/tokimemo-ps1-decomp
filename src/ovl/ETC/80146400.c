#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146400);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801464D4);

void func_801465F8(void) {
    s32 *p;
    s32 y;

    switch (D_800E6280.unk_1109) {
    case 0:
        func_80146B40();
        break;
    case 1:
        func_80146714();
        break;
    case 2:
        func_80148078();
        break;
    }
    func_80066C08(1);
    func_80064F48();
    func_80064DEC();
    func_800646CC();
    func_8006BA40();
    func_80065B0C(1);
    if (D_800E6280.unk_041 == 5) {
        func_8006764C(0);
    } else {
        func_80067870();
    }
    func_800578F4(0);
    p = &D_801230D0;
    for (y = 0x28; y != 0x58; y += 8) {
        func_8004B19C(p[10], 0x40, y);
        func_8004B19C(p[4], 0x50, y);
        p++;
    }
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146714);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801467C4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801468FC);

void func_80146A38(void) {
    if (func_800460CC() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(0x3FA4, 0);
        } else {
            set_dec_bri(0x80);
            func_8004284C();
            func_800578F4(0);
        }
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146AC0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146B40);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146BC4);

void func_80146C68(s32 arg0) {
    func_80048F64(0x60);
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    D_80120650[1] = 1;
    D_80120650[2] = 4;
    *(s32 *)&D_80120650[0x38] = 0x01000000;
    D_80120650[3] = 0x84;
    *(u8 **)&D_80120650[0xC] = D_80125CA8;
    *(u8 **)&D_80120650[0x10] = D_80125CAC;
    *(u8 **)&D_80120650[0x34] = D_80125CB0;
    D_80120650[5] = 2;
    D_80120650[6] = 1;
    D_80120650[0x43] = 0x10;
    *(s16 *)&D_80120650[0x16] = arg0;
    *(s16 *)&D_80120650[0x26] = -0x50;
    *(s16 *)&D_80120650[0x2A] = -0x40;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80146D4C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014716C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801471F4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014727C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147304);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014738C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147414);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801474FC);

void func_80147630(void) {
    func_80042808();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147650);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801476DC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147768);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801477F4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147880);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014790C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147D74);

void func_80147F0C(void) {
    if (((D_800E6280.unk_F88 & 0x20) && (u32)D_800E6280.unk_1104.w >= 0x101) || (D_800E6280.unk_F88 & 0x40)) {
        func_8006BD6C(1);
        func_80042808();
    }
    func_80148E98();
    func_80148EE8();
    func_80148BC8();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80147F7C);

void func_80147FFC(void) {
    parameter_show_init();
    parameter_show_init();
    func_80048E78();
    func_8004E58C();
    func_8004284C();
}

void func_8014803C(void) {
    if (D_800E6280.unk_041 == 5) {
        func_8004500C(0, 0);
    }
    func_80072B5C(0);
}

void func_80148078(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80147FFC();
        break;
    case 1:
        func_8014803C();
        break;
    }
    func_800646CC();
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_801480D0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80148BC8);

void func_80148E98(void) {
    func_80049A40(-0xA0, -0x68, 0x140, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void func_80148EE8(void) {
    func_80049A40(-0x41, -0x70, 0x84, 0x12, 4, 0x30E00, 0);
    dtd_on(4);
}

void func_80148F34(void) {
    RECT rect;

    rect.x = 0x100;
    rect.y = 0x1F0;
    rect.w = 0x10;
    rect.h = 2;
    func_8009C884(&rect, &D_80150754);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_80148F7C);
