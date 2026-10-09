#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061710);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061790);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800618B0);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006190C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061A3C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061EFC);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80061FC4);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006211C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_8006218C);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062210);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800623E4);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062524);

void func_800625C0(void) {
    u8 *p = D_8011ECD0 + D_801230D0 * 0x44;

    p[0x19C7] |= 0x40;
}

void func_800625F4(void) {
    func_800625C0();
    if ((u32)D_800E7384 >= 0x21U) {
        func_80042808();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062634);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800626B0);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062764);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_800627DC);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062840);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062948);

INCLUDE_ASM("asm/nonmatchings/main/80061710", func_80062C28);
