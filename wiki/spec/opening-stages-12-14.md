---
type: spec
unit: opening-stages-12-14
updated: 2026-10-29
sources: ["src/ovl/name_entry/prologue.c", "asm/ovl/NAME_ENT/data/name_entry/prologue.data.s", "src/main/actor_sprites.c", "src/main/background_images.c", "src/main/cd_queue.c", "src/main/game_mode.c", "src/main/scene_library.c", "include/game/display_records.h", "disc/files/CDROM/PROLODIR", "port/godot/flow/prologue_arrival.gd", "port/godot/services/prologue_media_queue.gd", "port/godot/services/first_arrival_board_store.gd", "port/godot/services/first_arrival_media_bridge.gd"]
---

# Port spec: opening mode stages 12-14 (Shiori load, line 18, line 19)

Part of [[tickets/T-39017-opening-stage-specs]]. This page covers the three mode-0x15 stages after stage 11 (`prologue_line_17`). It is a source reading, not a runtime trace. Statements marked HYPOTHESIS are not proven by the code. Overview row: [[spec/o-nameent]] (its stage-12 and stage-14 rows name the wrong CUT files; see "Corrections").

Dispatch: `prologue_mode_handler` (`prologue.c:2743`) calls `prologue_mode_stage_shiori_load` (12), `prologue_mode_stage_shiori_line_18` (13) and `prologue_mode_stage_shiori_line_19` (14). Each wrapper runs its `_run` body only when `mode_step == 0` (`prologue.c:1878`, `1986`, `2131`; bodies at `1831`, `1939`, `2065`). No code in these stages changes `mode_step`, so the body runs on every frame of the stage. After the switch, every frame, the handler runs `message_window_title_update`, `hud_screen_frame_draw(2)` (stage is not 0), `message_window_show`, then the START test: when START is pressed (`pad_pressed & PAD_START`) it calls `mode_stage_set(0x15)` (`prologue.c:2810-2821`).

`mode_stage_next` (`game_mode.c:156`) increments `mode_stage` and zeroes `mode_step`, `mode_stage_timer`, `mode_step_timer`, `mode_stage_scratch` and `substep`. Code after the switch in the same `_run` call then sees the new stage with substep 0. This matters for the RNG tail of stages 13 and 14.

## Shared mechanics

### Actor records

`actor_pool` is at 0x8011ECD0, with 0x44 bytes per record (`display_records.h:17`). The records these stages use are 96 at 0x80120650, 97 at 0x80120694, 98 at 0x801206D8 and 99 at 0x8012071C. Each `D_801206xx` write maps to a field:

| field | offset | 96 | 97 | 98 | 99 |
| --- | --- | --- | --- | --- | --- |
| `unk_01` (OT layer: drawn at `unk_01 - 1`, `actor_sprites.c` part loop) | +0x01 | 651 | 695 | 6D9 | 71D |
| `flags` | +0x02 | 652 | 696 | 6DA | 71E |
| `draw_flags` | +0x03 | 653 | 697 | 6DB | 71F |
| `unk_05` | +0x05 | 655 | 699 | 6DD | 721 |
| `frame_table` | +0x0C | 65C | 6A0 | 6E4 | 728 |
| `sequence_table` | +0x10 | 660 | 6A4 | 6E8 | 72C |
| `sequence_slot` | +0x16 | 666 | - | 6EE | 732 |
| frame (`unk_18`, the frame/step index) | +0x18 | 668 | 6AC | - | - |
| `x` | +0x26 | 676 | 6BA | 6FE | 742 |
| `y` | +0x2A | 67A | 6BE | 702 | 746 |
| `tpage_table` | +0x34 | 684 | 6C8 | 70C | 750 |
| `quad_flags` | +0x38 | 688 | 6CC | 710 | 754 |

Draw and animation rules come from `actor_draw_all` (`actor_sprites.c:84-370`):

- `flags & 0x40` is a static frame: `frame_table[+0x18]`.
- Otherwise the drawn frame is `sequence_table[sequence_slot][step*2+1]`, and +0x18 holds the step. When `flags & 1` is set, +0x08 increments every frame (`blink_period` = 1 after `actor_clear`, `actor_sprites.c:684`). When +0x08 reaches the duration `seq[step*2+2]`, the step advances. At the end of the sequence:
  - `flags & 2`: step returns to 0 and bits 0 and 1 clear.
  - `flags & 4`: bits 0 and 2 clear and step decrements (hold last).
  - `flags & 0x10`: ping-pong. The step runs back down, and bits 0 and 5 clear at step 0.
