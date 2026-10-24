#define MAIN_API_OVERRIDE_srand /* T-24094: int-returning view for func_801509A4 */
#include "common.h"
#include "ovl/BUNKASAI.h"

s32 srand();

typedef struct {
    void (*f[165])();
} FnTbl165; /* size 0x294 */
extern FnTbl165 D_80160810;

void func_80150930(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl165 tbl;

    tbl = D_80160810;
    idx = game_state.mode_stage;
    tbl.f[idx]();
}

void func_801509A4(void) {
    s32 temp_v1;
    u8 count;
    s32 i;
    BunkasaiGirlList sp30;

    sp30 = D_80160AA4;
    srand(game_state.timer_ticks);
    count = 0;
    for (i = 0; i < 11; i++) {
        if (game_state.girls[i].flags.f.met == 1) {
            sp30.v[count] = i;
            count++;
        }
    }
    temp_v1 = sp30.v[rand() % count];
    D_80122EBC = temp_v1;
    switch (temp_v1) {
    case 0:
        mode_stage_set(0xF);
        return;
    case 1:
        mode_stage_set(0x1C);
        return;
    case 2:
        mode_stage_set(0x2E);
        return;
    case 3:
        mode_stage_set(0x41);
        return;
    case 4:
        mode_stage_set(0x55);
        return;
    case 5:
        mode_stage_set(0x61);
        return;
    case 6:
        mode_stage_set(0x6D);
        return;
    case 7:
        mode_stage_set(0x79);
        return;
    case 8:
        mode_stage_set(0x85);
        return;
    case 9:
        mode_stage_set(0x91);
        return;
    case 10:
        mode_stage_set(0x9A);
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80150B7C(void) {
    u16 temp_v1_2;
    u32 temp_t2;
    u8 temp_v1;
    /* FAKE: T-22012, the record reads go through game while the selector and the 0xF4 halfword are read from
     * game_state directly; the split is what gives the hoisted constant 1 the register $v0 (all-game: 50 rows). */
    GameState *game = &game_state;

    temp_v1 = game_state.mode_step;
    switch (temp_v1) {
    case 0:
        talk_group = ((((u16) game_state.player_flags.h >> 0xC) * 3) + game_state.year) - 0x4D;
        mode_step_next();
        return;
    case 1:
        normal_date_speak();
        return;
    case 2:
        temp_v1_2 = game_state.player_flags.h;
        D_80122EB8 = (s32) ((u16) game_state.player_flags.h >> 0xC);
        temp_t2 = temp_v1_2 >> 0xC;
        switch (temp_t2) {
        case 0:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 0) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[1].flags.b[2] >> 4) == 0) && (game->girls[1].flags.f.met == 1)) {
                mode_stage_set(0x2A);
                return;
            }
            mode_stage_set(0xE);
            return;
        case 1:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 1) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[1].flags.b[2] >> 4) == 1) && (game->girls[1].flags.f.met == 1)) {
                mode_stage_set(0x2A);
                return;
            }
            mode_stage_set(0xE);
            return;
        case 2:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 2) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[2].flags.b[2] >> 4) == 2) && (game->girls[2].flags.f.met == 1)) {
                mode_stage_set(0x3B);
                return;
            }
            mode_stage_set(0xE);
            return;
        case 3:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 3) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[2].flags.b[2] >> 4) == 3) && (game->girls[2].flags.f.met == 1)) {
                mode_stage_set(0x3B);
                return;
            }
            mode_stage_set(0xE);
            return;
        case 4:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 4) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[3].flags.b[2] >> 4) == 4) && (game->girls[3].flags.f.met == 1)) {
                mode_stage_set(0x50);
                return;
            }
            mode_stage_set(0xE);
            return;
        case 5:
            if (((u8) game->girls[0].flags.h[1] >> 4) == 5) {
                mode_stage_set(1);
                return;
            }
            if ((((u8) game->girls[3].flags.b[2] >> 4) == 5) && (game->girls[3].flags.f.met == 1)) {
                mode_stage_set(0x50);
                return;
            }
            mode_stage_set(0xE);
            return;
        default:
            game_mode_reset_to_title();
            return;
        }
        break;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80150F20(void) {
    switch (game_state.mode_step) {
    case 0:
        actor_clear_range(0);
        cursor_show(0);
        func_80041168(0xFF);
        mode_step_next();
        return;
    case 1:
        func_80059E00();
        return;
    case 2:
        func_80059BC0();
        return;
    case 3:
        func_8004ADE4();
        mode_step_next();
        return;
    case 4:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        mode_step_next();
        return;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80068938(game_state.year, game_state.month, 0);
            mode_step_next();
            return;
        }
        return;
    case 6:
        cursor_actor_init(0);
        cursor_show(0);
        D_800B5BC8 = 0xFE;
        mode_stage_set((s32) (*(u8 *)&D_80122EB0));
        mode_step_set((s32) (*(u8 *)&D_80122EB4));
        return;
    default:
        game_mode_reset_to_title();
        break;
    }
}

void func_80151064(void) {
    switch (game_state.mode_step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B7F);
        mode_step_next();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            k_reset(0);
            message_window_set_title((s32) "天の声");
            k_disp_start(1);
            func_801323DC();
            game_state.current_girl = GIRL_YUKO;
            talk_names_set(8U);
            strcpy(name_code_girl_short, "　");
            talk_group = 0;
            talk_line = 0;
            talk_text_table = (s32) D_801604F0;
            talk_command_table = (s32) D_801604F4;
            talk_voice_table = (s32) D_801604F8;
            mode_step_next();
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
            game_mode_set(GAME_MODE_TT_GAME);
        }
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}
