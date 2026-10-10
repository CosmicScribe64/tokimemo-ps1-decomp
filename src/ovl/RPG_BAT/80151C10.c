#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80151C10", func_80151C10);

void func_80151F38(void) {
    s32 temp_v0;

    temp_v0 = func_8013E9A8(3);
    switch (temp_v0) {
    case 0:
        func_8013F15C(0x50E, 1, 0);
        return;
    case 1:
        func_8013F15C(0x50F, 1, 0);
        return;
    case 2:
        func_8013F15C(0x510, 1, 0);
        return;
    }
}
