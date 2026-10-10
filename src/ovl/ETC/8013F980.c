#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013F980", func_8013F980);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013F980", func_8013FA7C);

void func_8013FC08(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x280, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013F980", func_8013FC64);
