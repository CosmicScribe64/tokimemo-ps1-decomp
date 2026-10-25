/* memory_card.c: src/main/80053650.c; T-27059: object [0x80053650,0x80056300); BIOS memory-card events, card polling/file jobs and system/game save images dominate; first two helpers set/decode MDEC still-image buffers, so keep the complete original object (reuse m-flow candidate). Original object 0x80053650-0x80056300 (config/objects/main.txt). */
#include "common.h"
#include "common.h"
#include "game/game_state.h"
#include "game/save_image.h"
#include "game/system_data.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi_legacy.h"
#include "libetc.h"
#include "libgpu.h"
#include "libpress.h"
#include "main/bg.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/resident_views.h"
#include "main/memory_card.h"
#include "main/state.h"
#include "main/text.h"
#include "game/word_views.h"
#include "main/bg.h"
#include "main/memory_card.h"

void bg_still_buffers_set(s32 arg0, s32 arg1, s32 arg2) {
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    bg_still_variable_length_code_buffer = arg0;
    bg_still_out_buffer = arg1;
    bg_still_bitstream = arg2;
}

s32 bg_still_decode_to_vram(arg0, arg1, arg2, arg3)
u16 arg0;
u16 arg1;
u16 arg2;
u16 arg3;
{
    RECT rect;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original frame (0x58); real source unknown. T-10320 */
    u32 end;
    s32 v;

    if ((*(u32 *)bg_still_bitstream & 0xFFFF0000) != 0x38000000 || (((u32 *)bg_still_bitstream)[1] & 0xFF010000) != 0x10000) {
        return -1;
    }
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_80086870(bg_still_bitstream, bg_still_variable_length_code_buffer);
    func_800868BC(bg_still_variable_length_code_buffer, 2);
    rect.y = arg1;
    rect.w = 0x10;
    rect.x = arg0;
    rect.h = arg3;
    end = arg0 + arg2;
    if (rect.x < end) {
        v = ((arg3 + 8) >> 4) << 7;
        do {
            func_80086944(bg_still_out_buffer, v);
            func_80086984(0);
            func_8009C884(&rect, bg_still_out_buffer);
            rect.x += 0x10;
        } while (rect.x < end);
    }
    return 1;
}

/* Initializes software and hardware memory-card events and starts card services. */
void card_ev_set(void) {
    Sw_Start();
    Hw_Start();
    InitCARD(1);
    StartCARD();
    _bu_init();
    _card_auto(0);
}

/* Opens and enables software memory-card event notifications. */
void Sw_Start(void) {
    EnterCriticalSection();
    memory_card_software_event_done = OpenEvent(0xF4000001, 4, 0x2000, 0);
    memory_card_software_event_error = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    memory_card_software_event_timeout = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    memory_card_software_event_new_card = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    EnableEvent(memory_card_software_event_done);
    EnableEvent(memory_card_software_event_error);
    EnableEvent(memory_card_software_event_timeout);
    EnableEvent(memory_card_software_event_new_card);
    ExitCriticalSection();
}

/* Opens and enables hardware memory-card event notifications. */
void Hw_Start(void) {
    EnterCriticalSection();
    memory_card_hardware_event_done = OpenEvent(0xF0000011, 4, 0x2000, 0);
    memory_card_hardware_event_error = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    memory_card_hardware_event_timeout = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    memory_card_hardware_event_new_card = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    EnableEvent(memory_card_hardware_event_done);
    EnableEvent(memory_card_hardware_event_error);
    EnableEvent(memory_card_hardware_event_timeout);
    EnableEvent(memory_card_hardware_event_new_card);
    ExitCriticalSection();
}

/* Polls software memory-card events and returns the detected result code. */
u8 Sw_Test(void) {
    game_state.memory_card_result = 0;
    if (TestEvent(memory_card_software_event_done) != 0) {
        game_state.memory_card_result = 1;
    } else if (TestEvent(memory_card_software_event_error) != 0) {
        game_state.memory_card_result = 2;
    } else if (TestEvent(memory_card_software_event_timeout) != 0) {
        game_state.memory_card_result = 4;
    } else if (TestEvent(memory_card_software_event_new_card) != 0) {
        game_state.memory_card_result = 8;
    }
    if (game_state.memory_card_result != 0) {
        Sw_Clear();
    }
    return game_state.memory_card_result;
}

/* Clears pending software memory-card event notifications. */
void Sw_Clear(void) {
    TestEvent(memory_card_software_event_done);
    TestEvent(memory_card_software_event_error);
    TestEvent(memory_card_software_event_timeout);
    TestEvent(memory_card_software_event_new_card);
}

