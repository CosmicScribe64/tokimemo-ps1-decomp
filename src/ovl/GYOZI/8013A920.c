#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_801474C0;

void func_8013A920(void) {
    u8 sel = D_801474B8; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_8013A998();
        return;
    case 1:
        func_8013AAAC();
        return;
    case 2:
        func_8013AB34();
        return;
    default:
        func_8013ABBC();
        return;
    }
}

void func_8013A998(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_801474C0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013AA20(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AA48);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AAAC);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AB34);

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */
extern FnTbl28 D_801475D4;

void func_8013ABBC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl28 tbl;

    tbl = D_801475D4;
    idx = D_800F647A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013A920", func_8013AC38);

void func_8013AD30(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AD74(void) {
    D_801474A8 = 3;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013ADBC(void) {
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE04(void) {
    D_801474A8 = 9;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void func_8013AE4C(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}
