#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "common.h"
#include "game/model_records.h"

extern GraphicsCoordinate2 view_coordinates[];
extern GraphicsCoordinate2 D_80122CA0[];
extern GraphicsCoordinate2 D_80122840[];
void initView(void);
void initCoordinate();
void initModelingData_init(s32 *arg0);
s32 view_link_object(u32 *model_data_address, s32 destination_slot, u32 object_index);
void view_model_slots_clear();
void view_display_set_narrow_or_standard();
void view_display_set_wide_or_standard(s32 arg0);
void view_frame_buffers_clear(void);
void view_model_record_init(s32 arg0);

extern GraphicsCoordinate2 D_80122B10[];

#endif
