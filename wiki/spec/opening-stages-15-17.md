---
type: spec
unit: opening-stages-15-17
updated: 2026-10-29
sources: ["src/ovl/name_entry/prologue.c", "asm/ovl/NAME_ENT/data/name_entry/prologue.data.s", "src/main/background_images.c", "src/main/actor_sprites.c", "src/main/gpu_effects.c", "src/main/cd_queue.c", "src/main/frame_render_audio.c", "src/main/game_mode.c", "src/main/hud_panels.c", "port/godot/flow/prologue_arrival.gd", "port/godot/services/prologue_media_queue.gd", "port/godot/services/first_arrival_media_bridge.gd", "port/godot/services/first_arrival_board_store.gd", "port/godot/services/opening_audio_rules.gd", "port/godot/services/prologue_text_store.gd"]
---

# Port spec: opening mode stages 15-17 (lines 20-37)

Porting spec for `prologue_mode_handler` (`src/ovl/name_entry/prologue.c:2743-2822`) cases 15, 16 and 17. Ticket: [[tickets/T-39017-opening-stage-specs]]. Context: [[port-pause-2026-10-29]], [[port-adversarial-review]].

| Stage | Handler | Body | Lines |
| --- | --- | --- | --- |
| 15 | `prologue_mode_stage_lines_20_to_31` (2450) | `prologue_lines_20_to_31_run` (2227-2448) | 0x14-0x1F plus two built player answers |
| 16 | `prologue_mode_stage_lines_32_to_36` (2523) | `prologue_lines_32_to_36_run` (2456-2521) | 0x20-0x24 |
| 17 | `prologue_mode_stage_line_37` (2559) | `prologue_line_37_run` (2529-2557) | 0x25 |

All three bodies run only when `game_state.mode_step == 0`. Each frame ends with `dec_bg_draw(1)`. Then the outer handler calls `message_window_title_update()`, `hud_screen_frame_draw(2)` and `message_window_show()`. PAD_START jumps to stage 0x15 (prologue.c:2806-2821). `mode_stage_next` (`src/main/game_mode.c:156-167`) increments the stage and zeroes `mode_step`, `mode_stage_timer`, `mode_step_timer`, `mode_stage_scratch` and `substep`.

## Shared mechanics

### Line player

`prologue_line_start` (prologue.c:84-104) calls `k_sub_reset`, then `gnsx` with the speaker label (`(spk&0xF)==0xF` gives the player family name). It then calls `sndisp(text, 0, spk)`. When `(spk&0xF)!=0xF`, it calls `cd_audio_request_set(clip[n].start, clip[n].end, spk)` and `audio_command(0x300)`. Finally it sets `current_girl = spk`, `prologue_voice_started = 0` and calls `k_sub_disp_start(1)`.

`prologue_line_run` (106-191) advances `substep` once `mode_step_timer.u >= 0x3D`. For a voiced line, the timer only counts while the text is complete, the voice has started (`cd_audio_state()&4` was seen) and `cd_audio_state()&2` is set. For every line 20-37, `prologue_line_clips_second == prologue_line_clips` (checked). So the source never issues the second-clip re-request in these stages.

`cd_audio_request_set` (`src/main/cd_queue.c:487-494`) stores `[spk&0xF, cd_audio_speaker_offset_table[spk&0xF], start, end]`. The words below are raw table data (`prologue_line_clips` at 0x8014DA30, `_second` at 0x8014DB88, stride 8).

