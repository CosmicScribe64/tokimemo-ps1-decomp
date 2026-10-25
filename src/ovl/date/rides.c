/* rides.c: src/ovl/DATE/801461B0.c:20: amusement park rides: roller coaster, ghost house, Ferris wheel, scream machine (virtual ship is separate). Original object 0x801461B0-0x80149310 (config/objects/DATE.txt). */
#include "common.h"
#include "ovl/date.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF80);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF84);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF88);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF8C);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF90);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF94);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CF98);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", date_ride_frame);
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CFA0);
s16 D_8015CFA4[3] = { 0, 0, 0 };
s16 D_8015CFAC[3] = { 0, 0, 0 };
s16 D_8015CFB4[3] = { 0, 0, 0 };
s16 D_8015CFBC[3] = { 0, 0, 0 };
INCLUDE_RODATA("asm/ovl/DATE/data/date/rides.data", D_8015CFC4);

void date_ferris_wheel_resources_init(void) {
    D_8015CF80 = 0x801C6000;
    D_8015CF84 = 0x801AE000;
    D_8015CF88 = 0x801B2000;
    D_8015CF8C = 0x801B6000;
    D_8015CF90 = 0x801BA000;
    D_8015CF94 = 0x801BE000;
    D_8015CF98 = 0x801C2000;
}

void date_ride_dispatch(void) {
    /* T-10040: this dispatcher reads the signed-declared byte with lbu. */
    switch (*(u8 *)&date_ride_index) {
    case 0:
        date_ride_roller_coaster();
        return;
    case 1:
        date_ride_ghost_house();
        return;
    case 2:
        date_ride_ferris_wheel();
        return;
    case 3:
        date_ride_scream_machine();
        return;
    case 4:
        date_virtual_ship_dispatch();
        /* fallthrough */
    default:
        return;
    }
}

