#include "common.h"
#include "ovl/SHOUGATU.h"

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

void func_80138CA4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145218;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80138D2C(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-10520 */
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F = 3);
    func_80136F90();
    D_80144DFC = D_80144D98;
    D_80144E00 = D_80144DBC;
    D_80144E04 = D_80144DE0;
    func_8004284C();
}

void func_80138E1C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "中庭");
    func_8004284C();
}

void func_80138E60(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 8;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "美術室");
    func_8004284C();
}

void func_80138ED4(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 0xB;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "音楽室");
    func_8004284C();
}

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

void func_80138FC0(void) {
    s32 isA; /* high nibble of GameState.unk_0F4 is 4 */
    s32 isB; /* high nibble of the Rec38 flag byte +2 of record 3 is 4 */

    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b2 = 1;
    if ((D_800E6280.unk_0F4.h >> 0xC) == 4) {
        isA = 1;
    } else {
        isA = 0;
    }
    if ((D_800E6280.unk_1BC[3].unk_0C.b[2] >> 4) == 4) {
        isB = 1;
    } else {
        isB = 0;
    }
    if ((isA == 0) || (isB == 0)) {
        if (isB != 0) {
            D_80144E08 += 2;
        } else {
            D_80144E08 += 4;
            D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b2 = 0;
        }
    }
    func_8004284C();
}
