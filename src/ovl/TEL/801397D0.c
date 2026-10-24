#define MAIN_API_OVERRIDE_k_sub_reset /* T-10860: legacy integer reset calls in three matched handlers. */
#include "common.h"
#include "ovl/TEL.h"
#include "game.h"

/* The legacy call view returns int; canonical k_sub_reset retains void. */
s32 k_sub_reset();

void func_801397D0(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_stage) {
    case 0:
        func_801399CC();
        break;
    case 1:
        func_8013A228();
        break;
    case 2:
        func_8013A40C();
        break;
    case 3:
        func_8013A478();
        break;
    case 4:
        func_8013A79C();
        break;
    case 5:
        func_8013A820();
        break;
    case 6:
        func_8013BF70();
        break;
    case 7:
        func_8013C46C();
        break;
    case 8:
        func_8013C968();
        break;
    case 9:
        func_8013CB34();
        break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    hud_icon_bar_update(0);
    cursor_update();
    message_window_title_update();
    dec_bg_draw(0);
    hud_screen_frame_draw(0);
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
    }
}

void func_80139920(void) {
    s32 i;
    u32 sel;

    sel = ((AppointmentBits *)&game_state.unk_54C[0].w)->girl;
    for (i = 0; i != 8; i++) {
        if (((AppointmentBits *)&game_state.unk_54C[i])->active && sel == ((AppointmentBits *)&game_state.unk_54C[i])->girl) {
            game_state.unk_54C[i].b[3] &= 0xFFDF;
            printf("canceled %d \n", i);
        }
    }
    func_8006B0C8();
}

s32 func_801399CC(void) {
    switch (game_state.mode_step) {
    case 0:
        if (((u32) (game_state.unk_54C[0].w * 8) >> 0x1C) == 0xA) {
            mode_step_set(0xA);
            return;
        }
        func_8013AC0C();
        return;
    case 1:
        func_80139FEC();
        return;
    case 2:
        func_8013A2F0();
        return;
    case 3:
        func_8013BF50();
        return;
    case 10:
        func_8013AA30();
        return;
    case 11:
        func_80139AA4();
        return;
    default:
        mode_step_next();
        return;
    }
}

void func_80139AA4(void) {
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
s32 func_80139E3C(void) {
    GirlRecord *girl;
    s16 delta;
    u32 index;

    index = (u32)(game_state.unk_54C[0].w * 8) >> 0x1C;
    girl = &game_state.girls[index];
    delta = D_801444A0[index];
    girl->affection -= D_801444A0[index];
    girl->syoushin += delta;
}

s32 func_80139E90(void) {
    /* FAKE: 4 spare bytes above the buffers reproduce the original frame (0x150); real locals unknown. T-9180 */
    s32 pad;
    u8 name[0x100];
    u8 num[0x20];

    switch (game_state.substep) {
    case 0:
        func_80139E3C();
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
        func_80139920();
        mode_step_next();
        break;
    }
}

void func_80139FEC(void) {
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

s32 func_8013A228(void) {
    switch (game_state.mode_step) {
    case 0:
        if (((u32) (game_state.unk_54C[0].w * 8) >> 0x1C) == 0xA) {
            mode_step_set(0xA);
            return;
        }
        func_8013AC0C();
        return;
    case 1:
        func_80139FEC();
        return;
    case 2:
        func_80139E90();
        return;
    case 3:
        func_8013BF50();
        return;
    case 10:
        func_8013AA30();
        return;
    case 11:
        func_80139AA4();
        /* fallthrough */
    default:
        return;
    }
}

s32 func_8013A2F0(void) {
    switch (game_state.substep) {
    case 0:
        func_80139E3C();
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
        func_80139920();
        mode_step_next();
        break;
    }
}

s32 func_8013A40C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AA30();
        break;
    case 1:
        func_8013B54C();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013A478(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013A4FC();
        break;
    case 2:
        func_8013A76C();
        break;
    case 3:
        func_8013BF50();
        break;
    }
}

s32 func_8013A4FC(void) {
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

void func_8013A76C(void) {
    game_state.girls[11].flags.b[3] = game_state.girls[11].flags.b[3] + 1;
    mode_step_next();
}

s32 func_8013A79C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AA30();
        break;
    case 1:
        func_8013ADC0();
        break;
    case 2:
        func_8013BE38();
        break;
    case 3:
        func_8013BF50();
        break;
    }
}

s32 func_8013A820(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013A88C();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013A88C(void) {
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

s32 func_8013AA30(void) {
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
        func_8006509C();
        icon_disp_switch(1);
        schedule_command_icons_lock_to_record();
        icon_can_use_set(0, 0);
        icon_can_use_set(game_state.last_command.w & 0xF, 1);
        parameter_disp_switch(1);
        k_sub_reset_point_set();
        k_sub_disp_start(1);
        actor_clear_upper();
        temp_v0 = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
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

s32 func_8013AC0C(void) {
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
        func_8006509C();
        icon_disp_switch(1);
        schedule_command_icons_lock_to_record();
        icon_can_use_set(0, 0);
        icon_can_use_set(game_state.last_command.w & 0xF, 1);
        parameter_disp_switch(1);
        k_sub_reset_point_set();
        k_sub_disp_start(1);
        temp_v0 = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
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

s32 func_8013ADC0(void) {
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
        get_g_name(name, func_8013BDDC());
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


s32 func_8013B54C(void) {
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


s32 func_8013BDDC(void) {
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

void func_8013BE38(void) {
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
            game_state.girls[i].tokimeki -= D_801444B8[i];
            game_state.girls[i].affection -= D_801444C4[i];
        }
        i += 1;
    } while (i != 0xB);
    mode_step_next();
}

void func_8013BF50(void) {
    game_mode_set(GAME_MODE_DAY_SCREEN);
}

s32 func_8013BF70(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013BFDC();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013BFDC(void) {
    GameState *g;
    s32 status;
    s32 girl;

    g = &game_state;
    status = func_8013C3E0();
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

s32 func_8013C3E0(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b12 &&
            get_g_zyotai_s(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 func_8013C46C(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013C4D8();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013C4D8(void) {
    GameState *g;
    s32 status;
    s32 girl;

    g = &game_state;
    status = func_8013C8DC();
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

s32 func_8013C8DC(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (game_state.girls[i].flags.f.met && game_state.girls[i].flags.f.b13 &&
            get_g_zyotai_s(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 func_8013C968(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013C9D4();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013C9D4(void) {
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

s32 func_8013CB34(void) {
    switch (game_state.mode_step) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013CBA0();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013CBA0(void) {
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
