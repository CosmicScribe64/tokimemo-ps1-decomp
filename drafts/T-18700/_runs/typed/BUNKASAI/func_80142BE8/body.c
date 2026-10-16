void func_80142BE8(void) {
    BunkasaiColumns sp114;
    s32 spCA[1];
    BunkasaiGirlColumns spC4;
    s32 sp7A[1];
    BunkasaiGirlColumns sp74;
    s32 sp2A[1];
    BunkasaiGirlColumns sp24;
    s32 *var_a0;
    s32 *var_a0_2;
    s32 *var_a1;
    s16 temp_t9_2;
    s16 temp_t9_3;
    s16 temp_t9_4;
    s32 temp_t9;
    u32 temp_t4;
    u32 temp_t7;
    u32 var_v0;
    u8 *var_t2;
    u8 *var_t2_2;
    u8 *var_t2_3;
    u8 var_v0_2;

    sp114 = D_8016052C;
    spC4 = D_80160534;
    sp74 = D_80160584;
    sp24 = D_801605D4;
    temp_t4 = (u16) D_800E6280.unk_0F4.h >> 0xC;
    var_t2 = &D_800E6280.unk_038;
    temp_t7 = (((s16) *(s16 *)((unsigned char *)(&D_800E6280 + (*(sp114.v + temp_t4) * 4)) + (254)) / 20) + ((u32) ((u16) D_800E6280.unk_66C[temp_t4].unk_00 >> 3) / (u32) (D_800E6280.unk_03E - 0x5E))) & 0xFF;
    var_v0 = temp_t7;
    if (temp_t7 >= 0xBU) {
        var_v0 = 0xA;
    }
    temp_t9 = (D_800E6280.unk_03E - 0x5F) & 0xFF;
    switch (temp_t9) {
    default:
        var_v0_2 = sp11C;
        break;
    case 0:
        D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + var_v0 + (((u8) *(spC4.v + temp_t4) >> 1) & 0xFF);
        var_a1 = &spCA;
        do {
            *(s16 *)((unsigned char *)var_t2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (446)) + (((u8) *(var_a1 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2 + (614)) + var_v0);
            temp_t9_2 = *(s16 *)((unsigned char *)var_t2 + (614));
            var_t2 += 0xE0;
            *(s16 *)((unsigned char *)var_t2 + (390)) = (s16) (temp_t9_2 + (((u8) *(u8 *)((unsigned char *)(var_a1 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a1 += 6;
        } while (var_t2 != &D_800E6280.unk_1BC[5].unk_04);
        var_v0_2 = *(spC4.v + (D_80122EBC * 6) + ((u16) D_800E6280.unk_0F4.h >> 0xC));
        break;
    case 1:
        D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + var_v0 + (((u8) *(sp74.v + temp_t4) >> 1) & 0xFF);
        var_t2_2 = &D_800E6280.unk_038;
        var_a0 = &sp7A;
        do {
            *(s16 *)((unsigned char *)var_t2_2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (446)) + (((u8) *(var_a0 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_2 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_2 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2_2 + (614)) + var_v0);
            temp_t9_3 = *(s16 *)((unsigned char *)var_t2_2 + (614));
            var_t2_2 += 0xE0;
            *(s16 *)((unsigned char *)var_t2_2 + (390)) = (s16) (temp_t9_3 + (((u8) *(u8 *)((unsigned char *)(var_a0 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a0 += 6;
        } while (var_t2_2 != &D_800E6280.unk_1BC[5].unk_04);
        var_v0_2 = *(sp74.v + (D_80122EBC * 6) + ((u16) D_800E6280.unk_0F4.h >> 0xC));
        break;
    case 2:
        D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + var_v0 + (((u8) *(sp24.v + temp_t4) >> 1) & 0xFF);
        var_t2_3 = &D_800E6280.unk_038;
        var_a0_2 = &sp2A;
        do {
            *(s16 *)((unsigned char *)var_t2_3 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (446)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (446)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (446)) + (((u8) *(var_a0_2 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (502)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (502)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (502)) + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (6)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (558)) + var_v0);
            *(s16 *)((unsigned char *)var_t2_3 + (558)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (558)) + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (12)) >> 1) & 0xFF));
            *(s16 *)((unsigned char *)var_t2_3 + (614)) = (s16) (*(s16 *)((unsigned char *)var_t2_3 + (614)) + var_v0);
            temp_t9_4 = *(s16 *)((unsigned char *)var_t2_3 + (614));
            var_t2_3 += 0xE0;
            *(s16 *)((unsigned char *)var_t2_3 + (390)) = (s16) (temp_t9_4 + (((u8) *(u8 *)((unsigned char *)(var_a0_2 + ((u16) D_800E6280.unk_0F4.h >> 0xC)) + (18)) >> 1) & 0xFF));
            var_a0_2 += 6;
        } while (var_t2_3 != &D_800E6280.unk_1BC[5].unk_04);
        var_v0_2 = *(sp24.v + (D_80122EBC * 6) + ((u16) D_800E6280.unk_0F4.h >> 0xC));
        break;
    }
    if (var_v0_2 != 0) {
        var_v0_2 = (var_v0_2 - 1) & 0xFF;
    }
    D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
    D_800E6280.unk_1BC[D_80122EBC].unk_02 += var_v0_2;
}
