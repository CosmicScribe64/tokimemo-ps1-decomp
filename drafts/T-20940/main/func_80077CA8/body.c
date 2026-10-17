u8 func_80077CA8(void) {
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_03E;
    switch (temp_v1) {
    default:
        return temp_v1;
    case 0x5F:
        temp_v0 = D_800E6280.unk_03F;
        if (temp_v0 < 5U) {
            return 0U;
        }
        if (temp_v0 < 8U) {
            return 1U;
        }
        if (temp_v0 < 0xBU) {
            return 2U;
        }
        return 3U;
    case 0x60:
        temp_v0_2 = D_800E6280.unk_03F;
        if (temp_v0_2 < 2U) {
            return 3U;
        }
        if (temp_v0_2 < 5U) {
            return 4U;
        }
        if (temp_v0_2 < 8U) {
            return 5U;
        }
        if (temp_v0_2 < 0xBU) {
            return 6U;
        }
        return 7U;
    case 0x61:
        temp_v0_3 = D_800E6280.unk_03F;
        if (temp_v0_3 == 8) {
            return 0xDU;
        }
        if (temp_v0_3 < 2U) {
            return 7U;
        }
        if (temp_v0_3 < 5U) {
            return 8U;
        }
        if (temp_v0_3 < 8U) {
            return 9U;
        }
        if (temp_v0_3 < 0xBU) {
            return 0xAU;
        }
        return 0xBU;
    case 0x62:
        if ((u8) D_800E6280.unk_03F < 2U) {
            return 0xBU;
        }
        return 0xCU;
    }
}
