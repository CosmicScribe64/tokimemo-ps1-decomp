/* graduation_script_resources.c: ; src/ovl/ENDING/80132000.c:6-35 initializes the graduation dialogue script address tables. Original object 0x80132000-0x80133030 (config/objects/ENDING.txt). */
#define MAIN_API_OVERRIDE_get_now_k /* implicit declaration, as matched (main_api.h: u8(void)) */
#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80132000(void) {
    D_8013C280[0][0] = 0x801F00B8;
    D_8013C280[0][1] = 0x801F05A8;
    D_8013C280[0][2] = 0x801F0C70;
    D_8013C280[0][3] = 0x801F0F34;
    D_8013C280[0][4] = 0x801F14C4;
    D_8013C280[0][5] = 0x801F1FF4;
    D_8013C280[0][6] = 0x801F29EC;
    D_8013C280[0][7] = 0x801F32F8;
    D_8013C280[0][8] = 0x801F4100;
    D_8013C280[0][9] = 0x801F4D18;
    D_8013C280[0][10] = 0x801F5BD4;
    D_8013C280[0][11] = 0x801F6698;
    D_8013C280[0][12] = 0x801F6D9C;
    D_8013C280[0][13] = 0x801F70B4;
    D_8013C280[0][14] = 0x801F759C;
    D_8013C280[0][15] = 0x801F7A70;
    D_8013C280[0][16] = 0x801F7F98;
    D_8013C280[1][0] = 0x801F00D0;
    D_8013C280[1][1] = 0x801F05C4;
    D_8013C280[1][2] = 0x801F0C8C;
    D_8013C280[1][3] = 0x801F0F44;
    D_8013C280[1][4] = 0x801F14E4;
    D_8013C280[1][5] = 0x801F2028;
    D_8013C280[1][6] = 0x801F2A0C;
    D_8013C280[1][7] = 0x801F331C;
    D_8013C280[1][8] = 0x801F4144;
    D_8013C280[1][9] = 0x801F4D30;
    D_8013C280[1][10] = 0x801F5C1C;
    D_8013C280[1][11] = 0x801F66B4;
    D_8013C280[1][12] = 0x801F6DB8;
    D_8013C280[1][13] = 0x801F70BC;
    D_8013C280[1][14] = 0x801F75B8;
    D_8013C280[1][15] = 0x801F7AAC;
    D_8013C280[1][16] = 0x801F7FB0;
    D_8013C280[2][0] = 0x801F01A0;
    D_8013C280[2][1] = 0x801F07F8;
    D_8013C280[2][2] = 0x801F0EC0;
    D_8013C280[2][3] = 0x801F0F84;
    D_8013C280[2][4] = 0x801F17DC;
    D_8013C280[2][5] = 0x801F24FC;
    D_8013C280[2][6] = 0x801F2CCC;
    D_8013C280[2][7] = 0x801F3684;
    D_8013C280[2][8] = 0x801F476C;
    D_8013C280[2][9] = 0x801F4F2C;
    D_8013C280[2][10] = 0x801F62EC;
    D_8013C280[2][11] = 0x801F68E8;
    D_8013C280[2][12] = 0x801F7008;
    D_8013C280[2][13] = 0x801F7114;
    D_8013C280[2][14] = 0x801F7808;
    D_8013C280[2][15] = 0x801F7CA0;
    D_8013C280[2][16] = 0x801F8174;
}

void func_80132334(void) {
    switch (game_state.mode_stage) {
    case 0:
        func_80133030();
        return;
    case 1:
        func_80133540();
        return;
    case 2:
        func_80133AD0();
        return;
    case 3:
        func_80133F1C();
        return;
    case 4:
        func_80135DD0();
        return;
    case 5:
        func_80137340();
        return;
    case 6:
        func_801399B4();
        return;
    case 7:
        func_8013B760();
        return;
    default:
        func_80132400();
        return;
    }
}

void func_80132400(void) {
    game_mode_set(GAME_MODE_ETC_C2);
}

/* The scene dialogue entry: shows message text (kind 0 and 6: a title in brackets, 1 and 0xC: a spoken line
 * with its voice, 0xD and 0xF: the same after a wait of 0x3C frames). Old-style definition: kind, page, step
 * and color are read from their argument slots as u8/u16 at each use, and func_80132B04 calls it without a
 * prototype. T-20930 */
