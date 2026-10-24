/* FAKE: s32 return view keeps count in $v1 across the call in func_801352B0 because void return lets uopt colour count into $v0. T-24091 */
#define SHUGAKU_OVERRIDE_func_801343B0
#include "common.h"
#include "ovl/SHUGAKU.h"

s32 func_801343B0(void);

void func_80134880(void) {
    D_8013C190 = (u8 *)0x801EA87C;
    D_8013C194 = (u8 *)0x801EA89C;
    D_8013C198 = (u8 *)0x801EA940;
    D_8013C19C = *(s16 *)0x801EA968;
    D_8013C1A0 = (u8 *)0x801B0000;
    D_8013C1A4 = (u8 *)0x801B2000;
    D_8013C1A8 = (u8 *)0x801B6000;
    D_8013C1AC = (u8 *)0x801BA000;
    D_8013C1B0 = (u8 *)0x801BE000;
    D_8013C1B4 = (u8 *)0x801C2000;
    D_8013C1B8 = (u8 *)0x801C6000;
}

void func_80134930(void) {
    if (game_state.saved_stage == 3) {
        func_80134970();
        return;
    }
    func_80134B94();
}

void func_80134970(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl25 tbl;

    tbl = D_8013C1C4;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_801349EC(void) {
    game_state.current_girl = game_state.date_girl;
    dialogue_names_set(game_state.date_girl);
    main_step_next();
}

void func_80134A20(void) {
    if (D_8013C1BC != 0) {
        normal_date_move_place();
        return;
    }
    if (game_state.substep == 0) {
        dec_bg_show_switch(1);
        game_state.substep += 1;
    }
    func_8007C8A4();
}

void func_80134A8C(void) {
    strcpy(D_800CA19C, "ホテル・廊下");
    func_801365F4();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    if (dec_bg_brightness != 0) {
        D_8013C1BC = 1;
    } else {
        D_8013C1BC = 0;
    }
    main_step_next();
}

void func_80134B2C(void) {
    main_stage_set(game_state.saved_stage);
    main_step_set(game_state.saved_step);
}

void func_80134B64(void) {
    main_stage_set((game_state.saved_stage + 1) & 0xFF);
}

void func_80134B94(void) {
    u8 index; /* declared above table and reread after the call: same callback idiom as EVENT func_800FA6A0 (T-18000) */
    ShFnTbl41 table;

    table = D_8013C228;
    index = game_state.step;
    table.f[index](0x80);
    index = game_state.step;
    if (D_80085368 == table.f[index]) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
    }
    if (func_800853FC == table.f[index]) {
        ((Actor *)actor_pool)[98].brightness = ((Actor *)actor_pool)[96].brightness;
        if (((Actor *)actor_pool)[96].brightness < 9) {
            ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
        }
    }
}

