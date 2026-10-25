---
type: spec
updated: 2026-10-24
sources: ["src/main/sound_driver.c", "src/main/scene_library.c", "src/main/frame_render_audio.c", "include/main_api.h", "config/renames/proposals/m-dates-a-apply.tsv", "config/renames/proposals/m-dates-a-essential.tsv", "wiki/spec/core.md", "wiki/spec/h-cdio.md", "asm/libsnd.s"]
---

# Port spec: main sound driver (`src/main/sound_driver.c`)

Unit `m-dates-a` of phase 2 ([[tickets/T-27014-p2-m-dates-a]], parent [[tickets/T-27000-phase-2]]). The `config/renames/proposals/m-dates-a-apply.tsv` names and count are the historical preparation packet; current names follow the applied ledgers and source, with unapplied proposal rows remaining candidates. The `audio_` prefix follows the core ownership map. Facts are as the matched C implements them; a claim of game meaning that the code does not prove is marked HYPOTHESIS. Evidence beyond the C: the libsnd/libspu asm in `asm/libsnd.s`, five short cold-boot emulator runs (section 13) and the O.BIN developer names (`SD_*`, quoted in the table as historical evidence).

## 0. Scope correction (read first)

The work list labels this unit "date sequences, first half (`normal_date_*`, girl selection, portrait loads, event faces, 165 functions)". That label does not match the file. `src/main/sound_driver.c` (0x80079B10 to 0x8007C030) holds 54 functions and **all of them are the game's sound driver**: command queue, sound-bank (VAB) slots, background-music sequences, sound-effect keys, CD-audio voice clips and the level meter initialisation. The date, girl-selection, portrait and event-face functions are in the next file, `src/main/scene_library.c` (111 functions, unit `m-dates-b`); 54 + 111 = 165 is the number in the label. This spec covers the file named in the unit, which is a hardware-adjacent module: the driver is the game's own code (compiled with the game, not an SDK library) and its command protocol is the contract every scene uses, so it is described fully. What the PS1 libraries do underneath (SPU, libsnd internals) is named only where the driver calls them.

Overlaps: `core` already names `func_80044750` (`audio_command`, the 1,180-site sender), `func_80079B34` (`audio_command_pump`), `D_80125D10` (`audio_state`) and its type `AudioState`; this table keeps those names and adds the fields. `h-graphics` names the bank loader that sits in front of this driver (`frame_render_audio.c`) with the same `audio_` family; `h-cdio` names the CD side (`cd_audio_command`, `cd_audio_request_*`, `cd_audio_ticks`, `cd_audio_state`).

## 1. What the module is

The driver turns 16-bit command words into libsnd and CD-audio calls. The game never calls libsnd directly: it pushes a command word with `audio_command(id)` (core name of `func_80044750`, which masks the id to 16 bits and calls `audio_command_submit`), and once per frame `audio_command_pump` drains the queue. Four jobs:

1. **Sound banks.** Four VAB slots (0 to 3) hold the instrument/sample banks in SPU RAM. Commands upload bodies already read from the disc into main RAM, and assign the slots their roles.
2. **Background music (BGM).** One sequence at a time from a 16-entry sequence pack, with fade-out, chaining, pause, stop.
3. **Sound effects (SE).** One-shot key-on/key-off of a note of a program in one of four bank slots.
4. **Voice clips.** CD-XA audio streamed from the disc: the driver converts a clip request (speaker, start index, end index) into sectors, starts the CD, ramps the volume, fades at the end and can queue one clip behind another. A level meter reads the CD-audio capture buffer in the SPU so that the game can animate a mouth from the voice (section 8.5).

Everything is single-threaded: command handling and the per-frame tick run in one function chain called from the main loop.

## 2. Frame integration and entry points

| entry | name | when |
| --- | --- | --- |
| `func_80079B10` | `audio_command_submit` | any time; pushes `id & 0xFFFF` (queue of 32; a 33rd command in one frame is dropped) |
| `func_80079B34` | `audio_command_pump` | once per frame at the end of the main loop iteration after `draw_frame` and the CD pump, only while `GameState.sound_active` (`unk_034`) is nonzero (1 from `game_state_init`); the TT overlay also calls it itself, from its bank-load wait loops and one scene loop, where it does not go through the main loop |
| `func_8007B844` | `audio_init` | boot (`func_80042134`), soft reset (pad combination held 0x81 frames, `func_80042A00`, after command 0x7F), `game_mode_reset_to_title` (`func_80046500`) |
| `func_8007BDE8` | `audio_shutdown` | the two reset paths, directly before `audio_init` |
| `func_8007BFA8`, `func_8007BFB8` | `audio_bgm_next_clear`, `audio_reverb_off_toggle` | DATE overlay (`8014F0F0.c`) and the OPTION sound test (`801389A0.c`) |
| `func_80079E00` | `audio_bgm_bank_ready` | the bank wait in `frame_render_audio.c` (`func_80044F94`) |
| `SD_InitCDLevelInfo` | O.BIN name kept | `audio_init` |

One pump pass: pop commands until `audio_queue_pop` returns 0 and dispatch each by `id & 0xF00`; then `audio_xa_frame_update` (voice clip watch), then `audio_pending_update` (bank transfers, BGM tick, voice fade tick), then `audio_queue_clear`. Because the pop uses 0 as "empty", a command word of 0 ends the drain and the rest of the queue is discarded by the clear. Nothing in `src/` sends 0.

## 3. The command word

`id` is 16 bits. `id & 0xF00` is the **class**, the low byte the argument, bits 12 to 14 a **modifier**.

| class (`id & 0xF00`) | name (const) | handler | meaning |
| --- | --- | --- | --- |
| 0x000 | `AUDIO_CLASS_CONTROL` | `audio_control_dispatch` | control commands, section 3.1 |
| 0x200 | `AUDIO_CLASS_BGM` | `audio_command_bgm` | BGM, low byte = sequence number in the current pack |
| 0x300 | `AUDIO_CLASS_VOICE` | `audio_command_xa` | voice clip |
| 0x500 | `AUDIO_CLASS_SE_A` | `audio_command_se_a` | sound effect in the bank of `se_slot_a` |
| 0x600 | `AUDIO_CLASS_SE_B` | `audio_command_se_b` | sound effect in the bank of `se_slot_b` |
| 0x700 | `AUDIO_CLASS_SE_BGM_BANK` | `audio_command_se_bgm_bank` | sound effect in the bank of `bgm_slot` |
| 0x800 | `AUDIO_CLASS_SE_BGM_AUXILIARY` | `audio_command_se_bgm_auxiliary` | sound effect in the bank of `bgm_auxiliary_slot` |
| other | | none | ignored |

Modifier bits (`AudioModifier`): 0x1000 = stop instead of start (classes 2, 3, 5 to 8), 0x2000 = pause and 0x4000 = resume (classes 2 and 3 only). Class 0 uses the whole low byte; its bits 12 to 14 are not read.

### 3.1 Control commands (class 0, `id` in 0x00 to 0xFF)

`audio_control_dispatch` switches on `id & 0xF0`.

