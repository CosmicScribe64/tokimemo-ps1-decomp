#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013BA00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80145A80;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

/* bulk_m2c declarations (T-11190) */
extern u32 D_800E7384;
extern u8 D_800E71DF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
s32 func_8013BA7C(void) {
    s16 var_v1;
    u8 temp_a0;
    u8 var_v0;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (D_800E738D + 1) & 0xFF;
    }
    D_800E738D = var_v0;
    if (var_v0 == 1) {
        if ((u32) D_800E7384++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E738D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E738D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    var_v1 = 0;
    do {
        temp_a0 = D_800E6280.unk_1BC[var_v1].unk_0C.b[1];
        if (temp_a0 & 1) {
            D_800E6280.unk_1BC[var_v1].unk_0C.b[1] = temp_a0 & 0xFFFE;
            if (var_v1 != D_800E71DF) {
                D_800E6280.unk_1BC[var_v1].unk_06 += 1;
            }
        }
        var_v1 += 1;
    } while (var_v1 < 0xB);
    D_800E6280.unk_1BC[D_800E71DF].unk_02 += 8;
    D_800E6280.unk_1BC[D_800E71DF].unk_06 += 8;
    D_800E6280.unk_1BC[D_800E71DF].unk_0A -= 0xA;
    check_para_limit(temp_a0, &D_800E6280, 0x38);
    return func_80072B5C(1);
}

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
