#include "common.h"
#include "ovl/TAIIKU.h"

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80132000", func_80132000);

/* The GameState pointer makes the record stores indirect, so the runner list
 * byte is reloaded for each use as in the original (T-16554). */
void func_80132970(void) {
    GameState *gs;
    s32 i;

    gs = &D_800E6280;
    for (i = 0; i < 3; i++) {
        gs->unk_75E[gs->unk_F5E].unk_00 = D_80148EC0[i];
        gs->unk_75E[gs->unk_F5E].unk_02 = 0x13;
        gs->unk_75E[gs->unk_F5E].unk_01 = 0;
        gs->unk_75E[gs->unk_F5E].unk_03 = get_g_zyotai_h(D_80148EC0[i]);
        if ((i == 2) && (gs->unk_1BC[D_80148EC0[i]].unk_0A >= 0x4C)) {
            gs->unk_75E[gs->unk_F5E].unk_03 = 4;
        }
        gs->unk_75E[gs->unk_F5E].unk_04 = gs->unk_03E;
        gs->unk_75E[gs->unk_F5E].unk_05 = gs->unk_03F;
        gs->unk_75E[gs->unk_F5E].unk_06 = gs->unk_040;
        gs->unk_75E[gs->unk_F5E].unk_07 = gs->unk_041;
        gs->unk_F5E++;
        if (gs->unk_F5E >= 0xFF) {
            gs->unk_F5E = 0xFE;
        }
    }
}

