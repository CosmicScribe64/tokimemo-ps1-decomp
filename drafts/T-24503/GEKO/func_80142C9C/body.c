/* FAKE: T-24503, unused pad0/pad1/pad2 retain the observed stack slots.
 * Empty flag-address and ID-base tests adjust allocator priorities. The volatile
 * selector view prevents CSE of the packed-appointment comparison's fresh read.
 * Original volatility is unknown. This remains unmatched; see README. */
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
    if (*(u8 *)&D_800E6280.unk_03E == 0x60) {
        monthDays.day[1] = 0x1D;
    }
    spill = 0;
    if ((u32) monthDays.day[*(u8 *)&D_800E6280.unk_03F - 1] < (u32) ((*(u8 *)&D_800E6280.unk_040 - *(u8 *)&D_800E6280.unk_041) + 6)) {
        spill = 1;
    }
    year = func_80144198(0);
    month = func_80144198(1);
    day = func_80144198(2);
    *(u8 *)&D_800E6280.unk_F5F = D_800E6280.unk_69C[*(u8 *)&D_800E6280.unk_71C].unk_01;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "廊下");
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
        if (&flags[i]) {}
        flags[i] = 0;
        if ((D_800E6280.unk_120[i] == 1) &&
            (year >= minYear.v[i]) &&
            ((year != minYear.v[i]) || (month >= minMonth.v[i])) &&
            ((i != 0xE) || !(D_800E6280.unk_74C[*(u8 *)&D_800E6280.unk_F5F] & 1)) &&
            ((i != 0xF) || !(D_800E6280.unk_74C[*(u8 *)&D_800E6280.unk_F5F] & 2)) &&
            ((i != 7) || !(D_800E6280.unk_74C[*(u8 *)&D_800E6280.unk_F5F] & 4)) &&
            ((i != 0xE) || (tblA.v[*(u8 *)&D_800E6280.unk_F5F * 0xC + slot] != 0)) &&
            ((i != 0xF) || (tblB.v[*(u8 *)&D_800E6280.unk_F5F * 0xC + slot] != 0)) &&
            ((i != 7) || (((year != 0x61) || (month < 9U) || (month >= 0xCU) || (D_800E6280.unk_1BC[*(u8 *)&D_800E6280.unk_F5F].unk_14[0xC] == 0)) &&
                          ((year != 0x61) || (month < 3U) || (month >= 6U) || (D_800E6280.unk_1BC[*(u8 *)&D_800E6280.unk_F5F].unk_14[0xD] == 0)) &&
                          ((year != 0x5F) || (month < 9U) || (month >= 0xCU) || (D_800E6280.unk_1BC[*(u8 *)&D_800E6280.unk_F5F].unk_14[0xE] == 0)))) &&
            (i != 0) && (i != 1) && (i != 0x12) &&
            ((i != 0xD) || ((month != 2) && (month != 0xC))) &&
            ((i != 0xE) || (year != 0x5F) || (month >= 6U)) &&
            (((i != 0x10) && (i != 0x11)) || (D_800E6280.unk_044[spill].unk_00[day] == 2) || (month == 7)) &&
            (((i != 0x13) && (i != 0x14)) || (D_800E6280.unk_044[spill].unk_00[day] == 2) || (month == 0xC)) &&
            (i != banned.v[D_800E6280.unk_72C[*(u8 *)&D_800E6280.unk_F5F]]) &&
            ((*(volatile u8 *)&D_800E6280.unk_F5F != (((u32) (*(u32 *)&D_800E6280.unk_54C[0].w * 8)) >> 0x1C)) || (i != (*(u32 *)&D_800E6280.unk_54C[0].w & 0x1F))) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 1) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x10) && (i != 0x11) && (i != 0x14))) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 2) || (i != 0x14)) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 3) || ((i != 0x10) && (i != 0x11))) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 4) || (i != 7)) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 5) || (i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 9) || (((i != 0xD) || ((month != 1) && (month != 3) && (month != 6) && (month != 0xB))) && (i != 0x14))) &&
            ((*(u8 *)&D_800E6280.unk_F5F != 0xA) || ((i != 7) && (i != 0xB)))) {
            idx = ids.v[i];
            if (&flags[i]) {}
            p = &weights.v[*(u8 *)&D_800E6280.unk_F5F * 0x13];
            if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
                if (i == 0xD) {
                    idx++;
                }
            }
            total = total + p[idx];
            flags[i] = 1;
        }
    }
    func_800AE120(*(u32 *)&D_800E6280.unk_10F8);
    r = func_800AE0D0() % total;
    total = 0;
    sel = 2;
    q = ids.v;
    if (q) {}
    for (i = 0; i < 0x15; i++) {
        if (flags[i] != 0) {
            idx = q[i];
            p = &weights.v[*(u8 *)&D_800E6280.unk_F5F * 0x13];
            if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
                if (i == 0xD) {
                    idx++;
                }
            }
            total = total + p[idx];
            if (total >= r) {
                sel = i;
                break;
            }
        }
    }
    q = &ids.v[sel];
    D_801476E0 = *q;
    if ((month == 1) || (month == 3) || (month == 6) || (month == 0xB)) {
        if (q == &ids.v[13]) {
            D_801476E0 += 1;
        }
    }
    func_80144270();
    k_reset(1);
    *(s8 *)&D_800E6280.unk_10A2 = 0;
    *(s32 *)&D_800E6280.unk_10E8 = 1;
    func_8008585C();
    *(u8 *)&D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_801476D8 = 0;
    D_801476DC = 0;
    func_800847B8(*(u8 *)&D_800E6280.unk_F5F);
    func_80142760();
    switch (*(u8 *)&D_800E6280.unk_F5F) {
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
        if ((u32) (get_g_zyotai_s(*(u8 *)&D_800E6280.unk_F5F) & 0x7F) < 2U) {
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
    func_800AE0F0(D_800CA280, names.v[D_801476E0]);
    func_8004284C();
}
