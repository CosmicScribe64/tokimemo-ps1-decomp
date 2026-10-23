void func_8006D138(void) {
    GameState *gs = &D_800E6280;
    switch (gs->unk_03F) { default: break; }
    if (*(u32 *)gs->unk_F64 | (u32)gs) {}

    switch (gs->unk_03F) {
    case 6:
    case 7:
        if (gs->unk_03E == 0x5F) {
            if (((CharFlags *)gs->unk_F64)->b1) {
                gs->unk_120[1] = 1;
            }
            gs->unk_120[0xE] = 1;
        }
        if ((gs->unk_03E == 0x60) && (((CharFlags *)gs->unk_F64)->b5)) {
            gs->unk_120[5] = 1;
        }
        if (gs->unk_03E == 0x61) {

        }
        break;
    case 9:
    case 10:
        if ((gs->unk_03E == 0x5F) && (((CharFlags *)gs->unk_F64)->b2)) {
            gs->unk_120[7] = 1;
        }
        if ((gs->unk_03E == 0x60) && (((CharFlags *)gs->unk_F64)->b6)) {
            gs->unk_120[0xA] = 1;
        }
        if (gs->unk_03E == 0x61) {

        }
        break;
    case 1:
        if ((gs->unk_03E == 0x60) && (((CharFlags *)gs->unk_F64)->b3)) {
            gs->unk_120[3] = 1;
        }
        if (gs->unk_03E == 0x61) {
            if (((CharFlags *)gs->unk_F64)->b7) {
                gs->unk_120[6] = 1;
            }
        }
        if (gs->unk_03E == 0x62 + ((u32)gs->unk_03F < 0)) {

        }
        break;
    case 12:
        if ((gs->unk_03E == 0x5F) && (((CharFlags *)gs->unk_F64)->b3)) {
            gs->unk_120[3] = 1;
        }
        if ((gs->unk_03E == 0x60) && (((CharFlags *)gs->unk_F64)->b7)) {
            gs->unk_120[6] = 1;
        }
        if (gs->unk_03E == 0x61) {

        }
        break;
    case 3:
    case 4:
        if ((gs->unk_03E == 0x60) && (((CharFlags *)gs->unk_F64)->b4)) {
            gs->unk_120[0xD] = 1;
        }
        if ((gs->unk_03E == 0x61) && (gs->unk_F64[1] & 1)) {
            gs->unk_120[0xB] = 1;
        }
        break;
    }
    switch (gs->unk_03F) {
    case 5:
        if ((gs->unk_03E == 0x60) && !(((CharFlags *)gs->unk_F64)->b4)) {
            gs->unk_120[0xD] = 0x80;
        }
        if ((gs->unk_03E == 0x61) && !(gs->unk_F64[1] & 1)) {
            gs->unk_120[0xB] = 0x80;
            return;
        }
        return;
    case 8:
        if ((gs->unk_03E == 0x5F) && !(((CharFlags *)gs->unk_F64)->b1)) {
            gs->unk_120[1] = 0x80;
        }
        if ((gs->unk_03E == 0x60) && !(((CharFlags *)gs->unk_F64)->b5)) {
            gs->unk_120[5] = 0x80;
        }
        if (gs->unk_03E == 0x61) {
            return;
        }
        break;
    case 11:
        gs->unk_F64[1] &= 0xFF7F;
        if ((gs->unk_03E == 0x5F) && !(((CharFlags *)gs->unk_F64)->b2)) {
            gs->unk_120[7] = 0x80;
        }
        if ((gs->unk_03E == 0x60) && !(((CharFlags *)gs->unk_F64)->b6)) {
            gs->unk_120[0xA] = 0x80;
        }
        if (gs->unk_03E == 0x61) {
            return;
        }
        break;
    case 2:
        if ((gs->unk_03E == 0x60) && !(((CharFlags *)gs->unk_F64)->b3)) {
            gs->unk_120[3] = 0x80;
        }
        if ((gs->unk_03E == 0x61) && (!((CharFlags *)gs->unk_F64)->b7)) {
            gs->unk_120[6] = 0x80;
        }
        break;
    }
}
