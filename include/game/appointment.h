#ifndef GAME_APPOINTMENT_H
#define GAME_APPOINTMENT_H

#include "game/word_views.h"

typedef struct AppointmentEntry {
    /* 0x00 */ u32 unk_00 : 5;
    /* 0x00 */ u32 unk_05 : 19;
    /* 0x00 */ u32 b24 : 1;
    /* 0x00 */ u32 unk_25 : 4;
    /* 0x00 */ u32 active : 1;
    /* 0x00 */ u32 unk_30 : 2;
} AppointmentEntry; /* size 0x04 */

typedef union AppointmentSlots {
    /* 0x00 */ WordView a[8];
    /* 0x00 */ AppointmentEntry e0;
} AppointmentSlots; /* size 0x20 */

#endif /* GAME_APPOINTMENT_H */
