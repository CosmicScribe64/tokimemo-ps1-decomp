void func_800590CC(s32 arg0) {
    s32 w;
    s32 h;
    s32 *pw;
    s32 *ph;

    func_8009C674(0);
    func_8009C210(1);
    func_800AD950(0);
    func_8009C5E0(0);
    w = 0x140;
    h = 0xF0;
    pw = &w;
    ph = &h;
    if (arg0 != 0) {
        D_800E6280.unk_014[0] = 0x80;
        D_800E6280.unk_014[1] = 0x80;
        w = 0x100;
        D_800E6280.unk_000 = *pw;
        h = 0xF0;
        D_800E6280.unk_002 = *ph;
        D_800E6280.unk_004 = 0;
        func_80097F58(*pw, *ph, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else {
        D_800E6280.unk_014[0] = 0xA0;
        D_800E6280.unk_014[1] = 0xA0;
        D_800E6280.unk_000 = *pw;
        D_800E6280.unk_002 = *ph;
        D_800E6280.unk_004 = 0;
        func_80097F58(*pw, *ph, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}
