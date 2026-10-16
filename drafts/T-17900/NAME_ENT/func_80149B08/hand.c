s32 func_80149B08(void) {
    /* FAKE: unused homes preserve the original frame and mean spill slot (as func_801486F8). T-17900. */
    s32 unused;
    s32 padding[2];
    s32 level;
    s32 frame;
    s32 average;

    /* FAKE: dead initialization changes allocator range order (as func_801486F8). T-17900. */
    average = frame = 0;
    if (func_800460EC() & 4) {
        level = getCDlevel();
    } else {
        level = 0;
    }
    D_8014D1B4[D_800E6280.unk_10F8 & 3] = level;
    average = (D_8014D1B4[0] + D_8014D1B4[1] + D_8014D1B4[2] + D_8014D1B4[3]) / 4;
    frame = average / 10;
    D_8014D1B0 = level;
    if (frame >= 4) {
        frame = 3;
    }
    func_80048F64(0x61);
    if (func_800460EC() & 4) {
        D_80120695 = 9;
        D_80120696 = 0x40;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x80180258;
        D_801206A4 = 0x80180298;
        D_801206C8 = 0x8018024C;
        D_801206AC = frame + 6;
        D_80120699 = 0x80;
        D_801206BA = D_80120676;
        D_801206BE = -0x78;
        D_801206D9 = 9;
        D_801206DA = 0x40;
        D_80120710 = 0x41000000;
        D_801206DB = 0xA4;
        D_801206E4 = 0x80180258;
        D_801206E8 = 0x80180298;
        D_8012070C = 0x8018024C;
        D_801206F0 = frame + 10;
        D_801206DD = 0x80;
        D_801206FE = D_80120676;
        D_80120702 = -0x78;
        D_80120762 = 0;
    }
}
