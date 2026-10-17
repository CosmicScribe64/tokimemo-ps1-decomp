s32 func_80056BA8(s32 arg0, s32 arg1, s32 arg2) {
    DISPENV disp;
    DRAWENV draw;
    CdlFILE file;
    u32 start;
    s32 limit;
    s32 frames;
    s32 i;
    s32 id;
    s16 vol;

    D_800B58F4 = arg1;
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_8009C210(1);
    if (D_8011ECA0 == 0) {
        func_8009AC20();
        func_800422C8();
    }
    D_800B58F0 = 0;
    D_80125C58 = (StrCtx *)0x80180000;
    D_80125C58->width = 0x140;
    D_80125C58->height = 0xF0;
    frames = 0;
    i = 0;
    if (arg2 == 0) {
        while (func_80088E34(&file, arg0) == 0) {
            if (i++ >= 0x10000) {
                return 0;
            }
        }
        D_80125C58->pos[0] = file.pos[0];
        D_80125C58->pos[1] = file.pos[1];
        D_80125C58->pos[2] = file.pos[2];
    } else {
        func_80087EE8(arg2, D_80125C58);
    }
    strSetDefDecEnv(&D_80125C58->dec, 0, 0, 0, 0xF0);
    strInit((s32)D_80125C58, (s32)strCallback);
    strNextVlc((u8 *)&D_80125C58->dec);
    func_8008BEE0(0, 0, 1);
    func_8008BFB0(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv(&disp, D_80125C58->dec.rect[1].x, D_80125C58->dec.rect[1].y, D_80125C58->dec.rect[1].w, D_80125C58->dec.rect[1].h);
    func_8009F57C(&draw, D_80125C58->dec.rect[1].x, D_80125C58->dec.rect[1].y, D_80125C58->dec.rect[1].w, D_80125C58->dec.rect[1].h);
    if (D_800B5900 & 1) {
        disp.isrgb24 = 1;
        disp.disp.w = disp.disp.w * 2 / 3;
    }
    func_8009CCF4(&disp);
    func_8009CC04(&draw);
    limit = arg1 - 3;
    start = D_800E6280.unk_10F4;
    for (;;) {
        func_800868BC(D_80125C58->dec.vlcbuf[D_80125C58->dec.vlcid], (D_800B5900 & 1) + 2);
        func_80086944(D_80125C58->dec.imgbuf, D_80125C58->dec.slice.w * D_80125C58->dec.slice.h / 2);
        strNextVlc((u8 *)&D_80125C58->dec);
        if (!(D_800B5900 & 1)) {
            func_800422C8();
            func_8005715C((D_800E6280.unk_10F4 - start) / 6);
            func_800476C0();
            disp_string();
            func_8009AC20();
            func_8009AD00(&D_800E8C70 + D_8011ECA0 * 5);
        }
        strSync((SyncObj *)&D_80125C58->dec, 0);
        func_800AD950(0);
        id = 1;
        if (D_80125C58->dec.rectid != 0) {
            id = 0;
        }
        SetDefDispEnv(&disp, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        func_8009F57C(&draw, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        if (D_800B5900 & 1) {
            disp.isrgb24 = 1;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        func_8009CCF4(&disp);
        func_8009CC04(&draw);
        func_8009C5E0(1);
        if (!(D_800B58F0 < limit && frames < arg1 - 0xF)) {
            break;
        }
        D_800E6280.unk_10F8 += 1;
        frames++;
        if (D_80125C58->endflag == 1) {
            break;
        }
        func_80042A00();
        if (D_800E6280.unk_F88 & 0x860) {
            break;
        }
    }
    disp.isrgb24 = 0;
    func_8009CCF4(&disp);
    func_8009C5E0(0);
    func_8004111C();
    vol = 0x7F;
    do {
        func_8008B750(vol, vol);
        func_8008BFB0(0, vol, vol);
        vol -= 10;
    } while (vol > 0);
    func_8009C5E0(1);
    func_8009C674(0);
}
