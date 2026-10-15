#include "common.h"
#include "game.h"

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E62BA;
/* end bulk_m2c declarations */
void srn_tpage_show(u8 arg0, u8 arg1, u8 arg2, void *arg3, s32 arg4, s16 arg6, s16 arg7, s32 arg8) {
    s32 sp6C;
    s32 sp68;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 temp_a3;
    s32 temp_t0;
    s32 temp_t2;
    s32 temp_t4;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_8;
    s32 temp_v1;
    s8 temp_t6;
    s8 temp_v0_3;
    s8 temp_v0_7;
    s8 temp_v1_2;
    s8 temp_v1_3;
    u8 temp_s0;

    temp_s0 = arg1 & 0xFF;
    switch (temp_s0) {
    case 0:
        sp68 = 0x40;
        sp6C = 4;
        break;
    case 1:
        sp68 = 0x80;
        sp6C = 2;
        break;
    case 2:
        sp68 = 0x100;
        sp6C = 1;
        break;
    }
    arg1 = temp_s0;
    temp_v0 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0, 1);
    } else {
        SetSemiTrans(temp_v0, 0);
    }
    SetShadeTex(temp_v0, 0);
    *(u8 *)((unsigned char *)temp_v0 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0 + (5)) = (u8) D_800E62BA;
    temp_t6 = (arg0 & 0x20) * 4;
    *(s8 *)((unsigned char *)temp_v0 + (20)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_t6;
    *(s8 *)((unsigned char *)temp_v0 + (21)) = temp_t6;
    *(s8 *)((unsigned char *)temp_v0 + (28)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
    *(u8 *)((unsigned char *)temp_v0 + (6)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0 + (36)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (29)) = (s8) (temp_t6 + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s8 *)((unsigned char *)temp_v0 + (37)) = (s8) (temp_t6 + *(s16 *)((unsigned char *)arg3 + (6)));
    temp_t2 = arg4 & 0xFF;
    temp_v1 = -temp_t2;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + (*(s32 *)arg3));
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + (*(s32 *)arg3) + 0xFF);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + (*(s32 *)arg3));
    *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + (*(s32 *)arg3) + 0xFF);
    *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    temp_t9 = arg2 & 3;
    temp_a3 = ((arg0 & 0x10) * 0x10) + temp_t6;
    sp48 = temp_a3;
    sp50 = temp_t9;
    sp5C = temp_t2;
    sp58 = temp_v1;
    sp60 = (s32) temp_t6;
    sp54 = (s32) arg1;
    temp_t4 = ((arg0 & 0xF) << 6) + ((s32) (arg4 & 0x7F00) / sp6C);
    sp4C = temp_t4;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0((s32) arg1, temp_t9, temp_t4, temp_a3);
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74((s32) arg6, (s32) arg7);
    temp_t8 = arg8 * 4;
    sp44 = temp_t8;
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + temp_t8], temp_v0);
    temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_2);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_2, 1);
    } else {
        SetSemiTrans(temp_v0_2, 0);
    }
    SetShadeTex(temp_v0_2, 0);
    *(u8 *)((unsigned char *)temp_v0_2 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_2 + (5)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_2 + (6)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0;
    temp_t0 = (arg4 / 256) * 4;
    temp_v0_3 = temp_t0 + 3;
    temp_v1_2 = temp_t0 + 4;
    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = temp_v1_2;
    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_v0_3;
    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = temp_v0_3;
    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = temp_v1_2;
    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = (s8) *(s16 *)((unsigned char *)arg3 + (6));
    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = (s8) *(s16 *)((unsigned char *)arg3 + (6));
    temp_v0_4 = 0xFF - sp5C;
    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + (*(s32 *)arg3));
    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + (*(s32 *)arg3) + 1);
    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_2 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + (*(s32 *)arg3));
    *(s16 *)((unsigned char *)temp_v0_2 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_2 + (32)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] - sp5C) + (*(s32 *)arg3) + 0x100);
    *(s16 *)((unsigned char *)temp_v0_2 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    sp40 = temp_t0;
    sp3C = temp_v0_4;
    *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(sp54, sp50, 0x3C0, 0);
    *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&*(s32 *)((unsigned char *)(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44]) + (-4)), temp_v0_2);
    temp_v0_5 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_5);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_5, 1);
    } else {
        SetSemiTrans(temp_v0_5, 0);
    }
    SetShadeTex(temp_v0_5, 0);
    *(u8 *)((unsigned char *)temp_v0_5 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_5 + (5)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_5 + (20)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_5 + (28)) = 0;
    *(s8 *)((unsigned char *)temp_v0_5 + (12)) = 0;
    *(s8 *)((unsigned char *)temp_v0_5 + (21)) = (s8) sp60;
    *(s8 *)((unsigned char *)temp_v0_5 + (13)) = (s8) sp60;
    *(u8 *)((unsigned char *)temp_v0_5 + (6)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_5 + (36)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_5 + (29)) = (s8) (sp60 + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s8 *)((unsigned char *)temp_v0_5 + (37)) = (s8) (sp60 + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_5 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x100);
    *(s16 *)((unsigned char *)temp_v0_5 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_5 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x1FF);
    *(s16 *)((unsigned char *)temp_v0_5 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_5 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x100);
    *(s16 *)((unsigned char *)temp_v0_5 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_5 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x1FF);
    *(s16 *)((unsigned char *)temp_v0_5 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_5 + (22)) = func_8009ECB0(sp54, sp50, sp4C + sp68, sp48);
    *(s16 *)((unsigned char *)temp_v0_5 + (14)) = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_5);
    temp_v0_6 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_6);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_6, 1);
    } else {
        SetSemiTrans(temp_v0_6, 0);
    }
    SetShadeTex(temp_v0_6, 0);
    *(u8 *)((unsigned char *)temp_v0_6 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_6 + (5)) = (u8) D_800E62BA;
    temp_v0_7 = sp40 + 7;
    temp_v1_3 = sp40 + 8;
    *(s8 *)((unsigned char *)temp_v0_6 + (20)) = temp_v1_3;
    *(s8 *)((unsigned char *)temp_v0_6 + (12)) = temp_v0_7;
    *(s8 *)((unsigned char *)temp_v0_6 + (28)) = temp_v0_7;
    *(s8 *)((unsigned char *)temp_v0_6 + (21)) = 0;
    *(s8 *)((unsigned char *)temp_v0_6 + (13)) = 0;
    *(u8 *)((unsigned char *)temp_v0_6 + (6)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_6 + (36)) = temp_v1_3;
    *(s8 *)((unsigned char *)temp_v0_6 + (29)) = (s8) *(s16 *)((unsigned char *)arg3 + (6));
    *(s8 *)((unsigned char *)temp_v0_6 + (37)) = (s8) *(s16 *)((unsigned char *)arg3 + (6));
    *(s16 *)((unsigned char *)temp_v0_6 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + (*(s32 *)arg3) + 0x100);
    *(s16 *)((unsigned char *)temp_v0_6 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_6 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + (*(s32 *)arg3) + 0x101);
    *(s16 *)((unsigned char *)temp_v0_6 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_6 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + (*(s32 *)arg3) + 0x100);
    *(s16 *)((unsigned char *)temp_v0_6 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_6 + (32)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] - sp5C) + (*(s32 *)arg3) + 0x200);
    *(s16 *)((unsigned char *)temp_v0_6 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_6 + (22)) = func_8009ECB0(sp54, sp50, 0x3C0, 0);
    *(s16 *)((unsigned char *)temp_v0_6 + (14)) = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_6);
    temp_v0_8 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_8);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_8, 1);
    } else {
        SetSemiTrans(temp_v0_8, 0);
    }
    SetShadeTex(temp_v0_8, 0);
    *(u8 *)((unsigned char *)temp_v0_8 + (4)) = (u8) D_800E62BA;
    *(u8 *)((unsigned char *)temp_v0_8 + (5)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_8 + (20)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_8 + (28)) = 0;
    *(s8 *)((unsigned char *)temp_v0_8 + (12)) = 0;
    *(s8 *)((unsigned char *)temp_v0_8 + (21)) = (s8) sp60;
    *(s8 *)((unsigned char *)temp_v0_8 + (13)) = (s8) sp60;
    *(u8 *)((unsigned char *)temp_v0_8 + (6)) = (u8) D_800E62BA;
    *(s8 *)((unsigned char *)temp_v0_8 + (29)) = (s8) (sp60 + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s8 *)((unsigned char *)temp_v0_8 + (36)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_8 + (37)) = (s8) (sp60 + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_8 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x200);
    *(s16 *)((unsigned char *)temp_v0_8 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_8 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x2FF);
    *(s16 *)((unsigned char *)temp_v0_8 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)));
    *(s16 *)((unsigned char *)temp_v0_8 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x200);
    *(s16 *)((unsigned char *)temp_v0_8 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_8 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + (*(s32 *)arg3) + 0x2FF);
    *(s16 *)((unsigned char *)temp_v0_8 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + *(s16 *)((unsigned char *)arg3 + (2)) + *(s16 *)((unsigned char *)arg3 + (6)));
    *(s16 *)((unsigned char *)temp_v0_8 + (22)) = func_8009ECB0(sp54, sp50, sp4C + (sp68 * 2), sp48);
    *(s16 *)((unsigned char *)temp_v0_8 + (14)) = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_8);
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_vram_set);

