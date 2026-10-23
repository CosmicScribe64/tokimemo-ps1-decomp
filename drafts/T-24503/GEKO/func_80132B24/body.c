/* T-24503 unmatched candidate with the original 0x50 frame. The packed date is
 * a shared record-field expression; only day, girl and month need scalar homes. */
s32 func_80132B24(void) {
    u32 day;
    u8 girl;
    u8 month;
    GekoThresholds thr;
    GekoMonthDays days;

    thr = D_80144CF8;
    days = D_80144D10;
    if (D_800E6280.unk_03E == 0x60) {
        days.day[1] = 29;
    }
    girl = D_800E6280.unk_F5F;
    if (!D_800E6280.unk_1BC[girl].unk_0C.f.b6) {
        return 0;
    }
    if ((D_800E6280.unk_10F8 & 7) < thr.threshold[girl] || D_80144C50 == 0) {
        month = D_800E6280.unk_03F;
        day = D_800E6280.unk_040;
        if (month == (D_800E6280.unk_1BC[girl].unk_10.u & 0xF)) {
            if (((D_800E6280.unk_1BC[girl].unk_10.u << 23) >> 27) - day < 21 && ((D_800E6280.unk_1BC[girl].unk_10.u << 23) >> 27) - day != 0) {
                D_80144C50 = 10;
            }
        } else if (((D_800E6280.unk_1BC[girl].unk_10.u & 0xF) - month + 12) % 12 == 1 &&
                   ((D_800E6280.unk_1BC[girl].unk_10.u << 23) >> 27) + days.day[(month + 10) % 12] - day < 20) {
            D_80144C50 = 10;
        }
    }
    return girl;
}
