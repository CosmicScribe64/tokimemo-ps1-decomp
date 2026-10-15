#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80144D40;

void func_801338F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_80144D40;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013396C(void) {
    if (D_80144C50 == 0x64) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

void func_801339AC(void) {
    u16 raw;

    if (D_80144C50 == 0x64) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (D_800E6280.unk_F5F == 4) {
        if (D_80144C3C == 0x26) {
            raw = D_800E6280.unk_0F4.h;
            if (((raw >> 0xC) == 6) && ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6)) {
                D_80144C3C = 0x28;
            } else if (((raw >> 0xC) == 7) && ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7)) {
                D_80144C3C = 0x29;
            } else {
                D_80144C3C = 0x2A;
            }
        } else {
            func_8004284C();
        }
    } else {
        func_8004284C();
    }
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7384;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80133AA0(void) {
    u8 sp2B;
    s16 temp_a0;

    sp2B = get_g_zyotai_h(D_800E71DF);
    func_801322E8();
    temp_a0 = D_80144C40;
    if ((temp_a0 == 2) && (D_800E71DF == 8) && (D_80144C50 == 1)) {
        if (D_800E7384++ == 1) {
            func_80083440(3U);
        }
    }
    if ((temp_a0 == 4) && (D_800E71DF == 8)) {
        if (D_80144C50 == 1) {
            if (D_800E7384++ == 1) {
                func_80083440(sp2B);
            }
        }
    }
    if ((temp_a0 == 1) || (temp_a0 == 5)) {
        if (D_800E71DF == 0xA) {
            if (D_80144C50 == 1) {
                if (D_800E7384++ == 1) {
                    func_80083440(2U);
                }
            }
        }
    }
    if ((temp_a0 == 3) && (D_800E71DF == 0xA)) {
        if (D_80144C50 == 1) {
            if (D_800E7384++ == 1) {
                func_80083440(3U);
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801338F0", func_80133C50);

void func_80133F94(void) {
    u32 cur;

    D_80144C3C = (D_80144C3C + D_80144C50) - 1;
    cur = D_80144C50;
    if (cur == 0x64) {
        D_80144C3C = 0x28;
    }
    func_8004284C();
}

void func_80133FE8(void) {
    D_80144C3C = 0x27;
    func_8004284C();
}

void func_80134010(void) {
    bg_read_sub2(0x42D8);
    func_8004284C();
}