| id | const | effect |
| --- | --- | --- |
| 0x01 / 0x02 | `AUDIO_COMMAND_OUTPUT_MONO` / `_STEREO` | `audio_output_mode_set` (section 9). No code sends them; `audio_init` selects stereo |
| 0x03 | `AUDIO_COMMAND_XA_FORGET` | stop the voice clip and zero its bookkeeping (start, end, queued, resume, channel bytes) |
| 0x04 | `AUDIO_COMMAND_VOICES_KILL` | `audio_voices_kill`: flush, all voices key off, reverb off, reverb mode 3, reverb on |
| 0x05 | `AUDIO_COMMAND_FADE_VIA_MASTER` | set `AUDIO_STATE_FADE_VIA_MASTER`: the next BGM fade acts on the master volume and ends muted (section 6.4) |
| 0x06 | `AUDIO_COMMAND_XA_RESET` | `audio_xa_reset`: CD mix off, clear voice state, level meter and bookkeeping. Emulator: `func_800462C8` (CD read) sends it before every data read |
| 0x11 | `AUDIO_COMMAND_BANKS_TRANSFER_01` | `audio_bank_body_address_set` then dispose and request transfer for slots 0 and 1 |
| 0x12 | `AUDIO_COMMAND_BANKS_TRANSFER_23` | same for slots 2 and 3 |
| 0x13 to 0x16 | `AUDIO_COMMAND_BANK_TRANSFER_SLOT0..3` | one slot |
| 0x21 | `AUDIO_COMMAND_SE_SLOTS_SET0` | `se_slot_a = 0`, `se_slot_b = 3` |
| 0x24 | `AUDIO_COMMAND_SE_SLOTS_SET1` | `se_slot_a = 3`, `se_slot_b = 0` |
| 0x22 | `AUDIO_COMMAND_BGM_SLOTS_SET0` | `bgm_slot = 1`, `bgm_auxiliary_slot = 2` |
| 0x23 | `AUDIO_COMMAND_BGM_SLOTS_SET1` | `bgm_slot = 2`, `bgm_auxiliary_slot = 1` |
| 0x2F | `AUDIO_COMMAND_SEQ_PACK_SET0` | `sequence_pack_set = 0`, index the pack at RAM 0x8001C000 |
| 0x2E | `AUDIO_COMMAND_SEQ_PACK_SET1` | `sequence_pack_set = 1`, index the pack at 0x8002E800 |
| 0x71 | `AUDIO_COMMAND_STOP_BGM` | stop the sequence now |
| 0x74 | `AUDIO_COMMAND_STOP_XA` | stop the voice clip now; a clip queued behind it is moved into the current slot without starting, so it is lost |
| 0x7F | `AUDIO_COMMAND_STOP_ALL` | all three of the 0x70 family |
| 0x70 + bits | | bit 1 stops BGM, bit 2 `audio_voices_kill`, bit 4 stops the voice clip (so 0x72, 0x73, 0x75, 0x76, 0x77 also exist; only 0x71, 0x74, 0x7F are sent) |
| 0xB1, 0xC1, 0xD1 | `AUDIO_COMMAND_FADE_BGM_FAST/MEDIUM/SLOW` | fade out the BGM (section 6.3) |
| 0xB4, 0xC4, 0xD4 | `AUDIO_COMMAND_FADE_XA_FAST/MEDIUM/SLOW` | fade out the voice clip |
| 0xBF, 0xCF, 0xDF | `AUDIO_COMMAND_FADE_BOTH_*` | both (bits 1 and 4 set) |
| 0xB2, 0xC2 | `AUDIO_COMMAND_NOP_B2`, `_C2` | no effect: bit value 2 is not tested in the fade family (sent by GEKO and TAIIKU). HYPOTHESIS: meant to fade the sound-effect voices |
| 0xE1 / 0xE4 | `AUDIO_COMMAND_PAUSE_BGM` / `_XA` | 0xE1 pauses the sequence (TT only). 0xE4 is called pause for the voice clip (RPG_BAT 3 sites, BUNKA_SD 1) but, as a class-0 command, it is a cancel: with the CD start still pending it stops the CD and forgets the clip; with the clip playing it forgets the clip, cancels the fade it just started and issues no stop (section 8.4) |
| 0xF1 / 0xF4 | `AUDIO_COMMAND_RESUME_BGM` / `_XA` | 0xF1 resumes the sequence (TT only). 0xF4 never resumes a voice clip as a class-0 command: the paused bit is already clear, so it only forgets the clip (section 8.4); no sender in `src/` |

The family test is `id & 0xF0` in {0x10, 0x20, 0x70, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0}; any other value goes to `audio_command_system` which handles low bytes 1 to 6 (rows above) and ignores the rest.

### 3.2 BGM (class 2)

`audio_command_bgm` looks at `id & 0xF000` (the modifier):

| id | effect |
| --- | --- |
| `0x200 + n` | play sequence n of the current pack set (section 6.1) |
| `0x1200` (any low byte) | stop (same as 0x71); `audio_bgm_stop` and the sound test send it |
| `0x2200` / `0x4200` | pause / resume (same as 0xE1 / 0xF1) |

### 3.3 Voice clip (class 3)

`0x300` starts the clip prepared by `cd_audio_request_set` (section 8). `0x1300`, `0x2300`, `0x4300` stop, pause and resume it: 0x1300 does the same as class-0 0x74 (stop, then `audio_xa_promote_next`), while 0x2300 and 0x4300 call `audio_xa_control(0xE4 / 0xF4)` **without** the promote that the class-0 forms 0xE4 and 0xF4 add, so they behave differently from them (section 8.4). Only 0x300 is sent in `src/` (96 sites, `talk_voice_play` among them); the other three class-3 ids and 0xF4 have no sender.

### 3.4 Sound effects (classes 5 to 8)

Let `n = id & 0xFF`. The key is `(slot << 8) | (n >> 4)` for the program (the bank slot in the upper byte) and the note word `((n & 0xF) * 4 + 0x18) << 8`. So the high nibble of `n` picks the **program** inside the bank and the low nibble picks one of 16 **notes** `0x18, 0x1C, ... 0x54`; every literal id in `src/` has `n < 0x10`, i.e. program 0 and a different note per effect (the OPTION sound test counts effects 0 to 0x20, so banks hold at least programs 0 to 2; computed ids in RPG_BAT and TACO come from tables). With modifier 0x1000 the same key is released (`audio_se_stop`); otherwise `audio_se_play` releases the same key first (re-trigger), clears `AUDIO_STATE_FADE_VIA_MASTER`, restores the master volume to 0x7F and keys the note on with `se_volume` on both channels (0x5A, i.e. 90 of 127; 0x42 in mono). Effects are not stopped by a new BGM unless the BGM start kills voices (section 6.1).

The slot comes from the class: 0x5xx `se_slot_a`, 0x6xx `se_slot_b`, 0x7xx `bgm_slot`, 0x8xx `bgm_auxiliary_slot`. The slot variables change with the bank assignment commands, so the same id can sound different in different scenes (section 5).

### 3.5 Which ids the game sends

Counts are literal arguments of `func_80044750(...)` in `src/` (computed ids such as `seq | 0x200` and `n | 0x500` are not counted).

