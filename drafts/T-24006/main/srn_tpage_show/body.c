#include "psx_types.h"
void srn_tpage_show(u8 arg0, u8 arg1, u8 arg2, RECT *arg3, s32 arg4, s16 arg6, s16 arg7, s32 arg8) {
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
    POLY_FT4 *temp_v0;
    POLY_FT4 *temp_v0_2;
    s32 temp_v0_4;
    POLY_FT4 *temp_v0_5;
    POLY_FT4 *temp_v0_6;
    POLY_FT4 *temp_v0_8;
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
    temp_v0->r0 = (u8) D_800E6280.unk_03A;
    temp_v0->g0 = (u8) D_800E6280.unk_03A;
    temp_t6 = (arg0 & 0x20) * 4;
    temp_v0->u1 = 0xFF;
    temp_v0->v0 = temp_t6;
    temp_v0->v1 = temp_t6;
    temp_v0->u2 = 0;
    temp_v0->u0 = 0;
    temp_v0->b0 = (u8) D_800E6280.unk_03A;
    temp_v0->u3 = 0xFF;
    temp_v0->v2 = (s8) (temp_t6 + arg3->h);
    temp_v0->v3 = (s8) (temp_t6 + arg3->h);
    temp_t2 = arg4 & 0xFF;
    temp_v1 = -temp_t2;
    temp_v0->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + arg3->x);
    temp_v0->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + arg3->x + 0xFF);
    temp_v0->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + arg3->x);
    temp_v0->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0->x3 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1 + arg3->x + 0xFF);
    temp_v0->y3 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
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
    temp_v0->tpage = func_8009ECB0((s32) arg1, temp_t9, temp_t4, temp_a3);
    temp_v0->clut = func_8009ED74((s32) arg6, (s32) arg7);
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
    temp_v0_2->r0 = (u8) D_800E6280.unk_03A;
    temp_v0_2->g0 = (u8) D_800E6280.unk_03A;
    temp_v0_2->b0 = (u8) D_800E6280.unk_03A;
    temp_v0_2->v1 = 0;
    temp_t0 = (arg4 / 256) * 4;
    temp_v0_3 = temp_t0 + 3;
    temp_v1_2 = temp_t0 + 4;
    temp_v0_2->u1 = temp_v1_2;
    temp_v0_2->u0 = temp_v0_3;
    temp_v0_2->u2 = temp_v0_3;
    temp_v0_2->v0 = 0;
    temp_v0_2->u3 = temp_v1_2;
    temp_v0_2->v2 = (s8) arg3->h;
    temp_v0_2->v3 = (s8) arg3->h;
    temp_v0_4 = 0xFF - sp5C;
    temp_v0_2->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + arg3->x);
    temp_v0_2->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_2->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + arg3->x + 1);
    temp_v0_2->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_2->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v0_4 + arg3->x);
    temp_v0_2->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_2->x3 = (s16) ((D_800E6280.unk_014[D_8011ECA0] - sp5C) + arg3->x + 0x100);
    temp_v0_2->y3 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    sp40 = temp_t0;
    sp3C = temp_v0_4;
    temp_v0_2->tpage = func_8009ECB0(sp54, sp50, 0x3C0, 0);
    temp_v0_2->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&*(s32 *)((unsigned char *)(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44]) + (-4)), temp_v0_2);
    temp_v0_5 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_5);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_5, 1);
    } else {
        SetSemiTrans(temp_v0_5, 0);
    }
    SetShadeTex(temp_v0_5, 0);
    temp_v0_5->r0 = (u8) D_800E6280.unk_03A;
    temp_v0_5->g0 = (u8) D_800E6280.unk_03A;
    temp_v0_5->u1 = 0xFF;
    temp_v0_5->u2 = 0;
    temp_v0_5->u0 = 0;
    temp_v0_5->v1 = (s8) sp60;
    temp_v0_5->v0 = (s8) sp60;
    temp_v0_5->b0 = (u8) D_800E6280.unk_03A;
    temp_v0_5->u3 = 0xFF;
    temp_v0_5->v2 = (s8) (sp60 + arg3->h);
    temp_v0_5->v3 = (s8) (sp60 + arg3->h);
    temp_v0_5->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x100);
    temp_v0_5->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_5->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x1FF);
    temp_v0_5->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_5->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x100);
    temp_v0_5->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_5->x3 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x1FF);
    temp_v0_5->y3 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_5->tpage = func_8009ECB0(sp54, sp50, sp4C + sp68, sp48);
    temp_v0_5->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_5);
    temp_v0_6 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_6);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_6, 1);
    } else {
        SetSemiTrans(temp_v0_6, 0);
    }
    SetShadeTex(temp_v0_6, 0);
    temp_v0_6->r0 = (u8) D_800E6280.unk_03A;
    temp_v0_6->g0 = (u8) D_800E6280.unk_03A;
    temp_v0_7 = sp40 + 7;
    temp_v1_3 = sp40 + 8;
    temp_v0_6->u1 = temp_v1_3;
    temp_v0_6->u0 = temp_v0_7;
    temp_v0_6->u2 = temp_v0_7;
    temp_v0_6->v1 = 0;
    temp_v0_6->v0 = 0;
    temp_v0_6->b0 = (u8) D_800E6280.unk_03A;
    temp_v0_6->u3 = temp_v1_3;
    temp_v0_6->v2 = (s8) arg3->h;
    temp_v0_6->v3 = (s8) arg3->h;
    temp_v0_6->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + arg3->x + 0x100);
    temp_v0_6->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_6->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + arg3->x + 0x101);
    temp_v0_6->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_6->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp3C + arg3->x + 0x100);
    temp_v0_6->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_6->x3 = (s16) ((D_800E6280.unk_014[D_8011ECA0] - sp5C) + arg3->x + 0x200);
    temp_v0_6->y3 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_6->tpage = func_8009ECB0(sp54, sp50, 0x3C0, 0);
    temp_v0_6->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_6);
    temp_v0_8 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_8);
    if (arg2 != 0) {
        SetSemiTrans(temp_v0_8, 1);
    } else {
        SetSemiTrans(temp_v0_8, 0);
    }
    SetShadeTex(temp_v0_8, 0);
    temp_v0_8->r0 = (u8) D_800E6280.unk_03A;
    temp_v0_8->g0 = (u8) D_800E6280.unk_03A;
    temp_v0_8->u1 = 0xFF;
    temp_v0_8->u2 = 0;
    temp_v0_8->u0 = 0;
    temp_v0_8->v1 = (s8) sp60;
    temp_v0_8->v0 = (s8) sp60;
    temp_v0_8->b0 = (u8) D_800E6280.unk_03A;
    temp_v0_8->v2 = (s8) (sp60 + arg3->h);
    temp_v0_8->u3 = 0xFF;
    temp_v0_8->v3 = (s8) (sp60 + arg3->h);
    temp_v0_8->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x200);
    temp_v0_8->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_8->x1 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x2FF);
    temp_v0_8->y1 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y);
    temp_v0_8->x2 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x200);
    temp_v0_8->y2 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_8->x3 = (s16) (D_800E6280.unk_014[D_8011ECA0] + sp58 + arg3->x + 0x2FF);
    temp_v0_8->y3 = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3->y + arg3->h);
    temp_v0_8->tpage = func_8009ECB0(sp54, sp50, sp4C + (sp68 * 2), sp48);
    temp_v0_8->clut = func_8009ED74((s32) arg6, (s32) arg7);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + sp44], temp_v0_8);
}
