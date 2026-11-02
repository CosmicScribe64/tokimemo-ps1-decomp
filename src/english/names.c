/* T-39506: reversible player-name storage without changing save layouts.
 * The token range was unused by the English font. Full Shift-JIS characters
 * stay atomic, including a trail byte in the token range. Assignments in the
 * shared table are persistent save-format data and must never be reordered. */
#include "english/names.h"

#ifdef TOKIMEMO_ENGLISH

#define ENGLISH_NAME_PAIR(code, pair) pair,
static u8 english_name_pairs[48][3] = {
#include "english/name_pairs.inc"
};
#undef ENGLISH_NAME_PAIR

u8 *english_name_pair(u32 code) {
    if (code >= 0xB0 && code <= 0xDF) {
        return english_name_pairs[code - 0xB0];
    }
    return 0;
}

static s32 english_name_char_bytes(u8 *text) {
    u32 code;

    code = *text;
    return code >= 0x80 && (code < 0xA0 || code >= 0xE0) && text[1] != 0 ? 2 : 1;
}

s32 english_name_unpack(u8 *dst, u8 *src, s32 size) {
    u8 *piece;
    s32 count;
    s32 take;
    s32 advance;

    if (size <= 0) {
        return -1;
    }
    count = 0;
    dst[0] = 0;
    while (*src != 0) {
        advance = english_name_char_bytes(src);
        piece = advance == 1 ? english_name_pair(*src) : 0;
        take = piece != 0 ? 2 : advance;
        if (piece == 0) {
            piece = src;
        }
        if (count + take >= size) {
            return -1;
        }
        dst[count++] = piece[0];
        if (take == 2) {
            dst[count++] = piece[1];
        }
        dst[count] = 0;
        src += advance;
    }
    return count;
}

s32 english_name_pack(u8 *dst, u8 *src, s32 size) {
    s32 count;
    s32 advance;
    s32 take;
    s32 pair;
    u32 code;

    if (size <= 0) {
        return -1;
    }
    count = 0;
    dst[0] = 0;
    while (*src != 0) {
        advance = english_name_char_bytes(src);
        take = advance;
        code = *src;
        if (advance == 1 && src[1] != 0) {
            for (pair = 0; pair < 48; pair++) {
                if (src[0] == english_name_pairs[pair][0] && src[1] == english_name_pairs[pair][1]) {
                    code = 0xB0 + pair;
                    advance = 2;
                    break;
                }
            }
        }
        if (count + take >= size) {
            return -1;
        }
        dst[count++] = code;
        if (take == 2) {
            dst[count++] = src[1];
        }
        dst[count] = 0;
        src += advance;
    }
    return count;
}

#endif