/* Waits for a hardware memory-card event and returns its result code. */
u8 Hw_Test(void) {
    game_state.memory_card_result = 0;
    do {
        if (TestEvent(memory_card_hardware_event_done) != 0) {
            game_state.memory_card_result = 0x81;
        } else if (TestEvent(memory_card_hardware_event_error) != 0) {
            game_state.memory_card_result = 0x82;
        } else if (TestEvent(memory_card_hardware_event_timeout) != 0) {
            game_state.memory_card_result = 0x84;
        } else if (TestEvent(memory_card_hardware_event_new_card) != 0) {
            game_state.memory_card_result = 0x88;
        }
    } while (game_state.memory_card_result == 0);
    Hw_Clear();
    return game_state.memory_card_result;
}

/* Clears pending hardware memory-card event notifications. */
void Hw_Clear(void) {
    TestEvent(memory_card_hardware_event_done);
    TestEvent(memory_card_hardware_event_error);
    TestEvent(memory_card_hardware_event_timeout);
    TestEvent(memory_card_hardware_event_new_card);
}

/* Closes the hardware memory-card event handles. */
void Hw_Stop(void) {
    EnterCriticalSection();
    CloseEvent(memory_card_hardware_event_done);
    CloseEvent(memory_card_hardware_event_error);
    CloseEvent(memory_card_hardware_event_timeout);
    CloseEvent(memory_card_hardware_event_new_card);
    ExitCriticalSection();
}

void memory_card_step_set(u8 arg0) {
    game_state.memory_card_result = 0;
    game_state.memory_card_step = arg0;
}

void memory_card_step_next(void) {
    game_state.memory_card_result = 0;
    game_state.memory_card_step += 1;
}

void memory_card_start(void) {
    game_state.memory_card_busy = 1;
    game_state.memory_card_step = 0;
    game_state.memory_card_result = 0;
    game_state.memory_card_retry_count = 0;
    game_state.memory_card_timer = 0;
}

void memory_card_stop(void) {
    game_state.memory_card_busy = 0;
    game_state.memory_card_timer = 0;
}

void memory_card_event_handle(u8 arg0) {
    switch (Sw_Test()) {
    case 1:
        memory_card_step_next();
        return;
    case 8:
        memory_card_step_set(arg0);
        return;
    case 4:
        game_state.memory_card_retry_count += 1;
        memory_card_step_set(0);
        if ((u32)game_state.memory_card_retry_count >= 3) {
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xFF;
            return;
        }
    case 0:
        return;
    default:
        memory_card_step_set(0);
        break;
    }
}

s32 memory_card_status_wait(void) {
    s32 status;

    status = _card_status(0);
    while (status == 0) {
        status = _card_status(0);
    }
    switch (status) {
    case 1:
        close(memory_card_file_descriptor);
        break;
    case 2:
        close(memory_card_file_descriptor);
        break;
    case 4:
        close(memory_card_file_descriptor);
        break;
    case 8:
        close(memory_card_file_descriptor);
        break;
    case 0x11:
        game_state.memory_card_retry_count += 1;
        memory_card_step_set(0);
        close(memory_card_file_descriptor);
        break;
    case 0x21:
        game_state.memory_card_retry_count += 1;
        memory_card_step_set(0);
        close(memory_card_file_descriptor);
        break;
    }
    return status;
}

s32 memory_card_check_poll(void) {
    game_state.memory_card_timer += 1;
    if (game_state.memory_card_busy != 0) {
        /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in
         * TEL (T-10630). T-17000 */
        switch (*(u8 *)&game_state.memory_card_step) {
        case 0x0:
            game_state.memory_card_result = 0;
            Sw_Clear();
            if (_card_info(0) == 1) {
                memory_card_step_next();
            }
            break;
        case 0x1:
            memory_card_event_handle(0x20);
            break;
        case 0x2:
            if (_card_load(0) == 1) {
                memory_card_step_next();
            }
            break;
        case 0x3:
            memory_card_event_handle(0x40);
            break;
        case 0x4:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 1;
            break;
        case 0x20:
            func_800AD950(0);
            Hw_Clear();
            if (_card_clear(0) == 0) {
                memory_card_step_set(0);
            } else {
                Hw_Test();
                switch (game_state.memory_card_result) {
                default:
                case 0x0:
                case 0x81:
                    memory_card_step_set(0);
                    break;
                case 0x88:
                    memory_card_step_set(0x30);
                    break;
                }
            }
            break;
        case 0x30:
            if (_card_load(0) == 1) {
                memory_card_step_next();
            }
            break;
        case 0x31:
            memory_card_event_handle(0x40);
            break;
        case 0x32:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 1;
            break;
        case 0x40:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xF1;
            break;
        }
    }
    return (s32) game_state.memory_card_result;
}

