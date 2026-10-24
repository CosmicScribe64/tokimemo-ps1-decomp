#ifndef GAME_H
#define GAME_H

/* T-10890: parameter/name display view of the actor_pool record pool. */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

/* Game types (structs) of the main exe. The declarations of its functions and globals live in
 * main_api.h (T-3340); this file only adds types that need a struct definition. */

/* T-24501: saved records copied as whole objects by func_800552DC/func_80054FC0.
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
    /* 0x024 */ u8 unk_24[2];
    /* 0x026 */ u8 selected_slot;
    /* 0x027 */ u8 unk_27;
    /* 0x028 */ SaveConfigBlock config0;
    /* 0x03C */ SaveShooterSettings config1;
    /* 0x054 */ u8 unk_54[2];
    /* 0x056 */ u8 records[0x800];
    /* 0x856 */ u8 year[16];
    /* 0x866 */ u8 month[16];
    /* 0x876 */ u8 day[16];
    /* 0x886 */ u8 family_name[16][8];
    /* 0x906 */ u8 given_name[16][8];
    /* 0x986 */ u8 nickname[16][16];
    /* 0xA86 */ u8 club[16];
    /* 0xA96 */ u8 pad_A96[2];
    /* 0xA98 */ SaveAlbumParam metrics[9][16];
    /* 0xCD8 */ u8 unk_CD8[0x100];
    /* 0xDD8 */ u8 unk_DD8[0x100];
    /* 0xED8 */ u8 unk_ED8[0x28];
} SystemSaveImage; /* size 0xF00 */

typedef struct SystemDataSaveView {
    /* 0x000 */ u8 prefix[0x2C];
    /* 0x02C */ SaveConfigBlock config0;
    /* 0x040 */ SaveShooterSettings config1;
} SystemDataSaveView; /* size 0x58 */

/* GameState +0xC4..+0x140 (text buffers, +0xF8 and the nine +0xFC records) as the game save
 * copies it: one word-aligned 0x7C-byte record (func_8005557C). T-25105 */
typedef struct ProfileBlock {
    /* 0x000 */ s32 w[0x1F];
} ProfileBlock; /* size 0x7C */

/* GameState +0x56C..+0x66C as the game save copies it: one byte-aligned 0x100-byte record. T-25105 */
typedef struct SceneFlagBlock {
    /* 0x000 */ u8 b[0x100];
} SceneFlagBlock; /* size 0x100 */

/* One bank of the memory-card image holding a game save (func_8005557C writes it from GameState;
 * the comments name the source field). T-25105 */
typedef struct GameSaveImage {
    /* 0x000 */ s32 checksum;
    /* 0x004 */ u8 unk_004;             /* unk_03D */
    /* 0x005 */ u8 unk_005;             /* unk_03E */
    /* 0x006 */ u8 unk_006;             /* unk_03F */
    /* 0x007 */ u8 unk_007;             /* unk_040 */
    /* 0x008 */ u8 unk_008;             /* unk_041 */
    /* 0x009 */ u8 pad_009;
    /* 0x00A */ u16 unk_00A;            /* unk_042 */
    /* 0x00C */ MonthCalendar unk_00C[2];    /* unk_044 */
    /* 0x08C */ ProfileBlock unk_08C;      /* unk_0C4 */
    /* 0x108 */ GirlRecord unk_108[16];      /* unk_1BC */
    /* 0x488 */ u8 unk_488[16];         /* unk_53C */
    /* 0x498 */ WordView unk_498[8];      /* unk_54C */
    /* 0x4B8 */ WordView unk_4B8[12];     /* unk_66C */
    /* 0x4E8 */ SceneFlagBlock unk_4E8;     /* unk_56C */
    /* 0x5E8 */ u8 unk_5E8[4];          /* unk_728 */
    /* 0x5EC */ u8 unk_5EC[16];         /* unk_72C */
    /* 0x5FC */ u8 unk_5FC[16];         /* unk_73C */
    /* 0x60C */ u8 unk_60C[16];         /* unk_74C */
    /* 0x61C */ EventLogEntry unk_61C[256];  /* unk_75E */
    /* 0xE1C */ u8 unk_E1C;             /* unk_F6C */
    /* 0xE1D */ u8 unk_E1D;             /* unk_F6D */
    /* 0xE1E */ u8 unk_E1E;             /* unk_1092 */
    /* 0xE1F */ u8 unk_E1F;             /* unk_F6E */
    /* 0xE20 */ u8 unk_E20;             /* unk_03C */
    /* 0xE21 */ u8 pad_E21[3];
    /* 0xE24 */ WordView unk_E24;         /* unk_F68 */
    /* 0xE28 */ u8 unk_E28;             /* unk_F6F */
    /* 0xE29 */ u8 unk_E29;             /* unk_F70 */
    /* 0xE2A */ u8 unk_E2A;             /* unk_F73 */
    /* 0xE2B */ u8 unk_E2B;             /* unk_F75 */
    /* 0xE2C */ WordView unk_E2C;         /* unk_F64 */
    /* 0xE30 */ u8 unk_E30[0xD0];
} GameSaveImage; /* size 0xF00 */

