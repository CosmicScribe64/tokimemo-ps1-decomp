#include "common.h"
#include "game.h"

void func_80079B10(u16 arg0) {
    func_8007B5EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079B34);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079C70);

void func_80079D28(s16 arg0) {
    s32 off;
    s32 pad; /* FAKE: unused local; the original frame has 8 more bytes of locals (spill of off at sp+0x2C); real source unknown. T-4090 */

    off = arg0 * 4;
    if (func_8008C1B0(*(s32 *)((u8 *) D_80125D60 + off), arg0, *(s32 *)((u8 *) D_80125D80 + off)) == -1) {
        func_80059B04(((arg0 << 8) & 0xFF00) | 0x01010000, arg0);
        return;
    }
    D_80125D10.unk_00 |= 0x10000 << arg0;
    if (func_8008C620(*(s32 *)((u8 *) D_80125D70 + off), arg0) == -1) {
        func_80059B04(((arg0 << 8) & 0xFF00) | 0x01020000, arg0);
        return;
    }
    SsVabTransCompleted(1, arg0);
}

s32 func_80079E00(s32 arg0) {
    if ((D_80125D10.unk_04 & (0x10000000 << (u16)D_80125D10.unk_3C)) == 0) {
        if ((D_80125D10.unk_04 & (0x1000000 << (u16)D_80125D10.unk_3C)) == 0) {
            return 1;
        }
    }
    return 0;
}

void func_80079E4C(void) {
    if (D_80125D10.unk_00 & 0x400) {
        func_80079E9C();
    }
    if (D_80125D10.unk_04 & 0x200) {
        func_80079F00();
    }
}

