#ifndef MAIN_AUDIO_H
#define MAIN_AUDIO_H

#include "common.h"

#ifndef GAME_T10880_CALL_PAIRS
#define GAME_T10880_CALL_PAIRS
/* D_800B3B9C is indexed in 0x10-byte rows containing two call pairs.
 * NAME_ENT func_8014068C passes these words to cd_audio_request_set. */
typedef struct AudioRequestPair {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
} AudioRequestPair; /* size 0x08; table view: AudioRequestPair[][2] */
#endif

/* Work area at audio_state (0x50 bytes; main 80079B10 and 8007C030 only). The original keeps its
 * stores and loads in source order, as for one object (wiki/data-types.md); unk_44 and unk_48 are
 * message buffers passed to cd_audio_command. */
typedef struct AudioState {
    /* 0x00 */ u32 state_flags;
    /* 0x04 */ s32 request_flags;
    /* 0x08 */ s32 xa_clip_start;
    /* 0x0C */ s32 xa_clip_end;
    /* 0x10 */ s32 xa_next_start;
    /* 0x14 */ s32 xa_next_end;
    /* 0x18 */ s32 xa_resume_position;
    /* 0x1C */ s16 xa_volume;
    /* 0x1E */ s16 xa_volume_full;
    /* 0x20 */ s16 bgm_volume;
    /* 0x22 */ s16 bgm_volume_full;
    /* 0x24 */ u16 se_volume;
    /* 0x26 */ s16 se_volume_default;
    /* 0x28 */ s16 bgm_sequence_handle;
    /* 0x2A */ s16 bgm_sequence_id;
    /* 0x2C */ s16 bgm_next_sequence_id;
    /* 0x2E */ s16 bgm_fade_accumulator;
    /* 0x30 */ s16 bgm_fade_step;
    /* 0x32 */ s16 bgm_fade_rate;
    /* 0x34 */ u16 xa_fade_accumulator;
    /* 0x36 */ u16 xa_fade_step;
    /* 0x38 */ u16 xa_fade_rate;
    /* 0x3A */ s16 sequence_pack_set;
    /* 0x3C */ s16 bgm_slot;
    /* 0x3E */ s16 bgm_auxiliary_slot;
    /* 0x40 */ u16 se_slot_a; /* HYPOTHESIS: this slot holds the shared system-sound bank (h-graphics: files b,c are shared banks and slot 0 is the small one); fact: VAB slot for class 5 effects, 0 or 3 (ids 0x21/0x24) */
    /* 0x42 */ u16 se_slot_b;
    /* 0x44 */ u8 cd_mode_param[4];
    /* 0x48 */ u8 xa_params[8];
} AudioState; /* size 0x50 */

/* SD_GetCDLevel request/output prefix, verified from halfword accesses. T-10870. */
typedef struct CdLevelMeter {
    /* 0x00 */ u16 left; /* left output */
    /* 0x02 */ u16 right; /* right output */
    /* 0x04 */ u16 precision_bits; /* output precision: shift by 16 minus this value */
    /* 0x06 */ u16 sample_count; /* sample count */
    /* 0x08 */ u16 options; /* method, square and shaping selectors */
} CdLevelMeter; /* accessed size 0x0A */

