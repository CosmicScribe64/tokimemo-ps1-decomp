#ifndef GAME_SCHEDULE_RECORDS_H
#define GAME_SCHEDULE_RECORDS_H

#include "common.h"
#include "game/word_views.h"

#ifndef TOKIMEMO_SCHEDULE_FLAGS_H
#define TOKIMEMO_SCHEDULE_FLAGS_H
/* T-10880: word view at GameState +0xF4 in day_event_queue_build_week.
 * Bit 18 is tested with lw/sll 13/srl 31 and stored through byte +0xF6.
 * Bits 21..23 are a three-bit counter read and decremented through +0xF6.
 * T-20900: also DATE's DateFlag17 and DateFlags0F4 and OPTION's OptionPlayerBits (unk_08, unk_12: its f8,
 * f12; bits 16..20 tested one by one). */
typedef struct PlayerFlagBits {
    /* 0x00 */ u32 unk_00 : 8;
    /* 0x00 */ u32 health : 4; /* HealthState */
    /* 0x00 */ u32 club : 4;
    /* 0x00 */ u32 b16 : 1;
    /* 0x00 */ u32 b17 : 1;
    /* 0x00 */ u32 b18 : 1;
    /* 0x00 */ u32 b19 : 1;
    /* 0x00 */ u32 b20 : 1;
    /* 0x00 */ u32 recovery_days : 3;
    /* 0x00 */ u32 unk_24 : 8;
} PlayerFlagBits; /* size 0x04 */

/* T-10880: word view of GameState.unk_66C[11]. The counter occupies
 * bits 25..26; increment reads the word and writes byte +3 bits 1..2.
 * T-20900: also TEL's TelClubFlags (b24), KANGEI's KgSlot and ETC's EtcSlot (grade: unk_25, flag: b30) and
 * MASTER's MasterClubBits (count: unk_25, b30). RENSYU's RensyuClubCount is a u16 view and stays. */
typedef struct ClubFlagBits {
    /* 0x00 */ u32 unk_00 : 24;
    /* 0x00 */ u32 b24 : 1;
    /* 0x00 */ u32 club_progress_grade : 2;
    /* 0x00 */ u32 unk_27 : 2;
    /* 0x00 */ u32 b29 : 1;            /* T-20940: holiday_activity_play tests it (sll 2; bltz) */
    /* 0x00 */ u32 b30 : 1;
    /* 0x00 */ u32 unk_31 : 1;
} ClubFlagBits; /* size 0x04 */

/* T-10880: date and activity views of the eight GameState.unk_54C entries.
 * hud_appointment_list_sort sorts by month * 100 + day; January becomes month 13
 * temporarily when the current month is December. */
typedef struct AppointmentDate {
    /* 0x00 */ u16 unk_00 : 12;
    /* 0x00 */ u16 month : 4;
    /* 0x02 */ u16 day : 5;
    /* 0x02 */ u16 unk_15 : 11;
} AppointmentDate; /* size 0x04 */

/* T-20900: also main's TelEntryFlags, EntryFlags54C and EntryFlags29 (idx: unk_00, nib: unk_25, b29: active),
 * TEL's TelEntryFlags and OPTION's OptionScheduleBits (f0: unk_00, f25: unk_25). */
typedef struct AppointmentBits {
    /* 0x00 */ u32 place : 5;
    /* 0x00 */ u32 unk_05 : 19;
    /* 0x00 */ u32 flag : 1;
    /* 0x00 */ u32 girl : 4;
    /* 0x00 */ u32 active : 1;
    /* 0x00 */ u32 unk_30 : 2;
} AppointmentBits; /* size 0x04 */
#endif

#endif /* GAME_SCHEDULE_RECORDS_H */
