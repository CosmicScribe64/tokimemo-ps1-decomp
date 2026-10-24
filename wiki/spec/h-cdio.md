---
type: spec
unit: h-cdio
updated: 2026-10-24
sources: ["src/main/800451D0.c", "src/main/80046500.c", "src/main/80056300.c", "src/main/800563F0.c", "src/main/80057390.c", "src/main/80058D20.c", "src/main/80059710.c", "src/main/80059A20.c", "src/main/80059B40.c", "config/renames/proposals/h-cdio.tsv"]
---

# Spec h-cdio: CD, movies, MDEC backgrounds, 3D, mouse (main exe hardware layer)

Phase 2 spec for the Godot port (parent ticket T-27000, ticket [[tickets/T-27021-p2-h-cdio]]). This unit is mostly **hardware layer**: the port replaces it. Names, entry points and the data other code uses are listed here; internals are summarised only where game logic depends on them. All names below are proposals in `config/renames/proposals/h-cdio.tsv` (old name -> new name, with evidence). A name marked HYPOTHESIS there is a guess about game meaning; everything else is read from the code or the emulator. O.BIN developer names are kept (`strInit`, `dec_bg_cd_read`, `initView`, `InitMouse`, ...) and get an English comment row.

## 1. Module map

| old file (src/main/) | proposed name | what it is | entry points other code calls |
|---|---|---|---|
| `800451D0.c` | `cd_queue.c` | CD command ring over libcd, sector reads with an end-of-file marker test, file checksum, XA clip request, load base address | `cd_command`, `cd_queue_update` (main loop), `cd_queue_reset`, `cd_queue_idle`, `cd_read_sectors`, `cd_read_sectors_unchecked`, `cd_read_sync_status`, `cd_state_get`, `cd_read_count_get`, `file_checksum_ok`, `xa_clip_request_set/get`, `xa_position_get`, `load_dest_base_set/get` |
| `80046500.c` | `reset_iris_wipe.c` | full game reset to the title sequence, two number helpers, circle ("iris") screen wipe | `game_reset_to_title`, `decimal_digit`, `bcd_to_int`, `iris_texture_build`, `iris_wipe_draw` |
| `80056300.c` | `exit_to_shell.c` | tear down the hardware and reload the system launcher from disc | `exit_function` (mode 0xFF) |
| `800563F0.c` | `movie_player.c` | STR movie playback (PsyQ movie sample structure) | `movie_play`, `set_movie_offset` |
| `80057390.c` | `dec_bg.c` | MDEC-decoded background pictures: load, cache, draw, fade, crossfade, wipe | `dec_bg_cd_read`, `dec_bg_decode`, `dec_bg_show_switch/set`, `dec_bg_draw`, `dec_bg_fade_set`, `dec_bg_fade_draw`, `dec_bg_crossfade_draw`, `dec_bg_capture_screen`, `dec_bg_wipe_step` (counter `dec_bg_wipe_count`), `dec_bg_reset`, `set_dec_bri`, `get_last_gamen_mode` |
| `80058D20.c` | `model_3d.c` | libgs 3D setup (view, lights, 16 model slots), display width modes, per-frame model update | `initView`, `initModelingData_init`, `model_link_tmd_object`, `model_slots_clear`, `model_record_init`, `models_update_draw`, `display_set_256_or_320`, `display_set_512_or_320` |
| `80059710.c` | `mouse.c` | PsyQ mouse sample (position/range/sensitivity per port) | `InitMouse`, `SenseMouse`, `SetMouse`, `RangeMouse` (all four only from the input init) |
| `80059A20.c` | `debug_info.c` | state-number debug print, sound load error report | `debug_state_numbers_draw` (main loop, debug flag), `snd_load_error_report` (sound driver) |
| `80059B40.c` | `boot_mode.c` | mode 0: boot data loads and HUD initialisation | `boot_mode_run` (mode dispatcher, mode 0) |

Prefixes: `cd_` and `snd_` are the main-exe subsystem prefixes the naming audit proposes; `xa_`, `movie_`, `dec_bg_` (O.BIN), `model_`, `display_`, `boot_`, `iris_`, `mouse_`, `debug_` are new, one per module. `movie_` is used for the game's own movie wrappers (`movie_play`, `movie_caption_bar_draw`, `movie_captions_update`, the `movie_*` data) while the PsyQ sample functions keep their O.BIN `str*` names with comment rows; a second `str_` family would sit next to `strInit` and `strNext` and read like text-string code (`set_kanji_string`). Header split proposal (no header exists per module today, the declarations sit in `main_api.h`): `cd_queue.h`, `movie.h`, `dec_bg.h`, `model_3d.h`, `mouse.h`.

## 2. CD layer (`cd_queue.c`)

### 2.1 What it does

Every disc read of the game goes through a ring of 48 command entries that a per-frame poll feeds to libcd. Callers never call libcd directly: they call `cd_command` (public ids) or the read helpers, then poll a status byte. The port can replace the whole layer by direct file reads; the data other code relies on is the **status protocol** in 2.4.

