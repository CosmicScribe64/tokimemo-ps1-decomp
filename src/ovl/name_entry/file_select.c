/* file_select.c: src/ovl/NAME_ENT/80139740.c:85,1045,1753,3195; mode0x13 album/system-file selection, read/write/copy/new/clear flow, retained profiles and summary presentation. Original object 0x80139740-0x80142A80 (config/objects/NAME_ENT.txt). */
#define MAIN_API_OVERRIDE_prim_gradient_rect /* Original calls pass promoted integer coordinates. T-10880. */
#define MAIN_API_OVERRIDE_memory_card_start /* The original reads $v0 after the call in file_select_album_only_read. T-22023. */
#include "common.h"
#include "ovl/name_entry.h"

void prim_gradient_rect();
s32 memory_card_start();

s32 file_select_number_draw(u32 number, s32 screen_x, s32 screen_y) {
    u8 *p;
    s32 n;
    u32 remaining_number;
    u8 *e;

    remaining_number = number;
    for (n = 0;; n += 8) {
        p = (u8 *)GetWorkBase(0x10, draw_buffer);
        func_8009F090(p);
        SetSemiTrans(p, 0);
        p[4] = 0x80;
        p[5] = 0x80;
        p[6] = 0x80;
        *(s16 *)(p + 8) = game_state.draw_origin_x[draw_buffer] + screen_x - n;
        *(s16 *)(p + 0xA) = game_state.draw_origin_y[draw_buffer] + screen_y;
        e = file_select_digit_uv[remaining_number % 10];
        p[0xC] = e[0];
        p[0xD] = e[1];
        *(s16 *)(p + 0xE) = func_8009ED74(0, 0x1F0);
        AddPrim(draw_ot_tags + (draw_buffer << 10) + 0xC, p);
        if (remaining_number < 10) {
            dtd_on_tpage(0x200, 0, 3, 0, 1);
            return 1;
        }
        remaining_number = remaining_number / 10;
    }
}

/* T-25057: record display flags and animated tile coordinates. */
void file_select_girl_icons_visibility_update(void) {
    s32 information_page;
    s32 i;
    u32 tile[1]; /* FAKE: T-25057, memory local orders remainder before frame allocation. */

    i = 0;
    information_page = file_select_information_page % 3;
    for (; i < 13; i++) {
        if ((u32)sprite_quads[i].flags < 0) {} /* FAKE: T-25057, erased test orders record induction. */
        tile[0] = game_state.frame_count; /* FAKE: T-25057, dead initialization orders frame before index; removed by uopt. */
        if (information_page == 0) {
            if (((AlbumSlotRecord *)memory_card_block)[D_800E8BEF].flags[i] == 0) {
                sprite_quads[i].flags = 0;
                tile[0] = (game_state.frame_count >> 2) % 3;
                sprite_quads[i].unk_0E = (i / 8) * 0x30 - (0U - tile[0] * 0x10) + 0x80; /* FAKE: T-25057, unsigned identity preserves row-before-tile evaluation. */
            } else {
                sprite_quads[i].flags = 0x80000000;
            }
        } else {
            sprite_quads[i].flags = 0x80000000;
        }
    }
}

void file_select_girl_icon_set(s32 girl_index, s32 screen_x, s32 screen_y) {
    u8 *p;

    p = (u8 *)sprite_quads + girl_index * 36;
    p[0x14] = 0x80;
    p[0x15] = 0x80;
    p[0x16] = 0x80;
    *(s16 *)(p + 4) = screen_x;
    *(s16 *)(p + 6) = screen_y;
    *(s32 *)p = 0;
    *(s16 *)(p + 8) = 0x10;
    *(s16 *)(p + 0xA) = 0x10;
    p[0xE] = girl_index / 8 * 0x30 + game_state.frame_count / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (girl_index & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = girl_index * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    game_state.quad_layer[girl_index] = 2;
}

void file_select_information_page_input(void) {
    if (game_state.pad_pressed & PAD_TRIANGLE) {
        file_select_information_page += 1;
        return;
    }
    if ((game_state.pad_pressed & PAD_CIRCLE) && (cursor_x < 0x20) && (cursor_x >= -0x3F) && (cursor_y < 0x1C) && (cursor_y >= 0xD)) {
        file_select_information_page += 1;
    }
}

void file_select_information_page_draw(void) {
    if (file_select_information_page < 0) {
        file_select_information_page += 3;
    }
    switch (file_select_information_page % 3) {
    case 0:
        file_select_page_clear_counts_draw();
        break;
    case 1:
        file_select_page_parameters_draw();
        break;
    case 2:
        file_select_page_girl_stars_draw();
        break;
    }
    file_select_information_page_input();
}

void file_select_slot_header_draw(void) {
    u8 message_text[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    if (slot_info_from_system_data != 0) {
        strncpy(message_text, D_800B3288[D_800E7D36], 5);
        strcat(message_text, "番");
        set_kanji_string(-0x20, -0x5C, 0xF, (s32) message_text, 0);
        set_kanji_string(0x20, -0x4B, 0xF, (s32) system_data.family_name[D_800E7D36], 0);
        set_kanji_string(0x4E, -0x4B, 0xF, (s32) system_data.given_name[D_800E7D36], 0);
        set_kanji_string(0x20, -0x3B, 0xF, (s32) system_data.nickname[D_800E7D36], 0);
        set_kanji_string(0x20, -0x2B, 0xF, (s32) club_name_table[system_data.club[D_800E7D36]], 0);
        return;
    }
    strncpy(message_text, D_800B3288[game_state.save_slot], 5);
    strcat(message_text, "番");
    set_kanji_string(-0x20, -0x5C, 0xF, (s32) message_text, 0);
    set_kanji_string(0x20, -0x4B, 0xF, (s32) ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].family_name, 0);
    set_kanji_string(0x4E, -0x4B, 0xF, (s32) ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].given_name, 0);
    set_kanji_string(0x20, -0x3B, 0xF, (s32) ((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].nickname, 0);
    set_kanji_string(0x20, -0x2B, 0xF, (s32) club_name_table[((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].club >> 0xC], 0);
}

void file_select_page_clear_counts_draw(void) {
    s32 i;
    SystemData *system_record = &system_data;
    GameState *file_state = &game_state;

    if (slot_info_from_system_data != 0) {
        if (system_record->flags[0] != 0) {
            file_select_number_draw(system_record->flags[0], -4, 0x1E);
        }
        if (system_record->flags[1] != 0) {
            file_select_number_draw(system_record->flags[1], -4, 0x2E);
        }
        if (system_record->flags[2] != 0) {
            file_select_number_draw(system_record->flags[2], -4, 0x3E);
        }
        if (system_record->flags[3] != 0) {
            file_select_number_draw(system_record->flags[3], -4, 0x4E);
        }
        if (system_record->flags[4] != 0) {
            file_select_number_draw(system_record->flags[4], 0x3C, 0x1E);
        }
        if (system_record->flags[5] != 0) {
            file_select_number_draw(system_record->flags[5], 0x3C, 0x2E);
        }
        if (system_record->flags[6] != 0) {
            file_select_number_draw(system_record->flags[6], 0x3C, 0x3E);
        }
        if (system_record->flags[7] != 0) {
            file_select_number_draw(system_record->flags[7], 0x3C, 0x4E);
        }
        if (system_record->flags[12] != 0) {
            file_select_number_draw(system_record->flags[12], 0x88, 0xE);
        }
        if (system_record->flags[8] != 0) {
            file_select_number_draw(system_record->flags[8], 0x88, 0x1E);
        }
        if (system_record->flags[9] != 0) {
            file_select_number_draw(system_record->flags[9], 0x88, 0x2E);
        }
        if (system_record->flags[10] != 0) {
            file_select_number_draw(system_record->flags[10], 0x88, 0x3E);
        }
        if (system_record->flags[11] != 0) {
            file_select_number_draw(system_record->flags[11], 0x88, 0x4E);
        }
        for (i = 0; i < 4; i++) {
            if (system_record->flags[i] != 0) {
                file_select_girl_icon_set(i, -0x23, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 4; i < 8; i++) {
            if (system_record->flags[i] != 0) {
                file_select_girl_icon_set(i, 0x1E, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 8; i < 12; i++) {
            if (system_record->flags[i] != 0) {
                file_select_girl_icon_set(i, 0x69, (i % 4) * 0x10 + 0x1D);
            }
        }
        if (system_record->flags[12] != 0) {
            file_select_girl_icon_set(12, 0x69, 0xD);
        }
        for (i = 0; i < 8; i++) {
            if (system_record->flags[i] == 0) {
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
            }
        }
        for (i = 8; i < 12; i++) {
            if (system_record->flags[i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
            }
        }
        if (system_record->flags[12] == 0) {
            ((Actor *)actor_pool)[136].draw_flags |= 0x80;
            ((Actor *)actor_pool)[136].unk_01 = 4;
        } else {
            ((Actor *)actor_pool)[136].draw_flags &= 0x7F;
        }
    } else {
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[0] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[0], -4, 0x1E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[1] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[1], -4, 0x2E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[2] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[2], -4, 0x3E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[3] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[3], -4, 0x4E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[4] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[4], 0x3C, 0x1E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[5] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[5], 0x3C, 0x2E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[6] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[6], 0x3C, 0x3E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[7] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[7], 0x3C, 0x4E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[12] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[12], 0x88, 0xE);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[8] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[8], 0x88, 0x1E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[9] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[9], 0x88, 0x2E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[10] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[10], 0x88, 0x3E);
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[11] != 0) {
            file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[11], 0x88, 0x4E);
        }
        for (i = 0; i < 4; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] != 0) {
                file_select_girl_icon_set(i, -0x23, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 4; i < 8; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] != 0) {
                file_select_girl_icon_set(i, 0x1E, (i % 4) * 0x10 + 0x1D);
            }
        }
        for (i = 8; i < 12; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] != 0) {
                file_select_girl_icon_set(i, 0x69, (i % 4) * 0x10 + 0x1D);
            }
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[12] != 0) {
            file_select_girl_icon_set(12, 0x69, 0xD);
        }
        for (i = 0; i < 8; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] == 0) {
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
            }
        }
        for (i = 8; i < 12; i++) {
            if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
            }
        }
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[12] == 0) {
            ((Actor *)actor_pool)[136].draw_flags |= 0x80;
            ((Actor *)actor_pool)[136].unk_01 = 4;
        } else {
            ((Actor *)actor_pool)[136].draw_flags &= 0x7F;
        }
    }
}

