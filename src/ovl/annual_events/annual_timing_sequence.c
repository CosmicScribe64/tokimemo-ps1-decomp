/* annual_timing_sequence.c: ; src/ovl/GYOZI/8013C3B0.c:98-119 processes the timed S/T sequence choices. Original object 0x8013C3B0-0x8013CDD0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147B18;

void annual_timing_sequence_resource_addresses_init(void) {
    D_80147AD0 = (u8 *)0x801E60CC;
    D_80147AD4 = (u8 *)0x801E60D0;
    D_80147AD8 = (u8 *)0x801E60E4;
    D_80147ADC = *(s16 *)0x801E60F0;
    D_80147AE0 = (u8 *)0x801AE000;
    D_80147AE4 = (u8 *)0x801B0000;
    D_80147AE8 = (u8 *)0x801B2000;
    D_80147AEC = (u8 *)0x801B6000;
    D_80147AF0 = (u8 *)0x801BA000;
    D_80147AF4 = (u8 *)0x801BE000;
    D_80147AF8 = (u8 *)0x801C2000;
    D_80147AFC = (u8 *)0x801C6000;
    D_80147B00 = (u8 *)0x801CE000;
    D_80147B04 = (u8 *)0x801D2000;
    D_80147B08 = (u8 *)0x801D6000;
    D_80147B0C = (u8 *)0x801DA000;
    D_80147B10 = (u8 *)0x801DE000;
    D_80147B14 = (u8 *)0x801E2000;
}

void annual_timing_sequence_substate_dispatch(void) {
    u8 timing_sequence_substate = annual_scene_substate; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (timing_sequence_substate) {
    case 0:
        annual_timing_sequence_group_a_step_dispatch();
        return;
    case 1:
        annual_timing_sequence_group_b_step_dispatch();
        return;
    case 2:
        annual_timing_sequence_group_c_step_dispatch();
        return;
    case 3:
        annual_timing_sequence_group_d_step_dispatch();
        return;
    default:
        annual_timing_sequence_choice_sounds_dispatch();
        return;
    }
}

void annual_timing_sequence_group_a_step_dispatch(void) {
    s32 mode_step_index; /* declared before timing_sequence_callbacks: its stack slot sits above timing_sequence_callbacks (T-3330) */
    FnTbl20 timing_sequence_callbacks;

    timing_sequence_callbacks = D_80147B18;
    mode_step_index = D_800F647A;
    timing_sequence_callbacks.f[mode_step_index]();
}

extern FnTbl20 D_80147B68;

void annual_timing_sequence_group_b_step_dispatch(void) {
    s32 mode_step_index; /* declared before timing_sequence_callbacks: its stack slot sits above timing_sequence_callbacks (T-3330) */
    FnTbl20 timing_sequence_callbacks;

    timing_sequence_callbacks = D_80147B68;
    mode_step_index = D_800F647A;
    timing_sequence_callbacks.f[mode_step_index]();
}

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80147BB8;

void annual_timing_sequence_group_c_step_dispatch(void) {
    s32 mode_step_index; /* declared before timing_sequence_callbacks: its stack slot sits above timing_sequence_callbacks (T-3330) */
    FnTbl25 timing_sequence_callbacks;

    timing_sequence_callbacks = D_80147BB8;
    mode_step_index = D_800F647A;
    timing_sequence_callbacks.f[mode_step_index]();
}

extern FnTbl20 D_80147C1C;

void annual_timing_sequence_group_d_step_dispatch(void) {
    s32 mode_step_index; /* declared before timing_sequence_callbacks: its stack slot sits above timing_sequence_callbacks (T-3330) */
    FnTbl20 timing_sequence_callbacks;

    timing_sequence_callbacks = D_80147C1C;
    mode_step_index = D_800F647A;
    timing_sequence_callbacks.f[mode_step_index]();
}

