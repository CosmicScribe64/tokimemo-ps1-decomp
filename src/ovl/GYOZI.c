#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134000);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801342A4);

void func_801343A4(void) {
    D_800D9234 = (u8 *) &D_80145EB4;
    D_800D9238 = (u8 *) &D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(D_8012E66C, 1, 0);
}

void func_80134420(void) {
    D_800D9234 = (u8 *)&D_80145EB4;
    D_800D9238 = (u8 *)&D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(0xFF, 1, 0);
}

void func_80134498(void) {
    D_800D9234 = (u8 *) &D_80145EB4;
    D_800D9238 = (u8 *) &D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(D_8012E66C, 1, 2);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134520);

void func_8013459C(void) {
    func_800504CC(1, 0xB678, 0xB65A, 0xC2FD, 0xC2D9, 0xC2D4);
    func_8004DE1C();
}

void func_801345E0(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x200);
        func_8004DE1C();
    }
}

void func_8013461C(void) {
    func_80051DD8(0x33, 0x80197000, 0xA400);
    func_80134000();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134658);

void func_801347AC(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801347D4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134938);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134C10);

void func_80134C98(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

void func_80134CC0(void) {
    func_800BCE10(&D_800D92E0, &D_801458A0);
    func_8004DE1C();
}

void func_80134CF4(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

void func_80134D1C(void) {
    if (D_8012E66C != 0) {
        if ((D_800F62CF == 2) || (D_800F62CF == 7) || (D_800F62CF == 8) || (D_800F62CF == 9) || (D_800F62CF == 0xA)) {
            func_8008E310();
            return;
        }
        func_8008E2E0();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134D9C);

void func_80134EA0(void) {
    if (D_8012E66C == 0) {
        func_8004DEAC(3);
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134EDC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80134FD0);

void func_80135044(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013506C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135118);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135280);

void func_80135420(void) {
    func_800BCE10(&D_800D92E0, &D_801458E8);
    D_80145F64 = 0;
    D_80145F60 = 0;
    D_80145EC8 = 1;
    D_80145EB4 = 6;
    func_8004DE1C();
}

void func_80135478(void) {
    if (D_80145EC4 == 0) {
        func_8008A0D4(0x4107);
    } else {
        D_800F647A += 3;
    }
    func_8004DE1C();
}

void func_801354C8(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

void func_801354F0(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135518);

void func_80135628(void) {
    D_80145EB4 = 7;
    D_80145EA8 = D_80145E08;
    D_80145EAC = D_80145E40;
    D_80145EB0 = D_80145E78;
    func_8004DE1C();
    D_800F647A -= 5;
}

void func_80135690(void) {
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
}

void func_801356FC(void) {
    if (D_8012E66C == 0) {
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_80135730(void) {
    D_80145F60 = 1;
    D_80145EB4 = 0;
    D_80145EA8 = D_80145E00;
    D_80145EAC = D_80145E38;
    D_80145EB0 = D_80145E70;
    func_8004DDD8();
}

void func_80135790(void) {
    s32 temp_t6;

    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    switch (temp_t6) {
    case 0:
        break;
    case 1:
    case 2:
        D_80145EB4 += 1;
        break;
    case 3:
        D_80145EB4 += 2;
        break;
    }
    func_8004DE1C();
}

void func_80135814(void) {
    D_80145EB4 = 0x27;
    func_8004DE1C();
}

void func_8013583C(void) {
    if (D_8012E66C != 0) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_80135890(void) {
    if (D_800F62CF != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void func_801358CC(void) {
    D_80145EB4 = 0xF;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135900);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135988);

void func_80135A20(void) {
    if (D_80145F60 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135A5C(void) {
    if (D_8012E66C != 0) {
        func_8008E2E0();
        return;
    }
    func_8008E280();
}

void func_80135A98(void) {
    func_80090960(5, 0);
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80135ACC(void) {
    if ((D_800F62CF != 5) || (D_80145F60 != 0) || (D_80145EC8 == 0)) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void func_80135B30(void) {
    if (D_80145F60 != 0) {
        D_800F647A += 4;
    }
    func_8004DE1C();
}

void func_80135B70(void) {
    if (D_80145EC8 == 0) {
        func_8004DE1C();
        return;
    }
    func_801343A4();
}

void func_80135BAC(void) {
    if ((D_800F62CF == 5) && (D_80145EC8 == 0)) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

void func_80135C00(void) {
    if (D_80145EC8 != 0) {
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135C3C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135CA0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135D64);

void func_80135DE8(void) {
    func_801343A4();
    if (D_80145EB8 == 3) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x500);
        }
    }
}

void func_80135E3C(void) {
    D_800F5ACD = D_800F62CF;
    func_8004DE1C();
}

void func_80135E68(void) {
    func_80090960(5, 1);
    func_8008A0D4(0x4195);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80135E9C);

void func_80135F74(void) {
    switch (D_801460C0) {
    case 0:
        D_80145EB4 += 3;
        break;
    case 1:
        D_80145EB4 += 2;
        break;
    case 2:
        break;
    }
    func_8004DE1C();
}

void func_80135FE8(void) {
    s32 temp_t6;

    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    switch (temp_t6) {
    case 0:
        break;
    case 1:
    case 2:
        D_80145EB4 += 1;
        break;
    default:
        D_80145EB4 += 2;
        break;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013605C);

void func_801360DC(void) {
    if (D_8012E66C != 2) {
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80136124);

void func_801361A8(void) {
    D_8012E67C = D_8012E66C;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801361D4);

void func_80136220(void) {
    D_80146150 = (s32 *)0x801DABCC;
    D_80146154 = (s32 *)0x801DB66C;
    D_80146158 = (s32 *)0x801DABD4;
    D_8014615C = (s32 *)0x801DB688;
    D_80146160 = (s32 *)0x801DAC34;
    D_80146164 = (s32 *)0x801DB6B4;
    D_80146170 = 0x801C0000;
    D_80146174 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801362A0);

void func_801363BC(void) {
    func_80136220();
    if ((*D_80146150 & 0x80000000) && (*D_80146158 & 0x80000000) && (*D_80146160 & 0x80000000)) {
        func_8004DE1C();
        return;
    }
    D_800F647A -= 2;
}

void func_80136454(void) {
    func_80090960(5, 2);
    func_8008A0D4(0x419F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80136488);

void func_801366E4(void) {
    func_80086AB0(0x201);
    func_8004DE1C();
}

void func_8013670C(void) {
    func_80086AB0(0x200);
    func_8004DE1C();
}

void func_80136734(void) {
    if (D_800F62CF == 2) {
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_8013676C(void) {
    if ((D_80146178 == 3) || (D_80146178 == 4)) {
        D_80145EB4 += 2;
    } else if (D_80146178 != 0) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

void func_801367D4(void) {
    if (D_80146178 == 0) {
        D_80145EB4 += 2;
    } else if ((D_80146178 != 3) && (D_80146178 != 4)) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013683C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801368B8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137188);

void func_80137238(void) {
    func_800549E8(0x60);
    func_800549E8(0x61);
    func_800549E8(0x62);
    func_800549E8(0x63);
    D_801461A0 = 0;
    func_8004DE1C();
}

void func_8013727C(void) {
    func_80051DD8(0x37, 0x801C0000, 0xB081);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801372B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137524);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137798);

void func_80137854(void) {
    u32 temp_t8;

    temp_t8 = (u8)func_8005E0E0(5) & 0x7F;
    if ((D_800F62CF == 5) && (temp_t8 < 2U)) {
        if (D_800F6474++ == 0) {
            D_80145EB4 = 0x2A;
        }
        func_801343A4();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801378D0);

void func_80137A0C(void) {
    if ((u32) ((u8)func_8005E0E0(D_800F62CF) & 0x7F) >= 2U) {
        D_80145EB4 += 2;
    }
    if (D_800F53DE == 0x62) {
        D_80145EB4 += 1;
    }
    func_8004DE1C();
}

void func_80137A80(void) {
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void func_80137AA8(void) {
    if (D_80145F60 == 0) {
        D_800F53A0.girl[D_800F62CF].unk_0A += 1;
    }
    func_8008FB00();
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_80137B18(void) {
    func_80051DD8(0x13, 0x801B0000, 0xA3C8);
    func_801372B0();
    func_8004DE1C();
}

void func_80137B50(void) {
    D_8012E66C = D_80146364;
    func_8004DE1C();
}

void func_80137B7C(void) {
    D_80146364 = (u8) D_8012E66C;
    D_800D9248 = 0;
    D_800D924C = 0;
    func_801372B0();
    D_800D9258 = D_80146224;
    D_800D925C = D_80146258;
    D_800D9260 = D_8014628C;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137BEC);

void func_80137CAC(void) {
    u32 temp_t6;

    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    if (temp_t6 < 2U) {
        D_800D9248 = 6;
    } else if (temp_t6 == 2) {
        D_800D9248 = 8;
    } else if (temp_t6 == 3) {
        D_800D9248 = 0xA;
    } else {
        D_800D9248 = 0xC;
    }
    func_8004DE1C();
}

void func_80137D3C(void) {
    D_800D9248 = (D_8012E66C * 2) + 0xD;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137D70);

void func_80137E24(void) {
    D_800D9248 = 0x1D;
    D_800D9258 = D_80146358;
    D_800D925C = D_8014635C;
    D_800D9260 = D_80146360;
    func_80137E84();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137E84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137F48);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80137FEC);

void func_801381A4(void) {
    func_80051DD8(3, 0x801B0000, 0xA3FB);
    func_80137524();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801381DC);

void func_80138264(void) {
    D_800D9248 = 1;
    D_800D924C = 0;
    func_8013829C();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013829C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80138510);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80138784);

void func_80138858(void) {
    D_800D9234 = (u8 *)&D_80147198;
    D_800D9238 = (u8 *)&D_8014719C;
    D_800D923C = D_8014718C;
    D_800D9240 = D_80147190;
    D_800D9244 = D_80147194;
    func_8008D610(D_8012E66C, 1, 0);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801388E0);

void func_80138954(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void func_80138998(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x201);
        func_8004DE1C();
    }
}

void func_801389D4(void) {
    func_80051DD8(7, 0x80197000, 0xA464);
    func_80138510();
    func_8004DE1C();
}

void func_80138A10(void) {
    D_800F5AAE |= 0x10;
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, &D_801459E0);
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_80138510();
    D_8014718C = D_80147100;
    D_80147190 = D_80147134;
    D_80147194 = D_80147168;
    D_80147198 = 0;
    D_8014719C = 0;
    func_8008F618(D_800F62CF = 0xC);
    D_800F5AAE |= 8;
    func_8004DE1C();
}

void func_80138B50(void) {
    func_8008A0D4(0x3FED);
    func_80090960(6, 0);
    func_8004DE1C();
}

void func_80138B84(void) {
    func_8008A0D4(0x40C1);
    func_8004DE1C();
}

void func_80138BAC(void) {
    D_80147198 = (D_80147198 + (D_800F53DE * 3)) - 0x120;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80138BF0);

void func_80138C78(void) {
    D_800F62CF = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80138C9C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013907C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801393C8);

void func_801396D8(void) {
    D_80147220 += 1;
    func_8004DEE4(1);
}

void func_8013970C(void) {
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}

void func_80139764(void) {
    if ((D_80147240 == 1) && (D_800F62CF == 9)) {
        D_800F647A += 6;
        return;
    }
    func_8004DE1C();
}

void func_801397BC(void) {
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(0x401A);
    func_8004DE1C();
}

void func_801397F8(void) {
    func_8008F618(0);
    func_80072734(0x53A1);
    func_8004DE1C();
}

void func_80139828(void) {
    func_8008F618(9);
    func_80072734(0x63CD);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80139858);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80139920);

void func_80139994(void) {
    func_80050D60(0, 0);
    func_80081D30(1);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801399C0);

void func_80139A98(void) {
    func_8008A0D4(0x40ED);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80139AC0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80139D34);

void func_80139DA8(void) {
    func_80051DD8(3, 0x801B0000, 0xA3FB);
    func_80137524();
    func_8004DE1C();
}

void func_80139DE0(void) {
    func_80090960(0xD, 0x26);
    D_8014718C = D_80147100;
    D_80147190 = D_80147134;
    D_80147194 = D_80147168;
    D_80147198 = 9;
    D_8014719C = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80139E4C);

void func_8013A104(void) {
    func_8004DEAC(D_800F5AB1);
    func_8004DEE4(D_800F5AB2);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A140);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A2F4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A4E8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A5F4);

void func_8013A76C(void) {
    if (func_80050AB8() == 1) {
        func_8004DDD8();
    }
}

void func_8013A7A0(void) {
    func_80086AB0(0x200);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A7C8);

void func_8013A820(void) {
    D_800D9234 = (u8 *)&D_801474A8;
    D_800D9238 = (u8 *)&D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 0);
}

void func_8013A89C(void) {
    D_800D9234 = (u8 *) &D_801474A8;
    D_800D9238 = (u8 *) &D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 1);
}

void func_8013A920(void) {
    switch (D_801474B8) {
    case 0:
        func_8013A998();
        return;
    case 1:
        func_8013AAAC();
        return;
    case 2:
        func_8013AB34();
        return;
    default:
        func_8013ABBC();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013A998);

void func_8013AA20(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013AA48);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013AAAC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013AB34);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013ABBC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013AC38);

void func_8013AD30(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AD74(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013ADBC(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE04(void) {
    D_801474A8 = 9;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE4C(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013AE80(void) {
    switch (D_801474B8) {
    case 0:
        func_8013AF1C();
        return;
    case 1:
        func_8013B03C();
        return;
    case 2:
        func_8013B0C4();
        return;
    case 3:
        func_8013B2B8();
        return;
    case 4:
        func_8013B340();
        return;
    default:
        func_8013B3B4();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013AF1C);

void func_8013AFE8(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x501);
        }
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B03C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B0C4);

void func_8013B1D0(void) {
    func_8013A820();
    if (D_800F6474++ == 0) {
        if (D_801474AC == 3) {
            func_80086AB0(0x500);
        }
    }
}

void func_8013B228(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

void func_8013B250(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_8013B278(void) {
    if (D_80147650 != 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B2B8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B340);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B3B4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B43C);

void func_8013B55C(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void func_8013B5A0(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void func_8013B5E8(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_800BCE10(&D_800D92E0, "\216\300\214\261\216\272");
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B644);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B6CC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B740);

void func_8013B7B4(void) {
    func_8008A0D4(0x3FFF);
    func_8004DE1C();
}

void func_8013B7DC(void) {
    func_8008A0D4(0x3FE3);
    func_8004DE1C();
}

void func_8013B804(void) {
    if (D_80147650 != 0) {
        func_8008A0D4(0x406F);
    } else {
        func_8008A0D4(0x4065);
    }
    func_8004DE1C();
}

void func_8013B848(void) {
    func_8008A0D4(0x4065);
    func_8004DE1C();
}

void func_8013B870(void) {
    func_8008A0D4(0x406F);
    func_8004DE1C();
}

void func_8013B898(void) {
    if (D_80147650 != 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013B8D8(void) {
    if (D_80147650 == 0) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013B920(void) {
    switch (D_801474B8) {
    case 0:
        func_8013B980();
        return;
    case 1:
        func_8013B9FC();
        return;
    default:
        func_8013BA84();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B980);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013B9FC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BA84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BB0C);

void func_8013BBFC(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145B80);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BC40);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BCB4);

void func_8013BD28(void) {
    func_8008A0D4(0x4011);
    func_8004DE1C();
}

void func_8013BD50(void) {
    func_8008A0D4(0x4080);
    func_8004DE1C();
}

void func_8013BD78(void) {
    func_8008A0D4(0x4078);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BDA0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BEA0);

void func_8013BFB4(void) {
    func_8013BFD4();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013BFD4);

void func_8013C070(void) {
    func_80051DD8(0x71, 0x801B0000, 0x82A1);
    func_8013BEA0();
    func_8004DE1C();
}

void func_8013C0A8(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_8013C0D0(void) {
    func_8013A820();
    if (D_801474AC == 3) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C118);

void func_8013C310(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BA0);
    func_8004DE1C();
}

void func_8013C354(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013C37C(void) {
    func_8008A0D4(0x466F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C3B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C4D0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C560);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C5E8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C670);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C6EC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C774);

void func_8013C8E0(void) {
    func_80051DD8(0x71, 0x801AE000, 0x8453);
    func_8013C3B0();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013C91C);

void func_8013C9D0(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BB8);
    func_8004DE1C();
}

void func_8013CA14(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC0);
    func_8004DE1C();
}

void func_8013CA5C(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC8);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013CAA4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013CB18);

void func_8013CCDC(void) {
    func_8008A0D4(0x405B);
    func_8004DE1C();
}

void func_8013CD04(void) {
    func_8008A0D4(0x3FE3);
    func_8004DE1C();
}

void func_8013CD2C(void) {
    func_8008A0D4(0x405B);
    func_8004DE1C();
}

void func_8013CD54(void) {
    func_8008A0D4(0x4052);
    func_8004DE1C();
}

void func_8013CD7C(void) {
    func_8008A0D4(0x4643);
    func_8004DE1C();
}

void func_8013CDA4(void) {
    func_80072734(0x5E5A);
    func_8004DE1C();
}

void func_8013CDD0(void) {
    if (D_801474B8 == 0) {
        func_8013CE0C();
        return;
    }
    func_8013CEB0();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013CE0C);

void func_8013CE88(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013CEB0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013CF2C);

void func_8013D01C(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BE0);
    func_8004DE1C();
}

void func_8013D060(void) {
    D_801474A8 = 5;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BEC);
    func_8004DE1C();
}

void func_8013D0A8(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void func_8013D0D0(void) {
    if (D_800F62CF == 5) {
        func_80072734(0x5F0E);
    } else {
        func_80072734(0x5482);
    }
    func_8004DE1C();
}

void func_8013D120(void) {
    if (D_801474B8 == 0) {
        func_8013D15C();
        return;
    }
    func_8013D20C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D15C);

void func_8013D1E4(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D20C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D294);

void func_8013D3BC(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C00);
    func_8004DE1C();
}

void func_8013D400(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C0C);
    func_8004DE1C();
}

void func_8013D448(void) {
    func_8008A0D4(0x4276);
    func_8004DE1C();
}

void func_8013D470(void) {
    func_8008A0D4(0x4040);
    func_8004DE1C();
}

void func_8013D498(void) {
    func_80072734(0x6346);
    func_8004DE1C();
}

void func_8013D4C0(void) {
    func_80072734(0x6157);
    func_8004DE1C();
}

void func_8013D4E8(void) {
    if (((u32) D_800F5488 >> 0x1C) == 9) {
        D_801474A8 = 5;
    }
    func_8004DE1C();
}

void func_8013D528(void) {
    D_801474A8 = 3;
    func_8004DE1C();
}

void func_8013D550(void) {
    switch (D_801474B8) {
    case 0:
        func_8013D5B0();
        return;
    case 1:
        func_8013D62C();
        return;
    default:
        func_8013D6B4();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D5B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D62C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D6B4);

void func_8013D73C(void) {
    func_80072734(0x658F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D764);

void func_8013D884(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C20);
    func_8004DE1C();
}

void func_8013D8C8(void) {
    D_801474A8 = 0xC;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C28);
    func_8004DE1C();
}

void func_8013D910(void) {
    D_801474A8 = 0xF;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C34);
    func_8004DE1C();
}

void func_8013D958(void) {
    func_8008A0D4(0x3FF5);
    func_8004DE1C();
}

void func_8013D980(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void func_8013D9A8(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        D_801474A8 = 6;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013D9E8);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013DA58);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013DAA4);

void func_8013DB50(void) {
    func_8013DB70();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013DB70);

void func_8013DBE4(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013DC2C);

void func_8013DD38(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C40);
    func_8004DE1C();
}

void func_8013DD7C(void) {
    func_8008A0D4(0x3FC0);
    func_8004DE1C();
}

void func_8013DDA4(void) {
    if ((u8) D_800F5833 < 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

void func_8013DDE8(void) {
    if ((u8) D_800F5833 >= 2U) {
        D_801474A8 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013DE30);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E0A4);

void func_8013E148(void) {
    D_800D9234 = (u8 *)&D_80148108;
    D_800D9238 = (u8 *)&D_8014810C;
    D_800D923C = D_801480FC;
    D_800D9240 = D_80148100;
    D_800D9244 = D_80148104;
    func_8008D610(D_8012E66C, 1, 0);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E1D0);

void func_8013E24C(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void func_8013E290(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void func_8013E2CC(void) {
    func_80051DD8(9, 0x80197000, 0xA46B);
    func_8013DE30();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E308);

void func_8013E47C(void) {
    func_80090960(7, 0);
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013E4B0(void) {
    D_80148108 += ((u8) D_800F53DE >= 0x61U) * 4;
    func_8004DE1C();
}

void func_8013E4F4(void) {
    if (D_80148110 == 0) {
        D_80148108 += 1;
    }
    func_8004DE1C();
}

void func_8013E534(void) {
    if (D_80148110 != 0) {
        func_8004DE1C();
        return;
    }
    func_8013E148();
}

void func_8013E570(void) {
    if (D_80148110 != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E5B0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E638);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E724);

void func_8013E7D8(void) {
    D_800F62CF = (u8) D_8012E66C;
    func_80090960(7, 0);
    func_8008F618(D_800F62CF);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013E824);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013EB10);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013ED84);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013FBF8);

void func_8013FD20(void) {
    D_800D9234 = (u8 *)&D_80148620;
    D_800D9238 = (u8 *)&D_80148624;
    D_800D923C = D_80148614;
    D_800D9240 = D_80148618;
    D_800D9244 = D_8014861C;
    func_8008D610(D_8012E66C, 1, 0);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013FD9C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8013FE2C);

void func_8013FFD8(void) {
    D_800D9234 = (u8 *)&D_800D9250;
    D_800D9238 = (u8 *)&D_800D9254;
    D_800D923C = D_80148628;
    D_800D9240 = D_8014862C;
    D_800D9244 = D_80148630;
    func_8008D610(0xFF, 1, 0);
}

void func_80140050(void) {
    D_800D9234 = (u8 *) &D_800D9250;
    D_800D9238 = (u8 *) &D_800D9254;
    D_800D923C = D_80148628;
    D_800D9240 = D_8014862C;
    D_800D9244 = D_80148630;
    func_8008D610(D_8012E66C, 1, 0);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801400D0);

void func_80140160(void) {
    func_800504CC(1, 0xB755, 0xB747, 0xC956, 0xC90A, 0xC8EF);
    func_8004DE1C();
}

void func_801401A4(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void func_801401E0(void) {
    func_80051DD8(0xE, 0x80197000, 0xA474);
    func_8013EB10();
    func_8004DE1C();
}

void func_8014021C(void) {
    D_8012E68C = 0x1E;
    func_8004DE1C();
}

void func_80140244(void) {
    if (((u32) D_800F54A6 >= (u32) ((D_800F53DE * 0x19) - 0x8B1)) && (D_800F594F != 0)) {
        D_800F594F += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    if ((u32) D_800F54AE >= (u32) (((D_800F53DE * 0x1E) - ((D_800F53DE == 0x61) * 0xA)) - 0xADC)) {
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140330);

void func_8014041C(void) {
    if (D_800F594F == 1) {
        func_8013FD20();
        return;
    }
    func_8004DE1C();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140464);

void func_801406A4(void) {
    func_8008A0D4(0x4169);
    func_8004DE1C();
}

void func_801406CC(void) {
    func_80072734(0x6940);
    func_8004DE1C();
}

void func_801406F4(void) {
    func_8004DDD8();
}

void func_80140714(void) {
    func_8004DEAC(4);
}

void func_80140740(void) {
    D_801486D0 = 0x801A6558;
    D_801486D4 = 0x801A655C;
    D_801486D8 = 0x801A6580;
    D_801486E0 = 0x801A0000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140780);

void func_8014086C(void) {
    func_80086AB0(0x204);
    func_8004DE1C();
}

void func_80140894(void) {
    D_8012B8C3 |= 0x80;
    func_8004DE1C();
}

void func_801408C4(void) {
    func_80090960(4, 0);
    D_800F594E += 1;
    func_800BCE10(&D_800D9280, &D_80145D20);
    func_800BCE10(&D_800D9288, &D_80145D28);
    func_800BCE10(&D_800D92E0, &D_80145D30);
    D_80148620 = 9;
    func_8004DE1C();
    func_80140740();
    func_8004EE18(D_801486E0, 0x11, 1, 3, 0);
    func_800549E8(0x60);
    D_8012B8C1 = 0xC;
    D_8012B8C2 = 1;
    D_8012B8F8 = 0x41000000;
    D_8012B8C3 = 4;
    D_8012B8CC = D_801486D4;
    D_8012B8D0 = D_801486D8;
    D_8012B8F4 = D_801486D0;
    D_8012B8D4 = D_801486DC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B903 = 0x11;
    D_8012B8E6 = -0xA0;
    D_8012B8EA = -0x78;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 8;
}

void func_80140A30(void) {
    func_8008A0D4(0x4173);
    func_8004DE1C();
}

void func_80140A58(void) {
    D_80148620 = (D_80148620 + (D_800F594E * 3)) - 3;
    func_8004DE1C();
}

void func_80140A9C(void) {
    func_80072734(0x57AC);
    func_8004DE1C();
}

void func_80140AC4(void) {
    func_80051DD8(0xD, 0x801A0000, 0xB0B8);
    func_80140740();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140AFC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140C90);

void func_80140D7C(void) {
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        func_8004DE1C();
        return;
    }
    func_8013FD20();
}

void func_80140DCC(void) {
    if (D_80148750 != 0) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140E0C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80140F90);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801412A4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80141550);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80141790);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801418B0);

void func_80141900(void) {
    D_80148804 = 0x801DDBB4;
    D_80148808 = 0x801DDBB8;
    D_8014880C = 0x801DDC70;
    D_80148814 = 0x801DB000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80141944);

void func_80141A94(void) {
    func_8004C6A0(0, 0x40);
    func_80050D60(0, 0);
    func_80081D30(1);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80141ACC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014206C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014210C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801421B4);

void func_801425F4(void) {
    func_8008A0D4(0x4183);
    func_8004DE1C();
}

void func_8014261C(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_80129F40[i * 0x44 + 0x1983] = 0;
    }
    D_80148820 = 0;
    D_80148824 = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014267C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80142790);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801427E4);

void func_80142828(void) {
    func_80051DD8(0x4D, 0x801B0000, 0xB0C5);
    func_8004DE1C();
}

void func_80142858(void) {
    func_80051DD8(0xE, 0x801D7000, 0xB112);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014288C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801428EC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80142A8C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80142C14);

void func_80142D00(void) {
    D_801488F0 = 0x801B21E0;
    D_801488F4 = 0x801B21E4;
    D_801488F8 = 0x801B2228;
    D_80148900 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80142D40);

void func_80142DF0(void) {
    func_80051DD8(0xD, 0x801B0000, 0xAFE0);
    func_80142D00();
    func_8004DE1C();
}

void func_80142E28(void) {
    func_80086AB0(0x203);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80142E50);

void func_80142F6C(void) {
    func_80142D00();
    func_8004EE18(D_80148900, 0x11, 1, 2, 0);
    func_800549E8(0x60);
    D_8012B8C1 = 1;
    D_8012B8C2 = 0;
    D_8012B8F8 = 0x01000000;
    D_8012B8C3 = 0x84;
    D_8012B8CC = D_801488F4;
    D_8012B8D0 = D_801488F8;
    D_8012B8F4 = D_801488F0;
    D_8012B8D4 = D_801488FC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B8C6 = 1;
    D_8012B903 = 0x11;
    D_8012B8E6 = -0xA0;
    D_8012B8EA = -0x78;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 0;
    D_80148620 = 0x13;
    D_80148614 = D_801481FC;
    D_80148618 = D_80148230;
    D_8014861C = D_80148264;
    func_8004DE1C();
}

void func_801430B4(void) {
    func_8008A0D4(0x417B);
    func_8004DE1C();
}

void func_801430DC(void) {
    D_8012B8C2 = 9;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143110);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143160);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143314);

void func_80143390(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_801433B8(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801433E0);

void func_80143468(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_80143490(void) {
    if (((u32) D_800F55CA >> 4) == 3) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

void func_801434D8(void) {
    if (((u32) D_800F55CA >> 4) != 3) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143520);

void func_8014359C(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801435C4);

void func_8014364C(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_80143674(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        D_80148620 += 3;
    }
    func_8004DE1C();
}

void func_801436BC(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801436E4);

void func_80143758(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143780);

void func_80143808(void) {
    D_80148620 = 4;
    func_8004DE1C();
}

void func_80143830(void) {
    func_80086AB0(0x505);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143858);

void func_801438E0(void) {
    if (func_8005B32C() != 0) {
        func_8005B06C(1);
        func_8004DE1C();
    }
}

void func_80143918(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143940);

void func_801439C8(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801439F0);

void func_80143A78(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_80143AA0(void) {
    func_80072734(0x5617);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143AD0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143BA0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143C70);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143D20);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80143E08);

void func_80143EB8(void) {
    func_80086AB0(0x206);
    func_8004DE1C();
}

void func_80143EE0(void) {
    D_80148BEC = func_8005B28C();
    D_8012E68C = 0x72;
    func_8005B264(0x1E);
    func_8005AEFC(2);
    func_8004DE1C();
}

void func_80143F2C(void) {
    func_8005B264(D_80148BEC);
    func_8005AEFC(1);
    func_8004DE1C();
}

void func_80143F64(void) {
    func_8004F860(0x280, 0, 0x40, 0x80, D_80148B90);
    D_800F65C0 = 1;
    func_8004DE1C();
}

void func_80143FAC(void) {
    func_80051DD8(0x55, 0x801B0000, 0x7D78);
    func_80143AD0();
    func_8004DE1C();
}

void func_80143FE4(void) {
    D_80148EAC = 1;
    D_8012CAD4 = -0x64;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80144018);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801444CC);

void func_801447BC(void) {
    func_8008A0D4(0x46B9);
    func_80090960(4, 2);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801447F0);

void func_8014486C(void) {
    func_80072734(0x5BE4);
    func_8004DE1C();
}

void func_80144894(void) {
    func_80086AB0(0x207);
    func_8004DE1C();
}

void func_801448BC(void) {
    func_8008E408(5);
    func_8004DE1C();
}

void func_801448E4(void) {
    func_80051DD8(0x45, 0x801B0000, 0x825C);
    func_80143BA0();
    func_8004DE1C();
}

void func_8014491C(void) {
    D_8012B94B &= 0x7F;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014494C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80144A74);

void func_80144B28(void) {
    func_8004F860(0x280, 0, 0x40, 0x80, D_80148B90);
    D_800F65C0 = 1;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80144B70);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80145084);

void func_80145350(void) {
    func_8008A0D4(0x46B1);
    func_80090960(4, 3);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_80145384);

void func_801454F8(void) {
    func_80086AB0(0x205);
    func_8004DE1C();
}

void func_80145520(void) {
    func_80086AB0(0x504);
}

void func_80145540(void) {
    func_80051DD8(0x45, 0x801B0000, 0x88B4);
    func_80143C70();
    func_8004DE1C();
}

void func_80145578(void) {
    D_8012B960 = 0;
    D_8012B950 = 0;
    D_8012B94A = 3;
    func_80086AB0(0x503);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_801455BC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI", func_8014569C);

void func_801457D8(void) {
    func_8008A0D4(0x4280);
    func_8004DE1C();
}

void func_80145800(void) {
    func_8008A0D4(0x4718);
    func_80090960(4, 4);
    func_8004DE1C();
}
