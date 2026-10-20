#ifndef GAME_H
#define GAME_H

/* T-10890: parameter/name display view of the D_8011ECD0 record pool. */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

/* Game types (structs) of the main exe. The declarations of its functions and globals live in
 * main_api.h (T-3340); this file only adds types that need a struct definition. */

/* 8-byte record of the array at text_list (stride 8; see k_disp_switch). */
typedef struct Entry8 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ s8 unk_07;
} Entry8; /* size 0x08 */

/* Stack request of the memory-card file functions (func_80054694, func_80054704): name buffer plus a retry
 * counter; layout read from the frame (counter at +0x20 in func_80054704). */
typedef struct FileReq {
    /* 0x00 */ u8 name[32];
    /* 0x20 */ s32 retry;
} FileReq; /* size 0x24 */

/* Packed icon choice at GameState +0xF68, bits 4..7. */
typedef struct MenuIconBits {
    /* 0x00 */ u16 low : 4;
    u16 icon : 4;
    u16 rest : 8;
} MenuIconBits; /* size 0x02 */

#endif /* GAME_H */

#ifndef GAME_DISPLAY_ACTOR44_T10890
#define GAME_DISPLAY_ACTOR44_T10890
/* One record of the 0x44-byte display-record pools (T-20900): main D_8011ECD0 (160 records, D_80120650 is
 * record 96), EVENT D_800E9620 and GYOZI D_80129F40 (the same pool at the addresses those two overlays were
 * linked against). Field widths are the union of every unit's accesses: the original asm has lbu/sb only at
 * the byte fields (no lb through the pool symbols), lh/sh at the halfwords and lw/sw at +0x0C, +0x10, +0x34
 * and +0x38 (the other word accesses are whole-record copies). +0x24 .. +0x33 are also read as 16.16
 * fixed-point words: DisplayFixActor44 is that view of the same record. The per-unit views this replaces are
 * listed in wiki/data-types.md ("Shared-type unification"); TAIIKU's TaiikuActor44 stays (signed bytes). */
typedef struct DisplayActor44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s16 unk_28;
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ s16 unk_30;
    /* 0x32 */ s16 unk_32;
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} DisplayActor44; /* size 0x44 */

/* 16.16 fixed-point word: word arithmetic through w, the signed integer part (the coordinate the halfword
 * view of a display record reads at +2) through h[1]. One type for the former MASTER, KANGEI, DATE, TAIIKU and
 * NAME_ENT copies (T-20900). */
typedef union Fix32 {
    /* 0x00 */ s32 w;
    /* 0x00 */ s16 h[2];
} Fix32; /* size 0x04 */

/* DisplayActor44 with +0x24 and +0x28 as 16.16 fixed-point words and +0x2C, +0x30 as words (their
 * steps): the view of the units that move a record in fixed point (MASTER, KANGEI, DATE, EVENT, NAME_ENT,
 * TAIIKU, TEL). C89 has no anonymous unions, so the halfword users (most of the tree) keep DisplayActor44.
 * T-20900. */
typedef struct DisplayFixActor44 {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ Fix32 unk_24;
    /* 0x28 */ Fix32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u8 unk_40;
    /* 0x41 */ u8 unk_41;
    /* 0x42 */ u8 unk_42;
    /* 0x43 */ u8 unk_43;
} DisplayFixActor44; /* size 0x44 */

/* T-10890: image metadata consumed by func_80062DBC; eight observed parts. */
typedef struct DisplayImagePart0E {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
} DisplayImagePart0E; /* size 0x0E */

typedef struct DisplayImage78 {
    /* 0x00 */ s16 count;
    /* 0x02 */ s16 origin_x;
    /* 0x04 */ s16 origin_y;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ DisplayImagePart0E part[8];
} DisplayImage78; /* size 0x78 */

/* T-10890: two packed PSX 5:5:5 colours, interpolated as one word. */
typedef struct DisplayColorPair4 {
    /* 0x00 */ u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 pad_0 : 1;
    /* 0x02 */ u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 pad_1 : 1;
} DisplayColorPair4; /* size 0x04 */
#endif

#ifndef BUNKA_DISPATCH8_T10630_H
#define BUNKA_DISPATCH8_T10630_H
/* The two adjacent words at D_80122EB8, read and written together (one object): BUNKASAI's and BUNKAKEN's
 * BunkaDispatch8 (T-10630) and BUNKASAI's BunkSelectWords (T-9510) and BunkModePair (T-9740), one type since
 * T-20900. */
typedef struct BunkaDispatch8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} BunkaDispatch8; /* size 0x08 */
#endif

#ifndef GAME_SPRITE_SLOT24_T9740
#define GAME_SPRITE_SLOT24_T9740
/* T-9740: 0x24-byte records at D_801220D0, cleared by func_8004164C. */
typedef struct SpriteSlot24 {
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
} SpriteSlot24; /* size 0x24 */
#endif

#ifndef GAME_GS_BYTE_NIBBLES_T9910
#define GAME_GS_BYTE_NIBBLES_T9910
/* T-9910: byte view for the packed nibbles at GameState offset 0x0F5. */
typedef struct GsByteNibbles {
    /* 0x00, bits 0-3 */ u8 low : 4;
    /* 0x00, bits 4-7 */ u8 high : 4;
} GsByteNibbles; /* size 0x01 */
#endif

