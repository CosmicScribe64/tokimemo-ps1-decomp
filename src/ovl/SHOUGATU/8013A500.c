#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_8014570C;

void func_8013A500(void) {
    if (D_80144E14 == 0) {
        func_8013A53C();
        return;
    }
    func_8013A600();
}

typedef struct {
    void (*f[27])();
} FnTbl27; /* size 0x6C */
extern FnTbl27 D_801456A0;

void func_8013A53C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl27 tbl;

    tbl = D_801456A0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013A5B0(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8013A5D8(void) {
    func_80044750(0x503);
    func_8004284C();
}

void func_8013A600(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_8014570C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A500", func_8013A688);

void func_8013A7B0(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "近所の公園");
    func_8004284C();
}

void func_8013A7F4(void) {
    D_80144E08 = 6;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "プール");
    func_8004284C();
}

void func_8013A83C(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}

void func_8013A864(void) {
    bg_read_sub2(0x40A0);
    func_8004284C();
}

void func_8013A88C(void) {
    func_80062CD0(0x6449);
    func_8004284C();
}

void func_8013A8B4(void) {
    func_80062CD0(0x625A);
    func_8004284C();
}

void func_8013A8DC(void) {
    if (((u32) D_800E6280.unk_0F4.h >> 0xC) == 9) {
        D_80144E08 = 5;
    }
    func_8004284C();
}

void func_8013A91C(void) {
    D_80144E08 = 3;
    func_8004284C();
}
