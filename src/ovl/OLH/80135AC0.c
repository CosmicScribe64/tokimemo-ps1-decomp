#include "common.h"
#include "ovl/OLH.h"

void func_80135AC0(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80135B8C();
        break;
    case 0x1:
        func_80135CB4();
        break;
    case 0x10:
        func_80135DD4();
        break;
    case 0x20:
        func_80135EC0();
        break;
    case 0x30:
        func_80136000();
        break;
    case 0x40:
        func_80136124();
        break;
    case 0x50:
        func_80136210();
        break;
    case 0x60:
        func_801362C4();
        break;
    case 0x70:
        func_801363CC();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80135B8C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80135CB4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80135DD4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80135EC0);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80136000);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80136124);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80136210);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_801362C4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_801363CC);
