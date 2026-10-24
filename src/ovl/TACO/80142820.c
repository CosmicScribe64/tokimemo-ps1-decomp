#define MAIN_API_OVERRIDE_strncpy /* matched as int: the call's $v0 result range (main_api.h: void), T-19009 */
#include "common.h"
#include "ovl/TACO.h"

s32 strncpy();

void func_80142820(void) {
    switch (D_800E6280.step) {
    case 0:
        func_801428DC();
        break;
    case 1:
        func_80142FE8();
        break;
    case 2:
        func_80142AF0();
        break;
    case 3:
        func_80142EC4();
        break;
    case 4:
        func_80142EE4();
        break;
    case 5:
        func_80142F0C();
        break;
    case 6:
        func_80142F34();
        break;
    case 7:
        func_80142DE0();
        break;
    }
}

/* Settings block at D_800E7D10 +0x40 (Work800E7D10 in main_api.h since T-20900; the loader func_801345D4 in
 * 80133870.c has its own view of it, T-16562): byte 0 holds the "saved" flag, the mode and the selector, byte 1 two
 * more selectors. Every access relocates against the one symbol D_800E7D10: as1 then hoists the flag byte load over
 * the word stores, as the original does (separate symbols D_800E7D50/D_800E7D5C keep it after them). T-19009 */

void func_801428DC(void) {
    D_800E7D10.bits.flag = 1;
    D_800E7D10.w0 = D_8015EDF8;
    D_800E7D10.w1 = D_8015EDFC;
    D_800E7D10.w2 = D_8015EE00;
    strncpy(D_800E7D10.name0, D_8015EE04, 3);
    strncpy(D_800E7D10.name1, D_8015EE08, 3);
    strncpy(D_800E7D10.name2, D_8015EE0C, 3);
    /* the % 2 folds away, but keeps the extra copy of the value that the original has */
    D_800E7D10.bits.mode = D_8015EDD8 % 2;
    D_800E7D10.bits.sel = D_8015EDDC % 3;
    D_800E7D10.bits.hi = D_8015EDE0 % 3;
    D_800E7D10.bits.lo = D_8015EDE4 % 3;
    func_80041584();
    draw2d3d(0, 0);
    D_800E6280.screen_center_x[0] = 0xA0;
    D_800E6280.screen_center_x[1] = 0xA0;
    func_8009C674(0);
    func_8009C210(3);
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    if (func_80045288() == 0) {
        func_800452C4();
    }
    func_8005907C();
    func_80059048();
    func_801347A8();
    func_80048EB8(0);
    func_80048DAC(0);
    func_8004284C();
}

void func_80142AF0(void) {
    GameState *gs = &D_800E6280;
    s32 temp;

    switch (gs->substep) {
    case 0:
        if (func_80053F04() != 0) {
            /* FAKE: the first read of unk_1115 goes through a local so the three tests share a
             * register range that takes $v1 (found by tools/autotweak.py); T-19009 */
            temp = gs->card_result;
            if (temp == 1) {
                gs->substep += 1;
            } else if (gs->card_result == 0xF1) {
                gs->substep += 1;
            } else if (gs->card_result == 0xFF) {
                gs->substep += 1;
            }
        } else if ((u32)gs->card_timer >= 0x201U) {
            gs->substep += 1;
        }
        break;
    case 1:
        func_80053D10();
        if (gs->card_result == 1) {
            if (func_80054AF4(0) == 1) {
                gs->substep += 1;
                func_80053CE0();
                func_8005C4CC(0);
            } else {
                gs->substep = 0xFF;
            }
        } else {
            gs->substep = 0xFF;
        }
        break;
    case 2:
        if (func_80054284() == 3) {
            func_80053D10();
            gs->substep += 1;
        } else if (((u32)gs->card_timer >= 0x201U) || (gs->card_result == 0xF0)) {
            func_80053D10();
            gs->substep = 0xFF;
        }
        break;
    case 3:
        if (func_800552DC(0) == -1) {
            func_8005C4CC(1);
            gs->substep = 0xFF;
        } else {
            func_8005C4CC(1);
            gs->substep += 1;
        }
        break;
    case 4:
        func_80053CE0();
        D_800E7D36 = 0;
        gs->substep += 1;
        break;
    case 5:
        if (func_80054388() == 0x13) {
            func_80044750(0x50F);
            func_80053D10();
            gs->substep += 1;
        } else if (((u32)gs->card_timer >= 0x201U) || (gs->card_result == 0xF0)) {
            func_80044750(0x502);
            func_80053D10();
            gs->substep += 1;
        }
        break;
    case 0xFF:
    default:
        func_8004284C();
        break;
    }
}

typedef struct {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 pad1 : 30;
} TcBits;

void func_80142DE0(void) {
    func_8004E58C();
    dec_bg_reset();
    draw2d3d(1, 0);
    func_80048DAC(1);
    *(s32 *) &D_800E6280.menu_hit[3] = 0x9F;
    if (((TcBits *) &D_800E7D50)->flag == 1) {
        func_80048EB8(0);
        tpage_buf_clear();
        main_state_set(D_800E6280.saved_mode);
        func_80042908(D_800E6280.saved_stage);
        func_80042940(D_800E6280.saved_step);
        func_8006BC28(0);
        func_8006C848(0);
        message_window_init();
        hizuke_init();
        message_disp_switch(1);
        hizuke_disp_switch(1);
        return;
    }
    main_state_set(0x12);
}

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80142F0C(void) {
    func_8004ADE4();
    func_8004284C();
}

u8 func_80142F34(void) {
    switch (D_800E6280.substep) {
    case 0:
        cd_read_sectors(0x95, 0x801A0000, 0x9B94);
        D_800E6280.substep += 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.year, D_800E6280.month, 0);
            D_800E6280.substep += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

u8 func_80142FE8(void) {
    switch (D_800E6280.substep) {
    case 0:
        func_8004482C();
        func_800674B0();
        D_800E6280.substep += 1;
        break;
    case 1:
        if (func_80044E8C() == 1) {
            D_800E6280.substep += 1;
        }
        break;
    case 2:
        if (D_800B3220 == 0) {
            func_80053CE0();
        } else {
            func_8004284C();
        }
        func_8004284C();
        break;
    }
}
