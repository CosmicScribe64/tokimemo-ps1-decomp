#define MAIN_API_OVERRIDE_get_now_k /* matched with s32() (main_api.h: u8(void)) */
#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"
#include "psx_types.h"

s32 get_now_k();
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAC0);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAC4);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAC8);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CACC);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAD0);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAD4);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAD8);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CADC);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAE0);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAE4);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAE8);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAEC);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAF0);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAF4);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAF8);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CAFC);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB00);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB04);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB08);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB0C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB10);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB14);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB18);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB1C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB20);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB24);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB28);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB2C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB30);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB34);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB38);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB3C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB40);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB44);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB48);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB4C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB50);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB54);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB58);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB5C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB60);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB64);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB68);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB6C);
INCLUDE_RODATA("asm/ovl/ENDING/data/ENDING/801396E0.data", D_8013CB70);
/* Scene table, indexed by game_state.step in func_801399B4. Defined here as the original
 * did: as1 treats every function named in this file's data as an address-taken label, so the
 * jump-table dispatch of func_8013A2E8 gets an edge back to that function's entry, and the loop
 * changes as1's block weights and candidate budget there (T-25150, T-25127). */
EdFnTbl14 D_8013CB74 = { {
    func_80139D54, func_80139D24, func_8007C784, func_8013A2E8, func_80139DA8,
    func_8007C784, func_80139E08, func_8013A004, func_80139EC4, func_8013B5E4,
    func_8013B510, func_80139F3C, func_80139A58, game_reset_to_title,
} };

void func_801396E0(void) {
    D_8013CAC0[0][0] = 0x80180308;
    D_8013CAC0[0][1] = 0x80180728;
    D_8013CAC0[0][2] = 0x80180BCC;
    D_8013CAC0[0][3] = 0x80180ED4;
    D_8013CAC0[0][4] = 0x801812E4;
    D_8013CAC0[0][5] = 0x80181648;
    D_8013CAC0[0][6] = 0x80181A84;
    D_8013CAC0[0][7] = 0x80181ED0;
    D_8013CAC0[0][8] = 0x80182258;
    D_8013CAC0[0][9] = 0x80182684;
    D_8013CAC0[0][10] = 0x80182AB4;
    D_8013CAC0[0][11] = 0x80182C74;
    D_8013CAC0[0][12] = 0x801835E4;
    D_8013CAC0[0][13] = 0x80183CC4;
    D_8013CAC0[0][14] = 0x80184008;
    D_8013CAC0[1][0] = 0x80180314;
    D_8013CAC0[1][1] = 0x80180734;
    D_8013CAC0[1][2] = 0x80180BD8;
    D_8013CAC0[1][3] = 0x80180EE0;
    D_8013CAC0[1][4] = 0x801812F0;
    D_8013CAC0[1][5] = 0x80181654;
    D_8013CAC0[1][6] = 0x80181A90;
    D_8013CAC0[1][7] = 0x80181EDC;
    D_8013CAC0[1][8] = 0x80182264;
    D_8013CAC0[1][9] = 0x80182690;
    D_8013CAC0[1][10] = 0x80182AC0;
    D_8013CAC0[1][11] = 0x80182C80;
    D_8013CAC0[1][12] = 0x8018360C;
    D_8013CAC0[1][13] = 0x80183CD4;
    D_8013CAC0[1][14] = 0x80184014;
    D_8013CAC0[2][0] = 0x801803DC;
    D_8013CAC0[2][1] = 0x801807FC;
    D_8013CAC0[2][2] = 0x80180CA0;
    D_8013CAC0[2][3] = 0x80180FA8;
    D_8013CAC0[2][4] = 0x801813B8;
    D_8013CAC0[2][5] = 0x8018171C;
    D_8013CAC0[2][6] = 0x80181B58;
    D_8013CAC0[2][7] = 0x80181FA4;
    D_8013CAC0[2][8] = 0x8018232C;
    D_8013CAC0[2][9] = 0x80182758;
    D_8013CAC0[2][10] = 0x80182B88;
    D_8013CAC0[2][11] = 0x80182D48;
    D_8013CAC0[2][12] = 0x801839E4;
    D_8013CAC0[2][13] = 0x80183DB8;
    D_8013CAC0[2][14] = 0x801840DC;
}

