/* name_entry.c: English name entry (T-39506). Not part of the original executable: only the English
 * build compiles it into the island (see src/english/text.c). The functions replace NAME_ENT functions
 * of the same name without the english_ prefix (config/english_hooks.txt, unit NAME_ENT), so they run
 * only while NAME_ENT is in the overlay slot; the NAME_ENT symbols they use are resolved at the main
 * link from the default NAME_ENT build (tools/english_imports.py).
 *
 * Changes from src/ovl/name_entry/name_entry.c (wiki/english-text-runtime.md, "Name entry"):
 *   - a sixth tab, the English letter page (A-Z, a-z, hyphen, apostrophe, period, space), placed first
 *     and selected when the screen opens; the five original tabs move down to make room (0x1A apart
 *     instead of 0x20). Its selector is a text label until the label art (name-label-abc) is merged;
 *     english_tab_label_draw is the integration point;
 *   - names are NUL-terminated strings of single-byte letters, with kana from the original pages
 *     appended as two-byte characters: at most ENGLISH_NAME_LETTERS bytes for the family and given
 *     names, ENGLISH_NICKNAME_LETTERS for the nickname. The original fills unused slots with full-width
 *     spaces and overwrites in place; here a letter is appended at the end and CROSS deletes the last
 *     character;
 *   - one insertion marker after the last letter instead of one marker per two-byte slot;
 *   - name_entry_profile_store copies the strings as they are and joins the full name with a space. */
#include "common.h"
#include "ovl/name_entry.h"
#include "english/name_entry.h"
#include "english/script.h"
#include "english/text.h"

#ifdef TOKIMEMO_ENGLISH

#define NAME_ENTRY_TAB_LATIN 5 /* English letter page; NameEntryTab values 0-4 are the original tabs */
#define ENGLISH_TAB_COUNT 6

/* top y of each tab frame by NameEntryTab value, and the letter page layout */
#define ENGLISH_GRID_X (-0x78)
#define ENGLISH_GRID_Y (-0x30)
#define ENGLISH_CELL 16
#define ENGLISH_LATIN_FIRST_COLUMN 1
#define ENGLISH_LATIN_COLUMNS 13
#define ENGLISH_SPACE_COLUMN 5 /* the space key covers columns 5-8 of the last row */
#define ENGLISH_SPACE_WIDTH 4

static s16 english_tab_y[ENGLISH_TAB_COUNT] = { -0x16, 4, 0x1E, 0x38, 0x52, -0x30 };
static u8 english_latin_rows[4][ENGLISH_LATIN_COLUMNS + 1] = {
    "ABCDEFGHIJKLM", "NOPQRSTUVWXYZ", "abcdefghijklm", "nopqrstuvwxyz",
};
static u8 english_latin_marks[] = "-'.";
static u8 english_space_label[] = "Space";
static u8 english_tab_label[] = "ABC";
static u8 english_blank[] = "　"; /* the full-width space the original pages put in empty cells (D_800B3644) */

/* x of each name on the input screen (name_entry_names_draw) and its byte limit, by name part */
static s16 english_name_x[3] = { -0x68, -0x30, 0x48 };
static u8 english_name_limit[3] = { ENGLISH_NAME_LETTERS, ENGLISH_NAME_LETTERS, ENGLISH_NICKNAME_LETTERS };

static s32 english_name_entry_scroll_arrows_update(void);

static u8 *english_name_buffer(s32 name_part) {
    if (name_part == NAME_ENTRY_NAME_PART_FAMILY) {
        return name_entry_family_name;
    }
    if (name_part == NAME_ENTRY_NAME_PART_GIVEN) {
        return name_entry_given_name;
    }
    return name_entry_nickname;
}

static u8 *english_prompt_text(s32 name_part) {
    if (name_part == NAME_ENTRY_NAME_PART_FAMILY) {
        return name_entry_family_prompt_text;
    }
    if (name_part == NAME_ENTRY_NAME_PART_GIVEN) {
        return name_entry_given_prompt_text;
    }
    return name_entry_nickname_prompt_text;
}

