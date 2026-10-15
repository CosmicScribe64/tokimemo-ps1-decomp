void func_800591D8(s32 arg0) {
    func_80059308();
    func_8009C674(0);
    func_8009C210(1);
    func_8009C5E0(0);
    if ((arg0 != 0) && (D_800E6280.unk_000 != 0x200)) {
        D_800E6294 = 0x100;
        D_800E6296 = 0x100;
        D_800E6280.unk_000 = 0x200;
        D_800E6282 = 0xF0;
        D_800E6284 = 0;
        func_80097F58(0x200, 0xF0, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    } else if ((arg0 == 0) && (D_800E6280.unk_000 != 0x140)) {
        D_800E6294 = 0xA0;
        D_800E6296 = 0xA0;
        D_800E6280.unk_000 = 0x140;
        D_800E6282 = 0xF0;
        D_800E6284 = 0;
        func_80097F58(0x140, 0xF0, 0, 0, 0);
        func_8005907C();
        back_clear_switch(1);
    }
    func_800AD950(0);
    func_8009C5E0(1);
}
