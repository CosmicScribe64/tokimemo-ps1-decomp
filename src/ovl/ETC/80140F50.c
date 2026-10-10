#include "common.h"
#include "ovl/ETC.h"

u8 *func_80140F50(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_80150128;
        }
        return D_8015012C;
    }
    return D_80150108;
}

u8 *func_80140F94(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_8015012C;
        }
        return D_80150128;
    }
    return D_80150124;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80140FD8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141154);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801413FC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141468);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801415A4);

void func_80141700(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004482C();
        func_80042878(0xC4);
    }
    func_800578F4(2);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_8014175C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141980);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141ABC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141BE8);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141E10);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801421AC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142390);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142574);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_801428EC);

void func_80142974(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80143644();
        break;
    case 1:
        func_80143FE0();
        break;
    }
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_F88 & 0x200000) {
            if (D_800E6280.unk_1124 != 0) {
                func_80042878(0x11);
            } else {
                func_80042878(0xC4);
            }
        } else if (D_800E6280.unk_F88 & 0x400000) {
            if (D_800E6280.unk_1124 != 0) {
                func_80042878(0x11);
            } else {
                func_80042878(0x91);
            }
        }
    }
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142A4C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142AE8);
