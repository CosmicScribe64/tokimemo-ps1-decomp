/* player_birthday_girl_selection.c: src/ovl/SHUGAKU/80133430.c:9-65; binds player-birthday visit scripts, loads AF43, dispatches stage7 callbacks and selects the best eligible girl in trip_low_status_companion_select; called by 80136380.c:case7. Original object 0x80133430-0x80133D60 (config/objects/SHUGAKU.txt). */
#define MAIN_API_OVERRIDE_get_h_tokimeki /* u8 return narrows the metric byte on return (T-24122). */
#define MAIN_API_OVERRIDE_get_h_yuukou   /* u8 return narrows the metric byte on return (T-24122). */
#include "common.h"
#include "ovl/school_trip.h"

u8 get_h_tokimeki(s32 arg0);
u8 get_h_yuukou(s32 arg0);

void trip_scene_pointer_table_setup(void) {
    D_8013BE70 = 0x801B00DC;
    D_8013BE74 = 0x801B02AC;
    D_8013BE78 = 0x801B03C8;
    D_8013BE7C = 0x801B04C4;
    D_8013BE80 = 0x801B0668;
    D_8013BE84 = 0x801B0808;
    D_8013BE88 = 0x801B09DC;
    D_8013BE8C = 0x801B0BE8;
    D_8013BE90 = 0x801B0D8C;
    D_8013BE94 = 0x801B0F5C;
    D_8013BE98 = 0x801B1164;
    D_8013BE9C = 0x801B12D8;
    D_8013BEA0 = 0x801B1454;
    D_8013BEA4 = 0x801B00F4;
    D_8013BEA8 = 0x801B02C8;
    D_8013BEAC = 0x801B03D0;
    D_8013BEB0 = 0x801B04DC;
    D_8013BEB4 = 0x801B0680;
    D_8013BEB8 = 0x801B0820;
    D_8013BEBC = 0x801B09F8;
    D_8013BEC0 = 0x801B0C00;
    D_8013BEC4 = 0x801B0DA4;
    D_8013BEC8 = 0x801B0F78;
    D_8013BECC = 0x801B1180;
    D_8013BED0 = 0x801B12E0;
    D_8013BED4 = 0x801B1470;
    D_8013BED8 = 0x801B01A8;
    D_8013BEDC = 0x801B0398;
    D_8013BEE0 = 0x801B03F0;
    D_8013BEE4 = 0x801B0590;
    D_8013BEE8 = 0x801B0734;
    D_8013BEEC = 0x801B08D4;
    D_8013BEF0 = 0x801B0AE4;
    D_8013BEF4 = 0x801B0CB4;
    D_8013BEF8 = 0x801B0E58;
    D_8013BEFC = 0x801B1064;
    D_8013BF00 = 0x801B126C;
    D_8013BF04 = 0x801B1338;
    D_8013BF08 = 0x801B155C;
}

void trip_mode_step_callback_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    TripTravelSceneCallbacks tbl;

    tbl = D_8013BF20;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void trip_scene_sector_load(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    trip_scene_pointer_table_setup();
    mode_step_next();
}

void trip_scene_dialogue_setup(void) {
    talk_names_set(game_state.current_girl);
    trip_scene_pointer_table_setup();
    trip_companion_dialogue_pointer_bind();
    if (*(u8 *)&scene_trip_day_state != 2) {
        if (bg_brightness != 0) {
            scene_trip_day_state = 1;
        } else {
            scene_trip_day_state = 0;
        }
    }
    trip_companion_dialogue_group = 0;
    trip_companion_dialogue_line = 0;
    mode_step_next();
    if (*(u8 *)&scene_trip_day_state == 2) {
        game_state.mode_step += 2;
    }
}

