#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013A500(void) {
    if (D_80144E14 == 0) {
        func_8013A53C();
        return;
    }
    func_8013A600();
}

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

void func_8013A688(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_80122CE8 = 0;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    *(u8 *)&D_800E6280.unk_F5F = 6;
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    func_80136F90();
    D_80144DFC = D_80144DA8;
    D_80144E00 = D_80144DCC;
    D_80144E04 = D_80144DF0;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b2 = 1;
    func_8004284C();
}

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
