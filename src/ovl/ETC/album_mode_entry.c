/* album_mode_entry.c: ; src/ovl/ETC/8013FF80.c:9-24 dispatches album mode entry and stage state. Original object 0x8013FF80-0x80140F50 (config/objects/ETC.txt). */
#define MAIN_API_OVERRIDE_memcpy /* original called it undeclared (implicit int), as 8013BEF0.c (T-24706); keeps $v0 off the case 5 table address, T-25314 */
#define MAIN_API_OVERRIDE_prim_bg_slot_setup /* original called it undeclared (implicit int): its $v0 result keeps 0x01000000 off $v0 after the first call, T-25314 */
#include "common.h"
#include "ovl/ETC.h"

extern s32 memcpy();
extern s32 prim_bg_slot_setup();

void album_stage_load(void) {
    if (game_state.mode_step == 0) {
        album_load_step();
    }
}

void album_secret_mode_update(void);

s32 album_load_step(void) {
    /* FAKE: `register` makes i a VREG, so the dead entry read below is a direct unsigned
     * global stored into a VREG and cvt_pass.py emits OPTN 405 0 (zcopy off): the second
     * table loop keeps its runtime index. It replaces a dead two-case switch, whose extra VREG
     * cost 8 frame bytes and a `move v0,a1`. i declared before sp54, the scalars after it, put
     * sp54 at 0x54. T-25314 */
    register s32 i;
    s32 sp54[6];
    u32 j;
    u32 k;

    /* FAKE: the dead read of unk_F5F makes it an entry location for cvt_pass.py, so its
     * promoted range starts at the entry ($t0, reloaded in cases 0, 5 and 8, no entry load).
     * T-25314 */
    i = game_state.current_girl;
    switch ((*(u8 *)&game_state.substep)) {
    case 0:
        draw2d3d(1, 0);
        game_state.clear_color_g = 0;
        game_state.clear_color_r = 0;
        game_state.clear_color_b = 0;
        actor_draw_enable(1);
        actor_clear_range(0);
        hud_cursor_show(0);
        sprite_tables_clear();
        text_system_reset();
        back_clear_switch(1);
        tpage_buf_clear();
        album_voice_clip = 0;
        album_tpage_slots_reset();
        album_voice_clip_table = album_voice_clips[game_state.current_girl % 13U];
        audio_bank_load_start(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        (*(u8 *)&game_state.substep) += 1;
    default:
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_command(0x201);
            audio_bank_load_start(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 2:
        if (audio_bank_load_poll() == 1) {
            cd_read_sectors_nowait(1U, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69U, 0x80180000, 0xB389);
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 5:
        draw2d3d(1, 1);
        album_model_init();
        title_view_init();
        title_lights_init();
        title_coordinates_init();
        load_palette(0x80180000, 0x1F, 1, 3, 0);
        load_palette(0x80180200, 0x1E, 1, 3, 0);
        load_palette(0x80180400, 0x1D, 1, 3, 0);
        load_tpage_buf_lock(0xF, 0x801B0600, 1);
        for (i = 0; i < 6; i++) {
            sp54[i] = i * 0x4000 + 0x80198600;
        }
        i = 0; /* FAKE: dead reassignment keeps the first loop rolled (T-25128) */
        prim_bg_slot_setup(sp54, 1, 6, 3, 2, 0x1FE, -0xA0, -0x7D, 0xC, 0x15);
        D_801220F4 = 0x01000000;
        D_80122106 = 0;
        D_80122105 = 0;
        D_80122104 = 0;
        game_state.screen_brightness = 0;
        /* FAKE: the empty branch ends the uopt block, so j (set to 2 below) does not share a
         * block with 0x01000000, which then takes $a0 as in the original (T-25314) */
        goto split_peel; split_peel: ;
        /* FAKE: explicit peel plus unsigned 2..6 loop, the matched album_playback_ui_restore recipe
         * (T-24706): the unsigned body sums do not CSE with the signed peel literals. T-25128 */
        sp54[0] = 0x80180600; sp54[1] = 0x80184600; for (j = 2; j < 6; j++) { k = j * 0x4000; sp54[j] = 0x80180600 + k; } /* the k temporary gives the shift $v0 and the pointer $v1 (T-25314) */

        prim_bg_slot_setup(sp54, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        /* FAKE: the empty branch keeps the call's $v0 result out of the 0x80 chain's block,
         * so the chain keeps $v0 (T-25314) */
        goto split_call; split_call: ;
        bg_layers = 0x80000000;
        /* FAKE: element views give the chain its hidden VREG, which shares 0x80 in $v0; plain chains put it in $a3 (T-25314) */
        (&D_801220E0)[0] = (&D_801220E1)[0] = (&D_801220E2)[0] = 0x80;
        game_state.screen_brightness = 0;
        memcpy((void *)0x801E2000, 0x80180600, 0x18000);
        /* FAKE: flat view of album_intro_clip as in album_stage_finale; the * 2 index spends the
         * ugen slot the original spends. T-25128 */
        cd_audio_request_set(((s32 *)album_intro_clip)[game_state.current_girl * 2], ((s32 *)album_intro_clip)[game_state.current_girl * 2 + 1], game_state.current_girl);
        audio_command(0x300);
        (*(u8 *)&game_state.substep) += 1;
        break;
    case 6:
        if (cd_audio_state() & 4) {
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 7:
        album_secret_mode_update();
        if ((u8) game_state.screen_brightness < 0x81U) {
            D_80122106 = game_state.screen_brightness;
            D_80122105 = game_state.screen_brightness;
            D_80122104 = game_state.screen_brightness;
            game_state.screen_brightness += 1;
        }
        if (cd_audio_state() & 2) {
            album_phase += 1;
            pad_code_index = 0;
            pad_code_entered = 0;
            pad_code_timer = 0;
            game_state.mode_step_timer.w = 0;
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 8:
        j = game_state.screen_brightness;
        if (j < 0x81) {
            k = j & 0xFF; /* FAKE: the mask in a u32 local keeps the original's andi v0,v1,0xff for the shade stores (T-25314) */
            D_80122106 = k;
            D_80122105 = k;
            D_80122104 = k;
            game_state.screen_brightness = j + 1;
        }
        if (game_state.current_girl == GIRL_SHIORI) {
            if ((u32) game_state.mode_step_timer.w >= 0x1EU) {
                album_voice_play(0);
                if (album_voice_state >= 3) {
                    album_voice_state = 0;
                    (*(u8 *)&game_state.substep) += 1;
                }
            }
        } else if (game_state.current_girl == GIRL_MEGUMI) {
            if ((u32) game_state.mode_step_timer.w >= 0x1EU) {
                album_voice_play(0);
                if (album_voice_state >= 3) {
                    album_voice_state = 0;
                    (*(u8 *)&game_state.substep) += 1;
                }
            }
        } else if (game_state.current_girl == GIRL_REI) {
            if ((u32) game_state.mode_step_timer.w >= 0x1EU) {
                album_voice_play(0);
                if (album_voice_state >= 3) {
                    album_voice_state = 0;
                    (*(u8 *)&game_state.substep) += 1;
                }
            }
        } else {
            (*(u8 *)&game_state.substep) += 1;
        }
        game_state.mode_step_timer.w += 1;
        break;
    case 9:
        j = game_state.screen_brightness & 0xFF; /* FAKE: the mask splits the old value ($v0) from the incremented byte ($v1), as1 folds it into the lbu (T-25314) */
        D_80122106 = j;
        D_80122105 = j;
        D_80122104 = j;
        game_state.screen_brightness = j + 1;
        if (game_state.screen_brightness >= 0x80U) {

            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 10:
        album_clear_buffer_area();
        (*(u8 *)&game_state.substep) += 1;
        break;
    case 11:
        D_801220F4 = 0x01000000;
        *(s32 *)&bg_layers = 0x01000000;
        (*(u8 *)&game_state.substep) += 1;
        break;
    case 12:
        album_model_angle = 0x800;
        album_model_matrix_update();
        D_801220F4 = 0x80000000;
        (*(u8 *)&game_state.substep) += 1;
        break;
    case 13:
        album_secret_mode_update();
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if ((album_secret_mode == 0) || (album_secret_mode == 2)) {
                album_event_skip_other_girls();
            }
            (*(u8 *)&game_state.substep) += 1;
        }
        break;
    case 14:
        if (album_load_state >= 3) {
            if (album_model_angle >= 0xC00) {
                album_load_state = 0;
                mode_stage_next();
            } else {
                goto block_50;
            }
        } else {
block_50:
            if ((album_load_state == -1) && (album_model_angle >= 0xC00)) {
                mode_stage_next();
            } else {
                album_event_advance(1);
                album_model_angle += 8;
                album_model_matrix_update();
            }
        }
        break;
    }
}

void album_entry(void) {
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0:
        album_stage_load();
        break;
    case 1:
        album_stage_playback();
        break;
    case 2:
        album_stage_finale();
        break;
    case 0xFF:
        album_stage_save();
        break;
    }
    message_window_title_update();
    if (game_state.mode_stage != 0xFF) {
        if (album_skip_requested == 0 && (game_state.pad_pressed & PAD_START)) {
            album_clear_buffer_area();
            album_model_angle = 0x800;
            album_model_matrix_update();
            album_skip_requested = 1;
            mode_stage_set(2);
        }
    }
}
void album_tpage_slots_reset(void) {
    game_state.tpage_buf[10] = 0;
    game_state.tpage_buf[11] = 0;
    game_state.tpage_buf[12] = 0;
    game_state.tpage_buf[13] = 0;
    game_state.tpage_buf[14] = 0;
    game_state.tpage_buf[42] = 0;
    game_state.tpage_buf[43] = 0;
    game_state.tpage_buf[44] = 0;
    game_state.tpage_buf[45] = 0;
    game_state.tpage_buf[46] = 0;
    game_state.tpage_buf[0] = 0;
    game_state.tpage_buf[1] = 0;
    game_state.tpage_buf[2] = 0;
    game_state.tpage_buf[3] = 0;
    game_state.tpage_buf[4] = 0;
    game_state.tpage_buf[32] = 0;
    game_state.tpage_buf[33] = 0;
    game_state.tpage_buf[34] = 0;
    game_state.tpage_buf[35] = 0;
    game_state.tpage_buf[36] = 0;
    game_state.tpage_buf[16] = 0;
    game_state.tpage_buf[17] = 0;
    game_state.tpage_buf[18] = 0;
    game_state.tpage_buf[19] = 0;
    game_state.tpage_buf[20] = 0;
    game_state.tpage_buf[48] = 0;
    game_state.tpage_buf[49] = 0;
    game_state.tpage_buf[50] = 0;
    game_state.tpage_buf[51] = 0;
    game_state.tpage_buf[52] = 0;
    game_state.tpage_buf[5] = -1;
    game_state.tpage_buf[6] = -1;
    game_state.tpage_buf[7] = -1;
    game_state.tpage_buf[8] = -1;
    game_state.tpage_buf[9] = -1;
    game_state.tpage_buf[37] = -1;
    game_state.tpage_buf[38] = -1;
    game_state.tpage_buf[39] = -1;
    game_state.tpage_buf[40] = -1;
    game_state.tpage_buf[41] = -1;
    game_state.tpage_buf[21] = -1;
    game_state.tpage_buf[22] = -1;
    game_state.tpage_buf[23] = -1;
    game_state.tpage_buf[24] = -1;
    game_state.tpage_buf[25] = -1;
    game_state.tpage_buf[53] = -1;
    game_state.tpage_buf[54] = -1;
    game_state.tpage_buf[55] = -1;
    game_state.tpage_buf[56] = -1;
    game_state.tpage_buf[57] = -1;
    if ((album_event_is_special_portrait() == 1) && (album_event_weekday == 0xFF)) {
        game_state.tpage_buf[7] = 0;
    }
}

void album_secret_mode_update(void) {
    album_cleared_level();
    album_event_flag_count_check();
    /* FAKE: do-while block puts album_phase in $v0 instead of $a0; real source unknown. T-24207 */
    do { album_all_met_check(); } while (0);
    if (album_phase == 0) {
        /* FAKE: `!= 0` costs one ugen temporary and no code, so the rotor starts at $t7. T-24207 */
        if (((misc_all_girls_cleared() == 1) != 0) && (game_state.pad_held & 0x09000000)) {
            if (album_secret_mode != 1) {
                audio_command(0x50E);
            }
            album_secret_mode = 1;
        }
    } else if (album_phase == 1) {
        if (pad_code_entered == 1) {
            audio_command(0x50E);
            if (album_secret_mode == 1) {
                album_secret_mode = 3;
            } else {
                album_secret_mode = 2;
            }
        }
    }
}

s32 album_cleared_level(void) {
    s32 r;
    r = 0;
    if (game_state.options_dirty == 0) {
        if (game_state.girl_cleared[0] >= 2U && game_state.girl_cleared[1] >= 2U && game_state.girl_cleared[2] >= 2U && game_state.girl_cleared[3] >= 2U && game_state.girl_cleared[4] >= 2U && game_state.girl_cleared[5] >= 2U && game_state.girl_cleared[6] >= 2U && game_state.girl_cleared[7] >= 2U && game_state.girl_cleared[8] >= 2U && game_state.girl_cleared[9] >= 2U && game_state.girl_cleared[10] >= 2U && game_state.girl_cleared[11] >= 2U && game_state.girl_cleared[12] >= 2U) {
            return 2;
        }
        if (game_state.girl_cleared[0] != 0 && game_state.girl_cleared[1] != 0 && game_state.girl_cleared[2] != 0 && game_state.girl_cleared[3] != 0 && game_state.girl_cleared[4] != 0 && game_state.girl_cleared[5] != 0 && game_state.girl_cleared[6] != 0 && game_state.girl_cleared[7] != 0 && game_state.girl_cleared[8] != 0 && game_state.girl_cleared[9] != 0 && game_state.girl_cleared[10] != 0 && game_state.girl_cleared[11] != 0 && game_state.girl_cleared[12] != 0) {
            return 1;
        }
        return 0;
    }
    return r;
}

s32 album_event_flag_count_check(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 83; i++) {
        if (game_state.scene_flags[i] != 0) {
            n++;
        }
    }
    if (n >= 30) {
        return 1;
    }
    return 0;
}

s32 album_all_met_check(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 13; i++) {
        if (game_state.girls[i].flags.f.met) {
            n++;
        }
    }
    if (game_state.girls[11].flags.f.story_stage != 0) {
        n++;
    }
    if (n == 13) {
        return 1;
    }
    return 0;
}