s32 trip_low_status_companion_select(void) {
    s16 i;
    u8 best_i;
    u8 best_s;
    u8 s;

    best_i = 0xFF;
    best_s = 4;
    for (i = 0; i < 10; i++) {
        /* FAKE: dead branch adjusts the loop basic-block count so uopt assigns best_s to $s5 (T-24122). */
        if (1) {}
        if (select_girl_menu_enabled[i] != 0) {
            s = get_g_zyotai_s(i) & 0x7F;
            /* FAKE: duplicate `s == best_s` shifts uopt's live-range priority between s and best_s (T-24122). */
            if (s < best_s ||
                (s == best_s && s == best_s &&
                 get_h_tokimeki(i) + get_h_yuukou(i) >
                     get_h_tokimeki(best_i) + get_h_yuukou(best_i))) {
                best_i = i;
                best_s = s;
            }
        }
    }
    s = get_g_zyotai_s(best_i) & 0x7F;
    if (best_i == 4 && s < 2U && game_state.saved_mode_stage == 5) {
        if (game_state.scene_flags[0x20] != 0) {
            game_state.day_event_done_bits |= 8;
        }
    }
    if (best_i != 0xFF && s < 2U && !(game_state.day_event_done_bits & 8)) {
        /* FAKE: `(best_i & 0) +` keeps cfe's CVT J L narrowing so uopt emits `andi $a0, $s7, 0xff` (T-24122). */
        game_state.current_girl = (best_i & 0) + best_i;
        talk_names_set(game_state.current_girl);
        trip_companion_dialogue_pointer_bind();
        if (game_state.saved_game_mode != 6) {
            event_log_add(0xD, 0x26);
        } else {
            switch (trip_destination) {
            case 0:
                event_log_add(0xD, 0x27);
                break;
            case 1:
                event_log_add(0xD, 0x28);
                break;
            default:
                event_log_add(0xD, 0x29);
                break;
            }
        }
    } else {
        trip_companion_text_table = D_8013BE9C;
        trip_companion_command_table = D_8013BED0;
        trip_companion_voice_table = D_8013BF04;
        trip_companion_dialogue_group = 0;
        trip_companion_dialogue_line = 0;
        game_state.mode_step += 0x20;
        return 0;
    }
    mode_step_next();
}

void trip_companion_dialogue_pointer_bind(void) {
    switch (game_state.current_girl) {
    case GIRL_SHIORI:
        trip_companion_text_table = D_8013BE74;
        trip_companion_command_table = D_8013BEA8;
        trip_companion_voice_table = D_8013BEDC;
        return;
    case GIRL_MIO:
        trip_companion_text_table = D_8013BE88;
        trip_companion_command_table = D_8013BEBC;
        trip_companion_voice_table = D_8013BEF0;
        return;
    case GIRL_YUINA:
        trip_companion_text_table = D_8013BE7C;
        trip_companion_command_table = D_8013BEB0;
        trip_companion_voice_table = D_8013BEE4;
        return;
    case GIRL_AYAKO:
        trip_companion_text_table = D_8013BE80;
        trip_companion_command_table = D_8013BEB4;
        trip_companion_voice_table = D_8013BEE8;
        return;
    case GIRL_SAKI:
        trip_companion_text_table = D_8013BE98;
        trip_companion_command_table = D_8013BECC;
        trip_companion_voice_table = D_8013BF00;
        return;
    case GIRL_YUKARI:
        trip_companion_text_table = D_8013BE8C;
        trip_companion_command_table = D_8013BEC0;
        trip_companion_voice_table = D_8013BEF4;
        return;
    case GIRL_NOZOMI:
        trip_companion_text_table = D_8013BE90;
        trip_companion_command_table = D_8013BEC4;
        trip_companion_voice_table = D_8013BEF8;
        return;
    case GIRL_MIRA:
        trip_companion_text_table = D_8013BE84;
        trip_companion_command_table = D_8013BEB8;
        trip_companion_voice_table = D_8013BEEC;
        return;
    case GIRL_YUKO:
        trip_companion_text_table = D_8013BE70;
        trip_companion_command_table = D_8013BEA4;
        trip_companion_voice_table = D_8013BED8;
        return;
    case GIRL_MEGUMI:
        trip_companion_text_table = D_8013BE94;
        trip_companion_command_table = D_8013BEC8;
        trip_companion_voice_table = D_8013BEFC;
        return;
    case GIRL_YUMI:
        trip_companion_text_table = D_8013BEA0;
        trip_companion_command_table = D_8013BED4;
        trip_companion_voice_table = D_8013BF08;
        /* fallthrough */
    default:
        return;
    }
}

void trip_companion_talk_run(void) {
    talk_tables = (u8 *) &trip_companion_dialogue_group;
    talk_active_line_ptr = (u8 *) &trip_companion_dialogue_line;
    talk_active_text_table = trip_companion_text_table;
    talk_active_command_table = trip_companion_command_table;
    talk_active_voice_table = trip_companion_voice_table;
    talk_run(talk_choice, 1, 0);
}
