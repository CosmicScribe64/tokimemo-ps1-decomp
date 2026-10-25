/* annual_sound_callbacks.c: ; src/ovl/GYOZI/8013D470.c:9-28 provides sound and wait callbacks. Original object 0x8013D470-0x8013DB50 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80147F04;

void annual_callback_audio_4040(void) {
    func_8008A0D4(0x4040);
    func_8004DE1C();
}

void annual_callback_audio_6346(void) {
    func_80072734(0x6346);
    func_8004DE1C();
}

void annual_callback_audio_6157(void) {
    func_80072734(0x6157);
    func_8004DE1C();
}

void annual_callback_conditional_selector_5(void) {
    if (((u32) D_800F5488 >> 0x1C) == 9) {
        annual_scene_talk_group = 5;
    }
    func_8004DE1C();
}

void annual_callback_selector_3(void) {
    annual_scene_talk_group = 3;
    func_8004DE1C();
}

void annual_sound_callback_dispatch(void) {
    u8 sel = annual_scene_substate; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        annual_sound_callback_group_0_dispatch();
        return;
    case 1:
        annual_sound_callback_group_1_dispatch();
        return;
    default:
        annual_sound_callback_default_group_dispatch();
        return;
    }
}

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
extern FnTbl37 D_80147E70;

void annual_sound_callback_group_0_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_80147E70;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_sound_callback_group_1_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147F04;
    idx = D_800F647A;
    tbl.f[idx]();
}

extern FnTbl20 D_80147F54;

void annual_sound_callback_default_group_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80147F54;
    idx = D_800F647A;
    tbl.f[idx]();
}

void annual_callback_audio_658f(void) {
    func_80072734(0x658F);
    func_8004DE1C();
}

void annual_sound_scene_state_init(void) {
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
    D_800F53A0.unk_01C[0x1E] = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    /* FAKE: byte lvalue keeps the store and call argument in a0; original spelling unknown. T-9530 */
    func_8008F618(*(u8 *)&D_800F53A0.unk_F2F = 4);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_8014744C;
    annual_scene_talk_command_table = D_80147470;
    annual_scene_talk_voice_table = D_80147494;
    ((GirlFlag4 *)&D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10[0])->f = 1;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_sound_callbacks.rodata", D_80145C20);

void annual_sound_table_0_callback(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C20);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_sound_callbacks.rodata", D_80145C28);

void annual_sound_table_12_callback(void) {
    annual_scene_talk_group = 0xC;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C28);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_sound_callbacks.rodata", D_80145C34);

void annual_sound_table_15_callback(void) {
    annual_scene_talk_group = 0xF;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145C34);
    func_8004DE1C();
}

void annual_callback_audio_3ff5(void) {
    func_8008A0D4(0x3FF5);
    func_8004DE1C();
}

void annual_callback_audio_4037(void) {
    func_8008A0D4(0x4037);
    func_8004DE1C();
}

void annual_callback_conditional_selector_6(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        annual_scene_talk_group = 6;
    }
    func_8004DE1C();
}

void annual_callback_conditional_selector_8_or_10(void) {
    if (((D_800F5638 << 0x1D) >> 0x1F) == (D_800F5488 >> 0x1C)) {
        if (((u32)D_800F563A >> 4) == 7) {
            annual_scene_talk_group = 0xA;
        } else {
            annual_scene_talk_group = 8;
        }
    }
    func_8004DE1C();
}

void annual_callback_conditional_selector_4(void) {
    if (((D_800F5638 << 0x1D) >> 0x1F) != (D_800F5488 >> 0x1C)) {
        annual_scene_talk_group = 4;
    }
    func_8004DE1C();
}

void annual_callback_conditional_advance_sequence(void) {
    if (((u32) (D_800F5638 << 0x1D) >> 0x1F) == ((u32) D_800F5488 >> 0x1C)) {
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
}
