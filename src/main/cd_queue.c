/* cd_queue.c: src/main/800451D0.c; Original object 0x800451D0-0x80046500: CD command ring/pump, queued sector reads and checksum, read/audio state and XA clip requests. Original object 0x800451D0-0x80046500 (config/objects/main.txt). */
#define MAIN_API_OVERRIDE_cd_audio_command /* K&R u8 arg0 narrows the command byte on entry (T-24123). */
#define MAIN_API_OVERRIDE_func_800AD950 /* implicit int declaration (VSync returns int): a void prototype gives the
                                          * cd_command_pump wait-loop counter $v0 instead of $v1 (T-25123) */
#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/data/main/cd_queue.data", cd_read_checked);
INCLUDE_RODATA("asm/data/main/cd_queue.data", cd_read_retry_count);
INCLUDE_RODATA("asm/data/main/cd_queue.data", cd_sectors_left);
INCLUDE_RODATA("asm/data/main/cd_queue.data", cd_last_read_lba);
s32 cd_audio_request[4] = { 0, 0, 0, 0 };

s32 cd_load_destination_base_get(void) {
    return cd_load_destination_base;
}

void cd_load_destination_base_set(s32 arg0) {
    cd_load_destination_base = arg0;
}

void cd_read_sync_poll(void) {
    cd_read_status = 0x80;
    cd_sectors_left = func_80087E4C(1, cd_read_sync_result);
}

void cd_sync_poll(void) {
    switch (func_80087954(1, cd_sync_result)) {
    case 2:
        cd_read_status |= 2;
        break;
    case 5:
        cd_queue_phase = -1;
        break;
    }
}

s32 cd_queue_idle(void) {
    if (cd_queue_tail == cd_queue_head && cd_queue_phase == -1) {
        return 1;
    }
    return 0;
}

void cd_command_reset(void) {
    cd_queue_head = 0;
    cd_queue_tail = 0;
    cd_read_status = 2;
    cd_queue_phase = -1;
    cd_audio_position = 0;
    cd_state = 0;
    cd_read_count = 0;
    cd_read_checked = 0;
    cd_last_read_lba = 0;
}

s32 cd_command_push(arg0, arg1, arg2)
u8 arg0;
s32 arg1;
s32 arg2;
{
    s32 src;

    /* FAKE: the copy of arg2 keeps it in $a3 (move a3,a2) instead of its stack home. T-11330 */
    src = arg2;
    if (((cd_queue_tail + 1) % 0x30U) == (cd_queue_head % 0x30U)) {
        return -1;
    }
    cd_command_ring[cd_queue_tail].command = arg0;
    cd_command_ring[cd_queue_tail].lba = arg1;
    cd_command_ring[cd_queue_tail].param[15] = 0;
    if (src == 1) {
        cd_command_ring[cd_queue_tail].param[15] = 0xFF;
    } else if (src != 0) {
        memcpy(cd_command_ring[cd_queue_tail].param, src, 0x10);
    }
    /* FAKE: `D = D + 1` rather than `D++`, which gives another register order. T-11330 */
    cd_queue_tail = cd_queue_tail + 1;
    if (cd_queue_tail < 0x30U) {
        return 1;
    }
    cd_queue_tail = 0;
}

void cd_audio_command(cd_operation_id, sector_lba, command_params)
u8 cd_operation_id;
s32 sector_lba;
u8 *command_params;
{
    u8 buf[0x20];

    sector_lba = ((u32) sector_lba >= 0x4AB51U) ? 0x4AB50 : sector_lba;
    switch (cd_operation_id) {
    case 0:
        cd_command_push(0, 0, 0);
        break;
    case 9:
        cd_command_push(9, 0, 0);
        break;
    case 6:
        cd_last_read_lba = sector_lba;
        cd_command_push(6, sector_lba, (s32) command_params);
        break;
    case 14:
        cd_command_push(0xE, 0, (s32) command_params);
        break;
    case 13:
        if (sector_lba == 0) {
            cd_command_push(0xD, 0, (s32) command_params);
        } else {
            cd_command_push(0xD, 0, (s32) command_params);
            cd_command_push(2, sector_lba, 0);
            cd_command_push(0x1B, sector_lba, 0);
            cd_last_read_lba = sector_lba;
        }
        break;
    case 2:
        cd_command_push(2, sector_lba, 0);
        break;
    case 21:
        cd_command_push(2, sector_lba, 0);
        cd_command_push(0x15, sector_lba, 0);
        break;
    case 10:
        cd_command_push(0xA, sector_lba, 0);
        break;
    case 3:
        buf[0x10] = 5;
        cd_command_push(0xE, 0, (s32) &buf[0x10]);
        cd_command_push(2, sector_lba, 0);
        cd_command_push(3, 0, 0);
        break;
    case 27:
        break;
    }
}

