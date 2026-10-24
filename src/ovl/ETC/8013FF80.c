#define MAIN_API_OVERRIDE_memcpy /* original called it undeclared (implicit int), as 8013BEF0.c (T-24706); keeps $v0 off the case 5 table address, T-25314 */
#define MAIN_API_OVERRIDE_func_8004A1A0 /* original called it undeclared (implicit int): its $v0 result keeps 0x01000000 off $v0 after the first call, T-25314 */
#include "common.h"
#include "ovl/ETC.h"

extern s32 memcpy();
extern s32 func_8004A1A0();

void func_8013FF80(void) {
    if (D_800E6280.step == 0) {
        func_8013FFAC();
    }
}

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
    i = D_800E6280.current_girl;
    switch ((*(u8 *)&D_800E6280.substep)) {
    case 0:
        draw2d3d(1, 0);
        D_800E6280.clear_color_g = 0;
        D_800E6280.clear_color_r = 0;
        D_800E6280.clear_color_b = 0;
        func_80048DAC(1);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        tpage_buf_clear();
        D_801500C0 = 0;
        func_801408F0();
        D_801500C4 = D_8014E070[D_800E6280.current_girl % 13U];
        func_80044890(0, 0xBF98, 0xBF98, 0xCFA7, 0xCF8B, 0xCF83);
        (*(u8 *)&D_800E6280.substep) += 1;
    default:
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x201);
            func_80044890(1, 0xC5B7, 0xC594, 0xD6B2, 0xD664, 0xD643);
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_800462C8(1U, 0x801FA800, 0xBE09);
            cd_read_sectors(0x69U, 0x80180000, 0xB389);
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            (*(u8 *)&D_800E6280.substep) += 1;
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
        D_800E6280.screen_brightness = 0;
        /* FAKE: the empty branch ends the uopt block, so j (set to 2 below) does not share a
         * block with 0x01000000, which then takes $a0 as in the original (T-25314) */
        goto split_peel; split_peel: ;
        /* FAKE: explicit peel plus unsigned 2..6 loop, the matched func_8013CCE4 recipe
         * (T-24706): the unsigned body sums do not CSE with the signed peel literals. T-25128 */
        sp54[0] = 0x80180600; sp54[1] = 0x80184600; for (j = 2; j < 6; j++) { k = j * 0x4000; sp54[j] = 0x80180600 + k; } /* the k temporary gives the shift $v0 and the pointer $v1 (T-25314) */

        func_8004A1A0(sp54, 0, 6, 3, 2, 0x1FF, -0xA0, -0x7D, 0xD, 0x18);
        /* FAKE: the empty branch keeps the call's $v0 result out of the 0x80 chain's block,
         * so the chain keeps $v0 (T-25314) */
        goto split_call; split_call: ;
        D_801220D0 = 0x80000000;
        /* FAKE: element views give the chain its hidden VREG, which shares 0x80 in $v0; plain chains put it in $a3 (T-25314) */
        (&D_801220E0)[0] = (&D_801220E1)[0] = (&D_801220E2)[0] = 0x80;
        D_800E6280.screen_brightness = 0;
        memcpy((void *)0x801E2000, 0x80180600, 0x18000);
        /* FAKE: flat view of D_8014DB90 as in func_8013CF2C; the * 2 index spends the
         * ugen slot the original spends. T-25128 */
        func_80046290(((s32 *)D_8014DB90)[D_800E6280.current_girl * 2], ((s32 *)D_8014DB90)[D_800E6280.current_girl * 2 + 1], D_800E6280.current_girl);
        func_80044750(0x300);
        (*(u8 *)&D_800E6280.substep) += 1;
        break;
    case 6:
        if (func_800460EC() & 4) {
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 7:
        func_80140AE4();
        if ((u8) D_800E6280.screen_brightness < 0x81U) {
            D_80122106 = D_800E6280.screen_brightness;
            D_80122105 = D_800E6280.screen_brightness;
            D_80122104 = D_800E6280.screen_brightness;
            D_800E6280.screen_brightness += 1;
        }
        if (func_800460EC() & 2) {
            D_801500DC += 1;
            D_800B3CF8 = 0;
            D_800B3CFC = 0;
            D_800B3D00 = 0;
            D_800E6280.step_timer.w = 0;
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 8:
        j = D_800E6280.screen_brightness;
        if (j < 0x81) {
            k = j & 0xFF; /* FAKE: the mask in a u32 local keeps the original's andi v0,v1,0xff for the shade stores (T-25314) */
            D_80122106 = k;
            D_80122105 = k;
            D_80122104 = k;
            D_800E6280.screen_brightness = j + 1;
        }
        if (D_800E6280.current_girl == GIRL_SHIORI) {
            if ((u32) D_800E6280.step_timer.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.substep) += 1;
                }
            }
        } else if (D_800E6280.current_girl == GIRL_MEGUMI) {
            if ((u32) D_800E6280.step_timer.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.substep) += 1;
                }
            }
        } else if (D_800E6280.current_girl == GIRL_REI) {
            if ((u32) D_800E6280.step_timer.w >= 0x1EU) {
                func_8013F320(0);
                if (D_8015006C >= 3) {
                    D_8015006C = 0;
                    (*(u8 *)&D_800E6280.substep) += 1;
                }
            }
        } else {
            (*(u8 *)&D_800E6280.substep) += 1;
        }
        D_800E6280.step_timer.w += 1;
        break;
    case 9:
        j = D_800E6280.screen_brightness & 0xFF; /* FAKE: the mask splits the old value ($v0) from the incremented byte ($v1), as1 folds it into the lbu (T-25314) */
        D_80122106 = j;
        D_80122105 = j;
        D_80122104 = j;
        D_800E6280.screen_brightness = j + 1;
        if (D_800E6280.screen_brightness >= 0x80U) {

            (*(u8 *)&D_800E6280.substep) += 1;
        }
        break;
    case 10:
        func_8013FC08();
        (*(u8 *)&D_800E6280.substep) += 1;
        break;
    case 11:
        D_801220F4 = 0x01000000;
        *(s32 *)&D_801220D0 = 0x01000000;
        (*(u8 *)&D_800E6280.substep) += 1;
        break;
    case 12:
        D_801500D4 = 0x800;
        func_8013FA7C();
        D_801220F4 = 0x80000000;
        (*(u8 *)&D_800E6280.substep) += 1;
        break;
    case 13:
        func_80140AE4();
        if (D_800E6280.pad_pressed & PAD_CIRCLE) {
            if ((D_801500D8 == 0) || (D_801500D8 == 2)) {
                func_8013EEFC();
            }
            (*(u8 *)&D_800E6280.substep) += 1;
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

void func_801407F0(void) {
    D_800E6280.stage_timer += 1;
    switch (D_800E6280.stage) {
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
    if (D_800E6280.stage != 0xFF) {
        if (D_801500E0 == 0 && (D_800E6280.pad_pressed & PAD_START)) {
            func_8013FC08();
            D_801500D4 = 0x800;
            func_8013FA7C();
            D_801500E0 = 1;
            func_80042908(2);
        }
    }
}
void func_801408F0(void) {
    D_800E6280.tpage_buf[10] = 0;
    D_800E6280.tpage_buf[11] = 0;
    D_800E6280.tpage_buf[12] = 0;
    D_800E6280.tpage_buf[13] = 0;
    D_800E6280.tpage_buf[14] = 0;
    D_800E6280.tpage_buf[42] = 0;
    D_800E6280.tpage_buf[43] = 0;
    D_800E6280.tpage_buf[44] = 0;
    D_800E6280.tpage_buf[45] = 0;
    D_800E6280.tpage_buf[46] = 0;
    D_800E6280.tpage_buf[0] = 0;
    D_800E6280.tpage_buf[1] = 0;
    D_800E6280.tpage_buf[2] = 0;
    D_800E6280.tpage_buf[3] = 0;
    D_800E6280.tpage_buf[4] = 0;
    D_800E6280.tpage_buf[32] = 0;
    D_800E6280.tpage_buf[33] = 0;
    D_800E6280.tpage_buf[34] = 0;
    D_800E6280.tpage_buf[35] = 0;
    D_800E6280.tpage_buf[36] = 0;
    D_800E6280.tpage_buf[16] = 0;
    D_800E6280.tpage_buf[17] = 0;
    D_800E6280.tpage_buf[18] = 0;
    D_800E6280.tpage_buf[19] = 0;
    D_800E6280.tpage_buf[20] = 0;
    D_800E6280.tpage_buf[48] = 0;
    D_800E6280.tpage_buf[49] = 0;
    D_800E6280.tpage_buf[50] = 0;
    D_800E6280.tpage_buf[51] = 0;
    D_800E6280.tpage_buf[52] = 0;
    D_800E6280.tpage_buf[5] = -1;
    D_800E6280.tpage_buf[6] = -1;
    D_800E6280.tpage_buf[7] = -1;
    D_800E6280.tpage_buf[8] = -1;
    D_800E6280.tpage_buf[9] = -1;
    D_800E6280.tpage_buf[37] = -1;
    D_800E6280.tpage_buf[38] = -1;
    D_800E6280.tpage_buf[39] = -1;
    D_800E6280.tpage_buf[40] = -1;
    D_800E6280.tpage_buf[41] = -1;
    D_800E6280.tpage_buf[21] = -1;
    D_800E6280.tpage_buf[22] = -1;
    D_800E6280.tpage_buf[23] = -1;
    D_800E6280.tpage_buf[24] = -1;
    D_800E6280.tpage_buf[25] = -1;
    D_800E6280.tpage_buf[53] = -1;
    D_800E6280.tpage_buf[54] = -1;
    D_800E6280.tpage_buf[55] = -1;
    D_800E6280.tpage_buf[56] = -1;
    D_800E6280.tpage_buf[57] = -1;
    if ((func_8013F3F8() == 1) && (D_80150060 == 0xFF)) {
        D_800E6280.tpage_buf[7] = 0;
    }
}

void func_80140AE4(void) {
    func_80140BC4();
    func_80140DC8();
    /* FAKE: do-while block puts D_801500DC in $v0 instead of $a0; real source unknown. T-24207 */
    do { func_80140E80(); } while (0);
    if (D_801500DC == 0) {
        /* FAKE: `!= 0` costs one ugen temporary and no code, so the rotor starts at $t7. T-24207 */
        if (((func_80143240() == 1) != 0) && (D_800E6280.pad_held & 0x09000000)) {
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
    if (D_800E6280.options_dirty == 0) {
        if (D_800E6280.girl_cleared[0] >= 2U && D_800E6280.girl_cleared[1] >= 2U && D_800E6280.girl_cleared[2] >= 2U && D_800E6280.girl_cleared[3] >= 2U && D_800E6280.girl_cleared[4] >= 2U && D_800E6280.girl_cleared[5] >= 2U && D_800E6280.girl_cleared[6] >= 2U && D_800E6280.girl_cleared[7] >= 2U && D_800E6280.girl_cleared[8] >= 2U && D_800E6280.girl_cleared[9] >= 2U && D_800E6280.girl_cleared[10] >= 2U && D_800E6280.girl_cleared[11] >= 2U && D_800E6280.girl_cleared[12] >= 2U) {
            return 2;
        }
        if (D_800E6280.girl_cleared[0] != 0 && D_800E6280.girl_cleared[1] != 0 && D_800E6280.girl_cleared[2] != 0 && D_800E6280.girl_cleared[3] != 0 && D_800E6280.girl_cleared[4] != 0 && D_800E6280.girl_cleared[5] != 0 && D_800E6280.girl_cleared[6] != 0 && D_800E6280.girl_cleared[7] != 0 && D_800E6280.girl_cleared[8] != 0 && D_800E6280.girl_cleared[9] != 0 && D_800E6280.girl_cleared[10] != 0 && D_800E6280.girl_cleared[11] != 0 && D_800E6280.girl_cleared[12] != 0) {
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
        if (D_800E6280.event_flags[i] != 0) {
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
        if (D_800E6280.girls[i].flags.f.met) {
            n++;
        }
    }
    if (D_800E6280.girls[11].flags.f.story_stage != 0) {
        n++;
    }
    if (n == 13) {
        return 1;
    }
    return 0;
}