void file_select_page_girl_stars_draw(void) {
    s32 i;
    SystemData *system_record = &system_data;
    GameState *file_state = &game_state;

    k_sub_reset_point_set();
    k_sub_disp_start(0);
    for (i = 0; i < 13; i++) {
        ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
    }
    if (slot_info_from_system_data != 0) {
        for (i = 0; i < 8; i++) {
            if (system_record->slot_girl_marks[system_record->selected_slot][i] == 0) {
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 141].unk_18 = (system_record->slot_girl_marks[system_record->selected_slot][i] & 7) + 0x58;
            }
        }
        for (i = 8; i < 11; i++) {
            if (system_record->slot_girl_marks[system_record->selected_slot][i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 141].unk_18 = (system_record->slot_girl_marks[system_record->selected_slot][i] & 7) + 0x58;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (((GirlFlagBits *)&((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x2C + i * 0x38])->met == 0 ||
                ((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] == 0) {
                ((Actor *)actor_pool)[i + 128].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 128].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] != 0) {
                    ((Actor *)actor_pool)[i + 141].unk_18 = (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] & 7) + 0x58;
                }
            }
        }
        for (i = 8; i < 11; i++) {
            if (((GirlFlagBits *)&((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x2C + i * 0x38])->met == 0 ||
                ((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] == 0) {
                ((Actor *)actor_pool)[i + 129].draw_flags |= 0x80;
                ((Actor *)actor_pool)[i + 129].unk_01 = 4;
                ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
            } else {
                ((Actor *)actor_pool)[i + 129].draw_flags &= 0x7F;
                ((Actor *)actor_pool)[i + 141].draw_flags |= 0x80;
                if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] != 0) {
                    ((Actor *)actor_pool)[i + 141].unk_18 = (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].unk_2E8[0x37C + i] & 7) + 0x58;
                }
            }
        }
    }
    ((Actor *)actor_pool)[140].draw_flags |= 0x80;
    ((Actor *)actor_pool)[140].unk_01 = 4;
    ((Actor *)actor_pool)[136].draw_flags |= 0x80;
    ((Actor *)actor_pool)[136].unk_01 = 4;
}

void file_select_page_parameters_draw(void) {
    if (slot_info_from_system_data != 0) {
        file_select_number_draw(system_data.metrics[0][D_800E7D36].value, 16, 30);
        file_select_number_draw(system_data.metrics[1][D_800E7D36].value, 16, 46);
        file_select_number_draw(system_data.metrics[2][D_800E7D36].value, 16, 62);
        file_select_number_draw(system_data.metrics[3][D_800E7D36].value, 16, 78);
        file_select_number_draw(system_data.metrics[4][D_800E7D36].value, 128, 14);
        file_select_number_draw(system_data.metrics[5][D_800E7D36].value, 128, 30);
        file_select_number_draw(system_data.metrics[6][D_800E7D36].value, 128, 46);
        file_select_number_draw(system_data.metrics[7][D_800E7D36].value, 128, 62);
        file_select_number_draw(system_data.metrics[8][D_800E7D36].value, 128, 78);
        return;
    }
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[0].value, 16, 30);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[1].value, 16, 46);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[2].value, 16, 62);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[3].value, 16, 78);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[4].value, 128, 14);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[5].value, 128, 30);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[6].value, 128, 46);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[7].value, 128, 62);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[game_state.album_slot].metrics[8].value, 128, 78);
}

s32 file_select_page_indicator_update(s32 show_indicators) {
    s32 a;
    s32 b;
    s32 sel[3];
    s32 c;
    /* FAKE: T-25301, a register local stored from an unsigned global before the first branch is
     * what tools/cvt_pass.py reads as a switch temporary; it turns IDO's global copy propagation
     * off for the function, so the copies below stay apart from their masks as in the original.
     * The store is dead (the loop sets d) and emits nothing. */
    register s32 d = D_80120C2B;

    /* FAKE: T-25301, the empty test gives show_indicators the first optimizer bit, so it keeps $a0 */
    if (show_indicators) {
    }
    for (d = 0; d < 3; d++) {
        sel[d] = 0;
    }
    /* FAKE: T-25301, copies and masks each on one line (as1 schedules a line as one group); the
     * second empty test ends the block, so the copies give d, c, b, a their optimizer bits */
    d = D_80120C2B; c = D_80120BA3; b = D_80120B5F; a = D_80120BE7;
    if (show_indicators) {
    }
    b &= 0x7F; c &= 0x7F; a &= 0x7F; d &= 0x7F;
    if (show_indicators == 0) {
        D_80120BE7 = a;
        D_80120B5F = b;
        D_80120BA3 = c;
        D_80120C2B = d;
        /* FAKE: T-25301, the label puts `return 0` in a block of its own; uopt forbids $v0 to a
         * range used in the block that sets the return value, and d must stay in $v0 */
        goto ret0;
    ret0:
        return 0;
    }
    sel[file_select_information_page % 3] = 1;
    D_80120BE7 = a;
    if (sel[0] != 0) {
        a = (u8)(a | 0x80);
        c = (u8)(c | 0x80);
        D_80120BE7 = a;
    }
    D_80120B5F = b;
    if (sel[1] != 0) {
        b = (u8)(b | 0x80);
        D_80120B5F = b;
    }
    D_80120C2B = d;
    D_80120BA3 = c;
    if (sel[2] != 0) {
        d = (u8)(d | 0x80);
        D_80120C2B = d;
        D_80120BA3 = c = (u8)(c | 0x80);
    }
}

s32 file_select_button_highlight_update(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;

    a = 0;
    b = 0;
    if (game_state.mode_stage == 0) {
        return 0;
    }
    c = 0;
    d = 0;
    if (game_state.mode_step < 0xA0U) {
        a = 1;
    } else if (game_state.mode_step < 0xC0U) {
        b = 1;
    } else if (game_state.mode_step < 0xE0U) {
        c = 1;
    } else {
        d = 1;
    }
    if (a != 0) {
        ((Actor *)actor_pool_96)[3].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[3].draw_flags &= ~0x80;
    }
    if (b != 0) {
        ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[1].draw_flags &= ~0x80;
    }
    if (c != 0) {
        ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)->draw_flags &= ~0x80;
    }
    if (d != 0) {
        ((Actor *)actor_pool_96)[2].draw_flags |= 0x80;
    } else {
        ((Actor *)actor_pool_96)[2].draw_flags &= ~0x80;
    }
}

void file_select_slot_summary_draw(void) {
    /* FAKE: unused words preserve the 0x40 stack frame (T-24112) */
    s32 pad[3];
    s32 i;
    s32 slot_index;
    s32 cleared_girls;
    SystemData *system_record = &system_data;
    GameState *file_state = &game_state;

    if (slot_info_from_system_data != 0) {
        cleared_girls = 0;
        for (i = 0; i < 13; i++) {
            if (system_record->flags[i] != 0) {
                cleared_girls++;
            }
        }
        for (i = 0; i < 14; i++) {
            if (i == system_record->selected_slot - 1) {
                ((Actor *)actor_pool_96)[i + 4].unk_43 = 0x11;
            } else {
                ((Actor *)actor_pool_96)[i + 4].unk_43 = 0;
            }
        }
        slot_index = file_state->menu_cursor_0;
        if (file_state->menu_cursor_0 != -1) {
            D_80120C6F |= 0x80;
            D_80120C92 = slot_index / 7 * 0x1F - 0x82;
            D_80120C96 = slot_index % 7 * 0x10 - 0xB;
        }
        file_select_number_draw(system_record->year[system_record->selected_slot], 0x30, -0x60);
        file_select_number_draw(system_record->month[system_record->selected_slot], 0x50, -0x60);
        file_select_number_draw(system_record->day[system_record->selected_slot], 0x70, -0x60);
        file_select_number_draw(cleared_girls, 0x5A, -0x1D);
        return;
    }
    cleared_girls = 0;
    for (i = 0; i < 13; i++) {
        if (((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].flags[i] != 0) {
            cleared_girls++;
        }
    }
    for (i = 0; i < 14; i++) {
        if (i == file_state->save_slot - 1) {
            ((Actor *)actor_pool_96)[i + 4].unk_43 = 0x11;
        } else {
            ((Actor *)actor_pool_96)[i + 4].unk_43 = 0;
        }
    }
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].year, 0x30, -0x60);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].month, 0x50, -0x60);
    file_select_number_draw(((AlbumSlotRecord *)memory_card_block)[file_state->album_slot].day, 0x70, -0x60);
    file_select_number_draw(cleared_girls, 0x5A, -0x1D);
}

void file_select_cursor_to_slot(s32 slot_index) {
    s16 x;
    s16 y;

    x = (slot_index / 7) * 0x1F - 0x82;
    ((Actor *)actor_pool)->x = x;
    y = (slot_index % 7) * 0x10 - 0xB;
    ((Actor *)actor_pool)->y = y;
    ((Actor *)actor_pool)[119].draw_flags |= 0x80;
    ((Actor *)actor_pool)[119].x = x;
    ((Actor *)actor_pool)[119].y = y;
}

void file_select_slot_information_show(void) {
    text_system_reset();
    file_select_slot_header_draw();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    file_select_information_page = 1;
    file_select_cursor_to_slot(game_state.save_slot - 1);
}

void file_select_background_iris_update(void) {
    if ((game_state.mode_stage != 0 || game_state.mode_step != 0) && file_select_iris_radius < FILE_SELECT_IRIS_UPDATE_STOP) {
        file_select_iris_radius = (file_select_iris_radius * 16 + 0x70) / 15;
        file_select_iris_x += file_select_iris_x_speed;
        file_select_iris_y += file_select_iris_y_speed;
        if (file_select_iris_x >= 0xA1) {
            file_select_iris_x_speed = -(rand() & 0xF);
        }
        if (file_select_iris_y >= 0x79) {
            file_select_iris_y_speed = -(rand() & 0xF);
        }
        if (file_select_iris_x < -0xA0) {
            file_select_iris_x_speed = rand() & 0xF;
        }
        if (file_select_iris_y < -0x78) {
            file_select_iris_y_speed = rand() & 0xF;
        }
    }
    cd_iris_wipe_draw(0x1A, 0, 0x1F7, file_select_iris_radius, file_select_iris_x, file_select_iris_y, 0x01DF7FCF, 2);
}

void file_select_settings_restore(void) {
    if (file_select_saved_options_mask != 0) {
        printf("option  kidou back ");
        if (file_select_saved_options_mask & 0x80) {
            k_speed_set(file_select_saved_text_speed);
        }
        printf("kanji_speed%d \n", file_select_saved_text_speed);
        if (file_select_saved_icon_mode >= 3) {
            game_state.icon_mode = file_select_saved_icon_mode % 3;
        }
        printf("icon_mode %d \n", file_select_saved_icon_mode);
        if (file_select_saved_menu_mode >= 4) {
            game_state.menu_mode = file_select_saved_menu_mode & 3;
        }
        printf("menu_mode %d \n", file_select_saved_menu_mode);
        if (file_select_saved_options_mask & 8) {
            game_state.cursor_speed = file_select_saved_cursor_speed;
        }
        printf("cursor_speed %d \n", file_select_saved_cursor_speed);
        if (file_select_saved_voice_mode >= 3) {
            game_state.voice_mode = file_select_saved_voice_mode % 3;
        }
        printf("sound_mode %d \n", file_select_saved_voice_mode);
        if (file_select_saved_options_mask & 0x40) {
            game_state.click_mode = file_select_saved_click_mode;
        }
        printf("click_mode %d \n", file_select_saved_click_mode);
        if (file_select_saved_options_mask & 0x20) {
            game_state.double_click_interval = file_select_saved_double_click_interval;
        }
        printf("click_speed %d \n", file_select_saved_double_click_interval);
        if (file_select_saved_options_mask & 0x10) {
            game_state.confirm_button = file_select_saved_confirm_button;
        }
        printf("mouse_ok %d \n", file_select_saved_confirm_button);
        if (*(u8 *)&game_state.icon_mode == 2) {
            game_state.icon_count = 0xF;
        } else {
            game_state.icon_count = 0xE;
        }
        hud_icon_bar_init();
        icon_disp_switch(0);
    }
    file_select_menu_mode_lock = 0;
    file_select_empty_callback();
    file_select_icon_layout_validate();
}

