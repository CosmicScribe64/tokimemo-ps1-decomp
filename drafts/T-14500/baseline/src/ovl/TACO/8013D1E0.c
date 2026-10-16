#include "common.h"
#include "ovl/TACO.h"

/* FAKE: integer fallthrough retains the original index/pointer allocation. T-9550 */
s32 func_8013D1E0(TcPos position, TcPos velocity) {
    TacoParticle14 *particle;
    s32 index;
    index = D_8015E9F0[0].unk_00.unk6 % 16;
    particle = &D_8015E9F0[index];
    particle->unk_00 = position;
    particle->unk_08 = velocity;
    particle->unk_12 = 1;
    particle->unk_11 = 1;
    particle->unk_13 = 0x80;
    if (D_8015E9F0[0].unk_00.unk6++ >= 17) D_8015E9F0[0].unk_00.unk6 = 0;

}

/* FAKE: integer fallthrough retains original index/pointer allocation. T-9550 */
s32 func_8013D2A0(s16 x, s16 y, s16 z) {
    TacoParticle14 *particle;
    s32 index;
    index = D_8015E8B0[0].unk_00.unk6 % 16;
    particle = &D_8015E8B0[index];
    particle->unk_00.x = x;
    particle->unk_00.y = y;
    particle->unk_00.z = z;
    particle->unk_12 = 1;
    particle->unk_11 = 1;
    particle->unk_13 = 0x80;
    if (D_8015E8B0[0].unk_00.unk6++ >= 17) D_8015E8B0[0].unk_00.unk6 = 0;

}

/* FAKE: integer fall-through preserves the original loop/result registers. T-10010. */
s32 func_8013D348(void) {
    TacoParticle14 *particle;
    u32 i;

    for (i = 0; i < 16; i++) {
        particle = &D_8015E8B0[i];
        if (particle->unk_13 != 0) {
            func_8013D528((s32)particle->unk_00.x, (s32)particle->unk_00.y, (s32)particle->unk_00.z, particle->unk_11 - 1);
            if (particle->unk_12++ >= 5U) {
                particle->unk_12 = 0;
                if (particle->unk_11++ >= 7U) {
                    particle->unk_13 = 0;
                    particle->unk_11 = 0;
                }
            }
        }
    }
}

void func_8013D3F4(void) {
    TacoParticle14 *particle;
    s16 offset_x;
    s16 offset_y;
    u32 i;

    offset_x = func_800AE0D0() & 0x7F;
    offset_y = func_800AE0D0() & 0x3F;

    for (i = 0; i < 16; i++) {
        particle = &D_8015E9F0[i];
        if (particle->unk_13 != 0) {
            func_8013D838((s32) particle->unk_00.x, (s32) particle->unk_00.y, (s32) particle->unk_00.z, particle->unk_11 - 1);
            func_8013D838((s32) (s16) (particle->unk_00.x + offset_x), (s32) (s16) (particle->unk_00.y + offset_y), (s32) particle->unk_00.z, particle->unk_11 + 2);
            particle->unk_00.x += particle->unk_08.x;
            particle->unk_00.y += particle->unk_08.y;
            particle->unk_00.z += particle->unk_08.z;
            if (particle->unk_12++ >= 5U) {
                particle->unk_12 = 0;
                if (particle->unk_11++ >= 7U) {
                    particle->unk_13 = 0;
                    particle->unk_11 = 0;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D528);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D838);
