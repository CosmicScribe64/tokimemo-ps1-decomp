#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80137280(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "不良「そんな攻撃では、");
        func_800AE0F0(D_8015E850, "俺達の拳とは語れねぇぜ！");
        func_8014B738(D_8015E8F0, 2, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1D000007, 0x0300000C, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0E000045, 0x0B00004D, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801373A8(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "不良「口ほどにもないやつめ。");
        func_800AE0F0(D_8015E850, "出直してきやがれ！");
        func_8014B738(D_8015E8F0, 2, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E000073, 0x18000079, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1D000001, 0x02000006, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801374D0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "不良「くっ！　なかなかやるなっ！");
        func_800AE0F0(D_8015EA08, "しかたねぇ、番長様に御報告だ！");
        func_8014B738(D_8015EB70, 2, 0x104);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x0E000014, 0x0700001C, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0E000037, 0x0A000044, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014F500();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801375F8(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "？？「ちょっと待てぇーい！");
        func_8014B738(D_8015EB70, 1, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A00008E, 0x14000095, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014F500();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801376D4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「俺様は、この世界の番長だ！");
        func_800AE0F0(D_8015EA08, "子分達が随分と世話になったようだな。");
        func_800AE0F0(D_8015EA30, "だが、茶番はここまでだ！");
        func_800AE0F0(D_8015EA58, "礼をさせてもらうぞ。");
        func_800AE0F0(D_8015EA80, "これでもくらえぇーい！");
        func_800AE0F0(D_8015EAA8, "超眼力！");
        func_8014B738(D_8015EB70, 6, 0x10E);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x04000002, 0x03000011, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1A000096, 0x150000A3, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x1A0000A4, 0x160000B0, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(3);
        return;
    case 7:
        func_8013F1C4(0x1B000082, 0x0800008A, 3);
        D_8015ED8C[0] += 1;
        return;
    case 8:
        func_8013F250(4);
        return;
    case 9:
        func_8013F1C4(0x1B000095, 0x0A0000A0, 4);
        D_8015ED8C[0] += 1;
        return;
    case 10:
        func_8013F250(5);
        return;
    case 11:
        func_8013F1C4(0x190000AA, 0x140000B7, 5);
        D_8015ED8C[0] += 1;
        return;
    case 12:
        func_8013F250(0);
        return;
    case 13:
        func_8013F220();
        func_8014F500();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8013794C(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「ふっ、思い知ったか。");
        func_800AE0F0(D_8015EA08, "俺様に逆らう者の末路はこうなるのだ。");
        func_800AE0F0(D_8015EA30, "はっはっはっはっはっ・・・");
        func_8014B738(D_8015EB70, 3, 0x118);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A00005B, 0x0F000065, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0300004C, 0x0900005B, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x1A000066, 0x10000073, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(0);
        return;
    case 7:
        func_8013F220();
        func_8014F500();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80137AC8(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "天使の声「お願い、立ちあがって。");
        func_800AE0F0(D_8015EA08, "あなたには、");
        func_800AE0F0(D_8015EA30, "まだやるべきことがあるのよ！");
        func_8014B738(D_8015EB70, 3, 0xE6);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x05000013, 0x0300001C, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1F000081, 0x17000086, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x0500001D, 0x04000027, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(0);
        return;
    case 7:
        func_8013F220();
        func_8014F500();
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80137C44(void) {
    if (func_8013E90C(0x30) != 0) {
        func_8013E7C0(0x30, 0, 1, 1);
    }
    if (D_8015ED8C[0] >= 4) {
        if (D_8015ED94 != D_8015EDA0) {
            func_8013F15C(0x504, 2, 5);
        }
        D_8015ED94 += 9;
        if (D_8015EDA0 < D_8015ED94) {
            D_8015ED94 = D_8015EDA0;
        }
    }
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "そうよ、あなたにならできるわ！");
        func_800AE0F0(D_8015EA08, "新しい力をあげる。");
        func_800AE0F0(D_8015EA30, "彼女を・・・守ってあげて！");
        func_8014B738(D_8015EB70, 3, 0xF0);
        D_8015ED8C[0] += 1;
        break;
    case 1:
        func_8013F1C4(0x05000028, 0x05000035, 0);
        D_8015ED8C[0] += 1;
        break;
    case 2:
        func_8013F250(1);
        break;
    case 3:
        func_8013F1C4(0x06000003, 0x0400000C, 1);
        D_8015ED8C[0] += 1;
        break;
    case 4:
        func_8013F250(2);
        break;
    case 5:
        func_8013F1C4(0x0600000D, 0x05000017, 2);
        D_8015ED8C[0] += 1;
        break;
    case 6:
        func_8013F250(0);
        break;
    case 7:
        if (D_8015ED94 == D_8015EDA0) {
            func_8013F220();
            func_8013E7C0(0x30, 0, 1, 1);
            func_8014F500();
        }
        break;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80137E74(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「なにぃぃぃー！？");
        func_8014B738(D_8015EB70, 1, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A000074, 0x1100007A, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014F500();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80137F50(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「こわっぱが、");
        func_800AE0F0(D_8015EA08, "超眼力を見切った程度で図に乗りおって");
        func_800AE0F0(D_8015EA30, "俺様の真の力を見せてくれる。");
        func_800AE0F0(D_8015EA58, "ゆくぞーっ！");
        func_8014B738(D_8015EB70, 4, 0x118);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A00007B, 0x1200007F, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1A000080, 0x1300008D, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x0300005C, 0x0A00006A, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(3);
        return;
    case 7:
        func_8013F1C4(0x1E00009F, 0x1D0000A4, 3);
        D_8015ED8C[0] += 1;
        return;
    case 8:
        func_8013F250(0);
        return;
    case 9:
        func_8013F220();
        func_8014F500();
        return;
    }
}

void func_80138120(void) {
    switch (D_8015EDD8) {
    case 0:
        func_80137280();
        return;
    case 1:
        func_8014EF8C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138170);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138324);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_801387F0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138B9C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138F6C);

void func_80139460(void) {
    switch (D_8015EDD8) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8013E7C0(0x30, 7, 1, 1);
        func_8004500C(1, 0x203);
        func_8014EF3C();
        return;
    case 1:
        func_8014F080(0x78);
        return;
    case 2:
        func_801373A8();
        return;
    case 3:
        D_8015EC14 = 1;
        func_80042808();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80139510);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B6F8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B708);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B718);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80137280.rodata", D_8015B728);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80139E88);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_8013A034);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_8013A2F4);

void func_8013A464(s8 a0, s8 a1) {
    D_80121353 = a0;
    D_80121317 = a1;
}
