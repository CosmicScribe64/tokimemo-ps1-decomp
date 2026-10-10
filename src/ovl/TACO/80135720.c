#include "common.h"
#include "ovl/TACO.h"

void func_80135720(void) {
    D_8015F3F0 = 0;
    D_8015F3F4 = 0;
    D_8015F3F8 = 0;
    D_8015F3FC = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80135744);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801357B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_8013587C);

void func_80135944(void) {
    RECT r;

    r.x = 0x2C0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x14;
    func_8009C7F8(&r, 0, 0, 0);
    func_8009C674(0);
}

void func_80135994(void) {
    func_80042908(2);
}

void func_801359B4(void) {
    if (D_8015F3F4 == 1) {
        func_80135F6C();
    } else {
        func_80136988();
    }
    func_800578F4(2);
    func_80135A04();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80135A04);

void func_80135F6C(void) {
    D_8015F3FC += 1;
    switch (D_8015F3F0) {
    case 0:
        func_80136024();
        break;
    case 1:
        func_801361E8();
        break;
    case 2:
        func_80136438();
        break;
    case 3:
        func_80136688();
        break;
    }
    if (D_8015F3FC >= 0x1F) {
        D_8015F3FC = 0;
        D_8015F3F4 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136024);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801361E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136438);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136688);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801368D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136988);
