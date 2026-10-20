/* T-24301 draft, 168 rows: the F64 word tests are CharFlags bit-fields (sll/bgez) and b7 is the sign of
 * byte 0. The original keeps &D_800E6280 in $a0 for the F64 accesses only (lw 0xF64(a0)), loads the word
 * into $v0 once before the switch and hoists 0x5F/0x60/0x61 into a1..a3; empty `== 0x61` tests remain. */
void func_8006D138(void) {
    GameState *g = &D_800E6280;

    switch (g->unk_03F) {
    case 6:
    case 7:
        if (g->unk_03E == 0x5F) {
            if (((CharFlags *)g->unk_F64)->b1) {
                g->unk_120[1] = 1;
            }
            g->unk_120[0xE] = 1;
        }
        if ((g->unk_03E == 0x60) && ((CharFlags *)g->unk_F64)->b5) {
            g->unk_120[5] = 1;
        }
        break;
    case 9:
    case 10:
        if ((g->unk_03E == 0x5F) && ((CharFlags *)g->unk_F64)->b2) {
            g->unk_120[7] = 1;
        }
        if ((g->unk_03E == 0x60) && ((CharFlags *)g->unk_F64)->b6) {
            g->unk_120[0xA] = 1;
        }
        break;
    case 1:
        if ((g->unk_03E == 0x60) && ((CharFlags *)g->unk_F64)->b3) {
            g->unk_120[3] = 1;
        }
        if (g->unk_03E == 0x61) {
            if (((CharFlags *)g->unk_F64)->b7) {
                g->unk_120[6] = 1;
            }
        }
        break;
    case 12:
        if ((g->unk_03E == 0x5F) && ((CharFlags *)g->unk_F64)->b3) {
            g->unk_120[3] = 1;
        }
        if ((g->unk_03E == 0x60) && ((CharFlags *)g->unk_F64)->b7) {
            g->unk_120[6] = 1;
        }
        break;
    case 3:
    case 4:
        if ((g->unk_03E == 0x60) && ((CharFlags *)g->unk_F64)->b4) {
            g->unk_120[0xD] = 1;
        }
        if ((g->unk_03E == 0x61) && (g->unk_F64[1] & 1)) {
            g->unk_120[0xB] = 1;
        }
        break;
    }
    switch (g->unk_03F) {
    case 5:
        if ((g->unk_03E == 0x60) && !((CharFlags *)g->unk_F64)->b4) {
            g->unk_120[0xD] = 0x80;
        }
        if ((g->unk_03E == 0x61) && !(g->unk_F64[1] & 1)) {
            g->unk_120[0xB] = 0x80;
            return;
        }
        return;
    case 8:
        if ((g->unk_03E == 0x5F) && !((CharFlags *)g->unk_F64)->b1) {
            g->unk_120[1] = 0x80;
        }
        if ((g->unk_03E == 0x60) && !((CharFlags *)g->unk_F64)->b5) {
            g->unk_120[5] = 0x80;
        }
        if (g->unk_03E == 0x61) {
            return;
        }
        break;
    case 11:
        g->unk_F64[1] &= 0xFF7F;
        if ((g->unk_03E == 0x5F) && !((CharFlags *)g->unk_F64)->b2) {
            g->unk_120[7] = 0x80;
        }
        if ((g->unk_03E == 0x60) && !((CharFlags *)g->unk_F64)->b6) {
            g->unk_120[0xA] = 0x80;
        }
        if (g->unk_03E == 0x61) {
            return;
        }
        break;
    case 2:
        if ((g->unk_03E == 0x60) && !((CharFlags *)g->unk_F64)->b3) {
            g->unk_120[3] = 0x80;
        }
        if ((g->unk_03E == 0x61) && !((CharFlags *)g->unk_F64)->b7) {
            g->unk_120[6] = 0x80;
        }
        break;
    }
}
