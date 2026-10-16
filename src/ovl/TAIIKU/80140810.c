#define MAIN_API_OVERRIDE_func_80085368 /* callers ignore the result, the original calls it as int; blocks $v0 (T-16554) */
#define MAIN_API_OVERRIDE_func_800853FC /* same */
#include "common.h"
#include "ovl/TAIIKU.h"

s32 func_80085368();
s32 func_800853FC();

/* FAKE: literal asset bases retain the independent source loads; named
 * declarations hoist their bases. The asset data is loaded outside this
 * overlay (T-3300, T-9720). */
void func_80140810(void) {
    D_80149FB0 = 0x80180144;
    D_80149FB4 = 0x801820FC;
    D_80149FB8 = 0x80183E7C;
    D_80149FBC = 0x80185F94;
    D_80149FC0 = 0x80186674;
    D_80149FC4 = 0x80187088;
    D_80149FC8 = 0x80187F20;
    D_80149FCC = 0x80188BC8;
    D_80149FD0 = 0x80188DA4;
    D_80149FD4 = 0x80189A90;
    D_80149FD8 = 0x8018A794;
    D_80149FDC = 0x8018C0C8;
    D_80149FE0 = 0x8018CEE0;
    D_80149FE4 = 0x8018EA78;
    D_80149FE8 = 0x8018016C;
    D_80149FEC = 0x801823BC;
    D_80149FF0 = 0x80184100;
    D_80149FF4 = 0x801861D4;
    D_80149FF8 = 0x801866BC;
    D_80149FFC = 0x80187170;
    D_8014A000 = 0x80188048;
    D_8014A004 = 0x80188C98;
    D_8014A008 = 0x80188DB4;
    D_8014A00C = 0x80189BF4;
    D_8014A010 = 0x8018A89C;
    D_8014A014 = 0x8018C31C;
    D_8014A018 = 0x8018CFEC;
    D_8014A01C = 0x8018EC14;
    D_8014A020 = *(s16 *)0x8018EC40;
    D_8014A024 = *(s16 *)0x8018EC44;
    D_8014A028 = *(s16 *)0x8018EC48;
    D_8014A02C = *(s16 *)0x8018EC4C;
    D_8014A030 = *(s16 *)0x8018EC50;
    D_8014A034 = *(s16 *)0x8018EC54;
    D_8014A038 = *(s16 *)0x8018EC58;
    D_8014A03C = *(s16 *)0x8018EC5C;
    D_8014A040 = *(s16 *)0x8018EC60;
    D_8014A044 = *(s16 *)0x8018EC64;
    D_8014A048 = *(s16 *)0x8018EC68;
    D_8014A04C = *(s16 *)0x8018EC6C;
    D_8014A050 = *(s16 *)0x8018EC70;
    D_8014A054 = *(s16 *)0x8018EC74;
}

/* NON_MATCHING: T-16554, 33 differing instructions (66 diff lines); only ugen temporary
 * numbering after the case 22 block copy: the draft's end pointer is $t3, the original's $t4,
 * and every later $t register is shifted. Needs one more unseen temp in that copy; see
 * wiki/matching-notes.md T-16554. */
