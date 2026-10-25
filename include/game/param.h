#ifndef GAME_PARAM_H
#define GAME_PARAM_H

#include "common.h"

/* Nine records at +0xFC (separate members); the s16 at +2 holds the nine values parameter_show_init reads, KANGEI
 * copies whole records. */
typedef struct ParamValue {
    /* 0x00 */ s16 fraction;
    /* 0x02 */ s16 value;
} ParamValue; /* size 0x04 */

#endif /* GAME_PARAM_H */
