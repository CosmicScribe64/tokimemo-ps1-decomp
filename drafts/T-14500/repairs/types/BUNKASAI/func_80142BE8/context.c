#include "common.h"
#include "ovl/BUNKASAI.h"

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_80142700);

void func_80142B98(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80142BC8(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

/* bulk_m2c declarations (T-11190) */
typedef struct RepairBytes78 { /* 0x00 */ u8 b[78]; } RepairBytes78; /* size 0x4E */
typedef struct RepairBytes6 { /* 0x00 */ u8 b[6]; } RepairBytes6; /* size 0x6 */
extern s16 D_800E643E;
extern s32 D_800E62B8;
extern s32 D_800E6558;
extern RepairBytes6 D_8016052C;
extern RepairBytes78 D_80160534;
extern RepairBytes78 D_80160584;
extern RepairBytes78 D_801605D4;
extern u16 D_800E6374;
extern u8 D_800E62BE;
/* end bulk_m2c declarations */
/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_80142BE8(void) {
    RepairBytes6 selector;
    RepairBytes78 first;
    RepairBytes78 second;
    RepairBytes78 third;
    u8 *values;
    u8 fallback; /* Original default arm reads uninitialized sp+0x11C. */
    u8 amount;
    u32 column;
    u32 bonus;
    s32 year;
    s32 i;

    selector = D_8016052C;
    first = D_80160534;
    second = D_80160584;
    third = D_801605D4;
    column = (u16) D_800E6374 >> 12;
    bonus = (((GsRec0FC *)&D_800E6280.unk_0FC)[selector.b[column]].unk_02 / 20 +
        (u32) (D_800E6280.unk_66C[column].unk_00 >> 3) / (u32) (D_800E62BE - 0x5E)) & 0xFF;
    if (bonus >= 11) {
        bonus = 10;
    }
    year = (D_800E62BE - 0x5F) & 0xFF;
    values = 0;
    switch (year) {
    case 0: values = first.b; break;
    case 1: values = second.b; break;
    case 2: values = third.b; break;
    }
    if (values == 0) {
        amount = fallback;
    } else {
        D_800E6280.unk_1BC[0].unk_02 += bonus + (values[column] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += values[i * 6 + ((u16) D_800E6374 >> 12)] >> 1;
        }
        amount = values[D_80122EBC * 6 + ((u16) D_800E6374 >> 12)];
    }
    if (amount != 0) {
        amount = (amount - 1) & 0xFF;
    }
    D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
    D_800E6280.unk_1BC[D_80122EBC].unk_02 += amount;
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_801431D4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/80142700.rodata", D_8015D774);

void func_8014338C(void) {
    k_reset(0);
    func_8006612C(&D_8015D774);
    k_disp_start(1);
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_801433C0);