/* FAKE: view of the queue header bytes cd_queue_head..cd_queue_phase through one symbol; the load and
 * store order of the original needs one symbol, the real aggregate is not declared yet (T-24501). */
typedef struct CdRingState {
    /* 0x00 */ u8 head;
    /* 0x01 */ u8 tail;
    /* 0x02 */ u8 status;
    /* 0x03 */ s8 command_phase;
    /* 0x04 */ u8 pad[4];
} CdRingState; /* size 0x08 */
extern u16 cd_read_retry_count;
extern u32 *cd_read_end_word_pointer;
void cd_error_recover(void);

void cd_command_pump(void) {
    s32 i; /* home 0x4C unused: the loop test is strength-reduced (see the FAKE at the loop) */
    u32 sp48;
    s32 sp44;
    u16 pad16; /* FAKE: unused frame home at 0x40, not a source recovery. T-24501 */
    s32 pad; /* FAKE: measured frame home, not a source recovery. T-24501 */
    s32 sp38[1];
    u8 sp30[8];

    if (((CdRingState *)&cd_queue_head)->tail != ((CdRingState *)&cd_queue_head)->head) {
        cd_busy_marker_draw();
        if (((CdRingState *)&cd_queue_head)->command_phase == -1) {
            switch (cd_command_ring[((CdRingState *)&cd_queue_head)->head].command) {
            case 0:
                if (func_80087954(1, &sp30) != 0) {
                    cd_state = 0;
                    if (func_800879D0(9, 0, &cd_result) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = 1;
                    }
                }
                break;
            case 3:
                func_8008B750(0x7F, 0x7F);
                ((CdRingState *)&cd_queue_head)->status = 0;
                cd_state = 0;
                if (func_800879D0(3, 0, 0) == 1) {
                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                    ((CdRingState *)&cd_queue_head)->head += 1;
                }
                break;
            case 13:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    if (func_800879D0(0xD, cd_command_ring[((CdRingState *)&cd_queue_head)->head].param, &cd_result) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = -1;
                        ((CdRingState *)&cd_queue_head)->head += 1;
                    }
                }
                break;
            case 9:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    if (func_800879D0(9, 0, &cd_result) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = 1;
                    }
                }
                break;
            case 14:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    if (func_800879D0(0xE, cd_command_ring[((CdRingState *)&cd_queue_head)->head].param, &cd_result) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = -1;
                        ((CdRingState *)&cd_queue_head)->head += 1;
                    }
                }
                break;
            case 2:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    func_80087EE8(cd_command_ring[((CdRingState *)&cd_queue_head)->head].lba, sp38);
                    if (func_800879D0(2, sp38, &cd_result) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = -1;
                        ((CdRingState *)&cd_queue_head)->head += 1;
                    }
                }
                break;
            case 21:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    func_80087EE8(cd_command_ring[((CdRingState *)&cd_queue_head)->head].lba, sp38);
                    if (func_80087B1C(0x15, sp38) == 1) {
                        ((CdRingState *)&cd_queue_head)->command_phase = 1;
                    }
                }
                break;
            case 27:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    func_800879A8(0);
                    func_80087E08(0);
                    func_80088070();
                    func_80087EE8(cd_command_ring[((CdRingState *)&cd_queue_head)->head].lba, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    if (func_800880C0(0xC8) == 0) {
                        do {

                        } while (func_800880C0(0xC8) == 0);
                    }
                    cd_state = 0;
                    ((CdRingState *)&cd_queue_head)->command_phase = 1;
                }
                break;
            case 10:
                ((CdRingState *)&cd_queue_head)->status = 0;
                cd_state = 0;
                if (func_800879D0(0xA, 0, 0) == 0) {
                    do {

                    } while (func_800879D0(0xA, 0, 0) == 0);
                }
                ((CdRingState *)&cd_queue_head)->command_phase = -1;
                ((CdRingState *)&cd_queue_head)->head += 1;
                break;
            case 6:
                if (func_80087954(1, &sp30) != 0) {
                    ((CdRingState *)&cd_queue_head)->status = 0;
                    cd_state = 0;
                    sp44 = (*(u32 *)cd_command_ring[((CdRingState *)&cd_queue_head)->head].param & 0xFFFFFF) + 0x80000000;
                    sp48 = *(u32 *)cd_command_ring[((CdRingState *)&cd_queue_head)->head].param >> 24;
                    cd_read_end_word_pointer = (u32 *)((*(u32 *)cd_command_ring[((CdRingState *)&cd_queue_head)->head].param & 0xFFFFFF) + (sp48 << 11) + 0x7FFFFFFC);
                    func_800879A8(0);
                    func_80087E08(0);
                    cd_mode_scratch = 0x84;
                    if (func_80087C64(0xE, &cd_mode_scratch, 0) == 0) {
                        do {

                        } while (func_80087C64(0xE, &cd_mode_scratch, 0) == 0);
                    }
                    func_80087EE8(cd_command_ring[((CdRingState *)&cd_queue_head)->head].lba, sp38);
                    if (func_80087C64(2, sp38, 0) == 0) {
                        do {

                        } while (func_80087C64(2, sp38, 0) == 0);
                    }
                    func_80088070();
                    if (func_80087E6C((s32) sp48, sp44, 0x80) == 0) {
                        do {

                        } while (func_80087E6C((s32) sp48, sp44, 0x80) == 0);
                    }
                    ((CdRingState *)&cd_queue_head)->command_phase = 1;
                }
                break;
            }
        } else if (((CdRingState *)&cd_queue_head)->command_phase == 1) {
            switch (cd_command_ring[((CdRingState *)&cd_queue_head)->head].command) {
            case 0:
                if ((func_80087954(1, cd_result.bytes) != 0) && (func_800879D0(1, 0, &cd_result) == 1) && !(cd_result.bytes[0] & 0x40)) {
                    cd_state = 2;
                    ((CdRingState *)&cd_queue_head)->head += 1;
                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                }
                break;
            case 9:
                if ((func_80087954(1, cd_result.bytes) != 0) && (func_800879D0(1, 0, &cd_result) == 1) && !(cd_result.bytes[0] & 0x40)) {
                    cd_state = 2;
                    ((CdRingState *)&cd_queue_head)->head += 1;
                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                }
                break;
            case 21:
                if ((func_800879D0(1, 0, &cd_result) == 1) && !(cd_result.bytes[0] & 0x40)) {
                    cd_state = 2;
                    ((CdRingState *)&cd_queue_head)->head += 1;
                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                }
                break;
            case 6:
                if (func_80087954(1, cd_result.bytes) != 0) {
                    cd_sectors_left = func_80087E4C(1, cd_read_sync_result);
                    if (cd_sectors_left <= 0) {
                        if (cd_sectors_left == 0) {
                            if (cd_read_checked != 0) {
                                if ((*cd_read_end_word_pointer & 0xFFFF0000) != 0x10120000) {
                                    ((CdRingState *)&cd_queue_head)->status = 0;
                                    if ((++cd_read_retry_count & 3) == 3) {
                                        if (func_800879D0(0xA, 0, 0) == 0) {
                                            do {

                                            } while (func_800879D0(0xA, 0, 0) == 0);
                                        }
                                        /* FAKE: `* 1` makes uopt strength-reduce the test into an induction
                                         * temporary, spilled to the 0x2C slot below the locals as in the
                                         * original; the plain counter spills to its own home. T-25123 */
                                        for (i = 0; i * 1 != 0x200; i++) {
                                            func_800AD950(0);
                                        }
                                    }
                                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                                } else {
                                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                                    ((CdRingState *)&cd_queue_head)->head += 1;
                                    cd_state = 2;
                                    ((CdRingState *)&cd_queue_head)->status |= 1;
                                    /* FAKE: cd_read_count through the cd_state symbol: one object in the
                                     * original (aggregate unknown); as1 then keeps the status load
                                     * before the cd_state store. T-25123 */
                                    ((u8 *)&cd_state)[1] += 1;
                                }
                            } else {
                                ((CdRingState *)&cd_queue_head)->command_phase = -1;
                                ((CdRingState *)&cd_queue_head)->head += 1;
                                cd_state = 2;
                                ((CdRingState *)&cd_queue_head)->status |= 1;
                                /* FAKE: as above, cd_read_count through cd_state. T-25123 */
                                ((u8 *)&cd_state)[1] += 1;
                            }
                        } else if (cd_sectors_left < 0) {
                            ((CdRingState *)&cd_queue_head)->command_phase = -1;
                            cd_state = 0;
                            ((CdRingState *)&cd_queue_head)->status = 0;
                        }
                    }
                }
                break;
            case 27:
                if (func_80087954(1, cd_result.bytes) == 5) {
                    ((CdRingState *)&cd_queue_head)->command_phase = -1;
                    cd_error_recover();
                } else {
                    if (func_800879D0(1, 0, &cd_result) == 0) {
                        do {

                        } while (func_800879D0(1, 0, &cd_result) == 0);
                    }
                    if (cd_result.bytes[0] & 0x20) {
                        ((CdRingState *)&cd_queue_head)->status = 0;
                        ((CdRingState *)&cd_queue_head)->command_phase = -1;
                        if (cd_command_ring[((CdRingState *)&cd_queue_head)->head].param[15] != 0xFF) {
                            cd_state = 4;
                        }
                        ((CdRingState *)&cd_queue_head)->head += 1;
                    }
                }
                break;
            }
        }
    } else if (func_80087954(1, 0) == 5) {
        if (cd_last_command() == 0x1B) {
            ((CdRingState *)&cd_queue_head)->command_phase = -1;
            if (((CdRingState *)&cd_queue_head)->head != 0) {
                ((CdRingState *)&cd_queue_head)->head -= 1;
            } else {
                ((CdRingState *)&cd_queue_head)->head = 0x2F;
            }
            cd_error_recover();
        } else {
            func_800879D0(1, 0, 0);
        }
    } else if (cd_state == 4) {
        cd_audio_position = cd_position_lba();
    }
    if ((u8) ((CdRingState *)&cd_queue_head)->head >= 0x30U) {
        ((CdRingState *)&cd_queue_head)->head = 0;
    }
}

