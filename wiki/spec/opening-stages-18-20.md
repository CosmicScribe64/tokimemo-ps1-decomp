---
type: spec
updated: 2026-10-29
sources: ["src/ovl/name_entry/prologue.c", "src/main/actor_sprites.c", "src/main/background_images.c", "src/main/gpu_primitives.c", "src/main/gpu_effects.c", "src/main/cd_queue.c", "src/main/sound_driver.c", "src/main/game_mode.c", "src/main/frame_render_audio.c", "include/game/display_records.h", "asm/ovl/NAME_ENT/data/name_entry/prologue.data.s", "config/symbols_NAME_ENT.txt", "disc/track1.bin (ISO directory, read-only)", "disc/files/CDROM/PROLODIR/O_13.NBN", "disc/files/CDROM/PROLODIR/O_01.NBN", ".asset-cache/port/opening-voice-v2/opening-audio.json"]
---

# Opening stages 18-20 (lines 38-42, flash, closing): porting spec

Ticket: [[tickets/T-39017-opening-stage-specs]]. This page covers mode 0x15 stages 18 `prologue_mode_stage_lines_38_to_41`, 19 `prologue_mode_stage_flash` and 20 `prologue_mode_stage_closing` (dispatcher `prologue_mode_handler`, `src/ovl/name_entry/prologue.c:2743-2821`). Stage 21 and the START skip are in [[spec/opening-stages-21-end]]. The line player and stage overview are in [[spec/o-nameent]] sections 5.1-5.2 and 8.

Line numbers refer to the current checkout. Disc LBAs come from the ISO 9660 directory of `disc/track1.bin`, read in Docker (`tokimemo-recovery`, read-only mount, container `--rm`). The CDROM directory sector 14515 is read at +16, as `wiki/disc-layout.md` describes. Sprite-table values come from `disc/files/CDROM/PROLODIR/O_13.NBN`, read the same way. Game text is described by role, not quoted.

## 0. Common frame shape

- Each stage wrapper runs its `_run` only when `mode_step == 0` (`prologue.c:2612-2616`, `2680-2684`, `2708-2712`). Each `_run` ends with `dec_bg_draw(1)` on every frame, whichever substep runs.
- Each frame then runs the dispatcher tail: `message_window_title_update`, `hud_screen_frame_draw(2)`, `message_window_show`, then the START test (`mode_stage_set(0x15)`). The order and RNG note are in [[spec/opening-stages-21-end]] section 1.
- `mode_stage_next` (`src/main/game_mode.c:156-167`) increments `mode_stage` and zeroes `mode_step`, `mode_stage_timer`, `mode_step_timer`, `mode_stage_scratch` and `substep`. The new stage's substep 0 runs on the **next** frame.
- `actor_draw_all` runs from `draw_frame` (`src/main/frame_render_audio.c:61-62`) while `actor_draw_enabled == 1`. It draws each visible actor and then steps its sequence. Whether `draw_frame` runs before or after the mode handler in the main loop was not checked here (open question 1).
- `dec_bg_draw(1)` (`src/main/background_images.c:187-250`) adds two SPRTs on OT tag 0xD (`draw_ot_tags + 0x34`): 192x160 at (-160,-120) and 128x160 at (+32,-120), tinted by `bg_brightness`. If `dec_bg_show[1] == 0` it uses VRAM tpages (0x2C0,0)/(0x380,0); if it is 1, it uses (0x140,0x100)/(0x200,0x100).
- **RNG:** stages 18-20 make no `rand()` or `srand()` calls, including the line player (`prologue.c:84-179`). The only possible draw is `hud_screen_frame_draw`, and it draws only when the health state is 4, which is not true on a new game ([[spec/opening-stages-21-end]]).
- **Lip sync:** none. Stages 18-20 call no `prologue_lip_synchronization_*`. Actors 96-159 were cleared at the end of stage 16 (`prologue.c:2513`), and nothing sets an actor until stage 19 substep 0.

## 1. State on entry to stage 18 (left by stages 16-17)