void func_80079E9C(void) {
    if (func_80046274() >= (u32)D_80125D10.unk_0C) {
        if (!(D_80125D10.unk_04 & 0x100)) {
            func_8007AB24(0xB4);
        }
    } else {
        SD_GetCDLevel(&D_80125E60);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079F00);

void func_8007A008(void) {
    s32 v;

    v = D_80125D10.unk_04;
    if (v != 0) {
        if ((u32) v >> 24) {
            func_80079C70();
            v = D_80125D10.unk_04;
        }
        if (v & 0xFF) {
            func_8007ABE0();
            v = D_80125D10.unk_04;
        }
        if (v & 0xFF00) {
            func_8007A4DC();
        }
    }
}

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
    if (D_80125D10.unk_04 & 0x100) {
        func_8007A50C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A50C);

void func_8007A5BC(void) {
    func_8007A98C(0x74);
    if ((D_80125D10.unk_00 & 0x4000) == 0) {
        func_8007A868();
        if (D_80125D10.unk_08 != 0) {
            func_8007A924(D_80125D10.unk_08);
        }
    }
}

void func_8007A618(u32 arg0) {
    D_80125D10.unk_1E = 0x64;
    switch (arg0) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        D_80125D10.unk_1E = 0x7D;
        break;
    case 7:
        D_80125D10.unk_1E = 0x5F;
        break;
    case 9:
    case 11:
        D_80125D10.unk_1E = 0x5F;
        break;
    case 10:
        D_80125D10.unk_1E = 0x5A;
        break;
    case 12:
    case 13:
        D_80125D10.unk_1E = 0x6E;
        break;
    case 15:
        D_80125D10.unk_1E = 0x69;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A6AC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A868);

void func_8007A924(s32 arg0) {
    func_8007BA54();
    D_80125D10.unk_48[0] = D_80125D10.unk_48[6];
    D_80125D10.unk_48[1] = D_80125D10.unk_48[4];
    D_80125D10.unk_04 |= 0x200;
    func_80045414(0xD, arg0, D_80125D10.unk_48);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A98C);

void func_8007AB24(u16 arg0) {
    if (!(D_80125D10.unk_04 & 0x100)) {
        D_80125D10.unk_34 = 0;
        D_80125D10.unk_04 |= 0x100;
    }
    switch (arg0 & 0xFF) {
    default:
        D_80125D10.unk_38 = 0x100;
        D_80125D10.unk_36 = 0xA;
        return;
    case 0xB4:
        D_80125D10.unk_38 = 0x100;
        D_80125D10.unk_36 = 0xA;
        return;
    case 0xC4:
        D_80125D10.unk_38 = 0x100;
        D_80125D10.unk_36 = 1;
        return;
    case 0xD4:
        D_80125D10.unk_38 = 0x80;
        D_80125D10.unk_36 = 1;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007ABE0);

void func_8007AD6C(void) {
    if ((D_80125D10.unk_00 & 0x100000) && !(D_80125D10.unk_00 & 0x400)) {
        func_8008FD68(0);
        func_8008B750(0, 0);
    }
    func_8007B144(0x71);
    if (D_80125D10.unk_2A != -1) {
        func_8007AEC0();
    }
}

void func_8007ADD8(s32 arg0) {
    s32 a;

    a = arg0 & 0xFFFF;
    switch (a & 0xF000) {
    case 0x0:
        if (D_80125D10.unk_04 & 1) {
            D_80125D10.unk_2C = a & 0xFF;
            func_8007AD6C();
            return;
        }
        if (D_80125D10.unk_00 & 0x100) {
            D_80125D10.unk_2C = a & 0xFF;
            func_8007B2B4(0xB1, a);
            return;
        }
        D_80125D10.unk_2A = a & 0xFF;
        D_80125D10.unk_2C = -1;
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
    if (func_80079E00(D_80125D10.unk_3C) == 0) {
        D_80125D10.unk_04 |= 2;
    } else {
        func_8007AF0C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AF0C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B144);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B2B4);

void func_8007B358(u16 arg0) {
    s32 a;

    /* FAKE: shift split in two keeps the lhu result out of the lui register; permuter result, real source unknown. T-8010 */
    a = (((D_80125D50 << 2) << 6) & 0xFF00) | (((arg0 & 0xFF) >> 4) & 0xFF);
    if (arg0 & 0x1000) {
        func_8007B5CC(a, ((arg0 & 0xF) * 4 + 0x18) << 8 & 0xFF00);
        return;
    }
    func_8007B568(a, ((arg0 & 0xF) * 4 + 0x18) << 8 & 0xFF00);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B3DC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B460);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B4E4);

void func_8007B568(s32 arg0, s32 arg1) {
    func_80090D20();
    D_80125D10.unk_00 &= 0xFFEFFFFF;
    func_8008B750(0x7F, 0x7F);
    func_80090D60(arg0, arg1, D_80125D10.unk_24, D_80125D10.unk_24);
}

void func_8007B5CC(void) {
    func_80090D20();
}

s32 func_8007B5EC(u16 a) {
    if (D_80125CC0 >= 0x20) {
        return -1;
    }
    D_80125CC0++;
    (&D_80125CC0)[D_80125CC0] = a;
    return 0;
}

void func_8007B640(void) {
    D_80125CC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B64C);

void func_8007B6E0(void) {
    func_8007A98C(0x74);
    D_80125D10.unk_08 = 0;
    D_80125D10.unk_0C = 0;
    D_80125D10.unk_10 = 0;
    D_80125D10.unk_14 = 0;
    D_80125D10.unk_18 = 0;
    D_80125D10.unk_48[4] = 0;
    D_80125D10.unk_48[5] = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B734);

void func_8007B7E0(void) {
    if (func_80079524() == 0 || !(D_80125D10.unk_00 & 0x100)) {
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

void func_8007B99C(u16 arg0) {
    s32 v;

    v = func_800451D0();
    switch (arg0) {
    case 17:
        D_80125D70[0] = v;
        D_80125D70[1] = v + 0x10000;
        break;
    case 18:
        D_80125D70[2] = v;
        D_80125D70[3] = v + 0x28000;
        break;
    case 19:
        D_80125D70[0] = v;
        break;
    case 20:
        D_80125D70[1] = v;
        break;
    case 21:
        D_80125D70[2] = v;
        break;
    case 22:
        D_80125D70[3] = v;
        break;
    }
}

void func_8007BA54(void) {
    D_80125D10.unk_44[0] = 0xC8;
    func_8008BEE0(0, 0, 0);
    func_8008BFB0(0, 0, 0);
    func_80045414(0xE, 0, D_80125D10.unk_44);
}

void func_8007BAAC(void) {
    D_80125D10.unk_04 = 0;
    D_80125D10.unk_00 = 0x10;
    D_80125D10.unk_08 = 0;
    D_80125D10.unk_0C = 0;
    D_80125D10.unk_10 = 0;
    D_80125D10.unk_14 = 0;
    D_80125D10.unk_18 = 0;
    D_80125D10.unk_1C = 0x64;
    D_80125D10.unk_1E = 0x64;
    D_80125D10.unk_20 = 0x50;
    D_80125D10.unk_22 = 0x50;
    D_80125D10.unk_24 = 0x5A;
    D_80125D10.unk_26 = 0x5A;
    D_80125D10.unk_28 = 0;
    D_80125D10.unk_2A = -1;
    D_80125D10.unk_2C = -1;
    D_80125D10.unk_2E = 0;
    D_80125D10.unk_30 = 0x80;
    D_80125D10.unk_32 = 0xFF;
    D_80125D10.unk_34 = 0;
    D_80125D10.unk_36 = 0x80;
    D_80125D10.unk_38 = 0xFF;
    D_80125D10.unk_40 = 0;
    D_80125D10.unk_3C = 1;
    D_80125D10.unk_3E = 2;
    D_80125D10.unk_42 = 3;
    D_80125D10.unk_3A = 0;
    D_80125D10.unk_48[4] = 0;
    D_80125D10.unk_48[5] = 0;
    D_80125D10.unk_48[6] = 0;
    D_80125D10.unk_48[7] = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BBE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BCFC);

void func_8007BDE8(void) {
    s16 var_s0;

    if (D_80125D10.unk_00 & 0x1100) {
        func_80090580(D_80125D38);
        func_800907E0(D_80125D38);
    }
    func_8008FD68(0);
    var_s0 = 3;
    do {
        if ((0x10000 << var_s0) & D_80125D10.unk_00) {
            func_80090E60(var_s0);
        }
        var_s0 -= 1;
    } while (var_s0 >= 0);
    func_8008BE54();
    func_8008BEBC();
}

void func_8007BE94(s16 arg0) {
    /* FAKE: `^ 0` turns the left operand into an expression, which makes uopt swap the operands of the compare (decomp-permuter, score 0); real source unknown. T-4090 */
    if ((arg0 ^ 0) == (u16) D_80125D10.unk_3C && (D_80125D10.unk_00 & 0x100)) {
        func_8007AD6C();
    }
    D_80125D10.unk_04 |= 0x01000000 << arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BF04);

void func_8007BFA8(void) {
    D_80125D10.unk_2C = -1;
}

void func_8007BFB8(void) {
    if (D_80125D10.unk_00 & 0x10000000) {
        D_80125D10.unk_00 &= 0xEFFFFFFF;
        return;
    }
    D_80125D10.unk_00 |= 0x10000000;
}

void SD_InitCDLevelInfo(void) {
    D_80125E60 = 0;
    D_80125E62 = 0;
    D_80125E64 = 8;
    D_80125E66 = 0x20;
    D_80125E68 = 0x212;
}
