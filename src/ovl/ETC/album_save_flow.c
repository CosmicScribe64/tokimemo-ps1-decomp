/* album_save_flow.c: ; src/ovl/ETC/80140F50.c:4-24 provides album mode save and card-prompt flow. Original object 0x80140F50-0x80142B80 (config/objects/ETC.txt). */
#include "common.h"
#include "ovl/ETC.h"

u8 *func_80140F50(void) {
    if (game_state.input_device == INPUT_MOUSE) {
        if (game_state.confirm_button != 0) {
            return D_80150128;
        }
        return D_8015012C;
    }
    return D_80150108;
}

u8 *func_80140F94(void) {
    if (game_state.input_device == INPUT_MOUSE) {
        if (game_state.confirm_button != 0) {
            return D_8015012C;
        }
        return D_80150128;
    }
    return D_80150124;
}

void func_80140FD8(void) {
    GameState *g = &game_state;
    g->mode_step_timer.w += 1;
    switch ((u8) g->mode_step) {
    case 0x0:
        func_801413FC();
        break;
    case 0x1:
        func_80141468();
        break;
    case 0x2:
        func_801415A4();
        break;
    case 0x3:
        func_80141700();
        break;
    case 0x4:
        func_8014175C();
        break;
    case 0x5:
        func_80141980();
        break;
    case 0x6:
        func_80141BE8();
        break;
    case 0xA0:
        func_80141154();
        break;
    case 0xF0:
        func_80141E10();
        break;
    case 0xFD:
        func_80142574();
        break;
    case 0xFE:
        func_80142390();
        break;
    case 0xFF:
        func_801421AC();
        break;
    }
    func_801428EC();
}


void func_80141154(void) {
    u8 temp;
    temp = game_state.substep;
    switch (temp) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep += 1;
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep += 1;
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "初期化していいですか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_disp_start(5);
        game_state.substep += 1;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (func_80054108() == 1) {
                mode_step_set(4);
            } else {
                mode_step_set(2);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep += 1;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "別のメモリーカードを", 0);
        set_kanji_string(-0x78, 0x3E, 1, (s32) "差し込み口１", 0);
        set_kanji_string(-0x78, 0x3E, 0xF, (s32) "　　　　　　にセットして、", 0);
        set_kanji_string(-0x78, 0x4C, 0xF, (s32) "　ボタンを押してください。", 0);
        set_kanji_string(-0x78, 0x4C, 1, func_80140F50(), 0);
        game_state.substep += 1;
        k_disp_start(5);
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            mode_step_set(4);
        }
        break;
    }
    dec_bg_draw(2);
}

void func_801413FC(void) {
    printf("データセーブ");
    if (game_state.options_dirty != 0) {
        audio_bank_state_reset();
        game_mode_set(GAME_MODE_TITLE);
        return;
    }
    text_system_reset();
    set_dec_bri(0);
    game_state.screen_brightness = 0;
    mode_step_next();
}

s32 func_80141468(void) {
    switch (game_state.substep) {
    case 0:
        dec_bg_reset();
        cd_read_sectors(9U, 0x80162000, 0xA1A2);
        game_state.substep += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            game_state.substep += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x2C0, 0, 0x140, 0xF0);
        MoveSquare(0x2C0, 0x20, 0x140, 0xD0, 0x2C0, 0);
        func_8009C674(0);
        game_state.substep += 1;
        return;
    case 3:
        if (game_state.screen_brightness++ < 0x80U) {
            set_dec_bri(game_state.screen_brightness);
            dec_bg_draw(2);
            return;
        }
        mode_step_next();
        break;
    }
}

void func_801415A4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset_point_set();
        set_kanji_string(-0x78, 0x30, 0, "今回のアルバムモードのデータを", 0);
        set_kanji_string(-0x78, 0x3E, 0, "　ときめきメモリアル・システムファイル", 0);
        set_kanji_string(-0x78, 0x4C, 0, "にセーブしますか。", 0);
        set_kanji_string(-0x88, 0x5A, 0, D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(7);
        printf("データセーブメッセージ");
        game_state.substep += 1;
        break;
    case 1:
        if ((u32)game_state.mode_step_timer.w >= 0x41U) {
            mode_step_next();
        }
        break;
    }
    dec_bg_draw(2);
}

