/* annual_scene_callbacks.c: ; src/ovl/GYOZI/8013BD50.c:4-14 contains callbacks and a scene-state helper. Original object 0x8013BD50-0x8013C3B0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

void annual_scene_audio_command_play_4080(void) {
    func_8008A0D4(0x4080);
    func_8004DE1C();
}

void annual_scene_audio_command_play_4078(void) {
    func_8008A0D4(0x4078);
    func_8004DE1C();
}

void annual_scene_selected_girl_flag_and_score_update(void) {
    s32 a;
    s32 b;

    ((GirlFlag4 *)&D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10[0])->f = 1;
    if ((D_800F5488 >> 0x1C) == 4) {
        a = 1;
    } else {
        a = 0;
    }
    if ((D_800F5602 >> 4) == 4) {
        b = 1;
    } else {
        b = 0;
    }
    if (a == 0 && b != 0) {
        annual_scene_talk_group += 2;
    } else if (a == 0 && b == 0) {
        annual_scene_talk_group += 4;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10[0] &= 0xFFFB;
    }
    func_8004DE1C();
}

void annual_scene_resource_address_scalars_init(void) {
    D_801479B0 = (u8 *)0x801E8088;
    D_801479B4 = (u8 *)0x801E808C;
    D_801479B8 = (u8 *)0x801E80A0;
    D_801479BC = *(s16 *)0x801E80B4;
    D_801479C0 = (u8 *)0x801B0000;
    D_801479C4 = (u8 *)0x801B2000;
    D_801479C8 = (u8 *)0x801B4000;
    D_801479CC = (u8 *)0x801B8000;
    D_801479D0 = (u8 *)0x801BC000;
    D_801479D4 = (u8 *)0x801C0000;
    D_801479D8 = (u8 *)0x801C4000;
    D_801479DC = (u8 *)0x801C8000;
    D_801479E0 = (u8 *)0x801D0000;
    D_801479E4 = (u8 *)0x801D4000;
    D_801479E8 = (u8 *)0x801D8000;
    D_801479EC = (u8 *)0x801DC000;
    D_801479F0 = (u8 *)0x801E0000;
    D_801479F4 = (u8 *)0x801E4000;
}

void annual_scene_mode_step_dispatch_call(void) {
    annual_scene_54_entry_callback_dispatch();
}

typedef struct {
    void (*f[54])();
} FnTbl54; /* size 0xD8 */
extern FnTbl54 D_801479F8;

void annual_scene_54_entry_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl54 tbl;

    tbl = D_801479F8;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    if (annual_scene_talk_group == 0) {
        if (annual_scene_talk_line == 3) {
            D_8012E684 = 0;
        }
    }
}

void annual_scene_resource_command_and_addresses_init(void) {
    func_80051DD8(0x71, 0x801B0000, 0x82A1);
    annual_scene_resource_address_scalars_init();
    func_8004DE1C();
}

void annual_scene_audio_command_play_0501(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void annual_scene_state_flag_update(void) {
    annual_scene_talk_run_current();
    if (annual_scene_talk_line == 3) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

void annual_scene_resource_state_setup(void) {
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
    /* FAKE: indirect byte-store and reload via address-of shares $a0 across both stores and the call (T-9020 idiom, as in annual_scene_group_state_init and func_8013DC00). T-24084 */
    *(u8 *)&D_800F5ACD = *(u8 *)&D_800F62CF = 7;
    func_8008F618(*(u8 *)&D_800F62CF);
    annual_scene_mode_callback_addresses_init();
    annual_scene_talk_text_table = D_8014743C;
    annual_scene_talk_command_table = D_80147460;
    annual_scene_talk_voice_table = D_80147484;
    annual_scene_resource_address_scalars_init();
    if (D_800F53DF >= 6U && D_800F53DF < 0xAU) {
        func_8004EE18((s32)D_801479C0, 0x11, 1, 2, 0);
        func_8008FD1C((s32)D_801479C8, (s32)D_801479CC, (s32)D_801479D0, (s32)D_801479D4, (s32)D_801479D8, (s32)D_801479DC);
    } else {
        func_8004EE18((s32)D_801479C4, 0x11, 1, 2, 0);
        func_8008FD1C((s32)D_801479E0, (s32)D_801479E4, (s32)D_801479E8, (s32)D_801479EC, (s32)D_801479F0, (s32)D_801479F4);
    }
    func_8008FF60((s32)D_801479B4, (s32)D_801479B8, (s32)D_801479B0, D_801479BC);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_scene_callbacks.rodata", D_80145BA0);

void annual_scene_callback_state_reset(void) {
    annual_scene_talk_group = 0;
    annual_scene_talk_line = 0;
    func_800BCE10(&D_800D92A0, &D_80145BA0);
    func_8004DE1C();
}

void annual_scene_audio_command_play_3fed(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void annual_scene_audio_command_play_466f(void) {
    func_8008A0D4(0x466F);
    func_8004DE1C();
}
