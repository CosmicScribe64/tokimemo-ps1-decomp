s32 func_801392B4(void) {
    s32 previous_phase;
    s32 next_state;

    if (D_80122CDC == 0) {
        if (D_800E6280.unk_F5F == 8) {
            previous_phase = D_800E6280.unk_110A;
            D_800CA2DC = 6;
            func_801386C4();
            if (previous_phase == D_800E6280.unk_110A) {
                return 0;
            }
            D_800E6280.unk_110A--;
        }
        next_state = D_800CA2CC + 0x1A;
        D_8013C970 = next_state;
        D_800CA2DC = next_state;
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else {
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 -= 1;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 -= 2;
        check_para_limit();
    }
    func_8004284C();
}
