/* literature_speech_contest.c: src/ovl/BUNKA_SD/80132000.c:4,31,65; literature show loading, speaker/script selection and input flow; club/year dispatch is proved by src/ovl/BUNKA_SD/club_year_show_dispatch.c:14-51,63-89. Original object 0x80132000-0x80133540 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "game.h"
#include "ovl/BUNKA_SD.h"

s32 func_80132000(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(1, 0xBEFB, 0xBEE2, 0xD258, 0xD208, 0xD200);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 1) {
        if (audio_bank_load_poll() == 1) {
            game_state.substep = game_state.substep + 1;
        }
    } else if (game_state.substep == 2) {
        if (func_80044F94(1) == 1) {
            game_state.substep = game_state.substep + 1;
        }
    } else if (game_state.substep == 3) {
        cd_read_sectors(0x52, 0x80180000, 0x4B82);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 4) {
        if (cd_read_sync_status() & 1) {
            mode_step_next();
        }
    }
}

void func_80132148(void) {
    switch (D_80122EAC) {
    case 0:
        D_8013B7F0 = D_8013B74C;
        break;
    case 1:
        D_8013B7F0 = D_8013B794;
        D_8013B7F4 = D_8013B7B0;
        game_state.current_girl = GIRL_SOTOI;
        cd_audio_request_set(0x1E00004C, 0x10000051, 0xE);
        audio_command(0x300);
        break;
    case 2:
        D_8013B7F0 = D_8013B670;
        D_8013B7F4 = D_8013B6B4;
        game_state.current_girl = GIRL_SHIORI;
        cd_audio_request_set(0x1C0002E9, 0x410002F3, 0);
        audio_command(0x300);
        break;
    case 3:
        D_8013B7F0 = D_8013B6E4;
        D_8013B7F4 = D_8013B71C;
        game_state.current_girl = GIRL_MIO;
        cd_audio_request_set(0x1A0001B5, 0x150001CA, 1);
        audio_command(0x300);
        break;
    case 4:
    default:
        D_8013B7F0 = D_8013B7C8;
        D_8013B7F4 = D_8013B7DC;
        game_state.current_girl = GIRL_MIHARU;
        cd_audio_request_set(0x1F0000AD, 0x100000B3, 0xB);
        audio_command(0x300);
        break;
    }
    mode_step_next();
}

void func_801322EC(void) {
    s32 i;

    if (game_state.mode_step >= 5U && game_state.mode_step < 15U) {
        if (game_state.pad_held & PAD_CIRCLE) {
            if (cd_audio_state() != 2) {
                audio_command(0x74);
            }
            func_80132428();
            for (i = 0; i < 10; i++) {
                actor_pool[(i * 0x44) + 0x1986] = 0x82;
            }
        } else {
            for (i = 0; i < 10; i++) {
                actor_pool[(i * 0x44) + 0x1986] = 1;
            }
        }
    }
    if (game_state.mode_step >= 5U && game_state.mode_step < 14U && (game_state.pad_held & PAD_CROSS)) {
        audio_command(0xC4);
        mode_step_set(13);
    }
    func_80132428();
}

void func_80132428(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        audio_command(0xC1);
        func_80134260();
        break;
    case 1:
        if ((u32) game_state.mode_step_timer.u >= 0x1FU) {
            mode_step_next();
        }
        break;
    case 2:
        func_80132000();
        break;
    case 3:
        func_80132148();
        break;
    case 4:
        func_801325E8();
        audio_command(0xE4);
        break;
    case 5:
        func_80134418();
        break;
    case 6:
        func_8013261C();
        break;
    case 7:
        func_80132708();
        func_80132DFC();
        break;
    case 8:
        func_80132DB8(0xF);
        break;
    case 9:
        func_80132708();
        func_80132FF0();
        break;
    case 10:
        func_80132DB8(0x1E);
        break;
    case 11:
        func_80132708();
        func_80133204();
        break;
    case 12:
        func_801327E4();
        break;
    case 13:
        k_reset(1);
        mode_step_next();
        break;
    case 14:
        func_801344A8();
        break;
    case 15:
        func_80134314();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    game_state.mode_step_timer.u += 1;
}


void func_801325E8(void) {
    func_80132940();
    k_speed_set(TEXT_SPEED_EVERY_SIX_FRAMES);
    D_8013B7EC = 0;
    mode_step_next();
}

void func_8013261C(void) {
    /* FAKE: the read through a pointer makes uopt compare constant-first as the original does (its D_801206F0 is
     * probably a member of an aggregate we do not know); real source unknown. T-16930 */
    s16 *state = &D_801206F0;

    if (game_state.mode_step_timer.w == 1) {
        D_8013B808 = 0;
        D_801206DB = 0x84;
        D_801206DA = 5;
    }
    if ((D_80122EAC != 4) && (*state == 0x14)) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            audio_command(0x501);
        }
    }
    if ((D_80122EAC == 4) && (*state == 5)) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            audio_command(0x501);
        }
    }
    if (!((u8) D_801206DA & 1)) {
        mode_step_next();
    }
}

