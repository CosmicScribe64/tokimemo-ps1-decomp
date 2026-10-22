extern u8 D_800B68C0[16];
extern u8 D_800B68D0[16];
void func_80073CB0(void) {
    s32 sp28;
    s32 var_v1;
    u32 temp_t6;
    u32 temp_t9;
    u8 temp_a0;

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
    if (temp_a0) {}
    sp28 = var_v1;
    if (func_80066A84() == 1) {
        switch (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
        case 0:
            D_801230D0 = (s32) D_800B68C0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68C0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68C0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68C0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68C0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68C0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68C0[13];
            break;
        }
    } else {
        switch (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
        case 0:
            D_801230D0 = (s32) D_800B68D0[0 + var_v1];
            break;
        case 1:
            D_801230D0 = (s32) D_800B68D0[2 + var_v1];
            break;
        case 2:
            D_801230D0 = (s32) D_800B68D0[4 + var_v1];
            break;
        case 3:
            D_801230D0 = (s32) D_800B68D0[6 + var_v1];
            break;
        case 4:
            D_801230D0 = 0xFF;
            break;
        case 5:
            D_801230D0 = (s32) D_800B68D0[9 + var_v1];
            break;
        case 6:
            D_801230D0 = (s32) D_800B68D0[11 + var_v1];
            break;
        case 7:
            D_801230D0 = (s32) D_800B68D0[13];
            break;
        }
    }
    if (D_801230D0 == 0xFF) {
        sp28 = var_v1;
        if (func_80066A84() == 1) {
            temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t6) {
            case 0:
                D_801230D0 = (s32) (*D_800B6890)[var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B6890[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B6890[2][var_v1];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B6890[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B6890[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B6890[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B6890[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B6890[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B6890[9][var_v1];
                break;
            case 10:
                D_801230D0 = (s32) D_800B6890[0xA][var_v1];
                break;
            }
        } else {
            temp_t9 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            switch (temp_t9) {
            case 0:
                D_801230D0 = D_800B68A8[0][var_v1];
                break;
            case 1:
                D_801230D0 = (s32) D_800B68A8[1][var_v1];
                break;
            case 2:
                D_801230D0 = (s32) D_800B68A8[2][var_v1];
                break;
            case 3:
                D_801230D0 = (s32) D_800B68A8[3][var_v1];
                break;
            case 4:
                D_801230D0 = (s32) D_800B68A8[4][var_v1];
                break;
            case 5:
                D_801230D0 = (s32) D_800B68A8[5][var_v1];
                break;
            case 6:
                D_801230D0 = (s32) D_800B68A8[6][var_v1];
                break;
            case 7:
                D_801230D0 = (s32) D_800B68A8[7][var_v1];
                break;
            case 8:
                D_801230D0 = (s32) D_800B68A8[8][var_v1];
                break;
            case 9:
                D_801230D0 = (s32) D_800B68A8[9][var_v1];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][var_v1];
                break;
            }
        }
    }
}
