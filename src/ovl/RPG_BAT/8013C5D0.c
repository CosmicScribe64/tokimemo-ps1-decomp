#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8013C5D0(void) {
    s32 temp_v1;
    s32 temp_v1_2;

    switch (D_8015EDAC) {
    case 6:
        func_801516E4();
    default:
        break;
    case 7:
        func_800AE0F0(D_8015E74C, "片桐「はっ、");
        func_800AE0F0(D_8015E774, "いいかげんにしなさいっ！");
        func_8014B738((s32) &D_8015E814, 2, 0xA);
        func_8013F1C4(0x42, 0x0A000043, 0);
        D_8015EE48 = 3;
        func_8013E810(0x1D, 7, 1, 1);
        func_8014EBA8();
        break;
    case 8:
        temp_v1 = D_8015EDB4 + 1;
        D_8015EDB4 = temp_v1;
        if (temp_v1 >= 0x12D) {
            D_8015EE48 = 1;
            func_8014EBA8();
        }
        if (func_800460EC() == 4) {
            D_8015ED38 = 1;
        }
        if (D_8015ED38 == 1) {
            if (func_800460EC() == 2) {
                D_8015EE48 = 1;
                D_8015ED38 = 0;
                func_8014EBA8();
            }
        }
        break;
    case 9:
        func_8013F1C4(0x44, 0x0B000049, 1);
        D_8015EE48 = 3;
        func_8014EBA8();
        break;
    case 10:
        func_8014ED44(0x32);
        break;
    case 11:
        func_80150B90();
        temp_v1_2 = D_8015EDB4 + 1;
        D_8015EDB4 = temp_v1_2;
        if (temp_v1_2 >= 0x12D) {
            D_8015EE48 = 1;
            D_8015EE4C = D_8015E744 + 1;
            func_8013E810(0x1D, 0, 1, 1);
            func_8014EBA8();
        }
        if (func_800460EC() == 4) {
            D_8015ED38 = 1;
        }
        if (D_8015ED38 == 1) {
            if (func_800460EC() == 2) {
                D_8015EE48 = 1;
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                D_8015ED38 = 0;
                func_8014EBA8();
            }
        }
        break;
    case 12:
        func_80150B90();
        func_8014ED44(0x1E);
        break;
    case 13:
        func_80151984();
        break;
    case 14:
        D_8015EDAC = 0x64;
        break;
    }
}
