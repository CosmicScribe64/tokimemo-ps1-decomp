/* career_choice.c: ; src/ovl/ETC/801492C0.c:4-32 handles job or university choice mode. Original object 0x801492C0-0x8014ABD0 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

void career_choice_entry(void) {
    switch (game_state.mode_stage) {
    case 0:
        career_choice_stage_prompt();
        break;
    case 1:
        career_choice_stage_university();
        break;
    case 2:
        career_choice_stage_job();
        break;
    case 3:
        career_choice_stage_club_dream();
        break;
    case 4:
        career_choice_leave();
        break;
    }
    hud_cursor_update();
    hizuke_show();
    message_window_show();
    message_window_title_update();
    hud_screen_frame_draw(0);
    dec_bg_draw(get_last_gamen_mode());
    career_choice_panel_update();
    hud_mascot_update();
}

s32 career_choice_init_screen(void) {
    draw2d3d(1, 0);
    actor_draw_enable(1);
    actor_clear_upper();
    back_clear_switch(1);
    tpage_buf_clear();
    sprite_tables_clear();
    icon_disp_switch(0);
    parameter_show_init();
    parameter_disp_switch(1);
    message_window_init();
    text_system_reset();
    message_window_set_title("就職・進学");
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    message_window_title_show(0);
    dec_bg_draw(0);
    career_choice_question_no = 0;
    career_choice_threshold_start = rand() % 12;
    career_choice_tier = 0;
    career_choice_correct_count = 0;
    mode_step_next();
}


s32 career_choice_mastered_club(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (((ClubFlagBits *)&game_state.clubs[i])->unk_25 >= 3 && ((ClubFlagBits *)&game_state.clubs[i])->b30) {
            return i;
        }
    }
    return -1;
}

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
s32 career_choice_prompt(void) {
    s32 sp28;
    s32 r;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        if (game_state.player_flags_upper.b[1] != 0) {
            sndisp(career_choice_job_prompt_text, 0, 0xF);
        } else {
            sndisp(career_choice_university_prompt_text, 0, 0xF);
        }
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        r = career_choice_mastered_club();
        if (r != -1) {
            sp28 = r;
            x_taku_menu_set(0, 3);
            if (game_state.player_flags_upper.b[1] != 0) {
                x_taku_string_set(career_choice_job_first_choice_text, career_choice_job_second_choice_text, career_club_dream_name[sp28], 3);
            } else {
                x_taku_string_set(career_choice_university_first_choice_text, career_choice_university_second_choice_text, career_club_dream_name[sp28], 3);
            }
            k_sub_disp_start(3);
            game_state.menu_cursor_0 = 0;
        } else {
            x_taku_menu_set(0, 2);
            if (game_state.player_flags_upper.b[1] != 0) {
                x_taku_string_set(career_choice_job_first_choice_text, career_choice_job_second_choice_text, 0, 2);
            } else {
                x_taku_string_set(career_choice_university_first_choice_text, career_choice_university_second_choice_text, 0, 2);
            }
            k_sub_disp_start(2);
            game_state.menu_cursor_0 = 0;
        }
        mode_step_next();
        return;
    }
}

s32 career_choice_menu(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            if (game_state.player_flags_upper.b[1] != 0) {
                mode_stage_set(2);
            } else {
                mode_stage_set(1);
            }
            break;
        case 1:
            if (game_state.player_flags_upper.b[1] != 0) {
                mode_stage_set(1);
            } else {
                mode_stage_set(2);
            }
            break;
        case 2:
            mode_stage_set(3);
            break;
        }
    }
}

void career_choice_panel_update(void) {
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
}

s32 career_choice_stage_prompt(void) {
    switch (game_state.mode_step) {
    case 0:
        career_choice_init_screen();
        break;
    case 1:
        career_choice_prompt();
        break;
    case 2:
        career_choice_menu();
        break;
    }
}

s32 career_choice_university_tier_prompt(void) {
    switch (game_state.substep) {
    case 0:
        game_state.player_flags_upper.b[1] = 0;
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        sndisp(&career_choice_university_tier_prompt_text, 0, 0x1F);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set((s32)career_choice_university_first_tier_text, (s32)career_choice_university_second_tier_text, (s32)career_choice_university_third_tier_text, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        break;
    }
}

s32 career_choice_tier_menu(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (game_state.menu_cursor_0) {
        case 0:
            career_choice_tier = 0;
            mode_step_next();
            break;
        case 1:
            career_choice_tier = 1;
            mode_step_next();
            break;
        case 2:
            career_choice_tier = 2;
            mode_step_next();
            break;
        }
    }
}

void career_choice_tier_confirm(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_choice_university_tier_line[career_choice_tier], 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&career_choice_tier_confirm_text, 0, 0xF);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}


/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
s32 career_choice_exam_room_load(void) {
    s32 r;

    switch (*(u8 *)&game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_choice_exam_room_text, 0, 0x1F);
        k_sub_disp_start(2);
        r = dec_bg_cd_read(0x463B, 1);
        if (r != -1) {
            if (r == 0 || r == 1) {
                game_state.substep += 2;
                dec_bg_show_set(1, r);
                set_dec_bri(0x80);
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            if (bg_decode_result == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0x463B, 1);
            } else {
                set_dec_bri(0x80);
                game_state.substep += 1;
            }
        }
        break;
    case 2:
        parameter_disp_switch(0);
        dec_bg_draw(1);
        set_dec_bri(0x80);
        hizuke_init();
        mode_step_next();
        break;
    }
}

s32 career_choice_exam_sound_load(void) {
    switch (game_state.substep) {
    case 0:
        audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
        game_state.substep += 1;
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(1, 0x205);
            mode_step_next();
        }
        break;
    }
}

s32 career_choice_exam_sd_load(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors(9U, 0x801E0000, 0x7D81);
        game_state.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            addr_init_else_sd(1);
            career_choice_exam_face_init();
            mode_step_next();
        }
        break;
    }
}

void career_choice_exam_face_init(void) {
    actor_clear(0x60);
    ((Actor *)actor_pool_96)->unk_01 = 1;
    ((Actor *)actor_pool_96)->flags = 4;
    ((Actor *)actor_pool_96)->quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)->draw_flags = 0x84;
    *(u8 **)&actor_pool_96[0xC] = D_80125CA8;
    *(u8 **)&actor_pool_96[0x10] = D_80125CAC;
    *(u8 **)&actor_pool_96[0x34] = D_80125CB0;
    ((Actor *)actor_pool_96)->unk_05 = 2;
    ((Actor *)actor_pool_96)->blink_period = 1;
    ((Actor *)actor_pool_96)->unk_43 = 0x10;
    ((Actor *)actor_pool_96)->sequence_slot = 0;
    ((Actor *)actor_pool_96)->x = -0x50;
    ((Actor *)actor_pool_96)->y = -0x40;
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
}

void career_choice_exam_question(void) {
    /* FAKE: dead register load from unsigned global before switch triggers OPTN zcopy:0 in cvt_pass.py (T-24129). */
    register s32 pad;
    s32 answer_wrong;
    s32 club_bonus;
    s32 subject_score;

    pad = game_state.substep;
    answer_wrong = 0;
    switch (career_choice_question_no % 3) {
    case 0:
        club_bonus = game_state.clubs[0].practice_points >> 4;
        subject_score = game_state.param_liberal_arts.value;
        if ((club_bonus > 0) && (club_bonus >= 0x15)) {
            club_bonus = 0x14;
        }
        subject_score += club_bonus;
        if (game_state.player_flags.b[1] & 0xF) {
            subject_score = subject_score / 2;
        }
        break;
    case 1:
        club_bonus = game_state.clubs[2].practice_points >> 4;
        subject_score = game_state.param_science.value;
        if ((club_bonus > 0) && (club_bonus >= 0x15)) {
            club_bonus = 0x14;
        }
        subject_score += club_bonus;
        if (game_state.player_flags.b[1] & 0xF) {
            subject_score = subject_score / 2;
        }
        break;
    default:
        printf("DEFAUT:");
        /* fallthrough */
    case 2:
        club_bonus = game_state.clubs[4].practice_points >> 4;
        subject_score = game_state.param_art.value;
        if ((club_bonus > 0) && (club_bonus >= 0x15)) {
            club_bonus = 0x14;
        }
        subject_score += club_bonus;
        if (game_state.player_flags.b[1] & 0xF) {
            subject_score = subject_score / 2;
        }
        break;
    }
    if (career_choice_tier == 1) {
        subject_score += 0x26;
    }
    if (career_choice_tier == 2) {
        subject_score += 0x36;
    }
    career_choice_question_no += 1;
    if (subject_score < (&career_choice_threshold)[(career_choice_threshold_start + career_choice_question_no) % 12]) {
        answer_wrong = 1;
    }
    if (answer_wrong == 0) {
        career_choice_correct_count += 1;
    }
    ((Actor *)actor_pool_96)->flags = 5;
    ((Actor *)actor_pool_96)->unk_08 = 0;
    ((Actor *)actor_pool_96)->sequence_slot = answer_wrong;
    ((Actor *)actor_pool_96)->unk_18 = 0;
    game_state.substep += 1;
}