/* Selector label of the letter page tab. Integration point for the name-label-abc art: once merged,
 * draw it with a label actor like actors 109-113 and drop this text entry. */
static void english_tab_label_draw(void) {
    set_kanji_string(-0x9D + (0x1B - english_text_width(english_tab_label)) / 2, english_tab_y[NAME_ENTRY_TAB_LATIN] + 2,
                     name_entry_tab == NAME_ENTRY_TAB_LATIN ? 0xE : 0xA, english_tab_label, 0);
}

static void english_latin_page_draw(u8 *heading_text) {
    s32 row;

    text_system_reset();
    set_kanji_string(-0x98, -0x68, 0, heading_text, 0);
    english_text_cell = ENGLISH_CELL;
    for (row = 0; row < 4; row++) {
        set_kanji_string(ENGLISH_GRID_X + ENGLISH_LATIN_FIRST_COLUMN * ENGLISH_CELL, ENGLISH_GRID_Y + row * ENGLISH_CELL,
                         0xA, english_latin_rows[row], 0);
    }
    set_kanji_string(ENGLISH_GRID_X + ENGLISH_LATIN_FIRST_COLUMN * ENGLISH_CELL, ENGLISH_GRID_Y + 4 * ENGLISH_CELL, 0xA,
                     english_latin_marks, 0);
    english_text_cell = 0;
    set_kanji_string(ENGLISH_GRID_X + ENGLISH_SPACE_COLUMN * ENGLISH_CELL
                         + (ENGLISH_SPACE_WIDTH * ENGLISH_CELL - english_text_width(english_space_label)) / 2,
                     ENGLISH_GRID_Y + 4 * ENGLISH_CELL, 0xA, english_space_label, 0);
}

/* Draws the page of the current tab for a name part, then the names (as after every page change). */
static void english_page_draw(s32 name_part) {
    switch (name_entry_tab) {
    case NAME_ENTRY_TAB_HIRAGANA:
        name_entry_hiragana_page_draw((s32)english_prompt_text(name_part));
        break;
    case NAME_ENTRY_TAB_KATAKANA:
        name_entry_katakana_page_draw((s32)english_prompt_text(name_part));
        break;
    case NAME_ENTRY_TAB_SYMBOLS:
        name_entry_symbol_page_draw(english_prompt_text(name_part));
        break;
    case NAME_ENTRY_TAB_KANJI:
        name_entry_kanji_page_draw(name_part + 1);
        break;
    default:
        english_latin_page_draw(english_prompt_text(name_part));
        break;
    }
    k_sub_reset_point_set();
    english_name_entry_names_draw();
}

void english_name_entry_names_draw(void) {
    k_sub_reset();
    set_kanji_string(english_name_x[0], -0x58, 0xE, name_entry_family_name, 0);
    set_kanji_string(english_name_x[1], -0x58, 0xE, name_entry_given_name, 0);
    set_kanji_string(english_name_x[2], -0x58, 0xE, name_entry_nickname, 0);
    english_tab_label_draw();
    k_sub_disp_start(4);
}

/* The character under the cursor: a letter of the English page, or the two bytes the original
 * name_entry_character_put would store (taken from it through the first nickname slot, which is
 * restored). Returns the byte count, 0 for an empty cell. */