s32 memory_card_format(void) {
    if (format((u8 *)"bu00:") == 1) {
        return 1;
    }
    return 0;
}

s32 memory_card_scan_poll(void) {
    GameState *g = &game_state;

    g->memory_card_timer += 1;
    if (g->memory_card_busy != 0) {
        if (g->memory_card_step == 0) {
            g->memory_card_result = 0;
            g->save_slot = g->memory_card_step;
            if (memory_card_file_exists(g->memory_card_step) == 1) {
                D_800E8BEE = 1;
                memory_card_step_next();
            } else {
                g->memory_card_retry_count += 1;
                if (g->memory_card_retry_count >= 3U) {
                    D_800E8BEE = 0;
                    memory_card_step_next();
                }
            }
        } else if (g->memory_card_step < 0xFU) {
            g->save_slot = g->memory_card_step;
            if (memory_card_file_exists(g->save_slot) == 1) {
                D_800E7D14[g->save_slot] = 1;
            } else {
                D_800E7D14[g->save_slot] = 0;
            }
            g->memory_card_step += 1;
        } else {
            g->memory_card_busy = 0;
            g->memory_card_result = 2;
        }
    }
    return (s32) g->memory_card_result;
}

u8 memory_card_system_read_poll(void) {
    game_state.memory_card_timer += 1;
    if (game_state.memory_card_busy != 0) {
        switch (game_state.memory_card_step) {
        case 0:
            Sw_Clear();
            game_state.save_slot = 0;
            if (memory_card_file_read(0) == 1) {
                memory_card_step_next();
            } else {
                game_state.memory_card_result = 0xD0;
                game_state.memory_card_busy = 0;
            }
            break;
        case 1:
            if (memory_card_status_wait() == 1) {
                memory_card_step_next();
            }
            break;
        case 2:
            game_state.memory_card_result = 3;
            game_state.memory_card_busy = 0;
            break;
        case 16:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xF0;
            break;
        }
    }
    return game_state.memory_card_result;
}

s32 memory_card_system_write_poll(void) {
    game_state.memory_card_timer += 1;
    if (game_state.memory_card_busy != 0) {
        switch (game_state.memory_card_step) {
        case 0:
            Sw_Clear();
            game_state.save_slot = 0;
            if (memory_card_file_write(0) == 1) {
                memory_card_step_next();
            } else {
                game_state.memory_card_result = 0xD0;
                game_state.memory_card_busy = 0;
            }
            break;
        case 1:
            if (memory_card_status_wait() == 1) {
                memory_card_step_next();
            }
            break;
        case 2:
            game_state.memory_card_result = 0x13;
            game_state.memory_card_busy = 0;
            break;
        case 16:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xF0;
            break;
        }
    }
    return (s32) game_state.memory_card_result;
}

u8 memory_card_save_read_poll(void) {
    game_state.memory_card_timer += 1;
    if (game_state.memory_card_busy != 0) {
        switch (game_state.memory_card_step) {
        case 0:
            Sw_Clear();
            if (memory_card_file_read(game_state.save_slot) == 1) {
                memory_card_step_next();
            } else {
                game_state.memory_card_result = 0xD0;
                game_state.memory_card_busy = 0;
            }
            break;
        case 1:
            if (memory_card_status_wait() == 1) {
                memory_card_step_next();
            }
            break;
        case 2:
            game_state.memory_card_result = 4;
            game_state.memory_card_busy = 0;
            break;
        case 16:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xF0;
            break;
        }
    }
    return game_state.memory_card_result;
}

u8 memory_card_save_write_poll(void) {
    game_state.memory_card_timer += 1;
    if (game_state.memory_card_busy != 0) {
        switch (game_state.memory_card_step) {
        case 0:
            Sw_Clear();
            if (memory_card_file_write(game_state.save_slot) == 1) {
                memory_card_step_next();
            } else {
                game_state.memory_card_result = 0xD0;
                game_state.memory_card_busy = 0;
            }
            break;
        case 1:
            if (memory_card_status_wait() == 1) {
                memory_card_step_next();
            }
            break;
        case 2:
            game_state.memory_card_result = 0x14;
            game_state.memory_card_busy = 0;
            break;
        case 16:
            game_state.memory_card_busy = 0;
            game_state.memory_card_result = 0xF0;
            break;
        }
    }
    return game_state.memory_card_result;
}

