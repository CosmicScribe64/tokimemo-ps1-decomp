/* annual_dialogue_choices.c: ; src/ovl/GYOZI/80134FD0.c:25-73 reads the selected dialogue and prepares choice responses. Original object 0x80134FD0-0x80135900 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[60])();
} FnTbl60; /* size 0xF0 */
extern FnTbl60 D_80145F68;

void annual_dialogue_choice_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl60 tbl;

    tbl = D_80145F68;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_dialogue_choice_sound_play_0603(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

/* The setup callee's integer return type reproduces the counter-test registers,
 * as in the SHOUGATU sibling func_8013315C. T-20001. */
s32 annual_dialogue_choice_first_entry_sound_gate(void) {
    annual_mode_scene_data_setup();
    if (D_800F6474++ == 0) {
        if (((u8 **)D_80145EAC)[D_80145EB4][D_80145EB8 - 1] == 7) {
            if (func_800BDC20(((s32 **)D_80145EA8)[D_80145EB4][D_80145EB8 - 1], "\203\113\203\140\203\203") == 0) {
                func_80086AB0(0x60C);
            }
        }
    }
}

s32 annual_dialogue_choice_input_process_five(void) {
    s32 dialogue_row_audio_command_id;

    if ((func_8005B32C() != 0) && (D_80145EB8 < 5)) {
        if (D_80145EB8 == 0) {
            if (D_80145F64 < 0x5A) {
                D_80145F64 += 1;
                if (D_80145F64 == 1) {
                    func_80086AB0(0x60A);
                }
                if (D_80145F64 == 0x1E) {
                    func_80086AB0(0x604);
                }
                return 0;
            }
            if (!(D_800F647D & 7)) {
                dialogue_row_audio_command_id = (((D_800C3370[(D_800F62CF * 7) + (D_800F647D >> 3)] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                /* FAKE: retain the original unchanged-byte store before the call.
                 * Direct self-assignment is removed; original spelling unknown. T-20004. */
                D_800F647D |= 0;
                func_80086AB0(dialogue_row_audio_command_id);
            }
        }
        if (D_800F647D++ < 0x40U) {
            return 0;
        }
        D_800F647D = 0;
        func_8005B06C(1);
    }
    annual_mode_scene_data_setup();
    if (D_80145EB8 < 5) {
        if (D_800F6474++ == 1) {
            func_80086AB0(0x608);
        }
    }
}

s32 annual_dialogue_choice_input_process_three(void) {
    s32 dialogue_row_audio_command_id;

    if ((func_8005B32C() != 0) && (D_80145EB8 < 3)) {
        if (D_80145EB8 == 0) {
            if (D_80145F64 < 0x5A) {
                D_80145F64 += 1;
                if (D_80145F64 == 1) {
                    func_80086AB0(0x60A);
                }
                if (D_80145F64 == 0x1E) {
                    func_80086AB0(0x604);
                }
                return 0;
            }
            if (!(D_800F647D & 7)) {
                dialogue_row_audio_command_id = (((D_800C3370[(D_800F62CF * 7) + (D_800F647D >> 3)] + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                /* FAKE: retain the original unchanged-byte store before the call.
                 * Direct self-assignment is removed; original spelling unknown. T-20004. */
                D_800F647D |= 0;
                func_80086AB0(dialogue_row_audio_command_id);
            }
        }
        if (D_800F647D++ < 0x40U) {
            return 0;
        }
        D_800F647D = 0;
        func_8005B06C(1);
    }
    annual_mode_scene_data_setup();
    if ((D_80145EB8 < 3) && (D_800F6474 == 1)) {
        func_80086AB0(0x608);
    }
    if ((D_80145EB8 == 3) && (D_800F6474 == 1)) {
        func_80086AB0(0x60B);
    }
    D_800F6474 += 1;
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/annual_dialogue_choices.rodata", D_801458E8);

void annual_dialogue_choice_text_and_state_setup(void) {
    func_800BCE10(&D_800D92E0, &D_801458E8);
    D_80145F64 = 0;
    D_80145F60 = 0;
    D_80145EC8 = 1;
    D_80145EB4 = 6;
    func_8004DE1C();
}

void annual_dialogue_choice_conditional_sound_or_skip(void) {
    if (D_80145EC4 == 0) {
        func_8008A0D4(0x4107);
    } else {
        D_800F647A += 3;
    }
    func_8004DE1C();
}

void annual_dialogue_choice_sound_play_4107(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

void annual_dialogue_choice_sound_play_40e5(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

/* FAKE: s32 fall-through preserves the original return/register allocation. T-10010. */
s32 annual_dialogue_target_girl_choice_route(void) {
    s32 selected_dialogue_target_index;
    u8 previous_target_girl_index;

    if (D_8012E66C == 0xB) {
        annual_dialogue_fallback_resource_setup();
        return 0;
    }
    selected_dialogue_target_index = D_8012E66C;
    previous_target_girl_index = D_800F62CF;
    if (previous_target_girl_index == selected_dialogue_target_index && D_80145EC4 != 0) {
        func_8004DE1C();
        return 0;
    }
    if (D_800F53A0.girl[D_8012E66C].unk_10[3] == 0) {
        D_80145EB4 = 0;
        D_80145EA8 = D_80145E34;
        D_80145EAC = D_80145E6C;
        D_80145EB0 = D_80145EA4;
        func_8008F618(D_8012E66C);
        func_8004DE1C();
        return 0;
    }
    D_800F62CF = (u8)selected_dialogue_target_index;
    annual_girl_dialogue_tables_select(D_8012E66C);
    func_8008F618((s32) D_800F62CF);
    D_80145EB4 = 9;
    func_8004DE1C();
    func_8004DE1C();
    func_8004DE1C();
}

void annual_dialogue_default_choice_resource_setup(void) {
    D_80145EB4 = 7;
    D_80145EA8 = D_80145E08;
    D_80145EAC = D_80145E40;
    D_80145EB0 = D_80145E78;
    func_8004DE1C();
    D_800F647A -= 5;
}

void annual_dialogue_rank_score_gate(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
}

void annual_dialogue_zero_choice_extra_advance(void) {
    if (D_8012E66C == 0) {
        func_8004DE1C();
    }
    func_8004DE1C();
}

void annual_dialogue_fallback_resource_setup(void) {
    D_80145F60 = 1;
    D_80145EB4 = 0;
    D_80145EA8 = D_80145E00;
    D_80145EAC = D_80145E38;
    D_80145EB0 = D_80145E70;
    func_8004DDD8();
}

void annual_dialogue_rank_score_delta_apply(void) {
    s32 masked_target_status;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    masked_target_status = (u8)func_8005E0E0(D_800F62CF) & 0x7F;
    switch (masked_target_status) {
    case 0:
        break;
    case 1:
    case 2:
        D_80145EB4 += 1;
        break;
    case 3:
        D_80145EB4 += 2;
        break;
    }
    func_8004DE1C();
}

void annual_dialogue_score_set_27(void) {
    D_80145EB4 = 0x27;
    func_8004DE1C();
}

void annual_dialogue_nonzero_choice_score_advance(void) {
    if (D_8012E66C != 0) {
        D_80145EB4 += 1;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
    }
    func_8004DE1C();
}

void annual_dialogue_choice_exit_gate(void) {
    if (D_800F62CF != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void annual_dialogue_score_set_0f(void) {
    D_80145EB4 = 0xF;
    func_8004DE1C();
}
