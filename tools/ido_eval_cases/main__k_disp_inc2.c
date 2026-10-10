#include "common.h"
#include "game.h"
#include "main_only.h"

extern s32 D_80122CFC;

void k_disp_inc2(void) {
    u8 sp2B;

    D_80122CFC -= 1;
    if (D_80122CFC < 1) {
        D_80122CFC = 0;
        sp2B = get_k_speed();
        if (D_800E7200 & 0x600060) {
            k_speed_set(0U);
        }
        k_disp_inc();
        k_speed_set(sp2B);
    }
}
