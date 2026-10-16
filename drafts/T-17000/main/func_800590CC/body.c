void func_800590CC(s32 arg0) {
    func_8009C674(0);
    func_8009C210(1);
    func_800AD950(0);
    func_8009C5E0(0);
    if (arg0 != 0) {
        D_800E6280.unk_014[0] = 0x80;
        D_800E6280.unk_014[1] = 0x80;
        D_800E6280.unk_000 = 0x100;
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_004 = 0;
        func_80097F58(0x100, 0xF0, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else {
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_000 = 0x140;
        D_800E6280.unk_002 = 0xF0;
        D_800E6280.unk_004 = 0;
        func_80097F58(0x140, 0xF0, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}
