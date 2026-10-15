extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern s8 D_800E63A1;
extern s8 D_800E63A3;
extern s8 D_800E63A5;
extern s8 D_800E63A6;
extern s8 D_800E63A7;
extern s8 D_800E63AA;
extern s8 D_800E63AB;
extern s8 D_800E63AD;
extern s8 D_800E63AE;

void func_8006D138(void) {
    switch (D_800E62BF) {                           /* switch 1 */
    case 6:                                         /* switch 1 */
    case 7:                                         /* switch 1 */
        if (D_800E62BE == 0x5F) {
            if (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 2) {
                D_800E63A1 = 1;
            }
            D_800E63AE = 1;
        }
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x20)) {
            D_800E63A5 = 1;
        }
        if (D_800E62BE == 0x61) {

        }
        break;
    case 9:                                         /* switch 1 */
    case 10:                                        /* switch 1 */
        if ((D_800E62BE == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 4)) {
            D_800E63A7 = 1;
        }
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x40)) {
            D_800E63AA = 1;
        }
        if (D_800E62BE == 0x61) {

        }
        break;
    case 1:                                         /* switch 1 */
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E63A3 = 1;
        }
        if (D_800E62BE == 0x61) {
            if ((s8) D_800E6280.unk_F64[0] < 0) {
                D_800E63A6 = 1;
            }
        }
        if (D_800E62BE == 0x62) {

        }
        break;
    case 12:                                        /* switch 1 */
        if ((D_800E62BE == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E63A3 = 1;
        }
        if ((D_800E62BE == 0x60) && ((s8) D_800E6280.unk_F64[0] < 0)) {
            D_800E63A6 = 1;
        }
        if (D_800E62BE == 0x61) {

        }
        break;
    case 3:                                         /* switch 1 */
    case 4:                                         /* switch 1 */
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E63AD = 1;
        }
        if ((D_800E62BE == 0x61) && (D_800E6280.unk_F64[1] & 1)) {
            D_800E63AB = 1;
        }
        break;
    }
    switch (D_800E62BF) {                           /* switch 2; irregular */
    case 5:                                         /* switch 2 */
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E63AD = 0x80;
        }
        if ((D_800E62BE == 0x61) && !(D_800E6280.unk_F64[1] & 1)) {
            D_800E63AB = 0x80;
            return;
        }
        return;
    case 8:                                         /* switch 2 */
        if ((D_800E62BE == 0x5F) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 2)) {
            D_800E63A1 = 0x80;
        }
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x20)) {
            D_800E63A5 = 0x80;
        }
        if (D_800E62BE == 0x61) {
            return;
        }
        break;
    case 11:                                        /* switch 2 */
        D_800E6280.unk_F64[1] &= 0xFF7F;
        if ((D_800E62BE == 0x5F) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 4)) {
            D_800E63A7 = 0x80;
        }
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x40)) {
            D_800E63AA = 0x80;
        }
        if (D_800E62BE == 0x61) {
            return;
        }
        break;
    case 2:                                         /* switch 2 */
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E63A3 = 0x80;
        }
        if ((D_800E62BE == 0x61) && ((s8) D_800E6280.unk_F64[0] >= 0)) {
            D_800E63A6 = 0x80;
        }
        break;
    }
}
