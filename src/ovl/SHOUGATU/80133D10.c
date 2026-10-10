#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80133D10", func_80133D10);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80133D10", func_80133D98);

void func_80133E30(void) {
    if (D_80143B20 != 0) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

void func_80133E6C(void) {
    if (D_80122CDC != 0) {
        func_800833F0();
        return;
    }
    func_800833A0();
}

void func_80133EA8(void) {
    if ((D_80143B20 == 0) && (((u32) D_800E6280.unk_10F8 % 3U) == 0)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 3);
        } else {
            func_80085B3C(5, 0);
        }
    }
    bg_read_sub2(0x41ED);
    func_8004284C();
}

void func_80133F24(void) {
    if ((D_800E6280.unk_F5F != 5) || (D_80143B20 != 0) || (D_80143B18 == 0)) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_80133F88(void) {
    if (D_80143B20 != 0) {
        D_800E6280.unk_110A += 4;
    }
    func_8004284C();
}

void func_80133FC8(void) {
    if (D_80143B18 == 0) {
        func_8004284C();
        return;
    }
    func_801323D4();
}

void func_80134004(void) {
    if ((D_800E6280.unk_F5F == 5) && (D_80143B18 == 0)) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

void func_80134058(void) {
    if (D_80143B18 != 0) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

void func_80134094(void) {
    if (D_80122CDC != 0) {
        D_800E6280.unk_1BC[5].unk_06 -= 1;
        D_800E6280.unk_1BC[5].unk_02 -= 1;
        D_800E6280.unk_1BC[5].unk_0A += 0xA;
    } else {
        D_800E6280.unk_1BC[5].unk_02 += 1;
    }
    func_80084D3C();
    func_8004284C();
}
