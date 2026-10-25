#ifndef MAIN_CLUB_H
#define MAIN_CLUB_H

#include "common.h"

extern u8 *club_name_table[];

void join_club(void);
void join_club_init(void);
s32 join_club_select(void);
s32 join_club_message(void);
void join_club_exit(void);

#endif