| Line | Speaker byte | Voice clip words (start, end) | Text (`prologue_line_text_NN`) |
| --- | --- | --- | --- |
| 0x14 (20) | 0x0D Yoshio | 0x13000016, 0x0400002F | か、かわいいー。 藤崎詩織ちゃんか…。 このクラスでよかったぜ。 |
| 0x15 (21) | 0x1F (player name, silent) | none | さっきは、嫌だって 言ってたくせに…。） |
| 0x16 (22) | 0x0D | 0x18000079, 0x0F000086 | あっ、今、お前の事見たぜ。 あの娘知ってるのか？ |
| 0x17 (23) | 0x0F player | none | 知ってるも何も、 俺の幼なじみだよ。 |
| 0x18 (24) | 0x0D | 0x18000087, 0x10000094 | へぇ、そうなのか…。 じゃあ、彼女の誕生日は？ |
| 0x19 (25) | 0x0D | 0x1E00003D, 0x0B000040 | 血液型は？ |
| 0x1A (26) | 0x0D | 0x1D000021, 0x08000023 | 趣味は？ |
| 0x1B (27) | 0x0F | none | 音楽鑑賞って、 言ってたじゃないか。 |
| 0x1C (28) | 0x0D | 0x1E000041, 0x0C000047 | あっ、そうか。 スリーサイズは？ |
| 0x1D (29) | 0x0F | none | そんなの知るかよ。 |
| 0x1E (30) | 0x0D | 0x18000095, 0x1100009C | よし、 チェックだ。 チェック。 |
| 0x1F (31) | 0x0F | none | お、お前…。 そのメモ帳、女の子の事 ばっかりじゃないか…。 |
| 0x20 (32) | 0x0D | 0x13000030, 0x05000045 | ま、女の子の事で、 知りたいことがあったら、 俺に連絡しな。 力になるぜ。 |
| 0x21 (33) | 0x0F | none | そ、そうか…。 ありがとう。 |
| 0x22 (34) | 0x0D | 0x1800009D, 0x120000AA | それよりも。 お前、この高校の 伝説知ってるか？ |
| 0x23 (35) | 0x0F | none | 伝説？ 何それ？ |
| 0x24 (36) | 0x0D | 0x1400008A, 0x0B000099 | ほら、そこの窓から、 でっかい木が見えるだろ。 |
| 0x25 (37) | 0x0F | none | ん？ あぁ。 |

The speaker bytes come from `prologue_line_speakers` (0x8014DA04, data.s:543-588), and the texts from `prologue_line_texts` (data.s:495-539). Speaker 13 is Yoshio (`SPEAKER_YOSHIO` in `opening_audio_rules.gd:13`). Line 0x1E is voiced, but stage 15 runs no lip-sync on it.

### Lip-sync variants

Both variants use the same 4-sample `getCDlevel` ring: `level = (cd_audio_state()&4) ? getCDlevel() : 0`, `history[frame_count&3] = level`, `frame = min(avg/10, 3)`.

- **`prologue_lip_synchronization_lines_20_to_31`** (prologue.c:2139-2190). It always calls `actor_clear(0x61)` (actor 97). Only when `cd_audio_state()&4` is set, it writes the following:
  - Actor 97: `unk_01=9`, `flags=0x40`, `quad_flags=0x41000000`, `draw_flags=0xA4`, frame/sequence/tpage tables `0x80180258/0x80180298/0x8018024C`, `unk_18(frame)=frame+6`, `unk_05=0x80`, `x=actors[96].x`, `y=-0x78`.
  - Actor 98: the same fields, with `unk_18=frame+10`.
  - Actor 100: `flags=0`.

  When the voice is idle, actor 98 is not cleared and keeps its last frame. Actor 100's `flags=0` stops its loop but keeps the current sequence step.
- **`prologue_lip_synchronization_yoshio`** (843-879). It clears actor 97. When the voice is active, it sets actor 97 to `unk_01=9`, `flags=0x40`, `quad=0x41000000`, `draw=0xA4`, tables `0x801842C4/0x801842F0/0x801842BC`, `unk_18=6-frame`, `unk_05=0x80`, `x=-0xA0`, `y=-0x78`.

### Actor pool addresses

The actor pool base is 0x8011ECD0 and each record is 0x44 bytes (`include/game/display_records.h:17-54`). Actor 96 is at 0x80120650, 97 at 0x80120694, 98 at 0x801206D8, 99 at 0x8012071C, 100 at 0x80120760 and 101 at 0x801207A4. The `D_8012xxxx` names in the source resolve to these field offsets: +1 `unk_01`, +2 `flags`, +3 `draw_flags`, +5 `unk_05`, +0xC frame, +0x10 sequence, +0x16 `sequence_slot`, +0x18 frame index, +0x26 `x`, +0x2A `y`, +0x34 tpage, +0x38 quad.

