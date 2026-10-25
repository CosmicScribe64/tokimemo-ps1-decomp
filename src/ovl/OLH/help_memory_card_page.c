/* help_memory_card_page.c: ; src/ovl/OLH/80132FA0.c:30-47 draws the memory-card page. Original object 0x80132FA0-0x80133B80 (config/objects/OLH.txt). */
#include "common.h"
#include "ovl/OLH.h"

void help_memory_card_topic_dispatch(void) {
    switch (game_state.mode_step) {
    case 0x0:
        help_memory_card_menu_draw();
        break;
    case 0x1:
        help_memory_card_menu_input();
        break;
    case 0x10:
        help_memory_card_page_1_render();
        break;
    case 0x20:
        help_memory_card_page_2_render();
        break;
    case 0x30:
        help_memory_card_page_3_render();
        break;
    case 0x40:
        help_memory_card_page_4_render();
        break;
    case 0x50:
        help_memory_card_page_5_render();
        break;
    case 0x60:
        help_memory_card_page_6_render();
        break;
    case 0x70:
        help_memory_card_page_7_render();
        break;
    }
}

void help_memory_card_menu_draw(void) {
    s32 i;
    s32 r;
    s32 padx[1]; /* FAKE: unused words above and below the arrays reproduce the original frame 0xC0 and array offsets; real source unknown. T-9310 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];
    s32 pady[1];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"メモリーカード", 0);
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        if (i < 4) {
            set_kanji_string(a[i], b[i], 0xF, (&D_8013AF20)[i], 0);
        }
    }
    c[4] = c[5] = c[6] = 0;
    r = set_kanji_string(a[7], b[7], 0xF, D_8013AF3C, 0);
    menu_set(0, 8, a, b, c, d);
    k_disp_start(r);
    mode_step_next();
}

void help_memory_card_menu_input(void) {
    menu_check(0, cursor_x, cursor_y);
    menu_bar_show(0);
    menu_cursor_update(0);
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (game_state.menu_cursor_0 != -1) {
            audio_command(0x501);
            switch (*(u8 *)&game_state.menu_cursor_0) {
            case 0:
                mode_step_set(0x10);
                break;
            case 1:
                mode_step_set(0x20);
                break;
            case 2:
                mode_step_set(0x30);
                break;
            case 3:
                mode_step_set(0x40);
                break;
            case 4:
                mode_step_set(0x50);
                break;
            case 5:
                mode_step_set(0x60);
                break;
            case 6:
                mode_step_set(0x70);
                break;
            case 7:
                mode_stage_set(0);
                break;
            }
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_stage_set(0);
    }
}

s32 help_memory_card_page_1_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF20, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　ときめきメモリアルでは、１ブロックに", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "一つのアルバムを作成します。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　アルバムの最大数は、１４です。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_2_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF24, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　アルバムとは別に、システムファイルを", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "作成すると、オプションでおまけが使用で", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "きるようになります。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　システムファイルも、１ブロックを必要", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "とします。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_3_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF28, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　システムファイルがある場合、メモリー", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "カードアイコンが、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　　『セーブ中』", 0);
        set_kanji_string(-0x88, 0, 0xF, "と表示されている間は、絶対にメモリーカ", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "ードの抜き差しをしないでください。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_4_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF2C, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "『カードに空きがありません』", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "とのメッセージが表示された場合は、メモ", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "リーカードの他のデータを消去してから、", 0);
        set_kanji_string(-0x88, 0, 0xF, "セーブして下さい。", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "　ゲーム中は、ときめきメモリアル以外の", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "データの消去はできません。", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_5_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF30, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "い", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "う", 0);
        set_kanji_string(-0x88, 0, 0xF, "え", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "お", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "か", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "き", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_6_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF34, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "い", 0);
        set_kanji_string(-0x88, 0, 0xF, "う", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "え", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "お", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "か", 0);
        set_kanji_string(-0x88, 0x40, 0xF, "き", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}

s32 help_memory_card_page_7_render(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013AF38, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "あ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "い", 0);
        set_kanji_string(-0x88, 0, 0xF, "う", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "え", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "お", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "か", 0);
        set_kanji_string(-0x88, 0x40, 0xF, "き", 0);
        game_state.substep += 1;
        return;
    case 1:
        text_wait_confirm();
        return;
    case 2:
        mode_step_set(0);
        return;
    }
}