- An actor is skipped entirely, with no draw, no tpage allocation and no animation, unless `draw_flags & 0x80` is set and both `*(s32*)tpage_table` and `*(s32*)frame_table` have bit 31 set (`actor_sprites.c:93`).
- `quad_flags` 0x41000000 & 0x03000000 = 0x01000000, and `unk_04` stays 0 after `actor_clear`. So every actor here allocates with `search_tpage(part, i, 1)` (8-bit), not `search_tpage8`.

None of the setups below writes +0x04, +0x06, +0x07, +0x08 or the scales. Those values come from `actor_clear` (`actor_sprites.c:672`) through `actor_clear_upper` (records 0x60-0x9F, `actor_sprites.c:641`) or an explicit `actor_clear(n)`. The one exception is actor 97's stage-12 setup, which the stage-13 lip sync rewrites in place without clearing.

### Background (`dec_bg_*`, `background_images.c`)

- `dec_bg_cd_read(lba, 1)` (`:62`) returns 0 or 1 without reading when `lba` equals `bg_lba_cache[2]` (the LBA held by buffer 0) or `bg_lba_cache[3]` (buffer 1). On a miss it calls `cd_read_sectors_nowait(10, 0x80162000, lba)` and records `lba` in `bg_lba_cache[3 - dec_bg_show[1]]`. That entry belongs to the hidden buffer the next decode writes.
- `dec_bg_decode_to_vram(1)` (`:85`) decodes into buffer `b = 1 - dec_bg_show[1]`. Buffer 0 is VRAM (0x2C0,0) and buffer 1 is (0x140,0x100), each 320x160. The function returns `b`, and `dec_bg_show_set(1, b)` then shows it. On `bg_decode_result == -1` the source calls `dec_bg_reset()`, which clears all four cache LBAs, both show flags and `dec_bg_last_screen_mode`, sets brightness 0x80, and re-issues the same read. The port's `_decode_bg_and_check` (`prologue_arrival.gd:164`) retries without an explicit reset. It must also apply the reset's state changes or reject.
- `dec_bg_draw(1)` runs at the end of every stage-12/13/14 frame.

Cache state across these stages (derived from the reads, not traced):

- Entering stage 12, the two slot-1 buffers hold P_01_07 (0xACF6, stage 10 substep 7) and P_01_08 (0xAD05, stage 11), and P_01_08 is shown.
- Stage 12 misses on 0xAD0B and decodes it over the P_01_07 buffer.
- Stage 13's `dec_bg_show_switch(1)` shows P_01_08 again.
- Stage 14's 0xACEC misses (the buffers hold 0xAD05 and 0xAD0B) and decodes over the 0xAD0B buffer.

### CD reads

- `cd_read_sectors(n, dst, lba)` (`cd_queue.c:513`) sends `audio_command(6)` (`AUDIO_COMMAND_XA_RESET`, [[spec/m-dates-a]]) before queuing the read, and sets `cd_read_checked = 1`. A checked read completes only when the final word of the destination extent has 0x1012 in its top half ([[spec/h-cdio]] section 2). All five checked files below carry that trailer: CUT_14 0x1012053A, O_14 0x1012DA54, O_15 0x1012F97A, CUT_16 0x10129557, O_16 0x10124D0D.
- `cd_read_sectors_nowait` (used by `dec_bg_cd_read`) also sends `audio_command(6)`, with `cd_read_checked = 0`.
- `cd_read_sync_status() & 1` is the completion test.

### Background CUT slot

`prologue_background_slot_set(0x801A4000, 0xF)` (`prologue.c:601`) does the following:

- Calls `prim_bg_slot_setup` with six 0x4000 page addresses `0x801A4000 + i*0x4000`, tp 8, 3x2 pages, and palette row 0xF + 0x1E0 = 0x1EF (495) at (-0xA0,-0x78).
- Loads the CLUT at image+0x18000 (0x801BC000) to row 0xF, and its gray version (`color_palette_to_gray` into `game_state.scratch_words`) to row 0xE.
- Sets `bg_layer_order[0] = 0xD` and `bg_layers[0].unk_00 = 0x41000000`.