u8 cd_last_command(void) {
    return cd_command_ring[(cd_queue_head + 0x2FU) % 0x30U].command;
}

/* Return the CD read status byte (bit 0 = read finished) */
u8 cd_read_sync_status(void) {
    return cd_read_status;
}

u8 cd_done_count(void) {
    return cd_read_count;
}

u8 cd_audio_state(void) {
    return cd_state;
}

s32 cd_position_lba(void) {
    u8 pad[8]; /* FAKE: unused pad places local/saved/status at 0x44/0x3C/0x34 in the 0x58 frame. T-24111 */
    s32 retries;
    WordBytes *state;
    u8 local[4];
    u8 *index = &cd_queue_head;
    s32 saved;
    u8 status[8];

    saved = cd_result.word;
    state = &cd_result;
    /* FAKE: same-line zero init and 4-byte clear chain emit move v0, zero and sb v0 for bytes 2..0. T-24111 */
    retries = 0; (&state->bytes[0])[0] = (&state->bytes[1])[0] = (&state->bytes[2])[0] = (&state->bytes[3])[0] = 0;
    if (func_80087954(1, status) != 0) {
        while (func_80087C64(0x10, 0, state) == 0) {
            retries++;
            if (retries >= 16) {
                if (cd_last_command() == 0x1B) {
                    cd_queue_phase = -1;
                    if ((*index) != 0) {
                        (*index) -= 1;
                    } else {
                        (*index) = 0x2F;
                    }
                }
                state->word = saved;
                break;
            }
        }
    }
    local[0] = state->bytes[0];
    local[1] = state->bytes[1];
    local[2] = state->bytes[2];
    retries = func_80087FF0(local);
    if (cd_binary_coded_decimal_to_integer(state->bytes[0]) >= 0x4B) return 0;
    if (cd_binary_coded_decimal_to_integer(state->bytes[1]) >= 0x3C) return 0;
    if (cd_binary_coded_decimal_to_integer(state->bytes[2]) >= 0x4B) return 0;
    if (retries > 0) return retries;
    return 0;
}

