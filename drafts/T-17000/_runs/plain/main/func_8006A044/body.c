void func_8006A044(s32 arg0, s32 arg1) {
    u8 var_v0;

    if ((D_800E62BF != 2) && (D_800E62BF != 5) && (D_800E62BF != 8) && (D_800E62BF != 0xB)) {
        var_v0 = (*(u8 *)((unsigned char *)(&D_800E71E4) + (1)) | 0x40) & 0xFF;
    } else {
        var_v0 = (*(u8 *)((unsigned char *)(&D_800E71E4) + (1)) | 0x20) & 0xFF;
    }
    if ((D_800E62BE == 0x5F) && (D_800E62BF == 4)) {
        var_v0 = (var_v0 | 0x60) & 0xFF;
    }
    *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = var_v0;
    switch (D_800E62BF) {
    case 2:
    case 3:
    case 4:
        if (D_800E62BE == 0x5F) {
            D_800E71E4 = (u8) (D_800E71E4 | 1);
        }
        if (D_800E62BE == 0x60) {
            D_800E71E4 = (u8) (D_800E71E4 | 0x10);
        }
        *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = var_v0;
        if (D_800E62BE == 0x61) {
            *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = (u8) ((var_v0 | 1) & 0xFF);
            return;
        }
    default:
        return;
    case 5:
    case 6:
    case 7:
        if (D_800E62BE == 0x5F) {
            D_800E71E4 = (u8) (D_800E71E4 | 2);
        }
        if (D_800E62BE == 0x60) {
            D_800E71E4 = (u8) (D_800E71E4 | 0x20);
        }
        if (D_800E62BE == 0x61) {
            var_v0 = (var_v0 | 2) & 0xFF;
        }
        *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = (u8) ((var_v0 | 0x80) & 0xFF);
        return;
    case 8:
    case 9:
    case 10:
        if (D_800E62BE == 0x5F) {
            D_800E71E4 = (u8) (D_800E71E4 | 4);
        }
        if (D_800E62BE == 0x60) {
            D_800E71E4 = (u8) (D_800E71E4 | 0x40);
        }
        *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = var_v0;
        if (D_800E62BE == 0x61) {
            *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = (u8) ((var_v0 | 4) & 0xFF);
            return;
        }
        break;
    case 11:
    case 12:
    case 13:
        if (D_800E62BE == 0x5F) {
            D_800E71E4 = (u8) (D_800E71E4 | 8);
        }
        if (D_800E62BE == 0x60) {
            D_800E71E4 = (u8) (D_800E71E4 | 0x80);
        }
        *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = var_v0;
        if (D_800E62BE == 0x61) {
            *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = (u8) ((var_v0 | 8) & 0xFF);
            return;
        }
        break;
    case 1:
        if (D_800E62BE == 0x60) {
            D_800E71E4 = (u8) (D_800E71E4 | 8);
        }
        if (D_800E62BE == 0x61) {
            D_800E71E4 = (u8) (D_800E71E4 | 0x80);
        }
        *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = var_v0;
        if (D_800E62BE == 0x62) {
            *(u8 *)((unsigned char *)(&D_800E71E4) + (1)) = (u8) ((var_v0 | 8) & 0xFF);
        }
        break;
    }
}
