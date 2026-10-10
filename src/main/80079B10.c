#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80079B10(u16 arg0) {
    func_8007B5EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079B34);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079C70);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079D28);

s32 func_80079E00(s32 arg0) {
    if ((D_80125D14 & (0x10000000 << (u16)D_80125D4C)) == 0) {
        if ((D_80125D14 & (0x1000000 << (u16)D_80125D4C)) == 0) {
            return 1;
        }
    }
    return 0;
}

void func_80079E4C(void) {
    if (D_80125D10 & 0x400) {
        func_80079E9C();
    }
    if (D_80125D14 & 0x200) {
        func_80079F00();
    }
}

void func_80079E9C(void) {
    if (func_80046274() >= (u32)D_80125D1C) {
        if (!(D_80125D14 & 0x100)) {
            func_8007AB24(0xB4);
        }
    } else {
        SD_GetCDLevel(&D_80125E60);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079F00);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A008);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A080);

void func_8007A254(u16 arg0) {
    arg0 = arg0 & 0xFF;
    func_8007B99C(arg0);
    switch (arg0) {
    case 17:
        func_8007BF04(0);
        func_8007BE94(0);
        func_8007BF04(1);
        func_8007BE94(1);
        return;
    case 18:
        func_8007BF04(2);
        func_8007BE94(2);
        func_8007BF04(3);
        func_8007BE94(3);
        return;
    case 19:
        func_8007BF04(0);
        func_8007BE94(0);
        return;
    case 20:
        func_8007BF04(1);
        func_8007BE94(1);
        return;
    case 21:
        func_8007BF04(2);
        func_8007BE94(2);
        return;
    case 22:
        func_8007BF04(3);
        func_8007BE94(3);
        return;
    default:
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A354);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A43C);

