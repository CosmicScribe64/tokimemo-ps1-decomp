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

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132744);

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

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132954);

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

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132BD4);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132CDC);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80132420", func_80132E54);
