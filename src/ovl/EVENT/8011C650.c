#include "common.h"
#include "ovl/EVENT.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C0);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C4);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258C8);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258CC);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D0);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D4);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258D8);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258DC);
INCLUDE_RODATA("asm/ovl/EVENT/data/EVENT/8011C650.data", D_801258E0);
/* T-22009: the four halfword arrays are defined here so IDO shares one lui $at between the stores of
 * func_8011EDFC. D_801258FC ends at 0x80125902; D_80125904 is the zero fill that follows it up to the last
 * word of the overlay file, D_80125FFC (0x10124FE6; no EVENT code references it). */
s16 D_801258E4[3] = { 0, 0, 0 };
s16 D_801258EC[3] = { 0, 0, 0 };
s16 D_801258F4[3] = { 0, 0, 0 };
s16 D_801258FC[3] = { 0, 0, 0 };
u8 D_80125904[0x6F8] = { 0 };
u32 D_80125FFC = 0x10124FE6;

void func_8011C650(void) {
    D_801258C0 = 0x801C6000;
    D_801258C4 = 0x801AE000;
    D_801258C8 = 0x801B2000;
    D_801258CC = 0x801B6000;
    D_801258D0 = 0x801BA000;
    D_801258D4 = 0x801BE000;
    D_801258D8 = 0x801C2000;
}

void func_8011C6C4(void) {
    switch (*(u32 *)&D_80125320) {
    case 0:
        func_8011DDB8();
        return;
    case 1:
        func_8011EDFC();
        return;
    case 2:
        func_8011D5C4();
        return;
    case 3:
        func_8011C750();
        return;
    case 4:
        func_8011C750();
        return;
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011C750);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011D5C4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011DDB8);

/* T-10040: the implicit result view matches DATE func_80148924. */
s32 func_8011ED50(s16 arg0) {
    ((Rec24 *)D_800EC190)[4].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[5].unk_06 += arg0;
    ((Rec24 *)D_800EC190)[4].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[5].unk_0A -= arg0;
    ((Rec24 *)D_800EC190)[6].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[7].unk_0A += arg0;
    ((Rec24 *)D_800EC190)[6].unk_0F -= arg0;
    ((Rec24 *)D_800EC190)[7].unk_0F -= arg0;
}

void func_8011EDFC(void) {
    s16 *temp_v0;
    s16 *var_a0;
    s16 temp_v0_2;
    s16 var_a1;
    s16 var_s3;
    s32 temp_s1;
    s32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (((EventTransitionState *)&D_800B1AF0)->unk_00) {
    case 0:
        D_800EED08 = 1;
        func_8011F530(D_800B1AE4);
        func_800433D0(0x202);
        D_801258DC = 0;
        func_800352B4(0);
        func_800469F4(0x432A);
        D_801258E4[0] = -0x50;
        D_801258E4[1] = 0;
        D_801258E4[2] = 0x64;
        D_801258EC[0] = -0x28;
        D_801258EC[1] = 0;
        D_801258EC[2] = -0x3C;
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        D_801258F4[0] = (func_800789A0() % 3) - 1;
        D_801258F4[1] = (func_800789A0() % 3) - 1;
        D_801258F4[2] = (func_800789A0() % 3) - 1;
        D_801258FC[0] = (func_800789A0() % 3) - 1;
        D_801258FC[1] = (func_800789A0() % 3) - 1;
        D_801258FC[2] = (func_800789A0() % 3) - 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            break;
        }
        break;
    case 2:
        func_8002755C(1);
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    case 3:
        temp_s0 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046480();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1;
        if (temp_s0 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1 + 1;
            func_800433D0(0x201);
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258F4)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if (((u32) D_800B1AE4 % 30U) == (func_800789A0() % 30)) {
                (D_801258FC)[var_s3] = (func_800789A0() % 3) - 1;
            }

            if ((D_801258F4[var_s3] == 0) && (D_801258FC[var_s3] == 0)) {
                if (func_800789A0() % 2) {
                    D_801258F4[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                } else {
                    D_801258FC[var_s3] = ((func_800789A0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_801258E4)[var_s3];
            *temp_v0 += D_801258F4[var_s3];
            var_a0 = &(D_801258EC)[var_s3];
            *var_a0 += D_801258FC[var_s3];
            var_a1 = *temp_v0;
            if (var_a1 < -0x95) {
                D_801258F4[var_s3] = 1;
            }
            if (var_a1 >= 0x96) {
                D_801258F4[var_s3] = -1;
            }
            temp_v0_2 = *var_a0;
            if (temp_v0_2 < -0x6D) {
                D_801258FC[var_s3] = 1;
            }

            if (temp_v0_2 >= 0x1E) {
                D_801258FC[var_s3] = -1;

            }
            var_s3++;
        } while (var_s3 < 3);
        func_8001BB10(D_801258E4[0], D_801258EC[0], 0xA, 2, 2, 0x808080, 1, 1);
        func_8001BB10(D_801258E4[1], D_801258EC[1], 0xA, 2, 2, 0x804040, 1, 1);
        func_8001BB10(D_801258E4[2], D_801258EC[2], 0xA, 2, 2, 0x800000, 1, 1);
        if (D_801258DC++ == 0x12C) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
            func_800433D0(0x202);
            break;
        }
        break;
    case 5:
        temp_s0_2 = ((EventTransitionState *)&D_800B1AF0)->unk_06;
        temp_s1_3 = ((EventTransitionState *)&D_800B1AF0)->unk_00;
        func_80046598();
        ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3;
        if (temp_s0_2 != ((EventTransitionState *)&D_800B1AF0)->unk_06) {
            ((EventTransitionState *)&D_800B1AF0)->unk_00 = temp_s1_3 + 1;
        }
        ((EventTransitionState *)&D_800B1AF0)->unk_06 = temp_s0_2;
        break;
    case 6:
        func_80027438(1);
        ((EventTransitionState *)&D_800B1AF0)->unk_00 += 1;
        break;
    default:
        func_800433D0(0x206);
        D_800EED08 = 0;
        func_800352B4(1);
        func_80011DFC();
        break;
    }
}
