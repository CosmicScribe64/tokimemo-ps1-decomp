#ifndef MAIN_PORTRAIT_H
#define MAIN_PORTRAIT_H

#include "common.h"

void portrait_read_start(); /* unprototyped: main callers pass no argument, overlays pass one */
void portrait_build();
void portrait_value_clear_if_high(); /* unprototyped: callers pass an argument, the definition ignores it (a named parameter makes IDO spill a0: T-39312) */
void portrait_read_step(s32 arg0); /* forwards its argument to portrait_read_start */
void portrait_expression_select(u32 expression_variant);
void portrait_face_update();
void portrait_event_face_mouth_lipsync(void);
s32 portrait_event_face_eyes_animation_start(void);
#ifndef MAIN_API_OVERRIDE_portrait_layout_set
void portrait_layout_set(u8 arg0);
#endif
s32 portrait_show_checked(void);
s32 portrait_wink_update();
void portrait_quad_offsets(u8 expression_index);
s32 portrait_event_face_mouth_animation_start(void);

extern u8 D_800B5A64;
extern s32 D_800B5A98; /* T-17011: counter of portrait_face_update (CD level high-water mark) */

void bustup_wink(void);
void addr_init_bustup(void);
void face_expression_0(void);
void face_expression_1(void);
void face_expression_3(void);
void face_expression_4(void);
void face_expression_mood(void);
#ifndef MAIN_API_OVERRIDE_read_bustup
void read_bustup();
#endif
void event_face0(void);
void event_face1(void);
void read_bustup_uniform(void);

extern u8 D_800B5A68[][3];

#endif
