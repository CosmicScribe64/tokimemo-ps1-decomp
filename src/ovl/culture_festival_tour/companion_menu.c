/* companion_menu.c: src/ovl/BUNKAKEN/80143DD0.c:12: stages 1-3: ask a companion, Yoshio, club menu. Original object 0x80143DD0-0x80144F70 (config/objects/BUNKAKEN.txt). */
#define MAIN_API_OVERRIDE_talk_names_set /* T-24005: matched caller passes a word; callee narrows it. */
#include "common.h"
#include "ovl/culture_festival_tour.h"

void talk_names_set();

typedef struct {
    s32 labels[6];
} TourClubMenuLabels;
extern TourClubMenuLabels tour_club_menu_labels;

void tour_stage_ask_companion(void) {
    s32 girl_rank_3;
    s32 girl_rank_6;
    s32 companion_status;
    s32 companion_status_2;
    u8 companion_rank;
    u32 girl_rank;
    u32 girl_rank_2;
    u32 girl_rank_4;
    u32 girl_rank_5;

    switch (game_state.mode_step) {
    case 0:
        if (game_mode_scene_girl != 0xFF) {
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            mode_step_next();
        } else {
            talk_group = 1;
            talk_line = 0;
            mode_step_set(2);
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            tour_script_common();
            talk_group = 1;
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
        normal_date_two_select();
        break;
    case 4:
        if (talk_choice == 1) {
            mode_step_set(6);
        } else {
            mode_step_set(7);
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if (game_mode_scene_girl != 0xFF) {
            game_mode_scene_girl = 0xFF;
        }
        mode_stage_set(TOUR_STAGE_YOSHIO);
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        select_girl();
        break;
    case 9:
        if (talk_choice == declined_companion_girl) {
            talk_group = 4;
            talk_line = 0;
            mode_step_next();
        } else if (talk_choice == 0xB) {
            talk_group = 5;
            talk_line = 0;
            mode_step_set(5);
        } else {
            if ((talk_choice == 0) && ((u32) ((u8) game_state.girls[0].flags.b[2] >> 4) >= 6U)) {
                game_mode_scene_girl = 0xB;
            } else {
                game_mode_scene_girl = talk_choice;
            }
            talk_names_set(talk_choice);
            mode_step_set(0xC);
        }
        break;
    case 10:
        normal_date_speak();
        break;
    case 11:
        talk_group = 2;
        mode_step_set(7);
        break;
    case 12:
        normal_date_speak();
        break;
    case 13:
        portrait_read_step(tour_portrait_lba[game_mode_scene_girl]);
        break;
    case 14:
        addr_init_bustup();
        if (game_mode_scene_girl == 0xB) {
            game_state.current_girl = GIRL_SHIORI;
        } else {
            game_state.current_girl = (u8) game_mode_scene_girl;
        }
        portrait_show_checked();
        cd_read_sectors(tour_invite_script_sectors[game_mode_scene_girl], 0x801E8000, tour_invite_script_lba[game_mode_scene_girl]);
        break;
    case 15:
        normal_date_girl_in();
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            tour_invite_script_setter[game_mode_scene_girl]();
            tour_script_tables_apply();
            if (game_mode_scene_girl == 0) {
                mode_step_set(0x14);
            } else if (game_mode_scene_girl < 4) {
                mode_step_set(0x1F);
            } else {
                mode_step_set(0x2A);
            }
        }
        break;
    case 17:
        normal_date_girl_out();
        break;
    case 18:
        normal_date_speak();
        break;
    case 19:
        game_mode_scene_girl = 0xFF;
        mode_stage_set(TOUR_STAGE_YOSHIO);
        break;
    case 20:
        girl_rank = girl_state_rank(0);
        switch (girl_rank) {
        case 0:
        case 1:
            talk_group = 0x10;
            break;
        case 2:
            talk_group = 0x1B;
            break;
        case 3:
            talk_group = 0x24;
            break;
        case 4:
            talk_group = 0x2D;
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        girl_rank_2 = girl_state_rank(0);
        switch (girl_rank_2) {
        case 0:
        case 1:
            talk_group = 0x11;
            break;
        case 2:
            talk_group = 0x1C;
            break;
        case 3:
            talk_group = 0x25;
            break;
        case 4:
            talk_group = 0x2E;
            break;
        }
        /* The halfword conversion preserves the original temporary order. T-24005. */

        talk_group += (s16) ((u8) game_state.girls[0].flags.b[2] >> 4);
        mode_step_next();
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        if (girl_state_rank(0) == 4) {
            talk_group = 0x34;
            mode_step_set(0x11);
        } else {
            if (game_state.girls[0].flags.f.b2) {
                talk_group = 0x17;
            } else {
                talk_group = 0x18;
            }
            mode_step_next();
        }
        game_state.girls[0].flags.f.b2 = 1;
        break;
    case 25:
        normal_date_speak();
        break;
    case 26:
        girl_rank_3 = girl_state_rank(0);
        switch (girl_rank_3) {
        case 0:
        case 1:
            talk_group = 0x19;
            mode_step_next();
            break;
        case 2:
            talk_group = 0x22;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x2B;
            mode_step_next();
            break;
        }
        /* fallthrough */
    case 27:
        normal_date_speak();
        break;
    case 28:
        normal_date_girl_out();
        break;
    case 29:
        normal_date_speak();
        break;
    case 30:
        game_mode_scene_girl = 0xFF;
        mode_stage_set(TOUR_STAGE_YOSHIO);
        break;
    case 31:
        girl_rank_4 = girl_state_rank(game_mode_scene_girl);
        switch (girl_rank_4) {
        case 0:
        case 1:
            talk_group = 8;
            break;
        case 2:
            talk_group = 0xF;
            break;
        case 3:
            talk_group = 0x15;
            break;
        case 4:
            talk_group = 0x1B;
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        break;
    case 33:
        girl_rank_5 = girl_state_rank(game_mode_scene_girl);
        switch (girl_rank_5) {
        case 0:
        case 1:
            talk_group = 9;
            break;
        case 2:
            talk_group = 0x10;
            break;
        case 3:
            talk_group = 0x16;
            break;
        case 4:
            talk_group = 0x1C;
            break;
        }
        if (((game_mode_scene_girl == 1) && (((u8) game_state.girls[1].flags.b[2] >> 4) == 1)) || ((game_mode_scene_girl == 2) && (((u8) game_state.girls[2].flags.b[2] >> 4) == 3)) || ((game_mode_scene_girl == 3) && (((u8) game_state.girls[3].flags.b[2] >> 4) == 5))) {
            talk_group += 1;
        }
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        if (girl_state_rank(game_mode_scene_girl) == 4) {
            talk_group = 0xE;
            mode_step_set(0x11);
        } else {
            if ((game_mode_scene_girl == 1) && (girl_state_rank(1) < 2)) {
                talk_group = 0x12;
            } else if (game_state.girls[game_mode_scene_girl].flags.f.b2 == 1) {
                talk_group = 0xB;
            } else {
                talk_group = 0x14;
            }
            mode_step_next();
        }
        game_state.girls[game_mode_scene_girl].flags.f.b2 = 1;
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        girl_rank_6 = girl_state_rank(game_mode_scene_girl);
        switch (girl_rank_6) {
        case 0:
        case 1:
            talk_group = 0xC;
            if (((game_mode_scene_girl == 1) && (((u8) game_state.girls[1].flags.b[2] >> 4) == 1)) || ((game_mode_scene_girl == 2) && (((u8) game_state.girls[2].flags.b[2] >> 4) == 3)) || ((game_mode_scene_girl == 3) && (((u8) game_state.girls[3].flags.b[2] >> 4) == 5))) {
                talk_group = 0xD;
            }
            mode_step_next();
            break;
        case 2:
            talk_group = 0x13;
            mode_step_next();
            break;
        case 3:
            talk_group = 0x19;
            mode_step_next();
            break;
        }
        break;
    case 38:
        normal_date_speak();
        break;
    case 39:
        normal_date_girl_out();
        talk_group = 0xE;
        break;
    case 40:
        normal_date_speak();
        break;
    case 41:
        game_mode_scene_girl = 0xFF;
        mode_stage_set(TOUR_STAGE_YOSHIO);
        break;
    case 42:
        if (game_mode_scene_girl == 0xB) {
            companion_status = get_g_zyotai_s(GIRL_SHIORI);
        } else {
            /* FAKE: self-store preserves the original store in the call delay slot. T-24005. */
            game_mode_scene_girl = game_mode_scene_girl;
            companion_status = get_g_zyotai_s(game_mode_scene_girl);
        }
        companion_rank = companion_status & 0x7F;
        /* FAKE: empty test keeps the mask in t4 and the switch index in t5. T-24005. */
        if (companion_rank) {}
        switch (companion_rank) {
        case 0:
        case 1:
            talk_group = 3;
            break;
        case 2:
            talk_group = 4;
            break;
        case 3:
            talk_group = 5;
            break;
        case 4:
            if ((game_mode_scene_girl == 4) && (((u16) game_state.player_flags.h >> 0xC) == ((u8) game_state.girls[4].flags.b[2] >> 4))) {
                talk_group = 8;
            } else {
                talk_group = 6;
            }
            break;
        }
        talk_line = 0;
        mode_step_next();
        break;
    case 43:
        normal_date_speak();
        break;
    case 44:
        if (game_mode_scene_girl == 0xB) {
            companion_status_2 = get_g_zyotai_s(GIRL_SHIORI);
        } else {
            /* FAKE: self-store preserves the original store in the call delay slot. T-24005. */
            game_mode_scene_girl = game_mode_scene_girl;
            companion_status_2 = get_g_zyotai_s(game_mode_scene_girl);
        }
        if (companion_status_2 == 4) {
            mode_step_set(0x11);
        } else if ((game_mode_scene_girl == 8) && (girl_state_rank(8) >= 2)) {
            mode_step_next();
        } else {
            mode_stage_set(TOUR_STAGE_CLUB_MENU);
        }
        break;
    case 45:
        switch (game_state.year) {
        case 0x5F:
            talk_group = 9;
            break;
        case 0x60:
            talk_group = 0xA;
            break;
        case 0x61:
            talk_group = 0xB;
            break;
        }
        if (girl_state_rank(8) == 3) {
            talk_group += 3;
        }
        mode_step_next();
        break;
    case 46:
        normal_date_speak();
        break;
    case 47:
        talk_group = 0xF;
        mode_step_next();
        break;
    case 48:
        normal_date_speak();
        break;
    case 49:
        mode_stage_set(TOUR_STAGE_CLUB_MENU);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_yoshio(void) {
    switch (game_state.mode_step) {
    case 0:
        portrait_read_step(0x6C5F);
        break;
    case 1:
        addr_init_bustup();
        game_state.current_girl = GIRL_YOSHIO;
        portrait_show_checked();
        cd_read_sectors(5U, 0x801E8000, 0x49BA);
        break;
    case 2:
        normal_date_girl_in();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            talk_names_set(0xDU);
            tour_script_common();
            talk_group = 6;
            talk_line = 0;
            talk_text_table = (s32) tour_script_text;
            talk_command_table = (s32) tour_script_commands;
            talk_voice_table = (s32) tour_script_voices;
            mode_step_next();
        }
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        switch (game_state.year) {
        case 0x5F:
            break;
        case 0x60:
            talk_group += 1;
            break;
        case 0x61:
            talk_group += 2;
            break;
        default:
            game_mode_reset_to_title();
            break;
        }
        mode_step_next();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        normal_date_girl_out();
        break;
    case 8:
        talk_group = 0xA;
        mode_step_next();
        break;
    case 9:
        normal_date_speak();
        break;
    case 10:
        mode_stage_set(TOUR_STAGE_CLUB_MENU);
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void tour_stage_club_menu(void) {
    switch (game_state.mode_step) {
    case 0:
        tour_club_menu_init();
        mode_step_next();
        return;
    case 1:
        tour_club_menu_update();
        return;
    case 2:
        ((CultureFestivalDispatch *)&culture_exhibit_club)->club_id = talk_choice;
        if (((CultureFestivalDispatch *)&culture_exhibit_club)->girl == 0xFF) {
            talk_group = talk_choice + 0xC;
            mode_step_next();
            return;
        }
        mode_stage_next();
        return;
    case 3:
        normal_date_speak();
        return;
    case 4:
        mode_stage_next();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void tour_club_menu_init(void) {
    s32 i;
    s32 pad[2]; /* FAKE: unused local above the arrays reproduces the original frame 0xC0; real source unknown. T-9350 */
    s16 label_x[6];
    s16 label_y[6];
    s16 label_width[6];
    s16 label_height[6];
    TourClubMenuLabels club_menu_labels;

    club_menu_labels = tour_club_menu_labels;
    k_reset(1);
    for (i = 0; i < 6; i++) {
        label_x[i] = (i & 1) * 0x78 - 0x80;
        label_y[i] = (i >> 1) * 0x10 + 0x32;
        set_kanji_string(label_x[i], label_y[i], 0, club_menu_labels.labels[i], 0);
        label_width[i] = 0x78;
        label_height[i] = 0x10;
    }
    talk_menu_active = 1;
    k_disp_start(7);
    menu_set(0, 6, label_x, label_y, label_width, label_height);
    hud_cursor_actor_init(0);
}

void tour_club_menu_update(void) {
    s32 i;

    menu_check(0, cursor_x, cursor_y);
    menu_cursor_update(0);
    menu_bar_show(0);
    hud_cursor_show(0);
    hud_cursor_update();
    i = 0;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        for (i = 0; i < 6; i++) {
            if (game_state.menu_items[i * 2].hover != 0) {
                talk_choice = i;
                D_8011ECD3 &= ~0x80;
                k_reset(1);
                talk_menu_active = 0;
                mode_step_next();
            }
        }
    }
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_literature);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_drama);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_science);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_computer);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_art);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/culture_festival_tour/companion_menu.rodata", tour_menu_brass);
