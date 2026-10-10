#include "common.h"
#include "ovl/OLH.h"

void func_80134500(void) {
    switch (D_800E738A) {
    case 0x0:
        func_801345CC();
        break;
    case 0x1:
        func_801346F4();
        break;
    case 0x10:
        func_80134824();
        break;
    case 0x20:
        func_801348F4();
        break;
    case 0x30:
        func_801349FC();
        break;
    case 0x40:
        func_80134B3C();
        break;
    case 0x50:
        func_80134C28();
        break;
    case 0x60:
        func_80134D14();
        break;
    case 0x70:
        func_80134E00();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_801345CC);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_801346F4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_80134824);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_801348F4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_801349FC);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_80134B3C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_80134C28);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_80134D14);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134500", func_80134E00);
