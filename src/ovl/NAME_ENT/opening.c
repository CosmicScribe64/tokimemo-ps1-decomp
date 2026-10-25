/* opening.c: src/ovl/NAME_ENT/80142A80.c:53,277,695; attract picture-sequence callbacks, voice/palette presentation and opening movie flow. Original object 0x80142A80-0x80144AD0 (config/objects/NAME_ENT.txt). */
#define MAIN_API_OVERRIDE_prim_flash_gray /* original called it undeclared (implicit int): its $v0 result keeps v0 busy after the calls in opening_story_scene, T-25604 */
#define MAIN_API_OVERRIDE_D_8012070C /* opening_story_voice_meter_update: pointer view of the record field (main_api.h: s32), T-22007 */
#include "common.h"
#include "ovl/NAME_ENT.h"

extern s32 *D_8012070C;
extern s32 prim_flash_gray();

/* The 21 song-line strings are placed from their rodata pieces (rodata_pieces.py), not copied into the source. T-22007 */
extern u8 opening_lyric_text_00[];
extern u8 opening_lyric_text_01[];
extern u8 opening_lyric_text_02[];
extern u8 opening_lyric_text_03[];
extern u8 opening_lyric_text_04[];
extern u8 opening_lyric_text_05[];
extern u8 opening_lyric_text_06[];
extern u8 opening_lyric_text_07[];
extern u8 opening_lyric_text_08[];
extern u8 opening_lyric_text_09[];
extern u8 opening_lyric_text_10[];
extern u8 opening_lyric_text_11[];
extern u8 opening_lyric_text_12[];
extern u8 opening_lyric_text_13[];
extern u8 opening_lyric_text_14[];
extern u8 opening_lyric_text_15[];
extern u8 opening_lyric_text_16[];
extern u8 opening_lyric_text_17[];
extern u8 opening_lyric_text_18[];
extern u8 opening_lyric_text_19[];
extern u8 opening_lyric_text_20[];
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_00);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_01);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_02);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_03);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_04);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_05);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_06);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_07);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_08);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_09);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_10);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_11);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_12);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_13);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_14);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_15);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_16);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_17);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_18);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_19);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/opening.rodata", opening_lyric_text_20);

void opening_movie(void) {
    s32 pad; /* FAKE: unused; its stack home gives the original 72-byte frame. T-22007 */
    u8 sp43;
    s32 saved_text_speed;
    u8 sp34[8];

    game_state.screen_brightness = 0x80;
    sp43 = debug_hud_level_get();
    debug_hud_level_set(0);
    actor_deactivate(0);
    hud_corner_icon_set(1);
    hud_cursor_show(0);
    back_clear_switch(1);
    text_system_reset();
    saved_text_speed = get_k_speed();
    k_speed_set(TEXT_SPEED_SLOW);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_00, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_01, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_02, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_03, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_04, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_05, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_06, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_07, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_08, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_09, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_10, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_11, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_12, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_13, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_14, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_15, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_16, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_17, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_18, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_19, 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) opening_lyric_text_20, 0);
    k_disp_start(0);
    set_movie_offset(0x18, 0x10);
    movie_play(0, 0x33B, 0x820);
    func_80088070();
    sp34[0] = 0x80;
    cd_audio_command(0xE, 0, sp34);
    cd_audio_command(9, 0, 0);
    cd_audio_command(0xA, 0, 0);
    bg_still_buffers_set(0x80167000, 0x80162000, 0x80162000);
    func_8009C210(1);
    vram_clear_blocks(0);
    func_8009C674(0);
    actor_deactivate(0);
    game_mode_set(GAME_MODE_TITLE);
    debug_hud_level_set((s32) sp43);
    k_speed_set((u8) saved_text_speed);
    hud_corner_icon_set(0);
    hud_cursor_actor_init(0);
    hud_cursor_show(0);
    pad_code_index = 0;
    pad_code_entered = 0;
    pad_code_timer = 0;
}

