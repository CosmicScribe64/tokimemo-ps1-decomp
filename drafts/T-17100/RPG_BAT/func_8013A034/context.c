#define MAIN_API_OVERRIDE_D_80121317 /* stores 0x80 as a positive constant (main_api.h: s8) */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern u8 D_80121317;

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

void func_8013A034(void) {
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s32 temp_v0;

    switch (D_8015EE30.unk_04) {
    case 0:
        cd_read_sectors(0x2BU, 0x801B1000, 0x3E8C);
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
        D_8015EE8C = 0x801C5DC8;
        D_8015EEA4 = 0x801C5DDC;
        D_8015EEBC = 0x801C5EE4;
        D_800E6280.unk_1228[0x15] = -1;
        D_800E6280.unk_1228[0x16] = -1;
        D_800E6280.unk_1228[0x17] = -1;
        D_800E6280.unk_1228[0x18] = -1;
        D_800E6280.unk_1228[0x19] = -1;
        load_tpage_buf_lock(0x15, *(s32 *)0x801C5DC8, 1);
        load_tpage_buf_lock(0x16, *(s32 *)((unsigned char *)D_8015EE8C + (4)), 1);
        load_tpage_buf_lock(0x17, *(s32 *)((unsigned char *)D_8015EE8C + (8)), 1);
        load_tpage_buf_lock(0x18, *(s32 *)((unsigned char *)D_8015EE8C + (12)), 1);
        load_tpage_buf_lock(0x19, *(s32 *)((unsigned char *)D_8015EE8C + (16)), 1);
        var_v0 = ((DisplayActor44 *)D_8011ECD0);
        do {
            var_v0 += 1;
            *(s32 *)&var_v0[63].unk_0C = (s32) D_8015EEA4;
            *(s32 *)&var_v0[63].unk_10 = (s32) D_8015EEBC;
            *(s32 *)&var_v0[63].unk_34 = (s32) D_8015EE8C;
        } while ((u32) var_v0 < (u32) (&((DisplayActor44 *)D_8011ECD0)[72]));
        if (D_8015EB9C != 0xE) {
            var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[29];
            do {
                var_v0_2 += 1;
                *(s32 *)&var_v0_2[95].unk_0C = (s32) D_8015EEB8;
                *(s32 *)&var_v0_2[95].unk_10 = (s32) D_8015EED0;
                *(s32 *)&var_v0_2[95].unk_34 = (s32) D_8015EEA0;
                var_v0_2[95].unk_43 = 0x1C;
            } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[31]));
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
        temp_v0 = D_8015EE30.unk_0C + 1;
        D_8015EE30.unk_0C = temp_v0;
        if (temp_v0 >= 0xB5) {
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
