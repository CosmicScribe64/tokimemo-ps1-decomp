#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    u8 s[0xCC3];
} DateTxt; /* size 0xCC3 */

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_80149310);

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

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80149310", func_80149E40);

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
