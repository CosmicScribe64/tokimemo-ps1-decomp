extern ? D_800B68C0;
extern ? D_800B68C2;
extern ? D_800B68C4;
extern ? D_800B68C6;
extern ? D_800B68C9;
extern ? D_800B68CB;
extern u8 D_800B68CD;
extern ? D_800B68D0;
extern ? D_800B68D2;
extern ? D_800B68D4;
extern ? D_800B68D6;
extern ? D_800B68D9;
extern ? D_800B68DB;
extern u8 D_800B68DD;

void func_80073CB0(void) {
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;
    u8 temp_t3;
    u8 temp_t5;
    u8 var_t0;

    D_801230D0 = 0xFF;
    temp_a0 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_03;
    if (temp_a0 != 0) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if ((D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 3) && (temp_a0 == 5)) {
        var_v1 = 2;
    }
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        temp_t3 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
        switch (temp_t3) {                          /* switch 1 */
        case 0:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68C0 + var_v1);
            break;
        case 1:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68C2 + var_v1);
            break;
        case 2:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68C4 + var_v1);
            break;
        case 3:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68C6 + var_v1);
            break;
        case 4:                                     /* switch 1 */
            D_801230D0 = 0xFF;
            break;
        case 5:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68C9 + var_v1);
            break;
        case 6:                                     /* switch 1 */
            D_801230D0 = (s32) *(&D_800B68CB + var_v1);
            break;
        case 7:                                     /* switch 1 */
            D_801230D0 = (s32) D_800B68CD;
            break;
        }
    } else {
        temp_t5 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
        switch (temp_t5) {                          /* switch 2 */
        case 0:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68D0 + var_v1);
            break;
        case 1:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68D2 + var_v1);
            break;
        case 2:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68D4 + var_v1);
            break;
        case 3:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68D6 + var_v1);
            break;
        case 4:                                     /* switch 2 */
            D_801230D0 = 0xFF;
            break;
        case 5:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68D9 + var_v1);
            break;
        case 6:                                     /* switch 2 */
            D_801230D0 = (s32) *(&D_800B68DB + var_v1);
            break;
        case 7:                                     /* switch 2 */
            D_801230D0 = (s32) D_800B68DD;
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t6) {                      /* switch 3 */
            case 0:                                 /* switch 3 */
                D_801230D0 = (s32) (*D_800B6890)[var_v1];
                break;
            case 1:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[1][var_v1];
                break;
            case 2:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[2][var_v1];
                break;
            case 3:                                 /* switch 3 */
                var_t0 = D_800B6890[3][var_v1];
                goto block_55;
            case 4:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[4][var_v1];
                break;
            case 5:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[5][var_v1];
                break;
            case 6:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[6][var_v1];
                break;
            case 7:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[7][var_v1];
                break;
            case 8:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[8][var_v1];
                break;
            case 9:                                 /* switch 3 */
                D_801230D0 = (s32) D_800B6890[9][var_v1];
                break;
            case 10:                                /* switch 3 */
                D_801230D0 = (s32) D_800B6890[0xA][var_v1];
                break;
            }
        } else {
            temp_t9 = (u16) D_800E6280.unk_000 >> 0xC;
            switch (temp_t9) {                      /* switch 4 */
            case 0:                                 /* switch 4 */
                var_t0 = (*D_800B68A8)[var_v1];
block_55:
                D_801230D0 = (s32) var_t0;
                break;
            case 1:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[1][var_v1];
                break;
            case 2:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[2][var_v1];
                break;
            case 3:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[3][var_v1];
                break;
            case 4:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[4][var_v1];
                break;
            case 5:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[5][var_v1];
                break;
            case 6:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[6][var_v1];
                break;
            case 7:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[7][var_v1];
                break;
            case 8:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[8][var_v1];
                break;
            case 9:                                 /* switch 4 */
                D_801230D0 = (s32) D_800B68A8[9][var_v1];
                break;
            case 10:                                /* switch 4 */
                var_t0 = D_800B68A8[0xA][var_v1];
                goto block_55;
            }
        }
    }
}
