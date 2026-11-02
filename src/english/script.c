/* script.c: English dialogue parsers (T-39506). Not part of the original executable: only the English
 * build compiles it, into the code appended to the main executable (see src/english/text.c). Each
 * function replaces the original of the same name without the english_ prefix through
 * config/english_hooks.txt:
 *   talk_render, talk_speak_part, make_three_select, talk_names_set (src/main/scene_library.c),
 *   gnsx, sndisp, sndi, get_g_name, get_p_name (src/main/menu_speaker_girl.c).
 * They keep the originals' control flow, text entries and voice handling, and change what the English
 * text needs (wiki/english-text-runtime.md):
 *   - the script control bytes of include/english/script.h instead of 0x20, `q`, `n` and the name
 *     letters, scanned one byte at a time (the originals step two bytes and back up one on a code);
 *   - x positions from pixel widths (english_text_width) instead of byte counts: speaker labels are
 *     right-aligned against the quote mark, quote marks against the text column;
 *   - inline speaker labels of any length, ended by ENGLISH_LABEL_END;
 *   - line buffers of ENGLISH_LINE_BYTES (the originals have 64), bounded copies everywhere;
 *   - English honorifics from the translation records (c:main:80051438, SLPM_86.053:00072e34/38).
 * Names longer than the original fixed buffers (girl family names, name + honorific) are kept in full
 * in buffers of this file; the original buffer gets the bounded copy, see english_long_name. */
#include "common.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "libc.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/resident_views.h"
#include "main/import_views/scenes.h"
#include "main/name_code.h"
#include "main/speaker.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "english/script.h"
#include "english/text.h"

#ifdef TOKIMEMO_ENGLISH

#define ENGLISH_LINE_BYTES 160   /* one display line after name expansion */
#define ENGLISH_SCRIPT_BYTES 1024 /* T-39517: complete long English epilogues exceed 512 bytes */
#define ENGLISH_OPTION_BYTES 128 /* one choice option */
#define ENGLISH_LONG_NAME_BYTES 24
#define ENGLISH_GIRL_SHORT_BYTES 8 /* name_code_girl_short */
#define ENGLISH_GIRL_NAME_BYTES 12 /* girl_name_buf, up to scene_place_title */
#define ENGLISH_VALUE_O_BYTES 8    /* name_code_o, up to name_code_w */
#define ENGLISH_VALUE_W_BYTES 12   /* name_code_w, up to name_code_a */
#define ENGLISH_PLAYER_FORM_BYTES 12 /* name_code_player_form */
#define ENGLISH_FULL_NAME_BYTES 16 /* get_p_name form 15: family, space, given */
#define ENGLISH_STRING_POOL_BYTES 2048

/* x of the dialogue text column, and the right edge of the speaker label column */
#define ENGLISH_TEXT_X (-0x48)      /* text column when no speaker label is drawn (narration rows) */
#define ENGLISH_LABEL_LEFT (-0x80)  /* speaker labels start here, where the Japanese three-glyph names start */
#define ENGLISH_LABEL_GAP 6         /* between the speaker label and the opening mark */
#define ENGLISH_SCREEN_LEFT (-0x9E)

/* Translations of the original's literals: c:main:80051438 (get_g_name), c:main:8007ef48
 * (talk_speak_part), c:main:80081190 (talk_render), SLPM_86.053:00072e34/38 (the suffixes). */
static u8 english_quote[] = "\"";
static u8 english_open_paren[] = "(";
static u8 english_close_paren[] = ")";
static u8 english_suffix_familiar[] = "-kun";
static u8 english_suffix_polite[] = "-san";
static u8 english_suffix_girl[] = "-san";
static u8 english_suffix_yumi[] = "-chan";
static u8 english_name_shiori[] = "Shiori";
static u8 english_name_fujisaki[] = "Fujisaki";
static u8 english_name_yoshio[] = "Yoshio";

/* Room for main-executable strings that the English build moves out of slots that are too small
 * (tools/patch/text/retarget.py writes them here and re-points their references). Initialised, so
 * that it is part of the loaded file; the first byte is overwritten by the build or never read. */
u8 english_string_pool[ENGLISH_STRING_POOL_BYTES] = { 0xFF };
s32 english_string_pool_bytes = ENGLISH_STRING_POOL_BYTES;

/* Full forms of the girl names whose original buffers are too small (see english_long_name). */
static u8 english_girl_short_long[ENGLISH_LONG_NAME_BYTES];
static u8 english_girl_name_long[ENGLISH_LONG_NAME_BYTES];
static u8 english_value_o_long[ENGLISH_LONG_NAME_BYTES]; /* O value (New Year fortune): name_code_o has 8 bytes */
static u8 english_value_w_long[ENGLISH_LONG_NAME_BYTES]; /* W value (New Year wish): name_code_w has 12 bytes */

/* Bytes of the character at text: 2 for a Shift-JIS lead byte, else 1. */
static s32 english_char_bytes(u8 *text) {
    u32 c;

    c = *text;
    if (c >= 0x80 && (c < 0xA0 || c >= 0xE0) && text[1] != 0) {
        return 2;
    }
    return 1;
}