/* 8-byte record of the array at text_list (stride 8; see k_disp_switch). */
typedef struct TextEntry {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u8 attribute;
    /* 0x05 */ u8 glyph_count;
    /* 0x06 */ u8 extra;
    /* 0x07 */ s8 active;
} TextEntry; /* size 0x08 */

/* Stack request of the memory-card file functions (func_80054694, func_80054704): name buffer plus a retry
 * counter; layout read from the frame (counter at +0x20 in func_80054704). */
typedef struct MemoryCardFileRequest {
    /* 0x00 */ u8 name[32];
    /* 0x20 */ s32 retry;
} MemoryCardFileRequest; /* size 0x24 */

/* Packed icon choice at GameState +0xF68, bits 4..7. */
typedef struct LastCommandBits {
    /* 0x00 */ u16 low : 4;
    u16 icon : 4;
    u16 rest : 8;
} LastCommandBits; /* size 0x02 */

#endif /* GAME_H */

#ifndef GAME_DISPLAY_ACTOR44_T10890
#define GAME_DISPLAY_ACTOR44_T10890
/* One record of the 0x44-byte display-record pools (T-20900): main actor_pool (160 records, actor_pool_96 is
 * record 96), EVENT D_800E9620 and GYOZI D_80129F40 (the same pool at the addresses those two overlays were
 * linked against). Field widths are the union of every unit's accesses: the original asm has lbu/sb only at
 * the byte fields (no lb through the pool symbols), lh/sh at the halfwords and lw/sw at +0x0C, +0x10, +0x34
 * and +0x38 (the other word accesses are whole-record copies). +0x24 .. +0x33 are also read as 16.16
 * fixed-point words: ActorFixed is that view of the same record. The per-unit views this replaces are
 * listed in wiki/data-types.md ("Shared-type unification"); TAIIKU's TaiikuActor44 stays (signed bytes). */
typedef struct Actor {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 flags;
    /* 0x03 */ u8 draw_flags;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 blink_period;
    /* 0x07 */ u8 brightness;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *frame_table;
    /* 0x10 */ u8 *sequence_table;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 sequence_slot;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 scale_x; /* HYPOTHESIS: 0x1000 = 1.0 (actor_clear default) */
    /* 0x1C */ s16 scale_y; /* HYPOTHESIS: 0x1000 = 1.0 */
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 rotation;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 x_fraction;
    /* 0x26 */ s16 x;
    /* 0x28 */ s16 y_fraction;
    /* 0x2A */ s16 y;
    /* 0x2C */ s16 dx_fraction; /* HYPOTHESIS: x step per frame */
    /* 0x2E */ s16 dx; /* HYPOTHESIS: integer part of the x step; the parameter panel (parameter_show) also uses it as a 0/1/2 animation phase */
    /* 0x30 */ s16 dy_fraction; /* HYPOTHESIS: y step per frame */
    /* 0x32 */ s16 dy; /* HYPOTHESIS: integer part of the y step */
    /* 0x34 */ u8 *tpage_table;
    /* 0x38 */ s32 quad_flags;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} Actor; /* size 0x44 */

/* 16.16 fixed-point word: word arithmetic through w, the signed integer part (the coordinate the halfword
 * view of a display record reads at +2) through h[1]. One type for the former MASTER, KANGEI, DATE, TAIIKU and
 * NAME_ENT copies (T-20900). */
typedef union Fix32 {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} Fix32; /* size 0x04 */

/* Actor with +0x24 and +0x28 as 16.16 fixed-point words and +0x2C, +0x30 as words (their
 * steps): the view of the units that move a record in fixed point (MASTER, KANGEI, DATE, EVENT, NAME_ENT,
 * TAIIKU, TEL). C89 has no anonymous unions, so the halfword users (most of the tree) keep Actor.
 * T-20900. */
typedef struct ActorFixed {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 flags;
    /* 0x03 */ u8 draw_flags;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *frame_table;
    /* 0x10 */ u8 *sequence_table;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 sequence_slot;
    /* 0x18 */ s16 frame;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20; /* rotate, copied as one word by actor_draw_all (lw 0x20, T-25606) */
    /* 0x24 */ Fix32 x;
    /* 0x28 */ Fix32 y;
    /* 0x2C */ s32 dx;
    /* 0x30 */ s32 dy;
    /* 0x34 */ u8 *tpage_table;
    /* 0x38 */ s32 quad_flags;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} ActorFixed; /* size 0x44 */

/* T-10890: image metadata consumed by portrait_build; eight observed parts. */
typedef struct SpritePart {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
} SpritePart; /* size 0x0E */

typedef struct SpriteFrame {
    /* 0x00 */ s16 count;
    /* 0x02 */ s16 origin_x;
    /* 0x04 */ s16 origin_y;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ SpritePart part[8];
} SpriteFrame; /* size 0x78 */

