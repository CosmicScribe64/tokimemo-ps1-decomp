/* fortune_telling.c: src/ovl/BUNKASAI/8015AF90.c:63: shared computer fortune stage and its result lookup. Original object 0x8015AF90-0x8015B900 (config/objects/BUNKASAI.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* caller passes the full word without a prototype (BUNKAKEN T-9960 form) */
#include "common.h"
#include "ovl/BUNKASAI.h"

void talk_names_set();

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160380);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160388);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160390);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160398);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603A0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603A8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603B0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603B4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603BC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603C4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603C8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603D0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603D8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603DC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603E4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603EC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603F4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_801603FC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160404);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_8016040C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160414);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_8016041C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160424);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_8016042C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160434);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_8016043C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/fortune_telling.rodata", D_80160444);

/* Clone of BUNKAKEN func_80152680 (T-18700). culture_script_set_fortune is called without a declaration, an
 * implicit int function as BUNKAKEN's func_80132548: with a void prototype IDO swaps v0 and v1. */
void culture_fortune_scene(void) {
    GameState *g = &game_state;
    s32 i;
    CultureFortuneGirlNames names;
    CultureFortuneBloodLabels blood_types;
    CultureBloodTypeLabels player_blood_types;

    names = culture_fortune_girl_names;
    blood_types = culture_fortune_girl_blood_labels;
    player_blood_types = culture_blood_type_labels;
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
            culture_script_set_fortune();
            talk_line = 0;
            talk_text_table = culture_script_text_table;
            talk_command_table = culture_script_command_table;
            talk_voice_table = culture_script_voice_table;
            if ((game_mode_scene_girl == 0xFF) || ((((u16) g->player_flags.h >> 0xC) != 3) && (girl_state_rank(0) == 4) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) || ((((u16) g->player_flags.h >> 0xC) != 3) && (girl_state_rank(2) == 4) && (((u8) g->girls[2].flags.b[2] >> 4) == 3))) {
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
        set_kanji_string(-0x48, 0x42, 0, (s32)names.text[game_mode_scene_girl], 0);
        k_disp_start(4);
        culture_fortune_timer = 0;
        mode_step_next();
        break;
    case 9:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x15U) {
            culture_fortune_timer = 0;
            mode_step_next();
        }
        break;
    case 10:
        set_kanji_string(-0x10, 0x42, 0, (s32) "血液型", 0);
        k_disp_start(5);
        mode_step_next();
        break;
    case 11:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x15U) {
            culture_fortune_timer = 0;
            mode_step_next();
        }
        break;
    case 12:
        if ((game_mode_scene_girl == 0) || (game_mode_scene_girl == 0xB)) {
            set_kanji_string(0x28, 0x42, 0, (s32)player_blood_types.text[(u32) (g->girls[0].birthday.w << 0x15) >> 0x1E], 0);
        } else {
            set_kanji_string(0x28, 0x42, 0, (s32)blood_types.text[game_mode_scene_girl], 0);
        }
        k_disp_start(6);
        mode_step_next();
        break;
    case 13:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x15U) {
            culture_fortune_timer = 0;
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
        culture_fortune_timer = 0;
        mode_step_next();
        break;
    case 16:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x15U) {
            culture_fortune_timer = 0;
            mode_step_next();
        }
        break;
    case 17:
        set_kanji_string(-0x10, 0x52, 0, (s32) "血液型", 0);
        k_disp_start(9);
        mode_step_next();
        break;
    case 18:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x15U) {
            culture_fortune_timer = 0;
            mode_step_next();
        }
        break;
    case 19:
        set_kanji_string(0x28, 0x52, 0, (s32)player_blood_types.text[(u32) (g->player_birthday << 0x15) >> 0x1E], 0);
        k_disp_start(0xA);
        mode_step_next();
        break;
    case 20:
        culture_fortune_timer++;
        if ((culture_fortune_timer >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            culture_fortune_timer = 0;
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
        audio_command(0x508);
        mode_step_next();
        break;
    case 23:
        culture_fortune_timer++;
        if (culture_fortune_timer >= 0x3DU) {
            talk_group = culture_fortune_lookup() + 3;
            D_80122EA8 = culture_fortune_lookup();
            audio_command(0x50C);
            mode_step_next();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if ((girl_state_rank(0) == 4) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) {
            game_mode_scene_girl = 0;
        } else if ((girl_state_rank(2) == 4) && (((u8) g->girls[2].flags.b[2] >> 4) == 3)) {
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

s32 culture_fortune_lookup(void) {
    s32 result;
    s32 index;
    CultureFortuneBloodTable table;

    table = culture_fortune_blood_table;
    index = game_mode_scene_girl;
    if (index == 2 && ((u8) game_state.girls[2].flags.b[2] >> 4) == 3) {
        if (((u16) game_state.player_flags.h >> 12) != 3) {
            return 3;
        }
    }
    result = table.result[(u32) (game_state.girls[index].birthday.w << 21) >> 30][(u32) (game_state.player_birthday << 21) >> 30];
    return result;
}
