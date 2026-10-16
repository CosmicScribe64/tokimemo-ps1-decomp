#include "common.h"
#include "ovl/TACO.h"

void func_8015A900(void) {
    D_801604C0 = 0;
    D_801604C4 += 1;
    D_801604D0 = 0;
}

void func_8015A928(void) {
    D_801604C8 = 0;
    D_801604CC += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015A900", func_8015A948);

void func_8015AB18(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 += 0x100;
    if ((arg0 + D_8015EDB4)->unk78 > 0x1000) {
        func_8015C208();
    }
}

void func_8015AB80(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 -= 0xFA;
    if ((arg0 + D_8015EDB4)->unk78 < -0x7530) {
        func_8015C208();
    }
}
