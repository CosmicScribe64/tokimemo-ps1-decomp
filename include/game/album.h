#ifndef GAME_ALBUM_H
#define GAME_ALBUM_H

#include "common.h"
#include "game/system_data.h"

#ifndef TOKIMEMO_ALBUM_LAYOUT_H
#define TOKIMEMO_ALBUM_LAYOUT_H
/* T-10880: NAME_ENT's album record; the album work area itself is SystemData (main_api.h, T-20900). */
typedef struct AlbumSlotRecord {
    /* 0x000 */ u8 unk_000[0x205];
    /* 0x205 */ u8 year;
    /* 0x206 */ u8 month;
    /* 0x207 */ u8 day;
    /* 0x208 */ u8 unk_208[0x94];
    /* 0x29C */ u8 family_name[8];
    /* 0x2A4 */ u8 given_name[8];
    /* 0x2AC */ u8 nickname[16];
    /* 0x2BC */ u16 club;
    /* 0x2BE */ u8 unk_2BE[6];
    /* 0x2C4 */ AlbumParam metrics[9];
    /* 0x2E8 */ u8 unk_2E8[0x3A0];
    /* 0x688 */ u8 flags[14];
    /* 0x696 */ u8 unk_696[0x86A];
} AlbumSlotRecord; /* size 0xF00 */
#endif

#endif /* GAME_ALBUM_H */
