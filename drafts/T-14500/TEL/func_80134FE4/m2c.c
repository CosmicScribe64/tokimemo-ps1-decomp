? func_8005142C(u8);                                /* extern */
extern u8 D_800E6354;
extern ? D_800E6442;
extern ? D_800E644B;
extern u8 D_800E738D;
extern ? D_801406E0;
extern ? D_80140710;
extern ? D_80140770;

void func_80134FE4(void) {
    u32 temp_t0;

    switch (D_800E738D) {
    case 0:
        func_8004EAAC();
        func_8005142C(D_801405E4);
        D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] += 1;
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80051010(*(&D_801406E0 + (D_801405E4 * 4)), 0, D_801405E4);
        func_8004EAD4(0);
        D_800E738D += 1;
        func_80052060((s32) &D_80140710, (s32) &D_80140770, (s32) D_801405E4, (s32) D_801405E4);
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        func_8004EAAC();
        func_80050DFC(&D_800E6354);
        func_80050E8C("えっと… Pって言いますけど。", 0, 0xF);
        func_8004EAD4(0);
        D_800E738D += 1;
        break;
    case 3:
        func_80051DBC();
        break;
    case 4:
        if (func_80051A68(D_801405E4) == 4) {
            *(&D_800E6442 + (D_801405E4 * 0x38)) = 0x10;
            func_80042940(3);
            if (D_801405E4 == 7) {
                D_800E738D = 0;
            } else {
                D_800E738D = 4;
            }
        } else {
            temp_t0 = func_80051A68(D_801405E4) & 0x7F;
            if (temp_t0 < 2U) {
                if (D_801405E4 == 0) {
                    func_80042940(8);
                } else if (D_801405E4 == 7) {
                    if (*(&D_800E644B + (D_801405E4 * 0x38)) & 1) {
                        func_80042940(9);
                    } else {
                        goto block_17;
                    }
                } else {
block_17:
                    if (D_801405E4 == 9) {
                        func_80042940(0xA);
                    } else {
                        func_80042940(4);
                    }
                }
            } else if (temp_t0 == 2) {
                if ((D_801405E4 == 0xA) && (*(&D_800E644B + (D_801405E4 * 0x38)) & 1)) {
                    func_80042940(7);
                }
                func_80042940(5);
            } else {
                func_80042940(6);
            }
        }
        break;
    default:
        func_80042940(0x10);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
