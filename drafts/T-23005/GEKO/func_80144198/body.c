/* T-23005 hand draft (not built in the tree): 57 rows. The original keeps arg0 in a2 (move a2,a0); this form spills it to its home slot. */
s32 func_80144198(s32 arg0) {
    GekoMonthDays days;
    u8 year;
    u8 month;
    s16 len;
    s32 day;
    s32 retYear;
    s32 retMonth;

    year = D_800E6280.unk_03E;
    days = D_80147C54;
    if (year == 0x60) {
        days.day[1] = 0x1D;
    }
    month = D_800E6280.unk_03F;
    len = days.day[month - 1];
    day = (D_800E6280.unk_040 - D_800E6280.unk_041) + 6;
    retYear = year;
    retMonth = month;
    if (len < day) {
        retMonth = month + 1;
        day -= len;
        if (retMonth >= 0xD) {
            retMonth = 1;
            retYear = year + 1;
        }
    }
    if (arg0 == 0) {
        return retYear;
    }
    if (arg0 == 1) {
        return retMonth;
    }
    return day;
}
