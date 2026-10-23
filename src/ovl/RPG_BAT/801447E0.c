/* FAKE: T-25138, D_8015EBCC seen as one block up to the battle counters D_8015EBFC (header: s32[3]). In
 * func_80144C84 the indexed flag read must alias EBE0 and the counters (uopt reloads them after it) and as1
 * keeps the EBE0 load before the counter writebacks only when both share one symbol. The real aggregate's base
 * and extent are not recovered. */
#define RPG_BAT_OVERRIDE_D_8015EBCC /* RpgBattleView below, a partial view of the battle block (T-25138) */
#include "common.h"
#include "ovl/RPG_BAT.h"

typedef struct RpgBattleView {
    /* 0x00 */ s32 flag[3]; /* D_8015EBCC..D_8015EBD4 (RpgGauge.flag) */
    /* 0x0C */ s32 unk_0C;  /* D_8015EBD8 */
    /* 0x10 */ s32 unk_10;  /* D_8015EBDC */
    /* 0x14 */ s32 unk_14;  /* D_8015EBE0 */
    /* 0x18 */ u8 pad[0x18]; /* D_8015EBE4..D_8015EBFB, RpgPadSeq D_8015EBEC among them */
    /* 0x30 */ RpgStat stat; /* D_8015EBFC */
} RpgBattleView; /* size 0x6C */
extern RpgBattleView D_8015EBCC;

void func_801447E0(void) {
    /* T-24522: the result global carries the intermediate arithmetic range. */
    s32 k;
    s32 lim;

    if (D_800E6280.unk_118.unk_02 >= 0x97) {
        lim = 0x96;
    } else {
        lim = D_800E6280.unk_118.unk_02;
    }
    if (D_8015EB98 & 0x80000000) {
        switch (D_8015EDC4 & 0xFFFF) {
        case 1:
        default:
            D_8015EBB0 = func_8013EA00(D_8015EDA0 / 10, D_8015EDA0 * 5 / 100) + 0x64;
            break;
        case 2:
            k = D_8015EDA0 / 10;
            D_8015EBB0 = func_8013EA00(k, k) + 0x96;
            break;
        }
        D_8015EBB0 *= 0xE1 - lim;
        D_8015EBB0 /= 150;
        D_8015EBB0 = D_8015EBD8 * D_8015EBB0;
        D_8015EBB0 /= 1000;
        if (D_8015ED98 & 0x100) {
            D_8015EBD8 += 0x64;
            D_8015EBB0 = D_8015EBB0 / 2;
        }
    }
    if (D_8015EB98 & 0x100000) {
        k = 0xE1 - lim;
        switch (D_8015EDC4 & 0xFFFF) {
        case 2:
        default:
            D_8015EBB0 = func_8013E9A8(D_8015EDA0 * 0x19 / 100 - 0x1F4) + 0x1F4;
            if (D_8015ED98 & 0x100) {
                D_8015EBB0 = D_8015EBB0 / 3;
            }
            break;
        case 4:
            D_8015EBB0 = func_8013E9A8(D_8015EDA0 * 0x32 / 100 - 0x3E8) + 0x3E8;
            if (D_8015ED98 & 0x100) {
                D_8015EBB0 = D_8015EBB0 / 10;
            }
            break;
        }
        D_8015EBB0 *= k;
        D_8015EBB0 /= 150;
        D_8015EBB0 = D_8015EBD8 * D_8015EBB0;
        D_8015EBB0 /= 1000;
    }
    if (D_8015EB98 & 0xF0000) {
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015ED94 / 100;
        } else {
            D_8015EBB0 = D_8015ED94 / 10;
        }
    }
    if (D_8015EB98 & 0x08000000) {
        if (D_8015ED98 & 0x1000) {
            D_8015EBB0 = D_8015ED94 / 2;
            return;
        }
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015ED94 / 50;
        } else {
            D_8015EBB0 = D_8015ED94 / 5;
        }
        if (D_8015ED94 < 0x14) {
            D_8015EBB0 = 0;
        }
    }
}

