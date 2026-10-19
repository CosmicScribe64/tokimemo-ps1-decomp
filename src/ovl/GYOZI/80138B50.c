#define MAIN_API_OVERRIDE_func_800BCE10 /* matched with implicit int return view (main_api.h: void) */
#include "common.h"
#include "ovl/GYOZI.h"

s32 func_800BCE10();

void func_80138B50(void) {
    func_8008A0D4(0x3FED);
    func_80090960(6, 0);
    func_8004DE1C();
}

void func_80138B84(void) {
    func_8008A0D4(0x40C1);
    func_8004DE1C();
}

void func_80138BAC(void) {
    D_80147198 = (D_80147198 + (D_800F53DE * 3)) - 0x120;
    func_8004DE1C();
}

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80147248;

void func_80138BF0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_80147248;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80138C78(void) {
    D_800F62CF = 0;
    func_8004DE1C();
}

#ifdef NON_MATCHING
/* NON_MATCHING: CharFlags b1/b8 and s16 r before sp4C[13] match the 0x68 frame, sp+0x4C offset, and bit-field accesses; remaining diffs are $s3/$s4 swap (sp4C vs 1) and uopt zcopy:1 copy-propagating r into the D_80147224 loop preheader and hoisting the second "%d " into $s2. T-24116. */
void func_80138C9C(void) {
    s16 r;
    s16 sp4C[13];
    s16 i;
    s16 count;

    D_80147220 = 0;
    D_80147244 = 0;
    D_8014723C = 0;
    D_80147240 = 0;
    count = 0;
    if (((CharFlags *)D_800F53A0.girl[9].unk_10)->b1 == 0 &&
        D_800F53A0.girl[9].unk_06 >= 20) {
        D_80147240 = 1;
    }
    for (i = 0; i < 11; i++) {
        if ((((CharFlags *)D_800F53A0.girl[i].unk_10)->b1 != 0 || (i == 9 && D_80147240 == 1)) &&
            (func_8005F220(i) >= 0x32U || (D_80147240 == 1 && i == 9)) &&
            (D_80147240 == 0 || i != 0 || func_8005EBE0(0) >= 0x46U)) {
            sp4C[count] = i;
            D_8014723C += 1;
            count += 1;
            ((CharFlags *)D_800F53A0.girl[i].unk_10)->b8 = 1;
        }
    }
    for (i = 0; i < D_8014723C; i++) {
        func_800BCDF0("%d ", sp4C[i]);
    }
    func_800BCDF0("\n");
    r = (u32)D_800F6468 % (u32)D_8014723C;
    for (i = 0; i < D_8014723C; i++) {
        D_80147224[i] = sp4C[(i + r) % D_8014723C];
    }
    for (i = 0; i < D_8014723C; i++) {
        func_800BCDF0("%d ", D_80147224[i]);
    }
    func_800BCDF0("\n");
    for (i = 0; i < D_8014723C; i++) {
        if (D_80147224[i] == 0 && func_8005EBE0(0) >= 0x46U) {
            D_80147224[i] = D_80147224[D_8014723C - 1];
            D_80147224[D_8014723C - 1] = 0;
            break;
        }
        if (D_80147224[i] == 0) {
            D_80147224[i] = D_80147224[0];
            D_80147224[0] = 0;
            break;
        }
        if (D_80147224[i] == 9 && D_80147240 != 0) {
            D_80147224[i] = D_80147224[0];
            D_80147224[0] = 9;
            break;
        }
    }
    func_8004DE1C();
}
#else
INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_80138C9C);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: 7 resolved rows (same jump-table dispatch shift in branch delay slot as VALEN func_80132D0C, T-24033). T-24116. */
s32 func_8013907C(void) {
    s32 pad;
    u8 s;

    if (D_80147220 == D_8014723C) {
        func_8004DDD8();
        return 0;
    }
    func_8008F618(D_800F62CF = D_80147224[D_80147220]);
    s = func_8005E0E0(D_800F62CF) & 0x7F;
    D_80147198 = (func_8005EBE0(D_800F62CF) >= 0x46U) ? 1 : 0;
    if (D_800F62CF == 9 && D_80147240 == 1) {
        D_80147198 = 3;
    }
    if (D_800F62CF == 0 && D_80147240 == 1 && s < 2U) {
        D_80147198 = 3;
    }
    switch (D_800F62CF) {
    case 0:
        D_8014718C = D_801470F8;
        D_80147190 = D_8014712C;
        D_80147194 = D_80147160;
        break;
    case 1:
        D_8014718C = D_8014710C;
        D_80147190 = D_80147140;
        D_80147194 = D_80147174;
        break;
    case 2:
        D_8014718C = D_801470FC;
        D_80147190 = D_80147130;
        D_80147194 = D_80147164;
        break;
    case 3:
        D_8014718C = D_80147104;
        D_80147190 = D_80147138;
        D_80147194 = D_8014716C;
        break;
    case 4:
        D_8014718C = D_8014711C;
        D_80147190 = D_80147150;
        D_80147194 = D_80147184;
        break;
    case 5:
        D_8014718C = D_80147110;
        D_80147190 = D_80147144;
        D_80147194 = D_80147178;
        break;
    case 6:
        D_8014718C = D_80147114;
        D_80147190 = D_80147148;
        D_80147194 = D_8014717C;
        break;
    case 7:
        D_8014718C = D_80147108;
        D_80147190 = D_8014713C;
        D_80147194 = D_80147170;
        break;
    case 8:
        D_8014718C = D_801470F0;
        D_80147190 = D_80147124;
        D_80147194 = D_80147158;
        break;
    case 9:
        D_8014718C = D_80147118;
        D_80147190 = D_8014714C;
        D_80147194 = D_80147180;
        break;
    case 10:
        D_8014718C = D_80147120;
        D_80147190 = D_80147154;
        D_80147194 = D_80147188;
        break;
    }
    func_8004DE1C();
}
#else
INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_8013907C);
#endif

