#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[15])();
} FnTbl15; /* size 0x3C */
extern FnTbl15 D_80139AE8;

void func_80133C10(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl15 tbl;

    tbl = D_80139AE8;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80133C84(void) {
    D_800E71DF = D_800E69DD;
    func_80042808();
}

void func_80133CB0(void) {
    func_80046318(0x3B, 0x80197000, 0xAE60);
    func_80132E74();
    func_8004284C();
}

void func_80133CEC(void) {
    func_80046318(0x2D, 0x801B4400, 0xAE9B);
    func_80132E40();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80133C10", func_80133D28);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80133C10", func_80133EF8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80133C10", func_80133F74);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80133C10", func_80133FF4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80133C10", func_80134084);
