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

INCLUDE_ASM("asm/nonmatchings/main/800490C0", _sprite_set_light_effect1);

void func_80049A40(s16 a, s16 b, s16 c, s16 d, s32 e, s32 f, s32 g) {
    s32 sp28;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_t3_2;
    s32 temp_t6_2;
    s32 temp_t7_3;
    s32 temp_t9_3;
    s32 temp_v0;
    s8 temp_t4_2;
    s8 temp_t7_2;
    u32 temp_t3;
    u32 temp_t4;
    u32 temp_t5;
    u32 temp_t6;
    u32 temp_t7;
    u32 temp_t8;
    u32 temp_t8_2;
    u32 temp_t9;
    u32 temp_t9_2;

    temp_v0 = GetWorkBase(0x24, D_8011ECA0);
    sp28 = temp_v0;
    func_8009F054(temp_v0);
    if (g & 0x80) {
        SetSemiTrans(sp28, 1);
    } else {
        SetSemiTrans(sp28, 0);
    }
    temp_t9 = g & 0x7F;
    switch (temp_t9) {
    case 6:
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = 0;
        temp_t3 = (u32) f >> 0x10;
        temp_t4 = (u32) f >> 8;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = (s8) temp_t4;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = (s8) temp_t3;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = (s8) temp_t4;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = (s8) temp_t3;
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = (s8) f;
        break;
    case 5:
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = 0;
        temp_t5 = (u32) f >> 0x10;
        temp_t6 = (u32) f >> 8;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = (s8) temp_t6;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = (s8) temp_t5;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = (s8) temp_t6;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = (s8) temp_t5;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = (s8) f;
        break;
    case 4:
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = 0;
        temp_t7 = (u32) f >> 0x10;
        temp_t8 = (u32) f >> 8;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = 0;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = (s8) temp_t8;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = (s8) temp_t7;
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = (s8) temp_t8;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = (s8) temp_t7;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = (s8) f;
        break;
    case 3:
        temp_t7_2 = ((u32) f >> 8) * 8;
        temp_t4_2 = ((u32) f >> 0x10) * 8;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = temp_t4_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = temp_t4_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = temp_t4_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = temp_t4_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = temp_t7_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = temp_t7_2;
        break;
    case 1:
        temp_t8_2 = (u32) f >> 0x10;
        temp_t9_2 = (u32) f >> 8;
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = (s8) temp_t9_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = (s8) temp_t8_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = (s8) temp_t9_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = (s8) temp_t8_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = (s8) temp_t9_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = (s8) temp_t8_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = (s8) temp_t9_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = (s8) temp_t8_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = (s8) f;
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = (s8) f;
        break;
    case 2:
        temp_t3_2 = ((u32) f >> 0x10) & 0xFF;
        temp_t6_2 = ((u32) f >> 8) & 0xFF;
        temp_a0 = f & 0xFF;
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = (s8) ((s32) (temp_t3_2 * 8) / 10);
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = (s8) ((s32) (temp_t6_2 * 8) / 10);
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = (s8) ((s32) (temp_a0 * 8) / 10);
        temp_lo = (s32) (temp_t3_2 * 0xA) / 10;
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = (s8) temp_lo;
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = (s8) temp_lo;
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = (s8) temp_lo;
        temp_lo_2 = (s32) (temp_t6_2 * 0xA) / 10;
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = (s8) temp_lo_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = (s8) temp_lo_2;
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = (s8) temp_lo_2;
        temp_lo_3 = (s32) (temp_a0 * 0xA) / 10;
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = (s8) temp_lo_3;
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = (s8) temp_lo_3;
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = (s8) temp_lo_3;
        break;
    default:
        temp_t7_3 = ((u32) f >> 0x10) & 0xFF;
        temp_t9_3 = ((u32) f >> 8) & 0xFF;
        temp_a0_2 = f & 0xFF;
        *(s8 *)((unsigned char *)((void *) sp28) + (30)) = (s8) (temp_a0_2 * 4);
        *(s8 *)((unsigned char *)((void *) sp28) + (29)) = (s8) (temp_t9_3 * 4);
        *(s8 *)((unsigned char *)((void *) sp28) + (20)) = (s8) (temp_t7_3 * 6);
        *(s8 *)((unsigned char *)((void *) sp28) + (28)) = (s8) (temp_t7_3 * 4);
        *(s8 *)((unsigned char *)((void *) sp28) + (14)) = (s8) (temp_a0_2 * 8);
        *(s8 *)((unsigned char *)((void *) sp28) + (12)) = (s8) (temp_t7_3 * 8);
        *(s8 *)((unsigned char *)((void *) sp28) + (13)) = (s8) (temp_t9_3 * 8);
        *(s8 *)((unsigned char *)((void *) sp28) + (21)) = (s8) (temp_t9_3 * 6);
        *(s8 *)((unsigned char *)((void *) sp28) + (22)) = (s8) (temp_a0_2 * 6);
        *(s8 *)((unsigned char *)((void *) sp28) + (4)) = (s8) (temp_t7_3 * 0xA);
        *(s8 *)((unsigned char *)((void *) sp28) + (5)) = (s8) (temp_t9_3 * 0xA);
        *(s8 *)((unsigned char *)((void *) sp28) + (6)) = (s8) (temp_a0_2 * 0xA);
        break;
    }
    *(s16 *)((unsigned char *)((void *) sp28) + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + (s32) a);
    *(s16 *)((unsigned char *)((void *) sp28) + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + (s32) b);
    *(s16 *)((unsigned char *)((void *) sp28) + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + (s32) a + (s32) c);
    *(s16 *)((unsigned char *)((void *) sp28) + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + (s32) b);
    *(s16 *)((unsigned char *)((void *) sp28) + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + (s32) a);
    *(s16 *)((unsigned char *)((void *) sp28) + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + (s32) b + (s32) d);
    *(s16 *)((unsigned char *)((void *) sp28) + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + (s32) a + (s32) c);
    *(s16 *)((unsigned char *)((void *) sp28) + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + (s32) b + (s32) d);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (e * 4)], (s32) (void *) sp28);
}
