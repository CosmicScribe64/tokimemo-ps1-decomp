#include "common.h"
#include "ovl/DATE.h"

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80145AA0", func_80145AA0);

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
extern FnTbl15 D_8015CF3C;

void func_80145D14(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_8015CF3C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if ((D_80122D44 & 1) && (D_800B593C == 0x80)) {
        func_8006B900();
    }
}

void func_80145DB8(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80145AA0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80145AA0", func_80145DF0);

void func_80145E74(void) {
    D_8015CF34 = 1;
    D_8015CF38 = 0;
    func_800847B8(D_800E71DF);
    func_80145AA0();
    func_80145EC4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80145AA0", func_80145EC4);

void func_80146134(void) {
    D_800CA134 = &D_8015CF34;
    D_800CA138 = &D_8015CF38;
    D_800CA13C = D_8015CF1C;
    D_800CA140 = D_8015CF20;
    D_800CA144 = D_8015CF24;
    func_80082764(D_80122CDC, 1, 0);
}
