? func_80054FCC(?);                                 /* extern */
extern ? D_8014882C;

void func_801428EC(void) {
    s16 sp42;
    s16 sp40;
    s16 var_t0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a1_2;
    void *temp_v0;
    void *temp_v0_2;

    var_v1 = 0x1C - (((s16) *(s16 *)((unsigned char *)D_80129F40 + (6624)) / 512) * 4);
    var_t0 = 1;
    if (*(s16 *)((unsigned char *)D_80129F40 + (6624)) == 0x1000) {
        var_v1 = 0;
        var_t0 = 0;
    }
    temp_v0 = (*(&D_801483E8 + (D_80148828[D_80148818] + (D_800F53DE * 0xC))) * 8) + &D_8014882C;
    temp_a0 = (*(s16 *)((unsigned char *)D_80129F40 + (6634)) - var_t0) - 0x1C;
    temp_a1 = (*(s16 *)((unsigned char *)D_80129F40 + (6638)) + var_v1) - 0x1C;
    sp42 = var_v1;
    sp40 = var_t0;
    func_80142A8C(temp_a0, temp_a1, *(s16 *)((unsigned char *)D_80129F40 + (6634)) - 1, temp_a1, temp_a0, *(s16 *)((unsigned char *)D_80129F40 + (6638)) - 1, temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), 9, 0);
    temp_v0_2 = (*(&D_801483E8 + (D_80148828[D_80148818] + (D_800F53DE * 0xC))) * 8) + &D_8014882C;
    temp_a0_2 = *(s16 *)((unsigned char *)D_80129F40 + (6634)) + var_t0 + 0x1C;
    temp_a1_2 = (*(s16 *)((unsigned char *)D_80129F40 + (6638)) - var_v1) + 0x1C;
    func_80142A8C(temp_a0_2, temp_a1_2, temp_a0_2, (s32) *(s16 *)((unsigned char *)D_80129F40 + (6638)), (s32) *(s16 *)((unsigned char *)D_80129F40 + (6634)), temp_a1_2, temp_v0_2->unk0, *(s32 *)((unsigned char *)temp_v0_2 + (4)), 9, 0);
    func_80054FCC(0xB);
}
