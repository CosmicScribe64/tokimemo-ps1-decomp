#ifndef GAME_CLUB_H
#define GAME_CLUB_H

#include "common.h"

/* Twelve records at +0x66C (bzero 0x30 at init). */
typedef struct ClubRecord {
    /* 0x00 */ u16 practice_points; /* HYPOTHESIS: club practice points (KANGEI +2, thresholds 0x64/0xC8/0x164, ETC >> 4, 0x19B) */
    /* 0x02 */ u8 level; /* HYPOTHESIS: low nibble read through a u16 view is a team level; RENSYU adds 5, MASTER compares 8/10 */
    /* 0x03 */ u8 flags;
} ClubRecord; /* size 0x04 */

#endif /* GAME_CLUB_H */
