#include "common.h"
#include "ovl/OLH.h"

void func_80132FA0(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_8013306C();
        break;
    case 0x1:
        func_801331C8();
        break;
    case 0x10:
        func_801332F8();
        break;
    case 0x20:
        func_801333E4();
        break;
    case 0x30:
        func_80133508();
        break;
    case 0x40:
        func_8013362C();
        break;
    case 0x50:
        func_8013376C();
        break;
    case 0x60:
        func_801338C8();
        break;
    case 0x70:
        func_80133A24();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132FA0", func_8013306C);

void func_801331C8(void) {
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

s32 func_801332F8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF20, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　ときめきメモリアルでは、１ブロックに", 0);
        func_8004E788(-0x88, -0x20, 0xF, "一つのアルバムを作成します。", 0);
        func_8004E788(-0x88, -0x10, 0xF, "　アルバムの最大数は、１４です。", 0);
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

s32 func_801333E4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF24, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　アルバムとは別に、システムファイルを", 0);
        func_8004E788(-0x88, -0x20, 0xF, "作成すると、オプションでおまけが使用で", 0);
        func_8004E788(-0x88, -0x10, 0xF, "きるようになります。", 0);
        func_8004E788(-0x88, 0, 0xF, "　システムファイルも、１ブロックを必要", 0);
        func_8004E788(-0x88, 0x10, 0xF, "とします。", 0);
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

s32 func_80133508(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF28, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　システムファイルがある場合、メモリー", 0);
        func_8004E788(-0x88, -0x20, 0xF, "カードアイコンが、", 0);
        func_8004E788(-0x88, -0x10, 0xF, "　　『セーブ中』", 0);
        func_8004E788(-0x88, 0, 0xF, "と表示されている間は、絶対にメモリーカ", 0);
        func_8004E788(-0x88, 0x10, 0xF, "ードの抜き差しをしないでください。", 0);
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

s32 func_8013362C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF2C, 0);
        func_8004E788(-0x88, -0x30, 0xF, "『カードに空きがありません』", 0);
        func_8004E788(-0x88, -0x20, 0xF, "とのメッセージが表示された場合は、メモ", 0);
        func_8004E788(-0x88, -0x10, 0xF, "リーカードの他のデータを消去してから、", 0);
        func_8004E788(-0x88, 0, 0xF, "セーブして下さい。", 0);
        func_8004E788(-0x88, 0x10, 0xF, "　ゲーム中は、ときめきメモリアル以外の", 0);
        func_8004E788(-0x88, 0x20, 0xF, "データの消去はできません。", 0);
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

s32 func_8013376C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF30, 0);
        func_8004E788(-0x88, -0x30, 0xF, "あ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "い", 0);
        func_8004E788(-0x88, -0x10, 0xF, "う", 0);
        func_8004E788(-0x88, 0, 0xF, "え", 0);
        func_8004E788(-0x88, 0x10, 0xF, "お", 0);
        func_8004E788(-0x88, 0x20, 0xF, "か", 0);
        func_8004E788(-0x88, 0x30, 0xF, "き", 0);
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

s32 func_801338C8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF34, 0);
        func_8004E788(-0x88, -0x30, 0xF, "あ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "い", 0);
        func_8004E788(-0x88, 0, 0xF, "う", 0);
        func_8004E788(-0x88, 0x10, 0xF, "え", 0);
        func_8004E788(-0x88, 0x20, 0xF, "お", 0);
        func_8004E788(-0x88, 0x30, 0xF, "か", 0);
        func_8004E788(-0x88, 0x40, 0xF, "き", 0);
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

s32 func_80133A24(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013AF38, 0);
        func_8004E788(-0x88, -0x30, 0xF, "あ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "い", 0);
        func_8004E788(-0x88, 0, 0xF, "う", 0);
        func_8004E788(-0x88, 0x10, 0xF, "え", 0);
        func_8004E788(-0x88, 0x20, 0xF, "お", 0);
        func_8004E788(-0x88, 0x30, 0xF, "か", 0);
        func_8004E788(-0x88, 0x40, 0xF, "き", 0);
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