These are preconditions. Stages 18-20 never re-establish them, so a port must carry them over from the real stages 16-17. If they are missing, the port must reject the stage, never synthesise the state.

| item | value | source |
|---|---|---|
| actors 96-159 | cleared (`actor_clear_upper`) | `prologue.c:2513` (stage 16 substep 11) |
| `bg_layers` word 0 | `0x80000000` (cut layer 0 off; its `tpage_buf` locks stay) | `prologue.c:2514` |
| background slot 1 | P_01_12 decoded into the hidden buffer and shown by `dec_bg_show_set(1, dec_bg_decode_to_vram(1))` | `prologue.c:2540-2549` |
| `bg_lba_cache[3 - dec_bg_show[1]]` | 0x4973 (written when the read was issued; `D_800B5939` = `dec_bg_show[1]`) | `background_images.c:60-71` |
| O_13 read | `cd_read_sectors(0x41, 0x80180000, 0xAC50)` issued in the same frame as the P_01_12 decode. **No sync wait** follows it: stage 17 calls `mode_stage_next()` at once | `prologue.c:2550-2551` |
| `prologue_line` | 0x25 (37) | `prologue.c:2532` |
| `tpage_buf` | unchanged since stage 0 apart from the background locks (see section 4) | |

`cd_read_sectors` (`src/main/cd_queue.c:513-522`) sends `audio_command(6)` and queues CD command 6 with packed (count, destination). Nothing in stages 18-19 polls `cd_read_sync_status` before stage 19 uses the O_13 tables at 0x80180204. The only ordering is the stage 18 substep 1 wait for XA playback (bit 4). HYPOTHESIS: the CD queue serialises the data read before the XA play request from line 38, so the XA-started bit implies that the O_13 read has finished. A port must model this ordering explicitly. It must reject stage 19 substep 0's actor setup when the O_13 transfer has not completed, and must not assume the data is present.

## 2. Stage 18: `prologue_lines_38_to_41_run` (`prologue.c:2565-2610`)

| substep | lines | action | advance |
|---:|---|---|---|
| 0 | 2567-2571 | `prologue_line = 0x26` (38); `prologue_line_start()` | `substep += 1` same frame |
| 1 | 2572-2576 | `if (cd_audio_state() & 4) substep += 1`. **No `prologue_line_run`**, so the typewriter does not advance while it waits | when XA playback is reported |
| 2 | 2577-2579 | `prologue_line_run()` | `line_run` increments substep when `mode_step_timer >= 0x3D` |
| 3 | 2580-2584 | `prologue_line = 0x27` (39); `prologue_line_start()` | +1 |
| 4 | 2585-2587 | `prologue_line_run()` | timer |
| 5 | 2588-2592 | `prologue_line = 0x28` (40); `prologue_line_start()` | +1 |
| 6 | 2593-2595 | `prologue_line_run()` | timer |
| 7 | 2596-2600 | `prologue_line = 0x29` (41); `prologue_line_start()` | +1 |
| 8 | 2601-2603 | `prologue_line_run()` | timer |
| 9 | 2604-2607 | `prologue_scene_counter = 0`; `mode_stage_next()` | stage 19 next frame |

No CD reads, memcpy, actor, tpage or palette changes and no `audio_command` calls in stage 18 itself. The only audio requests are the ones `prologue_line_start` makes.

### 2.1 Lines (tables at `prologue_line_texts` 0x8014D958, `prologue_line_speakers` 0x8014DA04, `prologue_line_clips` 0x8014DA30, `prologue_line_clips_second` 0x8014DB88; `config/symbols_NAME_ENT.txt:425-428`)

