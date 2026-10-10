#include "common.h"
#include "game.h"
#include "main_only.h"


void normal_date_move_place(void) {
    switch (D_800E7384) {                           /* irregular */
    case 0:
        normal_date_move_place_init();
        return;
    case 1:
        normal_date_move_place_main();
        return;
    default:
        func_80046500();
        return;
    }
}
