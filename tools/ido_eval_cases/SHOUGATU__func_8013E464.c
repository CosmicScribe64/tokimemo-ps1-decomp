#include "common.h"
#include "ovl/SHOUGATU.h"

s32 func_80051A68();

void func_8013E464(void) {
    if ((func_80051A68(D_800E71DF) & 0x7F) != 4) {
        func_8004284C();
        return;
    }
    func_8013D2A8();
}
