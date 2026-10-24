#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_801476E4;

void func_80142760(void) {
    D_80147630 = 0x80197558;
    D_80147634 = 0x80197F60;
    D_80147638 = 0x80198830;
    D_8014763C = 0x801991A8;
    D_80147640 = 0x80199C00;
    D_80147644 = 0x8019A598;
    D_80147648 = 0x8019B0D4;
    D_8014764C = 0x8019BAEC;
    D_80147650 = 0x8019C4FC;
    D_80147654 = 0x8019CF64;
    D_80147658 = 0x8019D970;
    D_8014765C = 0x8019DF94;
    D_80147660 = 0x8019E5E4;
    D_80147664 = 0x80197624;
    D_80147668 = 0x80198024;
    D_8014766C = 0x801988FC;
    D_80147670 = 0x8019926C;
    D_80147674 = 0x80199CCC;
    D_80147678 = 0x8019A664;
    D_8014767C = 0x8019B1A0;
    D_80147680 = 0x8019BBB8;
    D_80147684 = 0x8019C5C8;
    D_80147688 = 0x8019D030;
    D_8014768C = 0x8019DA3C;
    D_80147690 = 0x8019DFBC;
    D_80147694 = 0x8019E6B0;
    D_80147698 = 0x801979D0;
    D_8014769C = 0x801983B0;
    D_801476A0 = 0x80198CA8;
    D_801476A4 = 0x801995F8;
    D_801476A8 = 0x8019A078;
    D_801476AC = 0x8019AA2C;
    D_801476B0 = 0x8019B54C;
    D_801476B4 = 0x8019BF64;
    D_801476B8 = 0x8019C974;
    D_801476BC = 0x8019D3DC;
    D_801476C0 = 0x8019DE04;
    D_801476C4 = 0x8019E05C;
    D_801476C8 = 0x8019EA5C;
}

void func_801429D4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_801476E4;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

s32 func_80142A5C(void) {
    if (D_800E6280.substep == 0) {
        func_800674B0();
        D_800E6280.substep += 1;
    }
    if (D_800E6280.substep == 1) {
        if (D_800E6280.step_timer.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.substep = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.substep += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_80142B20(void) {
    u8 *p = &D_800E6280.day_events[D_800E6280.day_event_index].girl;

    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[*p], D_800B36C8[*p], D_800B3708[*p]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80142BB8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.step_timer.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.step - 1) & 0xFF);
    }
}

void func_80142C38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80142C60(void) {
    cd_read_sectors(0x10, 0x80197000, 0xAF23);
    func_80142760();
    func_8004284C();
}

/* FAKE: T-24503, the unused locals pad0, pad1 and pad2 keep the original's stack slots.
 * FAKE: T-11900, the empty tests `if (&D_800E6280) {}` and `if (&flags[i]) {}` at the top of the first
 * loop body set the allocation priorities of the GameState base and of the flag pointer; the packed-appointment
 * compare reads D_800E71DF, which the original reloads with an absolute lui at that test.
 * T-25403: matched; drafts/T-25403/GEKO/func_80142C9C/NOTES.md (pass 2) explains the two FAKEs below. */
