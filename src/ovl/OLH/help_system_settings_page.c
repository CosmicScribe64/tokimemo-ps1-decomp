/* help_system_settings_page.c: ; src/ovl/OLH/80132420.c:30-46 draws the system-settings page. Original object 0x80132420-0x80132FA0 (config/objects/OLH.txt). */
#include "common.h"
#include "ovl/OLH.h"

void func_80132420(void) {
    switch (game_state.mode_step) {
    case 0x0:
        func_801324EC();
        break;
    case 0x1:
        func_80132614();
        break;
    case 0x10:
        func_80132744();
        break;
    case 0x20:
        func_80132884();
        break;
    case 0x30:
        func_80132954();
        break;
    case 0x40:
        func_80132AE8();
        break;
    case 0x50:
        func_80132BD4();
        break;
    case 0x60:
        func_80132CDC();
        break;
    case 0x70:
        func_80132E54();
        break;
    }
}

void func_801324EC(void) {
    s32 i;
    s32 r;
    s32 pad[1]; /* FAKE: unused local above the arrays reproduces the original frame; same as func_80133C4C. T-9350 */
    s16 a[8];
    s16 b[8];
    s16 c[8];
    s16 d[8];

    k_sub_reset();
    set_kanji_string(-0x50, -0x40, 0xF, (s32)"システム設定", 0);
    for (i = 0; i < 8; i++) {
        a[i] = -0x60;
        b[i] = i * 0x10 - 0x32;
        c[i] = 0xE8;
        d[i] = 0x10;
        r = set_kanji_string(a[i], b[i], 0xF, (&D_8013A700)[i], 0);
    }
    menu_set(0, 8, a, b, c, d);
    k_disp_start(r);
    mode_step_next();
}

void func_80132614(void) {
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

s32 func_80132744(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A700, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　アイコンの位置の初期設定には、", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "　　ＴＹＰＥ‐Ａ　と　ＴＹＰＥ‐Ｂ", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "があります。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　また、アイコンの位置を自由に移動でき", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "るフリーモードもあります。", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "お好みのアイコンの配置でお楽しみ下さい。", 0);
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

s32 func_80132884(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A704, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　マウスライク…カーソルが、方向キーの", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "上下左右に反応して動きます。", 0);
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

s32 func_80132954(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A708, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　メニューＡ…メニュー選択時に方向キー", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "を押すと、メニューアイテムの順番通りに", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "カーソルが移動します。", 0);
        set_kanji_string(-0x88, 0, 0xF, "　例えば、コマンドアイコンの場合は、", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "『文系』→『理系』→『芸術』→『運動』", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "→『部活』→『遊び』→『おしゃれ』", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "→『休養』→『電話』→『デート』→", 0);
        set_kanji_string(-0x88, 0x40, 0xF, "『情報』→『システム』→『カレンダー』", 0);
        set_kanji_string(-0x88, 0x50, 0xF, "→『セーブ／ロード』→『強制』の順です。", 0);
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

s32 func_80132AE8(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A70C, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　メニューＢ…メニュー選択時に方向キー", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "を押すと、その方向のメニューアイテムに", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "カーソルが移動します。", 0);
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

s32 func_80132BD4(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A710, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　マウスライク＋メニューＡ…通常は、マ", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "ウスライクと同じ動作をします。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "Ｌ１ボタンを押している間は、メニューＡ", 0);
        set_kanji_string(-0x88, 0, 0xF, "と同じ動作をします。", 0);
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

s32 func_80132CDC(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A714, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　選択肢がある場合に１Ｐコントローラの", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "セレクトボタンを押すと、", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "メニューモードを", 0);
        set_kanji_string(-0x88, 0, 0xF, "　『メニューＡ』→", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "　『マウスライク＋メニューＡ』→", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "　『メニューＢ』→", 0);
        set_kanji_string(-0x88, 0x30, 0xF, "　『マウスライク』", 0);
        set_kanji_string(-0x88, 0x40, 0xF, "の順で変更できます。", 0);
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

s32 func_80132E54(void) {
    switch (game_state.substep) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x88, -0x40, 1, D_8013A718, 0);
        set_kanji_string(-0x88, -0x30, 0xF, "　画面上にクリアしたことのある女の子の", 0);
        set_kanji_string(-0x88, -0x20, 0xF, "マスコットを表示することができます。", 0);
        set_kanji_string(-0x88, -0x10, 0xF, "　Ｒ１ボタンを押しながら方向キー操作で", 0);
        set_kanji_string(-0x88, 0, 0xF, "位置が変更できます。", 0);
        set_kanji_string(-0x88, 0x10, 0xF, "　Ｒ２ボタンを押しながら方向キー操作で", 0);
        set_kanji_string(-0x88, 0x20, 0xF, "紐の長さの変更ができます。", 0);
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
