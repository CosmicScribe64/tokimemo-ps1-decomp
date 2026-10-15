void func_801324DC(void) {
    u8 t;
    u8 six;

    func_80048390();
    six = 6;
    if ((D_800CA2F8 == 0xB) && (D_800CA2FC == 2)) {
        D_800CA148 += D_8015B658;
    } else {
        D_800CA148 += D_800CA2FC * D_8015B658;
    }
    D_800CA14C = 0;
    t = func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    D_8015BF04 = date_arrival * six + ((t >= 2) + (t >= 3)) * ((date_arrival == 0) + 1) - (date_arrival == 2) * 3;
    D_8015BF08 = 0;
    *(s16 *)&D_800CA15C = 0;
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
    if ((D_800E6280.unk_041 == six) && (D_800E6280.unk_040 < 8)) {
        D_8015B694 = 1;
    } else {
        D_8015B694 = 0;
    }
    func_80048390(date_arrival);
    func_8007C740();
    D_800CA2AC = 0;
    D_8015B67C = 0;
    func_8004284C();
}
