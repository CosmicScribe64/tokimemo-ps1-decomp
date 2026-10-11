#include "common.h"
#include "ovl/OLH.h"

void func_801364E0(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_801365AC();
        break;
    case 0x1:
        func_801366D4();
        break;
    case 0x10:
        func_80136804();
        break;
    case 0x20:
        func_80136960();
        break;
    case 0x30:
        func_80136A30();
        break;
    case 0x40:
        func_80136B00();
        break;
    case 0x50:
        func_80136BD0();
        break;
    case 0x60:
        func_80136CF4();
        break;
    case 0x70:
        func_80136E18();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/801364E0", func_801365AC);

void func_801366D4(void) {
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

s32 func_80136804(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6C0, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　システムと表示されている部分をクリッ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "クすると、システムの作成やシステムの作", 0);
        func_8004E788(-0x88, -0x10, 0xF, "り直しが出来ます。", 0);
        func_8004E788(-0x88, 0, 0xF, "　データの表示がおかしい場合や、他のメ", 0);
        func_8004E788(-0x88, 0x10, 0xF, "モリーカードからデータをコピーした場合", 0);
        func_8004E788(-0x88, 0x20, 0xF, "は、システムの作り直しを行なってくださ", 0);
        func_8004E788(-0x88, 0x30, 0xF, "い。", 0);
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

s32 func_80136960(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6C4, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　同じメモリーカード内で、データをコピ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ーすることができます。", 0);
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

s32 func_80136A30(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6C8, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　クリア人数そのままで、ゲームをプロロ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ーグからやり直すことが出来ます。", 0);
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

s32 func_80136B00(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6CC, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　ときめきメモリアルのデータを消去する", 0);
        func_8004E788(-0x88, -0x20, 0xF, "ことが出来ます。", 0);
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

s32 func_80136BD0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6D0, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　システムファイルがある場合は、全ての", 0);
        func_8004E788(-0x88, -0x20, 0xF, "アルバムのデータの合計クリア人数が表示", 0);
        func_8004E788(-0x88, -0x10, 0xF, "されます。", 0);
        func_8004E788(-0x88, 0, 0xF, "　システムファイルがない場合は、選んだ", 0);
        func_8004E788(-0x88, 0x10, 0xF, "アルバムのクリア人数が表示されます。", 0);
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

s32 func_80136CF4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6D4, 0);
        func_8004E788(-0x88, -0x30, 0xF, "白抜き数字で番号が表示されているのが、", 0);
        func_8004E788(-0x88, -0x20, 0xF, "現在選択されているアルバムです。", 0);
        func_8004E788(-0x88, -0x10, 0xF, "白抜き数字で番号が表示されているアルバ", 0);
        func_8004E788(-0x88, 0, 0xF, "ムをもう一度クリックすると、そのアルバ", 0);
        func_8004E788(-0x88, 0x10, 0xF, "ムでゲームが再開されます。", 0);
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

s32 func_80136E18(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013D6D8, 0);
        func_8004E788(-0x88, -0x30, 0xF, "　△ボタンを押すと、そのアルバムのパラ", 0);
        func_8004E788(-0x88, -0x20, 0xF, "メータや、女の子の評価、クリア回数が表", 0);
        func_8004E788(-0x88, -0x10, 0xF, "示されます。", 0);
        func_8004E788(-0x88, 0, 0xF, "　青くパラメータと表示されている部分を", 0);
        func_8004E788(-0x88, 0x10, 0xF, "クリックしても、表示を変えることができ", 0);
        func_8004E788(-0x88, 0x20, 0xF, "ます。", 0);
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