`actor_draw_all` picks the frame with `flags&0x40` as the direct index `frame_table[unk_18]`. Otherwise it reads `seq=sequence_table[sequence_slot]` and draws `frame_table[seq[step*2+1]]` with duration `seq[step*2+2]` (`actor_sprites.c:100-110`). The sequence advances only while `flags&1` is set (312-367). At the end, `flags&4` clears bits 0 and 2 and holds the last step. With no end bit set, the sequence loops to step 0.

### Background reads

`dec_bg_cd_read(lba, 1)` (`background_images.c:62-83`) checks the slot-1 cache entries `bg_lba_cache[2]` and `[3]`. On a miss, it calls `cd_read_sectors_nowait(0xA, 0x80162000, lba)`, which always transfers 10 sectors, and records `lba` in `bg_lba_cache[3 - dec_bg_show[1]]`. For stages 15-17, all three LBAs miss: the slot-1 cache holds P_01_09 (0xAD0B, stage 12) and P_01_06 (0xACEC, stage 14) at stage-15 entry.

`dec_bg_decode_to_vram(1)` decodes into buffer `1 - dec_bg_show[1]`. Buffer 0 is VRAM rect `[0x2C0,0,0x140,0xA0]` and buffer 1 is `[0x140,0x100,0x140,0xA0]`. The function returns the buffer number, and `dec_bg_show_set(1, …)` then shows it. The buffer alternates per successful decode, so the port must derive it from the live `dec_bg_show[1]`. The existing 0xACD4 descriptor pins buffer 1 for stage 5 (`first_arrival_board_store.gd:373`), and that pin must not be reused blindly.

`cd_read_sectors` and `_nowait` both issue `audio_command(6)` first (`cd_queue.c:501-520`). `dec_bg_reset` (37-48) zeroes all four cache LBAs and the show flags, and resets brightness to 0x80.

### Texture-page slots

The tpage slot model is the same as stage 10 (`STAGE10_FREE_TPAGE_SLOTS = [5,6,7,37,38,39,58]`, `first_arrival_board_store.gd:88-92`). Between stage 10 and stage 17, `prologue.c` adds no `load_tpage_buf_lock` or `tpage_buf_clear` call. Every `prologue_background_slot_set` re-locks the same slots 8-10/40-42 (tp=8, 3x2; `gpu_effects.c:51-102`, prologue.c:601-614).

`search_tpage(0x7FFF)` rebuilds `tpage_slot_state` from `tpage_buf` every frame (`actor_sprites.c:87, 509-555`). Each drawn part then takes the first free slot in that order. Parts are walked in ascending actor index, and within one actor from count-1 down to 0. A page already allocated in the same frame is reused. The allocation, with its `load_csr_tp` upload, repeats every frame.

## Stage 15: lines 20-31

### Entry state

Stage 14 case 6 (prologue.c:2100-2121) leaves the following state:

- **Memory:** O_16.NBN at 0x80180000 (copy of 0xC800 bytes) and CUT_16.NBN at 0x801A4000.
- **Background:** slot 1 shows P_01_06 (0xACEC). Layer 0 is the CUT_16 cut, locked in tpage slots 8-10/40-42 with palette offset 0xF.
- **Actors:** `actor_clear_upper` has run. Actor 96 has `unk_01=0xB`, `flags=0x40`, `draw=0xA4`, `quad=0x41000000`, tables `0x80180258/0x80180298/0x8018024C`, `unk_18=0`, `unk_05=0x80`, `x=-0xF0` and `y=-0x78`.
- **Actor 98 on the switch frame:** stage 14's per-frame `rand()` still runs after `mode_stage_next` in that frame (prologue.c:2125-2127). With `(rand()&0xFF)==0xFF` (1/256), it re-arms actor 98 through `prologue_actor98_animation_start_shiori_line_19` with O_16-relative pointers `0x80180294/0x801802C8/0x80180284`. The port must carry this RNG draw and its possible actor-98 state into stage 15. Stage 15 itself makes no `rand()` call.

