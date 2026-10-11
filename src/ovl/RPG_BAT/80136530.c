#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80136530", func_80136530);

void func_8013673C(void) {
    D_8012139E = 0x82;
    switch (D_8015EC74) {
    case 0:
        func_8013E810(0x32, 6, 5, 1);
        func_8013E7C0(0x33, 8, 1, 0);
        D_8015EC74 += 1;
        break;
    case 1:
        D_8015EC78 += 1;
        if (D_8015EC78 >= 7) {
            D_8015EC74 += 1;
        }
        break;
    case 2:
        func_8013E7C0(0x34, 0xC, 1, 1);
        func_8013E97C(0x33, 0, 0);
        func_8013E7C0(0x33, 8, 1, 1);
        func_8013F0F4(0x513, 1, 5);
        D_8015EC74 += 1;
        break;
    case 3:
        D_8015EC78 += 1;
        if (D_8015EC78 >= 0xA) {
            D_8015EC74 += 1;
        }
        break;
    case 4:
        D_8012139E = 1;
        D_8015EC78 = 0;
        D_8015EC74 = 0;
        func_8014EBA8();
        break;
    }
}
