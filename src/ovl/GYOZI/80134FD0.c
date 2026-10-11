#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[60])();
} FnTbl60; /* size 0xF0 */
extern FnTbl60 D_80145F68;

void func_80134FD0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl60 tbl;

    tbl = D_80145F68;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80135044(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134FD0", func_8013506C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134FD0", func_80135118);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134FD0", func_80135280);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80134FD0.rodata", D_801458E8);

void func_80135420(void) {
    func_800BCE10(&D_800D92E0, &D_801458E8);
    D_80145F64 = 0;
    D_80145F60 = 0;
    D_80145EC8 = 1;
    D_80145EB4 = 6;
    func_8004DE1C();
}

void func_80135478(void) {
    if (D_80145EC4 == 0) {
        func_8008A0D4(0x4107);
    } else {
        D_800F647A += 3;
    }
    func_8004DE1C();
}

void func_801354C8(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

void func_801354F0(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134FD0", func_80135518);

void func_80135628(void) {
    D_80145EB4 = 7;
    D_80145EA8 = D_80145E08;
    D_80145EAC = D_80145E40;
    D_80145EB0 = D_80145E78;
    func_8004DE1C();
    D_800F647A -= 5;
}

void func_80135690(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
}

void func_801356FC(void) {
    if (D_8012E66C == 0) {
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_80135730(void) {
    D_80145F60 = 1;
    D_80145EB4 = 0;
    D_80145EA8 = D_80145E00;
    D_80145EAC = D_80145E38;
    D_80145EB0 = D_80145E70;
    func_8004DDD8();
}

void func_80135790(void) {
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
    case 3:
        D_80145EB4 += 2;
        break;
    }
    func_8004DE1C();
}

void func_80135814(void) {
    D_80145EB4 = 0x27;
    func_8004DE1C();
}

void func_8013583C(void) {
    if (D_8012E66C != 0) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}

void func_80135890(void) {
    if (D_800F62CF != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void func_801358CC(void) {
    D_80145EB4 = 0xF;
    func_8004DE1C();
}
