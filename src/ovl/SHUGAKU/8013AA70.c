#include "common.h"
#include "ovl/SHUGAKU.h"

void func_8013AA70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl25 tbl;

    tbl = D_8013CC20;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013AAEC(void) {
    s32 temp;
    temp = D_800CA2CC;
    switch (temp) {
    case 0:
        func_800AE0F0(D_800CA23C, "京都");
        break;
    case 1:
        func_800AE0F0(D_800CA23C, "沖縄");
        break;
    case 2:
        func_800AE0F0(D_800CA23C, "北海道");
        break;
    }
    D_800CA2DC += D_800CA2F4 * (1 - D_800CA2EC);
    func_8004284C();
}

s32 func_8013ABAC(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800573AC();
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if ((u32)D_800E6280.unk_1104.w++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    func_80072338();
    if (D_800CA2EC != 0) {
        D_800E6280.unk_1BC[10].unk_06 += 1;
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 += 1;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 1;
    }
    func_80084D3C();
    return func_80072B5C(1);
}
