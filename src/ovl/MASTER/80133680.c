#include "ovl/MASTER.h"

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

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801337B8);

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_8013C5C8;

void func_80133A2C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_8013C5C8;
    idx = D_800E7389;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80133AA0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80133F80);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801344E0);

void func_80135040(void) {
    if (((D_800E644C & 0xF) == D_800E62BF) && (((u32)(D_800E644C << 0x17) >> 0x1B) == D_800E62C0)) {
        func_80138490();
        return;
    }
    func_80042808();
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801350A8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801358C8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801359F0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135CF4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135FB8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801363F4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80136E24);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80137044);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801375E8);

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

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80137BF4);

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

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80138088);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80138374);
