#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80149F90);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A070);

void func_8014A1D4(void) {
    func_8014F210();
    func_8015185C();
    if (((u32)D_800E6280.unk_1104.w % 180U) == 0) {
        func_801335A0(0, 0x500);
    }
    D_800E6280.unk_1104.w += 1;
}

void func_8014A234(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        func_8014A480(D_8015FE94[i], i, D_8015FEC4, D_8015FEA0, D_8015FEAC, D_800E6280.unk_1104.w);
    }
    func_8014A8E0();
    func_8014F2A0();
    if (D_800E6280.unk_1104.w % 180U == 0) {
        func_801335A0(0, 0x500);
    }
    func_8015185C();
    if (func_80147B24(0xB, D_8015FEAC) >= 0x55 && D_8015EDB4[16].unk84[1] == 0 && D_8015EDB4[19].unk84[1] == 0 && D_8015EDB4[20].unk84[1] == 0) {
        D_8015EDCC += 1;
        D_800E6280.unk_1104.w = 0;
        func_8004284C();
    }
    D_800E6280.unk_1104.w += 1;
}

void func_8014A390(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        func_8014A480(D_8015FE94[i], i, D_8015FEC4, D_8015FEA0, D_8015FEAC, 0);
    }
    func_8014F5F0();
}

void func_8014A42C(void) {
    func_8014A480(D_8015FE95, 1, D_8015FEC4, D_8015FEA0, D_8015FEAC, 1);
    func_8014F820();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A480);

void func_8014A79C(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        func_80147928(D_8015FE94[i], i, 0x801A7000, D_8015FEA0, D_8015FEAC, 1);
    }
    D_8015EDB4[16].unk3 = 0;
    D_8015EDB4[16].unk2 = 0;
    D_8015EDB4[17].unk3 = 0;
    D_8015EDB4[17].unk2 = 0;
    D_8015EDB4[18].unk84[2] = 0;
    D_8015EDB4[18].unk3 = 0;
    D_8015EDB4[18].unk2 = 0;
    D_8015EDB4[19].unk84[2] = 0;
    D_8015EDB4[19].unk3 = 0;
    D_8015EDB4[19].unk2 = 0;
    D_8015EDB4[20].unk84[2] = 0;
    D_8015EDB4[20].unk2 = 0;
    D_8015EDB4[21].unk3 = 0;
    D_8015EDB4[21].unk2 = 0;
}

