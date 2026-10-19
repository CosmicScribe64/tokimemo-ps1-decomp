#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"
#include "psx_types.h"

extern u32 D_80122CD0;

void func_80151460(void) {
    D_8015ED70 = 0x801E2118;
    D_8015ED74 = 0x801E2130;
    D_8015ED78 = 0x801E2154;
    D_8015ED7C = *(s16 *)0x801E216C;
    D_8015ED80 = 0x801B0000;
    D_8015ED84 = 0x801B2000;
    D_8015ED88 = 0x801B6000;
    D_8015ED8C = 0x801BA000;
    D_8015ED90 = 0x801BE000;
    D_8015ED94 = 0x801C2000;
    D_8015ED98 = 0x801C6000;
}

void func_80151510(void) {
    D_8015ED9C = 0x801D2160;
    D_8015EDA0 = 0x801D2168;
    D_8015EDA4 = 0x801D2188;
    D_8015EDA8 = *(s16 *)0x801D21A4;
    D_8015EDAC = 0x801B0000;
    D_8015EDB0 = 0x801B2000;
    D_8015EDB4 = 0x801B6000;
    D_8015EDB8 = 0x801BA000;
    D_8015EDBC = 0x801BE000;
    D_8015EDC0 = 0x801C2000;
    D_8015EDC4 = 0x801C6000;
}

void func_801515C0(void) {
    D_8015EDC8 = 0x801CE124;
    D_8015EDCC = 0x801CE128;
    D_8015EDD0 = 0x801CE148;
    D_8015EDD4 = *(s16 *)0x801CE15C;
    D_8015EDD8 = 0x801B0000;
    D_8015EDDC = 0x801B2000;
    D_8015EDE0 = 0x801B6000;
    D_8015EDE4 = 0x801BA000;
    D_8015EDE8 = 0x801BE000;
    D_8015EDEC = 0x801C2000;
    D_8015EDF0 = 0x801C6000;
}

void func_80151670(void) {
    D_8015EDF4 = 0x801CE080;
    D_8015EDF8 = 0x801CE084;
    D_8015EDFC = 0x801CE094;
    D_8015EE00 = *(s16 *)0x801CE0A8;
    D_8015EE04 = 0x801B0000;
    D_8015EE08 = 0x801B2000;
    D_8015EE0C = 0x801B6000;
    D_8015EE10 = 0x801BA000;
    D_8015EE14 = 0x801BE000;
    D_8015EE18 = 0x801C2000;
    D_8015EE1C = 0x801C6000;
}

void func_80151720(void) {
    D_8015EE20 = 0x801E4000;
}

