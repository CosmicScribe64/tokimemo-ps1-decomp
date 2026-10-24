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
    idx = game_state.stage;
    tbl.f[idx]();
}

void func_80145EBC(void) {
    GameState *g;

    g = &game_state;
    if (D_80122EBC == 0xFF) {
        switch (D_80122EB8) {
        case 0:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 0) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 0) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x2C);
                return;
            }
            main_stage_set(5);
            return;
        case 1:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 1) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[1].flags.b[2] >> 4) == 1) && (((u32) (g->girls[1].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x2C);
                return;
            }
            main_stage_set(5);
            return;
        case 2:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 2) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 2) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x30);
                return;
            }
            main_stage_set(9);
            return;
        case 3:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 3) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[2].flags.b[2] >> 4) == 3) && (((u32) (g->girls[2].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x30);
                return;
            }
            main_stage_set(5);
            return;
        case 4:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 4) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 4) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x35);
                return;
            }
            main_stage_set(5);
            return;
        case 5:
            if (((u8) g->girls[0].flags.b[2] >> 4) == 5) {
                main_stage_set(0x20);
                return;
            }
            if ((((u8) g->girls[3].flags.b[2] >> 4) == 5) && (((u32) (g->girls[3].flags.w << 0x1E) >> 0x1F) == 1)) {
                main_stage_set(0x35);
                return;
            }
            main_stage_set(5);
            return;
        default:
            game_reset_to_title();
            return;
        }
    } else {
        switch (D_80122EBC) {
        case 11:
            main_stage_set(0x12);
            return;
        case 4:
            main_stage_set(0x3A);
            return;
        case 5:
            main_stage_set(0x48);
            return;
        case 6:
            main_stage_set(0x55);
            return;
        case 7:
            main_stage_set(0x62);
            return;
        case 8:
            main_stage_set(0x6F);
            return;
        case 9:
            main_stage_set(0x7A);
            return;
        case 10:
            main_stage_set(0x83);
            return;
        default:
            game_reset_to_title();
            return;
        }
    }
}

void func_801462A0(void) {
    switch (game_state.step) {
    case 0:
        actor_clear_range(0);
        cursor_show(0);
        func_80041168(0xFF);
        main_step_next();
        return;
    case 1:
        func_80059E00();
        return;
    case 2:
        func_80059BC0();
        return;
    case 3:
        func_8004ADE4();
        main_step_next();
        return;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        main_step_next();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(game_state.year, game_state.month, 0);
            main_step_next();
            return;
        }
        return;
    case 6:
        cursor_actor_init(0);
        cursor_show(0);
        D_800B5BC8 = 0xFE;
        main_stage_set((s32) (*(u8 *)&D_80122EB0));
        main_step_set((s32) (*(u8 *)&D_80122EB4));
        return;
    default:
        game_reset_to_title();
        break;
    }
}

void func_801463E4(void) {
    switch (game_state.step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B7F);
        main_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            message_window_set_title((s32) "天の声");
            k_disp_start(1);
            func_80132514();
            game_state.current_girl = GIRL_YUKO;
            dialogue_names_set(8U);
            strcpy(name_buf_girl_short, "　");
            dialogue_group = 0;
            dialogue_line = 0;
            dialogue_text_table = (s32) D_80155E90;
            dialogue_command_table = (s32) D_80155E94;
            dialogue_voice_table = (s32) D_80155E98;
            main_step_next();
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        normal_date_bg_fadeout();
        break;
    case 4:
        if (game_state.input_device == INPUT_PAD) {
            actor_clear_range(0);
            cursor_show(0);
            set_dec_bri(0);
            main_state_set(MODE_TT_GAME);
        }
        break;
    default:
        game_reset_to_title();
        break;
    }
}