s32 memory_card_file_create(s32 file_index) {
    MemoryCardFileRequest req;
    s32 fd;

    memory_card_file_name_build(req.name, file_index);
    close(memory_card_file_descriptor);
    fd = open(req.name, 0x10200);
    memory_card_file_descriptor = fd;
    if (fd == -1) {
        return -1;
    }
    close(memory_card_file_descriptor);
    return 1;
}

s32 memory_card_file_delete(s32 file_index) {
    /* FAKE: retain the observed final stack counter write (T-9510). */
    volatile s32 counter;
    u8 name[32];
    s32 retry;

    memory_card_file_name_build(name, file_index);
    if (delete(name) != 1) {
        /* Original code reads this counter before initialization. */
        retry = counter;
        do {
            retry++;
            if (retry >= 3) {
                return -1;
            }
        } while (delete(name) != 1);
        counter = retry;
    }
    return 1;
}

s32 memory_card_file_read(s32 file_index) {
    /* FAKE: unused padding reproduces the 0x78-byte frame and filename
     * buffer at SP+0x4C; retry control flow matches naturally. T-9510. */
    s32 retry;
    s32 pad_before[2];
    u8 name[32];
    s32 pad_after[4];

    retry = 0;
    memory_card_block_clear();
    memory_card_file_name_build(name, file_index);
    close(memory_card_file_descriptor);
    memory_card_file_descriptor = open(name, 0x8001);
    if (memory_card_file_descriptor == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
        } while ((memory_card_file_descriptor = open(name, 0x8001)) == -1);
        retry = 0;
    }
    if (read(memory_card_file_descriptor, memory_card_block, 0x2000) == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
        } while (read(memory_card_file_descriptor, memory_card_block, 0x2000) == -1);
    }
    return 1;
}

