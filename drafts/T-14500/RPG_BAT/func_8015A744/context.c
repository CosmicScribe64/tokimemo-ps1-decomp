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

/* NON_MATCHING: T-12040, two dead register copies ($a1 = v, $a0 = x) and the order of the D_8015EC54 store differ, 17 lines */
void func_8015A744(void) {
    u8 *p;
    s32 i;
    s32 x;
    s32 r;
    s32 v;
    s32 n;
    s32 y;

    p = D_8011ECD0;
    i = 0;
    do {
        x = *(s16 *)(p + 0x2446);
        if (x < -0xB4 || *(s16 *)(p + 0x244A) >= 0x8D) {
            if (func_8013E9A8(5) == 0) {
                func_8013F0F4(0x501, 2, 2);
                func_8013E97C(i + 0x28, 0xF0, 0x28);
                r = func_8013E9A8(8) + 5;
                D_8015EC50 = r;
                v = func_8013E9A8(r);
                p[0x2423] = 0x94;
                n = -D_8015EC50;
                *(s16 *)(p + 0x244E) = n;
                *(s16 *)(p + 0x2452) = v;
                D_8015EC50 = n;
                D_8015EC54 = v;
                x = *(s16 *)(p + 0x2446);
            } else {
                func_8013E810(i + 0x28, 7, 1, 0);
                x = *(s16 *)(p + 0x2446);
            }
        }
        y = x;
        r = ((0x70 - x) << 12) / 240;
        i++;
        p += 0x44;
        *(s16 *)(p + 0x23F6) = r;
        *(s16 *)(p + 0x23F8) = r;
        D_8015EC68 = y;
    } while (i != 8);
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8015A1F0", func_8015A8B4);