/* Return type preserves the original fall-through schedule. T-20005. */
s32 annual_timing_sequence_choice_sounds_dispatch(void) {
    s32 mode_step_index; /* declared before timing_sequence_callbacks: its stack slot sits above it (T-3330) */
    GzCallbacks90 timing_sequence_callbacks;

    timing_sequence_callbacks = D_80147C6C;
    mode_step_index = D_800F647A;
    timing_sequence_callbacks.callbacks[mode_step_index]();
    if (D_800F62F0 & 0x100) {
        if (D_800F53A0.unk_01C[0x23] >= 6 && D_800F53A0.unk_01C[0x23] < 9) {
            func_8004EE18(D_80147AE0, 0x11, 1, 0, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 0, 0);
        }
        func_800BCDF0("S\n");
    }
    if (D_800F62F0 & 0x800) {
        if (D_800F53A0.unk_01C[0x23] >= 6 && D_800F53A0.unk_01C[0x23] < 9) {
            func_8004EE18(D_80147AE0, 0x11, 1, 2, 0);
        } else {
            func_8004EE18(D_80147AE4, 0x11, 1, 2, 0);
        }
        func_800BCDF0("T\n");
    }
}

void annual_timing_audio_resource_init(void) {
    func_80051DD8(0x71, 0x801AE000, 0x8453);
    annual_timing_sequence_resource_addresses_init();
    func_8004DE1C();
}

void annual_timing_scene_state_init(void) {
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_8012E688 = 0;
    D_8012E684 = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_8008FC10();
    /* FAKE: the store inside the call shares $a0 with the argument (T-9020, T-9170) */
    func_8008F618(*(u8 *)&D_800F62CF = 1);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147440;
    /* three words of a 0x24-byte record array reached through one base: as1 keeps the loads in order */
    annual_scene_talk_command_table = (&D_80147440)[9];
    annual_scene_talk_voice_table = (&D_80147440)[18];
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_timing_sequence.rodata", D_80145BB8);

void annual_timing_selector_0_callback(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BB8);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_timing_sequence.rodata", D_80145BC0);

void annual_timing_selector_3_callback(void) {
    annual_scene_talk_group = 3;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC0);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_timing_sequence.rodata", D_80145BC8);

void annual_timing_selector_6_callback(void) {
    annual_scene_talk_group = 6;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BC8);
    func_8004DE1C();
}

void annual_timing_library_title_callback(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 0xA;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "図書室");
    func_8004DE1C();
}

void annual_timing_theater_title_setup(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10[0])->f = 1;
    annual_scene_talk_group = 0xD;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "演劇部");
    annual_timing_sequence_resource_addresses_init();
    if (((u8) D_800F53A0.unk_01C[0x23] >= 6U) && ((u8) D_800F53A0.unk_01C[0x23] < 0xAU)) {
        func_8004EE18(D_80147AE0, 0x11, 1, 2, 0);
        func_8008FD1C(D_80147AE8, D_80147AEC, D_80147AF0, D_80147AF4, D_80147AF8, D_80147AFC);
        func_8008FF60(D_80147AD4, D_80147AD8, D_80147AD0, D_80147ADC);
    } else {
        func_8004EE18(D_80147AE4, 0x11, 1, 2, 0);
        func_8008FD1C(D_80147B00, D_80147B04, D_80147B08, D_80147B0C, D_80147B10, D_80147B14);
        func_8008FF60(D_80147AD4, D_80147AD8, D_80147AD0, D_80147ADC);
    }
    D_8012B8F8 |= 0x80000000;
    D_8012B8D6 = 0;
    D_8012E69C = 1;
    func_8004DE1C();
}

void annual_callback_audio_405b_a(void) {
    func_8008A0D4(0x405B);
    func_8004DE1C();
}

void annual_callback_audio_3fe3(void) {
    func_8008A0D4(0x3FE3);
    func_8004DE1C();
}

void annual_callback_audio_405b_b(void) {
    func_8008A0D4(0x405B);
    func_8004DE1C();
}

void annual_callback_audio_4052(void) {
    func_8008A0D4(0x4052);
    func_8004DE1C();
}

void annual_callback_audio_4643(void) {
    func_8008A0D4(0x4643);
    func_8004DE1C();
}

void annual_callback_audio_5e5a(void) {
    func_80072734(0x5E5A);
    func_8004DE1C();
}
