s32 func_8013B448(void) {
    u32 temp_t6;
    u8 temp_t8;
    u8 *var_t6;

    D_800E6280.unk_75E[D_800E71DE].unk_00 = D_800E71DF;
    D_800E6280.unk_75E[D_800E71DE].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E71DE].unk_04 = D_800E62BE;
    D_800E6280.unk_75E[D_800E71DE].unk_05 = D_800E62BF;
    D_800E6280.unk_75E[D_800E71DE].unk_06 = D_800E62C0;
    D_800E6280.unk_75E[D_800E71DE].unk_07 = D_800E62C1;
    temp_t6 = (u16) D_800E6374 >> 0xC;
    switch (temp_t6) {
    case 7:
        D_800E6280.unk_75E[D_800E71DE].unk_01 = 7;
        break;
    case 8:
        D_800E6280.unk_75E[D_800E71DE].unk_01 = 8;
        break;
    case 9:
        D_800E6280.unk_75E[D_800E71DE].unk_01 = 9;
        break;
    case 10:
        D_800E6280.unk_75E[D_800E71DE].unk_01 = 0xA;
        break;
    }
    switch (D_800E71DF) {
    case 0:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 1:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 2:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 3:
        var_t6 = (u8 *)&D_800E6280.unk_75E[D_800E71DE];
block_29:
        *(s8 *)((unsigned char *)var_t6 + (3)) = 1;
        break;
    case 4:
        if ((((u8) D_800E652A >> 4) == 7) && (((u16) D_800E6374 >> 0xC) == 7)) {
            D_800E6280.unk_75E[D_800E71DE].unk_03 = 5;
            D_800E6280.unk_75E[D_800E71DE].unk_07 = 0xFF;
        } else {
            D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        }
        break;
    case 5:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 6:
        if (((u16) D_800E6374 >> 0xC) == 9) {
            D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        } else {
            D_800E6280.unk_75E[D_800E71DE].unk_03 = 0;
        }
        break;
    case 7:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 0;
        break;
    case 8:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 9:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 5;
        D_800E6280.unk_75E[D_800E71DE].unk_07 = 0xFF;
        break;
    case 10:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 11:
        D_800E6280.unk_75E[D_800E71DE].unk_03 = 1;
        break;
    case 12:
        var_t6 = (u8 *)&D_800E6280.unk_75E[D_800E71DE];
        goto block_29;
    }
    temp_t8 = (D_800E71DE + 1) & 0xFF;
    D_800E71DE = temp_t8;
    if (temp_t8 >= 0xFFU) {
        D_800E71DE = 0xFE;
    }
}
