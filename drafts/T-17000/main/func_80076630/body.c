void func_80076630(void) {
    s32 sp2C[1];
    u32 temp_t0_2;
    u8 temp_t0;

    D_800E6280.unk_724 = 0xFF;
    D_800E6280.unk_725 = 0xFF;
    D_800E6280.unk_726 = 0xFF;
    D_800E6280.unk_727 = 0xFF;
    func_8006B640();
    if (func_800764AC((s32) &sp2C) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(D_800E6280.unk_0D4);
        sndisp((s32) "どこで、 待ち合わせだったかな？）", 0, 0x1F);
        k_sub_disp_start(1);
        func_8004284C();
        return;
    }
    temp_t0 = D_800E6280.unk_54C[0].w & 0x1F;
    D_800E6280.unk_724 = temp_t0;
    D_800E6280.unk_725 = temp_t0;
    D_800E6280.unk_726 = 0xFF;
    D_800E6280.unk_727 = 0xFF;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 = 0x80;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 = (u8) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C);
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = D_800E6280.unk_54C[0].w & 0x1F;
    temp_t0_2 = (u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C;
    D_800E6280.unk_F5F = (u8) temp_t0_2;
    D_800E6280.unk_75D = (u8) temp_t0_2;
    D_800E6280.unk_75C = D_800E6280.unk_54C[0].b[3] & 1;
    func_80042940(2);
}