void func_80151734(void) {
    switch (D_80122CD0) {
    case 2:
        func_80151F84();
        return;
    case 3:
        func_80152980();
        return;
    case 4:
        func_80152B08();
        return;
    case 5:
        func_80152BB4();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801517C8(void) {
    s16 i;
    for (i = 0; i < 12; i++) {
        D_8015EE24[i] = func_800AE0D0() % 4;
    }
    D_8015E208 = D_8015DEB4;
    D_8015E20C = D_8015DFF0;
    D_8015E210 = D_8015E12C;
    D_800E6280.unk_1BC[3].unk_02 += 2;
    D_800E6280.unk_1BC[3].unk_06 += 1;
    D_800E6280.unk_1BC[3].unk_0A -= 0x14;
    func_80084D3C();
    D_800E7D34 |= 1;
    func_8004284C();
}

void func_801518B8(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        load_tpage_buf_lock(i + 5, ((s32 *)D_8015ED70)[i], 1);
    }
    func_80151460();
    load_palette(D_8015ED80, 0x11, 1, 2, 0);
    func_80084E90(D_8015ED84, D_8015ED88, D_8015ED8C, D_8015ED90, D_8015ED94, D_8015ED98);
    func_800850D4(D_8015ED74, D_8015ED78, D_8015ED70, D_8015ED7C);
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = 3;
    D_80122D10 = 2;
    D_80122D0C = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 3;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
    func_8004284C();
}

void func_80151A20(void) {
    D_8015E208 = D_8015DEB8;
    D_8015E20C = D_8015DFF4;
    D_8015E210 = D_8015E130;
    func_80151510();
    func_80043914(D_8015EDAC, 0x11, 1, 2, 0);
    func_80084E90(D_8015EDB0, D_8015EDB4, D_8015EDB8, D_8015EDBC, D_8015EDC0, D_8015EDC4);
    func_800850D4(D_8015EDA0, D_8015EDA4, D_8015ED9C, (s32) D_8015EDA8);
    D_800CA360 = 1;
    func_80048F64(0x62);
    D_801206D9 = 9;
    D_801206DA = 0;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = D_8015EDA0;
    D_801206E8 = D_8015EDA4;
    D_8012070C = D_8015ED9C;
    D_801206EC = D_8015EDA8;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    D_800E6280.unk_1BC[3].unk_02 -= 1;
    D_800E6280.unk_1BC[3].unk_06 -= 1;
    D_800E6280.unk_1BC[3].unk_0A += 0x14;
    func_80084D3C();
    D_800CA224[0] = 3;
    D_800CA224[1] = 3;
    D_800CA224[2] = 3;
    D_800CA234[0] = 2;
    D_800CA234[1] = 2;
    D_800CA234[2] = 2;
    func_8004284C();
}

void func_80151C54(void) {
    D_8015E208 = D_8015DEBC;
    D_8015E20C = D_8015DFF8;
    D_8015E210 = D_8015E134;
    func_801515C0();
    func_80043914(D_8015EDD8, 0x11, 1, 2, 0);
    func_80084E90(D_8015EDDC, D_8015EDE0, D_8015EDE4, D_8015EDE8, D_8015EDEC, D_8015EDF0);
    func_800850D4(D_8015EDCC, D_8015EDD0, D_8015EDC8, (s32) D_8015EDD4);
    D_800E6280.unk_1BC[3].unk_02 += 2;
    D_800E6280.unk_1BC[3].unk_06 += 1;
    D_800E6280.unk_1BC[3].unk_0A -= 0x14;
    func_80084D3C();
    func_8004284C();
}

void func_80151D68(void) {
    D_8015E208 = D_8015DEC0;
    D_8015E20C = D_8015DFFC;
    D_8015E210 = D_8015E138;
    func_80151670();
    load_palette(D_8015EE04, 0x11, 1, 2, 0);
    func_80084E90(D_8015EE08, D_8015EE0C, D_8015EE10, D_8015EE14, D_8015EE18, D_8015EE1C);
    func_800850D4(D_8015EDF8, D_8015EDFC, D_8015EDF4, (s32) D_8015EE00);
    D_800CA360 = 1;
    func_8004284C();
}

void func_80151E44(void) {
    bg_read_sub2(0x491B);
    func_8004284C();
}

void func_80151E6C(void) {
    bg_read_sub2(0x4613);
    func_8004284C();
}

void func_80151E94(void) {
    bg_read_sub2(0x48C5);
    func_8004284C();
}

void func_80151EBC(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80151EE4(void) {
    bg_read_sub2(0x4856);
    func_8004284C();
}

void func_80151F0C(void) {
    bg_read_sub2(0x44F1);
    func_8004284C();
}

void func_80151F34(void) {
    bg_read_sub2(0x4900);
    func_8004284C();
}

void func_80151F5C(void) {
    bg_read_sub2(0x45E6);
    func_8004284C();
}

void func_80151F84(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl55 tbl;

    tbl = D_8015EE30;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
    if (D_800E6280.unk_110A == 0x1A || D_800E6280.unk_110A == 0x1B) {
        func_80152268();
    }
}

void func_80152024(void) {
    cd_read_sectors(1, 0x801E4000, 0x8FF2);
    func_80151720();
    func_8004284C();
}

/* The last call's result stays in $v0 as the return value: s32 with no final return (T-17204). */
s32 func_80152060(void) {
    s16 i;
    s32 found;
    s16 *rec;
    DateWords2 tbl;

    tbl = D_8015EF0C;
    if (D_800E6280.unk_1104.u == 0) {
        D_80120696 = 1;
        func_80046290(tbl.v[0], tbl.v[1], 0xF);
        func_80044750(0xBF);
        func_80044750(0x300);
        D_800E6280.unk_1104.u += 1;
    } else {
        if (func_80046094() == 9 && D_800E6280.unk_1104.u >= 0x3D) {
            D_80120696 = 0;
            func_8004284C();
            func_80044750(0x202);
            return 0;
        }
        if ((D_800E6280.unk_F88 & 0x400040) && D_800E6280.unk_1104.u >= 0x3D) {
            D_80120696 = 0;
            func_8004284C();
            D_80120668 = 3;
            func_80044750(0xB4);
            func_80044750(0x202);
            return 0;
        }
        found = 0;
        i = 0;
        do {
            rec = &((s16 *)D_8015EE20)[i * 3];
            if (D_800E6280.unk_1104.u >= rec[0]) {
                if (rec[1] >= D_800E6280.unk_1104.u) {
                    found = 1;
                    D_80120668 = rec[2];
                }
            }
            i++;
        } while (i < 0xA7);
        if (found == 0) {
            D_80120668 = 3;
        }
    }
    if (D_800E6280.unk_1104.u >= 0x67C) {
        D_80120696 = 0;
    }
    if (D_800E6280.unk_1104.u < 0x18E3) {
        D_80120696 = 1;
    }
    if (func_800460EC() & 4) {
        D_800E6280.unk_1104.u += 1;
    }
}

/* Result is the last call's $v0 (s32, no return). T-17204 */
s32 func_80152268(void) {
    s16 i;
    s32 v;
    s32 j;
    u32 n;

    if (D_800E6280.unk_10F8 % 51 == 0) {
        for (i = 0; i < 12; ) {
            D_8015EE24[i] = func_800AE0D0() % 4;
            ++i;
        }
    }
    n = 1000;
    /* FAKE: the dead copies `j = v` after each call keep `v` live across the call, so uopt promotes it to $s0 (with its
     * narrowing and the copy `move s0,tN`) and puts the divisor constants in $s1, as the original has; without them the
     * variable is folded away and `n` takes $s0. The original's real use of `v` after the calls is unknown. T-17204 */
    v = (s16)((D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)v, 0, 9);
    j = v;
    v = (s16)((D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)(v - 0xA), 1, 9);
    j = v;
    v = (s16)((D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)(v - 0x14), 2, 9);
    j = v;
    v = (s16)((D_800E6280.unk_10F8 * 16 + 0x1F4) % n);
    func_80152684((s16)v, 6, 9);
    j = v;
    v = (s16)((D_800E6280.unk_10F8 * 16 + 0x1F4) % n);
    func_80152684((s16)(v - 0xA), 7, 9);
    j = v;
    v = (s16)((D_800E6280.unk_10F8 * 16 + 0x1F4) % n);
    func_80152684((s16)(v - 0x14), 8, 9);
    j = v;
    dtd_on_tpage(0, 0, 9, 3, 0);
    n = 0x320;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)v, 3, 0xD);
    j = v;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)(v + 0xA), 4, 0xD);
    j = v;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16) % n);
    func_80152684((s16)(v + 0x14), 5, 0xD);
    j = v;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16 + 0x190) % n);
    func_80152684((s16)v, 9, 0xD);
    j = v;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16 + 0x190) % n);
    func_80152684((s16)(v + 0xA), 0xA, 0xD);
    j = v;
    v = (s16)(n - (D_800E6280.unk_10F8 * 16 + 0x190) % n);
    func_80152684((s16)(v + 0x14), 0xB, 0xD);
    j = v;
    dtd_on_tpage(0, 0, 0xD, 3, 0);
}

