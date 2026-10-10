#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B170);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B2A0);

void func_8013B398(void) {
    if (D_800E6280.unk_F88 != 0) {
        func_8004284C();
    }
}

void func_8013B3C4(void) {
    if (D_8015EDBC == 0) {
        if (func_80044C98() == 1) {
            func_80042908(8);
        }
    }
}

void func_8013B404(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B460);
