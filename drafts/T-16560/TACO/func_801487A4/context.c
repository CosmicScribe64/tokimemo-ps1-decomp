#include "common.h"
#include "ovl/TACO.h"

void func_801480B0(void) {
    if ((u32)D_800E6280.unk_1100 < 0x60 && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0((s32)"1ST BOSS", -0x30, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80148184();
        break;
    case 1:
        func_80148308();
        break;
    case 2:
        func_801483B0();
        break;
    case 3:
        func_801484E4();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_80148184);

void func_80148308(void) {
    s32 i;
    u32 t;

    func_8013A790(1, 1);
    t = D_800E6280.unk_1104.u;
    if (t < 0x47E) {
        D_800E6280.unk_1104.u = t; /* FAKE: the original writes the counter back before the call (store in the jal delay slot), source unknown. T-9370 */
        func_8014AB50(t);
    } else {
        func_80148B58(1);
        for (i = 1; i < 0x1F; i++) D_80127080[i].unk0 = 0;
        func_8004284C();
    }
    D_800E6280.unk_1104.u += 1;
}

void func_801483B0(void) {
    s32 i;

    func_8013A790(1, 1);
    for (i = 0; i < 0xB; i++) {
        func_801487A4(D_8015FA44[i], i, D_8015FA80, D_8015FA50, D_8015FA68);
    }
    func_80148D6C();
    func_8014B2A4(D_800E6280.unk_1104.w);
    if (func_80147B24(0xB, D_8015FA68) >= 0x50) {
        D_80127090.unk0 = 0x40000000;
        for (i = 3; i < 0x1F; i++) {
            D_80127080[i].unk0 = 0x40000000;
            /* FAKE: loop-invariant store; uopt hoists it behind the loop address setup, which places it as the original does. Real source unknown. T-10320 */
            D_801270A0.unk0 = 0x40000000;
        }
        func_801335A0(0, 0x505);
        D_8015EDCC += 1;
        func_8004284C();
    }
    D_800E6280.unk_1104.w += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_801484E4);

void func_8014866C(void) {
    D_80122760 = -0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x1E;
    D_8012276C = 0xE0;
    D_8012276D = 0xE0;
    D_8012276E = 0xE0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x28;
    D_80122774 = 0x32;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0xA;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x30;
    D_8012278D = 0x30;
    D_8012278E = 0x30;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

/* bulk_m2c declarations (T-11190) */
s32 func_800A0ECC();
/* end bulk_m2c declarations */
s32 func_801487A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp50;
    s32 sp4E;
    MATRIX sp5C;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s16 sp4C;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    TcObj50 *temp_v0_4;
    s32 temp_t0;
    u8 *temp_v0_5;
    u8 *temp_v0_6;
    u8 temp_v0;
    u8 var_a0;
    u8 *temp_a0;
    u8 *temp_v0_2;
    u8 *temp_v0_3;

    temp_v0 = D_8015EDB4[17].unk2;
    temp_t0 = arg1 + 0x10;
    if (temp_v0 == 1) {
        temp_v0_2 = arg2 + (arg1 * 8);
        sp44 = (s32) (*(s32 *)temp_v0_2);
        temp_a0 = (arg1 * 2) + arg3;
        sp40 = (s32) *(s16 *)((unsigned char *)temp_v0_2 + (2));
        D_8015EDB4[temp_t0].unk84[0] = (*(s32 *)temp_a0);
        if ((arg0 == 0x1C) && (D_8015EDB4[16].unk84[2] == 1)) {
            D_8015EDB4[temp_t0].unk84[0] = *(u8 *)((unsigned char *)temp_a0 + (1));
        }
        if (((arg0 == 0x1C) || (arg0 == 0x1B) || (arg0 == 0x15)) && (D_8015EDB4[21].unk84[2] == 1)) {
            D_8015EDB4[temp_t0].unk84[0] = *(u8 *)((unsigned char *)temp_a0 + (1));
        }
    } else if ((temp_v0 == 0x1D) && ((temp_v0_3 = arg2 + (arg1 * 8), sp44 = (s32) *(s16 *)((unsigned char *)temp_v0_3 + (4)), sp40 = (s32) *(s16 *)((unsigned char *)temp_v0_3 + (6)), D_8015EDB4[temp_t0].unk84[0] = *(u8 *)((unsigned char *)((arg1 * 2) + arg3) + (1)), (arg0 == 0x1B)) || (arg0 == 0x1C)) && (D_8015EDB4[22].unk84[2] != 0) && (D_8015EDB4[23].unk84[2] == 0)) {
        D_8015EDB4[temp_t0].unk84[0] = 0;
    }
    sp5C = *(MATRIX *)D_80125C60;
    sp3C = temp_t0 * 0x88;
    func_8015D0D0((s32) (D_80128880->x << 0xC) / 360, (s32) &sp5C);
    func_8015CF30((s32) (D_80128880[0].z << 0xC) / 360, (s32) &sp5C);
    func_8015D270((s32) (D_80128880[0].y << 0xC) / 360, (s32) &sp5C);
    temp_v0_4 = &D_80127480[arg0];
    sp54 = (s16) temp_v0_4->unk18;
    sp56 = (s16) temp_v0_4->unk1C;
    sp58 = (s16) temp_v0_4->unk20;
    func_800A0ECC(&sp5C, &sp54, &sp4C);
    *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[sp3C]) + (116)) = (s16) (D_80127480[0].unk18 + sp4C + sp44);
    *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[sp3C]) + (118)) = (s16) (D_80127480[0].unk1C + sp4E + sp40);
    *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[sp3C]) + (120)) = (s16) (D_80127480[0].unk20 + sp50);
    temp_v0_5 = &D_8015EDB4->unk0[sp3C];
    var_a0 = *(u8 *)((unsigned char *)temp_v0_5 + (133));
    if ((var_a0 < 0x15U) && (*(s16 *)((unsigned char *)temp_v0_5 + (130)) < *(s16 *)((unsigned char *)temp_v0_5 + (122)))) {
        *(u8 *)((unsigned char *)temp_v0_5 + (133)) = (u8) *(u8 *)((arg1 * 2) + arg4);
        temp_v0_6 = &D_8015EDB4->unk0[sp3C];
        *(s16 *)((unsigned char *)temp_v0_6 + (130)) = (s16) (*(s16 *)((unsigned char *)temp_v0_6 + (130)) + 1);
        var_a0 = *(u8 *)((unsigned char *)(&D_8015EDB4->unk0[sp3C]) + (133));
    }
    if (var_a0 == 0) {
        if (!(D_800E6280.unk_1104.w & 1)) {
            D_80127080[arg0].unk0 = 0x40000000;
            return;
        }
        D_80127080[arg0].unk0 = 0x80000000;
    }
}