### Substeps

Lines are cited by `case` in prologue.c 2235-2443. "Line N" means the substep calls `prologue_line = N; prologue_line_start(); substep++`, and the following substep calls `prologue_line_run()`.

| Sub | Source effect |
| --- | --- |
| 0 | Slide: if `actor96.x < -0xA0`, then `actor97.x = actor96.x = x+1`. Then line 0x14. |
| 1 | Same slide, then `line_run`, then lip-sync 20-31. The slide takes 80 frames from -0xF0 to -0xA0, spread over substeps 0-1. |
| 2-3 | Line 0x15 (silent player line). |
| 4 | `prologue_actor99_set_lines_20_to_31` (2192-2203): actor 99 gets `unk_01=9`, `flags=5`, `quad=0x41000000`, `draw=0xA4`, tables `0x80180258/0x80180298/0x8018024C`, `unk_05=0x80`, `x=-0xA0`, `y=-0x78`. `sequence_slot` and `unk_18` stay at their `actor_clear` value of 0. `substep++`. There is no line. |
| 5 | Wait until `!(actor99.flags & 1)`. O_16 sequence slot 0 plays frames 1,2,3,4,3,5 with durations 2,2,2,2,2,1, then clears bits 0 and 2 (one-shot). |
| 6-7 | Line 0x16, with lip-sync in 7. |
| 8-9 | Line 0x17. |
| 10 | `prologue_actor100_set_lines_20_to_31` (2205-2218): actor 100 gets `unk_01=0xA`, `flags=0`, `quad=0x41000000`, `draw=0xA4`, the same three tables, `sequence_slot=3`, `unk_05=0x80`, `x=-0xA0`, `y=-0x38`. Message-window OT index `D_800B5BD4=9` (used at `hud_panels.c:1457-1461`). Then line 0x18 starts in the same substep. |
| 11 | If `actor100.y >= -0x77`, then `y -= 1` (64 frames to -0x78). Then `line_run` and lip-sync. |
| 12 | Built player answer: `k_sub_reset`, `gnsx(player_family_name)`, text = `D_800B3288[month]` + "月" (2 bytes) + (`day>=10` ? `D_800B3288[day/10]` : nothing) + `D_800B3288[day%10]` + "日。" (4 bytes). The fields come from `girls[0].birthday` (`NameEntryBirthdayBits`: month bits 0-3, day bits 4-8). Then `sndisp(text,0,0xF)`, `current_girl=GIRL_PLAYER(15)`, `k_sub_disp_start(1)`, `substep++`. There is no `cd_audio_request_set` and no `audio_command`. `prologue_voice_started` is not reset. |
| 13 | `line_run` (player path), then `prologue_actor100_pulse` (2220-2225): if `!(flags&1)`, then `flags=1` (loops O_16 sequence slot 3: frame 14 for 10 ticks, frame 15 for 10 ticks). `D_800B5BD4=0xF`. |
| 14-15 | Line 0x19, with lip-sync in 15. |
| 16 | Built answer: `prologue_blood_texts[blood_type]` (Ｏ/Ａ/Ｂ/ＡＢ, data.s:28-71; `blood_type` = bits 9-10) + "型。" (4 bytes). Speaker 0xF, same calls as substep 12. |
| 17 | `line_run`, then pulse. |
| 18-19 | Line 0x1A, with lip-sync in 19. |
| 20-21 | Line 0x1B, with pulse in 21. |
| 22-23 | Line 0x1C, with lip-sync in 23. |
| 24-25 | Line 0x1D. |
| 26 | `x = actor96.x`. If `x >= -0x18F`: `x = (s16)(x-2)`, written to the `x` of actors 101, 100, 99, 98, 97 and 96, in that store order. Otherwise `substep++`. 120 slide frames reach -400, and the next frame advances. |
| 27-28 | Line 0x1E (voiced, no lip-sync). |
| 29-30 | Line 0x1F. |
| 31 | `dec_bg_cd_read(0xACD4,1)`: 10 sectors to 0x80162000. `substep++`. |
| 32 | When `cd_read_sync_status()&1`: `cd_read_sectors(0x31, 0x801A4000, 0xAA54)` (49 sectors, CUT_07). `substep++`. |
| 33 | When synced: `cd_read_sectors(0x19, 0x801E8000, 0xABCA)` (25 sectors, O_07). `substep++`. |
| 34 | When synced: `dec_bg_show_set(1, dec_bg_decode_to_vram(1))`. **On failure** (`bg_decode_result==-1`): `dec_bg_reset()` and `cd_read_sectors(0x31, 0x801A4000, 0xAA54)`. This re-reads the cut, not the background, and stays in 34; the next decode reuses 0x80162000. **On success:** `actor_clear_upper()`, `prologue_background_slot_set(0x801A4000, 0xF)`, `memcpy(0x80180000, 0x801E8000, 0xC800)`, `prologue_actor96_set_scene_06()`, `prologue_actor99_set_scene_06()`, `mode_stage_next()`. |

