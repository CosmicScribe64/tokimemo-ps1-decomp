#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80147D00;

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80147D64;

void func_8013CDD0(void) {
    if (D_801474B8 == 0) {
        func_8013CE0C();
        return;
    }
    func_8013CEB0();
}

void func_8013CE0C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80147D00;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013CE88(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

void func_8013CEB0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80147D64;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013CF2C(void) {
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9120 */
    func_8008F618(*(u8 *)&D_800F62CF = 5);
    func_8013A140();
    D_8014749C = D_80147444;
    D_801474A0 = D_80147468;
    D_801474A4 = D_8014748C;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013CDD0.rodata", D_80145BE0);

void func_8013D01C(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BE0);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013CDD0.rodata", D_80145BEC);

void func_8013D060(void) {
    D_801474A8 = 5;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BEC);
    func_8004DE1C();
}

void func_8013D0A8(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void func_8013D0D0(void) {
    if (D_800F62CF == 5) {
        func_80072734(0x5F0E);
    } else {
        func_80072734(0x5482);
    }
    func_8004DE1C();
}
