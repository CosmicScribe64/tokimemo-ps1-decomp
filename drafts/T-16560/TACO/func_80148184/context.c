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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015F848;
extern s32 D_8015F932;
void func_8014866C(void);
/* end bulk_m2c declarations */
s32 func_80148184(void) {
    s32 *var_s1;
    s32 *var_s2;
    Tc10 *var_s3;
    s32 var_s0;

    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x20;
        func_80143730(0x80162000, 0xE);
        func_80143730(0x80172000, 0xC);
        func_8014866C();
        func_80146D60(0);
        func_801471D0(0x8018D000);
        load_csr_tp(0x80162000, 8, 1, 0, 0, 0x100, 0x100);
        func_80146F74(0, D_8015EDB4->unk74 - 0xBB8, D_8015EDB4->unk76 + 0x3E8, -0x109A0, 0, 0xBE, 0, 0, 0);
        var_s3 = &D_80127080[1];
        var_s2 = &D_8015F932;
        var_s1 = &D_8015F848;
        var_s0 = 1;
        do {
            func_801472D4(var_s0, 0, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            var_s0 += 1;
            var_s1 += 2;
            var_s2 += 2;
            var_s3 += 1;
            *(s32 *)((unsigned char *)var_s3 + (-16)) = 0;
        } while (var_s0 != 0x1F);
        D_80128880->x = 0;
        D_80128880[0].y = 0xBE;
        D_80128880[0].z = 0;
        func_80148B58(0);
        func_8004284C();
    }
}

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801480B0", func_801487A4);

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
