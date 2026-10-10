#include "common.h"
#include "ovl/GYOZI.h"

void func_8013C3B0(void) {
    D_80147AD0 = (u8 *)0x801E60CC;
    D_80147AD4 = (u8 *)0x801E60D0;
    D_80147AD8 = (u8 *)0x801E60E4;
    D_80147ADC = *(s16 *)0x801E60F0;
    D_80147AE0 = (u8 *)0x801AE000;
    D_80147AE4 = (u8 *)0x801B0000;
    D_80147AE8 = (u8 *)0x801B2000;
    D_80147AEC = (u8 *)0x801B6000;
    D_80147AF0 = (u8 *)0x801BA000;
    D_80147AF4 = (u8 *)0x801BE000;
    D_80147AF8 = (u8 *)0x801C2000;
    D_80147AFC = (u8 *)0x801C6000;
    D_80147B00 = (u8 *)0x801CE000;
    D_80147B04 = (u8 *)0x801D2000;
    D_80147B08 = (u8 *)0x801D6000;
    D_80147B0C = (u8 *)0x801DA000;
    D_80147B10 = (u8 *)0x801DE000;
    D_80147B14 = (u8 *)0x801E2000;
}

void func_8013C4D0(void) {
    switch (D_801474B8) {
    case 0:
        func_8013C560();
        return;
    case 1:
        func_8013C5E8();
        return;
    case 2:
        func_8013C670();
        return;
    case 3:
        func_8013C6EC();
        return;
    default:
        func_8013C774();
        return;
    }
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013C560);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013C5E8);

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80147BB8;

void func_8013C670(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80147BB8;
    idx = D_800F647A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013C6EC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013C774);

void func_8013C8E0(void) {
    func_80051DD8(0x71, 0x801AE000, 0x8453);
    func_8013C3B0();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013C91C);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013C3B0.rodata", D_80145BB8);

void func_8013C9D0(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BB8);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013C3B0.rodata", D_80145BC0);

void func_8013CA14(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC0);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013C3B0.rodata", D_80145BC8);

void func_8013CA5C(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC8);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013CAA4);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013C3B0", func_8013CB18);

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
