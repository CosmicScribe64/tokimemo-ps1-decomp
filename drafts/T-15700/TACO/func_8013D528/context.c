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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
extern s32 D_80129C40;
s32 func_800A1070();
/* end bulk_m2c declarations */
s32 func_8013D528(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 sp88;
    s32 sp82;
    s32 sp8E;
    s32 sp86;
    s32 sp8C;
    s32 sp8A;
    s32 sp84;
    s32 sp98[8];
    s16 sp94;
    s16 sp92;
    s16 sp90;
    s16 sp80;
    s32 sp74[1];
    s32 sp70[1];
    s16 *var_s1;
    s16 temp_v0;
    s16 var_fp;
    s16 var_s0;
    s32 temp_v0_2;
    s32 var_at;
    s32 var_s1_2;
    s32 var_s7;
    s8 temp_a1;
    s8 temp_t6;
    u8 *var_s2;

    (*(s32 *)&sp98) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&sp98) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&sp98) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&sp98) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&sp98) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&sp98) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&sp98) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&sp98) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &sp98);
    func_800A1070(&sp98);
    var_fp = arg1 - 0x80;
    var_s7 = 0;
    sp94 = arg2;
    do {
        var_s1 = &sp80;
        sp92 = var_fp;
        var_s2 = var_s7 + var_s1;
        var_s0 = arg0 - 0x80;
loop_2:
        sp90 = var_s0;
        func_800AB170((s32) &sp90, (s32) var_s2, (s32) &sp74, (s32) &sp70);
        var_s1 += 8;
        var_s0 += 0x100;
        var_s2 += 8;
        if (var_s1 != &sp90) {
            goto loop_2;
        }
        var_s7 += 4;
        var_fp += 0x100;
    } while (var_s7 != 8);
    if ((sp88 >= -0x2C0) && (sp80 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp86 >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp8E) {
                var_s1_2 = (s32) (D_80122754 - arg2) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                    goto block_12;
                }
                var_at = var_s1_2 < 0x21;
                if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
block_12:
                    var_at = var_s1_2 < 0x21;
                }
                if (var_at == 0) {
                    var_s1_2 = var_s1_2 / 2;
                } else if (var_s1_2 >= 6) {
                    var_s1_2 -= 4;
                }
                temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                SetSemiTrans(temp_v0_2, 1);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(arg3 * 0x10, 0x1E4);
                temp_t6 = arg3 << 5;
                temp_a1 = temp_t6 + 0x1F;
                *(s8 *)((unsigned char *)temp_v0_2 + (20)) = temp_a1;
                *(s8 *)((unsigned char *)temp_v0_2 + (36)) = temp_a1;
                *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_t6;
                *(s8 *)((unsigned char *)temp_v0_2 + (28)) = temp_t6;
                *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x1F;
                *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x1F;
                *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0;
                *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp80;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp82;
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp84;
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp86;
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp88;
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp8A;
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp8C;
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp8E;
                AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s1_2 * 4)], temp_v0_2, 0x1F);
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D838);