s32 career_choice_exam_question_wait(void) {
    if (D_80120652 & 1) {
        game_state.mode_step_timer.w = 0;
        return;
    }
    if (D_80120658 == 0) {
        if (D_80120666 != 0) {
            audio_command(0x502);
        } else {
            audio_command(0x501);
        }
    }
    if (D_80120658++ >= 0x3D) {
        if (career_choice_question_no >= 0xA) {
            game_state.substep += 1;
            return;
        }
        game_state.substep -= 1;
    }
}

s32 career_choice_exam_run(void) {
    switch (game_state.substep) {
    case 0:
        career_choice_exam_question();
        break;
    case 1:
        career_choice_exam_question_wait();
        break;
    case 2:
        mode_step_next();
        break;
    }
}

s32 career_choice_exam_grade(void) {
    switch (career_choice_correct_count) {
    case 10:
        return 0;
    case 8:
    case 9:
        return 1;
    case 7:
        return 2;
    case 5:
    case 6:
        return 3;
    case 3:
    case 4:
        return 4;
    case 1:
    case 2:
    default:
        return 5;
    }
}

s32 career_choice_interview_ok(void) {
    if (game_state.param_trivia.value + game_state.param_looks.value > 0x100) {
        return 1;
    }
    return 0;
}

void career_choice_result_code_set(void) {
    if (game_state.player_flags_upper.b[1]) {
        switch (career_choice_tier) {
        case 2:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 0x20;
            } else if (career_choice_exam_grade() == 2 && career_choice_interview_ok()) {
                game_state.player_flags_upper.b[1] = 0x20;
            } else {
                game_state.player_flags_upper.b[1] = 0xA0;
            }
            break;
        case 1:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 0x10;
            } else if (career_choice_exam_grade() == 2 && career_choice_interview_ok()) {
                game_state.player_flags_upper.b[1] = 0x10;
            } else {
                game_state.player_flags_upper.b[1] = 0x90;
            }
            break;
        case 0:
        default:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 8;
            } else if (career_choice_exam_grade() == 2 && career_choice_interview_ok()) {
                game_state.player_flags_upper.b[1] = 8;
            } else {
                game_state.player_flags_upper.b[1] = 0x88;
            }
            break;
        }
    } else {
        switch (career_choice_tier) {
        case 2:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 4;
            } else {
                game_state.player_flags_upper.b[1] = 0x84;
            }
            break;
        case 1:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 2;
            } else {
                game_state.player_flags_upper.b[1] = 0x82;
            }
            break;
        case 0:
        default:
            if (career_choice_exam_grade() < 2) {
                game_state.player_flags_upper.b[1] = 1;
            } else {
                game_state.player_flags_upper.b[1] = 0x81;
            }
            break;
        }
    }
    printf("shinro %x \n", game_state.player_flags_upper.b[1]);
}