/* Bounded strcpy: dst gets at most size - 1 bytes and a NUL, never half a Shift-JIS character. */
void english_copy(u8 *dst, u8 *src, s32 size) {
    if (size <= 0) {
        return;
    }
    dst[0] = 0;
    english_append(dst, src, size);
}

/* Bounded strcat with the same rules. */
void english_append(u8 *dst, u8 *src, s32 size) {
    s32 n;
    s32 k;

    n = strlen(dst);
    while (*src != 0) {
        k = english_char_bytes(src);
        if (n + k > size - 1) {
            break;
        }
        dst[n++] = *src++;
        if (k == 2) {
            dst[n++] = *src++;
        }
    }
    dst[n] = 0;
}

/* The full form of a name whose original buffer holds only its first bytes: long_copy when buffer
 * is exactly the bounded copy of it, else buffer (an overlay wrote something else there). */
static u8 *english_long_name(u8 *buffer, u8 *long_copy, s32 size) {
    u8 bounded[ENGLISH_LONG_NAME_BYTES];

    english_copy(bounded, long_copy, size);
    if (long_copy[0] != 0 && strcmp(bounded, buffer) == 0) {
        return long_copy;
    }
    return buffer;
}

void english_value_set(s32 code, u8 *text) {
    if (code == ENGLISH_CODE_O) {
        english_copy(english_value_o_long, text, ENGLISH_LONG_NAME_BYTES);
        english_copy(name_code_o, english_value_o_long, ENGLISH_VALUE_O_BYTES);
    } else if (code == ENGLISH_CODE_W) {
        english_copy(english_value_w_long, text, ENGLISH_LONG_NAME_BYTES);
        english_copy(name_code_w, english_value_w_long, ENGLISH_VALUE_W_BYTES);
    }
}

void english_girl_short_set(u8 *name) {
    english_copy(english_girl_short_long, name, ENGLISH_LONG_NAME_BYTES);
    english_copy(name_code_girl_short, english_girl_short_long, ENGLISH_GIRL_SHORT_BYTES);
}

void english_girl_name_set(u8 *name) {
    english_copy(english_girl_name_long, name, ENGLISH_LONG_NAME_BYTES);
    english_copy(girl_name_buf, english_girl_name_long, ENGLISH_GIRL_NAME_BYTES);
}

static u8 *english_girl_short(void) {
    return english_long_name(name_code_girl_short, english_girl_short_long, ENGLISH_GIRL_SHORT_BYTES);
}

static u8 *english_girl_name(void) {
    return english_long_name(girl_name_buf, english_girl_name_long, ENGLISH_GIRL_NAME_BYTES);
}

/* Copies src to dst (ENGLISH_LINE_BYTES) with the name codes replaced by the names. */
static void english_names_expand(u8 *dst, u8 *src) {
    u8 one[3];
    u8 name[ENGLISH_LONG_NAME_BYTES];
    u8 *text;
    s32 k;

    dst[0] = 0;
    while (*src != 0) {
        text = 0;
        switch (*src) {
        case ENGLISH_CODE_PLAYER_FORM:
            text = name_code_player_form;
            break;
        case ENGLISH_CODE_FAMILY:
            text = name_code_family;
            break;
        case ENGLISH_CODE_GIVEN:
            text = name_code_given;
            break;
        case ENGLISH_CODE_FULL:
            english_copy(name, name_code_family, sizeof(name));
            english_append(name, " ", sizeof(name));
            english_append(name, name_code_given, sizeof(name));
            text = name;
            break;
        case ENGLISH_CODE_YOSHIO:
            text = english_name_yoshio;
            break;
        case ENGLISH_CODE_GIRL:
            text = english_girl_name();
            break;
        case ENGLISH_CODE_S:
            text = name_code_s;
            break;
        case ENGLISH_CODE_O:
            text = english_long_name(name_code_o, english_value_o_long, ENGLISH_VALUE_O_BYTES);
            break;
        case ENGLISH_CODE_W:
            text = english_long_name(name_code_w, english_value_w_long, ENGLISH_VALUE_W_BYTES);
            break;
        case ENGLISH_CODE_A:
            text = name_code_a;
            break;
        case ENGLISH_CODE_M:
            text = name_code_m;
            break;
        }
        if (text != 0) {
            src++;
        } else {
            k = english_char_bytes(src);
            one[0] = *src++;
            one[1] = 0;
            if (k == 2) {
                one[1] = *src++;
                one[2] = 0;
            }
            text = one;
        }
        english_append(dst, text, ENGLISH_LINE_BYTES);
    }
}

/* Strings still in the original script format (Japanese the writers left as they were) are converted
 * to the English control bytes, so they show as the original did: a string with no byte in
 * 0x01-0x1F and at least one Shift-JIS character is original. The conversion steps two bytes as
 * talk_render does: 0x20 ends a line, `q` an option, `n` is the empty option, the name letters become
 * their codes, and an inline label marker with its fixed label length becomes 0x1C/0x1D ... 0x1F. */
