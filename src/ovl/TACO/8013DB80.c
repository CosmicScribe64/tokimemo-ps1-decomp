#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DB80);

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, IDO shares the D_8015EB30 address between the two loops, the original rebuilds it (8 differing lines) */
void func_8013DDE4(void) {
    TcPart *p;
    s32 i;

    p = D_8015EB30;
    do {
        p->unk_12 = p->unk_12 + 1;
        if (p->unk_12 == 0xFF) {
            p->unk_13 = 0;
        }
        if (p->unk_04 >= 0x2711) {
            p->unk_13 = 0;
        }
        if (p->unk_13 & 0x80) {
            p->unk_00 = p->unk_00 + p->unk_08;
            p->unk_02 = p->unk_02 + p->unk_0A;
            p->unk_04 = p->unk_04 + p->unk_0C;
        }
        p++;
    } while (p < &D_8015EC70);
    p = D_8015EB30;
    i = 0;
    do {
        if (p->unk_13 & 0x80) {
            func_8013DEE4(i);
        }
        i++;
        p++;
    } while (i != 0x10);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DDE4);
#endif

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013DEE4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E424);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E4FC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013E778);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013DB80", func_8013EAA4);
