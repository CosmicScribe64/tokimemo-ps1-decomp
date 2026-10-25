/* annual_scene_mode_step_dispatch.c: ; src/ovl/GYOZI/8013A920.c:9-23 dispatches callbacks by D_800F647A. Original object 0x8013A920-0x8013AE80 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_801474C0;

void annual_scene_substate_dispatch(void) {
    u8 scene_substate = annual_scene_substate; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (scene_substate) {
    case 0:
        annual_primary_scene_mode_step_dispatch();
        return;
    case 1:
        annual_scene_mode_step_group_1_dispatch();
        return;
    case 2:
        annual_scene_mode_step_group_2_dispatch();
        return;
    default:
        annual_scene_group_default_mode_step_dispatch();
        return;
    }
}

void annual_primary_scene_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before scene_mode_step_callbacks: its stack slot sits above scene_mode_step_callbacks (T-3330) */
    FnTbl23 scene_mode_step_callbacks;

    scene_mode_step_callbacks = D_801474C0;
    mode_step_index = D_800F647A;
    scene_mode_step_callbacks.f[mode_step_index]();
}

void annual_scene_audio_command_0500(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

s32 annual_scene_dialogue_audio_gate(void) {
    annual_scene_talk_run_current();
    if (D_800F6474++ == 1) {
        if (annual_scene_talk_line == 3) {
            func_8008E408(3);
            func_80072FE8();
        }
    }
}

extern FnTbl23 D_8014751C;

void annual_scene_mode_step_group_1_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_8014751C;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl23 D_80147578;

void annual_scene_mode_step_group_2_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80147578;
    idx = D_800F647A;
    tbl.f[idx]();
}

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */
extern FnTbl28 D_801475D4;

void annual_scene_group_default_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl28 tbl;

    tbl = D_801475D4;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_scene_group_state_init(void) {
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
    *(u8 *)&D_800F5ACD = *(u8 *)&D_800F62CF = 8;
    func_8008F618(*(u8 *)&D_800F62CF);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147430;
    annual_scene_talk_command_table = D_80147454;
    annual_scene_talk_voice_table = D_80147478;
    func_8004DE1C();
}


void annual_scene_group_selector_set_zero(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void annual_scene_group_selector_set_three(void) {
    annual_scene_talk_group = 3;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void annual_scene_group_selector_set_six(void) {
    annual_scene_talk_group = 6;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void annual_scene_group_selector_set_nine(void) {
    annual_scene_talk_group = 9;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "\230\114\211\272");
    func_8004DE1C();
}

void annual_scene_group_audio_command_3fed(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}
