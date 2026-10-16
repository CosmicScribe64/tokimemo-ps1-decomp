#define MAIN_API_OVERRIDE_func_80083440 /* T-9570: old-style caller preserves the full argument register. */
void func_80083440();
#include "common.h"
#include "ovl/SHOUGATU.h"

void func_80137BB0(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80137C28();
        return;
    case 1:
        func_80137C28();
        return;
    case 2:
        func_80137C28();
        return;
    default:
        func_80137D68();
        return;
    }
}

void func_80137C28(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_80144E20;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80137C9C(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x500);
    func_8004284C();
}

void func_80137CD4(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x507);
    func_8004284C();
}

s32 func_80137D0C(void) {
    func_80137AB4();
    if (D_800E6280.unk_1104.w++ == 1) {
        if (D_80144E0C == 3) {
            func_80083440(3);
        }
    }
}

void func_80137D68(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl34 tbl;

    tbl = D_80144E80;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80137DE4(void) {
    func_80137AB4();
    if ((D_80144E0C == 3) || (D_80144E0C == 11)) {
        /* FAKE: dead selector test retains the target promotion. T-9570 */
        if ((D_80144E0C && D_80144E0C) != 0) { }
        func_80083440(3);
    } else if ((D_80144E0C == 5) || (D_80144E0C == 13)) {
        func_80083440(func_80051B48(8));
    }
    /* FAKE: zero increment promotes the selector into v1. T-9570 */
    D_80144E0C += 0;
}

void func_80137E54(void) {
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
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    *(u8 *)&D_800E6280.unk_75D = *(u8 *)&D_800E6280.unk_F5F = 8;
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    if ((u8) D_80144E14 < 3U) {
        func_800AE0F0(D_800CA188, "女生徒");
    }
    func_80136F90();
    D_80144DFC = D_80144D90;
    D_80144E00 = D_80144DB4;
    D_80144E04 = D_80144DD8;
    func_8004284C();
}

void func_80137F70(void) {
    D_80144E08 = D_80144E14 * 3;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "廊下");
    func_8004284C();
}

void func_80137FC4(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}
