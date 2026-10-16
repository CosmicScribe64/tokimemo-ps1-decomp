extern s16 D_800CA220;
extern s16 D_800CA226;
extern s16 D_800CA228;
extern s16 D_800CA22E;
extern s16 D_800CA230;
extern s16 D_800CA238;

void func_8013D5FC(void) {
    u8 temp_a0;
    u8 temp_t6;
    u8 var_v1;

    if ((D_800E6280.unk_F5F == 7) && (D_800E6280.unk_56C[0x31] != 0) && ((u8) D_8015B68C == 1)) {
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
        D_800E6280.unk_110A += 0xD;
        D_800CA230 = 4;
        D_800CA238 = 2;
        D_800CA220 = 4;
        D_800CA228 = 3;
    } else {
        if ((D_800E6280.unk_F5F == 8) && ((u8) D_8015B68C == 3) && (D_800E6280.unk_1BC[8].unk_14[0x18] != 0)) {
            temp_t6 = (D_800E6280.unk_110D + 1) & 0xFF;
            D_800E6280.unk_110D = temp_t6;
            if (D_800E6280.unk_110D == 0) {
                D_800CA14C = 2;
                D_800E6280.unk_110D = temp_t6;
            }
        }
        var_v1 = D_800E6280.unk_110D;
        if ((D_800E6280.unk_F5F == 8) && ((u8) D_8015B68C == 4) && (D_800E6280.unk_1BC[8].unk_14[0x19] != 0)) {
            var_v1 = (var_v1 + 1) & 0xFF;
            if (var_v1 == 0) {
                D_800CA14C = 2;
            }
        }
        D_800E6280.unk_110D = var_v1;
        if (var_v1 >= 0xBU) {
            D_800E6280.unk_110D = 0xA;
        }
        func_80085A60(D_800E6280.unk_F5F, (u8) D_8015B68C, 8);
        temp_a0 = D_800E6280.unk_F5F;
        if ((temp_a0 == 8) && ((u8) D_8015B68C == 3) && (D_800E6280.unk_1BC[8].unk_14[0x18] == 0) && (D_800CA14C == 3)) {
            func_80044750(0x74);
            func_8004E9F4(1);
            D_800CA148 += 1;
            D_800CA14C = 0;
            D_80122CE4 = 0;
            func_8004284C();
        }
        if ((temp_a0 == 8) && ((u8) D_8015B68C == 4)) {
            if ((D_800E6280.unk_1BC[8].unk_14[0x19] == 0) && (D_800CA14C == 3)) {
                func_80044750(0x74);
                func_8004E9F4(1);
                D_800CA148 += 1;
                D_800CA14C = 0;
                D_80122CE4 = 0;
                func_8004284C();
            }
        }
    }
}