void file_select_icon_layout_validate(void) {
    s32 i;
    /* FAKE: unused words preserve the 0x48 stack frame and place invalid_coordinates/origin_icons/saved_icon_mode spills at sp+0x34/0x30/0x2C. T-24106 */
    s32 pad[3];
    s32 invalid_coordinates;
    s32 origin_icons;
    s32 saved_icon_mode;

    invalid_coordinates = 0;
    origin_icons = 0;
    /* FAKE: pointer view on unk_F6C emits lbu (declared s8 in header). T-24106 */
    if (*(u8 *)&game_state.icon_mode == 2) {
        for (i = 0; i < 16; i++) {
            if (((s16 *)D_80125C10)[i * 2] >= 0x7D) {
                invalid_coordinates++;
            }
            if (((s16 *)D_80125C10)[i * 2] < -0x86) {
                invalid_coordinates++;
            }
            if (((s16 *)D_80125C10)[i * 2 + 1] < -0x54) {
                invalid_coordinates++;
            }
            if (((s16 *)D_80125C10)[i * 2 + 1] >= 0x55) {
                invalid_coordinates++;
            }
            if (((s16 *)D_80125C10)[i * 2] == 0 && ((s16 *)D_80125C10)[i * 2 + 1] == 0) {
                origin_icons++;
            }
        }
    }
    printf("err %d \n", invalid_coordinates);
    printf("zero %d \n", origin_icons);
    if (origin_icons >= 2) {
        if (invalid_coordinates != 0) {
            saved_icon_mode = *(u8 *)&game_state.icon_mode;
            *(u8 *)&game_state.icon_mode = 0;
            hud_icon_bar_init();
            icon_disp_switch(0);
            *(u8 *)&game_state.icon_mode = saved_icon_mode;
        }
    }
}

void file_select_menu_mode_unlock(void) {
    file_select_menu_mode_lock = 0;
}

void file_select_empty_callback(void) {
}

void file_select_init_settings(void) {
    u8 speed;

    switch (game_state.substep) {
    case 0:
        debug_menu_active = 0;
        draw2d3d(1, 0);
        actor_draw_enable(1);
        sprite_tables_clear();
        *(s32 *)&game_state.menu_hit[3] = 0xA0;
        actor_clear_upper();
        if (game_state.options_dirty != 0) {
            printf("option setting!");
            file_select_saved_options_mask = game_state.options_dirty;
            speed = get_k_speed();
            /* FAKE: single-line store+call schedules sb file_select_saved_text_speed before jal printf. T-11570 */
            file_select_saved_text_speed = speed; printf("kanji_speed%d \n", speed);
            /* FAKE: pointer views avoid cvt_pass widening on file_select_saved_icon_mode..file_select_saved_confirm_button so uopt CSEs the stores and schedules lui $a0 before lui $at. T-24106 */
            file_select_saved_icon_mode = game_state.icon_mode;
            printf("icon_mode %d \n", *(u8 *)&file_select_saved_icon_mode);
            file_select_saved_menu_mode = game_state.menu_mode;
            printf("menu_mode %d \n", *(u8 *)&file_select_saved_menu_mode);
            file_select_saved_cursor_speed = game_state.cursor_speed;
            printf("cursor_speed %d \n", *(u8 *)&file_select_saved_cursor_speed);
            file_select_saved_voice_mode = game_state.voice_mode;
            printf("sound_mode %d \n", *(u8 *)&file_select_saved_voice_mode);
            file_select_saved_click_mode = game_state.click_mode;
            printf("click_mode %d \n", *(u8 *)&file_select_saved_click_mode);
            file_select_saved_double_click_interval = game_state.double_click_interval;
            printf("click_speed %d \n", *(u8 *)&file_select_saved_double_click_interval);
            file_select_saved_confirm_button = game_state.confirm_button;
            printf("mouse_ok %d \n", *(u8 *)&file_select_saved_confirm_button);
        } else {
            file_select_saved_options_mask = 0;
        }
        game_state.options_dirty = 0;
        file_select_menu_mode_lock = 1;
        name_entry_retained_state = 0;
        srand(system_data.seed);
        k_speed_set(TEXT_SPEED_NORMAL);
        back_clear_switch(1);
        hud_mascot_display_switch(0);
        tpage_buf_clear();
        game_state.tile_layer_enabled = 0;
        message_window_title_show(0);
        text_system_reset();
        text_set_advance(0xE);
        game_state.screen_brightness = 0;
        scene_work = 0xFF;
        hud_cursor_actor_init(0);
        hud_cursor_show(0);
        file_select_iris_radius = 1;
        file_select_iris_x = -0x50;
        file_select_iris_y = -0x50;
        file_select_iris_x_speed = 8;
        file_select_iris_y_speed = 8;
        make_color_bar16(game_state.scratch_words, 0xDF7FCF, 0xDF7FCF, 0x10, 0);
        load_palette(game_state.scratch_words, 0x17, 1, 1, 0);
        cd_iris_texture_build(0x80180000, 0x1A, 0);
        audio_bank_load_start(0, 0xBF98, 0xBF79, 0xCD21, 0xCCD9, 0xCCD1);
        game_state.substep++;
        break;
    case 1:
        if (audio_bank_load_poll() == 1) {
            mode_step_next();
        }
        break;
    }
}

void file_select_init_assets(void) {
    /* FAKE: the fields are read through a GameState pointer declared before the rect. Written as direct game_state reads the
     * rect sat at sp+0x38, the original has it at sp+0x34 with the same frame (T-11570); found by autotweak gstate, T-20830. T-21520 */
    GameState *g = &game_state;
    RECT rect;

    g->screen_brightness += 4;
    if (g->screen_brightness >= 0x80) {
        g->screen_brightness = 0x80;
    }
    switch (g->substep) {
    case 0:
        audio_bank_start(0, 0);
        cd_read_sectors(5, 0x801E0000, 0x9DBA);
        g->substep++;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x89, 0x80180000, 0x9CB5);
            g->substep++;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            rect.x = 0x3E0;
            rect.y = 0x100;
            rect.h = 0x80;
            rect.w = 0x20;
            func_8009C884(&rect, (void *)0x801E0200);
            load_palette(0x801E0000, 0x18, 1, 2, 0);
            g->screen_brightness = 0x80;
            g->substep = 3;
        }
        break;
    case 3:
        mode_step_next();
        break;
    }
}

void file_select_init_menus(void) {
    s32 i;
    s32 column, row;
    s16 x[16], y[16], width[16], height[16];
    s16 x2[4], y2[4], width2[4], height2[4];
    s32 addresses[6], addresses2[6];
    Actor *record;

    for (i = 0; i < 6; i++) {
        addresses[i] = 0x80182000 + i * 0x4000;
        addresses2[i] = 0x801A4000 + i * 0x4000;
    }
    for (i = 0; i < 14; i++) {
        column = i / 7;
        row = i % 7;
        x[i] = column * 32 - 144;
        y[i] = row * 16 - 18;
        width[i] = 32;
        height[i] = 16;
    }
    menu_set(0, 14, x, y, width, height);
    for (i = 0; i < 4; i++) {
        column = i & 1;
        row = i & 2;
        x2[i] = column * 44 - 144;
        y2[i] = row * 8 - 96;
        width2[i] = 40;
        height2[i] = 20;
    }
    menu_set(1, 4, x2, y2, width2, height2);
    game_state.menu_cursor_0 = 0;
    game_state.menu_cursor_1 = -1;
    file_select_unused_word_0 = 0;
    file_select_information_page = 1;
    file_select_unused_word_1 = 0;
    file_select_unused_word_2 = 0;
    record = (Actor *)actor_pool_96;
    for (i = 0; i < 45; i++) {
        record[i].unk_18 = i;
        record[i].unk_01 = 5;
        record[i].flags = 64;
        record[i].quad_flags = 0x01000000;
        record[i].draw_flags = 36;
        record[i].frame_table = (u8 *)0x801C048C;
        record[i].sequence_table = (u8 *)0x801C0540;
        record[i].tpage_table = (u8 *)0x801C047C;
        record[i].unk_04 = 0;
        record[i].unk_43 = 0;
        record[i].unk_05 = 128;
        record[i].x = -160;
        record[i].y = -120;
        record[i].brightness = 128;
    }
    for (i = 0; i < 11; i++) {
        record[i + 45].unk_01 = 4;
        record[i + 45].flags = 64;
        record[i + 45].quad_flags = 0x40000000;
        record[i + 45].draw_flags = 36;
        record[i + 45].frame_table = sprite_frame_table;
        record[i + 45].sequence_table = actor_default_sequence_table;
        record[i + 45].tpage_table = actor_default_texture_blocks;
        record[i + 45].unk_04 = 8;
        record[i + 45].unk_18 = 89;
        record[i + 45].unk_43 = 0;
        record[i + 45].unk_05 = 1;
        record[i + 45].x = i / 4 * 65 - 15;
        record[i + 45].y = i % 4 * 16 + 36;
    }
    load_tpage_buf_lock(5, ((s32 *)D_80120684)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_80120684)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_80120684)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_80120684)[3], 1);
    ((Actor *)actor_pool)[114].draw_flags = 0;
    ((Actor *)actor_pool)[100].draw_flags |= 0x80;
    ((Actor *)actor_pool)[101].draw_flags |= 0x80;
    ((Actor *)actor_pool)[102].draw_flags |= 0x80;
    record = &((Actor *)actor_pool)[103];
    for (i = 0; i < 12; i++) {
        record[i].draw_flags |= 128;
    }
    if (slot_info_from_system_data != 0) {
        ((Actor *)actor_pool)[126].unk_43 = 0x11;
    } else {
        ((Actor *)actor_pool)[127].unk_43 = 0x11;
    }
    ((Actor *)actor_pool)[126].draw_flags |= 0x80;
    ((Actor *)actor_pool)[127].draw_flags |= 0x80;
    load_palette(0x801A2000, 0x10, 1, 1, 0);
    load_palette(0x801A2000, 0x11, 1, 1, 1);
    load_palette(0x80180000, 0x1F, 1, 1, 0);
    prim_bg_slot_setup((s32) addresses, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 0xA, 0xD);
    prim_bg_slot_setup((s32) addresses2, 1, 6, 3, 2, 0x1F0, -0xA0, -0x78, 8, 0xA);
    actor_clear(0x72);
    mode_stage_next();
}

void file_select_mode_stage_init(void) {
    switch (game_state.mode_step) {
    case 0:
        file_select_init_settings();
        return;
    case 1:
        file_select_init_assets();
        return;
    case 2:
        file_select_init_menus();
        return;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_memory_card_check_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_memory_card_check_text_2);

void file_select_memory_card_check_begin(void) {
    memory_card_state_reset();
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_memory_card_check_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_memory_card_check_text_2, 0);
    k_disp_start(2);
    mode_step_next();
}

