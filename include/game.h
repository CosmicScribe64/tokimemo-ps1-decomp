#ifndef GAME_H
#define GAME_H

/* T-10890: parameter/name display view of the D_8011ECD0 record pool. */

#include "common.h"
#include "libgpu.h"
#include "main_api.h"

/* Game types (structs) of the main exe. The declarations of its functions and globals live in
 * main_api.h (T-3340); this file only adds types that need a struct definition. */

/* 8-byte record of the array at D_800B3DC0 (stride 8; see k_disp_switch). */
typedef struct Entry8 {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ u8 unk_04;
    /* 0x05 */ u8 unk_05;
    /* 0x06 */ u8 unk_06;
    /* 0x07 */ s8 unk_07;
} Entry8; /* size 0x08 */

/* Sync-wait object of strSync (field meanings unknown). */
typedef struct SyncObj {
    /* 0x00 */ u8 unk_00[0x10];
    /* 0x10 */ Entry8 tbl[2];
    /* 0x20 */ s32 idx;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 unk_28[4];
    /* 0x2C */ s32 flag;
} SyncObj; /* size 0x30 */

/* Stack request of the memory-card file functions (func_80054694, func_80054704): name buffer plus a retry
 * counter; layout read from the frame (counter at +0x20 in func_80054704). */
typedef struct FileReq {
    /* 0x00 */ u8 name[32];
    /* 0x20 */ s32 retry;
} FileReq; /* size 0x24 */

typedef struct MovieVlcState {
    /* 0x00 */ s32 buffers[2];
    /* 0x08 */ s32 index;
} MovieVlcState; /* accessed prefix, size 0x0C. T-9570 */

#endif /* GAME_H */

#ifndef GAME_DISPLAY_ACTOR44_T10890
#define GAME_DISPLAY_ACTOR44_T10890
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
    /* 0x0A */ u8 pad_0A[2];
    /* 0x0C */ u8 *unk_0C;
    /* 0x10 */ u8 *unk_10;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ u8 pad_1C[2];
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ u8 pad_20[2];
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ u8 pad_24[2];
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ u8 pad_28[2];
    /* 0x2A */ s16 unk_2A;
    /* 0x2C */ u8 pad_2C[2];
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ u8 pad_30[3];
    /* 0x33 */ u8 unk_33;
    /* 0x34 */ u8 *unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ u8 pad_3C[7];
    /* 0x43 */ u8 unk_43;
} DisplayActor44; /* size 0x44 */

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

#ifndef GAME_STR_DEC_ENV_T9930
#define GAME_STR_DEC_ENV_T9930
typedef struct StrDecEnv30 {
    /* 0x00 */ u8 *buffer0;
    /* 0x04 */ u8 *buffer1;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u8 *buffer2;
    /* 0x10 */ RECT rect0;
    /* 0x18 */ RECT rect1;
    /* 0x20 */ s32 index;
    /* 0x24 */ RECT rect2;
    /* 0x2C */ s32 flag;
} StrDecEnv30; /* size 0x30; decoder environment, T-9930. */
#endif


#ifndef GAME_GS_BYTE_NIBBLES_T9910
#define GAME_GS_BYTE_NIBBLES_T9910
/* T-9910: byte view for the packed nibbles at GameState offset 0x0F5. */
typedef struct GsByteNibbles {
    /* 0x00, bits 0-3 */ u8 low : 4;
    /* 0x00, bits 4-7 */ u8 high : 4;
} GsByteNibbles; /* size 0x01 */
#endif