#ifndef TOKIMEMO_SCHEDULE_FLAGS_H
#define TOKIMEMO_SCHEDULE_FLAGS_H
/* T-10880: word view at GameState +0xF4 in func_8006D7EC.
 * Bit 18 is tested with lw/sll 13/srl 31 and stored through byte +0xF6.
 * Bits 21..23 are a three-bit counter read and decremented through +0xF6.
 * T-20900: also DATE's DateFlag17 and DateFlags0F4 and OPTION's OptionPlayerBits (unk_08, unk_12: its f8,
 * f12; bits 16..20 tested one by one). */
typedef struct ScheduleFlags {
    /* 0x00 */ u32 unk_00 : 8;
    /* 0x00 */ u32 unk_08 : 4;
    /* 0x00 */ u32 unk_12 : 4;
    /* 0x00 */ u32 b16 : 1;
    /* 0x00 */ u32 b17 : 1;
    /* 0x00 */ u32 b18 : 1;
    /* 0x00 */ u32 b19 : 1;
    /* 0x00 */ u32 b20 : 1;
    /* 0x00 */ u32 unk_21 : 3;
    /* 0x00 */ u32 unk_24 : 8;
} ScheduleFlags; /* size 0x04 */

/* T-10880: word view of GameState.unk_66C[11]. The counter occupies
 * bits 25..26; increment reads the word and writes byte +3 bits 1..2.
 * T-20900: also TEL's TelClubFlags (b24), KANGEI's KgSlot and ETC's EtcSlot (grade: unk_25, flag: b30) and
 * MASTER's MasterClubBits (count: unk_25, b30). RENSYU's RensyuClubCount is a u16 view and stays. */
typedef struct ScheduleClubFlags {
    /* 0x00 */ u32 unk_00 : 24;
    /* 0x00 */ u32 b24 : 1;
    /* 0x00 */ u32 unk_25 : 2;
    /* 0x00 */ u32 unk_27 : 2;
    /* 0x00 */ u32 b29 : 1;            /* T-20940: func_80077900 tests it (sll 2; bltz) */
    /* 0x00 */ u32 b30 : 1;
    /* 0x00 */ u32 unk_31 : 1;
} ScheduleClubFlags; /* size 0x04 */

/* T-10880: date and activity views of the eight GameState.unk_54C entries.
 * func_8006B0C8 sorts by month * 100 + day; January becomes month 13
 * temporarily when the current month is December. */
typedef struct ScheduleEntryDate {
    /* 0x00 */ u16 unk_00 : 12;
    /* 0x00 */ u16 month : 4;
    /* 0x02 */ u16 day : 5;
    /* 0x02 */ u16 unk_15 : 11;
} ScheduleEntryDate; /* size 0x04 */

/* T-20900: also main's TelEntryFlags, EntryFlags54C and EntryFlags29 (idx: unk_00, nib: unk_25, b29: active),
 * TEL's TelEntryFlags and OPTION's OptionScheduleBits (f0: unk_00, f25: unk_25). */
typedef struct ScheduleEntryFlags {
    /* 0x00 */ u32 unk_00 : 5;
    /* 0x00 */ u32 unk_05 : 19;
    /* 0x00 */ u32 b24 : 1;
    /* 0x00 */ u32 unk_25 : 4;
    /* 0x00 */ u32 active : 1;
    /* 0x00 */ u32 unk_30 : 2;
} ScheduleEntryFlags; /* size 0x04 */
#endif

#ifndef TOKIMEMO_ALBUM_LAYOUT_H
#define TOKIMEMO_ALBUM_LAYOUT_H
/* T-10880: NAME_ENT's album record; the album work area itself is Work800E7D10 (main_api.h, T-20900). */
typedef struct NameRecordF00 {
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
    /* 0x2C4 */ NameMetric4 metrics[9];
    /* 0x2E8 */ u8 unk_2E8[0x3A0];
    /* 0x688 */ u8 flags[14];
    /* 0x696 */ u8 unk_696[0x86A];
} NameRecordF00; /* size 0xF00 */
#endif

#ifndef TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
#define TOKIMEMO_SCHEDULE_GIRL_FLAGS_H
/* Additional views of Rec38.unk_0C, used by the scheduling routines. T-10880. */
typedef struct ScheduleGirlFlags {
    /* 0x00 */ u32 unk_00 : 16;
    /* 0x00 */ u32 b16 : 1;
    /* 0x00 */ u32 b17 : 1;
    /* 0x00 */ u32 b18 : 1;
    /* 0x00 */ u32 unk_19 : 1;
    /* 0x00 */ u32 unk_20 : 4;
    /* 0x00 */ u32 unk_24 : 8;
} ScheduleGirlFlags; /* size 0x04 */
#endif

#ifndef GAME_T10880_CALL_PAIRS
#define GAME_T10880_CALL_PAIRS
/* D_800B3B9C is indexed in 0x10-byte rows containing two call pairs.
 * NAME_ENT func_8014068C passes these words to func_80046290. */
typedef struct CallPair8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} CallPair8; /* size 0x08; table view: CallPair8[][2] */
#endif

#ifndef GAME_T17015_SRN_REC
#define GAME_T17015_SRN_REC
/* D_80121750: 0x28-byte records of the srn_* screen routines. srn_init fills one,
 * srn_vram_set and srn_set read it. T-17015. */
typedef struct SrnRec28 {
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
} SrnRec28; /* size 0x28 */
#endif