Stage 3 used palette offset 0xE. These stages use 0xF, the same as stage 11.

Writing `bg_layers = 0x80000000` (first word of BG slot 0) hides the CUT layer. HYPOTHESIS: this is libgs's display-off attribute bit 31, the same convention [[spec/h-graphics]] records for the 3D object records. The scene's own use supports this: stage 13 sets it at the moment it switches the backdrop away from the CUT_14 composition.

### Tpage slots

`actor_draw_all` calls `search_tpage(0x7FFF)` once per frame (`actor_sprites.c:87`). That call marks every slot with `tpage_buf[i] == -1` as free (0x7FFF) and every other slot as locked (0x7FFE). Allocation then runs in ascending actor index, with parts walked from `count-1` down to 0. Each page goes:

1. to a locked slot whose `tpage_buf` equals the page;
2. otherwise to a slot already allocated to the same page this frame;
3. otherwise to the next free slot in ascending index order, with `load_csr_tp` queuing the 8-bit page upload (`actor_sprites.c:509-552`).

The locks before stage 10 give the free order `STAGE10_FREE_TPAGE_SLOTS = [5, 6, 7, 37, 38, 39, 58]` (`first_arrival_board_store.gd:92`). Stages 12 and 14 call `prologue_background_slot_set` with the same tp 8, 3x2 shape as the case that list was derived from. The free order is therefore taken as unchanged. This is an inference: confirm that `prim_bg_slot_setup` writes the same `tpage_buf` entries. The CUT pages live at 0x801A4000+ and actor pages at 0x8018xxxx, so step 1 never matches an actor page.

The per-frame assignments below assume that no actor below 96 allocates (the same assumption as the stage-10 model).

| scene state | actors drawn (ascending) | page order -> slot |
| --- | --- | --- |
| stage 12 commit to stage 13 substep 3 | 97 O_14 frame 4..7 (1 part, page 0 = 0x80180130); 98 O_14 blink frames 0..3 (same page) | 0x80180130 -> 5 |
| stage 13 commit frame and stage 14 substeps 0-1 | 96 O_15 frame 0 (parts p0..p3 pages 2,2,1,2; walked 2,1,2,2); 97 O_15 frames 5..8 (page 0); 98 O_15 frames 1..4 (page 0) | 0x801882E0 -> 5, 0x801842E0 -> 6, 0x801802E0 -> 7 (when 97 or 98 is drawn) |
| stage 14 substep 2+ (actor 99 only) | 99 O_15 seq 2: frame 9 (2,2,1,2), 10 and 11 (2,2,1,0,0,2), 12 (0,3,0,0) | frame 9: page2 -> 5, page1 -> 6; frames 10 and 11: page2 -> 5, page0 -> 6, page1 -> 7; frame 12: page0 -> 5, page3 (0x8018C2E0) -> 6 |
| stage 14 substep 3+ with a blink armed | 98 O_15 seq 0 (page 0) drawn before 99 | page0 -> 5, then actor 99's pages take 6, 7, ... |
| stage 14 commit frame | 96 O_16 frame 0 (pages 2,2,1,2,2; walked 2,2,1,2,2) | 0x801882C0 -> 5, 0x801842C0 -> 6 |

Pages per resource (the `tpage_table` contents, read from the disc files):

- O_14: [0x80180130]
- O_15: [0x801802E0, 0x801842E0, 0x801882E0, 0x8018C2E0]
- O_16: [0x801802C0, 0x801842C0, 0x801882C0]

### 0x80180000 actor area

Each stage ends with `memcpy(0x80180000, buf, n)`, which overwrites only `n` bytes:

| stage | source | bytes copied | bytes kept from before |
| --- | --- | --- | --- |
| 12 | O_14 | 0x4800 | 0x80184800-0x801A0800 keep O_13's tail (stage 11) |
| 13 | O_15 | 0x10800 | - |
| 14 | O_16 | 0xC800 | 0x8018C800-0x80190800 keep O_15's tail |

The port must model this area as a byte-addressed overlay. Replacing the whole resource is wrong, because stale table pointers can dereference into it (see the RNG edges below).

### Lines, voice and lip sync

`prologue_line_start` (`prologue.c:84`):

