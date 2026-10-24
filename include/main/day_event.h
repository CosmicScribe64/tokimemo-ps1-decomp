#ifndef MAIN_DAY_EVENT_H
#define MAIN_DAY_EVENT_H

#include "common.h"

s32 day_event_queue_resync_after_master();
void day_event_queue_build_week(void);
void day_event_dispatch(void);
void day_event_today_clear(u8 arg0);
s32 day_event_next(s32 arg0);

#endif
