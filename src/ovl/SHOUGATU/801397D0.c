#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
extern FnTbl37 D_8014554C;

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80145498;

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_801453F8;

void func_801397D0(void) {
    D_801453B0 = (u8 *)0x801E60CC;
    D_801453B4 = (u8 *)0x801E60D0;
    D_801453B8 = (u8 *)0x801E60E4;
    D_801453BC = *(s16 *)0x801E60F0;
    D_801453C0 = (u8 *)0x801AE000;
    D_801453C4 = (u8 *)0x801B0000;
    D_801453C8 = (u8 *)0x801B2000;
    D_801453CC = (u8 *)0x801B6000;
    D_801453D0 = (u8 *)0x801BA000;
    D_801453D4 = (u8 *)0x801BE000;
    D_801453D8 = (u8 *)0x801C2000;
    D_801453DC = (u8 *)0x801C6000;
    D_801453E0 = (u8 *)0x801CE000;
    D_801453E4 = (u8 *)0x801D2000;
    D_801453E8 = (u8 *)0x801D6000;
    D_801453EC = (u8 *)0x801DA000;
    D_801453F0 = (u8 *)0x801DE000;
    D_801453F4 = (u8 *)0x801E2000;
}

void func_801398F0(void) {
    u8 sel = D_80144E14; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80139980();
        return;
    case 1:
        func_80139A08();
        return;
    case 2:
        func_80139A90();
        return;
    case 3:
        func_80139B0C();
        return;
    default:
        func_80139B94();
        return;
    }
}

void func_80139980(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801453F8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

extern FnTbl20 D_80145448;

void func_80139A08(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145448;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80139A90(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80145498;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

extern FnTbl20 D_801454FC;

void func_80139B0C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_801454FC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80139B94(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_8014554C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

s32 func_80139C10(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        if (D_800E6280.unk_1104.w++ == 1) {
            func_80044750(0x501);
        }
    }
}

void func_80139C68(void) {
    D_800E6280.unk_F5F = 1;
    func_80137AB4();
}

void func_80139C90(void) {
    cd_read_sectors(0x71, 0x801AE000, 0x8768);
    func_801397D0();
    func_8004284C();
}

void func_80139CCC(void) {
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_80122CF8 = 0;
    D_80122CF4 = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_80084E4C();
    /* FAKE: the store inside the call shares $a0 with the argument (T-9020, T-9170) */
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F = 1);
    func_80136F90();
    D_80144DFC = D_80144DA0;
    /* three words of a 0x24-byte record array reached through one base: as1 keeps the loads in order */
    D_80144E00 = (&D_80144DA0)[9];
    D_80144E04 = (&D_80144DA0)[18];
    func_8004284C();
}

void func_80139D80(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139DC4(void) {
    D_80144E08 = 3;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139E0C(void) {
    D_80144E08 = 6;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139E54(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 0xA;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "図書室");
    func_8004284C();
}

void func_80139EC8(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b2 = 1;
    D_80144E08 = 0xD;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "演劇部");
    func_801397D0();
    if (((u8)D_800E6280.unk_03F >= 6U) && ((u8)D_800E6280.unk_03F < 0xAU)) {
        func_80043914((s32)D_801453C0, 0x11, 1, 2, 0);
        func_80084E90((s32)D_801453C8, (s32)D_801453CC, (s32)D_801453D0, (s32)D_801453D4, (s32)D_801453D8, (s32)D_801453DC);
        func_800850D4((s32)D_801453B4, (s32)D_801453B8, (s32)D_801453B0, (s32)D_801453BC);
    } else {
        func_80043914((s32)D_801453C4, 0x11, 1, 2, 0);
        func_80084E90((s32)D_801453E0, (s32)D_801453E4, (s32)D_801453E8, (s32)D_801453EC, (s32)D_801453F0, (s32)D_801453F4);
        func_800850D4((s32)D_801453B4, (s32)D_801453B8, (s32)D_801453B0, (s32)D_801453BC);
    }
    D_80120688 |= 0x80000000;
    D_80120666 = 0;
    D_80122D0C = 1;
    D_800E6280.unk_F5F = 0xE;
    func_8004284C();
}

void func_8013A098(void) {
    bg_read_sub2(0x40BC);
    func_8004284C();
}

void func_8013A0C0(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013A0E8(void) {
    bg_read_sub2(0x40BC);
    func_8004284C();
}

void func_8013A110(void) {
    bg_read_sub2(0x40B2);
    func_8004284C();
}

void func_8013A138(void) {
    bg_read_sub2(0x46C4);
    func_8004284C();
}

void func_8013A160(void) {
    func_80062CD0(0x5D6E);
    func_8004284C();
}
