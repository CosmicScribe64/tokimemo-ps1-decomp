#include "common.h"
#include "game.h"

void func_80059A20(void) {
    func_8004B19C(D_800E6280.unk_110A, 0x90, 0x58);
    func_8004AE28(D_800E6280.unk_1104.w, 0x80, 0x58);
    func_8004B19C(D_800E6280.unk_1109, 0x90, 0x50);
    func_8004AE28(D_800E6280.unk_1100, 0x80, 0x50);
    func_8004B19C(D_800E6280.unk_1108, 0x90, 0x48);
    func_8004AE28(D_800E6280.unk_10FC, 0x80, 0x48);
    func_8004B19C(D_800E6280.unk_10F4, 0x80, 0x60);
    func_8004B19C(D_800E6280.unk_110D, 0x90, 0x60);
    func_8004B19C(D_800E7D34, 0x90, 0x40);
    func_8004B19C(GetSp(), 0x90, 0x68);
}

INCLUDE_RODATA("asm/data/main/80059A20.rodata", D_800AFDF0);

void func_80059B04(s32 arg0) {
    printf(D_800AFDF0, arg0);
    D_800B3D54[D_800B3D24] = 1;
}
