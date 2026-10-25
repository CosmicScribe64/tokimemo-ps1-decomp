#ifndef GAME_DAY_EVENT_H
#define GAME_DAY_EVENT_H

#include "common.h"

/* 32 records at +0x69C (cleared as words, then byte +0 set to 0xFF; indexed by GameState.day_event_index). */
typedef struct DayEvent {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 girl;
    /* 0x02 */ u8 scene;
    /* 0x03 */ u8 extra;
} DayEvent; /* size 0x04 */

/* 256 byte records at +0x75E (saved 8 bytes at a time; indexed from 1: code reads record i - 1). */
typedef struct EventLogEntry {
    /* 0x00 */ u8 girl;
    /* 0x01 */ u8 flag;
    /* 0x02 */ u8 scene;
    /* 0x03 */ u8 rank;
    /* 0x04 */ u8 year;
    /* 0x05 */ u8 month;
    /* 0x06 */ u8 day;
    /* 0x07 */ u8 weekday;
} EventLogEntry; /* size 0x08 */

#endif /* GAME_DAY_EVENT_H */
