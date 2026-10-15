#include "common.h"
#include "ovl/NAME_ENT.h"

/* bulk_m2c declarations (T-11190) */
extern s8 D_800B3D00;
extern s8 D_800E62BA;
extern s32 NULL;
/* end bulk_m2c declarations */
void func_80142A80(void) {
    u8 sp43;
    s32 sp3C;
    u8 sp34;

    D_800E62BA = 0x80;
    sp43 = func_8004ADD4();
    func_8004ADAC(0);
    func_80048DD0(0);
    func_8006C760(1);
    func_8006BD6C(0);
    back_clear_switch(1);
    func_8004E58C();
    sp3C = get_k_speed();
    k_speed_set(3U);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "　　　　　", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "好きとか　嫌いとか", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "最初に言い出したのは　誰なのかしら", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "駆け抜けてゆく　私のメモリアル", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "今日も鏡のまえで　髪をとかして　", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "ピンクのリップは　Ｓｗｅｅｔ　Ｍａｇｉｃ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "とっておきのコロン", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "さ・さ・や・き　キ・ラ・メ・キ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "ド・キ・ド・キ　大好き", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "あなただけに見つめてほしい", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "Ｔａｋｅ　ｔｈｅ　Ｃｈａｎｃｅ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "仕上げは上出来", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "Ｔｅｌｌ　Ｙｏｕｒ　Ｈｅａｒｔ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "準備はＯＫ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "素敵な\x97\\感　そよ風に乗せ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "Ｔｒｙ　Ｙｏｕｒ　ＬＵＣＫ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "制服のリボン", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "Ｔｅｌｌ　Ｙｏｕｒ　Ｌｏｖｅ", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "結び直したら", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "今日こそ言えそう", 0);
    set_kanji_string(-0x88, 0x5C, 8, (s32) "Ｌｏｖｅ　ｍｅ　ｐｌｅａｓｅ‥‥‥", 0);
    k_disp_start(0);
    set_movie_offset(0x18, 0x10);
    func_80056BA8(0, 0x33B, 0x820);
    func_80088070();
    sp34 = 0x80;
    func_80045414(0xE, 0, &sp34);
    func_80045414(9, 0, NULL);
    func_80045414(0xA, 0, NULL);
    func_80053650(0x80167000, 0x80162000, 0x80162000);
    func_8009C210(1);
    func_80041168(0);
    func_8009C674(0);
    func_80048DD0(0);
    main_state_set(0x12);
    func_8004ADAC((s32) sp43);
    k_speed_set((u8) sp3C);
    func_8006C760(0);
    func_8006BC28(0);
    func_8006BD6C(0);
    D_800B3CF8 = 0;
    D_800B3CFC = 0;
    D_800B3D00 = 0;
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142E24);

void func_80143498(void) {
    if ((func_800460EC() & 2) && D_8014D170 == 0) {
        func_80046290(0x06000CBB, 0x12000E0C, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        func_8004E788(-0x90, 0x14, 0, "ここ、きらめき高校には、", 0);
        func_8004E788(-0x90, 0x24, 0, "一つの伝説があります。", 0);
        func_8004E788(-0x90, 0x34, 0, "校庭のはずれにある一本の古木。", 0);
        func_8004E884(3);
        func_8004E93C(0, 0);
        func_8004E93C(1, 0);
        func_8004E93C(2, 0);
        D_8014D170 = 1;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);

void func_80144654(void) {
    if (D_800E6280.unk_F80 & 0x10) {
        k_disp_switch(0, 1);
        k_disp_switch(1, 1);
        k_disp_switch(2, 1);
        k_disp_switch(3, 1);
        func_80049A40(-0x9C, 8, 0x120, 0x58, 2, 0x808080, 0x83);
        dtd_on_tpage(0, 0, 2, 0, 0);
        return;
    }
    k_disp_switch(0, 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_disp_switch(3, 0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_8014472C);

void func_80144908(void) {
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

void func_801449DC(void) {
    func_8004AC18(0xFF);
    func_800452C4();
    func_80041168(0);
    func_80041584();
    func_80048DD0(0);
    func_80044750(0x71);
    func_80044750(0x7F);
    func_80042808();
}

void func_80144A34(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80142E24();
        break;
    case 1:
        func_80143580();
        break;
    case 2:
        func_801449DC();
        break;
    }
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(2);
    }
}
