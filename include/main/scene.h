#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "common.h"

s32 scene_still_show(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 scene_anim_show();
#ifndef MAIN_API_OVERRIDE_scene_screen_reset
void scene_screen_reset(s32 arg0, s32 arg1);
#endif

void scene_wait_cd_read(void);
void scene_bg_wipe_in(void);
s32 scene_bg_wipe_in_step();
s32 scene_bg_wipe_out_step();
void scene_frame_update();
void scene_still_layer_set();
#ifndef MAIN_API_OVERRIDE_scene_still_fade_out
void scene_still_fade_out(void);
#endif
void Default_Disp(void);

/* T-18000: SHUGAKU compares callback entries with this function address via %hi/%lo loads. */
#ifndef MAIN_API_OVERRIDE_scene_still_fade_in
void scene_still_fade_in(void);
#endif

extern u32 D_80122D3C;

#endif
