#define RPG_BAT_OVERRIDE_D_8015EDAC /* switched and compared as s32 here (RPG_BAT.h: u32), T-17111 */
#include "common.h"
#include "ovl/RPG_BAT.h"

extern s32 D_8015EDAC;

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801504E0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E74C, "紐緒「馬鹿な事言ってるんじゃないわ");
        func_8014B738(&D_8015E814, 1, 0x96);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x27, 0x0600002D, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EBA8();
        return;
    }
}

void func_801505B8(void) {
    s32 i;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　脱力漫才　　　　　　　");
        func_8014B738(&D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_801513C8();
        func_8014EBA8();
        func_8014EBA8();
        break;
    case 2:
        func_8013D418();
        break;
    case 3:
        func_80151108();
        break;
    case 4:
        if (D_8015EB98 == 0x08000000) {
            D_8015EDAC = 0xC8;
        } else {
            func_80151380(D_8015EDB0.unk_04);
            D_8015EDB0.unk_04 += 4;
            if (D_8015EDB0.unk_04 >= 0xE1) {
                func_8014EBA8();
            }
        }
        break;
    case 5:
        func_8013D350();
        if (D_8015EDC4 & 0x100000) {
            for (i = 0; i < 3; i++) {
                ((DisplayActor44 *)D_8011ECD0)[149 + i].unk_03 &= ~1;
            }
        }
        func_8014EBA8();
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        func_80150AA4();
        break;
    default:
        D_8015EDAC = 0x64;
    case 0x64:
        func_80151380(0xE0 - D_8015EDB0.unk_04);
        D_8015EDB0.unk_04 += 4;
        if (D_8015EDB0.unk_04 >= 0xE1) {
            func_801515F0();
            func_8014EBA8();
        }
        break;
    case 0x65:
        func_8014ED44(0x14);
        break;
    case 0x66:
        func_8013D59C();
        break;
    case 0x67:
        func_8014EBEC();
        break;
    case 0xC8:
        func_8014ED44(0x3C);
        break;
    case 0xC9:
        func_800AE0F0(D_8015E74C, "ロボ「ガガッ・・理解不能 ");
        func_8014B738(&D_8015E814, 1, 0x5A);
        func_8014EBA8();
        break;
    case 0xCA:
        func_8014EE3C(0);
        break;
    case 0xCB:
        func_8013D3E4();
        func_8014EBA8();
        break;
    case 0xCC:
        func_8014ED44(0x3C);
        break;
    case 0xCD:
        func_801504E0();
        break;
    case 0xCE:
        func_8013E97C(0x38, 0x140, 0x5E);
        func_8013E810(0x38, 1, 1, 1);
        func_8013E97C(0x32, 0x140, 0x60);
        func_8013E810(0x32, 6, 5, 1);
        ((DisplayActor44 *)D_8011ECD0)[152].unk_03 |= 1;
        ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 4;
        ((DisplayActor44 *)D_8011ECD0)[146].unk_01 = 4;
        func_8014EBA8();
        break;
    case 0xCF:
        ((DisplayActor44 *)D_8011ECD0)[152].unk_26 -= 0x10;
        ((DisplayActor44 *)D_8011ECD0)[146].unk_26 -= 0x10;
        D_8015EDB0.unk_04 += 1;
        if (D_8015EDB0.unk_04 == 2) {
            func_8013F0F4(0x513, 0, 5);
            func_8013E7C0(0x34, 0xC, 1, 1);
            func_8013E7C0(0x33, 8, 1, 1);
            ((DisplayActor44 *)D_8011ECD0)[146].unk_18 = 2;
        }
        if (D_8015EDB0.unk_04 == 0xA) {
            ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 6;
            ((DisplayActor44 *)D_8011ECD0)[146].unk_01 = 6;
        }
        if (D_8015EDB0.unk_04 >= 0x16) {
            func_8014EBA8();
        }
        break;
    case 0xD0:
        if (D_8015EDB0.unk_08 == 0x14) {
            func_8013E7C0(0x33, 0xFF, 1, 0);
        }
        func_8014ED44(0x3C);
        break;
    case 0xD1:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8013E7C0(0x33, 0xFF, 1, 0);
        D_8015EDAC = 0x65;
        break;
    }
}

