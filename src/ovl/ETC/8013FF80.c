#ifdef NON_MATCHING
#define MAIN_API_OVERRIDE_func_800AE0A0 /* guard calls it undeclared (implicit int) as 8013BEF0.c does (T-24706); keeps $v0 off the case 5 table address, T-25314 */
#endif
#include "common.h"
#include "ovl/ETC.h"

void func_8013FF80(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8013FFAC();
    }
}

/* NON_MATCHING: T-25314, 10 rows (T-25128 left 197). Left: 0x01000000 in $v0 instead of
 * $a0 in cases 5 and 11 (uopt colours the constant after j, which holds $a0 in the same
 * case 5 block). drafts/T-25314/ETC/func_8013FFAC/NOTES.md */
#ifdef NON_MATCHING
void func_80140AE4(void);
s32 func_8013FFAC(void) {
    /* FAKE: `register` makes i a VREG, so the dead entry read below is a direct unsigned
     * global stored into a VREG and cvt_pass.py emits OPTN 405 0 (zcopy off): the second
     * table loop keeps its runtime index. It replaces a dead two-case switch, whose extra VREG
     * cost 8 frame bytes and a `move v0,a1`. i declared before sp54, the scalars after it, put
     * sp54 at 0x54. T-25314 */
    register s32 i;
    s32 sp54[6];
    u32 j;
    u32 k;

    /* FAKE: the dead read of unk_F5F makes it an entry location for cvt_pass.py, so its
     * promoted range starts at the entry ($t0, reloaded in cases 0, 5 and 8, no entry load).
     * T-25314 */
    i = D_800E6280.unk_F5F;
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
        sp54[0] = 0x80180600; sp54[1] = 0x80184600; for (j = 2; j < 6; j++) { k = j * 0x4000; sp54[j] = 0x80180600 + k; } /* the k temporary gives the shift $v0 and the pointer $v1 (T-25314) */

        func_8004A1A0(sp54, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        D_801220D0 = 0x80000000;
        /* FAKE: element views give the chain its hidden VREG, which shares 0x80 in $v0; plain chains put it in $a3 (T-25314) */
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
        j = D_800E6280.unk_03A;
        if (j < 0x81) {
            k = j & 0xFF; /* FAKE: the mask in a u32 local keeps the original's andi v0,v1,0xff for the shade stores (T-25314) */
            D_80122106 = k;
            D_80122105 = k;
            D_80122104 = k;
            D_800E6280.unk_03A = j + 1;
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
        j = D_800E6280.unk_03A & 0xFF; /* FAKE: the mask splits the old value ($v0) from the incremented byte ($v1), as1 folds it into the lbu (T-25314) */
        D_80122106 = j;
        D_80122105 = j;
        D_80122104 = j;
        D_800E6280.unk_03A = j + 1;
        if (D_800E6280.unk_03A >= 0x80U) {

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
#else
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_8013FFAC);
#endif

void func_801407F0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013FF80();
        break;
    case 1:
        func_8013BF34();
        break;
    case 2:
        func_8013CF2C();
        break;
    case 0xFF:
        func_80140FD8();
        break;
    }
    func_80066334();
    if (D_800E6280.unk_1109 != 0xFF) {
        if (D_801500E0 == 0 && (D_800E6280.unk_F88 & 0x800)) {
            func_8013FC08();
            D_801500D4 = 0x800;
            func_8013FA7C();
            D_801500E0 = 1;
            func_80042908(2);
        }
    }
}
void func_801408F0(void) {
    D_800E6280.unk_1228[10] = 0;
    D_800E6280.unk_1228[11] = 0;
    D_800E6280.unk_1228[12] = 0;
    D_800E6280.unk_1228[13] = 0;
    D_800E6280.unk_1228[14] = 0;
    D_800E6280.unk_1228[42] = 0;
    D_800E6280.unk_1228[43] = 0;
    D_800E6280.unk_1228[44] = 0;
    D_800E6280.unk_1228[45] = 0;
    D_800E6280.unk_1228[46] = 0;
    D_800E6280.unk_1228[0] = 0;
    D_800E6280.unk_1228[1] = 0;
    D_800E6280.unk_1228[2] = 0;
    D_800E6280.unk_1228[3] = 0;
    D_800E6280.unk_1228[4] = 0;
    D_800E6280.unk_1228[32] = 0;
    D_800E6280.unk_1228[33] = 0;
    D_800E6280.unk_1228[34] = 0;
    D_800E6280.unk_1228[35] = 0;
    D_800E6280.unk_1228[36] = 0;
    D_800E6280.unk_1228[16] = 0;
    D_800E6280.unk_1228[17] = 0;
    D_800E6280.unk_1228[18] = 0;
    D_800E6280.unk_1228[19] = 0;
    D_800E6280.unk_1228[20] = 0;
    D_800E6280.unk_1228[48] = 0;
    D_800E6280.unk_1228[49] = 0;
    D_800E6280.unk_1228[50] = 0;
    D_800E6280.unk_1228[51] = 0;
    D_800E6280.unk_1228[52] = 0;
    D_800E6280.unk_1228[5] = -1;
    D_800E6280.unk_1228[6] = -1;
    D_800E6280.unk_1228[7] = -1;
    D_800E6280.unk_1228[8] = -1;
    D_800E6280.unk_1228[9] = -1;
    D_800E6280.unk_1228[37] = -1;
    D_800E6280.unk_1228[38] = -1;
    D_800E6280.unk_1228[39] = -1;
    D_800E6280.unk_1228[40] = -1;
    D_800E6280.unk_1228[41] = -1;
    D_800E6280.unk_1228[21] = -1;
    D_800E6280.unk_1228[22] = -1;
    D_800E6280.unk_1228[23] = -1;
    D_800E6280.unk_1228[24] = -1;
    D_800E6280.unk_1228[25] = -1;
    D_800E6280.unk_1228[53] = -1;
    D_800E6280.unk_1228[54] = -1;
    D_800E6280.unk_1228[55] = -1;
    D_800E6280.unk_1228[56] = -1;
    D_800E6280.unk_1228[57] = -1;
    if ((func_8013F3F8() == 1) && (D_80150060 == 0xFF)) {
        D_800E6280.unk_1228[7] = 0;
    }
}

void func_80140AE4(void) {
    func_80140BC4();
    func_80140DC8();
    /* FAKE: do-while block puts D_801500DC in $v0 instead of $a0; real source unknown. T-24207 */
    do { func_80140E80(); } while (0);
    if (D_801500DC == 0) {
        /* FAKE: `!= 0` costs one ugen temporary and no code, so the rotor starts at $t7. T-24207 */
        if (((func_80143240() == 1) != 0) && (D_800E6280.unk_F80 & 0x09000000)) {
            if (D_801500D8 != 1) {
                func_80044750(0x50E);
            }
            D_801500D8 = 1;
        }
    } else if (D_801500DC == 1) {
        if (D_800B3CFC == 1) {
            func_80044750(0x50E);
            if (D_801500D8 == 1) {
                D_801500D8 = 3;
            } else {
                D_801500D8 = 2;
            }
        }
    }
}

s32 func_80140BC4(void) {
    s32 r;
    r = 0;
    if (D_800E6280.unk_1124 == 0) {
        if (D_800E6280.unk_53C[0] >= 2U && D_800E6280.unk_53C[1] >= 2U && D_800E6280.unk_53C[2] >= 2U && D_800E6280.unk_53C[3] >= 2U && D_800E6280.unk_53C[4] >= 2U && D_800E6280.unk_53C[5] >= 2U && D_800E6280.unk_53C[6] >= 2U && D_800E6280.unk_53C[7] >= 2U && D_800E6280.unk_53C[8] >= 2U && D_800E6280.unk_53C[9] >= 2U && D_800E6280.unk_53C[10] >= 2U && D_800E6280.unk_53C[11] >= 2U && D_800E6280.unk_53C[12] >= 2U) {
            return 2;
        }
        if (D_800E6280.unk_53C[0] != 0 && D_800E6280.unk_53C[1] != 0 && D_800E6280.unk_53C[2] != 0 && D_800E6280.unk_53C[3] != 0 && D_800E6280.unk_53C[4] != 0 && D_800E6280.unk_53C[5] != 0 && D_800E6280.unk_53C[6] != 0 && D_800E6280.unk_53C[7] != 0 && D_800E6280.unk_53C[8] != 0 && D_800E6280.unk_53C[9] != 0 && D_800E6280.unk_53C[10] != 0 && D_800E6280.unk_53C[11] != 0 && D_800E6280.unk_53C[12] != 0) {
            return 1;
        }
        return 0;
    }
    return r;
}

s32 func_80140DC8(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 83; i++) {
        if (D_800E6280.unk_56C[i] != 0) {
            n++;
        }
    }
    if (n >= 30) {
        return 1;
    }
    return 0;
}

s32 func_80140E80(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 13; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1) {
            n++;
        }
    }
    if (D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
        n++;
    }
    if (n == 13) {
        return 1;
    }
    return 0;
}
