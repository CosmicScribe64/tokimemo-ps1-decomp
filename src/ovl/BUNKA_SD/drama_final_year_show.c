/* drama_final_year_show.c: src/ovl/BUNKA_SD/801369D0.c:4; drama show callbacks, event-list interpreter and images; club/year dispatch is proved by src/ovl/BUNKA_SD/club_year_show_dispatch.c:14-51,63-89. Original object 0x801369D0-0x80137890 (config/objects/BUNKA_SD.txt). */
#include "common.h"
#include "ovl/BUNKA_SD.h"

s32 func_801369D0(void) {
    if (game_state.substep == 0) {
        audio_bank_load_start(1, 0xBEFB, 0xBEE2, 0xD19D, 0xD170, 0xD164);
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
        cd_read_sectors(0x6E, 0x80180000, 0x4EAF);
        game_state.substep = game_state.substep + 1;
    } else if (game_state.substep == 4) {
        if (cd_read_sync_status() & 1) {
            D_8013CEC0 = D_8013C750;
            D_8013CEC4 = D_8013C7B4;
            game_state.current_girl = GIRL_SOTOI;
            mode_step_next();
        }
    }
}

void func_80136B3C(void) {
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x26 && (game_state.pad_held & PAD_CIRCLE)) {
        if (cd_audio_state() != 2) {
            audio_command(0x74);
        }
        func_80136BFC();
    }
    if (game_state.mode_step >= 3 && game_state.mode_step < 0x24 && (game_state.pad_held & PAD_CROSS)) {
        audio_command(0xC1);
        audio_command(0xC4);
        mode_step_set(0x23);
    }
    func_80136BFC();
}