void func_801399B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EdFnTbl14 tbl;

    tbl = D_8013CB74;
    idx = game_state.step;
    tbl.f[idx](0x80);
    if (D_80122CD4 != 0) {
        func_80139AA0(0x404040);
    }
}

void func_80139A58(void) {
    if (game_state.current_girl == GIRL_YOSHIO) {
        main_stage_set(7);
        main_step_set(0xF);
        return;
    }
    main_stage_set(8);
}

void func_80139AA0(s32 arg0) {
    func_80139B48(-0x64, -0x50, 0xC8, 0x10, arg0, 0, 1);
    func_80139B48(-0x64, 0x40, 0xC8, 0x10, arg0, 0, 1);
    dtd_on(0);
    func_80139B48(-0x64, -0x40, 0xC8, 0x80, arg0, 0xB, 1);
    dtd_on(0xB);
}

/* FAKE: the explicit "& 0xFF" masks and one source line per colour trio, as in the matched main sibling
 * _sprite_set_box_shade_tarao (T-17001; one colour instead of two): the masks leave the shifts in ugen
 * temporaries (t6, t7) and as1 schedules the stores by source line. The real source is unknown. T-20930 */
void func_80139B48(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    GameState *gs;
    POLY_G4 *p;

    p = (POLY_G4 *)GetWorkBase(0x24, draw_buffer);
    func_8009F054(p);
    SetSemiTrans(p, arg6);
    gs = &game_state;
    p->r0 = (arg4 >> 16) & 0xFF; p->g0 = (arg4 >> 8) & 0xFF; p->b0 = arg4 & 0xFF;
    p->r1 = (arg4 >> 16) & 0xFF; p->g1 = (arg4 >> 8) & 0xFF; p->b1 = arg4 & 0xFF;
    p->r2 = (arg4 >> 16) & 0xFF; p->g2 = (arg4 >> 8) & 0xFF; p->b2 = arg4 & 0xFF;
    p->r3 = (arg4 >> 16) & 0xFF; p->g3 = (arg4 >> 8) & 0xFF; p->b3 = arg4 & 0xFF;
    p->x0 = gs->screen_center_x[draw_buffer] + arg0;
    p->y0 = gs->screen_center_y[draw_buffer] + arg1;
    p->x1 = gs->screen_center_x[draw_buffer] + arg0 + arg2;
    p->y1 = gs->screen_center_y[draw_buffer] + arg1;
    p->x2 = gs->screen_center_x[draw_buffer] + arg0;
    p->y2 = gs->screen_center_y[draw_buffer] + arg1 + arg3;
    p->x3 = gs->screen_center_x[draw_buffer] + arg0 + arg2;
    p->y3 = gs->screen_center_y[draw_buffer] + arg1 + arg3;
    AddPrim(D_800E8CA0 + (draw_buffer << 10) + arg5 * 4, (s32)p);
}

void func_80139D24(void) {
    cd_read_sectors(9, 0x80180000, 0xAE18);
    main_step_next();
}

void func_80139D54(void) {
    actor_clear_range(0);
    cursor_show(0);
    sprite_tables_clear();
    text_system_reset();
    func_8004E500(3);
    D_80122CE4 = 0;
    D_80122CD4 = 0;
    main_step_next();
}

void func_80139DA8(void) {
    if (game_state.current_girl != GIRL_YOSHIO) {
        cd_read_sectors_nowait(9, 0x80162000, 0x497C);
    } else {
        cd_read_sectors_nowait(0xA, 0x80162000, 0x4985);
    }
    main_step_next();
}