void func_80150AA4(void) {
    switch (D_8015EB9C) {
    case 0:
        func_80134A70();
        return;
    case 1:
        func_8013E300();
        return;
    case 2:
        func_80136530();
        return;
    case 3:
        func_8013C5D0();
        return;
    case 4:
        func_80147F10();
        return;
    case 5:
        func_80143830();
        return;
    case 6:
        func_80141C30();
        return;
    case 7:
        func_8013A480();
        return;
    case 8:
        func_80132000();
        return;
    case 9:
        func_801454F0();
        return;
    case 10:
        func_801496F0();
        return;
    }
}

/* returns int without a value, selector read as a word: the switch temporary stays in $v1/$v0 (T-8080, T-9330) */
s32 func_80150B90(void) {
    switch (*(u32 *) &D_8015EC74) {
    case 0:
        func_8013E810(0x32, 6, 5, 1);
        D_8015EC74 += 1;
        return;
    case 1:
        D_8015EC78 += 1;
        if (D_8015EC78 >= 0xF) {
            D_8015EC74 += 1;
        }
        return;
    case 2:
        func_8013E7C0(0x34, 0xC, 1, 1);
        func_8013E97C(0x33, 0, 0);
        func_8013E7C0(0x33, 8, 1, 1);
        func_8013F0F4(0x513, 0, 5);
        D_8015EC74 += 1;
        return;
    case 3:
        D_8015EC78 += 1;
        if (D_8015EC78 >= 0x1F) {
            func_8013E7C0(0x33, 8, 1, 0);
            D_8015EC74 += 1;
        }
        return;
    }
}

/* (u32) on the s16 compare: its constant 1 is then the unsigned 1 of the case label and shares $v0 (T-9330) */
void func_80150CD8(void) {
    switch (*(u32 *) &D_8015EC74) {
    case 0:
        func_8013E810(0x32, 7, 5, 1);
        D_8015EC74 += 1;
        return;
    case 1:
        if ((u32) D_801213B0 == 1) {
            func_8013E97C(0x33, 0, 0);
            func_8013E7C0(0x33, 8, 1, 1);
            func_8013F0F4(0x512, 0, 5);
            D_8015EC74 += 1;
            return;
        }
    case 2:
        D_8015EC78 += 1;
        if (D_8015EC78 >= 0x1F) {
            func_8013E7C0(0x33, 8, 1, 0);
            D_8015EC74 += 1;
        }
        return;
    }
}

void func_80150DE0(void) {
    switch (D_8015EB9C) {
    case 0:
        D_8015EC58.unk_10 = 4;
        D_8015EC58.unk_14 = 0;
        break;
    case 1:
        D_8015EC58.unk_10 = 5;
        D_8015EC58.unk_14 = -2;
        break;
    case 2:
        D_8015EC58.unk_10 = 6;
        D_8015EC58.unk_14 = 2;
        break;
    case 3:
        D_8015EC58.unk_10 = 5;
        D_8015EC58.unk_14 = 2;
        break;
    case 4:
        D_8015EC58.unk_10 = 5;
        D_8015EC58.unk_14 = 2;
        break;
    case 5:
        D_8015EC58.unk_10 = 5;
        D_8015EC58.unk_14 = 2;
        break;
    case 6:
        D_8015EC58.unk_10 = 3;
        D_8015EC58.unk_14 = 2;
        break;
    case 7:
        D_8015EC58.unk_10 = 4;
        D_8015EC58.unk_14 = 1;
        break;
    case 8:
        D_8015EC58.unk_10 = 5;
        D_8015EC58.unk_14 = 1;
        break;
    case 9:
        D_8015EC58.unk_10 = 2;
        D_8015EC58.unk_14 = 0;
        break;
    case 10:
        D_8015EC58.unk_10 = 8;
        D_8015EC58.unk_14 = 2;
        break;
    }
    D_8015EC58.unk_00 = 0x118;
    D_8015EC58.unk_04 = 0x88;
    D_8015EC58.unk_08 = 0x118 - D_8015EC58.unk_10;
    D_8015EC58.unk_0C = 0x60 - D_8015EC58.unk_14;
}

