/* asset_loading_and_saved_settings.c: src/ovl/TACO/80133870.c:5 dispatches load steps; :32 and :101 request game asset blocks; :377 restores button settings, scores and three-character ranking names from SaveData. Original object 0x80133870-0x80134930 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

/* s32 with no return (the caller uses it as implicit int): v0 is live at the exit, so as1 cannot fill the delay slot of the branch before the last call with lui v0 and takes the move a1,zero instead. T-16562 */
s32 shooter_stage_load(void) {
    game_state.mode_step_timer.u += 1;
    switch (game_state.mode_step) {
    case 0:
        shooter_load_step_boot();
        break;
    case 1:
        shooter_load_step_sound_banks();
        break;
    case 2:
        shooter_load_step_textures();
        break;
    case 3:
        shooter_load_step_models();
        break;
    case 4:
        shooter_load_step_finish();
        break;
    }
    if (game_state.mode_step_timer.u & 0x10) {
        shooter_draw_text((s32) "NOW LOADING DATA ", -0x60, 0, 1);
    }
    if ((game_state.mode_step_timer.u != 0) && ((u32) game_state.mode_step_timer.u < 0x400U)) {
        shooter_draw_number(shooter_time_left--, 0, 0x32, 6);
    }
}

s32 shooter_load_step_boot(void) {
    switch (game_state.substep) {
    case 0:
        audio_command(0x24);
        shooter_time_left = 0x400;
        draw2d3d(1, 0);
        game_state.clear_color_r = 0;
        game_state.clear_color_g = 0;
        game_state.clear_color_b = 0;
        sprite_tables_clear();
        shooter_init_background_scroll();
        back_clear_switch(1);
        hud_cursor_show(0);
        text_system_reset();
        actor_clear_range(0);
        shooter_release_all_actor_models();
        shooter_loader_flag = 0;
        cd_read_sectors(0x21, 0x80180000, 0xB4F3);
        cd_read_sectors(1, 0x801FA800, 0xB514);
        vram_clear_blocks(0xFF);
        game_state.substep += 1;
        return;
    case 1:
    case 2:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
            return;
        }
        return;
    case 3:
        game_state.ovl_palette_a = 0x8018A000;
        game_state.ovl_frame_table = 0x801FAA7C;
        game_state.ovl_sequence_table = 0x801FAA88;
        game_state.ovl_tpage_table = 0x801FAA68;
        shooter_init_texture_pages();
        mode_step_next();
        break;
    }
}

void shooter_load_step_sound_banks(void) {
    switch (game_state.substep) {
    case 0:
        audio_bank_load_start(0, 0xC56D, 0xC54D, 0xD310, 0xD2C2, 0xD29D);
        game_state.substep += 1;
        return;
    case 1:
        if (audio_bank_transfer_poll() == 1) {
            game_state.substep += 1;
            return;
        }
    default:
        return;
    case 2:
        audio_bank_load_start(1, 0xC54B, 0xC519, 0xC7BB, 0xC777, 0xC766);
        game_state.substep += 1;
        return;
    case 3:
        if (audio_bank_transfer_poll() == 1) {
            game_state.substep += 1;
            return;
        }
        break;
    case 4:
        mode_step_next();
        break;
    }
}

void shooter_load_step_textures(void) {
    s32 temp_a1;
    s32 var_s0;

    switch (game_state.substep) {
    case 0:
        cd_read_sectors_nowait(5U, 0x801B8000, 0xB473);
        cd_read_sectors_nowait(3U, 0x801C0000, 0xB4BC);
        cd_read_sectors_nowait(0x21U, 0x801C8000, 0xB57F);
        game_state.substep += 1;
        return;
    case 1:
    case 2:
    case 3:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
            return;
        }
    default:
        return;
    case 4:
        shooter_upload_texture_banks();
        game_state.substep += 1;
        return;
    case 5:
        cd_read_sectors_nowait(0x20U, 0x80180000, 0xB489);
        cd_read_sectors_nowait(0x20U, 0x80190000, 0xB4D3);
        cd_read_sectors_nowait(8U, 0x80170000, 0xB47E);
        game_state.substep += 1;
        return;
    case 6:
    case 7:
    case 8:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
            return;
        }
        break;
    case 9:
        shooter_load_texture_page(0x80180000, 0x18);
        shooter_load_texture_page(0x80190000, 0x1A);
        shooter_load_texture_page(0x80170000, 0xA);
        game_state.substep += 1;
        return;
    case 10:
        cd_read_sectors_nowait(0x14U, 0x80180000, 0xB4BF);
        game_state.substep += 1;
        return;
    case 11:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
            return;
        }
        break;
    case 12:
        LoadSquare(0x380U, 0x100U, 0x20U, 0x80U, 0x80180000);
        LoadSquare(0x3A0U, 0x100U, 0x20U, 0x80U, 0x80182000);
        LoadSquare(0x380U, 0x180U, 0x20U, 0x80U, 0x80184000);
        LoadSquare(0x3A0U, 0x180U, 0x20U, 0x80U, 0x80186000);
        var_s0 = 0;
        do {
            temp_a1 = var_s0 & 0xFF;
            load_palette(0x80188000, temp_a1, 1, 3, temp_a1);
            var_s0 += 1;
        } while (var_s0 != 0xF);
        mode_step_next();
        break;
    }
}

