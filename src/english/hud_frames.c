/* hud_frames.c: room for HUD sprite frames of the English art (T-39508). Not part of the original
 * executable: only the English build (`ninja english`) links it into the main executable.
 *
 * Some MAINROM frames need more parts in English than their records in the executable hold (a
 * changed 16x16 cell is redrawn from 8x8 tiles, because the uploaded HUD sheets have no free 16x16
 * cell). tools/patch/art writes those frames' new records into this array after the link and points
 * their sprite_frame_table entries here; the array starts with a magic so the build finds it in the
 * linked executable. The records hold no pointers, so the array needs no relocation. */
#include "common.h"

#ifdef TOKIMEMO_ENGLISH

/* 16-byte magic, then frame records (count, origin, flags, 14-byte parts) written by the build. */
u8 english_hud_frames[0x800] = {
    'T', 'K', 'M', '-', 'E', 'N', '-', 'H', 'U', 'D', 'F', 'R', 'A', 'M', 'E', 'S'
};

/* Start of the frame records. Nothing calls it: tools/cc.py (trailing_pad.py) needs every object
 * to have a .text section. */
u8 *english_hud_frames_start(void) {
    return english_hud_frames + 16;
}

#endif
