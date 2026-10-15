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

/* T-12010: D_801255D0 is saved as a word and read as four bytes. */
typedef union WordBytes4 {
    u32 word;
    u8 bytes[4];
} WordBytes4;
extern WordBytes4 D_801255D0;
/* T-12040: halfword tested by the RPG battle command dispatcher. */
extern s16 D_80121428;

/* T-10730: dialogue voice table leaves, indexed with an eight-byte stride. */
typedef struct DialogueVoice8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} DialogueVoice8; /* size 0x08 */

/* T-10730: sixteen byte selectors copied to the dialogue handler's stack. */
typedef struct DialogueBytes16 {
    /* 0x00 */ u8 value[16];
} DialogueBytes16; /* size 0x10 */

/* T-10730: minimum dialogue work range at D_80122CD0. Reset clears words
 * 5..30; retain existing scalar/array declarations as views in older users. */
typedef struct DialogueWork {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 unk_3C;
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s32 unk_58;
    /* 0x5C */ s32 unk_5C;
    /* 0x60 */ s32 unk_60;
    /* 0x64 */ s32 unk_64;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 unk_6C;
    /* 0x70 */ s32 unk_70;
    /* 0x74 */ s32 unk_74;
    /* 0x78 */ s32 unk_78;
} DialogueWork; /* minimum size 0x7C */

/* T-10730: pointer slots at D_800CA134..D_800CA144. The cursor pointers
 * address signed halfwords; legacy scalar declarations remain valid views. */
typedef struct DialogueTables {
    /* 0x00 */ s16 *group;
    /* 0x04 */ s16 *line;
    /* 0x08 */ u8 ***text;
    /* 0x0C */ u8 **command;
    /* 0x10 */ DialogueVoice8 ***voice;
} DialogueTables; /* size 0x14 on PS1 */

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

#ifndef T9540_FLAGS_WORK24
#define T9540_FLAGS_WORK24
/* Word at 0x800E7D10, slot bytes at +4, and fourteen flags at +0x14; T-9540/T-12030. */
typedef struct FlagsWork24 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 unk_04[0x10];
    /* 0x14 */ u8 flags[14];
    /* 0x22 */ u8 pad22[2];
} FlagsWork24; /* size 0x24 */
#endif

/* Flag word at +0x0C of a Rec38 record (GameState +0x1C8 + 0x38 * i). The original tests single bits
 * with sll; bltz/bgez (bits 1, 2, 3, 6, 12-14 seen) and reads bits 9-11 as one value (sll 20; srl 29);
 * a bit-field store loads the byte into another register than its lui. T-9020 (was a view per file). */
typedef struct CharFlags {
    /* 0x00 */ u32 b0 : 1;
    /* 0x00 */ u32 b1 : 1;
    /* 0x00 */ u32 b2 : 1;
    /* 0x00 */ u32 b3 : 1;
    /* 0x00 */ u32 b4 : 1;
    /* 0x00 */ u32 b5 : 1;
    /* 0x00 */ u32 b6 : 1;
    /* 0x00 */ u32 b7 : 1;
    /* 0x00 */ u32 b8 : 1;
    /* 0x00 */ u32 f9 : 3;
    /* 0x00 */ u32 b12 : 1;
    /* 0x00 */ u32 b13 : 1;
    /* 0x00 */ u32 b14 : 1;
    /* 0x00 */ u32 rest : 17;
} CharFlags; /* size 0x04 */

