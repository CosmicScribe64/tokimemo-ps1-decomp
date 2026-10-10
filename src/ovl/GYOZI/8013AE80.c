#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_801477F8;

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

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */

extern FnTbl20 D_80147654;

void func_8013AF1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147654;
    idx = D_800F647A;
    tbl.f[idx]();
    if (D_801474A8 == 1) {
        if (D_801474AC == 8) {
            if (D_800F6474++ == 0) {
                func_80086AB0(0x500);
            }
        }
    }
}

void func_8013AFE8(void) {
    func_8013A820();
    if (D_801474AC == 2) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x501);
        }
    }
}

extern FnTbl20 D_801476A4;

void func_8013B03C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801476A4;
    idx = D_800F647A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B0C4);

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

typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
extern FnTbl29 D_80147784;

void func_8013B2B8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl29 tbl;

    tbl = D_80147784;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013B340(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_801477F8;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147858;

void func_8013B3B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147858;
    idx = D_800F647A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013AE80", func_8013B43C);

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

void func_8013B644(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    D_801474A8 = 0xE;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "教室");
    func_800BCE10(&D_800D92E0, "実験室");
    func_8004DE1C();
}

void func_8013B6CC(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    D_801474A8 = 0x12;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "実験室");
    func_8004DE1C();
}

void func_8013B740(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    D_801474A8 = 0x17;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "実験室");
    func_8004DE1C();
}

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
