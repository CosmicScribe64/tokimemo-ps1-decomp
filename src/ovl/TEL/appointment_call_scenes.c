/* appointment_call_scenes.c: src/ovl/TEL/801397D0.c:56: appointment-dependent phone scenes, same-girl appointment cancellation, ringing/text/voice and exit routing; body 60: sel = ((AppointmentBits *)&game_state.unk_54C[0].w)->girl; 61: for (i = 0; i != 8; i++) {. Original object 0x801397D0-0x8013CCD0 (config/objects/TEL.txt). */
#define MAIN_API_OVERRIDE_k_sub_reset /* T-10860: legacy integer reset calls in three matched handlers. */
#include "common.h"
#include "ovl/TEL.h"
#include "common.h"
#include "game/game_state.h"
#include "game/girl.h"
#include "game/schedule_records.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/message.h"
#include "main/speaker.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"

/* The legacy call view returns int; canonical k_sub_reset retains void. */
s32 k_sub_reset();

void phone_outgoing_call_mode_stage_dispatch(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_stage) {
    case 0:
        phone_date_invitation_mode_dispatch();
        break;
    case 1:
        phone_date_result_mode_dispatch();
        break;
    case 2:
        phone_outgoing_call_mode_dispatch_a();
        break;
    case 3:
        phone_outgoing_call_mode_dispatch_b();
        break;
    case 4:
        phone_special_call_mode_dispatch();
        break;
    case 5:
        phone_outgoing_call_mode_dispatch_c();
        break;
    case 6:
        phone_high_syoushin_call_dispatch();
        break;
    case 7:
        phone_lost_gift_response_dispatch();
        break;
    case 8:
        phone_club_absence_exit_dispatch();
        break;
    case 9:
        phone_culture_festival_club_notice_dispatch();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    hud_cursor_update();
    message_window_title_update();
    dec_bg_draw(0);
    hud_screen_frame_draw(0);
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
}

void phone_same_girl_appointments_cancel(void) {
    s32 i;
    u32 sel;

    sel = ((AppointmentBits *)&game_state.unk_54C[0].w)->girl;
    for (i = 0; i != 8; i++) {
        if (((AppointmentBits *)&game_state.unk_54C[i])->active && sel == ((AppointmentBits *)&game_state.unk_54C[i])->girl) {
            game_state.unk_54C[i].b[3] &= 0xFFDF;
            printf("canceled %d \n", i);
        }
    }
    hud_appointment_list_sort();
}

s32 phone_date_invitation_mode_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        if (((u32) (game_state.unk_54C[0].w * 8) >> 0x1C) == 0xA) {
            mode_step_set(0xA);
            return;
        }
        phone_answering_message_scene_init();
        return;
    case 1:
        phone_appointment_result_dialogue_run();
        return;
    case 2:
        phone_wrong_date_place_response();
        return;
    case 3:
        phone_return_to_day_screen();
        return;
    case 10:
        phone_home_call_scene_init();
        return;
    case 11:
        phone_opening_call_dialogue_run();
        return;
    default:
        mode_step_next();
        return;
    }
}

void phone_opening_call_dialogue_run(void) {
    switch (game_state.substep) {
    case 0:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.substep += 1;
            game_state.mode_step_timer.u = 0;
            audio_command(0x50A);
            return;
        }
        return;
    case 1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_8014544C, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 0, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 2:
        talk_text_step();
        return;
    case 3:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80145450, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 4:
        xa_wait();
        return;
    case 5:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80145454, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 2, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 6:
        talk_text_step();
        return;
    case 7:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80145458, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 8:
        xa_wait();
        return;
    case 9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_8014545C, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 4, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 10:
        talk_text_step();
        return;
    case 11:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80145460, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 12:
        xa_wait();
        return;
    case 13:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80145464, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 6, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 14:
        talk_text_step();
        return;
    case 15:
        audio_command(0x50B);
        game_state.substep += 1;
        return;
    default:
        mode_step_set(2);
        break;
    }
}

