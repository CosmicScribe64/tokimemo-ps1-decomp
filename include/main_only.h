#ifndef MAIN_ONLY_H
#define MAIN_ONLY_H

/* Declarations that only src/main/*.c may see (T-1200). Overlay headers never include
 * this file. Use it for a main-exe function whose matched prototype is narrower than the
 * view an overlay was matched with (u8 parameter in the definition, s32 in the overlay
 * callers): the two views cannot share one declaration without changing the overlay's
 * call code, so game.h leaves the symbol out and each side declares its own view. */

#include "common.h"

void func_80083440(u8 arg0);
void func_80065900(u8 arg0);
/* T-1340 */
void func_8007B99C(u16 arg0);
void func_8007BE94(s32 arg0);
void func_8007BF04(s32 arg0);
/* T-2090: overlay headers declare these symbols themselves */
extern s32 D_800E6378;
extern u8 D_800CA188[];
void func_80062CD0();
void k_disp_inc2();
void func_80083A10();
void check_k_scroll();

/* T-2030: overlays call it with s32 arguments, see include/ovl/*.h */
void draw2d3d(u8 arg0, u8 arg1);

/* T-2040: ETC calls these through unprototyped declarations, MASTER through its own u8 view */
void set_dec_bri(u8 arg0);
s32 dec_bg_cd_read(s32 arg0, s32 arg1);

#endif /* MAIN_ONLY_H */
