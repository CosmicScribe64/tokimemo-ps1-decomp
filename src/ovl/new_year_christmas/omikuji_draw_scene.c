/* omikuji_draw_scene.c: ; src/ovl/SHOUGATU/80134890.c:176-196 begins the omikuji prompt, actor movement and result presentation. Original object 0x80134890-0x80135B90 (config/objects/SHOUGATU.txt). */
#include "common.h"
#include "ovl/new_year_christmas.h"

void new_year_omikuji_resource_addresses_init(void) {
    D_80143DB0 = 0x801DABE0;
    D_80143DB4 = 0x801DB684;
    D_80143DB8 = 0x801DABE8;
    D_80143DBC = 0x801DB6A0;
    D_80143DC0 = 0x801DAC48;
    D_80143DC4 = 0x801DB6CC;
    D_80143DC8 = *(s16 *)0x801DB6E0;
    D_80143DCC = *(s16 *)0x801DB6E4;
    D_80143DD0 = 0x801C0000;
    D_80143DD4 = 0x801C2000;
}

void new_year_omikuji_mode_step_dispatch(void) {
    u16 mode_step_index; /* FAKE: u16 and the second call argument put mode_step_index in $a1 as the original does (permuter); real prototype unknown. T-8080 */
    FnTbl31 omikuji_scene_callbacks;

    omikuji_scene_callbacks = D_80143DE0;
    if (new_year_fallback_dialogue_flag != 0) {
        mode_step_index = game_state.mode_step;
        if ((void (*)())normal_date_girl_in == omikuji_scene_callbacks.f[mode_step_index]) {
            omikuji_scene_callbacks.f[mode_step_index] = mode_step_next;
        }
    }
    mode_step_index = game_state.mode_step;
    omikuji_scene_callbacks.f[mode_step_index](0x40, mode_step_index);
    if (D_80143DDC != 0) {
        _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0x64, 0xD, 0xE0FF, 0xFFFF);
        _sprite_set_box_shade_tarao(-0xA0, -0x14, 0x140, 0x3C, 0xD, 0x6060, 0xE0FF);
        dtd_on(0xD);
    }
}

/* K&R return shape: failure returns zero; scene advance falls through. T-10030. */
s32 new_year_omikuji_asset_guard(void) {
    new_year_omikuji_resource_addresses_init();
    if (((s32 *)D_80143DB4)[0] != 0x801CC000 ||
        ((s32 *)D_80143DB4)[1] != 0x801CE000 ||
        ((s32 *)D_80143DB4)[2] != 0x801D0000 ||
        ((s32 *)D_80143DB4)[3] != 0x801D2000 ||
        ((s32 *)D_80143DB4)[4] != 0x801D4000 ||
        ((s32 *)D_80143DB4)[5] != 0x801D6000 ||
        ((s32 *)D_80143DB4)[6] != 0x801D8000 ||
        ((s32 *)D_80143DB0)[0] != 0x801C4000 ||
        ((s32 *)D_80143DB0)[1] != 0x801C8000 ||
        ((s32 *)D_80143DC4)[0] != 0x801DB634 ||
        ((s32 *)D_80143DC4)[1] != 0x801DB644 ||
        ((s32 *)D_80143DC4)[2] != 0x801DB654 ||
        ((s32 *)D_80143DC0)[0] != 0x801DAB84 ||
        ((s32 *)D_80143DC0)[1] != 0x801DAB8C ||
        ((s32 *)D_80143DC0)[2] != 0x801DABA0 ||
        ((s32 *)D_80143DC0)[3] != 0x801DABB4 ||
        ((s32 *)D_80143DC0)[4] != 0x801DABC0 ||
        ((s32 *)D_80143DC0)[5] != 0x801DABCC) {
        game_state.mode_step -= 2;
        return 0;
    }
    if (cd_file_checksum_ok(0x37, 0x801C0000) == 0) {
        game_state.mode_step -= 2;
        return 0;
    }
    mode_step_next();
}