void file_select_memory_card_check_poll(void) {
    u8 memory_card_result;
    GameState *file_state; /* FAKE: file_state and the u8 copy of the selector only move it to $v1 (T-11120, permuter) */

    if (memory_card_check_poll() != 0) {
        hud_cursor_show(1);
        memory_card_stop();
        file_state = &game_state;
        memory_card_result = file_state->memory_card_result;
        if (memory_card_result == 1) {
            printf("EXIST CARD!");
            mode_stage_next();
        } else if (memory_card_result == 0xF1) {
            printf("FILE_SEL UNFORMAT FIND!");
            mode_stage_set(0xC0);
        } else if (memory_card_result == 0xFF) {
            printf("NO CARD!");
            mode_step_next();
        }
    } else if ((u32)game_state.memory_card_timer >= 0x201U) {
        hud_cursor_show(1);
        memory_card_stop();
        printf("NO COMUNICATION!");
        mode_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_no_memory_card_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_no_memory_card_text_2);

void file_select_no_memory_card_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_no_memory_card_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_no_memory_card_text_2, 0);
    k_disp_start(2);
    mode_step_next();
}

void file_select_no_memory_card_wait(void) {
    if (game_state.pad_pressed & 0x860) {
        mode_step_next();
    }
}

void file_select_play_without_memory_card_prompt(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "新たにゲームを始めますか？", 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x30, 1, hud_confirm_button_label(), 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x40, 2, hud_cancel_button_label(), 0);
    k_disp_start(5);
    mode_step_next();
}

void file_select_play_without_memory_card_answer(void) {
    if (game_state.pad_pressed & PAD_START) {
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

void file_select_insert_memory_card_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "メモリーカードを", 0);
    set_kanji_string(-0x40, 0x30, 1, "差し込み口１", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
    set_kanji_string(-0x40, 0x40, 1, hud_confirm_button_label(), 0);
    memory_card_stop();
    k_disp_start(5);
    mode_step_next();
}

void file_select_insert_memory_card_wait(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        memory_card_state_reset();
        mode_step_set(0);
    }
}

void file_select_mode_stage_memory_card_check(void) {
    switch (game_state.mode_step) {
    case 0:
        file_select_memory_card_check_begin();
        break;
    case 1:
        file_select_memory_card_check_poll();
        break;
    case 2:
        file_select_no_memory_card_message();
        break;
    case 3:
        file_select_no_memory_card_wait();
        break;
    case 4:
        file_select_play_without_memory_card_prompt();
        break;
    case 5:
        file_select_play_without_memory_card_answer();
        break;
    case 6:
        file_select_insert_memory_card_message();
        break;
    case 7:
        file_select_insert_memory_card_wait();
        break;
    }
}

/* Falls through without a return statement. */
s32 file_select_scan(void) {
    s32 pad[3]; /* FAKE: unused words above occupied_slots, the original frame has them (T-9020, T-11120) */
    s32 occupied_slots;
    s32 i;

    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep++;
        break;
    case 1:
        if (memory_card_scan_poll() == 2) {
            printf("SEARCH_CARD");
            game_state.substep++;
        } else if (game_state.mode_step_timer.u >= 0x201U) {
            memory_card_state_reset();
            mode_stage_set(1);
            memory_card_stop();
        }
        break;
    case 2:
        occupied_slots = 0;
        for (i = 0; i < 14; i++) {
            if (D_800E7D11[i + 4] != 0) {
                occupied_slots++;
            }
        }
        text_system_reset();
        if (slot_info_from_system_data != 0) {
            mode_step_next();
        } else if (occupied_slots != 0) {
            mode_step_set(0x20);
        } else {
            mode_step_set(0x40);
        }
        break;
    case 3:
        break;
    }
}

void file_select_system_file_read(void) {
    /* FAKE: unused words preserve the 0x40 stack frame. T-22023 */
    s32 pad[2];
    s32 slot_index;
    s32 i;
    GameState *file_state = &game_state;

    switch (file_state->substep) {
    case 0:
        memory_card_start();
        file_state->substep++;
        break;
    case 1:
        if (memory_card_system_read_poll() == 3) {
            memory_card_stop();
            file_state->substep++;
        } else if (((u32) file_state->memory_card_timer >= 0x201U) || (file_state->memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        if (memory_card_system_bank_load(0) == -1) {
            file_state->substep++;
        } else {
            text_system_reset();
            file_select_menu_mode_unlock();
            slot_index = 0xFF;
            if (system_data.slot_status[system_data.selected_slot] == 1) {
                slot_index = system_data.selected_slot - 1;
            } else {
                for (i = 1; i < 14; i++) {
                    if (system_data.slot_status[i] == 1) {
                        system_data.selected_slot = i;
                        slot_index = i - 1;
                        break;
                    }
                }
            }
            if (slot_index != 0xFF) {
                file_select_cursor_to_slot(slot_index);
            }
            file_select_slot_header_draw();
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            file_select_information_page = 1;
            mode_step_next();
        }
        break;
    case 3:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルは", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
        k_disp_start(2);
        memory_card_stop();
        file_state->substep++;
        break;
    case 4:
        if (file_state->pad_pressed & 0x60) {
            file_state->substep++;
        }
        break;
    case 5:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作り直しますか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, (s32) hud_confirm_button_label(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, (s32) hud_cancel_button_label(), 0);
        k_disp_start(6);
        file_state->substep++;
        break;
    case 6:
        if (file_state->pad_pressed & PAD_CIRCLE) {
            file_state->substep++;
        } else if (file_state->pad_pressed & PAD_CROSS) {
            for (i = 0; i < 4; i++) {
                if (memory_card_file_delete(0) == 1) {
                    break;
                }
            }
            mode_stage_set(1);
        }
        break;
    case 7:
        mode_stage_set(0xD0);
        break;
    }
}

void file_select_list_input(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1) && D_800E7D15[slot_index] == 1) {
        if (D_800E7D36 == slot_index + 1) {
            audio_command(0x501);
            mode_step_set(3);
            file_state->save_slot = D_800E7D36;
        } else {
            D_800E7D36 = file_state->menu_cursor_0 + 1;
            text_system_reset();
            file_select_slot_header_draw();
            k_sub_reset_point_set();
            k_sub_disp_start(0);
            file_select_cursor_to_slot(slot_index);
        }
    }
    file_select_page_indicator_update(1);
    file_select_slot_summary_draw();
    file_select_information_page_draw();
    file_select_buttons_input();
}

void file_select_album_load(void) {
    u8 memory_card_result;

    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep++;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep++;
        } else if ((memory_card_result = game_state.memory_card_result) == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xF0);
        }
        break;
    case 3:
        if (memory_card_game_bank_load(0) == 1) {
            file_select_menu_mode_unlock();
            printf("SYS0 DATA OK!");
            memory_card_stop();
            audio_command(0x501);
            memory_card_start();
            D_800E7D36 = game_state.save_slot;
            memory_card_slot_summary_store();
            game_state.substep++;
        } else {
            memory_card_result = game_state.memory_card_result;
            printf("s->mem_stat %d \n", memory_card_result);
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xF0);
        }
        break;
    case 4:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            mode_stage_set(3);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || ((memory_card_result = game_state.memory_card_result) == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
    file_select_slot_summary_draw();
}

void file_select_album_only_read(void) {
    s32 pad[2]; /* FAKE: unused words above the buffer, the original frame has them. T-22023 */
    u8 text[256];
    s32 i;
    u8 memory_card_result;
    GameState *file_state = &game_state;

    switch (file_state->substep) {
    case 0:
        memory_card_start();
        for (i = 1; i < 15; i++) {
            if (system_data.slot_status[i] == 1) {
                file_state->save_slot = i;
                break;
            }
        }
        if (i < 15) {
            file_state->substep++;
        } else {
            memory_card_stop();
            mode_step_set(0x40);
        }
        printf("load file %d \n", file_state->save_slot);
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            file_state->substep++;
        } else if ((memory_card_result = file_state->memory_card_result) == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) file_state->memory_card_timer >= 0x201U) || (memory_card_result == 0xD0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            file_select_slot_information_show();
            mode_step_next();
        } else {
            text_system_reset();
            strcpy(text, D_800B3288[file_state->save_slot]);
            strcat(text, "番アルバムのデータは");
            set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
            set_kanji_string(-0x40, 0x30, 0xF, (s32) "壊れています。", 0);
            D_800E7D14[file_state->save_slot] = 0xFF;
            memory_card_stop();
            file_state->substep = 0;
        }
        break;
    }
}

void file_select_album_only_list_input(void) {
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1) && D_800E7D15[slot_index] == 1) {
        if (file_state->save_slot == slot_index + 1) {
            mode_step_set(0x24);
        } else {
            mode_step_set(0x22);
            file_state->save_slot = file_state->menu_cursor_0 + 1;
        }
    }
    file_select_page_indicator_update(1);
    file_select_slot_summary_draw();
    file_select_information_page_draw();
    file_select_buttons_input();
}

void file_select_album_only_loading_message(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "ロードしています。", 0);
    k_disp_start(2);
    mode_step_next();
}

/* Original implicit-int dispatch falls through without a return statement. */
s32 file_select_album_only_read_chosen(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            game_state.substep += 1;
        } else if (game_state.memory_card_result == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            file_select_slot_information_show();
            mode_step_set(0x21);
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_debug_load_valid_text);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_debug_load_failed_text);

void file_select_album_only_load(void) {
    if (memory_card_game_bank_load(0) == 1) {
        file_select_menu_mode_unlock();
        printf(file_select_debug_load_valid_text);
        memory_card_stop();
        audio_command(0x501);
        mode_stage_set(3);
    } else {
        printf(file_select_debug_load_failed_text);
        memory_card_state_reset();
        mode_stage_set(1);
    }
    file_select_slot_summary_draw();
}

void file_select_no_data_prompt(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "このゲームのデータがありません", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "新たにアルバムを作成し、", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "ゲームをプレイしますか？", 0);
    set_kanji_string(-0x40, 0x50, 0xF, "　　はい…　　いいえ…　", 0);
    set_kanji_string(6, 0x50, 1, hud_confirm_button_label(), 0);
    set_kanji_string(0x5A, 0x50, 2, hud_cancel_button_label(), 0);
    k_disp_start(6);
    mode_step_next();
}

void file_select_no_data_answer(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        mode_step_set(0x44);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_play_unsaved_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_play_unsaved_text_2);

void file_select_no_data_decline_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_play_unsaved_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_play_unsaved_text_2, 0);
    k_disp_start(2);
    mode_step_next();
}

void file_select_no_data_decline_wait(void) {
    if (game_state.pad_pressed & 0x820) {
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_create_slot_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_create_slot_text_2);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_create_slot_text_3);

void file_select_create_slot_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_create_slot_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_create_slot_text_2, 0);
    set_kanji_string(-0x40, 0x40, 0xF, file_select_create_slot_text_3, 0);
    k_disp_start(3);
    mode_step_next();
}