void opening_story_load(void) {
    s32 row;
    s32 addresses[8];
    u8 scene_step;

    /* Four address columns per 0x10000-byte bank. T-20010. */
    for (row = 0; row < 2; row++) {
        addresses[row * 4 + 0] = 0x80180000 + row * 0x10000;
        addresses[row * 4 + 1] = 0x80184000 + row * 0x10000;
        addresses[row * 4 + 2] = 0x80188000 + row * 0x10000;
        addresses[row * 4 + 3] = 0x8018C000 + row * 0x10000;
    }
    scene_step = game_state.substep;
    switch (scene_step) {
    case 0:
        game_state.screen_brightness = 0x80;
        draw2d3d(1, 0);
        actor_draw_enable(1);
        hud_cursor_show(0);
        actor_deactivate(0);
        actor_clear_upper();
        sprite_tables_clear();
        text_system_reset();
        back_clear_switch(1);
        game_state.clear_color_g = 0;
        game_state.clear_color_r = 0;
        game_state.clear_color_b = 0;
        audio_command(0x7F);
        audio_bank_load_start(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        game_state.substep = game_state.substep + 1;
        return;
    case 1:
        if (audio_bank_load_poll() == 1) {
            audio_bank_start(1, 0);
            cd_read_sectors(0x21U, 0x80180000, 0x9FD5);
            game_state.substep = game_state.substep + 1;
            return;
        }
    default:
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) addresses, 0, 4, 2, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
            game_state.substep = game_state.substep + 1;
            return;
        }
        break;
    case 3:
        cd_read_sectors(0x21U, 0x80180000, 0xA048);
        game_state.substep = game_state.substep + 1;
        return;
    case 4:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) addresses, 0, 4, 2, 2, 0x1FE, -0xA0, -0x78, 8, 0xB);
            load_palette(0x80190000, 0x1E, 1, 3, 0);
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
            game_state.substep = game_state.substep + 1;
            return;
        }
        break;
    case 5:
        cd_read_sectors(0x21U, 0x80180000, 0x9FF6);
        game_state.substep = game_state.substep + 1;
        return;
    case 6:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) addresses, 0, 4, 2, 2, 0x1FD, -0xA0, -0x78, 8, 0xD);
            load_palette(0x80190000, 0x1D, 1, 3, 0);
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
            game_state.substep = game_state.substep + 1;
            return;
        }
        break;
    case 7:
        cd_read_sectors(0x41U, 0x80180000, 0x9F22);
        game_state.substep = game_state.substep + 1;
        return;
    case 8:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) addresses, 1, 8, 4, 2, 0x1EE, -0x140, -0x78, 8, 0x15);
            load_palette(0x801A0000, 0xE, 1, 3, 0);
            ((BgLayer *)&bg_layers)[1].unk_00 = 0x51000000;
            ((BgLayer *)&bg_layers)[1].unk_11 = 0;
            ((BgLayer *)&bg_layers)[1].unk_10 = 0;
            game_state.substep = game_state.substep + 1;
            ((BgLayer *)&bg_layers)[1].unk_12 = 0;
            return;
        }
        break;
    case 9:
        cd_read_sectors(0x31U, 0x801B0000, 0x9FA4);
        game_state.substep = game_state.substep + 1;
        return;
    case 10:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x41U, 0x80180000, 0x9EE1);
            game_state.substep = game_state.substep + 1;
            return;
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) addresses, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
            load_palette(0x801A0000, 0xF, 1, 3, 0);
            ((BgLayer *)&bg_layers)[0].unk_11 = 0;
            ((BgLayer *)&bg_layers)[0].unk_10 = 0;
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x51000000;
            game_state.substep = game_state.substep + 1;
            ((BgLayer *)&bg_layers)[0].unk_12 = 0;
            return;
        }
        break;
    case 12:
        cd_read_sectors(0x41U, 0x80180000, 0x9F63);
        game_state.substep = game_state.substep + 1;
        return;
    case 13:
        if (cd_read_sync_status() & 1) {
            cd_audio_request_set(0x06000BC8, 0x11000CBA, 0xF);
            audio_command(0x300);
            text_system_reset();
            set_kanji_string(-0x90, 0x14, 0, "始まりがあれば終わりがあるように、", 0);
            set_kanji_string(-0x90, 0x24, 0, "出会いがあればまた、別れもあるのです。", 0);
            set_kanji_string(-0x90, 0x34, 0, "永遠に続く二人の関係、", 0);
            set_kanji_string(-0x90, 0x44, 0, "それはどんなに幸せなことでしょう。", 0);
            k_disp_start(4);
            k_disp_switch(0, 0);
            k_disp_switch(1, 0);
            k_disp_switch(2, 0);
            k_disp_switch(3, 0);
            mode_step_next();
        }
        break;
    }
}

