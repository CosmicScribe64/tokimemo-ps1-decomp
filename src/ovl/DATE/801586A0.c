#include "common.h"
#include "ovl/DATE.h"

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_801586A0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80158750);

void func_80158800(void) {
    D_80160498 = 0x801CE0F4;
    D_8016049C = 0x801CE0F8;
    D_801604A0 = 0x801CE118;
    D_801604A4 = *(s16 *)0x801CE12C;
    D_801604A8 = 0x801B0000;
    D_801604AC = 0x801B2000;
    D_801604B0 = 0x801B6000;
    D_801604B4 = 0x801BA000;
    D_801604B8 = 0x801BE000;
    D_801604BC = 0x801C2000;
    D_801604C0 = 0x801C6000;
}

void func_801588B0(void) {
    D_801604C4 = 0x801CE1A0;
    D_801604C8 = 0x801CE1A4;
    D_801604CC = 0x801CE1D4;
    D_801604D0 = *(s16 *)0x801CE1F0;
    D_801604D4 = 0x801B0000;
    D_801604D8 = 0x801B2000;
    D_801604DC = 0x801B6000;
    D_801604E0 = 0x801BA000;
    D_801604E4 = 0x801BE000;
    D_801604E8 = 0x801C2000;
    D_801604EC = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80158960);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_801589F4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80158B1C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80158C8C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80158D98);

void func_80158F44(void) {
    bg_read_sub2(0x47D6);
    func_8004284C();
}

void func_80158F6C(void) {
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_80158F94(void) {
    bg_read_sub2(0x48E0);
    func_8004284C();
}

void func_80158FBC(void) {
    bg_read_sub2(0x45BF);
    func_8004284C();
}

void func_80158FE4(void) {
    bg_read_sub2(0x490A);
    func_8004284C();
}

void func_8015900C(void) {
    bg_read_sub2(0x45F8);
    func_8004284C();
}

void func_80159034(void) {
    bg_read_sub2(0x4868);
    func_8004284C();
}

void func_8015905C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4520);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_8015908C);

void func_8015913C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8C4B);
    func_801586A0();
    func_8004284C();
}

void func_80159174(void) {
    if (D_800E6807 != 0) {
        D_800CA150 = (u16) D_800CA150 + 1;
        D_800CA154 = 0;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_801591BC);

void func_80159268(void) {
    func_80046318(0x45, 0x801B0000, 0x8C88);
    func_80158750();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_801592A0);

void func_8015931C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8CCD);
    func_80158800();
    func_8004284C();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_80160714;

void func_80159354(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_80160714;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_801593C8(void) {
    D_800E71DF = 4;
    func_8014C5C8();
}

void func_801593F0(void) {
    D_800E71DF = 0xE;
    func_8014C5C8();
}

void func_80159418(void) {
    func_80046318(0x3D, 0x801B0000, 0x8D0A);
    func_801588B0();
    func_8004284C();
}

void func_80159450(void) {
    D_800E71DF = 0xE;
    D_80120666 = 4;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801586A0", func_80159484);