void file_select_create_slot_choose(void) {
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1)) {
        mode_step_next();
        file_state->save_slot = file_state->menu_cursor_0 + 1;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_system_file_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_system_file_text_2);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_system_file_text_3);

void file_select_create_system_file_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_system_file_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_system_file_text_2, 0);
    set_kanji_string(-0x40, 0x40, 0xF, file_select_system_file_text_3, 0);
    k_disp_start(3);
    mode_step_next();
}

void file_select_create_system_file_wait(void) {
    if (game_state.pad_pressed & 0x860) {
        mode_step_next();
    }
}

void file_select_create_system_file_prompt(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "システムファイルを作成しますか", 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x30, 1, hud_confirm_button_label(), 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x40, 2, hud_cancel_button_label(), 0);
    k_disp_start(5);
    mode_step_next();
}

void file_select_create_system_file_answer(void) {
    if (game_state.pad_pressed & 0x820) {
        mode_step_set(0x4C);
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_next();
    }
}

s32 file_select_create_album_only(void) {
    u8 message_text[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */
    u8 slot_status;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(message_text, D_800B32CC[game_state.save_slot - 1]);
        strcat(message_text, "番アルバムを作成しています。");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) message_text, 0);
        k_disp_start(2);
        slot_status = D_800E7D14[game_state.save_slot];
        if (slot_status == 0xFF) {
            game_state.substep += 1;
        } else if (slot_status == 1) {
            game_state.substep += 1;
        } else if (memory_card_file_create((s32) game_state.save_slot) == 1) {
            game_state.substep += 1;
        } else {
            mode_stage_set(4);
        }
        break;
    case 1:
        memory_card_start();
        boot_game_data_init();
        boot_settings_defaults_set();
        game_state.substep += 1;
        break;
    case 2:
        if (memory_card_save_write_poll() == 0x14) {
            game_state.substep += 1;
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 3:
        memory_card_stop();
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}


void file_select_create_album_and_system_file(void) {
    /* FAKE: unused words, the original frame has them. T-22023 */
    s32 pad[2];
    s32 i;
    u8 text[256];
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　と、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "システムファイルを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "作成しています。", 0);
        k_disp_start(3);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (memory_card_file_create((s32) game_state.save_slot) == 1) {
            game_state.substep += 1;
        } else {
            mode_stage_set(4);
        }
        break;
    case 2:
        if (memory_card_file_create(0) == 1) {
            game_state.substep += 1;
        } else {
            memory_card_file_delete((s32) game_state.save_slot);
            mode_stage_set(4);
        }
        break;
    case 3:
        memory_card_stop();
        D_800E7D36 = game_state.save_slot;
        i = 0;
        while (i < 15) {
            system_data.slot_status[i] = 0;
            ++i;
        }
        D_800E7D14[game_state.save_slot] = 1;
        *D_800E7D14 = 1;
        game_state.save_slot = 0;
        memory_card_start();
        memory_card_slot_summary_store();
        game_state.substep += 1;
        break;
    case 4:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.memory_card_timer) >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 5:
        memory_card_start();
        boot_game_data_init();
        boot_settings_defaults_set();
        boot_settings_defaults_set();
        game_state.save_slot = D_800E7D36;
        /* FAKE: pointer view avoids cvt_pass widening on unk_1118 so uopt keeps unk_1118 in $a1 across the switch. T-24106 */
        printf("s->file_no %x \n", *(u8 *)&game_state.save_slot);
        game_state.substep += 1;
        break;
    case 6:
        if (memory_card_save_write_poll() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.memory_card_timer) >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 7:
        memory_card_stop();
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_new_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_new_text_2);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_new_text_3);

void file_select_new_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_new_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_new_text_2, 0);
    set_kanji_string(-0x40, 0x40, 0xF, file_select_new_text_3, 0);
    k_disp_start(3);
    mode_step_next();
}

void file_select_new_wait(void) {
    if (game_state.pad_pressed & 0x860) {
        mode_step_next();
    }
    file_select_buttons_input();
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_new_choose_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_new_choose_text_2);

void file_select_new_choose_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_new_choose_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_new_choose_text_2, 0);
    k_disp_start(2);
    mode_step_next();
}

void file_select_new_choose_slot(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1)) {
        audio_command(0x501);
        file_state->save_slot = slot_index + 1;
        if (D_800E7D15[slot_index] != 0) {
            mode_step_next();
            file_state->save_slot = file_state->menu_cursor_0 + 1;
        } else if (slot_info_from_system_data != 0) {
            mode_step_set(0xA8);
        } else {
            mode_step_set(0xA9);
        }
    }
    file_select_buttons_input();
}

void file_select_new_redo_prompt(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "やりなおします。", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, hud_confirm_button_label(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, hud_cancel_button_label(), 0);
    k_disp_start(6);
    mode_step_next();
}

void file_select_new_redo_answer(void) {
    if (game_state.pad_pressed & PAD_CROSS) {
        audio_command(0x501);
        mode_step_set(0);
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (slot_info_from_system_data) {
            mode_step_set(0xA6);
        } else {
            mode_step_set(0xA7);
        }
    }
}

/* Packed date word: day in bits 0-3, month in bits 4-8, week in bits 9-10. */

void file_select_profile_retain(void) {
    memcpy(name_entry_retained_family_name, &game_state.player_family_name, 8);
    memcpy(name_entry_retained_given_name, &game_state.player_given_name, 8);
    memcpy(name_entry_retained_nickname, &game_state.player_nickname, 0xC);
    name_entry_retained_player_month = ((BirthdayBits *)&game_state.player_birthday)->month - 1;
    name_entry_retained_player_day = ((BirthdayBits *)&game_state.player_birthday)->day - 1;
    name_entry_retained_player_blood = (((BirthdayBits *)&game_state.player_birthday)->blood_type + 3) & 3;
    name_entry_retained_shiori_month = ((BirthdayBits *)&game_state.girls[0].birthday.w)->month - 1;
    name_entry_retained_shiori_day = ((BirthdayBits *)&game_state.girls[0].birthday.w)->day - 1;
    name_entry_retained_shiori_blood = (((BirthdayBits *)&game_state.girls[0].birthday.w)->blood_type + 3) & 3;
    name_entry_retained_state = 1;
}

void file_select_new_redo_load(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        printf("NEW FILE %d \n", game_state.save_slot);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (game_state.memory_card_result == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 3:
        if (memory_card_game_bank_load(0) == 1) {
            file_select_profile_retain();
            file_select_menu_mode_unlock();
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 4:
        printf("%d file replay", game_state.save_slot);
        boot_game_data_init();
        boot_settings_defaults_set();
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}

void file_select_new_redo_load_no_system(void) {
    switch (game_state.substep) {
    case 0:
        memory_card_start();
        printf("NEW FILE %d \n", game_state.save_slot);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (game_state.memory_card_result == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 3:
        if (memory_card_game_bank_load(0) == 1) {
            file_select_profile_retain();
            file_select_menu_mode_unlock();
            game_state.substep += 1;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 4:
        printf("%d file replay", game_state.save_slot);
        boot_game_data_init();
        boot_settings_defaults_set();
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}

void file_select_new_create_with_system(void) {
    u8 operation_step;
    s32 memory_card_operation;
    u8 text[256];

    operation_step = game_state.substep;
    switch (operation_step) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　を、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        k_disp_start(2);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if ((memory_card_operation = memory_card_file_create((s32) game_state.save_slot)) == 1) {
            game_state.substep += 1;
        } else {
            mode_stage_set(4);
        }
        break;
    case 2:
        memory_card_start();
        boot_game_data_init();
        boot_settings_defaults_set();
        game_state.substep += 1;
        break;
    case 3:
        if ((memory_card_operation = memory_card_save_write_poll()) == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.memory_card_timer) >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 4:
        memory_card_stop();
        memory_card_start();
        D_800E7D36 = game_state.save_slot;
        memory_card_slot_summary_store();
        game_state.substep += 1;
        break;
    case 5:
        if ((memory_card_operation = memory_card_system_write_poll()) == 0x13) {
            memory_card_stop();
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.memory_card_timer) >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 6:
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}

void file_select_new_create_album_only(void) {
    u8 operation_step;
    s32 memory_card_operation;
    u8 text[256];

    operation_step = game_state.substep;
    switch (operation_step) {
    case 0:
        text_system_reset();
        strcpy(text, D_800B32CC[game_state.save_slot - 1]);
        strcat(text, "番アルバム　を、");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        k_disp_start(2);
        game_state.substep += 1;
    default:
        break;
    case 1:
        if ((memory_card_operation = memory_card_file_create((s32) game_state.save_slot)) == 1) {
            game_state.substep += 1;
        } else {
            mode_stage_set(4);
        }
        break;
    case 2:
        memory_card_start();
        boot_game_data_init();
        boot_settings_defaults_set();
        game_state.substep += 1;
        break;
    case 3:
        if ((memory_card_operation = memory_card_save_write_poll()) == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
        } else if (((u32) (*(u32 *)&game_state.memory_card_timer) >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 4:
        file_select_settings_restore();
        game_mode_set(GAME_MODE_NAME_ENTRY);
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_copy_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_copy_text_2);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_copy_text_3);

void file_select_copy_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_copy_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_copy_text_2, 0);
    set_kanji_string(-0x40, 0x40, 0xF, file_select_copy_text_3, 0);
    k_disp_start(3);
    mode_step_next();
}

void file_select_copy_source_choose(void) {
    s32 pad[2]; /* FAKE: unused, gives the original's 0x38 frame; real source unknown. T-16920 */
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1) && D_800E7D15[slot_index] == 1) {
        audio_command(0x501);
        mode_step_next();
        file_state->save_slot = slot_index + 1;
    }
    file_select_buttons_input();
}

/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* Falls through without a return statement. */
/* The u8 mode copy and the s32 state give the original 0x138 frame (text at sp+0x30). T-11080 */
s32 file_select_copy_source_load(void) {
    s32 operation_step;
    u8 memory_card_result;
    u8 text[0x100];

    operation_step = game_state.substep;
    switch (operation_step) {
    case 0:
        memory_card_start();
        game_state.substep++;
    default:
        break;
    case 1:
        if (memory_card_save_read_poll() == 4) {
            game_state.substep++;
        } else if ((memory_card_result = game_state.memory_card_result) == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 2:
        if (memory_card_bank_validate(0) == 1) {
            file_select_slot_information_show();
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 3:
        if (memory_card_game_bank_load(0) == 1) {
            file_select_menu_mode_unlock();
            file_select_menu_mode_lock = 1;
            printf("COPY LOAD SUM !\n");
            game_state.substep++;
        } else {
            D_800E7D14[game_state.save_slot] = 0xFF;
            mode_stage_set(0xFF);
        }
        break;
    case 4:
        text_system_reset();
        strcpy(text, D_800B3288[game_state.save_slot]);
        strcat(text, "番アルバムを");
        set_kanji_string(-0x40, 0x20, 0xF, text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, "どのアルバムにコピーしますか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, "カーソルで選んで下さい。", 0);
        k_disp_start(3);
        mode_step_next();
        break;
    }
}


void file_select_copy_destination_choose(void) {
    s32 slot_index;
    GameState *file_state = &game_state;

    file_select_buttons_input();
    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if (file_state->pad_pressed & 0x820) {
        if (file_state->menu_cursor_0 != -1) {
            file_state->save_slot = file_state->menu_cursor_0 + 1;
            if (D_800E7D15[slot_index] != 0) {
                mode_step_next();
            } else if (memory_card_file_create(file_state->save_slot) == 1) {
                audio_command(0x501);
                mode_step_set(0xC6);
            } else {
                mode_stage_set(4);
            }
        }
    }
}

void file_select_copy_overwrite_prompt(void) {
    u8 buf[0x108]; /* FAKE: 8 bytes more than the neighbours give the original frame (0x140). T-9170 */

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムに");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "上書きしてもいいですか？", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, hud_confirm_button_label(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, hud_cancel_button_label(), 0);
    k_disp_start(6);
    mode_step_next();
}

void file_select_copy_overwrite_answer(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        audio_command(0x501);
        mode_step_next();
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
        game_state.substep = 2;
    }
}

void file_select_copy_route(void) {
    if (slot_info_from_system_data) {
        mode_step_set(0xC7);
    } else {
        mode_step_set(0xC8);
    }
}

s32 file_select_copy_write_with_system(void) {
    u8 message_text[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(message_text, D_800B3288[game_state.save_slot]);
        strcat(message_text, "番アルバムに");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) message_text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        k_disp_start(2);
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_write_poll() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            printf("NO MEM CARD \n");
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        memory_card_stop();
        D_800E7D36 = game_state.save_slot;
        game_state.save_slot = 0;
        memory_card_start();
        memory_card_slot_summary_store();
        game_state.substep += 1;
        break;
    case 3:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            mode_step_set(0);
            game_state.substep = 2;
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
}


s32 file_select_copy_write_album_only(void) {
    u8 message_text[0x108]; /* FAKE: observed frame and buffer slot, T-9580. */

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        strcpy(message_text, D_800B3288[game_state.save_slot]);
        strcat(message_text, "番アルバムに");
        set_kanji_string(-0x40, 0x20, 0xF, (s32) message_text, 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        k_disp_start(2);
        memory_card_start();
        game_state.substep += 1;
        break;
    case 1:
        if (memory_card_save_write_poll() == 0x14) {
            D_800E7D14[game_state.save_slot] = 1;
            game_state.substep += 1;
            memory_card_stop();
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            printf("NO MEM CARD \n");
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        mode_step_set(0);
        game_state.substep = 2;
        break;
    }
}


INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_clear_text_1);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_clear_text_2);

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_clear_text_3);

void file_select_clear_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_clear_text_1, 0);
    set_kanji_string(-0x40, 0x30, 0xF, file_select_clear_text_2, 0);
    set_kanji_string(-0x40, 0x40, 0xF, file_select_clear_text_3, 0);
    k_disp_start(3);
    mode_step_next();
}

void file_select_clear_choose_slot(void) {
    s32 slot_index;
    GameState *file_state = &game_state;

    menu_check(0, cursor_x, cursor_y);
    file_state->pad_pressed &= ~0x100;
    menu_cursor_update(0);
    slot_index = file_state->menu_cursor_0;
    if (file_state->menu_cursor_0 != -1) {
        D_80120C6F |= 0x80;
        D_80120C92 = slot_index / 7 * 0x1F - 0x82;
        D_80120C96 = slot_index % 7 * 0x10 - 0xB;
    }
    if ((file_state->pad_pressed & 0x820) && (file_state->menu_cursor_0 != -1) && D_800E7D15[slot_index] != 0) {
        mode_step_next();
        audio_command(0x501);
        file_state->save_slot = file_state->menu_cursor_0 + 1;
    }
    file_select_buttons_input();
}

void file_select_clear_prompt(void) {
    s32 pad; /* FAKE: unused, declared before buf for its stack slot (T-3330) */
    u8 buf[0x100];

    text_system_reset();
    strcpy(buf, D_800B3288[game_state.save_slot]);
    strcat(buf, "番アルバムを");
    set_kanji_string(-0x40, 0x20, 0xF, buf, 0);
    set_kanji_string(-0x40, 0x30, 0xF, "本当に消去していいですか？", 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_800B3C70, 0);
    set_kanji_string(0x14, 0x40, 1, hud_confirm_button_label(), 0);
    set_kanji_string(-0x40, 0x50, 0xF, D_800B3C88, 0);
    set_kanji_string(0x14, 0x50, 2, hud_cancel_button_label(), 0);
    k_disp_start(6);
    mode_step_next();
}

void file_select_clear_answer(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        audio_command(0x501);
        if (slot_info_from_system_data) {
            mode_step_set(0xE4);
        } else {
            mode_step_set(0xE5);
        }
    } else if (game_state.pad_pressed & PAD_CROSS) {
        mode_step_set(0);
    }
}

