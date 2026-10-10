#include "common.h"
#include "game.h"
#include "libapi.h"

void func_80041000(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    func_800985A0();
    D_8011ECA0 = func_80098370();
    func_800410AC();
}

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

void func_80041840(void) {
    D_800E71F4 = 1;
    D_800E71EF = 3;
    D_800E7312 = 1;
    D_80123110 = 0x80132000;
}

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

void func_80042058(void) {
    D_800E71DF = 0;
    D_800E71E0 = 0;
    D_800E71E1 = 0;
    D_800E71E2 = 0;
    D_800E71E3 = 0;
    D_800E71EC = 0;
    D_800E62BC = 2;
    D_800E71F0 = 0;
    D_800E71F1 = 0;
    D_800E71F3 = 0x20;
    D_800E71F5 = 0;
    D_800E71ED = 0xE;
    D_800E71EE = 0;
}