Stage 15 makes no audio bank or BGM change.

### Effects of substep 34 success

- **Background:** slot 1 shows P_01_04 in buffer `1-show[1]`.
- **Cut layer:** the slot set uses addresses `0x801A4000 + i*0x4000` for i=0..5 and locks them into tpage slots 8,9,10 (row 0) and 40,41,42 (row 1). It sets CLUT `func_8009ED74(0, 0x1EF)` and `load_palette(cut+0x18000, 0xF,1,2,0)`, and writes a grey copy to 0xE. Layer 0 gets `unk_00=0x41000000`, `x=-0xA0`, `y=-0x78`, 3x2 cells of 0x80x0x78, and `bg_layer_order[0]=0xD`.
- **Actor 96** (Yoshio, `prologue_actor96_set_scene_06`, 829-841): `unk_01=0xA`, `flags=0x40`, `quad=0x41000000`, `draw=0xA4`, tables `0x801842C4/0x801842F0/0x801842BC`, `unk_18=0`, `unk_05=0x80`, `x=-0xA0`, `y=-0x78`.
- **Actor 99** (919-930): `unk_01=9`, `flags=0`, the same quad/draw values, tables `0x801800F8/0x80180114/0x801800F4`, `unk_05=0x80`, `x=-0xA0`, `y=-0x78`.

The end of stage 15 matches stage 5 case 5 (prologue.c:803-817) with three differences:

- the source buffer is 0x801E8000 instead of 0x801F0000;
- the slot set runs inside the success branch, after the decode;
- the retry re-reads CUT_07 instead of 0xACD4.

### Tpage allocation

These results come from the O_16 and O_07 frame tables, parsed in Docker from `disc/files/CDROM/PROLODIR`:

- **O_16, during substeps 0-33:** the tpage table at 0x24C holds `[0x2C0, 0x42C0, 0x82C0]` and the frame table at 0x258 holds 16 frames. Frame 0 (actor 96) walks the pages 0x82C0, 0x82C0, 0x42C0, 0x82C0, 0x82C0. Frames 1-13 use 0x2C0 and frames 14-15 use 0x82C0. Allocation: 0x82C0 → slot 5, 0x42C0 → slot 6, and 0x2C0 (actors 97, 98, 99 frames) → slot 7. Actor 100 frames reuse 0x82C0 (slot 5).
- **From the switch frame, O_07 copy:** the actor-96 table 0x42BC holds `[0x4310, 0x8310]`, and frame 0 walks 0x4310 ×3, then 0x8310. The actor-99 table 0xF4 holds `[0x130]`. Allocation: 0x4310 → 5, 0x8310 → 6, 0x130 → 7.
- Lower-index actors are assumed to allocate nothing, as in the stage-10 model. That assumption is unproven against an actual VRAM capture.

## Stage 16: lines 32-36

Entry state comes from the stage-15 success above. Actor 99 is in sequence mode with `flags=0`, so it shows the static step 0 of O_07 seq slot 0, which is frame 0.

