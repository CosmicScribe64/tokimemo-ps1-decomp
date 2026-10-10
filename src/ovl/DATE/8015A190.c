#include "common.h"
#include "ovl/DATE.h"

void func_8015A190(void) {
    func_80046318(0x3D, 0x801B0000, 0x8E3F);
    func_80159560();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8015A190", func_8015A1C8);

void func_8015A250(void) {
    func_80046318(0x3D, 0x801B0000, 0x8E7C);
    func_80159610();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8015A190", func_8015A288);

void func_8015A328(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_8015A350(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_8015A378(void) {
    func_80046318(0x45, 0x801B0000, 0x8EB9);
    func_801596C0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8015A190", func_8015A3B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8015A190", func_8015A414);

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80160B80;

void func_8015A4A8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80160B80;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8015A530(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_8004284C();
}

void func_8015A564(void) {
    func_80062CD0(0x6BAB);
    func_8004284C();
}

extern FnTbl20 D_80160BD0;

void func_8015A58C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80160BD0;
    func_8006B900();
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8015A610(void) {
    func_80062CD0(0x6BD8);
    func_8004284C();
}
