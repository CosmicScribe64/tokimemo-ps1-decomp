/* mode_stage_and_club_routes.c: src/ovl/BUNKAKEN/80145E40.c:9-35; indexes the142-handler table by mode_stage and selects routes from current club, known girls and companion state. Original object 0x80145E40-0x80146560 (config/objects/BUNKAKEN.txt). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

typedef struct {
    void (*handler[142])();
} TourStageTable; /* size 0x238 */
extern TourStageTable tour_stage_table;

void tour_stage_run(void) {
    s32 stage_index; /* declared before stage_handlers: its stack slot sits above stage_handlers (T-3330) */
    TourStageTable stage_handlers;

    stage_handlers = tour_stage_table;
    stage_index = game_state.mode_stage;
    stage_handlers.handler[stage_index]();
}

void tour_stage_route_visit(void) {
    GameState *g;

    g = &game_state;
    if (game_mode_scene_girl == 0xFF) {
        switch (D_80122EB8) {
        case 0:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 0) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 0) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_MIO_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        case 1:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 1) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 1) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_MIO_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        case 2:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 2) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 2) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_YUINA_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_SCIENCE);
            return;
        case 3:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 3) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 3) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_YUINA_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        case 4:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 4) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 4) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_AYAKO_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        case 5:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 5) {
                mode_stage_set(TOUR_STAGE_SHIORI_CLUB_ROUTE);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 5) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                mode_stage_set(TOUR_STAGE_AYAKO_CLUB_ROUTE);
                return;
            }
            mode_stage_set(TOUR_STAGE_SOLO_ROUTE);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
    } else {
        switch (game_mode_scene_girl) {
        case 11:
            mode_stage_set(TOUR_STAGE_SHIORI_COMPANION_ROUTE);
            return;
        case 4:
            mode_stage_set(TOUR_STAGE_SAKI_ROUTE);
            return;
        case 5:
            mode_stage_set(TOUR_STAGE_YUKARI_ROUTE);
            return;
        case 6:
            mode_stage_set(TOUR_STAGE_NOZOMI_ROUTE);
            return;
        case 7:
            mode_stage_set(TOUR_STAGE_MIRA_ROUTE);
            return;
        case 8:
            mode_stage_set(TOUR_STAGE_YUKO_ROUTE);
            return;
        case 9:
            mode_stage_set(TOUR_STAGE_MEGUMI_ROUTE);
            return;
        case 10:
            mode_stage_set(TOUR_STAGE_YUMI_ROUTE);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
    }
}

void tour_stage_resume(void) {
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

void tour_stage_tt_narrator(void) {
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
            tour_script_narrator();
            game_state.current_girl = GIRL_YUKO;
            talk_names_set(8U);
            strcpy(name_code_girl_short, "　");
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) tour_script_text;
            talk_command_table = (s32) tour_script_commands;
            talk_voice_table = (s32) tour_script_voices;
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