| Sub | Source effect (prologue.c 2457-2518) |
| --- | --- |
| 0-1 | Line 0x20, with `prologue_lip_synchronization_yoshio` in 1. |
| 2-3 | Line 0x21. |
| 4 | `audio_bank_start(0, 1)` (`frame_render_audio.c:691-716`): if `(*(u32*)0x8001C000 & 0xFFFF) != 0x40`, it returns -1 and does nothing. Otherwise it issues `audio_command` 0x22, 0x21, 0x2F, 0x201 and sets `audio_bgm_set=0`, `audio_bgm_sequence=1`, `audio_bgm_playing=1`. Then line 0x22 starts in the same substep, which issues `audio_command(0x300)` after the request. |
| 5 | `line_run`, then lip-sync (Yoshio). |
| 6-7 | Line 0x23. |
| 8-9 | Line 0x24, with Yoshio lip-sync in 9. |
| 10 | `dec_bg_cd_read(0x496A, 1)` (P_01_10). `substep++`. |
| 11 | When synced: decode as above. **Failure:** `dec_bg_reset()` and `dec_bg_cd_read(0x496A,1)`. **Success:** `actor_clear_upper()`, `bg_layers = 0x80000000`, `mode_stage_next()`. The second write sets layer-0 `unk_00` to the cleared value that `bg_layers_clear` writes (`game_init.c:267`). `bg_layer_order[0]` stays 0xD, so `func_80098D00` is still called for layer 0 (`frame_render_audio.c:80-85`). How that function treats `unk_00 = 0x80000000` instead of 0x41000000 is not re-derived here; the port must decide it from that function, not from the name. |

Bank slot 0 is the arrival bank, loaded by `audio_bank_load_start(0, 0xC592, 0xC580, 0xD15F, 0xD110, 0xD102)` at prologue.c:183. Stage 10/11 loaded and started bank slot 1 (`audio_bank_load_start(1,0,0,0xCBD4,0xCBAA,0xCBA5)`, `audio_bank_start(1,0)`, 1735/1770). So substep 4 restarts sequence 1 of the still-resident slot-0 bank. Whether the slot-0 header at 0x8001C000 is still 0x40 at that point is a runtime fact. The port must model it and must not assume it.

Tpage allocation: actor 96 frame 0 gives 0x4310 → 5 and 0x8310 → 6. Actor 97 frames 3-6 (all 0x8310) reuse slot 6. Actor 99 gives 0x130 → 7. After `actor_clear_upper`, no upper actors draw.

## Stage 17: line 37

| Sub | Source effect (prologue.c 2530-2554) |
| --- | --- |
| 0-1 | Line 0x25 (silent player line). No lip-sync and no actors; the actors were cleared at the end of stage 16. |
| 2 | `dec_bg_cd_read(0x4973, 1)` (P_01_12). `substep++`. |
| 3 | When synced: decode. **Failure:** `dec_bg_reset()` and `dec_bg_cd_read(0x4973,1)`. **Success:** `cd_read_sectors(0x41, 0x80180000, 0xAC50)` (65 sectors, O_13, ends at 0x801A0800), then `mode_stage_next()` in the same frame. **There is no sync on this read in stage 17.** |

Stage 18 (2567-2577) does not poll `cd_read_sync_status` either. Line 0x26 starts at once, and substep 1 waits for `cd_audio_state()&4`. Ordering of the O_13 read before the XA play therefore rests on the CD command ring (`cd_audio_command`), and the port must preserve it. Stage 17 has no RNG, actor or bank activity. The tpage free list is unchanged, and slots 8-10/40-42 stay locked even though layer 0 is cleared.

## RNG

None of these function bodies calls `rand()`: stage 15-17 bodies, `prologue_line_start/run`, both lip-sync variants, `actor99/100` setup, pulse, `k_sub_*`, `sndisp` and `getCDlevel` (checked). The only per-frame draw on this path is `hud_screen_frame_draw`, which calls `rand()` twice when `(player_flags.b[1]&15)==4` (`hud_panels.c:1385-1400`). That path is shared with every prologue stage. The stage-14 switch-frame draw is covered under the stage-15 entry state.

