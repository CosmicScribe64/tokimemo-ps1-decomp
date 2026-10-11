#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_80148180;

void func_8013E5B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148180;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013E5B0", func_8013E638);

void func_8013E724(void) {
    s16 i;

    D_801480FC = D_8014808C;
    D_80148100 = D_801480C0;
    D_80148104 = D_801480F4;
    D_80148108 = 0;
    D_8014810C = 0;
    for (i = 0; i < 0xB; i++) {
        D_800D9388[i] = (D_800F53A0.girl[i].unk_10[1] & 1) >= 1;
    }
    func_8004DE1C();
}

void func_8013E7D8(void) {
    D_800F62CF = (u8) D_8012E66C;
    func_80090960(7, 0);
    func_8008F618(D_800F62CF);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013E5B0", func_8013E824);
