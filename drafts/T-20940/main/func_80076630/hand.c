void func_80076630(void) {
    u32 g;
    u32 list[32];

    D_800E6280.unk_724 = 0xFF;
    D_800E6280.unk_725 = 0xFF;
    D_800E6280.unk_726 = 0xFF;
    D_800E6280.unk_727 = 0xFF;
    func_8006B640();
    if (func_800764AC(list) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(D_800E6280.unk_0D4);
        sndisp("どこで、 待ち合わせだったかな？）", 0, 0x1F);
        k_sub_disp_start(1);
        func_8004284C();
        return;
    }
    g = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_00;
    D_800E6280.unk_724 = g;
    D_800E6280.unk_725 = g;
    D_800E6280.unk_726 = 0xFF;
    D_800E6280.unk_727 = 0xFF;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 = 0x80;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_25;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_00;
    g = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_25;
    D_800E6280.unk_F5F = g;
    D_800E6280.unk_75D = g;
    D_800E6280.unk_75C = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->b24;
    func_80042940(2);
}