void file_select_clear_delete_with_system(void) {
    switch (game_state.substep) {
    case 0:
        if (memory_card_file_delete((s32) game_state.save_slot) == 1) {
            printf("DELETE OK");
            D_800E7D14[game_state.save_slot] = 0;
            game_state.substep += 1;
        } else {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 1:
        memory_card_start();
        D_800E7D36 = game_state.save_slot;
        game_state.save_slot = 0;
        game_state.substep += 1;
        break;
    case 2:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (game_state.memory_card_result == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
}

void file_select_clear_delete_album_only(void) {
    if (game_state.substep == 0) {
        if (memory_card_file_delete(game_state.save_slot) == 1) {
            printf("DELETE OK");
            D_800E7D14[game_state.save_slot] = 0;
            mode_step_set(0);
            game_state.substep = 2;
            return;
        }
        memory_card_stop();
        memory_card_state_reset();
        mode_stage_set(1);
    }
}

void file_select_clear_done(void) {
    mode_step_set(0);
}

void file_select_mode_stage_list(void) {
    u8 mode_step;
    game_state.mode_step_timer.w += 1;
    mode_step = game_state.mode_step;
    switch (mode_step) {
    case 0x0:
        file_select_scan();
        return;
    case 0x1:
        file_select_system_file_read();
        return;
    case 0x2:
        file_select_list_input();
        return;
    case 0x3:
        file_select_album_load();
        return;
    case 0x20:
        file_select_album_only_read();
        return;
    case 0x21:
        file_select_album_only_list_input();
        return;
    case 0x22:
        file_select_album_only_loading_message();
        return;
    case 0x23:
        file_select_album_only_read_chosen();
        return;
    case 0x24:
        file_select_album_only_load();
        return;
    case 0x40:
        file_select_no_data_prompt();
        return;
    case 0x41:
        file_select_no_data_answer();
        return;
    case 0x42:
        file_select_no_data_decline_message();
        return;
    case 0x43:
        file_select_no_data_decline_wait();
        return;
    case 0x44:
        file_select_create_slot_message();
        return;
    case 0x45:
        file_select_create_slot_choose();
        return;
    case 0x46:
        file_select_create_system_file_message();
        return;
    case 0x47:
        file_select_create_system_file_wait();
        return;
    case 0x48:
        file_select_create_system_file_prompt();
        return;
    case 0x49:
        file_select_create_system_file_answer();
        return;
    case 0x4A:
        file_select_create_album_only();
        return;
    case 0x4C:
        file_select_create_album_and_system_file();
        return;
    case 0xA0:
        file_select_new_message();
        return;
    case 0xA1:
        file_select_new_wait();
        return;
    case 0xA2:
        file_select_new_choose_message();
        return;
    case 0xA3:
        file_select_new_choose_slot();
        return;
    case 0xA4:
        file_select_new_redo_prompt();
        return;
    case 0xA5:
        file_select_new_redo_answer();
        return;
    case 0xA6:
        file_select_new_redo_load();
        return;
    case 0xA7:
        file_select_new_redo_load_no_system();
        return;
    case 0xA8:
        file_select_new_create_with_system();
        return;
    case 0xA9:
        file_select_new_create_album_only();
        return;
    case 0xC0:
        file_select_copy_message();
        return;
    case 0xC1:
        file_select_copy_source_choose();
        return;
    case 0xC2:
        file_select_copy_source_load();
        return;
    case 0xC3:
        file_select_copy_destination_choose();
        return;
    case 0xC4:
        file_select_copy_overwrite_prompt();
        return;
    case 0xC5:
        file_select_copy_overwrite_answer();
        return;
    case 0xC6:
        file_select_copy_route();
        return;
    case 0xC7:
        file_select_copy_write_with_system();
        return;
    case 0xC8:
        file_select_copy_write_album_only();
        return;
    case 0xE0:
        file_select_clear_message();
        return;
    case 0xE1:
        file_select_clear_choose_slot();
        return;
    case 0xE2:
        file_select_clear_prompt();
        return;
    case 0xE3:
        file_select_clear_answer();
        return;
    case 0xE4:
        file_select_clear_delete_with_system();
        return;
    case 0xE5:
        file_select_clear_delete_album_only();
        return;
    case 0xE6:
        file_select_clear_done();
        return;
    default:
        file_select_settings_restore();
        game_mode_set(GAME_MODE_START);
        return;
    }
}

/* Falls through without a return statement. */
s32 file_select_mode_stage_album_broken(void) {
    u32 slot_index;

    switch (game_state.substep) {
    case 0:
        slot_index = game_state.save_slot - 1;
        cd_audio_request_set(D_800B3A3C[slot_index * 2], D_800B3A3C[slot_index * 2 + 1], slot_index % 13);
        audio_command(0x300);
        printf("CALL XA");
        game_state.substep++;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep++;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep++;
        }
        break;
    case 3:
        memory_card_stop();
        if (slot_info_from_system_data != 0) {
            mode_stage_set(2);
            mode_step_set(2);
        } else {
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
}

/* Falls through without a return statement. */
s32 file_select_mode_stage_memory_card_error(void) {
    u32 slot_index;

    switch (game_state.substep) {
    case 0:
        slot_index = game_state.save_slot - 1;
        cd_audio_request_set(D_800B394C[slot_index * 2], D_800B394C[slot_index * 2 + 1], slot_index % 13);
        audio_command(0x300);
        printf("CALL XA");
        game_state.substep++;
        break;
    case 1:
        if (cd_audio_state() & 4) {
            game_state.substep++;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            game_state.substep++;
        }
        break;
    case 3:
        memory_card_stop();
        memory_card_state_reset();
        mode_stage_set(1);
        break;
    }
}

void file_select_loaded_greeting(void) {
    /* FAKE: unused words preserve the 0x38 stack frame (T-24112) */
    s32 pad[3];
    u8 greeting_girl;
    u8 qualifying_girl;
    u8 final_event_girl;
    s32 clip_variant;
    s32 *clip_pair;

    /* FAKE: unk_110D and unk_03E are already u8; the (u8 *) views emit Ilod/Istr so as1 schedules the base address into the delay slot (T-24112) */
    switch (*(u8 *)&game_state.substep) {
    case 0:
        dec_bg_reset();
        dec_bg_show_switch(2);
        bg_capture_screen(0xF0);
        hud_cursor_show(0);
        sprite_tables_clear();
        game_state.event_log_count = hud_event_log_scan(&qualifying_girl, &final_event_girl, 1);
        greeting_girl = qualifying_girl;
        printf("girl %x \n", qualifying_girl);
        printf("i %x \n", greeting_girl);
        if (greeting_girl >= 13) {
            if (*(u8 *)&game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            } else if (*(u8 *)&game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            } else {
                audio_command(5);
                audio_command(0xC1);
            }
            *(u8 *)&game_state.substep = 4;
        } else if (greeting_girl == 11) {
            if (game_state.scene_flags[0x4D] != 0) {
                clip_variant = 1;
                clip_pair = D_800B3B9C + greeting_girl * 4 + clip_variant * 2;
                cd_audio_request_set(clip_pair[0], clip_pair[1], greeting_girl);
                audio_command(0x300);
                *(u8 *)&game_state.substep += 1;
            } else {
                clip_variant = 0;
                clip_pair = D_800B3B9C + greeting_girl * 4 + clip_variant * 2;
                cd_audio_request_set(clip_pair[0], clip_pair[1], greeting_girl);
                audio_command(0x300);
                *(u8 *)&game_state.substep += 1;
            }
        } else if (greeting_girl == 12) {
            if (system_data.flags[12] != 0 || game_state.girl_cleared[12] != 0) {
                if ((u8)game_state.girls[12].flags.b[3] >= 0x29U) {
                    clip_variant = 0;
                } else {
                    clip_variant = 1;
                }
                clip_pair = D_800B3B9C + greeting_girl * 4 + clip_variant * 2;
                cd_audio_request_set(clip_pair[0], clip_pair[1], greeting_girl);
                printf("伊集院A");
                audio_command(0x300);
                *(u8 *)&game_state.substep += 1;
            } else if ((u8)game_state.girls[12].flags.b[3] >= 0x29U) {
                clip_variant = 1;
                clip_pair = D_800B3B9C + greeting_girl * 4 + clip_variant * 2;
                cd_audio_request_set(clip_pair[0], clip_pair[1], greeting_girl);
                audio_command(0x300);
                printf("伊集院B");
                *(u8 *)&game_state.substep += 1;
            } else {
                printf("伊集院C");
                *(u8 *)&game_state.substep = 4;
            }
        } else {
            if ((get_g_zyotai_s(greeting_girl) & 0x7F) == 0) {
                clip_variant = 1;
            } else {
                clip_variant = 0;
            }
            clip_pair = D_800B3B9C + greeting_girl * 4 + clip_variant * 2;
            cd_audio_request_set(clip_pair[0], clip_pair[1], greeting_girl);
            audio_command(0x300);
            *(u8 *)&game_state.substep += 1;
        }
        break;
    case 1:
        game_state.screen_brightness = 0x80;
        set_dec_bri(0x80);
        sprite_tables_clear();
        if (cd_audio_state() & 4) {
            *(u8 *)&game_state.substep += 1;
        }
        break;
    case 2:
        if (cd_audio_state() & 2) {
            *(u8 *)&game_state.substep += 1;
            if (*(u8 *)&game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            } else if (*(u8 *)&game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            } else {
                audio_command(5);
                audio_command(0xC1);
            }
        }
        break;
    case 3:
        *(u8 *)&game_state.substep += 1;
        break;
    case 4:
        if ((u8)game_state.screen_brightness < 4U) {
            printf("A\n");
            game_state.clear_color_g = 0;
            game_state.clear_color_r = 0;
            game_state.clear_color_b = 0;
            mode_step_next();
        }
        break;
    }
    file_select_fade_step();
    dec_bg_draw(2);
    D_8011ECA4 = 1;
}

/* FAKE: s32 with no return value; the void form keeps the six fade stores in $t6 where the original copies the value to $v0 (T-22023) */
s32 file_select_fade_step(void) {
    /* FAKE: unk_03A is already u8; the (u8 *) view only moves the global to $a0 like the original (T-11570) */
    if (*(u8 *)&game_state.screen_brightness >= 3) {
        *(u8 *)&game_state.screen_brightness -= 2;
        /* the chain order sets the store order: D_801220E0 is stored last, the others from D_80122106 outward */
        D_801220E1 = D_801220E2 = D_80122104 = D_80122105 = D_80122106 = D_801220E0 = *(u8 *)&game_state.screen_brightness;
        if (*(u8 *)&game_state.screen_brightness < 0x80) {
            set_dec_bri(*(u8 *)&game_state.screen_brightness);
        }
    }
}

void file_select_loaded_leave(void) {
    if (game_state.substep++ >= 0x41U) {
        file_select_settings_restore();
        set_dec_bri(0);
        if (game_state.year == 0x62 && game_state.month == 3 && game_state.day == 1) {
            file_select_profile_retain();
            boot_game_data_init();
            boot_event_state_init();
            game_state.event_log_count = 0;
            game_mode_set(GAME_MODE_NAME_ENTRY);
        } else if (game_state.year == 0x5F && game_state.month == 4 && game_state.day == 4) {
            file_select_profile_retain();
            boot_game_data_init();
            boot_event_state_init();
            game_state.event_log_count = 0;
            game_mode_set(GAME_MODE_NAME_ENTRY);
        } else {
            hud_day_bgm_load();
            game_mode_set(GAME_MODE_COMMAND_SCREEN);
            mode_stage_set(0xFF);
        }
    }
    D_8011ECA4 = 1;
}

s32 file_select_mode_stage_loaded(void) {
    switch (game_state.mode_step) {
    case 0:
        file_select_loaded_greeting();
        break;
    case 1:
        file_select_loaded_leave();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/NAME_ENT/data/name_entry/file_select.rodata", file_select_no_space_text);

void file_select_no_space_message(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, file_select_no_space_text, 0);
    k_disp_start(2);
    mode_step_next();
}

/* FAKE: biased byte cursor, alternate end expression and statement layout preserve addressing and scheduling, T-9580. */
void file_select_no_space_check(void)
{
  u8 *slot_status_cursor;
  u8 *slot_scan_end;
  if (game_state.pad_pressed & 0x860)
  {
    slot_status_cursor = (u8 *) (&system_data); do
    {
      if (slot_status_cursor[4] == 1)
      {
        break;
      }
      slot_status_cursor++;
    }
    while (slot_status_cursor != (&D_800E7D1F));
    slot_scan_end = ((u8 *) (&system_data)) - -15;
    if (slot_status_cursor == slot_scan_end)
    {
      mode_step_next();
      return;
    }
    mode_stage_set(2);
    if (slot_info_from_system_data != 0)
    {
      mode_step_set(1);
      return;
    }
    mode_step_set(0x20);
  }
}

void file_select_no_space_hint(void) {
    text_system_reset();
    set_kanji_string(-0x40, 0x20, 0xF, "別のメモリーカードを差し込み口", 0);
    set_kanji_string(-0x40, 0x30, 0xF, "１にセットするか、他のゲームの", 0);
    set_kanji_string(-0x40, 0x40, 0xF, "データを消去してください。", 0);
    k_disp_start(2);
    mode_step_next();
}

void file_select_no_space_hint_wait(void) {
    if (game_state.pad_pressed & 0x860) {
        mode_stage_set(1);
        mode_step_set(6);
    }
}

void file_select_mode_stage_no_space(void) {
    switch (game_state.mode_step) {
    case 0:
        file_select_no_space_message();
        return;
    case 1:
        file_select_no_space_check();
        return;
    case 2:
        file_select_no_space_hint();
        return;
    case 3:
        file_select_no_space_hint_wait();
        return;
    }
}

void file_select_system_file_route(void) {
    if (slot_info_from_system_data) {
        mode_step_set(1);
    } else {
        mode_step_set(2);
    }
}

void file_select_system_file_rebuild(void) {
    s32 i;
    u8 memory_card_result;
    SystemData *system_record = &system_data;
    GameState *file_state = &game_state;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを作り直して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, hud_confirm_button_label(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, hud_cancel_button_label(), 0);
        k_disp_start(6);
        for (i = 0; i < 16; i++) file_select_rebuild_old_counts[i] = 0;
        game_state.substep++;
        break;
    case 1:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            for (i = 0; i < 16; i++) {
                file_select_rebuild_old_counts[i] = system_record->flags[i];
            }
            game_state.substep++;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        k_disp_start(3);
        game_state.substep++;
        break;
    case 3:
        if ((game_state.pad_pressed != 0) || ((u32) game_state.mode_step_timer.w >= 0x41U)) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        k_disp_start(2);
        game_state.mode_step_timer.w = 0;
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed != 0) {
            game_state.substep++;
        }
        break;
    case 6:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        for (i = 0; i < 16; i++) file_select_rebuild_counts[i] = 0;
        game_state.save_slot = 1;
        game_state.substep++;
        break;
    case 7:
        if (game_state.save_slot >= 15) {
            for (i = 0; i < 16; i++) {
                if (file_select_rebuild_old_counts[i] < file_select_rebuild_counts[i]) {
                    system_record->flags[i] = file_select_rebuild_counts[i];
                } else {
                    system_record->flags[i] = file_select_rebuild_old_counts[i];
                }
                if (file_select_rebuild_counts[i] != 0 && file_select_rebuild_counts[i] != file_select_rebuild_old_counts[i]) {
                    system_record->flags[i] = 1;
                }
            }
            game_state.substep = 32;
        } else {
            while (system_record->slot_status[file_state->save_slot] == 0) {
                game_state.save_slot++;
                if (game_state.save_slot >= 15) {
                    for (i = 0; i < 16; i++) {
                        if (file_select_rebuild_old_counts[i] < file_select_rebuild_counts[i]) {
                            system_record->flags[i] = file_select_rebuild_counts[i];
                        } else {
                            system_record->flags[i] = file_select_rebuild_old_counts[i];
                        }
                        if (file_select_rebuild_counts[i] != 0 && file_select_rebuild_counts[i] != file_select_rebuild_old_counts[i]) {
                            system_record->flags[i] = 1;
                        }
                    }
                    game_state.substep = 32;
                    break;
                }
            }
            if (game_state.substep != 32) {
                system_record->selected_slot = game_state.save_slot;
                game_state.substep++;
                memory_card_start();
            }
        }
        break;
    case 8:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep++;
        } else if ((memory_card_result = game_state.memory_card_result) == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 9:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep++;
        } else {
            system_record->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 7;
        }
        break;
    case 10:
        if (memory_card_game_bank_load(0) == 1) {
            memory_card_slot_summary_store();
            game_state.save_slot++;
            game_state.substep = 7;
            for (i = 0; i < 16; i++) {
                if (file_state->girl_cleared[i] != 0) {
                    file_select_rebuild_counts[i]++;
                }
            }
        } else {
            system_record->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 7;
        }
        break;
    case 32:
        memory_card_start();
        system_record->selected_slot = 0;
        game_state.substep++;
        break;
    case 33:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            mode_stage_set(2);
            mode_step_set(0);
            game_state.substep = 2;
            audio_command(0x50F);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || ((memory_card_result = game_state.memory_card_result) == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
}

/* Original implicit-int state machine falls through without a return statement. */
s32 file_select_system_file_create(void) {
    s32 i;
    u8 memory_card_result;
    SystemData *system_record = &system_data;
    GameState *file_state = &game_state;

    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "システムファイルを作成して", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "いいですか？", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) D_800B3C70, 0);
        set_kanji_string(0x14, 0x40, 1, hud_confirm_button_label(), 0);
        set_kanji_string(-0x40, 0x50, 0xF, (s32) D_800B3C88, 0);
        set_kanji_string(0x14, 0x50, 2, hud_cancel_button_label(), 0);
        k_disp_start(6);
        game_state.substep++;
        break;
    case 1:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            game_state.substep++;
        } else if (game_state.pad_pressed & PAD_CROSS) {
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "約１分ほど時間がかかりますが、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "途中でメモリーカードを", 0);
        set_kanji_string(-0x40, 0x40, 0xF, (s32) "抜かないで下さい。", 0);
        k_disp_start(3);
        game_state.mode_step_timer.w = 0;
        game_state.substep++;
        break;
    case 3:
        if ((game_state.pad_pressed != 0) || ((u32) game_state.mode_step_timer.w >= 0x41U)) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "準備が出来たら、", 0);
        set_kanji_string(-0x40, 0x30, 0xF, (s32) "何かボタンを押してください。", 0);
        k_disp_start(2);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed != 0) {
            game_state.substep++;
        }
        break;
    case 6:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, (s32) "しばらくお待ち下さい。", 0);
        k_disp_start(1);
        for (i = 0; i < 16; i++) file_select_rebuild_counts[i] = 0;
        game_state.save_slot = 1;
        game_state.substep++;
        break;
    case 7:
        if (memory_card_file_create(0) == 1) {
            slot_info_from_system_data = 1;
            game_state.substep++;
        } else {
            mode_stage_set(4);
        }
        break;
    case 8:
        if (game_state.save_slot >= 15) {
            game_state.substep = 32;
            for (i = 0; i < 16; i++) {
                system_record->flags[i] = file_select_rebuild_counts[i];
            }
        } else {
            while (system_record->slot_status[file_state->save_slot] == 0) {
                game_state.save_slot++;
                if (game_state.save_slot >= 15) {
                    for (i = 0; i < 16; i++) {
                        system_record->flags[i] = file_select_rebuild_counts[i];
                    }
                    game_state.substep = 32;
                    break;
                }
            }
            if (game_state.substep != 32) {
                system_record->selected_slot = game_state.save_slot;
                game_state.substep++;
                memory_card_start();
            }
        }
        break;
    case 9:
        if (memory_card_save_read_poll() == 4) {
            memory_card_stop();
            game_state.substep++;
        } else if ((memory_card_result = game_state.memory_card_result) == 0xF0) {
            memory_card_state_reset();
            mode_stage_set(1);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || (memory_card_result == 0xD0)) {
            memory_card_stop();
            mode_stage_set(0xFF);
        }
        break;
    case 10:
        if (memory_card_bank_validate(0) == 1) {
            game_state.substep++;
        } else {
            system_record->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 8;
        }
        break;
    case 11:
        if (memory_card_game_bank_load(0) == 1) {
            memory_card_slot_summary_store();
            game_state.save_slot++;
            game_state.substep = 8;
            for (i = 0; i < 16; i++) {
                if (file_state->girl_cleared[i] != 0) {
                    file_select_rebuild_counts[i]++;
                }
            }
        } else {
            system_record->slot_status[game_state.save_slot] = 0xFF;
            game_state.save_slot++;
            game_state.substep = 8;
        }
        break;
    case 32:
        memory_card_start();
        system_record->selected_slot = 0;
        game_state.substep++;
        break;
    case 33:
        if (memory_card_system_write_poll() == 0x13) {
            memory_card_stop();
            mode_stage_set(2);
            mode_step_set(0);
            game_state.substep = 2;
            audio_command(0x50F);
        } else if (((u32) game_state.memory_card_timer >= 0x201U) || ((memory_card_result = game_state.memory_card_result) == 0xF0)) {
            memory_card_stop();
            memory_card_state_reset();
            mode_stage_set(1);
        }
        break;
    }
}