#ifdef NON_MATCHING
s32 func_80140A90(void) {
    s32 i;

    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        D_80122EA0 = 0;
        func_8004E58C();
        tpage_buf_clear();
        D_800E6280.unk_1228[0x1A] = -1;
        func_80048E78();
        dec_bg_cd_read(0x413C, 1);
        D_8014A064 = 0;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_8005751C(1);
            func_80062CD0(0x5F5D);
            func_8004284C();
        }
        break;
    case 2:
        normal_date_move_place();
        break;
    case 3:
        func_8006B900();
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            func_8006612C((s32) "グラウンド");
            k_disp_start(1);
            addr_init_bustup();
            D_800E6280.unk_F5F = 1;
            func_80083474();
        }
        break;
    case 4:
        func_8006B900();
        cd_read_sectors(1U, 0x801E8000, 0xB7BE);
        func_8004284C();
        break;
    case 5:
        func_8006B900();
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(1U);
            func_80141B70();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8014A0A0;
            D_800CA164 = D_8014A0A4;
            D_800CA168 = D_8014A0A8;
            func_8004284C();
        }
        break;
    case 6:
    case 8:
    case 12:
    case 14:
        normal_date_speak();
        func_8006B900();
        break;
    case 7:
        normal_date_girl_in();
        func_8006B900();
        break;
    case 9:
        normal_date_two_select();
        func_8006B900();
        break;
    case 10:
        /* FAKE: scalar view keeps uopt from promoting D_80122CDC into $v0; the original selector
         * is in $v1. T-16554 */
        switch (*(s32 *)&D_80122CDC) {
        case 0:
            func_80042940(0x11);
            break;
        case 1:
            func_80042940(0xB);
            break;
        }
        func_8006B900();
        break;
    case 11:
        D_800E6280.unk_1BC[1].unk_06 -= 1;
        D_800E6280.unk_1BC[1].unk_02 -= 1;
        D_800E6280.unk_1BC[1].unk_0A += 0xA;
        func_80083440(3U);
        func_80063668();
        D_800CA148 = 0xA;
        D_800CA14C = 0;
        func_8006B900();
        func_8004284C();
        break;
    case 13:
        func_8006B900();
        normal_date_girl_out();
        break;
    case 15:
        normal_date_bg_fadeout();
        break;
    case 16:
        D_8014A064 = 2;
        break;
    case 17:
        normal_date_speak();
        func_8006B900();
        break;
    case 18:
        func_8006B900();
        dec_bg_cd_read(0x407A, 1);
        func_8004284C();
        break;
    case 19:
        normal_date_bggirl_fadeout();
        break;
    case 20:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            func_8006612C((s32) "校舎裏");
            D_80125CA0 = 0xC0;
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(0x45U, 0x80197000, 0xBB15);
            func_8004500C(0, 0x201);
            func_8004284C();
        }
        break;
    case 21:
        normal_date_bggirl_fadein();
        break;
    case 22:
        if (cd_read_sync_status() & 1) {
            func_80142180();
            load_palette(D_8014A0F0, 0x11, 1, 2, 0);
            func_80084E90(D_8014A0F4, D_8014A0F8, D_8014A0FC, D_8014A100, D_8014A104, D_8014A108);
            func_800850D4(D_8014A0E4, D_8014A0E8, D_8014A0E0, (s32) D_8014A0EC);
            for (i = 0; i < 2; i++) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 |= 8;
            }
            ((DisplayActor44 *)D_8011ECD0)[98] = ((DisplayActor44 *)D_8011ECD0)[96];
            ((DisplayActor44 *)D_8011ECD0)[98].unk_16 = 4;
            func_8004284C();
        }
        break;
    case 23:
        normal_date_speak();
        break;
    case 24:
        func_80047550();
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) "ドシン", 0);
        func_80044750(0x60A);
        Vblnk_Timer_Init();
        func_8004284C();
        break;
    case 25:
        if (Vblnk_Timer() >= 0x3D) {
            dec_bg_cd_read(0x4721, 1);
            k_reset(1);
            func_8004284C();
        }
        break;
    case 26:
        normal_date_bggirl_fadeout();
        break;
    case 27:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 28:
        func_80085368();
        ((DisplayActor44 *)D_8011ECD0)[98].unk_03 |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 += 4;
        if (((DisplayActor44 *)D_8011ECD0)[98].unk_07 >= 0x80) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = 0x80;
        }
        break;
    case 29:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 1;
        }
        break;
    case 30:
        normal_date_speak();
        break;
    case 31:
        ((DisplayActor44 *)D_8011ECD0)[98].unk_16 = 5;
        func_8004284C();
        break;
    case 32:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 1;
        }
        break;
    case 33:
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 3;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_16 = 1;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_08 = 0;
        func_8004284C();
        break;
    case 34:
        normal_date_speak();
        break;
    case 35:
        normal_date_speak();
        if (check_end_k() != 0) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0;
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 1;
        }
        break;
    case 36:
        normal_date_speak();
        break;
    case 37:
        func_800853FC();
        ((DisplayActor44 *)D_8011ECD0)[98].unk_07 -= 4;
        if (((DisplayActor44 *)D_8011ECD0)[98].unk_07 < 3) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_07 = 0;
        }
        break;
    case 38:
        D_8014A064 = 1;
        func_80085B3C(2, 0xD);
        D_800E6280.unk_1BC[1].unk_06 += 2;
        D_800E6280.unk_1BC[1].unk_02 += 1;
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
    return (s32) D_8014A064;
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80140810", func_80140A90);
#endif