void func_80134C98(void) {
    if (((game_state.girls[0].birthday.w & 0xF) == game_state.month) && (((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day)) {
        game_state.saved_stage = 5;
        game_state.saved_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        main_stage_set(6);
        return;
    }
    if ((game_state.month == (game_state.player_birthday & 0xF)) && (game_state.day == ((u32) (game_state.player_birthday << 0x17) >> 0x1B))) {
        game_state.saved_stage = 5;
        game_state.saved_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.daily_event_bits |= 8;
        main_stage_set(7);
        return;
    }
    main_step_next();
}

s32 func_80134D8C(void) {
    func_801343B0();
    if ((game_state.step_timer.w++ == 1) && (D_800CA2E8 == 2)) {
        D_80122D0C = 0;
        func_80067DD4();
    }
}

s32 func_80134DE8(void) {
    if (game_state.substep == 0) {
        k_reset(0);
        message_window_set_title("ホテル・廊下");
        game_state.substep += 1;
    }
    func_801343B0();
    if (D_800CA2E8 == 1) {
        if (game_state.step_timer.u++ == 1) {
            snd_command(0x503);
        }
    }
}

void func_80134E74(void) {
    snd_command(0x205);
    main_step_next();
}

void func_80134E9C(void) {
    cd_read_sectors(0x76, 0x801B0000, 0x8D47);
    func_80134880();
    main_step_next();
}

void func_80134ED4(void) {
    game_state.date_girl = game_state.current_girl;
    game_state.current_girl = GIRL_YOSHIO;
    dialogue_names_set(0xD);
    main_step_next();
}

void func_80134F18(void) {
    func_801365F4();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    func_80134880();
    load_palette(D_8013C1A0, 0x11, 1, 2, 0);
    event_still_show(D_8013C1A4, D_8013C1A8, D_8013C1AC, D_8013C1B0, D_8013C1B4, D_8013C1B8);
    event_anim_show(D_8013C194, D_8013C198, D_8013C190, D_8013C19C);
    D_80120698 = 8;
    D_80120654 = 8;
    D_801206AC = 3;
    /* actor_pool_96 and D_801206D8 are records 96 and 98 of one table: the loads stay behind the stores. T-9310, T-20900 */
    ((Actor *)actor_pool_96)[2] = ((Actor *)actor_pool_96)[0];
    ((Actor *)actor_pool_96)[2].flags = 0x40;
    ((Actor *)actor_pool_96)[2].unk_18 = 0x28;
    ((Actor *)actor_pool_96)[2].unk_01 -= 1;
    D_800CA2E4 = 3;
    D_800CA2E8 = 0;
    game_state.tpage_buf[5] = -1;
    game_state.tpage_buf[6] = -1;
    game_state.tpage_buf[7] = -1;
    game_state.tpage_buf[8] = -1;
    game_state.tpage_buf[9] = -1;
    D_80122D0C = 1;
    D_8013C1C0 = *(u8 *)&game_state.voice_mode;
    main_step_next();
}

void func_801350F4(void) {
    if (D_80122CDC != 0) {
        game_state.step += 0x1D;
        D_800CA2DC += 1;
        return;
    }
    event_log_add(2, 0x20);
    main_step_next();
    D_800CA2F4 = 1;
    game_state.event_flags[0x20] = 1;
    game_state.girls[game_state.current_girl].tokimeki += 5;
    game_state.girls[game_state.current_girl].affection += 2;
    check_para_limit();
}

void func_801351CC(void) {
    func_80137C3C();
    if ((game_state.girls[0].birthday.w & 0xF) == game_state.month && ((u32) (game_state.girls[0].birthday.w << 0x17) >> 0x1B) == game_state.day) {
        game_state.saved_stage += 1;
        game_state.saved_step = 0;
        game_state.date_girl = game_state.current_girl;
        game_state.current_girl = GIRL_SHIORI;
        main_stage_set(6);
        return;
    }
    main_stage_set((game_state.saved_stage + 1) & 0xFF);
}

void func_80135278(void) {
    game_state.voice_mode = D_8013C1C0;
    D_800CA2DC = 0x29;
    main_step_next();
}

void func_801352B0(void) {
    s32 pad; /* FAKE: unused local above page and count gives the 0x38 frame and puts page at sp+0x30 and count at sp+0x2C; real source unknown. T-24091 */
    s32 page;
    s32 count;

    if (*(u8 *)&game_state.voice_mode == 2) {
        *(u8 *)&game_state.voice_mode = 1;
    }
    count = game_state.step_timer.w;
    page = game_state.step;
    if (!(game_state.pad_pressed & 0x600060) || (D_801206FE < -0xBF)) {
        func_801343B0();
    }
    game_state.step_timer.w = count + 1;
    if (D_801206FE >= -0xBF) {
        if (!(game_state.step_timer.w & 1) && (check_end_k() != 0)
            && ((*(u8 *)&game_state.voice_mode == 0) || (func_80046094() == 9))) {
            D_801206FE--;
        }
        D_8011ED5B &= 0x7F;
    }
    if (page != game_state.step_timer.w) {
        if (D_801206FE >= -0xBF) {
            /* FAKE: u32 view prevents uopt from CSE-caching the s32 load of page across the branch, reproducing the reload from sp+0x30. T-24091 */
            game_state.step = *(u32 *)&page;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
