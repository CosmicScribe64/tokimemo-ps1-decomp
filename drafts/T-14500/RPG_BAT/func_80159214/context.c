#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80157AE0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「袖竜！");
        func_8014B738(D_8015E8F0, 1, 0x78);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x180000A6, 0x0B0000B0, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80157BA4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「男の意地にかけて、");
        func_800AE0F0(D_8015E850, "負ける訳にはいかねぇ！");
        func_8014B738(D_8015E8F0, 2, 0xDC);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E000083, 0x1A00008B, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1E000094, 0x1C00009E, 1);
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

s32 func_80157CCC(void) {
    switch (*D_8015ED8C) {
    case 0:
        D_8015ED38 = func_8013E9A8(6) ^ 0; /* FAKE: the original copies the call result into the switch temporary ($v1), source unknown. T-9370 */
        switch (D_8015ED38) {
        case 0:
            func_8013F1C4(0x180000B1, 0x0C0000B7, 0);
            break;
        case 1:
            func_8013F1C4(0x1200009B, 0x050000A4, 0);
            break;
        case 2:
            func_8013F1C4(0x1D0000AF, 0x200000B7, 0);
            break;
        case 3:
            func_8013F1C4(0x0300006B, 0x0B00006E, 0);
            break;
        case 4:
            func_8013F1C4(0x08000028, 0x06000030, 0);
            break;
        case 5:
            func_8013F1C4(0x150000AC, 0x0B0000B7, 0);
            break;
        }
        *D_8015ED8C += 1;
        break;
    case 1:
        func_8013F220();
        func_8014EF3C();
        break;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80157E20(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「ふっ！　情けない。");
        func_800AE0F0(D_8015E850, "俺様に楯突こうとは十年早いわ！");
        func_8014B738(D_8015E8F0, 2, 0x118);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1F000099, 0x1C0000A0, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x08000019, 0x05000027, 1);
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
s32 func_80157F48(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「ふっ、恐れ入ったぜっ");
        func_800AE0F0(D_8015EA08, "俺様を倒せるやつが、");
        func_800AE0F0(D_8015EA30, "この世にいたとはな・・・。");
        func_8014B738(D_8015EB70, 3, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1C00007A, 0x0C000083, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0300006F, 0x0C000076, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x160000A5, 0x080000AC, 2);
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
s32 func_801580C4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "番長「いや、");
        func_800AE0F0(D_8015EA08, "その名はもう俺にはふさわしくない。");
        func_800AE0F0(D_8015EA30, "これからは、お前が番長だ。");
        func_8014B738(D_8015EB70, 3, 0xE6);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x03000077, 0x0D000078, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x160000AD, 0x090000B7, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x140000AC, 0x0A0000B7, 2);
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
s32 func_80158240(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "元・番長「あばよ、彼女と仲良くなっ！");
        func_8014B738(D_8015EB70, 1, 0x104);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x0800000A, 0x04000018, 0);
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
s32 func_8015831C(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「貴様の力はそんなもんか！");
        func_8014B738(D_8015E8F0, 1, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E0000A5, 0x1E0000AF, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801583F8(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「片腹痛いわ！");
        func_8014B738(D_8015E8F0, 1, 0xB4);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x04000021, 0x05000028, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801584D4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「なかなかやるな！");
        func_8014B738(D_8015E8F0, 1, 0x96);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1700009B, 0x090000A1, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801585B0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「少しはできそうだな。");
        func_8014B738(D_8015E8F0, 1, 0xB4);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E00007A, 0x19000082, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8015868C(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「男の意地にかけて、");
        func_800AE0F0(D_8015E850, "負ける訳にはいかねぇ！");
        func_8014B738(D_8015E8F0, 2, 0xE6);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E000083, 0x1A00008B, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1E000094, 0x1C00009E, 1);
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
s32 func_801587B4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「俺様が・・・");
        func_800AE0F0(D_8015E850, "番長のこの俺様が・・・");
        func_800AE0F0(D_8015E878, "こんな所で負ける訳にはいかん！");
        func_8014B738(D_8015E8F0, 3, 0xFA);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x09000001, 0x02000006, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x10000099, 0x070000A2, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x05000036, 0x06000044, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(0);
        return;
    case 7:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80158930(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "番長「うぬぬぬぬ・・・");
        func_8014B738(D_8015E8F0, 1, 0x96);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1E0000B0, 0x1F0000B7, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

void func_80158A0C(void) {
    switch (D_8015EDD8) {
    case 0:
        D_8015EC74 = func_8013E9A8(2);
        func_8014EF3C();
        return;
    case 1:
        switch (D_8015EDC0 * 6 / D_8015EDCC) {
        case 0:
            if (D_8015EC74 == 0) {
                func_801587B4();
                return;
            }
            func_80158930();
            return;
        case 1:
            func_8015868C();
            return;
        case 2:
            func_801585B0();
            return;
        case 3:
            func_801584D4();
            return;
        case 4:
            func_801583F8();
            return;
        case 5:
            func_8015831C();
            return;
        case 6:
            func_8015831C();
            return;
        default:
            return;
        }
        break;
    case 2:
        func_8014F080(0x14);
        return;
    case 3:
        func_8014EF8C();
        break;
    }
}

void func_80158B70(void) {
    switch (D_8015EDDC) {
    case 0:
        D_8015EC78 = func_801445E8(0) - D_8015ED9C;
        if (D_8015EC78 <= 0) {
            D_8015EC78 = 1;
        }
        func_8014EF6C();
        return;
    case 1:
        func_8014F0BC(D_8015EC78);
        return;
    case 2:
        D_8015ED98 |= 0x10000;
        D_8015ED9C = 0;
        func_8014EF6C();
        return;
    case 3:
        func_8014F0BC(0x5A);
        return;
    case 4:
        func_8014EF3C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80158C4C);

void func_8015906C(void) {
    switch (D_8015EDD8) {
    case 0:
        func_80157BA4();
        break;
    case 1:
        func_8013E7C0(0x3B, 6, 5, 1);
        func_8014EF3C();
        break;
    case 2:
        if (D_8015EDE0 == 0x22) {
            func_8013F15C(0x509, 1, 0);
            func_8013E97C(0x3A, 0, 0);
            func_8013E7C0(0x3A, 7, 1, 1);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 >= 0x2B) {
            func_8014EF3C();
        }
        break;
    case 3:
        if (!(D_8015EDE0 & 1)) {
            func_8013E7C0(0x3A, 7, 1, 0);
        } else {
            func_8013E7C0(0x3A, 7, 1, 1);
        }
        D_8015EDE0 += 1;
        if (D_8015EDE0 >= 0x3D) {
            func_8013E7C0(0x3B, 8, 1, 1);
            D_8015EDC4 |= 0x400000;
            D_8015EDC4 &= ~0x200000;
            func_8014EF3C();
        }
        break;
    case 4:
        func_8014F080(0x14);
        break;
    case 5:
        func_8014EF8C();
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_801215D2;
extern s16 D_801215D4;
extern s32 D_800E6354;
extern s8 D_801215B9;
/* end bulk_m2c declarations */
void func_80159214(void) {
    s16 temp_v0;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t1_3;
    s32 var_t0;
    s32 var_v1;

    switch (D_8015EDD8) {
    case 0:
        func_80157CCC();
    default:
        break;
    case 1:
        func_8013E7C0(0x3B, 9, 5, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 2:
        temp_t1 = D_8015EDE0;
        if (temp_t1 == 0x6E) {
            func_8013E97C(0x3A, 0, 0);
            D_801215D2 = 0x100;
            D_801215D4 = 0x100;
            D_801215B9 = 4;
            func_8013E7C0(0x3A, 0xB, 1, 1);
            func_8013F15C(0x50B, 1, 0);
            func_8014EF3C();
        }
        D_8015EDE0 = temp_t1 + 1;
        break;
    case 3:
        temp_v0 = ((s32) (D_8015EDE0 << 0xD) / 256) + 0x100;
        D_801215D2 = temp_v0;
        D_801215D4 = temp_v0;
        temp_t1_2 = D_8015EDE0 + 3;
        if (temp_t1_2 < 0x100) {
            D_8015EDE0 = temp_t1_2;
            var_t0 = (temp_t1_2 / 2) * 0x10201;
        } else {
            D_8015EDE0 = temp_t1_2;
            var_t0 = 0x7FFE7F;
        }
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, var_t0, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (D_8015EDE0 >= 0x1F5) {
            D_801215D2 = 0x1000;
            D_801215D4 = 0x1000;
            D_801215B9 = 5;
            func_8013E7C0(0x3A, 0xFF, 1, 0);
            func_8013E7C0(0x3B, 8, 1, 1);
            var_v1 = D_8015ED98;
            if (var_v1 == 0x02000000) {
                var_v1 = 0x100;
            }
            D_8015ED98 = var_v1;
            if (var_v1 != 0x100) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            func_8014EF3C();
        }
        break;
    case 4:
        func_80049A40(-0xA0, -0x78, 0x140, 0xA8, 2, (0x7F - D_8015EDE0) * 0x10201, 0x81);
        dtd_on_tpage(0, 0, 2, 1, 0);
        temp_t1_3 = D_8015EDE0 + 7;
        D_8015EDE0 = temp_t1_3;
        if (temp_t1_3 >= 0x80) {
            func_8014EF3C();
        }
        break;
    case 5:
        func_801447E0();
        func_8014D11C((s32) D_8015E828, (s32) &D_800E6354, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8014EF3C();
        break;
    case 6:
        func_8014F178(1);
        break;
    case 7:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        if (D_8015ED94 == 0) {
            D_8015EDD8 = 0;
            D_8015EDC4 = (D_8015EDC4 & 0xFFFF0000) | 5;
            D_8015ED98 = 0x1000;
            func_8013E7C0(0x30, 7, 1, 1);
        }
        func_8014EF3C();
        break;
    case 8:
        func_8014F178(0);
        break;
    case 9:
        func_8014F080(0x14);
        break;
    case 10:
        func_8014EF8C();
        break;
    }
}

void func_80159620(void) {
    switch (D_8015EDD8) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8014F178(0);
        return;
    case 1:
        D_8015ED98 = 0x1000;
        func_8014F178(0);
        return;
    case 2:
        func_8014F080(0x14);
        return;
    case 3:
        func_8004500C(1, 0x203);
        func_8014EF3C();
        return;
    case 4:
        func_80157E20();
        return;
    case 5:
        D_8015EC14 = 1;
        func_80042808();
        return;
    }
}

void func_801596E8(void) {
    s32 temp_v0;

    switch (D_8015EE30.unk_00) {
    case 0:
        D_8015ED98 = 0x1000;
        func_8014F500();
    default:
        break;
    case 1:
        func_80157F48();
        break;
    case 2:
        func_800AE0F0(D_8015E9E0, &D_800E6280.unk_0D4);
        func_800AE100(D_8015E9E0, "「番長・・・。");
        func_8014B738((s32) D_8015EB70, 1, 0x5A);
        func_8014F500();
        break;
    case 3:
        func_8014F6D4(0);
        break;
    case 4:
        func_801580C4();
        break;
    case 5:
        func_8004E58C();
        D_8015EDBC = func_8013E934(0xB1, 0xB3, (s32) "番長") - 1;
        D_8015EE14 = func_8013E934(0xB1, 0xC3, (s32) girl_short_name_table[D_8015EB9C]) - 1;
        D_8015EE40 = func_8013E934(0x13, 0xBB, (s32) "元・番長") - 1;
        k_disp_start(D_8015EE40 + 1);
        k_disp_switch(D_8015EDBC, 1);
        k_disp_switch(D_8015EE14, 1);
        k_disp_switch(D_8015EE40, 1);
        k_sub_reset_point_set();
        func_8013F15C(0x515, 1, 0);
        func_8014F500();
        break;
    case 6:
        func_80158240();
        break;
    case 7:
        ((RpgHealthBlock *)D_8011ECD0)->record[155].unk_03 |= 1;
        ((RpgHealthBlock *)D_8011ECD0)->record[155].unk_26 -= 0x10;
        temp_v0 = D_8015EE38 + 1;
        D_8015EE38 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8014F500();
        }
        break;
    case 8:
        func_8014F5DC(0x1E);
        break;
    case 9:
        func_8013E7C0(0x30, 9, 1, 1);
        switch (D_8015EB9C) {
        case 0:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 1:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 2:
            func_8013E7C0(0x1D, 2, 1, 1);
            break;
        case 3:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 4:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 5:
            func_8013E7C0(0x1D, 2, 1, 1);
            break;
        case 6:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 7:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 8:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 9:
            func_8013E7C0(0x1D, 7, 1, 1);
            break;
        case 10:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        case 13:
            func_8013E7C0(0x1D, 4, 1, 1);
            break;
        }
        func_8014F500();
        break;
    case 10:
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "経験値　：３２７６７ＥＸＰ");
        func_800AE0F0(D_8015EA30, "お金　　：６５５３５ゴールド");
        func_80159B40();
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8004500C(1, 0x202);
        func_8014F500();
        break;
    case 11:
        func_8014F6D4(0);
        break;
    case 12:
        D_8015EC14 = 0;
        func_80042808();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E13C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E14C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E15C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E16C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E178);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E184);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E190);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E19C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80159B40);
