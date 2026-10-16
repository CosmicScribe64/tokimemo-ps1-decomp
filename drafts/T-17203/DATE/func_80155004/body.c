/* T-17203 draft of DATE func_80155004 (80154260.c); 46 differing lines against the original, all of
 * them register names (23 instructions): the instruction count (91), the branch layout (the original's
 * dead "li a3,0x80" after the "b"), the frame, the table copy, the loop and the stores match.
 * The colour byte is the global D_800B593C itself (uopt promotes it to one register and writes it back
 * after the loop, as the original does), not a local. Left: IDO colours i $v1, 0x801217D0 $a0, 0x24 $a1,
 * D_800B593C $a2; the original has i $a0, 0x801217D0 $a1, 0x24 $a2, D_800B593C $a3 and never uses $v1. */
void func_80155004(void) {
    s32 pad[2]; /* FAKE: eight unused bytes above tbl (frame 0x108) */
    FnTbl53 tbl;
    s16 i;

    tbl = D_8015F82C;
    tbl.f[D_800E6280.unk_110A](0x80);
    if (D_800E6280.unk_110A == 0xE) {
        if (D_800E6280.unk_F88 & 0xFF9FFF9F) {
            D_8015F5D0 = 1 - (u8) D_8015F5D0;
        }
        if ((u32) D_800E6280.unk_10F8 % 3U == 0) {
            if ((u8) D_8015F5D0 != 0) {
                D_800B593C = 0x80;
            } else if (D_800B593C == 0x80) {
                D_800B593C = 0x60;
            } else {
                D_800B593C = 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[96].unk_07 = D_800B593C;
            ((DisplayRecord44 *)D_8011ECD0)[97].unk_07 = D_800B593C;
            i = 0x3A;
            do {
                D_801217D0[i].unk_16 = D_800B593C;
                D_801217D0[i].unk_15 = D_800B593C;
                D_801217D0[i].unk_14 = D_800B593C;
                i += 1;
            } while (i < 0x40);
        }
    }
}
