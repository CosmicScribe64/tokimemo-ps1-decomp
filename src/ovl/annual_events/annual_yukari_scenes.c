/* annual_yukari_scenes.c: ; src/ovl/GYOZI/8013CDD0.c:64 sets character index 5 for Yukari before dispatching its scene callbacks. Original object 0x8013CDD0-0x8013D120 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80147D00;

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_80147D64;

void annual_yukari_scene_substate_dispatch(void) {
    if (annual_scene_substate == 0) {
        annual_yukari_scene_group_a_step_dispatch();
        return;
    }
    annual_yukari_scene_group_b_step_dispatch();
}

void annual_yukari_scene_group_a_step_dispatch(void) {
    s32 mode_step_index; /* declared before yukari_scene_callbacks: its stack slot sits above yukari_scene_callbacks (T-3330) */
    FnTbl25 yukari_scene_callbacks;

    yukari_scene_callbacks = D_80147D00;
    mode_step_index = D_800F647A;
    yukari_scene_callbacks.f[mode_step_index]();
}

void annual_yukari_scene_sound_play(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

void annual_yukari_scene_group_b_step_dispatch(void) {
    s32 mode_step_index; /* declared before yukari_scene_callbacks: its stack slot sits above yukari_scene_callbacks (T-3330) */
    FnTbl19 yukari_scene_callbacks;

    yukari_scene_callbacks = D_80147D64;
    mode_step_index = D_800F647A;
    yukari_scene_callbacks.f[mode_step_index]();
}

void annual_yukari_scene_setup(void) {
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
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9120 */
    func_8008F618(*(u8 *)&D_800F62CF = 5);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147444;
    annual_scene_talk_command_table = D_80147468;
    annual_scene_talk_voice_table = D_8014748C;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_yukari_scenes.rodata", D_80145BE0);

void annual_yukari_scene_text_state_reset(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BE0);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_yukari_scenes.rodata", D_80145BEC);

void annual_yukari_scene_text_state_select(void) {
    annual_scene_talk_group = 5;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BEC);
    func_8004DE1C();
}

void annual_yukari_scene_audio_play(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void annual_yukari_scene_voice_select(void) {
    if (D_800F62CF == 5) {
        func_80072734(0x5F0E);
    } else {
        func_80072734(0x5482);
    }
    func_8004DE1C();
}
