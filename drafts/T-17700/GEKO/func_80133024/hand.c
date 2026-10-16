/* T-17700 hand edit of the typed redraft: 40 funcdiff rows (best of 16 local-declaration variants); the
 * table sits 8 bytes too high in the frame until four bytes of locals precede it, and the loop keeps
 * unk_03F/unk_040 in $a3/$a2 in the original. */
s32 func_80133024(void) {
    u8 month;
    u8 day;
    s16 i;
    s32 n;
    GekoMonthDays days;

    days = D_80144D28;
    i = 1;
    if (D_800E6280.unk_03E == 0x60) {
        days.day[1] = 29;
    }
    D_80144C50 = 0;
    do {
        month = D_800E6280.unk_03F;
        day = D_800E6280.unk_040;
        n = days.day[month - 1];
        if ((u32) n >= (u32) (day + i)) {
            func_80132D04(0, (s16) (day + i - 1));
        } else {
            func_80132D04(1, (s16) (day + i - n - 1));
        }
        i++;
    } while (i < 20);
    func_80132B24();
}