s32 func_80132708(void) {
    u8 v;
    u32 q;
    s32 t;

    v = *(u8 *)&game_state.mode_stage_scratch;
    D_801206DA = 0x40;
    q = v / 14;
    if (q >= 2) {
        v = 0;
        q = 0;
    }
    t = q * 2;
    /* FAKE: dead test keeps q * 2 computed once before the switch (T-10310) */
    if (t < 0) {
    }
    switch (D_80122EAC) {
    case 0:
        D_801206F0 = 0x19 - t;
        break;
    case 1:
        D_801206F0 = 0x45 - t;
        break;
    case 2:
        D_801206F0 = 0x71 - t;
        break;
    case 3:
        D_801206F0 = 0x9D - t;
        break;
    case 4:
    default:
        D_801206F0 = 0xBD - t;
        break;
    }
    v = v + 1;
    game_state.mode_stage_scratch = v;
}

void func_801327E4(void) {
    if (game_state.mode_step_timer.w == 1) {
        D_8013B80C = 0;
        D_801206DB = 4;
        D_8012071F = 0x84;
        D_8012071E = 5;
    }
    if ((D_80122EAC != 3) && (D_80122EAC != 4)) {
        if (D_80120734 == 0) {
            D_8013B80C += 1;
            if (D_8013B80C == 0x14) {
                audio_command(0x501);
            }
        }
    }
    if ((D_80122EAC == 3) && (D_80120734 == 0xB)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            audio_command(0x501);
        }
    }
    if ((D_80122EAC == 4) && (D_80120734 == 5)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            audio_command(0x501);
        }
    }
    if (!((u8) D_8012071E & 1)) {
        mode_step_next();
    }
}

