/* annual_scene_mode_stage_dispatch.c: ; src/ovl/GYOZI/8013A140.c:41-92 dispatches scene callback groups by D_800F6479. Original object 0x8013A140-0x8013A920 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/GYOZI.h"

/* Bit 1 of the girl's flag byte. T-9720. */
typedef struct GyoziFlag2 {
    /* 0x00 */ u8 pad : 1;
    /* 0x00 */ u8 set : 1;
    /* 0x00 */ u8 rest : 6;
} GyoziFlag2; /* size 0x01 */

void annual_scene_mode_callback_addresses_init(void) {
    D_80147430 = 0x801976E8;
    D_80147434 = 0x80198B88;
    D_80147438 = 0x8019A368;
    D_8014743C = 0x8019ADBC;
    D_80147440 = 0x8019BC6C;
    D_80147444 = 0x8019C8EC;
    D_80147448 = 0x8019D434;
    D_8014744C = 0x8019E138;
    D_80147450 = 0x8019EAF8;
    D_80147454 = 0x80197760;
    D_80147458 = 0x80198C90;
    D_8014745C = 0x8019A400;
    D_80147460 = 0x8019ADE8;
    D_80147464 = 0x8019BD20;
    D_80147468 = 0x8019C934;
    D_8014746C = 0x8019D498;
    D_80147470 = 0x8019E1E0;
    D_80147474 = 0x8019EB54;
    D_80147478 = 0x80197C2C;
    D_8014747C = 0x80199724;
    D_80147480 = 0x8019AB1C;
    D_80147484 = 0x8019AFF0;
    D_80147488 = 0x8019C484;
    D_8014748C = 0x8019CC44;
    D_80147490 = 0x8019D8E4;
    D_80147494 = 0x8019E784;
    D_80147498 = 0x8019EDB0;
}

void annual_scene_mode_dispatch(void) {
    func_8008E6AC();
    switch (D_800F6479) {
    case 0:
        annual_scene_selected_girl_resource_setup();
        break;
    case 1:
        annual_scene_choice_completion_gate();
        break;
    case 2:
        annual_scene_resource_sector_read();
        break;
    case 3:
        func_80089244();
        if (D_800F647A != 0) {
            func_8004DDD8();
        }
        break;
    case 4:
        switch (D_801474B0) {
        case 0:
            talk_voice_play();
            break;
        case 1:
            annual_timing_sequence_substate_dispatch();
            break;
        case 2:
            annual_choice_scene_substate_dispatch();
            break;
        case 3:
            annual_ayako_scene_group_dispatch();
            break;
        case 4:
            annual_sound_callback_dispatch();
            break;
        case 5:
            annual_yukari_scene_substate_dispatch();
            break;
        case 6:
            annual_nozomi_scene_group_dispatch();
            break;
        case 7:
            annual_scene_mode_step_dispatch_call();
            break;
        case 8:
            annual_scene_substate_dispatch();
            break;
        case 9:
            talk_voice_play();
            break;
        case 10:
            annual_scene_mode_step_update();
            break;
        default:
            talk_voice_play();
            break;
        }
        break;
    case 5:
        annual_scene_selected_girl_state_apply();
        break;
    default:
        talk_voice_play();
        break;
    }
    func_8008A148();
    func_8008F54C();
    func_80076450(2);
    func_80074A14();
    func_80075BFC();
    func_800748B8();
    func_8008E8B4();
    func_800530E0();
}

void annual_scene_selected_girl_state_apply(void) {
    u8 girl;
    GyoziWork *work;

    work = &D_800F53A0;
    /* The table index is at +0x70C; selectors sit at +0x68D with a four-byte stride. T-9720. */
    *(GyoziWord4 *)&work->girl[work->unk_410[0x27D + work->unk_410[0x2FC] * 4]].unk_08[0] = D_801474B4;
    girl = work->unk_410[0x27D + work->unk_410[0x2FC] * 4];
    if (girl != 8) {
        ((GyoziFlag2 *)&work->girl[girl].unk_10[0])->set = 1;
    } else if (((u32)(*(s32 *)&work->girl[8].unk_10[0] << 20) >> 29) == 3) {
        ((GyoziFlag2 *)&work->girl[girl].unk_10[0])->set = 1;
    }
    func_80090960(8, work->unk_F2F * 10 + D_801474B8);
    func_80050D60(0, 0);
    func_80081D30(1);
}

extern s32 D_800C3280[];
extern s32 D_800C32C0[];
extern s32 D_800C3300[];

void annual_scene_selected_girl_resource_setup(void) {
    u16 a1;
    u16 a2;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800744A0();
    func_80074950();
    func_8009068C();
    D_800F53A0.unk_F2F = D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4];
    D_801474B8 = D_8012E660;
    if (D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4] == 2 ||
        D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4] == 6) {
        a1 = 0xB33B;
        a2 = 0xB322;
    } else {
        a1 = 0xB320;
        a2 = 0xB2FD;
    }
    func_800504CC(1, a1, a2,
                  D_800C3280[D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4]],
                  D_800C32C0[D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4]],
                  D_800C3300[D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4]]);
    D_801474B4 = *(GyoziWord4 *)&D_800F53A0.girl[D_800F53A0.unk_410[0x27D + D_800F53A0.unk_410[0x2FC] * 4]].unk_08[0];
    D_800F53A0.girl[D_800F53A0.unk_410[0x27D + D_800F5AAC * 4]].unk_0A = 0x32;
    func_8004DDD8();
}

void annual_scene_choice_completion_gate(void) {
    if (func_80050AB8() == 1) {
        func_8004DDD8();
    }
}

void annual_scene_audio_command_0200(void) {
    func_80086AB0(0x200);
    func_8004DE1C();
}

void annual_scene_resource_sector_read(void) {
    func_80051DD8(0x10, 0x80197000, 0xA3B8);
    annual_scene_mode_callback_addresses_init();
    D_801474B0 = D_800F5A2D[D_800F5AAC * 4];
    func_8004DDD8();
}

void annual_scene_talk_run_current(void) {
    D_800D9234 = (u8 *)&D_801474A8;
    D_800D9238 = (u8 *)&D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 0);
}

void annual_scene_talk_run_current_with_flags(void) {
    D_800D9234 = (u8 *) &D_801474A8;
    D_800D9238 = (u8 *) &D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 1);
}
