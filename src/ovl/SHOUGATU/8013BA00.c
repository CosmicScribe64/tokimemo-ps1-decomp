#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80145A80;

void func_8013BA00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145A80;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013BA00", func_8013BA7C);

void func_8013BC2C(void) {
    s16 i;

    D_801459FC = D_8014598C;
    D_80145A00 = D_801459C0;
    D_80145A04 = D_801459F4;
    D_80145A08 = 0;
    D_80145A0C = 0;
    for (i = 0; i < 0xB; i++) {
        D_800CA2B0[i] = (D_800E6280.unk_1BC[i].unk_0C.b[1] & 1) >= 1;
    }
    func_8004284C();
}

void func_8013BCE0(void) {
    D_800E6280.unk_F5F = (u8) D_80122CDC;
    func_80085B3C(7, 0);
    func_800847B8(D_800E6280.unk_F5F);
    func_8004284C();
}

void func_8013BD2C(void) {
    u8 s;

    s = func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    if (s >= 2U) {
        D_80145A08 += 2;
    }
    if (s >= 3U) {
        D_80145A08 += 2;
    }
    if (s == 4) {
        D_80145A08 += 2;
    }
    if (D_800E6280.unk_F5F == 9 && s < 2U) {
        func_80083440(0);
    }
    func_8004284C();
}

void func_8013BDE0(void) {

    D_80145A08 = 0;
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_801459FC = D_80145964;
        D_80145A00 = D_80145998;
        D_80145A04 = D_801459CC;
        break;
    case 1:
        D_801459FC = D_80145978;
        D_80145A00 = D_801459AC;
        D_80145A04 = D_801459E0;
        break;
    case 2:
        D_801459FC = D_80145968;
        D_80145A00 = D_8014599C;
        D_80145A04 = D_801459D0;
        break;
    case 3:
        D_801459FC = D_80145970;
        D_80145A00 = D_801459A4;
        D_80145A04 = D_801459D8;
        break;
    case 4:
        D_801459FC = D_80145988;
        D_80145A00 = D_801459BC;
        D_80145A04 = D_801459F0;
        break;
    case 5:
        D_801459FC = D_8014597C;
        D_80145A00 = D_801459B0;
        D_80145A04 = D_801459E4;
        break;
    case 6:
        D_801459FC = D_80145980;
        D_80145A00 = D_801459B4;
        D_80145A04 = D_801459E8;
        break;
    case 7:
        D_801459FC = D_80145974;
        D_80145A00 = D_801459A8;
        D_80145A04 = D_801459DC;
        break;
    case 8:
        D_801459FC = D_80145960;
        D_80145A00 = D_80145994;
        D_80145A04 = D_801459C8;
        break;
    case 9:
        D_801459FC = D_80145984;
        D_80145A00 = D_801459B8;
        D_80145A04 = D_801459EC;
        break;
    case 10:
        D_801459FC = D_80145990;
        D_80145A00 = D_801459C4;
        D_80145A04 = D_801459F8;
        break;
    }
    func_8004284C();
}