void func_80136BFC(void) {
    hud_screen_frame_draw(2);
    message_window_show();
    message_window_title_update();
    switch (game_state.mode_step) {
    case 0:
        func_80134260();
        audio_command(0x7F);
        break;
    case 1:
        func_801369D0();
        break;
    case 2:
        audio_command(0x200);
        func_80136EDC();
        break;
    case 3:
        func_80134418();
        break;
    case 4:
        func_80138898(0x78U);
        break;
    case 5:
        func_801344A8();
        break;
    case 6:
        func_801371BC();
        break;
    case 7:
        func_80134418();
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        func_801373D0();
        break;
    case 12:
        audio_command(0xC1);
        mode_step_next();
        break;
    case 13:
        func_801344A8();
        break;
    case 14:
        func_8013731C();
        break;
    case 15:
        func_8013785C(0x3C);
        break;
    case 16:
        func_8013722C();
        break;
    case 17:
        func_8013785C(0x3C);
        break;
    case 18:
        audio_command(0x201);
        mode_step_next();
        break;
    case 19:
        func_80134418();
        break;
    case 20:
        func_8013785C(0x78);
        break;
    case 21:
        func_801344A8();
        break;
    case 22:
        func_8013785C(0x1E);
        break;
    case 23:
        func_8013731C();
        break;
    case 24:
        func_8013785C(0xF);
        break;
    case 25:
        func_8013731C();
        break;
    case 26:
        func_801372BC();
        break;
    case 27:
        func_80134418();
        break;
    case 28:
    case 29:
    case 30:
        func_801373D0();
        break;
    case 31:
        audio_command(0x202);
        mode_step_next();
        break;
    case 32:
        func_8013785C(0x78);
        break;
    case 33:
    case 34:
        func_801373D0();
        break;
    case 35:
        audio_command(0xC1);
        audio_command(0x500);
        k_reset(1);
        mode_step_next();
        break;
    case 36:
        if ((u32) game_state.mode_step_timer.u >= 0x1FU) {
            mode_step_next();
        }
        break;
    case 37:
        func_801344A8();
        break;
    case 38:
        func_80134314();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    game_state.mode_step_timer.u += 1;
}

void func_80136EDC(void) {
    func_80136F3C();
    k_speed_set(TEXT_SPEED_EVERY_SIX_FRAMES);
    D_8013CEBC = 0;
    D_8013CEC8 = 1;
    D_8013CECC = 0;
    D_8013CED0 = 0;
    D_8013CED4 = 0;
    D_8013CED8 = 0;
    mode_step_next();
}

void func_80136F3C(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013CEE8, (i + 0x10) & 0xFF, 1, 1, i);
    }
    for (i = 0; i < 0x40; i++) {
        actor_clear(i + 0x60);
        ((Actor *)actor_pool)[96 + i].unk_01 = 4;
        ((Actor *)actor_pool)[96 + i].quad_flags = 0x01000000;
        ((Actor *)actor_pool)[96 + i].draw_flags = 0;
        ((Actor *)actor_pool)[96 + i].flags = 0x40;
        ((Actor *)actor_pool)[96 + i].frame_table = D_8013CEE0;
        ((Actor *)actor_pool)[96 + i].sequence_table = D_8013CEE4;
        ((Actor *)actor_pool)[96 + i].tpage_table = (u8 *)D_8013CEDC;
        ((Actor *)actor_pool)[96 + i].unk_18 = 0;
        ((Actor *)actor_pool)[96 + i].blink_period = 1;
        ((Actor *)actor_pool)[96 + i].brightness = 0;
        ((Actor *)actor_pool)[96 + i].x = -0xA0;
        ((Actor *)actor_pool)[96 + i].y = -0x78;
        ((Actor *)actor_pool)[96 + i].unk_05 = 0x80;
        ((Actor *)actor_pool)[96 + i].unk_43 = 0;
        ((Actor *)actor_pool)[96 + i].unk_04 = 0;
    }
    func_801377D4(0, 0x40, 1);
    func_801377D4(0x1F, 0x44, 1);
    load_tpage_buf_lock(5, D_8013CEDC[0], 1);
    load_tpage_buf_lock(6, D_8013CEDC[1], 1);
    load_tpage_buf_lock(7, D_8013CEDC[2], 1);
    load_tpage_buf_lock(8, D_8013CEDC[3], 1);
    load_tpage_buf_lock(9, D_8013CEDC[4], 1);
    load_tpage_buf_lock(0xA, D_8013CEDC[5], 1);
    load_tpage_buf_lock(0x25, D_8013CEDC[6], 1);
    load_tpage_buf_lock(0x26, D_8013CEDC[7], 1);
    load_tpage_buf_lock(0x27, D_8013CEDC[8], 1);
    load_tpage_buf_lock(0x28, D_8013CEDC[9], 1);
    load_tpage_buf_lock(0x29, D_8013CEDC[10], 1);
    load_tpage_buf_lock(0x2A, D_8013CEDC[11], 1);
    load_tpage_buf_lock(0x15, D_8013CEDC[12], 1);
}

void func_801371BC(void) {
    func_801377D4(0, 0x44, 1);
    func_801377D4(1, 0x18, 1);
    func_801377D4(2, 0, 1);
    func_801377D4(3, 1, 1);
    func_801377D4(0x1F, 0x41, 1);
    mode_step_next();
}

void func_8013722C(void) {
    func_801377D4(1, 0xFF, 0);
    func_801377D4(2, 0xFF, 0);
    func_801377D4(3, 0xFF, 0);
    func_801377D4(4, 0xFF, 0);
    func_801377D4(5, 0xFF, 0);
    func_801377D4(0, 0x44, 1);
    func_801377D4(0x1F, 0x42, 1);
    mode_step_next();
}

void func_801372BC(void) {
    func_801377D4(0, 0x44, 1);
    func_801377D4(1, 0x26, 1);
    func_801377D4(2, 0x27, 1);
    func_801377D4(0x1F, 0x43, 1);
    mode_step_next();
}

void func_8013731C(void) {
    if (game_state.mode_step_timer.u == 1) {
        func_80137824(D_8013C814[D_8013CECC]);
        game_state.mode_stage_timer = 1;
        func_80137620();
        return;
    }
    func_80137620();
    if (game_state.mode_stage_timer == 0) {
        D_8013CECC += 1;
        D_8013CEC8 = 1;
        D_8013CED0 = 0;
        D_8013CED4 = 0;
        k_reset(1);
        mode_step_next();
    }
}

