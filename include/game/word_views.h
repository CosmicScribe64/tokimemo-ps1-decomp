#ifndef GAME_WORD_VIEWS_H
#define GAME_WORD_VIEWS_H

#include "common.h"

/* T-12010: cd_result is saved as a word and read as four bytes. */
typedef union WordBytes {
    u32 word;
    u8 bytes[4];
} WordBytes;

/* One word read as a whole and through its halves and bytes: the original tests bit-fields of these
 * words with lw and shifts, and IDO-style narrowed loads (lbu, then a shift) of single bytes. */
/* T-14200: month/day packed in the birthday word and GirlRecord +0x10. */
typedef struct BirthdayBits {
    /* 0x00 */ u32 month : 4;
    u32 day : 5;
    u32 blood_type : 2; /* T-17021: set to 1 at init (boot_player_init); OptionDateBits.f9 */
    u32 rest : 21;
} BirthdayBits; /* size 0x04 */

typedef union WordView {
    /* 0x00 */ BirthdayBits date;
    /* 0x00 */ s32 w;
    /* 0x00 */ u32 u;
    /* 0x00 */ u16 h[2];
    /* 0x00 */ u8 b[4];
} WordView; /* size 0x04 */

/* A halfword read as a whole (lhu) and through its bytes (lbu). */
typedef union HalfView {
    /* 0x00 */ u16 h;
    /* 0x00 */ u8 b[2];
    /* 0x00 */ volatile u16 vh; /* FAKE: T-11910 volatile view: GEKO func_80133C50 reads the club here so uopt keeps the read in place */
} HalfView; /* size 0x02 */

#ifndef GAME_GS_BYTE_NIBBLES_T9910
#define GAME_GS_BYTE_NIBBLES_T9910
/* T-9910: byte view for the packed nibbles at GameState offset 0x0F5. */
typedef struct NibbleView {
    /* 0x00, bits 0-3 */ u8 low : 4;
    /* 0x00, bits 4-7 */ u8 high : 4;
} NibbleView; /* size 0x01 */
#endif

#endif /* GAME_WORD_VIEWS_H */
