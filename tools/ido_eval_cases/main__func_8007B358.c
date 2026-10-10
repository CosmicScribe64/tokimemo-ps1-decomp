#include "common.h"
#include "game.h"
#include "main_only.h"

extern u16 D_80125D50;

void func_8007B358(s32 arg0) {
    s32 temp_a2;
    s32 temp_a3;

    temp_a2 = arg0 & 0xFFFF;
    temp_a3 = ((D_80125D50 << 8) & 0xFF00) | (((u32) (temp_a2 & 0xFF) >> 4) & 0xFF);
    if (temp_a2 & 0x1000) {
        func_8007B5CC(temp_a3, ((((temp_a2 & 0xF) * 4) + 0x18) << 8) & 0xFF00, temp_a2, temp_a3);
        return;
    }
    func_8007B568(temp_a3, ((((temp_a2 & 0xF) * 4) + 0x18) << 8) & 0xFF00, temp_a2, temp_a3);
}