| id | sites | main senders (units) |
| --- | ---: | --- |
| 0x3 | 3 | main, NAME_ENT |
| 0x4 | 12 | TT only |
| 0x5 | 10 | NAME_ENT 7, main 3; every sender follows it at once with 0x71 or a BGM fade (0xB1, 0xC1, 0xD1); six of the ten sit next to a bank load (section 6.4) |
| 0x6 | 2 | main (the CD read functions) |
| 0x11 to 0x16 | 2 + 8 + 4 | main bank loader, TT |
| 0x21 to 0x24, 0x2E, 0x2F | 15 to 25 each | bank loader `main`, TT, KANGEI, SHOUGATU, DATE, GEKO, SHUGAKU, TAIIKU, TACO |
| 0x71 | 5 | main, NAME_ENT, RENSYU, TT |
| 0x74 | 35 | main 10, RPG_BAT 5, EN_NICHI 4, OPTION 4, TAIIKU 4, BUNKA_SD 4, DATE 2, DATE2, TACO |
| 0x7F | 21 | main 7, EN_NICHI 4, BUNKA_SD 4, NAME_ENT 2, TT 2, DATE, RPG_BAT |
| 0xB1 / 0xC1 / 0xD1 | 18 / 31 / 9 | TT 11 / BUNKA_SD 12 / NAME_ENT 3 and main 3 |
| 0xB4 / 0xC4 | 4 / 7 | main (dialogue, section 11), DATE; BUNKA_SD 5, DATE 2 |
| 0xBF / 0xCF | 64 / 16 | BUNKASAI 19, BUNKAKEN 17, MASTER 10, DATE 5, TAIIKU 5 / GEKO 4, SHOUGATU 3, DATE 3 |
| 0x200 to 0x209 | 105, 28, 41, 16, 13, 14, 9, 4, 2, 1 | almost every scene overlay; `0x200` (sequence 0) is by far the most frequent |
| 0x300 | 96 | NAME_ENT 20, main 18, BUNKA_SD 16, ETC 10, TAIIKU 7, ENDING 6, EN_NICHI 5, RPG_BAT 4, DATE 3, OPTION 3, TACO 2, GEKO, DATE2 |
| 0x500 to 0x50F | 46, 97, 30, 20, 14, 13, 9, 8, 10, 9, 25, 20, 16, 11, 4, 7 | `0x501` is the most used (OPTION 40, NAME_ENT 20, OLH 8, BUNKA_SD 7, DATE 5, main 5, ...); `0x50B` and `0x50C` are almost only TEL (17 and 12 of 20 and 16); `0x50D` main 8 of 11. HYPOTHESIS: 0x501 is the menu cursor/confirm sound, 0x50B/0x50C telephone sounds (from the senders only) |
| 0x600 to 0x60E | 1 to 8 each | DATE, KANGEI, ETC, SHOUGATU, GEKO, TAIIKU, DATE2, OMIMAI, NAME_ENT, main (0x60A GEKO 3, SHOUGATU 2, TAIIKU 2) |
| 0x700 | 2 | TAIIKU |
| 0x800 to 0x806 | 13, 11, 10, 3, 1, 1, 3 | TAIIKU only |
| stops | 0x1200 1, 0x1500 1, 0x1506 1, 0x1508 8, 0x1605 2, 0x1700 1, 0x1801/1802/1804/1805 3 each | 0x1508: GEKO 3, TEL 2, SHOUGATU 2, DATE2; the 0x16xx to 0x18xx ones: TAIIKU |

Computed senders: the bank loader (`seq | 0x200`), OPTION's sound test (`n | 0x500`, `n | 0x1200`), TACO (`v | 0x500` / `v | 0x600`), RPG_BAT (`a | 0x500` / `a | 0x600`), GEKO (a table-driven `0x6xx`), DATE (id tables), SHOUGATU, EN_NICHI, TT (id passed through).

## 4. Driver state

All state is one 0x50-byte object `audio_state` (type `AudioState`, `D_80125D10`) plus a few arrays (section 5) and the queue. The source also reaches fields through separate address labels (`D_80125D14` to `D_80125D5F`), and the VAB header, SPU-address and body-size arrays have separate labels for some later elements. These are aliases, not independent game variables, so the symbol-only packet omits them: the field and array names are authoritative. The root apply pass must migrate any remaining direct-label accesses and declarations to the aggregate without creating independent names for array elements. `D_80125D14` is also used by `scene_library.c`.

| offset | name | meaning |
| --- | --- | --- |
| 0x00 | `state_flags` | what is happening (section 4.1) |
| 0x04 | `request_flags` | pending work (section 4.1) |
| 0x08 / 0x0C | `xa_clip_start` / `xa_clip_end` | sectors of the current clip (0 = none) |
| 0x10 / 0x14 | `xa_next_start` / `xa_next_end` | sectors of the clip queued behind it |
| 0x18 | `xa_resume_position` | sector kept for a resume: the pause selector stores the playing position here (not below the clip start); `audio_xa_promote_next` overwrites it with the new clip start, so the class-0 pause never replays from it |
| 0x1C / 0x1E | `xa_volume` / `xa_volume_full` | CD-audio volume now (faded) / at start (per speaker) |
| 0x20 / 0x22 | `bgm_volume` / `bgm_volume_full` | sequence volume now / at start |
| 0x24 / 0x26 | `se_volume` / `se_volume_default` | key-on volume of effects, and its default (always equal) |
| 0x28 | `bgm_sequence_handle` | access number returned by the sequence-open call |
| 0x2A | `bgm_sequence_id` | sequence number to start or running, -1 none |
| 0x2C | `bgm_next_sequence_id` | sequence number waiting behind a fade, -1 none |
| 0x2E / 0x30 / 0x32 | `bgm_fade_accumulator` / `_step` / `_rate` | BGM fade accumulator (8.8), volume units per carry, accumulator increment per frame |
| 0x34 / 0x36 / 0x38 | `xa_fade_accumulator` / `_step` / `_rate` | the same for the voice fade |
| 0x3A | `sequence_pack_set` | 0 or 1: which sequence pack is current |
| 0x3C / 0x3E | `bgm_slot` / `bgm_auxiliary_slot` | VAB slots for the BGM bank (class 7) and its companion (class 8) |
| 0x40 / 0x42 | `se_slot_a` / `se_slot_b` | VAB slots for classes 5 and 6 (HYPOTHESIS: slot a holds the shared system-sound bank) |
| 0x44 | `cd_mode_param[4]` | parameter bytes of `cd_audio_command(0xE, ...)`; byte 0 = 0xC8 (double speed, real-time play, XA filter) |
| 0x48 | `xa_params[8]` | [0] XA filter file, [1] filter channel (passed to `cd_audio_command(0xD, ...)`); [4] channel and [6] file of the current clip; [5] and [7] the same for the queued clip |

Volumes are libsnd units, 0 to 127 (master volume 0x7F is full scale; the serial/CD volume and the sequence volume use the same range); the fade counters work on those values directly.

### 4.1 Flag words

`state_flags` (`AudioStateFlags`):

| bit | const | meaning |
| --- | --- | --- |
| 0x10 | `AUDIO_STATE_STEREO` | stereo output (set by `audio_state_init`) |
| 0x20 | `AUDIO_STATE_MONO` | mono output |
| 0x100 | `AUDIO_STATE_BGM_PLAYING` | a sequence is open and playing |
| 0x400 | `AUDIO_STATE_XA_PLAYING` | a voice clip is playing and its end is watched; the level meter is read only then |
| 0x1000 | `AUDIO_STATE_BGM_PAUSED` | sequence paused |
| 0x4000 | `AUDIO_STATE_XA_PAUSED` | voice clip paused. Set by the pause selector, but the class-0 pause command clears it again in the same call (`audio_xa_promote_next`); it stays set only after the class-3 form 0x2300, which nothing sends (then also while the pause fade runs) |
| 0x10000 << i | `AUDIO_STATE_BANK_OPEN_0..3` | slot i's header is open (advisory: `audio_bank_dispose` closes the VAB without clearing it) |
| 0x100000 | `AUDIO_STATE_FADE_VIA_MASTER` | BGM fades through the master volume (section 6.4) |
| 0x10000000 | `AUDIO_STATE_SOUND_TEST_REVERB_OFF` | toggled by the sound test; read only in mode 0x21 |

