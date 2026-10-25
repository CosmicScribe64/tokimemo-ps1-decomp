/* overlay_loader.c: src/main/800789E0.c; T-27059: original object [0x800789E0,0x80079B10); dispatches/loads/verifies overlays from sector and entry tables and includes the TT overlay load path. Retains TT launch/setup and the memory-card/system-file polling, read/apply/write flow at 0x80079800; these helpers are mixed content rather than overlay verification. Original object 0x800789E0-0x80079B10 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game/system_data.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgpu.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/draw.h"
#include "main/game_mode.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/resident_views.h"
#include "main/import_views/scenes.h"
#include "main/memory_card.h"
#include "main/message.h"
#include "main/ovl.h"
#include "main/settings.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/sys.h"
#include "main/timer.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/game_mode.h"
#include "main/ovl.h"

/* Entry of the overlay that game_mode_tt_game_mode_handler loads (LBA 0x9A6A at 0x80132000); not a main-exe symbol. */
extern s32 D_8013E850;
extern s32 D_80132000;
extern s32 D_80132040;
extern s32 D_8013209C;
extern s32 D_80132140;
extern s32 D_80132214;
extern s32 D_80132244;
extern s32 D_80132274;
extern s32 D_801322D4;
extern s32 D_80132334;
extern s32 D_80132FF8;
extern s32 D_80133120;
extern s32 D_801335E8;
extern s32 D_80133A2C;
extern s32 D_80134040;
extern s32 D_80134C90;
extern s32 D_80137144;
extern s32 D_80137490;
extern s32 D_80137890;
extern s32 D_80137F64;
extern s32 D_8013882C;
extern s32 D_801389A0;
extern s32 D_80139150;
extern s32 D_801397D0;
extern s32 D_8013B484;
extern s32 D_8013B5B0;
extern s32 D_8013C780;
extern s32 D_8013D180;
extern s32 D_801407F0;
extern s32 D_80141280;
extern s32 D_8014231C;
extern s32 D_801426D0;
extern s32 D_80142700;
extern s32 D_80142974;
extern s32 D_801448D4;
extern s32 D_80144AD0;
extern s32 D_80144F70;
extern s32 D_80145A6C;
extern s32 D_8014636C;
extern s32 D_801465F8;
extern s32 D_801492C0;
extern s32 D_8014ABD0;
extern s32 D_8014AF28;
extern s32 D_8014C08C;

void ovl_load_reset(void) {
    ovl_load_screen_mode = 0;
    ovl_load_state = 0;
    ovl_load_sectors = 0;
    ovl_load_buf = 0;
    ovl_load_lba = 0;
}

void ovl_loader_mode_handler(void) {
    u8 sel = ovl_load_state; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        ovl_load_start();
        break;
    case 1:
        ovl_load_poll();
        break;
    case 2:
        ovl_load_verify();
        break;
    case 3:
        ovl_load_enter();
        break;
    }
    ovl_load_screen_draw();
}

void ovl_load_screen_draw(void) {
    s32 pad; /* FAKE: unused local above the spill slot of n, the original has the spill at sp+0x28 (T-3330) */
    s32 n;

    n = 0;
    if (D_8011F113 & 0x80) {
        hizuke_show();
        n = 1;
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
        n += 1;
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
        n += 1;
    }
    if (*D_8011F3FF & 0x80) {
        hud_mascot_update();
        n += 1;
    }
    if (D_8011ECD3 & 0x80) {
        hud_cursor_update();
        n += 1;
    }
    if (game_state.ovl_target_game_mode == GAME_MODE_TELEPHONE_PROFILE) {
        cal_base_show();
    }
    if (n != 0) {
        hud_screen_frame_draw(0);
        switch (ovl_load_screen_mode) {
        case 0:
            if (D_80122D44 & 1) {
                hud_sprite_light_effect_draw();
            }
            if (D_80122D44 & 2) {
                message_window_title_update();
            }
            dec_bg_draw(get_last_gamen_mode());
            break;
        case 1:
            bg_wipe_step();
            dec_bg_draw(1);
            break;
        default:
            back_clear_switch(1);
            break;
        }
    }
}

