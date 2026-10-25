#ifndef MAIN_TPAGE_H
#define MAIN_TPAGE_H

#include "common.h"

void tpage_buf_clear_all(void);
#ifndef MAIN_API_OVERRIDE_tpage_buf_clear
void tpage_buf_clear(void);
#endif

s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1);
#ifndef MAIN_API_OVERRIDE_search_tpage_multi
s32 search_tpage_multi(s32 arg0, u8 arg1);
#endif
void _sys_default_tpage_set(void);
void load_tpage_buf_lock();

extern s32 D_800C5FD0;
extern s32 D_800C9734;
extern s32 D_800C9738;
extern s32 D_800C973C;
extern s32 D_800C974C;
extern s32 D_800C9750;
extern s32 D_800C9754;
extern s32 D_800C9758;

#endif
