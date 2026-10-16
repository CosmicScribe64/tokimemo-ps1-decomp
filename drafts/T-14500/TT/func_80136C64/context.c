#include "common.h"
#include "ovl/TT.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-11820, 17 lines: the head (old D_80152AF4 value, *D_80158A6C, page) differs; the original loads the old value straight into v0 and keeps the page in the ugen temp t7, here uopt copies the old value through t7 into a0 and gives the page v0. The unrolled six-record loop (i + 1 compare) matches. Tried: old/t/v locals in all orders, compare against the globals, assignment in the condition. */
void func_80136830(void) {
    u8 *c;
    u8 *p;
    s32 i;
    s32 old;
    s32 v;

    c = D_80158A6C;
    v = *(s32 *)c;
    old = D_80152AF4;
    D_80152AF0 = old;
    D_80152AF4 = v / 0x8000;
    if (old != v / 0x8000) {
        func_80133AD0(0x611);
    }
    if (*(s32 *)c == 0) {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            *(s16 *)(p + 0x42) = 0x3C0A;
        }
    } else {
        for (i = 0, p = D_80158AA4 + 0xC8; i < 6; i++, p += 0x64) {
            if (*(s32 *)c / 0x8000 == i + 1) {
                *(s16 *)(p + 0x42) = 0x3C09;
            } else {
                *(s16 *)(p + 0x42) = 0x3C0A;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136830);
#endif

void func_801369F4(void) {
    s32 *p;
    u8 *q;
    u8 k;

    q = D_80158A6C;
    p = D_80158A70;
    p[0] = 0;
    k = q[0xB];
    *(s32 *)q = 0x8000;
    *(s32 *)(q + 4) = 0x8000;
    switch (k) {
    default:
    case 0:
        p[1] = (s32)D_80151A80;
        p[2] = (s32)D_80151AC8;
        p[3] = (s32)func_80139B98;
        return;
    case 1:
        p[1] = (s32)D_80154520;
        p[2] = (s32)D_80154564;
        p[3] = (s32)func_801365C4;
        return;
    case 2:
        p[1] = (s32)D_80155D60;
        p[2] = (s32)D_80155D9C;
        p[3] = (s32)func_8013BB28;
        return;
    case 3:
        p[1] = (s32)D_801564E0;
        p[2] = (s32)D_80156534;
        p[3] = (s32)func_80140D84;
        break;
    }
}

/* T-11000 near miss: 0xFF compare constant is in $t0 in the original, $at here; all later constants shift one register (about 40 lines). */
#ifdef NON_MATCHING
void func_80136AD8(void) {
    u8 *a;
    u8 *b;
    u8 *c;

    a = D_80158A60;
    b = D_80158A64;
    c = D_80158A6C;
    func_8009C5E0(1);
    if (b[0x22] != 0) {
        if (a[0x52] == 0xFF) {
            u8 *p = D_80158AA8 + 0x9D8;

            *(s16 *)(D_80158AA8 + 0x9E2) = -0x24;
            *(s16 *)(p + 0xC) = 0x74;
            *(s16 *)(p + 0xE) = 9;
            p[2] = 2;
            p[0] = 1;
            *(s16 *)(p + 6) = 0;
            *(s16 *)(p + 0x10) = 0;
        }
        switch (a[0x52]) {
        default:
            *(u8 **)(a + 0x4C) = D_80150A50;
            *(s16 *)(c + 8) = 0;
            c[0xB] = 0;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 1:
            *(u8 **)(a + 0x4C) = D_80152E20;
            *(s16 *)(c + 8) = 0;
            c[0xB] = 0;
            c[0xC] = 2;
            c[0xD] = 0;
            break;
        case 2:
            *(u8 **)(a + 0x4C) = D_80151AF0;
            *(s16 *)(c + 8) = 0xC3E;
            c[0xB] = 1;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 3:
            *(u8 **)(a + 0x4C) = D_80154590;
            *(s16 *)(c + 8) = 0x131F;
            c[0xB] = 2;
            c[0xC] = 0;
            c[0xD] = 0;
            break;
        case 4:
            *(u8 **)(a + 0x4C) = D_8014ED60;
            *(s16 *)(c + 8) = 0x235D;
            c[0xB] = 3;
            c[0xC] = 2;
            c[0xD] = 0;
            break;
        }
    } else {
        *(s16 *)(c + 8) = 0;
        c[0xB] = 0;
        c[0xC] = 2;
        c[0xD] = 0;
    }
}
#else
INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80136830", func_80136AD8);
#endif

/* bulk_m2c declarations (T-11190) */
extern s32 D_80156494;
/* end bulk_m2c declarations */
void func_80136C64(void) {
    u8 *sp5C;
    u8 *sp58;
    u8 *sp54;
    s32 *sp50;
    u8 *sp4C;
    u8 *sp48;
    u8 *sp44;
    s32 sp30;
    s16 temp_v0_3;
    s16 var_a1;
    u16 temp_v0;
    u16 temp_v0_5;
    u8 *temp_v0_4;
    u8 *temp_v1;
    u8 *var_a2;
    u8 *var_v1;
    u8 temp_v0_2;

    sp5C = D_80158A60;
    sp50 = D_80158A70;
    sp4C = D_80158AA8;
    sp44 = D_80158A74;
    var_a2 = D_80158A64;
    *D_80158A70 += (*(s32 *)D_80158A6C);
    temp_v0 = *(u16 *)((unsigned char *)var_a2 + (2));
    switch (temp_v0) {
    case 0x0:
        if (*(u8 *)((unsigned char *)var_a2 + (34)) != 0) {
            temp_v0_2 = *(u8 *)((unsigned char *)sp5C + (82));
            if ((temp_v0_2 == 1) || (temp_v0_2 == 2) || (temp_v0_2 == 3) || (temp_v0_2 == 4)) {
                sp54 = D_80158A6C;
                sp58 = var_a2;
                sp30 = func_800460DC();
                if ((((sp30 - func_801430C4()) + 0x100) & 0xFF) >= 5) {
                    func_80133970();
                    func_80044750(0x21);
                    func_80044750(0x22);
                    func_80044750(0x2F);
                    func_80044750(4);
                    func_80044750(0x201);
                    goto block_16;
                }
            } else {
                sp54 = D_80158A6C;
                sp58 = var_a2;
                func_80044750(0xB1);
                goto block_16;
            }
        } else {
            sp54 = D_80158A6C;
            sp58 = var_a2;
            func_80044750(0x21);
            func_80044750(0x22);
            func_80044750(0x2F);
            func_80044750(4);
            func_80044750(0x201);
block_16:
            func_800AE080(sp54, 0x20);
            func_800AE080((u8 *) sp50, 0x20);
            *(s16 *)((unsigned char *)sp5C + (80)) = 0;
            func_8014CBD0();
            func_80148210();
            func_80143570();
            func_8014A7E0();
            func_80146BE0();
            func_80148DB0();
            func_80147CE0();
            func_8014AF3C();
            func_80132F4C();
            func_80133B70();
            func_8013BD4C();
            func_801473BC();
            func_801426F0();
            if (*(u8 *)((unsigned char *)sp58 + (34)) != 0) {
                *(s8 *)((unsigned char *)sp58 + (17)) = 0;
            } else {
                *(s8 *)((unsigned char *)sp58 + (17)) = 1;
            }
            func_80147D08();
            func_8014A814();
            func_80148DE4();
            func_801436BC();
            func_8014AF70();
            func_80132F80();
            *(s8 *)((unsigned char *)sp58 + (18)) = 1;
            *(s32 **)((unsigned char *)sp58 + (24)) = &D_80156494;
            *(s8 *)((unsigned char *)sp58 + (28)) = 2;
            func_80136AD8();
            func_801369F4();
            *(s16 *)((unsigned char *)sp4C + (10)) = -0x70;
            *(s16 *)((unsigned char *)sp4C + (12)) = 0x10;
            *(s16 *)((unsigned char *)sp4C + (14)) = 0xD;
            *(s8 *)((unsigned char *)sp4C + (2)) = 1;
            (*(s32 *)sp4C) = 1;
            *(s16 *)((unsigned char *)sp4C + (6)) = 0;
            *(s16 *)((unsigned char *)sp4C + (16)) = 0;
            *(s16 *)((unsigned char *)sp4C + (30)) = -0x48;
            *(s16 *)((unsigned char *)sp4C + (32)) = 0x10;
            *(s16 *)((unsigned char *)sp4C + (34)) = 0x27;
            *(s8 *)((unsigned char *)sp4C + (22)) = 1;
            *(s8 *)((unsigned char *)sp4C + (20)) = 1;
            *(s16 *)((unsigned char *)sp4C + (26)) = 0;
            *(s16 *)((unsigned char *)sp4C + (36)) = 0;
            func_80148584((s32) *(u16 *)((unsigned char *)sp54 + (8)), 0x27);
            var_v1 = D_80158AA4;
            var_a1 = -0x70;
            do {
                *(s16 *)((unsigned char *)var_v1 + (20)) = var_a1;
                var_a1 += 0x10;
                var_v1 += 0x64;
                *(s8 *)((unsigned char *)var_v1 + (-100)) = 1;
                *(s8 *)((unsigned char *)var_v1 + (-99)) = 0;
                *(s16 *)((unsigned char *)var_v1 + (-32)) = 0;
                *(s16 *)((unsigned char *)var_v1 + (-78)) = 0x18;
                *(s16 *)((unsigned char *)var_v1 + (-40)) = 0x10;
                *(s16 *)((unsigned char *)var_v1 + (-38)) = 0x10;
                *(s8 *)((unsigned char *)var_v1 + (-30)) = 0;
                *(s8 *)((unsigned char *)var_v1 + (-29)) = 0x70;
                *(s16 *)((unsigned char *)var_v1 + (-36)) = 0xA;
                *(s16 *)((unsigned char *)var_v1 + (-34)) = 0x3C0B;
                *(s8 *)((unsigned char *)var_v1 + (-41)) = 0x65;
                *(s16 *)((unsigned char *)var_v1 + (-84)) = 0;
            } while (var_a1 < -0x50);
            sp48 = var_v1;
            func_80144CA8();
            *(s8 *)((unsigned char *)var_v1 + (71)) = 0x30;
            temp_v0_3 = 0xD8 - (2 * 8);
            *(s16 *)((unsigned char *)var_v1 + (122)) = 0xD0;
            *(s8 *)((unsigned char *)var_v1 + (70)) = 0x50;
            *(s8 *)((unsigned char *)var_v1 + (170)) = 0x50;
            *(s8 *)((unsigned char *)var_v1 + (171)) = 0x38;
            *(s16 *)((unsigned char *)var_v1 + (66)) = 0x3C09;
            *(s16 *)((unsigned char *)var_v1 + (166)) = 0x3C09;
            *(s16 *)((unsigned char *)var_v1 + (366)) = 0x3C09;
            *(s16 *)((unsigned char *)var_v1 + (466)) = 0x3C09;
            *(s16 *)((unsigned char *)var_v1 + (566)) = 0x3C09;
            *(s16 *)((unsigned char *)var_v1 + (62)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (162)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (362)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (462)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (562)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (20)) = -0x78;
            *(s16 *)((unsigned char *)var_v1 + (120)) = -0x78;
            *(s16 *)((unsigned char *)var_v1 + (320)) = -0x78;
            *(s16 *)((unsigned char *)var_v1 + (420)) = -0x78;
            *(s16 *)((unsigned char *)var_v1 + (520)) = -0x78;
            *(s8 *)((unsigned char *)var_v1 + (59)) = 0x65;
            *(s8 *)((unsigned char *)var_v1 + (159)) = 0x65;
            *(s8 *)((unsigned char *)var_v1 + (359)) = 0x65;
            *(s8 *)((unsigned char *)var_v1 + (459)) = 0x65;
            *(s8 *)((unsigned char *)var_v1 + (559)) = 0x65;
            *(s16 *)((unsigned char *)var_v1 + (64)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (164)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (364)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (464)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (564)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (60)) = 0x10;
            *(s16 *)((unsigned char *)var_v1 + (160)) = 0x10;
            *(s16 *)((unsigned char *)var_v1 + (360)) = 0x10;
            *(s16 *)((unsigned char *)var_v1 + (460)) = 0x10;
            *(s16 *)((unsigned char *)var_v1 + (560)) = 0x10;
            *(s16 *)((unsigned char *)var_v1 + (22)) = 0xD8;
            (*(s32 *)var_v1) = 1;
            *(s8 *)((unsigned char *)var_v1 + (1)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (68)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (16)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (100)) = 1;
            *(s8 *)((unsigned char *)var_v1 + (101)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (168)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (116)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (300)) = 1;
            *(s8 *)((unsigned char *)var_v1 + (301)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (368)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (316)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (400)) = 1;
            *(s8 *)((unsigned char *)var_v1 + (401)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (468)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (416)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (516)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (568)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (501)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (500)) = 1;
            *(s8 *)((unsigned char *)var_v1 + (200)) = 1;
            *(s8 *)((unsigned char *)var_v1 + (201)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (268)) = 0;
            *(s16 *)((unsigned char *)var_v1 + (220)) = -0x78;
            *(s16 *)((unsigned char *)var_v1 + (222)) = temp_v0_3;
            *(s16 *)((unsigned char *)var_v1 + (262)) = 8;
            *(s16 *)((unsigned char *)var_v1 + (260)) = 0x10;
            *(s8 *)((unsigned char *)var_v1 + (270)) = (s8) (((2 / 2) * 0x10) + 0x50);
            *(s16 *)((unsigned char *)var_v1 + (322)) = (s16) (temp_v0_3 - 8);
            *(s8 *)((unsigned char *)var_v1 + (271)) = (s8) (((2 & 1) * 8) + 0x30);
            *(s16 *)((unsigned char *)var_v1 + (264)) = 0xA;
            *(s16 *)((unsigned char *)var_v1 + (266)) = 0x3C09;
            *(s8 *)((unsigned char *)var_v1 + (259)) = 0x65;
            *(s16 *)((unsigned char *)var_v1 + (216)) = 0;
            *(s8 *)((unsigned char *)var_v1 + (370)) = (s8) (((3 / 2) * 0x10) + 0x50);
            *(s16 *)((unsigned char *)var_v1 + (422)) = (s16) (temp_v0_3 - 0x10);
            *(s8 *)((unsigned char *)var_v1 + (371)) = (s8) (((3 & 1) * 8) + 0x30);
            *(s8 *)((unsigned char *)var_v1 + (470)) = (s8) (((4 / 2) * 0x10) + 0x50);
            *(s16 *)((unsigned char *)var_v1 + (522)) = (s16) (temp_v0_3 - 0x18);
            *(s8 *)((unsigned char *)var_v1 + (471)) = (s8) (((4 & 1) * 8) + 0x30);
            temp_v1 = var_v1 + 0x1F4;
            *(s8 *)((unsigned char *)temp_v1 + (71)) = (s8) (((5 & 1) * 8) + 0x30);
            *(s8 *)((unsigned char *)temp_v1 + (70)) = (s8) (((5 / 2) * 0x10) + 0x50);
            *(s8 *)((unsigned char *)sp44 + (89)) = 0;
            *(s8 *)((unsigned char *)sp44 + (88)) = 0;
            var_a2 = sp58;
            *(s16 *)((unsigned char *)var_a2 + (2)) = 0x40;
        default:
block_29:
            if ((*(u8 *)((unsigned char *)var_a2 + (34)) != 0) && (*(u8 *)((unsigned char *)sp5C + (82)) == 0xFF)) {
                temp_v0_4 = D_80158AA8 + 0x9D8;
                if (((u16) *(u16 *)((unsigned char *)sp5C + (8)) >> 4) & 1) {
                    *(s16 *)((unsigned char *)temp_v0_4 + (6)) = 1;
                } else {
                    *(s16 *)((unsigned char *)temp_v0_4 + (6)) = 0;
                }
                temp_v0_5 = *(u16 *)((unsigned char *)sp5C + (20));
                if ((temp_v0_5 & 0x800) || (temp_v0_5 & 0x20)) {
                    (*(s32 *)var_a2) = 1;
                    *(s16 *)((unsigned char *)var_a2 + (6)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (4)) = 0;
                    *(s16 *)((unsigned char *)var_a2 + (2)) = 0;
                    *(s8 *)((unsigned char *)var_a2 + (11)) = 0;
                    *(s8 *)((unsigned char *)var_a2 + (10)) = 0;
                    *(s8 *)((unsigned char *)var_a2 + (9)) = 0;
                }
            }
            func_801480BC();
            func_801441BC();
            func_8014AB48();
            func_80147074();
            func_80149108();
            func_8013C0A8();
            func_801475C8();
            func_8014C680();
            func_801333D4();
            func_80134BBC();
            func_8013D150();
            func_80136830();
            func_801455C8();
        }
        break;
    case 0x1:
        sp58 = var_a2;
        func_80133A60();
        func_80044750(0x21);
        func_80044750(0x22);
        func_80044750(0x2F);
        func_80044750(4);
        func_80044750(0x201);
        func_8014A7E0();
        func_80146BE0();
        func_80148DB0();
        func_8014AF3C();
        func_80132F4C();
        func_80133B70();
        func_8013BD4C();
        func_801473BC();
        func_801426F0();
        *(s8 *)((unsigned char *)sp58 + (17)) = 1;
        *(s8 *)((unsigned char *)sp44 + (87)) = 0;
        *(s8 *)((unsigned char *)sp44 + (88)) = 0;
        *(s8 *)((unsigned char *)sp44 + (89)) = 0;
        *(s8 *)((unsigned char *)sp44 + (90)) = 0;
        *(s8 *)((unsigned char *)sp44 + (91)) = 0;
        *(s8 *)((unsigned char *)sp44 + (92)) = 0;
        *(s8 *)((unsigned char *)sp44 + (93)) = 0;
        *(s8 *)((unsigned char *)sp44 + (94)) = 0;
        *(s8 *)((unsigned char *)sp44 + (95)) = 0;
        *(s16 *)((unsigned char *)sp44 + (4)) = 0x20;
        *(s8 *)((unsigned char *)sp44 + (13)) = 0;
        *(s8 *)((unsigned char *)sp44 + (14)) = 0;
        *(s8 *)((unsigned char *)sp44 + (15)) = 0;
        *(s16 *)((unsigned char *)sp44 + (6)) = 0;
        *(s16 *)((unsigned char *)sp44 + (8)) = 0;
        *(s16 *)((unsigned char *)sp44 + (10)) = 0;
        func_8014A814();
        *(s8 *)((unsigned char *)sp58 + (18)) = 1;
        *(s8 *)((unsigned char *)sp58 + (28)) = 2;
        func_801369F4();
        *(s16 *)((unsigned char *)var_a2 + (2)) = 0x40;
        goto block_29;
    case 0x40:
        sp58 = var_a2;
        func_80148584((s32) *(u16 *)((unsigned char *)D_80158A6C + (8)), 0x27);
        if ((u16) *(u16 *)((unsigned char *)sp50 + (2)) >= 0x401U) {
            *(s16 *)((unsigned char *)D_80158A8C + (4)) = 0;
            *(s8 *)((unsigned char *)D_80158A8C + (12)) = 0;
            *(s16 *)((unsigned char *)D_80158A8C + (10)) = 0;
            *(s16 *)((unsigned char *)D_80158A8C + (8)) = 0;
            *(s16 *)((unsigned char *)D_80158A8C + (6)) = 0;
            *(s8 *)((unsigned char *)D_80158A8C + (15)) = 0;
            *(s8 *)((unsigned char *)D_80158A8C + (14)) = 0;
            *(s8 *)((unsigned char *)D_80158A8C + (13)) = 0;
            *(s16 *)((unsigned char *)sp58 + (2)) = 0x50;
            var_a2 = sp58;
        }
        goto block_29;
    case 0x50:
        sp58 = var_a2;
        func_80148584((s32) *(u16 *)((unsigned char *)D_80158A6C + (8)), 0x27);
        (*(s32 (**)())((unsigned char *)sp50 + (12)))();
        goto block_29;
    case 0xA0:
        sp58 = var_a2;
        func_80148584((s32) *(u16 *)((unsigned char *)D_80158A6C + (8)), 0x27);
        *(s16 *)((unsigned char *)sp44 + (4)) = 0xA0;
        goto block_29;
    }
}