void new_year_omikuji_background_and_log_setup(void) {
    if ((new_year_fallback_dialogue_flag == 0) && (game_state.event_log[game_state.event_log_count - 1].scene != 5)) {
        if (new_year_shiori_route_flag != 0) {
            event_log_add(5, 5);
        } else {
            event_log_add(5, 2);
        }
    }
    bg_read_sub2(0x4200);
    mode_step_next();
}

void new_year_omikuji_actors_init(void) {
    s16 i;

    D_80143DDC = 0;
    new_year_omikuji_resource_addresses_init();
    load_palette(D_80143DD0, 0x11, 1, 2, 0);
    for (i = 0; i < 3; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[i + 0x60].unk_01 = 0xA;
        ((Actor *)actor_pool)[i + 0x60].flags = 0;
        ((Actor *)actor_pool)[i + 0x60].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[i + 0x60].draw_flags = 4;
        ((Actor *)actor_pool)[i + 0x60].frame_table = (u8 *)D_80143DB8;
        ((Actor *)actor_pool)[i + 0x60].sequence_table = (u8 *)D_80143DC0;
        ((Actor *)actor_pool)[i + 0x60].tpage_table = (u8 *)D_80143DB0;
        ((Actor *)actor_pool)[i + 0x60].unk_14 = D_80143DC8;
        ((Actor *)actor_pool)[i + 0x60].sequence_slot = 0;
        ((Actor *)actor_pool)[i + 0x60].unk_18 = 0;
        ((Actor *)actor_pool)[i + 0x60].blink_period = 1;
        ((Actor *)actor_pool)[i + 0x60].unk_43 = 0x11;
        ((Actor *)actor_pool)[i + 0x60].x = -0xA0;
        ((Actor *)actor_pool)[i + 0x60].y = -0x78;
        ((Actor *)actor_pool)[i + 0x60].brightness = 0x80;
        ((Actor *)actor_pool)[i + 0x60].unk_05 = 0;
        ((Actor *)actor_pool)[i + 0x60].unk_04 = 0;
    }
    ((Actor *)actor_pool)[0x60].x = 0;
    ((Actor *)actor_pool)[0x61].sequence_slot = 3;
    ((Actor *)actor_pool)[0x62].sequence_slot = 4;
    load_palette(D_80143DD4, 0x12, 0, 0, 2);
    actor_clear(0x63);
    ((Actor *)actor_pool)[0x63].unk_01 = 0xD;
    ((Actor *)actor_pool)[0x63].flags = 0;
    ((Actor *)actor_pool)[0x63].quad_flags = 0;
    ((Actor *)actor_pool)[0x63].draw_flags = 4;
    ((Actor *)actor_pool)[0x63].frame_table = (u8 *)D_80143DBC;
    ((Actor *)actor_pool)[0x63].sequence_table = (u8 *)D_80143DC4;
    ((Actor *)actor_pool)[0x63].tpage_table = (u8 *)D_80143DB4;
    ((Actor *)actor_pool)[0x63].unk_14 = D_80143DCC;
    ((Actor *)actor_pool)[0x63].sequence_slot = 0;
    ((Actor *)actor_pool)[0x63].unk_18 = 0;
    ((Actor *)actor_pool)[0x63].blink_period = 1;
    ((Actor *)actor_pool)[0x63].unk_43 = 0;
    ((Actor *)actor_pool)[0x63].x = -0xA0;
    ((Actor *)actor_pool)[0x63].y = -0x78;
    ((Actor *)actor_pool)[0x63].brightness = 0x80;
    ((Actor *)actor_pool)[0x63].unk_05 = 0x81;
    ((Actor *)actor_pool)[0x63].unk_04 = 0;
    mode_step_next();
}

void new_year_omikuji_audio_command_0201(void) {
    audio_command(0x201);
    mode_step_next();
}

