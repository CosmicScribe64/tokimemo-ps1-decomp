#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801484E0", func_801484E0);

void func_80148654(void) {
    switch (D_8015EDB0) {
    case 0:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8013F0F4(0x506, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        if (!(D_80121422 & 1)) {
            func_8013E7C0(0x34, 4, 5, 1);
            D_8015EDB8 += 1;
        }
        if (D_8015EDB8 >= 0xE) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801484E0", func_80148714);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801484E0", func_80148B08);
