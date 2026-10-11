#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80145760;

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80145808;

void func_8013A950(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_8013A9B0();
        return;
    case 1:
        func_8013AA24();
        return;
    default:
        func_8013AAAC();
        return;
    }
}

void func_8013A9B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80145760;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013AA24(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145808;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

extern FnTbl20 D_80145858;

void func_8013AAAC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145858;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013AB34(void) {
    func_80062CD0(0x66EC);
    func_8004284C();
}

void func_8013AB5C(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9120 */
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F = 4);
    func_80136F90();
    D_80144DFC = D_80144DAC;
    D_80144E00 = D_80144DD0;
    D_80144E04 = D_80144DF4;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b2 = 1;
    func_8004284C();
}

void func_8013AC7C(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

void func_8013ACC0(void) {
    D_80144E08 = 0xC;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013AD08(void) {
    D_80144E08 = 0xF;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "グランド");
    func_8004284C();
}

void func_8013AD50(void) {
    if (((u8) D_800E6280.unk_03F >= 6U) && ((u8) D_800E6280.unk_03F < 0xAU)) {
        bg_read_sub2(0x404D);
    } else {
        bg_read_sub2(0x4055);
    }
    func_8004284C();
}

void func_8013ADA4(void) {
    bg_read_sub2(0x4097);
    func_8004284C();
}

void func_8013ADCC(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7) {
        D_80144E08 = 6;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013A950", func_8013AE0C);

void func_8013AE6C(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) != ((u32) D_800E6280.unk_0F4.h >> 0xC)) {
        D_80144E08 = 4;
    }
    func_8004284C();
}

void func_8013AEB4(void) {
    if (((u32) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == ((u32) D_800E6280.unk_0F4.h >> 0xC)) {
        D_800E6280.unk_110A += 0xD;
        return;
    }
    func_8004284C();
}
