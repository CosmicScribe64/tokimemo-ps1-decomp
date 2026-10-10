#include "common.h"
#include "game.h"
#include "main_only.h"


s32 GetWorkBase(s32 arg0, s32 arg1) {
    s32 temp_a3;
    u8 *temp_a2;

    temp_a2 = (arg1 * 0xC) + D_800E6280;
    temp_a3 = *(s32 *)(temp_a2 + 0x20);
    *(s32 *)(temp_a2 + 0x20) = temp_a3 + ((arg0 + 3) / 4) * 4;
    return (temp_a3 * 4) + *(s32 *)(temp_a2 + 0x24);
}
