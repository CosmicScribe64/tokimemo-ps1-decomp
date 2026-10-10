#include "common.h"
#include "ovl/DATE.h"

void func_8014F0F0(void) {
    D_8015E690 = 0x801D21D4;
    D_8015E694 = 0x801D21DC;
    D_8015E698 = 0x801D21FC;
    D_8015E69C = *(s16 *)0x801D2210;
    D_8015E6A0 = 0x801B0000;
    D_8015E6A4 = 0x801B2000;
    D_8015E6A8 = 0x801B6000;
    D_8015E6AC = 0x801BA000;
    D_8015E6B0 = 0x801BE000;
    D_8015E6B4 = 0x801C2000;
    D_8015E6B8 = 0x801C6000;
}

void func_8014F1A0(void) {
    D_8015E6BC = 0x801D21E0;
    D_8015E6C0 = 0x801D21E8;
    D_8015E6C4 = 0x801D2228;
    D_8015E6C8 = *(s16 *)0x801D2240;
    D_8015E6CC = 0x801B0000;
    D_8015E6D0 = 0x801B2000;
    D_8015E6D4 = 0x801B6000;
    D_8015E6D8 = 0x801BA000;
    D_8015E6DC = 0x801BE000;
    D_8015E6E0 = 0x801C2000;
    D_8015E6E4 = 0x801C6000;
}

void func_8014F250(void) {
    D_8015E6E8 = 0x801D2320;
    D_8015E6EC = 0x801D2328;
    D_8015E6F0 = 0x801D2368;
    D_8015E6F4 = *(s16 *)0x801D2380;
    D_8015E6F8 = 0x801B0000;
    D_8015E6FC = 0x801B2000;
    D_8015E700 = 0x801B6000;
    D_8015E704 = 0x801BA000;
    D_8015E708 = 0x801BE000;
    D_8015E70C = 0x801C2000;
    D_8015E710 = 0x801C6000;
}

void func_8014F300(void) {
    D_8015E714 = 0x801B0000;
    D_8015E718 = 0x801B2000;
    D_8015E71C = 0x801B6000;
    D_8015E720 = 0x801BA000;
    D_8015E724 = 0x801BE000;
    D_8015E728 = 0x801C2000;
    D_8015E72C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F370);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F404);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F500);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F610);

void func_8014F744(void) {
    func_8014F250();
    load_palette(D_8015E6F8, 0x11, 1, 2, 0);
    func_80084E90(D_8015E6FC, D_8015E700, D_8015E704, D_8015E708, D_8015E70C, D_8015E710);
    func_800850D4(D_8015E6EC, D_8015E6F0, D_8015E6E8, D_8015E6F4);
    D_800CA360 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014F7F4);

void func_8014F8F0(void) {
    bg_read_sub2(0x47BA);
    func_8004284C();
}

void func_8014F918(void) {
    bg_read_sub2(0x47CD);
    func_8004284C();
}

void func_8014F940(void) {
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_8014F968(void) {
    bg_read_sub2(0x4350);
    func_8004284C();
}

void func_8014F990(void) {
    bg_read_sub2(0x4800);
    func_8004284C();
}

void func_8014F9B8(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_8014F9E0(void) {
    bg_read_sub2(0x48EA);
    func_8004284C();
}

void func_8014FA08(void) {
    bg_read_sub2(0x45BF);
    func_8004284C();
}

void func_8014FA30(void) {
    D_80122CDC = 0;
    D_800CA21C = 1;
    D_800CA21E = 1;
    D_800CA220 = 1;
    D_800CA234 = 0;
    D_800CA236 = 0;
    D_800CA238 = 0;
    D_800CA224 = 1;
    D_800CA226 = 1;
    D_800CA228 = 1;
    D_800CA22C = 0;
    D_800CA22E = 0;
    D_800CA230 = 0;
    func_8004284C();
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8014B1F0();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FAE0);

void func_8014FB68(void) {
    func_80046318(0x45, 0x801B0000, 0x7F61);
    func_8014F0F0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FBA0);

void func_8014FC48(void) {
    func_80046318(0x45, 0x801B0000, 0x7FA6);
    func_8014F1A0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FC80);

void func_8014FD08(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FD30);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FDA0);

void func_8014FE24(void) {
    func_80046318(0x45, 0x801B0000, 0x7FEB);
    func_8014F250();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FE5C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014F0F0", func_8014FE90);

void func_8014FF2C(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8014FF54(void) {
    if ((u16) D_800CA154 == 5) {
        func_80083440(1);
    } else if ((u16) D_800CA154 == 9) {
        func_80083440(2);
    } else if ((u16) D_800CA154 == 0xB) {
        func_80083440(get_g_zyotai_h(D_800E71DF));
    }
    func_8014C5C8();
}

void func_8014FFD0(void) {
    func_80046318(0x35, 0x801B0000, 0x8030);
    func_8014F300();
    func_8004284C();
}
