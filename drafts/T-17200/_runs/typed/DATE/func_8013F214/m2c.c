void func_8013F214(void) {
    s32 sp2C;
    u32 temp_t7;

    if ((D_800E6280.unk_724 == D_800E6280.unk_725) && (D_80122CDC == 0)) {
        sp2C = (s32) D_800E6280.unk_110A;
        func_8013BC68();
        if (sp2C != D_800E6280.unk_110A) {
            D_8015BF4C = 1;
            D_8015BF04 += 1;
            D_800E6280.unk_110A += 0xC;
        }
    } else {
        if ((D_800E6280.unk_724 != D_800E6280.unk_725) && (D_80122CDC == 0)) {
            D_800E6280.unk_110A += 0x1E;
            return;
        }
        D_8015BF3C += D_80122CDC;
        date_arrival = 2;
        temp_t7 = func_80051A68(D_800E6280.unk_F5F, 2, D_80122CDC) & 0x7F;
        D_8015BF04 = ((((temp_t7 >= 2U) + (temp_t7 >= 3U)) * ((date_arrival == 0) + 1)) + (date_arrival * 6)) - ((date_arrival == 2) * 3);
        if (D_80122CDC == 1) {
            if (D_800E6280.unk_724 == D_800E6280.unk_726) {
                D_800CA2F8 = D_800E6280.unk_725;
            } else {
                D_800CA2F8 = D_800E6280.unk_726;
            }
            D_800CA148 = 0;
        }
        if (D_80122CDC == 2) {
            if (D_800E6280.unk_724 == D_800E6280.unk_727) {
                D_800CA2F8 = D_800E6280.unk_725;
            } else {
                D_800CA2F8 = D_800E6280.unk_727;
            }
            D_800CA148 = 0;
        }
        D_800E6280.unk_F5E = D_800E6280.unk0 - 1;
        func_8004284C();
    }
}