void func_801393C8(void) {
    s32 pad;
    s32 bg;

    /* FAKE: dead stores touch D_800F62CF at entry for cvt_pass.py (T-20610) and promote D_80147244 to $v0; capturing func_800BCE10 into pad reserves $v0 across the switch and post-switch check. T-24130 */
    D_80147244 = D_800F62CF;
    D_80147244 = D_800F6468;
    if ((D_800F6468 & 7) == 0) {
        D_80147244 = 3;
    } else if ((D_800F6468 & 7) < 4U) {
        D_80147244 = 0;
    } else {
        D_80147244 = 1;
    }
    if (D_800F62CF == 0) {
        D_80147244 = D_800F6468 & 1;
    }
    switch (D_80147244) {
    case 0:
        pad = func_800BCE10(&D_800D92E0, "廊下");
        bg = 0x3FED;
        break;
    case 1:
        pad = func_800BCE10(&D_800D92E0, "教室");
        bg = 0x3FFF;
        break;
    case 2:
        pad = func_800BCE10(&D_800D92E0, "校舎裏");
        bg = 0x401A;
        break;
    default:
        pad = func_800BCE10(&D_800D92E0, "屋上");
        bg = 0x4023;
        break;
    }
    if (pad) {}
    if (D_80147220 == D_8014723C - 1) {
        pad = func_800BCE10(&D_800D92E0, "校門前");
        bg = 0x3FDA;
    }
    if (pad) {}
    if (D_80147220 == D_8014723C - 1 && D_800F62CF == 0 &&
        func_8005EBE0(D_800F62CF) >= 0x46U) {
        func_800BCE10(&D_800D92E0, "自宅前");
        bg = 0x40ED;
    }
    if (D_800F62CF == 9 && D_80147240 == 1 && D_80147244 >= 2U) {
        func_800BCE10(&D_800D92E0, "教室");
        bg = 0x3FF5;
    }
    if (D_800F62CF == 9 && D_80147240 == 1 && D_80147244 < 2U) {
        func_800BCE10(&D_800D92E0, "廊下");
        bg = 0x3FED;
    }
    switch (bg) {
    case 0x3FED:
        func_80090960(6, 0);
        break;
    case 0x3FF5:
        func_80090960(6, 1);
        break;
    case 0x40ED:
        func_80090960(6, 5);
        break;
    case 0x3FDA:
        func_80090960(6, 4);
        break;
    case 0x401A:
        func_80090960(6, 3);
        break;
    default:
        func_80090960(6, 2);
        break;
    }
    func_8008A0D4(bg);
    func_8004DE1C();
}

void func_801396D8(void) {
    D_80147220 += 1;
    func_8004DEE4(1);
}

void func_8013970C(void) {
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}

void func_80139764(void) {
    if ((D_80147240 == 1) && (D_800F62CF == 9)) {
        D_800F647A += 6;
        return;
    }
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80138B50.rodata", D_80145A40);

void func_801397BC(void) {
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(0x401A);
    func_8004DE1C();
}

void func_801397F8(void) {
    func_8008F618(0);
    func_80072734(0x53A1);
    func_8004DE1C();
}

void func_80139828(void) {
    func_8008F618(9);
    func_80072734(0x63CD);
    func_8004DE1C();
}

s32 func_80139858(void) {
    s32 v;

    v = D_800F5490;
    if (D_800F53E0 == ((u32)(v << 0x17) >> 0x1B) && D_800F53DF == (v & 0xF)
        && func_8005EBE0(D_800F62CF) >= 0x46U) {
        D_800F5AB1 = D_800F6479;
        /* FAKE: D_800F647A read through the D_800F5AB1 base so as1 keeps the load after the store. T-11430 */
        D_800F5AB2 = (&D_800F5AB1)[0x9C9] + 1;
        func_8004DEAC(3);
        return 0;
    }
    if (func_8005EBE0(D_800F62CF) >= 0x46U) {
        func_80138858();
        return;
    }
    func_8004DE1C();
}
