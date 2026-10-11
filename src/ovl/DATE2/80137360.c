#include "common.h"
#include "ovl/DATE2.h"

void func_80137360(void) {
    D_8013A770 = 0x801B0478;
    D_8013A774 = 0x801B13A4;
    D_8013A778 = 0x801B2998;
    D_8013A77C = 0x801B38C8;
    D_8013A780 = 0x801B4258;
    D_8013A784 = 0x801B4B98;
    D_8013A788 = 0x801B5574;
    D_8013A78C = 0x801B6560;
    D_8013A790 = 0x801B7B74;
    D_8013A794 = 0x801B8B18;
    D_8013A798 = 0x801B93D0;
    D_8013A79C = 0x801B9DA0;
    D_8013A7A0 = 0x801BA780;
    D_8013A7A4 = 0x801B0510;
    D_8013A7A8 = 0x801B14E0;
    D_8013A7AC = 0x801B2AD0;
    D_8013A7B0 = 0x801B3960;
    D_8013A7B4 = 0x801B42F0;
    D_8013A7B8 = 0x801B4C38;
    D_8013A7BC = 0x801B560C;
    D_8013A7C0 = 0x801B66A0;
    D_8013A7C4 = 0x801B7CB4;
    D_8013A7C8 = 0x801B8BB0;
    D_8013A7CC = 0x801B9468;
    D_8013A7D0 = 0x801B9EA8;
    D_8013A7D4 = 0x801BA818;
    D_8013A7D8 = 0x801B0888;
    D_8013A7DC = 0x801B1E14;
    D_8013A7E0 = 0x801B33B0;
    D_8013A7E4 = 0x801B3CF4;
    D_8013A7E8 = 0x801B4684;
    D_8013A7EC = 0x801B4FEC;
    D_8013A7F0 = 0x801B59F4;
    D_8013A7F4 = 0x801B6FB8;
    D_8013A7F8 = 0x801B85CC;
    D_8013A7FC = 0x801B8F28;
    D_8013A800 = 0x801B97E0;
    D_8013A804 = 0x801BA2C8;
    D_8013A808 = 0x801BAB90;
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_8013A81C;

void func_801375D4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_8013A81C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013765C(void) {
    func_80042908(2);
    func_80042940(D_800E6280.unk_722);
}

void func_8013768C(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80137360();
    func_8004284C();
}

void func_801376C4(void) {
    D_80122CDC = D_8013A818;
    func_8004284C();
}

void func_801376F0(void) {
    D_8013A818 = D_80122CDC;
    D_800CA148 = 0;
    D_800CA14C = 0;
    D_800CA160 = D_8013A774;
    D_800CA164 = D_8013A7A8;
    D_800CA168 = D_8013A7DC;
    func_8004284C();
}

void func_8013775C(void) {
    if (D_80122CDC != 0) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 -= 1;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += 5;
        func_80084D3C();
        D_800E6280.unk_110A += 0xD;
        return;
    }
    D_800CA148 += 1;
    func_8004284C();
}

void func_8013780C(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    if (t < 2) {
        D_800CA148 = 6;
    } else if (t == 2) {
        D_800CA148 = 8;
    } else if (t == 3) {
        D_800CA148 = 0xA;
    } else {
        D_800CA148 = 0xC;
    }
    D_800CA14C = 1;
    func_8004284C();
}

void func_801378A4(void) {
    s32 pad; /* FAKE: unused local above buf, puts buf at sp+0x28 as in the original; real source unknown. T-4010 */
    u8 buf[3] = "こ";

    D_800CA148 = D_80122CDC * 2 + 0xD;
    if (D_80122CDC == 0) {
        (**(u8 ***)(D_800CA160 + D_800CA148 * 4 + 4))[0] = buf[0];
        (**(u8 ***)(D_800CA160 + D_800CA148 * 4 + 4))[1] = buf[1];
    }
    func_8004284C();
}

void func_80137948(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(0) & 0x7F;
    if (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF) && D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) && t < 2) {
        D_800E6280.unk_71E |= 4;
        func_80042908(4);
        return;
    }
    func_8004284C();
}

void func_801379CC(void) {
    D_800CA148 = 0x1D;
    D_800CA160 = D_8013A80C;
    D_800CA164 = D_8013A810;
    D_800CA168 = D_8013A814;
    func_80137A2C();
    func_8004284C();
}

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */
extern DateTxt D_8013A878;

void func_80137A2C(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8013A878;
    func_800AE0F0(D_800CA25C, &tbl.s[D_800E6280.unk_F5F * 0x129 + D_800E6280.unk_03E * 0x63 + D_80122CDC * 0x21 - 0x24BD]);
}

void func_80137AF0(void) {
    D_8013A80C = D_800CA160;
    D_8013A810 = D_800CA164;
    D_8013A814 = D_800CA168;
    D_800CA148 = 0;
    D_800CA14C = (D_800CA14C + D_800E6280.unk_03E) - 0x5F;
    D_800CA160 = D_8013A79C;
    D_800CA164 = D_8013A7D0;
    D_800CA168 = D_8013A804;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_80137B94);
