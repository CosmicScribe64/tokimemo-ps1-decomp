/* career_guidance.c: ; src/ovl/ETC/8014ABD0.c:4-18 handles career-guidance talk mode. Original object 0x8014ABD0-0x8014C2D0 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/misc_scenes.h"

void career_guidance_entry(void) {
    switch (game_state.mode_stage) {
    case 0:
        career_guidance_stage_setup();
        break;
    case 1:
        career_guidance_stage_talk();
        break;
    case 2:
        career_guidance_stage_choice();
        break;
    case 3:
        career_guidance_stage_ask_others();
        break;
    case 4:
        career_guidance_stage_university();
        break;
    case 5:
        career_guidance_stage_job();
        break;
    case 6:
        career_guidance_leave();
        break;
    }
    hud_cursor_update();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    hud_screen_frame_draw(0);
    dec_bg_draw(1);
    hud_mascot_update();
}

void career_guidance_init_screen(void) {
    s32 temp_v0;
    s32 sp28;

    draw2d3d(1, 0);
    actor_draw_enable(1);
    actor_clear_upper();
    back_clear_switch(1);
    tpage_buf_clear();
    sprite_tables_clear();
    hud_cursor_actor_init(0);
    hizuke_init();
    icon_disp_switch(0);
    parameter_disp_switch(0);
    message_window_init();
    text_system_reset();
    message_window_title_show(0);
    set_dec_bri(0);
    temp_v0 = dec_bg_cd_read(0x4055, 1);
    sp28 = temp_v0;
    if (temp_v0 != -1) {
        if ((temp_v0 == 0) || (temp_v0 == 1)) {
            mode_step_next();
            mode_step_next();
            dec_bg_show_set(1, sp28);
        }
    } else {
        mode_step_next();
    }
}

void career_guidance_background_wait(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, dec_bg_decode_to_vram(1));
        mode_step_next();
    }
}

void career_guidance_title_show(void) {
    message_window_set_title("進路指導");
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    set_dec_bri(0x80);
    mode_stage_next();
}

s32 career_guidance_stage_setup(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_init_screen();
        break;
    case 1:
        career_guidance_background_wait();
        break;
    case 2:
        career_guidance_title_show();
        break;
    }
}

s32 career_guidance_intro_line(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_guidance_intro_text, 0, 0xF);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 career_guidance_assess_params(void) {
    s32 index;
    s32 count;

    career_guidance_body_mind_deficit = 0;
    career_guidance_trivia_deficit = 0;
    career_guidance_looks_deficit = 0;
    count = 0;
    career_guidance_academic_deficit = 0;
    /* The nine contiguous parameter records start at GameState +0xFC. */
    for (index = 1; index != 8; index++) {
        printf("param %d %d \n", index, ((ParamValue *)((u8 *)&game_state + 0xFC))[index].value);
        if (((ParamValue *)((u8 *)&game_state + 0xFC))[index].value >= 0x65) {
            count++;
        }
    }
    if (game_state.param_liberal_arts.value < 0x65) {
        career_guidance_academic_deficit += 1;
    }
    if (game_state.param_science.value < 0x65) {
        career_guidance_academic_deficit++;
    }
    if (game_state.param_art.value < 0x65) {
        career_guidance_academic_deficit++;
    }
    if (game_state.param_athletics.value < 0x65) {
        career_guidance_body_mind_deficit += 1;
    }
    if (game_state.param_guts.value < 0x65) {
        career_guidance_body_mind_deficit += 2;
    }
    if (game_state.param_trivia.value < 0x65) {
        career_guidance_trivia_deficit += 1;
    }
    if (game_state.param_looks.value < 0x65) {
        career_guidance_looks_deficit += 1;
    }
    return count;
}

s32 career_guidance_closest_girl(void) {
    s32 selected_girl;
    u8 *closeness_cursor;
    u8 closeness;
    s32 best_closeness;
    u8 closeness_scores[16];
    s16 sp50[16];
    s32 girl_index;

    girl_scores(closeness_scores, sp50);
    best_closeness = 0x3C;
    selected_girl = 0;
    for (girl_index = 1, closeness_cursor = &closeness_scores[1]; girl_index != 11; girl_index++, closeness_cursor++) {
        printf("%d %d \n", girl_index, *closeness_cursor);
        closeness = *closeness_cursor;
        if ((best_closeness < closeness) && game_state.girls[girl_index].flags.f.met) {
            selected_girl = girl_index;
            best_closeness = closeness;
        }
    }
    return selected_girl;
}

