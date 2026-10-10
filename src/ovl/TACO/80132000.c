#include "common.h"
#include "ovl/TACO.h"

void func_80132000(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80133870();
        break;
    case 1:
        func_801447D0();
        break;
    case 2:
        func_80134D8C();
        break;
    case 3:
        func_80135394();
        break;
    case 4:
        func_80135744();
        break;
    case 5:
        func_80136B40();
        break;
    case 6:
        func_80137590();
        break;
    case 7:
        func_80137A60();
        break;
    case 10:
        func_80137F90();
        break;
    case 11:
        func_80138280();
        break;
    case 12:
        func_801480B0();
        break;
    case 21:
        func_80138450();
        break;
    case 22:
        func_80148FE0();
        break;
    case 31:
        func_80138680();
        break;
    case 32:
        func_80149F90();
        break;
    case 8:
        func_80138890();
        break;
    case 9:
        func_80139170();
        break;
    case 45:
        func_8013B170();
        break;
    case 46:
        func_80142820();
        break;
    default:
        func_80046500();
        break;
    }
    func_80143E9C();
    func_801446D8();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132000", func_801321A0);
