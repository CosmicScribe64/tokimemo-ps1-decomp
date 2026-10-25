#ifndef GAME_SAVE_IMAGE_H
#define GAME_SAVE_IMAGE_H

#include "common.h"
#include "game/calendar.h"
#include "game/day_event.h"
#include "game/girl.h"
#include "game/system_data.h"
#include "game/word_views.h"

/* T-24501: saved records copied as whole objects by memory_card_system_bank_load/memory_card_system_bank_build.
 * HYPOTHESIS: these are settings records; field meanings remain unknown. The 20-byte
 * record owns the protection flag in its last byte. The 24-byte record preserves
 * SystemData bits, names and words without changing their canonical declarations. */
typedef struct SaveConfigBlock {
    /* 0x000 */ u16 unk_00;
    /* 0x002 */ u16 unk_02;
    /* 0x004 */ u16 unk_04;
    /* 0x006 */ u8 unk_06[10];
    /* 0x010 */ WordView flags; /* HYPOTHESIS: 0x14-byte settings record copied by the save routines; protection flag in the last byte */
} SaveConfigBlock; /* size 0x14 */

typedef struct SaveShooterSettings {
    /* 0x000 */ ShooterSettingsBits bits;
    /* 0x002 */ u8 name0[3];
    /* 0x005 */ u8 name1[3];
    /* 0x008 */ u8 name2[3];
    /* 0x00B */ u8 unk_0B;
    /* 0x00C */ s32 w0;
    /* 0x010 */ s32 w1;
    /* 0x014 */ s32 w2; /* HYPOTHESIS: 0x18-byte copy of SystemData +0x40.. (settings bits, three names, three words) */
} SaveShooterSettings; /* size 0x18 */

typedef union SaveAlbumParam {
    /* 0x000 */ AlbumParam metric;
    /* 0x000 */ WordView word;
} SaveAlbumParam; /* size 0x4 */

typedef struct SystemSaveImage {
    /* 0x000 */ s32 checksum;
    /* 0x004 */ u8 slot_status[16];
    /* 0x014 */ u8 flags[16];
    /* 0x024 */ u8 karaoke_unlock_bits[2];
    /* 0x026 */ u8 selected_slot;
    /* 0x027 */ u8 unk_27;
    /* 0x028 */ SaveConfigBlock config0;
    /* 0x03C */ SaveShooterSettings config1;
    /* 0x054 */ u8 album_state[2];
    /* 0x056 */ u8 album_entries[0x800];
    /* 0x856 */ u8 year[16];
    /* 0x866 */ u8 month[16];
    /* 0x876 */ u8 day[16];
    /* 0x886 */ u8 family_name[16][8];
    /* 0x906 */ u8 given_name[16][8];
    /* 0x986 */ u8 nickname[16][16];
    /* 0xA86 */ u8 club[16];
    /* 0xA96 */ u8 pad_A96[2];
    /* 0xA98 */ SaveAlbumParam metrics[9][16];
    /* 0xCD8 */ u8 slot_girl_marks[0x100];
    /* 0xDD8 */ u8 layout_settings[0x100];
    /* 0xED8 */ u8 unk_ED8[0x28];
} SystemSaveImage; /* size 0xF00 */

typedef struct SystemDataSaveView {
    /* 0x000 */ u8 prefix[0x2C];
    /* 0x02C */ SaveConfigBlock config0;
    /* 0x040 */ SaveShooterSettings config1;
} SystemDataSaveView; /* size 0x58 */

/* GameState +0xC4..+0x140 (text buffers, +0xF8 and the nine +0xFC records) as the game save
 * copies it: one word-aligned 0x7C-byte record (memory_card_game_bank_build). T-25105 */
typedef struct ProfileBlock {
    /* 0x000 */ s32 w[0x1F];
} ProfileBlock; /* size 0x7C */

/* GameState +0x56C..+0x66C as the game save copies it: one byte-aligned 0x100-byte record. T-25105 */
typedef struct SceneFlagBlock {
    /* 0x000 */ u8 b[0x100];
} SceneFlagBlock; /* size 0x100 */

/* One bank of the memory-card image holding a game save (memory_card_game_bank_build writes it from GameState;
 * the comments name the source field). T-25105 */
typedef struct GameSaveImage {
    /* 0x000 */ s32 checksum;
    /* 0x004 */ u8 vacation;             /* unk_03D */
    /* 0x005 */ u8 year;             /* unk_03E */
    /* 0x006 */ u8 month;             /* unk_03F */
    /* 0x007 */ u8 day;             /* unk_040 */
    /* 0x008 */ u8 weekday;             /* unk_041 */
    /* 0x009 */ u8 pad_009;
    /* 0x00A */ u16 day_count;            /* unk_042 */
    /* 0x00C */ MonthCalendar month_calendar[2];    /* unk_044 */
    /* 0x08C */ ProfileBlock profile;      /* unk_0C4 */
    /* 0x108 */ GirlRecord girls[16];      /* unk_1BC */
    /* 0x488 */ u8 girl_cleared[16];         /* unk_53C */
    /* 0x498 */ WordView appointments[8];      /* unk_54C */
    /* 0x4B8 */ WordView clubs[12];     /* unk_66C */
    /* 0x4E8 */ SceneFlagBlock scene_flags;     /* unk_56C */
    /* 0x5E8 */ u8 date_place_history[4];          /* unk_728 */
    /* 0x5EC */ u8 girl_last_date_spot[16];         /* unk_72C */
    /* 0x5FC */ u8 girl_last_date_spot_timer[16];         /* unk_73C */
    /* 0x60C */ u8 girl_scene_bits[16];         /* unk_74C */
    /* 0x61C */ EventLogEntry event_log[256];  /* unk_75E */
    /* 0xE1C */ u8 icon_mode;             /* unk_F6C */
    /* 0xE1D */ u8 icon_count;             /* unk_F6D */
    /* 0xE1E */ u8 menu_mode;             /* unk_1092 */
    /* 0xE1F */ u8 sd_anim_fast;             /* unk_F6E */
    /* 0xE20 */ u8 voice_mode;             /* unk_03C */
    /* 0xE21 */ u8 pad_E21[3];
    /* 0xE24 */ WordView last_command;         /* unk_F68 */
    /* 0xE28 */ u8 cursor_speed;             /* unk_F6F */
    /* 0xE29 */ u8 click_mode;             /* unk_F70 */
    /* 0xE2A */ u8 double_click_interval;             /* unk_F73 */
    /* 0xE2B */ u8 confirm_button;             /* unk_F75 */
    /* 0xE2C */ WordView quarter_flags;         /* unk_F64 */
    /* 0xE30 */ u8 unk_E30[0xD0];
} GameSaveImage; /* size 0xF00 */

/* One bank of the memory-card save image: D_80123320[2] follows the 0x200-byte header at memory_card_block; header
 * and banks together are one 0x2000-byte card block. T-17007 */
typedef struct MemoryCardBank {
    /* 0x000 */ s32 checksum;      /* byte sum of data (memory_card_bank_validate) */
    /* 0x004 */ u8 data[0xEFC];
} MemoryCardBank; /* size 0xF00 */

#endif /* GAME_SAVE_IMAGE_H */
