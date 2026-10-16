#include "common.h"
#include "psx_types.h"
#include "ovl/ETC.h"

typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 x1;
    /* 0x0E */ s16 y1;
    /* 0x10 */ s16 x2;
    /* 0x12 */ s16 y2;
    /* 0x14 */ s16 x3;
    /* 0x16 */ s16 y3;
} EtcPolyF4; /* size 0x18 */

void func_8013BEF0(void) {
    D_80150030 = (u8 *)0x8019DCFC;
    D_80150034 = (u8 *)0x8019DD00;
    D_80150038 = (u8 *)0x8019DDC0;
    D_8015003C = *(s16 *)0x8019DDF8;
}

void func_8013BF34(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013BFFC();
        return;
    case 1:
        func_8013C330(2, 0);
        return;
    case 2:
        func_8013C330(3, 1);
        return;
    case 3:
        func_8013C330(4, 2);
        return;
    case 4:
        func_8013C700();
        return;
    case 5:
        func_8013C8EC();
        return;
    case 6:
        func_8013CC74();
        return;
    case 7:
        func_8013CCE4();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013BFFC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C330);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7208;
extern s32 D_801500D8;
extern u32 D_800E7384;
extern u8 D_800E71DF;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_8013C700(void) {
    switch (D_800E738D) {
    case 0:
        func_8013D32C();
        func_80132C3C();
    default:
        break;
    case 1:
        func_8013D32C();
        func_8013D570(0, 0);
        func_8013E89C(5);
        if (D_80150070 >= 3) {
            D_800E738D += 1;
            D_80150070 = 0;
        } else if (D_80150070 == -1) {
            D_800E738D += 1;
        }
        break;
    case 2:
        func_8013D32C();
        func_8013D570(0, 0);
        if ((D_800E71DF == 0xC) || (D_800E71DF == 0xB)) {
            func_8013F054();
        }
        if (((D_8015006C == 0) || (D_8015006C >= 3)) && ((D_800E7208 & 0x20) || (D_801500D8 == 0) || (D_801500D8 == 1))) {
            D_800E738D += 1;
            D_8015006C = 0;
        }
        break;
    case 3:
        func_8013D32C();
        func_8013D570(D_800E7384 * 2, 3);
        if ((u32) D_800E7384 < 0x28U) {
            D_800E7384 += 1;
        } else {
            D_800E738D += 1;
        }
        break;
    case 4:
        func_8013FC08();
        D_801500D4 = 0x800;
        func_8013FA7C();
        func_8004284C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C8EC);

void func_8013CC74(void) {
    if ((D_80150054 == 0 && D_80150058 == 0 && D_8015005C == 0) || D_80150064 == 0xFF) {
        func_80042808();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013CCE4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013CF2C);

void func_8013D264(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x140, 0x100);
    func_8009C674(0);
}

void func_8013D2C0(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = (1 - D_8011ECA0) * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0, D_8011ECA0 * 0xF0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013D32C);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013D570);

void func_8013DEA0(s32 arg0) {
    SPRT *temp_v0;
    SPRT *temp_v0_2;

    temp_v0 = (SPRT *)func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    func_8009EF84(temp_v0, 0);
    func_8009EFB0(temp_v0, 0);
    temp_v0->r0 = 0x80;
    temp_v0->g0 = 0x80;
    temp_v0->b0 = 0x80;
    if (arg0 == 1) {
        temp_v0->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xA0);
        temp_v0->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
        temp_v0->v0 = 0;
        temp_v0->u0 = 0;
        /* FAKE: paired stores share a source line for the original as1 order. T-9930. */
        temp_v0->w = 0xC0; temp_v0->h = 0xA0;
    }
    func_8009EED0((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0);
    func_800493A8(0x140, 0x100, 0xD, 0, 2);
    temp_v0_2 = (SPRT *)func_800490F0(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_2);
    func_8009EF84(temp_v0_2, 0);
    func_8009EFB0(temp_v0_2, 0);
    temp_v0_2->r0 = 0x80;
    temp_v0_2->g0 = 0x80;
    temp_v0_2->b0 = 0x80;
    if (arg0 == 1) {
        temp_v0_2->x0 = (s16) (D_800E6280.unk_014[D_8011ECA0] + 0x20);
        temp_v0_2->y0 = (s16) (D_800E6280.unk_018[D_8011ECA0] - 0x65);
        temp_v0_2->v0 = 0;
        temp_v0_2->u0 = 0;
        /* FAKE: same dimension-store scheduling dependency as above. T-9930. */
        temp_v0_2->w = 0x80; temp_v0_2->h = 0xA0;
    }
    func_8009EED0((D_8011ECA0 << 0xA) + 0x34 + D_800E8CA0, temp_v0_2);
    func_800493A8(0x200, 0x100, 0xD, 0, 2);
}

