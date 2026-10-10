#include "common.h"
#include "ovl/KANGEI.h"

s32 func_80051A68();

void func_80138320(void) {
    if ((u32) (func_80051A68(0) & 0x7F) >= 2U) {
        D_800E738A -= 0xB;
        return;
    }
    func_8004284C();
}