void career_guidance_self_assessment(void) {
    GameState *g = &game_state;
    s32 saved_closest_girl;
    s32 qualified_param_count;
    s32 closest_girl;

    switch (g->substep) {
    case 0x0:
        qualified_param_count = career_guidance_assess_params();
        if (qualified_param_count == 7) {
            g->substep = 0x10;
            return;
        }
        if (qualified_param_count == 0) {
            g->substep = 0x20;
            return;
        }
        g->substep = 0x30;
        return;
    case 0x10:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&career_guidance_all_params_high_text, 0, 0xF);
        g->substep += 1;
        return;
    case 0x11:
        xa_wait();
        return;
    case 0x12:
        mode_stage_next();
        return;
    case 0x20:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&career_guidance_no_params_high_text, 0, 0xF);
        g->substep += 1;
        return;
    case 0x21:
        xa_wait();
        return;
    case 0x22:
        g->substep = 0x40;
        return;
    case 0x30:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&career_guidance_mixed_params_text, 0, 0xF);
        g->substep += 1;
        return;
    case 0x31:
        xa_wait();
        return;
    case 0x32:
        k_sub_reset();
        if (career_guidance_academic_deficit == 0) {
            g->substep += 2;
        } else if (career_guidance_academic_deficit == 1) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_one_academic_deficit_text, 0, 0xF);
            g->substep += 1;
        } else if (career_guidance_academic_deficit == 2) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_two_academic_deficits_text, 0, 0xF);
            g->substep += 1;
        } else {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_three_academic_deficits_text, 0, 0xF);
            g->substep += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x33:
        xa_wait();
        return;
    case 0x34:
        k_sub_reset();
        if (career_guidance_body_mind_deficit == 0) {
            g->substep += 2;
        } else if (career_guidance_body_mind_deficit == 1) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_athletics_deficit_text, 0, 0xF);
            g->substep += 1;
        } else if (career_guidance_body_mind_deficit == 2) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_guts_deficit_text, 0, 0xF);
            g->substep += 1;
        } else {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_body_mind_deficits_text, 0, 0xF);
            g->substep += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x35:
        xa_wait();
        return;
    case 0x36:
        k_sub_reset();
        if (career_guidance_trivia_deficit != 0) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_trivia_deficit_text, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->substep += 1;
            return;
        }
        g->substep += 2;
        return;
    case 0x37:
        xa_wait();
        return;
    case 0x38:
        k_sub_reset();
        if (career_guidance_looks_deficit != 0) {
            gnsx(g->player_family_name);
            sndisp(&career_guidance_looks_deficit_text, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->substep += 1;
            return;
        }
        g->substep += 2;
        return;
    case 0x39:
        xa_wait();
        return;
    case 0x3A:
        g->substep = 0x40;
        return;
    case 0x40:
        closest_girl = career_guidance_closest_girl();
        saved_closest_girl = closest_girl;
        if (closest_girl == 0) {
            g->substep = 0x50;
            return;
        }
        k_sub_reset();
        gnsx(g->player_family_name);
        g->current_girl = (s8) saved_closest_girl;
        sndisp(&career_guidance_selected_girl_text, 0, 0xF);
        g->substep += 1;
        return;
    case 0x41:
        xa_wait();
        return;
    case 0x42:
        g->substep = 0x50;
        return;
    case 0x50:
        k_sub_reset();
        gnsx(g->player_family_name);
        sndisp(&career_guidance_assessment_close_text, 0, 0xF);
        g->substep += 1;
        return;
    case 0x51:
        xa_wait();
        return;
    case 0x52:
        mode_stage_next();
        /* fallthrough */
    default:
        return;
    }
}


s32 career_guidance_stage_talk(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_intro_line();
        break;
    case 1:
        career_guidance_self_assessment();
        break;
    }
}

void career_guidance_question(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&career_guidance_question_open_text, 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&career_guidance_question_followup_text, 0, 0x1F);
        game_state.substep += 1;
        /* fallthrough */
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(&career_guidance_university_choice_text, &career_guidance_job_choice_text, &career_guidance_third_choice_text, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}

s32 career_guidance_choice_menu(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            mode_stage_set(4);
            break;
        case 1:
            mode_stage_set(5);
            break;
        case 2:
            mode_stage_set(3);
            break;
        }
    }
}

s32 career_guidance_stage_choice(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_question();
        break;
    case 1:
        career_guidance_choice_menu();
        break;
    }
}

