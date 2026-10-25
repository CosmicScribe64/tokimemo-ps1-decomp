#ifndef GAME_CALENDAR_H
#define GAME_CALENDAR_H

#include "common.h"

/* Two 0x40-byte records at +0x44 (bzero'd together, indexed by 0/1 with stride 0x40): two arrays
 * of 0x20 bytes indexed by a day number (+0x00[0] = 30 and +0x20[0] = 5 at init). */
typedef struct MonthCalendar {
    /* 0x00 */ u8 day_kind[0x20]; /* DayKind */
    /* 0x20 */ u8 marker[0x20]; /* CalendarMarker */
} MonthCalendar; /* size 0x40 */

#endif /* GAME_CALENDAR_H */
