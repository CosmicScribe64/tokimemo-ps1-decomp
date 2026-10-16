/* T-19020 draft, about 78 differing lines (frame and table offset match with two s16 locals declared before the table;
 * register allocation of the table base, the index and the s16 locals differs, case 4 keeps its constants in v1).
 * Needs in include/ovl/RENSYU.h:
 *   typedef struct RensyuTbl13 { s16 v[13]; } RensyuTbl13;  extern RensyuTbl13 D_801343A8; */
void func_80133990(void) {
    s16 var_a;
    s16 var_b;
    RensyuTbl13 tbl;

    tbl = D_801343A8;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004500C(1, 0);
        func_80083474();
        dec_bg_show_switch(1);
        break;
    case 1:
        normal_date_bggirl_fadein();
        break;
    case 2:
        if (D_801342C4 >= 3U) {
            var_a = tbl.v[D_801342D0] + 2;
            if (D_801342D0 == 4) {
                if (((u16) D_800E6280.unk_0F4.h >> 0xC) != (D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) {
                    if ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7) {
                        var_a += 1;
                    }
                    if ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6) {
                        var_a += 2;
                    }
                }
            }
            D_800CA148 = var_a;
            if (D_801342D0 == 6) {
                D_800CA148 = var_a;
                if (((u16) D_800E6280.unk_0F4.h >> 0xC) != (D_800E6280.unk_1BC[6].unk_0C.b[2] >> 4)) {
                    D_800CA148 = var_a + 1;
                }
            }
        } else {
            var_b = tbl.v[D_801342D0] + 3;
            if (D_801342D0 == 4) {
                var_b += 2;
            }
            D_800CA148 = var_b;
            if (D_801342D0 == 6) {
                D_800CA148 = var_b + 1;
            }
        }
        func_8004284C();
        break;
    case 3:
    case 6:
        normal_date_speak();
        break;
    case 4:
        if (D_801342C4 >= 3U) {
            if (D_801342D0 == 0xC) {
                D_800CA148 = 0x26;
            } else {
                D_800CA148 = 0x25;
            }
        } else if ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_03F == 0xC)) {
            D_800CA148 = 0x28;
        } else {
            D_800CA148 = 0x27;
        }
        func_8004284C();
        break;
    case 5:
        if (D_801342D0 == 0xC) {
            normal_date_girl_out();
        } else {
            func_8004284C();
        }
        break;
    case 7:
        if (D_801342D0 == 0xC) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        break;
    case 8:
        func_80044750(0xC1);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
