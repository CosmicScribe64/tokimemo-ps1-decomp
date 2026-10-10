#include "common.h"
#include "ovl/OLH.h"

void func_80133B80(void) {
    switch (D_800E738A) {
    case 0x0:
        func_80133C4C();
        break;
    case 0x1:
        func_80133D74();
        break;
    case 0x10:
        func_80133EA4();
        break;
    case 0x20:
        func_80133F90();
        break;
    case 0x30:
        func_80134060();
        break;
    case 0x40:
        func_80134184();
        break;
    case 0x50:
        func_80134254();
        break;
    case 0x60:
        func_80134324();
        break;
    case 0x70:
        func_801343F4();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80133C4C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80133D74);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80133EA4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80133F90);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80134060);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80134184);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80134254);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80134324);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_801343F4);