## Disc mapping

Verified from the ISO directory in Docker (`tools/assets/input.open_input`, `disc/track1.bin`).

| LBA | File | Size | Read in | Transfer |
| --- | --- | --- | --- | --- |
| 0xACD4 | CDROM/PROLODIR/P_01_04.BS | 17016 (9 sectors) | 15/31 | 10 sectors → 0x80162000; covers the first sector of P_01_04B (0xACDD) |
| 0xAA54 | CDROM/PROLODIR/CUT_07.NBN | 100352 (49) | 15/32, 15/34 retry | 0x31 → 0x801A4000 |
| 0xABCA | CDROM/PROLODIR/O_07.NBN | 51200 (25) | 15/33 | 0x19 → 0x801E8000, then `memcpy` 0xC800 → 0x80180000 |
| 0x496A | CDROM/BS/P_01_10.BS | 18432 (9) | 16/10, 16/11 retry | 10 sectors → 0x80162000; includes the first sector of P_01_12 (0x4973) |
| 0x4973 | CDROM/BS/P_01_12.BS | 18432 (9) | 17/2, 17/3 retry | 10 sectors → 0x80162000; includes the first sector of X_01_13.BS (0x497C) |
| 0xAC50 | CDROM/PROLODIR/O_13.NBN | 133120 (65) | 17/3 | 0x41 → 0x80180000 |
| 0xACBB / 0xAB7A | O_16.NBN / CUT_16.NBN | 51200 / 100352 | stage 14 (prerequisite) | 0x19 → 0x801E8000 → 0x80180000; 0x31 → 0x801A4000 |

The `o-nameent` table ([[spec/o-nameent]] §8) labels 0xAB49/0xAB7A as CUT_13/CUT_14 ("inferred"). The disc directory has CUT_14 and CUT_16, and that table should be corrected. The port's board-store ids lag the disc names by one: `cut_06`/`CUT_06_ID` is CUT_07.NBN and `o_06` is O_07.NBN (`first_arrival_board_store.gd:48-50, 240`).

## Port status (`port/godot/flow/prologue_arrival.gd`)

Stages 15-17 are dispatched (`prologue_arrival.gd:105-110`), but the bodies (1275-1385) do not follow the source and must be rewritten.

### Stage 15 (`_stage_15_lines_20_to_31`)

- The loop maps lines 20-25 onto substeps 0-11 two at a time. That is wrong:
  - substeps 4/5 are the actor-99 setup and its wait, not line 22;
  - substep 10 is actor-100 setup plus line 0x18.
- The x slide in substeps 0/1, the y slide in 11, lip-sync, pulse and the `D_800B5BD4` writes are all missing.
- Substep 26 increments a fabricated `prologue_actor_x` up to `>= 0x40`. The source decrements six actors' `x` by 2 down to < -0x18F.
- Substeps 27 and 30 are fabricated `CLOSING_HOLD_FRAMES` holds. In the source they are lines 0x1E and 0x1F.
- The reads have the wrong sizes, destinations and order:
  - 25 sectors to `ACTOR_DEST_PRIMARY` and 13 sectors to `ACTOR_DEST_SECONDARY`, where the source reads 49 sectors → 0x801A4000 and 25 sectors → 0x801E8000;
  - the background is decoded at 32, where the source decodes at 34 after both reads;
  - the retry re-issues the background read, where the source re-reads CUT_07;
  - the slot set, `memcpy`, actor setups and `actor_clear_upper` are missing.

### Stage 16 (`_stage_16_lines_32_to_36`)

- Line order and `bank_start(0,1)` at substep 4 are correct.
- Yoshio lip-sync is not called from the flow.
- The background read uses slot 0; the source uses slot 1.
- On success, `actor_clear_upper` and `bg_layers=0x80000000` are missing.

### Stage 17 (`_stage_17_line_37`)

- The O_13 read is 33 sectors to `ACTOR_DEST_SECONDARY`; the source reads 65 sectors to 0x80180000.
- The flow adds a substep 4 that waits for sync, which the source does not have. The stage must advance in the same frame as the read.

