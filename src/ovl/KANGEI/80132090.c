#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139950);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139954);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139958);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_8013995C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139960);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139964);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139968);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_8013996C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139970);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139974);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139978);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_8013997C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139980);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139984);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139988);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_8013998C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139990);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139994);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139998);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_8013999C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_801399A0);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_801399A4);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_801399A8);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_801399AC);
/* Scene table, indexed by D_800E6280.step in func_80132214. Defining it here as the
 * original did matters for code: as1 treats every function named in this file's data as an
 * address-taken label, so a jump-table dispatch in this file gets an edge back to the entry of
 * each named function (T-25150, T-25139). */
FnTbl22 D_801399B0 = { {
    func_80132514, func_801323A8, func_80132430, func_801324D8, func_8007C784,
    func_801327DC, func_8007C784, dec_init, change_dec_bg, func_80132C24,
    func_8007C784, func_80083474, func_8007C8A4, place_init, func_801324B0,
    func_80132354, normal_date_girl_in, normal_date_speak, normal_date_girl_out, normal_date_bg_out,
    func_80132290, func_80046500,
} };
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139A08);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139A34);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139A3C);
INCLUDE_RODATA("asm/ovl/KANGEI/data/KANGEI/80132090.data", D_80139A44);

void func_80132090(void) {
    D_80139950[0][0] = 0x801973AC;
    D_80139950[0][1] = 0x80197910;
    D_80139950[0][2] = 0x80197C44;
    D_80139950[0][3] = 0x80197F34;
    D_80139950[0][4] = 0x80198130;
    D_80139950[0][5] = 0x801982DC;
    D_80139950[0][6] = 0x801984F4;
    D_80139950[0][7] = 0x8019865C;
    D_80139950[1][0] = 0x8019740C;
    D_80139950[1][1] = 0x80197930;
    D_80139950[1][2] = 0x80197C74;
    D_80139950[1][3] = 0x80197F4C;
    D_80139950[1][4] = 0x80198140;
    D_80139950[1][5] = 0x801982F4;
    D_80139950[1][6] = 0x8019850C;
    D_80139950[1][7] = 0x80198668;
    D_80139950[2][0] = 0x801977AC;
    D_80139950[2][1] = 0x80197A70;
    D_80139950[2][2] = 0x80197E44;
    D_80139950[2][3] = 0x80198034;
    D_80139950[2][4] = 0x801981F0;
    D_80139950[2][5] = 0x801983DC;
    D_80139950[2][6] = 0x801985F4;
    D_80139950[2][7] = 0x801986DC;
}

