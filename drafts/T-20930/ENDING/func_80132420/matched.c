/* The scene dialogue entry: shows message text (kind 0 and 6: a title in brackets, 1 and 0xC: a spoken line
 * with its voice, 0xD and 0xF: the same after a wait of 0x3C frames). Old-style definition: kind, page, step
 * and color are read from their argument slots as u8/u16 at each use, and func_80132B04 calls it without a
 * prototype. T-20930 */
s32 func_80132420(text, kind, arg2, voices, page, step, color)
    u8 *text;
    u8 kind;
    s32 arg2;
    s32 ***voices;
    u16 page;
    u16 step;
    u8 color;
{
    /* FAKE: x is the second element of a six-halfword array and pad is never used; together they give the
     * original frame (0x260), the text buffer at sp+0x50 and x in memory at sp+0x256 (the original stores it
     * before set_kanji_string and reloads it after; a plain s16 is constant-folded). Real locals unknown. T-20930 */
    s16 x[6];
    u8 buf[0x204];
    u8 pad[8];

    if (kind != 0xFF) {
        func_800AE0F0(buf, text);
    }
    D_80122CF0 = 0;
    D_800CA2A8 = 0;
    switch (kind) {
    case 0x0:
        D_8013C360 = 0;
        k_speed_set(0x10);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "『 ", 0);
        dialogue_render(x[1] + 0xE, 0x48, 0, buf, 0, "", "", 0);
        D_80122D00 = 1;
        break;
    case 0x6:
        D_8013C360 = 0;
        k_speed_set(0x10);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "（ ", 0);
        dialogue_render(x[1] + 0xE, 0x48, 0, buf, 0, "）", "", 0);
        D_80122D00 = 1;
        break;
    case 0x1:
        D_8013C360 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                func_80046290(voices[page][step][0], voices[page][step][1], D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        set_kanji_string(-0x90, 0x48, color, "「 ", 0);
        dialogue_render(-0x82, 0x48, color, buf, 0, "", "", 0);
        D_80122CE4 = 1;
        break;
    case 0xC:
        D_8013C360 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                func_80046290(voices[page][step][0], voices[page][step][1], D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        set_kanji_string(-0x90, 0x48, color, "「 ", 0);
        dialogue_render(-0x82, 0x48, color, buf, 0, "", "", 0);
        D_80122D18 = 1;
        break;
    case 0xD:
        D_8013C360 = 0;
        if (D_8013C364++ < 0x3C) {
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                func_80046290(voices[page][step][0], voices[page][step][1], D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        dialogue_render(-0x82, D_800CA2C0, color, buf, 0, "", "", 0);
        D_80122D18 = 1;
        break;
    case 0xF:
        D_8013C360 = 0;
        if (D_8013C364++ < 0x3C) {
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                func_80046290(voices[page][step][0], voices[page][step][1], D_800E6280.unk_F5F);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        dialogue_render(-0x82, D_800CA2C0, color, buf, 0, "", "", 0);
        D_80122CE4 = 1;
        break;
    case 0xFF:
        func_8004284C();
        break;
    default:
        func_80046500();
        break;
    }
}
