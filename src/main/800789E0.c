#include "common.h"
#include "game.h"

/* Entry of the overlay that func_8007959C loads (LBA 0x9A6A at 0x80132000); not a main-exe symbol. */
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

void func_800789E0(void) {
    D_800B6D30 = 0;
    ovl_load_state = 0;
    ovl_load_sectors = 0;
    ovl_load_buf = 0;
    ovl_load_lba = 0;
}

void mode_overlay_loader(void) {
    u8 sel = ovl_load_state; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80078C48();
        break;
    case 1:
        ovl_load_poll();
        break;
    case 2:
        ovl_load_verify();
        break;
    case 3:
        func_80079070();
        break;
    }
    func_80078A94();
}

void func_80078A94(void) {
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
        mascot_update();
        n += 1;
    }
    if (D_8011ECD3 & 0x80) {
        cursor_update();
        n += 1;
    }
    if (game_state.overlay_target_mode == MODE_TELEPHONE_PROFILE) {
        cal_base_show();
    }
    if (n != 0) {
        screen_frame_draw(0);
        switch (D_800B6D30) {
        case 0:
            if (D_80122D44 & 1) {
                light_effect_draw();
            }
            if (D_80122D44 & 2) {
                message_window_title_update();
            }
            dec_bg_draw(get_last_gamen_mode());
            break;
        case 1:
            func_80057710();
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
s32 func_80078C48(void) {
    GameState *g = &game_state;
    s32 old;

    old = ovl_load_lba;
    back_clear_switch(1);
    ovl_load_buf = 0x80132000;
    D_800B6D30 = 0;
    switch (g->overlay_target_mode) {
    case 8:
    case MODE_NAME_ENTRY:
    case MODE_EXAM:
    case MODE_DATE:
    case MODE_RPG_BATTLE:
        break;
    case MODE_VALENTINE:
    case MODE_SEASONAL_PARTY:
    case MODE_GIRL_ENCOUNTER:
    case MODE_GIRL_INVITATION:
    case MODE_MEETING:
    case MODE_SPECIAL_EVENT:
    case MODE_SPORTS_FESTIVAL:
        func_80057640(0xA0);
        D_800B6D30 = 1;
        break;
    }
    if ((g->player_flags.b[1] & 0xF) == 3) {
        sprite_quads[0].flags = 0x80000000;
    }
    switch (g->overlay_target_mode) {
    case MODE_HELP:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x96AA;
        break;
    case MODE_FESTIVAL_STALLS:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x934A;
        break;
    case MODE_CLUB_MASTER_A:
    case MODE_CLUB_MASTER_B:
    case MODE_CLUB_MASTER_C:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x958A;
        break;
    case MODE_RPG_BATTLE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x982A;
        break;
    case MODE_CULTURE_STAGE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x91CA;
        break;
    case MODE_SPORTS_FESTIVAL:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x99AA;
        break;
    case MODE_CLUB_CAMP:
    case MODE_CLUB_CAMP_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x952A;
        break;
    case MODE_PRACTICE_MATCH:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x97CA;
        break;
    case MODE_CULTURE_FESTIVAL_A:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x916A;
        break;
    case MODE_CULTURE_FESTIVAL_B:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x910A;
        break;
    case MODE_PROLOGUE:
    case MODE_NAME_ENTRY:
    case MODE_NAME_ENT_3:
    case MODE_NAME_ENT_4:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x95EA;
        break;
    case MODE_TELEPHONE:
    case MODE_TELEPHONE_PROFILE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A0A;
        break;
    case MODE_ENDING:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x92EA;
        break;
    case MODE_EXAM:
    case MODE_CAREER_GUIDANCE:
    case MODE_CAREER_CHOICE:
    case MODE_ALBUM:
    case MODE_ETC_C2:
    case MODE_ETC_C3:
    case MODE_ETC_C4:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x93AA;
        break;
    case MODE_DATE:
    case MODE_DATE_SCENE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x922A;
        break;
    case MODE_AMUSEMENT_PARK:
    case MODE_AMUSEMENT_PARK_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x928A;
        break;
    case MODE_SICK_VISIT:
    case MODE_SICK_VISIT_2:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x970A;
        break;
    case MODE_PLAYER_BIRTHDAY:
    case MODE_GIRL_BIRTHDAY:
    case MODE_GIRL_ENCOUNTER:
    case MODE_GIRL_INVITATION:
    case MODE_SPECIAL_EVENT:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x946A;
        break;
    case MODE_SCHOOL_TRIP:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x98EA;
        break;
    case MODE_VALENTINE:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9ACA;
        break;
    case MODE_NEW_YEAR_A:
    case MODE_NEW_YEAR_B:
    case MODE_SEASONAL_PARTY:
    case MODE_MEETING:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x988A;
        break;
    case MODE_OPTION:
    case MODE_OPTION_2:
    case MODE_OPTION_3:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x976A;
        break;
    case MODE_TT_GAME:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        break;
    default:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x994A;
        break;
    }
    if (old == ovl_load_lba) {
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
    if (file_checksum_ok(ovl_load_sectors, ovl_load_buf) != 0) {
        ovl_load_state = 3;
        func_80042458();
    } else {
        ovl_load_state = 0;
        ovl_load_lba = 0;
    }
}

void func_80079070(void) {
    GameState *gs; /* FAKE: T-22002, the switch and the arg read go through gs: it keeps the selector in $v1,
                    * the arg load above the unk_1108 store, and takes frame word 0x34 */
    u8 pad; /* FAKE: T-22002, never used; holds frame byte 0x33 so the four saved bytes sit at 0x2F-0x32 */
    u8 sp32;
    u8 sp31;
    u8 sp30;
    u8 sp2F;
    s32 arg; /* s32 and passed as is, so its range is precoloured $a0 (a u8 converted at the call took $v0) */

    gs = &game_state;
    switch (gs->overlay_target_mode) {
    case MODE_HELP:
        overlay_entry = (u32) &D_80132000;
        break;
    case MODE_FESTIVAL_STALLS:
        overlay_entry = (u32) &D_80132000;
        break;
    case MODE_CLUB_MASTER_C:
        overlay_entry = (u32) &D_80132140;
        break;
    case MODE_CLUB_MASTER_B:
        overlay_entry = (u32) &D_80139150;
        break;
    case MODE_CLUB_MASTER_A:
        overlay_entry = (u32) &D_80133A2C;
        break;
    case MODE_CULTURE_STAGE:
        overlay_entry = (u32) &D_80137890;
        break;
    case MODE_RPG_BATTLE:
        overlay_entry = (u32) &D_8013B5B0;
        break;
    case MODE_SPORTS_FESTIVAL:
        overlay_entry = (u32) &D_80141280;
        break;
    case MODE_CLUB_CAMP_2:
        overlay_entry = (u32) &D_80132000;
        break;
    case MODE_CLUB_CAMP:
        overlay_entry = (u32) &D_80132FF8;
        break;
    case MODE_PRACTICE_MATCH:
        overlay_entry = (u32) &D_80132040;
        break;
    case MODE_CULTURE_FESTIVAL_A:
        overlay_entry = (u32) &D_80142700;
        break;
    case MODE_CULTURE_FESTIVAL_B:
        overlay_entry = (u32) &D_80144F70;
        break;
    case MODE_PROLOGUE:
        overlay_entry = (u32) &D_80144AD0;
        break;
    case MODE_NAME_ENT_3:
        overlay_entry = (u32) &D_8013209C;
        break;
    case MODE_NAME_ENT_4:
        overlay_entry = (u32) &D_8014AF28;
        break;
    case MODE_NAME_ENTRY:
        overlay_entry = (u32) &D_8014231C;
        break;
    case MODE_TELEPHONE:
        overlay_entry = (u32) &D_801397D0;
        break;
    case MODE_TELEPHONE_PROFILE:
        overlay_entry = (u32) &D_80132000;
        break;
    case MODE_ENDING:
        overlay_entry = (u32) &D_80132334;
        break;
    case MODE_ETC_C4:
        overlay_entry = (u32) &D_80145A6C;
        break;
    case MODE_CAREER_CHOICE:
        overlay_entry = (u32) &D_801492C0;
        break;
    case MODE_CAREER_GUIDANCE:
        overlay_entry = (u32) &D_8014ABD0;
        break;
    case MODE_ETC_C2:
        overlay_entry = (u32) &D_80142974;
        break;
    case MODE_ALBUM:
        overlay_entry = (u32) &D_801407F0;
        break;
    case MODE_ETC_C3:
        overlay_entry = (u32) &D_8014636C;
        break;
    case MODE_EXAM:
        overlay_entry = (u32) &D_801465F8;
        break;
    case MODE_DATE_SCENE:
        overlay_entry = (u32) &D_8014C08C;
        break;
    case MODE_DATE:
        overlay_entry = (u32) &D_801448D4;
        break;
    case MODE_AMUSEMENT_PARK_2:
        overlay_entry = (u32) &D_80137F64;
        break;
    case MODE_AMUSEMENT_PARK:
        overlay_entry = (u32) &D_80132214;
        break;
    case MODE_SICK_VISIT_2:
        overlay_entry = (u32) &D_80132000;
        break;
    case MODE_SICK_VISIT:
        overlay_entry = (u32) &D_80133120;
        break;
    case MODE_GIRL_ENCOUNTER:
        overlay_entry = (u32) &D_80132244;
        break;
    case MODE_PLAYER_BIRTHDAY:
        overlay_entry = (u32) &D_80134040;
        break;
    case MODE_GIRL_BIRTHDAY:
        overlay_entry = (u32) &D_80134C90;
        break;
    case MODE_SPECIAL_EVENT:
        overlay_entry = (u32) &D_8013882C;
        break;
    case MODE_GIRL_INVITATION:
        overlay_entry = (u32) &D_801426D0;
        break;
    case MODE_SCHOOL_TRIP:
        overlay_entry = (u32) &D_80137490;
        break;
    case MODE_VALENTINE:
        overlay_entry = (u32) &D_80132274;
        break;
    case MODE_NEW_YEAR_A:
        overlay_entry = (u32) &D_8013D180;
        break;
    case MODE_NEW_YEAR_B:
        overlay_entry = (u32) &D_801322D4;
        break;
    case MODE_SEASONAL_PARTY:
        overlay_entry = (u32) &D_8013B484;
        break;
    case MODE_MEETING:
        overlay_entry = (u32) &D_80137144;
        break;
    case MODE_OPTION:
        overlay_entry = (u32) &D_801335E8;
        break;
    case MODE_OPTION_2:
        overlay_entry = (u32) &D_801389A0;
        break;
    case MODE_OPTION_3:
        overlay_entry = (u32) &D_8013C780;
        break;
    default:
        overlay_entry = (u32) &D_80132000;
        break;
    }
    ovl_load_state = 0;
    sp32 = game_state.step;
    sp31 = game_state.substep;
    /* FAKE: T-17003, unk_110B/unk_110C are s8 in GameState but the original loads them with lbu,
     * so they are read through a u8 view (through gs they compile to lb). */
    sp2F = *(u8 *)&game_state.stage_scratch;
    sp30 = *(u8 *)&game_state.mode_scratch;
    arg = gs->stage;
    game_state.mode = game_state.overlay_target_mode;
    main_stage_set(arg);
    main_step_set((s32) sp32);
    game_state.substep = sp31;
    game_state.stage_scratch = sp2F;
    game_state.mode_scratch = sp30;
}

s32 func_80079524(void) {
    if (game_state.mode == 0xD0 || game_state.mode == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (game_state.mode == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (game_state.mode == 0x21) {
        return 1;
    }
    return 0;
}

void func_8007959C(void) {
    ovl_load_buf = 0x80132000;
    switch (game_state.stage) {
    case 0:
        ((ExtraModeFlags *)&D_800E7D4F)->b0 = 1;
        D_800B3220 = 0;
        func_80041168(0xFF);
        actor_clear_range(0);
        sprite_tables_clear();
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        cd_read_sectors(0x60U, ovl_load_buf, 0x9A6A);
        main_stage_next();
    default:
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            main_stage_next();
        }
        break;
    case 2:
        if (file_checksum_ok((s32) ovl_load_sectors, 0x80132000) != 0) {
            main_stage_next();
            func_80042458();
            func_8009C5E0(0);
        } else {
            main_stage_set(0);
        }
        break;
    case 3:
        overlay_entry = (u32) &D_8013E850;
        func_8009C674(0);
        func_8009C210(3);
        CloseEvent(D_8011ECA8);
        ((s32 (*)()) overlay_entry)();
        func_80041168(0xFF);
        main_stage_next();
        break;
    case 4:
        func_8009C674(0);
        func_8009C210(3);
        func_80042488();
        snd_bank_state_reset();
        dec_bg_reset();
        cd_cmd_reset();
        snd_command(0x7F);
        func_800451E0(0x801B8000);
        main_stage_next();
        break;
    case 5:
        func_80079800();
        break;
    case 6:
        if (game_state.options_dirty == 0) {
            main_state_set((s32) game_state.saved_mode);
            main_stage_set((s32) game_state.saved_stage);
            main_step_set((s32) game_state.saved_step);
        } else {
            main_state_set(MODE_BOOT);
            actor_clear_range(0);
            draw2d3d(0U, 0U);
            sprite_tables_clear();
            tpage_buf_clear();
        }
        break;
    }
}

void func_80079800(void) {
    GameState *gs = &game_state;
    s32 temp;

    switch (gs->substep) {
    case 0:
        card_start();
        gs->substep += 1;
        break;
    case 1:
        if (func_80053F04() != 0) {
            /* FAKE: the first read of unk_1115 goes through a local so the three tests share a
             * register range that takes $v1 (the TACO func_80142AF0 idiom); T-17003 */
            temp = gs->card_result;
            if (temp == 1) {
                gs->substep += 1;
            } else if (gs->card_result == 0xF1) {
                gs->substep += 1;
            } else if (gs->card_result == 0xFF) {
                gs->substep += 1;
            }
        } else if ((u32) gs->card_timer >= 0x201U) {
            gs->substep += 1;
        }
        break;
    case 2:
        func_8005C4CC(0);
        card_stop();
        if (gs->card_result == 1) {
            if (func_80054AF4(0) == 1) {
                gs->substep += 1;
                card_start();
            } else {
                gs->substep = 0xFF;
            }
        } else {
            gs->substep = 0xFF;
        }
        break;
    case 3:
        if (func_80054284() == 3) {
            card_stop();
            gs->substep += 1;
        } else if (((u32) gs->card_timer >= 0x201U) || (gs->card_result == 0xF0)) {
            card_stop();
            gs->substep = 0xFF;
        }
        break;
    case 4:
        if (func_800552DC(0) == -1) {
            gs->substep = 0xFF;
        } else {
            gs->substep += 1;
        }
        func_8005C4CC(1);
        break;
    case 5:
        card_start();
        D_800E7D36 = 0;
        gs->substep += 1;
        break;
    case 6:
        if (func_80054388() == 0x13) {
            snd_command(0x50F);
            card_stop();
            gs->substep += 1;
        } else if (((u32) gs->card_timer >= 0x201U) || (gs->card_result == 0xF0)) {
            snd_command(0x502);
            card_stop();
            gs->substep += 1;
        }
        break;
    case 0xFF:
    default:
        main_stage_next();
        break;
    }
}
