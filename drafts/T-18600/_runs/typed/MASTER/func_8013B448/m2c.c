void func_8013B448(void) {
    u32 temp_t6;
    u8 temp_t5;
    u8 temp_t8;
    void *var_t6;

    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (temp_t6) {                              /* switch 1; irregular */
    case 7:                                         /* switch 1 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 7;
        break;
    case 8:                                         /* switch 1 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 8;
        break;
    case 9:                                         /* switch 1 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 9;
        break;
    case 10:                                        /* switch 1 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 0xA;
        break;
    }
    temp_t5 = D_800E6280.unk_F5F;
    switch (temp_t5) {                              /* switch 2 */
    case 0:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 1:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 2:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 3:                                         /* switch 2 */
        var_t6 = &D_800E6280 + (D_800E6280.unk_F5E * 8);
block_29:
        *(s8 *)((unsigned char *)var_t6 + (1889)) = 1;
        break;
    case 4:                                         /* switch 2 */
        if ((((u8) D_800E6280.unk_1BC[4].unk_0C.h[1] >> 4) == 7) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 7)) {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        } else {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        }
        break;
    case 5:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 6:                                         /* switch 2 */
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9) {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        } else {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        }
        break;
    case 7:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        break;
    case 8:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 9:                                         /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        break;
    case 10:                                        /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 11:                                        /* switch 2 */
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 12:                                        /* switch 2 */
        var_t6 = &D_800E6280 + (D_800E6280.unk_F5E * 8);
        goto block_29;
    }
    temp_t8 = (D_800E6280.unk_F5E + 1) & 0xFF;
    D_800E6280.unk_F5E = temp_t8;
    if (temp_t8 >= 0xFFU) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