| line | text label (address) | speaker byte | plate (`prologue_line_start`, `prologue.c:84-104`) | clip1 (=clip2) words | XA request (opening-voice-v2) |
|---:|---|---|---|---|---|
| 38 | `prologue_line_text_38` (0x8014D86C) | 0x0D Yoshio | `girl_short_name_table[0x0D]` | 0x13000046, 0x0600005F (`prologue.data.s`, 0x8014DB60) | `CDROM/XA_G/YOSHIO.XA` (LBA 0x435AE = 275886 = `cd_audio_speaker_offset_table[13]`): absolute 278110-278942, file-relative 2224-3056; filter bytes 0x13 (start) / 0x06 (end); 25 audio sectors selected |
| 39 | `_39` (0x8014D8B8) | 0x0F player | `gnsx(player_family_name)` | 0,0 | none (speaker low nibble 0xF) |
| 40 | `_40` (0x8014D8D8) | 0x0D Yoshio | Yoshio short name | 0x180000AB, 0x130000B7 (0x8014DB70) | YOSHIO.XA absolute 281342-281758, relative 5456-5872; filter bytes 0x18 / 0x13; 12 audio sectors selected |
| 41 | `_41` (0x8014D908) | 0x0F player | family name | 0,0 | none |
| 42 (stage 20) | `_42` (0x8014D928) | 0x0F player (narration under the player plate) | family name | 0,0 | none |

The speaker bytes 0x8014DA2A-0x8014DA2E are 0D 0F 0D 0F 0F, and 0x8014DA2F (0x00) is the padding entry. The `prologue_line_clips_second` rows equal `prologue_line_clips` for lines 38-42, so `prologue_line_run` never requests a second clip (`prologue.c:157-163`). The XA window follows `audio_command_xa` (`src/main/sound_driver.c:383-421`): `(low24 * 0x20) + offset ∓ 0x10`, because the speaker is not 0xF; the term `D_80125D5D` is 0 on a fresh boot. The exporter is `tools/port/opening_voice_content.py:40-51`. The clip packs already exist in `.asset-cache/port/opening-voice-v2` (line 38 pack `clip-20c19c22…`, line 40 pack `clip-0e361795…`).

`prologue_line_start` for a voiced line: `k_sub_reset`, plate, `sndisp(text, 0, speaker)`, `cd_audio_request_set(clip1[0], clip1[1], speaker)`, `audio_command(0x300)`, `current_girl = speaker`, `prologue_voice_started = 0`, `k_sub_disp_start(1)`. For an unvoiced line the two audio calls are skipped. Completion rules (timer 0x3D, voice bits 4 and 2) are in [[spec/o-nameent]] section 5.1. The text speed is still the stage 0 value, `TEXT_SPEED_EVERY_SIX_FRAMES` (`prologue.c:208`).

Timing consequence: after `line_run` raises the substep, one frame is spent on the next `line_start` substep. Substep 9 costs one further frame before stage 19.

## 3. Stage 19: `prologue_flash_run` (`prologue.c:2618-2678`)

`prologue_scene_counter` enters at 0 (stage 18 substep 9). Helpers:

- `prologue_flash_boxes_draw` (`prologue.c:1257-1270`): colour `counter * 0x10101` if `counter < 0x100`, else 0xFFFFFF. It draws two flat (`mode & 0x7F == 1`) semi-transparent (`mode & 0x80`) `POLY_G4`s, `prim_gradient_rect(-0xA0,-0x78,0xA0,0xA0, 0xB, c, 0x81)` and `(0,-0x78,0xA0,0xA0, 0xB, c, 0x81)`, which cover x -160..160 and y -120..40. It then calls `dtd_on_tpage(0,0,0xB,1,1)`, a draw-mode prim on tag 0xB with tpage (x0,y0, abr 1 = additive, tp 1) (`src/main/gpu_primitives.c:94-100,171-219`).
- `prologue_white_boxes_draw` (`prologue.c:1272-1275`): the same two rects on tag **0xD** (the background tag), opaque (mode 1), colour 0xFFFFFF.

