/* annual_choice_scene_handlers.c: ; src/ovl/GYOZI/8013AE80.c:9-27 dispatches six choice-scene steps. Original object 0x8013AE80-0x8013B920 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_801477F8;

void annual_choice_scene_substate_dispatch(void) {
    switch (annual_scene_substate) {
    case 0:
        annual_choice_scene_mode_step_dispatch();
        return;
    case 1:
        annual_choice_scene_group_b_step_dispatch();
        return;
    case 2:
        annual_choice_scene_timing_step_dispatch();
        return;
    case 3:
        annual_choice_scene_fourth_mode_step_dispatch();
        return;
    case 4:
        annual_choice_scene_fifth_mode_step_dispatch();
        return;
    default:
        annual_choice_scene_default_mode_step_dispatch();
        return;
    }
}

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */

extern FnTbl20 D_80147654;

void annual_choice_scene_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before choice_scene_mode_step_callbacks: its stack slot sits above choice_scene_mode_step_callbacks (T-3330) */
    FnTbl20 choice_scene_mode_step_callbacks;

    choice_scene_mode_step_callbacks = D_80147654;
    mode_step_index = D_800F647A;
    choice_scene_mode_step_callbacks.f[mode_step_index]();
    if (annual_scene_talk_group == 1) {
        if (annual_scene_talk_line == 8) {
            if (D_800F6474++ == 0) {
                func_80086AB0(0x500);
            }
        }
    }
}

void annual_choice_scene_timing_advance(void) {
    annual_scene_talk_run_current();
    if (annual_scene_talk_line == 2) {
        if (D_800F6474++ == 0) {
            func_80086AB0(0x501);
        }
    }
}

extern FnTbl20 D_801476A4;

void annual_choice_scene_group_b_step_dispatch(void) {
    s32 mode_step_index; /* declared before choice_scene_callbacks: its stack slot sits above choice_scene_callbacks (T-3330) */
    FnTbl20 choice_scene_callbacks;

    choice_scene_callbacks = D_801476A4;
    mode_step_index = D_800F647A;
    choice_scene_callbacks.f[mode_step_index]();
}

void annual_choice_scene_timing_step_dispatch(void) {
    s32 mode_step_index; /* declared before choice_timing_callbacks: its stack slot sits above it (T-3330) */
    GzCallbacks90 choice_timing_callbacks;

    choice_timing_callbacks = D_801476F4;
    if (D_80147650 == 0 && D_800F647A >= 0x13 && D_800F647A < 0x1C) {
        func_80056574(0x60, -0x10, 0x30, 8, 8, 0xD);
    }
    if (D_80147650 == 1 && D_800F647A >= 0x13 && D_800F647A < 0x1C) {
        func_80056574(-0x3C, -0x20, 0x20, 8, 8, 0xD);
    }
    mode_step_index = D_800F647A;
    choice_timing_callbacks.callbacks[mode_step_index]();
}

void annual_choice_scene_substate_timing_advance(void) {
    annual_scene_talk_run_current();
    if (D_800F6474++ == 0) {
        if (annual_scene_talk_line == 3) {
            func_80086AB0(0x500);
        }
    }
}

void annual_choice_scene_audio_command_0500(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

void annual_choice_scene_audio_command_0501(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void annual_choice_scene_selector_advance(void) {
    if (D_80147650 != 0) {
        annual_scene_talk_group += 1;
    }
    func_8004DE1C();
}

typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
extern FnTbl29 D_80147784;

void annual_choice_scene_fourth_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl29 tbl;

    tbl = D_80147784;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_choice_scene_fifth_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_801477F8;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147858;

void annual_choice_scene_default_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147858;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_choice_scene_setup(void) {
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_8008F618(*(u8 *)&D_800F62CF = 2);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147434;
    annual_scene_talk_command_table = D_80147458;
    annual_scene_talk_voice_table = D_8014747C;
    if (((u32)D_800F55CA >> 4) == 3) {
        D_80147650 = 1;
    } else {
        D_80147650 = 0;
    }
    func_8004DE1C();
}

void annual_choice_scene_text_state_init_0(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void annual_choice_scene_text_state_init_3(void) {
    annual_scene_talk_group = 3;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\213\263\216\272");
    func_8004DE1C();
}

void annual_choice_scene_text_state_init_6(void) {
    annual_scene_talk_group = 6;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_800BCE10(&D_800D92E0, "\216\300\214\261\216\272");
    func_8004DE1C();
}

void annual_choice_scene_girl_flag_set_0e(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 0xE;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "教室");
    func_800BCE10(&D_800D92E0, "実験室");
    func_8004DE1C();
}

void annual_choice_scene_girl_flag_set_12(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 0x12;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "実験室");
    func_8004DE1C();
}

void annual_choice_scene_girl_flag_set_17(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 0x17;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "実験室");
    func_8004DE1C();
}

void annual_choice_scene_background_load_3fff(void) {
    func_8008A0D4(0x3FFF);
    func_8004DE1C();
}

void annual_choice_scene_background_load_3fe3(void) {
    func_8008A0D4(0x3FE3);
    func_8004DE1C();
}

void annual_choice_scene_sound_select(void) {
    if (D_80147650 != 0) {
        func_8008A0D4(0x406F);
    } else {
        func_8008A0D4(0x4065);
    }
    func_8004DE1C();
}

void annual_choice_scene_background_load_4065(void) {
    func_8008A0D4(0x4065);
    func_8004DE1C();
}

void annual_choice_scene_background_load_406f(void) {
    func_8008A0D4(0x406F);
    func_8004DE1C();
}

void annual_choice_scene_selector_advance_if_clear(void) {
    if (D_80147650 != 0) {
        annual_scene_talk_group += 1;
    }
    func_8004DE1C();
}

void annual_choice_scene_selector_advance_if_set(void) {
    if (D_80147650 == 0) {
        annual_scene_talk_group += 1;
    }
    func_8004DE1C();
}
