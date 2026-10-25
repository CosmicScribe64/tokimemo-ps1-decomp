#ifndef MAIN_VSYNC_H
#define MAIN_VSYNC_H

#include "common.h"

#ifndef MAIN_API_OVERRIDE_Vblnk_Timer_Init
void Vblnk_Timer_Init(void);
#endif
s32 Vblnk_Timer(void);

extern s32 vsync_timer_start;
extern s32 vsync_timer_elapsed;

#endif
