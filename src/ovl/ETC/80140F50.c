#include "common.h"
#include "ovl/ETC.h"

u8 *func_80140F50(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
            return D_80150128;
        }
        return D_8015012C;
    }
    return D_80150108;
}

u8 *func_80140F94(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
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
    if (D_800E7208 & 0x20) {
        func_8004284C();
    } else if (D_800E7208 & 0x40) {
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

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142974);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142A4C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80142AE8);