s32 career_guidance_ask_others_prompt(void) {
    s32 sp34; /* FAKE: unused local, the original frame is 8 bytes larger (T-3330 slot rule); source unknown. T-6040 */
    s32 sp30;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_guidance_ask_others_text, 0, 0xF);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        menu_girl_taku_set(0, &sp30);
        k_disp_start(sp30);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        break;
    }
}

void career_guidance_ask_others_select(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if ((game_state.pad_pressed & PAD_CIRCLE) && game_state.menu_cursor_0 != -1) {
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        sndisp(career_guidance_girl_plan_line[game_state.menu_cursor_0], 0, 0xF);
        mode_step_next();
    }
}

s32 career_guidance_ask_others_return(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        break;
    case 1:
        mode_stage_set(2);
        game_state.substep = 2;
        break;
    }
}

s32 career_guidance_stage_ask_others(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_ask_others_prompt();
        break;
    case 1:
        career_guidance_ask_others_select();
        break;
    case 2:
        career_guidance_ask_others_return();
        break;
    }
}

void career_guidance_university_talk(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp((u8 *) &career_guidance_university_choice_text, 0, 0xF);
        game_state.player_flags_upper.b[1] = 0;
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&career_guidance_verdict_intro_text, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(career_guidance_university_line[career_guidance_university_verdict() & 3], 0, 0xE);
        talk_voice_play((s32) &career_guidance_university_voice_a, (s32) &career_guidance_university_voice_b, career_guidance_university_verdict() & 3, 0xE);
        game_state.substep += 1;
        return;
    case 5:
        talk_text_step();
        return;
    case 6:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_guidance_reaction_line[career_guidance_university_verdict() & 3], 0, 0xF);
        game_state.substep += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        mode_stage_set(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 career_guidance_university_verdict(void) {
    s32 pad; /* FAKE: unused local, makes the frame 0x38 and puts the spill at sp+0x2C as in the original; real source unknown. T-4010 */
    s32 sum;
    s32 n;

    sum = 0;
    n = 0;
    if (game_state.param_liberal_arts.value >= 0x79) {
        n = 1;
        sum = game_state.param_liberal_arts.value - 0x78;
    }
    if (game_state.param_science.value >= 0x79) {
        n += 1;
        sum += game_state.param_science.value;
        sum -= 0x78;
    }
    if (game_state.param_art.value >= 0x79) {
        n += 1;
        sum += game_state.param_art.value;
        sum -= 0x78;
    }
    if ((n == 3) && (sum >= 0xC9)) {
        n = 3;
    } else if (n != 0) {
        n -= 1;
    }
    printf("thl %d \n", n);
    return 3 - n;
}

s32 career_guidance_job_verdict(void) {
    s32 n;

    n = -career_guidance_university_verdict() * 2 + 6;
    if (game_state.param_trivia.value >= 0x3D) {
        n += 1;
    }
    if (game_state.param_looks.value >= 0x3D) {
        n += 1;
    }
    if (n >= 7) {
        return 0;
    }
    if (n >= 5) {
        return 1;
    }
    if (n >= 3) {
        return 2;
    }
    return 3;
}

/* The fallthrough s32 form preserves the selector/result copies. T-10020. */
s32 career_guidance_teacher_background(void) {
    s32 result;
    GameState *state;

    state = &game_state;
    switch (state->substep) {
    case 0:
        result = dec_bg_cd_read(0x408D, 1);
        if (result != -1) {
            if (result == 0 || result == 1) {
                game_state.substep += 2;
                dec_bg_show_set(1, result);
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            game_state.substep += 1;
        }
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 career_guidance_stage_university(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_teacher_background();
        break;
    case 1:
        career_guidance_university_talk();
        break;
    }
}

void career_guidance_job_talk(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp((u8 *) &career_guidance_job_choice_text, 0, 0xF);
        game_state.player_flags_upper.b[1] = 1;
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&career_guidance_verdict_intro_text, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(career_guidance_job_line[career_guidance_job_verdict() & 3], 0, 0xE);
        talk_voice_play((s32) &career_guidance_job_voice_a, (s32) &career_guidance_job_voice_b, career_guidance_job_verdict() & 3, 0xE);
        game_state.substep += 1;
        return;
    case 5:
        talk_text_step();
        return;
    case 6:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_guidance_reaction_line[career_guidance_job_verdict() & 3], 0, 0xF);
        game_state.substep += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        mode_stage_set(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 career_guidance_stage_job(void) {
    switch (game_state.mode_step) {
    case 0:
        career_guidance_teacher_background();
        break;
    case 1:
        career_guidance_job_talk();
        break;
    }
}

void career_guidance_leave(void) {
    day_event_next(1);
}