void func_80141700(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        audio_bank_state_reset();
        game_mode_set(GAME_MODE_ETC_C4);
    }
    dec_bg_draw(2);
}

void func_8014175C(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "メモリーカードをチェックしています。", 0);
        k_sub_disp_start(1);
        game_state.substep += 1;
        memory_card_start();
        break;
    case 1:
        if (func_80053F04() != 0) {
            if (game_state.memory_card_result == 1) {
                memory_card_stop();
                printf("EXIST_CARD!");
                game_state.substep += 1;
            } else if (game_state.memory_card_result == 0xF1) {
                memory_card_stop();
                printf("UNFORMAT CARD!");
                mode_step_set(0xA0);
            } else if (game_state.memory_card_result == 0xFF) {
                memory_card_stop();
                printf("NO_CARD !");
                mode_step_set(0xFF);
            }
        } else if ((u32) game_state.memory_card_timer >= 0x201U) {
            memory_card_stop();
            printf("NO COMUNICATION!");
            mode_step_set(0xFF);
        }
        break;
    case 2:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 3:
        if (func_80054140() == 2) {
            game_state.substep += 1;
            memory_card_stop();
        } else if ((u32) game_state.memory_card_timer >= 0x201U) {
            mode_step_set(0xFF);
        }
        break;
    case 4:
        if ((u8) D_800E8BEE != 0) {
            mode_step_next();
        } else {
            mode_step_set(0xF0);
        }
        break;
    }
    dec_bg_draw(2);
}

void func_80141980(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, "ただいま、セーブ中です。", 0);
        k_sub_disp_start(2);
        game_state.substep += 1;
        memory_card_start();
        break;
    case 1:
        if (func_80054284() == 3) {
            memory_card_stop();
            printf("LOAD SYSTEM OK");
            game_state.substep += 1;
        } else if ((u32)game_state.memory_card_timer >= 0x201U || game_state.memory_card_result == 0xF0) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            mode_step_set(0xFD);
        } else {
            mode_step_next();
        }
        break;
    }
    dec_bg_draw(2);
}

void func_80141ABC(void) {
    s32 count;
    u8 value;
    s32 i;

    bzero(system_data.unk_058, 0x800);
    /* FAKE: retain this count-initializer/for source-line group for the
     * original preheader schedule. Original line layout unknown. T-24067 */
    count = 0; for (i = 0; i < 256; i++) {
        value = game_state.event_log[i].scene;
        printf("%d %d %d %d %d \n", game_state.event_log[i].year,
            game_state.event_log[i].month, game_state.event_log[i].day, value, value);
        if (!game_state.year || !game_state.month || !game_state.day ||
            (game_state.current_girl & 0xF) != game_state.event_log[i].girl) {
            continue;
        }
        ((EventLogEntry *)system_data.unk_058)[count] = game_state.event_log[i];
        count++;
    }
    system_data.unk_027[1] = count;
    system_data.unk_027[0] = 1;
    printf("ALBUM_MODE IN SAVE %d \n", system_data.unk_027[0]);
}

/* Retain the earlier caller's K&R implicit-int view. */
s32 func_80141BE8(void) {
    s32 girl;
    s32 *pair;

    switch (game_state.substep) {
    case 0:
        memory_card_start();
        func_80141ABC();
        game_state.substep += 1;
        break;
    case 1:
        if (func_80054388() == 0x13) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32)game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 2:
        printf("[C_GIRL]%d \n", game_state.current_girl);
        if (game_state.girl_cleared[game_state.current_girl & 0xF] & 1) {
            /* FAKE: cast stops uopt from folding the +26 word offset into lw. */
            girl = game_state.current_girl & 0xF;
            pair = (s32 *)((u32)&D_800B37D4[26]) + girl * 2;
            cd_audio_request_set(pair[0], pair[1], girl);
        } else {
            pair = D_800B37D4 + (game_state.current_girl & 0xF) * 2;
            cd_audio_request_set(pair[0], pair[1], game_state.current_girl & 0xF);
        }
        audio_command(0x300);
        game_state.substep += 1;
        break;
    case 3:
        if (cd_audio_state() & 4) {
            game_state.substep += 1;
        }
        break;
    case 4:
        if (cd_audio_state() & 2) {
            game_state.substep += 1;
        }
        break;
    case 5:
        if (game_state.screen_brightness-- < 8) {
            game_state.screen_brightness = 0;
            audio_bank_state_reset();
            game_mode_set(GAME_MODE_ETC_C4);
            game_state.current_girl += 0x10;
        }
        set_dec_bri(game_state.screen_brightness);
        break;
    }
    dec_bg_draw(2);
}