s32 file_select_mode_stage_system_file(void) {
    game_state.mode_step_timer.w += 1;
    switch (game_state.mode_step) {
    case 0:
        file_select_system_file_route();
        break;
    case 1:
        file_select_system_file_rebuild();
        break;
    case 2:
        file_select_system_file_create();
        break;
    }
    game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
}

s32 file_select_format_prompt(void) {
    switch (game_state.substep) {
    case 0:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        game_state.substep++;
    default:
        break;
    case 1:
        if (game_state.pad_pressed & 0x860) {
            game_state.substep++;
        }
        break;
    case 2:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "初期化していいですか？", 0);
        set_kanji_string(-0x40, 0x30, 0xF, D_800B3C70, 0);
        set_kanji_string(0x14, 0x30, 1, hud_confirm_button_label(), 0);
        set_kanji_string(-0x40, 0x40, 0xF, D_800B3C88, 0);
        set_kanji_string(0x14, 0x40, 2, hud_cancel_button_label(), 0);
        k_disp_start(5);
        game_state.substep++;
        break;
    case 3:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            if (memory_card_format() == 1) {
                mode_stage_set(2);
            } else {
                mode_stage_set(1);
            }
        } else if (game_state.pad_pressed & PAD_CROSS) {
            game_state.substep++;
        }
        break;
    case 4:
        text_system_reset();
        set_kanji_string(-0x40, 0x20, 0xF, "別のメモリーカードを", 0);
        set_kanji_string(-0x40, 0x30, 1, "差し込み口１", 0);
        set_kanji_string(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
        set_kanji_string(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
        set_kanji_string(-0x40, 0x40, 1, hud_confirm_button_label(), 0);
        k_disp_start(5);
        game_state.substep++;
        break;
    case 5:
        if (game_state.pad_pressed & PAD_CIRCLE) {
            mode_stage_set(1);
        }
        break;
    }
}