static s32 english_script_is_original(u8 *text) {
    s32 sjis;

    for (sjis = 0; *text != 0; text++) {
        if (*text < 0x20) {
            return 0;
        }
        if (english_char_bytes(text) == 2) {
            sjis = 1;
            text++;
        }
    }
    return sjis;
}

static void english_script_from_original(u8 *dst, u8 *src, s32 size) {
    static u8 codes[] = "PpmFYGSOWAM";
    u8 one[3];
    s32 label;
    s32 i;
    s32 k;

    dst[0] = 0;
    if (!english_script_is_original(src)) {
        english_copy(dst, src, size);
        return;
    }
    label = 0;
    one[1] = 0;
    switch (*src) {
    case '>':
    case '<':
        label = 4;
        break;
    case '-':
        label = 6;
        break;
    case '+':
        label = 2;
        break;
    }
    if (label != 0) {
        one[0] = *src == '<' ? ENGLISH_LABEL_THOUGHT : ENGLISH_LABEL_SPEECH;
        english_append(dst, one, size);
        for (i = 1; i <= label && src[i] != 0; i++) {
            one[0] = src[i];
            english_append(dst, one, size);
        }
        one[0] = ENGLISH_LABEL_END;
        english_append(dst, one, size);
        src += i;
    }
    while (*src != 0) {
        k = english_char_bytes(src);
        one[0] = *src;
        one[1] = 0;
        if (k == 2) {
            one[1] = src[1];
            one[2] = 0;
        } else if (*src == ' ') {
            one[0] = ENGLISH_LINE_END;
        } else if (*src == 'q') {
            one[0] = ENGLISH_OPTION_END;
        } else if (*src == 'n') {
            one[0] = ENGLISH_OPTION_EMPTY;
        } else {
            for (i = 0; codes[i] != 0; i++) {
                if (*src == codes[i]) {
                    one[0] = ENGLISH_CODE_PLAYER_FORM + i;
                }
            }
        }
        english_append(dst, one, size);
        src += k;
    }
}

/* The all-punctuation flag D_80122D1C (no reader found): 1 when every glyph of the text is a space,
 * period, comma, quote or ellipsis. */
static s32 english_punctuation_only(u8 *text) {
    u32 c;

    for (; (c = *text) != 0; text++) {
        if (c < 0x20 || c == ' ' || c == '.' || c == ',' || c == '"' || c == 0xA0 || c == 0xA1 || c == 0xA5) {
            continue;
        }
        return 0;
    }
    return 1;
}

/* x that puts text right before the column at right, at least at the screen's left edge. */
static s32 english_right_aligned(u8 *text, s32 right) {
    s32 x;

    x = right - english_text_width(text);
    if (x < ENGLISH_SCREEN_LEFT) {
        x = ENGLISH_SCREEN_LEFT;
    }
    return x;
}

s32 english_speaker_label_x(u8 *name) {
    return ENGLISH_LABEL_LEFT;
}

/* Text column of the last speaker label: continuation records (commands 0xD, 0x12, 0x11) hang under it. */
static s32 english_text_x = ENGLISH_TEXT_X;
/* x of the opening mark after the last gnsx label, for sndisp/sndi; 0: no label drawn */
static s32 english_phone_mark_x = 0;

/* Speaker label at the fixed left margin, the opening mark after it; returns the text column, where every
 * row of the speech starts (T-39506, owner decision: names start at one margin, rows hang under the text). */
static s32 english_speaker_draw(u8 *name, u8 *mark, s32 y, s32 text_color) {
    s32 mark_x;

    set_kanji_string(ENGLISH_LABEL_LEFT, y, text_color, name, 0);
    mark_x = ENGLISH_LABEL_LEFT + english_text_width(name) + ENGLISH_LABEL_GAP;
    set_kanji_string(mark_x, y, text_color, mark, 0);
    english_text_x = mark_x + english_text_width(mark);
    return english_text_x;
}

/* T-39517: wrap expanded display text, not the shorter name/value codes.
 * Return a word boundary, or an atomic character boundary for a long word. */
static s32 english_display_line_end(u8 *text, s32 limit) {
    u8 one[3];
    s32 i;
    s32 bytes;
    s32 width;
    s32 last_space;

    i = 0;
    width = 0;
    last_space = -1;
    while (text[i] != 0) {
        if (text[i] == ' ') {
            last_space = i;
        }
        bytes = english_char_bytes(&text[i]);
        one[0] = text[i];
        one[1] = bytes == 2 ? text[i + 1] : 0;
        one[2] = 0;
        width += english_text_width(one);
        if (width > limit) {
            if (last_space > 0) {
                return last_space;
            }
            return i > 0 ? i : bytes;
        }
        i += bytes;
    }
    return i;
}