s32 func_80144B7C(void) {
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x10000:
    case 0x20000:
    case 0x40000:
        return func_80144C54();
    case 0x100000:
        return func_80144C84();
    case 0x200000:
        return func_80145180();
    case 0x400000:
        return func_801452C0();
    case 0x8000000:
        D_8015EBFC.unk_38 += 2;
        return 1;
    case 0x80000000:
        return func_801453B4();
    }
}

s32 func_80144C54(void) {
    if (++D_8015EDD4 < 3) {
        return 1;
    }
    return 2;
}

s32 func_80144C84(void) {
    s32 sel;
    s32 total;
    s32 lim;
    s32 roll;

    if (D_800E6280.unk_118.unk_02 >= 0x97) {
        lim = 0x96;
    } else {
        lim = D_800E6280.unk_118.unk_02;
    }
    sel = func_8013E9A8(3);
    switch (sel) {
    case 0:
        D_8015EBAC = 0;
        break;
    case 1:
        D_8015EBAC = 1;
        break;
    case 2:
        D_8015EBAC = 2;
        break;
    }
    if (func_801447B8()) {
        if (D_8015EBCC.stat.unk_0C + D_8015EBCC.stat.unk_00 + D_8015EBCC.stat.unk_04 + D_8015EBCC.stat.unk_08 < 0x15) {
            D_8015EBCC.stat.unk_00 = 0xF;
            D_8015EBCC.stat.unk_04 = 0xF;
            D_8015EBCC.stat.unk_08 = 5;
            D_8015EBCC.stat.unk_0C = 0x1E;
        }
    } else {
        if (D_8015EBCC.stat.unk_0C + D_8015EBCC.stat.unk_00 + D_8015EBCC.stat.unk_04 + D_8015EBCC.stat.unk_08 < 0x15) {
            D_8015EBCC.stat.unk_00 = 0xA;
            D_8015EBCC.stat.unk_04 = 0xA;
            D_8015EBCC.stat.unk_08 = 0;
            D_8015EBCC.stat.unk_0C = 0x28;
        }
    }
    total = D_8015EBCC.unk_14 + D_8015EBCC.stat.unk_00 + D_8015EBCC.stat.unk_04 + D_8015EBCC.stat.unk_08 + D_8015EBCC.stat.unk_0C;
    roll = func_8013E9A8(total);
    if (roll < D_8015EBCC.stat.unk_00) {
        D_8015EBCC.stat.unk_00 -= 5;
        return 1;
    }
    if (roll < D_8015EBCC.stat.unk_04 + D_8015EBCC.stat.unk_00) {
        /* FAKE: T-24522, comma return keeps the decrement on the return line (as1 schedules per line). */
        return (D_8015EBCC.stat.unk_04 -= 5, 0);
    }
    if (roll < D_8015EBCC.unk_14 + D_8015EBCC.stat.unk_00 + D_8015EBCC.stat.unk_04 + D_8015EBCC.stat.unk_08) {
        if (D_8015EBCC.flag[D_8015EBAC] == 1) {
            D_8015EBB0 = func_8013EA00(0x64, 0x64);
            return 4;
        }
        D_8015EBCC.stat.unk_08 -= 5;
        D_8015EBCC.unk_14 -= 0x14;
        if (D_8015EBCC.stat.unk_08 < 0) {
            D_8015EBCC.stat.unk_08 = 0;
        }
        if (D_8015EBCC.unk_14 < 0) {
            D_8015EBCC.unk_14 = 0;
        }
        return 3;
    }
    D_8015EBCC.stat.unk_0C -= 5;
    if (func_801447B8() == 0) {
        D_8015EBCC.unk_0C += 0x19;
    }
    if (D_8015EBCC.flag[D_8015EBAC] == 1) {
        D_8015EBB0 = func_8013EA00(0x64, 0x64);
        return 4;
    }
    if (D_8015EDF0 != 0x40000000 && func_8013E9A8(0x64) < 5) {
        D_8015EBB0 = D_8015EDEC / 10;
        if (D_8015ED98 & 0x100) {
            D_8015EBB0 = D_8015EBB0 / 10;
        }
        D_8015EBCC.stat.unk_38 += 2;
        return 6;
    }
    if (D_8015ED98 != 0x100) {
        if (func_8013E9A8(0x64) < 5) {
            D_8015EBB0 = func_8013EA00(0xA, 0xA);
            D_8015EBB0 = D_8015EBCC.unk_0C * D_8015EBB0;
            D_8015EBB0 /= 1000;
            D_8015EBCC.stat.unk_38 += 1;
            return 5;
        }
    }
    D_8015EBB0 = func_8013EA00(0x7D, 0x4B);
    D_8015EBB0 *= 0xE1 - lim;
    D_8015EBB0 /= 150;
    D_8015EBB0 = D_8015EBCC.unk_0C * D_8015EBB0;
    D_8015EBB0 /= 1000;
    if (D_8015ED98 & 0x100) {
        D_8015EBB0 = D_8015EBB0 / 10;
    }
    D_8015EBCC.stat.unk_38 += 1;
    return 2;
}

