#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_80146ACC;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146B64;

void func_8013C980(void) {
    D_80146AA0 = 0x801CE218;
    D_80146AA4 = 0x801CE21C;
    D_80146AA8 = 0x801CE25C;
    D_80146AAC = *(s16 *)0x801CE27C;
    D_80146AB0 = 0x801B0000;
    D_80146AB4 = 0x801B2000;
    D_80146AB8 = 0x801B6000;
    D_80146ABC = 0x801BA000;
    D_80146AC0 = 0x801BE000;
    D_80146AC4 = 0x801C2000;
    D_80146AC8 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CA30);

void func_8013CA94(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013CAD0();
        return;
    }
    func_80046500();
}

void func_8013CAD0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_80146ACC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013CB58(void) {
    func_80046318(0x3D, 0x801B0000, 0x8440);
    func_8013C980();
    func_8004284C();
}

void func_8013CB90(void) {
    D_800B5A60 = 1;
    D_800E6280.unk_F5F = 0xE;
    func_800AE0F0(D_800CA188, "男子１");
    D_80120666 = 4;
    func_8004284C();
}

void func_8013CBE4(void) {
    func_800AE0F0(D_800CA188, "男子２");
    D_80120666 = 5;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CC20);

void func_8013CC60(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013CC9C();
        return;
    }
    func_80046500();
}

void func_8013CC9C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146B64;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CD18);

void func_8013CE98(void) {
    func_8013C980();
    func_80043914(D_80146AB0, 0x11, 1, 2, 0);
    func_80084E90(D_80146AB4, D_80146AB8, D_80146ABC, D_80146AC0, D_80146AC4, D_80146AC8);
    func_800850D4(D_80146AA4, D_80146AA8, D_80146AA0, D_80146AAC);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013C980", func_8013CF40);

void func_8013D06C(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013D094(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013D0BC(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}
