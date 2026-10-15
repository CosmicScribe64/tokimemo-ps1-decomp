void func_8013C978(void) {
    s16 temp_v0;
    s16 var_a0;
    s16 var_v0;
    s32 temp_t8;
    s32 var_a1;

    var_a1 = D_80149950;
    switch (var_a1) {
    case 0:
        temp_v0 = ((s16) D_80149986 >> 3) + 0x10;
        if (D_801206BE < temp_v0) {
            temp_t8 = (s16) *D_80149984 >> 3;
            if ((D_801206BA < (temp_t8 + 0x50)) && ((temp_t8 - 0x50) < D_801206BA)) {
                D_801206BE = temp_v0;
            }
        }
        if (D_801207CA < -0x1C0) {
            if (D_80149960 == 0) {
                D_8014999C = 0;
                D_80149950 = var_a1 + 1;
            }
        }
        break;
    case 1:
        D_80149950 = var_a1 + 1;
        break;
    case 2:
        D_80149950 = var_a1 + 1;
        break;
    case 3:
        if (D_80149990.unk_00 == 1) {
            var_a0 = 0x90;
        } else {
            D_80120697 |= 1;
            var_a0 = -0x90;
        }
        D_801206BA = var_a0;
        D_801206BE = 0x20;
        D_80149950 = var_a1 + 1;
        break;
    case 4:
        if ((D_80149990.unk_00 == 1) && (D_8014994C >= 4)) {
            D_801206BE = (s16) ((s16) D_80149986 >> 3);
            D_801206BA = ((s16) *D_80149984 >> 3) + 0x60;
            if (D_8014997C <= 0) {
                D_8014999C = 0;
                D_80149974.unk_00 = 0;
                var_a1 += 1;
            }
        }
        D_80149950 = var_a1;
        if (D_80149990.unk_00 == 2) {
            D_80149950 = var_a1;
            if (D_8014994C >= 4) {
                D_801206BE = (s16) ((s16) D_80149986 >> 3);
                D_801206BA = ((s16) *D_80149984 >> 3) - 0x60;
                D_80149950 = var_a1;
                if (D_8014997C >= 0) {
                    D_8014999C = 0;
                    D_80120697 = 0xA4;
                    D_80149974.unk_00 = 0;
                    D_801206BA = -0x50;
                    D_80149950 = var_a1 + 1;
                }
            }
        }
        break;
    case 5:
        if (D_801206BE < 0x30) {
            D_801206BE += 2;
        } else {
            D_801206BE = 0x30;
            if (D_80149990.unk_00 == 2) {
                D_80120697 |= 1;
            }
            D_8014999C = 0;
            D_80149950 = var_a1 + 1;
        }
        break;
    case 6:
        if ((((s32) D_80122EC4 % 3) == 0) || (var_v0 = 0, (D_80149940 == 2))) {
            var_v0 = -0x20;
        }
        if (D_80149990.unk_00 == 1) {
            if (var_v0 < D_801206BA) {
                D_801206BA -= 2;
            } else {
                D_801206BA = var_v0;
                D_80149950 = var_a1 + 1;
            }
        } else if (D_801206BA < var_v0) {
            D_801206BA += 2;
        } else {
            D_801206BA = var_v0;
            D_80149950 = var_a1 + 1;
        }
        break;
    default:
        func_80046500();
        break;
    }
}
