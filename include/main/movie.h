#ifndef MAIN_MOVIE_H
#define MAIN_MOVIE_H

#include "main/movie.h"

#include "common.h"
#include "libgpu.h"

/* PsyQ movie sample DECENV (strSetDefDecEnv, strCallback, strNext, strSync), T-9930, T-17014. */
typedef struct MovieDecodeEnvironment {
    /* 0x00 */ u8 *vlcbuf[2];
    /* 0x08 */ s32 vlcid;
    /* 0x0C */ u8 *imgbuf;
    /* 0x10 */ RECT rect[2];
    /* 0x20 */ s32 rectid;
    /* 0x24 */ RECT slice;
    /* 0x2C */ s32 isdone;
} MovieDecodeEnvironment; /* size 0x30 */

/* Movie (STR) player state at 0x80180000 behind movie_context (strInit, strNext, strCallback,
 * movie_play); the ring buffer behind +0x44 is reached by byte offset. T-17014 */
typedef struct MovieStreamContext {
    /* 0x00 */ u8 start_position[4];
    /* 0x04 */ MovieDecodeEnvironment decode_environment;
    /* 0x34 */ s32 end_flag;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 width;
    /* 0x40 */ s32 height;
} MovieStreamContext; /* accessed prefix, size 0x44+ */

void set_movie_offset(s16 arg0, s16 arg1);
void strCallback(void);
void strKickCD(s32 arg0);
s32 movie_play(s32 movie_name_or_zero, s32 requested_end_frame, s32 movie_start_sector);
void movie_captions_update(s32 arg0); /* defined after its caller movie_play */

extern s32 movie_offset_x;
extern s32 movie_offset_y;
extern s32 movie_flags;
extern u8 D_800E7D35;
extern MovieStreamContext *movie_context;
extern s32 D_80125C5C;
extern s32 movie_last_frame; /* frame number of the last movie frame (strNext) */
extern u32 movie_end_frame; /* end frame of the movie, set by movie_play (strNext) */
extern u8 movie_caption_marks[22]; /* movie subtitle frame table (movie_captions_update) */

#endif /* MAIN_MOVIE_H */
