#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013AD50);

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
