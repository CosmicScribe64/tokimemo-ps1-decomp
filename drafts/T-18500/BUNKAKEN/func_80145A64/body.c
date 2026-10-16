void func_80145A64(void) {
    GameState *g = &D_800E6280;
    u8 sp2B;
    u8 temp_t8;
    u8 var_a1;

    if (func_8008667C(D_80122EBC) != 4) {
        if (D_80122EA4 != D_80122EBC) {
            switch (D_80122EB8) {
            case 0:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 0) && (D_80122EBC == 0)) {
                    var_a1 = 3;
                } else {
                    var_a1 = 4;
                    if ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4) == 0) && (D_80122EBC == 1)) {
                        var_a1 = 3;
                    }
                }
block_45:
                sp2B = var_a1;
                break;
            case 1:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 1) && (D_80122EBC == 0)) {
                    var_a1 = 7;
                } else {
                    var_a1 = 8;
                    if ((((u8) g->unk_1BC[1].unk_0C.b[2] >> 4) == 1) && (D_80122EBC == 1)) {
                        var_a1 = 7;
                    }
                }
                goto block_45;
            case 2:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 2) && (D_80122EBC == 0)) {
                    var_a1 = 0xB;
                } else {
                    var_a1 = 0xC;
                    if ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 2) && (D_80122EBC == 2)) {
                        var_a1 = 0xB;
                    }
                }
                goto block_45;
            case 3:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3) && (D_80122EBC == 0)) {
                    var_a1 = 0xF;
                } else {
                    var_a1 = 0x10;
                    if ((((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3) && (D_80122EBC == 2)) {
                        var_a1 = 0xF;
                    }
                }
                goto block_45;
            case 4:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 4) && (D_80122EBC == 0)) {
                    var_a1 = 0x13;
                } else {
                    var_a1 = 0x14;
                    if ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 4) && (D_80122EBC == 3)) {
                        var_a1 = 0x13;
                    }
                }
                goto block_45;
            case 5:
                if ((((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 5) && (D_80122EBC == 0)) {
                    var_a1 = 0x17;
                } else {
                    var_a1 = 0x18;
                    if ((((u8) g->unk_1BC[3].unk_0C.b[2] >> 4) == 5) && (D_80122EBC == 3)) {
                        var_a1 = 0x17;
                    }
                }
                goto block_45;
            }
            if (D_80122EBC == 0xB) {
                g->unk_75E[g->unk_F5E].unk_00 = 0;
                g->unk_75E[g->unk_F5E].unk_03 = get_g_zyotai_h(0, sp2B);
            } else {
                g->unk_75E[g->unk_F5E].unk_00 = (u8) D_80122EBC;
                g->unk_75E[g->unk_F5E].unk_03 = get_g_zyotai_h(D_80122EBC, sp2B);
            }
            g->unk_75E[g->unk_F5E].unk_02 = 0x14;
            g->unk_75E[g->unk_F5E].unk_01 = sp2B;
            g->unk_75E[g->unk_F5E].unk_04 = g->unk_03E;
            g->unk_75E[g->unk_F5E].unk_05 = g->unk_03F;
            g->unk_75E[g->unk_F5E].unk_06 = g->unk_040;
            g->unk_75E[g->unk_F5E].unk_07 = g->unk_041;
            temp_t8 = g->unk_F5E + 1;
            g->unk_F5E = temp_t8;
            if ((u32) (temp_t8 & 0xFF) >= 0xFFU) {
                g->unk_F5E = 0xFE;
            }
        }
    }
}
