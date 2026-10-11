#include "common.h"
#include "ovl/OLH.h"

void func_80135AC0(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80135B8C();
        break;
    case 0x1:
        func_80135CB4();
        break;
    case 0x10:
        func_80135DD4();
        break;
    case 0x20:
        func_80135EC0();
        break;
    case 0x30:
        func_80136000();
        break;
    case 0x40:
        func_80136124();
        break;
    case 0x50:
        func_80136210();
        break;
    case 0x60:
        func_801362C4();
        break;
    case 0x70:
        func_801363CC();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80135AC0", func_80135B8C);

void func_80135CB4(void) {
    func_8004F984(0, D_8011ECF6, D_8011ECFA);
    func_80050B54(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E6280.unk_1093 != -1) {
            func_80044750(0x501);
            /* FAKE: lbu view of the s8 field (the compare above is lb); original declaration unknown. T-9180 */
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
                func_80042940(0x60);
                break;
            case 5:
                func_80042940(0x70);
                break;
            case 6:
                func_80042908(0);
                break;
            }
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042908(0);
    }
}

s32 func_80135DD4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEA0, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　Ｌ１＋Ｒ１＋セレクト＋スタートボタン", 0);
        func_8004E788(-0x88, -0x20, 0xF, "を少しの間押し続けることにより、ソフト", 0);
        func_8004E788(-0x88, -0x10, 0xF, "ウェアリセットがかけられます。", 0);
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

s32 func_80135EC0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEA4, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　２Ｐコントローラの", 0);
        func_8004E788(-0x88, -0x20, 0xF, "　　○＋×＋□＋△", 0);
        func_8004E788(-0x88, -0x10, 0xF, "ボタンを同時に押すと、１Ｐコントローラ", 0);
        func_8004E788(-0x88, 0, 0xF, "の○ボタンが連射される状態になります。", 0);
        func_8004E788(-0x88, 0x10, 0xF, "　連射の解除は、１Ｐコントローラの任意", 0);
        func_8004E788(-0x88, 0x20, 0xF, "のボタンを押すことによってできます。", 0);
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

s32 func_80136000(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEA8, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　２Ｐコントローラのスタートボタンを押", 0);
        func_8004E788(-0x88, -0x20, 0xF, "すと、通常コマンド実行のＳＤキャラの、", 0);
        func_8004E788(-0x88, -0x10, 0xF, "アニメスピードが少し速くなります。", 0);
        func_8004E788(-0x88, 0, 0xF, "　もう一度押すと、通常のスピードに戻り", 0);
        func_8004E788(-0x88, 0x10, 0xF, "ます。", 0);
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

s32 func_80136124(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEAC, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　デートなどで、場所を知りたいときは１", 0);
        func_8004E788(-0x88, -0x20, 0xF, "Ｐコントローラの△ボタンで、", 0);
        func_8004E788(-0x88, -0x10, 0xF, "場所ウィンドウを呼び出すことができます。", 0);
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

s32 func_80136210(void) {
    switch (D_800E6280.unk_110D) {                           /* irregular */
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEB0, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　歌詞が出ないときは高解像モードです。", 0);
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

s32 func_801362C4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEB0, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　普通の漢和辞典のように、部首別に漢字", 0);
        func_8004E788(-0x88, -0x20, 0xF, "がならんでいます。", 0);
        func_8004E788(-0x88, -0x10, 0xF, "　また、一部の漢字は訓読みでも登録され", 0);
        func_8004E788(-0x88, 0, 0xF, "ています。", 0);
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

s32 func_801363CC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013CEB4, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　極まれに、サウンドの転送に失敗するこ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "とがありますが、一旦データをセーブし、", 0);
        func_8004E788(-0x88, -0x10, 0xF, "ゲームを再立ち上げすれば、正常に復帰し", 0);
        func_8004E788(-0x88, 0, 0xF, "ます。", 0);
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