void career_choice_result_talk(void) {
    u32 temp_v0;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_choice_result_line[career_choice_exam_grade()], 0, 0x1F);
        temp_v0 = career_choice_exam_grade();
        switch (temp_v0) {
        case 0:
            audio_bank_start(1, 0x204);
            break;
        case 1:
            audio_bank_start(1, 0x201);
            break;
        case 2:
            audio_bank_start(1, 0x201);
            break;
        case 3:
            audio_bank_start(1, 0x200);
            break;
        case 4:
            audio_bank_start(1, 0x203);
            break;
        case 5:
            audio_bank_start(1, 0x203);
            break;
        }
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&career_choice_announce_line, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        career_choice_result_code_set();
        mode_stage_set(4);
        /* fallthrough */
    default:
        return;
    }
}


void career_choice_stage_university(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        career_choice_university_tier_prompt();
        return;
    case 1:
        career_choice_tier_menu();
        return;
    case 2:
        career_choice_tier_confirm();
        return;
    case 3:
        career_choice_exam_room_load();
        return;
    case 4:
        career_choice_exam_sound_load();
        return;
    case 5:
        career_choice_exam_sd_load();
        return;
    case 6:
        career_choice_exam_run();
        return;
    case 7:
        career_choice_result_talk();
        /* fallthrough */
    default:
        return;
    }
}

