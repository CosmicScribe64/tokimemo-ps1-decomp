void func_8013D5FC(void) {
    u8 temp_a0;
    u8 temp_t6;
    u8 var_v1;

    if ((D_800E71DF == 7) && (D_800E681D != 0) && ((u8) D_8015B68C == 1)) {
        func_80083440(4);
        D_800CA22E = 4;
        *D_800CA22C = 4;
        D_800CA236 = 2;
        *D_800CA234 = 2;
        D_800CA21E = 4;
        *D_800CA21C = 4;
        D_800CA226 = 3;
        *D_800CA224 = 3;
        D_800CA148 += 2;
        D_800E738A += 0xD;
        D_800CA230 = 4;
        D_800CA238 = 2;
        D_800CA220 = 4;
        D_800CA228 = 3;
    } else {
        if ((D_800E71DF == 8) && ((u8) D_8015B68C == 3) && (D_800E6628 != 0)) {
            temp_t6 = (D_800E738D + 1) & 0xFF;
            D_800E738D = temp_t6;
            if (D_800E738D == 0) {
                D_800CA14C = 2;
                D_800E738D = temp_t6;
            }
        }
        var_v1 = D_800E738D;
        if ((D_800E71DF == 8) && ((u8) D_8015B68C == 4) && (D_800E6629 != 0)) {
            var_v1 = (var_v1 + 1) & 0xFF;
            if (var_v1 == 0) {
                D_800CA14C = 2;
            }
        }
        D_800E738D = var_v1;
        if (var_v1 >= 0xBU) {
            D_800E738D = 0xA;
        }
        normal_date_speak(D_800E71DF, (u8) D_8015B68C, 8);
        temp_a0 = D_800E71DF;
        if ((temp_a0 == 8) && ((u8) D_8015B68C == 3) && (D_800E6628 == 0) && (D_800CA14C == 3)) {
            func_80044750(0x74);
            k_reset(1);
            D_800CA148 += 1;
            D_800CA14C = 0;
            D_80122CE4 = 0;
            func_8004284C();
        }
        if ((temp_a0 == 8) && ((u8) D_8015B68C == 4)) {
            if ((D_800E6629 == 0) && (D_800CA14C == 3)) {
                func_80044750(0x74);
                k_reset(1);
                D_800CA148 += 1;
                D_800CA14C = 0;
                D_80122CE4 = 0;
                func_8004284C();
            }
        }
    }
}
