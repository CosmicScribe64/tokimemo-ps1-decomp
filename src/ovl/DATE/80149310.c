#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */

void func_80149310(void) {
    D_8015CFD0 = 0x801B0478;
    D_8015CFD4 = 0x801B13A4;
    D_8015CFD8 = 0x801B2998;
    D_8015CFDC = 0x801B38C8;
    D_8015CFE0 = 0x801B4258;
    D_8015CFE4 = 0x801B4B98;
    D_8015CFE8 = 0x801B5574;
    D_8015CFEC = 0x801B6560;
    D_8015CFF0 = 0x801B7B74;
    D_8015CFF4 = 0x801B8B18;
    D_8015CFF8 = 0x801B93D0;
    D_8015CFFC = 0x801B9DA0;
    D_8015D000 = 0x801BA780;
    D_8015D004 = 0x801B0510;
    D_8015D008 = 0x801B14E0;
    D_8015D00C = 0x801B2AD0;
    D_8015D010 = 0x801B3960;
    D_8015D014 = 0x801B42F0;
    D_8015D018 = 0x801B4C38;
    D_8015D01C = 0x801B560C;
    D_8015D020 = 0x801B66A0;
    D_8015D024 = 0x801B7CB4;
    D_8015D028 = 0x801B8BB0;
    D_8015D02C = 0x801B9468;
    D_8015D030 = 0x801B9EA8;
    D_8015D034 = 0x801BA818;
    D_8015D038 = 0x801B0888;
    D_8015D03C = 0x801B1E14;
    D_8015D040 = 0x801B33B0;
    D_8015D044 = 0x801B3CF4;
    D_8015D048 = 0x801B4684;
    D_8015D04C = 0x801B4FEC;
    D_8015D050 = 0x801B59F4;
    D_8015D054 = 0x801B6FB8;
    D_8015D058 = 0x801B85CC;
    D_8015D05C = 0x801B8F28;
    D_8015D060 = 0x801B97E0;
    D_8015D064 = 0x801BA2C8;
    D_8015D068 = 0x801BAB90;
}

extern FnTbl28 D_8015D098;

void func_80149584(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl28 tbl;

    tbl = D_8015D098;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if ((D_80122D44 & 1) && (D_800B593C == 0x80)) {
        func_8006B900();
    }
}

void func_80149630(void) {
    D_8015D084 = 2;
    D_8015D088 = 0;
    func_8004284C();
}

void func_80149660(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80149310();
    func_8004284C();
}

void func_80149698(void) {
    D_80122CDC = D_8015D094;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_801496C4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_80149AC0);

void func_80149B78(void) {
    s32 pad; /* FAKE: unused local above buf, puts buf at sp+0x28 as in the original; real source unknown. T-4010 */
    u8 buf[3] = "こ";

    D_8015D084 = D_80122CDC * 2 + 0xD;
    if (D_80122CDC == 0) {
        (**(u8 ***)(D_8015D06C + D_8015D084 * 4 + 4))[0] = buf[0];
        (**(u8 ***)(D_8015D06C + D_8015D084 * 4 + 4))[1] = buf[1];
    }
    func_8004284C();
}

void func_80149C1C(void) {
    if ((D_800E62BF == (D_800E6378 & 0xF)) && (D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B))) {
        if ((u8)D_800E71DE < 0xFEU) {
            D_800E71DE -= 1;
        }
        func_80042878(0x50);
        func_80042908(3);
        return;
    }
    func_8004284C();
}

void func_80149CA0(void) {
    D_8015D084 = D_8015D08C;
    D_8015D06C = D_8015D078;
    D_8015D070 = D_8015D07C;
    D_8015D074 = D_8015D080;
    func_80149D04();
    func_8004284C();
}

extern DateTxt D_8015D108;

void func_80149D04(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    DateTxt tbl;

    tbl = D_8015D108;
    func_800AE0F0(D_800CA25C, &tbl.s[D_800E71DF * 0x129 + D_8015D090 * 0x63 + D_80122CDC * 0x21]);
}

void func_80149DC4(void) {
    D_800CA134 = &D_8015D084;
    D_800CA138 = &D_8015D088;
    D_800CA13C = D_8015D06C;
    D_800CA140 = D_8015D070;
    D_800CA144 = D_8015D074;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80149E40(void) {
    D_800CA134 = (u8 *)&D_8015D084;
    D_800CA138 = (u8 *)&D_8015D088;
    D_800CA13C = D_8015D06C;
    D_800CA140 = D_8015D070;
    D_800CA144 = D_8015D074;
    if (func_80082764(D_80122CDC, 1, 0) == 1) {
        func_8004284C();
    }
}

void func_80149ED0(void) {
    D_800CA134 = &D_8015D084;
    D_800CA138 = &D_8015D088;
    D_800CA13C = D_8015D06C;
    D_800CA140 = D_8015D070;
    D_800CA144 = D_8015D074;
    func_80082764(0xFF, 1, 0);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_80149F48);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_80149FF8);