void func_80142C9C(void) {
    s32 pad0;
    u8 day;
    u8 spill;
    u8 month;
    u8 year;
    u8 *q;
    u8 *p;
    u8 sel;
    u8 idx;
    u8 r;
    u8 sum;
    s16 i;
    u8 pad1[20];
    s16 j;
    u8 total;
    s16 slot;
    GekoBytes209 weights;
    u8 flags[21];
    s32 pad2;
    GekoMonthDays monthDays;
    GekoBytes132 tblA;
    GekoBytes132 tblB;
    GekoBytes21 minYear;
    GekoBytes21 minMonth;
    GekoText627 names;
    GekoBytes37 banned;
    GekoBytes21 ids;

    weights = D_80147764;
    monthDays = D_80147838;
    tblA = D_80147850;
    tblB = D_801478D4;
    minYear = D_80147958;
    minMonth = D_80147970;
    names = D_80147988;
    banned = D_80147BFC;
    ids = D_80147C24;
    if (*(u8 *)&D_800E6280.year == 0x60) {
        monthDays.day[1] = 0x1D;
    }
    spill = 0;
    if ((u32) monthDays.day[*(u8 *)&D_800E6280.month - 1] < (u32) ((*(u8 *)&D_800E6280.day - *(u8 *)&D_800E6280.weekday) + 6)) {
        spill = 1;
    }
    year = func_80144198(0);
    month = func_80144198(1);
    day = func_80144198(2);
    *(u8 *)&D_800E6280.current_girl = D_800E6280.day_events[*(u8 *)&D_800E6280.day_event_index].girl;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    strcpy(D_800CA19C, "廊下");
    func_80085B3C(9, 0);
    if (year == 0x5F) {
        if ((month >= 4U) && (month < 6U)) {
            slot = 0;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 1;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 2;
        } else {
            slot = 3;
        }
    } else if (year == 0x60) {
        if ((month != 0) && (month < 3U)) {
            slot = 3;
        } else if ((month >= 3U) && (month < 6U)) {
            slot = 4;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 5;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 6;
        } else {
            slot = 7;
        }
    } else if (year == 0x61) {
        if ((month != 0) && (month < 3U)) {
            slot = 7;
        } else if ((month >= 3U) && (month < 6U)) {
            slot = 8;
        } else if ((month >= 6U) && (month < 9U)) {
            slot = 9;
        } else if ((month >= 9U) && (month < 0xCU)) {
            slot = 10;
        } else {
            slot = 11;
        }
    } else {
        slot = 11;
    }
    total = 0;
    for (i = 0; i < 0x15; i++) {
        if (&D_800E6280) {}
        if (&flags[i]) {}
        flags[i] = 0;
        if ((D_800E6280.progress_state[i] == 1) &&
            (year >= minYear.v[i]) &&
            ((year != minYear.v[i]) || (month >= minMonth.v[i])) &&
            ((i != 0xE) || !(D_800E6280.girl_event_bits[*(u8 *)&D_800E6280.current_girl] & 1)) &&
            ((i != 0xF) || !(D_800E6280.girl_event_bits[*(u8 *)&D_800E6280.current_girl] & 2)) &&
            ((i != 7) || !(D_800E6280.girl_event_bits[*(u8 *)&D_800E6280.current_girl] & 4)) &&
            ((i != 0xE) || (tblA.v[*(u8 *)&D_800E6280.current_girl * 0xC + slot] != 0)) &&
            ((i != 0xF) || (tblB.v[*(u8 *)&D_800E6280.current_girl * 0xC + slot] != 0)) &&
            ((i != 7) || (((year != 0x61) || (month < 9U) || (month >= 0xCU) || (D_800E6280.girls[*(u8 *)&D_800E6280.current_girl].event_counters[0xC] == 0)) &&
                          ((year != 0x61) || (month < 3U) || (month >= 6U) || (D_800E6280.girls[*(u8 *)&D_800E6280.current_girl].event_counters[0xD] == 0)) &&
                          ((year != 0x5F) || (month < 9U) || (month >= 0xCU) || (D_800E6280.girls[*(u8 *)&D_800E6280.current_girl].event_counters[0xE] == 0)))) &&
            (i != 0) && (i != 1) && (i != 0x12) &&
            ((i != 0xD) || ((month != 2) && (month != 0xC))) &&
            ((i != 0xE) || (year != 0x5F) || (month >= 6U)) &&
            (((i != 0x10) && (i != 0x11)) || (D_800E6280.month_calendar[spill].day_kind[day] == 2) || (month == 7)) &&
            (((i != 0x13) && (i != 0x14)) || (D_800E6280.month_calendar[spill].day_kind[day] == 2) || (month == 0xC)) &&
            (i != banned.v[D_800E6280.girl_last_date_spot[*(u8 *)&D_800E6280.current_girl]]) &&
            ((D_800E71DF != (((u32) (*(u32 *)&D_800E6280.unk_54C[0].w * 8)) >> 0x1C)) || (i != (*(u32 *)&D_800E6280.unk_54C[0].w & 0x1F))) &&
            ((*(u8 *)&D_800E6280.current_girl != 1) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x10) && (i != 0x11) && (i != 0x14))) &&
            ((*(u8 *)&D_800E6280.current_girl != 2) || (i != 0x14)) &&
            ((*(u8 *)&D_800E6280.current_girl != 3) || ((i != 0x10) && (i != 0x11))) &&
            ((*(u8 *)&D_800E6280.current_girl != 4) || (i != 7)) &&
            ((*(u8 *)&D_800E6280.current_girl != 5) || (i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) &&
            ((*(u8 *)&D_800E6280.current_girl != 9) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x14))) &&
            ((*(u8 *)&D_800E6280.current_girl != 0xA) || ((i != 7) && (i != 0xB)))) {
            /* FAKE: T-25403, ids.v[i] read through names (ids sits 0x40 below it). The &ids address
             * constant then has no use in this loop, so uopt gives it $t4 in the second loop only. */
            idx = ((u8 *)&names)[i - 0x40];
            p = &weights.v[*(u8 *)&D_800E6280.current_girl * 0x13];
            if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
                if (i == 0xD) {
                    idx++;
                }
            }
            total += p[idx];
            flags[i] = 1;
        }
    }
    srand(*(u32 *)&D_800E6280.frame_count);
    r = rand() % total;
    total = 0;
    sel = 2;
    for (i = 0; i < 0x15; i++) {
        if (flags[i] == 0) {
            continue;
        }
        idx = ids.v[i];
        p = &weights.v[*(u8 *)&D_800E6280.current_girl * 0x13];
        if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
            if (i == 0xD) {
                idx++;
            }
        }
        total += p[idx];
        if (total >= r) {
            sel = i;
            break;
        }
    }
    q = &ids.v[sel];
    D_801476E0 = *q;
    if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
        /* FAKE: T-25403, comparing two variables keeps q first in the ucode, so uopt keeps
         * &ids.v[13] as its own constant (addiu t7,sp,0x41) instead of folding it into q. */
        p = &ids.v[13];
        if (q == p) {
            D_801476E0 += 1;
        }
    }
    func_80144270();
    k_reset(1);
    *(s8 *)&D_800E6280.tile_layer_enabled = 0;
    *(s32 *)&D_800E6280.quads_enabled = 1;
    func_8008585C();
    *(u8 *)&D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_801476D8 = 0;
    D_801476DC = 0;
    func_800847B8(*(u8 *)&D_800E6280.current_girl);
    func_80142760();
    switch (*(u8 *)&D_800E6280.current_girl) {
    case 0:
        D_801476CC = D_80147634;
        D_801476D0 = D_80147668;
        D_801476D4 = D_8014769C;
        break;
    case 1:
        D_801476CC = D_80147644;
        D_801476D0 = D_80147678;
        D_801476D4 = D_801476AC;
        break;
    case 2:
        D_801476CC = D_80147638;
        D_801476D0 = D_8014766C;
        D_801476D4 = D_801476A0;
        break;
    case 3:
        D_801476CC = D_8014763C;
        D_801476D0 = D_80147670;
        D_801476D4 = D_801476A4;
        break;
    case 4:
        D_801476CC = D_80147658;
        D_801476D0 = D_8014768C;
        D_801476D4 = D_801476C0;
        break;
    case 5:
        D_801476CC = D_80147648;
        D_801476D0 = D_8014767C;
        D_801476D4 = D_801476B0;
        break;
    case 6:
        D_801476CC = D_80147650;
        D_801476D0 = D_80147684;
        D_801476D4 = D_801476B8;
        if (((u8) get_g_zyotai_s(*(u8 *)&D_800E6280.current_girl) & 0x7F) < 2) {
            D_801476CC = D_8014764C;
            D_801476D0 = D_80147680;
            D_801476D4 = D_801476B4;
        }
        break;
    case 7:
        D_801476CC = D_80147640;
        D_801476D0 = D_80147674;
        D_801476D4 = D_801476A8;
        break;
    case 8:
        D_801476CC = D_80147630;
        D_801476D0 = D_80147664;
        D_801476D4 = D_80147698;
        break;
    case 9:
        D_801476CC = D_80147654;
        D_801476D0 = D_80147688;
        D_801476D4 = D_801476BC;
        break;
    case 10:
        D_801476CC = D_80147660;
        D_801476D0 = D_80147694;
        D_801476D4 = D_801476C8;
        break;
    }
    strcpy(D_800CA280, names.v[D_801476E0]);
    func_8004284C();
}