void new_year_omikuji_audio_command_0200(void) {
    audio_command(0x200);
    mode_step_next();
}

void new_year_omikuji_yuina_extra_step(void) {
    if ((game_state.current_girl == GIRL_YUINA) && (new_year_fallback_dialogue_flag == 0)) {
        mode_step_next();
    }
    mode_step_next();
}

void new_year_omikuji_dialogue_group_advance(void) {
    if ((new_year_omikuji_selector == 3) || (new_year_omikuji_selector == 4)) {
        new_year_dialogue_group += 2;
    } else if (new_year_omikuji_selector != 0) {
        new_year_dialogue_group += 1;
    }
    mode_step_next();
}

void new_year_omikuji_dialogue_group_advance_alt(void) {
    if (new_year_omikuji_selector == 0) {
        new_year_dialogue_group += 2;
    } else if ((new_year_omikuji_selector != 3) && (new_year_omikuji_selector != 4)) {
        new_year_dialogue_group += 1;
    }
    mode_step_next();
}

void new_year_omikuji_actors_show(void) {
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].flags = 1;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[2].flags = 1;
    ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    D_80143DDC = 1;
    mode_step_next();
}

s32 new_year_omikuji_scene(void) {
    /* FAKE: T-25132: the timer byte is read through a local pointer, so uopt keeps one register for it instead of
     * a promoted global plus a widened copy (as in DATE func_8013AF1C, T-21520). */
    GameState *g = &game_state;
    u8 sp84[] = "おみくじを引きます。";
    u8 sp6C[] = "うおりゃあああ〜・・・";
    u8 sp5C[] = "こ、これは！！";
    ShoStr7x5 sp38;

    sp38 = D_80143E5C;
    switch (game_state.mode_step_timer.w) {
    case 0:
        set_kanji_string(-0x68, 0x30, 0, sp84, 0);
        talk_line_wait = 1;
        ((Actor *)actor_pool)[96].dx = -4;
        game_state.mode_step_timer.w++;
        return 0;
    case 1:
        break;
    case 2:
        set_kanji_string(-0x68, 0x30, 0, sp6C, 0);
        talk_line_wait = 1;
        D_80143DDC = 0;
        ((Actor *)actor_pool)[99].draw_flags |= 0x80;
        ((Actor *)actor_pool)[99].flags = 1;
        ((Actor *)actor_pool)[99].sequence_slot = 1;
        ((Actor *)actor_pool)[99].unk_18 = 0;
        game_state.mode_step_timer.w++;
        return 0;
    case 3:
        ((ActorFixed *)actor_pool)[96].x.w += ((ActorFixed *)actor_pool)[96].dx;
        if (((Actor *)actor_pool)[96].x < -0x6F) {
            ((Actor *)actor_pool)[99].sequence_slot = 0;
            ((Actor *)actor_pool)[96].dx = -((Actor *)actor_pool)[96].dx;
            ((Actor *)actor_pool)[96].dx = ((Actor *)actor_pool)[96].dx + 2;
            if (((Actor *)actor_pool)[96].dx > 0x14) {
                ((Actor *)actor_pool)[96].dx = 0x14;
            }
        } else if (((Actor *)actor_pool)[96].x >= 0x70) {
            ((Actor *)actor_pool)[99].sequence_slot = 1;
            ((Actor *)actor_pool)[96].dx = -((Actor *)actor_pool)[96].dx;
            ((Actor *)actor_pool)[96].dx = ((Actor *)actor_pool)[96].dx - 2;
            if (((Actor *)actor_pool)[96].dx < -0x14) {
                ((Actor *)actor_pool)[96].dx = -0x14;
            }
        }
        if (((Actor *)actor_pool)[96].dx < -8 || ((Actor *)actor_pool)[96].dx > 8) {
            ((Actor *)actor_pool)[97].unk_08 += 1;
            ((Actor *)actor_pool)[98].unk_08 += 1;
        }
        if (((Actor *)actor_pool)[96].dx < -0xC || ((Actor *)actor_pool)[96].dx > 0xC) {
            ((Actor *)actor_pool)[97].unk_08 += 2;
            ((Actor *)actor_pool)[98].unk_08 += 2;
        }
        if (((Actor *)actor_pool)[96].dx < -0x10 || ((Actor *)actor_pool)[96].dx > 0x10) {
            ((Actor *)actor_pool)[97].unk_08 += 4;
            ((Actor *)actor_pool)[98].unk_08 += 4;
        }
        break;
    case 4:
        if ((((Actor *)actor_pool)[96].x >= 0 &&
             -((Actor *)actor_pool)[96].dx >= ((Actor *)actor_pool)[96].x) ||
            (((Actor *)actor_pool)[96].x <= 0 &&
             ((Actor *)actor_pool)[96].x >= -((Actor *)actor_pool)[96].dx)) {
            ((Actor *)actor_pool)[96].x = 0;
            D_80143DDC = 1;
            ((Actor *)actor_pool)[99].draw_flags &= 0x7F;
            game_state.mode_step_timer.w++;
        } else {
            ((ActorFixed *)actor_pool)[96].x.w += ((ActorFixed *)actor_pool)[96].dx;
            if (((Actor *)actor_pool)[96].x < -0x6F) {
                ((Actor *)actor_pool)[96].dx = 0xA;
            }
            if (((Actor *)actor_pool)[96].x >= 0x70) {
                ((Actor *)actor_pool)[96].dx = -0xA;
            }
        }
        return 0;
    case 5:
        if (g->substep == 0) {
            ((ActorFixed *)actor_pool)[96].dx = 0;
            ((ActorFixed *)actor_pool)[96].dy = 0xFFFC0000;
            ((Actor *)actor_pool)[96].draw_flags |= 0x10;
        }
        ((Actor *)actor_pool)[97].flags = 0;
        ((Actor *)actor_pool)[98].flags = 0;
        if (((Actor *)actor_pool)[96].y < -0xB4) {
            ((Actor *)actor_pool)[96].y = -0xB4;
        }
        if (g->substep++ == 0x1E) {
            ((ActorFixed *)actor_pool)[96].dy = 0;
            ((Actor *)actor_pool)[96].sequence_slot = (game_state.year == 0x61) + (game_state.year == 0x62) * 4 + 1;
            ((Actor *)actor_pool)[96].flags = 5;
        }
        if (g->substep == 0x4B) {
            D_80143DDC = 0;
            ((Actor *)actor_pool)[96].draw_flags &= 0x7F;
            ((Actor *)actor_pool)[99].draw_flags |= 0x80;
            ((Actor *)actor_pool)[99].flags = 9;
            ((Actor *)actor_pool)[99].sequence_slot = 2;
            ((Actor *)actor_pool)[99].unk_18 = 0;
            audio_command(0x500);
        }
        if (g->substep < 0x87) {
            return 0;
        }
        new_year_omikuji_result_text();
        set_kanji_string(-0x68, 0x30, 0, sp5C, 0);
        game_state.mode_step_timer.w++;
        return 0;
    case 6:
        /* The original ORs 8 into the compare result (the test is always true); kept as found, T-24029. */
        if ((((Actor *)actor_pool)[96].flags != 1) | 8) {
            game_state.mode_step_timer.w++;
            talk_line_wait = 1;
        }
        return 0;
    case 7:
        break;
    case 8:
        /* FAKE: T-25132: an empty test of the selector makes ugen load it at the head of the case, so as1
         * schedules its `lui` before the first flag store, as the original does (1 swapped pair without it). */
        if (new_year_omikuji_selector != 0) {
        }
        ((Actor *)actor_pool)[96].draw_flags |= 0x80;
        ((Actor *)actor_pool)[96].flags = 0x40;
        ((Actor *)actor_pool)[96].unk_18 = new_year_omikuji_selector + 0xD;
        ((Actor *)actor_pool)[96].x = -0xA0;
        ((Actor *)actor_pool)[96].y = -0x78;
        ((ActorFixed *)actor_pool)[96].dy = 0;
        /* FAKE: T-25132: the (s32) index casts read the widened selector, the one register of the tests (a plain
         * u8 index gives uopt a second register for it). */
        if (new_year_omikuji_selector == 3 || new_year_omikuji_selector == 4) {
            sprintf(sp6C, "%sだ・・・", sp38.s[(s32)new_year_omikuji_selector]);
            ((Actor *)actor_pool)[99].draw_flags |= 0x80;
            ((Actor *)actor_pool)[99].flags = 0x40;
            ((Actor *)actor_pool)[99].unk_18 = 7;
        } else if (new_year_omikuji_selector != 0) {
            sprintf(sp6C, "おっ、%sだ。", sp38.s[(s32)new_year_omikuji_selector]);
            ((Actor *)actor_pool)[99].unk_18 = 6;
            D_80143DDC = 1;
            ((Actor *)actor_pool)[99].draw_flags &= 0x7F;
        } else {
            sprintf(sp6C, "ラッキー！%sだ！！", sp38.s[(s32)new_year_omikuji_selector]);
            ((Actor *)actor_pool)[99].unk_18 = 6;
            D_80143DDC = 1;
            ((Actor *)actor_pool)[99].draw_flags &= 0x7F;
        }
        set_kanji_string(-0x68, 0x30, 0, sp6C, 0);
        talk_line_wait = 1;
        game_state.mode_step_timer.w++;
        return 0;
    case 9:
        break;
    default:
        mode_step_next();
        break;
    }
    if (talk_line_wait != 0) {
        if ((game_state.pad_pressed & 0x400040) && check_end_k() && game_state.voice_mode == VOICE_CANCELABLE) {
            k_reset(1);
            talk_line_wait = 0;
            talk_voice_state = 0;
            talk_voice_wait_frames = 0;
            D_8011ED5B &= 0xFF7F;
            game_state.mode_step_timer.w++;
            return 1;
        }
        if ((game_state.pad_pressed & 0x200020) && check_end_k()) {
            k_reset(1);
            talk_line_wait = 0;
            talk_voice_state = 0;
            talk_voice_wait_frames = 0;
            D_8011ED5B &= 0xFF7F;
            game_state.mode_step_timer.w++;
            return 1;
        }
        if (check_end_k()) {
            D_8011ED5B |= 0x80;
            hud_text_marker_bob();
        } else {
            D_8011ED5B &= 0x7F;
        }
    }
}

void new_year_omikuji_result_text(void) {
    s32 pad; /* FAKE: preserves labels at stack +0x28 (T-3330). T-24084 */
    ShoStr7x5 labels;

    labels = D_80143E80;
    new_year_omikuji_selector = game_state.frame_count & 7;
    if (new_year_omikuji_selector != 0) {
        new_year_omikuji_selector--;
    }
    if (game_state.current_girl == GIRL_AYAKO && new_year_omikuji_selector == 3) {
        new_year_omikuji_selector = 4;
    }
    strcpy(name_code_o, labels.s[new_year_omikuji_selector]);
    if (new_year_omikuji_selector == 0) {
        game_state.param_stress.value -= 5;
    }
    if (new_year_omikuji_selector == 4) {
        game_state.param_stress.value += 5;
    }
    check_para_limit();
}

void new_year_omikuji_actors_clear(void) {
    actor_clear(0x60);
    actor_clear(0x61);
    actor_clear(0x62);
    actor_clear(0x63);
    D_80143DDC = 0;
    mode_step_next();
}

void new_year_omikuji_data_sector_read(void) {
    cd_read_sectors(0x37, 0x801C0000, 0xBD5E);
    mode_step_next();
}