s32 memory_card_file_write(s32 file_index) {
    /* FAKE: pad1/pad2 reproduce the 0x70-byte frame (sp47/sp46 at SP+0x47/0x46,
     * idx spilled at SP+0x50); real locals unknown. T-24214. */
    u8 name[28];
    s32 idx;
    s32 pad1[2];
    u8 sp47;
    u8 sp46;
    s32 pad2[1];
    s32 retry;

    retry = 0;
    memory_card_block_clear();
    memory_card_file_name_build(name, file_index);
    close(memory_card_file_descriptor);
    memory_card_file_descriptor = open(name, 0x8002);
    if (memory_card_file_descriptor == -1) {
        do {
            retry++;
            if (memory_card_file_create(file_index) == -1) {
                return -1;
            }
            if (retry >= 16) {
                return -1;
            }
        } while ((memory_card_file_descriptor = open(name, 0x8002)) == -1);
        retry = 0;
    }
    strncpy(memory_card_block, "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(D_80123124, 0x40);
    strncpy(D_80123124, "ときめきメモリアル", 0x14);
    if (file_index != 0) {
        strncat(D_80123124, D_800B32CC[file_index - 1], 6);
    } else {
        strncat(D_80123124, "・システム", 0xC);
    }
    bzero(D_80123164, 0x1C);
    hud_event_log_scan(&sp47, &sp46, file_index);
    /* FAKE: T-24214, the no-op mask on the u8 takes one ugen temporary, so
     * the later temporaries (t2..t9) land on the original's registers. */
    idx = sp47 & 0xFF;
    if (idx >= 0xD) {
        idx = 0;
    }
    memcpy(D_80123180, &memory_card_icon_palettes[idx << 5], 0x20);
    memcpy(D_801231A0, (&memory_card_icon_images)[idx], 0x180);
    if (file_index != 0) {
        memory_card_game_bank_build(0);
    } else {
        memory_card_system_bank_build(0);
    }
    D_80123320[1] = D_80123320[0];
    while (write(memory_card_file_descriptor, memory_card_block, 0x2000) == -1) {
        retry++;
        if (retry >= 16) {
            return -1;
        }
    }
    return 1;
}

s32 memory_card_file_exists(s32 file_index) {
    u8 name[0x1C];
    s32 max;
    s32 i;
    s32 fd;

    if (file_index != 0) {
        max = 8;
    } else {
        max = 0x10;
    }
    memory_card_file_name_build(name, file_index);
    i = 0;
    fd = open(name, 2);
    memory_card_file_descriptor = fd;
    while (fd == -1) {
        i++;
        if (max < i) {
            return 0;
        }
        fd = open(name, 2);
        memory_card_file_descriptor = fd;
    }
    close(memory_card_file_descriptor);
    return 1;
}

void memory_card_system_file_probe(void) {
    if (memory_card_file_exists(0) != 0) {
        D_800E8BEE = 1;
        return;
    }
    D_800E8BEE = 0;
}

s32 memory_card_save_files_probe(void) {
    s32 i;
    s32 n;
    u8 *p;
    s32 t;

    n = 0;
    p = D_800E7D11;
    i = 1;
    do {
        t = memory_card_file_exists(i);
        i++;
        p++;
        p[3] = t;
        n++;
    } while (i != 0xF);
    return n;
}

void memory_card_block_clear(void) {
    bzero(memory_card_block, 0x2000);
}

s32 memory_card_file_write_sync(s32 file_index) {
    u8 name[0x1C];
    u32 i;
    u8 *p;

    memory_card_block_clear();
    memory_card_file_name_build(name, file_index);
    close(memory_card_file_descriptor);
    if ((memory_card_file_descriptor = open(name, 2)) == -1) {
        return -1;
    }
    strncpy(memory_card_block, "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(D_80123124, 0x40);
    strcpy(D_80123124, "ときめきメモリアル");
    if (file_index != 0) {
        strcat(D_80123124, D_800B32CC[file_index - 1]);
    } else {
        strcat(D_80123124, "・システム");
    }
    bzero(D_80123164, 0x1C);
    memcpy(D_80123180, memory_card_icon_palettes, 0x20);
    memcpy(D_801231A0, memory_card_icon_images, 0x180);
    if (file_index != 0) {
        memory_card_game_bank_build(0);
    } else {
        memory_card_system_bank_build(0);
    }
    D_80123320[1] = D_80123320[0];
    i = 0;
    p = memory_card_block;
    /* FAKE: a one-pass loop; the u32 counter makes the bound a second 0x2000 register next to the write
       size, as in the original. The real loop shape is unknown. T-17007 */
    for (; i < 0x2000; i += 0x2000) {
        if (write(memory_card_file_descriptor, p, 0x2000) != 0x2000) {
            close(memory_card_file_descriptor);
            return -1;
        }
        p += 0x2000;
    }
    return 1;
}

void memory_card_slot_summary_store(void) {
    SystemData *w = &system_data;
    GameState *g = &game_state;
    s32 pad[14]; /* FAKE: T-25105, 56 unused bytes of the original 0x80 frame; the real locals are unknown */
    s32 i;
    s32 slot;

    slot = w->selected_slot & 0xF;
    w->year[slot] = g->year;
    w->month[slot] = g->month;
    w->day[slot] = g->day;
    memcpy(w->family_name[slot], g->player_family_name, 8);
    memcpy(w->given_name[slot], g->player_given_name, 8);
    memcpy(w->nickname[slot], g->player_nickname, 0x10);
    for (i = 0; i < 9; i++) {
        /* each 4-byte record is copied as one word */
        *(s32 *)&w->metrics[i][slot] = ((s32 *)&g->param_condition)[i];
    }
    for (i = 0; i < 16; i++) {
        w->slot_girl_marks[slot][i] = g->girls[15].scene_counters[i];
    }
    w->club[slot] = g->player_flags.h >> 12;
}

s32 memory_card_system_bank_build(s32 arg0) {
    SystemSaveImage *bank;
    u8 *p;
    s32 sum;
    u32 offset;

    bank = (SystemSaveImage *)D_80123320;
    system_data.slot_status[system_data.selected_slot] = 1;
    memcpy(bank->slot_status, system_data.slot_status, 16);
    memcpy(bank->flags, system_data.flags, 16);
    bank->karaoke_unlock_bits[0] = system_data.karaoke_unlock_bits[0];
    bank->karaoke_unlock_bits[1] = system_data.karaoke_unlock_bits[1];
    if (system_data.selected_slot != 0) {
        bank->selected_slot = system_data.selected_slot % 15U;
    }
    bank->config0 = ((SystemDataSaveView *)&system_data)->config0;
    bank->config1 = ((SystemDataSaveView *)&system_data)->config1;
    bank->album_state[0] = system_data.album_state[0];
    bank->album_state[1] = system_data.album_state[1];
    memcpy(bank->album_entries, system_data.album_entries, 0x800);
    memcpy(bank->year, system_data.year, 16);
    memcpy(bank->month, system_data.month, 16);
    memcpy(bank->day, system_data.day, 16);
    memcpy(bank->family_name, system_data.family_name, 0x80);
    memcpy(bank->given_name, system_data.given_name, 0x80);
    memcpy(bank->nickname, system_data.nickname, 0x100);
    memcpy(bank->club, system_data.club, 16);
    memcpy(bank->metrics, system_data.metrics, 0x240);
    memcpy(bank->slot_girl_marks, system_data.slot_girl_marks, 0x100);
    if (game_state.options_dirty == 0) {
        if (system_data.selected_slot != 0) {
            memcpy(system_data.layout_settings, D_80125C10, 0x40);
            memcpy(&system_data.layout_settings[0xF0], &D_80125C50, 4);
            system_data.layout_settings[0xF4] = hud_mascot_choice;
            system_data.layout_settings[0xF5] = D_80125C54;
            system_data.layout_settings[0xF6] = D_80125C55;
            memcpy(bank->layout_settings, system_data.layout_settings, 0x100);
        }
    }
    /* byte sum of the selected bank's data, from the card block base.
       FAKE: T-25105, the checksum store below names memory_card_block in its address: as1 moves `lw ra`
       above a store only when the address names a global, as the original's epilogue shows (a store
       through a pointer local stays before it). The real source form is unknown. */
    p = &memory_card_block[arg0 * 0xF00 + 0x204];
    sum = 0;
    for (offset = 4; offset < 0xF00; offset++) {
        sum += *p++;
    }
    ((MemoryCardBank *)&memory_card_block[arg0 * 0xF00 + 0x200])->checksum = sum;
}

s32 memory_card_system_bank_load(s32 arg0) {
    SystemSaveImage *bank;

    if (memory_card_bank_validate(0) == -1) {
        return -1;
    }
    /* FAKE: T-24501, integer address arithmetic keeps the bank offset grouped
     * before adding the image base; pointer arithmetic reassociates +0x200 and
     * changes temporary numbering throughout both aggregate copies. */
    bank = (SystemSaveImage *)((game_state.album_slot * 0xF00 + 0x200) + (u32)memory_card_block);
    memcpy(system_data.flags, bank->flags, 16);
    system_data.karaoke_unlock_bits[0] |= bank->karaoke_unlock_bits[0];
    system_data.karaoke_unlock_bits[1] = bank->karaoke_unlock_bits[1];
    system_data.selected_slot = bank->selected_slot;
    if ((((SystemDataSaveView *)&system_data)->config0.flags.b[3] & 1) == 0) {
        ((SystemDataSaveView *)&system_data)->config0 = bank->config0;
    }
    if (system_data.bits.flag == 0) {
        ((SystemDataSaveView *)&system_data)->config1 = bank->config1;
    }
    system_data.album_state[0] = bank->album_state[0];
    system_data.album_state[1] = bank->album_state[1];
    memcpy(system_data.album_entries, bank->album_entries, 0x800);
    memcpy(system_data.year, bank->year, 16);
    memcpy(system_data.month, bank->month, 16);
    memcpy(system_data.day, bank->day, 16);
    memcpy(system_data.family_name, bank->family_name, 0x80);
    memcpy(system_data.given_name, bank->given_name, 0x80);
    memcpy(system_data.nickname, bank->nickname, 0x100);
    memcpy(system_data.club, bank->club, 16);
    memcpy(system_data.metrics, bank->metrics, 0x240);
    memcpy(system_data.slot_girl_marks, bank->slot_girl_marks, 0x100);
    memcpy(system_data.layout_settings, bank->layout_settings, 0x100);
    memcpy(D_80125C10, system_data.layout_settings, 0x40);
    memcpy(&D_80125C50, &system_data.layout_settings[0xF0], 4);
    hud_mascot_choice = system_data.layout_settings[0xF4];
    D_80125C54 = system_data.layout_settings[0xF5];
    D_80125C55 = system_data.layout_settings[0xF6];
}

/* Writes a game save into bank arg0 of the card image (GameSaveImage), then stores the bank's byte sum.
   FAKE: T-25105, the straight-line stores reach the bank through the byte offset
   `&memory_card_block[arg0 * 0xF00 + 0x200]` and the loops as an element of the bank array
   `((GameSaveImage *)&memory_card_block[0x200])[arg0]`. uopt keeps the two spellings as two values: the
   original's two base registers (a2 for the stores, a3 recomputed before the first loop, both
   spilled around get_k_speed). One spelling for both gives one base register. The real source
   form is unknown. */
s32 memory_card_game_bank_build(s32 arg0) {
    GameState *g;
    u8 *p;
    s32 sum;
    u32 i; /* one counter for every loop: only the last loop (the byte sum) is unrolled */

    g = &game_state;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->vacation = g->vacation;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->year = g->year;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->month = g->month;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->day = g->day;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->weekday = g->weekday;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->day_count = g->day_count;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->month_calendar[0] = g->month_calendar[0];
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->month_calendar[1] = g->month_calendar[1];
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->profile = *(ProfileBlock *)g->player_full_name;
    for (i = 0; i < 16; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].girls[i] = g->girls[i];
    }
    for (i = 0; i < 16; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_cleared[i] = g->girl_cleared[i];
    }
    for (i = 0; i < 8; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].appointments[i] = g->unk_54C[i];
    }
    for (i = 0; i < 12; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].clubs[i] = ((WordView *)g->clubs)[i];
    }
    g->saved_settings[3] = get_k_speed();
    g->saved_settings[2] = D_800B6708;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->scene_flags = *(SceneFlagBlock *)g->scene_flags;
    for (i = 0; i < 4; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].date_place_history[i] = g->date_place_history[i];
    }
    for (i = 0; i < 16; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_last_date_spot[i] = g->girl_last_date_spot[i];
    }
    for (i = 0; i < 16; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_last_date_spot_timer[i] = g->girl_last_date_spot_timer[i];
    }
    for (i = 0; i < 16; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_scene_bits[i] = g->girl_scene_bits[i];
    }
    for (i = 0; i < 256; i++) {
        ((GameSaveImage *)&memory_card_block[0x200])[arg0].event_log[i] = g->event_log[i];
    }
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->icon_mode = g->icon_mode;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->icon_count = g->icon_count;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->menu_mode = g->menu_mode;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->sd_anim_fast = g->sd_anim_fast;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->voice_mode = g->voice_mode;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->last_command = g->last_command;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->cursor_speed = g->cursor_speed;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->click_mode = g->click_mode;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->double_click_interval = g->double_click_interval;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->confirm_button = g->confirm_button;
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->quarter_flags = *(WordView *)g->quarter_flags;
    /* byte sum of the bank's data; the checksum store is a FAKE address form as in memory_card_system_bank_build (T-25105) */
    p = &memory_card_block[arg0 * 0xF00 + 0x204];
    sum = 0;
    for (i = 4; i < 0xF00; i++) {
        sum += *p++;
    }
    ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->checksum = sum;
}

