s32 func_801398E8(void) {
    s32 selection;
    u8 value;

    selection = D_80122CDC;
    value = D_800E6280.unk_F5F;
    if (value == selection) {
        value = D_800E6280.unk_110A;
        func_801386C4();
        if (value != D_800E6280.unk_110A) {
            D_800CA2DC -= 2;
            D_800E6280.unk_110A -= 3;
        }
        return 0;
    }
    value = selection;
    if (selection == 0xB) {
        D_800CA2DC = 0;
        D_800CA2D0 = D_8013C450;
        D_800CA2D4 = D_8013C484;
        D_800CA2D8 = D_8013C4B8;
        D_800CA2EC = 1;
        value = 0xD;
        D_800E6280.unk_F5F = value;
        func_800847B8(value);
        D_800E6280.unk_110A = 0x33;
        return 0;
    }
    D_800E6280.unk_F5F = value;
    func_800847B8(value);
    func_801399F4();
    func_8004284C();
}
