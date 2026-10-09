#include "common.h"
#include "game.h"
#include "libapi.h"

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_800420D0);

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_80042134);

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_800422C8);

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_800423D4);

/* The named temp gives the original's v1/v0 registers; `D += 0x377; return D;`
 * and `return D += 0x377;` load into t6 instead (T-0014). */
s32 func_80042400(void) {
    s32 t = D_800E7D10 + 0x377;

    D_800E7D10 = t;
    return t;
}

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_80042418);

void func_80042458(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_80042488);

INCLUDE_ASM("asm/nonmatchings/main/800420D0", func_800424FC);