- Shows the speaker plate `gnsx(girl_short_name_table[speaker])`.
- Shows the text `sndisp(prologue_line_texts[line], 0, speaker)`.
- For speaker & 0xF != 0xF: `cd_audio_request_set(clip.start, clip.end, speaker)` and `audio_command(0x300)`.
- Sets `current_girl = speaker`, `prologue_voice_started = 0`, `k_sub_disp_start(1)`.

`prologue_line_run` (`prologue.c:106`) advances the text. Voice gating works as follows:

- Once `cd_audio_state() & 4` has been seen, the line counts as started.
- On `cd_audio_state() & 2` (clip done) the line either switches to the second clip or counts hold frames. It switches when `clips[line].start != clips_second[line].start` and the second clip's start differs from the current request's start (`cd_audio_request_get()[2]`). The switch requests the second clip and sends 0x300, without resetting the timer. Otherwise, when the text is complete, `mode_step_timer` increments.
- While the voice is playing (bit 1 clear), the timer is zeroed.
- At timer >= 0x3D, the timer resets and `substep++`.

Line data (`prologue.data.s`):

| line | text label (addr) | speaker (addr) | clip 1 (start, end) | clip 2 |
| --- | --- | --- | --- | --- |
| 18 | `prologue_line_text_18` 0x8014D548 | 0x00 Shiori (0x8014DA16) | 0x00000035, 0x02000047 (0x8014DAC0) | same (0x8014DC18) |
| 19 | `prologue_line_text_19` 0x8014D574 | 0x00 Shiori (0x8014DA17) | 0x00000048, 0x03000058 (0x8014DAC8) | 0x00000059, 0x04000064 (0x8014DC20) |

Per [[spec/h-cdio]] section 2.5, the top byte of `end` is the XA file filter byte, the top byte of `start` is the channel, and the low 24 bits are clip indices. Speaker 0 selects `cd_audio_speaker_offset_table[0]`. Line 19 is the only two-clip line. The text itself is not reproduced here; use the labels.

The lip sync functions keep `prologue_cd_level_history[frame_count & 3] = level`, where level is `getCDlevel()` (`scene_library.c:269`) when `cd_audio_state() & 4`, else 0. They also compute `prologue_cd_level_last`, with `avg = sum/4` and `mouth = min(avg/10, 3)`. The history array is shared with the earlier stages' lip sync, so carry it in state; do not reset it per stage.

## Stage 12: `prologue_shiori_load_run` (`prologue.c:1831-1876`)

This stage has no line, no RNG call and no audio command except the per-read `audio_command(6)`. Each frame ends with `dec_bg_draw(1)`.

| substep | condition | action (source order) |
| --- | --- | --- |
| 0 | - | `cd_read_sectors(0x31, 0x801A4000, 0xAB49)`; substep 1 |
| 1 | `cd_read_sync_status() & 1` | `cd_read_sectors(9, 0x801F0000, 0xAC91)`; substep 2 |
| 2 | read done | `dec_bg_cd_read(0xAD0B, 1)` (miss: 10-sector nowait read to 0x80162000); substep 3 |
| 3 | read done **and** `!(actor96.flags & 1)` | `dec_bg_show_set(1, dec_bg_decode_to_vram(1))`; on -1: `dec_bg_reset(); dec_bg_cd_read(0xAD0B, 1)` and stay; else the setup below, then `mode_stage_next()` |

The substep-3 gate waits for actor 96's stage-11 animation to end. Stage 11 substep 6 set actor 96 to O_13 with flags 5 (animate, hold last), sequence slot 0 (`prologue.c:1803-1818`). The port's stage-12 code does not test this gate.

Setup on success, in source order:

1. `actor_clear_upper()`.
2. `prologue_background_slot_set(0x801A4000, 0xF)`, which loads CUT_14 as the CUT layer and makes it visible (attribute 0x41000000).
3. `memcpy(0x80180000, 0x801F0000, 0x4800)` (all of O_14).
4. Actor 97: `unk_01` 9, flags 0x40, `quad_flags` 0x41000000, `draw_flags` 0xA4, `frame_table` 0x801800F4, `sequence_table` 0x80180114, `tpage_table` 0x801800F0, frame (+0x18) 4, `unk_05` 0x80, x -0xA0, y -0x78.

Reads:

