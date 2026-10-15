extern u8 D_800E6354;
extern s32 D_800E67CC;
extern u8 D_800E699C;
extern s8 D_800E69A4;
extern s8 D_800E69A5;
extern s8 D_800E69A6;
extern s8 D_800E69A7;
extern s8 D_800E69DC;
extern s8 D_800E69DD;
extern s8 D_800E71DF;

void func_80076630(void) {
    ? sp2C;
    s8 temp_t0;
    u32 temp_t0_2;

    D_800E69A4 = 0xFF;
    D_800E69A5 = 0xFF;
    D_800E69A6 = 0xFF;
    D_800E69A7 = 0xFF;
    func_8006B640();
    if (func_800764AC((s32) &sp2C) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(&D_800E6354);
        sndisp((s32) "どこで、 待ち合わせだったかな？）", 0, 0x1F);
        k_sub_disp_start(1);
        func_8004284C();
        return;
    }
    temp_t0 = D_800E67CC & 0x1F;
    D_800E69A4 = temp_t0;
    D_800E69A5 = temp_t0;
    D_800E69A6 = 0xFF;
    D_800E69A7 = 0xFF;
    D_800E6280.unk_69C[D_800E699C].unk_00 = 0x80;
    D_800E6280.unk_69C[D_800E699C].unk_01 = (u8) ((u32) (D_800E67CC * 8) >> 0x1C);
    D_800E6280.unk_69C[D_800E699C].unk_02 = D_800E67CC & 0x1F;
    temp_t0_2 = (u32) (D_800E67CC * 8) >> 0x1C;
    D_800E71DF = (s8) temp_t0_2;
    D_800E69DD = (s8) temp_t0_2;
    D_800E69DC = D_800E6280.unk_54C[0].b[3] & 1;
    func_80042940(2);
}