/* FAKE: retain s32 fallthrough view and word stores into unk_1095[3]/[7]
 * (+0x1098/+0x109C); two consecutive increments in case 0 match the folded
 * addiu v0,t7,2 reload sequence. T-24129. */
s32 shooter_load_step_models(void) {
    switch (game_state.substep) {
    case 0:
        cd_read_sectors_nowait(1, 0x8018A000, 0xB3F2);
        cd_read_sectors_nowait(5, 0x8018A800, 0xB413);
        cd_read_sectors_nowait(0x12, 0x8018D000, 0xB438);
        game_state.substep += 1;
        game_state.substep += 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 5:
        cd_read_sectors_nowait(7, 0x80196000, 0xB45B);
        game_state.substep += 1;
        cd_read_sectors_nowait(3, 0x80199800, 0xB478);
        cd_read_sectors_nowait(2, 0x8019B000, 0xB47B);
        game_state.substep += 1;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 10:
        cd_read_sectors_nowait(1, 0x8019C000, 0xB486);
        cd_read_sectors_nowait(2, 0x8019C800, 0xB487);
        cd_read_sectors_nowait(4, 0x8019D800, 0xB4A9);
        cd_read_sectors_nowait(6, 0x8019F800, 0xB4AD);
        game_state.substep += 1;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 15:
        cd_read_sectors_nowait(2, 0x801A2800, 0xB4B3);
        cd_read_sectors_nowait(4, 0x801A3800, 0xB4B5);
        cd_read_sectors_nowait(3, 0x801A5800, 0xB4B9);
        cd_read_sectors_nowait(0xA, 0x801A7000, 0xB515);
        game_state.substep += 1;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 20:
        cd_read_sectors_nowait(1, 0x801AC000, 0xB534);
        cd_read_sectors_nowait(3, 0x801AC800, 0xB668);
        cd_read_sectors_nowait(3, 0x801AE000, 0xB66B);
        cd_read_sectors_nowait(3, 0x801AF800, 0xB66E);
        game_state.substep += 1;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 25:
        cd_read_sectors_nowait(2, 0x801B1000, 0xB671);
        cd_read_sectors_nowait(3, 0x801B2000, 0xB673);
        cd_read_sectors_nowait(3, 0x801B3800, 0xB676);
        cd_read_sectors_nowait(2, 0x801B5000, 0xB679);
        game_state.substep += 1;
        break;
    case 26:
    case 27:
    case 28:
    case 29:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
        }
        break;
    case 30:
        shooter_actors = (ShooterActor *) D_8011ED58;
        *(s32 *)&game_state.menu_hit[3] = 1;
        *(s32 *)&game_state.menu_hit[7] = 2;
        mode_step_next();
        break;
    }
}

/* implicit-int function (called before its definition): s32 with no return. The loop re-reads and re-writes one word (buf[0x80]) 16 times, as the asm does. */
s32 shooter_init_texture_pages(void) {
    u32 *buf;
    s32 i;

    tpage_buf_clear_all();
    LoadSquare(0x360, 0x180, 0x20, 0x80, (void *)0x80180000);
    load_tpage_buf_lock(0x3F, 0x80180000, 2);
    load_tpage_buf_lock(0x3D, 0x80182000, 2);
    load_tpage_buf_lock(0x1D, 0x80184000, 2);
    load_tpage_buf_lock(0x1C, 0x80186000, 2);
    load_tpage_buf_lock(0x3C, 0x80188000, 2);
    load_palette(0x8018A000, 0xF, 1, 1, 0);
    load_palette(0x8018A000, 0, 1, 1, 0);
    LoadSquare(0x360, 0x100, 0x20, 0x80, (void *)0x8018A200);
    load_palette(0x8018A000, 1, 1, 3, 0);
    load_tpage_buf_lock(0x1F, 0x8018C200, 1);
    buf = (u32 *)0x80190200;
    for (i = 0; i < 0x10; i++) {
        buf[0x80] &= 0x7FFF0000;
    }
    LoadSquare(0x100, 0x1F8, 0x100, 8, buf);
    func_8009C674(0);
}

void shooter_upload_texture_banks(void) {
    LoadSquare(0x100, 0x1E0, 0x100, 0x18, 0x801B8000);
    LoadSquare(0, 0x1F0, 0x100, 0x10, 0x801C0000);
    shooter_load_texture_page(0x801C8000, 8);
    shooter_load_texture_page(0x801C8000, 0xA);
    shooter_load_texture_page(0x801C8000, 0xC);
    shooter_load_texture_page(0x801C8000, 0xE);
    shooter_load_texture_page(0x801C8000, 0x18);
    shooter_load_texture_page(0x801C8000, 0x1A);
    func_8009C674(0);
}