`request_flags` (`AudioRequestFlags`):

| bit | const | meaning |
| --- | --- | --- |
| 0x1 | `AUDIO_REQUEST_BGM_FADE` | a BGM fade-out is running |
| 0x2 | `AUDIO_REQUEST_BGM_START` | a BGM start waits for its bank |
| 0x100 | `AUDIO_REQUEST_XA_FADE` | a voice fade-out is running |
| 0x200 | `AUDIO_REQUEST_XA_STARTING` | the CD start command was issued, waiting for the CD to report playing |
| 0x01000000 << i | `AUDIO_REQUEST_BANK_WANTED_i` | slot i transfer wanted |
| 0x10000000 << i | `AUDIO_REQUEST_BANK_RUNNING_i` | slot i transfer running |

`audio_pending_update` uses the three byte ranges: top byte -> bank transfers, low byte -> BGM tick, second byte -> voice fade tick.

### 4.2 Defaults (`audio_state_init`)

`state_flags` 0x10, `request_flags` 0, all sector words 0, `xa_volume` and `xa_volume_full` 100, `bgm_volume` and `bgm_volume_full` 0x50, `se_volume` and `se_volume_default` 0x5A, `bgm_sequence_id` and `bgm_next_sequence_id` -1, BGM fade step/rate 0x80/0xFF (overwritten by the first fade), voice fade step/rate 0x80/0xFF, `sequence_pack_set` 0, `bgm_slot` 1, `bgm_auxiliary_slot` 2, `se_slot_a` 0, `se_slot_b` 3, `xa_params[4..7]` 0. The emulator dump taken at a clip start matches these (section 13).

## 5. Banks

Four **slots**, each a VAB (header + sample body). Layout fixed by `audio_bank_layout_init`:

| slot | header in RAM | body size | SPU RAM start |
| ---: | --- | --- | --- |
| 0 | 0x80010000 | 0x10000 | 0x1010 |
| 1 | 0x80012000 | 0x28000 | 0x11010 |
| 2 | 0x80016000 | 0x28000 | 0x39010 |
| 3 | 0x8001A000 | 0x1A768 | 0x61010 |

The sizes are stored in `audio_bank_body_size` and never read. SPU RAM below 0x1010 is left free (the capture buffers of section 8.5 live there). Slots 0 and 1 form **set 0**, slots 2 and 3 **set 1**; the loader in `frame_render_audio.c` (`audio_bank_transfer_start`) sends the transfer and assignment commands for one set at a time (HYPOTHESIS: it loads the files of one set while the other set plays; this file does not show that):

| set | transfer | slot roles | commands |
| --- | --- | --- | --- |
| 0 | 0x11 (or 0x13 / 0x14) | class 5 = slot 0, class 6 = slot 3, BGM (class 7) = slot 1, class 8 = slot 2; pack at RAM 0x8001C000 | 0x22, 0x21, 0x2F |
| 1 | 0x12 (or 0x15 / 0x16) | class 5 = slot 3, class 6 = slot 0, BGM = slot 2, class 8 = slot 1; pack at 0x8002E800 | 0x23, 0x24, 0x2E |

The body RAM addresses are set when the transfer command arrives (`audio_bank_body_address_set`): `0x11` slot 0 at the load base and slot 1 at base + 0x10000; `0x12` slot 2 at the base and slot 3 at base + 0x28000; `0x13` to `0x16` one slot at the base. The base is `cd_load_destination_base_get` (`func_800451D0`), which the loader sets to 0x801B8000 (or 0x801C8000, 0x801E0000) before the command (`func_80044D54`).

**Transfer protocol.** `audio_command_bank_transfer` calls `audio_bank_dispose(slot)` (stop the sequence if it is in this slot; key off all effect voices if it is `se_slot_a`; close the VAB) and `audio_bank_request_transfer(slot)` (finish a running BGM fade if the slot is the BGM slot; set request bit 0x01000000 << slot). Each pump pass `audio_bank_transfer_schedule` takes the lowest requested slot when no transfer is running, marks it running and calls `audio_bank_transfer`, which opens the header (SsVabOpenHeadSticky), sets the open bit, uploads the body (SsVabTransBody) and waits for the end inside the same frame; so one slot is processed per frame. A failure returns an error code to `func_80059B04` (`audio_debug_load_error_report`, h-cdio): `0x01010000 | slot << 8` header, `0x01020000 | slot << 8` body, `0x01040000 | slot << 8 | pack set` sequence open. No retry.

**Ready test.** `audio_bgm_bank_ready` is true when neither the wanted nor the running bit of **`bgm_slot`** is set. The original reads the slot from the state and ignores its argument, so `func_80044F94` (bank wait, `audio_bank_transfer_done`), which asks about slots 0/1 or 2/3, effectively tests only the BGM slot twice. Emulator: after command 0x12 the BGM could start while slot 3's transfer was still requested (frame 1464).

**Sequence pack format** (`audio_sequence_pack_index`, RAM 0x8001C000 for set 0, 0x8002E800 for set 1): 16 little-endian words at +0x00, each an offset relative to pack + 0x10 (a 0 entry means "same as entry 0"), then 16 bytes at +0x40 (reverb depth of each sequence), sequence data from +0x50. The result is `audio_sequence_address[set][i]` and `audio_sequence_reverb_depth[set][i]`. The loader checks that the first offset is 0x40. A sequence id of 16 or more is not range-checked.

## 6. BGM

### 6.1 Starting (`0x200 | n`)

`audio_command_bgm`, case start:

1. A BGM fade is already running (`AUDIO_REQUEST_BGM_FADE`): store n in `bgm_next_sequence_id` and finish the fade at once (`audio_bgm_fade_finish`: stop, promote the queued id, start it).
2. Otherwise a sequence is playing: store n in `bgm_next_sequence_id` and start a **fast fade** (0xB1); the fade end starts n.
3. Otherwise `bgm_sequence_id = n`, `bgm_next_sequence_id = -1`, `audio_bgm_request_start`.

`audio_bgm_request_start` runs `audio_bgm_start` immediately if the BGM bank has no pending transfer, else sets `AUDIO_REQUEST_BGM_START` and `audio_bgm_update` starts it as soon as the bank is ready.

`audio_bgm_start`: (a) unless the game is in the TT mini game, the TACO shooter or `bgm_slot` is 2, kill all voices and reset reverb (`audio_voices_kill`): **effects still sounding are cut when a new BGM starts** in set 0; (b) reverb depth = `audio_sequence_reverb_depth[sequence_pack_set][bgm_sequence_id]` on both channels (forced 0 only in mode 0x21 while `AUDIO_STATE_SOUND_TEST_REVERB_OFF`); (c) open the sequence from `audio_sequence_address[sequence_pack_set][bgm_sequence_id]` on VAB `bgm_slot`; -1 reports error 0x0104xxxx and runs `audio_bgm_fade_finish`; (d) set `AUDIO_STATE_BGM_PLAYING`, clear paused, request bits 1 and 2 and the master-fade flag; (e) volume = the VAB's master volume (header byte) - 0x19, minus 0x18 more in mono, stored in both `bgm_volume` and `bgm_volume_full`; (f) master volume 0x7F, sequence volume set, play with `play_mode` 1 and repeat count 1 (looping, if any, comes from the sequence data: not examined). Emulator: master volume 0x7F gave `bgm_volume` 0x66.

### 6.2 Stop, pause, resume (`audio_bgm_control`)

