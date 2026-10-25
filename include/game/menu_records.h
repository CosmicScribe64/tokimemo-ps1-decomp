#ifndef GAME_MENU_RECORDS_H
#define GAME_MENU_RECORDS_H

#include "common.h"

/* 32 records at +0xF90 (indexed with stride 8 and in pairs with stride 16). */
typedef struct MenuItem {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 w;
    /* 0x05 */ u8 h;
    /* 0x06 */ u8 hover;
    /* 0x07 */ u8 disabled;
} MenuItem; /* size 0x08 */

#endif /* GAME_MENU_RECORDS_H */