s32 func_80139E08(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        printf("DEC_BS header Error \n");
        printf("%x %x\n", ((u32 *)D_80125C04)[0], ((u32 *)D_80125C04)[1]);
        dec_bg_lba_cache[2] = 0;
        dec_bg_lba_cache[3] = 0;
        main_step_next();
        game_state.step -= 3;
        return 0;
    }
    func_800536AC(0x140, 0x100, 0x140, 0xF0);
    main_step_next();
}

void func_80139EC4(void) {
    s16 i;

    D_80121874 += 4;
    if (D_80121874 >= 0x80) {
        D_80121874 = 0x80;
        main_step_next();
    }
    for (i = 4; i < 0xA; i++) {
        /* FAKE: the comma expression preserves the brightness register.
         * Use the existing unprototyped callee view. T-9530. */
        func_80084C98(i, (0, D_80121874));
    }
}

void func_80139F3C(void) {
    s16 i;
    s32 shade;
    /* FAKE: keep the existing scalar brightness view for a1 promotion;
     * SpriteQuad field/pointer views use v0 and do not match. T-10010. */
    if (D_80121874 == 128) {
        snd_command(0xD1);
        D_80122CD4 = 0;
    }
    D_80121874 -= 4;
    if (D_80121874 < 5U) {
        main_step_next();
        D_80121874 = 0;
    }
    for (i = 4; i < 10; i++) {
        /* FAKE: comma expression retains the brightness register, as in
         * the adjacent fade-in routine. T-10010. */
        func_80084C98(i, (0, D_80121874));
    }
    shade = (((u32)(D_80121874 << 6)) >> 7) & 0xFF;
    /* FAKE: the empty test preserves the final shift register. T-10010. */
    if (1) {
    }
    func_80139AA0((shade << 16) | (shade << 8) | shade);
}

void func_8013A004(void) {
    sprite_quads[4].flags = 0x02000000;
    sprite_quads[4].w = 0xC0;
    sprite_quads[4].h = 0x80;
    sprite_quads[4].unk_0E = 0;
    sprite_quads[4].unk_0F = 0x38;
    sprite_quads[4].unk_10 = 0;
    sprite_quads[4].unk_0C = 0x15;
    sprite_quads[4].unk_12 = 0x1F1;
    sprite_quads[4].tint_r = 0;
    sprite_quads[4].tint_g = 0;
    sprite_quads[4].tint_b = 0;
    sprite_quads[4].anchor_x = 0;
    sprite_quads[4].anchor_y = 0;
    sprite_quads[4].unk_1C = 0x1000;
    sprite_quads[4].unk_1E = 0x1000;
    sprite_quads[4].unk_20 = 0;
    sprite_quads[4].x = -0xA0;
    sprite_quads[4].y = -0x40;
    game_state.quad_layer[4] = 0xD;
    sprite_quads[5] = sprite_quads[4];
    sprite_quads[5].w = 0x80;
    sprite_quads[5].unk_0C = 0x18;
    sprite_quads[5].x = 0x20;
    game_state.quad_layer[5] = 0xD;
    sprite_quads[6] = sprite_quads[4];
    sprite_quads[6].unk_0F = 0;
    sprite_quads[6].h = 0x38;
    sprite_quads[6].y = -0x78;
    game_state.quad_layer[6] = 1;
    sprite_quads[7] = sprite_quads[5];
    sprite_quads[7].unk_0F = 0;
    sprite_quads[7].h = 0x38;
    sprite_quads[7].y = -0x78;
    game_state.quad_layer[7] = 1;
    sprite_quads[8] = sprite_quads[4];
    sprite_quads[8].y = 0x40;
    sprite_quads[8].unk_0F = 0xB8;
    sprite_quads[8].h = 0x38;
    game_state.quad_layer[8] = 1;
    sprite_quads[9] = sprite_quads[5];
    sprite_quads[9].y = 0x40;
    sprite_quads[9].unk_0F = 0xB8;
    sprite_quads[9].h = 0x38;
    game_state.quad_layer[9] = 1;
    snd_command(0x201);
    main_step_next();
}

