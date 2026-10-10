#include "common.h"
#include "ovl/GEKO.h"

s32 func_80051A68();

void func_80133250(void) {
    u32 temp_t6;

    temp_t6 = func_80051A68(D_800E71DF) & 0x7F;
    if ((temp_t6 == 4) || (D_80144C58 != 0)) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if ((D_800E71DF == 6) && (temp_t6 < 2U)) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}