void func_80148B58(s32 arg0) {
    s32 index;

    index = 0x10;
    do {
        func_80143B34(index);
        index += 1;
    } while (index < 0x3F);
    index = 0;
    if (arg0 > 0) {
        do {
            func_80147928(D_8015FA44[index], index, 0x8018D000, D_8015FA50, D_8015FA68, 2);
            index += 1;
        } while (index != 0xB);
    }
    D_8015EDB4[16].unk84[2] = 0;
    D_8015EDB4[16].unk3 = 0;
    D_8015EDB4[16].unk2 = 0;
    D_8015EDB4[17].unk84[2] = 0;
    D_8015EDB4[17].unk3 = 0;
    D_8015EDB4[17].unk2 = 1;
    D_8015EDB4[18].unk84[2] = 0;
    D_8015EDB4[18].unk3 = 0;
    D_8015EDB4[18].unk2 = 0;
    D_8015EDB4[19].unk84[2] = 0;
    D_8015EDB4[19].unk3 = 0;
    D_8015EDB4[19].unk2 = 0;
    D_8015EDB4[20].unk84[2] = 0;
    D_8015EDB4[20].unk3 = 0;
    D_8015EDB4[20].unk2 = 0;
    D_8015EDB4[21].unk84[2] = 0;
    D_8015EDB4[21].unk3 = 0;
    D_8015EDB4[21].unk2 = 0;
    D_8015EDB4[22].unk84[2] = 0;
    D_8015EDB4[22].unk3 = 0;
    D_8015EDB4[22].unk2 = 0;
    D_8015EDB4[23].unk84[2] = 0;
    D_8015EDB4[24].unk84[2] = 0;
    D_8015EDB4[24].unk3 = 0;
    D_8015EDB4[24].unk2 = 0;
    D_8015EDB4[25].unk84[2] = 0;
    D_8015EDB4[25].unk3 = 0;
    D_8015EDB4[25].unk2 = 0;
}

void func_80148D6C(void) {
    u32 v;
    u8 t;

    v = func_80147B24(0xB, D_8015FA68);
    if (D_8015EDB4[20].unk2 < v) {
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
        t = D_8015EDB4[20].unk3;
        if (t >= 3U) {
            D_8015EDB4[20].unk2 = v;
            D_80122760 = -0x64;
            D_80122764 = 0x1E;
            D_80122768 = -0x1E;
            D_8012276C = 0xE0;
            D_8012276D = 0xE0;
            D_8012276E = 0xE0;
            func_8009AD70(0, &D_80122760);
            D_80122770 = 0x28;
            D_80122774 = 0x32;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &D_80122770);
            D_80122780 = 0xA;
            D_80122784 = -0x14;
            D_80122788 = -0x64;
            D_8012278C = 0x30;
            D_8012278D = 0x30;
            D_8012278E = 0x30;
            func_8009AD70(2, &D_80122780);
            D_8015EDB4[20].unk3 = 0;
        } else {
            D_8015EDB4[20].unk3 = t + 1;
        }
    }
}
