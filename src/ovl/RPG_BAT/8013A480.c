#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8013A480(void) {
    s32 temp_v1;

    switch (D_8015EDAC) {
    case 6:
        func_801516E4();
    default:
        break;
    case 7:
        func_800AE0F0(D_8015E74C, "鏡「つまらないわ…よっ！");
        func_8014B738((s32) &D_8015E814, 1, 0x1F4);
        func_8013E810(0x1D, 0xA, 1, 1);
        func_8013F1C4(0x71, 0x11000078, 0);
        func_8014EBA8();
        break;
    case 8:
        func_8014ED44(0x5A);
        break;
    case 9:
        func_80150B90();
        temp_v1 = D_8015EDB4 + 1;
        D_8015EDB4 = temp_v1;
        if (temp_v1 >= 0x12D) {
            D_8015EE4C = D_8015E744 + 1;
            func_8013E810(0x1D, 0, 1, 1);
            func_8014EBA8();
        }
        if (func_800460EC() == 4) {
            D_8015ED38 = 1;
        }
        if (D_8015ED38 == 1) {
            if (func_800460EC() == 2) {
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                D_8015ED38 = 0;
                func_8014EBA8();
            }
        }
        break;
    case 10:
        func_80150B90();
        func_8014ED44(0x1E);
        break;
    case 11:
        func_80151984();
        break;
    case 12:
        D_8015EDAC = 0x64;
        break;
    }
}