void func_8013E094(u8 *arg0) {
    s32 pad; /* FAKE: unused slot above base, the original frame has it (T-3330 idiom, real source unknown). T-10710 */
    s16 base;
    u8 *p;
    s16 e1;
    s16 e2;

    if (D_8015004C == 0xA && D_80150050 == 1) {
        D_8011F536 = -0x15;
        D_8011F4F2 = -0x15;
    } else {
        D_8011F536 = -0xD;
        D_8011F4F2 = -0xD;
    }
    p = *(u8 **)arg0;
    if (*(s16 *)p >= 6) {
        base = *(s16 *)(p + 4);
        e1 = *(s16 *)(p + 0x50);
        e2 = *(s16 *)(p + 0x42);
        D_801217D6 = D_8011F536 + e1 - base;
        D_801217FA = D_8011F536 + e2 - base;
    }
    if (*(s16 *)p >= 4) {
        e1 = *(s16 *)(p + 0x26);
        e2 = *(s16 *)(p + 0x34);
        D_8012181E = D_8011F536 + e1 - base;
        D_80121842 = D_8011F536 + e2 - base;
    }
    if (*(s16 *)p >= 7) {
        e1 = *(s16 *)(p + 0x5E);
        D_80121866 = D_8011F536 + e1 - base;
    }
    if (*(s16 *)p == 8) {
        e2 = *(s16 *)(p + 0x6C);
        D_8012188A = D_8011F536 + e2 - base;
    }
}

void func_8013E1DC(arg0)
s32 arg0;
{
    EtcPolyF4 *poly;
    s32 sprite;

    poly = func_800490F0(0x18, D_8011ECA0);
    func_8009F02C(poly);
    func_8009EF84(poly, 1);
    func_8009EFB0(poly, 0);
    if (arg0 >= 0x100) {
        arg0 = 0xFF;
    }
    poly->x0 = 0;
    poly->y0 = D_800E6280.unk_018[D_8011ECA0] - 0x78;
    poly->x1 = 0x140;
    poly->y1 = D_800E6280.unk_018[D_8011ECA0] - 0x78;
    poly->x2 = 0;
    poly->y2 = D_800E6280.unk_018[D_8011ECA0] + 0x78;
    poly->x3 = 0x140;
    poly->y3 = D_800E6280.unk_018[D_8011ECA0] + 0x78;
    poly->b0 = arg0;
    poly->g0 = arg0;
    poly->r0 = arg0;
    func_8009EED0(D_8011ECA0 * 0x400 + D_800E8CA0, poly);
    sprite = func_800490F0(0xC, D_8011ECA0);
    func_8009D294(sprite, 0, 0, func_8009ECB0(1, 2, 0, 0), 0);
    func_8009EED0(D_8011ECA0 * 0x400 + D_800E8CA0, sprite);
    func_80049450(0);
}

void func_8013E364(void) {
    if (D_8015005C < 0xA && D_80150058 < 0xA) {
        func_8013FC64(D_80150054, 0x50, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x64, 0x2D, 0);
        func_8013E524(0xE5, 0x3E);
    } else if (D_8015005C >= 0xA && D_80150058 < 0xA) {
        func_8013FC64(D_80150054, 0x4B, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x5F, 0x2D, 0);
        func_8013E524(0xE0, 0x42);
    } else if (D_8015005C < 0xA && D_80150058 >= 0xA) {
        func_8013FC64(D_80150054, 0x4B, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x64, 0x2D, 0);
        func_8013E524(0xE0, 0x42);
    } else if (D_8015005C >= 0xA && D_80150058 >= 0xA) {
        func_8013FC64(D_80150054, 0x46, 0x2D, 0);
        func_8013FC64(-D_80150058, 0x5F, 0x2D, 0);
        func_8013E524(0xDB, 0x47);
    }
    func_8013FC64(-D_8015005C, 0x78, 0x2D, 0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E524);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E6AC);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013E89C);

