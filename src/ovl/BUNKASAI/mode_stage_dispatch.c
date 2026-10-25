/* mode_stage_dispatch.c: src/ovl/BUNKASAI/80150930.c:4; routes game_state.mode_stage through the culture-festival handler table; preserves the original dispatcher object. Original object 0x80150930-0x801511E0 (config/objects/BUNKASAI.txt). */
#define MAIN_API_OVERRIDE_srand /* T-24094: int-returning view for culture_visitor_pick */
#include "common.h"
#include "ovl/BUNKASAI.h"

s32 srand();

typedef struct {
    void (*stage_function[165])();
} CultureStageTable; /* size 0x294 */
extern CultureStageTable culture_stage_table;

void culture_stage_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    CultureStageTable tbl;

    tbl = culture_stage_table;
    idx = game_state.mode_stage;
    tbl.stage_function[idx]();
}

void culture_visitor_pick(void) {
    s32 temp_v1;
    u8 count;
    s32 i;
    CultureGirlList sp30;

    sp30 = culture_visitor_list_init;
    srand(game_state.timer_ticks);
    count = 0;
    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].flags.f.met == 1) {
            sp30.girl[count] = i;
            count++;
        }
    }
    temp_v1 = sp30.girl[rand() % count];
    game_mode_scene_girl = temp_v1;
    switch (temp_v1) {
    case 0:
        mode_stage_set(CULTURE_STAGE_SHIORI_VISIT);
        return;
    case 1:
        mode_stage_set(CULTURE_STAGE_MIO_VISIT);
        return;
    case 2:
        mode_stage_set(CULTURE_STAGE_YUINA_VISIT);
        return;
    case 3:
        mode_stage_set(CULTURE_STAGE_AYAKO_VISIT);
        return;
    case 4:
        mode_stage_set(CULTURE_STAGE_SAKI_VISIT);
        return;
    case 5:
        mode_stage_set(CULTURE_STAGE_YUKARI_VISIT);
        return;
    case 6:
        mode_stage_set(CULTURE_STAGE_NOZOMI_VISIT);
        return;
    case 7:
        mode_stage_set(CULTURE_STAGE_MIRA_VISIT);
        return;
    case 8:
        mode_stage_set(CULTURE_STAGE_YUKO_VISIT);
        return;
    case 9:
        mode_stage_set(CULTURE_STAGE_MEGUMI_VISIT);
        return;
    case 10:
        mode_stage_set(CULTURE_STAGE_YUMI_VISIT);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_stage_intro(void) {
    u16 temp_v1_2;
    u32 temp_t2;
    u8 temp_v1;
    /* FAKE: T-22012, the record reads go through game while the selector and the 0xF4 halfword are read from
     * game_state directly; the split is what gives the hoisted constant 1 the register $v0 (all-game: 50 rows). */
    GameState *game = &game_state;

    temp_v1 = game_state.mode_step;
    switch (temp_v1) {
    case 0:
        talk_group = ((((u16) game_state.player_flags.h >> 0xC) * 3) + game_state.year) - 0x4D;
        mode_step_next();
        return;
    case 1:
        normal_date_speak();
        return;
    case 2:
        temp_v1_2 = game_state.player_flags.h;
        D_80122EB8 = (s32) ((u16) game_state.player_flags.h >> 0xC);
        temp_t2 = temp_v1_2 >> 0xC;
        switch (temp_t2) {
        case 0:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 0) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[1].flags.b[2] >> 4) == 0) && (game->girls[1].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_MIO_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        case 1:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 1) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[1].flags.b[2] >> 4) == 1) && (game->girls[1].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_MIO_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        case 2:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 2) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[2].flags.b[2] >> 4) == 2) && (game->girls[2].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_YUINA_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        case 3:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 3) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[2].flags.b[2] >> 4) == 3) && (game->girls[2].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_YUINA_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        case 4:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 4) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[3].flags.b[2] >> 4) == 4) && (game->girls[3].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        case 5:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 5) {
                mode_stage_set(CULTURE_STAGE_SHIORI_MEMBER);
                return;
            }
            if ((((u8) game->girls[3].flags.b[2] >> 4) == 5) && (game->girls[3].flags.f.met == 1)) {
                mode_stage_set(CULTURE_STAGE_AYAKO_MEMBER);
                return;
            }
            mode_stage_set(CULTURE_STAGE_VISITOR_PICK);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void culture_stage_game_resume(void) {
    switch (game_state.mode_step) {
    case 0:
        actor_clear_range(0);
        hud_cursor_show(0);
        vram_clear_blocks(0xFF);
        mode_step_next();
        return;
    case 1:
        boot_data_load_step();
        return;
    case 2:
        boot_hud_textures_step();
        return;
    case 3:
        debug_font_load();
        mode_step_next();
        return;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        mode_step_next();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            calendar_month_load(game_state.year, game_state.month, 0);
            mode_step_next();
            return;
        }
        return;
    case 6:
        hud_cursor_actor_init(0);
        hud_cursor_show(0);
        D_800B5BC8 = 0xFE;
        mode_stage_set((s32) (*(u8 *)&D_80122EB0));
        mode_step_set((s32) (*(u8 *)&D_80122EB4));
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void culture_stage_game_launch(void) {
    switch (game_state.mode_step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B7F);
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            message_window_set_title((s32) "天の声");
            k_disp_start(1);
            culture_script_set_narrator();
            game_state.current_girl = GIRL_YUKO;
            talk_names_set(8U);
            strcpy(name_code_girl_short, "　");
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) culture_script_text_table;
            talk_command_table = (s32) culture_script_command_table;
            talk_voice_table = (s32) culture_script_voice_table;
            mode_step_next();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (game_state.input_device == INPUT_PAD) {
            actor_clear_range(0);
            hud_cursor_show(0);
            set_dec_bri(0);
            game_mode_set(GAME_MODE_TT_GAME);
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}
