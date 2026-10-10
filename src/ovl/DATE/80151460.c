#include "common.h"
#include "ovl/DATE.h"

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80151670);

void func_80151720(void) {
    D_8015EE20 = 0x801E4000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80151734);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_801517C8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_801518B8);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80151A20);

/* FAKE: D_800E64EA and D_800E64EE written as indexed views of D_800E64E6; stops IDO hoisting the later loads above the stores (same as func_80062D0C). Real source unknown. T-4040 */
void func_80151C54(void) {
    D_8015E208 = D_8015DEBC;
    D_8015E20C = D_8015DFF8;
    D_8015E210 = D_8015E134;
    func_801515C0();
    func_80043914(D_8015EDD8, 0x11, 1, 2, 0);
    func_80084E90(D_8015EDDC, D_8015EDE0, D_8015EDE4, D_8015EDE8, D_8015EDEC, D_8015EDF0);
    func_800850D4(D_8015EDCC, D_8015EDD0, D_8015EDC8, (s32) D_8015EDD4);
    D_800E64E6 += 2;
    (&D_800E64E6)[2] += 1;
    (&D_800E64E6)[4] -= 0x14;
    func_80084D3C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80151D68);

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80151F84);

void func_80152024(void) {
    func_80046318(1, 0x801E4000, 0x8FF2);
    func_80151720();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152060);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152268);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152684);

void func_80152948(void) {
    func_80046318(0x65, 0x801B0000, 0x82D7);
    func_80151460();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152980);

void func_80152AA8(void) {
    func_80046318(0x45, 0x801B0000, 0x833C);
    func_80151510();
    func_8004284C();
}

void func_80152AE0(void) {
    D_801206DA = 5;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152B08);

void func_80152B7C(void) {
    func_80046318(0x3D, 0x801B0000, 0x8381);
    func_801515C0();
    func_8004284C();
}

typedef struct {
    void (*f[47])();
} FnTbl47; /* size 0xBC */
extern FnTbl47 D_8015F09C;

void func_80152BB4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl47 tbl;

    tbl = D_8015F09C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152C3C);

void func_80152CA8(void) {
    func_80046318(0x3D, 0x801B0000, 0x83BE);
    func_80151670();
    func_8004284C();
}

/* FAKE: D_800E64EA and D_800E64EE written as indexed views of D_800E64E6; stops IDO hoisting the later loads above the stores (same as func_80151C54). Real source unknown. T-4040 */
void func_80152CE0(void) {
    if (D_80122CDC == 1) {
        D_800CA150 = (u16)D_800CA150 + 2;
        D_800CA154 = 0;
        D_800E64E6 += 2;
        (&D_800E64E6)[2] += 2;
        (&D_800E64E6)[4] -= 0x14;
        func_80084D3C();
        func_8004284C();
    } else {
        D_800E64E6 += 3;
        (&D_800E64E6)[2] += 1;
        (&D_800E64E6)[4] -= 0x14;
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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80151460", func_80152E34);
