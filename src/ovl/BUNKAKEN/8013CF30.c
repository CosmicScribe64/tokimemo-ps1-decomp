#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

void func_8013CF30(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x84);
            return;
        }
        func_80042908(0x85);
        return;
    case 1:
        func_80042908(0x86);
        return;
    case 2:
        func_80042908(0x87);
        return;
    case 3:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x88);
            return;
        }
        func_80042908(0x87);
        return;
    case 4:
        if (D_800E6280.unk_03E == 0x60) {
            func_80042908(0x89);
            return;
        }
        func_80042908(0x8A);
        return;
    case 5:
        func_80042908(0x8B);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D048);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D1F4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D4D8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013D82C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013DBE8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E2C0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E578);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013CF30", func_8013E7C4);

void func_8013EAC8(void) {
    func_800847B8(10);
    func_801324AC();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
