/* annual_ayako_scenes.c: ; src/ovl/GYOZI/8013B920.c:78 sets character index 3 for Ayako; the callbacks display the art/music-room sequences. Original object 0x8013B920-0x8013BD50 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147908;

void annual_ayako_scene_group_dispatch(void) {
    u8 sel = annual_scene_substate; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        annual_ayako_scene_primary_mode_step_dispatch();
        return;
    case 1:
        annual_ayako_scene_secondary_mode_step_dispatch();
        return;
    default:
        annual_ayako_scene_default_mode_step_dispatch();
        return;
    }
}

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_801478B0;

void annual_ayako_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_801478B0;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_ayako_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147908;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147958;

void annual_ayako_scene_default_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147958;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_ayako_scene_state_init(void) {
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
    func_8008F618(*(u8 *)&D_800F62CF = 3);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147438;
    annual_scene_talk_command_table = D_8014745C;
    annual_scene_talk_voice_table = D_80147480;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_ayako_scenes.rodata", D_80145B80);

void annual_ayako_scene_callback_state_reset(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145B80);
    func_8004DE1C();
}

void annual_ayako_art_room_state_init(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 8;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "美術室");
    func_8004DE1C();
}

void annual_ayako_music_room_state_init(void) {
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F62CF].unk_10[0])->f = 1;
    annual_scene_talk_group = 0xB;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, "音楽室");
    func_8004DE1C();
}

void annual_ayako_background_load(void) {
    func_8008A0D4(0x4011);
    func_8004DE1C();
}
