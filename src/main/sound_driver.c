/* sound_driver.c: src/main/80079B10.c; T-27059: original object [0x80079B10,0x8007C030); per-frame audio command queue, bank transfer/loading, BGM, sound effects, XA voice and CD-level meter; actual body is sound driver, not date sequences. Original object 0x80079B10-0x8007C030 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_func_8008BFB0 /* original narrows both volume arguments; T-12902 */
/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */

#include "common.h"
#include "common.h"
#include "game_enums.h"
#include "libcd.h"
#include "libsnd.h"
#include "libspu.h"
#include "main/audio.h"
#include "main/cd.h"
#include "main/ovl.h"
#include "main/ovl.h"
#include "main/audio.h"

void audio_command_bank_transfer(u16 arg0);
void audio_command_bank_assign(u16 arg0);
void audio_command_system(u8 arg0);
void func_8008BFB0(s32, s16, s16);

void audio_command_submit(u16 command_word) {
    audio_queue_push(command_word);
}

void audio_command_pump(void) {
    u16 r;

    r = audio_queue_pop();
    if (r != 0) {
        do {
            switch (r & 0xF00) {
            case 0x0:
                audio_control_dispatch(r);
                break;
            case 0x200:
                audio_command_bgm(r);
                break;
            case 0x300:
                audio_command_xa(r);
                break;
            case 0x500:
                audio_command_se_a(r);
                break;
            case 0x600:
                audio_command_se_b(r);
                break;
            case 0x700:
                audio_command_se_bgm_bank(r);
                break;
            case 0x800:
                audio_command_se_bgm_auxiliary(r);
                break;
            }
            r = audio_queue_pop();
        } while (r != 0);
    }
    audio_xa_frame_update();
    audio_pending_update();
    audio_queue_clear();
}

void audio_bank_transfer_schedule(void) {
    void audio_bank_transfer(s16 arg0);
    s32 sp2C;
    s32 bank_slot_index;
    s32 bank_request_mask;
    s32 flags;

    flags = audio_state.request_flags & 0xFF000000;
    for (bank_slot_index = 0; bank_slot_index < 4; bank_slot_index++) {
        bank_request_mask = (0x01000000 << bank_slot_index) & 0x0F000000;
        /* FAKE: signed ~0x0FFFFFFF in the loop condition vs unsigned 0xF0000000U in the body keeps the two 0xF0000000 masks in separate uopt constant entries so 0x0F000000 takes $a2, 0xF0000000 takes $a3, and the body bank_request_mask uses $at. T-24123 */
        if ((flags & bank_request_mask) && !(flags & ~0x0FFFFFFF)) {
            sp2C = (0x10000000 << bank_slot_index) & 0xF0000000U;
            audio_state.request_flags &= ~bank_request_mask;
            audio_state.request_flags |= sp2C;
            audio_bank_transfer(bank_slot_index);
            audio_state.request_flags &= ~sp2C;
            return;
        }
    }
}

void audio_bank_transfer(s16 arg0) {
    s32 off;
    s32 pad; /* FAKE: unused local; the original frame has 8 more bytes of locals (spill of off at sp+0x2C); real source unknown. T-4090 */

    off = arg0 * 4;
    if (func_8008C1B0(*(s32 *)((u8 *) audio_bank_header_address + off), arg0, *(s32 *)((u8 *) audio_bank_sound_ram_address + off)) == -1) {
        audio_debug_load_error_report(((arg0 << 8) & 0xFF00) | 0x01010000, arg0);
        return;
    }
    audio_state.state_flags |= 0x10000 << arg0;
    if (func_8008C620(*(s32 *)((u8 *) audio_bank_body_address + off), arg0) == -1) {
        audio_debug_load_error_report(((arg0 << 8) & 0xFF00) | 0x01020000, arg0);
        return;
    }
    SsVabTransCompleted(1, arg0);
}

s32 audio_bgm_bank_ready(s32 arg0) {
    if ((audio_state.request_flags & (0x10000000 << (u16)audio_state.bgm_slot)) == 0) {
        if ((audio_state.request_flags & (0x1000000 << (u16)audio_state.bgm_slot)) == 0) {
            return 1;
        }
    }
    return 0;
}

void audio_xa_frame_update(void) {
    if (audio_state.state_flags & 0x400) {
        audio_xa_check_end();
    }
    if (audio_state.request_flags & 0x200) {
        audio_xa_start_poll();
    }
}

void audio_xa_check_end(void) {
    if (cd_audio_ticks() >= (u32)audio_state.xa_clip_end) {
        if (!(audio_state.request_flags & 0x100)) {
            audio_xa_fade_start(0xB4);
        }
    } else {
        SD_GetCDLevel(&audio_cd_level);
    }
}