| LBA | file | sectors | destination | bytes on disc / read | sha256 (file) |
| --- | --- | --- | --- | --- | --- |
| 0xAB49 | CDROM/PROLODIR/CUT_14.NBN | 0x31 (49) | 0x801A4000 | 100352 / 100352 | 8059ef80df31cb4662db9c8b816b258066e1730251b7454e14d3426b42c2fefe |
| 0xAC91 | CDROM/PROLODIR/O_14.NBN | 9 | 0x801F0000 | 18432 / 18432 | f81f398f969215f0ca65f04ac6624fca8f733568f21bf719fec023c7eb30bee5 |
| 0xAD0B | CDROM/PROLODIR/P_01_09.BS | 10 | 0x80162000 | 15988 / 20480 | 78c2813b109d87b5692783bbfc1bfd69ce5c89d015ed2b070cc5c8f5c7082098 |

The P_01_09 read runs 2 sectors past the 8-sector file, into the gap before `CDROM/SERIFU/DATE_AS.BIN` at 0xAD15. Export the full 10-sector transfer the same way the existing `_native_extent` exports do.

O_14 tables:

- Frames at 0xF4..0x113: 8 frames, each 1 part on page 0.
- Sequence slot 0 (0x801800C0): frames 0,1,2,3,3, 2 ticks each.
- Sequence slot 1 (0x801800D8): frames 4,5,6,7,6, 2 ticks each.

## Stage 13: `prologue_shiori_line_18_run` (`prologue.c:1939-1978`)

| substep | condition | action |
| --- | --- | --- |
| 0 | - | `prologue_line = 0x12; prologue_line_start()` (voice request + 0x300); substep 1 |
| 1 | `cd_audio_state() & 4` | substep 2 (no text run while waiting) |
| 2 | - | `prologue_line_run()`, then `prologue_lip_synchronization_shiori()`; `line_run` moves to substep 3 after the 0x3D hold |
| 3 | - | `cd_read_sectors(0x21, 0x801E8000, 0xAC9A)`; substep 4 |
| 4 | read done | setup below, then `mode_stage_next()` |

Every frame, after the switch (including the frame `mode_stage_next` ran): one `rand()` call. If `(rand() & 0xFF) == 0xFF` and `!(actor98.flags & 1)`, the source calls `prologue_actor98_animation_start_shiori` (`prologue.c:1925`). Then `dec_bg_draw(1)`. The `rand()` is always evaluated. The flag test happens only on 0xFF. That gives exactly one RNG draw per stage-13 frame and none in stage 12.

`prologue_lip_synchronization_shiori` (`prologue.c:1884`):

- When `cd_audio_state() & 4`, it rewrites all of actor 97: `unk_01` 9, flags 0x40, `quad_flags` 0x41000000, `draw_flags` 0xA4, tables 0x801800F4/0x80180114/0x801800F0, frame `mouth + 4` (O_14 frames 4..7), `unk_05` 0x80, x -0xA0, y -0x78.
- Otherwise it only sets actor 97 frame = 4.

Blink (`prologue_actor98_animation_start_shiori`): actor 98 gets `unk_01` 9, flags 0x11 (animate, ping-pong), `quad_flags` 0x41000000, `draw_flags` 0xAC, the same O_14 tables, `sequence_slot` 0, `unk_05` 0x80, x -0xA0, y -0x78. Frame and timer are not written. After a ping-pong, flags end at 0 and the step rests at 0 (frame 0): at the sequence end `flags & 0x10` turns 0x11 into 0x21 and the step drops by 3 (`draw_flags & 8`); when the step reaches 0, the 0x20 branch clears `0xFFDE` (`actor_sprites.c:329-334`, `:351-360`). Drawn frames per tick: 0,0,1,1,2,2,3,3,3,3,2,2,1,1, then 0 at rest. While flags bit 0 is set, a 0xFF draw does not restart the blink.

Substep-4 setup:

1. `actor_clear_upper()`.
2. `dec_bg_show_switch(1)`, which shows the P_01_08 buffer again.
3. `bg_layers = 0x80000000`, which hides the CUT_14 layer.
4. `memcpy(0x80180000, 0x801E8000, 0x10800)` (all of O_15).
5. Actor 96: `unk_01` 9, flags 0x40, `quad_flags` 0x41000000, `draw_flags` 0xA4, `frame_table` 0x80180294, `sequence_table` 0x801802C8, `tpage_table` 0x80180284, frame (+0x18) 0, `unk_05` 0x80, x -0xA0, y -0x78.

