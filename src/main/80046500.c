#include "common.h"
#include "game.h"

s32 func_80046500(void) {
    func_800789E0();
    func_800412E0(0);
    func_80042878(0x12);
    func_80041584();
    func_80048DD0(0);
    back_clear_switch(1);
    dec_bg_reset();
    func_8004482C();
    tpage_buf_clear();
    func_80044750(0x7F);
    func_8007BDE8();
    func_8007B844();
    func_800452C4();
    func_80041878();
    func_80042058();
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046590);

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046684);

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046754);

INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046AC8);