s32 func_801373D0(void) {
    BsdEvent *list;
    BsdEvent *ev;
    s32 i;
    s32 slot;
    s32 img;

    if (game_state.mode_stage_timer == 0) {
        D_8013CED8 = 0;
        if (D_8013CED4 != 0) {
            switch (D_8013CED4) {
            case 1:
                game_state.mode_step_timer.w = 0;
                break;
            case 2:
                D_8013CECC++;
                D_8013CED0 = 0;
                D_8013CED4 = 0;
                D_8013CEC8 = 1;
                k_reset(1);
                mode_step_next();
                return 0;
            }
        }
    }
    list = D_8013CE8C[D_8013CECC];
    if (D_8013CED4 == 0) {
        D_8013CED4 = 1;
    }
    if (D_8013CED4 == 1) {
        for (i = D_8013CEC8; i < 0xFFFF; i++) {
            /* FAKE: T-22003, the word view with i * 2 takes the ugen temporary that the
             * original's index expression takes; &list[i] gives $t1 for $t2 in the loop */
            ev = (BsdEvent *)((s32 *)list + i * 2);
            if (D_8013CED0 == ev->unk_00) {
                if (ev->unk_04 == 0x0FFFFFFF) {
                    D_8013CED4 = 2;
                    i = 0xFFFF;
                } else {
                    slot = (ev->unk_04 & 0xFF0000) >> 16;
                    img = ev->unk_04 & 0xFF;
                    if (img == 0xFE) {
                        func_801377D4(slot, 0x50, 0);
                    } else {
                        func_801377D4(slot, img, 1);
                    }
                    if (ev->unk_04 & 0xF0000000) {
                        func_80137824(D_8013C814[D_8013CECC]);
                        game_state.mode_stage_timer = 1;
                        D_8013CED8 = 1;
                    }
                }
            } else {
                D_8013CEC8 = i;
                i = 0xFFFF;
            }
        }
    }
    D_8013CED0++;
    if (D_8013CED8 == 1) {
        func_80137620();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AAD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AAE8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB5C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AB88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ABA0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ABBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ABCC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ABDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ABF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC2C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC60);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AC94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ACB0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ACBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ACD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ACD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013ACF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD58);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD70);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/drama_final_year_show.rodata", D_8013AD88);

void func_80137620(void) {
    s32 index;
    s32 *coordinates;

    if (game_state.mode_stage_timer == 1) {
        game_state.mode_stage_timer++;
        if (D_8013CEBC != 21 && D_8013CEBC != 22) {
            set_kanji_string(-86, 64, 0, (s32)"「", 0);
        }
        index = D_8013CEBC;
        if (D_8013CEBC < 100) {
            do {
                set_kanji_string(-72, (s16)((index - D_8013CEBC) * 16 + 64), 0, (s32)(((u8 **)D_8013CEC0)[index] + 1), 0);
                if (((u8 **)D_8013CEC0)[index][0] == 103) {
                    if (!(game_state.pad_held & PAD_CIRCLE)) {
                        coordinates = (s32 *)D_8013CEC4;
                        coordinates += *(u8 *)&game_state.mode_stage_scratch;
                        cd_audio_request_set(coordinates[0], coordinates[1], game_state.current_girl);
                        audio_command(0x300);
                    }
                    (*(u8 *)&game_state.mode_stage_scratch) += 2;
                    D_8013CEBC = index + 1;
                    index = 100;
                }
                index++;
            } while (index < 100);
        }
    }
    if (cd_audio_state() == 2 && check_end_k() != 0) {
        game_state.mode_stage_timer = 0;
    }
    check_k_scroll();
    k_disp_inc();
}

void func_801377D4(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;

    if (arg2 == 1) {
        p = actor_pool + arg0 * 0x44;
        p[0x1983] = 0x80;
    } else {
        p = actor_pool + arg0 * 0x44;
        p[0x1983] = 0;
    }
    *(s16 *)(p + 0x1998) = arg1;
}

void func_80137824(s32 arg0) {
    k_disp_start(set_kanji_string(-0x80, 0x30, 0, arg0, 0));
}

void func_8013785C(u32 arg0) {
    if (arg0 < (u32) game_state.mode_step_timer.w) {
        mode_step_next();
    }
}