typedef struct {
    u8 *s[41];
} StrTbl41; /* size 0xA4 */
extern StrTbl41 D_8015EEF0;

void func_80150F84(void) {
    s32 a;
    s32 b;
    s32 c;
    StrTbl41 tbl;

    tbl = D_8015EEF0;
    a = func_8013E9A8(0x29);
    b = func_8013E9A8(0x29);
    if (b == a) {
        b = (b + 1) % 41;
    }
    c = func_8013E9A8(0x29);
    if (c == a) {
        c = (c + 1) % 41;
    }
    if (c == b) {
        c = (c + 1) % 41;
    }
    func_800AE0F0(D_8015E74C, tbl.s[a]);
    func_800AE0F0(D_8015E774, tbl.s[b]);
    func_800AE0F0(D_8015E79C, tbl.s[c]);
}

void func_80151108(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_80150F84();
        func_8014B738(&D_8015E814, 3, 0x5A);
        func_8013E7C0(0x34, 3, 1, 1);
        func_8014EBD0();
        return;
    case 1:
        if (D_800E6280.unk_F88 & 0x40) {
            func_8014EBD0();
            return;
        }
        func_8014ED80(0x3C);
        return;
    case 2:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8014EBD0();
        return;
    case 3:
        if (D_80121438 == 1) {
            func_8013F0F4(0x511, 0, 3);
        }
        func_8014EE98(1);
        return;
    case 4:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8014EBD0();
        return;
    case 5:
        if (D_800E6280.unk_F88 & 0x40) {
            func_8014EBD0();
            return;
        }
        func_8014ED80(0x32);
        return;
    case 6:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8014EBD0();
        return;
    case 7:
        if (D_80121438 == 1) {
            func_8013F0F4(0x511, 0, 1);
        }
        func_8014EE98(2);
        return;
    case 8:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8014EBD0();
        return;
    case 9:
        if (D_800E6280.unk_F88 & 0x40) {
            func_8014EBD0();
            return;
        }
        func_8014ED80(0x32);
        return;
    case 10:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8014EBD0();
        return;
    case 11:
        if (D_80121438 == 1) {
            func_8013F0F4(0x511, 0, 2);
        }
        func_8014EE98(3);
        return;
    case 12:
        func_8014EBA8();
        return;
    }
}

