#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004C3F0);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004D088);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004D320);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004D3C8);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004D6B8);

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004DACC);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004DDF0);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004E0FC);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004E350);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", func_8004E44C);