| substep | lines | draws (before the counter update) | counter | exit |
|---:|---|---|---|---|
| 0 | 2625-2645 | flash boxes with values 0,4,…,252 | `+= 4` | when `>= 0x100` (after 64 frames): actor 96 setup (below), `substep += 1` |
| 1 | 2646-2654 | white boxes, then flash boxes with values 256(white),252,…,4 | `-= 4` | when `< 3` (it is 0 after 64 frames) |
| 2 | 2655-2664 | white boxes, then flash boxes with values 0,…,252 | `+= 4` | when `>= 0x100` (64 frames): `D_80120653 = 0` (actor 96 `draw_flags = 0`, hidden), `substep += 1` |
| 3 | 2665-2672 | flash boxes only, with values 256,252,…,4 (P_01_12 visible again under the fading additive) | `-= 4` | when `< 3` (64 frames) |
| 4 | 2673-2676 | nothing but `dec_bg_draw(1)` | — | `mode_stage_next()` |

The stage takes 257 frames. The counter is not clamped, but it reaches exactly 0x100 and 0, so there is no overshoot. Stage 19 makes no CD reads, memcpy, audio or text calls.

### 3.1 Actor 96 setup (substep 0 exit, `prologue.c:2629-2643`)

Actor 96 is at `actor_pool + 96*0x44 = 0x80120650` (`actor_pool` 0x8011ECD0; struct `include/game/display_records.h:17-54`). First comes `actor_clear(0x60)` (`src/main/actor_sprites.c:672-`), which sets the defaults: `blink_period` 1, `brightness` 0x80, scale 0x1000, frame 0, tick 0, `unk_43` 0. Then these fields are set:

| symbol | offset/field | value |
|---|---|---|
| `D_80120651` | +0x01 `unk_01` (OT layer + 1) | 0x0C, so the actor is drawn on OT tag 0xB, the same tag as the flash boxes |
| `D_80120652` | +0x02 `flags` | 5 (bit0 sequence stepping; bit2 at the end of the sequence clears bits 0 and 2 and goes back one step, so the last frame holds) |
| `D_80120688` | +0x38 `quad_flags` | 0x41000000 (`& 0x03000000 = 0x01000000`, so `search_tpage(...,1)` loads 8-bit texture pages) |
| `D_80120653` | +0x03 `draw_flags` | 0xA4 (0x80 visible, 0x20 tint by `draw_brightness_get()`, 0x04 subtract origin) |
| `D_8012065C` | +0x0C `frame_table` | 0x80180204 (O_13 +0x204, 7 frame pointers) |
| `D_80120660` | +0x10 `sequence_table` | 0x80180220 (O_13 +0x220; entry 0 → 0x801801C4) |
| `D_80120684` | +0x34 `tpage_table` | 0x801801E4 (O_13 +0x1E4: 8 pointers 0x80180230 + k*0x4000, k=0..7) |
| `D_80120666` | +0x16 `sequence_slot` | 0 |
| `D_80120656` | +0x06 `blink_period` | 2 (no 0x80 bit, so the tick advances when `frame_count % 2 == 0`) |
| `D_80120655` | +0x05 `unk_05` | 0x80 (CLUT row base 0x1F0, since `unk_43 == 0`) |
| `D_80120676` | +0x26 `x` | -0xA0 |
| `D_8012067A` | +0x2A `y` | -0x78 |

Compare stage 11 (`prologue.c:1791-1820`), which builds the same O_13 actor from a 0x41-sector read to 0x801D8000 plus a `memcpy(0x80180000, 0x801D8000, 0x20800)`. Stage 11 uses `unk_01 = 0x0A`, keeps the default `blink_period` 1 and has no `unk_06 = 2`. Stage 19 differs in exactly those two fields.

**O_13 sequence 0** (0x801801C4, halfwords): count 7, then (frame, duration) pairs (0,6) (1,5) (2,5) (3,4) (4,7) (5,7) (6,0x50). A step advances when `tick >= duration` (`actor_sprites.c:311-339`; tick at `:324`). At count 7, flag bit 2 clears bits 0 and 2 and decrements the frame, so frame 6 holds (`:340-346`). With `blink_period` 2, one tick costs 2 frames, and which frames tick depends on `frame_count` parity. The actor is visible for about 128 frames: from substep 0's exit through substep 2's exit, where `draw_flags = 0`. Frame 6 is reached after 34 ticks (about 68 frames), so the whole sequence plays inside the flash.

