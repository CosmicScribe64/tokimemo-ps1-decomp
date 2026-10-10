#include "ovl/VALEN.h"

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_8013454C;

void func_80132760(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_8013454C;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_801327E8(void) {
    func_800847B8(9);
    func_80132348();
}

void func_80132810(void) {
    D_800E71DF = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132834);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80132D0C);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133088);

void func_801334DC(void) {
    D_80134520 += 1;
    func_80042940(1);
}

void func_80133510(void) {
    if (strcmp(D_800CA19C, D_800CA1DC) == 0) {
        func_8004284C();
    }
    strcpy(D_800CA19C, D_800CA1DC);
    func_8004284C();
}

void func_80133568(void) {
    if ((D_80134544 == 1) && (D_800E71DF == 9)) {
        D_800E738A += 6;
        return;
    }
    func_8004284C();
}

void func_801335C0(void) {
    func_800AE0F0(D_800CA1DC, "校舎裏");
    func_800AE0F0(D_800CA19C, "　");
    func_8007ED84(0x407A);
    func_8004284C();
}

void func_80133610(void) {
    func_800847B8(0);
    func_80062CD0(0x544A);
    func_8004284C();
}

void func_80133640(void) {
    func_800847B8(9);
    func_80062CD0(0x64D0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133670);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_80133728);

INCLUDE_ASM("asm/ovl/VALEN/nonmatchings/VALEN/80132760", func_8013387C);