### 2.2 Data

- Ring `cd_queue[48]`, type `CdCommand` (0x18 bytes): `command` (u8), `lba` (s32, a sector number), `param` (16 bytes). `param` byte 15 is cleared on every push, overwritten by the copied parameter bytes, or set to 0xFF when the caller passes the value 1 instead of a pointer (that marker keeps an XA read from moving `cd_state` to "XA playing"; no caller in `src/` passes 1). Only GEKO calls `cd_queue_push` directly (to queue a pause).
- Ring state `CdQueueState`: `cd_queue_head` (entry being served), `cd_queue_tail` (next free; both wrap at 48), `cd_read_status` (existing name), `cd_queue_phase` (s8: -1 = start the command at head, 1 = command issued, waiting).
- Status bytes read by game code: `cd_state` (0 busy, 2 done, 4 XA streaming), `cd_read_count` (counts finished reads), `cd_read_status` bit 0 (`CD_READ_DONE`: the last read finished), `xa_position` (current sector while XA plays).
- Read bookkeeping: `cd_read_checked` (1 after `cd_read_sectors`, 0 after the unchecked read), `cd_read_end_word_ptr` (last word of the destination), `cd_read_retry_count`, `cd_sectors_left` (last CdReadSync result: sectors left, 0 = done, -1 = error), `cd_last_read_lba`.
- `load_dest_base`: a RAM address. Loaders store it (0x8019A000, 0x801B8000, 0x801C8000, 0x801E0000, 0x80162000) before starting a load group through `func_80044750(group)`; the load-group switch in `80079B10.c` reads it back to place its buffers. It is not a CD value; it sits here only because of the object it lives in.

### 2.3 Public commands (`cd_command(id, lba, param)`)

`lba` is clamped to 0x4AB50 (a disc end sector). Ids and what they enqueue (enum `CdCommandId`; the ring struct is `CdCommand`). Ring entries use the libcd command codes themselves: 2 CdlSetloc, 3 CdlPlay, 6 read, 9 CdlPause, 10 CdlInit, 13 CdlSetfilter, 14 CdlSetmode, 21 CdlSeekL, 27 CdlReadS; the public ids below are the same numbers where they exist:

| id | constant | enqueues |
|---|---|---|
| 0 | `CD_CMD_PAUSE_ALT` | entry 0, served like pause (no src caller passes 0) |
| 2 | `CD_CMD_SETLOC` | entry 2 (set position) |
| 3 | `CD_CMD_PLAY_DA` | set mode 5, set position, play (CD-DA audio) |
| 6 | `CD_CMD_READ` | entry 6 with `param` = address word; also stores `cd_last_read_lba` |
| 9 | `CD_CMD_PAUSE` | entry 9 (10 callers: stop the drive after a stream or read) |
| 10 | `CD_CMD_INIT` | entry 10 (drive init) |
| 13 | `CD_CMD_XA_START` | set filter (file, channel in `param`); if `lba != 0` also set position + XA read (entry 0x1B) |
| 14 | `CD_CMD_SET_MODE` | set mode with the byte in `param` |
| 21 | `CD_CMD_SEEK` | set position, then seek |
| 27 | - | nothing |

Read parameter word (`cd_read_param_pack(sectors, dest, out)`): low 24 bits = destination address minus 0x80000000, top byte = sector count. Two read entry points: `cd_read_sectors(count, dest, lba)` (checked) and `cd_read_sectors_unchecked` (same, without the end-marker test, used for raw data such as MDEC pictures and the boot DUM.TMD read).

### 2.4 Per-frame poll (`cd_queue_update`), rules