| LBA | file | sectors | destination | bytes | sha256 |
| --- | --- | --- | --- | --- | --- |
| 0xAC9A | CDROM/PROLODIR/O_15.NBN | 0x21 (33) | 0x801E8000 | 67584 | 9d6aea8ea1356ee666fba64e1f76d22db106fac4ad08b4e99de87ffb9326a0db |

O_15 tables:

- Frames at 0x294..0x2C7: 13 frames.
- Sequence slot 0 (0x80180238): (1,1)(2,2)(3,1)(4,2)(3,2).
- Sequence slot 1 (0x80180250): (5,2)(6,2)(7,2)(8,2)(7,2).
- Sequence slot 2 (0x80180268): (9,1)(10,2)(11,1)(12,20)(10,1)(9,1), as (frame, ticks) pairs.

**RNG edge, substep-4 frame.** When `rand` hits 0xFF on the commit frame, actor 98 is armed with O_14 pointers into the just-copied O_15 bytes. `*(s32*)0x801800F0` is now 0x00580088, with bit 31 clear. So actor 98 is skipped by `actor_draw_all` and never animates, and its flags stay 0x11. Stage 14's blink gate (`!(actor98.flags & 1)`) then stays closed until stage 14 substep 2 clears actor 98. The port must reproduce this or reject it explicitly. It must not re-arm a valid blink.

## Stage 14: `prologue_shiori_line_19_run` (`prologue.c:2065-2129`)

| substep | condition | action |
| --- | --- | --- |
| 0 | - | `prologue_line = 0x13; prologue_line_start()` (clip 1 + 0x300); substep 1 |
| 1 | - | `prologue_line_run()` (requests clip 2 + 0x300 when clip 1 ends), then `prologue_lip_synchronization_shiori_line_19()`. There is no voice-start wait, unlike stage 13. |
| 2 | - | `prologue_actors_reset_shiori_line_19()`; substep 3 |
| 3 | `!(actor99.flags & 1)` | substep 4 **first**, then `dec_bg_cd_read(0xACEC, 1)` (miss, 10-sector nowait) |
| 4 | read done | `cd_read_sectors(0x31, 0x801A4000, 0xAB7A)`; substep 5 |
| 5 | read done | `cd_read_sectors(0x19, 0x801E8000, 0xACBB)`; substep 6 |
| 6 | read done | `dec_bg_show_set(1, dec_bg_decode_to_vram(1))`; on -1: `dec_bg_reset(); dec_bg_cd_read(0xACEC, 1)` and stay; else the setup below, then `mode_stage_next()` |

The P_01_06 transfer sits in the shared 10-sector buffer 0x80162000 while the two NBN reads run. The decode happens only in substep 6, after both NBN reads.

Every frame after the switch, the same RNG pattern as stage 13 runs. If `(rand() & 0xFF) == 0xFF` and `!(actor98.flags & 1)`, the source calls `prologue_actor98_animation_start_shiori_line_19` (`prologue.c:2030`). Then `dec_bg_draw(1)`.

`prologue_lip_synchronization_shiori_line_19` (`prologue.c:1992`) updates the history the same way, then **always** calls `actor_clear(0x61)`. When `cd_audio_state() & 4`, it sets actor 97: `unk_01` 8, flags 0x40, `quad_flags` 0x41000000, `draw_flags` 0xA4, tables 0x80180294/0x801802C8/0x80180284, frame `mouth + 5` (O_15 frames 5..8), `unk_05` 0x80, x -0xA0, y -0x78. Without voice, actor 97 stays cleared and is not drawn.

Blink `prologue_actor98_animation_start_shiori_line_19`: only when `!(actor99.flags & 1)`, it calls `actor_clear(0x62)`, then sets actor 98: `unk_01` 8, flags 3 (animate, one-shot), `quad_flags` 0x41000000, `draw_flags` 0xAC, the O_15 tables, `sequence_slot` 0, `unk_05` 0x80, x -0xA0, y -0x78. After the one-shot, the step returns to 0, so actor 98 rests drawn on O_15 frame 1.

Actor 98 is not drawn in stage 14 until the first blink fires. Stage 13's commit cleared it.

