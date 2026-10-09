#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800410AC);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, store order: the original stores h, w before x, y. */
void func_8004111C(void) {
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 480;
    func_8009C7F8(&rect, 0, 0, 0);
    func_8009C674(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80041000", func_8004111C);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041168);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800412E0);

void func_80041584(void) {
    func_800415B4(0, 0x40);
    func_8004164C(0, 2);
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800415B4);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_8004164C);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041840);

void func_80041878(void) {
    func_800418B0();
    func_800419FC();
    func_80041C2C();
    func_80041F48();
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800418B0);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800419FC);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041C2C);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041F48);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80042058);