- Stop (0x71, 0x1200): if playing or paused, set the sequence volume 0, stop and close the sequence; `bgm_sequence_id = bgm_next_sequence_id`, `bgm_next_sequence_id = -1`; clear playing, paused and the fade bit. It does not start the promoted id (only the fade end does).
- Pause (0xE1, 0x2200): pause, set paused, clear playing. Resume (0xF1, 0x4200): replay, set playing. Used only by TT.
- `audio_bgm_next_clear` (DATE) drops `bgm_next_sequence_id`.

### 6.3 Fades (`audio_bgm_fade_start`, `audio_bgm_update`)

A BGM fade starts only if none runs and a running fade ignores later fade ids entirely (the title screen sends 0xD1 every frame for about 60 frames without restarting anything). Each frame while playing and fading: `bgm_fade_accumulator += bgm_fade_rate`; when the sum passes 0xFF (carry into bit 8), the low byte is kept and `bgm_volume -= bgm_fade_step`. Below 0 the fade ends (section 6.4). Otherwise the new volume is applied.

| id | rate | step | volume loss per frame | frames from 102 |
| --- | --- | --- | --- | --- |
| 0xB1 | 0x100 | 10 | 10 | 11 |
| 0xC1 | 0xC0 | 1 | 0.75 | about 137 |
| 0xD1 | 0x80 | 1 | 0.5 | about 205 |

The voice fade (`audio_xa_fade_start`) resets its accumulator only when none runs, but always rewrites rate and step, so a newer fade id changes a running voice fade. It uses the same table with `0xB4` (0x100, 10), `0xC4` (**0x100**, 1: one unit per frame, not 0.75) and `0xD4` (0x80, 1), and ends when `xa_volume <= 0`.

### 6.4 Fade end (`audio_bgm_fade_finish`)

With `AUDIO_STATE_FADE_VIA_MASTER` set and no voice clip playing, the fade did not touch the sequence volume but scaled the **master volume** (`bgm_volume * 0x7F / bgm_volume_full`), so effects fade too; at the end all voices are keyed off and the master volume is 0 (restored to 0x7F by the next effect or BGM start). Then in both cases the sequence is stopped (`audio_bgm_control(0x71)`, which promotes the queued id) and, if `bgm_sequence_id != -1`, started. Command 0x05 sets the master-fade flag. All 10 senders in `src/` send 0x71 or a BGM fade (0xB1, 0xC1, 0xD1) directly after it; six of them sit next to a bank load (`func_80044890`), four do not (NAME_ENT `80132000.c:2374`, `80139740.c:2587`, `80139740.c:2654`, `80144AD0.c:2698`). HYPOTHESIS: it marks a scene exit or bank replacement where the effects should fade with the music.

## 7. Sound effects

See section 3.4 for the key. Additional facts: there is no polyphony control; the key-off of a note releases every voice of that note; `audio_voices_kill` (command 4, the BGM start, 0x7F) is the only mass stop; effect volume does not follow BGM fades unless the master-fade mode is on.

## 8. Voice clips

### 8.1 Request and start (`0x300`)

The caller first fills the 4-word request with `cd_audio_request_set(start, end, speaker)` (h-cdio): `[0]` speaker (0 to 15), `[1]` a per-speaker base offset, `[2]` the start word, `[3]` the end word. `audio_command_xa` reads it with `cd_audio_request_get` and:

1. `audio_speaker_volume_set(speaker)`: `xa_volume_full` = 100 by default; 125 for speaker 6; 95 for 7, 9, 11; 90 for 10; 110 for 12 and 13; 105 for 15. HYPOTHESIS: the speaker is the heroine's index (h-cdio: 25 of 104 requests pass the current girl) so the table is a per-voice loudness correction; 13 would be the friend and 15 the narrator or system voice (the prologue clip used 15).
2. Split each of the start and end words into a top byte and a 24-bit index; the top byte of the start is the **channel**, the top byte of the end the **file** number of the CD-XA filter (saved in `xa_params[5]` and `[7]`). Sectors: `sector = index * m + channel + base` where `m` = 8 for speaker 15 and 0x20 for every other speaker, and the start is lowered by 0x10 and the end raised by 0x10 sectors as a margin. Note that the same channel byte is added to both.
3. If a clip is playing (`AUDIO_STATE_XA_PLAYING`): leave the new clip in `xa_next_*` and start a fast voice fade (0xB4). Else if no start is pending (`AUDIO_REQUEST_XA_STARTING` clear) and not paused: `audio_xa_promote_next` then `audio_xa_play(xa_clip_start)`. If a start is pending the new clip simply waits in `xa_next_*`.

Worked example from the emulator (NAME_ENT prologue, frame 3836): request words `0xF`, `0x00011026`, `0x06000BC8`, `0x11000CBA` give start `0xBC8 * 8 + 6 + 0x11026 - 0x10 = 0x16E5C` and end `0xCBA * 8 + 6 + 0x11026 + 0x10 = 0x1760C`; both appeared in `audio_state`; channel 6, file 0x11 appeared in `xa_params`.

### 8.2 Playing

`audio_xa_play(sector)`: `audio_cd_audio_init` (mix off, serial volume 0, CD mode 0xC8), copy file and channel to `xa_params[0..1]`, set `AUDIO_REQUEST_XA_STARTING`, `cd_audio_command(0xD, sector, xa_params)` (h-cdio: XA start with filter, then positioning and read). The CD side reports "playing" in `cd_audio_state()` bit 4. `audio_xa_start_poll` waits for that bit; then it clears the starting bit and: if a next clip is already queued (`xa_next_start != 0`), stops and chains to it (`audio_xa_control(0x74)`, promote, play); otherwise, unless paused, sets `AUDIO_STATE_XA_PLAYING`, `xa_volume = xa_volume_full`, master volume 0x7F, serial (CD input) volume to `xa_volume` on both channels, mix on.

### 8.3 End

Each frame while playing, `audio_xa_check_end` compares `cd_audio_ticks()` (the sector the CD reports) with `xa_clip_end`: at or past the end it starts the fast fade (0xB4) once; otherwise it refreshes the level meter. The fade tick lowers `xa_volume` (section 6.3), and at 0 or less `audio_xa_fade_finish` stops the clip (`cd_audio_command(9)`, mix off, clear flags), then promotes the queued clip and, if there is one (`xa_clip_start != 0` after the promote), starts it. So a clip is cut about 11 frames after the CD reaches the end sector (which already includes the 0x10-sector margin), and a queued clip starts right after.

### 8.4 Stop, pause, resume (`audio_xa_control` and its callers)

`audio_xa_control(sel)` is the low-level routine. What a command does depends on which caller reaches it and on the call that follows.

