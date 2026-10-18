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
    u32 cur;

    cur = D_80144C50;
    if (cur == 0x64) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

void func_801339AC(void) {
    u32 cur;
    u16 raw;

    cur = D_80144C50;
    if (cur == 0x64) {
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

s32 func_80133AA0() {
    /* FAKE: `c50` is a word local above `state` that puts `state` at 0x2B(sp) and avoids cvt_pass widening on D_80144C50 so uopt keeps bne reg,$v1; `| 0` commutes xor $v0,$v1,reg. T-24090 */
    u32 c50;
    u8 state;

    state = get_g_zyotai_h(D_800E6280.unk_F5F);
    func_801322E8();
    if (*(s16 *)&D_80144C40 == 2 && D_800E6280.unk_F5F == 8 && (c50 = D_80144C50) == 1U && (D_800E6280.unk_1104.u++ | 0) == 1U) {
        func_80083440(3U);
    }
    if (*(s16 *)&D_80144C40 == 4 && D_800E6280.unk_F5F == 8 && (c50 = D_80144C50) == 1U && (D_800E6280.unk_1104.u++ | 0) == 1U) {
        func_80083440(state);
    }
    if ((*(s16 *)&D_80144C40 == 1 || *(s16 *)&D_80144C40 == 5) && D_800E6280.unk_F5F == 0xA && (c50 = D_80144C50) == 1U && (D_800E6280.unk_1104.u++ | 0) == 1U) {
        func_80083440(2U);
    }
    if (*(s16 *)&D_80144C40 == 3 && D_800E6280.unk_F5F == 0xA && (c50 = D_80144C50) == 1U && (D_800E6280.unk_1104.u++ | 0) == 1U) {
        func_80083440(3U);
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
