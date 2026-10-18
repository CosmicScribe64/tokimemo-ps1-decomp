void func_8005A410(void) {
    s32 invalid;
    s32 zeroes;
    u8 *p;
    s32 x;
    s32 y;
    s32 saved;

    invalid = 0;
    zeroes = 0;
    if ((u8)D_800E6280.unk_F6C == 2) {
        p = D_80125C10;
        do {
            x = *(s16 *)(p + 0);
            y = *(s16 *)(p + 2);
            if (x >= 125) invalid++;
            if (x < -134) invalid++;
            if (y < -84) invalid++;
            if (y >= 85) invalid++;
            if (x == 0 && y == 0) zeroes++;
            x = *(s16 *)(p + 4);
            y = *(s16 *)(p + 6);
            if (x >= 125) invalid++;
            if (x < -134) invalid++;
            if (y < -84) invalid++;
            if (y >= 85) invalid++;
            if (x == 0 && y == 0) zeroes++;
            p += 8;
        } while (p != D_80125C10 + 0x40);
    }
    if (zeroes >= 2 && invalid != 0) {
        D_800E6280.unk_F6D = 15;
        saved = (u8)D_800E6280.unk_F6C;
        D_800E6280.unk_F6C = 0;
        func_8006509C();
        icon_disp_switch(0);
        D_800E6280.unk_F6C = (u8)saved;
    }
}
