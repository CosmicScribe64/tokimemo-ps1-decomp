#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80136530(void) {
    s32 temp_v0;

    switch (D_8015EDAC) {
    case 6:
        func_801516E4();
    default:
        break;
    case 7:
        func_800AE0F0(D_8015E74C, "紐緒「馬鹿な事言ってるんじゃないわ");
        func_8014B738((s32) &D_8015E814, 1, 0x1F4);
        func_8013F1C4(0x27, 0x0600002D, 0);
        func_8013E810(0x1D, 0xB, 1, 1);
        func_8014EBA8();
        break;
    case 8:
        func_8014ED44(0x1E);
        break;
    case 9:
        temp_v0 = D_8015EDB4 + 1;
        D_8015EDB4 = temp_v0;
        if (temp_v0 >= 0x12D) {
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
        func_8013673C();
        break;
    case 11:
        func_8013673C();
        break;
    case 12:
        func_8013673C();
        break;
    case 13:
        func_8013673C();
        break;
    case 14:
        func_8013673C();
        break;
    case 15:
        func_8014ED44(0x1E);
        break;
    case 16:
        func_80151984();
        break;
    case 17:
        D_8015EDAC = 0x64;
        break;
    }
}

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
