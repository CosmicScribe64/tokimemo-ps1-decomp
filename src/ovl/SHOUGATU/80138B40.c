#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80145170;

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_801451C8;

void func_80138B40(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80138BA0();
        return;
    case 1:
        func_80138C1C();
        return;
    default:
        func_80138CA4();
        return;
    }
}

void func_80138BA0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80145170;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80138C1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801451C8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

extern FnTbl20 D_80145218;

void func_80138CA4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145218;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138D2C);

void func_80138E1C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "中庭");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138E60);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138ED4);

void func_80138F48(void) {
    bg_read_sub2(0x4071);
    func_8004284C();
}

void func_80138F70(void) {
    bg_read_sub2(0x40E2);
    func_8004284C();
}

void func_80138F98(void) {
    bg_read_sub2(0x40D9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80138B40", func_80138FC0);
