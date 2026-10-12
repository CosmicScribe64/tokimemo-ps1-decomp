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

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, temp numbering only: the original builds the D_8015EBAC base in $t0 before the i*8 product, 16 lines */
void func_8014AC74(void) {
    s32 i;
    s32 *p;
    u8 *rec;
    s32 t;
    s32 t5;
    u8 *ent;
    s32 v;

    p = &D_8015EBAC;
    i = 0;
    do {
        if (p[0x32] == 1) {
            t = i * 3;
            rec = D_8011ECD0 + t * 0x44;
            t5 = t + 5;
            ent = (u8 *)&D_8015EBAC + i * 8;
            if (!(rec[0x1A92] & 1)) {
                func_8013E810(t + 4, 7, 1, 1);
            }
            v = *(s16 *)(rec + 0x1B30);
            if (v >= 0x10) {
                v = 0x1E - v;
            }
            func_8013E97C(t5, *(s32 *)(ent + 0xDC), *(s32 *)(ent + 0xE0) - v * 8);
            if (!(rec[0x1B1A] & 1)) {
                func_8013E810(t + 4, 0xFF, 1, 0);
                func_8013E810(t5, 0xFF, 1, 0);
                func_8013E810(t + 6, 0xFF, 1, 0);
            }
        }
        i++;
        p++;
    } while (i != 4);
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014AC74);
#endif