void func_8014A8E0(void) {
    u32 v;
    u8 t;

    v = func_80147B24(0xB, D_8015FEAC);
    if (D_8015EDB4[21].unk2 < v) {
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
        t = D_8015EDB4[21].unk3;
        if (t >= 3U) {
            D_8015EDB4[21].unk2 = v;
            D_80122760 = 0x64;
            D_80122764 = 0x32;
            D_80122768 = -0x64;
            D_8012276C = 0x70;
            D_8012276D = 0x70;
            D_8012276E = 0x80;
            func_8009AD70(0, &D_80122760);
            D_80122770 = 0;
            D_80122774 = -0x64;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &D_80122770);
            D_80122780 = -0x64;
            D_80122784 = 0x32;
            D_80122788 = -0x64;
            D_8012278C = 0x80;
            D_8012278D = 0x80;
            D_8012278E = 0x90;
            func_8009AD70(2, &D_80122780);
            D_8015EDB4[21].unk3 = 0;
        } else {
            D_8015EDB4[21].unk3 = t + 1;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014AB50);

void func_8014B2A4(s32 arg0) {
    u8 t;

    func_8014D2E0();
    t = D_8015EDB4[16].unk84[2];
    if (t == 0 || t == 2 || t == 3 || t == 4) {
        func_80147B24(0xB, D_8015FA68);
        if (D_8015EDB4[17].unk2 == 1 && arg0 >= 0x259) {
            D_8015EDB4[17].unk84[2] = 1;
        }
    }
    if (D_8015EDB4[17].unk84[2] == 1) {
        func_8014B374(2);
    }
    if (D_8015EDB4[17].unk84[2] == 2) {
        func_8014B4B4(2);
    }
}

void func_8014B374(s32 arg0) {
    s32 i;

    if ((u32)arg0 < D_8015EDB4[17].unk3) {
        if (D_8015EDB4[17].unk2 == 1) {
            func_801335A0(0, 0x501);
        }
        for (i = 1; i < 0x1F; i++) {
            func_801472D4(i, D_8015EDB4[17].unk2, D_8015FEF0[i * 2 - 2], D_8015FEF0[i * 2 - 1], D_8015FFE0[i * 2 - 2], D_8015FFE0[i * 2 - 1]);
        }
        D_8015EDB4[17].unk2 += 1;
        D_8015EDB4[17].unk3 = 0;
    }
    if (D_8015EDB4[17].unk2 >= 0x1EU) {
        D_8015EDB4[17].unk84[2] = 0;
        D_8015EDB4[17].unk3 = D_8015EDB4[17].unk84[2];
        D_8015EDB4[17].unk2 = 0x1D;
        return;
    }
    D_8015EDB4[17].unk3 += 1;
}

void func_8014B4B4(s32 arg0) {
    s32 i;

    if ((u32)arg0 < D_8015EDB4[17].unk3) {
        if (D_8015EDB4[17].unk2 == 0x1D) {
            func_801335A0(0, 0x501);
        }
        for (i = 1; i < 0x1F; i++) {
            func_801472D4(i, D_8015EDB4[17].unk2, D_8015FEF0[i * 2 - 2], D_8015FEF0[i * 2 - 1], D_8015FFE0[i * 2 - 2], D_8015FFE0[i * 2 - 1]);
        }
        D_8015EDB4[17].unk2 -= 1;
        D_8015EDB4[17].unk3 = 0;
    }
    if (D_8015EDB4[17].unk2 == 0) {
        D_8015EDB4[17].unk84[2] = 0;
        D_8015EDB4[17].unk3 = D_8015EDB4[17].unk84[2];
        D_8015EDB4[17].unk2 = 1;
        return;
    }
    D_8015EDB4[17].unk3 += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B5F0);

void func_8014B790(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, u32 arg4) {
    s32 i;
    u8 t;

    if (arg4 < *arg0) {
        func_8014B8B4(arg2, arg3, *arg1);
        *arg1 += 1;
        *arg0 = 0;
    }
    if (*arg1 >= 8U) {
        D_8015EDB4[20].unk84[2] = 0;
        t = D_8015EDB4[20].unk84[2];
        *arg1 = t;
        *arg0 = t;
        for (i = arg2; i < arg2 + 3; i++) {
            func_801472D4(i, 1, D_8015FEF0[i * 2 - 2], D_8015FEF0[i * 2 - 1], D_8015FFE0[i * 2 - 2], D_8015FFE0[i * 2 - 1]);
        }
    } else {
        *arg0 += 1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B8B4);

void func_8014BC80(s32 arg0, s32 arg1) {
    s32 i;
    s32 n;
    s32 *p;

    /* FAKE: init statements on one source line; as1 schedules by source line and only this layout orders the prologue like the original. T-8030 */
    i = 0; n = arg0; p = &D_8016001C[arg1];
    for (; i < 3; ) {
        func_801478F8(n, *p);
        i++;
        n++;
        p++;
    }
}

void func_8014BCEC(s32 arg0) {
    s32 i;
    u8 t;

    if ((u32)arg0 < D_8015EDB4[21].unk3) {
        func_8014BE14(D_8015EDB4[21].unk2);
        D_8015EDB4[21].unk2 += 1;
        D_8015EDB4[21].unk3 = 0;
    }
    if (D_8015EDB4[21].unk2 >= 0x24U) {
        D_8015EDB4[21].unk84[2] = 0;
        t = D_8015EDB4[21].unk84[2];
        D_8015EDB4[21].unk2 = t;
        D_8015EDB4[21].unk3 = t;
        for (i = 1; i < 4; i++) {
            func_801472D4(i, 1, D_8015FEF0[i * 2 - 2], D_8015FEF0[i * 2 - 1], D_8015FFE0[i * 2 - 2], D_8015FFE0[i * 2 - 1]);
        }
        return;
    }
    D_8015EDB4[21].unk3 += 1;
}

void func_8014BE14(s32 arg0) {
    s32 i;

    if (arg0 < 8) {
        for (i = 0; i < 3; i++) {
            func_80147674(i + 1, D_80160034[i], 1, 0, 0);
        }
    } else if (arg0 >= 0x1C && arg0 < 0x24) {
        for (i = 0; i < 3; i++) {
            func_80147674(i + 1, D_80160034[i], 2, 0, 0);
        }
    }
}

void func_8014BED8(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_801478F8(i + 1, D_80160034[i]);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BF38);

void func_8014C0D4(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i < 5; i++) {
            func_80147674(i + 1, D_80160040[i], 1, 0, 0);
        }
        for (i = 0; i < 9; i++) {
            func_80147674(i + 0x13, D_80160054[i], 1, 0, 0);
        }
        for (i = 0; i < 10; i++) {
            func_80147674(i + 7, D_80160078[i], 1, 0, 0);
        }
        for (i = 0; i < 2; i++) {
            func_80147674(i + 0x1D, D_801600A0[i], 1, 0, 0);
        }
    }
}

