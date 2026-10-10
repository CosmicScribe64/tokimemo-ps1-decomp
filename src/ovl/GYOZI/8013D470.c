#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147F04;

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
    u8 sel = D_801474B8; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
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

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
extern FnTbl37 D_80147E70;

void func_8013D5B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_80147E70;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013D62C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147F04;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147F54;

void func_8013D6B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147F54;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013D73C(void) {
    func_80072734(0x658F);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013D764);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C20);

void func_8013D884(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C20);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C28);

void func_8013D8C8(void) {
    D_801474A8 = 0xC;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145C28);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013D470.rodata", D_80145C34);

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

void func_8013D9E8(void) {
    if (((D_800F5638 << 0x1D) >> 0x1F) == (D_800F5488 >> 0x1C)) {
        if (((u32)D_800F563A >> 4) == 7) {
            D_801474A8 = 0xA;
        } else {
            D_801474A8 = 8;
        }
    }
    func_8004DE1C();
}

void func_8013DA58(void) {
    if (((D_800F5638 << 0x1D) >> 0x1F) != (D_800F5488 >> 0x1C)) {
        D_801474A8 = 4;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013D470", func_8013DAA4);
