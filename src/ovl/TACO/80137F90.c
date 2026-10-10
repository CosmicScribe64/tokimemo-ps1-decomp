#include "common.h"
#include "ovl/TACO.h"

void func_80137F90(void) {
    D_800E7384 += 1;
    switch (D_800E738A) {
    case 0:
        func_8013801C();
        break;
    case 1:
        func_80138160();
        break;
    case 2:
        func_8013822C();
        break;
    }
    func_8013F250();
    func_80132FE8();
}
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137F90", func_8013801C);

void func_80138160(void) {
    func_8013A790(1, 1);
    if (D_8015EDBC == 0) {
        func_800450F4(0, 0x203);
        load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137F90", func_8013822C);
