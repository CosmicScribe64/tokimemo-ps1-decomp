#ifndef MAIN_API_H
#define MAIN_API_H

/* The one declaration of every main-exe symbol (T-3340). Maintained with tools/sync_protos.py.
 *
 *   - A main-exe function or global (address 0x80041000-0x8012B537, or a name from
 *     config/symbol_addrs*.txt) is declared here and nowhere else. include/game.h and every
 *     include/ovl/<NAME>.h include this file; tools/check_headers.py (ninja, CI) rejects any other
 *     declaration and names the fix. New symbol: add it here (python3 tools/sync_protos.py --write
 *     copies it from the header that has it).
 *   - The type is the one the main-exe definition has (matched C in src/main). While the function is
 *     still INCLUDE_ASM the type comes from the call sites; change it here when a definition shows
 *     better. `()` is kept on purpose where callers pass other arguments than the definition takes.
 *   - An overlay (or src/main file) that was matched against another view of a symbol (signed byte of
 *     an unsigned global, scalar of an array, another return type, an implicit declaration) keeps it
 *     explicitly: it defines MAIN_API_OVERRIDE_<symbol> with the reason before its first include and
 *     declares its own type next to its other declarations; the symbol is then wrapped in the
 *     #ifndef below. An override changes what that file's functions see, so add one only when the
 *     matched code needs it: python3 tools/sync_protos.py --prune tries to delete each (ninja decides).
 *     Rules: CODING_STANDARDS 8a. */

#include "common.h"
#include "libgpu.h"

/* ---- aggregate types (T-5000) ----
 * Records whose fields splat names as separate D_ globals. tools/type_recovery.py proposes them from
 * the original asm (indexed accesses with one stride, pointer walks, ordered store/load pairs);
 * evidence and limits: wiki/data-types.md. Field names stay unk_XX until the meaning is known. */

/* Flag word of Rec34; EVENT tests bits 12-14 one at a time (sll; bltz/bgez in the original). */
typedef struct Rec34Flags {
    u32 pad : 12;
    u32 b12 : 1;
    u32 b13 : 1;
    u32 b14 : 1;
    u32 rest : 17;
} Rec34Flags; /* size 0x04 */

/* 0x34-byte record of the table at D_800B0A04 (12 records; EVENT indexes it with stride 0x34). */
typedef struct Rec34 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ Rec34Flags unk_0C;
    /* 0x10 */ u8 pad10[7];
    /* 0x17 */ u8 unk_17;
    /* 0x18 */ u8 pad18[0xA];
    /* 0x22 */ u8 unk_22;
    /* 0x23 */ u8 pad23[4];
    /* 0x27 */ u8 unk_27;
    /* 0x28 */ u8 pad28[5];
    /* 0x2D */ u8 unk_2D;
    /* 0x2E */ u8 pad2E[6];
} Rec34; /* size 0x34 */

/* One word read as a whole and through its halves and bytes: the original tests bit-fields of these
 * words with lw and shifts, and IDO-style narrowed loads (lbu, then a shift) of single bytes. */
typedef union GsWord {
    /* 0x00 */ s32 w;
    /* 0x00 */ u32 u;
    /* 0x00 */ u16 h[2];
    /* 0x00 */ u8 b[4];
} GsWord; /* size 0x04 */

/* A halfword read as a whole (lhu) and through its bytes (lbu). */
typedef union GsHalf {
    /* 0x00 */ u16 h;
    /* 0x00 */ u8 b[2];
} GsHalf; /* size 0x02 */

/* 0x38-byte record of the table at GameState +0x1BC (16 records: a loop walks it with stride 0x38 up
 * to +0x53C; DATE, KANGEI and SHOUGATU index it; most code reads one record per girl, indexed by
 * GameState.unk_F5F). The first 0xC bytes are walked as four 0xC-byte steps elsewhere. +0x0C and
 * +0x10 are bit-field words read through the word and through single bytes (+0x10: bits 0-3 and
 * 4-8 are compared with the month and day at GameState +0x3F/+0x40). */
typedef struct Rec38 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ GsWord unk_0C;
    /* 0x10 */ GsWord unk_10;
    /* 0x14 */ u8 unk_14[0x24];
} Rec38; /* size 0x38 */

/* 0x24-byte record of the table at D_801217D0 (at least 12 records: ENDING walks 12; indexed with
 * stride 0x24, and code that reads entry i - 1 makes splat name D_801217AC). Record 0 is set field by
 * field in func_8005AD70; records 0-5 have a flag in bit 31 of unk_00. */
typedef struct Rec24 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ u8 unk_0E;
    /* 0x0F */ u8 unk_0F;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ u8 unk_14;
    /* 0x15 */ u8 unk_15;
    /* 0x16 */ u8 unk_16;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
} Rec24; /* size 0x24 */

/* Work area at D_80125D10 (0x50 bytes; main 80079B10 and 8007C030 only). The original keeps its
 * stores and loads in source order, as for one object (wiki/data-types.md); unk_44 and unk_48 are
 * message buffers passed to func_80045414. */
typedef struct Work80125D10 {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ u16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s16 unk_28;
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ s16 unk_30;
    /* 0x32 */ s16 unk_32;
    /* 0x34 */ u16 unk_34;
    /* 0x36 */ u16 unk_36;
    /* 0x38 */ u16 unk_38;
    /* 0x3A */ s16 unk_3A;
    /* 0x3C */ s16 unk_3C;
    /* 0x3E */ s16 unk_3E;
    /* 0x40 */ u16 unk_40;
    /* 0x42 */ u16 unk_42;
    /* 0x44 */ u8 unk_44[4];
    /* 0x48 */ u8 unk_48[8];
} Work80125D10; /* size 0x50 */

/* ---- GameState (T-5100) ----
 * The main bss block 0x800E6280..0x800E7D10 is one object of the original: loops over arrays deep
 * inside it keep 0x800E6280 in the base register and reach the arrays through large offsets
 * (search_tpage: +0x1128/+0x1228, OPTION: +0x163C..+0x17BC, ETC: +0x75E), which a compiler can only
 * do when they are one symbol. Layout, evidence and confidence: wiki/game-state.md. O.BIN names the
 * object of this size and position `sys` (hypothesis, not applied). Fields are named by offset;
 * C code reaches them only through D_800E6280 (tools/migrate_globals.py rewrites the old D_ names
 * and ninja rejects them). Records are named by their offset in GameState. */

/* Two records at +0x1C, one per D_8011ECA0 (SetWorkBase stores a work address and two words). */
typedef struct GsRec01C {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} GsRec01C; /* size 0x0C */

/* Two 0x40-byte records at +0x44 (bzero'd together, indexed by 0/1 with stride 0x40): two arrays
 * of 0x20 bytes indexed by a day number (+0x00[0] = 30 and +0x20[0] = 5 at init). */
typedef struct GsRec044 {
    /* 0x00 */ u8 unk_00[0x20];
    /* 0x20 */ u8 unk_20[0x20];
} GsRec044; /* size 0x40 */

/* Nine records at +0xFC (separate members); the s16 at +2 holds the nine values parameter_show_init reads, KANGEI
 * copies whole records. */
typedef struct GsRec0FC {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
} GsRec0FC; /* size 0x04 */

/* Twelve records at +0x66C (bzero 0x30 at init). */
typedef struct GsRec66C {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
} GsRec66C; /* size 0x04 */

/* 32 records at +0x69C (cleared as words, then byte +0 set to 0xFF; indexed by GameState.unk_71C). */
typedef struct GsRec69C {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
} GsRec69C; /* size 0x04 */

/* 256 byte records at +0x75E (saved 8 bytes at a time; indexed from 1: code reads record i - 1). */
typedef struct GsRec75E {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
} GsRec75E; /* size 0x08 */

/* 32 records at +0xF90 (indexed with stride 8 and in pairs with stride 16). */
typedef struct GsRecF90 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
} GsRecF90; /* size 0x08 */

/* 32 records at +0x1328 (palette_load_vram). */
typedef struct GsRec1328 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
} GsRec1328; /* size 0x08 */

/* 33 records at +0x142C (csr_load_vram). */
typedef struct GsRec142C {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u16 unk_08;
    /* 0x0A */ u16 unk_0A;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ u16 unk_0E;
} GsRec142C; /* size 0x10 */

