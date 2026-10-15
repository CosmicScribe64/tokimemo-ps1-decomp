#include "common.h"
#include "ovl/RPG_BAT.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6354;
extern s32 D_8015EDA4;
s32 func_800AE100();
/* end bulk_m2c declarations */
void func_8013AD50(void) {
    s32 temp_lo;

    if (!(D_8015EDF0 & 0xFF)) {
        if (D_8015ED98 & 0x100) {
            func_8013E7C0(0x30, 3, 1, 1);
        } else {
            func_8013D030();
        }
    }
    D_8015EDB4 += 1;
    switch (D_8015EDAC) {
    case 0:
        if (!(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF)) {
            if (D_8015EDB4 >= 0x97) {
                D_8015ED98 = 0x104;
                temp_lo = (s32) D_8015EDA0 / 20;
                D_8015EDA4 = temp_lo;
                if (temp_lo == 0) {
                    D_8015EDA4 = 1;
                }
                func_800AE0F0(D_8015E74C, &D_800E6354);
                func_800AE100(D_8015E74C, "の体力が５％回復した");
                func_8014B738((s32) &D_8015E814, 1, 0x3C);
                func_8014EBA8();
            }
        default: ;
        }
        break;
    case 1:
        func_8014EE3C(1);
        break;
    case 2:
        func_8013EA60(0, 0x21, D_8015EDA4, 0);
        func_8013F15C(0x502, 1, 0);
        func_8014EBA8();
        break;
    case 3:
        func_8014ED44(0x14);
        break;
    case 4:
        func_8014EBEC();
        D_8015ED9C = func_801445E8(1);
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013AF1C);

s32 func_8013B2F0(void) {

    if (!(D_8015EC74 & 1)) {
        switch (D_8015EDB0.unk_00) {
        case 0:
            func_8013E7C0(0x30, 6, 5, 1);
            func_8014EBD0();
            return;
        case 1:
            /* FAKE: s32 view and dead test keep counter in v1; source unknown. T-9730. */
            if (!D_8015EC74) {
            }
            func_8014ED80(4);
            return;
        case 2:
            func_8013E810(0x31, 0xB, 5, 1);
            func_8013F15C(0x501, 1, 0);
            func_8014EBD0();
            return;
        case 3:
            if (func_8013E90C(0x31) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
            }
            if (func_8013E90C(0x30) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
                if (--D_8015EC74 == 0) {
                    func_8014EBA8();
                    return;
                }
                D_8015EDB0.unk_00 = 0;
            }
            break;
        }
    } else {
        switch (D_8015EDB0.unk_00) {
        case 0:
            func_8013E7C0(0x30, 6, 5, 1);
            func_8014EBD0();
            return;
        case 1:
            func_8014ED80(4);
            return;
        case 2:
            func_8013E810(0x31, 0xC, 5, 1);
            func_8013F15C(0x501, 1, 0);
            func_8014EBD0();
            return;
        case 3:
            if (func_8013E90C(0x31) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
            }
            if (func_8013E90C(0x30) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
                if (--D_8015EC74 == 0) {
                    func_8014EBA8();
                    return;
                }
                D_8015EDB0.unk_00 = 0;
            } else {
                return;
            }
            break;
        }
    }
}

void func_8013B534(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x30, 6, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (func_8013E90C(0x30) != 0) {
            func_8014EBA8();
        }
        break;
    }
}
