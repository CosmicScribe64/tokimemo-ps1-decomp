#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_801460E0;

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135CA0", func_80135CA0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135CA0", func_80135D64);

void func_80135DE8(void) {
    func_801343A4();
    if (D_80145EB8 == 3) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x500);
        }
    }
}

void func_80135E3C(void) {
    D_800F5ACD = D_800F62CF;
    func_8004DE1C();
}

void func_80135E68(void) {
    func_80090960(5, 1);
    func_8008A0D4(0x4195);
    func_8004DE1C();
}

void func_80135E9C(void) {
    D_801460C0 = D_8012E66C;
    D_80145EB4 += D_8012E66C;
    func_800BCE10(D_800D9328, D_801460C4 + D_8012E66C * 9);
    if (D_8012E66C == 0) {
        D_800F549A += 0xA;
        (&D_800F549A)[2] += 0xA; /* FAKE: indexing the first symbol keeps the load after the store; matches, real source unknown. T-4090 */
        D_800F62CF = 0xE;
    } else if (D_8012E66C == 1) {
        D_800F5496 += 0x19;
        D_800F62CF = 0xE;
    }
    func_8008FB00();
    func_8004DE1C();
}

void func_80135F74(void) {
    switch (D_801460C0) {
    case 0:
        D_80145EB4 += 3;
        break;
    case 1:
        D_80145EB4 += 2;
        break;
    case 2:
        break;
    }
    func_8004DE1C();
}

void func_80135FE8(void) {
    s32 temp_t6;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t6 = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    switch (temp_t6) {
    case 0:
        break;
    case 1:
    case 2:
        D_80145EB4 += 1;
        break;
    default:
        D_80145EB4 += 2;
        break;
    }
    func_8004DE1C();
}

void func_8013605C(void) {
    switch ((u8) func_8005E0E0(D_800F62CF) & 0x7F) {
    case 0:
        D_80145EB4 += 2;
        break;
    case 1:
    case 2:
        D_80145EB4 += 1;
        break;
    }
    func_8004DE1C();
}

void func_801360DC(void) {
    if (D_8012E66C != 2) {
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135CA0", func_80136124);

void func_801361A8(void) {
    D_8012E67C = D_8012E66C;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80135CA0", func_801361D4);
