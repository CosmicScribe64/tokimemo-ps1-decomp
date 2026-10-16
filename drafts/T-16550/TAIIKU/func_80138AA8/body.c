void func_80138AA8(void) {
    s32 i;
    s32 pad1; /* FAKE: T-11640, frame slot */
    s32 pad2; /* FAKE: T-11640, frame slot */
    TaiikuW2 tpage;
    TaiikuW18 tbl;
    TaiikuW5 rgb;

    tpage = D_80149E2C;
    tbl = D_80149E34;
    rgb = D_80149E7C;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 1);
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 5] = 8;
        }
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 0x25] = 8;
        }
        D_800E6280.unk_1228[28] = 8;
        func_80048EB8(0);
        func_8006BD6C(0);
        for (i = 0; i < 2; i++) {
            D_80149DA4[i] = tpage.v[i];
        }
        for (i = 0; i < 18; i++) {
            D_80149DAC[i] = tbl.v[i];
        }
        D_80149DF4[0] = rgb.v[0];
        for (i = 1; i < 5; i++) {
            D_80149DF4[i] = rgb.v[i];
        }
        func_8004284C();
        break;
    case 1:
        cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
        Fade_Out_Init();
        func_8004284C();
        break;
    case 2:
        Fade_Out_Main();
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x27, 0x8018F000, 0xBAEE);
            func_8004284C();
        }
        break;
    case 3:
        if (Fade_Out_Main() != 0 && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80141BB0();
            for (i = 0; i < 2; i++) {
                D_800E6280.unk_1228[search_load_tpage_buf_lock(D_80149DA4[i], 1)] = i + 0x80000300;
                D_80149DA4[i] = i + 0x80000300;
            }
            for (i = 0; i < 16; i++) {
                load_palette(D_8014A0B4, (u8) (i + 0x10), 1, 1, i);
            }
            cd_read_sectors(0xB9, 0x8018F000, 0xBA17);
            func_80139AB0();
            func_80139DDC();
            func_80139E34();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