**O_13 frames** (count, then 14-byte parts at +8; page = `part.unk_04 >> 6`; parts drawn from `count-1` down to 0). The CLUT bits are `unk_06 = 0x1B00` in every part, so the CLUT row is `0x1F0 + 0xB = 0x1FB`:

| frame | ptr | parts | pages in first-use draw order |
|---:|---|---:|---|
| 0 | 0x80180000 | 4 | 0, 2 |
| 1 | 0x80180040 | 4 | 2, 1 |
| 2 | 0x80180080 | 3 | 3, 2 |
| 3 | 0x801800B4 | 5 | 1, 4 |
| 4 | 0x80180104 | 4 | 2, 5 |
| 5 | 0x80180144 | 4 | 2, 6 |
| 6 | 0x80180184 | 4 | 2, 7 |

**CLUT row 0x1FB** (load index 0x1B) is not loaded in stages 17-19. The only prologue load of index 0x1B is stage 0's `load_palette(0x80184490, i+16, 1, 2, i)` loop with i = 11, from O_01 data (`prologue.c:258-260`). Other prologue `load_palette` calls write indices 0xD, 0xE and 0xF only (`prologue.c:262,610,612,1254`). The port must therefore keep the stage-0 row 27 result (the arrival/`actor_palette_setup` path). It must not reload a palette, and it must reject the stage if that row is absent. That rows map to VRAM y = 0x1E0 + index is inferred from `prologue_background_slot_set`, which pairs `load_palette(...,0xF,...)` with CLUT `0xF + 0x1E0` (`prologue.c:601-615`).

## 4. Texture-page slots (search_tpage free-order model)

`actor_draw_all` calls `search_tpage(0x7FFF, …)` once per frame. This marks every `tpage_buf == -1` slot free (0x7FFF) and every other slot locked (0x7FFE) (`actor_sprites.c:509-524`). For each part, `search_tpage(part.unk_04, i, 1)` goes through three steps (`:525-555`):

1. It reuses a locked slot whose `tpage_buf` equals the page pointer, scanning i 15..5 and rows 3..0.
2. Otherwise it reuses a slot assigned earlier in this frame.
3. Otherwise it takes the first free slot in ascending order and queues `load_csr_tp(ptr, slot, 1, …)`. Every frame re-uploads, because the free/assigned state is rebuilt per frame.

Between stage 10 and stage 19 nothing calls `tpage_buf_clear`, `load_tpage_buf_lock` or `search_load_tpage_buf_lock`. `prologue_background_slot_set` re-locks the same cut slots 8-10/40-42 (`prim_bg_slot_setup` tp = 8, `src/main/gpu_effects.c:51-95`). The free set is therefore the one in `port/godot/services/first_arrival_board_store.gd` `STAGE10_FREE_TPAGE_SLOTS = [5, 6, 7, 37, 38, 39, 58]`, assuming no actor 0-95 draws in this frame (open question 2). The locked values do not collide with O_13 pointers:

- slot 26 holds O_01's `tpage_table[0]` = 0x801800D0 (O_01.NBN word at +0xA8);
- the cut slots hold 0x801A4000 + k*0x4000;
- the system-default words are main-data values (not re-read here).

Expected per-frame mapping for actor 96 in stage 19, when it is the only drawn actor: the first page in the order of section 3.1 goes to **slot 5**, the second to **slot 6**. For example, frame 0 maps page 0 to slot 5 and page 2 to slot 6, and frame 1 maps page 2 to slot 5 and page 1 to slot 6. A port should reuse `o11_tpage_slots`-style logic, parameterised by the O_13 layout (frame table +0x204, 7 frames, page table +0x1E4, 8 pages, file size 0x20800) rather than the O11 constants.

Background slot 1 is unchanged through stages 18-20: there are no `dec_bg_cd_read`, `dec_bg_decode_to_vram` or `dec_bg_show_set` calls. `bg_layers` stays 0x80000000.

