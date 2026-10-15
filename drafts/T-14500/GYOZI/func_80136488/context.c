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
    s16 var_s0;

    D_801461A0 = 0;
    func_80136220();
    func_8004EE18(D_80146170, 0x11, 1, 2, 0);
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        func_800549E8(var_s0 + 0x60);
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_01 = 0xA;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_02 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_38 = 0x01000000;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_03 = 4;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_0C = D_80146158;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_10 = D_80146160;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_34 = D_80146150;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_16 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_18 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_06 = 1;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_43 = 0x11;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_26 = -0xA0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_2A = -0x78;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_07 = 0x80;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_05 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_04 = 0;
        ((GyoziInitSprite44 *)D_80129F40)[var_s0 + 0x60].unk_14 = D_80146168;
    }
    ((GyoziInitSprite44 *)D_80129F40)[0x60].unk_26 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x61].unk_16 = 3;
    ((GyoziInitSprite44 *)D_80129F40)[0x62].unk_16 = 4;
    func_8004EE18(D_80146174, 0x12, 0, 0, 2);
    func_800549E8(0x63);
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_01 = 0xD;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_02 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_38 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_03 = 4;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_0C = D_8014615C;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_10 = D_80146164;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_34 = D_80146154;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_14 = D_8014616C;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_16 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_18 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_06 = 1;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_43 = 0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_26 = -0xA0;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_2A = -0x78;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_07 = 0x80;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_05 = 0x81;
    ((GyoziInitSprite44 *)D_80129F40)[0x63].unk_04 = 0;
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

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, temporaries: the original keeps (D_800F6468 & 7) in $t6/$t7 and the index in $v0, IDO uses $v1/$v0 (about 25 lines) */
void func_80137188(void) {
    u32 t;
    u32 v;

    t = D_800F6468 & 7;
    v = t;
    if (t != 0) {
        v = (t - 1) & 0xFF;
    }
    D_80146178 = v;
    func_800BCE10(&D_800D9320, &D_8014617C[v * 5]);
    v = D_80146178;
    if (v == 0) {
        D_800F54B6 -= 5;
    }
    D_80146178 = v;
    if (v == 6) {
        D_800F54B6 += 5;
    }
    func_8008FB00();
}
#else
INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80136220", func_80137188);
#endif

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
