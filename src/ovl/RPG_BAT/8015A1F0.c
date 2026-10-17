#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8015A1F0(void) {
    if (((s32) D_8015EDAC >= 6) && ((s32) D_8015EDAC < 9)) {
        func_8013F0F4(0x500, 0xE, 1);
    }
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　超球百裂弾　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
    default:
        break;
    case 2:
        func_8013D418();
        break;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        break;
    case 4:
        func_8014EDBC(0x34);
        break;
    case 5:
        func_8013E7C0(0x33, 6, 1, 1);
        func_8013E7C0(0x34, 4, 1, 1);
        func_8013E7C0(0x35, 5, 1, 1);
        func_8014EBA8();
        break;
    case 6:
        func_8013E97C(0x33, 0, (s32) (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        func_8013E97C(0x34, 0, (s32) (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        func_8013E97C(0x35, 0, (s32) (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        D_8015EDB0.unk_04 += 1;
        if (D_8015EDB0.unk_04 >= 0x3D) {
            func_8013E97C(0x33, 0, -0x30);
            func_8013E97C(0x34, 0, -0x30);
            func_8013E97C(0x35, 0, -0x30);
            func_8014EBA8();
        }
        break;
    case 7:
        func_8015A6D4();
        break;
    case 8:
        func_8015A744();
        D_8015EDB0.unk_04 += 1;
        if (D_8015EDB0.unk_04 >= 0x12D) {
            func_8014EBA8();
        }
        break;
    case 9:
        func_8015A8B4();
        if (D_8015EDB0.unk_04 == 0) {
            D_8015EDB0.unk_04 = 0x3C;
        }
        func_8013E97C(0x33, 0, (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        func_8013E97C(0x34, 0, (s32) (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        func_8013E97C(0x35, 0, (s32) (D_8015EDB0.unk_04 * -0x30 * D_8015EDB0.unk_04) / 3600);
        D_8015EDB0.unk_04 -= 1;
        if (D_8015EDB0.unk_04 >= 0xF) {
            func_8013F0F4(0x500, 0xE, 1);
        }
        if (D_8015EDB0.unk_04 <= 0) {
            func_8013F0F4(0x502, 1, 0);
            func_8013D350();
            func_8013E97C(0x33, 0, 0);
            func_8013E97C(0x34, 0, 0);
            func_8013E97C(0x35, 0, 0);
            func_8014EBA8();
        }
        break;
    case 10:
        func_8013E7C0(0x33, 0xFF, 1, 0);
        func_8013E7C0(0x34, 8, 5, 1);
        func_8013E7C0(0x35, 0xFF, 1, 0);
        func_8014EBA8();
        break;
    case 11:
        func_8014EDBC(0x34);
        break;
    case 12:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8014EBA8();
        break;
    case 13:
        func_8014ED44(0x14);
        break;
    case 14:
        func_8013D59C();
        break;
    case 15:
        func_8014EBEC();
        break;
    }
}


void func_8015A6D4(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        func_8013E810(i + 0x28, 7, 1, 1);
        func_8013E97C(i + 0x28, -0x19, 0x109);
    }
    func_8014EBA8();
}

/* FAKE: T-22013, two views of the displacement words: the scalars D_8015EC50/D_8015EC54 around the calls and
 * the RpgMotion20 pointer m for the negation and the rect word. Only this mix gives the original's promoted
 * registers (dx in $a0, the dead copy of dy in $a1, the copy of unk_26 in $a0 before the div); real source unknown. */
void func_8015A744(void) {
    DisplayActor44 *a;
    RpgMotion20 *m;
    s32 i;
    s32 r;

    m = (RpgMotion20 *)&D_8015EC50;
    a = (DisplayActor44 *)D_8011ECD0;
    for (i = 0; i < 8; i++) {
        if (a[i + 136].unk_26 < -0xB4 || a[i + 136].unk_2A >= 0x8D) {
            if (func_8013E9A8(5) == 0) {
                func_8013F0F4(0x501, 2, 2);
                func_8013E97C(i + 0x28, 0xF0, 0x28);
                D_8015EC50 = func_8013E9A8(8) + 5;
                D_8015EC54 = func_8013E9A8(D_8015EC50);
                m->dx = -m->dx;
                a[i + 136].unk_03 = 0x94;
                a[i + 136].unk_2E = m->dx;
                a[i + 136].unk_32 = D_8015EC54;
            } else {
                func_8013E810(i + 0x28, 7, 1, 0);
            }
        }
        m->rect.unk_10 = a[i + 136].unk_26;
        r = ((0x70 - m->rect.unk_10) << 12) / 240;
        a[i + 136].unk_1A = r;
        a[i + 136].unk_1C = r;
    }
}

/* FAKE: T-22013, x is reused as the offset of the first store. Reassigning x makes uopt keep the original's copy of the halfword (stored to D_8015EC68 from $a0) and compute i + 0x28 in the first block; real source unknown. p walks D_8011ECD0 as raw bytes: p + 0x2446/0x244A are DisplayActor44 unk_26/unk_2A of record i + 136, and after p += 0x44 the stores at p + 0x23F6/0x23F8 are unk_1A/unk_1C of the same record. */
void func_8015A8B4(void) {
    u8 *p;
    s32 i;
    s32 x;
    s32 r;
    s32 y;

    p = D_8011ECD0;
    i = 0;
    do {
        x = *(s16 *)(p + 0x2446);
        if (x < -0xB4 || *(s16 *)(p + 0x244A) >= 0x8D) {
            func_8013E810(i + 0x28, 7, 1, 0);
            x = *(s16 *)(p + 0x2446);
        }
        i++;
        y = x;
        r = ((0x70 - x) << 12) / 240;
        p += 0x44;
        x = 0x23F6;
        *(s16 *)(p + x) = r;
        *(s16 *)(p + 0x23F8) = r;
        D_8015EC68 = y;
    } while (i != 8);
}
