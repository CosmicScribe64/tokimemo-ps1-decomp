#include "common.h"
#include "ovl/TACO.h"

void func_80148FE0(void) {
    if (((u32) D_800E6280.unk_1100 < 0x60U) && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0("2ST BOSS", -0x30, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801490B4();
        break;
    case 1:
        func_8014927C();
        break;
    case 2:
        func_801493D4();
        break;
    case 3:
        func_80149538();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

void func_801490B4(void) {
    s32 i;
    s32 *a;
    u8 *b;
    Tc10 *c;
    s32 *d;

    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x30;
        func_80143730(0x80172000, 0xE);
        func_80143730(0x80162000, 0xC);
        func_80146E3C();
        func_80146D60(0);
        func_801471D0(0x80196000);
        func_80147400(0x80196000);
        func_80143730(0x80162000, 8);
        func_80146F74(0, D_8015EDB4->unk74, D_8015EDB4->unk76 + 0x44C, -0x640, -0xA, 0xB4, 0, 0, 0);
        for (i = 1; i < 0x24; i++) {
            func_801472D4(i, 0, D_8015FAE0[i - 1].a, D_8015FAE0[i - 1].b, D_8015FBF8[i - 1].a, D_8015FBF8[i - 1].b);
            D_80127080[i].unk0 = 0;
        }
        D_80128880[0].x = -0xA;
        D_80128880[0].y = 0xB4;
        D_80128880[0].z = 0;
        func_801521EC(1, 0);
        func_801521EC(2, 7);
        func_801521EC(3, 0xE);
        d = D_8015FC98;
        for (i = 0x1D; i != 0x21; i++) {
            func_801478F8(i, *d);
            d += 1;
        }
        func_801478F8(0x23, D_8015FC94);
        func_80149B7C();
        func_8004284C();
    }
}

void func_8014927C(void) {
    s32 i;

    func_80151D1C(3);
    func_8015266C(6);
    func_80152808(4);
    func_801532C4();
    for (i = 0; i < 0xC; i++) {
        func_801497EC(D_8015FCA8[i], i, D_8015FCD8, D_8015FCB4, D_8015FCC0);
    }
    if (D_800E6280.unk_1104.u % 240 == 0 && func_800AE0D0() % 3 == 0) {
        func_801335A0(0, 0x500);
    }
    if (D_800E6280.unk_1104.u < 0x12C) {
        func_80146F74(0, 0, -2, -0x28, 0, 0, 0, 0x3C, 0x3C);
    } else {
        func_8004284C();
    }
    D_800E6280.unk_1104.u++;
}

void func_801493D4(void) {
    s32 i;

    for (i = 0; i != 0xC; i++) {
        func_801497EC(D_8015FCA8[i], i, D_8015FCD8, D_8015FCB4, D_8015FCC0);
    }
    func_80149D1C();
    func_80151C30();
    if (D_800E6280.unk_1104.u % 240 == 0) {
        if (func_800AE0D0() % 3 == 0) {
            func_801335A0(0, 0x500);
        }
    }
    if (func_80147B24(0xC, D_8015FCC0) >= 0x55) {
        /* FAKE: the first byte is tested twice, as in the original asm (T-9330) */
        if (*((u8 *) D_8015EDB4 + 0x905) == 0 && *((u8 *) D_8015EDB4 + 0x98D) == 0 && *((u8 *) D_8015EDB4 + 0x905) == 0) {
            func_80154230(0x80196000, 9);
            D_8015EDCC += 1;
            func_8004284C();
        }
    }
    D_800E6280.unk_1104.u++;
}

void func_80149538(void) {
    s32 i;
    u8 shade;

    func_80146F74(0, 3, 5, -0x14, 0, 0, 0, 0, 0);
    if ((D_800E6280.unk_1104.w >= 0xB4) && (D_800E6280.unk_1104.w < 0xC7)) {
        func_80153E78(1, 0xE0, 0xE0, 0xE0);
    } else if (D_800E6280.unk_1104.w == 0xC7) {
        func_801335A0(0, 0x504);
    } else if ((D_800E6280.unk_1104.w >= 0xC8) && (D_800E6280.unk_1104.w < 0x140)) {
        if ((D_800E6280.unk_1104.w % 3) == 0) {
            func_80153E78(1, 0xF0, 0xF0, 0xF0);
        }
    } else if ((D_800E6280.unk_1104.w >= 0x140) && (D_800E6280.unk_1104.w < 0x1B8)) {
        shade = (D_800E6280.unk_1104.w * 2) - 0x280;
        func_80153E78(2, shade, shade, shade);
    } else if (D_800E6280.unk_1104.w == 0x1B8) {
        i = 1;
        do {
            D_80127080[i++].unk0 = 0x80000000;
        } while (i != 36);
        load_csr_tp(0x80162000, 0xC, 1, 0, 0, 0x100, 0x100);
        func_80143730(0x80172000, 0xE);
        D_80122760 = 0x14;
        D_80122764 = 0x64;
        D_80122768 = -0x64;
        D_8012276C = 0xB0;
        D_8012276D = 0xB0;
        D_8012276E = 0xB0;
        func_8009AD70(0, &D_80122760);
        D_80122770 = 0x14;
        D_80122774 = -0x64;
        D_80122778 = 0x64;
        D_8012277C = 0x80;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &D_80122770);
        D_80122780 = -0x14;
        D_80122784 = 0x14;
        D_80122788 = -0x64;
        D_8012278C = 0x60;
        D_8012278D = 0x60;
        D_8012278E = 0x60;
        func_8009AD70(2, &D_80122780);
        func_80042908(0x1F);
    }
    /* FAKE: the frame counter is read through its own symbol for the final `+= 1`. The original reads it twice, as two uopt
     * variables (the compare chain, then the increment); through GameState both reads join one web and the function is 33
     * rows off. Keep line with proof=func_80149538:33:d7dd7f6c in config/migrate_globals.txt, wiki/keep-lines.md. T-20820, T-21520 */
    D_800E7384 += 1;
}

/* NON_MATCHING: T-19019, 2 differing lines (1 row, row 182): the reload of the selector in the `if` body is `addu t2,v1,t3` (index first), the original has `addu t2,t3,v1` (base first). Frame, spill slot (0x4C) and every other instruction match.
 * Without a selector local, uopt merges the two reads of `unk84[1]` itself (PRE): the first test's load stays in $a0 on the paths that skip the body, and uopt inserts a reload at the end of the body. The PRE temporary takes the frame word below the `arg1 * 2` spill that the original has (a declared `u8 st` takes a word above it: spill 0x48, the earlier 4-line draft). `(idx + D_8015EDB4)` and `(D_8015EDB4 + idx)` compile alike (uopt orders the sum itself, index first); the inserted reload copies that order. A `D_8015EDB4[idx]` read is a different expression, so mixing the forms ends the merge (89 to 132 lines). With `st` and one loop counter every offset is right relative to the frame top, but the frame is 8 bytes short. Tried without effect on the frame: `st` in a nested block, `obj` as an expression (operands of the address compares must then be swapped), `<` loop tests, `do`/`while` loops, the first store through `idx`, a ternary for the merged value (its temporary sits above the spill, 21 lines). autotweak (437 tries) reaches 4 lines. */
#ifdef NON_MATCHING
s32 func_801497EC(s32 arg0, s32 arg1, s16 *arg2, u8 *arg3, u8 *arg4) {
    MATRIX mtx;
    TcPos in;
    TcPos out;
    s32 i;
    s32 j;
    s32 idx;
    s32 offX;
    s32 offY;
    TcObj50 *obj;

    idx = arg1 + 0x10;
    offX = arg2[arg1 * 2];
    offY = arg2[arg1 * 2 + 1];
    D_8015EDB4[arg1 + 16].unk84[0] = arg3[arg1];
    mtx = *(MATRIX *)D_80125C60;
    func_8015D0D0((D_80128880[0].x << 0xC) / 360, &mtx);
    func_8015CF30((D_80128880[0].z << 0xC) / 360, &mtx);
    func_8015D270((D_80128880[0].y << 0xC) / 360, &mtx);
    obj = &D_80127480[arg0];
    in.x = obj->unk18;
    in.y = obj->unk1C;
    in.z = obj->unk20;
    func_800A0ECC(&mtx, &in, &out);
    D_8015EDB4[idx].unk74 = D_80127480[0].unk18 + out.x + offX;
    D_8015EDB4[idx].unk76 = D_80127480[0].unk1C + out.y + offY;
    D_8015EDB4[idx].unk78 = D_80127480[0].unk20 + out.z;
    if ((idx + D_8015EDB4)->unk84[1] < 0x15U && (idx + D_8015EDB4)->unk82 < (idx + D_8015EDB4)->unk7A) {
        (idx + D_8015EDB4)->unk84[1] = arg4[arg1 * 2];
        (idx + D_8015EDB4)->unk82 += 1;
        D_8015EDB4[17].unk84[2] = 1;
    }
    if ((idx + D_8015EDB4)->unk84[1] == 0) {
        func_8015435C(arg0, 9);
        if (obj == &D_801274D0) {
            for (i = 4; i != 0x16; i += 3) {
                func_8015435C(i, 9);
            }
        }
        if (obj == &D_80127520) {
            for (j = 5; j != 0x17; j += 3) {
                func_8015435C(j, 9);
            }
        }
        if (obj == &D_80127570) {
            for (j = 6; j != 0x18; j += 3) {
                func_8015435C(j, 9);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801497EC);
#endif

void func_80149B7C(void) {
    s32 i;

    for (i = 0x10; i < 0x3F; i++) {
        func_80143B34(i);
    }
    for (i = 0; i < 0xC; i++) {
        func_80147928(D_8015FCA8[i], i, 0x80196000, D_8015FCB4, D_8015FCC0, 1);
    }
    *((u8 *) D_8015EDB4 + 0x906) = 0;
    *((u8 *) D_8015EDB4 + 0x883) = 0;
    *((u8 *) D_8015EDB4 + 0x882) = 0;
    *((u8 *) D_8015EDB4 + 0x98E) = 0;
    *((u8 *) D_8015EDB4 + 0x90B) = 0;
    *((u8 *) D_8015EDB4 + 0x90A) = 0;
    *((u8 *) D_8015EDB4 + 0xA16) = 1;
    *((u8 *) D_8015EDB4 + 0x993) = 0;
    *((u8 *) D_8015EDB4 + 0x992) = 0;
    *((u8 *) D_8015EDB4 + 0xA9E) = 1;
    *((u8 *) D_8015EDB4 + 0xA1B) = 0;
    *((u8 *) D_8015EDB4 + 0xA1A) = 0;
    *((u8 *) D_8015EDB4 + 0xB26) = 1;
    *((u8 *) D_8015EDB4 + 0xAA3) = 0;
    *((u8 *) D_8015EDB4 + 0xAA2) = 0;
    *((u8 *) D_8015EDB4 + 0xB2B) = 0;
    *((u8 *) D_8015EDB4 + 0xB2A) = 0;
    *((u8 *) D_8015EDB4 + 0xBB3) = 0;
    *((u8 *) D_8015EDB4 + 0xBB2) = 0;
}

void func_80149D1C(void) {
    s32 v;
    u8 b;

    v = func_80147B24(0xC, D_8015FCC0);
    if (*((u8 *) D_8015EDB4 + 0xBB2) < v) {
        D_80122760 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0x80;
        D_8012276E = 0x80;
        func_8009AD70(0, &D_80122760);
        D_80122770 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &D_80122770);
        D_80122780 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0x80;
        D_8012278E = 0x80;
        func_8009AD70(2, &D_80122780);
        b = *((u8 *) D_8015EDB4 + 0xBB3);
        if (b >= 3) {
            *((u8 *) D_8015EDB4 + 0xBB2) = v;
            D_80122760 = 0x14;
            D_80122764 = 0x64;
            D_80122768 = -0x64;
            D_8012276C = 0xB0;
            D_8012276D = 0xB0;
            D_8012276E = 0xB0;
            func_8009AD70(0, &D_80122760);
            D_80122770 = 0x14;
            D_80122774 = -0x64;
            D_80122778 = 0x64;
            D_8012277C = 0x80;
            D_8012277D = 0x80;
            D_8012277E = 0x80;
            func_8009AD70(1, &D_80122770);
            D_80122780 = -0x14;
            D_80122784 = 0x14;
            D_80122788 = -0x64;
            D_8012278C = 0x60;
            D_8012278D = 0x60;
            D_8012278E = 0x60;
            func_8009AD70(2, &D_80122780);
            *((u8 *) D_8015EDB4 + 0xBB3) = 0;
            return;
        }
        *((u8 *) D_8015EDB4 + 0xBB3) = b + 1;
    }
}