/* T-10890: two packed PSX 5:5:5 colours, interpolated as one word. */
typedef struct ColorPair555 {
    /* 0x00 */ u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 pad_0 : 1;
    /* 0x02 */ u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 pad_1 : 1;
} ColorPair555; /* size 0x04 */
#endif

#ifndef BUNKA_DISPATCH8_T10630_H
#define BUNKA_DISPATCH8_T10630_H
/* The two adjacent words at D_80122EB8, read and written together (one object): BUNKASAI's and BUNKAKEN's
 * CultureFestivalDispatch (T-10630) and BUNKASAI's BunkSelectWords (T-9510) and BunkModePair (T-9740), one type since
 * T-20900. */
typedef struct CultureFestivalDispatch {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04; /* HYPOTHESIS: the word pair D_80122EB8 shared by BUNKASAI and BUNKAKEN */
} CultureFestivalDispatch; /* size 0x08 */
#endif

#ifndef GAME_SPRITE_SLOT24_T9740
#define GAME_SPRITE_SLOT24_T9740
/* T-9740: 0x24-byte records at bg_layers, cleared by bg_layers_clear. */
typedef struct BgLayer {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 unk_12;
    /* 0x13 */ u8 pad_13;
    /* 0x14 */ u32 unk_14;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ u32 unk_20;
} BgLayer; /* size 0x24 */
#endif

#ifndef GAME_GS_BYTE_NIBBLES_T9910
#define GAME_GS_BYTE_NIBBLES_T9910
/* T-9910: byte view for the packed nibbles at GameState offset 0x0F5. */
typedef struct NibbleView {
    /* 0x00, bits 0-3 */ u8 low : 4;
    /* 0x00, bits 4-7 */ u8 high : 4;
} NibbleView; /* size 0x01 */
#endif

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
    /* 0x00 */ u32 unk_25 : 2;
    /* 0x00 */ u32 unk_27 : 2;
    /* 0x00 */ u32 b29 : 1;            /* T-20940: func_80077900 tests it (sll 2; bltz) */
    /* 0x00 */ u32 b30 : 1;
    /* 0x00 */ u32 unk_31 : 1;
} ClubFlagBits; /* size 0x04 */

/* T-10880: date and activity views of the eight GameState.unk_54C entries.
 * func_8006B0C8 sorts by month * 100 + day; January becomes month 13
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

#ifndef TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
#define TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
/* Additional views of GirlRecord.unk_0C, used by the scheduling routines. T-10880. */
typedef struct ScheduleGirlFlagBits {
    /* 0x00 */ u32 unk_00 : 16;
    /* 0x00 */ u32 b16 : 1;
    /* 0x00 */ u32 b17 : 1;
    /* 0x00 */ u32 b18 : 1;
    /* 0x00 */ u32 unk_19 : 1;
    /* 0x00 */ u32 unk_20 : 4;
    /* 0x00 */ u32 unk_24 : 8;
} ScheduleGirlFlagBits; /* size 0x04 */
#endif

#ifndef GAME_T10880_CALL_PAIRS
#define GAME_T10880_CALL_PAIRS
/* D_800B3B9C is indexed in 0x10-byte rows containing two call pairs.
 * NAME_ENT func_8014068C passes these words to cd_audio_request_set. */
typedef struct AudioRequestPair {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} AudioRequestPair; /* size 0x08; table view: AudioRequestPair[][2] */
#endif

#ifndef GAME_T17015_SRN_REC
#define GAME_T17015_SRN_REC
/* D_80121750: 0x28-byte records of the srn_* screen routines. srn_init fills one,
 * srn_vram_set and srn_set read it. T-17015. */
typedef struct SrnLayer {
    /* 0x00 */ u8 unk_00[2];
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04[2];
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u32 *unk_08;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ u8 unk_10;
    /* 0x11 */ u8 unk_11;
    /* 0x12 */ u8 unk_12;
    /* 0x13 */ u8 unk_13;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ u16 unk_18;
    /* 0x1A */ u8 unk_1A[2];
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ u32 unk_20;
    /* 0x24 */ u8 *unk_24;
} SrnLayer; /* size 0x28 */
/* Record i of D_80121750. srn_set indexes the global array directly: with a record pointer as1
 * cannot move the record loads above the stack stores (T-25006). */
#define SRN_REC(i) (((SrnLayer *)D_80121750)[i])
#endif

#ifndef GAME_T25006_MOD_REC
#define GAME_T25006_MOD_REC
/* D_801217A0: 0x14-byte tile-set records of the mod_* routines (mod_set_gpu reads one). T-25006. */
typedef struct ModTileSet {
    /* 0x00 */ u32 *unk_00;
    /* 0x04 */ u32 *unk_04;
    /* 0x08 */ u8 unk_08;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 unk_0C;
    /* 0x0D */ u8 unk_0D;
    /* 0x0E */ u8 unk_0E[6];
} ModTileSet; /* size 0x14 */
#endif
