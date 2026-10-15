#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80148114;

void func_8013E1D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80148114;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_8013E24C(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void func_8013E290(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void func_8013E2CC(void) {
    func_80051DD8(9, 0x80197000, 0xA46B);
    func_8013DE30();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013E1D0", func_8013E308);

void func_8013E47C(void) {
    func_80090960(7, 0);
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013E4B0(void) {
    D_80148108 += ((u8) D_800F53DE >= 0x61U) * 4;
    func_8004DE1C();
}

void func_8013E4F4(void) {
    if (D_80148110 == 0) {
        D_80148108 += 1;
    }
    func_8004DE1C();
}

void func_8013E534(void) {
    if (D_80148110 != 0) {
        func_8004DE1C();
        return;
    }
    func_8013E148();
}

void func_8013E570(void) {
    if (D_80148110 != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}
