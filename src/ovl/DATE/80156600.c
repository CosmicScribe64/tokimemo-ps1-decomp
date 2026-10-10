#include "common.h"
#include "ovl/DATE.h"

void func_80156600(void) {
    D_8015FCE0 = 0x801CE0DC;
    D_8015FCE4 = 0x801CE0E0;
    D_8015FCE8 = 0x801CE0F8;
    D_8015FCEC = *(s16 *)0x801CE10C;
    D_8015FCF0 = 0x801B0000;
    D_8015FCF4 = 0x801B2000;
    D_8015FCF8 = 0x801B6000;
    D_8015FCFC = 0x801BA000;
    D_8015FD00 = 0x801BE000;
    D_8015FD04 = 0x801C2000;
    D_8015FD08 = 0x801C6000;
}

void func_801566B0(void) {
    D_8015FD0C = 0x801D2100;
    D_8015FD10 = 0x801D2108;
    D_8015FD14 = 0x801D2128;
    D_8015FD18 = *(s16 *)0x801D2144;
    D_8015FD1C = 0x801B0000;
    D_8015FD20 = 0x801B2000;
    D_8015FD24 = 0x801B6000;
    D_8015FD28 = 0x801BA000;
    D_8015FD2C = 0x801BE000;
    D_8015FD30 = 0x801C2000;
    D_8015FD34 = 0x801C6000;
}

void func_80156760(void) {
    D_8015FD38 = 0x801D2240;
    D_8015FD3C = 0x801D2248;
    D_8015FD40 = 0x801D2288;
    D_8015FD44 = *(s16 *)0x801D22A0;
    D_8015FD48 = 0x801B0000;
    D_8015FD4C = 0x801B2000;
    D_8015FD50 = 0x801B6000;
    D_8015FD54 = 0x801BA000;
    D_8015FD58 = 0x801BE000;
    D_8015FD5C = 0x801C2000;
    D_8015FD60 = 0x801C6000;
}

void func_80156810(void) {
    D_8015FD64 = 0x801CE080;
    D_8015FD68 = 0x801CE084;
    D_8015FD6C = 0x801CE094;
    D_8015FD70 = *(s16 *)0x801CE0A8;
    D_8015FD74 = 0x801B0000;
    D_8015FD78 = 0x801B2000;
    D_8015FD7C = 0x801B6000;
    D_8015FD80 = 0x801BA000;
    D_8015FD84 = 0x801BE000;
    D_8015FD88 = 0x801C2000;
    D_8015FD8C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801568C0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80156954);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80156B20);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80156C40);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80156D60);

void func_80156EC0(void) {
    bg_read_sub2(0x4860);
    func_8004284C();
}

void func_80156EE8(void) {
    bg_read_sub2(0x44F1);
    func_8004284C();
}

void func_80156F10(void) {
    func_800AE0F0(D_800CA1DC, "無人島");
    func_8007ED84(0x45B6);
    func_8004284C();
}

void func_80156F4C(void) {
    bg_read_sub2(0x48CD);
    func_8004284C();
}

void func_80156F74(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80156F9C(void) {
    bg_read_sub2(0x48F7);
    func_8004284C();
}

void func_80156FC4(void) {
    bg_read_sub2(0x45D3);
    func_8004284C();
}

void func_80156FEC(void) {
    bg_read_sub2(0x492F);
    func_8004284C();
}

void func_80157014(void) {
    bg_read_sub2(0x4631);
    func_8004284C();
}

typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
extern FnTbl50 D_8015FD90;

void func_8015703C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_8015FD90;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_801570C4(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801570EC(void) {
    func_80046318(0x3D, 0x801B0000, 0x8984);
    func_80156600();
    func_8004284C();
}

void func_80157124(void) {
    if (get_g_zyotai_s(D_800E71DF) < 2U) {
        func_80083440(0);
    } else {
        func_80083440(get_g_zyotai_h(D_800E71DF));
    }
    func_8004284C();
}

void func_80157184(void) {
    u8 s;

    s = get_g_zyotai_s(D_800E71DF) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 5;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801571D4);

void func_80157284(void) {
    func_80046318(0x45, 0x801B0000, 0x89C1);
    func_801566B0();
    func_8004284C();
}

void func_801572BC(void) {
    func_80044750(0x500);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801572E4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801573F0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_8015745C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_8015750C);

void func_80157594(void) {
    func_80046318(0x45, 0x801B0000, 0x8A06);
    func_80156760();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801575CC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_8015761C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80157698);

void func_801577C4(void) {
    func_80046318(0x3D, 0x801B0000, 0x8A4B);
    func_80156810();
    func_8004284C();
}

void func_801577FC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_80157824(void) {
    u8 s;

    s = get_g_zyotai_s(D_800E71DF) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 3;
    }
    func_8004284C();
}
