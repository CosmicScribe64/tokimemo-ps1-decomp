/* scene_actor_resource_tables.c: ; src/ovl/TAIIKU/80140810.c:9-12 establishes actor/resource tables for its scene handlers. Original object 0x80140810-0x80141280 (config/objects/TAIIKU.txt). */
#define MAIN_API_OVERRIDE_scene_still_fade_in /* callers ignore the result, the original calls it as int; blocks $v0 (T-16554) */
#define MAIN_API_OVERRIDE_scene_still_fade_out /* callers ignore the result, the original calls it as int (T-16554) */
#include "common.h"
#include "ovl/TAIIKU.h"

s32 scene_still_fade_in();
s32 scene_still_fade_out();

/* FAKE: literal asset bases retain the independent source loads; named
 * declarations hoist their bases. The asset data is loaded outside this
 * overlay (T-3300, T-9720). */
void sports_scene_actor_resource_addresses_init(void) {
    D_80149FB0 = 0x80180144;
    D_80149FB4 = 0x801820FC;
    D_80149FB8 = 0x80183E7C;
    D_80149FBC = 0x80185F94;
    D_80149FC0 = 0x80186674;
    D_80149FC4 = 0x80187088;
    D_80149FC8 = 0x80187F20;
    D_80149FCC = 0x80188BC8;
    D_80149FD0 = 0x80188DA4;
    D_80149FD4 = 0x80189A90;
    D_80149FD8 = 0x8018A794;
    D_80149FDC = 0x8018C0C8;
    D_80149FE0 = 0x8018CEE0;
    D_80149FE4 = 0x8018EA78;
    D_80149FE8 = 0x8018016C;
    D_80149FEC = 0x801823BC;
    D_80149FF0 = 0x80184100;
    D_80149FF4 = 0x801861D4;
    D_80149FF8 = 0x801866BC;
    D_80149FFC = 0x80187170;
    D_8014A000 = 0x80188048;
    D_8014A004 = 0x80188C98;
    D_8014A008 = 0x80188DB4;
    D_8014A00C = 0x80189BF4;
    D_8014A010 = 0x8018A89C;
    D_8014A014 = 0x8018C31C;
    D_8014A018 = 0x8018CFEC;
    D_8014A01C = 0x8018EC14;
    D_8014A020 = *(s16 *)0x8018EC40;
    D_8014A024 = *(s16 *)0x8018EC44;
    D_8014A028 = *(s16 *)0x8018EC48;
    D_8014A02C = *(s16 *)0x8018EC4C;
    D_8014A030 = *(s16 *)0x8018EC50;
    D_8014A034 = *(s16 *)0x8018EC54;
    D_8014A038 = *(s16 *)0x8018EC58;
    D_8014A03C = *(s16 *)0x8018EC5C;
    D_8014A040 = *(s16 *)0x8018EC60;
    D_8014A044 = *(s16 *)0x8018EC64;
    D_8014A048 = *(s16 *)0x8018EC68;
    D_8014A04C = *(s16 *)0x8018EC6C;
    D_8014A050 = *(s16 *)0x8018EC70;
    D_8014A054 = *(s16 *)0x8018EC74;
}