static s32 english_cell_character(u8 *out) {
    s32 column;
    s32 row;
    u8 saved[2];
    u8 saved_cursor;

    column = name_entry_cell_column;
    row = name_entry_cell_row;
    if (name_entry_tab == NAME_ENTRY_TAB_LATIN) {
        column -= ENGLISH_LATIN_FIRST_COLUMN;
        if (row < 4 && column >= 0 && column < ENGLISH_LATIN_COLUMNS) {
            out[0] = english_latin_rows[row][column];
            return 1;
        }
        if (row == 4 && column >= 0 && column < (s32)strlen(english_latin_marks)) {
            out[0] = english_latin_marks[column];
            return 1;
        }
        column += ENGLISH_LATIN_FIRST_COLUMN;
        if (row == 4 && column >= ENGLISH_SPACE_COLUMN && column < ENGLISH_SPACE_COLUMN + ENGLISH_SPACE_WIDTH) {
            out[0] = ' ';
            return 1;
        }
        return 0;
    }
    if (name_entry_tab > NAME_ENTRY_TAB_KANJI) {
        return 0;
    }
    saved[0] = name_entry_nickname[0];
    saved[1] = name_entry_nickname[1];
    saved_cursor = name_entry_cursor;
    /* a cell the original leaves untouched reads as the blank */
    name_entry_nickname[0] = english_blank[0];
    name_entry_nickname[1] = english_blank[1];
    name_entry_cursor = 6;
    name_entry_character_put(NAME_ENTRY_NAME_PART_NICKNAME);
    out[0] = name_entry_nickname[0];
    out[1] = name_entry_nickname[1];
    name_entry_nickname[0] = saved[0];
    name_entry_nickname[1] = saved[1];
    name_entry_cursor = saved_cursor;
    if (out[0] == english_blank[0] && out[1] == english_blank[1]) {
        return 0;
    }
    return 2;
}

static void english_character_put(s32 name_part) {
    u8 character[3];
    u8 *name;
    s32 n;
    s32 k;

    k = english_cell_character(character);
    if (k == 0) {
        return;
    }
    name = english_name_buffer(name_part);
    n = strlen(name);
    if (n + k > english_name_limit[name_part] || (character[0] == ' ' && n == 0)) {
        return;
    }
    name[n] = character[0];
    if (k == 2) {
        name[n + 1] = character[1];
    }
    name[n + k] = 0;
}

/* CROSS: removes the last character (two bytes for a kana). */
static void english_character_delete(s32 name_part) {
    u8 *name;
    s32 i;
    s32 last;

    name = english_name_buffer(name_part);
    last = -1;
    for (i = 0; name[i] != 0; i += (name[i] >= 0x80 && (name[i] < 0xA0 || name[i] >= 0xE0) && name[i + 1] != 0) ? 2 : 1) {
        last = i;
    }
    if (last >= 0) {
        name[last] = 0;
    }
}

static void english_name_entry_slot_markers_update(s32 name_part) {
    Actor *marker;
    u8 *name;
    s32 i;

    marker = (Actor *)actor_pool + 98;
    for (i = 1; i < 11; i++) {
        marker[i].draw_flags &= 0x7F;
    }
    name = english_name_buffer(name_part);
    if ((s32)strlen(name) >= english_name_limit[name_part]) {
        marker->draw_flags &= 0x7F;
        return;
    }
    marker->draw_flags |= 0x80;
    marker->x = english_name_x[name_part] + english_text_width(name) + 3;
    marker->y = -0x4A;
    if (marker->unk_18 == 0xD) {
        marker->unk_18 = 0;
    }
    name_entry_actor_animation_start(98);
}

static void english_name_entry_tab_hit_test(void) {
    s32 i;

    name_entry_tab_hover = -1;
    for (i = 0; i < ENGLISH_TAB_COUNT; i++) {
        if (cursor_x >= -0x9D && cursor_x < -0x80 && english_tab_y[i] < cursor_y && cursor_y < english_tab_y[i] + 0x10) {
            name_entry_tab_hover = i;
        }
    }
}

/* One highlight actor (114) moves to the selected original tab; the others stay hidden. The highlight
 * sprite would cover the text label of the letter page, which shows its selection by color instead. */
static void english_name_entry_tab_highlight_update(void) {
    Actor *highlight;
    s32 i;

    highlight = (Actor *)actor_pool + 114;
    for (i = 0; i < 5; i++) {
        highlight[i].draw_flags &= 0x7F;
    }
    if (name_entry_tab != NAME_ENTRY_TAB_LATIN) {
        highlight->y = english_tab_y[name_entry_tab] + 8;
        highlight->draw_flags |= 0x80;
    }
    /* actor 119 (D_80120C96 in the original) points at the hovered tab */
    if (name_entry_tab_hover >= NAME_ENTRY_TAB_HIRAGANA) {
        ((Actor *)actor_pool)[119].y = english_tab_y[name_entry_tab_hover] + 8;
        return;
    }
    ((Actor *)actor_pool)[119].y = -0xB4;
}

