#include "common.h"
#include "ovl/DATE2.h"

void func_800AE0A0();

void func_80132E5C(void) {
    s32 temp_v0;
    u32 temp_v0_2;

    temp_v0 = D_800E7384 * 0x1400;
    func_800AE0A0(temp_v0 + 0x80180000, temp_v0 + 0x801C0000, 0x1400);
    temp_v0_2 = D_800E7384 + 1;
    D_800E7384 = temp_v0_2;
    if (temp_v0_2 == 0x12) {
        func_8004284C();
    }
}
