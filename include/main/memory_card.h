#ifndef MAIN_MEMORY_CARD_H
#define MAIN_MEMORY_CARD_H

#include "game/save_image.h"

#include "common.h"

/* Stack request of the memory-card file functions (memory_card_file_create, memory_card_file_delete): name buffer plus a retry
 * counter; layout read from the frame (counter at +0x20 in memory_card_file_delete). */
typedef struct MemoryCardFileRequest {
    /* 0x00 */ u8 name[32];
    /* 0x20 */ s32 retry;
} MemoryCardFileRequest; /* size 0x24 */

extern u8 memory_card_block[];
void card_ev_set(void);
void Sw_Start(void);
void Hw_Start(void);
u8 Sw_Test(void);
void Sw_Clear(void);
void Hw_Clear(void);
void memory_card_step_next(void);
#ifndef MAIN_API_OVERRIDE_memory_card_start
void memory_card_start(void);
#endif
#ifndef MAIN_API_OVERRIDE_memory_card_stop
void memory_card_stop(void);
#endif
s32 memory_card_check_poll(void);
s32 memory_card_scan_poll(void);
u8 memory_card_system_read_poll(void);
s32 memory_card_system_bank_load(s32 arg0);
s32 memory_card_system_write_poll();
u8 memory_card_save_read_poll(void);
s32 memory_card_file_exists(s32 file_index);
void memory_card_block_clear(void);
s32 memory_card_bank_validate(s32);
s32 memory_card_game_bank_load(s32 arg0);
s32 memory_card_file_name_build(u8 *buf, s32 arg1);
void memory_card_state_reset(void);

extern u8 memory_card_file_prefix[];
extern u8 D_800E7D13[];
extern s8 D_800E7D16;
extern s32 memory_card_software_event_done;
extern s32 memory_card_software_event_error;
extern s32 memory_card_software_event_timeout;
extern s32 memory_card_software_event_new_card;

extern s32 memory_card_file_descriptor;
extern s32 memory_card_hardware_event_done;
extern s32 memory_card_hardware_event_error;
extern s32 memory_card_hardware_event_timeout;
extern s32 memory_card_hardware_event_new_card;
extern MemoryCardBank D_80123320[2];
extern u8 D_80123324[];

#endif /* MAIN_MEMORY_CARD_H */
