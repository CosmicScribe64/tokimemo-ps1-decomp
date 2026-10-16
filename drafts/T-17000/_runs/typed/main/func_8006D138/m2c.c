void func_8006D138(void) {
    switch (D_800E6280.unk_03F) {                   /* switch 1 */
    case 6:                                         /* switch 1 */
    case 7:                                         /* switch 1 */
        if (D_800E6280.unk_03E == 0x5F) {
            if (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 2) {
                D_800E6280.unk_120[1] = 1;
            }
            D_800E6280.unk_120[0xE] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x20)) {
            D_800E6280.unk_120[5] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 9:                                         /* switch 1 */
    case 10:                                        /* switch 1 */
        if ((D_800E6280.unk_03E == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 4)) {
            D_800E6280.unk_120[7] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x40)) {
            D_800E6280.unk_120[0xA] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 1:                                         /* switch 1 */
        if ((D_800E6280.unk_03E == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
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
    case 12:                                        /* switch 1 */
        if ((D_800E6280.unk_03E == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E6280.unk_120[3] = 1;
        }
        if ((D_800E6280.unk_03E == 0x60) && ((s8) D_800E6280.unk_F64[0] < 0)) {
            D_800E6280.unk_120[6] = 1;
        }
        if (D_800E6280.unk_03E == 0x61) {

        }
        break;
    case 3:                                         /* switch 1 */
    case 4:                                         /* switch 1 */
        if ((D_800E6280.unk_03E == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E6280.unk_120[0xD] = 1;
        }
        if ((D_800E6280.unk_03E == 0x61) && (D_800E6280.unk_F64[1] & 1)) {
            D_800E6280.unk_120[0xB] = 1;
        }
        break;
    }
    switch (D_800E6280.unk_03F) {                   /* switch 2; irregular */
    case 5:                                         /* switch 2 */
        if ((D_800E6280.unk_03E == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E6280.unk_120[0xD] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x61) && !(D_800E6280.unk_F64[1] & 1)) {
            D_800E6280.unk_120[0xB] = 0x80;
            return;
        }
        return;
    case 8:                                         /* switch 2 */
        if ((D_800E6280.unk_03E == 0x5F) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 2)) {
            D_800E6280.unk_120[1] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x20)) {
            D_800E6280.unk_120[5] = 0x80;
        }
        if (D_800E6280.unk_03E == 0x61) {
            return;
        }
        break;
    case 11:                                        /* switch 2 */
        D_800E6280.unk_F64[1] &= 0xFF7F;
        if ((D_800E6280.unk_03E == 0x5F) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 4)) {
            D_800E6280.unk_120[7] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x40)) {
            D_800E6280.unk_120[0xA] = 0x80;
        }
        if (D_800E6280.unk_03E == 0x61) {
            return;
        }
        break;
    case 2:                                         /* switch 2 */
        if ((D_800E6280.unk_03E == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E6280.unk_120[3] = 0x80;
        }
        if ((D_800E6280.unk_03E == 0x61) && ((s8) D_800E6280.unk_F64[0] >= 0)) {
            D_800E6280.unk_120[6] = 0x80;
        }
        break;
    }
}