s32 sports_mio_ground_scene_step(void) {
    s32 i;

    portrait_wink_update();
    switch (game_state.mode_step) {
    case 0:
        audio_bank_wait_done = 0;
        text_system_reset();
        tpage_buf_clear();
        game_state.tpage_buf[0x1A] = -1;
        actor_clear_upper();
        dec_bg_cd_read(0x413C, 1);
        D_8014A064 = 0;
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_decode_to_vram(1);
            portrait_read_start(0x5F5D);
            mode_step_next();
        }
        break;
    case 2:
        normal_date_move_place();
        break;
    case 3:
        hud_sprite_light_effect_draw();
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            message_window_set_title((s32) "グラウンド");
            k_disp_start(1);
            addr_init_bustup();
            game_state.current_girl = GIRL_MIO;
            portrait_show_checked();
        }
        break;
    case 4:
        hud_sprite_light_effect_draw();
        cd_read_sectors(1U, 0x801E8000, 0xB7BE);
        mode_step_next();
        break;
    case 5:
        hud_sprite_light_effect_draw();
        if (cd_read_sync_status() & 1) {
            text_marker_hide();
            talk_names_set(1U);
            sports_mio_festival_talk_resource_addresses_init();
            talk_group = 0;
            talk_line = 0;
            talk_text_table = D_8014A0A0;
            talk_command_table = D_8014A0A4;
            talk_voice_table = D_8014A0A8;
            mode_step_next();
        }
        break;
    case 6:
    case 8:
    case 12:
    case 14:
        normal_date_speak();
        hud_sprite_light_effect_draw();
        break;
    case 7:
        normal_date_girl_in();
        hud_sprite_light_effect_draw();
        break;
    case 9:
        normal_date_two_select();
        hud_sprite_light_effect_draw();
        break;
    case 10:
        /* FAKE: scalar view keeps uopt from promoting D_80122CDC into $v0; the original selector
         * is in $v1. T-16554 */
        switch (*(s32 *)&D_80122CDC) {
        case 0:
            mode_step_set(0x11);
            break;
        case 1:
            mode_step_set(0xB);
            break;
        }
        hud_sprite_light_effect_draw();
        break;
    case 11:
        game_state.girls[1].affection -= 1;
        game_state.girls[1].tokimeki -= 1;
        game_state.girls[1].syoushin += 0xA;
        portrait_layout_set(3U);
        portrait_face_update();
        talk_group = 0xA;
        talk_line = 0;
        hud_sprite_light_effect_draw();
        mode_step_next();
        break;
    case 13:
        hud_sprite_light_effect_draw();
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_8014A064 = 2;
        break;
    case 17:
        normal_date_speak();
        hud_sprite_light_effect_draw();
        break;
    case 18:
        hud_sprite_light_effect_draw();
        dec_bg_cd_read(0x407A, 1);
        mode_step_next();
        break;
    case 19:
        normal_date_bggirl_fadeout();
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            message_window_set_title((s32) "校舎裏");
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            cd_read_sectors(0x45U, 0x80197000, 0xBB15);
            audio_bank_start(0, 0x201);
            mode_step_next();
        }
        break;
    case 21:
        normal_date_bggirl_fadein();
        break;
    case 22:
        if (cd_read_sync_status() & 1) {
            sports_mio_scene_picture_resource_addresses_init();
            load_palette(D_8014A0F0, 0x11, 1, 2, 0);
            scene_still_show(D_8014A0F4, D_8014A0F8, D_8014A0FC, D_8014A100, D_8014A104, D_8014A108);
            scene_anim_show(D_8014A0E4, D_8014A0E8, D_8014A0E0, (s32) D_8014A0EC);
            for (i = 0; i < 2; i++) {
                ((Actor *)actor_pool)[96 + i].unk_04 |= 8;
            }
            ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
            ((Actor *)actor_pool)[98].sequence_slot = 4;
            mode_step_next();
        }
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        draw_shake_start();
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) "ドシン", 0);
        audio_command(0x60A);
        Vblnk_Timer_Init();
        mode_step_next();
        break;
    case 25:
        if (Vblnk_Timer() >= 0x3D) {
            dec_bg_cd_read(0x4721, 1);
            k_reset(1);
            mode_step_next();
        }
        break;
    case 26:
        normal_date_bggirl_fadeout();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, dec_bg_decode_to_vram(1));
            mode_step_next();
        }
        break;
    case 28:
        scene_still_fade_in();
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        ((Actor *)actor_pool)[98].brightness += 4;
        if (((Actor *)actor_pool)[98].brightness >= 0x80) {
            ((Actor *)actor_pool)[98].brightness = 0x80;
        }
        break;
    case 29:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((Actor *)actor_pool)[98].flags = 0;
            ((Actor *)actor_pool)[98].unk_18 = 0;
        } else {
            ((Actor *)actor_pool)[98].flags = 1;
        }
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        ((Actor *)actor_pool)[98].sequence_slot = 5;
        mode_step_next();
        break;
    case 32:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((Actor *)actor_pool)[98].flags = 0;
            ((Actor *)actor_pool)[98].unk_18 = 0;
        } else {
            ((Actor *)actor_pool)[98].flags = 1;
        }
        break;
    case 33:
        ((Actor *)actor_pool)[96].sequence_slot = 3;
        ((Actor *)actor_pool)[97].sequence_slot = 1;
        ((Actor *)actor_pool)[97].unk_18 = 0;
        ((Actor *)actor_pool)[97].unk_08 = 0;
        mode_step_next();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((Actor *)actor_pool)[98].flags = 0;
            ((Actor *)actor_pool)[98].unk_18 = 0;
        } else {
            ((Actor *)actor_pool)[98].flags = 1;
        }
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        scene_still_fade_out();
        ((Actor *)actor_pool)[98].brightness -= 4;
        if (((Actor *)actor_pool)[98].brightness < 3) {
            ((Actor *)actor_pool)[98].brightness = 0;
        }
        break;
    case 38:
        D_8014A064 = 1;
        event_log_add(2, 0xD);
        game_state.girls[1].affection += 2;
        game_state.girls[1].tokimeki += 1;
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    Default_Disp();
    return (s32) D_8014A064;
}
