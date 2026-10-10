#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_80159D50(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "古式「お父様ーーっ！！");
        func_8014B738(D_8015E9CC, 1, 0xAA);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x05000000, 0x01000007, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F258();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80159D50", func_80159E28);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80159D50", func_8015A11C);