void func_8014C1DC(s32 arg0) {
    s32 i;

    if (arg0 < 0xA) {
        for (i = 0; i < 5; i++) {
            func_80147674(i + 1, D_80160040[i], 2, 0, 0);
        }
        for (i = 0; i < 9; i++) {
            func_80147674(i + 0x13, D_80160054[i], 2, 0, 0);
        }
        for (i = 0; i < 10; i++) {
            func_80147674(i + 7, D_80160078[i], 2, 0, 0);
        }
        for (i = 0; i < 2; i++) {
            func_80147674(i + 0x1D, D_801600A0[i], 2, 0, 0);
        }
    }
}

void func_8014C2E4(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_801478F8(i + 1, D_80160040[i]);
    }
    for (i = 0; i < 9; i++) {
        func_801478F8(i + 0x13, D_80160054[i]);
    }
    for (i = 0; i < 10; i++) {
        func_801478F8(i + 7, D_80160078[i]);
    }
    for (i = 0; i < 2; i++) {
        func_801478F8(i + 0x1D, D_801600A0[i]);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C3B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C578);

void func_8014C8DC(void) {
    s32 i;

    func_801478F8(0x12, D_801600A8);
    for (i = 0; i < 2; i++) {
        func_801478F8(i + 0x19, D_801600AC[i]);
    }
    func_801478F8(8, D_801600B4);
    for (i = 0; i < 2; i++) {
        func_801478F8(i + 4, D_801600B8[i]);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C978);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014CB24);

void func_8014D200(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_801478F8(i + 1, D_801600C0[i]);
    }
    for (i = 0; i < 3; i++) {
        func_801478F8(i + 0x19, D_801600D4[i]);
    }
    for (i = 0; i < 2; i++) {
        func_801478F8(i + 0x1D, D_801600E0[i]);
    }
    func_801478F8(8, D_801600E8);
    for (i = 0; i < 2; i++) {
        func_801478F8(i + 0x15, D_801600EC[i]);
    }
}

void func_8014D2E0(void) {
    switch (D_8015EDB4[16].unk84[2]) {
    case 0:
        func_8014D3B4(1);
        break;
    case 1:
        func_8014D918();
        break;
    case 2:
        func_8014DC94();
        break;
    case 3:
        func_8014E048();
        break;
    case 4:
        func_8014D3B4(-1);
        break;
    case 5:
        func_8014E4A4();
        break;
    case 6:
        func_8014EA4C();
        break;
    case 7:
        func_8014EC4C();
        break;
    case 8:
        func_8014EE3C();
        break;
    }
}