/* The first flag test fills its load-delay slot with the first string's `lui a1` only because
 * D_8013CB74 (top of the file) names this function: the jump-table dispatch then has an edge back
 * to the entry, so the flag block lies in a loop region, which as1 searches with its larger
 * candidate budget (T-25127; T-23002 and T-25056 traced the 11-block cutoff of the outer region). */
void func_8013A2E8(void) {
    u32 top;
    s16 i;
    s16 idx;
    u8 sel;
    s32 pad; /* FAKE (T-23002): unused word, the frame has 16 bytes above msg and the declared locals fill 13 */
    s32 msg;
    u8 bestGirl;
    s16 best;
    u8 kind;
    u8 name[27] = "M残念ながら 駄目だったが、";

    best = 0;
    bestGirl = 0xFF;
    for (i = 0; i < 7; i++) {
        /* FAKE (T-23002): the seven ParamValue records at +0x100 are separate members; indexed until their aggregate is known */
        if (best < (&game_state.param_liberal_arts)[i].value) {
            best = (&game_state.param_liberal_arts)[i].value;
            bestGirl = i;
        }
    }
    func_801396E0();
    dialogue_work = dialogue_render(-0x48, 0x48, 0, (*D_8013CAF0)[0], 0, "", "", 0);
    func_8013B640();
    if (game_state.player_flags_hi.b[1] & 1) {
        strcpy(D_800CA280, "一流大学");
    } else if (game_state.player_flags_hi.b[1] & 2) {
        strcpy(D_800CA280, "二流大学");
    } else if (game_state.player_flags_hi.b[1] & 4) {
        strcpy(D_800CA280, "三流大学");
    } else if (game_state.player_flags_hi.b[1] & 8) {
        strcpy(D_800CA280, "一流企業");
    } else if (game_state.player_flags_hi.b[1] & 0x10) {
        strcpy(D_800CA280, "二流企業");
    } else if (game_state.player_flags_hi.b[1] & 0x20) {
        strcpy(D_800CA280, "三流企業");
    } else if (((ClubFlagBits *)&game_state.clubs[0])->unk_25 >= 3) {
        strcpy(D_800CA280, "小説家は");
    } else if (((ClubFlagBits *)&game_state.clubs[1])->unk_25 >= 3) {
        strcpy(D_800CA280, "俳優は");
    } else if (((ClubFlagBits *)&game_state.clubs[2])->unk_25 >= 3) {
        strcpy(D_800CA280, "製薬会社は");
    } else if (((ClubFlagBits *)&game_state.clubs[3])->unk_25 >= 3) {
        strcpy(D_800CA280, "有名ゲーム会社は ");
    } else if (((ClubFlagBits *)&game_state.clubs[4])->unk_25 >= 3) {
        strcpy(D_800CA280, "画家は");
    } else if (((ClubFlagBits *)&game_state.clubs[5])->unk_25 >= 3) {
        strcpy(D_800CA280, "音楽家は");
    } else if (((ClubFlagBits *)&game_state.clubs[6])->unk_25 >= 3) {
        strcpy(D_800CA280, "プロ野球選手は");
    } else if (((ClubFlagBits *)&game_state.clubs[7])->unk_25 >= 3) {
        strcpy(D_800CA280, "Ｊリーガーは");
    } else if (((ClubFlagBits *)&game_state.clubs[8])->unk_25 >= 3) {
        strcpy(D_800CA280, "プロテニスプレイヤーは ");
    } else if (((ClubFlagBits *)&game_state.clubs[9])->unk_25 >= 3) {
        strcpy(D_800CA280, "水泳の実業団は");
    } else {
        strcpy(D_800CA280, "プロバスケプレイヤーは ");
    }
    top = 0;
    idx = 0;
    for (i = 0; i < 8; i++) {
        if (i == 7) {
            if (top < (game_state.progress_state[0x15 + i] >> 1)) {
                top = (game_state.progress_state[0x15 + i] >> 1) & 0xFF;
                idx = i;
            }
        } else if (i == 6) {
            if (top < (game_state.progress_state[0x15 + i] * 3 >> 2)) {
                top = (game_state.progress_state[0x15 + i] >> 1) & 0xFF;
                idx = i;
            }
        } else {
            if (top < game_state.progress_state[0x15 + i]) {
                top = game_state.progress_state[0x15 + i] & 0xFF;
                idx = i;
            }
        }
    }
    msg = (*D_8013CAF0)[idx + 1];
    D_800CA2C0 += 0x10;
    dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    msg = (*D_8013CAF0)[9];
    D_800CA2C0 += 0x10;
    dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    idx = 0xFF;
    for (i = 0; i < 11; i++) {
        if (((ClubFlagBits *)&game_state.clubs[i])->unk_25 >= 3 && (game_state.player_flags_hi.b[1] & 0x40) && !(game_state.player_flags_hi.b[1] & 0x80)) {
            msg = (*D_8013CAF0)[10 + i];
            kind = i;
            idx = i;
            break;
        }
    }
    D_800CA2C0 += 0x10;
    if (idx == 0xFF) {
        if (game_state.player_flags_hi.b[1] & 0x80) {
            if (game_state.player_flags_hi.b[1] & 0x40) {
                dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, (s32)name, 0, "", "", 0);
                func_8013B640();
            } else {
                msg = (*D_8013CAF0)[27];
                dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
                func_8013B640();
            }
            if (best < 0x78) {
                if ((game_state.player_flags_hi.b[1] & 1) || (game_state.player_flags_hi.b[1] & 2) || (game_state.player_flags_hi.b[1] & 4)) {
                    kind = 0xC;
                    msg = (*D_8013CAF0)[32];
                } else {
                    kind = 0xB;
                    msg = (*D_8013CAF0)[33];
                }
            } else if (bestGirl == 0 || bestGirl == 1 || bestGirl == 2 || bestGirl == 3) {
                kind = 0xD;
                msg = (*D_8013CAF0)[28];
            } else if (bestGirl == 4) {
                kind = 0xE;
                msg = (*D_8013CAF0)[29];
            } else if (bestGirl == 5) {
                kind = 0xF;
                msg = (*D_8013CAF0)[30];
            } else {
                kind = 0x10;
                msg = (*D_8013CAF0)[31];
            }
        } else if (game_state.player_flags_hi.b[1] & 1) {
            kind = 0x14;
            msg = (*D_8013CAF0)[21];
        } else if (game_state.player_flags_hi.b[1] & 2) {
            kind = 0x15;
            msg = (*D_8013CAF0)[22];
        } else if (game_state.player_flags_hi.b[1] & 4) {
            kind = 0x16;
            msg = (*D_8013CAF0)[23];
        } else if (game_state.player_flags_hi.b[1] & 8) {
            kind = 0x11;
            msg = (*D_8013CAF0)[24];
        } else if (game_state.player_flags_hi.b[1] & 0x10) {
            kind = 0x12;
            msg = (*D_8013CAF0)[25];
        } else {
            kind = 0x13;
            msg = (*D_8013CAF0)[26];
        }
    }
    dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    sel = game_state.current_girl;
    switch (sel) {
    case 0:
        if (kind == 0x14) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][1];
        break;
    case 1:
        if (kind == 0x14) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][6];
        break;
    case 2:
        if (kind == 0x11) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x12 || kind == 0x13 || kind == 0xE || kind == 0xF || kind == 0x10) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][2];
        break;
    case 3:
        if (kind == 4) {
            idx = 0;
        } else if (kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][4];
        break;
    case 4:
        if (kind == 0xD) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][10];
        break;
    case 5:
        if (kind == 0x15) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][7];
        break;
    case 6:
        if (kind == 9) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][8];
        break;
    case 7:
        if (kind == 0xF) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16 || kind == 0xD || kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][5];
        break;
    case 8:
        if (kind == 0xB) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x14 || kind == 0x15 || kind == 0x16 || kind == 0xD) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 0xF;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][0];
        break;
    case 9:
        if (kind == 0x12) {
            idx = 0;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0x11 || kind == 0x13 || kind == 0xE || kind == 0xF || kind == 0x10) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 1;
        }
        dialogue_text_table = D_8013CAC0[0][9];
        break;
    case 10:
        if (kind == 0x14 || kind == 0x15 || kind == 0x16) {
            idx = 0;
        } else if (kind == 0x11 || kind == 0x12 || kind == 0x13) {
            idx = 1;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xF) {
            idx = 3;
        } else if (kind == 0xC) {
            idx = 4;
        } else if (kind == 0xB) {
            idx = 5;
        } else {
            idx = 7;
        }
        dialogue_text_table = D_8013CAC0[0][13];
        break;
    case 11:
        if (kind == 0 || kind == 1 || kind == 2 || kind == 3 || kind == 6 || kind == 7 || kind == 8 || kind == 9) {
            idx = 1;
        } else if (kind == 4 || kind == 5 || kind == 0xA) {
            idx = 2;
        } else if (kind == 0xC) {
            idx = 3;
        } else if (kind == 0xB) {
            idx = 4;
        } else {
            idx = 0;
        }
        dialogue_text_table = D_8013CAC0[0][14];
        break;
    case 12:
        idx = 0;
        dialogue_text_table = D_8013CAC0[0][3];
        break;
    default:
        dialogue_text_table = D_8013CAC0[0][11];
        idx = 0;
        break;
    }
    msg = (*(s32 **)dialogue_text_table)[idx];
    D_800CA2C0 += 0x10;
    dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
    func_8013B640();
    sel = game_state.current_girl;
    if (sel != 0xC && sel != 0xD) {
        msg = (*(s32 **)dialogue_text_table)[6];
        D_800CA2C0 += 0x10;
        dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
        func_8013B640();
        msg = (*D_8013CAF0)[34];
        D_800CA2C0 += 0x10;
        dialogue_work = dialogue_render(-0x48, D_800CA2C0, 0, msg, 0, "", "", 0);
        func_8013B640();
    }
    D_800CA2C0 += 0x20;
    main_step_next();
}

