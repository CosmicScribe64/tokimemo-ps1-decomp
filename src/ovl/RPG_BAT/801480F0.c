#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801480F0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E904, "紐緒「これでもくらいなさい！");
        func_8014B738(D_8015E9CC, 1, 0xAA);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x12, 0x04000019, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F220();
        func_8014F230();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801480F0", func_801481C8);
