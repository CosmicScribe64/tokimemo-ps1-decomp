#define MAIN_API_OVERRIDE_func_800847B8 /* Word argument at this call; T-9520. */
#include "common.h"
#include "ovl/SHOUGATU.h"
void func_800847B8();

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

void func_801341E4(void) {
    u16 six;
    u32 result;
    u32 mode;

    /* FAKE: the narrow call cast reserves a temporary; the dead raw-byte
     * test retains separate raw/shifted ranges, and u16 six keeps the branch
     * operand order. T-10010. */
    six = 6;
    if (D_80143B20 == 0) {
        result = (u8) func_80051A68(D_800E6280.unk_F5F) & 0x7F;
        if ((D_800E6280.unk_F5F == 4) && (result >= 2U)) {
            mode = (u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4;
            if (D_800E6280.unk_1BC[4].unk_0C.b[2]) {
            }
            if ((mode == six) && (D_80143B04 == 1)) {
                D_80143B04 = 2;
            } else if ((mode != six) && (D_80143B04 == 0)) {
                D_80143B04 = 1;
            }
        }
    }
    func_801323D4();
}

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

void func_80134400(void) {
    s32 pad; /* FAKE: unused local declared first puts tbl at sp+0x28 as in the original; real source unknown. T-9340 */
    SjisStr3 tbl;

    tbl = D_80143D88;
    D_80143D20 = D_80122CDC;
    D_80143B00 += D_80122CDC;
    func_800AE0F0(D_800CA250, tbl.s[D_80122CDC]);
    if (D_80143B20 == 0 && D_800E6280.unk_75D == 3 && D_80122CDC == 2) {
        func_800AE0F0(D_800CA250, tbl.s[1]);
    }
    if (D_80122CDC == 0) {
        D_800E6280.unk_100.unk_02 += 0xA;
        D_800E6280.unk_104.unk_02 += 0xA;
        D_800E6280.unk_F5F = 0xE;
    } else if (D_80122CDC == 1) {
        D_800E6280.unk_0FC.unk_02 += 0x19;
        D_800E6280.unk_F5F = 0xE;
    }
    func_80084D3C();
    func_8004284C();
}

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

/* FAKE: empty do loops and size ^ 0 preserve the two original multiplications
 * and temporary allocation; keep statement-per-line layout. Source unknown.
 * The word-argument prototype override above is required here. T-9520. */
void func_80134730(void)
{
    u32 size;
    s32 index;
    func_800847B8(D_80122CDC);
    size = 0x38;
    index = D_80122CDC;
    do { } while (0);
    ((GameState *) (((u32) (&D_800E6280)) + (index * size)))->unk_1BC[0].unk_02 += 5;
    ((GameState *) (((u32) (&D_800E6280)) + (index * (size ^ 0))))->unk_1BC[0].unk_06 += 1;
    do { func_80084D3C(size, index); } while (0);
    D_800E6280.unk_F5F = 0xE;
    func_8004284C();
}

void func_801347B4(void) {
    D_80122CEC = D_80122CDC;
    func_8004284C();
}

/* NON_MATCHING: T-11310, 3 rows: the unk_F5F reload after get_g_zyotai_s goes to ugen temp $t7, the original allocates $a0. */
#ifdef NON_MATCHING
void func_801347E0(void) {
    u32 state;
    s32 girl;

    girl = D_800E6280.unk_75D;
    D_800E6280.unk_F5F = girl;
    func_800847B8(girl);
    D_80122CDC = D_80122CEC;
    state = get_g_zyotai_s(6) & 0x7F;
    girl = D_800E6280.unk_F5F;
    if ((girl == 6) && (state < 2) && (D_80143B20 == 0)) {
        D_80143AF4 = D_80143A60;
        D_80143AF8 = D_80143A9C;
        D_80143AFC = D_80143AD8;
    }
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801347E0);
#endif
