#ifndef GAME_H
#define GAME_H

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

#endif /* GAME_H */