s32 english_talk_render(s32 x, s32 y, s32 text_color, u8 *input_text, s32 ordering_slot, u8 *line_suffix,
                        u8 *speaker_label, s32 label_enabled) {
    u8 text[ENGLISH_SCRIPT_BYTES];
    u8 line[ENGLISH_LINE_BYTES];
    s32 start;
    s32 scan;
    s32 row;
    s32 line_x;
    s32 result;
    s32 line_start;
    s32 line_end;
    u8 saved;
    u32 c;

    x = (s16)x;
    y = (s16)y;
    text_color &= 0xFF;
    label_enabled &= 0xFF;
    if (*input_text == 0) {
        return 0;
    }
    english_script_from_original(text, input_text, sizeof(text));
    D_80122D1C = english_punctuation_only(text);
    row = 0;
    start = 0;
    for (scan = 0;; scan++) {
        c = text[scan];
        if (c != ENGLISH_LINE_END && c != ENGLISH_OPTION_END && c != 0) {
            continue;
        }
        text[scan] = 0;
        if (label_enabled != 0) {
            set_kanji_string(english_right_aligned(speaker_label, x - 1), y + row, text_color, speaker_label,
                             ordering_slot);
        }
        english_names_expand(line, &text[start]);
        /* as the original: every option line of a choice gets the suffix, other text only the last line */
        if (c != 0 ? label_enabled != 0 : *line_suffix != 0) {
            english_append(line, line_suffix, sizeof(line));
        }
        line_start = 0;
        do {
            /* Choice rows must remain one selectable row. Other text may gain
             * rows after expanding full names and career labels. T-39517. */
            line_end = line_start + english_display_line_end(&line[line_start],
                label_enabled != 0 ? 0x7FFFFFFF : 0x7C - x);
            saved = line[line_end];
            line[line_end] = 0;
            line_x = x;
            if (line[line_start] == '(') {
                /* hanging parenthesis, as the original does for a full-width one */
                line_x = english_right_aligned(english_open_paren, x);
            }
            result = set_kanji_string(line_x, y + row, text_color, &line[line_start], ordering_slot);
            line[line_end] = saved;
            line_start = line_end + (saved == ' ');
            if (saved != 0) {
                row += 0x10;
            }
        } while (saved != 0);
        if (c == 0) {
            talk_next_row_y = y + row + 0x10;
        }
        if (c == 0) {
            return result;
        }
        row += 0x10;
        start = scan + 1;
    }
}

/* Voice clip wait shared by the talk_speak_part cases: 0 when the line can be shown, 1 to return 0
 * (still waiting), 2 to return 0x64 (timed out). */
static s32 english_voice_wait(TalkWork *dialogue, s32 clip_start, s32 clip_end, s32 speaker) {
    if ((talk_voice_wait_frames++ == 0) && (dialogue->line_wait == 0)) {
        cd_audio_request_set(clip_start, clip_end, speaker);
        audio_command(0x300);
    }
    if (talk_voice_wait_frames < 0x400) {
        if (!(cd_audio_state() & 4) && dialogue->line_wait == 0) {
            talk_voice_waiting = 1;
            talk_voice_skip_check();
            if (talk_voice_waiting == 1) {
                return 1;
            }
        }
    } else {
        talk_voice_wait_frames = 0;
        audio_queue_clear();
        audio_command(0x74);
        return 2;
    }
    dialogue->voice_state = 1;
    talk_voice_wait_frames = 0;
    return 0;
}

/* Closing quote of a speech record (T-39506, owner decision: matched quotes): none when the next record
 * of the group continues the speech on the next row (commands 0xD, 0x12, 0x11) or when the text already
 * ends with a closing quote. The wrapper measured every page with it (tools/patch/text/speech.py). */
static u8 *english_speech_close(u8 *text, s32 group_index, s32 line_index) {
    TalkTables *tables;
    u32 next;
    u32 last;
    s32 n;

    tables = (TalkTables *)&talk_tables;
    next = tables->command[group_index][line_index + 1];
    if (next == 0xD || next == 0x12 || next == 0x11) {
        return "";
    }
    n = strlen(text);
    while (n > 0 && (text[n - 1] == ENGLISH_OPTION_EMPTY || text[n - 1] == ' ')) {
        n--;
    }
    if (n > 0) {
        last = text[n - 1];
        if (last == '"' || last == 0xA1 || last == 0xA3) {
            return "";
        }
    }
    return english_quote;
}

/* True when the line ends in the branch-zero tail (original `nn`); removes it when strip is set. */
static s32 english_branch_zero_tail(u8 *line, s32 strip) {
    s32 n;

    n = strlen(line);
    if (n >= 2 && line[n - 1] == ENGLISH_OPTION_EMPTY && line[n - 2] == ENGLISH_OPTION_EMPTY) {
        if (strip) {
            line[n - 2] = 0;
        }
        return 1;
    }
    return 0;
}

