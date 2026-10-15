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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149538);

/* bulk_m2c declarations (T-11190) */
extern s16 D_80128882;
extern s16 D_80128884;
extern s32 D_80127498;
extern s32 D_801274D0;
extern s32 D_80127520;
extern s32 D_80127570;
s32 func_800A0ECC();
/* end bulk_m2c declarations */
s32 func_801497EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp78[8];
    s16 sp74;
    s16 sp72;
    s16 sp70;
    s16 sp68;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    s32 sp4C;
    TcActor *temp_v0_2;
    TcActor *temp_v0_3;
    TcObj50 *temp_s2;
    s32 temp_v1;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    u8 var_a0;
    u8 *temp_v0;

    temp_v1 = arg1 * 2;
    sp5C = arg1 + 0x10;
    temp_v0 = arg2 + (temp_v1 * 2);
    sp58 = (s32) (*(s32 *)temp_v0);
    sp54 = (s32) *(s16 *)((unsigned char *)temp_v0 + (2));
    *(u8 *)((unsigned char *)(&D_8015EDB4[arg1]) + (2308)) = (u8) *(u8 *)(arg3 + arg1);
    (*(s32 *)&sp78) = (s32) (*(s32 *)D_80125C60);
    *(s32 *)((unsigned char *)(&sp78) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp78) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp78) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp78) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp78) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp78) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    *(s32 *)((unsigned char *)(&sp78) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    sp4C = temp_v1;
    func_8015D0D0((s32) (D_80128880->x << 0xC) / 360, (s32) &sp78);
    func_8015CF30((s32) (D_80128884 << 0xC) / 360, (s32) &sp78);
    func_8015D270((s32) (D_80128882 << 0xC) / 360, (s32) &sp78);
    temp_s2 = &D_80127480[arg0];
    sp70 = (s16) temp_s2->unk18;
    sp72 = (s16) temp_s2->unk1C;
    sp74 = (s16) temp_s2->unk20;
    func_800A0ECC(&sp78, &sp70, &sp68);
    D_8015EDB4[sp5C].unk74 = D_80127498 + sp68 + sp58;
    D_8015EDB4[sp5C].unk76 = D_8012749C + sp6A + sp54;
    D_8015EDB4[sp5C].unk78 = D_801274A0 + sp6C;
    temp_v0_2 = &D_8015EDB4[sp5C];
    var_a0 = temp_v0_2->unk84[1];
    if ((var_a0 < 0x15U) && (temp_v0_2->unk82 < temp_v0_2->unk7A)) {
        temp_v0_2->unk84[1] = *(s32 *)(sp4C + arg4);
        temp_v0_3 = &D_8015EDB4[sp5C];
        temp_v0_3->unk82 += 1;
        *(s8 *)((unsigned char *)D_8015EDB4 + (2446)) = 1;
        var_a0 = D_8015EDB4[sp5C].unk84[1];
    }
    if (var_a0 == 0) {
        func_8015435C(arg0, 9);
        var_s0 = 4;
        if (temp_s2 == &D_801274D0) {
            var_a0_2 = 4;
            do {
                func_8015435C(var_a0_2, 9);
                var_s0 += 3;
                var_a0_2 = var_s0;
            } while (var_s0 != 0x16);
        }
        var_s0_2 = 5;
        if (temp_s2 == &D_80127520) {
            var_a0_3 = 5;
            do {
                func_8015435C(var_a0_3, 9);
                var_s0_2 += 3;
                var_a0_3 = var_s0_2;
            } while (var_s0_2 != 0x17);
        }
        var_s0_3 = 6;
        if (temp_s2 == &D_80127570) {
            var_a0_4 = 6;
            do {
                func_8015435C(var_a0_4, 9);
                var_s0_3 += 3;
                var_a0_4 = var_s0_3;
            } while (var_s0_3 != 0x18);
        }
    }
}

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