void move_255_line(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;

    for (i = arg0; i <= arg1; i++) {
        MoveSquare((i * 0x40 + 0x3F) & 0xFFFF, 0, 1, 0xFF, ((arg2 & 0xF) << 6) + i - arg0, ((arg2 & 0x10) * 0x10) & 0xFFFF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_trans_vram);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", get_fnt);

/* FAKE: taking the address of the argument forces the home-slot spill (sw a0,0(sp)); real source unknown. T-0400 */
void func_8004DAC4(s32 arg0) {
    s32 *p = &arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set_gpu);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", mod_set);

INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", srn_set);

void srn_init(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_80121750 + arg0 * 0x28;
    p[0xD] = (arg1[0] & 0xFF00) >> 8;
    *(s16 *)(p + 2) = 0;
    *(s16 *)(p + 6) = 0;
    p[0xC] = 0;
    *(u32 **)(p + 8) = arg1 + 5;
    p[0xE] = (arg1[1] & 0xFF000000) >> 24;
    p[0xF] = (arg1[1] & 0xFF0000) >> 16;
    p[0x10] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in func_8004E44C. T-4070 */
    p[0x11] = arg1[1] & 0xFF;
    p[0x12] = (arg1[2] & 0xFF000000) >> 24;
    p[0x13] = (arg1[2] & 0xFF0000) >> 16;
    p[0x14] = (arg1[2] & 0xFF00) >> 8;
    p[0x15] = arg1[2] & 0xFF; /* FAKE: same no-op mask as above. T-4070 */
    *(s16 *)(p + 0x16) = 0;
    *(s16 *)(p + 0x18) = 0x1E0;
    *(u32 *)(p + 0x1C) = arg1[4];
    *(u32 *)(p + 0x20) = arg1[3];
    *(u8 **)(p + 0x24) = D_801217A0 + arg2 * 0x14;
}

void func_8004E44C(s32 arg0, u32 *arg1, s32 arg2) {
    u8 *p;

    p = D_801217A0 + arg0 * 0x14;
    *(s32 *)p = arg2;
    *(u32 **)(p + 4) = arg1 + 4;
    p[8] = (arg1[0] & 0xFF000000) >> 24;
    *(s16 *)(p + 0xA) = (arg1[0] & 0xFFFF) >> 24;
    p[0xC] = (arg1[1] & 0xFF000000) >> 24;
    p[0xD] = (arg1[1] & 0xFF0000) >> 16;
    p[0xE] = (arg1[1] & 0xFF00) >> 8;
    /* FAKE: the no-op "& 0xFF" consumes one IDO temp number, as in the original. T-2090 */
    p[0xF] = arg1[1] & 0xFF;
    *(s16 *)(p + 0x10) = ((u8 *)arg1)[8];
    *(s16 *)(p + 0x12) = ((u8 *)arg1)[0xC];
}
