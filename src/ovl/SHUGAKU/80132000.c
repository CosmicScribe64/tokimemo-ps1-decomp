#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt D_8013B1AC;

void func_80132000(void) {
    D_8013AFD0 = 0x801B0478;
    D_8013AFD4 = 0x801B13A4;
    D_8013AFD8 = 0x801B2998;
    D_8013AFDC = 0x801B38C8;
    D_8013AFE0 = 0x801B4258;
    D_8013AFE4 = 0x801B4B98;
    D_8013AFE8 = 0x801B5574;
    D_8013AFEC = 0x801B6560;
    D_8013AFF0 = 0x801B7B74;
    D_8013AFF4 = 0x801B8B18;
    D_8013AFF8 = 0x801B93D0;
    D_8013AFFC = 0x801B9DA0;
    D_8013B000 = 0x801BA780;
    D_8013B004 = 0x801B0510;
    D_8013B008 = 0x801B14E0;
    D_8013B00C = 0x801B2AD0;
    D_8013B010 = 0x801B3960;
    D_8013B014 = 0x801B42F0;
    D_8013B018 = 0x801B4C38;
    D_8013B01C = 0x801B560C;
    D_8013B020 = 0x801B66A0;
    D_8013B024 = 0x801B7CB4;
    D_8013B028 = 0x801B8BB0;
    D_8013B02C = 0x801B9468;
    D_8013B030 = 0x801B9EA8;
    D_8013B034 = 0x801BA818;
    D_8013B038 = 0x801B0888;
    D_8013B03C = 0x801B1E14;
    D_8013B040 = 0x801B33B0;
    D_8013B044 = 0x801B3CF4;
    D_8013B048 = 0x801B4684;
    D_8013B04C = 0x801B4FEC;
    D_8013B050 = 0x801B59F4;
    D_8013B054 = 0x801B6FB8;
    D_8013B058 = 0x801B85CC;
    D_8013B05C = 0x801B8F28;
    D_8013B060 = 0x801B97E0;
    D_8013B064 = 0x801BA2C8;
    D_8013B068 = 0x801BAB90;
}

typedef struct {
    void (*f[70])();
} FnTbl70; /* size 0x118 */
extern FnTbl70 D_8013B094;

void func_80132274(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl70 tbl;

    tbl = D_8013B094;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_801322F0);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132364);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_8013243C);

void func_801325C0(void) {
    func_80044890(0, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        if (D_800E6280.unk_1109 == 6) {
            func_80044750(0x200);
        }
        func_8004284C();
    }
    func_8004284C();
}

void func_80132638(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801326C0(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80132000();
    func_8004284C();
}

void func_801326F8(void) {
    func_80133430();
    D_8013B06C = D_8013BE74;
    D_8013B070 = D_8013BEA8;
    D_8013B074 = D_8013BEDC;
    D_8013B084 = 1;
    D_8013B088 = 0;
    func_8004284C();
}

void func_80132760(void) {
    if (D_800E6280.unk_721 != 6) {
        func_80085B3C(0xA, 0x29);
    } else {
        switch (D_800CA2CC) {                       /* irregular */
        case 0:
            func_80085B3C(0xA, 0x2A);
            break;
        case 1:
            func_80085B3C(0xA, 0x2B);
            break;
        default:
            func_80085B3C(0xA, 0x2C);
            break;
        }
    }
    func_80132000();
    func_8013282C();
    if ((D_800B593C != 0) && (D_80122CF8 != 0)) {
        D_800CA2F0 = 1;
        return;
    }
    D_800CA2F0 = 0;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_8013282C);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132B50);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132BCC);

void func_80132C1C(void) {
    if (D_800E6280.unk_721 == 6) {
        D_800E6280.unk_110A += 6;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80132C64);

void func_80132D14(void) {
    u32 t;

    t = func_80051A68(D_800E6280.unk_F5F);
    t = t & 0x7F;
    t = t & 0x7F; /* FAKE: the repeated mask reproduces the original's extra move; real source unknown. T-4070 */
    if (t >= 2U) {
        if (t == 2) {
            D_8013B084 += 2;
        } else if (t == 3) {
            D_8013B084 += 4;
        } else {
            D_8013B084 += 6;
        }
    }
    func_8004284C();
}

void func_80132DAC(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_8004284C();
}

void func_80132DE0(void) {
    if (D_800E6280.unk_721 != 6) {
        func_801386C4();
        return;
    }
    func_8004284C();
}

void func_80132E20(void) {
    D_8013B084 = D_8013B08C;
    D_8013B06C = D_8013B078;
    D_8013B070 = D_8013B07C;
    D_8013B074 = D_8013B080;
    func_80132E84();
    func_8004284C();
}

void func_80132E84(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8013B1AC;
    func_800AE0F0(D_800CA25C, &tbl.s[D_800E6280.unk_F5F * 0x129 + D_8013B090 * 0x63 + D_80122CDC * 0x21]);
}

void func_80132F44(void) {
    D_800CA134 = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80132FC0(void) {
    D_800CA134 = &D_8013B084;
    D_800CA138 = &D_8013B088;
    D_800CA13C = D_8013B06C;
    D_800CA140 = D_8013B070;
    D_800CA144 = D_8013B074;
    func_80082764(0xFF, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80133038);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_801330E8);

void func_80133298(void) {
    D_8013B084 = (D_80122CDC * 2) + 0xD;
    D_800CA2F0 = 2;
    func_8004284C();
}

void func_801332D8(void) {
    get_g_zyotai_s(D_800E6280.unk_75D);
    if (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF) && D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) {
        func_80042908(7);
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80132000", func_80133348);
