#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_80156870(void) {
    if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2) {
        return 1;
    }
    return 0;
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7208;
extern s8 D_80120E0A;
/* end bulk_m2c declarations */
void func_801568A4(void) {
    s32 temp_v1;
    s32 temp_v1_2;

    switch (D_8015EE04) {
    case 0:
        if (func_80156870() != 0) {
            func_800AE0F0(D_8015E904, "清川「フザケンナヨー！");
            func_800AE0F0(D_8015E92C, "あっ！いや、なんでもない・・・");
        } else {
            func_800AE0F0(D_8015E904, "清川「フザケンナヨー！");
            func_800AE0F0(D_8015E92C, "馬鹿野郎ーっ！");
        }
        func_8014F230();
    default:
        break;
    case 1:
        func_80143C28(1, 0);
        break;
    case 2:
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8013F15C(0x511, 1, 0);
        func_8014F230();
        break;
    case 3:
        if (D_80120E1C == 4) {
            func_8014B738((s32) D_8015E9CC, 2, 0x50);
            if (func_80156870() != 0) {
                func_8013F1C4(0x1F00005A, 0x1000005E, 0);
            } else {
                func_8013F1C4(0x1F00005F, 0x11000063, 0);
            }
            func_8014F230();
        }
        break;
    case 4:
        func_8014F448(1);
        break;
    case 5:
        temp_v1 = D_8015EE0C + 1;
        D_8015EE0C = temp_v1;
        if (temp_v1 >= 0x12D) {
            func_8014F230();
        }
        if (func_800460EC() == 2) {
            func_8014F230();
        }
        break;
    case 6:
        if (func_80156870() != 0) {
            D_8015EE48 = 3;
            func_8013F1C4(0x03000011, 0x0300001C, 1);
            func_8013E7C0(0x1D, 7, 5, 1);
        } else {
            func_8013F1C4(0x1F000064, 0x12000068, 1);
            D_80120E0A = 0x82;
            func_8013E7C0(0x1D, 6, 5, 1);
        }
        func_8014F230();
        break;
    case 7:
        D_8015E744 = 0x5A;
        if (func_8013E90C(0x1D) != 0) {
            if (func_80156870() != 0) {
                func_8014F230();
            } else {
                D_8015EE04 = 0x14;
            }
        }
        break;
    case 8:
        func_8013E7C0(0x1D, 7, 5, 1);
        func_8014F230();
        break;
    case 9:
        if (D_800E7208 & 0x40) {
            func_8013E7C0(0x1D, 7, 5, 1);
            if (D_8015EE48 == 3) {
                D_8015EE48 = 1;
            }
            D_8015EE04 = 0x14;
        }
        if (func_8013E90C(0x1D) != 0) {
            func_8013E7C0(0x1D, 7, 5, 1);
            temp_v1_2 = D_8015EE0C + 1;
            D_8015EE0C = temp_v1_2;
            if (temp_v1_2 >= 5) {
                if (D_8015EE48 == 3) {
                    D_8015EE48 = 1;
                }
                D_8015EE04 = 0x14;
            }
        }
        break;
    case 20:
        func_8014F350(0x28);
        break;
    case 21:
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8014F230();
        break;
    case 22:
        D_80120E0A = 1;
        func_8013E7C0(0x1D, 3, 1, 1);
        func_8014F230();
        break;
    case 23:
        func_80143CE8(3, 0);
        break;
    case 24:
        func_8014F350(0x14);
        break;
    case 25:
        func_8014F278();
        break;
    }
}
