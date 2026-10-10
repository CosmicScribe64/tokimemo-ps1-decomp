#include "common.h"
#include "game.h"
#include "main_only.h"


void don_wait(void) {
    D_800E7384 += 1;
    if ((u32) D_800E7384 >= 0x81U) {
        k_reset(1);
        func_8004284C();
    }
}
