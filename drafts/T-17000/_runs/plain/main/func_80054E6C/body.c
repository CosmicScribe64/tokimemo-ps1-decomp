void func_80054E6C(void) {
    void *sp34;
    void *sp30;
    GameState *var_v0;
    GameState *var_v0_2;
    s32 *var_v1;
    s32 temp_t6;
    s32 temp_t6_2;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2;
    u8 temp_t9;
    u8 *temp_a3;
    u8 *temp_v0;
    u8 *var_v1_2;

    temp_t6 = D_800E7D36 & 0xF;
    temp_a3 = &D_800E7D10 + temp_t6;
    *(u8 *)((unsigned char *)temp_a3 + (2136)) = (u8) D_800E62BE;
    *(u8 *)((unsigned char *)temp_a3 + (2152)) = (u8) D_800E62BF;
    temp_v0 = &D_800E7D10 + (temp_t6 * 8);
    sp30 = temp_v0;
    sp34 = temp_a3;
    *(u8 *)((unsigned char *)temp_a3 + (2168)) = (u8) D_800E62C0;
    memcpy((s32) (temp_v0 + 0x888), (s32) &D_800E6354, 8);
    memcpy((s32) (temp_v0 + 0x908), (s32) &D_800E635C, 8);
    memcpy((s32) ((temp_t6 * 0x10) + 0x988 + &D_800E7D10), (s32) &D_800E6364, 0x10);
    var_v0 = &D_800E6280;
    var_v1 = &(&D_800E7D10)[temp_t6];
    do {
        temp_t6_2 = (s32) var_v0->unk_0FC;
        var_v0 += 4;
        var_v1 += 0x40;
        *(s32 *)((unsigned char *)var_v1 + (2632)) = temp_t6_2;
    } while ((u32) var_v0 < (u32) &D_800E62A4);
    var_v0_2 = &D_800E6280;
    var_v1_2 = (temp_t6 * 0x10) + &D_800E7D10;
    do {
        temp_t0 = var_v0_2->unk_1BC[0xF].unk_14[1];
        temp_t1 = var_v0_2->unk_1BC[0xF].unk_14[2];
        temp_t2 = var_v0_2->unk_1BC[0xF].unk_14[3];
        temp_t9 = var_v0_2->unk_1BC[0xF].unk_14[0];
        var_v0_2 += 4;
        var_v1_2 += 4;
        *(u8 *)((unsigned char *)var_v1_2 + (3269)) = temp_t0;
        *(u8 *)((unsigned char *)var_v1_2 + (3270)) = temp_t1;
        *(u8 *)((unsigned char *)var_v1_2 + (3271)) = temp_t2;
        *(u8 *)((unsigned char *)var_v1_2 + (3268)) = temp_t9;
    } while (var_v0_2 != &D_800E6290);
    *(s8 *)((unsigned char *)temp_a3 + (3784)) = (s8) ((u16) D_800E6374 >> 0xC);
}
