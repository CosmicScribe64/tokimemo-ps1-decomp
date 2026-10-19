s32 func_80144198(s32 arg0) {
    GekoMonthDays days;
    u8 year;
    u8 month;
    u8 day;
    u8 week;
    s16 len;
    s32 limit;
    s32 sel_month;
    s32 sel_year;
    s32 ret;

    days = D_80147C54;
    year = D_800E6280.unk_03E;
    if (year == 0x60) {
        days.day[1] = 0x1D;
    }
    month = D_800E6280.unk_03F;
    day = D_800E6280.unk_040;
    week = D_800E6280.unk_041;
    len = days.day[month - 1];
    limit = day - week + 6;
    sel_year = year;
    sel_month = month;
    if (len < limit) {
        sel_month = month + 1;
        limit = limit - len;
        if (sel_month >= 0xD) {
            sel_month = 1;
            sel_year = year + 1;
        }
    }
    if (arg0 == 0) {
        ret = sel_year;
    } else if (arg0 == 1) {
        ret = limit;
    } else {
        ret = sel_month;
    }
    return ret;
}
