#include "common.h"
#include "ovl/EN_NICHI.h"

extern s8 D_80121533;
extern s16 D_80121548;
extern s32 D_80139AFC;

void func_80132DC4(void) {
    if (D_80139AFC == 0) {
        func_80044750(0x504);
    }
    D_80139AFC += 1;
    D_80121548 = 0x1E;
    func_80132E3C();
    if (D_80139AFC >= 0xB4) {
        D_80121533 = 0;
        D_80121531 = 3;
        func_80042808();
    }
}