/* FAKE: T-10850, T-17007: the dead initial assignments set the induction-register numbering, and
   writing all five on one line makes as1 emit the hoisted starts in the original order (q, r, b).
   The bank checksum is read as *(s32 *)(r + 0x200), not D_80123320[b].checksum: the field form
   changes the register allocation (r is the header-side induction pointer). */
s32 memory_card_bank_validate(s32 arg0) {
    s32 b;
    s32 n;
    s32 sum;
    s32 j;
    u8 *p;
    u8 *q;
    u8 *r;

    n = 0; b = 0; p = D_80123320[0].data; r = memory_card_block; q = D_80123320[0].data;
    for (; b < 2; b++) {
        n = 0;
        p = q;
        sum = 0;
        for (j = 4; j < 0xF00; j++) {
            if (*p != 0) {
                n++;
            }
            sum += *p++;
        }
        if (sum == *(s32 *)(r + 0x200) && n != 0) {
            game_state.album_slot = b;
            return 1;
        }
        r += 0xF00;
        q += 0xF00;
    }
    return -1;
}

/* The load half of memory_card_game_bank_build: copies bank game_state.album_slot & 1 back into GameState, field by field
   in the same order (T-25310). The bank address has the writer's two spellings (FAKE: T-25310, the same
   address forms as memory_card_game_bank_build, see its comment).
   The slot byte is read through g, set before the memory_card_bank_validate call: a plain game_state.album_slot read
   lets IDO fold arg0 into the bank base, which then splits into two live ranges. */
