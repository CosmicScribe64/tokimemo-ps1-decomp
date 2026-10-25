#ifndef MAIN_TITLE_H
#define MAIN_TITLE_H

#include "common.h"

void title_view_init(void);
void title_lights_init(void);
void title_coordinates_init();
void title_intro_backdrop_step(void);
void title_intro_logo_step(void);
void title_intro_stage(void);
void title_menu_setup_step(void);
void title_menu_cursor_update(void);
void title_menu_select_step(void);
void title_menu_highlight(void);
void title_exit_dispatch_step(void);
void title_exit_fade_step(void);
void title_exit_stage(void);

extern s16 title_photo_offsets[13]; /* T-25119: 13 halfwords title_setup_stage copies into the words at D_801230D4 */

#endif
