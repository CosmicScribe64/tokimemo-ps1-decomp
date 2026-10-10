#include "common.h"
#include "game.h"
#include "main_only.h"

u8 func_80073AD8();
u8 func_800741B8();
u8 func_8007437C();

u8 week_day_init(void) {
    D_800E7384 += 1;
    switch (D_800E738A) {                           /* irregular */
    case 0:
        return func_80073AD8();
    case 1:
        return func_800741B8();
    case 2:
        return func_8007437C();
    default:
        return D_800E738A;
    }
}
