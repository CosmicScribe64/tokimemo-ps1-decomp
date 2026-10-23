/* T-25128 best measured body for ETC func_8013FFAC (not a match; 197 resolved diff rows).
 * Drop it into src/ovl/ETC/8013FF80.c as
 *     #ifdef NON_MATCHING <this file> #else INCLUDE_ASM(...) #endif
 * and change include/ovl/ETC.h to `s32 func_8013FFAC(void);` (the s32 view; with the
 * shared `void` declaration and a `void` definition the same body gives 206 rows).
 * Measure: tools/docker.sh python3 tools/funcdiff.py func_8013FFAC --unit ETC --resolve --version c
 * See NOTES.md next to this file for what each FAKE does and what is left. */
void func_80140AE4(void);
s32 func_8013FFAC(void) {
    s32 i;
    s32 sp54[6];
    u32 j;

    /* FAKE: dead two-case switch; its VREG selector makes cvt_pass.py emit OPTN 405 0
     * (zcopy off), which keeps the second table loop at a runtime index and its
     * constants in ugen temporaries, and promotes unk_F5F to $t0. T-25128 */
    switch (D_800E6280.unk_F5F) { case 0: case 9: break; }
    switch ((*(u8 *)&D_800E6280.unk_110D)) {
    case 0:
        draw2d3d(1, 0);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_038 = 0;
        func_80048DAC(1);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        tpage_buf_clear();
        D_801500C0 = 0;
        func_801408F0();
        D_801500C4 = D_8014E070[D_800E6280.unk_F5F % 13U];
        func_80044890(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        (*(u8 *)&D_800E6280.unk_110D) += 1;
    default:
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x201);
            func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_800462C8(1U, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69U, 0x80180000, 0xB389);
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 5:
        draw2d3d(1, 1);
        func_8013F980();
        func_80061710();
        func_80061790();
        func_800618B0();
        load_palette(0x80180000, 0x1F, 1, 3, 0);
        load_palette(0x80180200, 0x1E, 1, 3, 0);
        load_palette(0x80180400, 0x1D, 1, 3, 0);
        load_tpage_buf_lock(0xF, 0x801B0600, 1);
        for (i = 0; i < 6; i++) {
            sp54[i] = i * 0x4000 + 0x80198600;
        }
        i = 0; /* FAKE: dead reassignment keeps the first loop rolled (T-25128) */
        func_8004A1A0(sp54, 1, 6, 3, 2, 0x1FE, -0xA0, -0x7D, 0xC, 0x15);
        D_801220F4 = 0x01000000;
        D_80122106 = 0;
        D_80122105 = 0;
        D_80122104 = 0;
        D_800E6280.unk_03A = 0;
        /* FAKE: explicit peel plus unsigned 2..6 loop, the matched func_8013CCE4 recipe
         * (T-24706): the unsigned body sums do not CSE with the signed peel literals. T-25128 */
        sp54[0] = 0x80180600; sp54[1] = 0x80184600; for (j = 2; j < 6; j++) sp54[j] = 0x80180600 + j * 0x4000;

        func_8004A1A0(sp54, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        D_801220D0 = 0x80000000;
        (&D_801220E0)[0] = (&D_801220E1)[0] = (&D_801220E2)[0] = 0x80;
        D_800E6280.unk_03A = 0;
        func_800AE0A0((void *)0x801E2000, 0x80180600, 0x18000);
        /* FAKE: flat view of D_8014DB90 as in func_8013CF2C; the * 2 index spends the
         * ugen slot the original spends. T-25128 */
        func_80046290(((s32 *)D_8014DB90)[D_800E6280.unk_F5F * 2], ((s32 *)D_8014DB90)[D_800E6280.unk_F5F * 2 + 1], D_800E6280.unk_F5F);
        func_80044750(0x300);
        (*(u8 *)&D_800E6280.unk_110D) += 1;
        break;
    case 6:
        if (func_800460EC() & 4) {
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 7:
        func_80140AE4();
        if ((u8) D_800E6280.unk_03A < 0x81U) {
            D_80122106 = D_800E6280.unk_03A;
            D_80122105 = D_800E6280.unk_03A;
            D_80122104 = D_800E6280.unk_03A;
            D_800E6280.unk_03A += 1;
        }
        if (func_800460EC() & 2) {
            D_801500DC += 1;
            D_800B3CF8 = 0;
            D_800B3CFC = 0;
            D_800B3D00 = 0;
            D_800E6280.unk_1104.w = 0;
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 8:
        if ((u8) D_800E6280.unk_03A < 0x81U) {
            /* FAKE: s8 store views move unk_03A to $v1 as in the original (T-25128) */
            *(s8 *)&D_80122106 = D_800E6280.unk_03A;
            *(s8 *)&D_80122105 = D_800E6280.unk_03A;
            *(s8 *)&D_80122104 = D_800E6280.unk_03A;
            D_800E6280.unk_03A += 1;
        }
        if (D_800E6280.unk_F5F == 0) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.unk_110D) += 1;
                }
            }
        } else if (D_800E6280.unk_F5F == 9) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.unk_110D) += 1;
                }
            }
        } else if (D_800E6280.unk_F5F == 0xC) {
            if ((u32) D_800E6280.unk_1104.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.unk_110D) += 1;
                }
            }
        } else {
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        D_800E6280.unk_1104.w += 1;
        break;
    case 9:
        D_80122106 = D_800E6280.unk_03A;
        D_80122105 = D_800E6280.unk_03A;
        D_80122104 = D_800E6280.unk_03A;
        if (++D_800E6280.unk_03A >= 0x80U) {

            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 10:
        func_8013FC08();
        (*(u8 *)&D_800E6280.unk_110D) += 1;
        break;
    case 11:
        D_801220F4 = 0x01000000;
        *(s32 *)&D_801220D0 = 0x01000000;
        (*(u8 *)&D_800E6280.unk_110D) += 1;
        break;
    case 12:
        D_801500D4 = 0x800;
        func_8013FA7C();
        D_801220F4 = 0x80000000;
        (*(u8 *)&D_800E6280.unk_110D) += 1;
        break;
    case 13:
        func_80140AE4();
        if (D_800E6280.unk_F88 & 0x20) {
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            (*(u8 *)&D_800E6280.unk_110D) += 1;
        }
        break;
    case 14:
        if (D_80150070 >= 3) {
            if (D_801500D4 >= 0xC00) {
                D_80150070 = 0;
                func_80042808();
            } else {
                goto block_50;
            }
        } else {
block_50:
            if ((D_80150070 == -1) && (D_801500D4 >= 0xC00)) {
                func_80042808();
            } else {
                func_8013E89C(1);
                D_801500D4 += 8;
                func_8013FA7C();
            }
        }
        break;
    }
}