/* FAKE: the implicit return keeps the selector in v0, and the repeated table
 * expression preserves the original address-temporary order after CSE. T-9720. */
s32 phone_missed_date_relationship_adjust(void) {
    GirlRecord *girl;
    s16 delta;
    u32 index;

    index = (u32)(game_state.unk_54C[0].w * 8) >> 0x1C;
    girl = &game_state.girls[index];
    delta = phone_missed_appointment_stat_delta[index];
    girl->affection -= phone_missed_appointment_stat_delta[index];
    girl->syoushin += delta;
}

s32 phone_missed_appointment_response(void) {
    /* FAKE: 4 spare bytes above the buffers reproduce the original frame (0x150); real locals unknown. T-9180 */
    s32 pad;
    u8 name[0x100];
    u8 num[0x20];

    switch (game_state.substep) {
    case 0:
        phone_missed_date_relationship_adjust();
        k_sub_reset();
        gnsx(game_state.player_family_name);
        strcpy(name, "しまった！！今日は、 ");
        get_g_name(num, (u32)(game_state.unk_54C[0].w * 8) >> 0x1C);
        strcat(name, num);
        strcat(name, "と デートの約束があったんだ。）");
        sndisp(name, 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        audio_bank_start(0, 0x202);
        sndisp("これでは、おれは、 三国一の極悪人だ〜）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 3:
        xa_wait();
        return;
    default:
        phone_same_girl_appointments_cancel();
        mode_step_next();
        break;
    }
}

void phone_appointment_result_dialogue_run(void) {
    s32 status;
    /* FAKE: unused word preserves the index spill at sp+0x30. T-10860. */
    s32 unused;
    u32 girl;

    girl = ((AppointmentBits *)&game_state.unk_54C[0].w)->girl;
    status = get_g_zyotai_s(girl);
    switch (game_state.substep) {
    case 0x0:
        xa_wait();
        return;
    case 0x1:
        if (status & 0x80) {
            game_state.substep = 0xA;
            return;
        }
        game_state.substep = 0x32;
        return;
    case 0xA:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80144738[girl], 0, (s32) girl);
        talk_voice_play((s32) D_80144768, (s32) D_801447B8, (s32) girl, (s32) girl);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 0xB:
        talk_text_step();
        return;
    case 0x1E:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80144AA4[girl], 0, (s32) girl);
        talk_voice_play((s32) D_80144AD4, (s32) D_80144B24, (s32) girl, (s32) girl);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 0x1F:
        talk_text_step();
        return;
    case 0x32:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80144D6C[girl], 0, (s32) girl);
        talk_voice_play((s32) D_80144D9C, (s32) D_80144DEC, (s32) girl, (s32) girl);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 0x33:
        talk_text_step();
        return;
    /* FAKE: retain the original 71-entry jump-table extent. T-10860. */
    case 0x46:
    default:
        mode_step_next();
        return;
    }
}

s32 phone_date_result_mode_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        if (((u32) (game_state.unk_54C[0].w * 8) >> 0x1C) == 0xA) {
            mode_step_set(0xA);
            return;
        }
        phone_answering_message_scene_init();
        return;
    case 1:
        phone_appointment_result_dialogue_run();
        return;
    case 2:
        phone_missed_appointment_response();
        return;
    case 3:
        phone_return_to_day_screen();
        return;
    case 10:
        phone_home_call_scene_init();
        return;
    case 11:
        phone_opening_call_dialogue_run();
        /* fallthrough */
    default:
        return;
    }
}

