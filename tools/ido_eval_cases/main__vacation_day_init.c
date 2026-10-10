#include "common.h"
#include "game.h"
#include "main_only.h"

u8 func_80074DE0();
u8 func_80074F24();
u8 func_8007505C();

u8 vacation_day_init(void) {
    D_800E7384 += 1;
    switch (D_800E738A) {                           /* irregular */
    case 0:
        return func_80074DE0();
    case 1:
        return func_80074F24();
    case 2:
        return func_8007505C();
    default:
        return D_800E738A;
    }
}