s32 cd_audio_ticks(void) {
    return cd_audio_position;
}

s32 *cd_audio_request_get(void) {
    return cd_audio_request;
}

void cd_audio_request_set(s32 clip_start_index, s32 clip_end_index, s32 speaker_id) {
    s32 idx = speaker_id & 0xF;

    cd_audio_request[0] = idx;
    cd_audio_request[2] = clip_start_index;
    cd_audio_request[3] = clip_end_index;
    cd_audio_request[1] = cd_audio_speaker_offset_table[idx];
}

void cd_read_param_pack(u8 sector_count, s32 destination_address, s32 *read_parameter_out) {
    *read_parameter_out = destination_address;
    ((u8 *)read_parameter_out)[3] = sector_count;
}

void cd_read_sectors_nowait(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    audio_command(6);
    cd_read_param_pack(arg0, arg1, (s32 *)buf);
    cd_audio_command(6, arg2, buf);
    cd_read_checked = 0;
}

/* Read n sectors to a buffer from an LBA through the CD command ring and wait */
void cd_read_sectors(u8 arg0, s32 arg1, s32 arg2) {
    /* FAKE: size 0x20 is taken from the original frame (local at 0x28 in 0x48);
     * the real type of this buffer is unknown. T-0016 */
    u8 buf[0x20];

    audio_command(6);
    cd_read_param_pack(arg0, arg1, (s32 *)buf);
    cd_audio_command(6, arg2, buf);
    cd_read_checked = 1;
}