void func_80141E10(void) {
    GameState *g = &game_state;
    switch (g->substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルがありません。", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "（システムファイルがないと", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "アルバムモードのセーブはできません）", 0);
        k_sub_disp_start(3);
        g->substep += 1;
        memory_card_start();
        break;
    case 1:
        if ((u32) g->mode_step_timer.u >= 0x41U) {
            g->substep += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成し、", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "アルバムモードをセーブしますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(6);
        g->substep += 1;
        break;
    case 3:
        if (g->pad_pressed & PAD_CIRCLE) {
            g->substep += 1;
        } else if (g->pad_pressed & PAD_CROSS) {
            audio_bank_state_reset();
            game_mode_set(GAME_MODE_ETC_C4);
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成しています。", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "（アルバムモードのデータをセーブ後、", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "ファイルセレクト画面にて、", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "システムファイルを作り直してください）", 0);
        k_sub_disp_start(5);
        g->substep += 1;
        break;
    case 5:
        if (func_80054694(0) == 1) {
            g->substep += 1;
        } else {
            mode_step_set(0xFE);
        }
        break;
    case 6:
        D_800E7D36 = g->save_slot;
        *D_800E7D14 = 1;
        g->save_slot = 0;
        memory_card_start();
        func_80054E6C();
        g->substep += 1;
        break;
    case 7:
        if (func_80054388() == 0x13) {
            memory_card_stop();
            g->substep += 1;
        } else if (((u32) g->memory_card_timer >= 0x201U) || (g->memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 8:
        mode_step_set(5);
        break;
    }
    dec_bg_draw(2);
}

void func_801421AC(void) {
    u8 *state = &game_state.substep;

    switch (*state) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "メモリーカードを認識できません。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "差し込み口１のメモリーカードの", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "接続を確認してください。", 0);
        k_sub_disp_start(3);
        *state += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.u >= 0x41U) {
            *state += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "データのセーブを行ないますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(5);
        *state += 1;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            mode_step_set(4);
        } else if (game_state.pad_pressed & PAD_CROSS) {
            audio_bank_state_reset();
            game_mode_set(GAME_MODE_ETC_C4);
        }
        break;
    }
    dec_bg_draw(2);
}

void func_80142390(void) {
    u8 *state = &game_state.substep;

    switch (*state) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "メモリーカードに空きブロックがありません。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "他の空きブロックのあるメモリーカードを", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "用意してください。", 0);
        k_sub_disp_start(3);
        *state += 1;
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.u >= 0x41U) {
            *state += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "データのセーブを行ないますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(5);
        *state += 1;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            mode_step_set(4);
        } else if (game_state.pad_pressed & PAD_CROSS) {
            audio_bank_state_reset();
            game_mode_set(GAME_MODE_ETC_C4);
        }
        break;
    }
    dec_bg_draw(2);
}