void func_80143AE4(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80143B0C(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(D_80122CDC, 1, 0);
}

void func_80143B88(void) {
    D_800CA134 = &D_801476D8;
    D_800CA138 = &D_801476DC;
    D_800CA13C = D_801476CC;
    D_800CA140 = D_801476D0;
    D_800CA144 = D_801476D4;
    func_80082764(0xFF, 1, 0);
}

void func_80143C00(void) {
    if (D_80122CDC != 0) {
        D_801476D8 = 0x17;
        D_800E6280.girls[D_800E6280.current_girl].syoushin += 0xF;
        func_80042940(0x18);
    } else {
        D_801476D8 += D_801476E0;
        func_8004284C();
    }
    check_para_limit();
}

void func_80143C94(void) {
    D_801476D8 = 0x14;
    func_8004284C();
}

void func_80143CBC(void) {
    s16 i;
    u8 a;
    u8 b;

    if (D_80122CDC != 0) {
        if (D_800E6280.current_girl == GIRL_YUINA) {
            D_801476D8 = 0x18;
        } else {
            D_801476D8 = 0x17;
        }
        D_800E6280.girls[D_800E6280.current_girl].syoushin += 0x14;
        func_80042940(0x18);
    } else {
        D_801476D8 = 0x15;
        for (i = 0; i < 0xB; i++) {
            if (D_800E6280.girls[i].flags.f.met && D_800E6280.current_girl != i) {
                a = D_800E6280.girls[i].tokimeki / 20;
                b = rand() % (u32)(a + 1);
                if ((u8)(get_g_zyotai_s(i) & 0x7F) < 2) {
                    D_800E6280.girls[i].syoushin = D_800E6280.girls[i].syoushin + b + 6;
                } else {
                    D_800E6280.girls[i].syoushin = D_800E6280.girls[i].syoushin + b + 4;
                }
            }
        }
        func_8004284C();
    }
    check_para_limit();
}

void func_80143EA4(void) {
    if (D_80122CDC != 0) {
        if (D_800E6280.current_girl == GIRL_YUINA || D_800E6280.current_girl == GIRL_MIRA || D_800E6280.current_girl == GIRL_MEGUMI || D_800E6280.current_girl == GIRL_YUMI) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    D_800E6280.girls[D_800E6280.current_girl].affection += 1;
    func_8004284C();
}

void func_80143F48(void) {
    u8 kind;
    GekoDateEntry entry;
    GekoBytes21 tbl;
    s32 pad; /* FAKE: unused local below tbl that sizes the frame to 0x50 while keeping entry at sp+0x48 and tbl at sp+0x30 (T-3330) */

    tbl = D_80147C3C;
    if (D_80122CDC != 0) {
        D_801476CC = D_8014765C;
        D_801476D0 = D_80147690;
        D_801476D4 = D_801476C4;
        D_801476D8 = D_800E6280.girls[D_800E6280.current_girl].syoushin / 20;
        if (D_801476D8 >= 5) {
            D_801476D8 = 4;
        }
    } else {
        entry.bits.place = tbl.v[D_801476E0];
        entry.bits.year = func_80144198(0);
        entry.bits.month = func_80144198(1);
        entry.bits.day = func_80144198(2);
        entry.bits.unk_21 = 6;
        entry.bits.unk_24 = 1;
        entry.bits.girl = D_800E6280.current_girl;
        entry.bits.active = 1;
        entry.bits.unk_30 = 0;
        func_8006B5BC(entry.word);
        kind = tbl.v[D_801476E0];
        if (kind == 0xE) {
            D_800E6280.girl_event_bits[D_800E6280.current_girl] |= 1;
        }
        if (kind == 0xF) {
            D_800E6280.girl_event_bits[D_800E6280.current_girl] |= 2;
        }
        if (kind == 7) {
            D_800E6280.girl_event_bits[D_800E6280.current_girl] |= 4;
        }
    }
    func_8004284C();
}

s32 func_80144198(s32 selector) {
    s16 days_in_month;
    s32 date[3];
    GekoMonthDays days;
    u8 year;
    u8 month;

    year = D_800E6280.year;
    /* FAKE: T-24602, overwritten initial values and the empty test establish
     * result/selector live ranges. The array also keeps the day calculation
     * in one register; separate scalar results create an extra range. */
    date[2] = 0;
    date[1] = 0;
    date[0] = year;
    if (selector) {}
    days = D_80147C54;
    if (year == 0x60) {
        days.day[1] = 29;
    }
    month = D_800E6280.month;
    date[1] = month;
    date[2] = D_800E6280.day - D_800E6280.weekday + 6;
    days_in_month = days.day[month - 1];
    date[0] = year;
    if (days_in_month < date[2]) {
        date[1] = month + 1;
        date[2] -= days_in_month;
        if (date[1] >= 13) {
            date[1] = 1;
            date[0] = year + 1;
        }
    }
    if (selector == 0) {
        return date[0];
    }
    if (selector == 1) {
        return date[1];
    }
    return date[2];
}

void func_80144270(void) {
    s32 pad; /* FAKE: unused local that moves the table to sp+0x2C and the frame to 0x48 (T-3330 rule, T-23005) */
    GekoBytes21 tbl;

    tbl = D_80147C6C;
    if ((*(u8 *)&D_800E6280.current_girl == 1) && (*(u8 *)&D_800E6280.year == 0x60) && ((*(u8 *)&D_800E6280.month == 0xC) || (*(u8 *)&D_800E6280.month == 1) || ((*(u8 *)&D_800E6280.month == 2) && (*(u8 *)&D_800E6280.day < 0x15U))) && (D_800E6280.event_flags[0xC] == 0)) {
        D_801476E0 = tbl.v[0xE];
        D_800E6280.progress_state[0xE] = 1;
    }
    if ((*(u8 *)&D_800E6280.current_girl == 3) && (*(u8 *)&D_800E6280.year == 0x61) && ((*(u8 *)&D_800E6280.month == 9) || (*(u8 *)&D_800E6280.month == 0xA) || ((*(u8 *)&D_800E6280.month == 0xB) && (*(u8 *)&D_800E6280.day < 0x15U))) && (D_800E6280.event_flags[0x18] == 0)) {
        D_801476E0 = tbl.v[7];
        D_800E6280.progress_state[7] = 1;
    }
    if ((*(u8 *)&D_800E6280.current_girl == 4) && ((*(u8 *)&D_800E6280.month == 9) || ((*(u8 *)&D_800E6280.month == 0xA) && (*(u8 *)&D_800E6280.day < 0x15U))) && (*(u8 *)&D_800E6280.year >= 0x60U) && (D_800E6280.event_flags[0x1E] == 0)) {
        D_801476E0 = tbl.v[0xD];
        D_800E6280.progress_state[0xD] = 1;
    }
    if ((*(u8 *)&D_800E6280.current_girl == 7) && ((*(u8 *)&D_800E6280.month == 3) || (*(u8 *)&D_800E6280.month == 4) || ((*(u8 *)&D_800E6280.month == 5) && (*(u8 *)&D_800E6280.day < 0x15U))) && (D_800E6280.event_flags[0x2F] == 0)) {
        D_801476E0 = tbl.v[2];
        D_800E6280.progress_state[2] = 1;
    }
    if (*(u8 *)&D_800E6280.current_girl == 0xA) {
        if (*(u8 *)&D_800E6280.year >= 0x60U) {
            if ((*(u8 *)&D_800E6280.month == 0xA) && (*(u8 *)&D_800E6280.day < 0x15U) && (D_800E6280.event_flags[0x47] == 0)) {
                D_801476E0 = tbl.v[0xD];
                D_800E6280.progress_state[0xD] = 1;
            }
        }
    }
    if (*(u8 *)&D_800E6280.current_girl == 8) {
        if ((((*(u8 *)&D_800E6280.year == 0x5F) && (*(u8 *)&D_800E6280.month == 0xC)) || ((*(u8 *)&D_800E6280.year == 0x60) && (*(u8 *)&D_800E6280.month == 1)) || ((*(u8 *)&D_800E6280.year == 0x60) && (*(u8 *)&D_800E6280.month == 2) && (*(u8 *)&D_800E6280.day < 0x15U))) && (D_800E6280.girls[8].event_counters[6] == 0)) {
            D_801476E0 = tbl.v[3];
            D_800E6280.progress_state[3] = 1;
        }
        if ((*(u8 *)&D_800E6280.year == 0x60) && (D_800E6280.girls[8].event_counters[9] == 0) && ((*(u8 *)&D_800E6280.month == 6) || (*(u8 *)&D_800E6280.month == 7))) {
            D_801476E0 = tbl.v[5];
            D_800E6280.progress_state[5] = 1;
        }
        if ((*(u8 *)&D_800E6280.year == 0x60) && (D_800E6280.girls[8].event_counters[0x12] == 0) && ((*(u8 *)&D_800E6280.month == 9) || (*(u8 *)&D_800E6280.month == 0xA) || ((*(u8 *)&D_800E6280.month == 0xB) && (*(u8 *)&D_800E6280.day < 0x15U)))) {
            D_801476E0 = tbl.v[0xA];
            D_800E6280.progress_state[0xA] = 1;
        }
        if ((*(u8 *)&D_800E6280.year == 0x61) && (D_800E6280.girls[8].event_counters[0x13] == 0) && ((*(u8 *)&D_800E6280.month == 3) || (*(u8 *)&D_800E6280.month == 4) || ((*(u8 *)&D_800E6280.month == 5) && (*(u8 *)&D_800E6280.day < 0x15U)))) {
            D_801476E0 = tbl.v[0xB];
            D_800E6280.progress_state[0xB] = 1;
        }
        if ((*(u8 *)&D_800E6280.year == 0x61) && (D_800E6280.event_flags[0x39] == 0) && ((*(u8 *)&D_800E6280.month == 9) || (*(u8 *)&D_800E6280.month == 0xA) || ((*(u8 *)&D_800E6280.month == 0xB) && (*(u8 *)&D_800E6280.day < 0x15U)))) {
            D_801476E0 = tbl.v[0xF];
            D_800E6280.progress_state[0xF] = 1;
        }
        if ((*(u8 *)&D_800E6280.year == 0x61) && ((*(u8 *)&D_800E6280.month == 0xC) || (*(u8 *)&D_800E6280.month == 1) || ((*(u8 *)&D_800E6280.month == 2) && (*(u8 *)&D_800E6280.day < 0xBU)))) {
            D_801476E0 = tbl.v[0xC];
            D_800E6280.progress_state[0xC] = 1;
        }
        if (((func_80144198(0) == 0x5F) && (func_80144198(1) == 0xC) && (func_80144198(2) == 0xA)) || ((func_80144198(0) == 0x60) && (func_80144198(1) == 0xC) && (func_80144198(2) == 8)) || ((func_80144198(0) == 0x61) && (func_80144198(1) == 0xC) && (func_80144198(2) == 7))) {
            D_801476E0 = tbl.v[8];
            D_800E6280.progress_state[8] = 1;
        }
    }
}
