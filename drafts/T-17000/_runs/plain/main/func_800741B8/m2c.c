extern ? D_800B67AC;
extern u8 D_800E6375;
extern u16 D_800E6376;
extern ? D_800E691D;
extern u8 D_800E699C;
extern s32 D_800E71E8;
extern u8 D_800E738D;

u8 func_800741B8(void) {
    u8 var_v0;

    var_v0 = D_800E738D;
    if (var_v0 == 0) {
        func_80073CB0();
        if (((D_800E6375 & 0xF) == 3) && !(D_800E6376 & 1) && ((D_800E71E8 & 0xF) == 7)) {
            cd_read_sectors(9U, 0x801E0000, 0x7D8A);
            var_v0 = 1;
            D_800E738D = 1;
        } else if (*(&D_800E691D + (D_800E699C * 4)) != 4) {
            cd_read_sectors(0x29U, 0x801E0000, *(&D_800B67AC + (D_801230D0 * 4)));
            var_v0 = 1;
            D_800E738D = 1;
        } else {
            cd_read_sectors(0x29U, 0x801E0000, D_800B6810[D_801230D0]);
            var_v0 = 1;
            D_800E738D = 1;
        }
    } else if (var_v0 == 1) {
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            if ((D_800E6375 & 0xF) == 3) {
                if (!(D_800E6376 & 1)) {
                    if ((D_800E71E8 & 0xF) == 7) {
                        if (file_checksum_ok(9, 0x801E0000) != 0) {
                            var_v0 = func_8004284C();
                        } else {
                            var_v0 = 0;
                            goto block_20;
                        }
                    } else {
                        goto block_16;
                    }
                } else {
                    goto block_17;
                }
            } else {
block_16:
block_17:
                if (file_checksum_ok(0x29, 0x801E0000) != 0) {
                    var_v0 = func_8004284C();
                } else {
                    var_v0 = 0;
block_20:
                    D_800E738D = 0;
                }
            }
        }
    }
    return var_v0;
}
