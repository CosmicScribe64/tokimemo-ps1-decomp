#ifndef MAIN_ENTRY_UI_SCENES_H
#define MAIN_ENTRY_UI_SCENES_H

#include "common.h"

extern u8 *D_800B3288[];
extern u8 **D_800B32CC;
extern u8 D_800B3644[3];
/* OPTION walks byte 3 of 0x44-byte records from here (the loop base register is this address,
 * not one of actor_pool; T-7020). */
#ifndef MAIN_API_OVERRIDE_D_801206D8
extern u8 D_801206D8[];
#endif
extern s16 D_80125C80[]; /* four s16 slots, written at index (tick & 3) (ENDING func_80137730) */
extern u8 *place_name_table[];

#endif