- **Stop (selector 0x74).** Mix off, `cd_audio_command(9)`, clear playing, paused, starting and fade bits. The clip words stay. The commands 0x74 (class 0) and 0x1300 (class 3) follow it with `audio_xa_promote_next` (a queued clip becomes the current one without starting); `audio_xa_fade_finish` follows it with the promote and a start of the queued clip; forget (0x03) and reset (0x06) clear the words.
- **Pause (selector 0xE4).** Three cases. Start pending (`AUDIO_REQUEST_XA_STARTING`): set paused and `cd_audio_command(9)`, a CD stop at once. Playing: store the position in `xa_resume_position` (not before the clip start), clear playing, set paused and start the fast voice fade 0xB4 (this sets the fade-running bit; the later ticks lower the volume). Neither: nothing.
- **Resume (selector 0xF4).** Only if paused is set: clear it; with a clip queued call `audio_xa_fade_finish`, else `audio_xa_play(xa_resume_position)`.
- **Class-0 forms 0xE4 and 0xF4, the only forms the game sends.** `audio_control_dispatch` calls `audio_xa_control` and then **always** `audio_xa_promote_next`. The promote clears playing, paused, starting and fade-running bits, overwrites `xa_resume_position` and the clip words from the queue slot (empty unless a clip was queued), zeroes the level meter outputs and issues no CD command. Results:
  - 0xE4 with the start pending: the CD is stopped at once and the clip is forgotten (a queued clip becomes the current one, not started).
  - 0xE4 with the clip playing: the fast fade the pause just started is cancelled in the same call (the fade-running bit is cleared, so no tick ever lowers the volume), paused is cleared, the stored position is overwritten and **no stop is issued**. The CD stream keeps playing at the clip volume until another command stops it (a stop, a new clip, a reset) or the drive reaches the end of the data (not examined), while the driver has forgotten the clip: no end detection, no fade at the end, the level meter reads 0.
  - 0xE4 with nothing playing or starting: only the promote.
  - 0xF4: paused is already clear, so `audio_xa_control` does nothing; the promote alone forgets a playing clip exactly as in the second case. It never resumes anything.
- **Class-3 forms 0x2300 and 0x4300** call `audio_xa_control(0xE4 / 0xF4)` without the promote, so paused can survive. A pause during playback is then a fast fade whose end calls the stop selector (which clears paused again) and starts a queued clip; a resume restarts the CD from `xa_resume_position`, or finishes the chain when a clip is queued. This is read from the code only: nothing in `src/` sends these ids and no emulator run covered them.
- **Senders.** Voice pause occurs only as the class-0 0xE4: RPG_BAT (3 sites) and BUNKA_SD (1 site); nothing sends 0xF4. In each RPG_BAT site the `case` that sends 0x300 is followed by the `case` that sends 0xE4 (the step counter advances in between); in two of the three sites the case after that waits for `cd_audio_state() & 4`, the third counts a timer up to 0xB5. HYPOTHESIS: the 0xE4 arrives while the CD start is still pending (a seek and XA read take many frames), so it takes the first case: the clip is started and cancelled at once. No emulator run covered pause, so the branch taken is not observed. BUNKA_SD's site is in step 4 of its scene, once (the step then advances); the clip is started in step 2 (`func_80132000`), and step 3 (`func_80132148`) lies between, so the clip may be playing or already ended; its length was not examined.

### 8.5 Level meter

`audio_cd_level` (`D_80125E60`, type `CdLevelMeter`) is initialised by `SD_InitCDLevelInfo`: outputs 0, `precision_bits` 8, `sample_count` 0x20, `options` 0x212. `audio_xa_check_end` calls `SD_GetCDLevel` (in `scene_library.c`, unit m-dates-b) every frame while a clip plays: it reads 0x200 bytes of each CD-audio capture buffer (SPU RAM 0x0000 left, 0x0400 right) when no bank transfer is running and the clip is not paused, then with mode 0x212 takes the average of 32 absolute sample values per channel (spaced 8 samples apart), doubles it, and shifts right by `16 - precision_bits` to get 0 to 255. `getCDlevel` returns the mean of left and right while `AUDIO_STATE_XA_PLAYING`, else 0. Callers: the main HUD portrait code (`hud_panels.c`, `hud_screens.c`) compare it with three per-girl thresholds (`D_800B5A68[girl][0..2]`) and pick a mouth index 0 to 3 while `cd_audio_state() & 4`; NAME_ENT and GEKO read it directly. HYPOTHESIS: this is lip-sync of the portrait mouth from the voice loudness. For the port the equivalent is the short-term amplitude of the playing clip, 0 to 255.

## 9. Output mode, reverb, reset

- `audio_output_mode_set(1)` (mono): libsnd mono flag, `AUDIO_STATE_MONO`, CD mix matrix `{0x4F,0x4F,0x4F,0x4F}` (each channel to both), attenuation 0x18. Any other value (stereo): stereo flag, `AUDIO_STATE_STEREO`, matrix `{0x7F,0,0x7F,0}`, attenuation 0. Attenuation lowers `bgm_volume_full` (0x50 - att), `se_volume` and its default (0x5A - att) and, in `audio_bgm_start`, the VAB-based BGM volume. No code in `src/` sends ids 1 or 2; `audio_init` selects stereo.
- Reverb: `audio_init` sets type 3, depth 0x3C both channels and switches the reverb unit on; every BGM start sets depth from the pack table; `audio_voices_kill` switches reverb off, resets mode 3 and on.
- Reset: `audio_shutdown` stops and closes a running sequence, keys everything off, closes each VAB whose open bit is set (3 down to 0), then ends and quits libsnd. `audio_init` follows.

## 10. Calls into the SDK (not renamed here)

Identified from the argument shapes and the libsnd asm in `asm/libsnd.s` (internal labels such as `_snd_seq_tick_mode`, `_svm_vab_used`, `SpuVmSeKeyOn`); the address is where the driver calls it. These are SDK functions and belong to the SDK naming; they are listed so that the port knows what each action is.