The main loop calls it once per frame after the mode handler and the draw (2341 calls in the first 3600 emulator frames; also called by the TT overlay's own loop).

1. If head == tail (nothing queued): when libcd reports a disk error (sync result 5) and the last started command was the XA read, the entry is re-queued (head moves back one, wrapping 0 -> 47) and `cd_error_recover` runs; any other disk error just issues a status request. Otherwise, while `cd_state == 4`, `xa_position` is refreshed from `cd_position_lba` every frame.
2. If an entry is queued, a yellow marker is drawn while a debug flag is on (`cd_busy_marker_draw`), then by `cd_queue_phase`:
   - **start (-1)**: the command at head is issued once the previous libcd command has completed. Pause, seek, set-position, set-filter, set-mode, read and XA read wait until the previous libcd command has completed, clear `cd_state` and the read status bits, and issue the command; play and init are issued without that wait (init retries until the drive accepts it). Fire-and-forget entries (play, set filter, set mode, set position, init) advance head immediately; pause, seek, read and XA read move to phase 1.
   - **wait (1)**: pause and seek finish when a status request reports the drive is not seeking (status bit 0x40 clear): `cd_state = 2`, head advances. Read finishes when CdReadSync reports 0 sectors left (a negative result restarts the entry): `cd_state = 2`, `cd_read_status |= 1`, `cd_read_count += 1`, head advances. XA read finishes when the status shows the drive reading (bit 0x20): `cd_state = 4` (not for entries with param byte 15 = 0xFF), head advances; a disk error (sync result 5) instead restarts the entry after `cd_error_recover`.
3. Head wraps to 0 at 48.

Read details: a read sets the drive to double speed (mode byte 0x84), positions, flushes, and starts the sector read at mode 0x80. The checked read (`cd_read_checked == 1`) additionally requires the final 32-bit word of the destination to have 0x1012 in its top half; when it does not, nothing advances, a retry counter increments, the drive is reinitialised and 512 vertical blanks are waited on the 3rd, 7th, 11th, ... failure (counter & 3 == 3), and the read command is issued again. So every file that goes through `cd_read_sectors` ends in a 4-byte trailer: 16-bit big-endian byte sum, then the two bytes 0x12 0x10 (little-endian half 0x1012). `file_checksum_ok(blocks, buf)` is the matching verify: sum of all bytes of the first `blocks * 2048 - 4`, as a u16, equal to the big-endian u16 at the trailer. Loaders that use both: overlay loader (`ovl_load_verify`, `800789E0.c`), the 45-sector common block at 0x80180000 (`80085E30.c`, `8007C030.c`), 9- and 41-sector blocks at 0x801E0000 (`800737A0.c`).

**Port note:** a port that reads files directly needs none of this retry logic. What must be kept is the *order* of loads and the `cd_state`/`cd_read_status` handshake only if game logic is ported unchanged: overlays poll `cd_read_sync_status() & 1` for "load finished" and `cd_state_get() & 2` / `& 4` for "XA command done" / "XA playing".

### 2.5 XA clip request

`xa_clip_request_set(start, end, speaker)` fills a four-word request: `speaker` (low 4 bits of the third argument), `xa_speaker_offset_table[speaker]` (16 words), `start`, `end`. The sound driver in `80079B10.c` (`func_8007A6AC`, `func_8007A868`, `func_8007A924`) consumes it: the top byte of `end` becomes filter byte 0 (XA file number) and the top byte of `start` filter byte 1 (channel); the low 24 bits are a clip index that it multiplies by 0x20 sectors (8 for speaker 0xF), adds a base and the table offset to, and widens by 0x10 sectors on each side. It then starts `CD_CMD_XA_START`. Emulator: the title-attract prologue in NAME_ENT requests three clips with speaker 0xF at frames 3836, 4653 and 6754. The patch notes describe voiced dialogue as XA clips ([[ref-english-patch]]); callers are NAME_ENT (prologue), EN_NICHI, DATE, ETC, and `menu_check`-adjacent code in `8004F870.c`. `xa_position_get` returns the sector reached, for lip-sync/end detection.

### 2.6 Number helpers (used here and elsewhere)

- `bcd_to_int(word)`: eight nibbles, nibble i times 10^i. Used on the CD position bytes.
- `cd_position_lba()`: reads the drive position (libcd get-location-L, up to 16 tries), checks minute < 75, second < 60, frame < 75, converts with CdPosToInt; returns 0 when invalid. Only the poll calls it.

## 3. Reset, digits, iris wipe (`reset_iris_wipe.c`)

- `game_reset_to_title()`: the default arm of the mode/stage switches in main and every overlay (471 of the 517 references to it in `src/` are a `default:` arm). It runs, in this order: the overlay loader init (`func_800789E0`), the GameState initializer with argument 0 (`func_800412E0`), `main_state_set(0x12)`, `func_80041584`, `func_80048DD0(0)`, `back_clear_switch(1)`, the background reset (`dec_bg_reset`), the load-group state reset (`func_8004482C`), `tpage_buf_clear`, load group 0x7F (`func_80044750(0x7F)`), the sound state resets (`func_8007BDE8`, `func_8007B844`), the CD ring reset (`cd_queue_reset`) and two more init calls (`func_80041878`, `func_80042058`), and returns -1. It is the game's "unknown state, restart" path. Mode 0x12 is the title sequence (handler `func_80062840` in `80061710.c`; stages 0..3 with rotating models; the menu leads to new game, mode 0x13, and options, mode 0x20).
- `decimal_digit(value, position)`: 1-based digit of a decimal number (1 = ones); positions of 6 and up return 0. Used by the calendar and parameter panels to draw numbers digit by digit (emulator: `(95, 2)` -> 9 at frame 1300).
- Iris wipe: `iris_texture_build(buf, slot, mode)` builds one 128x128 quadrant of a 4-bit circle mask (mode high nibble non-zero: 15-step radial shading; low nibble non-zero: flat circle of radius 127 in that palette index; zero: the area outside the circle in index 1) and uploads it (32x128 words) to the texture page cell `slot` (x = (slot & 15) * 64, y = 256 * bit 4 + 128 * bit 5). `iris_wipe_draw(page, clut_x, clut_y, radius, cx, cy, color, order)` draws four mirrored quads with it, radius in pixels = `radius * 128 / 4096`, centre = screen centre + (cx, cy); colour word: bits 28-31 non-zero = semi-transparent with RGB from bits 0..23, else grey 0x80; bits 24-27 non-zero = also fill the four rectangles outside the circle with that RGB; `order` low nibble = ordering-table slot. SHUGAKU calls it with radius `cnt*cnt/2` from the top-left corner (-160, -120) to grow the circle; NAME_ENT uses it for a prologue transition. Port: a shader or a masked sprite replaces both.

## 4. Movie player (`movie_player.c`)

### 4.1 Flow

`movie_play(name, end_frame, start_sector)`:

1. State lives at fixed address 0x80180000 (`movie_ctx`, type `StrContext`): start position, `StrDecEnv` (two VLC buffers at +0x12D44 and +0x48D44, an image slice buffer at +0x10044, two target rectangles, slice rectangle), end flag, width/height (initially 320x240).
2. Position: `start_sector != 0` -> CdIntToPos. With 0 it would search the file by name (retry up to 0x10000 times, return 0 when not found); both callers pass sector and name 0.
3. `strSetDefDecEnv(env, 0, 0, 0, 240)` places rectangle 0 at VRAM (0,0) and rectangle 1 at (0,240); slice 16 px wide (15-bit) or 24 px wide (24-bit). In 24-bit mode the rectangle is 480 words wide (320 pixels) and the display uses RGB24 at 320.
4. `strInit` resets the MDEC, installs `strCallback` as the output callback, sets the CD stream ring (32 sectors) and starts the stream (`strKickCD`: seek, then streaming read at mode 0x1C0). `strNextVlc` takes the first frame.
5. Loop per frame: start MDEC input of the current VLC buffer and output of one slice, VLC-decode the next frame, `strSync` (wait with timeout), vertical blank, swap display and draw environment to the rectangle just decoded, display on.
6. Exit when the stream header frame number reaches `end_frame - 3`, or the loop count reaches `end_frame - 15`, or the stream's end flag is set, or the pad word (`GameState.unk_F88`) has any of bits 0x860 (skip). Then the display returns to 15-bit and the sound volume pair is set for vol = 0x7F, 0x75, ... down to 7 (step 10, loop while vol > 0) without waiting between steps, so it ends at 7, not zero (effectively one drop to a low volume), and the display is switched on again.
7. `strCallback` copies each decoded slice to VRAM at the slice rectangle plus the movie offset (`set_movie_offset(x, y)`; in 24-bit mode the offset is scaled by 3/2, i.e. `(v*3+2)/3`), advances along the row, and flips the rectangle when a row is complete. `strNext` validates the frame header against the sector header and changes the picture size when the stream's size differs (clearing a 640x480 VRAM rectangle).

`movie_flags` (`D_800B5900`) has bit 0 = 24-bit mode and starts at 1 in the data; no code in `src/` writes it, so both movies play in 24-bit mode. The 15-bit branch, which draws the game UI over the movie each frame and calls `movie_captions_update`, is therefore never taken in the shipped data (emulator: no caption call in the first 292 frames of the opening movie). HYPOTHESIS: it is left over from an earlier build. `movie_captions_update(ticks)` walks 22 threshold bytes (`movie_caption_marks`, scaled by 0x50/0xF) against `(frame counter - start) / 6` and drives the typewriter text system (`k_disp_switch`, `k_disp_start`) one line at a time; after the last mark it clears the text band. The two callers that prepare 22 lines before the opening movie (NAME_ENT) suggest captions or lyrics; this is a HYPOTHESIS.

### 4.2 The two movies in the game

| where | arguments (`end`, `start sector`) | evidence |
|---|---|---|
| mode 0x10 handler `func_80062C28` (`80061710.c`) | 0xA7 (167), sector 0x190 (400), offset (0, 0) | emulator: called at frame 622; disc layout: the file at sector 400 is named KONAMIC3.STR, HYPOTHESIS: the publisher logo; afterwards the handler pauses and inits the drive, resets the texture buffer and calls `main_state_set(0)` |
| NAME_ENT `func_80142A80` (prologue) | 0x33B (827), sector 0x820 (2080), offset (0x18, 0x10) | emulator: frame 7208, called from the NAME_ENT entry `func_80144AD0`; disc layout: the opening movie file starts at sector 2080 |

After each movie: `func_80088070` (flush), for NAME_ENT also set mode 0x80 (`cd_command(14)`), pause (`cd_command(9)`), init (`cd_command(10)`), a decode-buffer setup call (`func_80053650(0x80167000, 0x80162000, 0x80162000)`), display on.

## 5. MDEC backgrounds (`dec_bg.c`)

The game shows still pictures (backgrounds, event and date scenes) as MDEC-compressed data read from disc into 0x80162000, decoded into VRAM, and drawn as sprites. 246 call sites of `dec_bg_cd_read` and 315 of `dec_bg_decode` in overlays and main use it.

### 5.1 Layers and VRAM

- Layer 0 (`DEC_BG_STRIP_88`): 320x88; layer 1 (`DEC_BG_PANEL_160`): 320x160; draw-only value 2 (`DEC_BG_FULL_240`): 240 lines read as layer 1's 160 lines over layer 0's 88 lines. Each layer has two VRAM buffers (double buffer). Buffer 0: layer 1 at (0x2C0, 0), layer 0 at (0x2C0, 0xA0); buffer 1: layer 1 at (0x140, 0x100), layer 0 at (0x140, 0x1A0).
- State: `dec_bg_shown[layer]` (which buffer is shown), `dec_bg_lba_cache[4]` (LBA of the picture in each buffer: indices 0..1 layer 0, 2..3 layer 1), `dec_bg_brightness` (0x80 normal, set by `set_dec_bri`), `dec_bg_fade_amount` (set by `dec_bg_fade_set`; 0x80 = full brightness), `dec_bg_wipe_count` (steps of `dec_bg_wipe_step`), `dec_bg_last_layer` (`get_last_gamen_mode`; -1 after `dec_bg_reset`), `dec_bg_decode_result`.

### 5.2 Entry points and rules

- `dec_bg_cd_read(lba, layer)`: returns 0 if the picture is already in buffer 0, 1 if in buffer 1 (no read), else starts an unchecked read of the compressed data to 0x80162000, records the LBA for the hidden buffer and returns -1. (Only layers 0 and 1 are meaningful; any argument other than 1 selects layer 0.) Callers wait until the read finishes (`cd_read_sync_status() & 1`).
- `dec_bg_decode(layer)`: sets the decode buffers (`func_80053650`), decodes into the **hidden** buffer (`1 - shown`) with `func_800536AC(x, y, 320, h)` and returns that buffer's index. Callers then call `dec_bg_show_switch(layer)` (toggle) or `dec_bg_show_set(layer, buf)` to display it (the cached-picture path uses the returned index of `dec_bg_cd_read` directly).
- `dec_bg_draw(layer)` (nothing for -1): two sprites per frame: a 192-wide piece at the left (screen x -160) and a 128-wide piece at x +32, from the shown buffer's texture page (second piece at +0xC0 columns), colour = brightness. Layer 0 is drawn at y = centre - 0x30 using texture v 0xA0; layers 1 and 2 at y = centre - 0x78 using v 0. Records the layer in `dec_bg_last_layer`.
- `dec_bg_fade_set(amount)`: stores `dec_bg_fade_amount` (0..0x80), read by both fade draws. NAME_ENT sets 0x80, lowers it by 3 per frame while it is at least 0x11, later raises it by 4 per frame back to 0x80.
- Geometry shared by `dec_bg_fade_draw` and `dec_bg_crossfade_draw`: `layer` 1 = 320x160 at y = `draw_origin_y` - 0x78, texture v 0; any other value = the 320x88 layer at y = `draw_origin_y` - 0x30, v 0xA0 (layer 2 is not special-cased). Each picture is a 192-wide left piece (x = `draw_origin_x` - 0xA0) and a 128-wide right piece (x = `draw_origin_x` + 0x20); the right piece's texture page is the left one +0xC0 columns. Buffer 0 is at tpage (0x2C0, 0), buffer 1 at (0x140, 0x100). The tpage word comes from the libgpu builder (`func_8009ECB0`, inferred = GetTPage) with colour mode 2 (15-bit) and a rate argument that is 1 for the semi-transparent sprites and 0 for the opaque ones. The slot numbers below are the ordering-table slots passed to the draw calls (offsets 0x28/4 = 10, 0x30/4 = 12, 0x34/4 = 13); which slot composites first (table direction) and what the semi-transparent pass blends with were not checked in the emulator.
- `dec_bg_fade_draw(layer)` (4 sprites, all from the **shown** buffer): left and right piece semi-transparent with colour `0x80 - amount` at slot 10; left and right piece opaque with colour `amount` at slot 13. At amount 0x80 the semi-transparent pair is black and the opaque pair is at full brightness. HYPOTHESIS: it fades one picture in or out; the callers are `normal_date_girl_in` and `normal_date_girl_out` with layer 1.
- `dec_bg_crossfade_draw(layer)` (6 sprites): left and right piece semi-transparent with colour `0x80 - amount` from the **hidden** buffer at slot 10; left and right piece opaque with colour `amount` from the **shown** buffer at slot 12; left and right piece opaque with colour `amount` from the **hidden** buffer at slot 13. It is not `dec_bg_fade_draw` with the buffers exchanged. HYPOTHESIS: a cross-fade between the picture on screen and one just decoded into the hidden buffer (NAME_ENT decodes into the hidden buffer, then calls this each frame while changing the amount; `normal_date_move_place_*` use it for place changes). Both opaque pairs use the same colour, so any blend comes from the semi-transparent pair; confirm in the emulator (date overlay, place change) before a port builds a fade from this description.
- `dec_bg_capture_screen(lines)`: copies `lines` rows of the currently displayed frame buffer (x 0, y 240 * buffer) into the hidden layer-1 buffer, clears that buffer's cache entry, shows it and resets `dec_bg_wipe_count`; callers: title/loader code in `80061710.c`, `800789E0.c`, NAME_ENT.
- `dec_bg_wipe_step()`: one step of a diagonal wipe: brightness -8 (clamped to 0 when it underflows), `dec_bg_wipe_count` +1, then 15 bands of up to 32 black diagonal lines (width grows by 2 per step); callers: `func_80078A94` and `pre_syogatu_init1`.

## 6. 3D and display (`model_3d.c`)

Used by three places: the title sequence (rotating models), the TACO shooter, and a few ETC/DATE/TAIIKU scenes; the main frame draw (`func_80043510`) calls `models_update_draw` every frame (2026 calls in the first 3600 emulator frames).

- `initView`: projection distance 1000, two clip-style setters called with 100 and 0x10000 (HYPOTHESIS: near and far clip), reference view `view_ref` (eye at z = 1000, looking at the origin). Called from TACO `func_80134930`.
- `initLight`: three directional lights (`light_0` white, `light_1` green, `light_2` red) and zero ambient; `initCoordinate`: 16 `GsCoordinate2` records `model_coords`, each with z translation -4000; both have no caller in `src/` (only reachable by the original build, or dead).
- Model slots: 16 object records `model_objects` (16 bytes each, word 0 = 0x80000000 hides the slot) linked to the 16 coordinate records. `initModelingData_init(tmd)` maps a TMD and links its objects to all 16 slots; `model_link_tmd_object(tmd, slot, n)` links object `n` into one slot (the object count word is replaced by 15 when it is above 16, so 16 passes unchanged; returns -1 when `n` is not below the count); `model_slots_clear` hides all 16.
- `model_record_init(i)`: resets display-table record `i` as a model (active 1, flags 0, rotation 0, scale 0x1000 each axis, position and velocity 0).
- `models_update_draw()` (every frame): the records are pairs of `DisplayActor44` slots (stride 0x88, loop `i += 2` from a start index stored at GameState + 0x109C up to 160). For each record with `active` and `flags & MODEL_FLAG_VISIBLE (0x80)`: rotation matrix from the 16-bit angles (`rot`), scale matrix from `scale` (a zero axis counts as 0x1000), multiplied, translation from `pos` (and `pos += vel` first if `flags & MODEL_FLAG_MOVING (0x10)`), stored in the record, then the object is sorted into the frame's ordering table (`model_base_matrix` is the start matrix; depth shift 6). Record layout: type `ModelRecord` (0x88 bytes).
- Display modes: `display_set_256_or_320(wide)` (wide != 0: 256x240, centre x 0x80; else 320x240, centre x 0xA0) and `display_set_512_or_320(wide)` (512x240 centre x 0x100 / 320x240; only when the width differs; clears VRAM first). Both write GameState width/height/interlace, call the libgs init and `back_clear_switch(1)`. Callers: ETC (256/320), TACO (512 for the shooter). GameState fields they write: `screen_width` (0x000), `screen_height` (0x002), `screen_interlace` (0x004) and `draw_origin_x[2]` (0x014, half the width: 0x80, 0xA0 or 0x100). `draw_origin_y[2]` (0x018) is not touched here. Both origin fields are named by the graphics unit (added to every hand-built primitive; 0xA0 and 0x168/0x78 at reset, plus the screen shake) and used by this unit's sprite draws.

## 7. Mouse (`mouse.c`)

A PsyQ sample module, mostly unused. `InitMouse(buf1, buf2)`, `SenseMouse(3, 4)` (x multiplier 3, y multiplier 4), `SetMouse(0, 0xA0, 0x78)` (start at the screen centre) and `RangeMouse(0, 0x140, 0, 0xF0)` (0..320, 0..240) run once from `func_80042BD4`, which `func_80042AC8` (`80042A00.c`) calls when the controller kind in `GameState.unk_F74` changes to 0 (the pad type byte of port 1 has high nibble 1, i.e. a mouse is plugged in; value 1 = other controller, 0xFF = none); 0 emulator hits, no mouse is attached there. `mouse_read` / `mouse_update` / `MouseState` have no caller. The live mouse logic is elsewhere: `func_80042CB0` (`80042A00.c`) reads the pad receive buffer directly (bytes 4 and 5 = signed dx, dy; byte 3 low nibble bits 4 and 8 = the two buttons, active low), scales dx, dy by `GameState.unk_F6F / 3`, and clamps the cursor to x -0x96..0x8C, y -0x64..0x64 relative to the centre. Pad type byte 0x12 means mouse (`PAD_TYPE_MOUSE`); buttons 0x4 / 0x8 are the right / left button in the hardware's bit order (`MOUSE_BUTTON_RIGHT`, `MOUSE_BUTTON_LEFT`; the game maps them to confirm and cancel, swapped by `unk_F75`). Port: map the Godot mouse to the same cursor variables; the sample module is not needed.

## 8. Boot mode and debug (`boot_mode.c`, `debug_info.c`)

### 8.1 Startup timeline (emulator, first 3600 frames)

| frame | event |
|---|---|
| 1 | main loop runs, mode 0x10 (initial value set in `80041000.c`) |
| 621 | `cd_command(14)`: set mode, from the sound init |
| 622 | `movie_play(0, 0xA7, 0x190)` (logo movie, HYPOTHESIS: publisher logo) |
| 1256 | movie done: `cd_command(9)`, `cd_command(10)`, `main_state_set(0)` |
| 1256-1300 | mode 0, step 0: 53 sectors from LBA 0x9D85 to 0x80180000 (checked read) |
| 1300 | read done: `boot_hud_init`, then 8 sectors from LBA 0x9C97 to 0x801F0000 (unchecked) |
| 1318 | step 1: `boot_hud_textures_load` |
| 1322 | mode 0x12 (title): its own loads start (`func_80044890`, unchecked reads) |
| 1565 | `models_update_draw` starts running every frame |
| 3534-3589 | overlay NAME_ENT loaded (96 sectors) and verified with `file_checksum_ok` |
| 3836, 4653, 6754 | NAME_ENT prologue: `xa_clip_request_set` with speaker 0xF |
| 7208 | NAME_ENT `func_80142A80`: `movie_play(0, 0x33B, 0x820)` (opening movie); `movie_captions_update` has 0 hits in the first 292 movie frames |

### 8.2 Rules

- `boot_mode_run` (mode 0 handler): stage 0 runs `boot_mode_step`; any other stage calls `main_state_set(0x12)`.
- `boot_mode_step` by step (`GameState.unk_110A`): 0 `boot_data_load_step` (sub-step in `unk_110D`: 0 start the 53-sector read; 1 when `cd_read_sync_status() & 1`, run `boot_hud_init` and start the 8-sector read; 2 when done, next step); 1 `boot_hud_textures_step`; 2 `func_8004ADE4` and next step; anything else `func_80042808` (stage + 1).
- `boot_hud_init`: for 16 palette slots, sets bit 15 (the PS1 semi-transparency bit) of each non-zero 15-bit colour in two sub-tables (D_800C62C0, 32 words; D_800C6200, 8 words; two colours per word) and uploads the palette block at D_800C6020 to that slot; then initialises the calendar (`hizuke_init`), parameter panel (`parameter_show_init`, twice), icon and message window objects and hides them.
- `boot_hud_textures_load`: loads the HUD texture pages (ids 0x1B-0x1F and 0x3B-0x3F) from the table pointers D_800C9730 and D_800C5FCC. The code adds 0x80000 to every table word once (guarded by testing the same bit in the first word) before the loads and subtracts it again after; it also subtracts 0x80000 from the matching GameState words (`unk_1228` entries 0x1B-0x1F and 0x3B-0x3F), which `load_tpage_buf_lock` filled while the bit was set. The meaning of that bit is open.
- `debug_state_numbers_draw`: prints mode (`unk_1108`), stage (`unk_1109`), step (`unk_110A`), `unk_110D`, four counters (`unk_1104`, `unk_1100`, `unk_10FC`, `unk_10F4`), one byte at D_800E7D34 and the stack pointer at fixed screen positions; the main loop calls it only when D_800B3220 is non-zero. It never ran in 3600 emulator frames.
- `snd_load_error_report(code)`: prints the code (debug format string), then sets `D_800B3D54[D_800B3D24] = 1`, the load-group error flag. The sound driver passes `(slot << 8) | 0x01010000` for a failed VAB open, `0x01020000` for a failed body transfer, `0x01040000 | bank` for a failed sequence open.
- `exit_function` (mode 0xFF, no src caller sets it): waits for the drawing queue, stops the CD, clears the CD callbacks, closes the event, stops the four root counters, resets graphics and callbacks, and loads the system launcher executable from the disc. Port: quit.

## 9. How this connects to the rest of the game

- Every loader (overlay loader, load groups `func_80044750`, boot, dec_bg, TT, TACO, NAME_ENT) uses `cd_read_sectors` / `cd_read_sync_status`; every overlay that ends or aborts calls `game_reset_to_title`.
- Voiced lines: `xa_clip_request_set` + the sound driver in `80079B10.c` + `CD_CMD_XA_START`; the "done" and "playing" bits of `cd_state` pace text and lip movement in NAME_ENT, EN_NICHI, OPTION, DATE, ETC.
- Backgrounds: all scene overlays call `dec_bg_cd_read` then `dec_bg_decode` then `dec_bg_show_*`, and draw with `dec_bg_draw` or the fade and cross-fade draws each frame.
- Mode ids used here follow the core unit's enum (this table has no `MainMode` rows): 0 `MODE_BOOT`, 0x10 `MODE_START` (its handler `func_80062C28` plays the logo movie, then switches to mode 0), 0x12 `MODE_TITLE`, 0xFF `MODE_EXIT` (`exit_function`).

## 10. Open questions

- Layer meaning: layer 0 is drawn at screen y 72..160 and layer 1 at 0..160 from the same texture page area; whether layer 0 is a lower foreground strip or the lower half of a 240-line picture is unresolved (the full-240 draw reads both stacked in buffer 0).
- `cd_busy_marker_draw` condition: the flag byte D_8011ED17 bit 0x80 is not identified (a debug switch is likely).
- `D_80125C5C` (read in `strCallback`, never written) and `D_800B5930` / `D_800B5934` (written only by `dec_bg_reset`) have no known use.
- The start slot of `models_update_draw` (GameState + 0x109C) is unnamed.
- Whether the 15-bit movie branch and the 22 caption marks are ever active (needs a build or hack that clears bit 0 of `movie_flags`; the shipped data never does).
- `cd_last_read_lba` is written but never read in `src/`.
- The meaning of bit 0x80000 in the HUD texture tables around `load_tpage_buf_lock` (`boot_hud_textures_load`).
- Whether `cd_queue_push` can overflow: `cd_command` ignores a full ring (-1 return), no guard was seen.
- Who triggers mode 0xFF (nothing in `src/` sets it).
- SDK identities of the unnamed libcd/libpress/libgs/libspu calls in this unit (`func_80087954` = CdSync, `func_800879D0` = CdControl, `func_80087C64` = CdControlB, `func_80087E4C` = CdReadSync, `func_80087E6C` = CdRead, `func_80087EE8` = CdIntToPos, `func_800880C0` = CdRead2, `func_80087FF0` = CdPosToInt, `func_8009A4D0` = GsSortObject4, `func_80099540` = GsSetRefView2) are inferred from signatures and the movie sample, not from the SDK symbol table.
- What the fade draws show: `dec_bg_fade_draw` uses one buffer for a semi-transparent and an opaque pass, `dec_bg_crossfade_draw` three passes over two buffers. How the passes composite (ordering-table direction, the semi-transparency bit of the decoded 15-bit pixels) needs an emulator run in a date place change; until then "fade" and "cross-fade" are HYPOTHESIS readings (section 5.2).

## 11. Names to settle at merge (overlaps with other proposals)

Checked against `core.tsv` and `h-graphics.tsv` on 2026-10-24 (the files exist in the sibling phase-2 worktrees, not in this tree).

- Settled here: `GameState.unk_014/unk_018` are `draw_origin_x/y` (the graphics unit's name; it adds them to every primitive and includes the shake); `unk_000/002/004` stay `screen_width/height/interlace`. `MainMode` rows were dropped; the core enum owns them. The ring struct is `CdCommand` in both proposals; this unit's enum of public ids is `CdCommandId`.
- Different names for the same object (core name -> this unit's name). Recommendation: take this unit's names for the nine files of this unit (its callers and bodies were read in full) and do not mix the core `cd_cmd_*` / `cd_audio_*` family with the `cd_queue_*` / `xa_*` family; if the core names win, apply them as a whole.

| old | core | h-cdio |
|---|---|---|
| `func_800452C4` | `cd_cmd_reset` | `cd_queue_reset` |
| `func_80045318` | `cd_cmd_push` | `cd_queue_push` |
| `func_80045414` | `cd_audio_command` | `cd_command` |
| `func_800455C4` | `cd_cmd_pump` | `cd_queue_update` |
| `func_800460DC` | `cd_done_count` | `cd_read_count_get` |
| `func_800460EC` | `cd_audio_state` | `cd_state_get` |
| `func_80046274` | `cd_audio_ticks` | `xa_position_get` |
| `func_80046290` | `cd_audio_request_set` | `xa_clip_request_set` |
| `func_800462C8` | `cd_read_sectors_nowait` | `cd_read_sectors_unchecked` |
| `func_8005751C` | `dec_bg_decode_to_vram` | `dec_bg_decode` |
| `func_8005A06C` | `mode_boot` | `boot_mode_run` |
| `D_80125130` | `cd_cmd_ring` | `cd_queue` |
| `D_800B3D70` | `cd_audio_request` | `xa_clip_request` |
| `D_800B5938` | `dec_bg_show` | `dec_bg_shown` |
| `D_800B5948` | `last_screen_mode` | `dec_bg_last_layer` |
| `GameState.unk_000/002/004` | `video_width/height/interlace` | `screen_width/height/interlace` |