void audio_xa_start_poll(void) {
    if (cd_audio_state() & 4) {
        audio_state.request_flags &= ~0x200;
        if (audio_state.xa_next_start != 0) {
            audio_xa_control(0x74);
            audio_xa_promote_next();
            if (!(audio_state.state_flags & 0x4000)) {
                audio_xa_play(audio_state.xa_clip_start);
            }
        } else if (!(audio_state.state_flags & 0x4000)) {
            /* Snapshot volume before flags to preserve load scheduling. T-12902 */
            audio_state.xa_volume = audio_state.xa_volume_full;
            audio_state.state_flags = (audio_state.state_flags | 0x400) & 0xFFEFFFFF;
            func_8008B750(0x7F, 0x7F);
            func_8008BFB0(0, audio_state.xa_volume, audio_state.xa_volume);
            func_8008BEE0(0, 0, 1);
        }
    }
}

void audio_pending_update(void) {
    s32 v;

    v = audio_state.request_flags;
    if (v != 0) {
        if ((u32) v >> 24) {
            audio_bank_transfer_schedule();
            v = audio_state.request_flags;
        }
        if (v & 0xFF) {
            audio_bgm_update();
            v = audio_state.request_flags;
        }
        if (v & 0xFF00) {
            audio_xa_fade_update();
        }
    }
}

void audio_control_dispatch(audio_control_word)
u16 audio_control_word;
{
    void audio_bgm_fade_start(u16 audio_control_word);

    switch (audio_control_word & 0xF0) {
    case 0x10:
        audio_command_bank_transfer(audio_control_word);
        break;
    case 0x20:
        audio_command_bank_assign(audio_control_word);
        break;
    case 0x70:
        if (audio_control_word & 1) {
            audio_bgm_control(0x71);
        }
        if (audio_control_word & 2) {
            audio_voices_kill();
        }
        if (audio_control_word & 4) {
            audio_xa_control(0x74);
            audio_xa_promote_next();
        }
        break;
    case 0xE0:
        if (audio_control_word & 1) {
            audio_bgm_control(0xE1);
        }
        if (audio_control_word & 4) {
            audio_xa_control(0xE4);
            audio_xa_promote_next();
        }
        break;
    case 0xF0:
        if (audio_control_word & 1) {
            audio_bgm_control(0xF1);
        }
        if (audio_control_word & 4) {
            audio_xa_control(0xF4);
            audio_xa_promote_next();
        }
        break;
    case 0xB0:
    case 0xC0:
    case 0xD0:
        if (audio_control_word & 1) {
            audio_bgm_fade_start(audio_control_word & 0xFFF1);
        }
        if (audio_control_word & 4) {
            audio_xa_fade_start(audio_control_word & 0xFFF4);
        }
        break;
    default:
        audio_command_system(audio_control_word & 0xFF);
        break;
    }
}

void audio_command_bank_transfer(u16 arg0) {
    arg0 = arg0 & 0xFF;
    audio_bank_body_address_set(arg0);
    switch (arg0) {
    case 17:
        audio_bank_dispose(0);
        audio_bank_request_transfer(0);
        audio_bank_dispose(1);
        audio_bank_request_transfer(1);
        return;
    case 18:
        audio_bank_dispose(2);
        audio_bank_request_transfer(2);
        audio_bank_dispose(3);
        audio_bank_request_transfer(3);
        return;
    case 19:
        audio_bank_dispose(0);
        audio_bank_request_transfer(0);
        return;
    case 20:
        audio_bank_dispose(1);
        audio_bank_request_transfer(1);
        return;
    case 21:
        audio_bank_dispose(2);
        audio_bank_request_transfer(2);
        return;
    case 22:
        audio_bank_dispose(3);
        audio_bank_request_transfer(3);
        return;
    default:
        return;
    }
}

void audio_command_bank_assign(u16 arg0) {
    arg0 = arg0 & 0xFF;
    switch (arg0) {
    case 33:
        D_80125D50 = 0;
        D_80125D52 = 3;
        return;
    case 34:
        D_80125D4C = 1;
        D_80125D4E = 2;
        return;
    case 35:
        D_80125D4C = 2;
        D_80125D4E = 1;
        return;
    case 36:
        D_80125D50 = 3;
        D_80125D52 = 0;
        return;
    case 47:
        D_80125D4A = 0;
        audio_sequence_pack_index(0x8001C000, 0);
        return;
    case 46:
        D_80125D4A = 1;
        audio_sequence_pack_index(0x8002E800, 1);
        return;
    default:
        return;
    }
}

void audio_command_system(u8 arg0) {
    arg0 &= 0xFF;
    switch (arg0) {
    case 1:
    case 2:
        audio_output_mode_set(arg0);
        break;
    case 3:
        audio_xa_forget(arg0);
        break;
    case 6:
        audio_xa_reset(arg0);
        break;
    case 4:
        audio_voices_kill(arg0);
        break;
    case 5:
        audio_state.state_flags |= 0x100000;
        break;
    }
}