typedef struct GameState {
    /* 0x000 */ u16 unk_000;
    /* 0x002 */ u16 unk_002;
    /* 0x004 */ u16 unk_004;
    /* 0x006 */ u8 unk_006[0xE];
    /* 0x014 */ s16 unk_014[2];        /* indexed by D_8011ECA0 */
    /* 0x018 */ s16 unk_018[2];        /* indexed by D_8011ECA0 */
    /* 0x01C */ GsRec01C unk_01C[2];  /* indexed by D_8011ECA0 (SetWorkBase) */
    /* 0x034 */ u8 unk_034;
    /* 0x035 */ u8 unk_035;
    /* 0x036 */ u8 unk_036;
    /* 0x037 */ u8 unk_037;
    /* 0x038 */ u8 unk_038;
    /* 0x039 */ u8 unk_039;
    /* 0x03A */ u8 unk_03A;
    /* 0x03B */ u8 unk_03B;
    /* 0x03C */ s8 unk_03C;
    /* 0x03D */ u8 unk_03D;
    /* 0x03E */ u8 unk_03E;            /* 0x5F at init; with +0x3F..+0x41 a date (year, month, day, weekday?) */
    /* 0x03F */ u8 unk_03F;            /* 4 at init; compared with Rec38.unk_10 bits 0-3 */
    /* 0x040 */ u8 unk_040;            /* 4 at init; compared with Rec38.unk_10 bits 4-8 */
    /* 0x041 */ u8 unk_041;
    /* 0x042 */ s16 unk_042;
    /* 0x044 */ GsRec044 unk_044[2];
    /* 0x0C4 */ u8 unk_0C4[0x10];      /* +0xC4..+0xF4: text buffers (NAME_ENT) */
    /* 0x0D4 */ u8 unk_0D4[8];
    /* 0x0DC */ u8 unk_0DC[8];
    /* 0x0E4 */ u8 unk_0E4[0x10];
    /* 0x0F4 */ GsHalf unk_0F4;
    /* 0x0F6 */ GsHalf unk_0F6;
    /* 0x0F8 */ s32 unk_0F8;
    /* 0x0FC */ GsRec0FC unk_0FC;        /* +0xFC..+0x120: nine records, members (an array sums in another order, ETC func_8014A32C) */
    /* 0x100 */ GsRec0FC unk_100;
    /* 0x104 */ GsRec0FC unk_104;
    /* 0x108 */ GsRec0FC unk_108;
    /* 0x10C */ GsRec0FC unk_10C;
    /* 0x110 */ GsRec0FC unk_110;
    /* 0x114 */ GsRec0FC unk_114;
    /* 0x118 */ GsRec0FC unk_118;
    /* 0x11C */ GsRec0FC unk_11C;
    /* 0x120 */ u8 unk_120[0x9C];
    /* 0x1BC */ Rec38 unk_1BC[16];
    /* 0x53C */ u8 unk_53C[0x10];
    /* 0x54C */ GsWord unk_54C[8];
    /* 0x56C */ u8 unk_56C[0xFC];
    /* 0x668 */ u8 unk_668[4];
    /* 0x66C */ GsRec66C unk_66C[12];
    /* 0x69C */ GsRec69C unk_69C[32];
    /* 0x71C */ u8 unk_71C;
    /* 0x71D */ u8 unk_71D;
    /* 0x71E */ u8 unk_71E;
    /* 0x71F */ u8 unk_71F;
    /* 0x720 */ u8 unk_720;
    /* 0x721 */ u8 unk_721;
    /* 0x722 */ u8 unk_722;
    /* 0x723 */ u8 unk_723;
    /* 0x724 */ u8 unk_724;
    /* 0x725 */ u8 unk_725;
    /* 0x726 */ u8 unk_726;
    /* 0x727 */ u8 unk_727;
    /* 0x728 */ u8 unk_728[4];
    /* 0x72C */ u8 unk_72C[0x10];      /* +0x72C, +0x73C, +0x74C: one byte per girl (loops of 11) */
    /* 0x73C */ u8 unk_73C[0x10];
    /* 0x74C */ u8 unk_74C[0x10];
    /* 0x75C */ u8 unk_75C;
    /* 0x75D */ u8 unk_75D;
    /* 0x75E */ GsRec75E unk_75E[256];
    /* 0xF5E */ u8 unk_F5E;
    /* 0xF5F */ u8 unk_F5F;            /* index of the current girl's Rec38 record */
    /* 0xF60 */ s8 unk_F60;
    /* 0xF61 */ s8 unk_F61;
    /* 0xF62 */ s8 unk_F62;
    /* 0xF63 */ s8 unk_F63;
    /* 0xF64 */ u8 unk_F64[4];
    /* 0xF68 */ GsWord unk_F68;
    /* 0xF6C */ s8 unk_F6C;
    /* 0xF6D */ s8 unk_F6D;
    /* 0xF6E */ s8 unk_F6E;
    /* 0xF6F */ u8 unk_F6F;
    /* 0xF70 */ s8 unk_F70;
    /* 0xF71 */ s8 unk_F71;
    /* 0xF72 */ u8 unk_F72;
    /* 0xF73 */ s8 unk_F73;
    /* 0xF74 */ u8 unk_F74;
    /* 0xF75 */ u8 unk_F75;
    /* 0xF76 */ u8 unk_F76;
    /* 0xF77 */ u8 unk_F77;
    /* 0xF78 */ u8 unk_F78;
    /* 0xF79 */ u8 unk_F79;
    /* 0xF7A */ s16 unk_F7A;
    /* 0xF7C */ s16 unk_F7C;
    /* 0xF7E */ u8 unk_F7E[2];
    /* 0xF80 */ s32 unk_F80;
    /* 0xF84 */ s32 unk_F84;
    /* 0xF88 */ s32 unk_F88;
    /* 0xF8C */ s32 unk_F8C;
    /* 0xF90 */ GsRecF90 unk_F90[32];
    /* 0x1090 */ u8 unk_1090;
    /* 0x1091 */ u8 unk_1091;
    /* 0x1092 */ u8 unk_1092;
    /* 0x1093 */ s8 unk_1093;           /* menu_check indexes +0x1093 by menu number (lb/sb) */
    /* 0x1094 */ s8 unk_1094;
    /* 0x1095 */ s8 unk_1095[0xD];
    /* 0x10A2 */ s8 unk_10A2;
    /* 0x10A3 */ u8 unk_10A3;
    /* 0x10A4 */ u8 unk_10A4;
    /* 0x10A5 */ u8 unk_10A5[0x43];    /* one byte per entry of the Rec24 table at D_801217D0 */
    /* 0x10E8 */ s32 unk_10E8;
    /* 0x10EC */ u8 unk_10EC[4];
    /* 0x10F0 */ s32 unk_10F0;
    /* 0x10F4 */ s32 unk_10F4;
    /* 0x10F8 */ u32 unk_10F8;
    /* 0x10FC */ s32 unk_10FC;
    /* 0x1100 */ s32 unk_1100;
    /* 0x1104 */ GsWord unk_1104;      /* the move/place selector (normal_date_move_place); BUNKA_SD reads it as u32 */
    /* 0x1108 */ u8 unk_1108;
    /* 0x1109 */ u8 unk_1109;
    /* 0x110A */ u8 unk_110A;
    /* 0x110B */ s8 unk_110B;
    /* 0x110C */ s8 unk_110C;
    /* 0x110D */ u8 unk_110D;
    /* 0x110E */ u8 unk_110E;
    /* 0x110F */ u8 unk_110F;
    /* 0x1110 */ u8 unk_1110;
    /* 0x1111 */ u8 unk_1111;
    /* 0x1112 */ u8 unk_1112;
    /* 0x1113 */ u8 unk_1113;
    /* 0x1114 */ u8 unk_1114;
    /* 0x1115 */ u8 unk_1115;
    /* 0x1116 */ u8 unk_1116;
    /* 0x1117 */ u8 unk_1117;
    /* 0x1118 */ u8 unk_1118;
    /* 0x1119 */ u8 unk_1119;
    /* 0x111A */ s8 unk_111A;
    /* 0x111B */ u8 unk_111B;
    /* 0x111C */ u8 unk_111C;
    /* 0x111D */ u8 unk_111D;
    /* 0x111E */ u8 unk_111E[2];
    /* 0x1120 */ s32 unk_1120;
    /* 0x1124 */ u8 unk_1124;
    /* 0x1125 */ u8 unk_1125[3];
    /* 0x1128 */ s32 unk_1128[64];     /* search_tpage: set from +0x1228 */
    /* 0x1228 */ s32 unk_1228[64];
    /* 0x1328 */ GsRec1328 unk_1328[32];
    /* 0x1428 */ u8 unk_1428;
    /* 0x1429 */ u8 unk_1429;
    /* 0x142A */ u8 unk_142A[2];
    /* 0x142C */ GsRec142C unk_142C[33];
    /* 0x163C */ s32 unk_163C[128];    /* OPTION fills it as words; +0x163C..+0x167C saved by memcpy */
    /* 0x183C */ u8 unk_183C[0x200];
    /* 0x1A3C */ s32 unk_1A3C;         /* +0x1A3C..+0x1A84: addresses set by NAME_ENT */
    /* 0x1A40 */ u8 unk_1A40[0xC];
    /* 0x1A4C */ s32 unk_1A4C;
    /* 0x1A50 */ u8 unk_1A50[0xC];
    /* 0x1A5C */ s32 unk_1A5C;
    /* 0x1A60 */ u8 unk_1A60[0xC];
    /* 0x1A6C */ s32 unk_1A6C;
    /* 0x1A70 */ u8 unk_1A70[0xC];
    /* 0x1A7C */ s32 unk_1A7C;
    /* 0x1A80 */ u8 unk_1A80[4];
    /* 0x1A84 */ s32 unk_1A84;
    /* 0x1A88 */ u8 unk_1A88[8];
} GameState; /* size 0x1A90 */

