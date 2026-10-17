void func_80077900(void) {
    u8 b;
    s32 girl;
    u32 c;
    u32 club;

    if (D_800E6280.unk_1104.w == 0x20) {
        switch (((DisplayActor44 *)D_8011ECD0)[96].unk_16 & 1) {
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        { u8 b = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02; parameter_up_down(D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01, 1, b); }
        parameter_show_init();
        return;
    }
    if (D_800E6280.unk_1104.u <= 0x40) {
        return;
    }
    func_80048F64(0x60);
    func_80048F64(0x61);
    if ((D_800E6280.unk_041 == 6) && (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4)
        && ((c = D_800E6280.unk_0F4.h >> 12) != 11) && (D_800E6280.unk_66C[c].unk_00 == 6)) {
        girl = func_80053564(c);
        D_800E6280.unk_75D = girl;
        if ((girl != -1) && (D_800E6280.unk_042 < 0x2DA)) {
            club = D_800E6280.unk_0F4.h >> 12;
            if (club == 10) {
                if (girl == 0) {
                    if (D_800E6280.unk_1BC[girl].unk_0C.f.b2 && D_800E6280.unk_1BC[girl].unk_0C.f.b1
                        && !((ScheduleClubFlags *)&D_800E6280.unk_66C[club])->b29
                        && !D_800E6280.unk_1BC[girl].unk_0C.f.b18) {
                        D_800E6280.unk_1BC[girl].unk_0C.f.b18 = 1;
                        main_state_set(0x60, club);
                        return;
                    }
                }
                if (D_800E6280.unk_1BC[girl].unk_0C.f.b2 && D_800E6280.unk_1BC[girl].unk_0C.f.b1
                    && !((ScheduleClubFlags *)&D_800E6280.unk_66C[club])->b29
                    && !D_800E6280.unk_1BC[girl].unk_0C.f.b18) {
                    D_800E6280.unk_1BC[girl].unk_0C.f.b18 = 1;
                    main_state_set(0x60, club);
                    return;
                }
            } else if (D_800E6280.unk_1BC[girl].unk_0C.f.b1 && D_800E6280.unk_1BC[girl].unk_0C.f.b2
                       && !((ScheduleClubFlags *)&D_800E6280.unk_66C[club])->b29
                       && !D_800E6280.unk_1BC[girl].unk_0C.f.b18) {
                D_800E6280.unk_1BC[girl].unk_0C.f.b18 = 1;
                main_state_set(0x60, club);
                return;
            }
        }
    }
    func_80072B5C(0);
}
