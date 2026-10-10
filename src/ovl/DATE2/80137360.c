#include "common.h"
#include "ovl/DATE2.h"

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_80137360);

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_8013A81C;

void func_801375D4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_8013A81C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_8013765C(void) {
    func_80042908(2);
    func_80042940(D_800E69A2);
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

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_8013775C);

void func_8013780C(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(D_800E71DF) & 0x7F;
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

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_801378A4);

void func_80137948(void) {
    u32 t;

    t = (u8)get_g_zyotai_s(0) & 0x7F;
    if (D_800E62BF == (D_800E6378 & 0xF) && D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B) && t < 2) {
        D_800E699E |= 4;
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

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_80137A2C);

void func_80137AF0(void) {
    D_8013A80C = D_800CA160;
    D_8013A810 = D_800CA164;
    D_8013A814 = D_800CA168;
    D_800CA148 = 0;
    D_800CA14C = (D_800CA14C + D_800E62BE) - 0x5F;
    D_800CA160 = D_8013A79C;
    D_800CA164 = D_8013A7D0;
    D_800CA168 = D_8013A804;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80137360", func_80137B94);
