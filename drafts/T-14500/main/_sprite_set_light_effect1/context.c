#include "common.h"
#include "game.h"

void SetWorkBase(s32 arg0, s32 arg1) {
    D_800E6280.unk_01C[arg1].unk_08 = arg0;
    D_800E6280.unk_01C[arg1].unk_00 = 0x10C00;
    D_800E6280.unk_01C[arg1].unk_04 = 0;
}

s32 GetWorkBase(s32 arg0, s32 arg1) {
    s32 ret;

    ret = D_800E6280.unk_01C[arg1].unk_08 + (D_800E6280.unk_01C[arg1].unk_04 * 4);
    D_800E6280.unk_01C[arg1].unk_04 += ((arg0 + 3) / 4) * 4;
    return ret;
}

void sprite_set_gpu_poly_ft4(Rec24 *arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (!(arg0->unk_00 & 0x80000000)) {
        p = (u8 *)GetWorkBase(0x28, arg2);
        func_8009F040(p);
        if (arg0->unk_00 & 0x40000000) {
            SetSemiTrans(p, 1);
        } else {
            SetSemiTrans(p, 0);
        }
        SetShadeTex(p, 0);
        p[4] = arg0->unk_14;
        p[5] = arg0->unk_15;
        p[6] = arg0->unk_16;
        *(s16 *)(p + 8) = D_800E6280.unk_014[arg2] + arg0->unk_04 - arg0->unk_18;
        *(s16 *)(p + 0xA) = D_800E6280.unk_018[arg2] + arg0->unk_06 - arg0->unk_1A;
        *(s16 *)(p + 0x10) = D_800E6280.unk_014[arg2] + arg0->unk_04 - arg0->unk_18 + (u16)arg0->unk_08;
        *(s16 *)(p + 0x12) = D_800E6280.unk_018[arg2] + arg0->unk_06 - arg0->unk_1A;
        *(s16 *)(p + 0x18) = D_800E6280.unk_014[arg2] + arg0->unk_04 - arg0->unk_18;
        *(s16 *)(p + 0x1A) = D_800E6280.unk_018[arg2] + arg0->unk_06 - arg0->unk_1A + arg0->unk_0A;
        *(s16 *)(p + 0x20) = D_800E6280.unk_014[arg2] + arg0->unk_04 - arg0->unk_18 + (u16)arg0->unk_08;
        *(s16 *)(p + 0x22) = D_800E6280.unk_018[arg2] + arg0->unk_06 - arg0->unk_1A + arg0->unk_0A;
        p[0xC] = arg0->unk_0E;
        p[0xD] = arg0->unk_0F;
        p[0x14] = arg0->unk_0E + (u16)arg0->unk_08;
        p[0x15] = arg0->unk_0F;
        p[0x1C] = arg0->unk_0E;
        p[0x1D] = arg0->unk_0F + arg0->unk_0A;
        p[0x24] = arg0->unk_0E + (u16)arg0->unk_08;
        p[0x25] = arg0->unk_0F + arg0->unk_0A;
        *(s16 *)(p + 0x16) = (u16)arg0->unk_0C | 0x80;
        *(s16 *)(p + 0xE) = func_8009ED74(arg0->unk_10, arg0->unk_12);
        AddPrim(D_800E8CA0 + (arg2 << 10) + arg1 * 4, p);
    }
}

void goto_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;
    u16 tpage;

    tpage = func_8009ECB0(arg4, arg3, arg0, arg1);
    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 0, tpage, 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg2 * 4, work);
    safe_env(arg2);
}

void safe_env(s32 arg0) {
    u8 *p;

    p = (u8 *)GetWorkBase(0xC, D_8011ECA0);
    func_8009F0B8(p);
    *(s16 *)(p + 8) = 0;
    *(s16 *)(p + 0xA) = 0;
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg0 * 4, (s32)p);
}

void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 work;

    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 1, func_8009ECB0(arg4, arg3, arg0, arg1), 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg2 * 4, work);
    safe_env(arg2);
}

void dtd_on(s32 arg0) {
    s32 work;

    work = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294(work, 0, 1, 0, 0);
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg0 * 4, work);
    safe_env(arg0);
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
/* end bulk_m2c declarations */
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 temp_lo;
    s32 temp_t6;
    s32 temp_v0;

    sp34 = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 0x10) & 0xFF);
    sp30 = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (((u32) arg7 >> 8) & 0xFF);
    sp2C = ((s32) (rsin(D_800E7378 * 0xFF0) << 5) / 4096) + (arg7 & 0xFF);
    sp28 = abs(rsin((D_800E7378 + arg8) * 0xFF0));
    temp_v0 = GetWorkBase(0x1C, D_8011ECA0);
    sp38 = temp_v0;
    func_8009F004(temp_v0);
    SetSemiTrans(sp38, 1);
    *(s8 *)((unsigned char *)sp38 + (4)) = (s8) ((s32) (sp34 * 3) / 5);
    *(s8 *)((unsigned char *)sp38 + (5)) = (s8) ((s32) (sp30 * 3) / 5);
    *(s8 *)((unsigned char *)sp38 + (6)) = (s8) ((s32) (sp2C * 3) / 5);
    temp_lo = 0 / 5;
    *(s8 *)((unsigned char *)sp38 + (14)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (13)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (12)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (22)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (21)) = (s8) temp_lo;
    *(s8 *)((unsigned char *)sp38 + (20)) = (s8) temp_lo;
    if (arg4 == 0) {
        *(s16 *)((unsigned char *)sp38 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        *(s16 *)((unsigned char *)sp38 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        temp_t6 = (s32) (sp28 * 8) / 4096;
        *(s16 *)((unsigned char *)sp38 + (16)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg2) - temp_t6);
        *(s16 *)((unsigned char *)sp38 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        *(s16 *)((unsigned char *)sp38 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2 + temp_t6);
        *(s16 *)((unsigned char *)sp38 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    } else {
        *(s16 *)((unsigned char *)sp38 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
        *(s16 *)((unsigned char *)sp38 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
        *(s16 *)((unsigned char *)sp38 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
        *(s16 *)((unsigned char *)sp38 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
        *(s16 *)((unsigned char *)sp38 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg4);
        *(s16 *)((unsigned char *)sp38 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
    }
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg6 * 4)], sp38);
}

INCLUDE_ASM("asm/nonmatchings/main/800490C0", func_80049A40);