### Services

- **Media queue:** `prologue_media_queue.gd:97, 115-124` admits no stage 15-17 tuple. Every read here would be rejected.
- **Media bridge:** the stage-5 → 6 setup (`first_arrival_media_bridge.gd:651`) is the closest existing piece. It is pinned to stage 5 substep 5, 0x801F0000 and the 0xACD4 retry.
- **Built answers 100/101:** the text construction (`prologue_text_store.gd:95-110`) matches the source fields. However, `opening_audio_rules.gd:359-360` assigns them the "shiori" track, and `lip_sync_variant_for_line` (401-404) assigns `shiori_birthday_speak`/`shiori_blood_type_speak`. The source line is a silent player line (speaker 0xF). The port must drop the voice and lip-sync for 100/101.
- **`lip_sync_lines_20_to_31`** (`opening_audio_rules.gd:682-700`) writes fabricated fields: `flags 1`, `sequence_slot 1`, `z 0x3c`, and fixed x/y of -52/-39 and -61/-80. It also clears actor 97 only in the idle branch. It must write exactly the source fields above.
- **`bank_start`** (`opening_audio_rules.gd:175-207`) has no bank-slot residency or the 0x40 header test. It must model slot 0 (0x8001C000) and slot 1 (0x8002E800) separately, and treat the not-ready case as a silent -1, not an error.
- **XA:** `prologue_line_services.prepare` prepares lines 0-42 (`prologue_line_services.gd:58`). Lines 20-36 have no actual playback evidence yet.

## Resources the exporter must add

1. **O_16.NBN** (0xACBB, 25 sectors, sha256 prefix 0bde8b1c5285bf6f) and **CUT_16.NBN** (0xAB7A, 49 sectors, de1f091899464b4c), from stage 14. They are needed for the actor tables at 0x24C/0x258/0x298 and the stage-15 cut layer. Include the frame and page tables listed above.
2. **CUT_07** (0xAA54) and **O_07** (0xABCA). These are already exported as `cut_06`/`o_06`. Add the stage 15 tuples: `(0xAA54, 49, 0x801A4000)` at 15/32 and the 15/34 retry, and `(0xABCA, 25, 0x801E8000)` at 15/33. Also add a `memcpy` operation of 0xC800 bytes from 0x801E8000 to 0x80180000. This is the same class of operation as the open O11 case-11 copy.
3. **P_01_04** (0xACD4). The transfer is already exported. Add a decode whose buffer follows the live `dec_bg_show[1]`, with both rects.
4. **P_01_10.BS** (0x496A) and **P_01_12.BS** (0x4973), new. Export the 10-sector native extents (20480 bytes, including the trailing sector of the next file), the MDEC decode for both buffers, and transfer, word and source SHAs, in the format of `first_arrival_board_content.py`.
5. **O_13.NBN** (0xAC50, 65 sectors, 38c0149c0fb6bd9b), new. Admit it at 17/3 → 0x80180000 and at stage 11 → 0x801D8000 (prologue.c:1791).
6. **XA voice** for lines 0x14, 16, 18, 19, 1A, 1C, 1E, 20, 22 and 24, with the clip words above, plus `getCDlevel` envelopes for the lip-sync lines. Text for lines 20-37 and the built-answer pieces: `D_800B3288` rows, 月, 日。, Ｏ/Ａ/Ｂ/ＡＢ and 型。.
7. **Tpage slot maps** for O_16 (0x82C0:5, 0x42C0:6, 0x2C0:7) and O_07 (0x4310:5, 0x8310:6, 0x130:7). Compute them per frame from the drawn actors, as `o11_tpage_slots` does. Do not use constants.

## Evidence limits

- Exact VRAM slot placement needs an emulator capture; it has not been compared against VRAM.
- Whether bank slot 0 still has a 0x40 header at stage 16 has not been checked against a running game.
- The `D_800B5BD4` meaning (message-window OT index) is inferred from its two readers in `hud_panels.c`.
- `unk_01` and `unk_05` keep their source names; their meanings are not established.