/* ovl_load_lba is assigned in every case and read after the switch: uopt promotes it and writes it back in each
 * branch. The s32 result has no return on the second path (K&R), which keeps v0 live at the exit. T-16220, T-16930 */
s32 ovl_load_start(void) {
    GameState *g = &game_state;
    s32 resident_lba;

    resident_lba = ovl_load_lba;
    back_clear_switch(1);
    ovl_load_buf = 0x80132000;
    ovl_load_screen_mode = 0;
    switch (g->ovl_target_game_mode) {
    case 8:
    case GAME_MODE_FILE_SELECT:
    case GAME_MODE_EXAM:
    case GAME_MODE_DATE:
    case GAME_MODE_RPG_BATTLE:
        break;
    case GAME_MODE_VALENTINE:
    case GAME_MODE_SEASONAL_PARTY:
    case GAME_MODE_GIRL_ENCOUNTER:
    case GAME_MODE_GIRL_INVITATION:
    case GAME_MODE_MEETING:
    case GAME_MODE_SPECIAL_EVENT:
    case GAME_MODE_SPORTS_FESTIVAL:
        bg_capture_screen(0xA0);
        ovl_load_screen_mode = 1;
        break;
    }
    if ((g->player_flags.b[1] & 0xF) == 3) {
        sprite_quads[0].flags = 0x80000000;
    }
    switch (g->ovl_target_game_mode) {
    case GAME_MODE_HELP:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x96AA;
        break;
    case GAME_MODE_FESTIVAL_STALLS:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x934A;
        break;
    case GAME_MODE_CLUB_MASTER_A:
    case GAME_MODE_CLUB_MASTER_B:
    case GAME_MODE_CLUB_MASTER_C:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x958A;
        break;
    case GAME_MODE_RPG_BATTLE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x982A;
        break;
    case GAME_MODE_CULTURE_STAGE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x91CA;
        break;
    case GAME_MODE_SPORTS_FESTIVAL:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x99AA;
        break;
    case GAME_MODE_CLUB_CAMP:
    case GAME_MODE_CLUB_CAMP_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x952A;
        break;
    case GAME_MODE_PRACTICE_MATCH:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x97CA;
        break;
    case GAME_MODE_CULTURE_FESTIVAL_A:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x916A;
        break;
    case GAME_MODE_CULTURE_FESTIVAL_B:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x910A;
        break;
    case GAME_MODE_OPENING:
    case GAME_MODE_FILE_SELECT:
    case GAME_MODE_NAME_ENTRY:
    case GAME_MODE_PROLOGUE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x95EA;
        break;
    case GAME_MODE_TELEPHONE:
    case GAME_MODE_TELEPHONE_PROFILE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A0A;
        break;
    case GAME_MODE_ENDING:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x92EA;
        break;
    case GAME_MODE_EXAM:
    case GAME_MODE_CAREER_GUIDANCE:
    case GAME_MODE_CAREER_CHOICE:
    case GAME_MODE_ALBUM:
    case GAME_MODE_ETC_C2:
    case GAME_MODE_ETC_C3:
    case GAME_MODE_ETC_C4:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x93AA;
        break;
    case GAME_MODE_DATE:
    case GAME_MODE_DATE_SCENE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x922A;
        break;
    case GAME_MODE_AMUSEMENT_PARK:
    case GAME_MODE_AMUSEMENT_PARK_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x928A;
        break;
    case GAME_MODE_SICK_VISIT:
    case GAME_MODE_SICK_VISIT_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x970A;
        break;
    case GAME_MODE_PLAYER_BIRTHDAY:
    case GAME_MODE_GIRL_BIRTHDAY:
    case GAME_MODE_GIRL_ENCOUNTER:
    case GAME_MODE_GIRL_INVITATION:
    case GAME_MODE_SPECIAL_EVENT:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x946A;
        break;
    case GAME_MODE_SCHOOL_TRIP:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x98EA;
        break;
    case GAME_MODE_VALENTINE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9ACA;
        break;
    case GAME_MODE_NEW_YEAR_A:
    case GAME_MODE_NEW_YEAR_B:
    case GAME_MODE_SEASONAL_PARTY:
    case GAME_MODE_MEETING:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x988A;
        break;
    case GAME_MODE_OPTION:
    case GAME_MODE_OPTION_2:
    case GAME_MODE_OPTION_3:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x976A;
        break;
    case GAME_MODE_TT_GAME:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        break;
    default:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x994A;
        break;
    }
    if (resident_lba == ovl_load_lba) {
        ovl_load_state = 3;
        return 0;
    }
    cd_read_sectors(0x60, ovl_load_buf, ovl_load_lba);
    ovl_load_state = 1;
}