s32 func_8013EAC0(void) {
    if (D_80150054 == 0 && D_80150058 == 0 && D_8015005C == 0) {
        while (1) {
            if (D_80150064 < 0xFF) {
                D_80150064 += 1;
            }
            func_8013E6AC();
            if (D_80150054 != 0 && D_80150058 != 0 && D_8015005C != 0) {
                return 1;
            }
            if (D_80150064 == 0xFF) {
                return 0;
            }
        }
    }
    return 1;
}

s32 func_8013EB90(s32 arg0) {
    s32 temp_v0;

    func_8013B1F0(D_80150040, D_80150044, D_8015004C, D_80150050);
    if ((D_8014F990 == 0) && ((D_80150040 == 2) || (D_80150040 == 8))) {
        if ((D_80150040 == 8) && (D_80150044 == 0xE)) {
            cd_read_sectors(0x71U, 0x801AE000, 0x8768);
            D_8015007C = 1;
            D_80150068 = arg0;
        } else if ((D_80150040 == 8) && (D_80150044 == 0x46)) {
            cd_read_sectors(0x71U, 0x801B0000, 0x85B6);
            D_8015007C = 1;
            D_80150068 = arg0;
        } else if (D_80150040 == 2) {
            temp_v0 = D_80150044;
            cd_read_sectors(D_8014F828[temp_v0], 0x801B0000, D_8014F6C0[temp_v0]);
            D_8015007C = 1;
            if (D_80150044 == 0x20) {
                D_80150068 = arg0;
            }
        }
    }
}

void func_8013ECDC(void) {
    func_801408F0();
    if (D_8014F990 == 1) {
        func_8013ED8C();
    } else if (D_8015007C == 1) {
        func_80136448();
    }
}

void func_8013ED38(void) {
    D_80150084 = (u8 *)0x80180084;
    D_80150088 = (u8 *)0x80180098;
    D_8015008C = (u8 *)0x8018009C;
    D_80150090 = *(s16 *)0x801960AC;
    D_80150094 = (u8 *)0x801800AC;
}

void func_8013ED8C(void) {
    func_8013ED38();
    func_800415B4(0, 0x40);
    func_80062DBC(D_80150094, D_80150084, D_80150088, D_8015008C, D_80150048);
    func_8013E094(D_80150088);
}

void func_8013EDF8(void) {
    if (D_8014F990 == 1) {
        func_80063668();
    }
}

void func_8013EE28(void) {
    if (D_8014F990 == 1) {
        func_8013EEAC();
    } else if (D_8015007C == 1) {
        func_8013EE7C();
    }
}

void func_8013EE7C(void) {
    D_8015007C = 0;
    D_801220D0 = 0x80000000;
    D_80120653 = 0;
    D_80120697 = 0;
    D_801206DB = 0;
}

void func_8013EEAC(void) {
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_80120653 = 0;
    D_801217D0[0].unk_00 = 0x80000000;
    D_801217D0[1].unk_00 = 0x80000000;
    D_801217D0[2].unk_00 = 0x80000000;
    D_801217D0[3].unk_00 = 0x80000000;
    D_801217D0[4].unk_00 = 0x80000000;
    D_801217D0[5].unk_00 = 0x80000000;
}

void func_8013EEFC(void) {
loop_1:
    func_8013E6AC();
    if ((D_80150054 == 0) && (D_80150058 == 0) && (D_8015005C == 0)) {
loop_4:
        if (D_80150064 < 0xFF) {
            D_80150064 += 1;
        }
        func_8013E6AC();
        if ((D_80150054 == 0) || (D_80150058 == 0) || (D_8015005C == 0)) {
            if (D_80150064 == 0xFF) {
                D_80150070 = -1;
            } else {
                goto loop_4;
            }
        }
    }
    if (D_8015004C != D_800E6280.unk_F5F) {
        if (D_80150070 == -1) {
            return;
        }
        if (D_80150064 < 0xFF) {
            D_80150064 += 1;
            goto loop_1;
        }
        if (D_80150064 == 0xFF) {
            D_80150070 = -1;
        } else {
            goto loop_1;
        }
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013F054);

void func_8013F320(s32 arg0) {
    switch (D_8015006C) {
    case 0:
        func_80046290(D_801500C4[arg0 * 2], D_801500C4[arg0 * 2 + 1], D_800E6280.unk_F5F);
        func_80044750(0x300);
        D_8015006C += 1;
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_8015006C += 1;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_8015006C += 1;
        }
        break;
    }
}