u16 date_ride_scream_machine(void) {
    GameState *game;
    s16 i;
    s16 v;
    u8 x;
    RECT rect;
    s32 saved_mode_step;
    u32 saved_step_timer;
    SpriteQuad *p;
    DateMovieWork *movie;
    u8 value;
    u32 color;
    u32 timer;

    game = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (game->mode_step_timer.u) {
    case 0:
        D_80122D2C = 1;
        srand(game_state.frame_count);
        audio_command(0x206);
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        D_8015CFA0 = 2;
        game_state.substep = 0;
        date_ride_frame = 0; for (i = 0; i < 2; i++) { /* FAKE: one source line keeps the as1 store order (as in func_801348F0) T-24018 */
            sprite_quads[i + 4].w = (i << 6) + 0x80;
            sprite_quads[i + 4].flags = 0x82000000;
            sprite_quads[i + 4].h = 0;
            sprite_quads[i + 4].unk_0E = 0;
            sprite_quads[i + 4].unk_0F = 0xA0;
            sprite_quads[i + 4].unk_10 = 0;
            sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            sprite_quads[i + 4].unk_12 = 0x1E0;
            sprite_quads[i + 4].tint_r = 0;
            sprite_quads[i + 4].tint_g = 0;
            sprite_quads[i + 4].tint_b = 0;
            sprite_quads[i + 4].anchor_x = 0;
            sprite_quads[i + 4].anchor_y = 0;
            sprite_quads[i + 4].unk_1C = 0x1000;
            sprite_quads[i + 4].unk_1E = 0x1000;
            sprite_quads[i + 4].unk_20 = 0;
            sprite_quads[i + 4].x = (i << 7) - 0xA0;
            sprite_quads[i + 4].y = -0x78;
            game_state.quad_layer[i + 4] = 0xB;
        }
        for (i = 0; i < 2; i++) {
            sprite_quads[i + 6].w = (i << 6) + 0x80;
            sprite_quads[i + 6].flags = 0x82000000;
            sprite_quads[i + 6].h = 0xA0;
            sprite_quads[i + 6].unk_0E = 0;
            sprite_quads[i + 6].unk_0F = 0;
            sprite_quads[i + 6].unk_10 = 0;
            sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            sprite_quads[i + 6].unk_12 = 0x1E0;
            sprite_quads[i + 6].tint_r = 0;
            sprite_quads[i + 6].tint_g = 0;
            sprite_quads[i + 6].tint_b = 0;
            sprite_quads[i + 6].anchor_x = 0;
            sprite_quads[i + 6].anchor_y = 0;
            sprite_quads[i + 6].unk_1C = 0x1000;
            sprite_quads[i + 6].unk_1E = 0x1000;
            sprite_quads[i + 6].unk_20 = 0;
            sprite_quads[i + 6].x = (i << 7) - 0xA0;
            sprite_quads[i + 6].y = -0x78;
            game_state.quad_layer[i + 6] = 0xB;
        }
        dec_bg_cd_read(0x43CB, 1);
        game->mode_step_timer.u += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x80183000);
            game->mode_step_timer.u += 1;
        }
        break;
    case 2:
        dec_bg_cd_read(0x43D4, 1);
        game->mode_step_timer.u += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801A1000);
            game->mode_step_timer.u += 1;
        }
        break;
    case 4:
        dec_bg_cd_read(0x43DE, 1);
        game->mode_step_timer.u += 1;
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x140, 0x100, 0x180, 0xA0);
            rect.x = 0x140;
            rect.y = 0x100;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801BF000);
            game->mode_step_timer.u += 1;
        }
        break;
    case 6:
        dec_bg_cd_read(0x43E7, 1);
        game->mode_step_timer.u += 1;
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x280, 0, 0x180, 0xA0);
            rect.x = 0x280;
            rect.y = 0;
            rect.w = 0x180;
            rect.h = 0xA0;
            func_8009C8E0(&rect, (void *)0x801DD000);
            game->mode_step_timer.u += 1;
        }
        break;
    case 8:
        for (i = 0; i < 4; i++) {
            sprite_quads[i + 4].flags = 0x02000000;
        }
        game->mode_step_timer.u += 1;
        audio_command(0x500);
        break;
    case 9:
        value = sprite_quads[4].tint_r;
        value += 2;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        if (value == 0x80) {
            game->mode_step_timer.u += 1;
        }
        break;
    case 10:
        break;
    case 11:
        value = sprite_quads[4].tint_r;
        value -= 2;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        if (value == 0) {
            for (i = 0; i < 4; i++) {
                sprite_quads[i + 4].flags = 0x82000000;
            }
            game->mode_step_timer.u += 1;
        }
        break;
    case 12:
        saved_mode_step = game_state.mode_step;
        saved_step_timer = game->mode_step_timer.u;
        read_bustup();
        /* FAKE: one source line preserves as1 callback load scheduling. T-24089. */
        game->mode_step_timer.u = saved_step_timer; if (saved_mode_step != game_state.mode_step) {
            game_state.mode_step--;
            game->mode_step_timer.u += 1;
        }
        break;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            game->mode_step_timer.u += 1;
        }
        break;
    case 14:
        saved_step_timer = game->mode_step_timer.u;
        saved_mode_step = game_state.mode_step;
        portrait_show_checked();
        if (game_state.mode_step != saved_mode_step + 1) {
            game_state.mode_step = saved_mode_step;
            game->mode_step_timer.u = 12;
            break;
        }
        game_state.mode_step--;
        game->mode_step_timer.u = saved_step_timer + 1;
        break;
    case 15:
        cd_read_sectors_nowait(0xA, 0x80162000, 0x4359);
        game->mode_step_timer.u += 1;
        break;
    case 17:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            game->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        game->mode_step_timer.u += 1;
        break;
    case 18:
        audio_command(0x200);
        saved_step_timer = game->mode_step_timer.u;
        date_load_girl_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer + 1;
        break;
    case 20:
        saved_step_timer = game->mode_step_timer.u;
        date_load_shared_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer + 1;
        break;
    case 22:
        saved_step_timer = game->mode_step_timer.u;
        date_load_choice_tables();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer + 1;
        break;
    default:
        hizuke_disp_switch(1);
        D_80122D2C = 0;
        mode_step_next();
        break;
    }
    if (game->mode_step_timer.u < 9 || game->mode_step_timer.u >= 12) {
        return 0;
    }
    p = sprite_quads;
    v = p[4].unk_0E;
    v += D_8015CFA0;
    if (v < 0) {
        v = 0;
        D_8015CFA0 = (rand() % 4) + 2;
    }
    if (v >= 0x40) {
        v = 0x3F;
        D_8015CFA0 = -2 - (rand() % 4);
    }
    x = p[4].unk_0F;
    p[7].unk_0E = v;
    p[6].unk_0E = v;
    p[5].unk_0E = v;
    p[4].unk_0E = v;
    if (x != 0) {
        p[4].h += 0x14;
        p[5].h += 0x14;
        x -= 0x14;
        p[5].unk_0F -= 0x14;
    } else {
        p[4].h -= 0x14;
        p[4].y += 0x14;
        p[5].h -= 0x14;
        p[5].y += 0x14;
    }
    if (p[6].unk_0F != 0) {
        p[6].h += 0x14;
        p[7].h += 0x14;
        p[6].unk_0F -= 0x14;
        p[7].unk_0F -= 0x14;
    } else {
        p[6].h -= 0x14;
        p[6].y += 0x14;
        p[7].h -= 0x14;
        p[7].y += 0x14;
    }
    p[4].unk_0F = x;
    if ((x == 0 && (game->substep == 1 || game->substep == 3)) || (p[6].unk_0F == 0 && (game->substep == 0 || game->substep == 2))) {
        switch (game_state.substep) { /* FAKE: game-> in the test, the symbol here; one access path keeps the byte in $v0 and drops the reload T-24018 */
        case 0:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 4].h = 0;
                sprite_quads[i + 4].unk_0F = 0xA0;
                sprite_quads[i + 4].y = -0x78;
                sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            }
            LoadSquare(0x140, 0x100, 0x180, 0xA0, 0x80183000);
            break;
        case 1:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 6].h = 0;
                sprite_quads[i + 6].unk_0F = 0xA0;
                sprite_quads[i + 6].y = -0x78;
                sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            }
            LoadSquare(0x280, 0, 0x180, 0xA0, 0x801A1000);
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 4].h = 0;
                sprite_quads[i + 4].unk_0F = 0xA0;
                sprite_quads[i + 4].y = -0x78;
                sprite_quads[i + 4].unk_0C = (i * 2) + 0x15;
            }
            LoadSquare(0x140, 0x100, 0x180, 0xA0, 0x801BF000);
            break;
        case 3:
            for (i = 0; i < 2; i++) {
                sprite_quads[i + 6].h = 0;
                sprite_quads[i + 6].unk_0F = 0xA0;
                sprite_quads[i + 6].y = -0x78;
                sprite_quads[i + 6].unk_0C = (i * 2) + 0xA;
            }
            LoadSquare(0x280, 0, 0x180, 0xA0, 0x801DD000);
            break;
        }
        if (++game_state.substep >= 4) {
            game_state.substep = 0;
        }
    }
    timer = (date_ride_frame + 1) & 0xFFFF;
    date_ride_frame = timer;
    if (timer == 0x12C) {
        game->mode_step_timer.u += 1;
    }
}

