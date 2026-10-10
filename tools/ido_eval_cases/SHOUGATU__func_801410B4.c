#include "common.h"
#include "ovl/SHOUGATU.h"

extern u8 D_800E64B8;

void func_801410B4(void) {
    D_800E64B8 |= 4;
    D_80145F2C = 3;
    func_8004284C();
}
