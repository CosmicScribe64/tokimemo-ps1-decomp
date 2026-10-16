void func_801324DC(void) {
    u32 temp_t6;

    tpage_buf_clear();
    if ((D_800CA2F8 == 0xB) && (D_800CA2FC == 2)) {
        D_800CA148 += D_8015B658;
    } else {
        D_800CA148 += D_800CA2FC * D_8015B658;
    }
    D_800CA14C = 0;
    temp_t6 = get_g_zyotai_s(D_800E71DF) & 0x7F;
    D_8015BF04 = ((((temp_t6 >= 2U) + (temp_t6 >= 3U)) * ((date_arrival == 0) + 1)) + (date_arrival * 6)) - ((date_arrival == 2) * 3);
    D_8015BF08 = 0;
    D_800CA15C = 0;
    D_8015BF18 = 0;
    D_8015BF0C = 0;
    D_8015BF10 = 0;
    D_8015BF34 = 0;
    D_8015BF38 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    D_8015BF28 = 0;
    D_8015BF30 = 0;
    D_8015BF1C = 0;
    D_8015BF20 = 0;
    D_8015B69C = 0;
    D_80122D34 = 0;
    if ((D_800E62C1 == 6) && ((u8) D_800E62C0 < 8U)) {
        D_8015B694 = 1;
    } else {
        D_8015B694 = 0;
    }
    tpage_buf_clear(date_arrival, 6);
    addr_init_bustup();
    D_800CA2AC = 0;
    D_8015B67C = 0;
    func_8004284C();
}
