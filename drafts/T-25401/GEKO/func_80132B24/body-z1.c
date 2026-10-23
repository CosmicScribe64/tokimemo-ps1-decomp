s32 func_80132B24(void) {
    u32 date;
    u8 girl;
    u8 month;
    GekoThresholds thr;
    GekoMonthDays days;
    u32 day;

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
        date = D_800E6280.unk_1BC[girl].unk_10.w;
        month = D_800E6280.unk_03F;
        day = D_800E6280.unk_040;
        if (month == (date & 0xF)) {
            if (((date << 23) >> 27) - day < 21 && ((date << 23) >> 27) - day != 0) {
                D_80144C50 = 10;
            }
        } else if (((date & 0xF) - month + 12) % 12 == 1 &&
                   ((date << 23) >> 27) + days.day[(month + 10) % 12] - day < 20) {
            D_80144C50 = 10;
        }
    }
    if (girl) {}
}
