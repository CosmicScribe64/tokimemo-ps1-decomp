#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_80049FF0);

void func_8004A14C(s32 arg0, RECT rect, s32 arg3) {
    func_80049A40(rect.x, rect.y, rect.w, rect.h, 1, arg3, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A1A0);

void func_8004A414(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp8C;
    s32 sp88;
    s32 sp7C;
    s32 sp78;
    s32 sp74;
    s32 sp6C;
    s32 sp68;
    s32 sp50;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_t4;
    s32 temp_t4_2;
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_v1;

    temp_t7 = arg3 & 0x7F;
    temp_t8 = arg4 & 0x7F;
    arg3 = temp_t7;
    if (temp_t7 < 8) {
        arg3 = 8;
    }
    arg4 = temp_t8;
    if (temp_t8 < 8) {
        arg4 = 8;
    }
    temp_v0 = arg0 - arg2;
    temp_t4 = arg0 + arg2;
    sp7C = temp_t4;
    sp8C = temp_v0;
    if (temp_v0 < temp_t4) {
        sp74 = arg1 + arg2;
        sp78 = arg1 - arg2;
        do {
            var_v1 = sp78;
            if (var_v1 < sp74) {
                temp_v0_2 = arg0 - sp8C;
                sp6C = temp_v0_2 * temp_v0_2;
                sp68 = arg2 * arg2;
                do {
                    temp_v0_3 = arg1 - var_v1;
                    if ((sp6C + (temp_v0_3 * temp_v0_3)) < sp68) {
                        sp88 = var_v1;
                        temp_s1 = arg3 / 2;
                        temp_s2 = arg4 / 2;
                        temp_t5 = arg5 * 4;
                        sp50 = temp_t5;
                        temp_v0_4 = GetWorkBase(0x34, D_8011ECA0);
                        func_8009F068(temp_v0_4);
                        SetSemiTrans(temp_v0_4, 0);
                        *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + (s16) sp8C) - temp_s1);
                        *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) sp88) - temp_s2);
                        *(s16 *)((unsigned char *)temp_v0_4 + (20)) = (s16) (((D_800E6280.unk_014[D_8011ECA0] + (s16) sp8C) - temp_s1) + (s16) arg3);
                        *(s16 *)((unsigned char *)temp_v0_4 + (22)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + (s16) sp88) - temp_s2);
                        *(s16 *)((unsigned char *)temp_v0_4 + (32)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + (s16) sp8C) - temp_s1);
                        *(s16 *)((unsigned char *)temp_v0_4 + (34)) = (s16) (((D_800E6280.unk_018[D_8011ECA0] + (s16) sp88) - temp_s2) + (s16) arg4);
                        *(s16 *)((unsigned char *)temp_v0_4 + (44)) = (s16) (((D_800E6280.unk_014[D_8011ECA0] + (s16) sp8C) - temp_s1) + (s16) arg3);
                        *(s8 *)((unsigned char *)temp_v0_4 + (49)) = 0x60;
                        *(s8 *)((unsigned char *)temp_v0_4 + (37)) = 0x60;
                        *(s16 *)((unsigned char *)temp_v0_4 + (46)) = (s16) (((D_800E6280.unk_018[D_8011ECA0] + (s16) sp88) - temp_s2) + (s16) arg4);
                        *(s8 *)((unsigned char *)temp_v0_4 + (48)) = 0xD0;
                        *(s8 *)((unsigned char *)temp_v0_4 + (36)) = 0xC0;
                        *(s8 *)((unsigned char *)temp_v0_4 + (25)) = 0x50;
                        *(s8 *)((unsigned char *)temp_v0_4 + (24)) = 0xD0;
                        *(s8 *)((unsigned char *)temp_v0_4 + (13)) = 0x50;
                        *(s8 *)((unsigned char *)temp_v0_4 + (12)) = 0xC0;
                        *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (18)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (17)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (16)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (30)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (29)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (28)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (42)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (41)) = 0x80;
                        *(s8 *)((unsigned char *)temp_v0_4 + (40)) = 0x80;
                        *(s16 *)((unsigned char *)temp_v0_4 + (26)) = func_8009ECB0(0, 0, 0x3C0, 0x100);
                        temp_s1_2 = rand();
                        *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74((temp_s1_2 & 8) + D_800E6280.unk_014[D_8011ECA0] + (s16) sp8C, (rand() & 8) + (s16) sp88 + D_800E6280.unk_018[D_8011ECA0]);
                        AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + temp_t5], temp_v0_4);
                    }
                    var_v1 += arg4;
                } while (var_v1 < sp74);
            }
            temp_t4_2 = sp8C + arg3;
            sp8C = temp_t4_2;
        } while (temp_t4_2 < sp7C);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A814);

void func_8004AC18(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    func_80049A40(-0xA0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    func_80049A40(0, -0x78, 0xA0, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

void func_8004ACC8(s32 arg0) {
    func_80049A40(-0x100, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    func_80049A40(0, -0x78, 0x100, 0xF0, 1, arg0, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
