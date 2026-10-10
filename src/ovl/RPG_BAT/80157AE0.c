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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80157CCC);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80158A0C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80158B70);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80158C4C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_8015906C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80159214);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80159620);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_801596E8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E13C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E14C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E15C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E16C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E178);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E184);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E190);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80157AE0.rodata", D_8015E19C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80157AE0", func_80159B40);
