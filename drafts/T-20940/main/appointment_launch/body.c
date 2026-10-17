void appointment_launch(void) {
    s32 i;
    s32 n;
    u32 g;
    u32 tmp[3];

    tmp[0] = D_800E6280.unk_725;
    tmp[1] = D_800E6280.unk_726;
    tmp[2] = D_800E6280.unk_727;
    D_800E6280.unk_725 = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_00;
    n = 2;
    for (i = 0; i < 3; i++) {
        if (tmp[i] != (D_800E6280.unk_54C[0].w & 0x1F)) {
            ((GsWord *)&D_800E6280.unk_724)->b[n] = tmp[i];
            n++;
        }
    }
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_00 = 0x80;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_25;
    D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02 = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_00;
    g = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->unk_25;
    D_800E6280.unk_F5F = g;
    D_800E6280.unk_75D = g;
    D_800E6280.unk_75C = ((ScheduleEntryFlags *)&D_800E6280.unk_54C[0])->b24;
    main_state_set(0x50);
}