void english_name_entry_frames_draw(s32 alternate_color) {
    s32 col;
    s32 i;

    col = alternate_color != 0 ? 0x90002 : 0x8081F;
    for (i = 0; i < ENGLISH_TAB_COUNT; i++) {
        prim_gradient_rect(-0x9D, english_tab_y[i], 0x1B, 0xF, 4, col, 0x83);
    }
    prim_gradient_rect(-0x9A, -0x6A, 0x130, 0x20, 4, col, 0x83);
    prim_gradient_rect(-0x7F, -0x40, 0x11D, 0x70, 4, col, 0x83);
    if (name_entry_tab == NAME_ENTRY_TAB_KANJI) {
        prim_gradient_rect(-0x7A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(-0x1A, 0x36, 0x50, 0x30, 4, col, 0x83);
        prim_gradient_rect(0x46, 0x36, 0x50, 0x30, 4, col, 0x83);
    }
    dtd_on_tpage(0, 0, 4, 0, 0);
    if (!(game_state.pad_pressed & PAD_CIRCLE) && !(game_state.pad_pressed & PAD_START)) {
        if (!(game_state.pad_pressed & PAD_CROSS)) {
            name_entry_preset_chord_check();
        }
    }
}

/* Trailing spaces are dropped; an empty name gets a preset, as in the original. */
static s32 english_name_entry_name_finish(s32 name_part) {
    u8 *name;
    u8 *preset;
    s32 n;
    s32 preset_row;

    preset_row = rand() % 15;
    name = english_name_buffer(name_part);
    n = strlen(name);
    while (n > 0 && name[n - 1] == ' ') {
        name[--n] = 0;
    }
    if (n != 0) {
        return 0;
    }
    if (name_part == NAME_ENTRY_NAME_PART_FAMILY) {
        preset = name_entry_preset_family_names + preset_row * 8;
    } else if (name_part == NAME_ENTRY_NAME_PART_GIVEN) {
        preset = name_entry_preset_given_names + preset_row * 8;
    } else {
        preset = name_entry_preset_nicknames + preset_row * 12;
    }
    english_copy(name, preset, english_name_limit[name_part] + 1);
    return 0;
}

void english_name_entry_step_init(void) {
    Actor *actor;
    s32 i;

    if (*(u8 *)&name_entry_retained_state < 2) {
        if (*(u8 *)&name_entry_retained_state == 1) {
            memcpy(name_entry_family_name, name_entry_retained_family_name, 8);
            memcpy(name_entry_given_name, name_entry_retained_given_name, 8);
            memcpy(name_entry_nickname, name_entry_retained_nickname, 0xC);
            name_entry_family_name[7] = 0;
            name_entry_given_name[7] = 0;
            name_entry_nickname[11] = 0;
            name_entry_player_month = (u8)name_entry_retained_player_month % 12;
            name_entry_player_day = (u8)name_entry_retained_player_day % 31;
            name_entry_player_blood = (u8)name_entry_retained_player_blood & 3;
            name_entry_shiori_month = (u8)name_entry_retained_shiori_month % 12;
            name_entry_shiori_day = (u8)name_entry_retained_shiori_day % 31;
            name_entry_shiori_blood = (u8)name_entry_retained_shiori_blood & 3;
        } else {
            for (i = 0; i < 0xC; i++) {
                if (i < 8) {
                    name_entry_family_name[i] = 0;
                    name_entry_given_name[i] = 0;
                }
                name_entry_nickname[i] = 0;
            }
            name_entry_player_month = NAME_ENTRY_SELECTION_UNSET;
            name_entry_player_day = NAME_ENTRY_SELECTION_UNSET;
            name_entry_player_blood = NAME_ENTRY_SELECTION_UNSET;
            name_entry_shiori_month = NAME_ENTRY_SELECTION_UNSET;
            name_entry_shiori_day = NAME_ENTRY_SELECTION_UNSET;
            name_entry_shiori_blood = NAME_ENTRY_SELECTION_UNSET;
            name_entry_preset_chord_check();
        }
    }
    name_entry_cursor = 0;
    name_entry_tab = NAME_ENTRY_TAB_LATIN;
    name_entry_reading = 0;
    name_entry_reading_column = -1;
    name_entry_reading_row = -1;
    name_entry_input_actors_init();
    /* the five tab labels and their highlights follow the new tab positions */
    actor = (Actor *)actor_pool;
    for (i = 0; i < 5; i++) {
        actor[109 + i].y = english_tab_y[i] + 8;
        actor[114 + i].y = english_tab_y[i] + 8;
    }
    cd_audio_request_set(name_entry_family_prompt_clip_start, name_entry_family_prompt_clip_end, 0);
    audio_command(0x300);
    english_page_draw(NAME_ENTRY_NAME_PART_FAMILY);
    *(u8 *)&name_entry_retained_state = 2;
    mode_step_next();
}

/* name_entry_given_name_init and name_entry_nickname_init */
static void english_name_part_init(s32 name_part, s32 clip_start, s32 clip_end) {
    text_system_reset();
    if (name_entry_tab == NAME_ENTRY_TAB_DONE) {
        name_entry_tab = NAME_ENTRY_TAB_LATIN;
    }
    name_entry_cursor = name_part == NAME_ENTRY_NAME_PART_GIVEN ? 3 : 6;
    D_801206AC = 0;
    D_801206BA = 0;
    D_801206BE = 0;
    cd_audio_request_set(clip_start, clip_end, 0);
    audio_command(0x300);
    english_page_draw(name_part);
    mode_step_next();
}

void english_name_entry_given_name_init(void) {
    english_name_part_init(NAME_ENTRY_NAME_PART_GIVEN, name_entry_given_prompt_clip_start, name_entry_given_prompt_clip_end);
}

void english_name_entry_nickname_init(void) {
    english_name_part_init(NAME_ENTRY_NAME_PART_NICKNAME, name_entry_nickname_prompt_clip_start,
                           name_entry_nickname_prompt_clip_end);
}

/* name_entry_family_name_input, name_entry_given_name_input and name_entry_nickname_input */
static void english_name_part_input(s32 name_part) {
    hud_cursor_update();
    name_entry_cell_hit_test();
    english_name_entry_tab_hit_test();
    english_name_entry_tab_highlight_update();
    english_name_entry_slot_markers_update(name_part);
    if (name_entry_tab == NAME_ENTRY_TAB_KANJI) {
        name_entry_reading_hit_test();
        name_entry_reading_select();
    }
    english_name_entry_scroll_arrows_update();
    if (game_state.pad_pressed & PAD_CROSS) {
        english_character_delete(name_part);
        english_name_entry_names_draw();
    } else if (game_state.pad_pressed & PAD_CIRCLE) {
        if (name_entry_cell_column >= 0 && name_entry_cell_row >= 0) {
            english_character_put(name_part);
            english_name_entry_names_draw();
        }
        if (name_entry_tab == NAME_ENTRY_TAB_KANJI || name_entry_tab == NAME_ENTRY_TAB_SYMBOLS) {
            english_page_draw(name_part);
        }
        if (name_entry_tab_hover == NAME_ENTRY_TAB_DONE) {
            name_entry_tab = NAME_ENTRY_TAB_DONE;
            english_name_entry_name_finish(name_part);
            english_name_entry_names_draw();
            audio_command(0x501);
            mode_step_next();
        } else if (name_entry_tab_hover >= 0) {
            name_entry_tab = name_entry_tab_hover;
            english_page_draw(name_part);
        }
    } else if (game_state.pad_pressed & PAD_START) {
        english_name_entry_name_finish(name_part);
        english_name_entry_names_draw();
        audio_command(0x501);
        mode_step_next();
    }
}

void english_name_entry_family_name_input(void) {
    english_name_part_input(NAME_ENTRY_NAME_PART_FAMILY);
}

void english_name_entry_given_name_input(void) {
    english_name_part_input(NAME_ENTRY_NAME_PART_GIVEN);
}

void english_name_entry_nickname_input(void) {
    english_name_part_input(NAME_ENTRY_NAME_PART_NICKNAME);
}

/* As the original, but a scrolled page is followed by the names and the tab label again. */
static s32 english_name_entry_scroll_arrows_update(void) {
    Actor *actor;
    s32 scroll;

    actor = (Actor *)actor_pool;
    if (name_entry_tab != NAME_ENTRY_TAB_SYMBOLS && name_entry_tab != NAME_ENTRY_TAB_KANJI) {
        actor[121].draw_flags = 0;
        actor[122].draw_flags = 0;
        actor[123].draw_flags = 0;
        actor[124].draw_flags = 0;
        return 0;
    }
    actor[121].unk_18 = 0x19;
    actor[122].unk_18 = 0x1B;
    actor[121].draw_flags = 0x80;
    actor[122].draw_flags = 0x80;
    actor[123].draw_flags = 0;
    actor[124].draw_flags = 0;
    scroll = 0;
    if (actor[0].x >= 0x89 && actor[0].x < 0x98) {
        if (actor[0].y >= -0x2F && actor[0].y < -0x20) {
            actor[121].unk_18 = 0x1A;
            if (game_state.pad_held & PAD_CIRCLE) {
                actor[123].draw_flags = 0x80;
            }
            if (game_state.pad_pressed & PAD_CIRCLE) {
                if (name_entry_page_scroll > 0) {
                    name_entry_page_scroll--;
                }
                scroll = 1;
            }
        } else if (actor[0].y >= 0x11 && actor[0].y < 0x20) {
            actor[122].unk_18 = 0x1C;
            if (game_state.pad_held & PAD_CIRCLE) {
                actor[124].draw_flags = 0x80;
            }
            if (game_state.pad_pressed & PAD_CIRCLE) {
                name_entry_page_scroll++;
                scroll = 1;
            }
        }
    }
    if (scroll) {
        /* the original draws the kanji page here for both tabs */
        name_entry_kanji_page_draw(0);
        k_sub_reset_point_set();
        english_name_entry_names_draw();
    }
    return 0;
}

/* Copies the names into GameState as they are (bounded, NUL kept) and the full name with a space. */
s32 english_name_entry_profile_store(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        game_state.player_family_name[i] = 0;
        game_state.player_given_name[i] = 0;
    }
    for (i = 0; i < 0x10; i++) {
        game_state.player_full_name[i] = 0;
        game_state.player_nickname[i] = 0;
    }
    english_copy(game_state.player_family_name, name_entry_family_name, ENGLISH_NAME_LETTERS + 1);
    english_copy(game_state.player_given_name, name_entry_given_name, ENGLISH_NAME_LETTERS + 1);
    english_copy(game_state.player_nickname, name_entry_nickname, ENGLISH_NICKNAME_LETTERS + 1);
    english_copy(game_state.player_full_name, game_state.player_family_name, 0x10);
    english_append(game_state.player_full_name, " ", 0x10);
    english_append(game_state.player_full_name, game_state.player_given_name, 0x10);
    ((WordView *)&game_state.player_birthday)->date.month = name_entry_player_month + 1;
    ((WordView *)&game_state.player_birthday)->date.day = name_entry_player_day + 1;
    ((WordView *)&game_state.player_birthday)->date.blood_type = (name_entry_player_blood + 1) & 3;
    game_state.girls[0].birthday.date.month = name_entry_shiori_month + 1;
    game_state.girls[0].birthday.date.day = name_entry_shiori_day + 1;
    game_state.girls[0].birthday.date.blood_type = (name_entry_shiori_blood + 1) & 3;
    return 0;
}

#endif
