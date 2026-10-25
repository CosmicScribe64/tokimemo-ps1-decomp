#ifndef MAIN_PAD_H
#define MAIN_PAD_H

/* T-27061: Controller polling and pad input state. */
#include "common.h"

extern s8 pad_turbo;
extern u8 pad_reset_frames;
extern u32 pad_code_table[20];
#ifndef MAIN_API_OVERRIDE_pad_code_entered
extern u8 pad_code_entered;
#endif
extern u8 pad_code_index;
void pad_update(void);
s32 pad_device_detect();
void mouse_driver_init(void);
void pad_driver_init(void);
void pad_state_clear(void);
void mouse_to_pad();
s32 pad_raw_read(void);
s32 pad_poll(void);
void func_800433D0(s32 arg0);
void pad_code_update(void);

extern u8 pad_buffer_1[];
extern u8 pad_buffer_2[];

/* T-14200: canonical declaration of the existing MAIN definition. */
void pad_defaults_set(void);

#endif /* MAIN_PAD_H */
