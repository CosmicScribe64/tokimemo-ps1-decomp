#include "common.h"
#include "ovl/OLH.h"

void func_80132420(void) {
    switch (D_800E6280.unk_110A) {
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

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_801324EC);

void func_80132614(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E6280.unk_1093 != -1) {
            func_80044750(0x501);
            switch (*(u8 *)&D_800E6280.unk_1093) {
            case 0:
                func_80042940(0x10);
                break;
            case 1:
                func_80042940(0x20);
                break;
            case 2:
                func_80042940(0x30);
                break;
            case 3:
                func_80042940(0x40);
                break;
            case 4:
                func_80042940(0x50);
                break;
            case 5:
                func_80042940(0x60);
                break;
            case 6:
                func_80042940(0x70);
                break;
            case 7:
                func_80042908(0);
                break;
            }
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
    }
}

s32 func_80132744(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A700, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　アイコンの位置の初期設定には、", 0);
        func_8004E788(-0x88, -0x20, 0xF, "　　ＴＹＰＥ‐Ａ　と　ＴＹＰＥ‐Ｂ", 0);
        func_8004E788(-0x88, -0x10, 0xF, "があります。", 0);
        func_8004E788(-0x88, 0, 0xF, "　また、アイコンの位置を自由に移動でき", 0);
        func_8004E788(-0x88, 0x10, 0xF, "るフリーモードもあります。", 0);
        func_8004E788(-0x88, 0x20, 0xF, "お好みのアイコンの配置でお楽しみ下さい。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132884(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A704, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　マウスライク…カーソルが、方向キーの", 0);
        func_8004E788(-0x88, -0x20, 0xF, "上下左右に反応して動きます。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132954(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A708, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　メニューＡ…メニュー選択時に方向キー", 0);
        func_8004E788(-0x88, -0x20, 0xF, "を押すと、メニューアイテムの順番通りに", 0);
        func_8004E788(-0x88, -0x10, 0xF, "カーソルが移動します。", 0);
        func_8004E788(-0x88, 0, 0xF, "　例えば、コマンドアイコンの場合は、", 0);
        func_8004E788(-0x88, 0x10, 0xF, "『文系』→『理系』→『芸術』→『運動』", 0);
        func_8004E788(-0x88, 0x20, 0xF, "→『部活』→『遊び』→『おしゃれ』", 0);
        func_8004E788(-0x88, 0x30, 0xF, "→『休養』→『電話』→『デート』→", 0);
        func_8004E788(-0x88, 0x40, 0xF, "『情報』→『システム』→『カレンダー』", 0);
        func_8004E788(-0x88, 0x50, 0xF, "→『セーブ／ロード』→『強制』の順です。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132AE8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A70C, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　メニューＢ…メニュー選択時に方向キー", 0);
        func_8004E788(-0x88, -0x20, 0xF, "を押すと、その方向のメニューアイテムに", 0);
        func_8004E788(-0x88, -0x10, 0xF, "カーソルが移動します。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132BD4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A710, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　マウスライク＋メニューＡ…通常は、マ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ウスライクと同じ動作をします。", 0);
        func_8004E788(-0x88, -0x10, 0xF, "Ｌ１ボタンを押している間は、メニューＡ", 0);
        func_8004E788(-0x88, 0, 0xF, "と同じ動作をします。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132CDC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A714, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　選択肢がある場合に１Ｐコントローラの", 0);
        func_8004E788(-0x88, -0x20, 0xF, "セレクトボタンを押すと、", 0);
        func_8004E788(-0x88, -0x10, 0xF, "メニューモードを", 0);
        func_8004E788(-0x88, 0, 0xF, "　『メニューＡ』→", 0);
        func_8004E788(-0x88, 0x10, 0xF, "　『マウスライク＋メニューＡ』→", 0);
        func_8004E788(-0x88, 0x20, 0xF, "　『メニューＢ』→", 0);
        func_8004E788(-0x88, 0x30, 0xF, "　『マウスライク』", 0);
        func_8004E788(-0x88, 0x40, 0xF, "の順で変更できます。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}

s32 func_80132E54(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013A718, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　画面上にクリアしたことのある女の子の", 0);
        func_8004E788(-0x88, -0x20, 0xF, "マスコットを表示することができます。", 0);
        func_8004E788(-0x88, -0x10, 0xF, "　Ｒ１ボタンを押しながら方向キー操作で", 0);
        func_8004E788(-0x88, 0, 0xF, "位置が変更できます。", 0);
        func_8004E788(-0x88, 0x10, 0xF, "　Ｒ２ボタンを押しながら方向キー操作で", 0);
        func_8004E788(-0x88, 0x20, 0xF, "紐の長さの変更ができます。", 0);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80052000();
        return;
    case 2:
        func_80042940(0);
        return;
    }
}
