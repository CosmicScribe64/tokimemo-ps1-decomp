#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80139E50(void) {
    switch (D_80122EB8) {
    case 0:
        if (D_800E62BE == 0x61) {
            func_80042908(7);
            return;
        }
        func_80042908(6);
        return;
    case 1:
        func_80042908(8);
        return;
    case 2:
        func_80042908(9);
        return;
    case 3:
        switch (D_800E62BE) {
        case 0x5F:
            func_80042908(0xA);
            return;
        case 0x60:
            func_80042908(0xC);
            return;
        case 0x61:
            func_80042908(0xD);
            return;
        }
        break;
    case 4:
        switch (D_800E62BE) {
        case 0x5F:
            func_80042908(0xE);
            return;
        case 0x60:
            func_80042908(0xF);
            return;
        case 0x61:
            func_80042908(0x10);
            return;
        }
        break;
    case 5:
        func_80042908(0x11);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_80139FB8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A118);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A2FC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A518);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A6F4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013A8CC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013ABF0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013ADC4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013AF00);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013B170);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80139E50", func_8013B2AC);

void func_8013B4A0(void) {
    func_800847B8(0);
    func_801324E0();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
