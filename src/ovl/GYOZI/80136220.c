#include "common.h"
#include "ovl/GYOZI.h"

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

void func_801362A0(void) {
    u8 index;
    GyoziCallbackTable30 table;

    table = D_801461A4;
    if (D_80145F60 != 0) {
        if (table.callback[D_800F647A] == D_80089BD4) {
            table.callback[D_800F647A] = D_80089B6C;
        }
    }
    index = D_800F647A;
    table.callback[index](0x40, index);
    if ((u8)D_801461A0 != 0) {
        func_800904A8(-0xA0, -0x78, 0x140, 0x64, 0xD, 0xE0FF, 0xFFFF);
        func_800904A8(-0xA0, -0x14, 0x140, 0x3C, 0xD, 0x6060, 0xE0FF);
        func_80054FCC(0xD);
    }
}

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

void func_80136488(void) {
    s16 i;

    D_801461A0 = 0;
    func_80136220();
    func_8004EE18(D_80146170, 0x11, 1, 2, 0);
    for (i = 0; i < 3; i++) {
        func_800549E8(i + 0x60);
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_01 = 0xA;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_02 = 0;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_03 = 4;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_0C = (u8 *)D_80146158;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_10 = (u8 *)D_80146160;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_34 = (u8 *)D_80146150;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_14 = D_80146168;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_16 = 0;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_18 = 0;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_06 = 1;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_43 = 0x11;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_26 = -0xA0;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_2A = -0x78;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_07 = 0x80;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_05 = 0;
        ((DisplayActor44 *)D_80129F40)[i + 0x60].unk_04 = 0;
    }
    ((DisplayActor44 *)D_80129F40)[0x60].unk_26 = 0;
    ((DisplayActor44 *)D_80129F40)[0x61].unk_16 = 3;
    ((DisplayActor44 *)D_80129F40)[0x62].unk_16 = 4;
    func_8004EE18(D_80146174, 0x12, 0, 0, 2);
    func_800549E8(0x63);
    ((DisplayActor44 *)D_80129F40)[0x63].unk_01 = 0xD;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_02 = 0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_38 = 0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_03 = 4;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_0C = (u8 *)D_8014615C;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_10 = (u8 *)D_80146164;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_34 = (u8 *)D_80146154;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_14 = D_8014616C;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_16 = 0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_18 = 0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_06 = 1;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_43 = 0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_2A = -0x78;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_07 = 0x80;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_05 = 0x81;
    ((DisplayActor44 *)D_80129F40)[0x63].unk_04 = 0;
    func_8004DE1C();
}

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

void func_8013683C(void) {
    D_8012B8C0[0].unk_03 |= 0x80;
    D_8012B8C0[1].unk_02 = 1;
    D_8012B8C0[1].unk_03 |= 0x80;
    D_8012B8C0[2].unk_02 = 1;
    D_8012B8C0[2].unk_03 |= 0x80;
    D_801461A0 = 1;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_801368B8);

void func_80137188(void) {
    D_80146178 = D_800F6468 & 7;
    if (D_80146178 != 0) {
        D_80146178--;
    }
    func_800BCE10(&D_800D9320, &D_8014617C[D_80146178 * 5]);
    if (D_80146178 == 0) {
        D_800F54B6 -= 5;
    }
    if (D_80146178 == 6) {
        D_800F54B6 += 5;
    }
    func_8008FB00();
}

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