/* The Rec38 flag word: whole, by halves and bytes, or by bits. */
typedef union Rec38Flags {
    /* 0x00 */ s32 w;
    /* 0x00 */ u32 u;
    /* 0x00 */ u16 h[2];
    /* 0x00 */ u8 b[4];
    /* 0x00 */ CharFlags f;
} Rec38Flags; /* size 0x04 */

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
    /* 0x0C */ Rec38Flags unk_0C;
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
    /* 0x042 */ u16 unk_042;
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
extern s32 D_80094710;
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
extern u8 D_80094748[];
extern u8 D_80094764[];
extern u8 D_8009476C[];
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_80094818[];
extern u8 D_8009483C[];
extern s16 D_80094868;
extern u8 D_800AFDF0[];
extern u8 D_800AFF6C[];
extern u8 D_800AFF78[];
extern u8 D_800B00FC[];
extern u8 D_800B0194[];
extern u8 D_800B0860[];
extern u8 D_800B0896;
extern u8 D_800B0897;
extern u8 D_800B0898;
extern u8 D_800B0929[];
extern u32 D_800B0940;
extern s32 D_800B0948;
extern s16 D_800B094E;
extern s16 D_800B0956;
extern s16 D_800B095E;
extern s16 D_800B0966;
extern s16 D_800B096E;
extern Rec34 D_800B0A04[]; /* 12 records */
extern u8 D_800B0A14;
extern u8 D_800B0AFF;
extern s16 D_800B0B0E[];
extern u8 D_800B0B64[];
extern u8 D_800B0B6C[];
extern s16 D_800B0B76;
extern u8 D_800B0BCE;
extern u8 D_800B0BCF;
extern s8 D_800B0E31;
extern u8 D_800B0E42;
extern u8 D_800B0E43;
extern u8 D_800B0E6A;
extern u8 D_800B0F2C;
extern u8 D_800B0F2D;
extern u8 D_800B0F2E;
extern u8 D_800B0F2F;
extern u8 D_800B0F30;
extern u8 D_800B0F35[];
extern u8 D_800B1746;
extern s32 D_800B1764;
extern u8 D_800B188D[];
extern u32 D_800B1AE4;
extern s32 D_800B1AF0;
extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern u8 D_800B1AF9;
extern s32 D_800B1C1C;
extern s32 D_800B1C70;
extern s32 D_800B1CF0;
extern u8 D_800B3220;
extern u8 *D_800B3288[];
extern u8 **D_800B32CC;
extern s32 D_800B3538[];
extern u8 *girl_short_name_table[];
extern u8 *D_800B3624;
extern u8 *D_800B3630;
extern u8 D_800B3644[3];
#ifndef MAIN_API_OVERRIDE_D_800B3688
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
#ifndef MAIN_API_OVERRIDE_D_800B3748
extern s32 **D_800B3748;
#endif
extern void *D_800B374C; /* image data passed to LoadSquare (func_800673B8) */
extern u8 D_800B3750[][3];
extern s32 D_800B37D4[];
extern u8 *club_name_table[];
extern s32 D_800B394C[];
extern s32 D_800B3A3C[];
extern s32 D_800B3B9C[];
extern u8 D_800B3C6C;
extern u8 D_800B3C70[];
extern u8 D_800B3C88[];
extern s8 D_800B3CA0;
extern u8 D_800B3CFC;
extern u8 D_800B3D24;
extern u8 D_800B3D40;
extern u8 D_800B3D44;
extern u8 D_800B3D48;
extern u8 D_800B3D54[];
extern u8 D_800B3D60;
extern s32 D_800B3D68;
extern s32 D_800B3D6C;
extern s32 D_800B3D70[4]; /* defined in src/main/800451D0.c (T-9010) */
extern s32 D_800B3648[16];
extern u8 D_800B3D80;
extern u8 D_800B3D84[];
extern s8 D_800B3DB0;
extern u8 text_list[];
extern u8 D_800B3DC7[];  /* stride 8 from k_disp_switch: likely a field of an 8-byte struct array */
extern u8 D_800B3F58[];
/* Text display state, defined in src/main/8004E500.c (T-9150). */
typedef struct KWork {
    /* 0x0 */ s16 count;   /* number of entries in text_list */
    /* 0x2 */ s16 total;
    /* 0x4 */ s16 mark;
    /* 0x6 */ u8 speed;
    /* 0x7 */ u8 pad_07;
    /* 0x8 */ s16 pos;
    /* 0xA */ u8 unk_6A;
    /* 0xB */ u8 pad_0B;
} KWork; /* size 0xC */
extern KWork text_list_count;
/* Four signed menu row positions, read by x_taku_menu_set. */
extern s16 D_800B40F0[4];
extern u8 D_800B4100[13];
extern u8 D_800B4110[13];
extern u8 D_800B4120[13];
extern s8 D_800B4130[13][8];
extern u8 D_800B4198[7];
extern u8 D_800B41A0[];
extern u8 D_800B41B0[20][20];
extern u8 D_800B4341[]; /* table read by func_80052E60 */
extern s8 D_800B4349[11];
extern u8 D_800B4355[10];
extern u8 D_800B4361[10];
extern s8 D_800B436C[15];
extern s8 D_800B437C[15];
extern s32 D_800B58E4;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern s32 D_800B5900;
extern s32 D_800B5920[4];
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
extern s16 D_800B59F0;
extern s16 D_800B59F4;
extern u8 D_800B5A60;
extern u8 D_800B5A64;
extern s16 D_800B5B24;
extern u32 D_800B5B28[4][8];
extern u32 D_800B5BA8[8];
extern u8 D_800B5BC8;
extern u8 D_800B5BCC;
extern u8 D_800B5BD0;
extern s8 D_800B5BD4;
extern s32 D_800B5BD8[];
extern s32 D_800B5BE8[];
extern s32 D_800B5BF8[];
extern s16 D_800B5C08;
extern u8 D_800B5C10[];
extern u8 D_800B5C2C[][17];
extern u8 D_800B5D0C[];
extern u8 D_800B5D1C[][17];
extern u8 D_800B5DFC[];
extern u8 D_800B5E0C[];
extern u8 D_800B5E25[];
extern u8 D_800B5E3E[];
extern u8 D_800B5E57[];
extern u8 D_800B5E70[];
extern u8 D_800B5E89[];
extern u8 D_800B5EA4[][3][36];
#ifndef MAIN_API_OVERRIDE_D_800B66A4
extern s32 D_800B66A4[];
#endif
extern s16 D_800B66E0[];
extern s16 D_800B66F4[];
extern u8 D_800B6708;
extern u8 D_800B6724[];
extern u8 D_800B6728[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern s8 D_800B6740;
extern s32 D_800B6748;
extern s32 D_800B674C;
extern s32 D_800B6750;
extern s32 D_800B6754;
extern s32 D_800B6758;
extern s32 D_800B6760[];
extern s32 D_800B6780[];
extern s32 D_800B6810[];
extern s32 D_800B69E0[];
extern u8 D_800B6D30;
extern u8 ovl_load_state;
extern u8 ovl_load_sectors;
extern s32 ovl_load_buf;
extern s32 ovl_load_lba;
extern u8 font_glyphs[];
extern s8 D_800C51C4;
extern s32 D_800C5FCC;
extern s32 D_800C5FD0;
extern u8 D_800C6220[];
extern u8 D_800C9730[];
extern s32 D_800C9734;
extern s32 D_800C9738;
extern s32 D_800C973C;
extern s32 D_800C974C;
extern s32 D_800C9750;
extern s32 D_800C9754;
extern s32 D_800C9758;
extern u8 sprite_frame_table[];
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
#ifndef MAIN_API_OVERRIDE_D_800CA158
extern u8 D_800CA158;
#endif
extern u8 D_800CA15C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern u8 D_800CA16C[];
extern u8 D_800CA174[];
extern u8 D_800CA17C[];
extern u8 D_800CA188[];
extern u8 girl_name_buf[];
extern u8 D_800CA19C[];
extern u8 D_800CA1DC[];
/* Four s16[4] rows (T-9020): DATE indexes each from its own base (D_800CA22C[i]) and sets a row
 * with one chain assignment, which gives the original's shared register only for array elements. */
extern s16 D_800CA21C[4];
extern s16 D_800CA21E;
extern s16 D_800CA224[4];
extern s16 D_800CA22C[4];
extern s16 D_800CA234[4];
extern s16 D_800CA236;
extern u8 D_800CA23C[];
extern u8 D_800CA250[];
extern u8 D_800CA25C[];
extern u8 D_800CA2A4[4]; /* [1], [2] are the former D_800CA2A5/6 */
extern u8 D_800CA2A5;
extern u8 D_800CA2A6;
extern u8 D_800CA2A8;
extern s16 D_800CA2AC;
extern u8 D_800CA2B0[];
extern s16 D_800CA2C0;
extern u8 D_800CA2C4;
extern s32 D_800CA2C8;
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
extern s16 D_800CA300[12];
extern s16 D_800CA318[12];
extern s16 D_800CA348[12];
extern u8 D_800CA360;
extern s8 D_800CA364;
extern u8 D_800CA368;
extern DialogueBytes16 D_800CA36C;
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
extern u8 D_800D9334[];
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
#ifndef MAIN_API_OVERRIDE_D_800E7D10
extern s32 D_800E7D10;
#endif
extern u8 D_800E7D11[];
extern u8 D_800E7D13[];
extern u8 D_800E7D14[];
extern s8 D_800E7D15;
extern s8 D_800E7D16;
extern u8 D_800E7D1F;
extern u8 D_800E7D2C;
extern u8 D_800E7D34;
extern u8 D_800E7D36;
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
extern u8 D_800E7D4F;
extern s32 D_800E7D50;
extern u8 D_800E7D68;
#ifndef MAIN_API_OVERRIDE_D_800E8BEE
extern s8 D_800E8BEE;
#endif
extern u8 D_800E8BEF;
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
extern u8 D_800E9E5F;
extern u8 D_800E9E63;
extern s16 D_800E9E74;
extern u8 D_800EAFA0[];
extern s8 D_800EAFA2;
extern u8 D_800EAFA7;
extern s16 D_800EAFB6;
extern s16 D_800EAFB8;
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
extern s32 D_800EC2F8;
extern s16 D_800EC2FC;
extern s16 D_800EC2FE;
extern s16 D_800EC300;
extern s16 D_800EC302;
extern s16 D_800EC304;
extern s8 D_800EC306;
extern s8 D_800EC307;
extern s16 D_800EC308;
extern s16 D_800EC30A;
extern u8 D_800EC30C;
extern u8 D_800EC30D;
extern u8 D_800EC30E;
extern s16 D_800EC310;
extern s16 D_800EC312;
extern s16 D_800EC314;
extern s16 D_800EC316;
extern s32 D_800EC318;
#ifndef MAIN_API_OVERRIDE_D_800EECB0
extern s32 D_800EECB0;
#endif
extern s32 D_800EECB4;
extern s32 D_800EECBC;
extern s32 D_800EECC0;
extern s32 D_800EECC8;
extern s32 D_800EECCC;
extern s32 D_800EECD0;
extern s32 D_800EECD4;
extern s32 D_800EECE0;
extern s32 D_800EECE4;
extern s32 D_800EECE8;
extern s32 D_800EECEC;
extern s32 D_800EECF0;
extern u8 D_800F19AB;
extern u8 D_800F53DA;
extern u8 D_800F53DE;
extern u8 D_800F53DF;
extern u8 D_800F53E0;
extern u32 D_800F5488;
extern s32 D_800F5490;
extern s16 D_800F5496;
extern s16 D_800F549A;
extern s16 D_800F54A6;
extern s16 D_800F54AE;
extern s16 D_800F54B2;
extern s16 D_800F5556;
extern u8 D_800F55CA;
extern u8 D_800F5602;
extern u32 D_800F5638;
extern u8 D_800F563A;
extern s16 D_800F56DE;
extern s32 D_800F56E0;
extern s16 D_800F574E;
extern s32 D_800F5750;
extern u8 D_800F5833;
extern u8 D_800F5902;
extern u8 D_800F592F;
extern u8 D_800F593D;
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
extern u8 D_800F7710[];
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
#ifndef MAIN_API_OVERRIDE_D_8011ECD0
extern u8 D_8011ECD0[]; /* 0x44-byte records; DATE uses entries 96..99 (T-10730). */
#endif
extern u8 D_8011ECD3;
extern s16 D_8011ECE8;
extern s32 D_8011ECF4;
extern s16 D_8011ECF6;
extern s32 D_8011ECF8;
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
extern u8 D_8011ED58[];
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
extern u8 D_8011ED9C[];
extern s8 D_8011ED9F;
extern s8 D_8011EDE3;
extern u8 D_8011EFBC[];
extern u8 D_8011F044[];
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
#ifndef MAIN_API_OVERRIDE_D_8011F414
extern u8 D_8011F414;
#endif
extern s32 D_8011F41C;
extern s16 D_8011F422;
extern s16 D_8011F426;
extern s16 D_8011F458;
extern s16 D_8011F466;
extern s16 D_8011F46A;
extern u8 D_8011F4CA;
extern u8 D_8011F4CB;
extern u8 D_8011F4CF;
extern s16 D_8011F4D0;
extern s16 D_8011F4DE;
extern s16 D_8011F4E0;
extern s16 D_8011F4EE;
extern s16 D_8011F4F2;
extern s8 D_8011F50D;
extern u8 D_8011F50E;
extern u8 D_8011F50F;
extern u8 D_8011F513;
extern s16 D_8011F522;
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
#ifndef MAIN_API_OVERRIDE_D_80120656
extern s8 D_80120656;
#endif
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
#ifndef MAIN_API_OVERRIDE_D_80120674
extern s16 D_80120674;
#endif
extern s16 D_80120676;
#ifndef MAIN_API_OVERRIDE_D_80120678
extern s16 D_80120678;
#endif
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
#ifndef MAIN_API_OVERRIDE_D_801206B8
extern s16 D_801206B8;
#endif
extern s16 D_801206BA;
#ifndef MAIN_API_OVERRIDE_D_801206BC
extern s16 D_801206BC;
#endif
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
extern s32 D_80120760;
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
#ifndef MAIN_API_OVERRIDE_D_801207B0
extern s32 D_801207B0;
#endif
#ifndef MAIN_API_OVERRIDE_D_801207B4
extern s32 D_801207B4;
#endif
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
extern s16 D_80120896;
extern s16 D_8012089A;
extern s16 D_801208CC;
extern s16 D_801208DE;
extern u8 D_801208FB;
extern s16 D_80120912[];
extern s16 D_80120922;
extern u8 D_8012093F;
extern s16 D_80120966;
extern s16 D_801209AA;
extern s16 D_801209EE;
extern s16 D_80120A32;
extern s16 D_80120A72;
extern s16 D_80120A76;
extern s16 D_80120AA8;
extern s16 D_80120AB6;
extern s16 D_80120ABA;
extern s16 D_80120AFA;
extern s16 D_80120AFE;
extern s16 D_80120B3E;
extern s16 D_80120B42;
extern u8 D_80120B5F;
extern s16 D_80120B82;
extern s16 D_80120B86;
extern u8 D_80120BA3;
extern s16 D_80120BC6;
extern s16 D_80120BCA;
extern s8 D_80120BE3;
extern u8 D_80120BE7;
extern s16 D_80120C0A;
extern s16 D_80120C0E;
extern u8 D_80120C2B;
extern s16 D_80120C4E;
extern s16 D_80120C52;
extern u8 D_80120C6F;
extern s16 D_80120C92;
extern s16 D_80120C96;
extern s16 D_80120CD6;
extern s16 D_80120CDA;
extern s16 D_80120D08[];
extern s16 D_80120D1A;
extern s16 D_80120D1E;
extern s16 D_80120D5E;
extern s16 D_80120D62;
extern s16 D_80120DA2;
extern s16 D_80120DA6;
extern s8 D_80120DC1;
extern s8 D_80120E05;
extern u8 D_80120E07;
extern s16 D_80120E1C;
extern u8 D_80120E4E;
extern s16 D_80120E60;
extern s8 D_80120E93;
extern s16 D_80120EE6;
extern s16 D_80120EF6;
extern s16 D_80120EFA;
extern s16 D_80120F2A;
extern s16 D_80120F3A;
extern s16 D_80120F3E;
extern s16 D_8012114C;
extern u8 D_8012117A;
extern u8 D_801211BE;
extern u8 D_80121202;
extern s16 D_8012121A;
#ifndef MAIN_API_OVERRIDE_D_8012121C
extern s16 D_8012121C;
#endif
extern s32 D_80121238;
extern u8 D_80121246;
extern s32 D_8012127C;
extern s8 D_801212CB;
extern s8 D_80121311;
extern u8 D_80121313;
#ifndef MAIN_API_OVERRIDE_D_80121317
extern s8 D_80121317;
#endif
extern s16 D_80121328;
extern s8 D_80121353;
extern u8 D_80121357;
extern s16 D_8012136C;
extern s8 D_80121397;
extern s8 D_80121399;
extern u8 D_8012139B;
extern u8 D_8012139E;
extern s16 D_801213B0;
extern s8 D_801213DD;
extern u8 D_801213DF;
extern s16 D_801213F4;
extern s8 D_80121421;
extern u8 D_80121422;
extern u8 D_80121423;
extern u8 D_80121426;
extern s16 D_80121438;
extern u8 D_80121467;
extern s8 D_8012146B;
extern s32 D_8012149C;
extern s8 D_801214A7;
extern u8 D_801214AB;
extern s8 D_801214AF;
extern s32 D_801214E0;
extern s8 D_801214EB;
extern u8 D_801214EF;
extern s8 D_801214F3;
extern s32 D_80121524;
extern s8 D_8012152F;
extern u8 D_80121531;
extern u8 D_80121533;
extern s16 D_80121548;
extern s16 D_80121556;
extern s16 D_8012155A;
extern s16 D_8012158C;
extern s8 D_801215FD;
extern u8 D_801215FF;
extern s8 D_80121603;
extern s32 D_80121634;
extern s8 D_8012163F;
extern s8 D_80121647;
extern s8 D_80121685;
extern s8 D_8012168B;
extern u8 D_8012170F;
extern s16 D_80121726;
extern s16 D_80121728;
extern u8 D_80121750[];
extern u8 D_801217A0[];
extern Rec24 D_801217D0[]; /* at least 12 records */
extern s16 D_801217D6;
extern u8 D_801217E4;
extern u8 D_801217E5;
extern u8 D_801217E6;
extern s16 D_801217F8;
extern s16 D_801217FA;
#ifndef MAIN_API_OVERRIDE_D_80121808
extern s32 D_80121808;
#endif
extern u8 D_80121809;
extern u8 D_8012180A;
extern s32 D_80121818; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_8012181E;
extern s32 D_8012183C; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_80121842;
extern s32 D_80121860; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_80121864; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_80121866;
#ifndef MAIN_API_OVERRIDE_D_80121874
extern u8 D_80121874;
#endif
extern s32 D_80121884; /* EVENT data at this address; for the other units it is a field of D_801217D0[] */
extern s16 D_8012188A;
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
extern u8 D_80122104;
extern u8 D_80122105;
extern u8 D_80122106;
extern s16 D_80122790;
extern s32 D_80122794;
extern s8 D_80122798;
extern s8 D_80122799;
extern s8 D_8012279A;
/* 0x50-byte records at D_801227A0 (16 entries; D_80122CA0 is the end address the loops compare against) */
typedef struct TaiikuBig {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4[0x1C];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24[0x2C];
} TaiikuBig; /* size 0x50 */
extern TaiikuBig D_801227A0[];
extern TaiikuBig D_80122CA0[];
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
extern s32 D_80122CE8;
extern s32 D_80122CEC;
extern s32 D_80122CF0;
extern s32 D_80122CF4;
extern s32 D_80122CF8;
extern s32 D_80122CFC;
extern s32 D_80122D00;
extern s32 D_80122D04;
extern s32 D_80122D08;
extern s32 D_80122D0C;
extern s32 D_80122D10;
extern s32 D_80122D14;
extern s32 D_80122D18;
extern s32 D_80122D1C;
extern s32 D_80122D20;
extern s32 D_80122D28;
extern s32 D_80122D2C;
extern s32 D_80122D30;
extern s32 D_80122D34;
extern s32 D_80122D38;
extern u32 D_80122D3C;
extern s32 D_80122D40;
extern s32 D_80122D44;
extern s32 D_80122D4C;
extern u8 D_80122DD0[];
extern s32 D_80122E94;
extern u32 D_80122E9C;
extern s32 D_80122EA0;
extern s32 D_80122EA4;
extern s32 D_80122EAC;
#ifndef MAIN_API_OVERRIDE_D_80122EB8
extern s32 D_80122EB8;
#endif
extern s32 D_80122EBC;
extern s32 D_80122EC0;
extern s32 D_80122EC4;
#ifndef MAIN_API_OVERRIDE_D_80122EC8
extern s32 D_80122EC8;
#endif
extern s32 D_80122ECC;
extern s32 D_801230D0;
extern s32 D_801230E0[];
extern s32 D_801230E4;
extern s32 D_801230F4;
extern u32 D_80123110;
extern u8 D_80123120[];
extern s32 D_80125120;
extern s32 D_80125124;
extern u8 D_80125128;
extern u8 D_80125129;
extern u8 cd_read_status;
extern s8 D_8012512B;
/* 0x18-byte entry of the 48-entry ring at D_80125130 (read index D_80125128, write index
 * D_80125129; filled by func_80045318). data[15] is cleared on every write and set to 0xFF when
 * the caller passes 1 instead of a data pointer. */
typedef struct Ring18 {
    /* 0x00 */ u8 cmd;
    /* 0x04 */ s32 arg;
    /* 0x08 */ u8 data[16];
} Ring18; /* size 0x18 */
extern Ring18 D_80125130[];
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
extern u32 D_801255F0[];
extern s32 D_80125BF4;
extern s32 D_80125BF8;
extern s32 D_80125BFC;
extern s32 D_80125C00;
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
extern s16 D_80125CA0;
extern s32 D_80125CA4;
extern s32 D_80125CA8;
extern s32 D_80125CAC;
extern s32 D_80125CB0;
extern u16 D_80125CC0;
extern Work80125D10 D_80125D10;
extern s32 D_80125D14;
extern s32 D_80125D18;
extern s32 D_80125D1C;
extern s32 D_80125D20;
extern s32 D_80125D24;
extern s32 D_80125D28;
extern s16 D_80125D4A;
extern s8 D_80125D5C;
extern u8 D_80125D5D;
extern s16 D_80125D38;
extern s16 D_80125D3A;
extern s16 D_80125D3C;
extern u16 D_80125D4C;
extern u16 D_80125D4E;
extern u16 D_80125D50;
extern u16 D_80125D52;
extern u8 D_80125D5E;
extern u8 D_80125D5F;
extern s32 D_80125D60[];
extern s32 D_80125D64;
extern s32 D_80125D68;
extern s32 D_80125D6C;
extern s32 D_80125D70[];
extern s32 D_80125D80[];
extern s32 D_80125D84;
extern s32 D_80125D88;
extern s32 D_80125D8C;
extern s32 D_80125D90;
extern s32 D_80125D94;
extern s32 D_80125D98;
extern s32 D_80125D9C;
extern u16 D_80125E60;
extern u16 D_80125E62;
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;
extern u8 D_80125E70[];
extern s16 D_80126080[]; /* two 0x200-entry s16 tables (+0, +0x400), SD_CalcCDAve */
extern s32 D_8012749C;
extern s32 D_801274A0;
extern s32 D_80129F10;
extern u8 D_80129F40[];
extern s32 D_800B699C[];
extern s8 D_80120E49;
extern u16 D_800B670A[];
extern u8 D_80089C84;
extern u8 D_800CA2BC;
extern s16 D_800EB042;
extern s16 D_800EB044;
extern u8 D_80123324[];
extern u8 D_800B0E72;
extern u32 D_800B5970[16];
extern u32 D_800B59B0[16];
extern u32 D_800B0C4C;
extern u8 D_800E94A0[];
extern u8 D_800FD4A0[];
extern s16 D_800B0962;
extern u8 D_800E7D4E;
extern s32 D_80122E98;
extern s32 D_80122EA8;
extern s32 D_80122EB0;
extern s32 D_80122EB4;
extern u8 D_8011EDE0[];
extern u8 D_8011EEF0[];
extern u8 *D_800B342C[];
extern u8 *D_800B3610;
extern u8 *D_800B361C;
extern u8 *D_800B3628;
extern s32 D_800EED08;
extern s32 D_801230D4;
extern s32 D_801230D8;
extern s16 D_800F54B6;
extern u8 D_800D9320[];
extern TaiikuBig D_80122840[];
extern TaiikuBig D_80122B10[];
extern u8 D_80122660[];
extern s16 D_800CA330[];
extern u32 D_800E3680[16];
extern s16 D_801217D4;
extern u8 D_800EC1C8;
extern u8 D_800EC1A4;
extern u8 D_800EC1A5;
extern u8 D_800EC1A6;
extern u8 D_800EC1C9;
extern u8 D_800EC1CA;
extern s32 D_80080710;
extern s16 D_800F1990[14];
#ifndef MAIN_API_OVERRIDE_D_80120694
extern u8 D_80120694[];
#endif
extern s32 D_80122CD8;
extern u8 D_800B3CF8;

/* T-10860: declaration generated by sync_protos.py. */
#ifndef MAIN_API_STRNCAT_T10860
#define MAIN_API_STRNCAT_T10860
#endif
extern s32 D_800EECC4;

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
void func_80037A90();
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
void func_800422C8(void);
void func_80042418(void);
void func_80042458(void);
void func_80042488(void);
s32 func_80042808(void);
s32 func_8004284C(void);
void main_state_set();
s32 func_80042908(s32 arg0);
s32 func_80042940(s32 arg0);
s32 func_80042960(void);
s32 func_80042AC8();
void func_80042C30(void);
void func_80042CB0();
s32 func_80043010(void);
void func_800430C0();
void func_800433D0(s32 arg0);
s32 func_80046590(s16 arg0, u8 arg1);
s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1);
u8 *get_k_work(s32 arg0);
void glyph_expand_1bpp();
void draw2d3d(u8 arg0, u8 arg1);
void func_800438DC();
void back_clear_switch(s32 arg0);
void func_800438F0();
s32 func_80043914();
s32 load_palette();
s32 load_csr_tp();
void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4);
void func_8004435C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4);
#ifndef MAIN_API_OVERRIDE_func_80044750
s32 func_80044750(s32 arg0);
#endif
s32 func_80044774(s32 arg0);
u8 func_8004480C(void);
void func_8004482C(void);
void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 func_80044C98(void);
s32 func_80044E8C(void);
s32 func_8004500C();
s32 func_800450F4(s32 arg0, s32 arg1);
s32 func_800451D0();
void func_800452C4(void);
void func_80045414(s32 arg0, s32 arg1, u8 *arg2);
void func_80045B14(void);
s32 func_80045FF4(void);
#ifndef MAIN_API_OVERRIDE_func_80046094
u8 func_80046094(void);
#endif
u8 cd_read_sync_status(void);
u8 func_800460DC(void);
u8 func_800460EC(void);
s32 func_80046274(void);
void func_80046290(s32, s32, s32);
void func_800462BC(u8 arg0, s32 arg1, s32 *arg2);
#ifndef MAIN_API_OVERRIDE_func_800462C8
void func_800462C8(u8 arg0, s32 arg1, s32 arg2);
#endif
void cd_read_sectors(u8 arg0, s32 arg1, s32 arg2);
s32 file_checksum_ok();
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
void func_8004955C();
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
#ifndef MAIN_API_OVERRIDE_func_80049A40
void func_80049A40(s16 a, s16 b, s16 c, s16 d, s32 e, s32 f, s32 g);
#endif
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
void func_8004C90C();
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
void func_8004E9F4();
void k_reset(s32 arg0);
void func_8004EA98();
void k_sub_reset_point_set(void);
#ifndef MAIN_API_OVERRIDE_func_8004EAAC
void func_8004EAAC();
#endif
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
s8 menu_check();
s8 menu_check_1();
s8 menu_check_2();
void func_8004FC10();
void func_800504CC();
s32 func_80050AB8();
void func_80050B54();
void menu_bar_show();
void func_80050C24(s32 arg0, s32 arg1);
void x_taku_menu_set(s32 arg0, s32 arg1);
void func_80050D60();
void func_80050D78();
void x_taku_string_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#ifndef MAIN_API_OVERRIDE_func_80050DFC
void func_80050DFC(u8 *arg0);
#endif
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
s32 func_80053F04(void);
s32 func_80054140(void);
u8 func_80054284(void);
s32 func_800552DC(s32 arg0);
void func_80053EFC();
s32 func_80054388();
u8 func_8005448C(void);
void func_80054864();
void func_8005493C();
void func_800549E8();
s32 func_80054AF4(s32 arg0);
void *func_80054B60();
void func_80054C4C(void);
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
s32 func_8005742C(s32 arg0, s32 arg1);
s32 func_8005751C();
void func_80057640();
s32 func_80057710(void);
void func_800578F4(s32 a);
void func_80057D28(s32);
void func_80058398();
void func_80058D20();
void initView(void);
void func_80058EB0();
void func_80058F0C(s32 *arg0);
void initModelingData_init(s32 *arg0);
s32 func_80058F9C(u32 *arg0, s32 arg1, u32 arg2);
void func_80059048();
void func_800590CC();
void func_800591D8(s32 arg0);
void func_80059308(void);
void func_80059688(s32 arg0);
void SenseMouse(u16 arg0, u16 arg1);
void SetMouse(s32 arg0, u32 arg1, u32 arg2);
void func_80059938(s32 port);
void func_80059B04();
void func_80059B40(void);
void func_80059BC0(void);
void func_80059BE8(void);
s32 func_80059E00(void);
s32 func_8005A1A0(void);
void func_8005A2A8(void);
void func_8005A37C(void);
void func_8005A3E8(void);
void func_8005A560(void);
void func_8005A668();
void func_8005ABC0();
void func_8005ABD0(void);
void func_8005AC70(s32 arg0);
void func_8005AD70(void);
void func_8005AE60();
void func_8005AEFC();
s32 func_8005AF28(void);
s32 func_8005AFC8(void);
s32 func_8005B040();
void func_8005B06C();
s32 func_8005B0F4();
s32 func_8005B1A8(void);
void func_8005B264();
s8 func_8005B28C();
s32 func_8005B2BC();
s32 func_8005B32C();
u32 func_8005B368();
void func_8005B39C(void);
s32 func_8005B43C();
void func_8005B798(void);
void func_8005B830(void);
void func_8005B8A0(void);
void func_8005B8E0(void);
void func_8005B908();
void func_8005BD20();
void func_8005C4CC(s32 arg0);
void func_8005D174(void);
s32 func_8005D448(void);
s32 func_8005BE8C(void);
void func_8005BCEC(void);
void func_8005C5BC(void);
void func_8005CF6C(void);
void func_8005CA94(void);

