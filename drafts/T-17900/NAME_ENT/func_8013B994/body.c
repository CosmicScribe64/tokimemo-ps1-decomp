void func_8013B994(void) {
    s32 err;
    s32 zero;
    s32 i;
    s32 x;
    s32 y;
    u8 saved;

    err = 0;
    zero = 0;
    if (*(u8 *)&D_800E6280.unk_F6C == 2) {
        for (i = 0; i < 8; i++) {
            x = ((s16 *)D_80125C10)[i * 4];
            if (x >= 0x7D) {
                err++;
            }
            if (x < -0x86) {
                err++;
            }
            y = ((s16 *)D_80125C10)[i * 4 + 1];
            if (y < -0x54) {
                err++;
            }
            if (y >= 0x55) {
                err++;
            }
            if (x == 0 && y == 0) {
                zero++;
            }
            x = ((s16 *)D_80125C10)[i * 4 + 2];
            if (x >= 0x7D) {
                err++;
            }
            if (x < -0x86) {
                err++;
            }
            y = ((s16 *)D_80125C10)[i * 4 + 3];
            if (y < -0x54) {
                err++;
            }
            if (y >= 0x55) {
                err++;
            }
            if (x == 0 && y == 0) {
                zero++;
            }
        }
    }
    func_800AE0B0("err %d \n", err);
    func_800AE0B0("zero %d \n", zero);
    if (zero >= 2) {
        if (err != 0) {
            saved = *(u8 *)&D_800E6280.unk_F6C;
            *(u8 *)&D_800E6280.unk_F6C = 0;
            func_8006509C();
            func_80064FA4(0);
            *(u8 *)&D_800E6280.unk_F6C = saved;
        }
    }
}