`prologue_actors_reset_shiori_line_19` (`prologue.c:2047`):

1. `actor_clear` 0x60, 0x61, 0x62 and 0x63.
2. Actor 99: `unk_01` 8, flags 5 (animate, hold last), `quad_flags` 0x41000000, `draw_flags` 0xA4, the O_15 tables, `sequence_slot` 2, `unk_05` 0x80, x -0xA0, y -0x78.

Sequence 2 runs 26 ticks (1+2+1+20+1+1), during which bit 0 is set. It ends holding step 5, which is frame 9. Substep 3 waits for it.

Substep-6 setup:

1. `actor_clear_upper()`.
2. `prologue_background_slot_set(0x801A4000, 0xF)`, which loads CUT_16 and makes the layer visible.
3. `memcpy(0x80180000, 0x801E8000, 0xC800)` (all of O_16).
4. Actor 96: `unk_01` 0xB, flags 0x40, `quad_flags` 0x41000000, `draw_flags` 0xA4, `frame_table` 0x80180258, `sequence_table` 0x80180298, `tpage_table` 0x8018024C, frame (+0x18) 0, `unk_05` 0x80, x **-0xF0**, y -0x78.

| LBA | file | sectors | destination | bytes on disc / read | sha256 (file) |
| --- | --- | --- | --- | --- | --- |
| 0xACEC | CDROM/PROLODIR/P_01_06.BS | 10 | 0x80162000 | 18492 / 20480 | already exported as `names_acec` (`first_arrival_board_content.py`) |
| 0xAB7A | CDROM/PROLODIR/CUT_16.NBN | 0x31 (49) | 0x801A4000 | 100352 | de1f091899464b4c04916815f71d6da40221da0cf3ab30c6336765e7ba279b17 |
| 0xACBB | CDROM/PROLODIR/O_16.NBN | 0x19 (25) | 0x801E8000 | 51200 | 0bde8b1c5285bf6f1509adbfab894a983c0f5cd5c5238979643f7bbd84abe03f |

O_16 tables:

- Frames at 0x258..0x297: 16 frames.
- Sequence slot 0: (1,2)(2,2)(3,2)(4,2)(3,2)(5,1).
- Sequence slot 1: frames 6,7,8,9,8.
- Sequence slot 2: frames 10..13,12.
- Sequence slot 3: (14,10)(15,10).

Stage 15 owns these slots. They are listed only so the exporter covers the whole file.

**RNG edge, substep-6 frame.** After the commit, actors 98 and 99 are cleared, so a 0xFF draw arms actor 98 with O_15 table offsets into O_16 bytes. `*(s32*)0x80180284` is 0x8018017C and `*(s32*)0x80180294` is 0x801801DC, both with bit 31 set, so the draw test passes. The sequence pointer `*(s32*)0x801802C8` is 0x1D1D1D1D, which is not a RAM address. The original's behaviour from there is unknown. The port must reject this path explicitly (at about 1/256 on that one frame, determined by the LCG state). It must not draw or guess.

## Port status

`port/godot/flow/prologue_arrival.gd:1147-1273` has stage 11-14 bodies that do not follow the source and must be replaced, not patched:

- **Wrong sector counts and destinations.** Stage 12 has 25 to 0x80180000 and 5 to 0x8018C800, where the source has 49 to 0x801A4000 and 9 to 0x801F0000. Stage 13 has 17 where the source has 33 to 0x801E8000. Stage 14 has 25 and 13 where the source has 49 and 25. Stage 11 has 25 and 33 where the source has 49 and 65 (0x41) to 0x801D8000 (concurrent owner).
- **Invented brightness steps.** The stage-12 `counter -= 2` fade, the stage-13 fade-in at substeps 0-1 and the stage-11 `SHIORI_WAIT_FRAMES` wait have no source.
- **Wrong background slot.** Stage 14 reads its background with slot 0 (`background_read(...,0xacec,0)`). The source uses layer 1.
- **Wrong RNG.** RNG is drawn only inside the line substep. The source draws once per frame, after the switch.
- **Missing source behaviour.** The actor-96 animation gate, the actor-99 wait, `actor_clear(0x61)` per frame, the two-clip line, `dec_bg_show_switch`, the `bg_layers` hide, the memcpy and actor setups, the CUT slot set, and `dec_bg_draw(1)` are all absent.

