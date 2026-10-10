#include "common.h"
#include "game.h"
#include "main_only.h"


s32 func_8007B5EC(u16 a) {
    u16 temp_t9;

    if ((u16) D_80125CC0 >= 0x20U) {
        return -1;
    }
    temp_t9 = D_80125CC0 + 1;
    D_80125CC0 = temp_t9;
    (&D_80125CC0)[temp_t9 & 0xFFFF] = a & 0xFFFF;
    return 0;
}