void func_8005C7C0(void);
void func_8005C7FC(void);
extern u8 D_800B59F8[];
extern u8 D_800B5A1C[];
void func_8005F09C(void);
s32 func_8005FDEC(void);
s32 func_8005DEA0(s32 arg0);
s32 func_8005E150(void);
s32 system_settings_init(void);
s32 func_800603C0(void);
void func_8005D1B0();
void func_8005D9B4();
s32 func_8005E0E0();
void join_club(void);
void join_club_init(void);
s32 join_club_select(void);
s32 join_club_message(void);
void join_club_exit(void);
s32 func_80060B24();
void func_80060B78(void);
void uwasa_exit0(void);
s32 uwasa0();
s32 pre_xmas_init(void);
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
void func_80063520(u32 arg0);
void func_80063668();
void bustup_wink(void);
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
#ifndef MAIN_API_OVERRIDE_func_80066104
void func_80066104();  /* the matched definition takes no parameter; pre_xmas_init passes a string through $a0 */
#endif
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
void func_80067C28(void);
s32 func_80067DD4(void);
void func_80067E34(void);
void func_80067F04(void);
s16 func_80068898(s32 arg0, s32 arg1);
s16 func_800688F0(s32 arg0, s32 arg1);
void func_80068938(u8 arg0, u8 arg1, s32 arg2);
void func_80068EC0();
void cal_base_show(void);
void func_80069128();
void cal_sprite_init(void);
void func_800696DC(s32, s32);
void func_8006A044();
void func_8006A2CC();
void func_8006B0C8();
void club_list_menu(void);
void func_8006B900(void);
void func_8006BA40(void);
void func_8006BC28(s32 arg0);
void func_8006BD6C(s32 arg0);
s32 func_8006C334(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
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
s32 func_8006EB64(u8 *arg0, u8 *arg1);
s32 func_8006EEC4(u8 *arg0, u8 *arg1);
void yuukou_down(void);
void syoushin_up(void);
void func_80070F80(void);
void func_80070FD0(void);
void func_8007132C(void);
void func_80072338(void);
void func_8007259C(u8 arg0);
void func_800726F0(void);
void func_80072734();
s32 func_80072944(void);
void func_80072998(void);
void func_80072B20(void);
s32 func_80072B5C(s32 arg0);
void func_80072C68(void);
void func_80072CA0(void);
void func_80072FE8();
void func_800737A0(void);
u8 week_day_init(void);
s32 get_weekly_bg_sector(void);
u8 func_80073AD8(void);
u8 func_800741B8(void);
u8 func_8007437C(void);
void func_800744A0();
s32 week_day_main0(void);
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
s32 func_80076B48(void);
void func_80076C7C(void);
void appointment_launch(void);
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
s32 func_800789A0();
void func_800789E0(void);
void func_80078A94();
void func_80078C48();
void ovl_load_poll(void);
void ovl_load_verify(void);
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
#ifndef MAIN_API_OVERRIDE_func_8007B3DC
void func_8007B3DC(u16 arg0);
#endif
#ifndef MAIN_API_OVERRIDE_func_8007B460
void func_8007B460(u16 arg0);
#endif
#ifndef MAIN_API_OVERRIDE_func_8007B4E4
void func_8007B4E4(u16 arg0);
#endif
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
void func_8007BF04(s16 arg0);
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
s32 normal_date_girl_in_init(void);
s32 normal_date_girl_in_main(void);
#ifndef MAIN_API_OVERRIDE_normal_date_girl_out
void normal_date_girl_out(void);
#endif
void normal_date_girl_out_init(void);
s32 normal_date_girl_out_main(void);
s32 normal_date_bg_fadein(void);
void func_8007E934();
void normal_date_bg_fadeout(void);
s32 normal_date_bggirl_fadein(void);
s32 normal_date_bggirl_fadeout(void);
void normal_date_move_place(void);
void normal_date_move_place_init(void);
s32 normal_date_move_place_main(void);
void place_init(void);
void bg_read_sub2(s32 arg0);
void func_8007ED84();
void check_k_scroll();
void func_8007EDF8();
void wait_sub_sub(s16 arg0);
void func_80081128(void);
#ifndef MAIN_API_OVERRIDE_dialogue_render
void dialogue_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
#endif
void func_80081D30();
s32 func_80082764();
s32 make_three_select();
void normal_date_three_select(void);
s32 normal_date_three_select_init(void);
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
s32 func_80083474(void);
s32 func_80083808();
void func_80083A10();
void func_80083B24();
void read_bustup();
void func_800846C0();
void k_disp_inc2();
#ifndef MAIN_API_OVERRIDE_func_800847B8
void func_800847B8(u8 arg0);
#endif
#ifndef MAIN_API_OVERRIDE_select_girl_init
void select_girl_init(s32 arg0);
#endif
s32 select_girl_main(s32 arg0);
#ifndef MAIN_API_OVERRIDE_func_80084C98
void func_80084C98();
#endif
void sprite_brightness(s16 idx, u8 val);
void check_para_limit(void);
void func_80084D3C();
void func_80084E4C(void);
s32 func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 func_800850D4();
void func_80085234();
void func_80085368(void);
void func_800853FC(void);
void event_face0(void);
void event_face1(void);
void func_800854C8();
void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80085678();
void func_8008585C();
void don_wait(void);
void normal_date_speak_1line();
void func_80085A60();
void normal_date_speak(void);
s32 func_80085B3C();
void func_80085CD4(u8 a);
void func_80085E30(s32 arg0, s32 arg1);
void Vblnk_Timer_Init(void);
s32 Vblnk_Timer(void);
void Scroll(s32, s32, s32, s32, s32);
void func_80085F0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);
void Default_Disp(void);
void func_80086424();
s32 func_8008667C(s32 a);
void func_800867CC(void);
void DecDCTReset(s32 mode);
void func_800869A4(s32 a);
void func_800869C8(s32 arg0);
void func_80086AB0(s32 arg0);
s32 func_80087954(s32 arg0, u8 *arg1);
s32 func_800879D0();
s32 func_80087C64();
s32 func_80087E4C(s32 arg0, u8 *arg1);
s32 func_80087E6C(s32 arg0, s32 arg1, s32 arg2);
void func_80088070();
s32 func_800880C0();
void func_80088150(u8 *p, s32 n);
void func_80088180(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80089200();
void func_80089244();
void D_800896E0();
void D_800898F0();
void D_80089B6C();
void D_80089BD4();
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
#ifndef MAIN_API_OVERRIDE_func_8008BFB0
void func_8008BFB0();
#endif
s32 func_8008C1B0();
s32 func_8008C620();
#ifndef MAIN_API_OVERRIDE_SsVabTransCompleted
void SsVabTransCompleted();
#endif
#ifndef MAIN_API_OVERRIDE_func_8008D610
void func_8008D610();
#endif
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
#ifndef MAIN_API_OVERRIDE_func_8009B430
void func_8009B430(s32 arg0, u8 *arg1, s32 arg2, s32 arg3);
#endif
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
s32 func_8009ED74();
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
void func_800AB984();
void func_800ABA38();
void func_800ABAB8();
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
/* GYOZI calls this entry with an implicit-int ABI; semantics unknown. T-13202. */
s32 func_800BCE40();
s32 func_800AE0E0(void *arg0);
s32 strlen(u8 *s);
void func_800AE0F0(void *dst, void *src);  /* strcpy (SDK libc) */
void strcpy(u8 *dst, u8 *src);
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
s32 func_80067DFC(void);
s32 func_80053418();
s32 func_800463E8(void);
#ifndef MAIN_API_OVERRIDE_func_80049FF0
s32 func_80049FF0();
#endif
void place_init2(void);
void normal_date_bg_out(void);
void func_8004A9CC();
void func_8004AFB8();
void func_800651F8();
void func_80065234();
void func_8006B5BC(GsWord arg0);
/* SD_GetCDLevel request/output prefix, verified from halfword accesses. T-10870. */
typedef struct CdLevelWork {
    /* 0x00 */ u16 unk_00; /* left output */
    /* 0x02 */ u16 unk_02; /* right output */
    /* 0x04 */ u16 unk_04; /* output precision: shift by 16 minus this value */
    /* 0x06 */ u16 unk_06; /* sample count */
    /* 0x08 */ u16 unk_08; /* method, square and shaping selectors */
} CdLevelWork; /* accessed size 0x0A */
void read_bustup_uniform(void);
void set_tarao_sprt();
void set_tarao_bg(s32 arg0);
void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n);
void SD_CalcCDAve(s16 *left, s16 *right, u16 start, u16 count);
s32 func_8006F218(u8 *arg0, u8 *arg1);