void audio_xa_fade_update(void) {
    if (audio_state.request_flags & 0x100) {
        audio_xa_fade_tick();
    }
}

void audio_xa_fade_tick(void) {
    if (audio_state.state_flags & 0x4400) {
        D_80125D44 += D_80125D48;
        /* FAKE: the flag test reads D_80125D44 through an s16 view (a plain u16 test changes the uopt range order: sum in $v1, temporaries one register off); real source unknown. T-17012 */
        if (((*(s16 *)&D_80125D44) & 0xFF00) != 0) {
            D_80125D44 &= 0xFF;
            D_80125D2C -= D_80125D46;
            if (D_80125D2C <= 0) {
                audio_xa_fade_finish();
                return;
            }
            func_8008BFB0(0, D_80125D2C, D_80125D2C);
        }
    }
}

void audio_xa_fade_finish(void) {
    audio_xa_control(0x74);
    if ((audio_state.state_flags & 0x4000) == 0) {
        audio_xa_promote_next();
        if (audio_state.xa_clip_start != 0) {
            audio_xa_play(audio_state.xa_clip_start);
        }
    }
}

void audio_speaker_volume_set(u32 speaker_id) {
    audio_state.xa_volume_full = 0x64;
    switch (speaker_id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        audio_state.xa_volume_full = 0x7D;
        break;
    case 7:
        audio_state.xa_volume_full = 0x5F;
        break;
    case 9:
    case 11:
        audio_state.xa_volume_full = 0x5F;
        break;
    case 10:
        audio_state.xa_volume_full = 0x5A;
        break;
    case 12:
    case 13:
        audio_state.xa_volume_full = 0x6E;
        break;
    case 15:
        audio_state.xa_volume_full = 0x69;
        break;
    }
}

void audio_command_xa(audio_voice_command)
u16 audio_voice_command;
{
    /* FAKE: unused 4-byte local reproduces the original 0x30 stack frame size; real source unknown. T-24118 */
    s32 pad;
    u32 temp_a0;
    s32 temp_v1;
    s32 *temp_v0_2;

    switch (audio_voice_command & 0xF000) {
    case 0x0:
        temp_v0_2 = (s32 *)cd_audio_request_get();
        temp_a0 = temp_v0_2[0];
        temp_v1 = temp_v0_2[1];
        audio_state.xa_next_start = temp_v0_2[2];
        audio_state.xa_next_end = temp_v0_2[3];
        /* FAKE: u32 view of the s32 sector fields emits lbu at +0 instead of sra 24 and lw/and for the 24-bit mask; real field type/union unknown. T-24118 */
        audio_state.xa_params[5] = (*(u32 *)&audio_state.xa_next_start >> 24) & 0xFF;
        audio_state.xa_params[7] = (*(u32 *)&audio_state.xa_next_end >> 24) & 0xFF;
        *(u32 *)&audio_state.xa_next_start &= 0xFFFFFF;
        *(u32 *)&audio_state.xa_next_end &= 0xFFFFFF;
        audio_speaker_volume_set(temp_a0);
        if (temp_a0 == 0xF) {
            temp_a0 = 8;
        } else {
            temp_a0 = 0x20;
        }
        audio_state.xa_next_start = (audio_state.xa_next_start * temp_a0) + D_80125D5D + temp_v1 - 0x10;
        audio_state.xa_next_end = (audio_state.xa_next_end * temp_a0) + D_80125D5D + temp_v1 + 0x10;
        if (audio_state.state_flags & 0x400) {
            audio_xa_fade_start(0xB4);
        } else if (!(audio_state.request_flags & 0x200) && !(audio_state.state_flags & 0x4000)) {
            audio_xa_promote_next();
            audio_xa_play(audio_state.xa_clip_start);
        }
        break;
    case 0x1000:
        audio_xa_control(0x74);
        audio_xa_promote_next();
        break;
    case 0x2000:
        audio_xa_control(0xE4);
        break;
    case 0x4000:
        audio_xa_control(0xF4);
        break;
    }
}

void audio_xa_promote_next(void) {
    s32 t;
    audio_cd_level = 0;
    D_80125E62 = 0;
    t = audio_state.xa_next_start;
    audio_state.xa_clip_end = audio_state.xa_next_end;
    audio_state.xa_params[4] = audio_state.xa_params[5];
    audio_state.xa_params[6] = audio_state.xa_params[7];
    audio_state.xa_next_start = 0;
    audio_state.xa_next_end = 0;
    audio_state.xa_params[5] = 0;
    audio_state.xa_params[7] = 0;
    audio_state.xa_resume_position = t;
    audio_state.state_flags &= ~0x400;
    audio_state.state_flags &= ~0x4000;
    audio_state.request_flags &= ~0x200;
    audio_state.request_flags &= ~0x100;
    /* Keep this store after the masks to preserve early flag loads. T-12901 */
    audio_state.xa_clip_start = t;
}