void career_choice_job_tier_prompt(void) {
    switch (game_state.substep) {
    case 0:
        game_state.player_flags_upper.b[1] = 1;
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(&career_choice_job_tier_prompt_text, 0, 0x1F);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        x_taku_menu_set(0, 3);
        x_taku_string_set(&career_choice_job_first_tier_text, &career_choice_job_second_tier_text, &career_choice_job_third_tier_text, 3);
        k_sub_disp_start(3);
        game_state.menu_cursor_0 = 0;
        mode_step_next();
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}

void career_choice_job_tier_confirm(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(career_choice_job_tier_line[career_choice_tier], 0, 0xF);
        game_state.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(&career_choice_tier_confirm_text, 0, 0xF);
        game_state.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        mode_step_next();
        /* fallthrough */
    default:
        return;
    }
}


void career_choice_stage_job(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        career_choice_job_tier_prompt();
        return;
    case 1:
        career_choice_tier_menu();
        return;
    case 2:
        career_choice_job_tier_confirm();
        return;
    case 3:
        career_choice_exam_room_load();
        return;
    case 4:
        career_choice_exam_sound_load();
        return;
    case 5:
        career_choice_exam_sd_load();
        return;
    case 6:
        career_choice_exam_run();
        return;
    case 7:
        career_choice_result_talk();
        /* fallthrough */
    default:
        return;
    }
}

s32 career_choice_club_dream(void) {
    s32 temp_v0;
    s32 sp28;

    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        gnsx(&game_state.player_family_name);
        temp_v0 = career_choice_mastered_club();
        sp28 = temp_v0;
        sndisp(career_club_dream_line[temp_v0], 0, 0xF);
        if ((u16)game_state.clubs[sp28].practice_points >= 0x19BU) {
            game_state.player_flags_upper.b[1] = 0x40;
        } else {
            game_state.player_flags_upper.b[1] = 0xC0;
        }
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        mode_stage_set(4);
        break;
    }
}

void career_choice_stage_club_dream(void) {
    if (game_state.mode_step == 0) {
        career_choice_club_dream();
    }
}

void career_choice_leave(void) {
    actor_clear_upper();
    printf("shinro %x \n", game_state.player_flags_upper.b[1]);
    day_event_next(1);
}