extern AudioState audio_state;
#ifndef MAIN_API_OVERRIDE_audio_command
s32 audio_command(s32 arg0);
#endif
s32 audio_bgm_stop(s32 arg0);
u8 audio_bgm_is_playing(void);
void audio_bank_state_reset(void);
void audio_bank_load_start(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 audio_bank_transfer_poll(void);
s32 audio_bank_load_poll(void);
s32 audio_bank_start();
s32 audio_bgm_play_short(s32 arg0, s32 arg1);
void audio_debug_load_error_report();
void audio_command_submit(u16 command_word);
void audio_bank_transfer_schedule(void);
void audio_bank_transfer(s16 arg0);
s32 audio_bgm_bank_ready(s32 arg0);
void audio_xa_frame_update(void);
void audio_xa_check_end(void);
void audio_xa_start_poll(void);
void audio_pending_update(void);
void audio_control_dispatch();
void audio_xa_fade_update();
void audio_xa_fade_tick(void);
void audio_xa_fade_finish();
void audio_command_xa();
void audio_xa_promote_next(void);
void audio_xa_play(s32 arg0);
void audio_xa_control();
void audio_xa_fade_start(u16 arg0);
void audio_bgm_update();
void audio_bgm_fade_finish();
void audio_command_bgm();
void audio_bgm_request_start(void);
void audio_bgm_start(void);
void audio_bgm_control();
void audio_bgm_fade_start();
void audio_command_se_a(u16 audio_se_a_command);
#ifndef MAIN_API_OVERRIDE_audio_command_se_b
void audio_command_se_b(u16 audio_se_b_command);
#endif
#ifndef MAIN_API_OVERRIDE_audio_command_se_bgm_bank
void audio_command_se_bgm_bank(u16 audio_se_bgm_command);
#endif
#ifndef MAIN_API_OVERRIDE_audio_command_se_bgm_auxiliary
void audio_command_se_bgm_auxiliary(u16 audio_se_auxiliary_command);
#endif
void audio_se_play(s32 sound_key, s32 packed_note);
void audio_se_stop();
s32 audio_queue_push(u16 a);
void audio_queue_clear(void);
u16 audio_queue_pop();
void audio_xa_forget();
void audio_xa_reset();
void audio_voices_kill();
void audio_init(void);
void audio_bank_layout_init();
void audio_bank_body_address_set(u16 arg0);
void audio_cd_audio_init(void);
void audio_state_init(void);
void audio_output_mode_set();
void audio_shutdown(void);
void audio_bank_request_transfer(s16 arg0);
void audio_bank_dispose(s16 arg0);
void audio_bgm_next_clear();
void audio_reverb_off_toggle(void);
void SD_InitCDLevelInfo(void);
void audio_bank_wait_latch(void);

extern u8 D_800B3D54[];
/* T-11190: declarations added by tools/bulk_m2c.py. */
/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s32 D_800B3D30;
extern s32 D_800B3D34;
extern s32 D_800B3D38;
extern s32 D_800B3D3C;

extern u8 D_800B3D24;
extern u8 audio_bgm_playing;
extern u8 audio_bgm_set;
extern u8 audio_bgm_sequence;
extern u16 D_80125CC0;
extern u16 D_80125CC2;
extern s32 D_80125D18;
extern s32 D_80125D1C;
extern s32 D_80125D20;
extern s32 D_80125D24;
extern s32 D_80125D28;
extern s16 D_80125D2C;
extern u16 D_80125D44;
extern u16 D_80125D46;
extern u16 D_80125D48;
extern s16 D_80125D4A;
extern s8 D_80125D5C;
extern u8 D_80125D5D;
extern s16 D_80125D38;
extern s16 D_80125D3A;
extern s16 D_80125D3C;
extern u16 D_80125D4C;
extern u16 D_80125D4E;
extern u16 D_80125D50;
extern u16 D_80125D52;
extern s32 audio_bank_header_address[];
extern s32 D_80125D64;
extern s32 D_80125D68;
extern s32 D_80125D6C;
extern s32 audio_bank_body_address[];
extern s32 audio_bank_sound_ram_address[];
extern s32 D_80125D84;
extern s32 D_80125D88;
extern s32 D_80125D8C;
extern s32 audio_bank_body_size;
extern s32 D_80125D94;
extern s32 D_80125D98;
extern s32 D_80125D9C;
/* T-17000: per-channel tables of audio_sequence_pack_index and audio_bgm_start, indexed by channel << 6 and
 * channel << 5: 0x40-byte rows of words and 0x20-byte rows of halfwords (two rows each would end at
 * audio_sequence_reverb_depth and audio_cd_level). */
extern s32 audio_sequence_address[][16];
extern s16 audio_sequence_reverb_depth[][16];
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;
extern u8 audio_sequence_table[];
extern u8 D_800B3D20;

void SD_GetCDLevel();
void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n);
void SD_CalcCDAve(s16 *left, s16 *right, u16 start, u16 count);
/* T-10860: generated from src/main/scene_library.c by sync_protos.py. */
s16 getCDlevel(void);

extern u8 D_800AFDF0[];
extern s32 D_80125D14;
extern u8 D_80125D5E;
extern u8 D_80125D5F;
extern u16 audio_cd_level;
extern u16 D_80125E62;
extern s16 D_80126080[]; /* two 0x200-entry s16 tables (+0, +0x400), SD_CalcCDAve */

#endif /* MAIN_AUDIO_H */
