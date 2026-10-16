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

/* bulk_m2c declarations (T-11190) */
extern s16 D_8012B91A;
extern s16 D_8012B95E;
extern s16 D_8012B9A0;
extern s16 D_8012B9A2;
extern s16 D_8012B9A4;
extern s16 D_8012B9B2;
extern s16 D_8012B9B6;
extern s32 *D_8012B998;
extern s32 *D_8012B99C;
extern s32 *D_8012B9C0;
extern s32 D_8012B9C4;
extern s8 D_8012B98D;
extern s8 D_8012B98E;
extern s8 D_8012B98F;
extern s8 D_8012B990;
extern s8 D_8012B991;
extern s8 D_8012B992;
extern s8 D_8012B993;
extern s8 D_8012B9CF;
/* end bulk_m2c declarations */
void func_80136488(void) {
    s16 var_s0;
    s32 temp_lo;
    u8 *temp_v0;

    D_801461A0 = 0;
    func_80136220();
    func_8004EE18(D_80146170, 0x11, 1, 2, 0);
    var_s0 = 0;
    do {
        func_800549E8(var_s0 + 0x60);
        temp_lo = var_s0 * 0x44;
        var_s0 += 1;
        temp_v0 = &D_80129F40[temp_lo];
        *(s8 *)((unsigned char *)temp_v0 + (6529)) = 0xA;
        *(s8 *)((unsigned char *)temp_v0 + (6530)) = 0;
        *(s32 *)((unsigned char *)temp_v0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)temp_v0 + (6531)) = 4;
        *(s32 **)((unsigned char *)temp_v0 + (6540)) = (s32 *) D_80146158;
        *(s32 **)((unsigned char *)temp_v0 + (6544)) = (s32 *) D_80146160;
        *(s32 **)((unsigned char *)temp_v0 + (6580)) = (s32 *) D_80146150;
        *(s16 *)((unsigned char *)temp_v0 + (6550)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (6552)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (6534)) = 1;
        *(s8 *)((unsigned char *)temp_v0 + (6595)) = 0x11;
        *(s16 *)((unsigned char *)temp_v0 + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)temp_v0 + (6570)) = -0x78;
        *(s8 *)((unsigned char *)temp_v0 + (6535)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6533)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (6532)) = 0;
        *(s16 *)((unsigned char *)temp_v0 + (6548)) = (s16) D_80146168;
    } while (var_s0 < 3);
    D_8012B8E6 = 0;
    D_8012B91A = 3;
    D_8012B95E = 4;
    func_8004EE18(D_80146174, 0x12, 0, 0, 2);
    func_800549E8(0x63);
    D_8012B98D = 0xD;
    D_8012B98E = 0;
    D_8012B9C4 = 0;
    D_8012B98F = 4;
    D_8012B998 = D_8014615C;
    D_8012B99C = D_80146164;
    D_8012B9C0 = D_80146154;
    D_8012B9A0 = D_8014616C;
    D_8012B9A2 = 0;
    D_8012B9A4 = 0;
    D_8012B992 = 1;
    D_8012B9CF = 0;
    D_8012B9B2 = -0xA0;
    D_8012B9B6 = -0x78;
    D_8012B993 = 0x80;
    D_8012B991 = 0x81;
    D_8012B990 = 0;
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