void opening_story_second_narration(void) {
    if ((cd_audio_state() & 2) && opening_second_narration_started == 0) {
        cd_audio_request_set(0x06000CBB, 0x12000E0C, 0xF);
        audio_command(0x300);
        text_system_reset();
        set_kanji_string(-0x90, 0x14, 0, "ここ、きらめき高校には、", 0);
        set_kanji_string(-0x90, 0x24, 0, "一つの伝説があります。", 0);
        set_kanji_string(-0x90, 0x34, 0, "校庭のはずれにある一本の古木。", 0);
        k_disp_start(3);
        k_disp_switch(0, 0);
        k_disp_switch(1, 0);
        k_disp_switch(2, 0);
        opening_second_narration_started = 1;
    }
}

/* Same-file functions defined below. */
void opening_story_text_toggle(void);
void opening_story_voice_meter_update(void);
void opening_story_actor97_random_animation(void);

/* Introduction scene state machine. T-25604: case 9 hides the three records with one chain
 * (sprite_quads[2] is read back, so uopt keeps it in a2 and the selector gets t0), prim_flash_gray is
 * called undeclared (its $v0 result keeps v0 busy after each call), and every tile fill counts with
 * i. */
void opening_story_scene(void) {
    s32 pad0; /* FAKE: unused; with pad1 its stack home gives the original 0x90 frame. T-25604 */
    s32 tiles[8];
    s32 pad1; /* FAKE: unused, see pad0. T-25604 */
    s32 i;
    u8 scene_step;

    /* FAKE: T-25319, T-25604: a reference to unk_F5F before the first branch satisfies the entry
     * rule of tools/cvt_pass.py, so the CVTs of the case 16/17 compares go and F5F is one promoted
     * range (v0) as in the target; uopt removes the self-store. */
    game_state.current_girl = game_state.current_girl;
    for (i = 0; i < 8; i++) {
        tiles[i] = 0x80180000 + i * 0x4000;
    }
    scene_step = game_state.substep;
    switch (scene_step) {
    case 0:
        if ((((BgLayer *)&bg_layers)[0].unk_10 = ((BgLayer *)&bg_layers)[0].unk_11 = ((BgLayer *)&bg_layers)[0].unk_12++) >= 0x81U) {
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x01000000;
            game_state.substep++;
            opening_second_narration_started = 0;
        }
        break;
    case 1:
        if ((game_state.frame_count & 3) == 3) {
            if (((BgLayer *)&bg_layers)[0].unk_04-- < -0xF0) {
                game_state.substep++;
                ((BgLayer *)&bg_layers)[0].unk_00 = 0x51000000;
            }
        }
        break;
    case 2:
        if ((((BgLayer *)&bg_layers)[0].unk_10 = ((BgLayer *)&bg_layers)[0].unk_11 = ((BgLayer *)&bg_layers)[0].unk_12--) != 0) {
            if ((((BgLayer *)&bg_layers)[1].unk_10 = ((BgLayer *)&bg_layers)[1].unk_11 = ((BgLayer *)&bg_layers)[1].unk_12++) >= 0x81U) {
                ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
                ((BgLayer *)&bg_layers)[1].unk_00 = 0x01000000;
                game_state.substep++;
            }
        }
        /* fallthrough */
    case 3:
        if ((game_state.frame_count & 3) == 3) {
            ((BgLayer *)&bg_layers)[0].unk_04 -= 1;
            if (((BgLayer *)&bg_layers)[1].unk_04++ >= -0xEF) {
                prim_bg_slot_setup((s32) tiles, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
                load_palette((s32 *)0x801A0000U, 0xF, 1, 3, 0);
                ((BgLayer *)&bg_layers)[0].unk_11 = 0;
                ((BgLayer *)&bg_layers)[0].unk_00 = 0x51000000;
                ((BgLayer *)&bg_layers)[0].unk_12 = 0;
                ((BgLayer *)&bg_layers)[0].unk_10 = 0;
                game_state.substep++;
            }
        }
        break;
    case 4:
        opening_story_second_narration();
        if ((((BgLayer *)&bg_layers)[1].unk_10 = ((BgLayer *)&bg_layers)[1].unk_11 = ((BgLayer *)&bg_layers)[1].unk_12--) != 0) {
            if ((((BgLayer *)&bg_layers)[0].unk_10 = ((BgLayer *)&bg_layers)[0].unk_11 = ((BgLayer *)&bg_layers)[0].unk_12++) >= 0x81U) {
                ((BgLayer *)&bg_layers)[1].unk_00 = 0x80000000;
                ((BgLayer *)&bg_layers)[0].unk_00 = 0x01000000;
                game_state.substep++;
            }
        }
        /* fallthrough */
    case 5:
        if ((game_state.frame_count & 3) == 3) {
            if (((BgLayer *)&bg_layers)[1].unk_04 < -0xA0) {
                ((BgLayer *)&bg_layers)[1].unk_04 += 1;
            }
            if (((BgLayer *)&bg_layers)[0].unk_04-- < -0x140) {
                ((BgLayer *)&bg_layers)[0].unk_00 = 0x51000000;
                game_state.substep++;
            }
            /* FAKE: T-25319, T-25604: ^ 0 drops the a0 argument preference of the promoted field;
             * gives the target's move a0,v0 and F8 in a0. */
            debug_print_hex(((BgLayer *)&bg_layers)[0].unk_04 ^ 0, 0, 0);
            if (((BgLayer *)&bg_layers)[0].unk_04 == -0x118) {
                text_system_reset();
                set_kanji_string(-0x90, 0x14, 0, "そのたもとで卒業の日に、", 0);
                set_kanji_string(-0x90, 0x24, 0, "女の子からの告白で生まれた", 0);
                set_kanji_string(-0x90, 0x34, 0, "恋人達は、永遠に幸せな関係に", 0);
                set_kanji_string(-0x90, 0x44, 0, "なれるという伝説が…", 0);
                k_disp_start(4);
                k_disp_switch(0, 0);
                k_disp_switch(1, 0);
                k_disp_switch(2, 0);
                k_disp_switch(3, 0);
            }
        }
        break;
    case 6:
        if (((game_state.frame_count & 3) == 3) && (((BgLayer *)&bg_layers)[0].unk_04 >= -0x13F)) {
            ((BgLayer *)&bg_layers)[0].unk_04 -= 1;
        }
        if ((((BgLayer *)&bg_layers)[0].unk_10 = ((BgLayer *)&bg_layers)[0].unk_11 = ((BgLayer *)&bg_layers)[0].unk_12--) == 0) {
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
            /* FAKE: T-25131, T-25319, T-25604 (from T-20008): the target has this nested fill loop
             * (the outer loop runs once); its original source form is unknown. */
            for (i = 0; i < 8; i++) {
                for (i = 0; i < 8; i++) {
                    tiles[i] = 0x801B0000 + i * 0x4000;
                }
            }
            prim_bg_slot_setup((s32) tiles, 1, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
            load_palette((s32 *)0x801C8000U, 0xE, 1, 3, 0);
            ((BgLayer *)&bg_layers)[1].unk_10 = ((BgLayer *)&bg_layers)[1].unk_11 = ((BgLayer *)&bg_layers)[1].unk_12 = 0;
            for (i = 0; i < 3; i++) {
                *(u32 *)&sprite_quads[i].flags = 0x01000000;
                sprite_quads[i].x = i * 0x50 - 0xD0;
                sprite_quads[i].y = -0x78;
                sprite_quads[i].w = 0xFF;
                sprite_quads[i].h = 0xF0;
                sprite_quads[i].unk_0C = i * 2 + 9;
                sprite_quads[i].unk_12 = 0x1FF - i;
                sprite_quads[i].unk_1C = 0x1000;
                sprite_quads[i].unk_1E = 0x1000;
                game_state.quad_layer[i] = 7;
            }
            game_state.quad_layer[1] = 8;
            sprite_quads[1].y = -0x6E;
            game_state.substep++;
        }
        break;
    case 7:
        if ((sprite_quads[0].tint_r = sprite_quads[0].tint_g = sprite_quads[0].tint_b = sprite_quads[1].tint_r = sprite_quads[1].tint_g = sprite_quads[1].tint_b = sprite_quads[2].tint_r = sprite_quads[2].tint_g = sprite_quads[2].tint_b++) >= 0x81U) {
            game_state.substep++;
        } else {
            ((BgLayer *)&bg_layers)[1].unk_10 = ((BgLayer *)&bg_layers)[1].unk_11 = ((BgLayer *)&bg_layers)[1].unk_12++;
        }
        break;
    case 8:
        if ((game_state.frame_count & 3) == 3) {
            if (sprite_quads[0].x-- < -0x160) {
                audio_command(0xD1);
                text_system_reset();
                cd_read_sectors(0x31U, 0x80180000U, 0xA017);
                ((BgLayer *)&bg_layers)[0].unk_10 = 0;
                game_state.substep++;
            } else {
                sprite_quads[2].x += 1;
            }
        }
        break;
    case 9:
        if ((u8) ((BgLayer *)&bg_layers)[0].unk_10 < 0xFEU) {
            prim_flash_gray(((BgLayer *)&bg_layers)[0].unk_10++);
        } else {
            prim_flash_gray(0xFF);
            *(u32 *)&sprite_quads->flags = *(u32 *)&sprite_quads[1].flags = *(u32 *)&sprite_quads[2].flags = 0x80000000;
            ((BgLayer *)&bg_layers)[1].unk_00 = 0x80000000;
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x01000000;
            ((BgLayer *)&bg_layers)[0].unk_11 = ((BgLayer *)&bg_layers)[0].unk_12 = ((BgLayer *)&bg_layers)[0].unk_10 = 0x80;
            ((BgLayer *)&bg_layers)[1].unk_10 = 0xFF;
            game_state.substep++;
        }
        break;
    case 10:
        prim_flash_gray(0xFF);
        if (cd_read_sync_status() & 1) {
            prim_bg_slot_setup((s32) tiles, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette((s32 *)0x801C8000U, 0xF, 1, 3, 0);
            ((BgLayer *)&bg_layers)[0].unk_00 = 0x01000000;
            cd_read_sectors(0xAEU, 0x80180000U, 0x9DF6);
            game_state.substep++;
        }
        break;
    case 11:
        prim_flash_gray(0xFF);
        if (cd_read_sync_status() & 1) {
            tpage_buf_clear();
            i = 0;
            do {
                load_palette((s32 *)0x80180000U, (i + 0x10) & 0xFF, 1, 2, i);
                i += 1;
            } while (i < 3);
            col2sepia(0x80180000U, &game_state.scratch_words[0], 8, 4, 0);
            load_palette(&game_state.scratch_words[0], 0x10, 1, 2, 0);
            i = 0x60;
            actor_clear(i);
            ((Actor *)actor_pool)[i].unk_01 = 5;
            ((Actor *)actor_pool)[i].flags = 4;
            ((Actor *)actor_pool)[i].quad_flags = 0x41000000;
            ((Actor *)actor_pool)[i].draw_flags = 0x84;
            ((Actor *)actor_pool)[i].frame_table = (u8 *)0x801A2624;
            ((Actor *)actor_pool)[i].sequence_table = (u8 *)0x801A263C;
            ((Actor *)actor_pool)[i].tpage_table = (u8 *)0x801A2610;
            ((Actor *)actor_pool)[i].sequence_slot = 0;
            ((Actor *)actor_pool)[i].unk_04 = 8;
            ((Actor *)actor_pool)[i].unk_05 = 0x80;
            ((Actor *)actor_pool)[i].x = -0xA0;
            ((Actor *)actor_pool)[i].y = -0x78;
            game_state.mode_step_timer.u = 0;
            game_state.substep++;
        }
        break;
    case 12:
        if (game_state.mode_step_timer.u == 0xC8) {
            D_80120652 |= 1;
        }
        if (((BgLayer *)&bg_layers)[1].unk_10 != 0) {
            prim_flash_gray(((BgLayer *)&bg_layers)[1].unk_10--);
        } else if ((u32) game_state.mode_step_timer.u >= 0x12DU) {
            game_state.substep++;
        } else if (!(D_80120652 & 1)) {
            game_state.substep++;
        }
        break;
    case 13:
        /* FAKE: T-25131, T-25319, T-25604 (from T-20008): nested fill loop as in case 6. */
        for (i = 0; i < 6; i++) {
            for (i = 0; i < 6; i++) {
                tiles[i] = 0x80186000 + i * 0x4000;
            }
        }
        prim_bg_slot_setup((s32) tiles, 1, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0x15);
        ((BgLayer *)&bg_layers)[0].unk_00 = 0x80000000;
        ((BgLayer *)&bg_layers)[1].unk_00 = 0x01000000;
        load_palette((s32 *)0x8019E000U, 0xF, 1, 2, 0);
        col2sepia(0x8019E000U, &game_state.scratch_words[0], 8, 4, 0);
        for (i = 0; i != 128; i++) {
            if (i < 32) {
                ((s32 *)game_state.scratch_buffer)[i] = game_state.scratch_words[i];
            } else {
                ((s32 *)game_state.scratch_buffer)[i] = ((s32 *)0x8019E000)[i];
            }
        }
        load_palette((s32 *)game_state.scratch_buffer, 0xF, 1, 2, 0);
        col2sepia(0x80180200U, &game_state.scratch_words[0], 8, 4, 0);
        load_palette(&game_state.scratch_words[0], 0x11, 1, 2, 0);
        actor_clear_upper();
        text_system_reset();
        cd_audio_request_set(0x39D, 0x060003A9, 0xF);
        audio_command(0x300);
        text_system_reset();
        set_kanji_string(-0x90, 0x44, 0, "あなたが…、", 0);
        k_disp_start(1);
        k_disp_switch(0, 0);
        game_state.mode_step_timer.u = 0;
        game_state.substep++;
        break;
    case 14:
        if (cd_audio_state() & 4) {
            game_state.mode_step_timer.u = 0;
            game_state.substep++;
        }
        break;
    case 15:
        opening_story_voice_meter_update();
        opening_story_actor97_random_animation();
        if (game_state.pad_pressed & 0xFFFF) {
            game_state.substep++;
        } else if ((u32) game_state.mode_step_timer.u >= 0x101U) {
            game_state.substep++;
        } else if ((cd_audio_state() & 2) && ((u32) game_state.mode_step_timer.u >= 0x79U)) {
            game_state.substep++;
        }
        break;
    case 16:
        /* FAKE: T-25131, T-25319, T-25604 (from T-20008): nested fill loop as in case 6. */
        for (i = 0; i < 6; i++) {
            for (i = 0; i < 6; i++) {
                tiles[i] = 0x801BA7F0 + i * 0x4000;
            }
        }
        prim_bg_slot_setup((s32) tiles, 0, 6, 3, 2, 0x1EE, -0xA0, -0x78, 8, 0x15);
        ((BgLayer *)&bg_layers)[1].unk_00 = 0x80000000;
        ((BgLayer *)&bg_layers)[0].unk_00 = 0x01000000;
        load_palette((s32 *)0x801D27F0U, 0xE, 1, 2, 0);
        col2sepia(0x801D27F0U, (s32 *)game_state.scratch_buffer, 8, 4, 0);
        load_palette((s32 *)game_state.scratch_buffer, 0xE, 1, 2, 0);
        col2sepia(0x80180400U, &game_state.scratch_words[0], 8, 4, 0);
        load_palette(&game_state.scratch_words[0], 0x12, 1, 2, 0);
        actor_clear_upper();
        i = 0x60;
        actor_clear(i);
        ((Actor *)actor_pool)[i].unk_01 = 5;
        ((Actor *)actor_pool)[i].flags = 0;
        ((Actor *)actor_pool)[i].quad_flags = 0x41000000;
        ((Actor *)actor_pool)[i].draw_flags = 0xA4;
        ((Actor *)actor_pool)[i].frame_table = (u8 *)0x801D6B60;
        ((Actor *)actor_pool)[i].sequence_table = (u8 *)0x801D6B90;
        ((Actor *)actor_pool)[i].tpage_table = (u8 *)0x801D6B54;
        ((Actor *)actor_pool)[i].sequence_slot = 0;
        ((Actor *)actor_pool)[i].unk_04 = 8;
        ((Actor *)actor_pool)[i].unk_05 = 0x80;
        ((Actor *)actor_pool)[i].x = -0xA0;
        ((Actor *)actor_pool)[i].y = -0x78;
        game_state.mode_step_timer.u = 0;
        game_state.substep++;
        if (game_state.current_girl >= 0x11U) {
            game_state.current_girl = (game_state.current_girl & 0xF) + 0x10;
        }
        if ((game_state.current_girl == 0x10) || (((u32) game_state.pad_held >> 0x10) != 0)) {
            game_state.current_girl = 0x10;
            cd_audio_request_set(0x3AA, 0x070003B5, 0xF);
            text_system_reset();
            set_kanji_string(-0x90, 0x44, 0, "す・き・で・す", 0);
            k_disp_start(1);
            k_disp_switch(0, 0);
            audio_command(0x300);
        }
        break;
    case 17:
        if (game_state.current_girl == 0x10) {
            if ((cd_audio_state() & 4) && ((u32) game_state.mode_step_timer.u >= 0x21U)) {
                D_80120652 = 3;
                D_80120668 = 1;
                game_state.mode_step_timer.u = 0;
                game_state.substep++;
            }
        } else {
            D_80120652 = 3;
            D_80120668 = 1;
            game_state.substep++;
            game_state.mode_step_timer.u = 0;
        }
        break;
    case 18:
        if ((u32) game_state.mode_step_timer.u >= 0x41U) {
            game_state.mode_step_timer.u = 0;
            game_state.substep++;
        }
        break;
    case 19:
        prim_flash_gray((s32) game_state.mode_step_timer.u);
        if ((u32) game_state.mode_step_timer.u >= 0xFFU) {
            mode_step_next();
        }
        break;
    }
    opening_story_text_toggle();
}

void opening_story_text_toggle(void) {
    if (game_state.pad_held & PAD_TRIANGLE) {
        k_disp_switch(0, 1);
        k_disp_switch(1, 1);
        k_disp_switch(2, 1);
        k_disp_switch(3, 1);
        prim_gradient_rect(-0x9C, 8, 0x120, 0x58, 2, 0x808080, 0x83);
        dtd_on_tpage(0, 0, 2, 0, 0);
        return;
    }
    k_disp_switch(0, 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_disp_switch(3, 0);
}

void opening_story_voice_meter_update(void) {
    s32 level;
    s32 avg;

    if (cd_audio_state() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
        if (((u32)game_state.pad_held >> 16) != 0) {
            level = game_state.mode_stage_timer & 0x3F;
        }
    }
    D_80125C80[game_state.frame_count & 3] = level;
    /* FAKE: the original computes a four-sample average that nothing reads; the empty compare keeps it from being dropped. T-22007 */
    avg = (D_80125C80[0] + D_80125C80[1] + D_80125C80[2] + D_80125C80[3]) / 4;
    if (avg == 0x7FFF) {
    }
    if (cd_audio_state() & 4) {
        if (!((u8)D_801206DA & 1) || !(D_801206DB & 0x80)) {
            actor_clear(0x62);
            D_801206D9 = 3;
            D_801206DA = 0x11;
            D_80120710 = 0x41000000;
            D_801206DB = 0xAC;
            D_801206E4 = 0x801B27B0;
            D_801206E8 = 0x801B27D8;
            D_8012070C = (s32 *)0x801B27AC;
            D_801206EE = 1;
            D_801206DC = 8;
            D_801206DD = 0x80;
            D_801206FE = -0xA0;
            D_80120702 = -0x78;
            load_tpage_buf_lock(5, *D_8012070C, 1);
        }
    } else {
        actor_clear(0x62);
        D_801206DB = 0;
        D_801206DA = 0;
        D_801206F0 = 0;
        D_801206E0 = 0;
    }
}

void opening_story_actor97_random_animation(void) {
    if (!(D_80120696 & 1) && ((rand() & 0x7F) == 0x7F)) {
        D_80120695 = 4;
        D_80120696 = 3;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x801B27B0;
        D_801206A4 = 0x801B27D8;
        D_801206C8 = 0x801B27AC;
        D_801206AA = 0;
        D_80120698 = 8;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void opening_story_end(void) {
    prim_flash_gray(0xFF);
    cd_command_reset();
    vram_clear_blocks(0);
    sprite_tables_clear();
    actor_deactivate(0);
    audio_command(0x71);
    audio_command(0x7F);
    mode_stage_next();
}

void opening_story_mode_stage(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        opening_story_load();
        break;
    case 1:
        opening_story_scene();
        break;
    case 2:
        opening_story_end();
        break;
    }
    if (game_state.pad_pressed & 0x860) {
        mode_step_set(2);
    }
}
