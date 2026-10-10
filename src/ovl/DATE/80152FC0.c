#include "common.h"
#include "ovl/DATE.h"

void func_80152FC0(void) {
    D_8015F160 = 0x801CE124;
    D_8015F164 = 0x801CE128;
    D_8015F168 = 0x801CE148;
    D_8015F16C = *(s16 *)0x801CE15C;
    D_8015F170 = 0x801B0000;
    D_8015F174 = 0x801B2000;
    D_8015F178 = 0x801B6000;
    D_8015F17C = 0x801BA000;
    D_8015F180 = 0x801BE000;
    D_8015F184 = 0x801C2000;
    D_8015F188 = 0x801C6000;
}

void func_80153070(void) {
    D_8015F18C = 0x801CE0F4;
    D_8015F190 = 0x801CE0F8;
    D_8015F194 = 0x801CE118;
    D_8015F198 = *(s16 *)0x801CE12C;
    D_8015F19C = 0x801B0000;
    D_8015F1A0 = 0x801B2000;
    D_8015F1A4 = 0x801B6000;
    D_8015F1A8 = 0x801BA000;
    D_8015F1AC = 0x801BE000;
    D_8015F1B0 = 0x801C2000;
    D_8015F1B4 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153120);

void func_801531D0(void) {
    D_8015F1E4 = 0x801CE0F4;
    D_8015F1E8 = 0x801CE0F8;
    D_8015F1EC = 0x801CE118;
    D_8015F1F0 = *(s16 *)0x801CE12C;
    D_8015F1F4 = 0x801B0000;
    D_8015F1F8 = 0x801B2000;
    D_8015F1FC = 0x801B6000;
    D_8015F200 = 0x801BA000;
    D_8015F204 = 0x801BE000;
    D_8015F208 = 0x801C2000;
    D_8015F20C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153280);

void func_80153320(void) {
    D_8015E208 = D_8015DECC;
    D_8015E20C = D_8015E008;
    D_8015E210 = D_8015E144;
    D_800CA21C = 1;
    D_800CA220 = 2;
    D_800CA21E = 4;
    D_800CA224 = 1;
    D_800CA228 = 2;
    D_800CA226 = 3;
    D_800CA22C = 0;
    D_800CA230 = 2;
    D_800CA22E = 4;
    D_800CA234 = 0;
    D_800CA238 = 1;
    D_800CA236 = 2;
    func_80152FC0();
    load_palette(D_8015F170, 0x11, 1, 2, 0);
    func_80084E90(D_8015F174, D_8015F178, D_8015F17C, D_8015F180, D_8015F184, D_8015F188);
    func_800850D4(D_8015F164, D_8015F168, D_8015F160, D_8015F16C);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_8015347C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_801535D8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_801537C0);

void func_80153928(void) {
    D_8015E208 = D_8015DEDC;
    D_8015E20C = D_8015E018;
    D_8015E210 = D_8015E154;
    D_800E65C6 += 3;
    /* FAKE: indexing the first symbol keeps as1 from hoisting this load above the previous store (matches; real source unknown). T-4050 */
    (&D_800E65C6)[2] += 2;
    func_80084D3C();
    func_8004284C();
}

void func_801539A4(void) {
    bg_read_sub2(0x486E);
    func_8004284C();
}

void func_801539CC(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

void func_801539FC(void) {
    bg_read_sub2(0x48D6);
    func_8004284C();
}

void func_80153A24(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80153A4C(void) {
    bg_read_sub2(0x482A);
    func_8004284C();
}

void func_80153A74(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80153A9C(void) {
    bg_read_sub2(0x4347);
    D_800E71DE -= 1;
    func_80085B3C(0, D_80122D08);
    func_8004284C();
}

void func_80153AE8(void) {
    bg_read_sub2(0x4891);
    func_8004284C();
}

void func_80153B10(void) {
    bg_read_sub2(0x4558);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153B38);

void func_80153BC0(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_80153BE8(void) {
    func_80046318(0x3D, 0x801B0000, 0x847D);
    func_80152FC0();
    func_8004284C();
}

void func_80153C20(void) {
    D_800CA150 = (u16) D_800CA150 + D_80122CDC;
    if (D_80122CDC == 0) {
        load_palette(D_8015F170, 0x11, 1, 2, 0);
        D_8015E204 = 0;
    } else if (D_80122CDC == 1) {
        load_palette(D_8015F170, 0x11, 1, 2, 2);
        D_8015E204 = 2;
    } else {
        load_palette(D_8015F170, 0x11, 1, 2, 1);
        D_8015E204 = 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153CEC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153D94);

void func_80153DE4(void) {
    func_80046318(0x3D, 0x801B0000, 0x84BA);
    func_80153070();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153E1C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80153E90);

void func_80153F10(void) {
    func_8014C5C8();
    if (D_800E7384 == 1) {
        if ((u16) D_800CA154 == 7) {
            func_80083440(1);
        }
    }
}

void func_80153F5C(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80153F84(void) {
    if (D_800E738D == 0) {
        func_80044750(0x205);
        D_800E738D += 1;
    }
    func_8014C5C8();
}

void func_80153FCC(void) {
    func_80046318(0x3D, 0x801B0000, 0x84F7);
    func_80153120();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80154004);

void func_801540B8(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_801540E0(void) {
    func_80046318(0x3D, 0x801B0000, 0x8534);
    func_801531D0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80152FC0", func_80154118);
