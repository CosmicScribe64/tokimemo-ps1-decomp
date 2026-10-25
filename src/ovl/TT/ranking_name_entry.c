/* ranking_name_entry.c: src/ovl/TT/80145E60.c:4: ranking insertion, name-entry display and button input state; body 10: a = tt_sys; 11: b = tt_run;. Original object 0x80145E60-0x80146BE0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

s32 tt_ranking_try_insert(void) {
    u8 *sys;
    u8 *run;
    u16 elapsed_frames;
    u16 ranking_slot;

    sys = tt_sys;
    run = tt_run;
    elapsed_frames = *(u16 *)(run + 8);
    if (elapsed_frames < *(u16 *)(sys + 0x30)) {
        *(u16 *)(sys + 0x3C) = *(u16 *)(sys + 0x36);
        *(u16 *)(sys + 0x36) = *(u16 *)(sys + 0x30);
        *(u16 *)(sys + 0x30) = *(u16 *)(run + 8);
        memcpy(sys + 0x3E, sys + 0x38, 3);
        memcpy(sys + 0x38, sys + 0x32, 3);
        *(u16 *)(run + 0x18) = 0;
    } else if (elapsed_frames < *(u16 *)(sys + 0x36)) {
        *(u16 *)(sys + 0x3C) = *(u16 *)(sys + 0x36);
        *(u16 *)(sys + 0x36) = *(u16 *)(run + 8);
        memcpy(sys + 0x3E, sys + 0x38, 3);
        *(u16 *)(run + 0x18) = 1;
    } else if (elapsed_frames < *(u16 *)(sys + 0x3C)) {
        *(u16 *)(sys + 0x3C) = *(u16 *)(run + 8);
        *(u16 *)(run + 0x18) = 2;
    } else {
        return 0;
    }
    sys[*(u16 *)(run + 0x18) * 6 + 0x32] = 0xA;
    sys[*(u16 *)(run + 0x18) * 6 + 0x33] = 0x2F;
    sys[*(u16 *)(run + 0x18) * 6 + 0x34] = 0x2F;
    sys[*(u16 *)(run + 0x18) * 6 + 0x35] = 0xFF;
    ranking_slot = *(u16 *)(run + 0x18);
    tt_name_text_set(sys + 0x32 + ranking_slot * 6, ranking_slot + 0x2B);
    return 1;
}

void tt_name_entry_init(void) {
    u8 *src;
    u8 *p;
    s32 i;
    s32 angle;
    u8 *run;
    u8 *a;

    run = tt_run;
    src = tt_transforms;
    a = tt_texts;
    *(s16 *)(a + 0x14A) = -0x40;
    *(s16 *)(a + 0x14C) = 0x48;
    *(s16 *)(a + 0x14E) = 0x11;
    a[0x142] = 1;
    a[0x140] = 1;
    *(u16 *)(a + 0x146) = 8;
    *(s16 *)(a + 0x150) = 0;
    a += 0x154;
    *(s16 *)(a + 0xA) = 0;
    *(s16 *)(a + 0xC) = 0x58;
    *(s16 *)(a + 0xE) = 0x26;
    a[2] = 1;
    a[0] = 1;
    *(u16 *)(a + 6) = 8;
    *(s16 *)(a + 0x10) = 0;
    /* Reusing a for the second record keeps its range in v0, so the shared 8 gets v1 and run a0. T-11860 */
    a = tt_sprites;
    a[0] = 1;
    a[1] = 0;
    *(u16 *)(a + 0x44) = 8;
    *(s16 *)(a + 0x14) = 0x34;
    *(s16 *)(a + 0x16) = *(u16 *)(run + 0x18) * 16 + 0x7E;
    *(s16 *)(a + 0x3C) = 8;
    *(s16 *)(a + 0x3E) = 8;
    a[0x46] = 0x30;
    a[0x47] = 0x10;
    *(s16 *)(a + 0x40) = 0xA;
    *(s16 *)(a + 0x42) = 0x3C0F;
    a[0x3B] = 0x65;
    *(s16 *)(a + 0x10) = 0;
    for (i = 0, angle = 0, p = tt_sprites + 0x1900; i < 3; i++, angle += 0x55, p += 0x64) {
        p[0] = 1;
        p[1] = 0;
        p[0x3B] = 0x80;
        *(u8 **)(p + 0x48) = src;
        p[0x4D] = 0;
        p[0x4E] = 1;
        p[0x38] = 0x40;
        p[0x39] = 0x40;
        p[0x3A] = 0x40;
        p[0x50] = 0;
        *(s16 *)(p + 0x14) = *(s16 *)(src + 4) - 0x24;
        *(s16 *)(p + 0x16) = *(s16 *)(src + 6) - 0x24;
        *(s16 *)(p + 0x14) = func_800A0140(angle) / 128;
        *(s16 *)(p + 0x16) = rsin(angle) / 48 + 0x78;
        p[0x46] = (i & 0xF) * 8;
        p[0x47] = (i / 16) * 8;
        p[0x4F] = 0;
        *(s16 *)(p + 0x3C) = 8;
        *(s16 *)(p + 0x3E) = 8;
        *(s16 *)(p + 0x40) = 0xA;
        *(s16 *)(p + 0x42) = 0x3C00;
        p[0x4C] = 0;
        *(s16 *)(p + 0x10) = 0xD;
    }
}

