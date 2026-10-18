/* T-23003 hand draft of main:func_8006D138 (920 B): gs-> bit-field form (Rec38Flags view of the word at +0xF64). Structure
 * differs from the original by the load of that word, which the original makes before the switch dispatch (lw v0,0xF64(a0)
 * with a0 = &D_800E6280 live to the end); no source form tried reproduces it. Not built (src keeps INCLUDE_ASM). */
void func_8006D138(void) {
    GameState *gs = &D_800E6280;

    switch (D_800E6280.unk_03F) {
    case 6:
    case 7:
        if (D_800E6280.unk_03E == 0x5F) {
            if (((Rec38Flags *)gs->unk_F64)->f.b1) {
                D_800E6280.unk_120[1] = 1;
            }
            D_800E6280.unk_120[0xE] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && (((Rec38Flags *)gs->unk_F64)->f.b5)) {
            D_800E6280.unk_120[5] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 9:
    case 10:
        if ((D_800E6280.unk_03E == 0x5F) && (((Rec38Flags *)gs->unk_F64)->f.b2)) {
            D_800E6280.unk_120[7] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && (((Rec38Flags *)gs->unk_F64)->f.b6)) {
            D_800E6280.unk_120[0xA] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 1:
        if ((D_800E6280.unk_03E == 0x60) && (((Rec38Flags *)gs->unk_F64)->f.b3)) {
            D_800E6280.unk_120[3] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {
            if ((s8) D_800E6280.unk_F64[0] < 0) {
                D_800E6280.unk_120[6] = 1;
            }
        }
        if (D_800E6280.unk_03E == 0x62) {

        }
        break;
    case 12:
        if ((D_800E6280.unk_03E == 0x5F) && (((Rec38Flags *)gs->unk_F64)->f.b3)) {
            D_800E6280.unk_120[3] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && ((s8) D_800E6280.unk_F64[0] < 0)) {
            D_800E6280.unk_120[6] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 3:
    case 4:
        if ((D_800E6280.unk_03E == 0x60) && (((Rec38Flags *)gs->unk_F64)->f.b4)) {
            D_800E6280.unk_120[0xD] = 1;
        }
        if ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_F64[1] & 1)) {
            D_800E6280.unk_120[0xB] = 1;
        }
        break;
    }
    switch (D_800E6280.unk_03F) {
    case 5:
        if ((D_800E6280.unk_03E == 0x60) && !(((Rec38Flags *)gs->unk_F64)->f.b4)) {
            D_800E6280.unk_120[0xD] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x61) && !(D_800E6280.unk_F64[1] & 1)) {
            D_800E6280.unk_120[0xB] = 0x80;
            return;
        }
        return;
    case 8:
        if ((D_800E6280.unk_03E == 0x5F) && !(((Rec38Flags *)gs->unk_F64)->f.b1)) {
            D_800E6280.unk_120[1] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x60) && !(((Rec38Flags *)gs->unk_F64)->f.b5)) {
            D_800E6280.unk_120[5] = 0x80;
        }
        if (D_800E6280.unk_03E == 0x61) {
            return;
        }
        break;
    case 11:
        D_800E6280.unk_F64[1] &= 0xFF7F;
        if ((D_800E6280.unk_03E == 0x5F) && !(((Rec38Flags *)gs->unk_F64)->f.b2)) {
            D_800E6280.unk_120[7] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x60) && !(((Rec38Flags *)gs->unk_F64)->f.b6)) {
            D_800E6280.unk_120[0xA] = 0x80;
        }
        if (D_800E6280.unk_03E == 0x61) {
            return;
        }
        break;
    case 2:
        if ((D_800E6280.unk_03E == 0x60) && !(((Rec38Flags *)gs->unk_F64)->f.b3)) {
            D_800E6280.unk_120[3] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x61) && ((s8) D_800E6280.unk_F64[0] >= 0)) {
            D_800E6280.unk_120[6] = 0x80;
        }
        break;
    }
}
