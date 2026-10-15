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
extern s16 D_800E643E;
extern s32 D_800E62B8;
extern s32 D_800E6558;
extern s32 D_8016052C;
extern s32 D_80160534;
extern s32 D_80160584;
extern s32 D_801605D4;
extern u16 D_800E6374;
extern u8 D_800E62BE;
/* end bulk_m2c declarations */
void func_80142BE8(void) {
    typedef struct { s32 w[19]; } M2cBlock4C; /* m2c memcpy of 76 bytes */
    s32 sp114[2];
    s32 spCA[1];
    s32 spC4[19];
    s32 sp7A[1];
    s32 sp74[19];
    s32 sp2A[1];
    s32 sp24[19];
    s32 *var_a0;
    s32 *var_a0_2;
    s32 *var_a1;
    s32 *var_t2;
    s32 *var_t2_2;
    s32 *var_t2_3;
    s16 temp_t9_2;
    s16 temp_t9_3;
    s16 temp_t9_4;
    s32 temp_t9;
    u32 temp_t4;
    u32 temp_t7;
    u32 var_v0;
    u8 var_v0_2;

    (*(s32 *)&sp114) = (s32) D_8016052C;
    *(u16 *)((unsigned char *)(&sp114) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_8016052C) + (4));
    *(M2cBlock4C *)(&spC4) = *(M2cBlock4C *)(&D_80160534);
    *(u16 *)((unsigned char *)(&spC4 + 0x48) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80160534 + 0x48) + (4));
    *(M2cBlock4C *)(&sp74) = *(M2cBlock4C *)(&D_80160584);
    *(u16 *)((unsigned char *)(&sp74 + 0x48) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80160584 + 0x48) + (4));
    *(M2cBlock4C *)(&sp24) = *(M2cBlock4C *)(&D_801605D4);
    *(u16 *)((unsigned char *)(&sp24 + 0x48) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_801605D4 + 0x48) + (4));
    temp_t4 = (u16) D_800E6374 >> 0xC;
    var_t2 = &D_800E62B8;
    temp_t7 = (((s16) *(s16 *)((unsigned char *)(&D_800E6280 + (*(&sp114 + temp_t4) * 4)) + (254)) / 20) + ((u32) ((u16) D_800E6280.unk_66C[temp_t4].unk_00 >> 3) / (u32) (D_800E62BE - 0x5E))) & 0xFF;
    var_v0 = temp_t7;
    if (temp_t7 >= 0xBU) {
        var_v0 = 0xA;
    }
    temp_t9 = (D_800E62BE - 0x5F) & 0xFF;
    switch (temp_t9) {
    default:
        var_v0_2 = sp11C;
        break;
    case 0:
        D_800E643E = D_800E643E + var_v0 + (((u8) *(&spC4 + temp_t4) >> 1) & 0xFF);
        var_a1 = &spCA;
        do {
            *(s16 *)((unsigned char *)var_t2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (446)) + (((u8) *(var_a1 + ((u16) D_800E6374 >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6374 >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6374 >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (614)) + var_v0);
            temp_t9_2 = *(s16 *)((unsigned char *)var_t2 + (614));
            var_t2 += 0xE0;
            *(s16 *)((unsigned char *)var_t2 + (390)) = (s16) (temp_t9_2 + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6374 >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a1 += 0x18;
        } while (var_t2 != &D_800E6558);
        var_v0_2 = *(&spC4 + (D_80122EBC * 6) + ((u16) D_800E6374 >> 0xC));
        break;
    case 1:
        D_800E643E = D_800E643E + var_v0 + (((u8) *(&sp74 + temp_t4) >> 1) & 0xFF);
        var_t2_2 = &D_800E62B8;
        var_a0 = &sp7A;
        do {
            *(s16 *)((unsigned char *)var_t2_2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (446)) + (((u8) *(var_a0 + ((u16) D_800E6374 >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6374 >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6374 >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (614)) + var_v0);
            temp_t9_3 = *(s16 *)((unsigned char *)var_t2_2 + (614));
            var_t2_2 += 0xE0;
            *(s16 *)((unsigned char *)var_t2_2 + (390)) = (s16) (temp_t9_3 + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6374 >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a0 += 0x18;
        } while (var_t2_2 != &D_800E6558);
        var_v0_2 = *(&sp74 + (D_80122EBC * 6) + ((u16) D_800E6374 >> 0xC));
        break;
    case 2:
        D_800E643E = D_800E643E + var_v0 + (((u8) *(&sp24 + temp_t4) >> 1) & 0xFF);
        var_t2_3 = &D_800E62B8;
        var_a0_2 = &sp2A;
        do {
            *(s16 *)((unsigned char *)var_t2_3 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (446)) + (((u8) *(var_a0_2 + ((u16) D_800E6374 >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6374 >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6374 >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (614)) + var_v0);
            temp_t9_4 = *(s16 *)((unsigned char *)var_t2_3 + (614));
            var_t2_3 += 0xE0;
            *(s16 *)((unsigned char *)var_t2_3 + (390)) = (s16) (temp_t9_4 + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6374 >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a0_2 += 0x18;
        } while (var_t2_3 != &D_800E6558);
        var_v0_2 = *(&sp24 + (D_80122EBC * 6) + ((u16) D_800E6374 >> 0xC));
        break;
    }
    if (var_v0_2 != 0) {
        var_v0_2 = (var_v0_2 - 1) & 0xFF;
    }
    D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
    D_800E6280.unk_1BC[D_80122EBC].unk_02 += var_v0_2;
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_801431D4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/80142700.rodata", D_8015D774);

void func_8014338C(void) {
    k_reset(0);
    func_8006612C(&D_8015D774);
    k_disp_start(1);
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_801433C0);
