#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80143D24;

void func_80134120(void) {
    void (**p)();
    FnTbl25 tbl;

    tbl = D_80143D24;
    if (D_80143B20 != 0) {
        p = &tbl.f[D_800E6280.unk_110A];
        if (*p == normal_date_girl_in || tbl.f[D_800E6280.unk_110A] == normal_date_girl_out) {
            tbl.f[D_800E6280.unk_110A] = func_8004284C;
            /* FAKE: empty double test (decomp-permuter, score 0); it only shifts the register choice of base, index and pointer to the original's; real source unknown. T-4090 */
            if (!D_800E6280.unk_110A && !D_800E6280.unk_110A) {
            }
        }
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801341E4);

void func_8013428C(void) {
    func_801323D4();
    if ((D_80143B04 == 2) && (D_800E6280.unk_110D == 0) && (D_8011ED5B & 0x80)) {
        func_80044750(0x202);
        D_800E6280.unk_110D += 1;
    }
    if ((D_800E6280.unk_110D == 0) && (D_80143B04 == 3)) {
        func_80044750(0x202);
        D_800E6280.unk_110D += 1;
    }
}

void func_80134340(void) {
    D_800E6280.unk_75D = D_800E6280.unk_F5F;
    func_8004284C();
}

void func_8013436C(void) {
    if ((D_80143B20 == 0) && !(D_800E6280.unk_10F8 & 1) && (D_800E6280.unk_75E[D_800E6280.unk_F5E - 1].unk_02 != 5)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 4);
        } else {
            func_80085B3C(5, 1);
        }
    }
    bg_read_sub2(0x41F6);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134400);

void func_80134554(void) {
    u8 sel = D_80143D20; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        break;
    case 0:
        D_80143B00 += 3;
        break;
    case 1:
        D_80143B00 += 2;
        break;
    }
    func_8004284C();
}

void func_801345C8(void) {
    s32 v;

    if (D_80143B20 == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)func_80051A68(D_800E6280.unk_F5F) & 0x7F;
        if ((v != 0) && (v != 1)) {
            if (v != 2) {
                D_80143B00 += 2;
            } else {
                D_80143B00 += 1;
            }
        }
    }
    func_8004284C();
}

void func_80134648(void) {
    s32 v;

    if (D_80143B20 == 0) {
        /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
        v = (u8)func_80051A68(D_800E6280.unk_F5F) & 0x7F;
        switch (v) {
        case 0:
        case 1:
            D_80143B00 += 2;
            break;
        case 2:
            D_80143B00 += 1;
            break;
        }
    } else {
        D_80143B00 += 2;
    }
    func_8004284C();
}

void func_801346E8(void) {
    if (D_80122CDC != 2) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134730);

void func_801347B4(void) {
    D_80122CEC = D_80122CDC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801347E0);
