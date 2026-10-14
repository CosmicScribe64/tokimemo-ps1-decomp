#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_80137640(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x3B);
            return;
        case 0x60:
            func_80042908(0x3C);
            return;
        case 0x61:
            func_80042908(0x3D);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x3E);
        return;
    case 2:
        func_80042908(0x3F);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x40);
            return;
        case 0x60:
            func_80042908(0x41);
            return;
        case 0x61:
            func_80042908(0x42);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x43);
            return;
        case 0x60:
            func_80042908(0x44);
            return;
        case 0x61:
            func_80042908(0x45);
            return;
        }
        break;
    case 5:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x47);
            return;
        }
        func_80042908(0x46);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137804);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137A38);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137CD8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80137FB8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138340);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_801386C0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138A08);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80138EB8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139184);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139364);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_8013954C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139780);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80137640", func_80139A6C);

void func_80139DEC(void) {
    func_800847B8(4);
    func_8013223C();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
