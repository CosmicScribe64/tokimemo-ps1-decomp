#ifndef MAIN_ONLY_H
#define MAIN_ONLY_H

/* Declarations that only src/main/*.c may see (T-1200). Overlay headers never include
 * this file. Use it for a main-exe function whose matched prototype is narrower than the
 * view an overlay was matched with (u8 parameter in the definition, s32 in the overlay
 * callers): the two views cannot share one declaration without changing the overlay's
 * call code, so game.h leaves the symbol out and each side declares its own view. */

#include "common.h"

void func_80083440(u8 arg0);

#endif /* MAIN_ONLY_H */