void audio_xa_play(s32 arg0) {
    audio_cd_audio_init();
    audio_state.xa_params[0] = audio_state.xa_params[6];
    audio_state.xa_params[1] = audio_state.xa_params[4];
    audio_state.request_flags |= 0x200;
    cd_audio_command(0xD, arg0, audio_state.xa_params);
}

/* FAKE: the constant branch and empty tests in cases 0xE4/0xF4 keep the selector in $v0 across the flag loads so audio_state.unk_00 takes $v1 and D_80125D14 takes $a0. T-24118 */
void audio_xa_control(arg0)
u16 arg0;
{
    s32 temp_v0;

    if (1) {
        temp_v0 = arg0 & 0xFF;
    }
    switch (temp_v0) {
    case 0x74:
        func_8008BEE0(0, 0, 0);
        cd_audio_command(9, 0, 0);
        audio_state.state_flags &= ~0x400;
        audio_state.state_flags &= ~0x4000;
        D_80125D14 &= ~0x200;
        D_80125D14 &= ~0x100;
        break;
    case 0xE4:
        if (D_80125D14 & 0x200) {
            if (temp_v0) {
            }
            audio_state.state_flags |= 0x4000;
            cd_audio_command(9, 0, 0);
        } else if (audio_state.state_flags & 0x400) {
            audio_state.xa_resume_position = cd_audio_ticks();
            if ((u32) audio_state.xa_clip_start >= (u32) audio_state.xa_resume_position) {
                audio_state.xa_resume_position = audio_state.xa_clip_start;
            }
            audio_state.state_flags = (audio_state.state_flags & ~0x400) | 0x4000;
            audio_xa_fade_start(0xB4);
        }
        break;
    case 0xF4:
        if (audio_state.state_flags & 0x4000) {
            audio_state.state_flags &= ~0x4000;
            if (audio_state.xa_next_start != 0) {
                audio_xa_fade_finish();
            } else {
                audio_xa_play(audio_state.xa_resume_position);
            }
        } else if (temp_v0) {
        }
        break;
    }
}

void audio_xa_fade_start(u16 arg0) {
    if (!(audio_state.request_flags & 0x100)) {
        audio_state.xa_fade_accumulator = 0;
        audio_state.request_flags |= 0x100;
    }
    switch (arg0 & 0xFF) {
    default:
        audio_state.xa_fade_rate = 0x100;
        audio_state.xa_fade_step = 0xA;
        return;
    case 0xB4:
        audio_state.xa_fade_rate = 0x100;
        audio_state.xa_fade_step = 0xA;
        return;
    case 0xC4:
        audio_state.xa_fade_rate = 0x100;
        audio_state.xa_fade_step = 1;
        return;
    case 0xD4:
        audio_state.xa_fade_rate = 0x80;
        audio_state.xa_fade_step = 1;
        return;
    }
}

void audio_bgm_update(void) {
    s16 var_a0;

    if ((D_80125D14 & 2) && (audio_bgm_bank_ready((s16)D_80125D4C) != 0)) {
        audio_bgm_start();
    }
    if ((audio_state.state_flags & 0x100) && (D_80125D14 & 1)) {
        /* FAKE: u16 views of unk_2E/unk_30/unk_32 emit lhu as in the original while keeping SoundState's s16 fields for sibling functions. T-24118 */
        *(u16 *)&audio_state.bgm_fade_accumulator += *(u16 *)&audio_state.bgm_fade_rate;
        if ((audio_state.bgm_fade_accumulator & 0xFF00) != 0) {
            *(u16 *)&audio_state.bgm_fade_accumulator &= 0xFF;
            audio_state.bgm_volume -= *(u16 *)&audio_state.bgm_fade_step;
            if (audio_state.bgm_volume < 0) {
                audio_bgm_fade_finish();
                return;
            }
            if ((audio_state.state_flags & 0x100000) && !(audio_state.state_flags & 0x400)) {
                if (audio_state.bgm_volume_full == 0) {
                    var_a0 = 0;
                } else {
                    var_a0 = (audio_state.bgm_volume * 0x7F) / audio_state.bgm_volume_full;
                }
                /* FAKE: explicit 16-bit sign-extension reproduces the second sll 16 / sra 16 for $a1 when calling unprototyped func_8008B750. T-24118 */
                func_8008B750(var_a0, (var_a0 << 16) >> 16);
                return;
            }
            /* FAKE: explicit 16-bit sign-extension reproduces the second lh / sll 16 / sra 16 for $a2 when calling unprototyped func_8008C720. T-24118 */
            func_8008C720(D_80125D38, audio_state.bgm_volume, (audio_state.bgm_volume << 16) >> 16);
        }
    }
}

