#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147908;

void func_8013B920(void) {
    u8 sel = D_801474B8; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_8013B980();
        return;
    case 1:
        func_8013B9FC();
        return;
    default:
        func_8013BA84();
        return;
    }
}

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_801478B0;

void func_8013B980(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_801478B0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013B9FC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147908;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147958;

void func_8013BA84(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147958;
    idx = D_800F647A;
    tbl.f[idx]();
}

void func_8013BB0C(void) {
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
    func_8008F618(*(u8 *)&D_800F62CF = 3);
    func_8013A140();
    D_8014749C = D_80147438;
    D_801474A0 = D_8014745C;
    D_801474A4 = D_80147480;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013B920.rodata", D_80145B80);

void func_8013BBFC(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145B80);
    func_8004DE1C();
}

void func_8013BC40(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    D_801474A8 = 8;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "美術室");
    func_8004DE1C();
}

void func_8013BCB4(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    D_801474A8 = 0xB;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, "音楽室");
    func_8004DE1C();
}

void func_8013BD28(void) {
    func_8008A0D4(0x4011);
    func_8004DE1C();
}