void func_80142574(void) {
    GameState *g = &game_state;
    switch (g->substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルが壊れています。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "システムファイルを作り直していいですか？", 0);
        k_sub_disp_start(3);
        g->substep += 1;
        break;
    case 1:
        if ((u32) g->mode_step_timer.u >= 0x41U) {
            g->substep += 1;
        }
        break;
    case 2:
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(6);
        g->substep += 1;
        break;
    case 3:
        if (g->pad_pressed & PAD_CIRCLE) {
            k_sub_reset();
            set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成しています。", 0);
            set_kanji_string(-0x78, 0x3E, 0, (s32) "（アルバムモードのデータをセーブ後、", 0);
            set_kanji_string(-0x78, 0x4C, 0, (s32) "ファイルセレクト画面のシステムを", 0);
            set_kanji_string(-0x78, 0x5A, 0, (s32) "クリックして、システムファイルを", 0);
            set_kanji_string(-0x78, 0x68, 0, (s32) "作り直してください）", 0);
            k_sub_disp_start(5);
            g->substep += 1;
        } else if (g->pad_pressed & PAD_CROSS) {
            audio_bank_state_reset();
            game_mode_set(GAME_MODE_ETC_C4);
        }
        break;
    case 4:
        if (func_80054704(0) == 1) {
            g->substep += 1;
        } else {
            mode_step_set(0xFF);
        }
        break;
    case 5:
        if (func_80054694(0) == 1) {
            g->substep += 1;
        } else {
            mode_step_set(0xFF);
        }
        break;
    case 6:
        D_800E7D36 = g->save_slot;
        *D_800E7D14 = 1;
        g->save_slot = 0;
        func_80054E6C();
        g->substep += 1;
        break;
    case 7:
        if (func_80054388() == 0x13) {
            memory_card_stop();
            g->substep += 1;
        } else if (((u32) g->memory_card_timer >= 0x201U) || (g->memory_card_result == 0xF0)) {
            memory_card_stop();
            mode_step_set(0xFF);
        }
        break;
    case 8:
        mode_step_set(5);
        break;
    }
    dec_bg_draw(2);
}

s32 func_801428EC(void) {
    s32 color;
    u8 *shade = &game_state.screen_brightness;
    color = *shade;
    color = color * 0x10101 + 0x141414;

    prim_gradient_rect(-0x8C, 0x2C, 0x128, 0x40, 4, color, 0x81);
    dtd_on_tpage(0, 0, 4, 2, 0);
}

void func_80142974(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80143644();
        break;
    case 1:
        func_80143FE0();
        break;
    }
    if (cd_audio_state() & 4) {
        if (game_state.pad_pressed & 0x200000) {
            if (game_state.options_dirty != 0) {
                game_mode_set(GAME_MODE_OPENING);
            } else {
                game_mode_set(GAME_MODE_ETC_C4);
            }
        } else if (game_state.pad_pressed & 0x400000) {
            if (game_state.options_dirty != 0) {
                game_mode_set(GAME_MODE_OPENING);
            } else {
                game_mode_set(GAME_MODE_ALBUM);
            }
        }
    }
}
void func_80142A4C(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite_quads[i].x = i * 0x80 - 0x60;
        sprite_quads[i].flags = 0x01000000;
        sprite_quads[i].y = -0x40;
        sprite_quads[i].w = 0x80;
        /* s16 view of the u16 member: the constant is then shared with unk_08 */
        *(s16 *)&sprite_quads[i].h = 0x80;
        sprite_quads[i].unk_0C = i + 0x15;
        sprite_quads[i].unk_0E = 0;
        sprite_quads[i].unk_0F = 0;
        sprite_quads[i].unk_10 = 0;
        sprite_quads[i].unk_12 = 0x1FF;
        sprite_quads[i].tint_r = 0;
        sprite_quads[i].tint_g = 0;
        sprite_quads[i].tint_b = 0;
        sprite_quads[i].anchor_x = 0;
        sprite_quads[i].anchor_y = 0;
        sprite_quads[i].unk_1C = 0x1000;
        sprite_quads[i].unk_1E = 0x1000;
        sprite_quads[i].unk_20 = 0;
        game_state.quad_layer[i] = 0;
    }
}

void func_80142AE8(void) {
    s32 i;
    u8 v;
    u8 a;

    if (D_8015018C > 0) {
        for (i = 0; i < 2; i++) {
            v = sprite_quads[i].tint_r;
            a = v + 1;
            if (v < 0x80) {
                sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[i].tint_r = a;
            }
        }
    }
    if (D_8015018C < 0) {
        for (i = 0; i < 2; i++) {
            v = sprite_quads[i].tint_r;
            a = v - 1;
            if (v != 0) {
                sprite_quads[i].tint_g = sprite_quads[i].tint_b = sprite_quads[i].tint_r = a;
            } else {
                sprite_quads[i].flags = 0x80000000;
            }
        }
    }
}
