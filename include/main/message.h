#ifndef MAIN_MESSAGE_H
#define MAIN_MESSAGE_H

#include "common.h"

void message_disp_switch(s32 arg0);
void message_window_init(void);
void message_window_show(void);
void message_window_title_show(s32 arg0);
void message_window_set_title_hidden(u8 *arg0);  /* forwards the title to message_window_set_title */
void message_window_set_title();  /* callers pass one pointer, or nothing */
void message_window_title_update();

extern s16 D_800B5B24;
extern s16 D_8011ED28;

#endif
