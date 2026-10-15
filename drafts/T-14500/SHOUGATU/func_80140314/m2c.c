extern u8 D_800E62BE;
extern ? D_801461D8;
extern ? D_80146208;

void func_80140314(void) {
    s16 spA2;
    s16 spA0;
    ? sp70;
    ? sp4C;
    s16 var_t0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a1_2;
    void *temp_v0;
    void *temp_v0_2;

    M2C_MEMCPY_ALIGNED(&sp70, &D_801461D8, 0x30);
    M2C_MEMCPY_ALIGNED(&sp4C, &D_80146208, 0x24);
    var_v1 = 0x1C - (((s16) D_801206B0 / 512) * 4);
    var_t0 = 1;
    if (D_801206B0 == 0x1000) {
        var_v1 = 0;
        var_t0 = 0;
    }
    temp_a0 = (D_801206BA - var_t0) - 0x1C;
    temp_v0 = &sp70 + (*(u8 *)((unsigned char *)(&sp4C + D_80146168[D_80146158] + (D_800E62BE * 0xC)) + (-1140)) * 8);
    temp_a1 = (D_801206BE + var_v1) - 0x1C;
    spA0 = var_t0;
    spA2 = var_v1;
    func_80140510(temp_a0, temp_a1, D_801206BA - 1, temp_a1, temp_a0, D_801206BE - 1, temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), 9, 0);
    temp_a1_2 = (D_801206BE - var_v1) + 0x1C;
    temp_v0_2 = (*(u8 *)((unsigned char *)(sp + (D_80146168[D_80146158] + (D_800E62BE * 0xC))) + (-1064)) * 8) + &sp70;
    temp_a0_2 = D_801206BA + var_t0 + 0x1C;
    func_80140510(temp_a0_2, temp_a1_2, temp_a0_2, (s32) D_801206BE, (s32) D_801206BA, temp_a1_2, temp_v0_2->unk0, *(s32 *)((unsigned char *)temp_v0_2 + (4)), 9, 0);
    func_8004955C(0xB);
}
