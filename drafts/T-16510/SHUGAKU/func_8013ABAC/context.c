#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_8013CC20;

void func_8013AA70(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E6672;
extern u32 D_800E7384;
extern u8 D_800E71DF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_8013ABAC(u8 arg0) {
    u8 var_v0;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        dec_bg_reset();
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
        return;
    }
block_8:
    func_80072338();
    if (D_800CA2EC != 0) {
        D_800E6672 += 1;
    } else {
        D_800E6280.unk_1BC[D_800E71DF].unk_02 += 1;
        D_800E6280.unk_1BC[D_800E71DF].unk_06 += 1;
    }
    check_para_limit();
    func_80072B5C(1);
}