void audio_bgm_fade_finish(void) {
    if ((audio_state.state_flags & 0x100000) && !(audio_state.state_flags & 0x400)) {
        func_8008FD68(0);
        func_8008B750(0, 0);
    }
    audio_bgm_control(0x71);
    if (audio_state.bgm_sequence_id != -1) {
        audio_bgm_request_start();
    }
}

void audio_command_bgm(s32 audio_bgm_command) {
    s32 command_word;

    command_word = audio_bgm_command & 0xFFFF;
    switch (command_word & 0xF000) {
    case 0x0:
        if (audio_state.request_flags & 1) {
            audio_state.bgm_next_sequence_id = command_word & 0xFF;
            audio_bgm_fade_finish();
            return;
        }
        if (audio_state.state_flags & 0x100) {
            audio_state.bgm_next_sequence_id = command_word & 0xFF;
            audio_bgm_fade_start(0xB1, command_word);
            return;
        }
        audio_state.bgm_sequence_id = command_word & 0xFF;
        audio_state.bgm_next_sequence_id = -1;
        audio_bgm_request_start();
        return;
    case 0x1000:
        audio_bgm_control(0x71, command_word);
        return;
    case 0x2000:
        audio_bgm_control(0xE1, command_word);
        return;
    case 0x4000:
        audio_bgm_control(0xF1, command_word);
        return;
    }
}

void audio_bgm_request_start(void) {
    if (audio_bgm_bank_ready(audio_state.bgm_slot) == 0) {
        audio_state.request_flags |= 2;
    } else {
        audio_bgm_start();
    }
}

void audio_bgm_start(void) {
    s16 func_800900A0();
    s32 func_8008C720(s16, s16, s16);
    u8 sp2C[0x24];

    if ((game_mode_is_tt_game() == 0) && (game_mode_is_shooter() == 0) && (D_80125D4C != 2)) {
        audio_voices_kill();
    }
    if (audio_state.state_flags & 0x10000000) {
        if (game_mode_is_option_2() != 0) {
            func_8008FF5C(0, 0);
        }
    } else {
        /* FAKE: explicit 16-bit sign-extension reproduces sll 16 / sra 16 for $a1 when calling unprototyped func_8008FF5C. T-24123 */
        func_8008FF5C(audio_sequence_reverb_depth[(u16)D_80125D4A][D_80125D3A], (audio_sequence_reverb_depth[(u16)D_80125D4A][D_80125D3A] << 16) >> 16);
    }
    D_80125D38 = func_800900A0(audio_sequence_address[(u16)D_80125D4A][D_80125D3A], (s16)D_80125D4C);
    if (D_80125D38 == -1) {
        audio_debug_load_error_report(((D_80125D4C << 8) & 0xFF00) | 0x01040000 | ((u16)D_80125D4A & 0xFF));
        audio_bgm_fade_finish();
        return;
    }
    audio_state.bgm_volume = audio_state.bgm_volume_full;
    audio_state.state_flags |= 0x100;
    audio_state.state_flags &= ~0x1000;
    audio_state.request_flags &= ~2;
    audio_state.request_flags &= ~1;
    audio_state.state_flags &= ~0x100000;
    audio_state.request_flags &= ~2;
    func_80090270(audio_state.bgm_slot, sp2C);
    audio_state.bgm_volume = sp2C[0x18] - 0x19;
    if (audio_state.state_flags & 0x20) {
        audio_state.bgm_volume -= 0x18;
    }
    audio_state.bgm_volume_full = audio_state.bgm_volume;
    func_8008B750(0x7F, 0x7F);
    func_8008C720(D_80125D38, audio_state.bgm_volume, audio_state.bgm_volume);
    func_80090350(D_80125D38, 1, 1);
}

/* FAKE: the constant branch retains the selector in v0 as in audio_bgm_fade_start. T-10020, T-24111 */
void audio_bgm_control(arg0)
u16 arg0;
{
    s32 temp_v0;

    if (1) {
        temp_v0 = arg0 & 0xFF;
    }
    switch (temp_v0) {
    case 0x71:
        if ((audio_state.state_flags & 0x100) || (audio_state.state_flags & 0x1000)) {
            func_8008C720(D_80125D38, 0, 0);
            func_80090580(D_80125D38);
            func_800907E0(D_80125D38);
        }
        D_80125D3A = D_80125D3C;
        D_80125D3C = -1;
        audio_state.state_flags = audio_state.state_flags & ~0x100 & ~0x1000;
        D_80125D14 &= ~1;
        break;
    case 0xE1:
        if (audio_state.state_flags & 0x100) {
            func_80090AB0(D_80125D38);
            audio_state.state_flags = (audio_state.state_flags & ~0x100) | 0x1000;
        }
        break;
    case 0xF1:
        if (audio_state.state_flags & 0x1000) {
            SsSeqReplay(D_80125D38);
            audio_state.state_flags = (audio_state.state_flags & ~0x1000) | 0x100;
        }
        break;
    }
}