`prologue_media_queue.gd:108-130` whitelists actor reads only through stage 10. Stage 12-14 reads would be rejected, which is correct until resources exist. Stage 14's 0xACEC read is a second request for a background stage 6 already read. `background_read` rejects it ("cache reuse is not implemented") unless an `allow_re_read` rule for stage 14 substep 4 is added. Case 3 raises the substep before it calls `dec_bg_cd_read(0xACEC, 1)` (`prologue.c:2081-2084`), so the read is issued with substep 4. The case-6 retry after a failed decode (`prologue.c:2104`, substep 6) follows `dec_bg_reset()`, which the port refuses with `SHIORI_BG_RESET_UNSUPPORTED`. That rule is justified by the cache derivation above: the buffers hold 0xAD05 and 0xAD0B.

`first_arrival_media_bridge.gd` ends at `_rei_line17_setup` (the stage-10 case-8 scene switch). It has no stage-11 to stage-14 setup, lip sync or blink effects. `FIRST_ARRIVAL` `FILES` (`first_arrival_board_store.gd:66`) list nothing for these stages except `names_acec`.

Implementation order:

1. Stage 11 (concurrent owner).
2. Stage 12 setup (`shiori_load` effect).
3. Stage 13 lip sync, blink and setup.
4. Stage 14 lip sync, blink, actor reset and setup.
5. An XA two-clip path in `line_run` (shared with the native audio work).
6. Explicit rejection for the stage-14 commit-frame blink.

## Resources the exporter must add

New dirs only under `.asset-cache/`, with the same manifest style as `first_arrival_board_content.py`: exact file entry, `_native_extent` transfer and sha. Transfer sha values are to be computed by the exporter.

| id (suggested) | file | LBA | sectors | destination | notes |
| --- | --- | --- | --- | --- | --- |
| `prologue.cut_14.0xab49` | CUT_14.NBN | 0xAB49 | 49 | 0x801A4000 | CUT layer, 6 pages + CLUT at +0x18000 (row 0x1EF), gray CLUT row 0x1EE |
| `prologue.o_14.0xac91` | O_14.NBN | 0xAC91 | 9 | 0x801F0000 -> 0x80180000 (0x4800) | page 0x80180130 |
| `prologue.names.0xad0b` | P_01_09.BS | 0xAD0B | 10 | 0x80162000 | decoded 320x160 RGB555 plus preview, as the `names_acec` set |
| `prologue.o_15.0xac9a` | O_15.NBN | 0xAC9A | 33 | 0x801E8000 -> 0x80180000 (0x10800) | 4 pages |
| `prologue.cut_16.0xab7a` | CUT_16.NBN | 0xAB7A | 49 | 0x801A4000 | CUT layer |
| `prologue.o_16.0xacbb` | O_16.NBN | 0xACBB | 25 | 0x801E8000 -> 0x80180000 (0xC800) | 3 pages |
| (existing) `prologue.names.0xacec` | P_01_06.BS | 0xACEC | 10 | 0x80162000 | reuse; needs the stage-14 re-read rule |

The exporter also needs:

- XA clips for lines 18 and 19 (three clip requests). These belong to the native audio exporter; resolve them from `cd_audio_request_set` with speaker 0.
- Stage 11's P_01_08 (0xAD05) and O_13 (0xAC50, 65 sectors to 0x801D8000, copied 0x20800). These are outside this page's stages but are prerequisites.

## Corrections to other pages

The [[spec/o-nameent]] stage table names the wrong CUT files:

- Stage 12 says "CUT_13". No such file exists; LBA 0xAB49 is CUT_14.NBN.
- Stage 14 says "CUT_14". LBA 0xAB7A is CUT_16.NBN.

Its LBA-to-file mapping above comes from the ISO directory (`tools/assets/input.py` `entries()`) on `disc/track1.bin`.

## Evidence limits

- This is a source and disc-data reading only. No emulator capture checks the substep timing, the RNG state, the tpage slots or the getCDlevel values.
- Tpage slot numbers assume the stage-10 free order and no allocation below actor 96.
- The `bg_layers` 0x80000000 meaning is a HYPOTHESIS.
- Frame counts per table come from the gap between the frame and sequence tables, and are confirmed by the sequence references (max frame used: O_14 7, O_15 12, O_16 15).
