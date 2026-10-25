/* graduation_script_resources.c: ; src/ovl/ENDING/80132000.c:6-35 initializes the graduation dialogue script address tables. Original object 0x80132000-0x80133030 (config/objects/ENDING.txt). */
#define MAIN_API_OVERRIDE_get_now_k /* implicit declaration, as matched (main_api.h: u8(void)) */
#include "common.h"
#include "ovl/ending.h"
#include "common.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/game_mode.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"

void ending_graduation_dialogue_resource_table_init(void) {
    ending_dialogue_resource_tables[0][0] = 0x801F00B8;
    ending_dialogue_resource_tables[0][1] = 0x801F05A8;
    ending_dialogue_resource_tables[0][2] = 0x801F0C70;
    ending_dialogue_resource_tables[0][3] = 0x801F0F34;
    ending_dialogue_resource_tables[0][4] = 0x801F14C4;
    ending_dialogue_resource_tables[0][5] = 0x801F1FF4;
    ending_dialogue_resource_tables[0][6] = 0x801F29EC;
    ending_dialogue_resource_tables[0][7] = 0x801F32F8;
    ending_dialogue_resource_tables[0][8] = 0x801F4100;
    ending_dialogue_resource_tables[0][9] = 0x801F4D18;
    ending_dialogue_resource_tables[0][10] = 0x801F5BD4;
    ending_dialogue_resource_tables[0][11] = 0x801F6698;
    ending_dialogue_resource_tables[0][12] = 0x801F6D9C;
    ending_dialogue_resource_tables[0][13] = 0x801F70B4;
    ending_dialogue_resource_tables[0][14] = 0x801F759C;
    ending_dialogue_resource_tables[0][15] = 0x801F7A70;
    ending_dialogue_resource_tables[0][16] = 0x801F7F98;
    ending_dialogue_resource_tables[1][0] = 0x801F00D0;
    ending_dialogue_resource_tables[1][1] = 0x801F05C4;
    ending_dialogue_resource_tables[1][2] = 0x801F0C8C;
    ending_dialogue_resource_tables[1][3] = 0x801F0F44;
    ending_dialogue_resource_tables[1][4] = 0x801F14E4;
    ending_dialogue_resource_tables[1][5] = 0x801F2028;
    ending_dialogue_resource_tables[1][6] = 0x801F2A0C;
    ending_dialogue_resource_tables[1][7] = 0x801F331C;
    ending_dialogue_resource_tables[1][8] = 0x801F4144;
    ending_dialogue_resource_tables[1][9] = 0x801F4D30;
    ending_dialogue_resource_tables[1][10] = 0x801F5C1C;
    ending_dialogue_resource_tables[1][11] = 0x801F66B4;
    ending_dialogue_resource_tables[1][12] = 0x801F6DB8;
    ending_dialogue_resource_tables[1][13] = 0x801F70BC;
    ending_dialogue_resource_tables[1][14] = 0x801F75B8;
    ending_dialogue_resource_tables[1][15] = 0x801F7AAC;
    ending_dialogue_resource_tables[1][16] = 0x801F7FB0;
    ending_dialogue_resource_tables[2][0] = 0x801F01A0;
    ending_dialogue_resource_tables[2][1] = 0x801F07F8;
    ending_dialogue_resource_tables[2][2] = 0x801F0EC0;
    ending_dialogue_resource_tables[2][3] = 0x801F0F84;
    ending_dialogue_resource_tables[2][4] = 0x801F17DC;
    ending_dialogue_resource_tables[2][5] = 0x801F24FC;
    ending_dialogue_resource_tables[2][6] = 0x801F2CCC;
    ending_dialogue_resource_tables[2][7] = 0x801F3684;
    ending_dialogue_resource_tables[2][8] = 0x801F476C;
    ending_dialogue_resource_tables[2][9] = 0x801F4F2C;
    ending_dialogue_resource_tables[2][10] = 0x801F62EC;
    ending_dialogue_resource_tables[2][11] = 0x801F68E8;
    ending_dialogue_resource_tables[2][12] = 0x801F7008;
    ending_dialogue_resource_tables[2][13] = 0x801F7114;
    ending_dialogue_resource_tables[2][14] = 0x801F7808;
    ending_dialogue_resource_tables[2][15] = 0x801F7CA0;
    ending_dialogue_resource_tables[2][16] = 0x801F8174;
}

void ending_mode_stage_dispatch(void) {
    switch (game_state.mode_stage) {
    case 0:
        ending_candidate_selection_step_dispatch();
        return;
    case 1:
        ending_companion_scene_step_dispatch();
        return;
    case 2:
        ending_secondary_scene_step_dispatch();
        return;
    case 3:
        ending_scene_effect_step_dispatch();
        return;
    case 4:
        ending_audience_scene_step_dispatch();
        return;
    case 5:
        ending_audio_scene_step_dispatch();
        return;
    case 6:
        ending_career_summary_step_dispatch();
        return;
    case 7:
        ending_song_scene_step_dispatch();
        return;
    default:
        ending_graduation_return_to_etc_mode();
        return;
    }
}