s32 func_8013F3F8(void) {
    if (D_80150040 == 0x15 &&
        (D_80150044 == 6 || D_80150044 == 7 || D_80150044 == 8 || D_80150044 == 9 || D_80150044 == 10)) {
        if ((D_8015004C == 0 || D_8015004C == 1 || D_8015004C == 4 || D_8015004C == 6 || D_8015004C == 9 ||
             D_8015004C == 10) &&
            D_80150048 == 5) {
            return 1;
        }
    }
    return 0;
}

void func_8013F498(void) {
    /* FAKE: unsigned halfword stores prevent IDO sharing constant 7 with the byte selector. T-9750. */
    func_801408F0();
    func_8013BEF0();
    func_80048F64(0x60);
    D_80120651 = 0;
    D_80120688 = 0x01000000;
    D_80120657 = 0x80;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0xD;
    D_8012065C = D_80150034;
    D_80120660 = D_80150038;
    D_80120684 = D_80150030;
    D_80120664 = D_8015003C;
    D_80120652 = 1;
    D_80120653 = 0x84;
    D_80120668 = 2;
    if (D_8015004C == 0) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 0;
        } else {
            D_80120666 = 1;
        }
    }
    if (D_8015004C == 1) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 2;
        } else {
            D_80120666 = 3;
        }
    }
    if (D_8015004C == 4) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 4;
        } else {
            D_80120666 = 5;
        }
        if (D_80150060 == 0xFF) {
            *(u16 *)&D_801217D0[0].unk_0C = 7;
            D_801217D0[0].unk_0E = 0x70;
        }
    }
    if (D_8015004C == 6) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 6;
        } else {
            D_80120666 = 7;
            D_8012067A = -0x12;
        }
    }
    if (D_8015004C == 9) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 8;
        } else {
            D_80120666 = 9;
        }
        if (D_80150060 == 0xFF) {
            *(u16 *)&D_801217D0[0].unk_0C = 7;
            D_801217D0[0].unk_0E = 0x70;
        }
    }
    if (D_8015004C == 0xA) {
        if ((D_80150058 == 6) || (D_80150058 == 7) || (D_80150058 == 8) || (D_80150058 == 9)) {
            D_80120666 = 0xA;
        } else {
            D_80120666 = 0xB;
        }
    }
}

void func_8013F77C(void) {
    /* FAKE: record view spans legacy scalar declarations; canonical main BSS aggregate is pending. T-9750. */
    /* FAKE: loop store grouped on header line retains as1 unrolled-store order. T-9750. */
    s32 i;

    for (i = 0; i < 13; i++) { D_800E6280.unk_53C[i] = 1; }
    D_800E6280.unk_53C[11] = 5;
    D_800E6280.unk_F5F = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_00 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_02 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_01 = 0x1D;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_03 = 0;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_04 = 0x60;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_05 = 4;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_06 = 0x19;
    ((EtcConfig8 *)&D_800E7D10)[11].unk_07 = 0;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_00 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_02 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_01 = 0x1E;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_03 = 0;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_04 = 0x60;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_05 = 8;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_06 = 0x19;
    ((EtcConfig8 *)&D_800E7D10)[12].unk_07 = 0;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_00 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_02 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_01 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_03 = 0;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_04 = 0x60;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_05 = 7;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_06 = 0x19;
    ((EtcConfig8 *)&D_800E7D10)[13].unk_07 = 0;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_00 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_02 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_01 = 0xA;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_03 = 0;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_04 = 0x60;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_05 = 2;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_06 = 0x19;
    ((EtcConfig8 *)&D_800E7D10)[14].unk_07 = 0;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_00 = 0xB;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_02 = 0x12;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_01 = 0;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_03 = 0;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_04 = 0x60;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_05 = 5;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_06 = 0x19;
    ((EtcConfig8 *)&D_800E7D10)[15].unk_07 = 0;
}
