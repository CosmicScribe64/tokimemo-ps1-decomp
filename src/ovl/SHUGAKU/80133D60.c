#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80133D60(void) {
    D_8013BFD0 = (u8 *)0x801D2384;
    D_8013BFD4 = (u8 *)0x801D238C;
    D_8013BFD8 = (u8 *)0x801D23F0;
    D_8013BFDC = *(s16 *)0x801D240C;
    D_8013BFE0 = (u8 *)0x801B0000;
    D_8013BFE4 = (u8 *)0x801B2000;
    D_8013BFE8 = (u8 *)0x801B6000;
    D_8013BFEC = (u8 *)0x801BA000;
    D_8013BFF0 = (u8 *)0x801BE000;
    D_8013BFF4 = (u8 *)0x801C2000;
    D_8013BFF8 = (u8 *)0x801C6000;
}

void func_80133E10(void) {
    u8 index;
    ShFnTbl44 table;

    table = D_8013C008;
    if (game_state.step < 8) {
        light_effect_draw();
    } else if (game_state.step >= 0x1D && game_state.step < 0x23) {
        light_effect_draw();
    }
    index = game_state.step;
    table.f[index](0x80);
    index = game_state.step;
    if (D_80085368 == table.f[index]) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (index >= 0xD && index < 0x16) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (func_800853FC == table.f[index]) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
        if (((Actor *)actor_pool)[98].brightness < 8) {
            ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
        }
    }
}

void func_80133F6C(void) {
    if (((game_state.girls[0].birthday.w & 0xF) == game_state.month) && (((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day)) {
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        main_stage_set(6);
        return;
    }
    if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B))) {
        game_state.date_girl = game_state.current_girl;
        main_stage_set(7);
        return;
    }
    main_step_next();
}

s32 func_80134024(void) {
    func_801343B0();
    if (game_state.step_timer.w++ == 0x3C) {
        D_801206AA = 1;
    }
}

void func_80134070(void) {
    game_state.girls[game_state.current_girl].affection = game_state.girls[game_state.current_girl].affection + 2;
    check_para_limit();
    snd_command(0x201);
    func_80132BCC();
}

void func_801340C8(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x83FB);
    func_80133D60();
    main_step_next();
}

void func_80134100(void) {
    if (game_state.param_liberal_arts.value >= 0x78) {
        main_step_next();
    }
    main_step_next();
}

void func_80134138(void) {
    game_state.current_girl = GIRL_AYAKO;
    D_80120666 = 2;
    D_801206EE = 4;
    dialogue_names_set(3);
    main_step_next();
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/SHUGAKU/80133D60.rodata", D_8013AD70);

void func_80134184(void) {
    game_state.current_girl = GIRL_SOTOI;
    D_80120666 = 4;
    D_801206EE = 2;
    strcpy(&name_buf_girl_short, &D_8013AD70);
    main_step_next();
}

void func_801341DC(void) {
    if (game_state.param_liberal_arts.value >= 0x78) {
        D_800CA2E4 += 3;
    }
    main_step_next();
}

void func_80134220(void) {
    snd_command(0x204);
    bg_read_sub2(0x476A);
    event_log_add(2, 0x1A);
    main_step_next();
}

void func_8013425C(void) {
    func_801365F4();
    D_8013BFFC = D_8013C530;
    D_8013C000 = D_8013C66C;
    D_8013C004 = D_8013C7A8;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    func_80133D60();
    load_palette(D_8013BFE0, 0x11, 1, 2, 0);
    event_still_show(D_8013BFE4, D_8013BFE8, D_8013BFEC, D_8013BFF0, D_8013BFF4, D_8013BFF8);
    event_anim_show(D_8013BFD4, D_8013BFD8, D_8013BFD0, D_8013BFDC);
    ((Actor *)actor_pool_96)[1].unk_04 = 8;
    ((Actor *)actor_pool_96)->unk_04 = 8;
    *(Actor *) &actor_pool_96[0x88] = *(Actor *) actor_pool_96;
    ((Actor *)actor_pool_96)[2].sequence_slot = 4;
    main_step_next();
}

void func_801343B0(void) {
    dialogue_tables = &D_800CA2E4;
    D_800CA138 = &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    dialogue_run(D_80122CDC, 1, 0);
}

void func_8013442C(void) {
    dialogue_tables = (u8 *) &D_800CA2E4;
    D_800CA138 = (u8 *) &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    dialogue_run(0xFF, 1, 0);
}