void func_80132214(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_801399B0;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

s32 func_80132290(void) {
    if (D_800E6280.substep == 0) {
        func_800674B0();
        D_800E6280.substep += 1;
    }
    if (D_800E6280.substep == 1) {
        if (D_800E6280.step_timer.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.substep = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.substep += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_80132354(void) {
    u32 v;
    s32 lo;

    v = get_g_zyotai_h(D_800E6280.current_girl);
    lo = v & 0xFF;
    if (v >= 3U) {
        func_80083440(3);
    } else {
        func_80083440(lo);
    }
    func_8004284C();
}

void func_801323A8(void) {
    func_80044890(1, 0xBF98, 0xBF79, (&D_800B3688)[D_800E6280.current_girl], (&D_800B36C8)[D_800E6280.current_girl], (&D_800B3708)[D_800E6280.current_girl]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132430(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.step_timer.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.step - 1) & 0xFF);
    }
}

void func_801324B0(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801324D8(void) {
    cd_read_sectors(3, 0x80197000, 0xAF3C);
    func_80132090();
    func_8004284C();
}

void func_80132514(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E6280.tile_layer_enabled = 0;
    D_800E6280.quads_enabled = 1;
    func_8008585C();
    D_800E6280.screen_brightness = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    func_80084E4C();
    func_801325D0();
    func_8004284C();
}

void func_801325D0(void) {
    D_800E6280.current_girl = D_800E6280.date_girl;
    func_800847B8(D_800E6280.date_girl);
    func_80132090();
    func_8013260C();
}

void func_8013260C(void) {
    switch (D_800E6280.current_girl) {
    case GIRL_SHIORI:
        D_800CA160 = D_80139950[0][0];
        D_800CA164 = D_80139950[1][0];
        D_800CA168 = D_80139950[2][0];
        return;
    case GIRL_MIO:
        D_800CA160 = D_80139950[0][3];
        D_800CA164 = D_80139950[1][3];
        D_800CA168 = D_80139950[2][3];
        return;
    case GIRL_YUINA:
        D_800CA160 = D_80139950[0][1];
        D_800CA164 = D_80139950[1][1];
        D_800CA168 = D_80139950[2][1];
        return;
    case GIRL_AYAKO:
        D_800CA160 = D_80139950[0][2];
        D_800CA164 = D_80139950[1][2];
        D_800CA168 = D_80139950[2][2];
        return;
    case GIRL_SAKI:
        D_800CA160 = D_80139950[0][6];
        D_800CA164 = D_80139950[1][6];
        D_800CA168 = D_80139950[2][6];
        return;
    case GIRL_YUKARI:
        D_800CA160 = D_80139950[0][4];
        D_800CA164 = D_80139950[1][4];
        D_800CA168 = D_80139950[2][4];
        return;
    case GIRL_NOZOMI:
        D_800CA160 = D_80139950[0][5];
        D_800CA164 = D_80139950[1][5];
        D_800CA168 = D_80139950[2][5];
        return;
    default:
        D_800CA160 = D_80139950[0][7];
        D_800CA164 = D_80139950[1][7];
        D_800CA168 = D_80139950[2][7];
        return;
    }
}

void func_801327DC(void) {
    u32 club;

    /* FAKE: the two empty tests of the club halfword are plain uses of the global (no widening
     * CVT); with them uopt keeps the halfword in a register range and every read of it is
     * `lui v1; lhu v1`, as in the original. Any two such tests anywhere in the function do it,
     * one is not enough. T-25139 */
    if (D_800E6280.player_flags.h) {}
    func_80085B3C(0x10, D_800E6280.player_flags.h >> 12);
    if (D_800E6280.player_flags.h) {} /* FAKE: second plain use, see above. T-25139 */
    switch (D_800E6280.player_flags.h >> 12) {
    case 0:
        strcpy(D_800CA19C, "文芸部");
        bg_read_sub2(0x40BC);
        break;
    case 1:
        strcpy(D_800CA19C, "演劇部");
        bg_read_sub2(0x40B2);
        break;
    case 2:
        strcpy(D_800CA19C, "科学部");
        bg_read_sub2(0x40C6);
        break;
    case 3:
        strcpy(D_800CA19C, "電脳部");
        bg_read_sub2(0x40D0);
        break;
    case 4:
        strcpy(D_800CA19C, "美術部");
        bg_read_sub2(0x40D9);
        break;
    case 5:
        strcpy(D_800CA19C, "吹奏楽部");
        bg_read_sub2(0x40E2);
        break;
    case 6:
        strcpy(D_800CA19C, "野球部");
        bg_read_sub2(0x4097);
        break;
    case 7:
        strcpy(D_800CA19C, "サッカー部");
        bg_read_sub2(0x4097);
        break;
    case 8:
        strcpy(D_800CA19C, "テニス部");
        bg_read_sub2(0x4097);
        break;
    case 9:
        strcpy(D_800CA19C, "水泳部");
        bg_read_sub2(0x40A0);
        break;
    default:
        strcpy(D_800CA19C, "バスケ部");
        bg_read_sub2(0x40A9);
        break;
    }
    D_800CA14C = 0;
    switch (D_800E6280.current_girl) {
    case GIRL_SHIORI:
        club = D_800E6280.player_flags.h >> 12;
        if (club < 6) {
            D_800CA148 = 0;
        } else if (club == 6) {
            D_800CA148 = 2;
        } else if (club == 7) {
            D_800CA148 = 4;
        } else {
            D_800CA148 = 6;
        }
        if (!D_800E6280.girls[D_800E6280.current_girl].flags.f.b2) {
            D_800CA148 += 1;
        }
        break;
    case GIRL_MIO:
        D_800CA148 = 0;
        if (!D_800E6280.girls[D_800E6280.current_girl].flags.f.b2) {
            D_800CA148 = 1;
        }
        break;
    case GIRL_YUINA:
        D_800CA148 = 0;
        if (!D_800E6280.girls[D_800E6280.current_girl].flags.f.b2) {
            D_800CA148 = 1;
        }
        if (D_800E6280.girls[D_800E6280.current_girl].flags.f.b19) {
            D_800CA148 = 2;
        }
        break;
    case GIRL_AYAKO:
        if ((D_800E6280.player_flags.h >> 12) == 4) {
            D_800CA148 = 0;
        } else {
            D_800CA148 = 1;
        }
        if (!D_800E6280.girls[D_800E6280.current_girl].flags.f.b2) {
            D_800CA148 += 2;
        }
        break;
    case GIRL_SAKI:
        if ((D_800E6280.player_flags.h >> 12) == 6) {
            D_800CA148 = 0;
        } else {
            D_800CA148 = 1;
        }
        break;
    case GIRL_YUKARI:
        D_800CA148 = 0;
        if (!D_800E6280.girls[D_800E6280.current_girl].flags.f.b2) {
            D_800CA148 = 1;
        }
        break;
    case GIRL_NOZOMI:
        D_800CA148 = 0;
        /* FAKE: the (u8) narrowing spends one ugen temporary that as1 deletes, which gives the
         * original's andi t1 and addiu t3. T-25139 */
        if (((u8) get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
            D_800CA148 += 1;
        }
        break;
    default:
        D_800CA148 = 0;
        break;
    }
    func_8004284C();
}

void func_80132C24(void) {
    u8 girl;
    KangeiWords11 all;
    KangeiWords2 first;
    KangeiWords2 second;
    KangeiWords2 third;

    all = D_80139A08;
    first = D_80139A34;
    second = D_80139A3C;
    third = D_80139A44;
    if ((D_800E6280.month < 6) || (D_800E6280.month >= 10)) {
        all.unk_00[0] = 0x544A;
        all.unk_00[3] = 0x544A;
        all.unk_00[5] = 0x544A;
        first.unk_00[0] = 0x5D41;
        second.unk_00[1] = 0x522E;
        third.unk_00[1] = 0x58AF;
    }
    girl = D_800E6280.current_girl;
    switch (girl) {
    case 0:
        func_80062CD0(all.unk_00[D_800E6280.player_flags.h >> 12]);
        break;
    case 1:
        func_80062CD0(first.unk_00[D_800E6280.player_flags.h >> 12]);
        break;
    case 2:
        func_80062CD0(second.unk_00[(D_800E6280.player_flags.h >> 12) - 2]);
        break;
    case 3:
        func_80062CD0(third.unk_00[(D_800E6280.player_flags.h >> 12) - 4]);
        break;
    case 4:
        func_80062CD0(0x66EC);
        break;
    case 5:
        func_80062CD0(0x5FE4);
        break;
    case 6:
        func_80062CD0(0x625A);
        break;
    default:
        func_80062CD0(0x6962);
        break;
    }
    func_8004284C();
}