/* T-10860: generated from src/main/8007C030.c by sync_protos.py. */
s16 getCDlevel(void);
#ifndef MAIN_API_OVERRIDE_strncat
u8 *strncat(u8 *dst, u8 *src, s32 count);
#endif

/* T-10860: declaration generated by sync_protos.py. */
#ifndef MAIN_API_STRNCAT_T10860
#define MAIN_API_STRNCAT_T10860
#endif
void func_80046600(void);
void func_800464E8(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern u8 D_800B0895;
extern u8 D_800B0F31;
extern u8 D_800B0F32;
extern u8 D_800B0F33;
extern u8 D_800B0F34;

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 D_800B3D30;
extern s32 D_800B3D34;
extern s32 D_800B3D38;
extern s32 D_800B3D3C;
extern u8 D_800B3D20;

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
s32 func_8007E81C();

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern u8 D_800B3634;
extern u8 D_800B3638;

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s16 D_80120BB8;

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8005C414(void);
void func_8005D56C(void);
s32 func_8005D804(void);
void func_8005D31C(void);
void func_8005C968(void);
s32 func_8005BAE0(void);
s32 func_8005BC38(void);
s32 func_8005BB84(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8005E018(void);
void func_8005E2C0(void);
void func_8005E40C(void);
void func_8005E454(void);
void func_8005E5C8(void);
void func_8005E7A4(void);
void func_8005E7F0(void);
void func_8005E924(void);
void func_8005E9EC(void);
void func_8005EA38(void);
void func_8005EB74(void);
void func_8005EC78(void);
void func_8005ECB8(void);
void func_8005EE00(void);
void func_8005EF04(void);
void func_8005EF44(void);
void func_8005F22C(void);
void func_8005F26C(void);
void func_8005F478(void);
void func_8005FA20(void);
void func_8005F6E4(void);
void func_8005FDA0(void);
void func_8005FF68(void);
void func_80060104(void);
void func_80060504(void);
void func_80060150(void);

#endif /* MAIN_API_H */

#ifndef OVL_EVENT_H
#define OVL_EVENT_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_80120674 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120678 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206B8 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206BC /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120650 /* EVENT overlay data at this address, s32 (main_api.h: u8[]) */
#define MAIN_API_OVERRIDE_D_80120658 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_8012066C /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120698 /* EVENT overlay data at this address, s32 (main_api.h: u8) */
#define MAIN_API_OVERRIDE_D_80120694 /* EVENT overlay data at this address, s32 (main_api.h: u8[]) */
#define MAIN_API_OVERRIDE_D_801206A8 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206AC /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206C8 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801206E0 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801206F0 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120720 /* EVENT overlay data at this address, s16 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_801206DC /* EVENT overlay data at this address, s32 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_D_801206D8 /* EVENT overlay data at this address, s32 (main_api.h: u8[]) */
#define MAIN_API_OVERRIDE_D_8012121C /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_801207B0 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801207B4 /* EVENT overlay data at this address, s16 (main_api.h: s32) */
#define MAIN_API_OVERRIDE_D_801206EC /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120724 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120730 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80120734 /* EVENT overlay data at this address, s32 (main_api.h: s16) */
#define MAIN_API_OVERRIDE_D_80121874 /* EVENT overlay data at this address, s32 (main_api.h: u8) */
#define MAIN_API_OVERRIDE_D_801206B0 /* EVENT overlay data at this address, s32 (main_api.h: volatile s16) */

#include "common.h"
#include "game.h"

void func_80011DFC();
void func_8010B430();
s32 func_8001496C();
extern s32 D_80123E90;
extern s32 D_80123E94;
extern s32 D_80123E98;
extern s32 D_80123E9C;
extern s32 D_80123EA0;
extern s32 D_80123EA4;
extern s32 D_80123EA8;
void func_80012D64(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 D_801258C0;
extern s32 D_801258C4;
extern s32 D_801258C8;
extern s32 D_801258CC;
extern s32 D_801258D0;
extern s32 D_801258D4;
extern s32 D_801258D8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s16 D_8012437C;
extern s16 D_80124380;
extern s32 D_80124224;
extern s32 D_80124290;
extern s32 D_801242FC;
extern s16 D_80124384;
extern s16 D_80124388;
extern s32 D_801241D4;
extern s32 D_801241E4;
extern s32 D_801241F4;
extern u8 D_8012531C;
void func_8011C6C4(void);
extern s32 D_8012079C;
extern s32 D_801207A0;
extern s16 D_801207A4;
extern s32 D_801207A8;
extern s32 D_80120CA8;
extern s32 D_80120CAC;
extern s32 D_80120CB0;
extern s32 D_80120CB4;
extern s32 D_80120CB8;
extern s32 D_80120CBC;
extern s32 D_80120CC0;
extern s32 D_8012120C;
extern s32 D_80121210;
extern s32 D_80121214;
extern s32 D_80121218;
extern s32 D_8012121C;
extern s32 D_80121220;
extern s32 D_80121224;
extern s32 D_801215E0;
extern s32 D_801215E4;
extern s32 D_801215E8;
extern s32 D_801215EC;
extern s32 D_801215F0;
extern s32 D_801215F4;
extern s32 D_801215F8;
extern s32 D_80122730;
extern s32 D_80122734;
extern s32 D_80122738;
extern s32 D_8012273C;
extern s32 D_801252BC;
extern s32 D_801252C0;
extern s32 D_801252C4;
extern s32 D_801252C8;
extern s32 D_801252CC;
extern s32 D_801252D0;
extern s32 D_801252DC;
extern s32 D_801252E0;
extern s32 D_80120690;
extern s32 D_80120694;
extern s32 D_80120698;
extern s16 D_8012069C;
extern s32 D_801206A8;
extern s32 D_801206AC;
extern s32 D_801206B0;
extern s32 D_801206B4;
extern s32 D_801206B8;
extern s32 D_801206BC;
extern s32 D_801206C0;
extern s32 D_801206C4;
extern s16 D_801206C8;
extern s32 D_801206D0;
extern s32 D_801206D4;
extern s32 D_801206D8;
extern s32 D_801206DC;
extern s32 D_801206E0;
extern s32 D_801206EC;
extern s32 D_801206F0;
extern s32 D_801206F8;
extern s32 D_801206FC;
extern s32 D_80120700;
extern s32 D_80120704;
extern s32 D_80120708;
extern s32 D_80120714;
extern s32 D_80120718;
extern s32 D_8012071C;
extern s16 D_80120720;
extern s32 D_80120724;
extern s32 D_80120730;
extern s32 D_80120734;
extern s32 D_80120738;
extern s32 D_8012073C;
extern s32 D_80120740;
extern s32 D_80120744;
extern s32 D_80120748;
extern s16 D_8012074C;
extern s32 D_80120758;
extern s32 D_8012075C;
extern s32 D_80120764;
extern s32 D_80120768;
extern s32 D_80120774;
extern s32 D_8012077C;
extern s32 D_80120780;
extern s32 D_80120784;
extern s32 D_80120788;
extern s32 D_8012078C;
extern s32 D_80120790;
extern s32 D_80120C24;
extern s32 D_80120C28;
extern s32 D_80120C2C;
extern s16 D_80120C30;
extern s32 D_80120C34;
extern s32 D_80120C38;
extern s32 D_80120C3C;
extern s32 D_80120C40;
extern s32 D_80120C44;
extern s32 D_80120C48;
extern s32 D_80120C4C;
extern s32 D_80120C50;
extern s32 D_80120C54;
extern s32 D_80120C58;
extern s16 D_80120C5C;
extern s32 D_80120C60;
extern s32 D_80120C64;
extern s32 D_80120C68;
extern s32 D_80120C6C;
extern s32 D_80120C70;
extern s32 D_80120C74;
extern s32 D_80120C78;
extern s32 D_80120C7C;
extern s32 D_80120C80;
extern s32 D_80120C84;
extern s16 D_80120C88;
extern s32 D_80120C8C;
extern s32 D_80120C90;
extern s32 D_80120C94;
extern s32 D_80120C98;
extern s32 D_80120C9C;
extern s32 D_80120CA0;
extern s32 D_80120CA4;
extern s32 D_80120CC4;
extern s32 D_80120CC8;
extern s32 D_80120CCC;
extern s16 D_80120CD0;
extern s32 D_80120CD4;
extern s32 D_80120CD8;
extern s32 D_80120CDC;
extern s32 D_80120CE0;
extern s32 D_80120CE4;
extern s32 D_80120CE8;
extern s32 D_80120CEC;
extern s32 D_801211E0;
extern s32 D_801211E4;
extern s32 D_801211E8;
extern s16 D_801211EC;
extern s32 D_801211F0;
extern s32 D_801211F4;
extern s32 D_801211F8;
extern s32 D_801211FC;
extern s32 D_80121200;
extern s32 D_80121204;
extern s32 D_80121208;
extern s32 D_80121228;
extern s32 D_8012122C;
extern s32 D_80121230;
extern s16 D_80121234;
extern s32 D_8012123C;
extern s32 D_80121240;
extern s32 D_80121244;
extern s32 D_80121248;
extern s32 D_8012124C;
extern s32 D_80121250;
extern s32 D_80121288;
extern s32 D_8012128C;
extern s32 D_80121290;
extern s16 D_80121294;
extern s32 D_80121298;
extern s32 D_8012129C;
extern s32 D_801212A0;
extern s32 D_801212A4;
extern s32 D_801212A8;
extern s32 D_801212AC;
extern s32 D_801212B0;
extern s32 D_80121800;
extern s32 D_80121804;
extern s16 D_8012180C;
extern s32 D_80121810;
extern s32 D_80121814;
extern s32 D_8012181C;
extern s32 D_80121820;
extern s32 D_80121824;
extern s32 D_80121828;
extern s32 D_8012182C;
extern s32 D_80121830;
extern s32 D_80121834;
extern s16 D_80121838;
extern s32 D_80121840;
extern s32 D_80121844;
extern s32 D_80121848;
extern s32 D_8012184C;
extern s32 D_80121850;
extern s32 D_80121854;
extern s32 D_80121858;
extern s32 D_8012185C;
extern s32 D_80121868;
extern s32 D_8012186C;
extern s32 D_80121870;
extern s32 D_80121874;
extern s32 D_80121878;
extern s32 D_8012187C;
extern s32 D_80121880;
extern s32 D_80121888;
extern s32 D_8012188C;
extern s16 D_80121890;
extern s32 D_80121894;
extern s32 D_80121898;
extern s32 D_8012189C;
extern s32 D_801218A0;
extern s32 D_801218A4;
extern s32 D_801218A8;
extern s32 D_801218AC;
extern s32 D_801218B0;
extern s32 D_801218B4;
extern s32 D_801218B8;
extern s16 D_801218BC;
extern s32 D_801218C0;
extern s32 D_801218C4;
extern s32 D_801218C8;
extern s32 D_801218CC;
extern s32 D_801218D0;
extern s32 D_801218D4;
extern s32 D_801218D8;
extern s32 D_80121D20;
extern s32 D_80121D24;
extern s32 D_80121D28;
extern s16 D_80121D2C;
extern s32 D_80121D30;
extern s32 D_80121D34;
extern s32 D_80121D38;
extern s32 D_80121D3C;
extern s32 D_80121D40;
extern s32 D_80121D44;
extern s32 D_80121D48;
extern s32 D_80121D4C;
extern s32 D_80121D50;
extern s32 D_80121D54;
extern s16 D_80121D58;
extern s32 D_80121D5C;
extern s32 D_80121D60;
extern s32 D_80121D64;
extern s32 D_80121D68;
extern s32 D_80121D6C;
extern s32 D_80121D70;
extern s32 D_80121D74;
extern s32 D_80121D78;
extern s32 D_80121D7C;
extern s32 D_80121D80;
extern s16 D_80121D84;
extern s32 D_80121D88;
extern s32 D_80121D8C;
extern s32 D_80121D90;
extern s32 D_80121D94;
extern s32 D_80121D98;
extern s32 D_80121D9C;
extern s32 D_80121DA0;
extern s32 D_80121DA4;
extern s32 D_80121DA8;
extern s32 D_80121DAC;
extern s16 D_80121DB0;
extern s32 D_80121DB4;
extern s32 D_80121DB8;
extern s32 D_80121DBC;
extern s32 D_80121DC0;
extern s32 D_80121DC4;
extern s32 D_80121DC8;
extern s32 D_80121DCC;
extern s32 D_80121DD0;
extern s32 D_80121DD4;
extern s32 D_80121DD8;
extern s16 D_80121DDC;
extern s32 D_80121DE0;
extern s32 D_80121DE4;
extern s32 D_80121DE8;
extern s32 D_80121DEC;
extern s32 D_80121DF0;
extern s32 D_80121DF4;
extern s32 D_80121DF8;
extern s32 D_801221D0;
extern s32 D_801221D4;
extern s32 D_801221D8;
extern s16 D_801221DC;
extern s32 D_801221E0;
extern s32 D_801221E4;
extern s32 D_801221E8;
extern s32 D_801221EC;
extern s32 D_801221F0;
extern s32 D_801221F4;
extern s32 D_801221F8;
extern s32 D_801221FC;
extern s32 D_80122200;
extern s32 D_80122204;
extern s16 D_80122208;
extern s32 D_8012220C;
extern s32 D_80122210;
extern s32 D_80122214;
extern s32 D_80122218;
extern s32 D_8012221C;
extern s32 D_80122220;
extern s32 D_80122224;
extern s32 D_80122228;
extern s32 D_8012222C;
extern s32 D_80122230;
extern s16 D_80122234;
extern s32 D_80122238;
extern s32 D_8012223C;
extern s32 D_80122240;
extern s32 D_80122244;
extern s32 D_80122248;
extern s32 D_8012224C;
extern s32 D_80122250;
extern s32 D_80122254;
extern s32 D_80122258;
extern s32 D_8012225C;
extern s16 D_80122260;
extern s32 D_80122264;
extern s32 D_80122268;
extern s32 D_8012226C;
extern s32 D_80122270;
extern s32 D_80122274;
extern s32 D_80122278;
extern s32 D_8012227C;
extern s32 D_80122280;
extern s32 D_80122284;
extern s32 D_80122288;
extern s16 D_8012228C;
extern s32 D_80122290;
extern s32 D_80122294;
extern s32 D_80122298;
extern s32 D_8012229C;
extern s32 D_801222A0;
extern s32 D_801222A4;
extern s32 D_801222A8;
extern s32 D_80122680;
extern s32 D_80122684;
extern s32 D_80122688;
extern s16 D_8012268C;
extern s32 D_80122690;
extern s32 D_80122694;
extern s32 D_80122698;
extern s32 D_8012269C;
extern s32 D_801226A0;
extern s32 D_801226A4;
extern s32 D_801226A8;
extern s32 D_801226AC;
extern s32 D_801226B0;
extern s32 D_801226B4;
extern s16 D_801226B8;
extern s32 D_801226BC;
extern s32 D_801226C0;
extern s32 D_801226C4;
extern s32 D_801226C8;
extern s32 D_801226CC;
extern s32 D_801226D0;
extern s32 D_801226D4;
extern s32 D_801226D8;
extern s32 D_801226DC;
extern s32 D_801226E0;
extern s16 D_801226E4;
extern s32 D_801226E8;
extern s32 D_801226EC;
extern s32 D_801226F0;
extern s32 D_801226F4;
extern s32 D_801226F8;
extern s32 D_801226FC;
extern s32 D_80122700;
extern s32 D_80122704;
extern s32 D_80122708;
extern s32 D_8012270C;
extern s16 D_80122710;
extern s32 D_80122714;
extern s32 D_80122718;
extern s32 D_8012271C;
extern s32 D_80122720;
extern s32 D_80122724;
extern s32 D_80122728;
extern s32 D_8012272C;
extern s32 D_80122B30;
extern s32 D_80122B34;
extern s32 D_80122B38;
extern s16 D_80122B3C;
extern s32 D_80122B40;
extern s32 D_80122B44;
extern s32 D_80122B48;
extern s32 D_80122B4C;
extern s32 D_80122B50;
extern s32 D_80122B54;
extern s32 D_80122B58;
extern s32 D_80122B5C;
extern s32 D_80122B60;
extern s32 D_80122B64;
extern s16 D_80122B68;
extern s32 D_80122B6C;
extern s32 D_80122B70;
extern s32 D_80122B74;
extern s32 D_80122B78;
extern s32 D_80122B7C;
extern s32 D_80122B80;
extern s32 D_80122B84;
extern s32 D_80122B88;
extern s32 D_80122B8C;
extern s32 D_80122B90;
extern s16 D_80122B94;
extern s32 D_80122B98;
extern s32 D_80122B9C;
extern s32 D_80122BA0;
extern s32 D_80122BA4;
extern s32 D_80122BA8;
extern s32 D_80122BAC;
extern s32 D_80122BB0;
extern s32 D_80122BB4;
extern s32 D_80122BB8;
extern s32 D_80122BBC;
extern s16 D_80122BC0;
extern s32 D_80122BC4;
extern s32 D_80122BC8;
extern s32 D_80122BCC;
extern s32 D_80122BD0;
extern s32 D_80122BD4;
extern s32 D_80122BD8;
extern s32 D_80122BDC;
extern s32 D_80122BE0;
extern s32 D_80122BE4;
extern s32 D_80122BE8;
extern s16 D_80122BEC;
extern s32 D_80122BF0;
extern s32 D_80122BF4;
extern s32 D_80122BF8;
extern s32 D_80122BFC;
extern s32 D_80122C00;
extern s32 D_80122C04;
extern s32 D_80122C08;
extern s32 D_80122F9C;
extern s32 D_80122FA0;
extern s32 D_80122FA4;
extern s16 D_80122FA8;
extern s32 D_80122FAC;
extern s32 D_80122FB0;
extern s32 D_80122FB4;
extern s32 D_80122FB8;
extern s32 D_80122FBC;
extern s32 D_80122FC0;
extern s32 D_80122FC4;
extern s32 D_80122FC8;
extern s32 D_80122FCC;
extern s32 D_80122FD0;
extern s16 D_80122FD4;
extern s32 D_80122FD8;
extern s32 D_80122FDC;
extern s32 D_80122FE0;
extern s32 D_80122FE4;
extern s32 D_80122FE8;
extern s32 D_80122FEC;
extern s32 D_80122FF0;
extern s32 D_80122FF4;
extern s32 D_80122FF8;
extern s32 D_80122FFC;
extern s16 D_80123000;
extern s32 D_80123004;
extern s32 D_80123008;
extern s32 D_8012300C;
extern s32 D_80123010;
extern s32 D_80123014;
extern s32 D_80123018;
extern s32 D_8012301C;
extern s32 D_80123020;
extern s32 D_80123024;
extern s32 D_80123028;
extern s16 D_8012302C;
extern s32 D_80123030;
extern s32 D_80123034;
extern s32 D_80123038;
extern s32 D_8012303C;
extern s32 D_80123040;
extern s32 D_80123044;
extern s32 D_80123048;
extern s32 D_80123430;
extern s32 D_80123434;
extern s32 D_80123438;
extern s16 D_8012343C;
extern s32 D_80123440;
extern s32 D_80123444;
extern s32 D_80123448;
extern s32 D_8012344C;
extern s32 D_80123450;
extern s32 D_80123454;
extern s32 D_80123458;
extern s32 D_8012345C;
extern s32 D_80123460;
extern s32 D_80123464;
extern s16 D_80123468;
extern s32 D_8012346C;
extern s32 D_80123470;
extern s32 D_80123474;
extern s32 D_80123478;
extern s32 D_8012347C;
extern s32 D_80123480;
extern s32 D_80123484;
extern s32 D_80123488;
extern s32 D_8012348C;
extern s32 D_80123490;
extern s16 D_80123494;
extern s32 D_80123498;
extern s32 D_8012349C;
extern s32 D_801234A0;
extern s32 D_801234A4;
extern s32 D_801234A8;
extern s32 D_801234AC;
extern s32 D_801234B0;
extern s32 D_801234B4;
extern s32 D_801234B8;
extern s32 D_801234BC;
extern s16 D_801234C0;
extern s32 D_801234C4;
extern s32 D_801234C8;
extern s32 D_801234CC;
extern s32 D_801234D0;
extern s32 D_801234D4;
extern s32 D_801234D8;
extern s32 D_801234DC;
extern s32 D_801234E0;
extern s32 D_801234E4;
extern s32 D_801234E8;
extern s16 D_801234EC;
extern s32 D_801234F0;
extern s32 D_801234F4;
extern s32 D_801234F8;
extern s32 D_801234FC;
extern s32 D_80123500;
extern s32 D_80123504;
extern s32 D_80123508;
extern s32 D_80123880;
extern s32 D_80123884;
extern s32 D_80123888;
extern s16 D_8012388C;
extern s32 D_80123890;
extern s32 D_80123894;
extern s32 D_80123898;
extern s32 D_8012389C;
extern s32 D_801238A0;
extern s32 D_801238A4;
extern s32 D_801238A8;
extern s32 D_801238AC;
extern s32 D_801238B0;
extern s32 D_801238B4;
extern s16 D_801238B8;
extern s32 D_801238BC;
extern s32 D_801238C0;
extern s32 D_801238C4;
extern s32 D_801238C8;
extern s32 D_801238CC;
extern s32 D_801238D0;
extern s32 D_801238D4;
extern s32 D_801238D8;
extern s32 D_801238DC;
extern s32 D_801238E0;
extern s16 D_801238E4;
extern s32 D_801238E8;
extern s32 D_801238EC;
extern s32 D_801238F0;
extern s32 D_801238F4;
extern s32 D_801238F8;
extern s32 D_801238FC;
extern s32 D_80123900;
extern s32 D_80123904;
extern s32 D_80123908;
extern s32 D_8012390C;
extern s16 D_80123910;
extern s32 D_80123914;
extern s32 D_80123918;
extern s32 D_8012391C;
extern s32 D_80123920;
extern s32 D_80123924;
extern s32 D_80123928;
extern s32 D_8012392C;
extern s32 D_80123930;
extern s32 D_80123934;
extern s32 D_80123938;
extern s16 D_8012393C;
extern s32 D_80123940;
extern s32 D_80123944;
extern s32 D_80123948;
extern s32 D_8012394C;
extern s32 D_80123950;
extern s32 D_80123954;
extern s32 D_80123958;
extern s32 D_8012395C;
extern s32 D_80123960;
extern s32 D_80123964;
extern s16 D_80123968;
extern s32 D_8012396C;
extern s32 D_80123970;
extern s32 D_80123974;
extern s32 D_80123978;
extern s32 D_8012397C;
extern s32 D_80123980;
extern s32 D_80123984;
extern s32 D_80123EAC;
extern s32 D_80123EB0;
extern s32 D_80123EB4;
extern s16 D_80123EB8;
extern s32 D_80123EBC;
extern s32 D_80123EC0;
extern s32 D_80123EC4;
extern s32 D_80123EC8;
extern s32 D_80123ECC;
extern s32 D_80123ED0;
extern s32 D_80123ED4;
extern s32 D_801240F0;
extern s32 D_801240F4;
extern s32 D_801240F8;
extern s16 D_801240FC;
extern s32 D_80124100;
extern s32 D_80124104;
extern s32 D_80124108;
extern s32 D_8012410C;
extern s32 D_80124110;
extern s32 D_80124114;
extern s32 D_80124118;
extern s32 D_801241D0;
extern s32 D_801241D8;
extern s32 D_801241DC;
extern s32 D_801241E0;
extern s32 D_801241E8;
extern s32 D_801241EC;
extern s32 D_801241F0;
extern s32 D_801241F8;
extern s32 D_801241FC;
extern s32 D_80120BF0;
extern s32 D_80120BF4;
extern s32 D_80120BF8;
extern s16 D_80120BFC;
extern s32 D_80120C00;
extern s32 D_80120C04;
extern s32 D_80120C08;
extern s32 D_80120C0C;
extern s32 D_80120C10;
extern s32 D_80120C14;
extern s32 D_80120C18;
extern s32 D_80120C1C;
extern s32 D_80120C20;
extern s32 D_80121254;
extern s32 D_80121258;
extern s32 D_8012125C;
extern s16 D_80121260;
extern s32 D_80121264;
extern s32 D_80121268;
extern s32 D_8012126C;
extern s32 D_80121270;
extern s32 D_80121274;
extern s32 D_80121278;
extern s32 D_80121280;
extern s32 D_80121284;
extern s32 D_80122F70;
extern s32 D_80122F74;
extern s32 D_80122F78;
extern s16 D_80122F7C;
extern s32 D_80122F80;
extern s32 D_80122F84;
extern s32 D_80122F88;
extern s32 D_80122F8C;
extern s32 D_80122F90;
extern s32 D_80122F94;
extern s32 D_80122F98;
extern s32 D_80125270;
extern s32 D_80125274;
extern s32 D_80125278;
extern s32 D_80125280;
extern s32 D_80125284;
extern s32 D_80125288;
extern s32 D_8012528C;
extern s32 D_80125290;
extern s32 D_80125294;
extern s32 D_80125298;
extern s32 D_8012529C;
extern s32 D_801252A0;
extern s32 D_801252A4;
extern s32 D_801252A8;
extern s32 D_801252AC;
extern s32 D_801252B0;
extern s32 D_801252B4;
extern s32 D_801252B8;
extern s32 D_801251D0;
extern s32 D_801251D4;
extern s32 D_801251D8;
extern s32 D_801251DC;
extern s32 D_801251E0;
extern s32 D_801251E4;
extern s32 D_801251E8;
extern s32 D_801251EC;
extern s32 D_801251F0;
extern s32 D_801251F4;
extern s32 D_801251F8;
extern s32 D_801251FC;
extern s32 D_80125200;
extern s32 D_80125204;
extern s32 D_80125208;
extern s32 D_8012520C;
extern s32 D_80125210;
extern s32 D_80125214;
extern s32 D_80125218;
extern s32 D_8012521C;
extern s32 D_80125220;
extern s32 D_80125224;
extern s32 D_80125228;
extern s32 D_8012522C;
extern s32 D_80125230;
extern s32 D_80125234;
extern s32 D_80125238;
extern s32 D_8012523C;
extern s32 D_80125240;
extern s32 D_80125244;
extern s32 D_80125248;
extern s32 D_8012524C;
extern s32 D_80125250;
extern s32 D_80125254;
extern s32 D_80125258;
extern s32 D_8012525C;
extern s32 D_80125260;
extern s32 D_80125264;
extern s32 D_80125268;
extern s32 D_8012526C;
extern s32 D_80124200;
extern s32 D_80124204;
extern s32 D_80124208;
extern s32 D_8012420C;
extern s32 D_80124210;
extern s32 D_80124214;
extern s32 D_80124218;
extern s32 D_8012421C;
extern s32 D_80124220;
extern s32 D_80124228;
extern s32 D_8012422C;
extern s32 D_80124230;
extern s32 D_80124234;
extern s32 D_80124238;
extern s32 D_8012423C;
extern s32 D_80124244;
extern s32 D_80124248;
extern s32 D_8012424C;
extern s32 D_80124250;
extern s32 D_80124254;
extern s32 D_80124258;
extern s32 D_8012425C;
extern s32 D_80124260;
extern s32 D_80124264;
extern s32 D_80124268;
extern s32 D_8012426C;
extern s32 D_80124270;
extern s32 D_80124274;
extern s32 D_80124278;
extern s32 D_8012427C;
extern s32 D_80124280;
extern s32 D_80124284;
extern s32 D_80124288;
extern s32 D_8012428C;
extern s32 D_80124294;
extern s32 D_80124298;
extern s32 D_8012429C;
extern s32 D_801242A0;
extern s32 D_801242A4;
extern s32 D_801242A8;
extern s32 D_801242B0;
extern s32 D_801242B4;
extern s32 D_801242B8;
extern s32 D_801242BC;
extern s32 D_801242C0;
extern s32 D_801242C4;
extern s32 D_801242C8;
extern s32 D_801242CC;
extern s32 D_801242D0;
extern s32 D_801242D4;
extern s32 D_801242D8;
extern s32 D_801242DC;
extern s32 D_801242E0;
extern s32 D_801242E4;
extern s32 D_801242E8;
extern s32 D_801242EC;
extern s32 D_801242F0;
extern s32 D_801242F4;
extern s32 D_801242F8;
extern s32 D_80124300;
extern s32 D_80124304;
extern s32 D_80124308;
extern s32 D_8012430C;
extern s32 D_80124310;
extern s32 D_80124314;
extern s32 D_8012431C;
extern s32 D_80124320;
extern s32 D_80124324;
extern s32 D_80124328;
extern s32 D_8012432C;
extern s32 D_80124330;
extern s32 D_80124334;
extern s32 D_80124338;
extern s32 D_8012433C;
extern s32 D_80124340;
extern s32 D_80124240;
extern s32 D_801242AC;
extern s32 D_80124318;
extern s32 D_801202D0;
extern s32 D_801202D4;
extern s32 D_801202D8;
extern s32 D_801202DC;
extern s32 D_801202E0;
extern s32 D_801202E4;
extern s32 D_801202E8;
extern s32 D_801202EC;
extern s32 D_801202F0;
extern s32 D_801202F4;
extern s32 D_801202F8;
extern s32 D_801202FC;
extern s32 D_80120300;
extern s32 D_80120304;
extern s32 D_80120308;
extern s32 D_8012030C;
extern s32 D_80120310;
extern s32 D_80120314;
extern s32 D_80120318;
extern s32 D_8012031C;
extern s32 D_80120320;
extern s32 D_80120324;
extern s32 D_80120328;
extern s32 D_8012032C;
extern s32 D_80120330;
extern s32 D_80120334;
extern s32 D_80120338;
extern s32 D_8012033C;
extern s32 D_80120340;
extern s32 D_80120344;
extern s32 D_80120348;
extern s32 D_8012034C;
extern s32 D_80120350;
extern s32 D_80120354;
extern s32 D_80120358;
extern s32 D_8012035C;
extern s32 D_80120360;
extern s32 D_80120364;
extern s32 D_80120368;
extern s32 D_8012036C;
extern s32 D_80120370;
extern s32 D_80120374;
extern s32 D_80120378;
extern s32 D_8012037C;
extern s32 D_80120380;
extern s32 D_80120384;
extern s32 D_80120388;
extern s32 D_8012038C;
extern s32 D_80120390;
extern s32 D_80120394;
extern s32 D_80120398;
extern s32 D_8012039C;
extern s32 D_801203A0;
extern s32 D_801203A4;
extern s32 D_801203A8;
extern s32 D_801203AC;
extern s32 D_801203B0;
extern s32 D_801203B4;
extern s32 D_801203B8;
extern s32 D_801203BC;
extern s32 D_801203C0;
extern s32 D_801203C4;
extern s32 D_801203C8;
extern s32 D_801203CC;
extern s32 D_801203D0;
extern s32 D_801203D4;
extern s32 D_801203D8;
extern s32 D_801203DC;
extern s32 D_801203E0;
extern s32 D_801203E4;
extern s32 D_801203E8;
extern s32 D_801203EC;
extern s32 D_801203F0;
extern s32 D_801203F4;
extern s32 D_801203F8;
extern s32 D_801203FC;
extern s32 D_80120400;
extern s32 D_80120404;
extern s32 D_80120408;
extern s32 D_8012040C;
extern s32 D_80120410;
extern s32 D_80120414;
extern s32 D_80120418;
extern s32 D_8012041C;
extern s32 D_80120420;
extern s32 D_80120424;
extern s32 D_80120428;
extern s32 D_8012042C;
extern s32 D_80120430;
extern s32 D_80120434;
extern s32 D_80120438;
extern s32 D_8012043C;
extern s32 D_80120440;
extern s32 D_80120444;
extern s32 D_80120448;
extern s32 D_8012044C;
extern s32 D_80120450;
extern s32 D_80120454;
extern s32 D_80120458;
extern s32 D_8012045C;
extern s32 D_80120460;
extern s32 D_80120464;
extern s32 D_80120468;
extern s32 D_8012046C;
extern s32 D_80120470;
extern s32 D_80120474;
extern s32 D_80120478;
extern s32 D_8012047C;
extern s32 D_80120480;
extern s32 D_80120484;
extern s32 D_80120488;
extern s32 D_8012048C;
extern s32 D_80120490;
extern s32 D_80120494;
extern s32 D_80120498;
extern s32 D_8012049C;
extern s32 D_801204A0;
extern s32 D_801204A4;
extern s32 D_801204A8;
extern s32 D_801204AC;
extern s32 D_801204B0;
extern s32 D_801204B4;
extern s32 D_801204B8;
extern s32 D_801204BC;
extern s32 D_801204C0;
extern s32 D_801204C4;
extern s32 D_801204C8;
extern s32 D_801204CC;
extern s32 D_801204D0;
extern s32 D_801204D4;
extern s32 D_801204D8;
extern s32 D_801204DC;
extern s32 D_801204E0;
extern s32 D_801204E4;
extern s32 D_801204E8;
extern s32 D_801204EC;
extern s32 D_801204F0;
extern s32 D_801204F4;
extern s32 D_801204F8;
extern s32 D_801204FC;
extern s32 D_80120500;
extern s32 D_80120504;
extern s32 D_80120508;
extern s32 D_8012050C;
extern s32 D_80120510;
extern s32 D_80120514;
extern s32 D_80120518;
extern s32 D_8012051C;
extern s32 D_80120520;
extern s32 D_80120524;
extern s32 D_80120528;
extern s32 D_8012052C;
extern s32 D_80120530;
extern s32 D_80120534;
extern s32 D_80120538;
extern s32 D_8012053C;
extern s32 D_80120540;
extern s32 D_80120544;
extern s32 D_80120548;
extern s32 D_8012054C;
extern s32 D_80120550;
extern s32 D_80120554;
extern s32 D_80120558;
extern s32 D_8012055C;
extern s32 D_80120560;
extern s32 D_80120564;
extern s32 D_80120568;
extern s32 D_8012056C;
extern s32 D_80120570;
extern s32 D_80120574;
extern s32 D_80120578;
extern s32 D_8012057C;
extern s32 D_80120580;
extern s32 D_80120584;
extern s32 D_80120588;
extern s32 D_8012058C;
extern s32 D_80120590;
extern s32 D_80120594;
extern s32 D_80120598;
extern s32 D_8012059C;
extern s32 D_801205A0;
extern s32 D_801205A4;
extern s32 D_801205A8;
extern s32 D_801205AC;
extern s32 D_801205B0;
extern s32 D_801205B4;
extern s32 D_801205B8;
extern s32 D_801205BC;
extern s32 D_801205C0;
extern s32 D_801205C4;
extern s32 D_801205C8;
extern s32 D_801205CC;
extern s32 D_801205D0;
extern s32 D_801205D4;
extern s32 D_801205D8;
extern s32 D_801205DC;
extern s32 D_801205E0;
extern s32 D_801205E4;
extern s32 D_801205E8;
extern s32 D_801205EC;
extern s32 D_801205F0;
extern s32 D_801205F4;
extern s32 D_801205F8;
extern s32 D_801205FC;
extern s32 D_80120600;
extern s32 D_80120604;
extern s32 D_80120608;
extern s32 D_8012060C;
extern s32 D_80120610;
extern s32 D_80120614;
extern s32 D_80120618;
extern s32 D_8012061C;
extern s32 D_80120620;
extern s32 D_80120624;
extern s32 D_80120628;
extern s32 D_8012062C;
extern s32 D_80120630;
extern s32 D_80120634;
extern s32 D_80120638;
extern s32 D_8012063C;
extern s32 D_80120640;
extern s32 D_80120644;
extern s32 D_80120648;
extern s32 D_8012064C;
extern s32 D_80120650;
extern s32 D_80120658;
extern s32 D_8012066C;
extern s32 D_80120670;
extern s32 D_80120674;
void func_80011E28();
void func_80011EC4();
void func_800335E0();
void func_800201FC();
extern u8 D_801243B4;
extern u8 D_801252E4;
extern u8 D_80125320;
extern u8 D_80125324;
extern u8 D_801252F4;
extern u8 D_801252F8;
extern u8 D_801252FC;
extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;
void func_80015D28();
void func_800F7220();
void func_800F72D0();
void func_800F7380();
void func_800F7430();
void func_800F74E0();
void func_800F7590();
void func_800F9900();
void func_800F9C4C();
void func_800F99CC();
void func_800F9A7C();
void func_800F9B2C();
void func_800F9BDC();
void func_800FBBF0();
void func_800FBCA0();
void func_800FBD10();
void func_800FBDC0();
void func_800FD2F0();
void func_800FDC20();
void func_800FDCD0();
void func_800FDD80();
void func_800FDE30();
void func_800FDEE0();
void func_800FFBE0();
void func_800FFC90();
void func_800FFD40();
void func_800FFDF0();
void func_800FFEA0();
void func_801011A0();
void func_80101250();
void func_80101300();
void func_801013B0();
void func_80101460();
void func_801024B0();
void func_80102560();
void func_80102610();
void func_801026C0();
void func_80102770();
void func_80103C80();
void func_80103D30();
void func_80103DE0();
void func_80103E90();
void func_80103F40();
void func_801050D0();
void func_801051A0();
void func_80105250();
void func_80105300();
void func_80106C20();
void func_80106CD0();
void func_80106D80();
void func_80106E30();
void func_80106EE0();
void func_80107FC0();
void func_80108330();
void func_80108070();
void func_80108120();
void func_801081D0();
void func_8010A280();
void func_8010B360();
void func_801101B4();
void func_80015FE0();
void func_800F9DF8();
void func_800FA5C0();
void func_800FA6A0();
void func_80118B4C();
void func_8010BB90();
void func_8010FE10();
void func_80108280();
void func_80110094();
void func_800143DC();
void func_801053B0();
void func_800137AC();
void func_8001A16C();
void func_80108B24();
void func_800F7074();
void func_801189B4();
void func_80118A60();
extern u8 D_801243B0;
void func_801190F0();
void func_800F6000();
extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
void func_80114B0C();
void func_800189C4();
void func_8001A0CC();
void func_800F9730();
void func_8001A1EC();
s32 func_8002328C();
extern u8 D_80125300;
s32 func_8002336C();
extern u8 D_80125304;
extern u8 D_80125308;
void func_80033E88();
extern s8 D_80125310;
void func_801103A0();
void func_800FBE90();
void func_800F7640();
void func_800F77A0();
void func_800F8C2C();
void func_801055D0();
void func_80105910();
void func_80105DF8();

void func_80107090(void);

void func_80100070();

void func_80100254();
void func_80101610();
void func_8010A4F0();
void func_8010A5AC();
void func_8010AE7C();
void func_8010B490();

extern u8 D_8012436C;
extern s16 D_80124370;
extern u8 D_80125314;
void func_800FA25C();

extern u8 D_80125328;
void func_800FD4FC();
void func_800FD604();
void func_800FD6E0();
extern s8 D_801252E8;
extern s16 D_80124344;
extern u8 D_80124374;
extern s16 D_80124378;
extern u8 D_80125318;
void func_800FC040();
void func_80102950();
void func_80102A24();
void func_801085C0();
void func_80108680();
void func_801087F8();
extern s8 D_80123988;
extern s32 D_8012398C;
void func_80108D00(void);
void func_801087A8(void);
void func_800FCF40(void);
void func_800FC46C(void);
void func_80103208(void);
void func_80109948(void);
void func_800FC994(void);
void func_800FD1C0(void);
void func_800FBFE0();
void func_800FCD0C();
void func_800FCDD8();
void func_800FD00C();
void func_80102890();
void func_801028F0();
void func_801034E8();
void func_801035BC();
void func_801039E8();
void func_80103ABC();
void func_801084A0();
void func_80108500();
void func_80109E14();
void func_80109EE8();
void func_8010A008();
void func_8010A0E4();
void func_80108560();
extern s16 D_8012435C;
extern u8 D_80124360;
void func_800FD3DC();
void func_800FD43C();
void func_800FD49C();

extern s16 D_801207AC;
extern s16 D_801207B0;
extern s16 D_801207B4;
void func_801015B0(void);
void func_80101F24(void);
void func_80102118(void);
void func_801021FC(void);
void func_801022C0(void);
void func_80036A8C();
void func_800372E0(s32 a);
void func_80035774(void);
void func_800358D0(void);
void func_8010F4B8(void);
s32 func_80116360(void);
void func_80116460(void);
void func_8011651C(void);
void func_80118F40(void);
void func_80118FC8(void);
s32 func_80117A68();
s32 func_80117A10(void);
void func_80117CEC(void);
extern u8 D_80124364;
extern u8 D_80124368;
void func_8011838C(void);
void func_800FDBF4(void);
void func_8010BB64(void);

void func_800FCFA4(void);
void func_800FCFC4(void);

void func_80107030(void);

extern u8 D_8012411C;
void func_80108AD8(void);
void func_80108DD4(void);
void func_80109FB0(void);
void func_800F8970(void);

void func_8010A3D0(void);
void func_8010A430(void);
void func_8010A490(void);
void func_8010AD70(void);
void func_8010B29C(void);
void func_800F9DBC(void);
void func_800FA584(void);
void func_800FB67C(void);
void func_800FB728(void);
void func_800FB7E8(void);
void func_800FB920(void);
void func_800FA664(void);
void func_80107CEC(void);
void func_80107E20(void);
void func_80107A28(void);
void func_80107BB0(void);
extern s8 D_801212B4;
void func_80104090(void);
void func_801049B4(void);
void func_80104B50(void);
void func_80104E64(void);
void func_80104F80(void);
void func_80105510(void);
void func_80105570(void);
void func_80106834(void);
void func_80106908(void);
void func_80106A8C(void);
void func_80106B48(void);
void func_80100010(void);
void func_801001F4(void);
void func_80100CEC(void);
void func_80100E30(void);
void func_80100F04(void);
void func_80100FC0(void);
void func_80101104(void);
void func_801010B4(void);
void func_80104F30(void);

#endif /* OVL_EVENT_H */
extern s16 D_80124348;
void func_800FE030(void);
void func_800FEA38(void);
void func_800FF61C();
void func_800FF770();
void func_800FF82C();
void func_800F7740(void);
void func_800F84E0(void);
void func_800F86C4(void);
void func_800F8B50(void);
void func_800F93FC(void);
void func_800F9558(void);
extern s8 D_801218DC[];

#ifndef OVL_EVENT_W8_3_TYPES_H
#define OVL_EVENT_W8_3_TYPES_H
typedef struct EventCallbackTable47 {
    /* 0x00 */ void (*callback[47])(s32);
} EventCallbackTable47; /* size 0xBC */
extern EventCallbackTable47 D_80120E24;
void func_800FA850(void);
void func_80105B00(void);
void func_80105BC4(void);

#endif /* OVL_EVENT_W8_3_TYPES_H */

#ifndef OVL_EVENT_RECORDS_T9740
#define OVL_EVENT_RECORDS_T9740
/* View of the 0x44-byte D_800E9620 records 30/31 (T-9740).
 * D_800EC190 uses the shared Rec24 game-record layout. */
typedef struct EventFlagRecord44 {
    /* 0x00 */ u8 pad0[3];
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 pad4[3];
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 pad8[0x3C];
} EventFlagRecord44; /* size 0x44 */
s32 func_8010B8A4(void);
#endif

#ifndef EVENT_TABLE_E7_T9940
#define EVENT_TABLE_E7_T9940
typedef struct EventTableE7 {
    /* 0x00 */ u8 unk_00[11][21];
} EventTableE7; /* size 0xE7; inline copy includes a three-byte tail */
extern EventTableE7 D_8012536C;
#endif


#ifndef OVL_EVENT_T10040
#define OVL_EVENT_T10040
/* Forward declarations for the original object, T-10040. */
void func_8011C650(void);
u16 func_8011C750(void);
void func_8011D5C4(void);
void func_8011DDB8(void);
void func_8011EDFC(void);

/* T-10040: dialogue data and object-local forward declarations. */
s32 func_8011ED50(s16 arg0);

/* Stage/fade view at D_800B1AF0; store/load ordering in 8011EDFC. */
typedef struct EventTransitionState {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 pad04[2];
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 pad07;
} EventTransitionState; /* size 0x08; T-10040 */
extern s16 D_801258E4[3];
extern s16 D_801258EC[3];
extern s16 D_801258F4[3];
extern s16 D_801258FC[3];
extern u16 D_801258DC;
typedef struct EvFnTbl53 { void (*f[53])(); } EvFnTbl53;
extern EvFnTbl53 D_801225A8;
typedef struct EvFnTbl44 { void (*f[44])(); } EvFnTbl44;
extern EvFnTbl44 D_801228E0;
void func_8011B5A8(void);

#endif

#ifndef OVL_EVENT_T10630_H
#define OVL_EVENT_T10630_H
/* Movie control bytes at D_800F1990, cleared by func_8011DDB8. */
typedef struct EventMovieWork {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ u8 pad0C[12];
    /* 0x18 */ s8 unk_18;
    /* 0x19 */ s8 unk_19;
    /* 0x1A */ s8 unk_1A;
    /* 0x1B */ u8 pad1B;
} EventMovieWork; /* size 0x1C, minimum observed extent */
s32 func_80015A44();
void func_8002469C();
void func_80027438();
void func_8002746C();
void func_8002755C();
void func_800352B4();
#endif

/* T-10630: signed UV step and RNG seed helper in the EVENT object. */
extern s16 D_801258E0;
void func_8011F530(u32 seed);

#ifndef EVENT_SELECTION_TYPES_T10730
#define EVENT_SELECTION_TYPES_T10730
/* T-10730: unsigned triples selected by girl, option, then its two-state flag. */
typedef struct EventChoice3 {
    /* 0x00 */ u8 value[3];
} EventChoice3; /* size 0x03 */

typedef struct EventPlaceChoices18 {
    /* 0x00 */ EventChoice3 choice[3][2];
} EventPlaceChoices18; /* size 0x12 */

typedef struct EventRideChoices36 {
    /* 0x00 */ EventChoice3 choice[6][2];
} EventRideChoices36; /* size 0x24 */

#endif

#ifndef OVL_EVENT_MOTION_T10730
#define OVL_EVENT_MOTION_T10730
/* Record view recovered from func_8011B848, D_800E9620.
 * Records 96..99 use signed high halves of the two fixed-point words.
 * Byte names remain provisional; existing raw-array declarations are views. */
typedef struct EventMotion44 {
    /* 0x00 */ u8 pad00[2];
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 pad04[3];
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ u8 pad0A[0xC];
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ u8 pad1A[0xA];
    /* 0x24 */ GsWord unk_24;
    /* 0x28 */ GsWord unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ u8 pad30[0x14];
} EventMotion44; /* size 0x44 */
typedef struct EventMuseumChoices15 {
    /* 0x00 */ EventChoice3 choice[5];
} EventMuseumChoices15; /* size 0x0F */
#endif

#ifndef OVL_EVENT_STATE_T10730
#define OVL_EVENT_STATE_T10730
/* T-10730: legacy EVENT state, offsets relative to D_800B0860. */
typedef struct EventStateView {
    /* 0x000 */ u8 pad_000[0x35];
    /* 0x035 */ u8 unk_035;
    /* 0x036 */ u8 unk_036;
    /* 0x037 */ u8 unk_037;
    /* 0x038 */ u8 pad_038[0xAC];
    /* 0x0E4 */ Rec38Flags unk_0E4;
    /* 0x0E8 */ u8 pad_0E8[0xBC];
    /* 0x1A4 */ Rec34 unk_1A4[12];
    /* 0x414 */ u8 pad_414[0x1B6];
    /* 0x5CA */ u8 unk_5CA[0x4B];
    /* 0x615 */ u8 pad_615[0xB3];
    /* 0x6C8 */ s8 unk_6C8;
    /* 0x6C9 */ s8 unk_6C9;
    /* 0x6CA */ s8 unk_6CA;
    /* 0x6CB */ u8 pad_6CB[0x81B];
    /* 0xEE6 */ u8 unk_EE6;
    /* 0xEE7 */ u8 pad_EE7[0x150];
    /* 0x1037 */ u8 unk_1037;
    /* 0x1038 */ u8 pad_1038[0x25D];
    /* 0x1295 */ u8 unk_1295;
    /* 0x1296 */ u8 unk_1296;
    /* 0x1297 */ u8 pad_1297;
} EventStateView; /* minimum size 0x1298 */
#endif

#ifndef OVL_EVENT_MOVIE_STATE_T10830_H
#define OVL_EVENT_MOVIE_STATE_T10830_H
/* Scene-control view at D_800B1AF0. Extends EventTransitionState's
 * observed prefix with the page selector used by func_8011C750. */
typedef struct EventMovieState {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 pad04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 pad07[2];
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ u8 pad0A[2];
} EventMovieState; /* size 0x0C; observed fields through byte 0x09 */
#endif

#ifndef OVL_EVENT_PARTICLE_T10870_H
#define OVL_EVENT_PARTICLE_T10870_H
/* func_8011B190 initializes records 96..105 at D_800E9620. The same
 * allocation's record 31 uses byte 0x43. Coordinate high halves are signed. */
typedef struct EventParticleView44 {
    /* 0x00 */ u8 pad00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ u8 unk_07;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u8 pad1E[6];
    /* 0x24 */ GsWord unk_24;
    /* 0x28 */ GsWord unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ u8 pad3C[7];
    /* 0x43 */ u8 unk_43;
} EventParticleView44; /* size 0x44 */

/* Partial state view at D_800B0860. The word table's observed accesses
 * extend through element 57; its full allocation size is not yet known. */
typedef struct EventEffectTables {
    /* 0x0000 */ u8 pad0000[0x1031];
    /* 0x1031 */ u8 unk_1031[6];
    /* 0x1037 */ u8 pad1037[0x371];
    /* 0x13A8 */ s32 unk_13A8[58];
} EventEffectTables; /* size 0x1490, minimum observed extent */

extern s32 D_801257DC[10];
extern s16 D_8012527C;
extern s32 D_8012585C;
extern s16 D_80125860;
extern u8 D_80125864;
/* func_8011BE00 preserves twenty horizontal coordinate words. */
extern GsWord D_80125868[20];
#endif

#ifndef OVL_EVENT_RESULT_ROW_T10870_H
#define OVL_EVENT_RESULT_ROW_T10870_H
/* Three signed results at D_8012438C and D_8012439C. The rows are
 * separate allocations/views, not adjacent elements of one array. */
typedef struct EventResultRow6 {
    /* 0x00 */ s16 value[3];
} EventResultRow6; /* size 0x06 */
#endif
typedef struct EventBirthday4 {
    /* 0x00 */ u32 month : 4;
    /* 0x00 */ u32 day : 5;
    /* 0x00 */ u32 rest : 23;
} EventBirthday4; /* size 0x04 */
extern s8 D_8012532C;
extern s16 D_80124354;
extern u8 D_80124358;
extern u8 D_801243AC;
extern s16 D_801243A2;
extern s8 D_8012530C;
void func_80011E8C();

typedef struct EventTable2D6 {
    /* 0x00 */ u8 unk_00[22][33];
} EventTable2D6; /* size 0x2D6 */
extern EventTable2D6 D_80125504;
extern s16 D_8012434C;
extern s16 D_80124350;
void func_80115C18(void);
