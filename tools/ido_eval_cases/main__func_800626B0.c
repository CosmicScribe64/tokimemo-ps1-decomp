#include "common.h"
#include "game.h"
#include "main_only.h"


void func_800626B0(void) {
    func_8004AC18(0xFF);
    switch (D_801230D0) {                           /* irregular */
    case 0:
        func_80042878(0x13);
        break;
    case 1:
        func_80042878(0x20);
        if (D_800B3220 == 1) {
            if (D_800E7200 & 0x2000) {
                func_80042878(0x21);
            } else if (D_800E7200 & 0x8000) {
                func_80042878(0x22);
            }
        }
        break;
    default:
        func_80046500();
        break;
    }
}
