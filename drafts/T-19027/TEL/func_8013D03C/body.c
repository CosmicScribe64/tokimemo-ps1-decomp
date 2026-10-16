void func_8013D03C(s32 arg0, s32 arg1) {
    /* FAKE: unused 32-byte local reproduces the frame (slots above n); real use unknown. T-19027 */
    u8 pad[32];
    s32 n;
    u8 buf[32];
    s32 i;
    s32 j;
    u32 day;
    s32 v;
    u8 *notes;
    TelSignDays days;
    Rec38 *p;

    if (arg0 == 0xB) {
        n = 0xC;
    } else {
        n = arg0;
    }
    p = &D_800E6280.unk_1BC[n];
    p->unk_0C.f.b3 = 1;
    p->unk_0C.f.b6 = 1;
    p->unk_0C.f.b2 = 1;
    p->unk_0C.f.b7 = 1;
    D_800E6280.unk_1A3C = 0x801B01B0;
    D_800E6280.unk_1A4C = 0x801B01C0;
    D_800E6280.unk_1A5C = 0;
    D_800E6280.unk_1A6C = 0x801B0200;
    for (i = 0; i < 0x10; i++) {
        load_palette(D_800E6280.unk_1A6C, (i + 0x10) & 0xFF, 1, 2, i);
    }
    func_80048E78();
    for (i = 0; i < 8; i++) {
        func_80048F64(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).unk_02 = 0x40;
        REC(i + 0x60).unk_38 = 0x40000000;
        REC(i + 0x60).unk_03 = 0x84;
        REC(i + 0x60).unk_0C = sprite_frame_table;
        REC(i + 0x60).unk_10 = D_800C9A14;
        REC(i + 0x60).unk_34 = D_800C9730;
        REC(i + 0x60).unk_14 = D_800C9A54;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 1;
    }
    REC(0x60).unk_18 = D_800B3778[n * 7] + 0x4E;
    REC(0x60).unk_26 = 0x27;
    REC(0x60).unk_2A = -0x5A;
    REC(0x61).unk_18 = D_800B3778[n * 7 + 1] + 0x4E;
    REC(0x61).unk_26 = 0x31;
    REC(0x61).unk_2A = -0x5A;
    REC(0x62).unk_18 = D_800B3778[n * 7 + 2] + 0x4E;
    REC(0x62).unk_26 = 0x3B;
    REC(0x62).unk_2A = -0x5A;
    REC(0x63).unk_18 = D_800B3778[n * 7 + 3] + 0x4E;
    REC(0x63).unk_26 = 0x4C;
    REC(0x63).unk_2A = -0x5A;
    REC(0x64).unk_18 = D_800B3778[n * 7 + 4] + 0x4E;
    REC(0x64).unk_26 = 0x56;
    REC(0x64).unk_2A = -0x5A;
    REC(0x65).unk_18 = D_800B3778[n * 7 + 5] + 0x4E;
    REC(0x65).unk_26 = 0x60;
    REC(0x65).unk_2A = -0x5A;
    REC(0x66).unk_18 = D_800B3778[n * 7 + 6] + 0x4E;
    REC(0x66).unk_26 = 0x6A;
    REC(0x66).unk_2A = -0x5A;
    REC(0x67).unk_18 = 0x41;
    REC(0x67).unk_26 = -0x10;
    REC(0x67).unk_2A = -0x1B;
    for (i = 8; i != 9; i++) {
        func_80048F64(i + 0x60);
        REC(i + 0x60).unk_01 = 4;
        REC(i + 0x60).unk_02 = 0x40;
        REC(i + 0x60).unk_38 = 0x01000000;
        REC(i + 0x60).unk_03 = 0x84;
        REC(i + 0x60).unk_0C = (u8 *)D_800E6280.unk_1A4C;
        REC(i + 0x60).unk_10 = (u8 *)D_800E6280.unk_1A5C;
        REC(i + 0x60).unk_14 = 0;
        REC(i + 0x60).unk_04 = 8;
        REC(i + 0x60).unk_05 = 0x80;
        REC(i + 0x60).unk_34 = (u8 *)D_800E6280.unk_1A3C;
    }
    REC(0x68).unk_18 = arg0;
    REC(0x68).unk_26 = 0x40;
    REC(0x68).unk_2A = -0x28;
    if (arg0 == 0xB) {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)D_800B3538[12], 0);
    } else {
        set_kanji_string(-0x80, -0x62, 3, (u8 *)D_800B3538[arg0], 0);
    }
    set_kanji_string(-0x20, -0x62, 5, "電話番号", 0);
    set_kanji_string(-0x8A, -0x4E, 4, "誕生日", 0);
    strcpy(buf, D_800B32CC[p->unk_10.date.month - 1]);
    strcat(buf, "月");
    day = p->unk_10.date.day;
    if (day >= 10) {
        strcat(buf, D_800B3288[(s32)day / 10]);
    } else {
        strcat(buf, D_800B3644);
    }
    strcat(buf, D_800B3288[(s32)p->unk_10.date.day % 10]);
    strcat(buf, "日");
    set_kanji_string(-0x58, -0x4E, 3, buf, 0);
    set_kanji_string(-0x8A, -0x1E, 4, "ＢＷＨ", 0);
    if (D_800E6280.unk_042 >= 0x2DA) {
        j = 2;
    } else {
        j = 0;
        if (D_800E6280.unk_042 >= 0x16D) {
            j = 1;
        }
    }
    set_kanji_string(-0x58, -0x1E, 3, (u8 *)0x801B2334 + (arg0 * 3 + j) * 0x12, 0);
    set_kanji_string(-0x8A, -0x3E, 4, "星座", 0);
    days = D_80146FD0;
    v = p->unk_10.date.month - 1;
    if (days.w[v] < p->unk_10.date.day) {
        v++;
    }
    set_kanji_string(-0x58, -0x3E, 3, (u8 *)0x801B22BC + (v % 12) * 10, 0);
    set_kanji_string(-0x8A, -0x2E, 4, "血液型", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(D_800E6280.unk_1BC[12].unk_10.w << 0x15)) >> 0x1E) * 8, 0);
    } else {
        set_kanji_string(-0x58, -0x2E, 3, (u8 *)0x801B229C + (((u32)(p->unk_10.w << 0x15)) >> 0x1E) * 8, 0);
    }
    set_kanji_string(-0x8A, -0xE, 4, "クラブ", 0);
    if (arg0 == 0xB) {
        set_kanji_string(-0x58, -0xE, 3, "私設部", 0);
    } else {
        set_kanji_string(-0x58, -0xE, 3, (u8 *)0x801B2200 + (p->unk_0C.b[2] >> 4) * 12, 0);
    }
    set_kanji_string(-0x8A, 2, 4, "備考", 0);
    notes = (u8 *)0x801B25F4 + ((arg0 * 6 + j * 2) << 5);
    set_kanji_string(-0x58, 2, 3, notes, 0);
    set_kanji_string(-0x58, 0x12, 3, notes + 0x20, 0);
}
