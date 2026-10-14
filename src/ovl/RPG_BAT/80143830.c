#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80143830(void) {
    s32 temp_v0;

    switch (D_8015EDAC) {
    case 6:
        func_800AE0F0(D_8015E74C, "古式「まぁ、おもしろいですねぇ。");
        func_8013F1C4(0x79, 0x12000085, 0);
        func_8014B738((s32) &D_8015E814, 1, 0x1F4);
        func_8013E7C0(0x1D, 6, 1, 1);
        func_8014EBA8();
    default:
        break;
    case 7:
        temp_v0 = D_8015EDB4 + 1;
        D_8015EDB4 = temp_v0;
        if (temp_v0 >= 0x12D) {
            func_8013E7C0(0x1D, 0, 1, 1);
            D_8015EE4C = D_8015E744 + 1;
            func_8014EBA8();
        }
        if (func_800460EC() == 4) {
            D_8015ED38 = 1;
        }
        if (D_8015ED38 == 1) {
            if (func_800460EC() == 2) {
                func_8013E7C0(0x1D, 0, 1, 1);
                D_8015EE4C = D_8015E744 + 1;
                D_8015ED38 = 0;
                func_8014EBA8();
            }
        }
        break;
    case 8:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8014EBA8();
        break;
    case 9:
        func_8014ED44(0x1E);
        break;
    case 10:
        D_8015EDAC = 0x64;
        break;
    }
}