s32 english_talk_speak_part(u8 *line_text, s32 speak_code, s32 choice, void *voice_table, s32 group_index,
                            s32 line_index, s32 text_color) {
    TalkWork *dialogue;
    TalkVoiceRef *voice;
    TalkSelectors voice_speaker_ids;
    u8 line[ENGLISH_SCRIPT_BYTES];
    u8 label[ENGLISH_LINE_BYTES];
    u8 *p;
    u8 *variant;
    s32 y;
    s32 wait;
    s32 count;
    s32 thought;
    s32 has_voice;

    speak_code &= 0xFF;
    choice &= 0xFF;
    group_index &= 0xFFFF;
    line_index &= 0xFFFF;
    text_color &= 0xFF;
    dialogue = (TalkWork *)&talk_work;
    voice_speaker_ids = talk_voice_speaker_by_girl;
    if (speak_code != 0xFF) {
        english_script_from_original(line, line_text, sizeof(line));
    }
    dialogue->voice_state = 0;
    talk_voice_waiting = 0;
    y = dialogue->row_offset * 0x10 + 0x30;
    /* the voice entry is read only by the cases that speak, as in the original */
    voice = 0;
    has_voice = 0;
    if (speak_code == 0x7 || speak_code == 0x1 || speak_code == 0xF || speak_code == 0xC || speak_code == 0xD
        || speak_code == 0x12 || speak_code == 0xB || speak_code == 0xA || speak_code == 0x2 || speak_code == 0x5) {
        voice = ((TalkVoiceRef ***)voice_table)[group_index][line_index];
        has_voice = (voice->clip_start != 0 || voice->clip_end != 0) && *(u8 *)&game_state.voice_mode != 0;
    }
    wait = 0;
    switch (speak_code) {
    case 0x7:
        if (has_voice) {
            wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end,
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        if (line[0] == ENGLISH_LABEL_SPEECH || line[0] == ENGLISH_LABEL_THOUGHT) {
            thought = line[0] == ENGLISH_LABEL_THOUGHT;
            for (p = &line[1]; *p != 0 && *p != ENGLISH_LABEL_END; p++) {
            }
            if (*p != 0) {
                *p++ = 0;
            }
            english_copy(label, &line[1], sizeof(label));
            english_speaker_draw(label, thought ? english_open_paren : english_quote, y, text_color);
            english_talk_render(english_text_x, y, text_color, p, 0,
                                thought ? english_close_paren : english_speech_close(p, group_index, line_index),
                                "", 0);
        } else if (strcmp(line, "And then...") == 0) {
            english_talk_render(-40, y + 0x10, text_color, line, 0, "", "", 0);
        } else if (strcmp(line, "Click.") == 0 || strcmp(line, "Click") == 0 || strcmp(line, "Rattle") == 0
                   || strcmp(line, "Rustle, rustle") == 0) {
            english_talk_render(-0x80, y, 2, line, 0, "", "", 0);
        } else if (strcmp(line, "Stare, stare") == 0 || strcmp(line, "Bump") == 0) {
            english_talk_render(-0x80, y, 1, line, 0, "", "", 0);
        } else if (strcmp(line, "Ding-dong") == 0) {
            english_talk_render(-40, y + 0x10, 2, line, 0, "", "", 0);
        } else {
            english_talk_render(-0x80, y, text_color, line, 0, "", "", 0);
        }
        dialogue->line_wait = 1;
        break;
    case 0x10:
        english_talk_render(-0x80, y, text_color, line, 0, "", "", 0);
        dialogue->line_auto = 1;
        break;
    case 0x11:
        english_talk_render(english_text_x, talk_next_row_y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_auto = 1;
        break;
    case 0x9:
        if (line[0] != 0) {
            if (strcmp(line, "Bump") == 0) {
                english_talk_render(-0x80, y, 1, line, 0, "", "", 0);
            }
            english_talk_render(-0x80, y, text_color, line, 0, "", "", 0);
        }
        break;
    case 0x0:
        if (english_branch_zero_tail(line, 0) && dialogue->choice != 0) {
            break;
        }
        english_branch_zero_tail(line, 1);
        english_speaker_draw(name_code_family, english_quote, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_wait = 1;
        break;
    case 0xE:
        english_speaker_draw(name_code_family, english_open_paren, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0, english_close_paren, "", 0);
        break;
    case 0x1:
        if (has_voice && (!english_branch_zero_tail(line, 0) || dialogue->choice == 0)) {
            wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end,
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        if (english_branch_zero_tail(line, 0) && dialogue->choice != 0) {
            break;
        }
        english_branch_zero_tail(line, 1);
        english_speaker_draw(english_girl_short(), english_quote, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_wait = 1;
        break;
    case 0xF:
        if (has_voice) {
            wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end,
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        english_speaker_draw(english_girl_short(), english_quote, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_clear = 1;
        break;
    case 0xC:
        talk_voice_cancelled = 0;
        if (has_voice) {
            wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end,
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        if (english_branch_zero_tail(line, 0) && dialogue->choice != 0) {
            break;
        }
        english_branch_zero_tail(line, 1);
        english_speaker_draw(english_girl_short(), english_quote, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_auto = 1;
        break;
    case 0xD:
    case 0x12:
        /* continuation lines: the original also queues a blank full-width glyph at the mark position */
        if (has_voice && talk_voice_cancelled == 0) {
            wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end,
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        if (speak_code == 0xD && english_branch_zero_tail(line, 0) && dialogue->choice != 0) {
            break;
        }
        if (speak_code == 0xD) {
            english_branch_zero_tail(line, 1);
        }
        english_talk_render(english_text_x, talk_next_row_y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        if (speak_code == 0xD) {
            dialogue->line_wait = 1;
        } else {
            dialogue->line_auto = 1;
        }
        break;
    case 0xB:
    case 0xA:
        if (voice->clip_start != 0 || voice->clip_end != 0) {
            if (*(u8 *)&game_state.voice_mode != 0) {
                wait = english_voice_wait(dialogue, voice->clip_start, voice->clip_end, speak_code == 0xB ? 0 : 0xD);
                if (wait != 0) {
                    break;
                }
            }
        }
        if (english_branch_zero_tail(line, 0) && dialogue->choice != 0) {
            break;
        }
        english_branch_zero_tail(line, 1);
        english_speaker_draw(speak_code == 0xB ? english_name_fujisaki : english_name_yoshio, english_quote, y,
                             text_color);
        english_talk_render(english_text_x, y, text_color, line, 0,
                            english_speech_close(line, group_index, line_index), "", 0);
        dialogue->line_wait = 1;
        break;
    case 0x4:
    case 0x6:
        english_speaker_draw(name_code_family, english_open_paren, y, text_color);
        english_talk_render(english_text_x, y, text_color, line, 0, english_close_paren, "", 0);
        dialogue->line_wait = 1;
        break;
    case 0x3:
        if (choice != 0xFF) {
            junban_init();
            english_make_three_select(line);
        } else {
            talk_choice_order[0] = 0;
            talk_choice_order[1] = 1;
            talk_choice_order[2] = 2;
        }
        k_disp_start(english_talk_render(-0x72, y, text_color, line, 0, english_quote, english_quote, 1));
        break;
    case 0x8:
        talk_choice_order[0] = 0;
        talk_choice_order[1] = 1;
        k_disp_start(english_talk_render(-0x72, y, text_color, line, 0, english_quote, english_quote, 1));
        break;
    case 0x13:
        talk_choice_order[0] = 0;
        talk_choice_order[1] = 1;
        k_disp_start(english_talk_render(-0x72, y, text_color, line, 0, english_close_paren, english_open_paren, 1));
        break;
    case 0x2:
    case 0x5:
        if (((s32 *)voice)[dialogue->choice * 2] != 0 && *(u8 *)&game_state.voice_mode != 0) {
            wait = english_voice_wait(dialogue, ((s32 *)voice)[dialogue->choice * 2],
                                      ((s32 *)voice)[dialogue->choice * 2 + 1],
                                      voice_speaker_ids.value[game_state.current_girl]);
            if (wait != 0) {
                break;
            }
        }
        /* reply variant number `choice`, separated by ENGLISH_OPTION_END */
        variant = line;
        count = 0;
        for (p = line;; p++) {
            if (*p == ENGLISH_OPTION_END || *p == 0) {
                if (count == choice || *p == 0) {
                    *p = 0;
                    break;
                }
                count++;
                variant = p + 1;
            }
        }
        if (*variant != ENGLISH_OPTION_EMPTY) {
            english_speaker_draw(english_girl_short(), english_quote, y, text_color);
            english_talk_render(english_text_x, y, text_color, variant, 0,
                                english_speech_close(variant, group_index, line_index), "", 0);
            dialogue->line_wait = 1;
        }
        break;
    case 0xFF:
        mode_step_next();
        break;
    default:
        game_mode_reset_to_title();
        break;
    }
    if (wait == 1) {
        return 0;
    }
    if (wait == 2) {
        return 0x64;
    }
    return 0;
}

s32 english_make_three_select(u8 *source_text) {
    u8 options[3][ENGLISH_OPTION_BYTES];
    s32 n;
    s32 start;
    s32 i;
    u32 c;

    n = 0;
    start = 0;
    for (i = 0;; i++) {
        c = source_text[i];
        if (c != ENGLISH_OPTION_END && c != ENGLISH_LINE_END && c != 0) {
            continue;
        }
        source_text[i] = 0;
        if (n < 3) {
            english_copy(options[n++], &source_text[start], ENGLISH_OPTION_BYTES);
        }
        if (c == 0) {
            break;
        }
        start = i + 1;
    }
    for (; n < 3; n++) {
        options[n][0] = 0;
    }
    sprintf(source_text, "%s\013%s\013%s", options[talk_choice_order[0]], options[talk_choice_order[1]],
            options[talk_choice_order[2]]);
    return 0;
}

void english_talk_names_set(s32 girl_id) {
    girl_id &= 0xFF;
    english_copy(name_code_family, game_state.player_family_name, ENGLISH_NAME_LETTERS + 1);
    english_copy(name_code_given, game_state.player_given_name, ENGLISH_NAME_LETTERS + 1);
    if (girl_id < 0xE) {
        english_get_p_name(name_code_player_form, girl_id);
        english_copy(english_girl_short_long, girl_short_name_table[girl_id], ENGLISH_LONG_NAME_BYTES);
        english_copy(name_code_girl_short, english_girl_short_long, ENGLISH_GIRL_SHORT_BYTES);
        english_get_g_name(girl_name_buf, girl_id);
    }
}

void english_gnsx(u8 *speaker_name) {
    set_kanji_string(ENGLISH_LABEL_LEFT, 0x32, 0, speaker_name, 0);
    english_phone_mark_x = ENGLISH_LABEL_LEFT + english_text_width(speaker_name) + ENGLISH_LABEL_GAP;
}

/* Dynamic telephone sentences concatenate dates and place names after the build-time
 * layout pass. Wrap those completed rows too, keeping explicit paragraph breaks. T-39511. */
static void english_phone_wrap(u8 *text, s32 speaker_style) {
    u8 one[3];
    s32 i;
    s32 last_space;
    s32 width;
    s32 limit;
    s32 count;
    s32 k;

    limit = 0x7C - ENGLISH_TEXT_X - english_text_width(english_quote);
    if (english_phone_mark_x != 0) {
        limit = 0x7C - english_phone_mark_x - 2 * english_text_width(english_quote);
    }
    if (speaker_style == 0xFF) {
        limit = 0x7C + 0x80;
    }
    last_space = -1;
    width = 0;
    count = 1;
    for (i = 0; text[i] != 0; i += english_char_bytes(&text[i])) {
        if (text[i] == ENGLISH_LINE_END) {
            count++;
        }
    }
    for (i = 0; text[i] != 0;) {
        if (text[i] == ENGLISH_LINE_END) {
            width = 0;
            last_space = -1;
            i++;
            continue;
        }
        if (text[i] == ' ') {
            last_space = i;
        }
        k = english_char_bytes(&text[i]);
        one[0] = text[i];
        one[1] = k == 2 ? text[i + 1] : 0;
        one[2] = 0;
        /* Name codes expand later in sndi; allow the longest player full name. */
        if (text[i] == ENGLISH_CODE_PLAYER_FORM || text[i] == ENGLISH_CODE_GIRL) {
            width += 116;
        } else {
            width += english_text_width(one);
        }
        if (width > limit && last_space >= 0 && count < 6) {
            text[last_space] = ENGLISH_LINE_END;
            i = last_space + 1;
            width = 0;
            last_space = -1;
            count++;
        } else {
            i += k;
        }
    }
}

void english_sndisp(u8 *text, s32 first_row, s32 speaker_style) {
    u8 block_text[ENGLISH_SCRIPT_BYTES];
    u8 closed_line[ENGLISH_LINE_BYTES];
    s32 line_starts[6];
    s32 line_count;
    s32 i;
    s32 close_row;
    s32 length;
    u8 *closing;

    english_script_from_original(block_text, text, sizeof(block_text));
    english_phone_wrap(block_text, speaker_style);
    line_starts[0] = 0;
    line_count = 1;
    for (i = 0; block_text[i] != 0; i++) {
        if (block_text[i] == ENGLISH_LINE_END && line_count < 6) {
            block_text[i] = 0;
            line_starts[line_count++] = i + 1;
        }
    }
    game_state.text_first_line = check_set_k();
    close_row = line_count - 1;
    closing = english_quote;
    if (speaker_style == 0x1F) {
        closing = english_close_paren;
    } else if (speaker_style == 0x2F || speaker_style == 0xFF || first_row != 0) {
        closing = "";
    } else {
        /* Close the spoken part before a separate parenthetical thought. T-39511. */
        for (i = 1; i < line_count; i++) {
            if (block_text[line_starts[i]] == '(') {
                close_row = i - 1;
                break;
            }
        }
    }
    for (i = 0; i < line_count; i++) {
        english_copy(closed_line, &block_text[line_starts[i]], sizeof(closed_line));
        length = strlen(closed_line);
        if (i == close_row && length > 0 && *closing != 0
            && closed_line[length - 1] != *closing
            && closed_line[length - 1] != 0xA1 && closed_line[length - 1] != 0xA3) {
            english_append(closed_line, closing, sizeof(closed_line));
        }
        english_sndi(closed_line, first_row + i, speaker_style);
    }
    game_state.text_lines_total = check_set_k();
    game_state.text_scroll_state = 0x80;
    english_phone_mark_x = 0;
}

void english_sndi(u8 *text, s32 row, s32 speaker_style) {
    u8 original[ENGLISH_LINE_BYTES];
    u8 expanded[ENGLISH_LINE_BYTES];
    u8 decorated[ENGLISH_LINE_BYTES];
    u8 name[ENGLISH_LONG_NAME_BYTES];
    u8 one[3];
    u8 *mark;
    u8 *piece;
    s32 k;
    s32 mark_x;
    s32 text_x;

    game_state.text_scroll_state = 0;
    english_script_from_original(original, text, sizeof(original));
    text = original;
    expanded[0] = 0;
    while (*text != 0) {
        if (*text == ENGLISH_CODE_PLAYER_FORM) {
            english_get_p_name(name, (speaker_style & 15) == 15 ? 15 : game_state.current_girl);
            piece = name;
            text++;
        } else if (*text == ENGLISH_CODE_GIRL) {
            english_get_g_name(name, game_state.current_girl);
            piece = english_girl_name_long;
            text++;
        } else {
            k = english_char_bytes(text);
            one[0] = *text++;
            one[1] = 0;
            if (k == 2) {
                one[1] = *text++;
                one[2] = 0;
            }
            piece = one;
        }
        english_append(expanded, piece, sizeof(expanded));
    }
    if (speaker_style == 0x1F) {
        mark = english_open_paren;
    } else if (speaker_style == 0x2F) {
        mark = "";
    } else {
        mark = english_quote;
    }
    /* after a gnsx label the mark follows the name and the rows hang under the text after the mark;
     * without one the mark hangs left of the usual text column */
    if (english_phone_mark_x != 0) {
        mark_x = english_phone_mark_x;
    } else {
        mark_x = ENGLISH_TEXT_X - english_text_width(mark);
    }
    text_x = mark_x + english_text_width(mark);
    if (speaker_style == 0xFF) {
        mark_x = text_x = -0x80;
    }
    switch (row) {
    case 0:
        /* one entry, as the original: the mark and the first row */
        english_copy(decorated, mark, sizeof(decorated));
        english_append(decorated, expanded, sizeof(decorated));
        set_kanji_string(mark_x, 0x32, 0, decorated, 0);
        return;
    case 1:
        set_kanji_string(text_x, 0x42, 0, expanded, 0);
        return;
    default:
        set_kanji_string(text_x, 0x52, 0, expanded, 0);
        return;
    }
}

s32 english_get_g_name(u8 *name_buf, s32 girl_id) {
    if (girl_id == 0) {
        english_copy(english_girl_name_long, english_name_shiori, ENGLISH_LONG_NAME_BYTES);
    } else if (girl_id == 0xA) {
        english_copy(english_girl_name_long, girl_short_name_yumi_given, ENGLISH_LONG_NAME_BYTES);
        english_append(english_girl_name_long, english_suffix_yumi, ENGLISH_LONG_NAME_BYTES);
    } else if (girl_id == 0xC) {
        english_copy(english_girl_name_long, girl_short_name_rei, ENGLISH_LONG_NAME_BYTES);
    } else {
        english_copy(english_girl_name_long, girl_short_name_table[girl_id & 0xF], ENGLISH_LONG_NAME_BYTES);
        english_append(english_girl_name_long, english_suffix_girl, ENGLISH_LONG_NAME_BYTES);
    }
    /* callers pass girl_name_buf or larger buffers; the full form stays in english_girl_name_long */
    english_copy(name_buf, english_girl_name_long, ENGLISH_GIRL_NAME_BYTES);
    if (girl_id >= 0x10 || girl_id < 0) {
        return -1;
    }
    return 1;
}

/* Name plus honorific for get_p_name. */
static void english_player_name(u8 *name_buf, u8 *name, u8 *suffix) {
    english_copy(name_buf, name, ENGLISH_PLAYER_FORM_BYTES);
    if (suffix != 0) {
        english_append(name_buf, suffix, ENGLISH_PLAYER_FORM_BYTES);
    }
}

void english_get_p_name(u8 *name_buf, s32 girl_form) {
    u8 *family;
    u8 *given;
    u8 *nickname;
    s32 close;
    s32 rank;

    family = game_state.player_family_name;
    given = game_state.player_given_name;
    nickname = game_state.player_nickname;
    close = 0;
    rank = 0;
    if ((u32)girl_form <= 10) {
        rank = get_g_zyotai_s(girl_form);
        close = rank & 0x80;
        rank &= 0x7F;
    }
    switch (girl_form) {
    case 0:
        if (close) {
            english_player_name(name_buf, nickname, 0);
        } else {
            english_player_name(name_buf, given, english_suffix_familiar);
        }
        return;
    case 1:
    case 9:
        english_player_name(name_buf, family, close ? english_suffix_familiar : english_suffix_polite);
        return;
    case 2:
    case 7:
        english_player_name(name_buf, close ? given : family, english_suffix_familiar);
        return;
    case 3:
    case 6:
        if (close) {
            english_player_name(name_buf, nickname, 0);
        } else {
            english_player_name(name_buf, family, english_suffix_familiar);
        }
        return;
    case 4:
        english_player_name(name_buf, close ? given : family, english_suffix_familiar);
        return;
    case 5:
    case 10:
        english_player_name(name_buf, close ? given : family, english_suffix_polite);
        return;
    case 8:
        if (close) {
            english_player_name(name_buf, nickname, 0);
        } else {
            english_player_name(name_buf, given, english_suffix_familiar);
        }
        return;
    case 13:
        english_player_name(name_buf, given, 0);
        return;
    case 11:
        english_player_name(name_buf, given, english_suffix_familiar);
        return;
    case 12:
        english_player_name(name_buf, family, english_suffix_familiar);
        return;
    case 15:
        /* sndi's 20-byte buffer: family name, space, given name */
        english_copy(name_buf, family, ENGLISH_FULL_NAME_BYTES);
        english_append(name_buf, " ", ENGLISH_FULL_NAME_BYTES);
        english_append(name_buf, given, ENGLISH_FULL_NAME_BYTES);
        return;
    default:
        english_player_name(name_buf, family, english_suffix_familiar);
        return;
    }
}

#endif
