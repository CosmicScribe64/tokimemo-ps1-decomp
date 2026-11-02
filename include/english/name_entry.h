#ifndef ENGLISH_NAME_ENTRY_H
#define ENGLISH_NAME_ENTRY_H

/* English name entry (T-39506, src/english/name_entry.c): replacements of NAME_ENT functions reached
 * through config/english_hooks.txt (unit NAME_ENT). */

#include "common.h"

void english_name_entry_step_init(void);
void english_name_entry_family_name_input(void);
void english_name_entry_given_name_init(void);
void english_name_entry_given_name_input(void);
void english_name_entry_nickname_init(void);
void english_name_entry_nickname_input(void);
void english_name_entry_names_draw(void);
void english_name_entry_frames_draw(s32 alternate_color);
s32 english_name_entry_profile_store(void);

#endif