void func_80151380(s32 arg0) {
    s32 temp_v0;

    D_8012146B = arg0;
    D_801214AF = arg0;
    D_801214F3 = arg0;
    if (arg0 < 0x80) {
        temp_v0 = 0x80 - arg0;
        D_80121603 = temp_v0;
        D_80121647 = temp_v0;
        D_8012168B = temp_v0;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801513C8(void) {
    func_80043914(D_8015EEE4, 0x1B, 1, 3, 7);
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x100000:
        func_8013E97C(0x35, 0x78, 0x68);
        func_8013E97C(0x36, 0x40, 0x58);
        func_8013E97C(0x37, 0x38, 0x88);
        func_8013E810(0x35, 9, 1, 1);
        func_8013E810(0x36, 9, 1, 1);
        func_8013E810(0x37, 9, 1, 1);
        D_8012149C = 0x51000000;
        D_801214E0 = 0x51000000;
        D_80121524 = 0x51000000;
        D_8012146B = 0;
        D_801214AF = 0;
        D_801214F3 = 0;
        D_801214A7 = 0x1B;
        D_801214EB = 0x1B;
        D_8012152F = 0x1B;
        if (D_8015EBCC[0] == 1) {
            D_80121467 |= 1;
        }
        if (D_8015EBCC[1] == 1) {
            D_801214AB |= 1;
        }
        if (D_8015EBCC[2] == 1) {
            D_801214EF |= 1;
        }
        break;
    case 0x200000:
        func_8013E97C(0x35, 0, 0);
        func_8013E7C0(0x35, 0xA, 1, 1);
        D_8012149C = 0x51000000;
        D_8012146B = 0;
        D_801214A7 = 0x1B;
        break;
    case 0x400000:
        func_8013E97C(0x35, 0, 0);
        func_8013E7C0(0x35, 0xB, 1, 1);
        D_8012149C = 0x51000000;
        D_8012146B = 0;
        D_801214A7 = 0x1B;
        break;
    }
}

s32 func_801515F0(void) {
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x100000:
        func_8013E810(0x35, 9, 1, 0);
        func_8013E810(0x36, 9, 1, 0);
        func_8013E810(0x37, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        *(u8 *)&D_80121647 = 0x80;
        *(u8 *)&D_8012168B = 0x80;
        break;
    case 0x200000:
        func_8013E810(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    case 0x400000:
        func_8013E810(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    }
}

void func_801516E4(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_80150DE0();
        func_8013E97C(0x1D, 0x118, 0x88);
        func_8013E97C(0x32, D_8015EC58.unk_10 + 0x118, D_8015EC58.unk_14 + 0x88);
        func_8013E810(0x1D, 0, 1, 1);
        func_8013E810(0x32, 7, 5, 1);
        D_801213B0 = 1;
        D_80120E05 = 4;
        D_80121399 = 3;
        func_8014EBD0();
        break;
    case 1:
        func_8013E97C(0x1D,
                      (D_8015EC58.unk_08 - D_8015EC58.unk_00) * D_8015EDB0.unk_08 / 100 + D_8015EC58.unk_00,
                      (D_8015EC58.unk_0C - D_8015EC58.unk_04) * D_8015EDB0.unk_08 / 100 + D_8015EC58.unk_04);
        func_8013E97C(0x32,
                      (D_8015EC58.unk_08 - D_8015EC58.unk_00) * D_8015EDB0.unk_08 / 100 + D_8015EC58.unk_00 + D_8015EC58.unk_10,
                      (D_8015EC58.unk_0C - D_8015EC58.unk_04) * D_8015EDB0.unk_08 / 100 + D_8015EC58.unk_04 + D_8015EC58.unk_14);
        D_8015EDB0.unk_08 = D_8015EDB0.unk_08 + 4;
        if (D_8015EDB0.unk_08 >= 0x65) {
            func_8014EBD0();
        }
        break;
    case 2:
        func_8014EBA8();
        break;
    }
}

void func_80151984(void) {
    if (D_8015EDB8 == 0) {
        func_8013E7C0(0x34, 2, 1, 1);
        func_8013E7C0(0x33, 8, 1, 0);
    }
    func_8013E97C(0x1D, (D_8015EC58.unk_00 - D_8015EC58.unk_08) * D_8015EDB8 / 100 + D_8015EC58.unk_08,
                  (D_8015EC58.unk_04 - D_8015EC58.unk_0C) * D_8015EDB8 / 100 + D_8015EC58.unk_0C);
    func_8013E97C(0x32, (D_8015EC58.unk_00 - D_8015EC58.unk_08) * D_8015EDB8 / 100 + D_8015EC58.unk_08 + D_8015EC58.unk_10,
                  (D_8015EC58.unk_04 - D_8015EC58.unk_0C) * D_8015EDB8 / 100 + D_8015EC58.unk_0C + D_8015EC58.unk_14);
    D_8015EDB8 += 4;
    if (D_8015EDB8 >= 0x65) {
        func_8013E97C(0x1D, 0, 0);
        if (D_8015EB9C == 1 && D_8015EDF0 == 0x1000) {
            func_8013E7C0(0x1D, 7, 1, 1);
        } else {
            func_8013E7C0(0x1D, 0, 1, 1);
        }
        func_8013E810(0x32, 0xFF, 1, 0);
        D_80121399 = 5;
        func_8014EBA8();
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1C4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1F0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D200);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D210);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D220);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D230);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D240);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D254);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D268);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D27C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D290);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2A4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D308);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D31C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D330);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D348);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D360);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D378);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D38C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3A0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3E8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D400);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D41C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D438);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D454);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D470);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D48C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4AC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D50C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D52C);