## 5. Stage 20: `prologue_closing_run` (`prologue.c:2686-2706`)

| substep | lines | action |
|---:|---|---|
| 0 | 2688-2693 | `prologue_line = 0x2A` (42); **`prologue_scene_counter = 0` before** `prologue_line_start()`; `substep += 1` |
| 1 | 2694-2696 | `prologue_line_run()` (unvoiced: the timer runs once the typewriter completes; `>= 0x3D` raises the substep) |
| 2 | 2697-2703 | `if (prologue_scene_counter++ >= 0x3D)`: **post-increment**, so the test fires on the 62nd frame (old value 0x3D, new 0x3E). It then sends `audio_command(5)` (sets the master-volume fade flag), `audio_command(0xD1)` (slow BGM fade, about 205 frames; [[spec/m-dates-a]] sections 6.3-6.4) and calls `mode_stage_next()` |

The BGM being faded is the bank 0 variant 1 started by `audio_bank_start(0, 1)` in stage 16 substep 4 (`prologue.c:2476`). Stage 21 sends 5/0xD1 again on its first frame. A running fade ignores later fade ids ([[spec/m-dates-a]] section 6.3). Stage 20 makes no CD, actor, tpage or RNG calls. Actor 96 still exists, with `draw_flags = 0` (hidden) and its sequence stepping only while drawn.

## 6. Port status (checked 2026-10-29)

`port/godot/flow/prologue_arrival.gd` has `_stage_18_lines_38_to_41`, `_stage_19_flash` and `_stage_20_closing` (lines 1387-1451). They diverge from the source as follows:

| # | where | divergence | required change |
|---:|---|---|---|
| 1 | stage 18 (`:1387-1406`) | Substep numbering is line pairs at 0/1, 2/3, 4/5, 6/7. Source substep 1 (the `cd_audio_state() & 4` wait, `prologue.c:2572-2576`) is missing, so line 38's typewriter starts before the voice and every later substep index is shifted by one | Add the wait substep through `services["cd_audio_state"]`, rejecting a non-int result as stages 8-10 do (`:965-968`). Use the source numbering 0-9 |
| 2 | stage 18 | The `counter = 0` and `next_stage()` of source substep 9 are folded into line 41's `line_run` frame (`if state.substep > 7`), so the stage ends one frame early | Use a separate substep 9 frame |
| 3 | stage 19 (`:1409-1435`) | Substep 3 subtracts 2 (128 frames); the source subtracts 4 (64 frames). There is no substep 4 frame. The `FLASH_MAX` clamp and the `<= 0` test are harmless only because of the exact 4-step arithmetic. Use `< 3` as in the source | Match `prologue.c:2665-2676` |
| 4 | stage 19 | No actor 96 setup at the substep 0 exit and no `draw_flags = 0` at the substep 2 exit | Add a media-bridge service that builds actor 96 from the completed O_13 transfer (section 3.1) and rejects otherwise |
| 5 | stage 19 | No presentation for the flash and white boxes (tags 0xB/0xD, additive abr 1) | Emit draw records per substep, with values as in the section 3 table |
| 6 | stage 20 (`:1438-1451`) | Pre-increment test (`counter += 1; if counter >= 0x3D`) fires on the 61st frame, one frame early. Substep 0 does not write `prologue_scene_counter = 0` | Compare the old value; write the reset before `line_start` |
| 7 | 18-20 | Only `outer_draw(2)` is emitted; there is no `dec_bg_draw(1)` presentation of background slot 1 (P_01_12) | Emit the slot 1 background draw every frame |

These preconditions come from stages 16-17. They belong to that spec, but stages 18-20 depend on them:

- **Stage 16** (`:1340-1365`) reads and decodes 0x496A into slot **0**, but the source uses slot 1 (`prologue.c:2503-2511`). It also omits `actor_clear_upper()` and `bg_layers = 0x80000000` on success.
- **Stage 17** (`:1368-1384`) calls `actor_read(33, 0x8018C800, 0xAC50)`, but the source reads 0x41 (65) sectors to **0x80180000** (`prologue.c:2550`). Stage 17 also adds a substep 4 CD wait that the source does not have; the source calls `mode_stage_next()` in the same frame. Stage 11's port uses the same wrong `actor_read(33, ACTOR_DEST_SECONDARY, 0xac50)` (`:1166`); the source is `0x41, 0x801D8000` plus the memcpy.

Media and services:

- `port/godot/services/first_arrival_media_bridge.gd` rejects everything after stage 10. `background_decode` reports "outside its source stage" (`:330`), and the line hook rejects lines other than 0-16 (`:409`). Stages 16-20 therefore cannot run in production today. This is a correct explicit rejection.
- There are no board-store or exporter entries for P_01_10, P_01_12 or O_13. Lines 38 and 40 do have exported XA packs (opening-voice-v2); lines 39, 41 and 42 have none, as the source requires.
- `tools/port/prologue_runtime_content.py` already exports texts, speakers and clip pairs for all 43 lines.

## 7. Resources the exporter must add

All of these are original-disc extents with sha256 pins, read through the existing `_native_extent` pattern (`tools/port/first_arrival_board_content.py:163-207`) into a new `.asset-cache/port/<name>` directory:

| id | file | LBA | logical size | transfer | consumer |
|---|---|---|---|---|---|
| `bg_496a` | `CDROM/BS/P_01_10.BS` | 0x496A (18794) | 18432 (9 sectors) | `dec_bg_cd_read` always reads **10** sectors (20480 B, to 0x80162000; `background_images.c:60-71`). The 10th sector is the first sector of P_01_12 | stage 16 decode into slot 1, buffer `1 - dec_bg_show[1]`. Rect (0x2C0,0,320,160) for buffer 0 or (0x140,0x100,320,160) for buffer 1 (`background_images.c:85-104`) |
| `bg_4973` | `CDROM/BS/P_01_12.BS` | 0x4973 (18803) | 18432 | 10 sectors. The 10th is the first sector of `X_01_13.BS` (LBA 0x497C) | stage 17 decode; shown through stages 18-20 |
| `o13` | `CDROM/PROLODIR/O_13.NBN` | 0xAC50 (44112) | 133120 = 0x20800 = exactly 0x41 sectors | 65 sectors to 0x80180000 (stage 17), or to 0x801D8000 plus a 0x20800 memcpy (stage 11) | stage 11 actor 96 and stage 19 actor 96. Export the frame table (+0x204, 7), sequence table (+0x220 → +0x1C4), page table (+0x1E4, 8 × 0x4000-byte 8-bit blocks at +0x230..+0x20230) and part rows |
| `clut_row_1b` | from O_01 at 0x80184490 (stage 0 loop, i = 11) | — | — | — | stage 11/19 actor CLUT 0x1FB. Reuse the stage 0 arrival palette export if it already holds row 27; otherwise add it with source citation |
| (exists) | `CDROM/XA_G/YOSHIO.XA` clip windows for lines 38 and 40 | see section 2.1 | — | — | already in `.asset-cache/port/opening-voice-v2` |

## 8. Open questions

1. Is `draw_frame`/`actor_draw_all` called before or after `game_mode` dispatch within a frame? This decides whether actor 96 is drawn on the substep 0 exit frame and the substep 2 exit frame. It also fixes the order of actor 96 and the flash boxes within OT tag 0xB: AddPrim prepends, so a later add draws first. Check the main loop before implementing the ordering.
2. Do any actors 0-95 (HUD or message window) draw during stages 18-20 and take free tpage slots before actor 96? The board store's stage 10 model assumes they do not. Verify against an emulator VRAM dump at stage 19.
3. The CD-queue serialisation between the unwaited O_13 data read and line 38's XA request (section 1) is a HYPOTHESIS. Confirm it in `cd_queue.c` command processing or in an emulator trace.
4. Check the `load_palette` arg4 (sub-palette i) source offset for row 27 against the stage 0 port export before reuse.
