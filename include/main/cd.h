#ifndef MAIN_CD_H
#define MAIN_CD_H

#include "game/word_views.h"

#include "common.h"

/* 0x18-byte entry of the 48-entry ring at cd_command_ring (read index cd_queue_head, write index
 * cd_queue_tail; filled by cd_command_push). data[15] is cleared on every write and set to 0xFF when
 * the caller passes 1 instead of a data pointer. */
typedef struct CdCommand {
    /* 0x00 */ u8 command;
    /* 0x04 */ s32 lba;
    /* 0x08 */ u8 param[16];
} CdCommand; /* size 0x18 */

extern u8 cd_read_checked;
extern s32 cd_sectors_left;
extern s32 cd_last_read_lba;
extern s32 cd_audio_request[4]; /* defined in src/main/cd_queue.c (T-9010) */
extern u8 cd_read_status;
extern CdCommand cd_command_ring[];
s32 cd_load_destination_base_get();
#ifndef MAIN_API_OVERRIDE_cd_command_reset
void cd_command_reset(void);
#endif
#ifndef MAIN_API_OVERRIDE_cd_audio_command
void cd_audio_command(s32 cd_operation_id, s32 sector_lba, u8 *command_params);
#endif
#ifndef MAIN_API_OVERRIDE_cd_last_command
u8 cd_last_command(void);
#endif
u8 cd_read_sync_status(void);
u8 cd_done_count(void);
u8 cd_audio_state(void);
s32 cd_audio_ticks(void);
void cd_audio_request_set(s32, s32, s32);
void cd_read_param_pack(u8 sector_count, s32 destination_address, s32 *read_parameter_out);
#ifndef MAIN_API_OVERRIDE_cd_read_sectors_nowait
void cd_read_sectors_nowait(u8 arg0, s32 arg1, s32 arg2);
#endif
#ifndef MAIN_API_OVERRIDE_cd_read_sectors
void cd_read_sectors(u8 arg0, s32 arg1, s32 arg2);
#endif
s32 cd_file_checksum_ok();
s32 cd_busy_marker_draw(void);

extern WordBytes cd_result;
extern s32 cd_audio_speaker_offset_table[16];
extern u8 cd_queue_head;
extern u8 cd_queue_tail;
extern s8 cd_queue_phase;
extern s32 cd_audio_position;
extern u8 cd_state;
extern u8 cd_read_count;
extern u8 cd_mode_scratch;
extern u8 cd_read_sync_result[];
extern u8 cd_sync_result[];
extern s32 cd_load_destination_base;

#endif /* MAIN_CD_H */