void ovl_load_poll(void) {
    if (cd_read_sync_status() & 1) {
        ovl_load_state = 2;
    }
}

void ovl_load_verify(void) {
    if (cd_file_checksum_ok(ovl_load_sectors, ovl_load_buf) != 0) {
        ovl_load_state = 3;
        sys_cache_flush();
    } else {
        ovl_load_state = 0;
        ovl_load_lba = 0;
    }
}

void ovl_load_enter(void) {
    GameState *gs; /* FAKE: T-22002, the switch and the arg read go through gs: it keeps the selector in $v1,
                    * the arg load above the unk_1108 store, and takes frame word 0x34 */
    u8 pad; /* FAKE: T-22002, never used; holds frame byte 0x33 so the four saved bytes sit at 0x2F-0x32 */
    u8 saved_mode_step;
    u8 saved_substep;
    u8 saved_game_mode_scratch;
    u8 saved_mode_stage_scratch;
    s32 arg; /* s32 and passed as is, so its range is precoloured $a0 (a u8 converted at the call took $v0) */

    gs = &game_state;
    switch (gs->ovl_target_game_mode) {
    case GAME_MODE_HELP:
        ovl_entry = (u32) &D_80132000;
        break;
    case GAME_MODE_FESTIVAL_STALLS:
        ovl_entry = (u32) &D_80132000;
        break;
    case GAME_MODE_CLUB_MASTER_C:
        ovl_entry = (u32) &D_80132140;
        break;
    case GAME_MODE_CLUB_MASTER_B:
        ovl_entry = (u32) &D_80139150;
        break;
    case GAME_MODE_CLUB_MASTER_A:
        ovl_entry = (u32) &D_80133A2C;
        break;
    case GAME_MODE_CULTURE_STAGE:
        ovl_entry = (u32) &D_80137890;
        break;
    case GAME_MODE_RPG_BATTLE:
        ovl_entry = (u32) &D_8013B5B0;
        break;
    case GAME_MODE_SPORTS_FESTIVAL:
        ovl_entry = (u32) &D_80141280;
        break;
    case GAME_MODE_CLUB_CAMP_2:
        ovl_entry = (u32) &D_80132000;
        break;
    case GAME_MODE_CLUB_CAMP:
        ovl_entry = (u32) &D_80132FF8;
        break;
    case GAME_MODE_PRACTICE_MATCH:
        ovl_entry = (u32) &D_80132040;
        break;
    case GAME_MODE_CULTURE_FESTIVAL_A:
        ovl_entry = (u32) &D_80142700;
        break;
    case GAME_MODE_CULTURE_FESTIVAL_B:
        ovl_entry = (u32) &D_80144F70;
        break;
    case GAME_MODE_OPENING:
        ovl_entry = (u32) &D_80144AD0;
        break;
    case GAME_MODE_NAME_ENTRY:
        ovl_entry = (u32) &D_8013209C;
        break;
    case GAME_MODE_PROLOGUE:
        ovl_entry = (u32) &D_8014AF28;
        break;
    case GAME_MODE_FILE_SELECT:
        ovl_entry = (u32) &D_8014231C;
        break;
    case GAME_MODE_TELEPHONE:
        ovl_entry = (u32) &D_801397D0;
        break;
    case GAME_MODE_TELEPHONE_PROFILE:
        ovl_entry = (u32) &D_80132000;
        break;
    case GAME_MODE_ENDING:
        ovl_entry = (u32) &D_80132334;
        break;
    case GAME_MODE_ETC_C4:
        ovl_entry = (u32) &D_80145A6C;
        break;
    case GAME_MODE_CAREER_CHOICE:
        ovl_entry = (u32) &D_801492C0;
        break;
    case GAME_MODE_CAREER_GUIDANCE:
        ovl_entry = (u32) &D_8014ABD0;
        break;
    case GAME_MODE_ETC_C2:
        ovl_entry = (u32) &D_80142974;
        break;
    case GAME_MODE_ALBUM:
        ovl_entry = (u32) &D_801407F0;
        break;
    case GAME_MODE_ETC_C3:
        ovl_entry = (u32) &D_8014636C;
        break;
    case GAME_MODE_EXAM:
        ovl_entry = (u32) &D_801465F8;
        break;
    case GAME_MODE_DATE_SCENE:
        ovl_entry = (u32) &D_8014C08C;
        break;
    case GAME_MODE_DATE:
        ovl_entry = (u32) &D_801448D4;
        break;
    case GAME_MODE_AMUSEMENT_PARK_2:
        ovl_entry = (u32) &D_80137F64;
        break;
    case GAME_MODE_AMUSEMENT_PARK:
        ovl_entry = (u32) &D_80132214;
        break;
    case GAME_MODE_SICK_VISIT_2:
        ovl_entry = (u32) &D_80132000;
        break;
    case GAME_MODE_SICK_VISIT:
        ovl_entry = (u32) &D_80133120;
        break;
    case GAME_MODE_GIRL_ENCOUNTER:
        ovl_entry = (u32) &D_80132244;
        break;
    case GAME_MODE_PLAYER_BIRTHDAY:
        ovl_entry = (u32) &D_80134040;
        break;
    case GAME_MODE_GIRL_BIRTHDAY:
        ovl_entry = (u32) &D_80134C90;
        break;
    case GAME_MODE_SPECIAL_EVENT:
        ovl_entry = (u32) &D_8013882C;
        break;
    case GAME_MODE_GIRL_INVITATION:
        ovl_entry = (u32) &D_801426D0;
        break;
    case GAME_MODE_SCHOOL_TRIP:
        ovl_entry = (u32) &D_80137490;
        break;
    case GAME_MODE_VALENTINE:
        ovl_entry = (u32) &D_80132274;
        break;
    case GAME_MODE_NEW_YEAR_A:
        ovl_entry = (u32) &D_8013D180;
        break;
    case GAME_MODE_NEW_YEAR_B:
        ovl_entry = (u32) &D_801322D4;
        break;
    case GAME_MODE_SEASONAL_PARTY:
        ovl_entry = (u32) &D_8013B484;
        break;
    case GAME_MODE_MEETING:
        ovl_entry = (u32) &D_80137144;
        break;
    case GAME_MODE_OPTION:
        ovl_entry = (u32) &D_801335E8;
        break;
    case GAME_MODE_OPTION_2:
        ovl_entry = (u32) &D_801389A0;
        break;
    case GAME_MODE_OPTION_3:
        ovl_entry = (u32) &D_8013C780;
        break;
    default:
        ovl_entry = (u32) &D_80132000;
        break;
    }
    ovl_load_state = 0;
    saved_mode_step = game_state.mode_step;
    saved_substep = game_state.substep;
    /* FAKE: T-17003, unk_110B/unk_110C are s8 in GameState but the original loads them with lbu,
     * so they are read through a u8 view (through gs they compile to lb). */
    saved_mode_stage_scratch = *(u8 *)&game_state.mode_stage_scratch;
    saved_game_mode_scratch = *(u8 *)&game_state.game_mode_scratch;
    arg = gs->mode_stage;
    game_state.game_mode = game_state.ovl_target_game_mode;
    mode_stage_set(arg);
    mode_step_set((s32) saved_mode_step);
    game_state.substep = saved_substep;
    game_state.mode_stage_scratch = saved_mode_stage_scratch;
    game_state.game_mode_scratch = saved_game_mode_scratch;
}