s32 date_ride_ferris_wheel(void) {
    /* FAKE: unused RECT reserves the observed frame words. T-17201. */
    RECT rect;
    GameState *game;
    u32 saved_step_timer;
    s16 var_v1;
    u8 x;
    /* FAKE: second fade copy holds v1 in cases 6/7/9; dead stores in the tail block cvt_pass widening so uopt promotes date_ride_frame to t0. T-24028, T-24089. */
    u32 y;
    DateMovieWork *movie;

    game = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (game->mode_step_timer.u) {
    case 0:
        audio_command(0x201);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        /* FAKE: one source line keeps the as1 store order of the original. T-17201. */
        date_ride_frame = 0; for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 10].w = (var_v1 << 6) + 0x80;
            sprite_quads[var_v1 + 10].flags = 0x82000000;
            sprite_quads[var_v1 + 10].h = 0xa0;
            sprite_quads[var_v1 + 10].unk_0E = 0;
            sprite_quads[var_v1 + 10].unk_0F = 0;
            sprite_quads[var_v1 + 10].unk_10 = 0;
            sprite_quads[var_v1 + 10].unk_0C = (var_v1 * 2) + 0x15;
            sprite_quads[var_v1 + 10].unk_12 = 0x1E0;
            sprite_quads[var_v1 + 10].tint_r = 0;
            sprite_quads[var_v1 + 10].tint_g = 0;
            sprite_quads[var_v1 + 10].tint_b = 0;
            sprite_quads[var_v1 + 10].anchor_x = 0;
            sprite_quads[var_v1 + 10].anchor_y = 0;
            sprite_quads[var_v1 + 10].unk_1C = 0x1000;
            sprite_quads[var_v1 + 10].unk_1E = 0x1000;
            sprite_quads[var_v1 + 10].unk_20 = 0;
            sprite_quads[var_v1 + 10].x = (var_v1 << 7) - 0xA0;
            sprite_quads[var_v1 + 10].y = -0x96;
            game_state.quad_layer[var_v1 + 10] = 0xC;
        }
        dec_bg_cd_read(0x43C2, 1);
        game->mode_step_timer.u += 1;
        return 0;
    case 1:
        if (cd_read_sync_status() & 1) {
            bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
            game->mode_step_timer.u += 1;
        }
        return 0;
    case 2:
        cd_read_sectors(0x31, 0x801AE000, 0xBCD1);
        date_ferris_wheel_resources_init();
        game->mode_step_timer.u += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (cd_read_sync_status() & 1) {
            game->mode_step_timer.u += 1;
        }
        return 0;
    case 4:
        load_palette(D_8015CF80, 0x11, 1, 0, 0);
        date_ferris_wheel_still_copy(D_8015CF84, D_8015CF88, D_8015CF8C, D_8015CF90, D_8015CF94, D_8015CF98);
        game->mode_step_timer.u += 1;
        return 0;
    case 5:
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            sprite_quads[var_v1 + 4].flags &= 0x7FFFFFFF;
        }
        game->mode_step_timer.u += 1;
        return 0;
    case 6:
        x = (sprite_quads[4].tint_r += 1); y = sprite_quads[4].tint_g = x; sprite_quads[4].tint_b = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            sprite_quads[var_v1 + 4].tint_r = sprite_quads[var_v1 + 4].tint_g = sprite_quads[var_v1 + 4].tint_b = sprite_quads[4].tint_r;
        }
        if (sprite_quads[4].tint_r == 0x40) {
            game->mode_step_timer.u += 1;
        }
        return 0;
    case 7:
        x = (sprite_quads[4].tint_r += 1); y = sprite_quads[4].tint_g = x; sprite_quads[4].tint_b = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            sprite_quads[var_v1 + 4].tint_r = sprite_quads[var_v1 + 4].tint_g = sprite_quads[var_v1 + 4].tint_b = sprite_quads[4].tint_r;
        }
        if (sprite_quads[4].tint_r == 0x80) {
            game->mode_step_timer.u += 1;
        }
        break;
    case 8:
        break;
    case 9:
        x = (sprite_quads[4].tint_r -= 2); y = sprite_quads[4].tint_g = x; sprite_quads[4].tint_b = y; /* FAKE: one source line preserves store order. T-24028. */
        for (var_v1 = 1; var_v1 < 8; var_v1++) {
            sprite_quads[var_v1 + 4].tint_r = sprite_quads[var_v1 + 4].tint_g = sprite_quads[var_v1 + 4].tint_b = sprite_quads[4].tint_r;
        }
        if (sprite_quads[4].tint_r == 0) {
            for (var_v1 = 0; var_v1 < 8; var_v1++) {
                sprite_quads[var_v1 + 4].flags |= 0x80000000;
            }
            game->mode_step_timer.u += 1;
        }
        break;
    case 10:
        cd_read_sectors_nowait(0xA, 0x80162000, 0x4350);
        game->mode_step_timer.u += 1;
        return 0;
    case 12:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            game->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        game->mode_step_timer.u += 1;
        return 0;
    case 13:
        saved_step_timer = game->mode_step_timer.u;

        date_load_girl_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return 0;
    case 15:
        saved_step_timer = game->mode_step_timer.u;

        date_load_shared_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return 0;
    case 17:
        saved_step_timer = game->mode_step_timer.u;

        date_load_choice_tables();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return 0;
    default:
        mode_step_next();
        D_80122D2C = 0;
        audio_command(0x200);
        hizuke_disp_switch(1);
        return 0;
    }
    date_ride_frame += (y = 1);
    if (date_ride_frame % (y = 19U) == 0x12) {
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 10].y += 1;
        }
        sprite_quads[var_v1 + 10].h -= 1;
    }
    if (date_ride_frame % (y = 5U) == 4) {
        for (var_v1 = 0; var_v1 < 6; var_v1++) {
            sprite_quads[var_v1 + 4].y += 1;
        }
        for (var_v1 = 3; var_v1 < 6; var_v1++) {
            if (sprite_quads[var_v1 + 4].h != 0) {
                sprite_quads[var_v1 + 4].h -= 1;
            } else {
                sprite_quads[var_v1 + 1].h -= 1;
            }
        }
    }
    if (date_ride_frame == (y = 0x12C)) {
        game->mode_step_timer.u += 1;
    }
}

