#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_8013C884;

void func_801378B0(void) {
    D_8013C870 = 0x801D60E0;
    D_8013C874 = 0x801D60E4;
    D_8013C878 = 0x801D60F4;
    D_8013C880 = 0x801D0000;
}

void func_801378F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8013C884;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80137978(void) {
    D_800CA2DC = 0;
    func_8004284C();
}

void func_8013799C(void) {
    s32 pad; /* FAKE: unused local above buf, puts buf at sp+0x30 as in the original; real source unknown. T-4010 */
    u8 buf[3] = "　";

    if (D_800E6280.unk_0F4.b[1] & 0xF) {
        if (D_800E6280.unk_110D == 0) {
            D_800CA2DC = 2;
            (**(u8 ***)(D_800CA2D0 + 8))[0] = buf[0];
            (**(u8 ***)(D_800CA2D0 + 8))[1] = buf[1];
            (**(u8 ***)(D_800CA2D0 + 8))[2] = buf[2];
        }
        func_801386C4();
        if (D_800E6280.unk_110D == 0) {
            if (D_800CA2DC == 2) {
                func_8004E788(-0x48, 0x30, 0, "くぅー、", 0);
                func_8004E788(-0x48, 0x40, 0, "体の調子が、最悪だぜ。", 0);
                func_8004E788(-0x48, 0x50, 0, "でも、修学旅行だから", 0);
                func_8004E788(-0x48, 0x60, 0, "集合場所に行かなきゃ。", 0);
                D_800E6280.unk_110D = 1;
            }
        }
    } else {
        func_8004284C();
    }
}

void func_80137AF8(void) {
    func_801386C4();
    if ((D_800CA2E0 == 2) && (D_800E6280.unk_1104.w == 1)) {
        D_80122CFC = 0x3C;
    }
    D_800E6280.unk_1104.w += 1;
}

void func_80137B54(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC512, 0xC4E7, 0xD557, 0xD508, 0xD4F1);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80137BBC(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80137C3C(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    switch (D_800CA2CC) {                           /* irregular */
    case 0:
        func_80044750(0x200);
        break;
    case 1:
        func_80044750(0x201);
        break;
    default:
        func_80044750(0x202);
        break;
    }
    func_8004284C();
}

void func_80137CC0(void) {
    func_80046318(0x2D, 0x80197000, 0xAF7C);
    func_80136380();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_80137CFC);

void func_80137F04(void) {
    s32 temp_v0;

    D_80122CE0 = 0xFF;
    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FAD, 0);
    if (temp_v0 == *D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (temp_v0 == (1 - *D_800B5938)) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

s32 func_80137F90(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800573AC();
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_8005751C(0);
    func_8004284C();
}

void func_8013801C(void) {
    dec_bg_show_switch(0);
    func_8004284C();
}

void func_80138044(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8008585C();
    func_800AE0F0(D_800CA19C, "修学旅行");
    func_8004E9F4(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    switch (D_800CA2CC) {
    case 0:
        func_8007ED84(0x421C);
        break;
    case 1:
        func_8007ED84(0x420A);
        break;
    default:
        func_8007ED84(0x4213);
        break;
    }
    func_8004284C();
}

void func_80138154(void) {
    u8 sel = D_800CA2CC; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        bg_read_sub2(0x4237);
        break;
    case 1:
        bg_read_sub2(0x422E);
        break;
    default:
        bg_read_sub2(0x4241);
        break;
    }
    func_8004284C();
}

void func_801381BC(void) {
    D_800CA2CC = D_80122CDC;
    switch (D_80122CDC) {
    case 0:
        func_800AE0F0(D_800CA23C, "京都・奈良");
        func_800AE0F0(D_800CA1DC, "京都・奈良");
        break;
    case 1:
        func_800AE0F0(D_800CA23C, "沖縄");
        func_800AE0F0(D_800CA1DC, "沖縄");
        break;
    case 2:
        func_800AE0F0(D_800CA23C, "北海道");
        func_800AE0F0(D_800CA1DC, "北海道");
        break;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_80138290);

void func_80138320(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        func_801383C8();
        break;
    case 1:
        if (func_800460CC() & 1) {
            D_800E6280.unk_1104.w += 1;
        }
        break;
    case 2:
        func_80138400();
        break;
    case 3:
        func_801385A8();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_801383C8(void) {
    func_80046318(0xD, 0x801D0000, 0xBB98);
    D_800E6280.unk_1104.w += 1;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_80138400);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_801385A8);

void func_8013864C(void) {
    D_800CA134 = &D_800CA2DC;
    D_800CA138 = &D_800CA2E0;
    D_800CA13C = D_800CA2D0;
    D_800CA140 = D_800CA2D4;
    D_800CA144 = D_800CA2D8;
    func_80082764(0xFF, 1, 0);
}

void func_801386C4(void) {
    D_800CA134 = &D_800CA2DC;
    D_800CA138 = &D_800CA2E0;
    D_800CA13C = D_800CA2D0;
    D_800CA140 = D_800CA2D4;
    D_800CA144 = D_800CA2D8;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_80138740);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/801378B0", func_801388D8);
