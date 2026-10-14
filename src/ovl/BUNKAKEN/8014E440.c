#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_8014E440(void) {
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x63);
            return;
        case 0x60:
            func_80042908(0x64);
            return;
        case 0x61:
            func_80042908(0x65);
            return;
        default:
            func_80046500();
            return;
        }
        break;
    case 1:
        func_80042908(0x66);
        return;
    case 2:
        func_80042908(0x67);
        return;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x68);
            return;
        case 0x60:
            func_80042908(0x69);
            return;
        case 0x61:
            func_80042908(0x6A);
            return;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80042908(0x6B);
            return;
        case 0x60:
            func_80042908(0x6C);
            return;
        case 0x61:
            func_80042908(0x6D);
            return;
        }
        break;
    case 5:
        func_80042908(0x6E);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014E5E0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014E890);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014EAF0);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014EE54);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F214);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F624);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014F9D8);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8014FEF4);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150244);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_8015049C);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150680);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8014E440", func_80150958);

void func_80150CC8(void) {
    func_800847B8(7);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
