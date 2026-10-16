extern s32 D_800E67CC;
extern u8 D_800E699C;
extern u8 D_800E69A5;
extern u8 D_800E69A6;
extern u8 D_800E69A7;
extern s8 D_800E69DC;
extern s8 D_800E69DD;
extern s8 D_800E71DF;

void appointment_launch(void) {
    ? sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    s32 *var_v0;
    s32 temp_a0;
    s32 var_v1;
    u32 temp_t1;
    u8 temp_t6;

    temp_t6 = D_800E69A5;
    D_800E69A5 = D_800E67CC & 0x1F;
    var_v0 = &sp30;
    var_v1 = 2;
    sp30 = (s32) temp_t6;
    sp34 = (s32) D_800E69A6;
    sp38 = (s32) D_800E69A7;
    do {
        temp_a0 = *var_v0;
        var_v0 += 4;
        if (temp_a0 != (D_800E6280.unk_54C[0].w & 0x1F)) {
            *(s8 *)((unsigned char *)(&D_800E6280 + var_v1) + (1828)) = (s8) temp_a0;
            var_v1 += 1;
        }
    } while (var_v0 != &sp3C);
    D_800E6280.unk_69C[D_800E699C].unk_00 = 0x80;
    D_800E6280.unk_69C[D_800E699C].unk_01 = (u8) ((u32) (D_800E67CC * 8) >> 0x1C);
    D_800E6280.unk_69C[D_800E699C].unk_02 = D_800E67CC & 0x1F;
    temp_t1 = (u32) (D_800E67CC * 8) >> 0x1C;
    D_800E71DF = (s8) temp_t1;
    D_800E69DD = (s8) temp_t1;
    D_800E69DC = D_800E6280.unk_54C[0].b[3] & 1;
    main_state_set(0x50);
}