/* ---- globals ---- */
extern s32 D_8007E7D0[];
extern s32 D_8007E810[];
extern s32 D_8007E850[];
extern s8 D_8008093C;
extern u16 D_80094714;
extern u16 D_80094718;
extern u8 *D_8009471C;
extern u8 *D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s16 D_80094730;
extern s16 D_80094734;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_80094764[];
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_800AFDF0[];
extern u8 D_800AFF6C[];
extern u8 D_800AFF78[];
extern u8 D_800B00FC[];
extern u8 D_800B0194[];
extern u8 D_800B0860[];
extern u8 D_800B0896;
extern u32 D_800B0940;
extern s16 D_800B094E;
extern s16 D_800B0956;
extern s16 D_800B095E;
extern s16 D_800B0966;
extern Rec34 D_800B0A04[]; /* 12 records */
extern u8 D_800B0B64[];
extern u8 D_800B0B6C[];
extern u8 D_800B0BCF;
extern s8 D_800B0E31;
extern u8 D_800B0E42;
extern u8 D_800B0E43;
extern u8 D_800B0E6A;
extern u8 D_800B0F2D;
extern u8 D_800B0F2F;
extern u8 D_800B1746;
extern u32 D_800B1AE4;
extern s32 D_800B1AF0;
extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern u8 D_800B1AF9;
extern s32 D_800B1C1C;
extern s32 D_800B1C70;
extern u8 D_800B3220;
extern u8 *D_800B3288[];
extern u8 *D_800B35F4[];
#ifndef MAIN_API_OVERRIDE_D_800B3688
extern u8 *D_800B3624;
extern u8 *D_800B3630;
extern s32 D_800B3688[];
#endif
extern s32 D_800B36AC;
#ifndef MAIN_API_OVERRIDE_D_800B36C8
extern s32 D_800B36C8[];
#endif
extern s32 D_800B36EC;
#ifndef MAIN_API_OVERRIDE_D_800B3708
extern s32 D_800B3708[];
#endif
extern s32 D_800B372C;
extern void *D_800B374C; /* image data passed to LoadSquare (func_800673B8) */
extern u8 D_800B3C6C;
extern u8 D_800B3C70[];
extern u8 D_800B3C88[];
extern s8 D_800B3CA0;
extern u8 D_800B3D24;
extern u8 D_800B3D40;
extern u8 D_800B3D44;
extern u8 D_800B3D48;
extern u8 D_800B3D54[];
extern u8 D_800B3D60;
extern s32 D_800B3D68;
extern s32 D_800B3D6C;
extern s32 D_800B3D70;  /* only sw and &D_800B3D70 seen */
extern u8 D_800B3D80;
extern s8 D_800B3DB0;
extern u8 D_800B3DC0[];
extern u8 D_800B3DC7[];  /* stride 8 from k_disp_switch: likely a field of an 8-byte struct array */
extern u8 D_800B3F58[];
extern s16 D_800B3F60;  /* lh/sh; also lbu elsewhere */
extern s16 D_800B3F62;
extern s16 D_800B3F64;
extern u8 D_800B3F66;
extern s16 D_800B3F68;
extern u8 D_800B3F6A;
extern u8 D_800B41A0[];
extern u8 D_800B4341[]; /* table read by func_80052E60 */
extern s32 D_800B58E4;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern s32 D_800B5900;
extern s32 D_800B5920;
extern s32 D_800B5924;
extern s32 D_800B5928;
extern s32 D_800B592C;
#ifndef MAIN_API_OVERRIDE_D_800B5938
extern u8 D_800B5938[];  /* flags; D_800B593C and D_800B5940 are also declared as scalars */
#endif
extern u8 D_800B5939;
extern u8 D_800B593C;
extern u8 D_800B5940;
extern s32 D_800B5944;
extern s32 D_800B5948;
extern s32 D_800B594C;
extern s32 D_800B5950[];
extern s32 D_800B5960[];
extern u8 D_800B5A60;
extern u8 D_800B5A64;
extern u8 D_800B5BC8;
extern s8 D_800B5BD4;
extern s32 D_800B5BD8[];
extern s32 D_800B5BE8[];
extern s32 D_800B5BF8[];
extern s16 D_800B5C08;
extern u8 D_800B6724[];
extern u8 D_800B6728[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern u8 D_800B6D30;
extern u8 D_800B6D34;
extern u8 D_800B6D38;
extern s32 D_800B6D3C;
extern s32 D_800B6D40;
extern s8 D_800C51C4;
extern s32 D_800C5FCC;
extern s32 D_800C5FD0;
extern u8 D_800C9730[];
extern s32 D_800C9734;
extern s32 D_800C9738;
extern s32 D_800C973C;
extern s32 D_800C974C;
extern s32 D_800C9750;
extern s32 D_800C9754;
extern s32 D_800C9758;
extern u8 D_800C975C[];
extern u8 D_800C9A14[];
extern s16 D_800C9A54;
extern u8 D_800C9A60[];
extern u8 D_800C9D60[];
extern u16 D_800C9F60[];
extern u16 D_800C9FF8[];
extern u16 D_800CA048[];
extern u16 D_800CA108[];
extern s32 D_800CA120;
extern s32 D_800CA124;
extern s32 D_800CA128;
extern s32 D_800CA130;
extern u8 *D_800CA134;
extern u8 *D_800CA138;
extern s32 D_800CA13C;
extern s32 D_800CA140;
extern s32 D_800CA144;
extern s16 D_800CA148;  /* also address-taken by the date code */
extern s16 D_800CA14C;
extern s16 D_800CA150;
extern s16 D_800CA154;
extern u8 D_800CA158;
extern u8 D_800CA15C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern u8 D_800CA16C[];
extern u8 D_800CA174[];
extern u8 D_800CA17C[];
extern u8 D_800CA188[];
extern u8 D_800CA190[];
extern u8 D_800CA19C[];
extern u8 D_800CA1DC[];
extern s16 D_800CA21C;
extern s16 D_800CA21E;
extern s16 D_800CA220;
extern s16 D_800CA224;
extern s16 D_800CA226;
extern s16 D_800CA228;
extern s16 D_800CA22C;
extern s16 D_800CA22E;
extern s16 D_800CA230;
extern s16 D_800CA234;
extern s16 D_800CA236;
extern s16 D_800CA238;
extern u8 D_800CA23C[];
extern u8 D_800CA25C[];
extern u8 D_800CA2A4;
extern u8 D_800CA2A5;
extern u8 D_800CA2A6;
extern u8 D_800CA2A8;
extern s16 D_800CA2AC;
extern s16 D_800CA2C0;
extern u8 D_800CA2C4;
extern u8 D_800CA2CC;
extern s32 D_800CA2D0;
extern s32 D_800CA2D4;
extern s32 D_800CA2D8;
extern s16 D_800CA2DC;
extern s16 D_800CA2E0;
extern s16 D_800CA2E4;
extern s16 D_800CA2E8;
extern u8 D_800CA2EC;
extern s8 D_800CA2F0;
extern u8 D_800CA2F4;
extern u8 D_800CA2F8;
extern u8 D_800CA2FC;
extern s8 D_800CA360;
extern s8 D_800CA364;
extern s8 D_800CA368;
extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;
extern s16 D_800D9248;
extern s16 D_800D924C;
extern s16 D_800D9250;
extern s16 D_800D9254;
extern s32 D_800D9258;
extern s32 D_800D925C;
extern s32 D_800D9260;
extern u8 D_800D9280;
extern u8 D_800D9288;
extern s32 D_800D92A0;
extern s32 D_800D92E0;
extern u8 D_800D9328[];
extern u8 D_800D9388[];
extern s32 D_800DE500[];
extern s8 *D_800E36C0[];
extern s32 D_800E36C8[];
extern s32 D_800E36D0[];
extern s32 D_800E36D8;
extern s32 D_800E36DC;
extern s32 D_800E36E0;
extern s32 D_800E36E4;
extern u16 D_800E36E8;
extern u16 D_800E36EA;
extern s32 D_800E36F0;
extern s32 D_800E36F4;
extern GameState D_800E6280;
extern s32 D_800E7D10;
extern u8 D_800E7D11[];
extern u8 D_800E7D14[];
extern u8 D_800E7D1F;
extern u8 D_800E7D34;
extern u16 D_800E7D3C;
extern u16 D_800E7D3E;
extern u16 D_800E7D40;
extern u8 D_800E7D42;
extern u8 D_800E7D43;
extern u8 D_800E7D44;
extern u8 D_800E7D45;
extern u8 D_800E7D46;
extern u8 D_800E7D47;
extern u8 D_800E7D48;
extern u8 D_800E7D49;
extern u8 D_800E7D4A;
extern u8 D_800E7D4B;
extern s32 D_800E7D4C;
extern s32 D_800E7D50;
extern u8 D_800E7D68;
#ifndef MAIN_API_OVERRIDE_D_800E8BEE
extern s8 D_800E8BEE;
#endif
extern u8 D_800E8BF0[];
extern u8 D_800E8C30[];
extern s32 D_800E8C70;
extern u8 *D_800E8C74;
extern s32 D_800E8C84;
extern u8 *D_800E8C88;
extern u8 D_800E8CA0[];
extern u8 D_800E90A0[];
extern u8 D_800E9620[];
extern u8 D_800E96AB;
extern s8 D_800E96EF;
extern s8 D_800E9733;
extern u8 D_800E9E63;
extern u8 D_800EAFA0[];
extern s8 D_800EAFA2;
extern u8 D_800EAFA7;
extern s16 D_800EAFB6;
extern s32 D_800EAFD8;
extern u8 D_800EAFEB;
extern s8 D_800EB029;
extern u8 D_800EB02A;
extern u8 D_800EB02B;
extern s8 D_800EB02C;
extern s8 D_800EB02D;
extern s8 D_800EB02E;
extern u8 D_800EB02F;
extern s16 D_800EB030;
extern s32 D_800EB034;
extern s32 D_800EB038;
extern s16 D_800EB03C;
extern s16 D_800EB03E;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;
extern s32 D_800EB05C;
extern s32 D_800EB060;
extern s8 D_800EB06B;
extern s8 D_800EB06D;
extern s8 D_800EB06E;
extern u8 D_800EB06F;
extern s8 D_800EB070;
extern s8 D_800EB071;
extern s8 D_800EB072;
extern u8 D_800EB073;
extern s32 D_800EB078;
extern s32 D_800EB07C;
extern s16 D_800EB080;
extern s16 D_800EB082;
extern s16 D_800EB084;
extern s16 D_800EB092;
extern s16 D_800EB096;
extern s32 D_800EB0A0;
extern s32 D_800EB0A4;
extern s8 D_800EB0AF;
extern u8 D_800EC190[];
#ifndef MAIN_API_OVERRIDE_D_800EECB0
extern s32 D_800EECB0;
#endif
extern s32 D_800EECB4;
extern s32 D_800EECBC;
extern s32 D_800EECC0;
extern s32 D_800EECC8;
extern s32 D_800EECCC;
extern s32 D_800EECD0;
extern s32 D_800EECE0;
extern s32 D_800EECE4;
extern s32 D_800EECE8;
extern s32 D_800EECEC;
extern u8 D_800F19AB;
extern u8 D_800F53DA;
extern u8 D_800F53DE;
extern u32 D_800F5488;
extern s16 D_800F5496;
extern s16 D_800F549A;
extern s16 D_800F54A6;
extern s16 D_800F54AE;
extern s16 D_800F54B2;
extern u8 D_800F55CA;
extern u32 D_800F5638;
extern u8 D_800F563A;
extern u8 D_800F5833;
extern u8 D_800F594E;
extern u8 D_800F594F;
extern u8 D_800F5A2D[];
extern u8 D_800F5AAC;
extern u8 D_800F5AAE;
extern u8 D_800F5AB1;
extern u8 D_800F5AB2;
extern u8 D_800F5ACD;
extern u8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s32 D_800F6468;
extern s32 D_800F6474;
extern u8 D_800F6479;
extern u8 D_800F647A;
extern s32 D_800F65C0;
extern s32 D_800F6600;
extern s32 D_800F6680;
extern s32 D_8011ECA0;
extern s8 D_8011ECA4;
extern s32 D_8011ECA8;
extern s32 D_8011ECAC;
extern s32 D_8011ECB0;
extern s32 D_8011ECB4;
extern s32 D_8011ECB8;
extern s32 D_8011ECBC;
extern s32 D_8011ECC0;
extern s32 D_8011ECC4;
extern s32 D_8011ECC8;
extern u8 D_8011ECD0[]; /* 3 records of 0x44 bytes; s16 at +0x19EA is read */
extern u8 D_8011ECD3;
extern s16 D_8011ECE8;
extern s16 D_8011ECF6;
extern s16 D_8011ECFA;
extern u8 D_8011ED15;
extern u8 D_8011ED16;
extern u8 D_8011ED17;
extern u8 D_8011ED18;
extern u8 D_8011ED19;
extern u8 D_8011ED1B;
extern u8 *D_8011ED20;
extern u8 *D_8011ED24;
extern s16 D_8011ED28;
extern s16 D_8011ED2C;
extern s16 D_8011ED3A;
extern s16 D_8011ED3E;
extern u8 *D_8011ED48;
extern s32 D_8011ED4C;
extern u8 D_8011ED57;
extern u8 D_8011ED59;
extern u8 D_8011ED5A;
extern u8 D_8011ED5B;
extern u8 D_8011ED5C;
extern u8 D_8011ED5D;
extern u8 *D_8011ED64;
extern u8 *D_8011ED68;
extern s16 D_8011ED70;
extern s16 D_8011ED7E;
extern s16 D_8011ED82;
extern u8 *D_8011ED8C;
extern s32 D_8011ED90;
extern s8 D_8011ED9F;
extern s8 D_8011EDE3;
extern u8 D_8011F0CD;
extern u8 D_8011F0CE;
extern u8 D_8011F0CF;
extern u8 D_8011F0D0;
extern u8 D_8011F0D1;
extern u8 *D_8011F0D8;
extern u8 *D_8011F0DC;
extern s16 D_8011F0E0;
extern s16 D_8011F0E2;
extern s16 D_8011F0E4;
extern s16 D_8011F0F2;
extern s16 D_8011F0F6;
extern u8 *D_8011F100;
extern s32 D_8011F104;
extern u8 D_8011F10F;
extern u8 D_8011F113;
extern u8 D_8011F3FF[];
extern u8 D_8011F4CA;
extern u8 D_8011F4CB;
extern u8 D_8011F4CF;
extern s16 D_8011F4D0;
extern s16 D_8011F4DE;
extern s16 D_8011F4E0;
extern s16 D_8011F4EE;
extern s16 D_8011F4F2;
extern u8 D_8011F50E;
extern u8 D_8011F50F;
extern u8 D_8011F513;
extern s16 D_8011F524;
extern s16 D_8011F532;
extern s16 D_8011F536;
extern u8 D_8011F553;
extern s16 D_8011FDFA;
extern s16 D_8011FE3E;
extern s16 D_8011FE82;
extern s16 D_8011FEC6;
extern s16 D_8011FF0A;
extern s16 D_8011FF4E;
extern s16 D_8011FF92;
extern s16 D_8011FFD6;
extern s16 D_8012001A;
extern s16 D_8012005E;
extern s16 D_801200A2;
extern s16 D_801200E6;
extern s16 D_8012012A;
extern s16 D_8012016E;
extern s16 D_801201B2;
extern s16 D_801201F6;
extern s16 D_8012023A;
extern s16 D_8012027E;
extern s16 D_801202C2;
extern s16 D_80120306;
extern s16 D_8012034A;
extern s16 D_8012038E;
extern s16 D_801203D2;
extern s16 D_80120416;
extern s16 D_8012045A;
extern s16 D_8012049E;
extern s16 D_801204E2;
extern s16 D_80120526;
extern s16 D_8012056A;
extern s16 D_8012059A[];
extern s16 D_801205AE;
extern s16 D_801205F2;
extern s16 D_80120636;
#ifndef MAIN_API_OVERRIDE_D_80120650
extern u8 D_80120650[];
#endif
extern u8 D_80120651;
extern u8 D_80120652;
extern u8 D_80120653;
extern u8 D_80120654;
extern u8 D_80120655;
extern s8 D_80120656;
extern u8 D_80120657;
#ifndef MAIN_API_OVERRIDE_D_80120658
extern s16 D_80120658;
#endif
extern u8 *D_8012065C;
extern u8 *D_80120660;
extern s16 D_80120664;
extern s16 D_80120666;
extern s16 D_80120668;
extern s16 D_8012066A;
#ifndef MAIN_API_OVERRIDE_D_8012066C
extern s16 D_8012066C;
#endif
extern s16 D_80120676;
extern s16 D_8012067A;
extern u8 *D_80120684;
extern s32 D_80120688;
extern s8 D_80120693;
extern u8 D_80120695;
extern u8 D_80120696;
extern u8 D_80120697;
#ifndef MAIN_API_OVERRIDE_D_80120698
extern u8 D_80120698;
#endif
extern u8 D_80120699;
extern u8 D_8012069A;
extern u8 D_8012069B;
extern s32 D_801206A0;
extern s32 D_801206A4;
#ifndef MAIN_API_OVERRIDE_D_801206A8
extern s16 D_801206A8;
#endif
extern s16 D_801206AA;
#ifndef MAIN_API_OVERRIDE_D_801206AC
extern s16 D_801206AC;
#endif
#ifndef MAIN_API_OVERRIDE_D_801206B0
extern volatile s16 D_801206B0;
#endif
extern s16 D_801206BA;
extern s16 D_801206BE;
#ifndef MAIN_API_OVERRIDE_D_801206C8
extern s32 D_801206C8;
#endif
extern s32 D_801206CC;
extern u8 D_801206D7;
/* OPTION walks byte 3 of 0x44-byte records from here (the loop base register is this address,
 * not one of D_8011ECD0; T-7020). */
#ifndef MAIN_API_OVERRIDE_D_801206D8
extern u8 D_801206D8[];
#endif
extern u8 D_801206D9;
#ifndef MAIN_API_OVERRIDE_D_801206DA
extern s8 D_801206DA;
#endif
extern u8 D_801206DB;
#ifndef MAIN_API_OVERRIDE_D_801206DC
extern s8 D_801206DC;
#endif
extern u8 D_801206DD;
extern s8 D_801206DE;
extern u8 D_801206DF;
#ifndef MAIN_API_OVERRIDE_D_801206E0
extern s16 D_801206E0;
#endif
extern s32 D_801206E4;
extern s32 D_801206E8;
#ifndef MAIN_API_OVERRIDE_D_801206EC
extern s16 D_801206EC;
#endif
extern s16 D_801206EE;
#ifndef MAIN_API_OVERRIDE_D_801206F0
extern s16 D_801206F0;
#endif
extern s16 D_801206F2;
extern s16 D_801206F4;
extern s16 D_801206FE;
extern s16 D_80120702;
extern s32 D_8012070C;
extern s32 D_80120710;
extern s8 D_8012071B;
extern s8 D_8012071D;
extern u8 D_8012071E;
extern u8 D_8012071F;
#ifndef MAIN_API_OVERRIDE_D_80120720
extern s8 D_80120720;
#endif
#ifndef MAIN_API_OVERRIDE_D_80120721
extern s8 D_80120721;
#endif
extern s8 D_80120722;
extern u8 D_80120723;
#ifndef MAIN_API_OVERRIDE_D_80120724
extern s16 D_80120724;
#endif
extern s32 D_80120728;
extern s32 D_8012072C;
#ifndef MAIN_API_OVERRIDE_D_80120730
extern s16 D_80120730;
#endif
extern s16 D_80120732;
#ifndef MAIN_API_OVERRIDE_D_80120734
extern s16 D_80120734;
#endif
extern s16 D_80120742;
extern s16 D_80120746;
extern s32 D_80120750;
extern s32 D_80120754;
extern s8 D_8012075F;
extern u8 D_80120761;
extern u8 D_80120762;
extern u8 D_80120763;
extern u8 D_80120765;
extern u8 D_80120767;
extern s32 D_8012076C;
extern s32 D_80120770;
extern s16 D_80120776;
extern s16 D_80120778;
extern s16 D_80120786;
extern s16 D_8012078A;
extern s32 D_80120794;
extern s32 D_80120798;
extern u8 D_801207A5;
extern u8 D_801207A6;
extern u8 D_801207A7;
extern u8 D_801207A9;
extern s32 D_801207B0;
extern s32 D_801207B4;
extern s16 D_801207BA;
extern s16 D_801207CA;
extern s16 D_801207CE;
extern s32 D_801207D8;
extern s32 D_801207DC;
extern u8 D_801207E9;
extern u8 D_801207EA;
extern u8 D_801207EB;
extern u8 D_801207ED;
extern s32 D_801207F4;
extern s32 D_801207F8;
extern s16 D_801207FE;
extern s16 D_8012080E;
extern s16 D_80120812;
extern s32 D_8012081C;
extern s32 D_80120820;
extern u8 D_8012082D;
extern u8 D_8012082E;
extern u8 D_8012082F;
extern u8 D_80120831;
extern s32 D_80120838;
extern s32 D_8012083C;
extern s16 D_80120844;
extern s16 D_80120852;
extern s16 D_80120856;
extern s32 D_80120860;
extern s32 D_80120864;
extern s16 D_8012089A;
extern s16 D_801208DE;
extern s16 D_80120922;
extern s16 D_80120966;
extern s16 D_801209AA;
extern s16 D_801209EE;
extern s16 D_80120A32;
extern s16 D_80120A76;
extern s16 D_80120AB6;
extern s16 D_80120ABA;
extern s16 D_80120AFA;
extern s16 D_80120AFE;
extern s16 D_80120B3E;
extern s16 D_80120B42;
extern s16 D_80120B82;
extern s16 D_80120B86;
extern u8 D_80120BA3;
extern s16 D_80120BC6;
extern s16 D_80120BCA;
extern s8 D_80120BE3;
extern s16 D_80120C0A;
extern s16 D_80120C0E;
extern s16 D_80120C4E;
extern s16 D_80120C52;
extern u8 D_80120C6F;
extern s16 D_80120C92;
extern s16 D_80120C96;
extern s16 D_80120CD6;
extern s16 D_80120CDA;
extern s16 D_80120D1A;
extern s16 D_80120D1E;
extern s16 D_80120D5E;
extern s16 D_80120D62;
extern s16 D_80120DA2;
extern s16 D_80120DA6;
extern s8 D_80120DC1;
extern s8 D_80120E05;
extern u8 D_80120E07;
extern s16 D_80120EE6;
extern s16 D_80120EF6;
extern s16 D_80120EFA;
extern s16 D_80120F2A;
extern s16 D_80120F3A;
extern s16 D_80120F3E;
extern u8 D_8012117A;
extern u8 D_801211BE;
extern u8 D_80121202;
extern s32 D_80121238;
extern u8 D_80121246;
extern s32 D_8012127C;
extern s8 D_801212CB;
extern u8 D_80121313;
extern s8 D_80121317;
extern s16 D_80121328;
extern s8 D_80121353;
extern u8 D_80121357;
extern s16 D_8012136C;
extern s8 D_80121399;
extern u8 D_8012139B;
extern s16 D_801213B0;
extern s8 D_801213DD;
extern u8 D_801213DF;
extern s16 D_801213F4;
extern s8 D_80121421;
extern u8 D_80121422;
extern u8 D_80121423;
extern u8 D_80121426;
extern s16 D_80121438;
extern s8 D_8012146B;
extern s8 D_801214AF;
extern s8 D_801214F3;
extern u8 D_80121531;
extern u8 D_80121533;
extern s16 D_80121548;
extern s8 D_801215FD;
extern s8 D_80121603;
extern s8 D_80121647;
extern s8 D_80121685;
extern s8 D_8012168B;
extern s16 D_80121726;
extern s16 D_80121728;
extern u8 D_80121750[];
extern u8 D_801217A0[];
extern Rec24 D_801217D0[]; /* at least 12 records */
extern s16 D_801217F8;
extern s32 D_80121818; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s32 D_8012183C; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s32 D_80121860; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_80121864; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
#ifndef MAIN_API_OVERRIDE_D_80121874
extern u8 D_80121874;
#endif
extern s32 D_80121884; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s32 D_80121FD4;
extern s16 D_80121FD8;
extern s16 D_80121FDA;
extern s16 D_80121FDC;
extern s16 D_80121FDE;
extern s16 D_80121FE0;
extern s8 D_80121FE2;
extern s8 D_80121FE3;
extern s16 D_80121FE4;
extern s16 D_80121FE6;
extern u8 D_80121FE8;
extern u8 D_80121FE9;
extern u8 D_80121FEA;
extern s16 D_80121FEC;
extern s16 D_80121FEE;
extern s16 D_80121FF0;
extern s16 D_80121FF2;
extern s32 D_80121FF4;
extern u32 D_801220D0;
extern s16 D_801220D4;
extern s16 D_801220D6;
extern u8 D_801220E0;
extern u8 D_801220E1;
extern u8 D_801220E2;
extern s16 D_801220E8;
extern s16 D_801220EA;
extern s16 D_801220EC;
extern s16 D_801220EE;
extern s32 D_801220F0;
extern s32 D_801220F4;
extern u32 D_80122640[];
extern s32 D_80122740;
extern s32 D_80122744;
extern s32 D_80122748;
extern s32 D_8012274C;
extern s32 D_80122750;
extern s32 D_80122754;
extern s32 D_80122758;
extern s32 D_8012275C;
extern s32 D_80122760;
extern s32 D_80122764;
extern s32 D_80122768;
extern u8 D_8012276C;
extern u8 D_8012276D;
extern u8 D_8012276E;
extern s32 D_80122770;
extern s32 D_80122774;
extern s32 D_80122778;
extern u8 D_8012277C;
extern u8 D_8012277D;
extern u8 D_8012277E;
extern s32 D_80122780;
extern s32 D_80122784;
extern s32 D_80122788;
extern u8 D_8012278C;
extern u8 D_8012278D;
extern u8 D_8012278E;
extern u8 D_801227A4[];
#ifndef MAIN_API_OVERRIDE_D_80122CD0
extern s32 D_80122CD0;
#endif
extern s32 D_80122CD4;
extern s32 D_80122CDC;
extern s32 D_80122CE0;
extern s32 D_80122CE4;
extern s32 D_80122CEC;
extern s32 D_80122CF0;
extern s32 D_80122CF4;
extern s32 D_80122CF8;
extern s32 D_80122CFC;
extern s32 D_80122D04;
extern s32 D_80122D08;
extern s32 D_80122D0C;
extern s32 D_80122D10;
extern s32 D_80122D20;
extern s32 D_80122D2C;
extern s32 D_80122D30;
extern s32 D_80122D38;
extern s32 D_80122D40;
extern s32 D_80122D44;
extern s32 D_80122D4C;
extern s32 D_80122EA0;
extern s32 D_80122EAC;
#ifndef MAIN_API_OVERRIDE_D_80122EB8
extern s32 D_80122EB8;
#endif
extern s32 D_80122EBC;
extern s32 D_80122EC0;
#ifndef MAIN_API_OVERRIDE_D_80122EC8
extern s32 D_80122EC8;
#endif
extern s32 D_80122ECC;
extern s32 D_801230D0;
extern s32 D_801230F4;
extern u32 D_80123110;
extern u8 D_80123120[];
extern s32 D_80125120;
extern s32 D_80125124;
extern u8 D_80125128;
extern u8 D_80125129;
extern u8 D_8012512A;
extern s8 D_8012512B;
extern u8 D_80125130[];
extern s32 D_801255B0;
extern u8 D_801255B4;
extern u8 D_801255B5;
extern u8 D_801255B8;
extern u8 D_801255C0[];
extern u8 D_801255C8[];
extern s32 D_801255D8;
extern u8 D_801255DC;
extern s16 D_801255E0;
extern s16 D_801255E2;
extern s32 D_80125C04;
extern s32 D_80125C08;
extern s32 D_80125C0C;
extern u8 D_80125C10[];
extern s16 D_80125C50;
extern s16 D_80125C52;
extern u8 D_80125C54;
extern u8 D_80125C55;
extern u8 *D_80125C58;
extern u8 D_80125C60[];
extern u8 D_80125C90[];
extern u16 D_80125CA0;
extern s32 D_80125CA4;
extern s32 D_80125CA8;
extern s32 D_80125CAC;
extern s32 D_80125CB0;
extern u16 D_80125CC0;
extern Work80125D10 D_80125D10;
extern s16 D_80125D38;
extern u16 D_80125D50;
extern s32 D_80125D60[];
extern s32 D_80125D70[];
extern s32 D_80125D80[];
extern u16 D_80125E60;
extern u16 D_80125E62;
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;
extern u8 D_80125E70[];
extern s16 D_80126080[]; /* two 0x200-entry s16 tables (+0, +0x400), SD_CalcCDAve */
extern s32 D_8012749C;
extern s32 D_801274A0;
extern u8 D_80129F40[];

/* ---- functions ---- */
void func_80010678();
void func_80012D2C();
void func_80012D40();
void func_80017E50();
void func_8001886C();
void func_80018944();
void func_8001FD50();
void func_8003535C();
void func_8003580C();
void func_80036884();
void func_8003C2C0();
void func_800410AC(void);
void func_8004111C(void);
void func_80041168(s32 arg0);
void func_80041584(void);
void func_800415B4(s32 arg0, s32 arg1);
void func_8004164C(s32 arg0, s32 arg1);
void func_80041878(void);
void func_800418B0(void);
void func_800419FC(void);
void func_80041C2C(void);
void func_80041F48(void);
void func_80042058(void);
void func_80042134();
void func_80042458(void);
void func_80042488(void);
void func_80042808(void);
void func_8004284C(void);
void func_80042878();
void func_80042908(s32 arg0);
void func_80042940(s32 arg0);
void func_80042960(void);
s32 func_80042AC8();
void func_80042C30(void);
void func_80042CB0();
s32 func_80043010(void);
void func_800430C0();
void func_800433D0(s32 arg0);
void draw2d3d(u8 arg0, u8 arg1);
void func_800438DC();
void back_clear_switch(s32 arg0);
void func_800438F0();
void func_80043914(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void load_palette(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void load_csr_tp(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4);
void func_8004435C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4);
s32 func_80044750(s32 arg0);
s32 func_80044774(s32 arg0);
u8 func_8004480C(void);
void func_8004482C(void);
void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 func_80044C98(void);
s32 func_80044E8C(void);
s32 func_8004500C();
void func_800450F4(s32 arg0, s32 arg1);
s32 func_800451D0();
void func_800452C4(void);
void func_80045414(s32 arg0, s32 arg1, u8 *arg2);
void func_80045B14(void);
s32 func_80045FF4(void);
#ifndef MAIN_API_OVERRIDE_func_80046094
u8 func_80046094(void);
#endif
u8 func_800460CC(void);
u8 func_800460DC(void);
u8 func_800460EC(void);
s32 func_80046274(void);
void func_80046290();
void func_800462BC(u8 arg0, s32 arg1, s32 *arg2);
#ifndef MAIN_API_OVERRIDE_func_800462C8
void func_800462C8(u8 arg0, s32 arg1, s32 arg2);
#endif
void func_80046318(u8 arg0, s32 arg1, s32 arg2);
s32 func_8004636C();
s32 func_80046500(void);
void func_800469F4();
void func_80046A7C();
void func_80046B88();
void func_80047550(void);
void func_80047560();
void tpage_buf_clear_all(void);
void func_80048390();
void tpage_buf_clear(void);
void _sys_default_tpage_set(void);
void load_tpage_buf_lock();
void func_80048DAC(s32 arg0);
void func_80048DD0(s32 arg0);
void func_80048E78(void);
void func_80048EB8(s32 arg0);
void func_80048F64(s32 arg0);
s32 GetWorkBase(s32 arg0, s32 arg1);
void *func_800490F0();
void safe_env(s32 arg0);
void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800494BC();
void dtd_on(s32 arg0);
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80049A40(s16 a, s16 b, s16 c, s16 d, s32 e, s32 f, s32 g);
s32 func_80049B20();
void func_8004A1A0();
void func_8004A2C4();
void func_8004A4D8();
void func_8004A734(void);
void func_8004A764();
void func_8004A7C4();
void func_8004A7F4();
#ifndef MAIN_API_OVERRIDE_func_8004A8EC
void func_8004A8EC();
#endif
void func_8004AC18(s32 arg0);
void func_8004ACA8();
void func_8004ACC8(s32 arg0);
void func_8004ADE4(void);
void func_8004AE28(s32 a0, s32 a1, s32 a2);
void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3);
void func_8004AEB0();
void func_8004B19C(s32 a, s32 b, s32 c);
void func_8004B338(s32 arg0, s32 arg1, s32 arg2);
void func_8004B358(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004BB54();
s32 func_8004BC20(s32 arg0);
void func_8004C060();
void func_8004C250();
void func_8004C360();
void func_8004C46C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8004C670();
void func_8004C6A0();
void func_8004C6B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004C808();
void func_8004C984();
void func_8004CA18();
void func_8004CA48();
void func_8004CDDC();
void func_8004CF30();
void srn_vram_set(s32, s32, s32, s32);
void func_8004D320();
void func_8004DDD8();
void func_8004DE1C(void);
void func_8004DEAC();
void func_8004DEE4();
void srn_init(s32, u32 *, s32);
void func_8004E44C(s32 arg0, u32 *arg1, s32 arg2);
void func_8004E500(s32 arg0);
void func_8004E58C(void);
s32 func_8004E788(s16 x, s16 y, s32 c, s32 d, s32 e); /* returns a value: SHUGAKU 80138A60 passes it on */
#ifndef MAIN_API_OVERRIDE_set_kanji_string
s32 set_kanji_string(s16 x, s16 y, u8 col, u8 *str, s32 arg4);
#endif
void func_8004E884();
void k_disp_start(s32 arg0);
void k_disp_switch(s32 arg0, s32 arg1);
s32 func_8004E970();
void func_8004E9F4();
void k_reset(s32 arg0);
void func_8004EA98();
void k_sub_reset_point_set(void);
void func_8004EAAC();
void k_sub_reset(void);
void func_8004EAD4();
void k_sub_disp_start(s32 arg0);
s16 k_disp_goto_line_end(void);
void k_speed_set(u8 arg0);
u8 get_k_speed(void);
s32 k_disp_inc(void);
s32 check_end_k(void);
s32 func_8004ECB4();
s32 func_8004ECF0();
void func_8004EDE0();
void func_8004EDF4();
void func_8004EE18();
void func_8004F860();
void func_8004F870();
void func_8004F984();
void menu_check();
void func_8004FC10();
void func_800504CC();
s32 func_80050AB8();
void func_80050B54();
void menu_bar_show();
void func_80050C24(s32 arg0, s32 arg1);
void x_taku_menu_set(s32 arg0, s32 arg1);
void func_80050D60();
void x_taku_string_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80050DFC(u8 *arg0);
void gnsx(u8 *arg0);
void func_80050E8C(u8 *arg0, s32 arg1, s32 arg2);
void sndisp();
void func_80051010();
void sndi(u8 *arg0, s32 arg1, s32 arg2);
s32 get_g_name(u8 *arg0, s32 arg1);
void func_80051508(u8 *arg0, s32 arg1);
void get_p_name();
s32 func_80051A68();
s32 get_g_zyotai_s(s32 arg0);
u32 func_80051B48();
s32 get_g_zyotai_h(s32 arg0);
void func_80051DBC();
void xa_wait(void);
void func_80051DD8();
void func_80052000();
void func_80052060();
u32 get_h_tokimeki(s32 arg0);
u32 get_h_yuukou(s32 arg0);
s32 birth_day_check_days(s32 arg0, s32 arg1, s32 arg2);
void func_800530E0();
s32 func_8005352C();
void func_80053650(s32 arg0, s32 arg1, s32 arg2);
s32 func_800536AC();
void card_ev_set(void);
void Sw_Start(void);
void Hw_Start(void);
u8 Sw_Test(void);
void Sw_Clear(void);
void Hw_Clear(void);
void func_80053CC0(void);
void func_80053CE0(void);
void func_80053D10(void);
void func_80053EFC();
s32 func_80054388();
u8 func_8005448C(void);
void func_80054864();
void func_8005493C();
void func_800549E8();
s32 func_80054AF4(s32 arg0);
s32 func_80055A38(s32);
s32 func_80055AFC(s32 arg0);
void func_80056070(u8 *buf, s32 arg1);
void func_80056284(void);
void set_movie_offset(s16 arg0, s16 arg1);
void strKickCD(s32 arg0);
void func_80056BA8();
void func_80057390();
void set_dec_bri(u8 arg0);
s32 get_last_gamen_mode(void);
void dec_bg_reset(void);
void func_800573AC();
void dec_bg_show_switch(s32 arg0);
void dec_bg_show_set(s32 arg0, s32 arg1);
void func_80057418(s32, s32);
s32 dec_bg_cd_read(s32 arg0, s32 arg1);
s32 func_8005742C();
s32 func_8005751C();
void func_80057640();
void func_80057710();
void func_800578F4(s32 a);
void func_80057D28(s32);
void func_80058398();
void func_80058D20();
void initView(void);
void func_80058EB0();
void func_80058F0C(s32 arg0);
void initModelingData_init(s32 arg0);
void func_80059048();
void func_800590CC();
void func_800591D8(s32 arg0);
void func_80059688(s32 arg0);
void SenseMouse(u16 arg0, u16 arg1);
void SetMouse(s32 arg0, u32 arg1, u32 arg2);
void func_80059938(void);
void func_80059B04();
void func_80059B40(void);
void func_80059BC0(void);
void func_80059BE8(void);
void func_80059E00();
void func_8005A37C(void);
void func_8005A3E8(void);
void func_8005A668();
void func_8005ABC0();
void func_8005ABD0(void);
void func_8005AC70(s32 arg0);
void func_8005AD70(void);
void func_8005AEFC();
s32 func_8005AF28(void);
s32 func_8005AFC8(void);
s32 func_8005B040();
void func_8005B06C();
s32 func_8005B0F4();
void func_8005B1A8();
void func_8005B264();
s8 func_8005B28C();
s32 func_8005B2BC();
s32 func_8005B32C();
u32 func_8005B368();
void func_8005B39C(void);
s32 func_8005B43C();
void func_8005B830(void);
void func_8005B8A0(void);
void func_8005B8E0(void);
void func_8005C4CC(s32 arg0);
void func_8005D174(void);
void func_8005D1B0();
s32 func_8005E0E0();
void join_club_init(void);
void join_club_select(void);
void join_club_message(void);
void join_club_exit(void);
s32 func_80060B24();
void func_80060B78(void);
void uwasa_exit0(void);
s32 uwasa0();
void pre_xmas_init(void);
s32 func_80060EA0();
s32 pre_syogatu_init();
s32 func_80061634();
void func_80061710(void);
void func_80061790(void);
void func_800618B0();
void func_80061EFC(void);
void func_8006211C(void);
void func_8006218C(void);
void func_80062210(void);
void func_800623E4(void);
void func_80062524(void);
void func_800625C0(void);
void func_800626B0(void);
void func_80062764(void);
void func_800627DC(void);
void func_80062CD0(); /* unprototyped: main callers pass no argument, overlays pass one */
void func_80062DBC();
void func_800634FC(); /* unprototyped: callers pass an argument, the definition ignores it */
void func_80063668();
void func_80063930(s32);
void parameter_disp_switch();
void parameter_show_init(void);
void parameter_show(void);
void func_8006492C(s32 arg0);
void hizuke_disp_switch(s32 arg0);
void func_800649D4();
void hizuke_init(void);
void func_80064DEC();
void hizuke_show(void);
void func_80064E48();
void message_disp_switch(s32 arg0);
void func_80064E84();
void message_window_init(void);
void func_80064F48();
void message_window_show(void);
void func_80064FA4(s32);
void icon_disp_switch(s32 a);
void icon_can_use_set(s32 idx, s32 on);
void func_8006509C(void);
void func_80065900(u8 arg0);
void func_80065B0C(s32 a);
void func_80065F34(s32 arg0);
void func_80066104(void);
void func_8006612C();  /* callers pass one pointer, or nothing */
void func_80066334();
s32 func_80066A2C(void);
s32 func_80066A84(void);
void func_80066C08(s32 a);
void func_80067438(void);
void func_800674B0(void);
void func_8006764C(s32 arg0);
void func_800676AC();
void func_80067870(void);
void func_80067DD4(void);
void func_80067E34(void);
void func_80067F04(void);
s16 func_80068898(s32 arg0, s32 arg1);
s16 func_800688F0(s32 arg0, s32 arg1);
void func_80068938();
void func_80068EC0();
void cal_base_show(void);
void func_80069128();
void cal_sprite_init(void);
void func_800696DC(s32, s32);
void func_8006A044();
void func_8006A2CC();
void func_8006B0C8();
void func_8006B648(void);
void func_8006B900(void);
void func_8006BA40(void);
void func_8006BC28(s32 arg0);
void func_8006BD6C(s32 arg0);
void func_8006C334(s32 a, s32 b, s32 c, s32 d);
s32 func_8006C700(void);
void func_8006C848(s32 arg0);
u8 *func_8006CA9C(void);
u8 *func_8006CAE0(void);
void _schedule_init(void);
void last_date_spot_timer_dec(void);
void restore_bgm(void);
s32 func_8006CDD4(void);
void func_8006D038(void);
void func_8006D138(void);
void func_8006D6E0();
void yuukou_down(void);
void syoushin_up(void);
void func_80070F80(void);
void func_80070FD0(void);
void func_8007132C(void);
void func_80072338(void);
void func_800726F0(void);
void func_80072734();
s32 func_80072944(void);
void func_80072998(void);
void func_80072B20(void);
s32 func_80072B5C(s32 arg0);
void func_80072C68(void);
void func_80072CA0(void);
void func_800737A0(void);
u8 week_day_init(void);
s32 get_weekly_bg_sector(void);
u8 func_80073AD8(void);
u8 func_800741B8(void);
u8 func_8007437C(void);
void func_800744A0();
void week_day_main0(void);
void week_day_exit0(void);
void func_800748B8();
void func_80074950();
void func_80074A14();
u8 vacation_day_init(void);
u8 func_80074DE0(void);
u8 func_80074F24(void);
u8 func_8007505C(void);
void func_800755B4(void);
void func_80075A64(void);
void func_80075BFC();
void func_80075C24(void);
s32 func_80075C84();
s32 func_80075FA0();
s32 func_80076000();
void func_80076450();
void func_80076630(void);
void func_80076B48(void);
void func_80076C7C(void);
void func_80076DA0(void);
void func_8007739C(void);
void func_80077694(void);
void func_80077900(void);
void func_80077C50(void);
void func_80077E30(void);
void magazine_exit(void);
void holiday_tel(void);
void holiday_tel_init(void);
void holiday_tel_call(void);
void telephone_class_init(void);
void holiday_club_join_exit(void);
void func_80078950();
void func_80078970();
void func_800789E0(void);
void func_80078A94();
void func_80078C48();
void func_80078FE0(void);
void func_80079014(void);
void func_80079070();
s32 func_80079524(void);
void func_80079B10(u16 arg0);
void func_80079C70(void);
void func_80079D28(s16 arg0);
s32 func_80079E00(s32 arg0);
void func_80079E9C(void);
void func_80079F00(void);
void func_8007A4DC();
void func_8007A50C(void);
void func_8007A5BC();
void func_8007A868(void);
void func_8007A924(s32 arg0);
void func_8007A98C();
void func_8007AB24(u16 arg0);
void func_8007ABE0();
void func_8007AD6C();
void func_8007AEC0(void);
void func_8007AF0C(void);
void func_8007B144();
void func_8007B2B4();
void func_8007B568(s32 arg0, s32 arg1);
void func_8007B5CC();
s32 func_8007B5EC(u16 a);
void func_8007B640(void);
void func_8007B6E0();
void func_8007B734();
void func_8007B7E0();
void func_8007B844(void);
void func_8007B8F8();
void func_8007B99C(u16 arg0);
void func_8007BA54(void);
void func_8007BAAC(void);
void func_8007BCFC();
void func_8007BDE8(void);
void func_8007BE94(s16 arg0);
void func_8007BF04(s32 arg0);
void func_8007BFA8();
void func_8007BFB8(void);
void SD_InitCDLevelInfo(void);
void SD_GetCDLevel();
void func_8007C6A8(s32 arg0);
void addr_init_bustup(void);
void func_8007C740();
void func_8007C8A4(void);
s32 func_8007C8D8();
s32 func_8007D8AC();
s32 func_8007E390(void);
#ifndef MAIN_API_OVERRIDE_normal_date_girl_in
void normal_date_girl_in();
#endif
void normal_date_girl_in_init();
void normal_date_girl_in_main();
void normal_date_girl_out(void);
void normal_date_girl_out_init(void);
void normal_date_girl_out_main(void);
void func_8007E81C();
void normal_date_bg_fadein(void);
void func_8007E934();
void normal_date_bg_fadeout(void);
void normal_date_bggirl_fadeout(void);
void normal_date_move_place(void);
void normal_date_move_place_init(void);
void normal_date_move_place_main(void);
void place_init(void);
void bg_read_sub2(s32 arg0);
void func_8007ED84();
void check_k_scroll();
void func_8007EDF8();
void wait_sub_sub(s16 arg0);
void func_80081190(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
void func_80081D30();
s32 func_80082764();
void normal_date_three_select(void);
void normal_date_three_select_init(void);
s32 normal_date_three_select_main(void);
void normal_date_two_select(void);
void normal_date_two_select_init(void);
s32 normal_date_two_select_main(void);
void func_80083378(void);
void func_800833A0(void);
void func_800833F0(void);
void func_80083418(void);
#ifndef MAIN_API_OVERRIDE_func_80083440
void func_80083440(u8 arg0);
#endif
void func_80083474(void);
void func_80083808();
void func_80083A10();
void func_80083B24();
void read_bustup();
void func_800846C0();
void k_disp_inc2();
#ifndef MAIN_API_OVERRIDE_func_800847B8
void func_800847B8(u8 arg0);
#endif
void select_girl_init(s32 arg0);
void select_girl_main(s32 arg0);
void func_80084C98();
void sprite_brightness(s16 idx, u8 val);
void check_para_limit(void);
void func_80084D3C();
void func_80084E4C(void);
void func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800850D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80085234();
void func_80085368(void);
void func_800853FC(void);
void event_face0(void);
void event_face1(void);
void func_800854C8();
void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_8008585C();
void don_wait(void);
void normal_date_speak_1line();
void func_80085A60();
void normal_date_speak(void);
void func_80085B3C(s32 arg0, s32 arg1);
void func_80085CD4(u8 a);
void func_80085E30(s32 arg0, s32 arg1);
void Vblnk_Timer_Init(void);
s32 Vblnk_Timer(void);
void Scroll(s32, s32, s32, s32, s32);
void func_80085F0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Default_Disp(void);
s32 func_8008667C(s32 a);
void DecDCTReset(s32 mode);
void func_800869A4(s32 a);
void func_800869C8(s32 arg0);
void func_80086AB0(s32 arg0);
s32 func_80087954(s32 arg0, u8 *arg1);
s32 func_800879D0();
s32 func_80087C64();
s32 func_80087E4C(s32 arg0, u8 *arg1);
void func_80088070();
s32 func_800880C0();
void func_80088150(u8 *p, s32 n);
void func_80088180(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80089200();
void func_80089244();
void D_800896E0();
void D_800898F0();
void func_8008A0D4(s32 arg0);
void func_8008A148();
void func_8008B750();
void func_8008B8C4();
void func_8008B904();
void func_8008BAA4();
void func_8008BE34();
void func_8008BE54();
void func_8008BEBC();
void func_8008BEE0();
void func_8008BFB0();
s32 func_8008C1B0();
s32 func_8008C620();
void SsVabTransCompleted();
void func_8008D610();
void func_8008E280();
void func_8008E2E0();
void func_8008E310();
void func_8008E408();
void func_8008E6AC();
void func_8008E8B4();
void func_8008F54C();
void func_8008F618();
void func_8008FB00();
void func_8008FC10();
void func_8008FD1C();
void func_8008FD68();
void func_8008FEB0();
void func_8008FED0();
void func_8008FEF0();
void func_8008FF5C();
void func_8008FF60();
void func_80090580(s16);
void func_8009068C();
void func_800907E0(s16);
void func_80090960();
void func_80090D20(void);
void func_80090D60();
void func_80090DB0();
void func_80090DF0();
void func_80090E30();
void func_80090E60(s16);
void func_800949B0();
void func_80097D90(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_80098370(void);
void func_80098380(void);
void func_80098490(s32 a, s32 b, s32 c, s32 d);
void func_80098530(void);
void func_800985A0(void);
void func_80099540(s32 *p);
void func_80099E30(s32 a, void *p);
void func_8009AD30(s32 arg0);
void func_8009AD50(s32 arg0);
void func_8009AD60(s32 arg0);
s32 func_8009AD70();
void GsSetAmbient();
void func_8009B310(s32 arg0, s32 arg1, s32 arg2);
void func_8009B340();
void func_8009B3C0(s32 arg0);
void func_8009B430(s32 arg0, u8 *arg1, s32 arg2, s32 arg3);
void func_8009B560();
void func_8009C210(s32 arg0);
void func_8009C5E0();
void func_8009C674(s32 arg0);
void func_8009C7F8(RECT *rect, s32 arg1, s32 arg2, s32 arg3);
void func_8009C884(RECT *rect, void *arg1);
void func_8009C8E0(RECT *rect, void *arg1);
void func_8009C93C(RECT *rect, s32 arg1, s32 arg2);
void func_8009CC04();
void func_8009CCBC();
void func_8009CCF4();
void func_8009D10C();
void func_8009D294(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_8009ECB0(s32 a, s32 b, s32 c, s32 d);
void AddPrim(void *ot, s32 prim);
void func_8009EED0();
void SetSemiTrans();
void func_8009EF84();
void SetShadeTex();
void func_8009F02C();
void func_8009F054();
void func_8009F0A4();
void func_8009F0B8(u8 *p);
void func_8009F0F4();
void InitGeom(void);
s32 func_800A0070(s32);
s32 rsin();
s32 func_800A0140(s32 a);
void func_800A09D0();
void func_800A0C64();
void func_800A0F4C();
#ifndef MAIN_API_OVERRIDE_func_800AD950
void func_800AD950();
#endif
s32 InitCARD(s32 arg0);
s32 StartCARD(void);
void _bu_init(void);
s32 _card_auto(s32 arg0);
s32 _card_status(s32 chan);
void bzero(void *p, s32 n);
void func_800AE080(u8 *arg0, s32 arg1);
void func_800AE090(void *dst, void *src, s32 n);
void func_800AE0A0(void *arg0, s32 arg1, s32 arg2);
void memcpy();
void func_800AE0B0(void *arg0, ...); /* printf-like (SDK libc) */
s32 printf(u8 *fmt, ...);
s32 func_800AE0C0(s32);
s32 func_800AE0D0(void);
s32 rand();
s32 func_800AE0E0(void *arg0);
s32 strlen(u8 *s);
void func_800AE0F0(void *dst, void *src);  /* strcpy (SDK libc) */
void strcpy(u8 *dst, u8 *src);
void func_800AE100(void *dst, void *src);
void strcat(u8 *dst, u8 *src);
void func_800AE120(s32 arg0);
s32 strcmp(u8 *a, u8 *b);
s32 SetSp();
void InitHeap();
s32 GetGp();
s32 OpenEvent(u32 desc, s32 spec, s32 mode, void (*func)(void));
s32 EnableEvent(s32 ev);
s32 SetRCnt(u32 spec, u32 target, u32 mode);
s32 StartRCnt(u32 spec);
void CloseEvent(s32 ev);
void InitPAD(u8 *buf1, s32 len1, u8 *buf2, s32 len2);
void StartPAD(void);
void ChangeClearPAD(s32 arg0);
s32 TestEvent(s32 ev);
s32 close(s32 fd);
s32 format(u8 *path);
s32 open(u8 *name, s32 mode);
s32 GetSp(void);
void func_800BCDF0();
void func_800BCE10();
s32 func_800BDC20();
void func_801040F0(void);

#endif /* MAIN_API_H */
