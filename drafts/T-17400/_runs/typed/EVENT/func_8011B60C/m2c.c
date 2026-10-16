void func_8011B60C(void) {
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s16 var_v0_4;
    s16 var_v0_5;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_t2;
    s32 temp_t5;
    u8 *temp_v1_3;
    u8 *temp_v1_4;
    u8 temp_t1;
    void *temp_v1;
    void *temp_v1_2;

    temp_t1 = D_80125864 - 4;
    D_800EC190->unk_15 = temp_t1;
    D_800EC190->unk_14 = temp_t1;
    *(u8 *)((unsigned char *)D_800EC190 + (57)) = temp_t1;
    *(u8 *)((unsigned char *)D_800EC190 + (56)) = temp_t1;
    D_80125864 = temp_t1;
    D_800EC190->unk_16 = temp_t1;
    *(u8 *)((unsigned char *)D_800EC190 + (58)) = temp_t1;
    *(u8 *)((unsigned char *)D_800E9620 + (2115)) = temp_t1;
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 0x44;
        var_v0 += 1;
        *(u8 *)((unsigned char *)(D_800E9620 + temp_lo) + (6535)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (2115));
    } while (var_v0 < 4);
    var_v0_2 = 0;
    do {
        temp_lo_2 = var_v0_2 * 0x24;
        var_v0_2 += 1;
        temp_v1 = D_800EC190 + temp_lo_2;
        *(u8 *)((unsigned char *)temp_v1 + (166)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (2115));
        *(u8 *)((unsigned char *)temp_v1 + (165)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (2115));
        *(u8 *)((unsigned char *)temp_v1 + (164)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (2115));
    } while (var_v0_2 < 6);
    *(u8 *)((unsigned char *)D_800E9620 + (2115)) = (u8) *(u8 *)((unsigned char *)D_800E9620 + (2115));
    if (*(u8 *)((unsigned char *)D_800E9620 + (2115)) == 0) {
        D_800EECF0 = 0;
        D_800EECD0 = 0;
        *(u8 *)((unsigned char *)D_800E9620 + (2111)) = (u8) (*(u8 *)((unsigned char *)D_800E9620 + (2111)) & 0xFF7F);
        D_800EC190->unk_15 = 0x80;
        D_800EC190->unk_14 = 0x80;
        *(u8 *)((unsigned char *)D_800EC190 + (57)) = 0x80U;
        *(u8 *)((unsigned char *)D_800EC190 + (56)) = 0x80U;
        D_800EC190->unk_00 |= 0x80000000;
        *(s32 *)((unsigned char *)D_800EC190 + (36)) = (s32) (*(s32 *)((unsigned char *)D_800EC190 + (36)) | 0x80000000);
        D_800EC190->unk_16 = 0x80;
        *(u8 *)((unsigned char *)D_800EC190 + (58)) = 0x80U;
        var_v0_3 = 0;
        do {
            temp_lo_3 = var_v0_3 * 0x24;
            var_v0_3 += 1;
            temp_v1_2 = D_800EC190 + temp_lo_3;
            *(s32 *)((unsigned char *)temp_v1_2 + (144)) = (s32) (*(s32 *)((unsigned char *)temp_v1_2 + (144)) | 0x80000000);
        } while (var_v0_3 < 6);
        func_80012D64(D_80094710, 0x10, 1, 2, 0);
        var_v0_4 = 0xB;
        do {
            temp_t2 = var_v0_4 * 4;
            var_v0_4 += 1;
            temp_v1_3 = &D_800B0860[temp_t2];
            *(s32 *)((unsigned char *)temp_v1_3 + (5032)) = 0x77;
            *(s32 *)((unsigned char *)temp_v1_3 + (5160)) = 0x77;
        } while (var_v0_4 < 0x10);
        var_v0_5 = 0x15;
        do {
            temp_t5 = var_v0_5 * 4;
            var_v0_5 += 1;
            temp_v1_4 = &D_800B0860[temp_t5];
            *(s32 *)((unsigned char *)temp_v1_4 + (5032)) = 0x77;
            *(s32 *)((unsigned char *)temp_v1_4 + (5160)) = 0x77;
        } while (var_v0_5 < 0x1A);
        func_80011DFC(0x77, D_800B0860);
    }
}
