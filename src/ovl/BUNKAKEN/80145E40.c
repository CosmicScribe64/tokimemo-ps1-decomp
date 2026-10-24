#include "common.h"
#include "ovl/BUNKAKEN.h"

typedef struct {
    void (*f[142])();
} FnTbl142; /* size 0x238 */
extern FnTbl142 D_80156060;

void func_80145E40(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl142 tbl;

    tbl = D_80156060;
    idx = D_800E6280.stage;
    tbl.f[idx]();
}

void func_80145EBC(void) {
    GameState *g;

    g = &D_800E6280;
    if (D_80122EBC == 0xFF) {
        switch (D_80122EB8) {
        case 0:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 0) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 0) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2C);
                return;
            }
            func_80042908(5);
            return;
        case 1:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 1) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 1) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x2C);
                return;
            }
            func_80042908(5);
            return;
        case 2:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 2) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 2) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x30);
                return;
            }
            func_80042908(9);
            return;
        case 3:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 3) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 3) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x30);
                return;
            }
            func_80042908(5);
            return;
        case 4:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 4) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 4) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x35);
                return;
            }
            func_80042908(5);
            return;
        case 5:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 5) {
                func_80042908(0x20);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 5) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                func_80042908(0x35);
                return;
            }
            func_80042908(5);
            return;
        default:
            func_80046500();
            return;
        }
    } else {
        switch (D_80122EBC) {
        case 11:
            func_80042908(0x12);
            return;
        case 4:
            func_80042908(0x3A);
            return;
        case 5:
            func_80042908(0x48);
            return;
        case 6:
            func_80042908(0x55);
            return;
        case 7:
            func_80042908(0x62);
            return;
        case 8:
            func_80042908(0x6F);
            return;
        case 9:
            func_80042908(0x7A);
            return;
        case 10:
            func_80042908(0x83);
            return;
        default:
            func_80046500();
            return;
        }
    }
}

void func_801462A0(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80048EB8(0);
        func_8006BD6C(0);
        func_80041168(0xFF);
        func_8004284C();
        return;
    case 1:
        func_80059E00();
        return;
    case 2:
        func_80059BC0();
        return;
    case 3:
        func_8004ADE4();
        func_8004284C();
        return;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        func_8004284C();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E6280.year, D_800E6280.month, 0);
            func_8004284C();
            return;
        }
        return;
    case 6:
        func_8006BC28(0);
        func_8006BD6C(0);
        D_800B5BC8 = 0xFE;
        func_80042908((s32) (*(u8 *)&D_80122EB0));
        func_80042940((s32) (*(u8 *)&D_80122EB4));
        return;
    default:
        func_80046500();
        break;
    }
}

void func_801463E4(void) {
    switch (D_800E6280.step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B7F);
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            func_8006612C((s32) "天の声");
            k_disp_start(1);
            func_80132514();
            D_800E6280.current_girl = GIRL_YUKO;
            func_800847B8(8U);
            strcpy(D_800CA188, "　");
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            func_8004284C();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (D_800E6280.input_device == INPUT_PAD) {
            func_80048EB8(0);
            func_8006BD6C(0);
            set_dec_bri(0);
            main_state_set(MODE_TT_GAME);
        }
        break;
    default:
        func_80046500();
        break;
    }
}
