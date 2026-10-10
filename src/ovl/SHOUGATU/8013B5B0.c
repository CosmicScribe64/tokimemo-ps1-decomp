#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80145A14;

void func_8013B5B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80145A14;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013B62C(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    func_8004284C();
}

void func_8013B694(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013B71C(void) {
    func_80046318(9, 0x80197000, 0xAFB3);
    func_8013B210();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B5B0", func_8013B758);

void func_8013B8CC(void) {
    func_80085B3C(7, 0);
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013B900(void) {
    D_80145A08 += ((u8) D_800E6280.unk_03E >= 0x61U) * 4;
    func_8004284C();
}

void func_8013B944(void) {
    if (D_80145A10 == 0) {
        D_80145A08 += 1;
    }
    func_8004284C();
}

void func_8013B984(void) {
    if (D_80145A10 != 0) {
        func_8004284C();
        return;
    }
    func_8013B528();
}

void func_8013B9C0(void) {
    if (D_80145A10 != 0) {
        func_80042808();
        return;
    }
    func_8004284C();
}