void func_8014D3B4(s32 arg0) {
    if (D_8015EDB4[16].unk3 >= 3) {
        func_8014D530(D_8015EDB4[16].unk2, arg0);
        D_8015EDB4[16].unk2 += 1;
        D_8015EDB4[16].unk3 = 0;
        if (!(D_8015EDB4[16].unk2 & 1)) {
            func_80147C98(0, 0x1B, 0x1F4, (func_800AE0D0() & 0xFF) - 0x80, 0x7D0, 0x12C, 0);
            func_80147C98(0, 0x1B, -0x1F4, (func_800AE0D0() & 0xFF) - 0x80, 0x7D0, 0x12C, 0);
        }
    }
    if (D_8015EDB4[16].unk2 >= 0x3C || func_800AE0D0() % 200 == 0) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
    } else {
        D_8015EDB4[16].unk3 += 1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D530);

void func_8014D918(void) {
    u8 v;

    v = D_8015EDB4[16].unk2;
    if (v < 0x19 || v >= 0x7D) {
        if (D_8015EDB4[16].unk3 >= 2) {
            func_8014DA6C(v);
            D_8015EDB4[16].unk2 += 1;
            D_8015EDB4[16].unk3 = 0;
            v = D_8015EDB4[16].unk2;
        }
    } else if (v >= 0x19 && v < 0x7D) {
        func_8014DA6C(v);
        D_8015EDB4[16].unk2 += 1;
        D_8015EDB4[16].unk3 = 0;
        v = D_8015EDB4[16].unk2;
    }
    if (v >= 0x96 || D_8015EDB4[17].unk84[2] == 1) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
    } else {
        D_8015EDB4[16].unk3 += 1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014DA6C);

void func_8014DC94(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014DF50(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 += 1;
        D_8015EDB4[16].unk3 = 0;
        if (!(D_8015EDB4[16].unk2 & 1)) {
            if (D_8015EDB4[17].unk2 == 1) {
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) + 0xF4, (func_800AE0D0() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) - 0x2F4, (func_800AE0D0() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
            } else if (func_80147B24(0xB, D_8015FA68) < 0x32) {
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) + 0x2C, (func_800AE0D0() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) - 0x22C, (func_800AE0D0() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
            } else {
                func_80147C98(0, 0x1B, (func_800AE0D0() & 0x3FF) - 0x200, (func_800AE0D0() & 0x3FF) - 0x200, 0x7D0, 0x190, 0x81);
            }
        }
    }
    if (D_8015EDB4[16].unk2 >= 0x14U) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 += 1;
}

