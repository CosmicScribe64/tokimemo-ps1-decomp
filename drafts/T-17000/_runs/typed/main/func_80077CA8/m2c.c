u8 func_80077CA8(void) {
    switch (D_800E6280.unk_03E) {                   /* irregular */
    default:
        return D_800E6280.unk_03E;
    case 0x5F:
        if ((u8) D_800E6280.unk_03F < 5U) {
            return 0U;
        }
        if ((u8) D_800E6280.unk_03F < 8U) {
            return 1U;
        }
        if ((u8) D_800E6280.unk_03F < 0xBU) {
            return 2U;
        }
        return 3U;
    case 0x60:
        if ((u8) D_800E6280.unk_03F < 2U) {
            return 3U;
        }
        if ((u8) D_800E6280.unk_03F < 5U) {
            return 4U;
        }
        if ((u8) D_800E6280.unk_03F < 8U) {
            return 5U;
        }
        if ((u8) D_800E6280.unk_03F < 0xBU) {
            return 6U;
        }
        return 7U;
    case 0x61:
        if (D_800E6280.unk_03F == 8) {
            return 0xDU;
        }
        if ((u8) D_800E6280.unk_03F < 2U) {
            return 7U;
        }
        if ((u8) D_800E6280.unk_03F < 5U) {
            return 8U;
        }
        if ((u8) D_800E6280.unk_03F < 8U) {
            return 9U;
        }
        if ((u8) D_800E6280.unk_03F < 0xBU) {
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