s32 func_80145180(void) {
    s32 total;
    s32 roll;

    if (D_8015EDC0 < D_8015EDCC / 3) {
        D_8015EBFC.unk_08 = 0xF;
        D_8015EBFC.unk_04 = 5;
        D_8015EBFC.unk_00 = 0;
        return 3;
    }
    total = D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04;
    if (total < 0xB) {
        D_8015EBFC.unk_00 = 0x14;
        D_8015EBFC.unk_08 = 0x14;
        D_8015EBFC.unk_04 = 0xF;
        total = 0x37;
    }
    roll = func_8013E9A8(total);
    if (roll < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 0;
    }
    if (roll < D_8015EBFC.unk_04 + D_8015EBFC.unk_00) {
        D_8015EBFC.unk_04 -= 5;
        return 1;
    }
    D_8015EBFC.unk_08 -= 5;
    D_8015EBFC.unk_38 += 2;
    return 2;
}

s32 func_801452C0(void) {
    s32 total;
    s32 roll;

    total = D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04;
    if (total < 0xB) {
        D_8015EBFC.unk_08 = 0xF;
        D_8015EBFC.unk_00 = 0x19;
        D_8015EBFC.unk_04 = 0xF;
        total = 0x37;
    }
    roll = func_8013E9A8(total);
    if (roll < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 0;
    }
    if (roll < D_8015EBFC.unk_04 + D_8015EBFC.unk_00) {
        D_8015EBFC.unk_04 -= 5;
        return 1;
    }
    D_8015EBFC.unk_08 -= 5;
    D_8015EBFC.unk_38 += 2;
    return 4;
}

/* FAKE: the word view at +0x0C fills an unrecovered RpgStat field; the
 * comma return preserves the original decrement scheduling. T-10010. */
s32 func_801453B4(void) {
    s32 roll;
    s32 total;

    total = D_8015EBFC.unk_0C + D_8015EBFC.unk_00 + D_8015EBFC.unk_04 + D_8015EBFC.unk_08;
    if (total < 6) {
        D_8015EBFC.unk_0C = 0xA;
        D_8015EBFC.unk_04 = 5;
        D_8015EBFC.unk_08 = 0xA;
        D_8015EBFC.unk_00 = 0xF;
        total = 0x28;
    }
    roll = func_8013E9A8(total);
    if (roll < D_8015EBFC.unk_00) {
        D_8015EBFC.unk_00 -= 5;
        return 0;
    }
    if (roll < (D_8015EBFC.unk_04 + D_8015EBFC.unk_00)) {
        return (D_8015EBFC.unk_04 -= 5, 4);
    }
    if (roll < (D_8015EBFC.unk_08 + D_8015EBFC.unk_00 + D_8015EBFC.unk_04)) {
        D_8015EBFC.unk_08 -= 5;
        return 1;
    }
    D_8015EBFC.unk_0C -= 5;
    return 2;
}