/* FAKE (T-9780): the s32 fall-through preserves the original timer result register. */
s32 func_8013B510(void) {
    s16 timer;
    s16 y;
    u32 max;
    u32 count;
    TextEntry *entry;

    if (game_state.frame_count & 3) {
        return 0;
    }
    max = get_now_k() & 0xFF;
    count = 0;
    do {
        entry = (TextEntry *)get_k_work(count);
        entry->y--;
        y = entry->y;
        count++;
        if (y >= 81 || y < -80) {
            entry->active = 0;
        } else {
            entry->active = 1;
        }
        /* FAKE (T-9780): normalize in the condition to retain the delay-slot copy. */
    } while (max >= (count = count & 0xFF));
    timer = --D_800CA2C0;
    if (timer < -64) {
        main_step_next();
    }
}

s32 func_8013B5E4(void) {
    if ((u32)game_state.step_timer.w++ < 0x80U) {
        return 0;
    }
    k_disp_start(dialogue_work);
    D_80122CD4 = 1;
    main_step_next();
}

/* FAKE (T-23002): s32 with no return, so the callers (func_8013A2E8) see a live $v0 after the call; the real return type is unknown. */
s32 func_8013B640(void) {
    s32 temp_v0;
    s32 var_s0;
    u32 temp_s1;
    u8 *temp_v0_2;

    temp_v0 = get_now_k();
    temp_s1 = temp_v0 & 0xFF;
    get_k_work(temp_v0);
    var_s0 = 0;
    if (temp_s1 != 0) {
        do {
            temp_v0_2 = get_k_work(var_s0);
            var_s0 = (s32) (s16) (var_s0 + 1);
            *(s8 *)((unsigned char *)temp_v0_2 + (7)) = 0;
        } while ((u32) var_s0 < temp_s1);
    }
}
