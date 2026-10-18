/* T-22029 draft: 44 differing rows (frame and registers agree; the original keeps D_8011ECD0 in two registers and does not distribute 0x61 into the displacement). FAKE: buf[0x10C] only reproduces the 0x188 frame. */
void func_8014790C(void) {
    /* FAKE: the 0x10C size only reproduces the 0x188 frame and the buffer at 0x7C (original size unknown). T-22029 */
    u8 buf[0x10C];
    s32 i;
    s16 *px;
    s16 *py;
    s32 *pv;
    u8 *r;
    s16 x;
    s16 y;
    s32 v;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_8006612C("結果発表 ");
        gnsx(D_800E6280.unk_0D4);
        sndisp("今日は、試験結果の 発表の日だ。）", 0, 0x1F);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        func_8004500C(1, 0x202);
        hizuke_init();
        func_80048E78();
        D_800E6280.unk_110D += 1;
        cd_read_sectors(0xD, 0x801E0000, 0x9CA8);
        break;
    case 1:
        if ((u32) D_800E7384 >= 0x101) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004E58C();
            icon_disp_switch(0);
            parameter_disp_switch(0);
            hizuke_disp_switch(0);
            func_80048F64(0x60);
            D_80120651 = 4;
            D_80120652 = 0x40;
            D_80120688 = 0;
            D_80120653 = 0xA4;
            D_8012065C = (u8 *) 0x801E64A0;
            D_80120660 = (u8 *) 0x801E6520;
            D_80120684 = (u8 *) 0x801E6494;
            D_80120668 = 0;
            D_80120655 = 1;
            D_80120654 = 8;
            D_80120676 = 0;
            D_8012067A = -0x28;
            D_80120657 = 0x80;
            func_80148E98();
            func_80148EE8();
            func_801480D0();
            px = D_80150630;
            py = D_8015064C;
            pv = D_80150684;
            r = D_8011ECD0;
            i = 0;
            do {
                x = *px;
                y = *py;
                v = *pv;
                r[0x19C5] = 3;
                r[0x19C6] = 0x40;
                *(s32 *) &r[0x19FC] = 0;
                r[0x19C7] = 0x24;
                *(s32 *) &r[0x19D0] = 0x801E64A0;
                *(s32 *) &r[0x19D4] = 0x801E6520;
                *(s32 *) &r[0x19F8] = 0x801E6494;
                *(s16 *) &r[0x19DC] = 0x1F;
                r[0x19C9] = 1;
                r[0x19C8] = 8;
                r[0x19CB] = 0x80;
                *(s16 *) &r[0x19EA] = x;
                *(s16 *) &r[0x19EE] = y;
                if (v == 0xD) {
                    strncpy(buf, D_800E6280.unk_0D4, 8);
                    strncat(buf, D_800E6280.unk_0DC, 8);
                    set_kanji_string(*px + 0x18, *py, 3, buf, 0);
                    func_800AE0B0("%s\n", D_800E6280.unk_0C4);
                } else if (v == 0xA) {
                    set_kanji_string(x + 0x18, y, 3, D_800B3538[13], 0);
                } else if (v == 0xB) {
                    if (D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
                        set_kanji_string(x + 0x18, y, 3, D_800B3538[11], 0);
                    } else {
                        ((DisplayActor44 *) D_8011ECD0)[i + 0x61].unk_03 = 0xA4;
                    }
                } else if (D_800E6280.unk_1BC[v].unk_0C.f.b1) {
                    set_kanji_string(x + 0x18, y, 3, D_800B3538[v], 0);
                } else {
                    ((DisplayActor44 *) D_8011ECD0)[i + 0x61].unk_03 = 0xA4;
                }
                i += 1;
                r += 0x44;
                px += 1;
                py += 1;
                pv += 1;
            } while (i != 0xE);
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            func_80148F34();
            func_8004284C();
        }
        break;
    }
}
