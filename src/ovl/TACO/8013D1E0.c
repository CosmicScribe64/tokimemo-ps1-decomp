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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D348);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D3F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D528);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D838);
