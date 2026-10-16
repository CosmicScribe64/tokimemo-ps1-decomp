#include "common.h"
#include "game.h"

s32 func_80049FF0(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u32 arg4, u32 arg5, u8 arg6) {
    s32 temp_v0;

    temp_v0 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F11C(temp_v0);
    if (arg6 & 0x80) {
        SetSemiTrans(temp_v0, 1);
    } else {
        SetSemiTrans(temp_v0, 0);
    }
    *(s8 *)((unsigned char *)temp_v0 + (5)) = (s8) (arg4 >> 8);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = (s8) (arg4 >> 0x10);
    *(s8 *)((unsigned char *)temp_v0 + (13)) = (s8) (arg5 >> 8);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) (arg5 >> 0x10);
    *(s8 *)((unsigned char *)temp_v0 + (6)) = (s8) arg4;
    *(s8 *)((unsigned char *)temp_v0 + (14)) = (s8) arg5;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg2);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg3);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + ((arg6 & 0x7F) * 4)], temp_v0);
}

void func_8004A14C(s32 arg0, RECT rect, s32 arg3) {
    func_80049A40(rect.x, rect.y, rect.w, rect.h, 1, arg3, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A1A0);

INCLUDE_ASM("asm/nonmatchings/main/80049FF0", func_8004A414);

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
