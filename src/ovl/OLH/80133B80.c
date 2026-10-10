#include "common.h"
#include "ovl/OLH.h"

void func_80133B80(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80133C4C();
        break;
    case 0x1:
        func_80133D74();
        break;
    case 0x10:
        func_80133EA4();
        break;
    case 0x20:
        func_80133F90();
        break;
    case 0x30:
        func_80134060();
        break;
    case 0x40:
        func_80134184();
        break;
    case 0x50:
        func_80134254();
        break;
    case 0x60:
        func_80134324();
        break;
    case 0x70:
        func_801343F4();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80133C4C);

void func_80133D74(void) {
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

s32 func_80133EA4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013B740, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　画面の中央が現在の位置です。", 0);
        func_8004E788(-0x88, -0x20, 0xF, "左が過去一週間、右がこの先一週間のあな", 0);
        func_8004E788(-0x88, -0x10, 0xF, "たのバイオリズムです。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80133F90(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013B744, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　画面中央の黒い線と交差する日が、コマ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ンドが特に失敗しやすい日になります。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_80134060);

s32 func_80134184(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013B74C, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　赤は主に、身体を使うコマンドに影響し", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ます。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80134254(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013B750, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　青は、主に感性を必要とするコマンドに", 0);
        func_8004E788(-0x88, -0x20, 0xF, "影響します。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

s32 func_80134324(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013B754, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　緑は、主に頭脳を必要とするコマンドに", 0);
        func_8004E788(-0x88, -0x20, 0xF, "影響します。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80133B80", func_801343F4);
