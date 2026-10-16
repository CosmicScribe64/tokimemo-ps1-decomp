void func_80138C1C(void) {
    s32 sp34[1];
    GameState *var_v0;
    GameState *var_v0_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a1_3;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 temp_a0;
    u8 temp_a0_2;
    u8 temp_a0_3;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *temp_v0_3;

    var_v0 = &D_800E6280;
    do {
        var_v0 += 1;
        var_v0->unk_0C4[0xF] = 0;
        var_v0->unk_0D4[7] = 0;
    } while ((u32) var_v0 < (u32) &D_800E6280.unk_006[2]);
    var_v0_2 = &D_800E6280;
    do {
        var_v0_2 += 1;
        var_v0_2->unk_044[1].unk_20[0x1F] = 0;
        var_v0_2->unk_0DC[7] = 0;
    } while ((u32) var_v0_2 < (u32) &D_800E6280.unk_006[0xA]);
    var_v1 = D_8014CC74;
    var_a1 = 0;
    do {
        temp_a0 = (*(s32 *)var_v1);
        if (((temp_a0 != *D_800B3644) || (var_v1[1] != D_800B3645)) && ((temp_a0 != 0) || (var_v1[1] != 0))) {
            temp_v0 = &D_800E6280 + (var_a1 * 2);
            *(u8 *)((unsigned char *)temp_v0 + (212)) = temp_a0;
            var_a1 += 1;
            *(u8 *)((unsigned char *)temp_v0 + (213)) = (u8) var_v1[1];
        }
        var_v1 += 2;
    } while ((u32) var_v1 < (u32) &D_8014CC74[6]);
    var_a1_2 = 0;
    var_v1_2 = D_8014CC7C;
    do {
        temp_a0_2 = (*(s32 *)var_v1_2);
        if (((temp_a0_2 != *D_800B3644) || (var_v1_2[1] != D_800B3645)) && ((temp_a0_2 != 0) || (var_v1_2[1] != 0))) {
            temp_v0_2 = &D_800E6280 + (var_a1_2 * 2);
            *(u8 *)((unsigned char *)temp_v0_2 + (220)) = temp_a0_2;
            var_a1_2 += 1;
            *(u8 *)((unsigned char *)temp_v0_2 + (221)) = (u8) var_v1_2[1];
        }
        var_v1_2 += 2;
    } while ((u32) var_v1_2 < (u32) &D_8014CC7C[6]);
    var_a1_3 = 0;
    var_v1_3 = D_8014CC84;
    do {
        temp_a0_3 = (*(s32 *)var_v1_3);
        if (((temp_a0_3 != *D_800B3644) || (*(u8 *)((unsigned char *)var_v1_3 + (1)) != D_800B3645)) && ((temp_a0_3 != 0) || (*(u8 *)((unsigned char *)var_v1_3 + (1)) != 0))) {
            temp_v0_3 = &D_800E6280 + (var_a1_3 * 2);
            *(u8 *)((unsigned char *)temp_v0_3 + (228)) = temp_a0_3;
            var_a1_3 += 1;
            *(u8 *)((unsigned char *)temp_v0_3 + (229)) = (u8) *(u8 *)((unsigned char *)var_v1_3 + (1));
        }
        var_v1_3 += 2;
    } while (var_v1_3 != &D_8014CC8E);
    func_800AE090(&sp34, D_800E6280.unk_0D4, 8);
    func_800AE110(&sp34, D_800E6280.unk_0DC, 8);
    func_800AE090(D_800E6280.unk_0C4, &sp34, 0x10);
    D_800E6280.unk_0F8 = (u8) (((D_8014CC90 + 1) & 0xF) | (D_800E6280.unk_0F8 & 0xFFF0));
    *(u16 *)((unsigned char *)(&D_800E6280) + (248)) = (u16) ((((D_8014CC94 + 1) * 0x10) & 0x1F0) | (*(u16 *)((unsigned char *)(&D_800E6280) + (248)) & 0xFE0F));
    *(u8 *)((unsigned char *)(&D_800E6280) + (249)) = (u8) ((((D_8014CC98 + 1) * 2) & 6) | (*(u8 *)((unsigned char *)(&D_800E6280) + (249)) & 0xFFF9));
    D_800E6280.unk_1BC[0].unk_10.b[0] = ((D_8014CC9C + 1) & 0xF) | (D_800E6280.unk_1BC[0].unk_10.b[0] & 0xFFF0);
    D_800E6280.unk_1BC[0].unk_10.h[0] = (((D_8014CCA0 + 1) * 0x10) & 0x1F0) | (D_800E6280.unk_1BC[0].unk_10.h[0] & 0xFE0F);
    D_800E6280.unk_1BC[0].unk_10.b[1] = (((D_8014CCA4 + 1) * 2) & 6) | (D_800E6280.unk_1BC[0].unk_10.b[1] & 0xFFF9);
}
