/* T-22019 cleaned draft of ETC:func_8013CCE4 (43 rows off, not guarded: over about 30 rows).
 * Frame (0x70) and the case 2 table loop match; open: the switch compares sit in $v1 in the original
 * (selector copy `or v0,v1,zero` stays dead) where this draft compares $v0, and the promoted
 * D_80150068 sits in $a0 (here $v0); the case 2 byte stores use a 0x80 value kept in $a0 with an
 * `andi v0,a0,0xFF` for the E0/E1 chain. */
s32 func_8013CCE4(void) {
    s32 pad; /* FAKE: unused slot, the original frame has it (real source unknown). T-22019 */
    s32 buf[6];
    u32 i;

    switch (D_800E6280.unk_110D) {
    case 0:
        if (D_80150068 > 0) {
            cd_read_sectors(0x69U, 0x801A0000, 0xB389);
            D_800E6280.unk_110D += 1;
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 1:
        if (D_80150068 == 0) {
            D_800E6280.unk_110D += 1;
        } else if (D_80150068 > 0) {
            if (cd_read_sync_status() & 1) {
                func_800AE0A0((void *)0x801E2000, 0x801A0600, 0x18000);
                if (D_80150068 == 1) {
                    D_80150068 = 2;
                }
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 2:
        if (D_800E6280.unk_F88 & 0x20) {
            buf[0] = 0x801E2000;
            buf[1] = 0x801E6000;
            for (i = 2; i < 6; i++) {
                buf[i] = i * 0x4000 + 0x801E2000;
            }
            func_8004A1A0(buf, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 5);
            D_801220D0 = 0x01000000;
            D_801220E1 = 0x80;
            D_801220E0 = 0x80;
            D_800E6280.unk_03A = 0x80;
            D_800E6280.unk_110D += 1;
            D_801220E2 = 0x80;
        }
        break;
    case 3:
        if (D_801500D4 >= 0xC00) {
            func_80042940(0);
        } else {
            D_801500D4 += 8;
            func_8013FA7C();
        }
        break;
    }
}