void func_80132940(void) {
    u16 frame;
    s32 i;
    Actor *record;

    i = 0;
    do {
        load_palette(D_8013B804, (i + 0x10) & 0xFF, 1, 1, i);
        i += 1;
    } while (i < 0x10);
    actor_clear(0x60);
    ((Actor *)actor_pool_96)[0].unk_01 = 5;
    ((Actor *)actor_pool_96)[0].quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)[0].draw_flags = 0x84;
    ((Actor *)actor_pool_96)[0].flags = 0x40;
    ((Actor *)actor_pool_96)[0].frame_table = D_8013B7FC;
    ((Actor *)actor_pool_96)[0].sequence_table = D_8013B800;
    ((Actor *)actor_pool_96)[0].tpage_table = D_8013B7F8;
    ((Actor *)actor_pool_96)[0].unk_18 = 0xC4;
    ((Actor *)actor_pool_96)[0].brightness = 0;
    ((Actor *)actor_pool_96)[0].x = -0xA0;
    ((Actor *)actor_pool_96)[0].y = -0x78;
    ((Actor *)actor_pool_96)[0].unk_05 = 0x80;
    ((Actor *)actor_pool_96)[0].unk_43 = 0;
    ((Actor *)actor_pool_96)[0].unk_04 = 0;
    actor_clear(0x61);
    ((Actor *)actor_pool_96)[1].unk_01 = 4;
    ((Actor *)actor_pool_96)[1].quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)[1].draw_flags = 0x84;
    ((Actor *)actor_pool_96)[1].flags = 0x40;
    ((Actor *)actor_pool_96)[1].frame_table = D_8013B7FC;
    ((Actor *)actor_pool_96)[1].sequence_table = D_8013B800;
    ((Actor *)actor_pool_96)[1].tpage_table = D_8013B7F8;
    ((Actor *)actor_pool_96)[1].unk_18 = 0;
    ((Actor *)actor_pool_96)[1].brightness = 0;
    ((Actor *)actor_pool_96)[1].x = -0xA0;
    ((Actor *)actor_pool_96)[1].y = -0x78;
    ((Actor *)actor_pool_96)[1].unk_05 = 0x80;
    ((Actor *)actor_pool_96)[1].unk_43 = 0;
    ((Actor *)actor_pool_96)[1].unk_04 = 0;
    record = (Actor *)D_8011ED58;
    i = 2;
    do {
        actor_clear(i + 0x60);
        record[96].unk_01 = 4;
        record[96].quad_flags = 0x01000000;
        record[96].draw_flags = 4;
        record[96].frame_table = D_8013B7FC;
        record[96].sequence_table = D_8013B800;
        record[96].tpage_table = D_8013B7F8;
        record++;
        frame = D_80122EAC * 2 + i;
        i += 1;
        record[95].sequence_slot = frame - 2;
        record[95].blink_period = 1;
        record[95].brightness = 0;
        record[95].x = -0xA0;
        record[95].y = -0x78;
        record[95].unk_05 = 0x80;
        record[95].unk_43 = 0;
        record[95].unk_04 = 0;
    } while (i != 4);
    actor_clear(0x64);
    ((Actor *)actor_pool_96)[4].unk_01 = 4;
    ((Actor *)actor_pool_96)[4].quad_flags = 0x01000000;
    ((Actor *)actor_pool_96)[4].draw_flags = 0x84;
    ((Actor *)actor_pool_96)[4].flags = 0x40;
    ((Actor *)actor_pool_96)[4].frame_table = D_8013B7FC;
    ((Actor *)actor_pool_96)[4].sequence_table = D_8013B800;
    ((Actor *)actor_pool_96)[4].tpage_table = D_8013B7F8;
    switch (D_80122EAC) {
    case 0:
        ((Actor *)actor_pool_96)[4].unk_18 = 0xBF;
        break;
    case 1:
        ((Actor *)actor_pool_96)[4].unk_18 = 0xC2;
        break;
    case 2:
        ((Actor *)actor_pool_96)[4].unk_18 = 0xC0;
        break;
    case 3:
        ((Actor *)actor_pool_96)[4].unk_18 = 0xC1;
        break;
    case 4:
    default:
        ((Actor *)actor_pool_96)[4].unk_18 = 0xC3;
        break;
    }
    ((Actor *)actor_pool_96)[4].brightness = 0;
    ((Actor *)actor_pool_96)[4].x = -0xA0;
    ((Actor *)actor_pool_96)[4].y = -0x78;
    ((Actor *)actor_pool_96)[4].unk_05 = 0x80;
    ((Actor *)actor_pool_96)[4].unk_43 = 0;
    ((Actor *)actor_pool_96)[4].unk_04 = 0;
    load_tpage_buf_lock(5, ((s32 *)D_8013B7F8)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_8013B7F8)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_8013B7F8)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_8013B7F8)[3], 1);
    load_tpage_buf_lock(9, ((s32 *)D_8013B7F8)[4], 1);
    load_tpage_buf_lock(0xA, ((s32 *)D_8013B7F8)[5], 1);
    load_tpage_buf_lock(0x25, ((s32 *)D_8013B7F8)[6], 1);
    load_tpage_buf_lock(0x26, ((s32 *)D_8013B7F8)[7], 1);
    load_tpage_buf_lock(0x27, ((s32 *)D_8013B7F8)[8], 1);
}


void func_80132DB8(u32 arg0) {
    func_80132708();
    if (arg0 < (u32)game_state.mode_step_timer.u) {
        mode_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139910);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139920);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139938);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139958);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139970);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139988);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139998);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_801399B4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_801399C8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_801399E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A54);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139A9C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139ABC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139AD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139AE4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B44);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B64);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139B88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139BA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139BBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139BDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139BF8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C38);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139C8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139CA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139CBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139CD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139CF0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139D18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139D30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139D48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139D74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139D8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139DA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139DC0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139DD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139DF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E34);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139E94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139EA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139EC4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139EDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/literature_speech_contest.rodata", D_80139EF4);

void func_80132DFC(void) {
    if (game_state.mode_step_timer.w == 1) {
        set_kanji_string(-0x78, 0x30, 0, (s32) "題目", 0);
        switch (D_80122EAC) {
        case 0:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「僕の高校生活」", 0);
            break;
        case 1:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「僕の存在」", 0);
            if (!(game_state.pad_held & PAD_CIRCLE)) {
                cd_audio_request_set(0x1E00004C, 0x10000051, 0xE);
                audio_command(0x300);
            }
            break;
        case 2:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「高校生活を振り返って」", 0);
            if (!(game_state.pad_held & PAD_CIRCLE)) {
                cd_audio_request_set(0x1C0002E9, 0x410002F3, 0);
                audio_command(0x300);
            }
            break;
        case 3:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「貧血と私」", 0);
            break;
        case 4:
        default:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「届け私の想い」", 0);
            if (!(game_state.pad_held & PAD_CIRCLE)) {
                cd_audio_request_set(0x1F0000AD, 0x100000B3, 0xB);
                audio_command(0x300);
            }
            break;
        }
    } else if ((cd_audio_state() == 2) && (check_end_k() != 0)) {
        mode_step_next();
    }
    check_k_scroll();
    k_disp_inc();
}