s32 shooter_load_step_finish(void) {
    switch (game_state.substep) {
    case 0:
        game_state.clear_color_g = 0;
        game_state.clear_color_b = 0;
        game_state.tile_layer_enabled = 0;
        game_state.screen_brightness = 0x80;
        game_state.clear_color_r = 0;
        draw2d3d(1, 1);
        initView();
        shooter_set_white_lights();
        initCoordinate();
        shooter_load_settings_and_scores();
        initModelingData_init(0x8018A000);
        shooter_init_coordinate_objects();
        shooter_link_model_handles((s32 *)0x801AC000);
        game_state.substep += 1;
        break;
    case 1:
        mode_stage_set(1);
        break;
    }
}

/* File-local view of the settings block at system_data (0x800E7D50 = +0x40), T-16562.
   The flag test reads the word at +0x40 through this view too: uopt then sees the byte
   read-modify-write of the else path overlap a word access and keeps &system_data in $v0
   (with D_800E7D50 in the test it makes the byte a plain global with lui/%lo addresses). */
typedef struct ShooterSettingsWordView {
    u32 pad0 : 2;
    u32 mode : 1;
    u32 sel : 2;
} ShooterSettingsWordView;
typedef struct ShooterSettingsHalfView {
    u16 pad0 : 12;
    u16 hi : 4;
} ShooterSettingsHalfView;
typedef struct ShooterSettingsByteView {
    u8 b0;
    u8 lo : 4;
} ShooterSettingsByteView;
typedef struct ShooterSettingsFlagView {
    u8 flag : 1;
} ShooterSettingsFlagView;
typedef struct ShooterSettingsBlock {
    u8 pad00[0x40];
    union {         /* 0x40: one view per load width of the original */
        s32 word;
        ShooterSettingsWordView w;
        ShooterSettingsHalfView h;
        ShooterSettingsByteView b;
        ShooterSettingsFlagView f;
        struct {
            u8 pad[2];
            u8 name0[3];    /* 0x42 */
            u8 name1[3];    /* 0x45 */
            u8 name2[3];    /* 0x48 */
            u8 pad4B;
            s32 ranking_score_1;      /* 0x4C */
            s32 ranking_score_2;      /* 0x50 */
            s32 ranking_score_3;      /* 0x54 */
        } s;
    } set;
} ShooterSettingsBlock;

void shooter_load_settings_and_scores(void) {
    ShooterSettingsBlock *save;

    save = (ShooterSettingsBlock *)&system_data;
    if (save->set.word & 1) {
        shooter_high_score_1 = save->set.s.ranking_score_1;
        shooter_high_score_2 = save->set.s.ranking_score_2;
        shooter_high_score_3 = save->set.s.ranking_score_3;
        shooter_setting_auto_shot = save->set.w.mode;
        shooter_setting_roll_button = save->set.w.sel;
        shooter_setting_shot_button = save->set.h.hi;
        shooter_setting_bomb_button = save->set.b.lo;
        strncpy(shooter_high_score_name_1, save->set.s.name0, 3);
        strncpy(shooter_high_score_name_2, save->set.s.name1, 3);
        strncpy(shooter_high_score_name_3, save->set.s.name2, 3);
        return;
    }
    save->set.f.flag = 1;
    shooter_high_score_1 = 0x2EA694;
    shooter_high_score_2 = 0x1F6454;
    shooter_high_score_3 = 0x102214;
    strcpy(shooter_high_score_name_1, "K.K");
    strcpy(shooter_high_score_name_2, "K.T");
    strcpy(shooter_high_score_name_3, "N.S");
}

void shooter_init_coordinate_objects(void) {
    ShooterCoordinate *p;

    /* FAKE: init and loop head on one source line give the original's addiu order (as1 schedules by line, T-7020) */
    p = D_80127480; do {
        func_80099E30(0, p);
        p->z = -0xFA0;
        *(s32 *)p = 0;
        p++;
    } while (p != (ShooterCoordinate *)D_80128880);
}

void shooter_hide_all_models(void) {
    s32 i;

    view_model_slots_clear();
    for (i = 0; i < 0x40; i++) {
        D_80127080[i].attribute = 0x80000000;
    }
}

void shooter_link_model_handles(s32 *arg0) {
    s32 *dop;
    s32 i;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    dop++;
    for (i = 0; i < 0x40; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i], 0);
        D_80127080[i].links[0] = (s32)&D_80127480[i];
        D_80127080[i].attribute = 0x80000000;
    }
}

void shooter_clear_particle_tables(void) {
    bzero((u8 *) shooter_player_primary_shots, 0x140);
    bzero((u8 *) shooter_wingman_primary_shots, 0x140);
    bzero((u8 *) shooter_player_secondary_shots, 0x140);
    bzero((u8 *) shooter_wingman_secondary_shots, 0x140);
    bzero((u8 *) shooter_bomb_shots, 0x140);
    bzero((u8 *) shooter_hit_sparks, 0x140);
    bzero((u8 *) shooter_explosions, 0x140);
    bzero((u8 *) shooter_enemy_bullets, 0x140);
    bzero((u8 *) shooter_items, 0x140);
}