s32 memory_card_game_bank_load(s32 arg0) {
    GameState *g;
    u8 *p;
    u32 i;
    s32 pad[2]; /* FAKE: T-25310, two unknown frame words (frame 0x48, spill homes 0x2C/0x30) */

    g = &game_state;
    if (memory_card_bank_validate(0) == -1) {
        return -1;
    }
    arg0 = g->album_slot & 1;
    if (((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->year == 0) {
        return -1;
    }
    if (((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->month == 0) {
        return -1;
    }
    g->vacation = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->vacation;
    g->year = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->year;
    g->month = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->month;
    g->day = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->day;
    g->weekday = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->weekday;
    g->day_count = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->day_count;
    for (i = 0; i < 2; i++) {
        g->month_calendar[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].month_calendar[i];
    }
    *(ProfileBlock *)g->player_full_name = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->profile;
    for (i = 0; i < 16; i++) {
        g->girls[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].girls[i];
    }
    for (i = 0; i < 16; i++) {
        g->girl_cleared[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_cleared[i];
    }
    for (i = 0; i < 8; i++) {
        g->unk_54C[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].appointments[i];
    }
    for (i = 0; i < 12; i++) {
        ((WordView *)g->clubs)[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].clubs[i];
    }
    *(SceneFlagBlock *)g->scene_flags = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->scene_flags;
    k_speed_set(g->saved_settings[3]);
    D_800B6708 = g->saved_settings[2];
    for (i = 0; i < 4; i++) {
        g->date_place_history[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].date_place_history[i];
    }
    for (i = 0; i < 16; i++) {
        g->girl_last_date_spot[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_last_date_spot[i];
    }
    for (i = 0; i < 16; i++) {
        g->girl_last_date_spot_timer[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_last_date_spot_timer[i];
    }
    for (i = 0; i < 16; i++) {
        g->girl_scene_bits[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].girl_scene_bits[i];
    }
    /* FAKE: T-25310, the result is stored and never read; the original reads bzero's v0 after the call,
       which keeps v0 out of the unrolled loop's start values (end pointer in v0, the counter range in t4,
       the bank bases in t5 and ra). Its real use is unknown. */
    p = bzero(g->event_log, 0x800);
    for (i = 0; i < 256; i++) {
        g->event_log[i] = ((GameSaveImage *)&memory_card_block[0x200])[arg0].event_log[i];
    }
    g->event_log_count = hud_event_log_scan(&g->current_girl, &g->date_girl, 1);
    g->icon_mode = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->icon_mode;
    g->icon_count = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->icon_count;
    g->menu_mode = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->menu_mode;
    g->sd_anim_fast = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->sd_anim_fast;
    g->voice_mode = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->voice_mode;
    g->last_command = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->last_command;
    g->cursor_speed = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->cursor_speed;
    g->click_mode = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->click_mode;
    g->double_click_interval = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->double_click_interval;
    g->confirm_button = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->confirm_button;
    *(WordView *)g->quarter_flags = ((GameSaveImage *)&memory_card_block[arg0 * 0xF00 + 0x200])->quarter_flags;
    return 1;
}

s32 memory_card_file_name_build(u8 *buf, s32 arg1) {
    s32 i;

    /* FAKE: same-line zero init and while condition places move v1, zero after li a0, 20. T-24123 */
    i = 0; while (i < 0x14) {
        buf[i] = 0;
        i++;
    }
    strcpy(buf, memory_card_file_prefix);
    switch (arg1) {
    case 0:
        strcat(buf, "0");
        break;
    case 1:
        strcat(buf, "1");
        break;
    case 2:
        strcat(buf, "2");
        break;
    case 3:
        strcat(buf, "3");
        break;
    case 4:
        strcat(buf, "4");
        break;
    case 5:
        strcat(buf, "5");
        break;
    case 6:
        strcat(buf, "6");
        break;
    case 7:
        strcat(buf, "7");
        break;
    case 8:
        strcat(buf, "8");
        break;
    case 9:
        strcat(buf, "9");
        break;
    case 10:
        strcat(buf, "A");
        break;
    case 11:
        strcat(buf, "B");
        break;
    case 12:
        strcat(buf, "C");
        break;
    case 13:
        strcat(buf, "D");
        break;
    case 14:
        strcat(buf, "E");
        break;
    case 15:
        strcat(buf, "F");
        break;
    default:
        return 0;
    }
    return 1;
}

void memory_card_state_reset(void) {
    s32 i;

    memory_card_stop();
    memory_card_block_clear();
    D_800E7D14[0] = 0;
    D_800E7D15[0] = 0;
    /* FAKE: the store inside the for-init places it after the loop prologue as in the original; real source unknown. T-9140 */
    for (i = 0, D_800E7D16 = 0; i < 3; i++) {
        D_800E7D13[i * 4 + 5] = 0;
        D_800E7D13[i * 4 + 6] = 0;
        D_800E7D13[i * 4 + 7] = 0;
        D_800E7D13[i * 4 + 4] = 0;
    }
    memory_card_start();
    D_800E8BEE = 0;
}
