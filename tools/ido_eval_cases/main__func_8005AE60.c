#include "common.h"
#include "game.h"
#include "main_only.h"

extern u8 D_800E739B;

void func_8005AE60(void) {
    if (D_800E738A == 0) {
        func_8005B39C();
    }
    func_8005AD70();
    if (D_800E739B == 1) {
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            D_800E739B = 0;
            func_80053D10();
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80044750(0x502);
            D_800E739B = 0;
            func_80053D10();
        }
    } else if (D_800E739B == 0) {
        func_80065900(0);
    }
}