void file_select_mode_stage_unformatted(void) {
    if (game_state.mode_step == 0) {
        file_select_format_prompt();
    }
}

/* Records 128..140, then 141..151 (flag byte +3). The shared counter keeps the first loop rolled; the second (11 trips) is unrolled by four. T-11080 */
void file_select_mode_handler(void) {
    s32 i;

    for (i = 0; i < 13; i++) {
        ((Actor *)actor_pool)[i + 128].draw_flags &= 0x7F;
    }
    for (i = 0; i < 11; i++) {
        ((Actor *)actor_pool)[i + 141].draw_flags &= 0x7F;
    }
    file_select_page_indicator_update(0);
    game_state.mode_stage_timer += 1;
    switch (game_state.mode_stage) {
    case 0x0:
        file_select_mode_stage_init();
        break;
    case 0x1:
        file_select_mode_stage_memory_card_check();
        break;
    case 0x2:
        file_select_mode_stage_list();
        break;
    case 0x3:
        file_select_mode_stage_loaded();
        break;
    case 0x4:
        file_select_mode_stage_no_space();
        break;
    case 0xC0:
        file_select_mode_stage_unformatted();
        break;
    case 0xD0:
        file_select_mode_stage_system_file();
        break;
    case 0xF0:
        file_select_mode_stage_album_broken();
        break;
    case 0xFF:
        file_select_mode_stage_memory_card_error();
        break;
    }
    file_select_background_iris_update();
    file_select_girl_icons_visibility_update();
    k_disp_inc();
    hud_cursor_update();
    file_select_button_highlight_update();
    file_select_slot_status_overlay_draw();
    file_select_flash_update();
    if (file_select_menu_mode_lock != 0) {
        game_state.menu_mode = MENU_MODE_MOUSE_LIKE;
    }
}

void file_select_flash_update(void) {
    if ((game_state.mode_stage | game_state.mode_step) != 0) {
        scene_work -= 1;
    }
    if (scene_work > 0) {
        prim_flash_gray(scene_work);
    }
}

s32 file_select_slot_status_overlay_draw(void) {
    s32 temp_t3;
    s32 temp_t7;
    s32 slot_index;

    for (slot_index = 0; slot_index < 14; slot_index++) {
        if (system_data.slot_status[slot_index + 1] == 0) {
            prim_gradient_rect(((slot_index / 7) * 0x1F) - 0x90, ((slot_index % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (system_data.slot_status[slot_index + 1] == 0xFF) {
            prim_gradient_rect(((slot_index / 7) * 0x1F) - 0x90, ((slot_index % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
    }
    dtd_on_tpage(0, 0, 2, 2, 0);
    if (game_state.mode_stage != 0) {
        if (slot_info_from_system_data != 0) {
            temp_t7 = game_state.frame_count & 0x1F;
            if (temp_t7 != 0) {
                if (temp_t7 == 0x10) {
                    ((Actor *)actor_pool)[126].draw_flags &= 0x7F;
                }
            } else {
                ((Actor *)actor_pool)[126].draw_flags |= 0x80;
            }
            ((Actor *)actor_pool)[126].unk_43 = 0x11;
            ((Actor *)actor_pool)[127].unk_43 = 0;
        } else {
            temp_t3 = game_state.frame_count & 0x1F;
            if (temp_t3 != 0) {
                if (temp_t3 == 0x10) {
                    ((Actor *)actor_pool)[127].draw_flags &= 0x7F;
                }
            } else {
                ((Actor *)actor_pool)[127].draw_flags |= 0x80;
            }
            ((Actor *)actor_pool)[126].unk_43 = 0;
            ((Actor *)actor_pool)[127].unk_43 = 0x11;
        }
    }
}

void file_select_buttons_input(void) {
    s8 *operation_cursor; /* FAKE: the switch reads the selector through a pointer so the later read stays a separate range; permuter idiom, T-11120 */

    menu_check(1, cursor_x, cursor_y);
    menu_cursor_update(1);
    operation_cursor = &game_state.menu_cursor_1;
    if (game_state.pad_pressed & PAD_CIRCLE) {
        switch (*operation_cursor) {
        case 0:
            file_select_slot_markers_hide();
            mode_step_set(0xC0);
            printf("0");
            break;
        case 1:
            file_select_slot_markers_hide();
            mode_step_set(0xA0);
            printf("1");
            break;
        case 2:
            file_select_slot_markers_hide();
            mode_step_set(0xE0);
            printf("2");
            break;
        case 3:
            mode_step_set(0);
            printf("3");
            break;
        }
        sprite_quads_clear(0, 0x40);
        if (game_state.menu_cursor_1 >= 0) {
            file_select_information_page = 1;
        }
    }
    if (file_select_system_button_click() != 0) {
        sprite_quads_clear(0, 0x40);
    }
}

s32 file_select_system_button_click(void) {
    if (game_state.pad_pressed & PAD_CIRCLE) {
        if (cursor_x < -0x50 && cursor_x >= -0x8F) {
            if (cursor_y < -0x18 && cursor_y >= -0x27) {
                audio_command(0x50F);
                mode_stage_set(0xD0);
                return 1;
            }
        }
    }
    return 0;
}

void file_select_slot_markers_hide(void) {
    s32 i;

    /* FAKE: loop body on the for line; as1 schedules by source line and then fills the delay slot like the original. T-9170 */
    for (i = 0; i < 13; i++) sprite_quads[i].flags = 0x80000000;
}