void tt_name_entry_grid_wave(void) {
    u8 *run = tt_run;
    s16 *transform = (s16 *)tt_transforms;
    s32 i;

    for (i = 0; i != 0x19; i++) {
        s16 v = func_800A0140(*(u16 *)(run + 0x1E));

        transform += 8;
        transform[-8] = v;
        transform[-7] = 0x1000;
    }
    *(u16 *)(run + 0x1E) += 0x10;
}

void tt_name_entry_fade_in(void) {
    u8 *core;
    u8 *q;
    s32 i;

    core = tt_core;
    core[0x1D] += 4;
    core[0x1E] = core[0x1F] = core[0x1D];
    if (core[0x1D] >= 0x80U) {
        q = tt_texts;
        for (i = 0; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 0;
        }
        q = tt_sprites;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 0;
            q += 0x64;
        }
        *(s16 *)(core + 2) = 0x40;
    }
    tt_name_entry_grid_wave();
}

void tt_name_entry_update(void) {
    u8 *sys;
    u8 *core;
    u8 *run;
    u8 *r;
    u8 *sprite;
    u8 *ctl;
    u16 k;
    u16 v;
    u8 col;
    u16 i;

    sys = tt_sys;
    core = tt_core;
    run = tt_run;
    r = tt_texts + *(u16 *)(run + 0x18) * 0x3C;
    sprite = tt_sprites;
    ctl = sys + 0x10;
    switch (*(u16 *)(core + 4)) {
    case 0:
        *(s16 *)(run + 0x1A) = 0;
        *(s16 *)(run + 0x1C) = 0xA;
        *(s16 *)(core + 4) = 0x40;
        break;
    case 0x40:
        if (*(u16 *)(ctl + 4) & 0x8000) {
            tt_se_request(0x615);
            (*(u16 *)(run + 0x1C))--;
            if (*(s16 *)(run + 0x1C) < 0) {
                *(s16 *)(run + 0x1C) = 0x2F;
            }
            *(s16 *)(run + 0x10) = 0;
        } else if (*(u16 *)(ctl + 4) & 0x2000) {
            tt_se_request(0x615);
            if (++*(u16 *)(run + 0x1C) >= 0x30) {
                *(s16 *)(run + 0x1C) = 0;
            }
            *(s16 *)(run + 0x10) = 0;
        }
        if (*(u16 *)ctl & 0x8000) {
            if (++*(u16 *)(run + 0x10) >= 0x1F) {
                tt_se_request(0x615);
                *(s16 *)(run + 0x10) = 0x1A;
                (*(u16 *)(run + 0x1C))--;
                if (*(s16 *)(run + 0x1C) < 0) {
                    *(s16 *)(run + 0x1C) = 0x2F;
                }
            }
        } else if (*(u16 *)ctl & 0x2000) {
            if (++*(u16 *)(run + 0x10) >= 0x1F) {
                tt_se_request(0x615);
                *(s16 *)(run + 0x10) = 0x1A;
                if (++*(u16 *)(run + 0x1C) >= 0x30) {
                    *(s16 *)(run + 0x1C) = 0;
                }
            }
        }
        k = *(u16 *)(ctl + 4);
        if (k & 4) {
            v = *(u16 *)(run + 0x1A);
            if (v != 0) {
                tt_se_request(0x617);
                v = --*(u16 *)(run + 0x1A);
            }
            *(s16 *)(run + 0x1C) = ((TtRankingEntry *)(sys + 0x30))[*(u16 *)(run + 0x18)].name[v];
            k = *(u16 *)(ctl + 4);
        } else if (k & 8) {
            v = *(u16 *)(run + 0x1A);
            if (v < 2) {
                tt_se_request(0x617);
                v = ++*(u16 *)(run + 0x1A);
            }
            *(s16 *)(run + 0x1C) = ((TtRankingEntry *)(sys + 0x30))[*(u16 *)(run + 0x18)].name[v];
            k = *(u16 *)(ctl + 4);
        }
        if (k & 0x20) {
            tt_se_request(0x616);
            v = *(u16 *)(run + 0x1A);
            if (v < 2) {
                *(u16 *)(run + 0x1A) = v + 1;
                *(s16 *)(run + 0x1C) = 0xA;
                k = *(u16 *)(ctl + 4);
            } else {
                core[0xA] = 1;
                k = *(u16 *)(ctl + 4);
            }
        }
        if ((k & 0x40) && *(u16 *)(run + 0x1A) != 0) {
            tt_se_request(0x617);
            ((TtRankingEntry *)(sys + 0x30))[*(u16 *)(run + 0x18)].name[*(u16 *)(run + 0x1A)] = 0x3F;
            (*(u16 *)(run + 0x1A))--;
            *(s16 *)(run + 0x1C) = ((TtRankingEntry *)(sys + 0x30))[*(u16 *)(run + 0x18)].name[*(u16 *)(run + 0x1A)];
            k = *(u16 *)(ctl + 4);
        }
        if (k & 0x800) {
            tt_se_request(0x616);
            core[0xA] = 1;
        }
        ((TtRankingEntry *)(sys + 0x30))[*(u16 *)(run + 0x18)].name[*(u16 *)(run + 0x1A)] = *(u16 *)(run + 0x1C);
        break;
    }
    switch (*(u16 *)(run + 0x18)) {
    case 0:
        col = 2;
        break;
    case 1:
        col = 6;
        break;
    case 2:
        col = 4;
        break;
    }
    v = (*(u16 *)(sys + 0xA) & 3) ? col : 0xF;
    r[0x2A] = v;
    r[0x16] = v;
    r[2] = v;
    i = *(u16 *)(run + 0x18);
    tt_name_text_set(((TtRankingEntry *)(sys + 0x30))[i].name, i + 0x2B);
    *(s16 *)(sprite + 0x14) = *(u16 *)(run + 0x1A) * 8 + 0x34;
    *(s16 *)(sprite + 0x16) = *(u16 *)(run + 0x18) * 16 + 0x7E;
    sprite[0] = (*(u16 *)(sys + 0xA) >> 3) & 1;
}