void func_8014DF50(s32 arg0) {
    s32 t;

    t = (-0x41A0 - D_801274A0) / 15;
    if (arg0 < 5) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        func_80146F74(0, -0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
}

void func_8014E048(void) {
    u8 v;

    v = D_8015EDB4[16].unk2;
    if (v < 0x14 || v >= 0x32) {
        if (D_8015EDB4[16].unk3 >= 3) {
            func_8014E1A0(v);
            D_8015EDB4[16].unk2 += 1;
            D_8015EDB4[16].unk3 = 0;
            v = D_8015EDB4[16].unk2;
        }
    } else if (v >= 0x14 && v < 0x32) {
        if (D_8015EDB4[16].unk3 != 0) {
            func_8014E1A0(v);
            D_8015EDB4[16].unk2 += 1;
            D_8015EDB4[16].unk3 = 0;
            v = D_8015EDB4[16].unk2;
        }
    }
    if (v >= 0x50) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
    } else {
        D_8015EDB4[16].unk3 += 1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E1A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E4A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E5F8);

void func_8014EA4C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EB24(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x50U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EB24(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, 0, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[24].unk84[2] = 1;
        func_8014C8DC();
    } else if ((arg0 >= 0xF) && (arg0 < 0x41)) {
        if (D_8015EDB4[24].unk84[2] == 1) {
            func_8014C3B4(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x41) && (arg0 < 0x50)) {
        func_80146F74(0, 0, 0xF, 0x1E, 0, 0, 0, 0x32, 0x1F4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014EC4C);

void func_8014EDCC(s32 arg0) {
    if (arg0 == 1) {
        D_8015EDB4[21].unk84[2] = 1;
        func_8014BED8();
    } else if ((arg0 >= 2) && (arg0 < 0x32)) {
        if (D_8015EDB4[21].unk84[2] == 1) {
            func_8014BCEC(0);
        }
    }
}

void func_8014EE3C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EF14(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x73U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EF14(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, -1, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[25].unk84[2] = 1;
        func_8014D200();
    } else if ((arg0 >= 0xF) && (arg0 < 0x64)) {
        if (D_8015EDB4[25].unk84[2] == 1) {
            func_8014C978(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x64) && (arg0 < 0x72)) {
        func_80146F74(0, 0, 0xF, 0x1E, 1, 0, 0, 0x32, 0x1F4);
    }
}

void func_8014F044(void) {
    s32 r;
    s32 h;
    u8 t;

    r = func_800AE0D0();
    t = D_8015EDB4[17].unk2;
    /* FAKE: `t ^ 0` only swaps the operands of the first bne (const reg first); found by the permuter, real source unknown. T-8030 */
    if ((t ^ 0) == 1) {
        h = r % 100;
        if (h < 0x14) {
            D_8015EDB4[16].unk84[2] = 0;
        } else if (h >= 0x28 && h < 0x37) {
            if (D_8015EDB4[21].unk84[1] != 0 || D_8015EDB4[25].unk84[1] != 0) {
                D_8015EDB4[16].unk84[2] = 1;
            } else {
                D_8015EDB4[16].unk84[2] = 7;
            }
        } else if (h >= 0x37 && h < 0x46) {
            D_8015EDB4[16].unk84[2] = 2;
        } else if (h >= 0x46 && h < 0x5A) {
            D_8015EDB4[16].unk84[2] = 3;
        } else if (h >= 0x14 && h < 0x28) {
            D_8015EDB4[16].unk84[2] = 4;
        } else if (h >= 0x5A) {
            D_8015EDB4[16].unk84[2] = 7;
        }
    } else if (t == 0x1D) {
        h = r % 100;
        if (h >= 0 && h < 0x28) {
            D_8015EDB4[16].unk84[2] = 2;
        } else if (h >= 0x28 && h < 0x46) {
            D_8015EDB4[16].unk84[2] = 5;
        } else if (h >= 0x46 && h < 0x5F) {
            D_8015EDB4[16].unk84[2] = 6;
        } else if (h >= 0x5F) {
            D_8015EDB4[16].unk84[2] = 8;
        }
    }
}

void func_8014F210(void) {
    func_80151264(5);
    func_8014FBD4(7);
    func_80146F74(0, 0, 1, 0, 0, 0, 0, 0x32, 0x1F4);
    if (func_800AE0C0(D_8012749C - D_8015EDB4->unk76) < 0x64) {
        func_8014A79C();
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F2A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F5F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F9B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FBD4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FDC0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FFDC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801508D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150B3C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150DE8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150F44);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801511C0);

void func_80151264(u32 arg0) {
    if (arg0 < D_8015EDB4[16].unk3) {
        func_8015131C(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if (D_8015EDB4[16].unk2 >= 0x7CU) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8015131C);

void func_8015163C(s32 arg0, s32 arg1) {
    if (!(arg1 & 1)) {
        D_80127080[arg0].unk0 = 0x40000000;
        return;
    }
    D_80127080[arg0].unk0 = 0x80000000;
}

void func_80151678(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x32;
    D_80122768 = -0x64;
    D_8012276C = 0x70;
    D_8012276D = 0x70;
    D_8012276E = 0x80;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = -0x64;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0x32;
    D_80122788 = -0x64;
    D_8012278C = 0x80;
    D_8012278D = 0x80;
    D_8012278E = 0x90;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

void func_801517AC(s32 arg0) {
    s32 level;
    s32 color;

    level = arg0;
    if (level < 0x100) {
        color = level * 0x10101;
    } else {
        color = 0xFFFFFF;
    }
    func_80049A40(-0x100, -0x78, 0x100, 0xF0, 1, color, 0x81);
    func_80049A40(0, -0x78, 0x100, 0xF0, 1, color, 0x81);
    dtd_on_tpage(0, 0, 1, 1, 1);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8015185C);