/* True when the 16-bit sum of all bytes of a loaded file except the last 4 equals the big-endian u16 stored at its end. */
s32 cd_file_checksum_ok(blocks, buf)
u8 blocks;
u8 *buf;
{
    u32 lo;
    u16 sum;
    u32 hi;
    u32 i;
    u8 *p0;
    u8 *p1;
    u8 *end;

    sum = 0;
    i = 0;
    if (((blocks << 11) - 4) != 0) {
        p0 = buf;
        p1 = buf + 1;
        /* FAKE: goto into for-loop body omits the redundant entry test while keeping the u16 sum narrowing in the for-step after sltu. T-24087 */
        goto loop;
        for (; i < (blocks << 11) - 4; i += 2, p0 += 2, p1 += 2) {
        loop:
            sum += *p0 + *p1;
        }
    }
    end = i + buf;
    hi = end[0];
    lo = end[1];
    if (sum != ((hi << 8) + lo)) {
        return 0;
    }
    return 1;
}

/* s32 fallthrough preserves original constant scheduling (T-9710). */
s32 cd_busy_marker_draw(void) {
    if ((D_8011ED17 & 0x80) && (game_state.frame_count & 4)) {
        prim_gradient_line(0x9E, 0x5A, 0x9E, 0x62, 0xFFFF00, 0xFFFF00, 0x80);
        prim_gradient_line(0x9D, 0x5A, 0x9D, 0x62, 0xFFFF00, 0xFFFF00, 0x80);
    }
}

void cd_error_recover(void) {
    s32 i;

    cd_mode_scratch = 0x84;
    while (func_80087C64(0xE, &cd_mode_scratch, 0) == 0) {
    }
    for (i = 0; i != 0x200; i++) {
        func_800AD950(0);
    }
}