s32 phone_wrong_date_place_response(void) {
    switch (game_state.substep) {
    case 0:
        phone_missed_date_relationship_adjust();
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp("しまった！！ デートの場所を 間違えたのか〜。）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        audio_bank_start(0, 0x202);
        sndisp("おれは、なんという ボケをかまして しまったんだ〜）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 3:
        xa_wait();
        return;
    default:
        phone_same_girl_appointments_cancel();
        mode_step_next();
        break;
    }
}

s32 phone_outgoing_call_mode_dispatch_a(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_home_call_scene_init();
        break;
    case 1:
        phone_missed_appointment_result_dialogue_run();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_outgoing_call_mode_dispatch_b(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_miharu_call_dialogue();
        break;
    case 2:
        phone_miharu_progress_increment();
        break;
    case 3:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_miharu_call_dialogue(void) {
    /* FAKE: unused word and declaration order retain girl at sp+0x2C. T-10860. */
    s32 unused;
    s32 table_index;
    s32 girl;


    girl = (s32) game_state.girls[11].flags.b[3];
    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        k_sub_reset();
        gnsx(girl_short_name_table[11]);
        table_index = girl;
        sndisp(D_80145C44[girl][0], 0, 0xB);
        talk_voice_play(D_80145C78[table_index], D_80145CAC[table_index], 0, 0xB);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 2:
        talk_text_step();
        return;
    case 3:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80145C44[girl][1], 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 4:
        xa_wait();
        return;
    case 5:
        k_sub_reset();
        gnsx(girl_short_name_table[11]);
        table_index = girl;
        sndisp(D_80145C44[girl][2], 0, 0xB);
        talk_voice_play(D_80145C78[table_index], D_80145CAC[table_index], 2, 0xB);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 6:
        talk_text_step();
        return;
    case 7:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80145C44[girl][3], 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 8:
        xa_wait();
        return;
    default:
        mode_step_next();
        return;
    }
}

void phone_miharu_progress_increment(void) {
    game_state.girls[11].flags.b[3] = game_state.girls[11].flags.b[3] + 1;
    mode_step_next();
}

s32 phone_special_call_mode_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_home_call_scene_init();
        break;
    case 1:
        phone_yoshio_relationship_report_run();
        break;
    case 2:
        phone_relationship_deltas_apply();
        break;
    case 3:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_outgoing_call_mode_dispatch_c(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_fireworks_call_dialogue();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_fireworks_call_dialogue(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        audio_command(0x50A);
        k_sub_reset();
        gnsx("留さん");
        sndisp("おう、 俺だ！留だ！ ", 0, 0xD);
        talk_voice_play(0x1D000095, 0x1C00009B, -1, 0xE);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 2:
        talk_text_step();
        return;
    case 3:
        k_sub_reset();
        gnsx("留さん");
        sndisp("今年の花火大会ではよぅ、 俺の昇り龍乱れ七変化を 楽しみにしてくんな！", 0, 0xD);
        talk_voice_play(0x0C000022, 0x05000039, -1, 0xE);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 4:
        talk_text_step();
        return;
    case 5:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp("何だ、この電話は？", 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 6:
        xa_wait();
        return;
    default:
        mode_step_next();
        return;
    }
}

s32 phone_home_call_scene_init(void) {
    s32 temp_v0;

    /* FAKE: scalar selector view preserves branch and decoder-result registers. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title((s32) "自宅");
        k_sub_disp_start(1);
        parameter_show_init();
        parameter_show_init();
        message_window_init();
        hizuke_init();
        hud_icon_bar_init();
        icon_disp_switch(1);
        schedule_command_icons_lock_to_record();
        icon_can_use_set(0, 0);
        icon_can_use_set(game_state.last_command.w & 0xF, 1);
        parameter_disp_switch(1);
        k_sub_reset_point_set();
        k_sub_disp_start(1);
        actor_clear_upper();
        temp_v0 = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                dec_bg_show_set(0, temp_v0);
                set_dec_bri(0x80);
                game_state.substep = 2;
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, dec_bg_decode_to_vram(0));
            set_dec_bri(0x80);
            game_state.substep = 2;
        }
        break;
    case 2:
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.substep += 1;
        }
        break;
    case 3:
        audio_command(0x509);
        sndi("Ｔｒｒｒｒ", 0, 0x2F);
        mode_step_next();
        break;
    }
}

s32 phone_answering_message_scene_init(void) {
    s32 temp_v0;

    /* FAKE: scalar selector view preserves branch and decoder-result registers. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        text_system_reset();
        message_window_set_title((s32) "自宅");
        parameter_show_init();
        parameter_show_init();
        message_window_init();
        hizuke_init();
        actor_clear_upper();
        hud_icon_bar_init();
        icon_disp_switch(1);
        schedule_command_icons_lock_to_record();
        icon_can_use_set(0, 0);
        icon_can_use_set(game_state.last_command.w & 0xF, 1);
        parameter_disp_switch(1);
        k_sub_reset_point_set();
        k_sub_disp_start(1);
        temp_v0 = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);
        if (temp_v0 != -1) {
            if ((temp_v0 == 0) || (temp_v0 == 1)) {
                dec_bg_show_set(0, temp_v0);
                set_dec_bri(0x80);
                game_state.substep = 2;
            }
        } else {
            game_state.substep += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, dec_bg_decode_to_vram(0));
            set_dec_bri(0x80);
            game_state.substep = 2;
        }
        break;
    case 2:
        gnsx(&game_state.player_family_name[0]);
        sndisp("おや？ 留守番電話に、メッセージが 入っているみたいだ。 聞いてみるか。）", 0, 0x1F);
        k_sub_disp_start(1);
        mode_step_next();
        break;
    }
}

s32 phone_yoshio_relationship_report_run(void) {
    /* FAKE: unused locals place buffers at sp+0x54/+0x34 in the 0x158 frame. T-10860. */
    s32 frame_pad0;
    u8 text[256];
    u8 name[32];
    s32 frame_pad[2];

    /* FAKE: scalar selector view retains v1 across both tables. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0x0:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.mode_step_timer.u = 0;
            game_state.substep += 1;
        }
        break;
    case 0x1:
        k_sub_reset();
        audio_command(0x509);
        sndi("Ｔｒｒｒｒ", 0, 0x2F);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 0x2:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.mode_step_timer.u = 0;
            game_state.substep += 1;
        }
        break;
    case 0x3:
        k_sub_reset();
        audio_command(0x509);
        sndi("Ｔｒｒｒｒ", 0, 0x2F);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 0x4:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.substep += 1;
        }
        break;
    case 0x5:
        k_sub_reset();
        audio_command(0x50A);
        gnsx(D_800B3628);
        sndisp(D_801450C8, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 0, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x6:
        talk_text_step();
        break;
    case 0x7:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_801450CC, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x8:
        xa_wait();
        break;
    case 0x9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450D0, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 2, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xA:
        talk_text_step();
        break;
    case 0xB:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_801450D4, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xC:
        xa_wait();
        break;
    case 0xD:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450D8, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 4, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xE:
        talk_text_step();
        break;
    case 0xF:
        k_sub_reset();
        sndisp("　　　　・　　　 　　　・　　　 　　　・　　　", 0, 0x2F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x10:
        xa_wait();
        break;
    case 0x11:
        audio_bank_start(0, 0x201);
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        strcpy(text, "どうやら好雄の話では、俺が ");
        get_g_name(name, phone_high_syoushin_girl_find());
        strcat(text, name);
        strcat(text, "を、 傷つけたといううわさが、 流れてたそうだ。）");
        sndisp(text, 0, 0x1F);
        k_sub_disp_start(0);
        ((TextEntry *)get_k_work(3))->attribute = 1;
        game_state.substep += 1;
        break;
    case 0x12:
        xa_wait();
        break;
    case 0x13:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp("これからは、もっと 好雄に電話して、 自分の評価をチェック しなきゃなー。 その時、変なマークが、 付いていたら、要注意だな。）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x14:
        xa_wait();
        break;
    case 0x15:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450E0, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 6, 0xD);
        game_state.substep += 1;
        break;
    case 0x16:
        talk_text_step();
        break;
    case 0x17:
        game_state.substep = 0xF0;
        break;
    case 0xD0:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80144FB8, 0, 0xD);
        talk_voice_play((s32) D_80144FC8, (s32) D_80144FE8, 0, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xD1:
        talk_text_step();
        break;
    case 0xD2:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80144FBC, 0, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xD3:
        xa_wait();
        break;
    case 0xD4:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80144FC0, 0, 0xD);
        talk_voice_play((s32) D_80144FC8, (s32) D_80144FE8, 2, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xD5:
        talk_text_step();
        break;
    case 0xD6:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp(D_80144FC4, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xD7:
        xa_wait();
        break;
    case 0xD8:
        audio_command(0x50B);
        game_state.substep = 0xF0;
        break;
    case 0xF0:
        audio_command(0x50B);
        audio_bank_start(0, 0x200);
        game_state.substep += 1;
        break;
    default:
        mode_step_next();
        break;
    }
}


s32 phone_missed_appointment_result_dialogue_run(void) {
    /* FAKE: unused words reproduce the observed 0x158-byte frame. T-10860. */
    s32 frame_pad[2];
    u32 girl;
    u8 text[256];
    u8 name[32];

    /* FAKE: scalar selector view retains v1 through both jump tables. T-10860. */
    switch (*(u8 *)&game_state.substep) {
    case 0x0:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.mode_step_timer.u = 0;
            game_state.substep += 1;
        }
        break;
    case 0x1:
        k_sub_reset();
        audio_command(0x509);
        sndi("Ｔｒｒｒｒ", 0, 0x2F);
        k_sub_disp_start(1);
        game_state.substep += 1;
        break;
    case 0x2:
        k_disp_inc();
        if ((u32) game_state.mode_step_timer.u >= 0x4CU) {
            game_state.substep += 1;
        }
        break;
    case 0x3:
        k_sub_reset();
        audio_command(0x50A);
        gnsx(D_800B3628);
        sndisp(D_801450C8, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 0, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x4:
        talk_text_step();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_801450CC, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        if ((((AppointmentBits *)&game_state.unk_54C[0])->girl) == 0xA) {
            game_state.substep = 0x1E;
        }
        break;
    case 0x6:
        xa_wait();
        break;
    case 0x7:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450D0, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 2, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x8:
        talk_text_step();
        break;
    case 0x9:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_801450D4, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xA:
        xa_wait();
        break;
    case 0xB:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450D8, 0, 0xF);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 4, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xC:
        talk_text_step();
        break;
    case 0xD:
        k_sub_reset();
        girl = ((AppointmentBits *)&game_state.unk_54C[0])->girl;
        gnsx(D_800B3628);
        sndisp(D_801452C8[girl], 0, 0xD);
        talk_voice_play((s32) D_801452F0, (s32) D_80145340, (s32) girl, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xE:
        talk_text_step();
        break;
    case 0xF:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_801450DC, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x10:
        xa_wait();
        break;
    case 0x11:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_801450E0, 0, 0xD);
        talk_voice_play((s32) D_801450E4, (s32) D_8014511C, 6, 0xD);
        game_state.substep += 1;
        break;
    case 0x12:
        talk_text_step();
        break;
    case 0x13:
        game_state.substep = 0xF0;
        break;
    case 0x1E:
        xa_wait();
        break;
    case 0x1F:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80145454, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 2, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x20:
        talk_text_step();
        break;
    case 0x21:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80145458, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x22:
        xa_wait();
        break;
    case 0x23:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_8014545C, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 4, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x24:
        talk_text_step();
        break;
    case 0x25:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp(D_80145460, 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x26:
        xa_wait();
        break;
    case 0x27:
        k_sub_reset();
        gnsx(D_800B3628);
        sndisp(D_80145464, 0, 0xD);
        talk_voice_play((s32) D_80145468, (s32) D_801454A0, 6, 0xD);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0x28:
        talk_text_step();
        break;
    case 0x29:
        game_state.substep = 0xF0;
        break;
    case 0xF0:
        audio_command(0x50B);
        game_state.substep += 1;
        break;
    case 0xF1:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        strcpy(text, "そうかっ！ ");
        get_g_name(name, ((AppointmentBits *)&game_state.unk_54C[0])->girl);
        strcat(text, name);
        strcat(text, "は、 怒って来なかったのか。）");
        sndisp(text, 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xF2:
        xa_wait();
        break;
    case 0xF3:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        audio_bank_start(0, 0x202);
        sndisp("ウルトラショーック ！！！！！！！！）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xF4:
        xa_wait();
        break;
    case 0xF5:
        k_sub_reset();
        gnsx(game_state.player_family_name);
        sndisp("電話しなきゃ………。）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        break;
    case 0xF6:
        xa_wait();
        break;
    default:
        mode_step_next();
        break;
    }
}


s32 phone_high_syoushin_girl_find(void) {
    s32 i;
    s32 found;

    for (i = 0; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met &&
            game_state.girls[i].syoushin >= 0x5B) {
            found = i;
            break;
        }
    }
    return found;
}

void phone_relationship_deltas_apply(void) {
    /* FAKE: unused homes preserve the 0x38 frame and selected at sp+0x2C. T-10860. */
    s32 unused[2];
    s32 selected;
    s32 i;

    for (i = 0; i < 0xB; i++) {
        if (game_state.girls[i].flags.f.met && (game_state.girls[i].syoushin >= 0x5B)) {
            selected = i;
            break;
        }
    }
    i = 0;
    game_state.girls[selected].syoushin = 0;
    ((TelRecordFlags *)&game_state.girls[selected].flags)->b16 = 0;
    ((TelRecordFlags *)&game_state.girls[selected].flags)->b12 = 0;
    ((TelRecordFlags *)&game_state.girls[selected].flags)->b13 = 0;
    do {
        if (game_state.girls[i].flags.f.met == 1) {
            game_state.girls[i].tokimeki -= phone_high_syoushin_tokimeki_penalty[i];
            game_state.girls[i].affection -= phone_high_syoushin_affection_penalty[i];
        }
        i += 1;
    } while (i != 0xB);
    mode_step_next();
}

void phone_return_to_day_screen(void) {
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

s32 phone_high_syoushin_call_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_high_syoushin_call_response();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_high_syoushin_call_response(void) {
    GameState *g;
    s32 status;
    s32 girl;

    g = &game_state;
    status = phone_b12_girl_find();
    g->current_girl = status;
    girl = (s32) status;
    switch (g->substep) {
    case 0x0:
        xa_wait();
        break;
    case 0x1:
        if (get_g_zyotai_s(girl) & 0x80) {
            g->substep = 0xA;
            break;
        }
        if (get_g_zyotai_s(girl) == 3) {
            g->substep = 0x32;
            break;
        }
        g->substep = 0x1E;
        break;
    case 0xA:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80146008[girl], 0, girl);
        talk_voice_play((s32) D_80146038, (s32) D_80146090, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0xB:
        talk_text_step();
        break;
    case 0xC:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("喜んでもらえたかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0xD:
        xa_wait();
        break;
    case 0xE:
        g->substep = 0xF0;
        break;
    case 0x1E:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_801462F0[girl], 0, girl);
        talk_voice_play((s32) D_80146320, (s32) D_80146378, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x1F:
        talk_text_step();
        break;
    case 0x20:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("それなりに、 喜んでもらえたのかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x21:
        xa_wait();
        break;
    case 0x22:
        g->substep = 0xF0;
        break;
    case 0x32:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_801465B0[girl], 0, girl);
        talk_voice_play((s32) D_801465E0, (s32) D_80146638, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x33:
        talk_text_step();
        break;
    case 0x34:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("まあまあかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x35:
        xa_wait();
        break;
    case 0x36:
        g->substep = 0xF0;
        break;
    case 0xF0:
        g->substep += 1;
        break;
    default:
        g->girls[girl].flags.f.b12 = 0;
        mode_step_next();
        break;
    }
}

s32 phone_b12_girl_find(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b12 &&
            get_g_zyotai_s(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 phone_lost_gift_response_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_b13_girl_response();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_b13_girl_response(void) {
    GameState *g;
    s32 status;
    s32 girl;

    g = &game_state;
    status = phone_b13_girl_find();
    g->current_girl = status;
    girl = (s32) status;
    switch (g->substep) {
    case 0x0:
        xa_wait();
        break;
    case 0x1:
        if (get_g_zyotai_s(girl) & 0x80) {
            g->substep = 0xA;
            break;
        }
        if (get_g_zyotai_s(girl) == 3) {
            g->substep = 0x32;
            break;
        }
        g->substep = 0x1E;
        break;
    case 0xA:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80146904[girl], 0, girl);
        talk_voice_play((s32) D_80146934, (s32) D_8014698C, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0xB:
        talk_text_step();
        break;
    case 0xC:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("喜んでもらえたかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0xD:
        xa_wait();
        break;
    case 0xE:
        g->substep = 0xF0;
        break;
    case 0x1E:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80146BF4[girl], 0, girl);
        talk_voice_play((s32) D_80146C24, (s32) D_80146C7C, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x1F:
        talk_text_step();
        break;
    case 0x20:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("それなりに、 喜んでもらえたのかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x21:
        xa_wait();
        break;
    case 0x22:
        g->substep = 0xF0;
        break;
    case 0x32:
        k_sub_reset();
        gnsx(girl_short_name_table[girl]);
        sndisp(D_80146EE4[girl], 0, girl);
        talk_voice_play((s32) D_80146F14, (s32) D_80146F6C, girl, girl);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x33:
        talk_text_step();
        break;
    case 0x34:
        k_sub_reset();
        gnsx(&g->player_family_name[0]);
        sndisp("まあまあかな。）", 0, 0x1F);
        k_sub_disp_start(0);
        g->substep += 1;
        break;
    case 0x35:
        xa_wait();
        break;
    case 0x36:
        g->substep = 0xF0;
        break;
    case 0xF0:
        g->substep += 1;
        break;
    default:
        g->girls[girl].flags.f.b13 = 0;
        mode_step_next();
        break;
    }
}

s32 phone_b13_girl_find(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b13 &&
            get_g_zyotai_s(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 phone_club_absence_exit_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_club_leave_state_apply();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_club_leave_state_apply(void) {
    u32 temp_t8;

    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        k_sub_reset();
        gnsx("部長");
        temp_t8 = (u16) game_state.player_flags.h >> 0xC;
        ((ClubFlagBits *)&game_state.clubs[temp_t8])->b24 = 1;
        game_state.player_flags.b[1] = (game_state.player_flags.b[1] & 0xFF0F) | 0xB0;
        sndisp("君は、今日クラブを 休んでしまったようなので、 退部してもらう。", 0, 0xD);
        talk_voice_play(0x0D000020, 0x0400002F, -1, 0xE);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 2:
        talk_text_step();
        return;
    case 3:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp("クラブを、 やめさせられてしまった。", 0, 0xF);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 4:
        xa_wait();
        return;
    default:
        mode_step_next();
        return;
    }
}

s32 phone_culture_festival_club_notice_dispatch(void) {
    switch (game_state.mode_step) {
    case 0:
        phone_answering_message_scene_init();
        break;
    case 1:
        phone_culture_festival_club_notice_dialogue();
        break;
    case 2:
        phone_return_to_day_screen();
        break;
    }
}

s32 phone_culture_festival_club_notice_dialogue(void) {
    switch (game_state.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        k_sub_reset();
        gnsx("部長");
        sndisp("明日から文化祭の準備をする。 クラブに参加するように！", 0, 0xD);
        talk_voice_play(0x0D000001, 0x02000010, -1, 0xE);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 2:
        talk_text_step();
        return;
    case 3:
        k_sub_reset();
        gnsx(&game_state.player_family_name[0]);
        sndisp("文化祭の出展の 準備をしなきゃ。）", 0, 0x1F);
        k_sub_disp_start(0);
        game_state.substep += 1;
        return;
    case 4:
        xa_wait();
        return;
    default:
        game_mode_set(GAME_MODE_COMMAND_SCREEN);
        return;
    }
}