void func_8007A4DC(void) {
    if (D_80125D14 & 0x100) {
        func_8007A50C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A50C);

void func_8007A5BC(void) {
    func_8007A98C(0x74);
    if ((D_80125D10 & 0x4000) == 0) {
        func_8007A868();
        if (D_80125D18 != 0) {
            func_8007A924(D_80125D18);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A618);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A6AC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A868);

/* FAKE: indexed views of D_80125D58 stop IDO from hoisting the later loads above the stores (see cal_sprite_disp_switch). Real source unknown. T-2090 */
void func_8007A924(s32 arg0) {
    func_8007BA54();
    D_80125D58[0] = D_80125D5E;
    D_80125D58[1] = D_80125D58[4];
    *(s32 *)(D_80125D58 - 0x44) |= 0x200;
    func_80045414(0xD, arg0, D_80125D58);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A98C);

void func_8007AB24(u16 arg0) {
    if (!(D_80125D14 & 0x100)) {
        D_80125D44 = 0;
        D_80125D14 |= 0x100;
    }
    switch (arg0 & 0xFF) {
    default:
        D_80125D48 = 0x100;
        D_80125D46 = 0xA;
        return;
    case 0xB4:
        D_80125D48 = 0x100;
        D_80125D46 = 0xA;
        return;
    case 0xC4:
        D_80125D48 = 0x100;
        D_80125D46 = 1;
        return;
    case 0xD4:
        D_80125D48 = 0x80;
        D_80125D46 = 1;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007ABE0);

void func_8007AD6C(void) {
    if ((D_80125D10 & 0x100000) && !(D_80125D10 & 0x400)) {
        func_8008FD68(0);
        func_8008B750(0, 0);
    }
    func_8007B144(0x71);
    if (D_80125D3A != -1) {
        func_8007AEC0();
    }
}

void func_8007ADD8(s32 arg0) {
    s32 a;

    a = arg0 & 0xFFFF;
    switch (a & 0xF000) {
    case 0x0:
        if (D_80125D14 & 1) {
            D_80125D3C = a & 0xFF;
            func_8007AD6C();
            return;
        }
        if (D_80125D10 & 0x100) {
            D_80125D3C = a & 0xFF;
            func_8007B2B4(0xB1, a);
            return;
        }
        D_80125D3A = a & 0xFF;
        D_80125D3C = -1;
        func_8007AEC0();
        return;
    case 0x1000:
        func_8007B144(0x71, a);
        return;
    case 0x2000:
        func_8007B144(0xE1, a);
        return;
    case 0x4000:
        func_8007B144(0xF1, a);
        return;
    }
}

void func_8007AEC0(void) {
    if (func_80079E00(D_80125D4C) == 0) {
        D_80125D14 |= 2;
    } else {
        func_8007AF0C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AF0C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B144);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B2B4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B358);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B3DC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B460);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B4E4);

void func_8007B568(s32 arg0, s32 arg1) {
    func_80090D20();
    D_80125D10 &= 0xFFEFFFFF;
    func_8008B750(0x7F, 0x7F);
    func_80090D60(arg0, arg1, D_80125D34, D_80125D34);
}

void func_8007B5CC(void) {
    func_80090D20();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B5EC);

void func_8007B640(void) {
    D_80125CC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B64C);

void func_8007B6E0(void) {
    func_8007A98C(0x74);
    D_80125D18 = 0;
    D_80125D1C = 0;
    D_80125D20 = 0;
    D_80125D24 = 0;
    D_80125D28 = 0;
    D_80125D5C = 0;
    D_80125D5D = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B734);

void func_8007B7E0(void) {
    if (func_80079524() == 0 || !(D_80125D10 & 0x100)) {
        func_80090DB0();
        func_8008FD68(0);
        func_8008FED0();
        func_800949B0(3);
        func_8008FEB0();
    }
}

void func_8007B844(void) {
    func_8008B8C4();
    func_8008BAA4(1);
    func_80090DF0(0x18);
    func_8008B904(D_80125E70, 1, 1);
    func_80090E30(1);
    func_8008FEF0(3);
    func_800949B0(3);
    func_8008FEB0();
    func_8008FF5C(0x3C, 0x3C);
    func_8008B750(0x7F, 0x7F);
    func_8007BCFC(2);
    func_8007BAAC();
    func_8007B8F8();
    func_8007BA54();
    SD_InitCDLevelInfo();
    func_8007B640();
    func_8008BE34();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B8F8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B99C);

void func_8007BA54(void) {
    D_80125D54 = 0xC8;
    func_8008BEE0(0, 0, 0);
    func_8008BFB0(0, 0, 0);
    func_80045414(0xE, 0, &D_80125D54);
}

void func_8007BAAC(void) {
    D_80125D14 = 0;
    D_80125D10 = 0x10;
    D_80125D18 = 0;
    D_80125D1C = 0;
    D_80125D20 = 0;
    D_80125D24 = 0;
    D_80125D28 = 0;
    D_80125D2C = 0x64;
    D_80125D2E = 0x64;
    D_80125D30 = 0x50;
    D_80125D32 = 0x50;
    D_80125D34 = 0x5A;
    D_80125D36 = 0x5A;
    D_80125D38 = 0;
    D_80125D3A = -1;
    D_80125D3C = -1;
    D_80125D3E = 0;
    D_80125D40 = 0x80;
    D_80125D42 = 0xFF;
    D_80125D44 = 0;
    D_80125D46 = 0x80;
    D_80125D48 = 0xFF;
    D_80125D50 = 0;
    D_80125D4C = 1;
    D_80125D4E = 2;
    D_80125D52 = 3;
    D_80125D4A = 0;
    D_80125D5C = 0;
    D_80125D5D = 0;
    D_80125D5E = 0;
    D_80125D5F = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BBE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BCFC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BDE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BE94);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BF04);

void func_8007BFA8(void) {
    D_80125D3C = -1;
}

void func_8007BFB8(void) {
    if (D_80125D10 & 0x10000000) {
        D_80125D10 &= 0xEFFFFFFF;
        return;
    }
    D_80125D10 |= 0x10000000;
}

void SD_InitCDLevelInfo(void) {
    D_80125E60 = 0;
    D_80125E62 = 0;
    D_80125E64 = 8;
    D_80125E66 = 0x20;
    D_80125E68 = 0x212;
}
