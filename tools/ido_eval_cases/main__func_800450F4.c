#include "common.h"
#include "game.h"
#include "main_only.h"


void func_800450F4(s32 arg0, s32 arg1) {
    s32 *var_v0;

    if (arg0 != 0) {
        var_v0 = (s32 *)0x8002E800;
    } else {
        var_v0 = (s32 *)0x8001C000;
    }
    if ((*var_v0 & 0xFFFF) != 0x40) {
        return;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
}