s32 func_80132420(text, kind, arg2, voices, page, step, color)
    u8 *text;
    u8 kind;
    s32 arg2;
    s32 ***voices;
    u16 page;
    u16 step;
    u8 color;
{
    /* FAKE: x is the second element of a six-halfword array and pad is never used; together they give the
     * original frame (0x260), the text buffer at sp+0x50 and x in memory at sp+0x256 (the original stores it
     * before set_kanji_string and reloads it after; a plain s16 is constant-folded). Real locals unknown. T-20930 */
    s16 x[6];
    u8 buf[0x204];
    u8 pad[8];

    if (kind != 0xFF) {
        strcpy(buf, text);
    }
    D_80122CF0 = 0;
    D_800CA2A8 = 0;
    switch (kind) {
    case 0x0:
        D_8013C360 = 0;
        k_speed_set(0x10);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "『 ", 0);
        talk_render(x[1] + 0xE, 0x48, 0, buf, 0, "", "", 0);
        D_80122D00 = 1;
        break;
    case 0x6:
        D_8013C360 = 0;
        k_speed_set(0x10);
        x[1] = -0x66;
        set_kanji_string(x[1], 0x48, 0, "（ ", 0);
        talk_render(x[1] + 0xE, 0x48, 0, buf, 0, "）", "", 0);
        D_80122D00 = 1;
        break;
    case 0x1:
        D_8013C360 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                cd_audio_request_set(voices[page][step][0], voices[page][step][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        set_kanji_string(-0x90, 0x48, color, "「 ", 0);
        talk_render(-0x82, 0x48, color, buf, 0, "", "", 0);
        D_80122CE4 = 1;
        break;
    case 0xC:
        D_8013C360 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                cd_audio_request_set(voices[page][step][0], voices[page][step][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        set_kanji_string(-0x90, 0x48, color, "「 ", 0);
        talk_render(-0x82, 0x48, color, buf, 0, "", "", 0);
        D_80122D18 = 1;
        break;
    case 0xD:
        D_8013C360 = 0;
        if (D_8013C364++ < 0x3C) {
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                cd_audio_request_set(voices[page][step][0], voices[page][step][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        talk_render(-0x82, D_800CA2C0, color, buf, 0, "", "", 0);
        D_80122D18 = 1;
        break;
    case 0xF:
        D_8013C360 = 0;
        if (D_8013C364++ < 0x3C) {
            return 0;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        if (voices[page][step][0] != 0) {
            if (D_800CA2AC++ == 0 && D_80122CE4 == 0) {
                cd_audio_request_set(voices[page][step][0], voices[page][step][1], game_state.current_girl);
                audio_command(0x300);
            }
            if (!(cd_audio_state() & 4) && D_80122CE4 == 0) {
                D_800CA2A8 = 1;
                return 0;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
        }
        talk_render(-0x82, D_800CA2C0, color, buf, 0, "", "", 0);
        D_80122CE4 = 1;
        break;
    case 0xFF:
        mode_step_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
}

/* Steps the scene dialogue: talk_tables/138 point at the page and step halfwords (D_8013C358, D_8013C35C),
 * D_800CA13C/140/144 hold the text, kind and voice tables of the scene (func_80132D98). Returns 1 when a
 * message has finished. T-20930 */
s32 func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    if (D_80122D00 != 0) {
        if (check_end_k() != 0) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                if ((u32)++game_state.mode_step_timer.w >= 0x3C) {
                    D_80122D00 = 0;
                    k_reset(1);
                    return 1;
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && game_state.pad_held == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122D18 = 0;
                return 1;
            }
        }
    } else if (D_80122CE4 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && game_state.pad_held == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122CE4 = 0;
                k_reset(1);
                return 1;
            }
        }
    } else {
        game_state.mode_step_timer.w = 0;
        func_80132420(((u8 ***)D_800CA13C)[*(s16 *)talk_tables][*(s16 *)D_800CA138], ((u8 **)D_800CA140)[*(s16 *)talk_tables][*(s16 *)D_800CA138], (u8)arg0, D_800CA144, *(s16 *)talk_tables, *(s16 *)D_800CA138, (u8)arg2);
        if (D_8013C364 == 0) {
            if ((u8)arg1 != 0 && D_800CA2A8 == 0) {
                if (((u8 **)D_800CA140)[*(s16 *)talk_tables][*(s16 *)D_800CA138] == 0xFF) {
                    *(s16 *)talk_tables += 1;
                    *(s16 *)D_800CA138 = 0;
                } else {
                    *(s16 *)D_800CA138 += 1;
                }
            }
        }
    }
    return 0;
}

void func_80132D98(void) {
    talk_tables = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C.unk_00;
    D_800CA140 = D_8013C34C.unk_04;
    D_800CA144 = D_8013C34C.unk_08;
    func_80132B04(0xFF, 1, 1);
}

void func_80132E10(void) {
    audio_bank_load_start(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    mode_step_next();
}

void func_80132E70(void) {
    if (audio_bank_load_poll() == 1) {
        mode_step_next();
    }
    if ((u32) game_state.mode_step_timer.w++ >= 0x400U) {
        cd_command_reset();
        audio_bank_state_reset();
        mode_step_set((game_state.mode_step - 1) & 0xFF);
    }
}

void func_80132EF0(void) {
    audio_command(0x200);
    mode_step_next();
}

/* Port of main check_k_scroll (src/main/scene_library.c), with ENDING's own limits. The unsigned
 * compare keeps the operand order of the bne (as in check_k_scroll). */
/* FAKE: sp40 keeps the unused result of the second get_k_work call; without it the frame is 8
 * bytes smaller and both spill homes move. Real source unknown. T-18300 */
void func_80132F18(void) {
    s32 sp3C;
    TextEntry *sp50;
    s32 sp40;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    TextEntry *temp_v0_2;
    TextEntry *temp_v0_3;

    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp50 = (TextEntry *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp3C = temp_a0;
    sp40 = get_k_work(temp_a0);
    if (sp50->y >= 0x68) {
        var_a0 = sp3C;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (TextEntry *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->y;
                    if ((u32) temp_v1 == 0x48U) {
                        temp_v0_2->y = (s16) (temp_v1 - 0x10);
                        temp_v0_2->active = 0;
                    } else {
                        temp_v0_2->y = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            D_800CA2BC = 0;
            sp50->y = 0x58;
            sp50->active = 1;
            var_a0 = sp3C;
        }
    }
    temp_v0_3 = (TextEntry *) get_k_work(var_a0);
    if (temp_v0_3->y >= 0x68) {
        D_800CA2BC = 1;
        temp_v0_3->active = 0;
    }
}
