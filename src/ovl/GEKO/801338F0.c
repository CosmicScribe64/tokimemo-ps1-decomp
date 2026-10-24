#define MAIN_API_OVERRIDE_strcpy /* FAKE: T-25002 implicit-int strcpy view (s32 return) keeps the original return register allocation in func_80133C50. */
#include "common.h"
#include "ovl/GEKO.h"

extern s32 strcpy();

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

void func_80133C50(void) {
    s32 v;
    u32 g;
    GekoNames12 names;
    u32 temp;

    names = D_80144D8C;
    strcpy(&D_800CA280, names.name[D_800E6280.unk_0F4.h >> 12]);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b0 = 1;
    if (D_80144C50 == 10) {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.f.b6 = 1;
    }
    D_80144C3C = 0x16;
    v = get_g_zyotai_s(D_800E6280.unk_F5F);
    /* FAKE: T-25002 reading D_80144C50 through the word local g keeps bne t8,a0 operand order */
    if ((g = D_80144C50) == 2U) {
        g = D_800E6280.unk_F5F;
        D_80144C50 = 0;
        if (g == 0) {
            /* FAKE: T-11910 the volatile club view keeps this read before the record load (uopt otherwise sinks it to
             * the first compare), giving the original ugen order; also used in the 1/2/3 branch. */
            v = D_800E6280.unk_0F4.vh >> 12;
            temp = D_800E6280.unk_1BC[g].unk_0C.b[2] >> 4;
            g = temp;
            if (v == g && (u32)v < 6) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6 && g < 6) {
                func_80132A4C(3);
            } else if ((u32)v < 6 && g >= 6) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 1 || g * 1 == 2 || g == 3 /* FAKE: T-25002 g * 1 puts the shared constant 2 first in the beq */) {
            v = D_800E6280.unk_0F4.vh >> 12;
            g = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[2] >> 4;
            if (v == g) {
                func_80132A4C(2);
            } else if (v != g && (u32)v < 6) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 4 || g == 5 || g == 10) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(3);
            } else if ((u8)(v & 0x7F) < 2) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if (g == 6 || g == 7 || g == 8 || g == 9) {
            if ((D_800E6280.unk_0F4.h >> 12) < 6) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    func_8004284C();
}

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
