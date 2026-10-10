#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132D00", func_80132D00);

void func_80132D88(void) {
    func_80044750(0x603);
    func_8004284C();
}

void func_80132DB0(void) {
    func_800AE0F0(D_800CA1DC, "正月");
    func_8004284C();
}

void func_80132DE4(void) {
    bg_read_sub2(0x4144);
    func_8004284C();
}

void func_80132E0C(void) {
    if (D_80122CDC != 0) {
        if ((D_800E6280.unk_F5F == 2) || (D_800E6280.unk_F5F == 7) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 9) || (D_800E6280.unk_F5F == 0xA)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132D00", func_80132E8C);

void func_80132F90(void) {
    if (D_80122CDC == 0) {
        func_80042908(3);
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80132D00", func_80132FCC);
