#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80141C30(void) {
    if (func_80156870() != 0) {
        func_80141C70();
        return;
    }
    func_80141E38();
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80141C30", func_80141C70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80141C30", func_80141E38);
