/* annual_hallway_mode_step_dispatch.c: ; src/ovl/GYOZI/8013E1D0.c:9-16 dispatches its callback table using D_800F647A. Original object 0x8013E1D0-0x8013E5B0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80148114;

void annual_hallway_mode_step_dispatch(void) {
    s32 mode_step_index; /* declared before hallway_mode_step_callbacks: its stack slot sits above hallway_mode_step_callbacks (T-3330) */
    FnTbl25 hallway_mode_step_callbacks;

    hallway_mode_step_callbacks = D_80148114;
    mode_step_index = D_800F647A;
    hallway_mode_step_callbacks.f[mode_step_index](0x80);
}

void annual_hallway_resource_pointer_setup(void) {
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}

void annual_hallway_conditional_sound(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x202);
        func_8004DE1C();
    }
}

void annual_hallway_scene_resource_command(void) {
    func_80051DD8(9, 0x80197000, 0xA46B);
    annual_hallway_resource_addresses_init();
    func_8004DE1C();
}

void annual_hallway_scene_state_init(void) {
    s16 i;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, "廊下");
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom, as SHOUGATU func_8013B758); T-17600 */
    func_8008F618(*(u8 *)&D_800F62CF = 0xC);
    annual_hallway_resource_addresses_init();
    annual_hallway_talk_text_table = D_8014806C;
    annual_hallway_talk_command_table = D_801480A0;
    annual_hallway_talk_voice_table = D_801480D4;
    annual_hallway_talk_group = 0;
    annual_hallway_talk_line = 0;
    D_80148110 = 0;
    for (i = 0; i < 0xB; i++) {
        if (D_800F53A0.girl[i].unk_10[1] & 1) {
            D_80148110 = 1;
        }
    }
    func_8004DE1C();
}

void annual_hallway_audio_setup(void) {
    func_80090960(7, 0);
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void annual_hallway_year_counter_delta(void) {
    annual_hallway_talk_group += ((u8) D_800F53DE >= 0x61U) * 4;
    func_8004DE1C();
}

void annual_hallway_flag_clear_counter_delta(void) {
    if (D_80148110 == 0) {
        annual_hallway_talk_group += 1;
    }
    func_8004DE1C();
}

void annual_hallway_flag_clear_route(void) {
    if (D_80148110 != 0) {
        func_8004DE1C();
        return;
    }
    annual_hallway_scene_load_setup();
}

void annual_hallway_flag_set_exit_route(void) {
    if (D_80148110 != 0) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}
