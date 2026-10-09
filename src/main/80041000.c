#include "common.h"
#include "game.h"
#include "libapi.h"

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

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800420D0);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80042134);

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_800422C8);

void func_800423D4(void) {
    D_800E7374 += 1;
    D_800E7D10 += 0x377;
}

/* The named temp gives the original's v1/v0 registers; `D += 0x377; return D;`
 * and `return D += 0x377;` load into t6 instead (T-0014). */
s32 func_80042400(void) {
    s32 t = D_800E7D10 + 0x377;

    D_800E7D10 = t;
    return t;
}

void func_80042418(void) {
    bzero(&D_800E7D10, 0xEE0);
    func_80042488();
    card_ev_set();
    func_80042C30();
}

void func_80042458(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80042488);

void func_800424FC(void) {
    EnterCriticalSection();
    CloseEvent(D_8011ECA8);
    ExitCriticalSection();
}