/* FAKE: s32 with normal bare returns preserves the original v0 liveness;
 * only the decoder-error path returns zero. Real return type unknown. T-12100. */
s32 date_ride_roller_coaster(void) {
    GameState *game;
    s32 current;
    s32 saved_mode_step;
    u32 saved_step_timer;
    RECT rect;
    DateMovieWork *movie;
    /* FAKE: unused view reserves the observed frame slot. T-12100. */
    DateMovieHeader8 *header;
    u8 value;
    u32 color;
    s16 var_v1;

    game = &game_state;
    movie = (DateMovieWork *)bg_lba_cache;
    cd_read_checked = 0;
    switch (game->mode_step_timer.u) {
    case 0:
        audio_command(0x7F);
        D_80122D2C = 1;
        movie->unk_06 = 0;
        movie->unk_02 = 0;
        movie->unk_04 = 0;
        movie->unk_00 = 0;
        movie->unk_19 = 0;
        movie->unk_18 = 0;
        movie->unk_28 = -1;
        movie->unk_0A = 0;
        movie->unk_1A = 0;
        hizuke_disp_switch(0);
        date_ride_frame = 0;
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].w = (var_v1 << 6) + 0x80;
            sprite_quads[var_v1 + 4].flags = 0x82000000;
            sprite_quads[var_v1 + 4].h = 0xa0;
            sprite_quads[var_v1 + 4].unk_0E = 0;
            sprite_quads[var_v1 + 4].unk_0F = 0x0;
            sprite_quads[var_v1 + 4].unk_10 = 0;
            sprite_quads[var_v1 + 4].unk_0C = (var_v1 * 2) + 0x15;
            sprite_quads[var_v1 + 4].unk_12 = 0x1E0;
            sprite_quads[var_v1 + 4].tint_r = 0;
            sprite_quads[var_v1 + 4].tint_g = 0;
            sprite_quads[var_v1 + 4].tint_b = 0;
            sprite_quads[var_v1 + 4].anchor_x = 0;
            sprite_quads[var_v1 + 4].anchor_y = 0;
            sprite_quads[var_v1 + 4].unk_1C = 0x1000;
            sprite_quads[var_v1 + 4].unk_1E = 0x1000;
            sprite_quads[var_v1 + 4].unk_20 = 0;
            sprite_quads[var_v1 + 4].x = (var_v1 << 7) - 0xA0;
            sprite_quads[var_v1 + 4].y = -0x78;
            game_state.quad_layer[var_v1 + 4] = 0xb;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].w = (var_v1 << 6) + 0x80;
            sprite_quads[var_v1 + 6].flags = 0x82000000;
            sprite_quads[var_v1 + 6].h = 0x0;
            sprite_quads[var_v1 + 6].unk_0E = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xa0;
            sprite_quads[var_v1 + 6].unk_10 = 0;
            sprite_quads[var_v1 + 6].unk_0C = (var_v1 * 2) + 0xa;
            sprite_quads[var_v1 + 6].unk_12 = 0x1E0;
            sprite_quads[var_v1 + 6].tint_r = 0;
            sprite_quads[var_v1 + 6].tint_g = 0;
            sprite_quads[var_v1 + 6].tint_b = 0;
            sprite_quads[var_v1 + 6].anchor_x = 0;
            sprite_quads[var_v1 + 6].anchor_y = 0;
            sprite_quads[var_v1 + 6].unk_1C = 0x1000;
            sprite_quads[var_v1 + 6].unk_1E = 0x1000;
            sprite_quads[var_v1 + 6].unk_20 = 0;
            sprite_quads[var_v1 + 6].x = (var_v1 << 7) - 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
            game_state.quad_layer[var_v1 + 6] = 0xb;
        }
        dec_bg_cd_read(0x43B1, 1);
        game->mode_step_timer.u += 1;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 19:
    case 27:
    case 41:
    case 44:
    case 47:
    case 49:
        if (cd_read_sync_status() & 1) {
            game->mode_step_timer.u += 1;
            return;
        }
        return;
    case 2:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x80183000);
        game->mode_step_timer.u += 1;
        return;
    case 3:
        dec_bg_cd_read(0x43AA, 1);
        game->mode_step_timer.u += 1;
        return;
    case 5:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x8019C000);
        game->mode_step_timer.u += 1;
        return;
    case 6:
        dec_bg_cd_read(0x43A3, 1);
        game->mode_step_timer.u += 1;
        return;
    case 8:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801B5000);
        game->mode_step_timer.u += 1;
        return;
    case 9:
        dec_bg_cd_read(0x439C, 1);
        game->mode_step_timer.u += 1;
        return;
    case 11:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0x100;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C8E0(&rect, (void *)0x801CE000);
        game->mode_step_timer.u += 1;
        return;
    case 12:
        dec_bg_cd_read(0x4395, 1);
        game->mode_step_timer.u += 1;
        return;
    case 14:
        bg_still_decode_to_vram(0x140, 0, 0x140, 0xA0);
        game->mode_step_timer.u += 1;
        return;
    case 15:
        dec_bg_cd_read(0x437D, 1);
        game->mode_step_timer.u += 1;
        return;
    case 17:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        game->mode_step_timer.u += 1;
        return;
    case 18:
        dec_bg_cd_read(0x4385, 1);
        game->mode_step_timer.u += 1;
        return;
    case 20:
        bg_still_decode_to_vram(0x280, 0, 0x140, 0xA0);
        game->mode_step_timer.u += 1;
        return;
    case 21:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            sprite_quads[var_v1 + 4].flags = (s32) (sprite_quads[var_v1 + 4].flags & 0x7FFFFFFF);
        }
        game->mode_step_timer.u++;
        return;
    case 22:
        value = sprite_quads[4].tint_r;
        value += 2;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;

        if (value == 0x80) {
            game->mode_step_timer.u++;
            return;
        }
        break;
    case 23:
        if (!(date_ride_frame & 7)) {
            date_ride_quad_shift(4);
                if (((u16) date_ride_frame % 24U) == 0) {
                audio_command(0x503);
                    }
        }
        if (++date_ride_frame == 0x140) {
            date_ride_frame = 0;
            game_state.mode_step_timer.u += 1;
        }
        break;
    case 24:
        game->mode_step_timer.u++;
        return;
    case 25:
        back_clear_switch(0);
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            sprite_quads[var_v1 + 4].flags = (s32) (sprite_quads[var_v1 + 4].flags | 0x80000000);
        }
        game->mode_step_timer.u += 1;
        return;
    case 26:
        dec_bg_cd_read(0x438D, 1);
        game->mode_step_timer.u += 1;
        return;
    case 28:
        bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0);
        rect.x = 0x140;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x280, 0);
        game->mode_step_timer.u += 1;
        back_clear_switch(1);
        return;
    case 29:
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].flags = 0x02000000;
            sprite_quads[var_v1 + 4].h = 0xA0;
            sprite_quads[var_v1 + 4].unk_0F = 0;
            sprite_quads[var_v1 + 4].y = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].flags = 0x02000000;
            sprite_quads[var_v1 + 6].h = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
        }
        date_ride_frame = 0;
        audio_command(0x500);
        game->mode_step_timer.u += 1;
        return;
    case 30:
    case 32:
    case 34:
        date_ride_quad_shift(8);
        if (++date_ride_frame == 0x14) {
            game->mode_step_timer.u += 1;
            return;
        }
        break;
    case 31:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801CE000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].h = 0xA0;
            sprite_quads[var_v1 + 4].unk_0F = 0;
            sprite_quads[var_v1 + 4].y = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].h = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
        }
        date_ride_frame = 0;
        game->mode_step_timer.u += 1;
        return;
    case 33:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x801B5000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].h = 0xA0;
            sprite_quads[var_v1 + 4].unk_0F = 0;
            sprite_quads[var_v1 + 4].y = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].h = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
        }
        date_ride_frame = 0;
        game->mode_step_timer.u += 1;
        return;
    case 35:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x8019C000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].h = 0xA0;
            sprite_quads[var_v1 + 4].unk_0F = 0;
            sprite_quads[var_v1 + 4].y = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].h = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
        }
        date_ride_frame = 0;
        game->mode_step_timer.u += 1;
        return;
    case 36:
        date_ride_quad_shift(8);
        if ((u16) date_ride_frame >= 8U) {
            value = sprite_quads[4].tint_r;
        value -= 4;
        color = (value & 0xFF);
            sprite_quads[7].tint_b = color;
            sprite_quads[7].tint_g = color;
            sprite_quads[7].tint_r = color;
            sprite_quads[6].tint_b = color;
            sprite_quads[6].tint_g = color;
            sprite_quads[6].tint_r = color;
            sprite_quads[5].tint_b = color;
            sprite_quads[5].tint_g = color;
            sprite_quads[5].tint_r = color;
            sprite_quads[4].tint_b = color;
            sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;
        }
        if (++date_ride_frame == 0x14) {
            game->mode_step_timer.u += 1;
            return;
        }
        break;
    case 37:
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x140;
        rect.h = 0xA0;
        func_8009C93C(&rect, 0x140, 0x100);
        LoadSquare(0x280U, 0U, 0x140U, 0xA0U, 0x80183000);
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 4].h = 0xA0;
            sprite_quads[var_v1 + 4].unk_0F = 0;
            sprite_quads[var_v1 + 4].y = -0x78;
        }
        for (var_v1 = 0; var_v1 < 2; var_v1++) {
            sprite_quads[var_v1 + 6].h = 0;
            sprite_quads[var_v1 + 6].unk_0F = 0xA0;
            sprite_quads[var_v1 + 6].y = -0x78;
        }
        date_ride_frame = 0;
        game->mode_step_timer.u += 1;
        return;
    case 38:
        date_ride_quad_shift(8);
        value = sprite_quads[4].tint_r;
        value -= 4;
        color = (value & 0xFF);
        sprite_quads[7].tint_b = color;
        sprite_quads[7].tint_g = color;
        sprite_quads[7].tint_r = color;
        sprite_quads[6].tint_b = color;
        sprite_quads[6].tint_g = color;
        sprite_quads[6].tint_r = color;
        sprite_quads[5].tint_b = color;
        sprite_quads[5].tint_g = color;
        sprite_quads[5].tint_r = color;
        sprite_quads[4].tint_b = color;
        sprite_quads[4].tint_g = color;
        sprite_quads[4].tint_r = color;

        if (value < 4U) {
            game->mode_step_timer.u += 1;
            return;
        }
        break;
    case 39:
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            sprite_quads[var_v1 + 4].flags = (s32) (sprite_quads[var_v1 + 4].flags | 0x80000000);
        }
        game->mode_step_timer.u++;
        return;
    case 40:
        saved_step_timer = game->mode_step_timer.u;

        saved_mode_step = (s32) game_state.mode_step;
        /* FAKE: one source line preserves as1 callback load scheduling. T-12100. */
        read_bustup(game_state.mode_step); game->mode_step_timer.u = saved_step_timer; if (saved_mode_step != game_state.mode_step) { game_state.mode_step -= 1; game->mode_step_timer.u++; } break;
    case 42:
        saved_mode_step = game_state.mode_step;
        saved_step_timer = game->mode_step_timer.u;

        portrait_show_checked();
        current = saved_mode_step + 1;
        if (game_state.mode_step != current) {
            game_state.mode_step = saved_mode_step;
            game->mode_step_timer.u = 0x28;
            return;
        }
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return;
    case 43:
        cd_read_sectors_nowait(0xAU, 0x80162000, 0x433E);
        game->mode_step_timer.u += 1;
        return;
    case 45:
        if (((((DateMovieHeader8 *)bg_still_bitstream)->unk_00 & 0xFFFF0000) != 0x38000000) || ((((DateMovieHeader8 *)bg_still_bitstream)->unk_04 & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            movie->unk_04 = 0;
            movie->unk_06 = 0;
            game->mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        game->mode_step_timer.u += 1;
        return;
    case 46:
        saved_step_timer = game->mode_step_timer.u;

        date_load_girl_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return;
    case 48:
        saved_step_timer = game->mode_step_timer.u;

        date_load_shared_script();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return;
    case 50:
        saved_step_timer = game->mode_step_timer.u;

        date_load_choice_tables();
        game_state.mode_step -= 1;
        game->mode_step_timer.u = saved_step_timer;
        game->mode_step_timer.u++;
        return;
    default:
        mode_step_next();
        D_80122D2C = 0;
        audio_command(0x200);
        hizuke_disp_switch(1);
        break;
    }
    return;
}

s32 date_ride_quad_shift(s16 a0) {
    sprite_quads[4].y += a0;
    sprite_quads[5].y += a0;
    sprite_quads[4].h -= a0;
    sprite_quads[5].h -= a0;
    sprite_quads[6].h += a0;
    sprite_quads[7].h += a0;
    sprite_quads[6].unk_0F -= a0;
    sprite_quads[7].unk_0F -= a0;
}

s32 date_ride_ghost_house(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 saved_step_timer;
    u32 saved_step_timer_2;
    u8 saved_mode_step;
    u8 saved_mode_step_2;

    switch (game_state.mode_step_timer.u) {
    case 0:
        D_80122D2C = 1;
        D_8015CFC4 = 0x64;
        srand((*(s32 *)&game_state.frame_count));
        date_ride_frame = 0;
        hizuke_disp_switch(0);
        date_ride_background_skip(0x43BA);
        D_8015CFA4[0] = -0x50;
        D_8015CFA4[1] = 0;
        D_8015CFA4[2] = 0x64;
        D_8015CFAC[0] = -0x28;
        D_8015CFAC[1] = 0;
        D_8015CFAC[2] = -0x3C;
        game_state.mode_step_timer.u += 1;
        D_8015CFB4[0] = (rand() % 3) - 1;
        D_8015CFB4[1] = (rand() % 3) - 1;
        D_8015CFB4[2] = (rand() % 3) - 1;
        D_8015CFBC[0] = (rand() % 3) - 1;
        D_8015CFBC[1] = (rand() % 3) - 1;
        D_8015CFBC[2] = (rand() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            game_state.mode_step_timer.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)bg_still_bitstream)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)bg_still_bitstream)[1] & 0xFF010000) != 0x10000)) {
            printf("DEC_BS header Error \n");
            ((RECT *)bg_lba_cache)[0].w = 0;
            ((RECT *)bg_lba_cache)[0].h = 0;
            game_state.mode_step_timer.u -= 2;
            return 0;
        }
        dec_bg_decode_to_vram(1);
        dec_bg_show_switch(1);
        audio_command(0x203);
        game_state.mode_step_timer.u += 1;
        break;
    case 3:
        saved_mode_step = game_state.mode_step;
        saved_step_timer = game_state.mode_step_timer.u;
        normal_date_bg_fadein();
        game_state.mode_step_timer.u = saved_step_timer;
        if (saved_mode_step != game_state.mode_step) {
            game_state.mode_step_timer.u = saved_step_timer + 1;
        }
        game_state.mode_step = saved_mode_step;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&game_state.frame_count) % 30U) == (rand() % 30)) {
                (D_8015CFB4)[var_s3] = (rand() % 3) - 1;
            }

            if (((u32) (*(s32 *)&game_state.frame_count) % 30U) == (rand() % 30)) {
                (D_8015CFBC)[var_s3] = (rand() % 3) - 1;
            }

            if ((D_8015CFB4[var_s3] == 0) && (D_8015CFBC[var_s3] == 0)) {
                if ((remainder = rand() % 2)) {
                    D_8015CFB4[var_s3] = ((rand() % 2) * 2) - 1;
                } else {
                    D_8015CFBC[var_s3] = ((rand() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8015CFA4)[var_s3];
            *temp_v0 += D_8015CFB4[var_s3];
            temp_a0 = &(D_8015CFAC)[var_s3];
            *temp_a0 += D_8015CFBC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8015CFB4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8015CFB4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8015CFBC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8015CFBC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        prim_glow_disc((s32) D_8015CFA4[0], (s32) D_8015CFAC[0], 0xA, 2, 2, 0x808080, 3, 1);
        prim_glow_disc((s32) D_8015CFA4[1], (s32) D_8015CFAC[1], 0xA, 2, 2, 0x804040, 3, 1);
        prim_glow_disc((s32) D_8015CFA4[2], (s32) D_8015CFAC[2], 0xA, 2, 2, 0x800000, 3, 1);
        if (((u32) (*(s32 *)&game_state.frame_count) % 50U) == 0) {
            if (!(rand() & 3)) {
                audio_command(0x504);
            } else if ((rand() % 4) == 1) {
                audio_command(0x505);
            } else if ((rand() % 4) == 2) {
                audio_command(0x506);
            } else {
                audio_command(0x507);
                D_8015CFC4 = 0;
            }
        }
        if (D_8015CFC4++ == 0x14) {
            audio_command(0x507);
        }
        if (D_8015CFC4 >= 0x65) {
            D_8015CFC4 = 0x64;
        }
        if (date_ride_frame++ == 0x12C) {
            game_state.mode_step_timer.u += 1;
            break;
        }
        break;
    case 5:
        saved_mode_step_2 = game_state.mode_step;
        saved_step_timer_2 = game_state.mode_step_timer.u;
        normal_date_bg_fadeout();
        game_state.mode_step_timer.u = saved_step_timer_2;
        if (saved_mode_step_2 != game_state.mode_step) {
            game_state.mode_step_timer.u = saved_step_timer_2 + 1;
        }
        game_state.mode_step = saved_mode_step_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        game_state.mode_step_timer.u += 1;
        break;
    default:
        audio_command(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        mode_step_next();
        break;
    }
}

void date_ride_background_skip(s32 background_lba) {
    s32 background_buffer_result;

    cd_read_checked = 0;
    background_buffer_result = dec_bg_cd_read(background_lba, 1);
    if (background_buffer_result == D_800B5939) {
        printf("same\n");
        game_state.mode_step_timer.w += 3;
        return;
    }
    if (background_buffer_result == (1 - D_800B5939)) {
        printf("another\n");
        game_state.mode_step_timer.w += 2;
    }
}

/* FAKE: s32 fall-through keeps the record base in v0 (T-9780). */
s32 date_ferris_wheel_still_copy(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    SpriteQuad *record;
    s16 i;

    scene_still_show(arg0, arg1, arg2, arg3, arg4, arg5);
    /* FAKE: shared setup/loop line preserves instruction scheduling (T-9780). */
    i = 0; do {
        record = &sprite_quads[i];
        record[4] = record[58];
        game_state.quad_layer[i + 4] = game_state.quad_layer[i + 58];
        i++;
    } while (i < 6);
}