typedef struct {
    s32 a;
    s32 b;
} DtEnt;
typedef struct {
    DtEnt e[4];
} DtTbl; /* size 0x20 */
extern DtTbl D_8015EF14;

typedef struct {
    s32 a;
    s32 b;
} DtEnt;
typedef struct {
    DtEnt e[4];
} DtTbl; /* size 0x20 */
extern DtTbl D_8015EF14;

typedef struct {
    s32 a;
    s32 b;
} DtEnt;
typedef struct {
    DtEnt e[4];
} DtTbl; /* size 0x20 */
extern DtTbl D_8015EF14;

void func_80152684(arg0, arg1, arg2)
    s16 arg0;
    s16 arg1;
    s16 arg2;
{
    /* FAKE: 8 unused bytes above tbl give the original's frame (0x78) and tbl offset (0x50); the real locals are unknown. T-9310 */
    s32 pad[2];
    DtTbl tbl;

    tbl = D_8015EF14;
    if (arg0 < 0x140) {
        func_80152E34(0x140, -0x118, 0xA0 - arg0, 0x28, (0xA0 - arg0) + (s16)(arg0 / 32 + 0xA), 0x28, tbl.e[*(u8 *)&D_8015EE24[arg1]].a, tbl.e[*(u8 *)&D_8015EE24[arg1]].b, arg2, 1);
        return;
    }
    if (arg0 < 0x154) {
        func_80152E34(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, 0x28, tbl.e[*(u8 *)&D_8015EE24[arg1]].a, tbl.e[*(u8 *)&D_8015EE24[arg1]].b, arg2, 1);
        func_80152E34(0x140, -0x118, -0xA0, 0x28, (s16)(arg0 / 32 + 0xA) - arg0 + 0xA0, 0x28, tbl.e[*(u8 *)&D_8015EE24[arg1]].a, tbl.e[*(u8 *)&D_8015EE24[arg1]].b, arg2, 1);
        return;
    }
    func_80152E34(0x140, -0x118, -0xA0, 0x28 - (s16)((arg0 * 7 - 0x8C0) / 8), -0xA0, (0x28 - (s16)((arg0 * 7 - 0x8C0) / 8)) + (s16)(0xF - (arg0 - 0x140) / 32), tbl.e[*(u8 *)&D_8015EE24[arg1]].a, tbl.e[*(u8 *)&D_8015EE24[arg1]].b, arg2, 1);
}