void func_80132FF0(void) {
    /* FAKE: 24-byte capacity reproduces observed stack homes; original capacity unknown. T-12090 */
    u8 sp38[24];
    s32 sp34;

    if (game_state.mode_step_timer.u == 1) {
        switch (D_80122EAC) {
        case 0:
            sp34 = strlen(game_state.player_family_name);
            sp34 += strlen(game_state.player_given_name);
            strcpy(sp38, game_state.player_family_name);
            strcat(sp38, "　");
            strcat(sp38, game_state.player_given_name);
            set_kanji_string((s16) (0x60 - (sp34 * 7)), 0x40, 0, (s32) sp38, 0);
            break;
        case 1:
            if (!(game_state.pad_held & PAD_CIRCLE)) {
                cd_audio_request_set(0x0B00001B, 0x05000020, 0xE);
                audio_command(0x300);
            }
            set_kanji_string(0x36, 0x40, 0, (s32) "弁　論者", 0);
            break;
        case 2:
            if (!(game_state.pad_held & PAD_CIRCLE)) {
                cd_audio_request_set(0x1D00005B, 0x0B000061, 0);
                audio_command(0x300);
            }
            set_kanji_string(0x28, 0x40, 0, (s32) "藤崎　詩織", 0);
            break;
        case 3:
            set_kanji_string(0x28, 0x40, 0, (s32) "如月　未緒", 0);
            break;
        case 4:
        default:
            set_kanji_string(0x44, 0x40, 0, (s32) "女生徒", 0);
            break;
        }
    } else if ((cd_audio_state() == 2) && (check_end_k() != 0)) {
        mode_step_next();
    }
    check_k_scroll();
    k_disp_inc();
}


void func_80133204(void) {
    s32 index;
    u8 command;
    s32 *voice;

    if (game_state.mode_step_timer.u == 1) {
        k_reset(1);
        switch (D_80122EAC) {
        case 0:
            set_kanji_string((s16) (-0x68 - (strlen(game_state.player_family_name) * 4)), 0x30, 0, (s32) game_state.player_family_name, 0);
            break;
        case 1:
            set_kanji_string(-0x80, 0x30, 0, (s32) "弁論者", 0);
            break;
        case 2:
            set_kanji_string(-0x78, 0x30, 0, (s32) "藤崎", 0);
            break;
        case 3:
            set_kanji_string(-0x78, 0x30, 0, (s32) "如月", 0);
            break;
        case 4:
        default:
            set_kanji_string(-0x80, 0x30, 0, (s32) "女生徒", 0);
            break;
        }
        set_kanji_string(-0x56, 0x30, 0, (s32) "「", 0);
        index = D_8013B7EC;
        if (index < 0x64) {
            do {
                set_kanji_string(-0x48, (s16) (((index - D_8013B7EC) * 0x10) + 0x30), 0, ((u8 **)D_8013B7F0)[index] + 1, 0);
                command = *((u8 **)D_8013B7F0)[index];
                if (command == 0x67) {
                    if (!(game_state.pad_held & PAD_CIRCLE) && (D_80122EAC != 0)) {
                        voice = (s32 *)D_8013B7F4 + game_state.substep;
                        cd_audio_request_set(voice[0], voice[1], (s32) game_state.current_girl);
                        audio_command(0x300);
                    }
                    game_state.substep += 2;
                    D_8013B7EC = index + 1;
                    index = 0x64;
                } else if (command == 0x45) {
                    if (!(game_state.pad_held & PAD_CIRCLE)) {
                        if (D_80122EAC != 0) {
                            voice = (s32 *)D_8013B7F4 + game_state.substep;
                            cd_audio_request_set(voice[0], voice[1], (s32) game_state.current_girl);
                            audio_command(0x300);
                        }
                    }
                    game_state.substep = 0xFF;
                    D_8013B7EC = index + 1;
                    index = 0x64;
                }
                index += 1;
            } while (index < 0x64);
        }
    }
    if ((cd_audio_state() == 2) && (check_end_k() != 0)) {
        if (game_state.substep == 0xFF) {
            k_reset(1);
            mode_step_next();
        }
        game_state.mode_step_timer.u = 0;
    }
    check_k_scroll();
    k_disp_inc();
}
