/* computer_fortune.c: src/ovl/BUNKAKEN/80152680.c:61: stage 11: fortune telling by blood type and its grid. Original object 0x80152680-0x80153050 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* Caller passes the full word without a prototype (T-9960). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void talk_names_set();

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D20);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D28);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D30);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D38);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D40);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D48);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D50);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D54);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D5C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D64);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D68);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D70);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D78);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D7C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D84);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D8C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D94);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155D9C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DA4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DAC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DB4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DBC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DC4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DCC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DD4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DDC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/computer_fortune.rodata", D_80155DE4);

void tour_stage_fortune_telling(void) {
    GameState *g = &game_state;
    s32 i;
    TourFortuneNames names;
    TourFortuneBloodLabels blood_types;
    TourFortunePlayerLabels player_blood_types;

    names = tour_fortune_girl_names;
    blood_types = tour_fortune_girl_blood_labels;
    player_blood_types = tour_fortune_player_blood_labels;
    switch (g->mode_step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B80);
        for (i = 0; i < 11; i++) {
            select_girl_menu_enabled[i] = g->girls[i].flags.f.met;
        }
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            tour_script_fortune();
            talk_line = 0;
            talk_text_table = (s32) tour_script_text;
            talk_command_table = (s32) tour_script_commands;
            talk_voice_table = (s32) tour_script_voices;
            if ((game_mode_scene_girl == 0xFF) || ((((u16) g->player_flags.h >> 0xC) != 3) && ((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) || ((((u16) g->player_flags.h >> 0xC) != 3) && ((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) && (((u8) g->girls[2].flags.b[2] >> 4) == 3))) {
                talk_group = 0;
                mode_step_next();
            } else {
                talk_group = 1;
                mode_step_set(5);
            }
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        select_girl2();
        break;
    case 4:
        game_mode_scene_girl = D_80122CDC;
        talk_names_set(D_80122CDC);
        mode_step_next();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) "＊パソコン　ＮＭＳ＿９０００＊", 0);
        k_disp_start(2);
        mode_step_next();
        break;
    case 7:
        audio_command(0x50A);
        mode_step_next();
        break;
    case 8:
        if (game_mode_scene_girl == 0xB) {
            set_kanji_string(-0x80, 0x42, 0, (s32) *girl_short_name_table, 0);
        } else {
            set_kanji_string(-0x80, 0x42, 0, (s32) girl_short_name_table[game_mode_scene_girl], 0);
        }
        set_kanji_string(-0x48, 0x42, 0, (s32)names.names[game_mode_scene_girl], 0);
        k_disp_start(4);
        tour_fortune_wait_counter = 0;
        mode_step_next();
        break;
    case 9:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x15U) {
            tour_fortune_wait_counter = 0;
            mode_step_next();
        }
        break;
    case 10:
        set_kanji_string(-0x10, 0x42, 0, (s32) "血液型", 0);
        k_disp_start(5);
        mode_step_next();
        break;
    case 11:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x15U) {
            tour_fortune_wait_counter = 0;
            mode_step_next();
        }
        break;
    case 12:
        if ((game_mode_scene_girl == 0) || (game_mode_scene_girl == 0xB)) {
            set_kanji_string(0x28, 0x42, 0, (s32)player_blood_types.blood_labels[(u32) (g->girls[0].birthday.w << 0x15) >> 0x1E], 0);
        } else {
            set_kanji_string(0x28, 0x42, 0, (s32)blood_types.blood_labels[game_mode_scene_girl], 0);
        }
        k_disp_start(6);
        mode_step_next();
        break;
    case 13:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x15U) {
            tour_fortune_wait_counter = 0;
            mode_step_next();
        }
        break;
    case 14:
        audio_command(0x50A);
        mode_step_next();
        break;
    case 15:
        set_kanji_string(-0x80, 0x52, 0, (s32) g->player_family_name, 0);
        set_kanji_string(-0x48, 0x52, 0, (s32) g->player_given_name, 0);
        k_disp_start(8);
        tour_fortune_wait_counter = 0;
        mode_step_next();
        break;
    case 16:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x15U) {
            tour_fortune_wait_counter = 0;
            mode_step_next();
        }
        break;
    case 17:
        set_kanji_string(-0x10, 0x52, 0, (s32) "血液型", 0);
        k_disp_start(9);
        mode_step_next();
        break;
    case 18:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x15U) {
            tour_fortune_wait_counter = 0;
            mode_step_next();
        }
        break;
    case 19:
        set_kanji_string(0x28, 0x52, 0, (s32)player_blood_types.blood_labels[(u32) (g->player_birthday << 0x15) >> 0x1E], 0);
        k_disp_start(0xA);
        mode_step_next();
        break;
    case 20:
        tour_fortune_wait_counter++;
        if ((tour_fortune_wait_counter >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            tour_fortune_wait_counter = 0;
        }
        if ((g->pad_pressed & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            D_8011ED5B &= 0x7F;
            mode_step_next();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        audio_command(0x50C);
        mode_step_next();
        break;
    case 23:
        tour_fortune_wait_counter++;
        if (tour_fortune_wait_counter >= 0x3DU) {
            talk_group = tour_fortune_result() + 3;
            D_80122EA8 = tour_fortune_result();
            mode_step_next();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if (((girl_state_rank(0) == 4) || (D_80122EA4 == 0)) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) {
            game_mode_scene_girl = 0;
        } else if (((girl_state_rank(2) == 4) || (D_80122EA4 == 2)) && (((u8) g->girls[2].flags.b[2] >> 4) == 3)) {
            game_mode_scene_girl = 2;
        }
        mode_stage_set((s32) *(u8 *)&D_80122EB0);
        mode_step_set((s32) *(u8 *)&D_80122EB4);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* FAKE: unused word fixes the table offset; expression layout retains address/value allocation. T-9550 */
s32 tour_fortune_result(void)
{
  s32 result;
  u32 pad;
  TourFortuneGrid choices;
  choices = tour_fortune_grid;
  if ((((game_mode_scene_girl == 2) && ((game_state.girls[2].flags.b[2] >> 4) == 3)) && (girl_state_rank(2) < 4)) && (D_80122EA4 != 2))
  {
    return 3;
  }
  result = choices.result_by_blood[(game_state.girls[game_mode_scene_girl].birthday.u << 21) >> 30][(((u32) game_state.player_birthday) << 21) >> 30];
  return result;
}
