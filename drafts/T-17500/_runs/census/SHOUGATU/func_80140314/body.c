void func_80140314(void) {
    typedef struct { s32 w[12]; } M2cBlock30; /* m2c memcpy of 48 bytes */
    typedef struct { s32 w[9]; } M2cBlock24; /* m2c memcpy of 36 bytes */
    s16 spA2;
    s16 spA0;
    ShoColors6x2 sp70;
    ShoBytes3x12 sp4C;
    s16 var_t0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a1;
    s32 temp_a1_2;
    u8 *temp_v0;
    u8 *temp_v0_2;

    sp70 = D_801461D8;
    sp4C = D_80146208;
    var_v1 = 0x1C - (((s16) D_801206B0 / 512) * 4);
    var_t0 = 1;
    if (D_801206B0 == 0x1000) {
        var_v1 = 0;
        var_t0 = 0;
    }
    temp_a0 = (D_801206BA - var_t0) - 0x1C;
    temp_v0 = sp70.v + (*(u8 *)((unsigned char *)(sp4C.v + D_80146168[D_80146158] + (D_800E6280.unk_03E * 0xC)) + (-1140)) * 8);
    temp_a1 = (D_801206BE + var_v1) - 0x1C;
    spA0 = var_t0;
    spA2 = var_v1;
    func_80140510(temp_a0, temp_a1, D_801206BA - 1, temp_a1, temp_a0, D_801206BE - 1, (*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), 9, 0);
    temp_a1_2 = (D_801206BE - var_v1) + 0x1C;
    temp_v0_2 = (*(u8 *)((unsigned char *)(sp + (D_80146168[D_80146158] + (D_800E6280.unk_03E * 0xC))) + (-1064)) * 8) + sp70.v;
    temp_a0_2 = D_801206BA + var_t0 + 0x1C;
    func_80140510(temp_a0_2, temp_a1_2, temp_a0_2, (s32) D_801206BE, (s32) D_801206BA, temp_a1_2, (*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), 9, 0);
    dtd_on(0xB);
}
