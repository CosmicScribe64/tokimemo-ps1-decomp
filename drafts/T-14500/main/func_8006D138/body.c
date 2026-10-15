void func_8006D138(void) {
    switch (D_800E62BF) {
    case 6:
    case 7:
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
    case 9:
    case 10:
        if ((D_800E62BE == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 4)) {
            D_800E63A7 = 1;
        }
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x40)) {
            D_800E63AA = 1;
        }
        if (D_800E62BE == 0x61) {

        }
        break;
    case 1:
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
    case 12:
        if ((D_800E62BE == 0x5F) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E63A3 = 1;
        }
        if ((D_800E62BE == 0x60) && ((s8) D_800E6280.unk_F64[0] < 0)) {
            D_800E63A6 = 1;
        }
        if (D_800E62BE == 0x61) {

        }
        break;
    case 3:
    case 4:
        if ((D_800E62BE == 0x60) && (*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E63AD = 1;
        }
        if ((D_800E62BE == 0x61) && (D_800E6280.unk_F64[1] & 1)) {
            D_800E63AB = 1;
        }
        break;
    }
    switch (D_800E62BF) {
    case 5:
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 0x10)) {
            D_800E63AD = 0x80;
        }
        if ((D_800E62BE == 0x61) && !(D_800E6280.unk_F64[1] & 1)) {
            D_800E63AB = 0x80;
            return;
        }
        return;
    case 8:
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
    case 11:
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
    case 2:
        if ((D_800E62BE == 0x60) && !(*(s32 *)((unsigned char *)(&D_800E6280) + (3940)) & 8)) {
            D_800E63A3 = 0x80;
        }
        if ((D_800E62BE == 0x61) && ((s8) D_800E6280.unk_F64[0] >= 0)) {
            D_800E63A6 = 0x80;
        }
        break;
    }
}