void ending_graduation_return_to_etc_mode(void) {
    game_mode_set(GAME_MODE_ETC_C2);
}

/* The scene dialogue entry: shows message text (kind 0 and 6: a title in brackets, 1 and 0xC: a spoken line
 * with its voice, 0xD and 0xF: the same after a wait of 0x3C frames). Old-style definition: kind, page, step
 * and color are read from their argument slots as u8/u16 at each use, and ending_scene_dialogue_step_update calls it without a
 * prototype. T-20930 */
s32 ending_dialogue_line_render(dialogue_text, dialogue_kind, voice_or_text_arg, voice_range_table, dialogue_page_index, dialogue_step_index, dialogue_color)
    u8 *dialogue_text;
    u8 dialogue_kind;
    s32 voice_or_text_arg;
    s32 ***voice_range_table;
    u16 dialogue_page_index;
    u16 dialogue_step_index;
    u8 dialogue_color;
{
    /* FAKE: x is the second element of a six-halfword array and pad is never used; together they give the
     * original frame (0x260), the dialogue_text buffer at sp+0x50 and x in memory at sp+0x256 (the original stores it
     * before set_kanji_string and reloads it after; a plain s16 is constant-folded). Real locals unknown. T-20930 */
    s16 x[6];
    u8 buf[0x204];
    u8 pad[8];

    if (dialogue_kind != 0xFF) {
        strcpy(buf, dialogue_text);
    }
    talk_voice_state = 0;
    talk_voice_waiting = 0;
    switch (dialogue_kind) {
    case 0x0:
        ending_audio_scene_tick_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_SEVENTEEN_FRAMES);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "『 ", 0);
        talk_render(x[1] + 0xE, 0x48, 0, buf, 0, "", "", 0);
        talk_line_auto = 1;
        break;
    case 0x6:
        ending_audio_scene_tick_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_SEVENTEEN_FRAMES);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "（ ", 0);
        talk_render(x[1] + 0xE, 0x48, 0, buf, 0, "）", "", 0);
        talk_line_auto = 1;
        break;
    case 0x1:
        ending_audio_scene_tick_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_NINE_FRAMES);
        if (voice_range_table[dialogue_page_index][dialogue_step_index][0] != 0) {
            if (talk_voice_wait_frames++ == 0 && talk_line_wait == 0) {
                cd_audio_request_set(voice_range_table[dialogue_page_index][dialogue_step_index][0], voice_range_table[dialogue_page_index][dialogue_step_index][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && talk_line_wait == 0) {
                talk_voice_waiting = 1;
                return 0;
            }
            talk_voice_state = 1;
            talk_voice_wait_frames = 0;
        }
        set_kanji_string(-0x90, 0x48, dialogue_color, "「 ", 0);
        talk_render(-0x82, 0x48, dialogue_color, buf, 0, "", "", 0);
        talk_line_wait = 1;
        break;
    case 0xC:
        ending_audio_scene_tick_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_NINE_FRAMES);
        if (voice_range_table[dialogue_page_index][dialogue_step_index][0] != 0) {
            if (talk_voice_wait_frames++ == 0 && talk_line_wait == 0) {
                cd_audio_request_set(voice_range_table[dialogue_page_index][dialogue_step_index][0], voice_range_table[dialogue_page_index][dialogue_step_index][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && talk_line_wait == 0) {
                talk_voice_waiting = 1;
                return 0;
            }
            talk_voice_state = 1;
            talk_voice_wait_frames = 0;
        }
        set_kanji_string(-0x90, 0x48, dialogue_color, "「 ", 0);
        talk_render(-0x82, 0x48, dialogue_color, buf, 0, "", "", 0);
        talk_line_clear = 1;
        break;
    case 0xD:
        ending_audio_scene_tick_timer = 0;
        if (ending_audio_scene_text_timer++ < 0x3C) {
            return 0;
        }
        ending_audio_scene_text_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_NINE_FRAMES);
        if (voice_range_table[dialogue_page_index][dialogue_step_index][0] != 0) {
            if (talk_voice_wait_frames++ == 0 && talk_line_wait == 0) {
                cd_audio_request_set(voice_range_table[dialogue_page_index][dialogue_step_index][0], voice_range_table[dialogue_page_index][dialogue_step_index][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && talk_line_wait == 0) {
                talk_voice_waiting = 1;
                return 0;
            }
            talk_voice_state = 1;
            talk_voice_wait_frames = 0;
        }
        talk_render(-0x82, talk_next_row_y, dialogue_color, buf, 0, "", "", 0);
        talk_line_clear = 1;
        break;
    case 0xF:
        ending_audio_scene_tick_timer = 0;
        if (ending_audio_scene_text_timer++ < 0x3C) {
            return 0;
        }
        ending_audio_scene_text_timer = 0;
        k_speed_set(TEXT_SPEED_EVERY_NINE_FRAMES);
        if (voice_range_table[dialogue_page_index][dialogue_step_index][0] != 0) {
            if (talk_voice_wait_frames++ == 0 && talk_line_wait == 0) {
                cd_audio_request_set(voice_range_table[dialogue_page_index][dialogue_step_index][0], voice_range_table[dialogue_page_index][dialogue_step_index][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && talk_line_wait == 0) {
                talk_voice_waiting = 1;
                return 0;
            }
            talk_voice_state = 1;
            talk_voice_wait_frames = 0;
        }
        talk_render(-0x82, talk_next_row_y, dialogue_color, buf, 0, "", "", 0);
        talk_line_wait = 1;
        break;
    case 0xFF:
        mode_step_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* Steps the scene dialogue: talk_tables/138 point at the page and step halfwords (ending_scene_dialogue_page_index, ending_scene_step_index),
 * talk_active_text_table/140/144 hold the text, kind and voice tables of the scene (ending_scene_dialogue_table_bind). Returns 1 when a
 * message has finished. T-20930 */
s32 ending_scene_dialogue_step_update(s32 voice_or_text_arg, s32 advance_dialogue, s32 dialogue_color) {
    if (talk_line_auto != 0) {
        if (check_end_k() != 0) {
            if (talk_voice_state == 0 || (talk_voice_state == 1 && cd_last_command() == 9)) {
                if ((u32)++game_state.mode_step_timer.w >= 0x3C) {
                    talk_line_auto = 0;
                    k_reset(1);
                    return 1;
                }
            }
        }
    } else if (talk_line_clear != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && game_state.pad_held == 0)) {
            if (talk_voice_state == 0 || (talk_voice_state == 1 && cd_last_command() == 9)) {
                talk_line_clear = 0;
                return 1;
            }
        }
    } else if (talk_line_wait != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && game_state.pad_held == 0)) {
            if (talk_voice_state == 0 || (talk_voice_state == 1 && cd_last_command() == 9)) {
                talk_line_wait = 0;
                k_reset(1);
                return 1;
            }
        }
    } else {
        game_state.mode_step_timer.w = 0;
        ending_dialogue_line_render(((u8 ***)talk_active_text_table)[*(s16 *)talk_tables][*(s16 *)talk_active_line_ptr], ((u8 **)talk_active_command_table)[*(s16 *)talk_tables][*(s16 *)talk_active_line_ptr], (u8)voice_or_text_arg, talk_active_voice_table, *(s16 *)talk_tables, *(s16 *)talk_active_line_ptr, (u8)dialogue_color);
        if (ending_audio_scene_text_timer == 0) {
            if ((u8)advance_dialogue != 0 && talk_voice_waiting == 0) {
                if (((u8 **)talk_active_command_table)[*(s16 *)talk_tables][*(s16 *)talk_active_line_ptr] == 0xFF) {
                    *(s16 *)talk_tables += 1;
                    *(s16 *)talk_active_line_ptr = 0;
                } else {
                    *(s16 *)talk_active_line_ptr += 1;
                }
            }
        }
    }
    return 0;
}

void ending_scene_dialogue_table_bind(void) {
    talk_tables = (u8 *)&ending_scene_dialogue_page_index;
    talk_active_line_ptr = (u8 *)&ending_scene_step_index;
    talk_active_text_table = ending_scene_dialogue_tables.text_table;
    talk_active_command_table = ending_scene_dialogue_tables.command_table;
    talk_active_voice_table = ending_scene_dialogue_tables.voice_table;
    ending_scene_dialogue_step_update(0xFF, 1, 1);
}

void ending_scene_audio_bank_load_begin(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void ending_scene_audio_bank_load_wait(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void ending_scene_audio_bank_play_start(void) {
    audio_command(0x200);
    mode_step_next();
}

/* Port of main check_k_scroll (src/main/scene_library.c), with ENDING's own limits. The unsigned
 * compare keeps the operand order of the bne (as in check_k_scroll). */
/* FAKE: sp40 keeps the unused result of the second get_k_work call; without it the frame is 8
 * bytes smaller and both spill homes move. Real source unknown. T-18300 */
void ending_scroll_dialogue_text_entries(void) {
    s32 sp3C;
    TextEntry *sp50;
    s32 sp40;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    TextEntry *temp_v0_2;
    TextEntry *temp_v0_3;

    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp50 = (TextEntry *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp3C = temp_a0;
    sp40 = get_k_work(temp_a0);
    if (sp50->y >= 0x68) {
        var_a0 = sp3C;
        if (talk_text_scroll_pending != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (TextEntry *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->y;
                    if ((u32) temp_v1 == 0x48U) {
                        temp_v0_2->y = (s16) (temp_v1 - 0x10);
                        temp_v0_2->active = 0;
                    } else {
                        temp_v0_2->y = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            talk_text_scroll_pending = 0;
            sp50->y = 0x58;
            sp50->active = 1;
            var_a0 = sp3C;
        }
    }
    temp_v0_3 = (TextEntry *) get_k_work(var_a0);
    if (temp_v0_3->y >= 0x68) {
        talk_text_scroll_pending = 1;
        temp_v0_3->active = 0;
    }
}
