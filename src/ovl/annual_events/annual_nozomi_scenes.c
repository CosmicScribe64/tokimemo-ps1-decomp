/* annual_nozomi_scenes.c: ; src/ovl/GYOZI/8013D120.c:65 sets character index 6 for Nozomi before installing scene resources and talk callbacks. Original object 0x8013D120-0x8013D470 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147E18;

void annual_nozomi_scene_group_dispatch(void) {
    if (annual_scene_substate == 0) {
        annual_nozomi_scene_primary_mode_step_dispatch();
        return;
    }
    annual_nozomi_scene_secondary_mode_step_dispatch();
}

typedef struct {
    void (*f[26])();
} FnTbl26; /* size 0x68 */
extern FnTbl26 D_80147DB0;

void annual_nozomi_scene_primary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26 tbl;

    tbl = D_80147DB0;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_nozomi_scene_audio_command_0502(void) {
    func_80086AB0(0x502);
    func_8004DE1C();
}

void annual_nozomi_scene_secondary_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147E18;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_nozomi_scene_state_init(void) {
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
    D_8012E678 = 0;
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    /* FAKE: the byte-typed store as the argument keeps the constant in $a0 (T-9020 idiom); T-9340 */
    func_8008F618(*(u8 *)&D_800F62CF = 6);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_80147448;
    annual_scene_talk_command_table = D_8014746C;
    annual_scene_talk_voice_table = D_80147490;
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10[0])->f = 1;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_nozomi_scenes.rodata", D_80145C00);

void annual_nozomi_scene_callback_state_reset(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C00);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_nozomi_scenes.rodata", D_80145C0C);

void annual_nozomi_scene_selector_state_init(void) {
    annual_scene_talk_group = 6;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C0C);
    func_8004DE1C();
}

void annual_nozomi_scene_background_load(void) {
    func_8008A0D4(0x4276);
    func_8004DE1C();
}
