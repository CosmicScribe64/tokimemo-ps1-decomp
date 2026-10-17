#define RPG_BAT_OVERRIDE_func_8013E7C0 /* implicit int view: the call's $v0 moves the D_8015EC74 counter to $v1 (func_801387F0, T-17103) */
#define RPG_BAT_OVERRIDE_D_8015EE8C /* pointer view: func_8013A034 reads the asset table through it (header: s32, defined in 8014E780.c) */
#define MAIN_API_OVERRIDE_D_80121317 /* stores 0x80 as a positive constant (main_api.h: s8) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern u8 D_80121317;
s32 func_8013E7C0();

extern s32 *D_8015EE8C;

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

/* FAKE: unsigned lvalue view reproduces the original actor-index promotion;
 * the shared overlay header retains older matched signed users. T-9760 */
void func_80138170(void) {
    switch (D_8015EDD8) {
    case 0:
        switch ((*(u32 *)&D_8015EBAC)) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        func_8014B738(D_8015E8F0, 2, 0x3C);
        func_8013E810((*(u32 *)&D_8015EBAC) + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F0F8((*(u32 *)&D_8015EBAC) + 0x3B);
        return;
    case 2:
        func_8013E810((*(u32 *)&D_8015EBAC) + 0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8014F178(2);
        return;
    case 4:
        func_8014C360(0, 0x104, 0x66);
        func_8014EF3C();
        return;
    case 5:
        func_8014F178(0);
        return;
    case 6:
        func_8014F080(0x14);
        return;
    case 7:
        func_8014EF8C();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80137280", func_80138324);

/* FAKE: pad1/pad2 are unused locals that reproduce the 16 extra frame bytes above sp34; the
 * unsigned lvalue view of D_8015EBAC is the actor-index promotion of func_80138170 (T-9760);
 * func_8013E7C0 is called with the implicit int view (file-level override). T-17103 */
void func_801387F0(void) {
    s32 pad1;
    s32 pad2;
    s32 sp34;

    switch (D_8015EDD8) {
    case 0:
        switch ((*(u32 *) &D_8015EBAC)) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        func_8014D11C(D_8015E878, D_800E6280.unk_0D4, "は", D_8015EBB0, "のダメージを受けた");
        func_8014B738(D_8015E8F0, 3, 0x3C);
        func_8013E810((*(u32 *) &D_8015EBAC) + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F0F8((*(u32 *) &D_8015EBAC) + 0x3B);
        return;
    case 2:
        func_8013E810((*(u32 *) &D_8015EBAC) + 0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8014F178(1);
        return;
    case 4:
        if (D_8015ED98 & 0x100) {
            func_8013E7C0(0x30, 3, 1, 1);
        } else {
            func_8013E7C0(0x30, 5, 1, 1);
        }
        D_80121531 = 3;
        func_8013E7C0(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp34 = func_8013E9A8(0x14);
        func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        return;
    case 5:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            sp34 = func_8013E9A8(0x14);
            func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
            func_8013E7C0(0x38, 0xC, 5, 1);
            D_8015EC74 -= 1;
            if (D_8015EC74 == 0) {
                func_8014EF3C();
                return;
            }
        }
    default:
        return;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 7:
        func_8014F178(2);
        return;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            func_8014EF8C();
            D_8015EDC4 = 0x100007;
            return;
        }
        func_8014EF3C();
        return;
    case 9:
        func_8014F178(0);
        return;
    case 10:
        func_8014F080(0x14);
        return;
    case 11:
        D_8015ED98 &= ~0x1000;
        func_8014EF8C();
        break;
    }
}

/* FAKE: pad1/pad2, the u32 view of D_8015EBAC and the implicit int func_8013E7C0 are the same
 * three tricks as in func_801387F0 (this function is its sibling). T-17103 */
void func_80138B9C(void) {
    s32 pad1;
    s32 pad2;
    s32 sp34;

    switch (D_8015EDD8) {
    case 0:
        switch ((*(u32 *) &D_8015EBAC)) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        func_8014D11C(D_8015E878, D_800E6280.unk_0D4, "は", D_8015EBB0, "のダメージを受けた");
        func_8014B738(D_8015E8F0, 3, 0x3C);
        func_8013E810((*(u32 *) &D_8015EBAC) + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F0F8((*(u32 *) &D_8015EBAC) + 0x3B);
        return;
    case 2:
        func_8013E810((*(u32 *) &D_8015EBAC) + 0x3B, 0, 1, 1);
        func_8014EF3C();
        return;
    case 3:
        func_8014F178(1);
        return;
    case 4:
        func_8013E7C0(0x30, 4, 1, 1);
        D_8015ED98 |= 0x1000;
        if (D_8015ED98 & 0x10000) {
            D_8015ED9C = 0;
            D_8015ED98 &= ~0x10000;
            D_8015EE50.unk_00 = 4;
        }
        D_80121531 = 3;
        func_8013E7C0(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp34 = func_8013E9A8(0x14);
        func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        return;
    case 5:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            sp34 = func_8013E9A8(0x14);
            func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
            func_8013E7C0(0x38, 0xC, 5, 1);
            D_8015EC74 -= 1;
            if (D_8015EC74 == 0) {
                func_8014EF3C();
                return;
            }
        }
    default:
        return;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
            return;
        }
        break;
    case 7:
        func_8014F178(2);
        return;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            func_8014EF8C();
            D_8015EDC4 = 0x100007;
            return;
        }
        func_8014EF3C();
        return;
    case 9:
        func_8014F178(0);
        return;
    case 10:
        func_8014F080(0x14);
        return;
    case 11:
        D_8015ED98 = D_8015ED98 & ~0x1000 & 0xFFFFFF;
        func_8014EF8C();
        break;
    }
}

void func_80138F6C(void) {
    /* FAKE: two unused words reproduce the original 0x40-byte frame and spill home. T-20003. */
    s32 frame_pad[2];
    s32 random_value;
    s32 remaining;

    switch (D_8015EDD8) {
    case 0:
        /* FAKE: this explicit word view keeps the selector in v1 instead of v0.
         * The original source spelling is unknown. T-20003. */
        switch (*(s32 *)&D_8015EBAC) {
        case 0:
            func_800AE0F0(D_8015E828, "不良Ａの攻撃");
            break;
        case 1:
            func_800AE0F0(D_8015E828, "不良Ｂの攻撃");
            break;
        case 2:
            func_800AE0F0(D_8015E828, "不良Ｃの攻撃");
            break;
        }
        func_800AE0F0(D_8015E850, "不良パンチ");
        func_800AE0F0(D_8015E878, D_800E6280.unk_0D4);
        func_800AE100(D_8015E878, "は");
        func_800AE100(D_8015E878, girl_short_name_table[D_8015EB9C]);
        func_800AE100(D_8015E878, "をかばった");
        func_8014D11C((s32) D_8015E8A0, (s32) D_800E6280.unk_0D4, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 4, 0x3C);
        func_8013E810(D_8015EBAC + 0x3B, 1, 5, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF3C();
    default:
        break;
    case 1:
        func_8014F0F8(D_8015EBAC + 0x3B);
        break;
    case 2:
        func_8014F178(1);
        break;
    case 3:
        func_8013E810(D_8015EBAC + 0x3B, 0, 1, 1);
        func_8014EF3C();
        break;
    case 4:
        D_8015EDE0 += 1;
        func_8013E97C(0x30, (s32) (D_8015EDE0 * -0x18) / 4, (s32) (D_8015EDE0 * 0x30) / 4);
        if (D_8015EDE0 == 4) {
            func_8014EF3C();
        }
        break;
    case 5:
        func_8013E7C0(0x30, 5, 1, 1);
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 3;
        func_8013E7C0(0x38, 0xC, 5, 1);
        D_8015EC74 = func_8013E9A8(4) + 1;
        random_value = func_8013E9A8(0x14);
        func_8013E97C(0x38, random_value - 0x1D, func_8013E9A8(0x14) + 0x2B);
        func_8013F15C(0x506, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        if (func_8013E90C(0x38) != 0) {
            func_8013F15C(0x506, 1, 0);
            random_value = func_8013E9A8(0x14);
            func_8013E97C(0x38, random_value - 0x1D, func_8013E9A8(0x14) + 0x2B);
            func_8013E7C0(0x38, 0xC, 5, 1);
            if (--D_8015EC74 == 0) {
                func_8014EF3C();
            }
        }
        break;
    case 7:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
        }
        break;
    case 8:
        func_8014F178(3);
        break;
    case 9:
        func_8013EA60(0, 0x20, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            func_8014EF8C();
            D_8015EDC4 = 0x100007;
        } else {
            func_8014EF3C();
        }
        break;
    case 10:
        func_8014F178(0);
        break;
    case 11:
        D_8015EDE0 += 1;
        remaining = 4 - D_8015EDE0;
        func_8013E97C(0x30, (s32) (remaining * -0x18) / 4, (s32) (remaining * 0x30) / 4);
        if (D_8015EDE0 == 4) {
            func_8014EF3C();
        }
        break;
    case 12:
        func_8014F080(0x14);
        break;
    case 13:
        D_8015ED98 &= ~0x1000;
        func_8014EF8C();
        break;
    }
}

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

void func_80139E88(void) {
    /* the three scalars sit between the tables: they set flags at sp+0x64 (T-17103) */
    RpgStrTbl4 names;
    s32 n;
    s32 count;
    s32 i;
    RpgWordTbl4 flags;

    names = D_8015E530;
    flags = D_8015E540;
    n = 0;
    if (func_8013E9A8(5) == 0) {
        count = 3;
    } else {
        count = 2;
    }
    for (i = 0; i < count; i++) {
        switch (func_8013E9A8(0xA)) {
        case 0:
        case 1:
        case 2:
        case 3:
            flags.v[0] = 1;
            break;
        case 4:
        case 5:
        case 6:
            flags.v[1] = 1;
            break;
        case 7:
        case 8:
            flags.v[2] = 1;
            break;
        case 9:
            flags.v[3] = 1;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (flags.v[i] == 1) {
            func_800AE0F0(*(void **)(D_8015EB70 + n * 4 + 12), "おたから：");
            func_800AE100(*(void **)(D_8015EB70 + n * 4 + 12), names.s[i]);
            n++;
        }
    }
    D_8015ED14 = n + 3;
}

void func_8013A034(void) {
    s32 i;

    switch (D_8015EE30.unk_04) {
    case 0:
        cd_read_sectors(0x2B, 0x801B1000, 0x3E8C);
        func_8014F524();
    default:
        break;
    case 1:
        if (D_8015EBFC.unk_28 == 0) {
            func_8014F500();
        }
        if (cd_read_sync_status() & 1) {
            D_8015EBFC.unk_28 = 0;
            func_8014F524();
        }
        break;
    case 2:
        /* FAKE: the three constants are stored through their addresses (T-17103). A plain assignment
         * makes uopt promote the globals to registers and sink the stores below the -1 stores
         * (original: stores in source order, all three still read from memory later); the real
         * source is unknown. */
        *(s32 **) &D_8015EE8C = (s32 *) 0x801C5DC8;
        *(s32 *) &D_8015EEA4 = 0x801C5DDC;
        *(s32 *) &D_8015EEBC = 0x801C5EE4;
        D_800E6280.unk_1228[0x15] = -1;
        D_800E6280.unk_1228[0x16] = -1;
        D_800E6280.unk_1228[0x17] = -1;
        D_800E6280.unk_1228[0x18] = -1;
        D_800E6280.unk_1228[0x19] = -1;
        load_tpage_buf_lock(0x15, D_8015EE8C[0], 1);
        load_tpage_buf_lock(0x16, D_8015EE8C[1], 1);
        load_tpage_buf_lock(0x17, D_8015EE8C[2], 1);
        load_tpage_buf_lock(0x18, D_8015EE8C[3], 1);
        load_tpage_buf_lock(0x19, D_8015EE8C[4], 1);
        for (i = 0; i < 72; i++) {
            ((DisplayActor44 *) D_8011ECD0)[64 + i].unk_0C = (u8 *) D_8015EEA4;
            ((DisplayActor44 *) D_8011ECD0)[64 + i].unk_10 = (u8 *) D_8015EEBC;
            ((DisplayActor44 *) D_8011ECD0)[64 + i].unk_34 = (u8 *) D_8015EE8C;
        }
        if (D_8015EB9C != 0xE) {
            for (i = 29; i < 31; i++) {
                ((DisplayActor44 *) D_8011ECD0)[96 + i].unk_0C = (u8 *) D_8015EEB8;
                ((DisplayActor44 *) D_8011ECD0)[96 + i].unk_10 = (u8 *) D_8015EED0;
                ((DisplayActor44 *) D_8011ECD0)[96 + i].unk_34 = (u8 *) D_8015EEA0;
                ((DisplayActor44 *) D_8011ECD0)[96 + i].unk_43 = 0x1C;
            }
        }
        func_8014F524();
        break;
    case 3:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8014F524();
        break;
    case 4:
        func_80044750(0xE4);
        func_8014F524();
        break;
    case 5:
        D_8015EE30.unk_0C += 1;
        if (D_8015EE30.unk_0C >= 0xB5) {
            func_8014F500();
        }
        if (func_800460EC() & 4) {
            func_8014F500();
        }
        break;
    }
}

void func_8013A464();

void func_8013A2F4(void) {
    D_8015EE38 += 1;
    switch (D_8015EE38 % 4) {
    case 0:
        if (D_8015EE38 < 0x78) {
            func_8013A464(0x1E, 0xFF);
        } else {
            func_8013A464(0x1F, 0xFF);
        }
        break;
    case 1:
        if (D_8015EE38 < 0x78) {
            func_8013A464(0x1E, 0x80);
        } else {
            func_8013A464(0x1F, 0x80);
        }
        break;
    case 2:
        if (D_8015EE38 < 0x3C) {
            func_8013A464(0x1E, 0xFF);
        } else {
            func_8013A464(0x1F, 0xFF);
        }
        break;
    case 3:
        if (D_8015EE38 < 0x3C) {
            func_8013A464(0x1E, 0x80);
        } else {
            func_8013A464(0x1F, 0x80);
        }
        break;
    }
    if (D_8015EE38 >= 0xB5) {
        D_80121353 = 0x1F;
        D_80121397 = 0x1F;
        D_80121317 = 0x80;
        func_8014F500();
    }
}

void func_8013A464(a0, a1)
    s8 a0;
    s8 a1;
{
    D_80121353 = a0;
    D_80121317 = a1;
}