s32 game_mode_is_tt_game(void) {
    if (game_state.game_mode == 0xD0 || game_state.game_mode == 0xD1) {
        return 1;
    }
    return 0;
}

s32 game_mode_is_shooter(void) {
    if (game_state.game_mode == 0x90) {
        return 1;
    }
    return 0;
}

s32 game_mode_is_option_2(void) {
    if (game_state.game_mode == 0x21) {
        return 1;
    }
    return 0;
}

void game_mode_tt_game_mode_handler(void) {
    ovl_load_buf = 0x80132000;
    switch (game_state.mode_stage) {
    case 0:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        debug_hud_enabled = 0;
        vram_clear_blocks(0xFF);
        actor_clear_range(0);
        sprite_tables_clear();
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        cd_read_sectors(0x60U, ovl_load_buf, 0x9A6A);
        mode_stage_next();
    default:
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            mode_stage_next();
        }
        break;
    case 2:
        if (cd_file_checksum_ok((s32) ovl_load_sectors, 0x80132000) != 0) {
            mode_stage_next();
            sys_cache_flush();
            func_8009C5E0(0);
        } else {
            mode_stage_set(0);
        }
        break;
    case 3:
        ovl_entry = (u32) &D_8013E850;
        func_8009C674(0);
        func_8009C210(3);
        CloseEvent(timer_event);
        ((s32 (*)()) ovl_entry)();
        vram_clear_blocks(0xFF);
        mode_stage_next();
        break;
    case 4:
        func_8009C674(0);
        func_8009C210(3);
        timer_event_start();
        audio_bank_state_reset();
        dec_bg_reset();
        cd_command_reset();
        audio_command(0x7F);
        cd_load_destination_base_set(0x801B8000);
        mode_stage_next();
        break;
    case 5:
        game_mode_tt_game_save_step();
        break;
    case 6:
        if (game_state.options_dirty == 0) {
            game_mode_set((s32) game_state.saved_game_mode);
            mode_stage_set((s32) game_state.saved_mode_stage);
            mode_step_set((s32) game_state.saved_mode_step);
        } else {
            game_mode_set(GAME_MODE_BOOT);
            actor_clear_range(0);
            draw2d3d(0U, 0U);
            sprite_tables_clear();
            tpage_buf_clear();
        }
        break;
    }
}

