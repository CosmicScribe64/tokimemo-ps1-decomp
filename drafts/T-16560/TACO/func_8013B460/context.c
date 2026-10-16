#include "common.h"
#include "ovl/TACO.h"

void func_8013B170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013B2A0();
        break;
    case 1:
        func_8013B398();
        break;
    case 2:
        func_8013B3C4();
        break;
    }
    if (D_800E6280.unk_110A != 0) {
        if (D_800E6280.unk_10F8 & 0x10) {
            if ((u8) D_8015EDCC >= 3U) {
                func_801432F0((s32) "MISSION COMPLETE", -0x40, 0, 2);
            } else if (D_8015EDEC == 0) {
                func_801432F0((s32) "TIME OVER", -0x40, 0, 1);
            } else {
                func_801432F0((s32) "GAME OVER", -0x40, 0, 1);
            }
        }
        func_8013B460(0x64);
    } else {
        func_8013F250();
    }
    func_80143B58();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B2A0);

void func_8013B398(void) {
    if (D_800E6280.unk_F88 != 0) {
        func_8004284C();
    }
}

void func_8013B3C4(void) {
    if (D_8015EDBC == 0) {
        if (func_80044C98() == 1) {
            func_80042908(8);
        }
    }
}

void func_8013B404(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

s32 func_8013B460(s32 arg0) {
    s32 sp4C;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_s4;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_v0 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0);
    SetSemiTrans(temp_v0, 0);
    SetShadeTex(temp_v0, 0);
    temp_s4 = 0x64 - arg0;
    temp_lo = (s32) (temp_s4 * 0x10) / 100;
    *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(2, 0, 0x200, 0);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
    temp_v1 = ((s32) (temp_s4 << 9) / 100) - temp_lo;
    temp_lo_2 = (s32) (temp_s4 << 6) / 100;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + temp_v1) - 0x100);
    temp_lo_3 = (s32) (arg0 << 8) / 100;
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2) - 0x78);
    *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + temp_v1 + temp_lo_3) - 0x100);
    temp_lo_4 = (s32) (arg0 * 0xF0) / 100;
    *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2) - 0x78);
    *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + temp_v1) - 0x100);
    *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2 + temp_lo_4) - 0x78);
    *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + temp_v1 + temp_lo_3) - 0x100);
    *(s8 *)((unsigned char *)temp_v0 + (37)) = 0xF0;
    *(s8 *)((unsigned char *)temp_v0 + (36)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (29)) = 0xF0;
    *(s8 *)((unsigned char *)temp_v0 + (28)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (21)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (20)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = 0;
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
    *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2 + temp_lo_4) - 0x78);
    sp4C = temp_lo;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, temp_v0, temp_lo);
    temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(temp_v0_2);
    SetSemiTrans(temp_v0_2, 0);
    SetShadeTex(temp_v0_2, 0);
    *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(2, 0, 0x300, 0);
    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
    temp_v1_2 = ((s32) (temp_s4 << 8) / 100) - sp4C;
    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1_2);
    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2) - 0x78);
    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1_2 + temp_lo_3);
    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2) - 0x78);
    *(s16 *)((unsigned char *)temp_v0_2 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1_2);
    *(s16 *)((unsigned char *)temp_v0_2 + (26)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2 + temp_lo_4) - 0x78);
    *(s16 *)((unsigned char *)temp_v0_2 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + temp_v1_2 + temp_lo_3);
    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0xF0;
    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0xF0;
    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = 0;
    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0;
    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0;
    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = 0;
    *(s16 *)((unsigned char *)temp_v0_2 + (34)) = (s16) ((D_800E6280.unk_018[D_8011ECA0] + temp_lo_2 + temp_lo_4) - 0x78);
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, temp_v0_2);
}