void func_80152948(void) {
    cd_read_sectors(0x65, 0x801B0000, 0x82D7);
    func_80151460();
    func_8004284C();
}

void func_80152980(void) {
    s32 idx;
    DateDispatch80152980 tbl;
    tbl = D_8015EF34;
    if (D_800E6280.unk_110A >= 0x10 && D_800E6280.unk_110A < 0x13) {
        func_8014C9A8();
    }
    if (D_80122D04 == 0 && D_800B593C == 0x80) {
        func_8006B900();
    }
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A >= 0xF && D_800E6280.unk_110A < 0x14) {
        D_801206DB |= 0x80;
        D_801206DF = D_800B593C;
    }
    if (D_800E6280.unk_110A >= 0x14) {
        D_801206DB &= ~0x80;
    }
}

void func_80152AA8(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x833C);
    func_80151510();
    func_8004284C();
}

void func_80152AE0(void) {
    D_801206DA = 5;
    func_8004284C();
}

void func_80152B08(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_8015EFF4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80152B7C(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8381);
    func_801515C0();
    func_8004284C();
}

void func_80152BB4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_8015F09C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80152C3C(void) {
    func_8014C5C8();
    if (((u16)D_800CA154 == 7) && (D_800E6280.unk_1104.u == 1) && (D_80122CDC == 1)) {
        func_80083440(0);
    }
    D_800E6280.unk_1104.w += 1;
}

void func_80152CA8(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x83BE);
    func_80151670();
    func_8004284C();
}

void func_80152CE0(void) {
    if (D_80122CDC == 1) {
        D_800CA150 = (u16)D_800CA150 + 2;
        D_800CA154 = 0;
        D_800E6280.unk_1BC[3].unk_02 += 2;
        D_800E6280.unk_1BC[3].unk_06 += 2;
        D_800E6280.unk_1BC[3].unk_0A -= 0x14;
        func_80084D3C();
        func_8004284C();
    } else {
        D_800E6280.unk_1BC[3].unk_02 += 3;
        D_800E6280.unk_1BC[3].unk_06 += 1;
        D_800E6280.unk_1BC[3].unk_0A -= 0x14;
        func_80084D3C();
    }
    func_8004284C();
}

void func_80152DBC(void) {
    if (D_80122CDC == 0) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

void func_80152DF8(void) {
    if (D_80122CDC == 0) {
        func_80083378();
        return;
    }
    func_800833A0();
}

/* Implicit int with no return value: the callers above come before this definition (T-20600). */
s32 func_80152E34(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 c0, u32 c1, s32 z, s32 trans) {
    GameState *g;
    POLY_G3 *poly;

    g = &D_800E6280;
    poly = func_800490F0(0x1C, D_8011ECA0);
    func_8009F004(poly);
    SetSemiTrans(poly, trans);
    /* setRGBn-style comma lines with masks: separate statements give other registers (T-20600). */
    poly->r0 = (c0 >> 16) & 0xFF, poly->g0 = (c0 >> 8) & 0xFF, poly->b0 = c0 & 0xFF;
    poly->r1 = (c1 >> 16) & 0xFF, poly->g1 = (c1 >> 8) & 0xFF, poly->b1 = c1 & 0xFF;
    poly->r2 = (c1 >> 16) & 0xFF, poly->g2 = (c1 >> 8) & 0xFF, poly->b2 = c1 & 0xFF;
    poly->x0 = g->unk_014[D_8011ECA0] + x0;
    poly->y0 = g->unk_018[D_8011ECA0] + y0;
    poly->x1 = g->unk_014[D_8011ECA0] + x1;
    poly->y1 = g->unk_018[D_8011ECA0] + y1;
    poly->x2 = g->unk_014[D_8011ECA0] + x2;
    poly->y2 = g->unk_018[D_8011ECA0] + y2;
    func_8009EED0(&D_800E8CA0[(D_8011ECA0 << 10) + z * 4], poly);
}
