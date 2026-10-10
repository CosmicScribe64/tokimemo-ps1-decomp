#include "common.h"
#include "game.h"
#include "main_only.h"

extern s16 D_80120658;
extern s16 D_801206EE;
extern s16 D_801206F0;

void func_80086640(void) {
    s16 temp_v0;

    temp_v0 = D_80120666;
    D_80120666 = D_801206EE;
    D_801206EE = temp_v0;
    D_80120668 = 0;
    D_801206F0 = 0;
    D_80120658 = 0;
}