/* FAKE: the constant branch retains the selector in v0. Keeping it on the
 * flag-store line also preserves the ORI scheduling. T-10020. */
void audio_bgm_fade_start(arg0)
u16 arg0;
{
    s32 temp_v0;
    if (!(audio_state.request_flags & 1))
    {
        audio_state.bgm_fade_accumulator = 0;
        audio_state.request_flags |= 1; if (1)
        {
            temp_v0 = arg0 & 0xFF;
        }
        switch (temp_v0)
        {
        case 0xB1:
            audio_state.bgm_fade_rate = 0x100;
            audio_state.bgm_fade_step = 0xA;
            return;

        case 0xC1:
            audio_state.bgm_fade_rate = 0xC0;
            audio_state.bgm_fade_step = 1;
            return;

        case 0xD1:
            audio_state.bgm_fade_rate = 0x80;
            audio_state.bgm_fade_step = 1;
            break;
        }
    }
}

void audio_command_se_a(u16 audio_se_a_command) {
    s32 packed_effect_key;

    /* FAKE: shift split in two keeps the lhu result out of the lui register; permuter result, real source unknown. T-8010 */
    packed_effect_key = (((D_80125D50 << 2) << 6) & 0xFF00) | (((audio_se_a_command & 0xFF) >> 4) & 0xFF);
    if (audio_se_a_command & 0x1000) {
        audio_se_stop(packed_effect_key, ((audio_se_a_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
        return;
    }
    audio_se_play(packed_effect_key, ((audio_se_a_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
}

void audio_command_se_b(u16 audio_se_b_command) {
    s32 a;

    /* FAKE: shift split in two keeps the lhu result out of the lui register; permuter result, real source unknown. T-8010 */
    a = (((D_80125D52 << 2) << 6) & 0xFF00) | (((audio_se_b_command & 0xFF) >> 4) & 0xFF);
    if (audio_se_b_command & 0x1000) {
        audio_se_stop(a, ((audio_se_b_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
        return;
    }
    audio_se_play(a, ((audio_se_b_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
}

void audio_command_se_bgm_bank(u16 audio_se_bgm_command) {
    s32 a;

    /* FAKE: shift split in two keeps the lhu result out of the lui register; permuter result, real source unknown. T-8010 */
    a = (((D_80125D4C << 2) << 6) & 0xFF00) | (((audio_se_bgm_command & 0xFF) >> 4) & 0xFF);
    if (audio_se_bgm_command & 0x1000) {
        audio_se_stop(a, ((audio_se_bgm_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
        return;
    }
    audio_se_play(a, ((audio_se_bgm_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
}

void audio_command_se_bgm_auxiliary(u16 audio_se_auxiliary_command) {
    s32 a;

    /* FAKE: shift split in two keeps the lhu result out of the lui register; permuter result, real source unknown. T-8010 */
    a = (((D_80125D4E << 2) << 6) & 0xFF00) | (((audio_se_auxiliary_command & 0xFF) >> 4) & 0xFF);
    if (audio_se_auxiliary_command & 0x1000) {
        audio_se_stop(a, ((audio_se_auxiliary_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
        return;
    }
    audio_se_play(a, ((audio_se_auxiliary_command & 0xF) * 4 + 0x18) << 8 & 0xFF00);
}

void audio_se_play(s32 sound_key, s32 packed_note) {
    func_80090D20();
    audio_state.state_flags &= 0xFFEFFFFF;
    func_8008B750(0x7F, 0x7F);
    func_80090D60(sound_key, packed_note, audio_state.se_volume, audio_state.se_volume);
}

void audio_se_stop(void) {
    func_80090D20();
}

s32 audio_queue_push(u16 a) {
    if (D_80125CC0 >= 0x20) {
        return -1;
    }
    D_80125CC0++;
    (&D_80125CC0)[D_80125CC0] = a;
    return 0;
}

void audio_queue_clear(void) {
    D_80125CC0 = 0;
}

u16 audio_queue_pop(void) {
    u16 i;
    u16 ret;

    if (D_80125CC0 > 0x20) {
        D_80125CC0 = 0x20;
    }
    if (D_80125CC0 == 0) {
        ret = 0;
    } else {
        ret = D_80125CC2;
        /* FAKE: (&D_80125CC0)[i] indexes the queue through the scalar count symbol while keeping D_80125CC2 as a separate scalar relocation. T-24127 */
        for (i = 1; i < D_80125CC0; i++) {
            (&D_80125CC0)[i] = (&D_80125CC0)[i + 1];
        }
        D_80125CC0--;
    }
    return ret;
}

void audio_xa_forget(void) {
    audio_xa_control(0x74);
    audio_state.xa_clip_start = 0;
    audio_state.xa_clip_end = 0;
    audio_state.xa_next_start = 0;
    audio_state.xa_next_end = 0;
    audio_state.xa_resume_position = 0;
    audio_state.xa_params[4] = 0;
    audio_state.xa_params[5] = 0;
}

void audio_xa_reset(void) {
    func_8008BEE0(0, 0, 0);
    audio_state.state_flags &= ~0x400;
    audio_state.state_flags &= ~0x4000;
    D_80125D14 &= ~0x200;
    D_80125D14 &= ~0x100;
    audio_cd_level = 0;
    D_80125E62 = 0;
    D_80125D18 = 0;
    D_80125D1C = 0;
    D_80125D20 = 0;
    D_80125D24 = 0;
    D_80125D28 = 0;
    D_80125D5C = 0;
    D_80125D5D = 0;
}

void audio_voices_kill(void) {
    if (game_mode_is_tt_game() == 0 || !(audio_state.state_flags & 0x100)) {
        func_80090DB0();
        func_8008FD68(0);
        func_8008FED0();
        func_800949B0(3);
        func_8008FEB0();
    }
}

void audio_init(void) {
    func_8008B8C4();
    func_8008BAA4(1);
    func_80090DF0(0x18);
    func_8008B904(audio_sequence_table, 1, 1);
    func_80090E30(1);
    func_8008FEF0(3);
    func_800949B0(3);
    func_8008FEB0();
    func_8008FF5C(0x3C, 0x3C);
    func_8008B750(0x7F, 0x7F);
    audio_output_mode_set(2);
    audio_state_init();
    audio_bank_layout_init();
    audio_cd_audio_init();
    SD_InitCDLevelInfo();
    audio_queue_clear();
    func_8008BE34();
}

void audio_bank_layout_init(void) {
    audio_bank_header_address[0] = 0x80010000;
    D_80125D64 = 0x80012000;
    D_80125D68 = 0x80016000;
    D_80125D6C = 0x8001A000;
    audio_bank_sound_ram_address[0] = 0x1010;
    D_80125D84 = audio_bank_sound_ram_address[0] + 0x10000;
    D_80125D88 = D_80125D84 + 0x28000;
    D_80125D8C = D_80125D88 + 0x28000;
    audio_bank_body_size = 0x10000;
    D_80125D94 = 0x28000;
    D_80125D98 = 0x28000;
    D_80125D9C = 0x1A768;
}

void audio_bank_body_address_set(u16 arg0) {
    s32 v;

    v = cd_load_destination_base_get();
    switch (arg0) {
    case 17:
        audio_bank_body_address[0] = v;
        audio_bank_body_address[1] = v + 0x10000;
        break;
    case 18:
        audio_bank_body_address[2] = v;
        audio_bank_body_address[3] = v + 0x28000;
        break;
    case 19:
        audio_bank_body_address[0] = v;
        break;
    case 20:
        audio_bank_body_address[1] = v;
        break;
    case 21:
        audio_bank_body_address[2] = v;
        break;
    case 22:
        audio_bank_body_address[3] = v;
        break;
    }
}

void audio_cd_audio_init(void) {
    audio_state.cd_mode_param[0] = 0xC8;
    func_8008BEE0(0, 0, 0);
    func_8008BFB0(0, 0, 0);
    cd_audio_command(0xE, 0, audio_state.cd_mode_param);
}

void audio_state_init(void) {
    audio_state.request_flags = 0;
    audio_state.state_flags = 0x10;
    audio_state.xa_clip_start = 0;
    audio_state.xa_clip_end = 0;
    audio_state.xa_next_start = 0;
    audio_state.xa_next_end = 0;
    audio_state.xa_resume_position = 0;
    audio_state.xa_volume = 0x64;
    audio_state.xa_volume_full = 0x64;
    audio_state.bgm_volume = 0x50;
    audio_state.bgm_volume_full = 0x50;
    audio_state.se_volume = 0x5A;
    audio_state.se_volume_default = 0x5A;
    audio_state.bgm_sequence_handle = 0;
    audio_state.bgm_sequence_id = -1;
    audio_state.bgm_next_sequence_id = -1;
    audio_state.bgm_fade_accumulator = 0;
    audio_state.bgm_fade_step = 0x80;
    audio_state.bgm_fade_rate = 0xFF;
    audio_state.xa_fade_accumulator = 0;
    audio_state.xa_fade_step = 0x80;
    audio_state.xa_fade_rate = 0xFF;
    audio_state.se_slot_a = 0;
    audio_state.bgm_slot = 1;
    audio_state.bgm_auxiliary_slot = 2;
    audio_state.se_slot_b = 3;
    audio_state.sequence_pack_set = 0;
    audio_state.xa_params[4] = 0;
    audio_state.xa_params[5] = 0;
    audio_state.xa_params[6] = 0;
    audio_state.xa_params[7] = 0;
}

s32 audio_sequence_pack_index(u8 *sequence_pack_base, u16 sequence_pack_set) {
    s32 i;
    s32 off;

    for (i = 0; i < 0x10; i++) {
        /* FAKE: (u32)sequence_pack_base converts i * 4 from signed to unsigned before adding sequence_pack_base, keeping i * 4 as the 4x-unrolled induction variable without folding sequence_pack_base into it. T-24127 */
        off = *(s32 *)(i * 4 + (u32)sequence_pack_base);
        if (off == 0) {
            off = *(s32 *)sequence_pack_base;
        }
        audio_sequence_address[sequence_pack_set][i] = (s32)(sequence_pack_base + off + 0x10);
        audio_sequence_reverb_depth[sequence_pack_set][i] = sequence_pack_base[i + 0x40];
    }
}

void audio_output_mode_set(s32 arg0) {
    u8 volume[4];
    u8 attenuation;

    /* FAKE: paired assignments share source lines to reproduce independent
     * byte-store scheduling; original spelling unknown. T-24062 */
    if ((arg0 & 0xFFFF) == 1) {
        func_80090E40();
        audio_state.state_flags &= ~0x10;
        audio_state.state_flags |= 0x20;
        volume[2] = 0x4F; volume[0] = 0x4F;
        volume[3] = 0x4F; volume[1] = 0x4F;
        attenuation = 0x18;
    } else {
        func_80090E50();
        audio_state.state_flags &= ~0x20;
        audio_state.state_flags |= 0x10;
        volume[2] = 0x7F; volume[0] = 0x7F;
        volume[3] = 0;
        volume[1] = 0;
        attenuation = 0;
    }
    func_80087DC8(volume);
    audio_state.bgm_volume_full = 0x50 - attenuation;
    audio_state.bgm_volume = audio_state.bgm_volume_full;
    /* FAKE: unsigned halfword view preserves the target conversion and
     * temporary numbering; the canonical member stays signed. T-24062 */
    *(u16 *)&audio_state.se_volume_default = 0x5A - attenuation;
    audio_state.se_volume = *(u16 *)&audio_state.se_volume_default;
}

void audio_shutdown(void) {
    s16 var_s0;

    if (audio_state.state_flags & 0x1100) {
        func_80090580(D_80125D38);
        func_800907E0(D_80125D38);
    }
    func_8008FD68(0);
    var_s0 = 3;
    do {
        if ((0x10000 << var_s0) & audio_state.state_flags) {
            func_80090E60(var_s0);
        }
        var_s0 -= 1;
    } while (var_s0 >= 0);
    func_8008BE54();
    func_8008BEBC();
}

void audio_bank_request_transfer(s16 arg0) {
    /* FAKE: `^ 0` turns the left operand into an expression, which makes uopt swap the operands of the compare (decomp-permuter, score 0); real source unknown. T-4090 */
    if ((arg0 ^ 0) == (u16) audio_state.bgm_slot && (audio_state.state_flags & 0x100)) {
        audio_bgm_fade_finish();
    }
    audio_state.request_flags |= 0x01000000 << arg0;
}

void audio_bank_dispose(s16 arg0) {
    if (arg0 == D_80125D4C) {
        if (audio_state.state_flags & 0x100) {
            func_80090580(D_80125D38);
            func_800907E0(D_80125D38);
        }
        D_80125D3A = -1;
        D_80125D3C = -1;
    } else if (arg0 == D_80125D50) {
        func_8008FD68(0);
    }
    func_80090E60(arg0);
}

void audio_bgm_next_clear(void) {
    audio_state.bgm_next_sequence_id = -1;
}

void audio_reverb_off_toggle(void) {
    if (audio_state.state_flags & 0x10000000) {
        audio_state.state_flags &= 0xEFFFFFFF;
        return;
    }
    audio_state.state_flags |= 0x10000000;
}

/* Reset the CD level meter work area: outputs 0, precision 8 bits, 32 samples per channel, mode 0x212 (average, no squaring) */
void SD_InitCDLevelInfo(void) {
    audio_cd_level = 0;
    D_80125E62 = 0;
    D_80125E64 = 8;
    D_80125E66 = 0x20;
    D_80125E68 = 0x212;
}
