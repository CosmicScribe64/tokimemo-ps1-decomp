#include "ovl/MASTER.h"

void func_80132000(void) {
    D_8013C2D0 = 0x801E8A1C;
    D_8013C2D4 = 0x801E8B58;
    D_8013C2D8 = 0x801E94B8;
}

void func_80132034(void) {
    D_8013C2D0 = 0x801E89CC;
    D_8013C2D4 = 0x801E8AFC;
    D_8013C2D8 = 0x801E93E4;
}

void func_80132068(void) {
    D_8013C2D0 = 0x801E898C;
    D_8013C2D4 = 0x801E8AB8;
    D_8013C2D8 = 0x801E93A0;
}

void func_8013209C(void) {
    D_8013C2D0 = 0x801E896C;
    D_8013C2D4 = 0x801E8AA0;
    D_8013C2D8 = 0x801E938C;
}

void func_801320D0(void) {
    D_8013C2D0 = 0x801E8ABC;
    D_8013C2D4 = 0x801E8C00;
    D_8013C2D8 = 0x801E9608;
}

void func_80132104(void) {
    D_8013C2D0 = 0x801E8950;
    D_8013C2D4 = 0x801E8A84;
    D_8013C2D8 = 0x801E9370;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80132138);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80132140);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801321B0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80132444);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801326B4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80133374);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013348C);

void func_80133680(void) {
    D_8013C4A0 = 0x801E8660;
    D_8013C4A4 = 0x801E8740;
    D_8013C4A8 = 0x801E8AF8;
}

void func_801336B4(void) {
    D_8013C4A0 = 0x801EAA54;
    D_8013C4A4 = 0x801EAE50;
    D_8013C4A8 = 0x801ED1C4;
}

void func_801336E8(void) {
    D_8013C4A0 = 0x801E8A10;
    D_8013C4A4 = 0x801E8B7C;
    D_8013C4A8 = 0x801E93D4;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80133718);

void func_80133724(void) {
    D_8013C4B0 = 0x801EAFC4;
    D_8013C4B4 = 0x801EB144;
    D_8013C4B8 = 0x801EAFF0;
    D_8013C4BC = 0x801EB14C;
    D_8013C4C0 = 0x801EB030;
    D_8013C4C4 = 0x801EB174;
    D_8013C4C8 = *(s16 *)0x801EB17C;
    D_8013C4CC = *(s16 *)0x801EB180;
    D_8013C4D0 = 0x801B4400;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801337B8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80133A2C);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80133AA0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80133F80);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801344E0);

void func_80135040(void) {
    if (((D_800E644C & 0xF) == D_800E62BF) && (((u32)(D_800E644C << 0x17) >> 0x1B) == D_800E62C0)) {
        func_80138490();
        return;
    }
    func_80042808();
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801350A8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801358C8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801359F0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80135CF4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80135FB8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801363F4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80136E24);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80137044);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801375E8);

void func_80137B5C(void) {
    if (D_800E738A == 0) {
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
    } else if (func_80044E8C() == 1) {
        func_80048F64(0x60);
        D_800E699E |= 0x10;
        func_8008585C();
        set_dec_bri(0);
        func_8006D6E0();
        func_80072B5C(1);
    }
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80137BF4);

void func_80137F94(void) {
    func_80138EC0();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C714;
    D_80120660 = (u8 *)D_8013C718;
    D_80120684 = (u8 *)D_8013C710;
    D_80120664 = D_8013C71C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80138088);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80138374);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80138490);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80138E58);

void func_80138EC0(void) {
    D_8013C710 = 0x8019DCFC;
    D_8013C714 = 0x8019DD00;
    D_8013C718 = 0x8019DDC0;
    D_8013C71C = *(s16 *)0x8019DDF8;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80138F04);

void func_80138F10(void) {
    D_8013C720 = 0x801A403C;
    D_8013C724 = 0x801A4048;
    D_8013C728 = 0x801A4098;
    D_8013C72C = *(s16 *)0x801A40B0;
    D_8013C730 = 0x80197000;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80138F64);

void func_80138F70(void) {
    D_8013C740 = 0x8019DCFC;
    D_8013C744 = 0x8019DD00;
    D_8013C748 = 0x8019DDC0;
    D_8013C74C = *(s16 *)0x8019DDF8;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80138FB4);

void func_80138FC0(void) {
    D_8013C750 = 0x801EAA54;
    D_8013C754 = 0x801EAE50;
    D_8013C758 = 0x801ED1C4;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_MASTER_80138FF4);

void func_80139000(void) {
    D_8013C760 = 0x801A42D4;
    D_8013C764 = 0x801A42E0;
    D_8013C768 = 0x801A4330;
    D_8013C76C = *(s16 *)0x801A4338;
    D_8013C770 = 0x80197000;
}

void func_80139054(void) {
    D_8013C760 = 0x801A3E3C;
    D_8013C764 = 0x801A3E48;
    D_8013C768 = 0x801A3E90;
    D_8013C76C = *(s16 *)0x801A3E98;
    D_8013C770 = 0x80197000;
}

void func_801390A8(void) {
    D_8013C760 = 0x801A42E4;
    D_8013C764 = 0x801A42F0;
    D_8013C768 = 0x801A4368;
    D_8013C76C = *(s16 *)0x801A4370;
    D_8013C770 = 0x80197000;
}

void func_801390FC(void) {
    D_8013C760 = 0x801A432C;
    D_8013C764 = 0x801A4338;
    D_8013C768 = 0x801A4394;
    D_8013C76C = *(s16 *)0x801A43A4;
    D_8013C770 = 0x80197000;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80139150);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_801391E4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80139810);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80139910);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_80139CC8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013A784);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013ACF0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013B20C);

void func_8013B354(void) {
    func_80138F70();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C744;
    D_80120660 = (u8 *)D_8013C748;
    D_80120684 = (u8 *)D_8013C740;
    D_80120664 = D_8013C74C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013B448);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER", func_8013B804);