void game_mode_tt_game_save_step(void) {
    GameState *gs = &game_state;
    s32 temp;

    switch (gs->substep) {
    case 0:
        memory_card_start();
        gs->substep += 1;
        break;
    case 1:
        if (memory_card_check_poll() != 0) {
            /* FAKE: the first read of unk_1115 goes through a local so the three tests share a
             * register range that takes $v1 (the TACO func_80142AF0 idiom); T-17003 */
            temp = gs->memory_card_result;
            if (temp == 1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xF1) {
                gs->substep += 1;
            } else if (gs->memory_card_result == 0xFF) {
                gs->substep += 1;
            }
        } else if ((u32) gs->memory_card_timer >= 0x201U) {
            gs->substep += 1;
        }
        break;
    case 2:
        settings_save_load_layout_stash(0);
        memory_card_stop();
        if (gs->memory_card_result == 1) {
            if (memory_card_file_exists(0) == 1) {
                gs->substep += 1;
                memory_card_start();
            } else {
                gs->substep = 0xFF;
            }
        } else {
            gs->substep = 0xFF;
        }
        break;
    case 3:
        if (memory_card_system_read_poll() == 3) {
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32) gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            memory_card_stop();
            gs->substep = 0xFF;
        }
        break;
    case 4:
        if (memory_card_system_bank_load(0) == -1) {
            gs->substep = 0xFF;
        } else {
            gs->substep += 1;
        }
        settings_save_load_layout_stash(1);
        break;
    case 5:
        memory_card_start();
        D_800E7D36 = 0;
        gs->substep += 1;
        break;
    case 6:
        if (memory_card_system_write_poll() == 0x13) {
            audio_command(0x50F);
            memory_card_stop();
            gs->substep += 1;
        } else if (((u32) gs->memory_card_timer >= 0x201U) || (gs->memory_card_result == 0xF0)) {
            audio_command(0x502);
            memory_card_stop();
            gs->substep += 1;
        }
        break;
    case 0xFF:
    default:
        mode_stage_next();
        break;
    }
}
