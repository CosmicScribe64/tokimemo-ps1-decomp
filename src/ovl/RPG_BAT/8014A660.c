#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014A660);

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, only the register of the reloaded arg0 differs ($a3 vs $t9), 6 lines */
void func_8014AB84(s32 arg0, s32 arg1, s32 arg2) {
    func_8013F0F4(0x500, 1, 0);
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 4 + 0xC8) = 1;
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 8 + 0xDC) = arg1;
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 8 + 0xE0) = arg2;
    func_8013E97C(arg0 * 3 + 4, arg1, arg2);
    func_8013E97C(arg0 * 3 + 5, arg1, arg2);
    func_8013E97C(arg0 * 3 + 6, arg1, arg2);
    func_8013E810(arg0 * 3 + 4, 6, 5, 1);
    func_8013E810(arg0 * 3 + 5, 8, 1, 1);
    func_8013E810(arg0 * 3 + 6, 0xD, 5, 1);
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014AB84);
#endif

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014AC74);