void tt_name_entry_fade_out(void) {
    u8 *core;
    u8 *q;
    s32 i;

    core = tt_core;
    if (core[0x1D] == 0x80U) {
        i = 0;
        q = tt_texts;
        for (; i < 0x80; i++) {
            q += 0x14;
            *(s16 *)(q - 0xE) = 8;
        }
        q = tt_sprites;
        for (i = 0; i < 0x80; i++) {
            *(s16 *)(q + 0x44) = 8;
            q += 0x64;
        }
    }
    core[0x1D] -= 4;
    core[0x1E] = core[0x1F] = core[0x1D];
    if (core[0x1D] == 0) {
        core[9] = 1;
    }
    tt_name_entry_grid_wave();
}

/* a4 and a8 (declared around c) give the original frame: c at sp+0x3C, col at sp+0x37. The colour is stored with one
 * chain so it is loaded once at the join; three separate stores keep it in v0 and move r and 0x80. T-11860 */
void tt_state_name_entry(void) {
    u8 *core;
    u8 *sprite;
    u8 *run;
    u8 *text_record;
    u8 col;
    u8 *r;

    core = tt_core;
    run = tt_run;
    switch (*(u16 *)(core + 2)) {
    case 0:
        audio_command(0x22);
        audio_command(0x2F);
        audio_command(4);
        audio_command(0x206);
        core[0x1C] = 1;
        tt_sprites_clear();
        tt_texts_clear();
        tt_player_clear();
        tt_shots_clear();
        tt_bombs_clear();
        tt_bomb_targets_clear();
        tt_options_clear();
        tt_air_enemies_clear();
        tt_ground_targets_clear();
        tt_enemy_shots_clear();
        tt_bells_clear();
        tt_clouds_clear();
        tt_boss_sprites_clear();
        core[0x11] = 0;
        if (tt_ranking_try_insert() != 0) {
            core[0x1D] = 0;
            core[0x1E] = 0;
            core[0x1F] = 0;
            tt_name_entry_init();
            tt_ranking_rows_init(2, 6, 4);
            tt_ranking_panel_init(0xC, 1);
            *(s16 *)(run + 0x1E) = 0;
        } else {
            core[9] = 1;
        }
        *(u16 *)(core + 0x26) = 0xE10;
        *(u16 *)(core + 2) = 0x20;
        break;
    case 0x20:
        tt_name_entry_fade_in();
        break;
    case 0x40:
        tt_name_entry_grid_wave();
        tt_name_entry_update();
        if (core[0xA] != 0 || --*(u16 *)(core + 0x26) == 0) {
            text_record = tt_texts;
            r = text_record + *(u16 *)(run + 0x18) * 0x3C;
            sprite = tt_sprites;
            sprite[0] = 0;
            core[0xA] = 0;
            *(u16 *)(core + 4) = 0;
            *(u16 *)(core + 2) = 0x80;
            *(u16 *)(core + 0x28) = 0x3C;
            switch (*(u16 *)(run + 0x18)) {
            case 0:
                col = 2;
                break;
            case 1:
                col = 6;
                break;
            case 2:
                col = 4;
                break;
            }
            /* FAKE: one chain store leaves col with a single use, so its range splits to a stack slot as in the original. T-11860 */
            r[2] = r[0x16] = r[0x2A] = col;
            *(u16 *)(core + 2) = 0x70;
        }
        break;
    case 0x70:
        tt_name_entry_grid_wave();
        if (--*(u16 *)(core + 0x28) == 0) {
            core[0x1D] = 0x80;
            core[0x1E] = 0x80;
            core[0x1F] = 0x80;
            *(u16 *)(core + 2) = 0x80;
        }
        break;
    case 0x80:
        tt_name_entry_fade_out();
        break;
    }
    if (core[9] != 0) {
        *(u16 *)(core + 0) = 7;
        *(u16 *)(core + 6) = 0;
        *(u16 *)(core + 4) = 0;
        *(u16 *)(core + 2) = 0;
        core[0xB] = 0;
        core[0xA] = 0;
        core[9] = 0;
    }
    tt_countdown_text_set(*(u16 *)(core + 0x26) / 60, 0x26);
}