| address | libsnd/libspu function (HYPOTHESIS by shape unless the asm names it) | used for |
| --- | --- | --- |
| 0x8008B8C4 | SsInit (asm calls `_SsInit`) | `audio_init` |
| 0x8008BAA4 | SsSetTickMode, argument 1 (the asm maps 0 and 1 to 60 ticks per second, 2 to 240 and 3 to 120; the PsyQ header names the value 1 `SS_TICK60`, HYPOTHESIS: the header is not in this repo) | `audio_init` |
| 0x80090DF0 | SsSetReservedVoice(0x18) (asm accepts 1 to 24) | `audio_init` |
| 0x8008B904 | SsSetTableSize(table, 1, 1) (asm stores `_snd_seq_s_max`/`_t_max`) | one sequence table |
| 0x80090E30 | sets a byte read by SpuVmFlush; likely SsSetAutoKeyOffMode(1) | `audio_init` |
| 0x8008FEF0, 0x8008FF5C, 0x8008FEB0, 0x8008FED0, 0x800949B0 | SsUtSetReverbType, SsUtSetReverbDepth (scales by 0x7FFF/0x7F in the asm), reverb on, reverb off, reverb mode/work area | reverb |
| 0x8008B750, 0x8008C720 | SsSetMVol(l, r), SsSeqSetVol(handle, l, r) | volumes |
| 0x8008BEE0, 0x8008BFB0 | SsSetSerialAttr(0, 0, on/off), SsSetSerialVol(0, l, r) | CD-audio mix and volume |
| 0x80087DC8 | CD mix matrix (CdMix, via `CD_vol`) | stereo/mono |
| 0x80090E40, 0x80090E50 | SsSetMono, SsSetStereo (store 1 / 0 in a halfword) | output mode |
| 0x8008C1B0, 0x8008C620, SsVabTransCompleted (named) | SsVabOpenHeadSticky(header, slot, spu), SsVabTransBody(body, slot), wait | `audio_bank_transfer` |
| 0x80090E60 | SsVabClose(slot) | `audio_bank_dispose`, `audio_shutdown` |
| 0x80090270 | SsUtGetVabHdr(slot, header buffer); byte 0x18 = master volume | BGM volume |
| 0x800900A0 | SsSeqOpen(address, slot) (the asm holds the library's "can't open sequence" string) | `audio_bgm_start` |
| 0x80090350, 0x80090580, 0x800907E0, 0x80090AB0, SsSeqReplay (named) | SsSeqPlay(handle, 1, 1), SsSeqStop, SsSeqClose, SsSeqPause, SsSeqReplay | BGM control |
| 0x80090D60, 0x80090D20 | key on / key off of one effect note (asm calls `SpuVmSeKeyOn` / `SpuVmSeKeyOff`) | `audio_se_play`, `audio_se_stop` |
| 0x8008FD68 | SsUtAllKeyOff(0) | stops |
| 0x80090DB0 | guarded SpuVmFlush | `audio_voices_kill` |
| 0x8008BE34, 0x8008BE54, 0x8008BEBC | SsStart2, SsEnd, SsQuit | init/shutdown |
| 0x80094B80, 0x80094C60, 0x80094C90 | SpuIsTransferCompleted, SpuSetTransferStartAddr, SpuRead | level meter (`scene_library.c`) |

## 11. How it connects to the rest of the game

- **Main loop** (`func_80042134`): `audio_init` at boot and after a reset; the pump after rendering while `sound_active`.
- **Bank loader** (`frame_render_audio.c`, h-graphics): sends the transfer (0x11 to 0x16) and assignment (0x21 to 0x24, 0x2E, 0x2F) commands, `0x200 | n` to start a sequence and `0x1200` to stop it; `audio_bank_transfer_done` is its completion test.
- **CD layer** (h-cdio): `cd_audio_command` ids 9 (stop), 0xD (XA start), 0xE (set mode); `cd_audio_state`, `cd_audio_ticks`, `cd_audio_request_get`, `cd_load_destination_base_get`; `cd_read_sectors_nowait` sends 0x06 first.
- **Dialogue** (`scene_library.c`: the talk runner `func_80082764` (core: `talk_run`) and `func_80081128`): when a line is voiced and the player presses the skip buttons it sends 0xB4, which fades the clip out in about 11 frames, and ends the line (the runner tests `pad_pressed & 0x400040` and `voice_mode` (`GameState.unk_03C`) equal to 2; `func_80081128` tests `pad_pressed & 0x600060` once its counter `D_800CA2AC` exceeds 0x200). `talk_voice_play` (`func_80052060`) sends 0x300 when `voice_mode != 0`.
- **Portrait mouths and NAME_ENT/GEKO mouths**: `getCDlevel` (section 8.5).
- **Scenes**: 1,180 `audio_command` sites in main and 24 overlays: BGM via the bank loader, effects directly, fades on scene exits (the 0xBF/0xCF family is the standard scene-exit fade of the festival, MASTER, DATE and GEKO scenes).
- **OPTION sound test** (mode 0x21, `801389A0.c`): effect counter 0 to 0x20 sends `0x500 | n`; BGM counter 0 to 16 with two buttons sending `0x200 | n` for pack set 0 or 1, a counter above 16 sends `0x1200 | n` (stop); a voice page sets a per-girl clip request (`cd_audio_request_set` with a per-girl table) and sends 0x300; one button calls `audio_reverb_off_toggle`.
- **TT**: calls the pump itself, uses 0x04, 0xE1/0xF1 and its own banks; the driver skips the voice kill at BGM start in TT (mode 0xD0 or 0xD1) and TACO (0x90), and `audio_voices_kill` itself does nothing in TT while a BGM plays.
- **Reset paths**: command 0x7F, `audio_shutdown`, `audio_init` (soft reset and return to title).

Cross-unit requests (not in this table): `func_80079524`, `func_80079554`, `func_80079578` in `overlay_loader.c` (m-flow) test the main mode for TT (0xD0/0xD1), TACO (0x90) and the OPTION sound test (0x21); this spec relies on those meanings. Suggested names `mode_is_tt`, `mode_is_taco`, `mode_is_sound_test`.

## 12. Port notes

- **Replace**: libsnd, the SPU and CD-XA playback. **Keep**: the command ids (the contract) and their effects, the timing of fades in frames at 60 Hz, the one-BGM / one-voice-plus-one-queued model, the bank set discipline (the port can load whole sets), per-speaker voice loudness, the level meter output.
- **Identity of content**. A BGM id is not global: `0x200 | n` indexes the current pack set, and the pack comes with the scene's bank load. An effect id means (bank slot of its class, program, note) and the slot's content comes from the scene's bank load. A voice clip is identified by (speaker, start word, end word) from the request. The port needs a table from (scene bank-load arguments) to packs and banks, which lives with the loader (h-graphics) and the callers.
- **Quirks to decide on**: (a) a command word of 0 ends the drain and the rest of the queue is lost; (b) more than 32 commands in a frame are dropped; (c) `audio_bgm_bank_ready` ignores its argument; (d) a new BGM kills sounding effects, except in TT, TACO or when the BGM bank is slot 2; (e) the voice pause that the game sends (class-0 0xE4) is a cancel, not a pause: with the CD start pending it stops the CD and forgets the clip, during playback it only forgets the clip (cancelled fade, no stop, level meter 0, CD stream unstopped), and 0xF4 never resumes; the port must decide whether to copy this or to cancel the clip outright (section 8.4); (f) ids 0xB2/0xC2 do nothing, 0x01/0x02 are never sent; (g) the same BGM fade id re-sent every frame must not restart or alter a running fade, while a voice fade takes the newest rate; (h) `getCDlevel` is 0 when not playing, so a mouth index is 0 outside voice.
- **Not needed**: the SPU layout, header RAM addresses, body sizes, `audio_bank_body_size`, the libsnd table buffer.

## 13. Emulator evidence

Cold boots with `tools/emu/run.sh break func_80079B10` (every command), `watch 0x80125D10:8` (flag writes), `break func_8007A924` and `func_8007A6AC` with a memory dump. Observed in the first 5100 frames (a fifth run, `break func_80090D60`, caught the key-on at frame 4821):

| frame | event |
| ---: | --- |
| 1256 to 1322 | command 0x06 before CD reads; 0x7F stops at 1322 |
| 1462 to 1464 | 0x12 (slots 2, 3): request bits 0x04000000, 0x0C000000, running 0x40000000, state 0x00040010 then 0x000C0010; 0x23, 0x24, 0x2E sent twice, then 0x200: state 0x00040110 (BGM playing, slot 2 open) while slot 3 was still being requested |
| 3533 to 3596 | 0x7F, then 0x23/0x24/0x2E/0x200 again for the NAME_ENT bank |
| 3836 | 0x300 for the prologue clip: `audio_command_xa` entry; `audio_xa_play` at frame 3837 with `xa_clip_start` 0x16E5C, `xa_clip_end` 0x1760C, `xa_volume_full` 105, `bgm_volume` 0x66, `bgm_slot` 2, `bgm_auxiliary_slot` 1, `se_slot_a` 3, `se_slot_b` 0, `sequence_pack_set` 1, `cd_mode_param[0]` 0xC8, `xa_params` [4] 6 and [6] 0x11 |
| 4401, 4641 | 0x71 and 0x7F entering the title |
| 4820 to 4821 | 0x501 (button press on the title menu): `audio_command_se_a` calls `audio_se_play`, which keys on with bank word 0x0300 (slot 3, program 0), note word 0x1C00 (note index 1), volume 0x5A on both channels |
| 4854 to 4911 | 0xD1 every frame |

No emulator run covered pause, chained clips, SE ids other than 0x501, mono or the sound test. The pause and resume behaviour in section 8.4 is read from the matched C alone.

## 14. Function map

Addresses are in `src/main/sound_driver.c` (proposed file name `src/main/sound_driver.c`). "O.BIN" is the developer name from the alignment table (confidence med/low; only `SD_InitCDLevelInfo` is applied in the repo, and the code reading agrees with every one of these names).

| address | name | O.BIN | role |
| --- | --- | --- | --- |
| 80079B10 | `audio_command_submit` | SD_Call_SD | push a command |
| 80079B34 | `audio_command_pump` | SD_Task | per-frame drain and dispatch |
| 80079C70 | `audio_bank_transfer_schedule` | SD_HandleVBLVAB | one bank transfer per call |
| 80079D28 | `audio_bank_transfer` | SD_TransVAB | open header, upload body |
| 80079E00 | `audio_bgm_bank_ready` | SD_ISVabAvailable | bank ready test |
| 80079E4C | `audio_xa_frame_update` | SD_XATask | voice watch |
| 80079E9C | `audio_xa_check_end` | SD_CheckXASector | end detection, level meter |
| 80079F00 | `audio_xa_start_poll` | SD_IsXAReady | clip start completion |
| 8007A008 | `audio_pending_update` | SD_HandleVBL | per-frame pending work |
| 8007A080 | `audio_control_dispatch` | SD_BranchCTRL | class 0 |
| 8007A254 | `audio_command_bank_transfer` | SD_VABTransfer | ids 0x11 to 0x16 |
| 8007A354 | `audio_command_bank_assign` | SD_BufferUpdate | ids 0x21 to 0x24, 0x2E, 0x2F |
| 8007A43C | `audio_command_system` | SD_HandleSys | ids 1 to 6 |
| 8007A4DC | `audio_xa_fade_update` | SD_HandleVBLXA | voice fade gate |
| 8007A50C | `audio_xa_fade_tick` | SD_HandleXAFadeOut | voice fade step |
| 8007A5BC | `audio_xa_fade_finish` | SD_FinishFadeOutXA | stop and chain |
| 8007A618 | `audio_speaker_volume_set` | none | per-speaker loudness |
| 8007A6AC | `audio_command_xa` | SD_XACall | class 3 |
| 8007A868 | `audio_xa_promote_next` | SD_XAParamUpdate | queued clip becomes current |
| 8007A924 | `audio_xa_play` | SD_PlayXA | start the CD stream |
| 8007A98C | `audio_xa_control` | SD_StopXA | stop, pause, resume |
| 8007AB24 | `audio_xa_fade_start` | SD_FadeXA | start voice fade |
| 8007ABE0 | `audio_bgm_update` | SD_HandleVBLSEQ | BGM tick |
| 8007AD6C | `audio_bgm_fade_finish` | SD_FinishFadeOutSEQ | fade end |
| 8007ADD8 | `audio_command_bgm` | SD_SEQCall | class 2 |
| 8007AEC0 | `audio_bgm_request_start` | SD_PlaySeq | start or wait |
| 8007AF0C | `audio_bgm_start` | SD_SeqGo | open and play |
| 8007B144 | `audio_bgm_control` | SD_StopSeq | stop, pause, resume |
| 8007B2B4 | `audio_bgm_fade_start` | SD_FadeSeq | start BGM fade |
| 8007B358 / B3DC / B460 / B4E4 | `audio_command_se_a` / `_b` / `_bgm_bank` / `_bgm_aux` | SD_SECall / OtherSECall / SECallInSEQ / OtherSECallInSEQ | classes 5 to 8 |
| 8007B568 / B5CC | `audio_se_play` / `audio_se_stop` | SD_PlaySE / StopSE | key on / off |
| 8007B5EC / B640 / B64C | `audio_queue_push` / `_clear` / `_pop` | SD_AddEvt / FlashEvent / GetNextEvent | queue |
| 8007B6E0 | `audio_xa_forget` | none | command 3 |
| 8007B734 | `audio_xa_reset` | SD_KillXA | command 6 |
| 8007B7E0 | `audio_voices_kill` | SD_KillSPU | command 4 |
| 8007B844 | `audio_init` | SD_Init | boot |
| 8007B8F8 | `audio_bank_layout_init` | SD_SetVabAdrs | fixed layout |
| 8007B99C | `audio_bank_body_address_set` | SD_GetVabAdrs | body addresses |
| 8007BA54 | `audio_cd_audio_init` | SD_InitCD | CD mute and mode |
| 8007BAAC | `audio_state_init` | SD_InitStruct | defaults |
| 8007BBE8 | `audio_sequence_pack_index` | SD_SetSeqAdrs | pack table |
| 8007BCFC | `audio_output_mode_set` | SD_ChnChange | mono/stereo |
| 8007BDE8 | `audio_shutdown` | SD_Reset | teardown |
| 8007BE94 | `audio_bank_request_transfer` | SD_GetVAB | request a slot |
| 8007BF04 | `audio_bank_dispose` | SD_DisposeVAB | free a slot |
| 8007BFA8 | `audio_bgm_next_clear` | none | cancel queued BGM |
| 8007BFB8 | `audio_reverb_off_toggle` | none | sound test flag |
| 8007BFF8 | `SD_InitCDLevelInfo` (kept) | applied | level meter defaults |

## 15. Open questions

1. The unit label (section 0): the `normal_date_*`, `read_bustup*`, `event_face*`, `set_tarao_*`, dialogue and `select_girl*` functions are in `scene_library.c`. Unit `m-dates-b`'s focus line lists only `select_girl*`, `change_girl`, `change_yoshio`, `day_plus`, `check_para_limit` and the SD sprite setup (SD here is super-deformed sprites, not the sound driver), so nothing in a focus list names the `normal_date_*` scene steps, the portrait loads or the event faces. They need an owner.
2. Prefix and O.BIN policy: the table uses `audio_*` (core) with the developer's `SD_*` name in the note; only `SD_InitCDLevelInfo` (applied) keeps its O.BIN name. Whether to promote the other 49 `SD_*` names verbatim is a decision for the owner of `symbol_addrs_obin.txt`; the code reading agrees with all of them. `h-graphics` now uses the same `audio_` names as the applied core and this table; `func_80044750` remains `audio_command`. The unresolved O.BIN decision is separate: unapplied `SD_*` names are quoted as notes only, and only the applied `SD_InitCDLevelInfo` is a comment row.
3. `CdLevelWork` (`CdLevelMeter`) is shared with `scene_library.c`; the m-dates-b table repeats these rows with the same names (review round 1 of that unit).
4. What do the 16 notes of program 0 in each bank sound like, and which scene loads which bank into which slot? Needs the bank loader arguments (h-graphics) joined to the disc; a port data task, not a code question.
5. HYPOTHESIS items to confirm with the disc data: that the speaker index is the girl index (the volume table), that sequences loop through events inside the SEQ data (repeat count 1 is passed), the meanings of ids 0x501 (menu sound?) and 0x50B/0x50C (telephone?).
6. Which sound-effect classes 6 and 8 serve outside TAIIKU (class 8 is TAIIKU only, class 7 two sites).
7. `func_80090E30(1)` (flag byte read by the SPU flush): likely manual key-off mode; matters only if the port models key-off timing.
8. Voice pause (section 8.4): confirm the branch RPG_BAT takes with a short emulator run (`tools/emu/run.sh break func_8007A98C` from a save state at the RPG_BAT step that sends 0x300): start pending or playing, and whether the CD stream keeps running after 0xE4 (`cd_last_command()` stays at the XA read instead of 9). `h-cdio` section 2.4 says the sound driver queues the pauses that end a clip at `func_8007A98C` cases 0x74 and 0xE4; per this spec the 0xE4 pause is queued only in the start-pending branch.
