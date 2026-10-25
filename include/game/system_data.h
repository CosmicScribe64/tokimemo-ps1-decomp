#ifndef GAME_SYSTEM_DATA_H
#define GAME_SYSTEM_DATA_H

#include "common.h"

#ifndef T9540_FLAGS_WORK24
#define T9540_FLAGS_WORK24
/* T-10880: one entry of the album metrics (main album display, NAME_ENT card routines). */
typedef struct AlbumParam {
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ s16 value;
} AlbumParam; /* size 0x04 */

/* T-19009: the settings bits at system_data +0x40 (TACO): the saved flag, the mode and a selector in byte 0, two
 * more selectors in byte 1. */
typedef struct ShooterSettingsBits {
    u8 flag : 1;
    u8 unk_1 : 1;
    u8 mode : 1;
    u8 sel : 2;
    u8 unk_5 : 3;
    u8 lo : 4;
    u8 hi : 4; /* HYPOTHESIS: settings bits at SystemData +0x40 (saved flag, mode, selectors); read by TACO */
} ShooterSettingsBits; /* size 0x02 */

/* The 0xEE0-byte work area at 0x800E7D10 (main clears it with bzero(&system_data, 0xEE0); +0 is the word main
 * steps by 0x377 and NAME_ENT seeds srand with). One struct for main's FlagsWork24 (T-9540, T-12030), the album
 * view NameMetricWork of main and NAME_ENT (T-10880) and TACO's settings TcSetAll (T-19009), T-20900. TACO's
 * loader still reads +0x40 through its own word/byte union (ShooterSettingsBlock), ETC the 8-byte records from +0x58 through
 * AlbumLogEntry. */
typedef struct SystemData {
    /* 0x000 */ s32 seed;
    /* 0x004 */ u8 slot_status[16];
    /* 0x014 */ u8 flags[16];
    /* 0x024 */ u8 karaoke_unlock_bits[2];
    /* 0x026 */ u8 selected_slot;
    /* 0x027 */ u8 album_state[0x19];
    /* 0x040 */ ShooterSettingsBits bits;
    /* 0x042 */ u8 name0[3];
    /* 0x045 */ u8 name1[3];
    /* 0x048 */ u8 name2[3];
    /* 0x04B */ u8 unk_04B;
    /* 0x04C */ s32 w0;
    /* 0x050 */ s32 w1;
    /* 0x054 */ s32 w2;
    /* 0x058 */ u8 album_entries[0x800];
    /* 0x858 */ u8 year[16];
    /* 0x868 */ u8 month[16];
    /* 0x878 */ u8 day[16];
    /* 0x888 */ u8 family_name[16][8];
    /* 0x908 */ u8 given_name[16][8];
    /* 0x988 */ u8 nickname[16][16];
    /* 0xA88 */ AlbumParam metrics[9][16];
    /* 0xCC8 */ u8 slot_girl_marks[16][16]; /* per slot: the 16 bytes of GameState +0x518 (T-25105) */ /* HYPOTHESIS: per save slot 16 bytes copied from girl record 15's event counters by card_slot_summary_store; NAME_ENT shows an icon of frame 0x58 + (v & 7) for each heroine 0-10 whose byte is non-zero (a per-girl state of that save) */
    /* 0xDC8 */ u8 layout_settings[0x100];
    /* 0xEC8 */ u8 club[16];
    /* 0xED8 */ u8 unk_ED8[8];
} SystemData; /* size 0xEE0 */
#endif

/* T-17000: bit-field view of the flag byte D_800E7D4F. `lui rA; lbu rB,%lo(D_800E7D4F)(rA); ori; sb` with
 * rB != rA is a bit-field store (T-7010); game_mode_tt_game_mode_handler sets b0 so. BUNKASAI and BUNKAKEN read the same
 * view (their BunkaReturnFlags, BunkasaiModeFlags and BunkakenModeFlags, folded in by T-20900). */
typedef struct ExtraModeFlags {
    u8 b0 : 1;
    u8 b1 : 1;
    u8 stage_select : 3;
    u8 unk_5_7 : 3; /* HYPOTHESIS: byte D_800E7D4F (TT unlock bit, festival return mode bits) in SystemData */
} ExtraModeFlags; /* size 0x01 */

#endif /* GAME_SYSTEM_DATA_H */
