---
type: spec
updated: 2026-10-24
sources: ["src/main/scene_library.c", "include/main_api.h", "wiki/spec/core.md", "wiki/spec/m-hud.md", "wiki/spec/m-dates-a.md", "wiki/emulator.md", "config/renames/proposals/m-dates-b.tsv", "config/renames/proposals/m-dates-b-apply.tsv", "config/renames/proposals/m-dates-b-essential.tsv"]
---

# Port spec: main scene library (m-dates-b)

Unit `m-dates-b` of phase 2 ([[tickets/T-27015-p2-m-dates-b]], parent [[tickets/T-27000-phase-2]]): the main-exe object `src/main/scene_library.c` (0x8007C030-0x80085E30, 111 symbols). The task text called it "girls, day advance and limits"; those are 6 of its functions. The object is the shared **scene step library**: the building blocks that every event scene (date, walk home, New Year, school trip, culture festival, ending ...) strings together, plus the dialogue engine they all use. The original analysis is `config/renames/proposals/m-dates-b.tsv`; the reconciled supported-row batch and essential inventory describe historical preparation. Current names follow applied ledgers and source; rows still absent from those ledgers remain candidates. Shared core names follow [[spec/core]], hardware names follow the `h-graphics` and `h-cdio` tables; names owned by `m-dates-a` (sound driver), `m-hud` and `m-weekday` follow their current applied owners. The original analysis retains stale aliases and unsupported `file`, `comment` and inline `const` rows; do not use it as an apply batch. Old names are given in backticks where a reader needs the link to the current code.

Scope (user decision of 2026-10-24): game logic, state and flow are described fully; the PS1 hardware layer (GPU primitives, sprite tables, VRAM, sound driver, CD/XA, MDEC) is named at module level (section 12). No game text is quoted: strings are described by role.

How facts were obtained: the matched C of this object, its callers and the step tables in the overlay asm data, and the emulator (section 15: about twenty short runs from a boot-to-title save state into the GEKO special-event scene, a BUNKAKEN culture-festival scene and a DATE scene, using memory writes to enter the scene). A claim of game meaning that neither the code nor a run proves is marked HYPOTHESIS.

## 1. What the library is

Every event scene overlay is a **table of function pointers indexed by `step`** (`GameState.mode_step`, `unk_110A`). The overlay's mode handler picks the table (by girl and scene id) and calls `table[step](n)` once per frame; the call parameter `n` is one constant per table (0x20 in KANGEI, 0x80 in the GEKO girl-0 scene) and only `wait_sub_sub` reads it. A step function does its work for one frame and calls `mode_step_next` when it is finished (a step that is not finished simply returns and is called again next frame), or branches with `mode_step_set(k)`. Steps that must be repeated after a failed disc read step *back*: `mode_step_next` followed by `step -= 3` is a net retry two steps earlier. This object holds the step functions that all tables share (the O.BIN names `normal_date_*`, `place_init`, `dec_init`, `hizuke_*`, `select_girl*`, ...) and the data they share. The overlay step tables reference them hundreds of times (for example the DATE tables hold `scene_wait_cd_read` 304 times, `normal_date_girl_out` 160, `dec_init` 129); BUNKASAI and BUNKAKEN call `normal_date_speak` 762 and 572 times from C.

The scene state is a per-scene work area (`talk_work`, section 3) and the shared `GameState` fields of [[spec/core]]. `step_timer` (`unk_1104`) is **not** advanced by the main loop: the setters clear it and the step functions that need a counter increment it themselves (wipes, `wait_sub_sub`, `scene_wait_cd_read`, `don_wait`). The two-phase steps (`normal_date_girl_in`, `normal_date_girl_out`, `normal_date_move_place`, `normal_date_two_select`, `normal_date_three_select`, `select_girl`, `select_girl2`) use it as the phase: 0 runs the init half, which sets it to 1, 1 runs the main half, anything else calls `game_mode_reset_to_title` (the usual error path).

A typical scene start, as observed in the emulator (GEKO special event, girl 0, scene 1; frame numbers are frames of the run, the scene's first frame is 64; the table calls `f[step](0x80)`):

| frame | step function | what it does |
|---|---|---|
| 64 | setup | `talk_names_set(girl)`, `talk_clear`, `addr_init_bustup`, `check_para_limit`, `text_marker_hide` |
| 208 | `scene_wait_cd_read` | waits for an earlier CD read |
| 253 | `scene_still_show`, `scene_anim_show` | set up the scene picture layers |
| 272 | `bg_read_sub2(lba)` | start loading the background picture |
| 291, 293 | `dec_init`, `change_dec_bg` | check and decode the picture, show it |
| 294 | `read_bustup_uniform` | start loading the girl's portrait (winter uniform in April: sector 0x544A) |
| 327 | `portrait_show_checked` | verify the loaded file and build the portrait |
| 329-344 | `scene_bg_wipe_in` | reveal wipe, 16 frames, kind 4 (April % 7) |
| 345 | `place_init` | place title into the message window |
| 347 | `normal_date_speak` -> `talk_run` | the script starts |

The same order (read, wait, decode, show; portrait read, wait, build; wipe; title; speak) is the skeleton of every table.

## 2. Step reference

Names: proposed new name (current name). "Table refs" counts distinct `.word` references in the overlay data asm (the aggregated and the per-object data files; a per-symbol island file that repeats a word of them is counted once: a raw `grep` over all data files finds 27 `scene_bg_wipe_in` words in GEKO and 25 `portrait_expression_3` words in DATE, the distinct words are 26 and 24); "C" counts calls in the overlay C sources, comments excluded, under the O.BIN name and the `func_` name together.

### 2.1 Background

| step | what it does | duration | users |
|---|---|---|---|
| `bg_read_sub2(lba)` (helper, O.BIN name kept) | `cd_read_checked = 0`, then `dec_bg_cd_read(lba, layer 1)`. If the picture is already cached in the shown buffer it calls `mode_step_next` three times (skips wait, decode and show); if cached in the other buffer, twice (skips wait and decode); otherwise a read was started and nothing is skipped. The calling step adds its own `mode_step_next` | 1 call | C: DATE 181, GEKO 70, SHOUGATU 42, KANGEI 39, SHUGAKU 22, OMIMAI 16 |
| `scene_wait_cd_read` (`func_8007C784`) | waits until `cd_read_sync_status` bit 0 is set, then steps. After 0x402 calls in one step it resets the CD ring (`cd_cmd_reset`) and the background cache (`dec_bg_reset`, keeping `dec_bg_shown[1]`, `dec_bg_last_layer` low byte and `dec_bg_brightness`) and steps back one | until the disc is done | DATE 304, GEKO 144, SHOUGATU 129, SHUGAKU 63, KANGEI 61, DATE2 18, ENDING 13, VALEN 12, OMIMAI 7 |
| `dec_init` | checks the MDEC bitstream header of the read picture in the input buffer `D_80125C04` (unnamed, section 16): the high half of the first word must be `0x3800` and the second word must have a top byte of 0 and bit 16 set (`(word1 & 0xFF010000) == 0x00010000`, the version bit); on a bad header it zeroes the cached LBAs of layer 1 and goes two steps back (re-read); else `dec_bg_decode_to_vram(1)` into the hidden buffer and steps | 1 call | DATE 129, GEKO 56, SHOUGATU 51, SHUGAKU 22, KANGEI 13 |
| `change_dec_bg` | `dec_bg_show_switch(1)`: the decoded picture becomes the shown buffer; steps | 1 call | DATE 94, GEKO 55, SHOUGATU 46, SHUGAKU 19, KANGEI 12 |
| `scene_bg_show` (`func_8007C844`) | `scene_bg_visible = 1`, brightness 0x80; **no caller** (dead code) | 1 | none |
| `scene_bg_hide` (`func_8007C878`) | `scene_bg_visible = 0`, brightness 0 | 1 | SHOUGATU 3, KANGEI 2, GEKO 1 |
| `scene_bg_wipe_in` (`func_8007C8A4` + `func_8007C8D8`) | reveal wipe, section 5 | 16 frames | SHOUGATU 30, GEKO 26, KANGEI 9, SHUGAKU 7, OMIMAI 2 (C: SHUGAKU 2, GEKO 2, DATE 1) |
| `normal_date_bg_out` (+ `func_8007D8AC`) | cover wipe, section 5 | 18 frames | SHOUGATU 35, GEKO 23, SHUGAKU 11, KANGEI 9, OMIMAI 3 |
| `normal_date_bg_fadein`, `normal_date_bg_fadeout` | brightness +4 / -4 per frame (about 32 frames) | 32 | tables DATE 47 / 22, GEKO 8 / 13; C: BUNKASAI 149 / 304, BUNKAKEN 54 / 104, MASTER 9 / 11 |
| `normal_date_bggirl_fadein`, `normal_date_bggirl_fadeout` | the same with the portrait tinted by the same brightness; fade-out hides the portrait when the brightness is below 8 | 32 | tables SHOUGATU 6 / 7, DATE 6 / 34, GEKO 6 / 9; C: BUNKAKEN 184 / 241, BUNKASAI 100 / 39, MASTER 4 / 5 |
| `normal_date_move_place` | change of location: cross-fade from the old to the newly decoded background, section 5 | 18 (1 + 16 + 1) | tables DATE 70, SHOUGATU 9, VALEN 5, GEKO 3, DATE2 3, ENDING 2; C: MASTER 8, SHUGAKU 2 |
| `place_init`, `place_init2` | `k_reset(0)`, `message_window_set_title(scene_place_title)` (`_2`: `scene_place_title_2`), step | 1 | DATE 67, SHOUGATU 34, GEKO 30, SHUGAKU 11, KANGEI 6 |

### 2.2 Portrait, HUD, girl

| step | what it does | users |
|---|---|---|
| `read_bustup`, `read_bustup_uniform`, `read_bustup_swimsuit` (`bustup_return` calls `read_bustup`) | start the CD read of the portrait file of `current_girl`, section 6 | uniform: SHOUGATU 27, GEKO 27, SHUGAKU 12, VALEN 2; casual: DATE 14, GEKO 4, SHOUGATU 1; swimsuit: DATE 2 |
| `portrait_show_checked` (core), `face_show` (`func_80083628`), `face_show_checked_hidden` (`func_800836A0`) | verify (optional) and build the portrait from the loaded file, section 6 | tables SHOUGATU 56, GEKO 34, DATE 16, SHUGAKU 13, KANGEI 8 (`face_show`: DATE2 6, ENDING 3); C: BUNKASAI 182, BUNKAKEN 89, MASTER 10 |
| `normal_date_girl_in`, `normal_date_girl_out`, `normal_date_girl_suddenin`, `normal_date_girl_suddenout` | portrait fade in and out (18 frames each: 1 init + 16 + 1; emulator: 18 calls), or at once, section 5 | tables in / out: DATE 141 / 160, GEKO 30 / 30, DATE2 12 / 11, SHUGAKU 11 / 10; C: BUNKASAI 105 / 208, BUNKAKEN 44 / 95 |
| `face_expression_mood`, `face_expression_0` .. `face_expression_4` (`func_80083338`, `func_80083378` .. `func_80083418`) | set the portrait layout by mood rank or fixed index; the expression interpretation remains HYPOTHESIS | DATE 79 (mood), 12 (0), 15 (1), 14 (2), 24 (3), 4 (4) |
| `vram_bustup_clear` | `actor_clear` 31 and 30, free `tpage_buf[5..9]` | SHOUGATU, GEKO |
| `change_girl` | `current_girl = date_girl` (the girl of the scene) | SHOUGATU 3, GEKO 2 |
| `change_yoshio` | `current_girl = 13` (Yoshio): portrait, names and voice code now address him | SHOUGATU 5, GEKO 3 |
| `yosi_trans` | `audio_bank_start(0, 0x200)`: restart sequence 0 of sound bank 0; used around the Yoshio scenes | SHOUGATU 4, GEKO 2 |
| `hizuke_hide`, `hizuke_appear` | `hizuke_disp_switch(0 / 1)`: the calendar date HUD (actors 16-23) | DATE 45 each |
| `day_plus` | one day forward, section 10 | SHUGAKU 5 |
| `scene_mouth_lock_clear` (`func_80084E6C`) | `scene_mouth_lock = 0` | SHOUGATU |
| `don_init2`, `don_wait` | `screen_shake_start` (the bang effect); wait 0x81 frames, `k_reset(1)` | SHOUGATU 5 / 3, GEKO 4 / 5, DATE 3 / 1 (`don_init2` / `don_wait`) |
| `wait_sub_sub(n)` | `step_timer++`, steps when it reaches `n` (the table parameter) | DATE 12, SHOUGATU 4, GEKO 4, SHUGAKU 2 |

### 2.3 Dialogue and choices

| step | what it does |
|---|---|
| `normal_date_speak` | wires `talk_tables` (group, line, text, command, voice) from the scene's table pointers and runs `talk_run(talk_choice, 1, 0)` once per frame; the group ends with command 0xFF, which steps the scene (section 7) |
| `normal_date_speak_012` | same with choice 0xFF: a three-option line keeps the order 0, 1, 2 (DATE 6 refs, TAIIKU) |
| `normal_date_speak_1line` | runs `talk_run`; steps when it returns 1, so exactly one line per step (OMIMAI, SHOUGATU) |
| `normal_date_two_select`, `normal_date_three_select` | choice menus, section 8 |
| `select_girl`, `select_girl2` | heroine picker, section 8 |

Per-frame helpers that the overlay handlers call outside the step table: `portrait_wink_update` (`func_80083808`, section 6), `scene_frame_update` (`func_80083A10`), `check_k_scroll` (window scroll, section 7), `k_disp_inc2` (typewriter, section 7).

## 3. Scene state

`talk_work` (`D_80122CD0`, type `TalkWork`, at least 0x7C bytes) is the scene's work area. The core table names words 0 and 1 (`scene`, `girl`); `talk_clear` zeroes words 5-30 (+0x14..+0x78) at every scene start, which includes `date_spot` and `rank_override`. Members are named through `TalkWork`; address labels inside this aggregate are not separate data symbols in the reconciled table.

| offset | `TalkWork` field | meaning |
|---|---|---|
| +0x00 | `scene` | scene id of a special event (core) |
| +0x04 | `girl` | girl id of a special event (core) |
| +0x0C | `choice` | last menu pick: heroine index (11 = back), 0/1 of a two-way menu, original option index of a three-way menu; also the reply index the runner uses (311 uses) |
| +0x14 | `line_wait` | 1: a spoken line is on screen and waits for the confirm press; 2: its voice was cancelled |
| +0x1C | `choice_saved` | copy of `choice` kept across a sub-scene (SHOUGATU, DATE) |
| +0x20 | `voice_state` | 1 once the line's voice clip was started; 0xA set by DATE for lines that need no voice wait |
| +0x24 | `girl_visible` | the portrait is on screen |
| +0x28 | `bg_visible` | nonzero: `scene_frame_update` draws background layer 1 |
| +0x2C | `text_delay` | frames before `k_disp_inc2` reveals the next glyph; above 0 it also blocks the speaking mouth |
| +0x30 | `line_auto` | 1: the line ends by itself (codes 0xC, 0x10, 0x11, 0x12); 2: cancelled |
| +0x34 | `event_still_visible` | the full-screen scene still is shown (hides the pad cursor) |
| +0x38 | `date_spot` | date spot id of the current date (core 5.4); 96 uses |
| +0x3C | `event_eyes_hold` | HYPOTHESIS: 1 stops the periodic eye blink of the scene picture |
| +0x40 | `event_mouth_anim` | mouth mode of the scene picture: 0 follows the voice level, 1 periodic animation every 240 frames, 2 neither (section 9) |
| +0x48 | `line_clear` | 1: code 0xF line, cut when text and voice end |
| +0x4C | `text_silent` | the text is only ellipsis or punctuation: no mouth movement |
| +0x50 | `mouth_lock` | HYPOTHESIS: speech animation of the portrait is off |
| +0x58 | `row_offset` | HYPOTHESIS: text rows shift down by 16 per unit; never written except by `talk_clear`, so 0 |
| +0x5C | `cursor_hide` | DATE and DATE2 set it around their own menus |
| +0x60 | `menu_active` | a scene menu is up (pad cursor shown) |
| +0x6C | `wipe_kind` | wipe pattern 0-6 |
| +0x78 | `rank_override` | see `event_log_add`, section 10 |

Words +0x08, +0x10, +0x18, +0x44, +0x54, +0x64, +0x68, +0x70 and +0x74 remain `unk_XX` unless a supported field proposal exists. Most are overlay-only scene-specific state; +0x74 (`D_80122D44`) is also read by the main overlay loader: bit 0 gates the light effect and bit 1 the title banner slide. DATE and GEKO overlays write those bits. Since D44 is inside `TalkWork` at +0x74, it is not a separate data symbol. Its field name is deferred pending coordinated migration of the direct main and overlay references. For +0x10, `D_80122CE0` is the RPG battle launch kind (see the `RpgLaunch` rows of the o-rpgbat table).

Other state owned by this object (all `main`):

| name (old) | meaning |
|---|---|
| `talk_tables` (`D_800CA134`) | one aggregate with five pointers: group cursor, line cursor, text table, command table, voice table; rewritten by every `normal_date_speak*` from the scene's own cells (`talk_group` `D_800CA148`, `talk_line` `D_800CA14C`; GEKO uses a second pair `D_800CA150/154`; text/command/voice base pointers `D_800CA160/164/168`). The +0x04..+0x10 slots are fields of `TalkTables`, not separate data aliases. |
| `name_buf_*`, `girl_name_buf` | buffers of the name codes, section 7.4 |
| `talk_choice_order` (`D_800CA2A4`) | order of the three options, initial 0, 1, 2 |
| `talk_voice_waiting` (`D_800CA2A8`), `talk_voice_wait_frames` (`D_800CA2AC`), `talk_voice_cancelled` (`D_800CA2C4`) | voice wait state, section 7.3 |
| `talk_next_row_y` (`D_800CA2C0`) | y of the row below the last line drawn (continuation lines start there) |
| `talk_text_scroll_pending` (`D_800CA2BC`) | `check_k_scroll` holds a row below the window |
| `select_girl_menu_enabled` (`D_800CA2B0`), `select_girl_menu_x/y/width/height` (`D_800CA300/318/330/348`) | heroine picker data, section 8 |
| `scene_place_title`, `scene_place_title_2` (`D_800CA19C`, `D_800CA1DC`) | 0x40-byte place name buffers |
| `face_images/_frame_tables/_sequences/_palette` (`D_800CA120/124/128/130`) | pointers into the loaded portrait file, section 6 |
| `talk_voice_speaker_by_girl` (`D_800CA36C`) | speaker id per expression index for the voice request: 0-10 identity, 11 -> 9, 12 and 13 -> 0xD, 14 -> 0xE, 15 -> 0xF |
| `scene_mouth_frames_reversed_saved` (`D_800CA360`), `scene_trip_day_state` | HYPOTHESIS names, section 9 and 10; `mouth_frames_reversed` (`D_800B5A60`) is the name of the m-hud table |
| `sd_table_*`, `sd_palette/_image_table/_sequence_table/_texture_blocks` | SD sprite lookup, section 11 |

## 4. Background loading protocol

Backgrounds are MDEC-compressed pictures read from disc into a buffer and decoded into one of two VRAM buffers of layer 1 (a 320 x 160 picture area above the dialogue window; layer geometry is the hardware units' business). The scene's table does, for a new background: a scene function calls `bg_read_sub2(lba)` (the cache logic above), then `scene_wait_cd_read`, `dec_init`, `change_dec_bg`. A failed header check re-reads (there is no retry limit). `dec_bg_brightness` is the tint of the shown background and is what wipes and fades change; `scene_bg_visible` says whether `scene_frame_update` draws the plain background at all (fades and `girl_in/out` draw it themselves through `dec_bg_fade_draw` / `dec_bg_crossfade_draw`).

## 5. Wipes and fades

All counts are calls (frames) and were read from the code; the reveal wipe was also run in the emulator.

| effect | frames | per frame | notes |
|---|---|---|---|
| reveal wipe (`scene_bg_wipe_in`) | 16 | brightness +8 from the current value (0 after `scene_bg_hide`) to 0x80 | kind = month % 7 is stored in `TalkWork.wipe_kind` when `step_timer` is 0; black lines are drawn with `prim_line_gradient` (ordering slot 0xD) and removed progressively; `scene_bg_visible` becomes 2 (kind 6: stays 0 until the end) |
| cover wipe (`normal_date_bg_out`) | 18 | brightness -8 to 0, clamped | the same seven kinds, lines added progressively (kinds 1 and 2 use ordering slot 6); kind 6 clears `scene_bg_visible` at once and the end clears it for every kind |
| background fade in / out | about 32 | brightness +4 / -4 | the step ends at brightness >= 0x7D / < 4 |
| portrait fade in (`normal_date_girl_in`) | 1 init + 16 + 1 finishing | fade amount `dec_bg_fade_amount` +8 from 0 to 0x80; portrait tint and actor 31 brightness follow it | the face sprites 0 and 1 are unhidden on the last call; `scene_bg_visible` = 1 afterwards |
| portrait fade out (`normal_date_girl_out`) | 1 init + 16 + 1 | fade amount -8 from 0x80 | on the last call actors 30 and 31 and sprites 0 and 1 are hidden and their tint restored to 0x80 |
| move place (`normal_date_move_place`) | 1 init + 16 + 1 | fade amount -8, `dec_bg_crossfade_draw(1)` each frame | at the end `dec_bg_show_switch(1)` makes the new picture the shown one, the portrait sprites are hidden |

Wipe kinds (`SceneWipe`, month % 7): 0 diagonal blocks (15 bands 32 px apart, 2 lines per band per frame), 1 centre lines (rows around y = -0x29, 5 lines per frame), 2 blinds (ten bands of horizontal lines 16 px apart, one per band per frame), 3 diagonal sweep (32 lines per frame: the reveal runs from the top-left to the bottom-right, the cover back), 4 hatch (kind 3 with four interleaved line sets, 64 per frame; the kind of April, seen in the emulator as black hatching retreating from the top-left), 5 iris (nested black rectangle outlines, 10 px inset per frame), 6 mosaic (a 32 x 20 grid of 10 x 8 cells from the shown background buffer: strip height `step_timer / 2` on the reveal, `8 - step_timer / 2` on the cover; even columns anchored at the top of the cell, odd columns at the bottom). The port may replace the wipes by engine transitions; the duration (16 and 18 frames at 60 Hz) and the month-keyed choice are the contract.

## 6. Portraits (bust-ups)

**File choice.** `read_bustup` (casual clothes), `read_bustup_uniform` and `read_bustup_swimsuit` start `portrait_read_start(lba)` for the file of `current_girl`: 0x2D sectors from the BUSTUP disc folder to 0x80180000. File k of the folder (164 files, name order) starts at sector `0x4FE5 + 0x2D * k`, so the table below also gives the file names (checked: every sector maps to a name with the girl's code).

- casual clothes: month 3-5 file x10, 6-8 x11, 9-11 x12, otherwise (12, 1, 2) x13; only girls 0-10 and Yoshio (13, one file).
- uniform: month 6-9 file x01 (summer uniform), otherwise x02; Sotoi (14) has one file.
- swimsuit: girls 0-10 one file x14 each; girl 8 has one per calendar year (file A for year 95, B for 96, C otherwise).



Which of the three functions a scene calls is fixed by its table or its C: the uniform function is referenced by the SHOUGATU (27), GEKO (27), SHUGAKU (12), VALEN (2), ENDING (1) and OMIMAI (1) tables and called from C in GEKO and OMIMAI; the casual function by 14 DATE, 4 GEKO and 1 SHOUGATU table entries and by C in DATE (3), GEKO (1) and DATE2 (4); the swimsuit function by two DATE table entries. No rule about the place of the date is coded here. Girls 11 and 12 have no casual or swimsuit file in these functions.

**Build.** `portrait_show_checked` first checks the loaded file for integrity, but only while the main mode is in 0x50-0x6F (the girl scenes): the five image pointers at `face_images` must be 0x801820AC + 0x4000 * k (k = 0..4), `face_sequences` must read 0x8018005C and 0x80180070, `face_frame_tables[0]` must be 0x80180000 and the 0x2D-sector checksum must match (`file_checksum_ok`). Any failure makes the step go two steps back (re-read). Then `portrait_build(palette, images, frame tables, sequences, rank)` runs with `rank = get_g_zyotai_h(current_girl) & 0x7F` (0 for girl ids >= 12). The portrait is built from five image blocks in texture slots 5-9 and has two invisible animation-driver actors, `ACTOR_BUSTUP_MOUTH` (31, sequence slot 0) and `ACTOR_BUSTUP_EYES` (30, sequence slot 1), and six sprite quads, of which sprites 0 and 1 are the 16 x 16 mouth and eye cells (`SPRITE_BUSTUP_MOUTH`, `SPRITE_BUSTUP_EYES`; quads 2-5 are hair and body pieces by the m-hud reading, not checked here). `face_show` is the same without the check; `face_show_checked_hidden` (DATE only) checks pointers and checksum, goes four steps back on failure, and leaves the portrait hidden at brightness 0 for a later fade-in. The disc integrity retry is a hardware-layer concern and can be dropped in a port.

**Expression.** `face_expression_*` and `portrait_layout_set(arg)` select one of five portrait layouts through `portrait_quad_offsets(arg)`, which sets the texture page (7, 8 or 9) and the u offset (0x30 / 0x70 for sprite 0, 0 / 0x40 for sprite 1) of the two face sprites: arg 0 = page 7 low, 1 = page 7 high, 2 = page 8 low, 3 = page 8 high, 4 = page 9 low, 5 = page 9 high. The argument is the mood rank 0 (best) to 4 (`face_expression_mood` takes `get_g_zyotai_h & 0x7F`; the five fixed variants pass 0-4), not a girl id. Sprite 0 is the mouth cell and sprite 1 the eye cell: `portrait_face_update` (m-hud) sets the V offset of sprite 0 from the voice-level thresholds (or the frame of the mouth driver) and that of sprite 1 from the frame of the eye driver. The expression interpretation remains HYPOTHESIS.

**Per-frame driver** (`portrait_wink_update`, `func_80083808`): while `scene_girl_visible` it shows actors 31 and 30 and the face sprites, runs the portrait animation (`func_80063668`) and winks every 240 frames (`frame_count % 240 == 0`, `portrait_wink`); `scene_mouth_lock` zeroes the v offset of sprite 0. While only `scene_event_still_visible` is set it hides the portrait and animates the scene picture by the mouth mode (`scene_event_mouth_anim`, section 9): mode 1 starts the mouth animation (`event_mouth_anim_start`, `func_80067DFC`) once every 240 frames, mode 0 calls `event_mouth_lipsync` (`func_80067C28`) every frame, which sets the mouth frame from the voice level, and mode 2 does neither; the eyes blink (`event_eyes_anim_start`, `func_80067DD4`) every 240 frames unless `scene_event_eyes_hold` is set. Otherwise it hides everything.

**Speaking mouth.** Voiced lines drive the mouth from the loudness of the CD audio (`getCDlevel`, section 11). While `voice_mode == 0` (no voices) `talk_run` animates the mouth instead from the typewriter: for girl lines (codes 1, 2, 5, 0xA, 0xB, 0xC, 0xD) while text is still being typed and `text_delay == 0`, it calls `portrait_speech` (portrait visible, `text_silent` 0, `mouth_lock` 0) or `event_mouth_anim_start` (`func_80067DFC`; scene still visible, `event_mouth_anim` 0, `text_silent` 0). Emulator (GEKO girl-0 scene, frames 700-1000): with `voice_mode` poked to 0 `portrait_speech` ran 35 times and `getCDlevel` never; with the default 2 `getCDlevel` ran 214 times and `portrait_speech` never. The port can map both to "mouth moves while text types or audio is loud".

## 7. Dialogue engine

### 7.1 Data

The script of a scene is three parallel tables the overlay points `D_800CA160/164/168` at: `text[group][line]` (pointer to a Shift-JIS string), `command[group][line]` (one byte, the speak code) and `voice[group][line]` (pointer to a `TalkVoiceRef`: two words `clip_start`, `clip_end`; both zero = no voice; code 2 and 5 lines hold one pair per reply, indexed `choice * 2`). A group is a run of lines closed by a line whose command is 0xFF; `talk_group` and `talk_line` are the cursor (`D_800CA148/14C`). Text bytes are Shift-JIS pairs with a few single ASCII bytes that act as control letters (7.4). **Where the script lives**: in a file of the SERIFU disc folder (45 files: DATE_<girl>, EVENT_<girl>, GEKO, G_APPEAR, G_BIRTH, G_INVITE, SHOGATU, SHUGAKU, XMAS, VALEN, WHITE, OMIMAI, ENDING, EPILOG and others), not in the overlay. The scene reads the file to a fixed RAM address (BUNKAKEN: 5 sectors from sector 0x49BA to 0x801E8000) and the file is linked for that address: Shift-JIS strings first, then the pointer arrays, command byte runs and voice entries, all with absolute pointers. The overlay only stores the three table addresses as constants (BUNKAKEN: text 0x801E92FC, command 0x801E9598, voice 0x801EA298, written to `D_800CA160/164/168` by `func_801324E0`; other scenes have one such setter per script section). Checked by parsing the disc data: groups 0-5 of that file have the command runs [0, FF], [0, 8, FF], [0, FF], [0, 0, FF], [0, FF], [6, FF], exactly the sequences the emulator showed; voice entries are pointers to eight-byte pairs, (0, 0) for no voice. A port needs a dumper for these files (open questions).

Text window geometry (screen centre origin): rows at y = 0x30, 0x40, 0x50 (16 apart; `TalkWork.row_offset` shifts all by 16 per unit); label at x = -0x68 - 4 * (label bytes), so a three-character label starts at -0x80 and ends at the opening quote at x = -0x56; text at x = -0x48; unlabelled narration at x = -0x80; continuation rows at x = -0x48 from `talk_next_row_y`. A line that starts with an opening parenthesis is drawn 14 px further left. Choice menu rows: x = -0x72 text, menu rectangles x = -0x80, width 0xFC, height 16, y = 0x30, 0x40 (0x50).

### 7.2 `talk_run(choice, advance, color)` (`func_80082764`)

One call per frame. `advance` is 1 for every scene step. It is a state machine over `line_auto`, `line_clear`, `line_wait`:

1. `TalkWork.line_auto != 0` (codes 0xC, 0x10, 0x11, 0x12): when the text is complete (`check_end_k`) and the voice is done, clear it and the wait counter. "Voice done" means `TalkWork.voice_state == 0`, or `TalkWork.voice_state == 1` with the CD not playing (`cd_state_get() != 4`), or `voice_mode == 0`. If `voice_mode == 2` and cross is pressed on either pad (`pad_pressed & 0x400040`), send sound command 0xB4 (stop the clip), set `TalkWork.line_auto = 2` and `talk_voice_cancelled`; at 2 the state clears as soon as the text is complete and the CD is quiet.
2. else `line_clear != 0` (code 0xF): when text and voice are done, `k_reset(1)` (cut the list) and clear it.
3. else `TalkWork.line_wait != 0` (the normal spoken line): cross with `voice_mode == 2` cancels the voice (`TalkWork.line_wait = 2`); then, with the text complete and the voice done, circle on either pad (`pad_pressed & 0x200020`) ends the line (`k_reset(1)`, hide the continue marker, clear `TalkWork.voice_state`, return 1). `TalkWork.voice_state == 0xA` also counts as done. After a cancel (`TalkWork.line_wait == 2`) the line ends by itself, without a confirm press, as soon as the text is complete and the CD is quiet. While text and voice are done and the player has not confirmed, the continue marker (actor 2, `text_marker_draw_flags` bit 7) is shown and animated (`func_8006C934`); otherwise it is hidden.
4. else (idle): `step_timer = 0`, call `talk_speak_part(text, command, TalkWork.choice, voice, group, line, color)` for the current line; unless it was waiting for a voice (`talk_voice_waiting`) or returned 0x64 (voice aborted), advance: `line + 1`, or for a command-0xFF line `group + 1`, `line = 0`.
5. After the state machine, when a line is active, text is still typing and `voice_mode == 0`: lip movement as in section 6.

`voice_mode` values (core): 0 off, 1 on, 2 on and cancelable (the new-game default, seen as 2 in the emulator).

### 7.3 `talk_speak_part` (20 command codes)

It copies the line, then switches on the code. Quote and label styles are drawn with `set_kanji_string`; the line text goes through `talk_render`. **Voice**: for voiced codes (1, 2, 5, 7, 0xA, 0xB, 0xC, 0xD, 0xF, 0x12), when the clip pair is nonzero and `voice_mode != 0`: on the first call (`talk_voice_wait_frames == 0`, `TalkWork.line_wait == 0`) it sends `xa_clip_request_set(clip_start, clip_end, speaker)` and sound command 0x300; the line is **not shown** until the CD reports the clip playing (`cd_state_get() & 4`): each call increments the counter, sets `talk_voice_waiting` and returns 0 (the line stays current). After 0x200 calls `talk_voice_skip_check` lets circle or cross cancel the wait; at 0x400 the wait is aborted (the sound command queue is cleared, sound command 0x74 sent, the call returns 0x64 and the line is retried). Then `TalkWork.voice_state = 1` and the line is drawn. The speaker id is `talk_voice_speaker_by_girl[current_girl]` (code 0xB uses 0, code 0xA uses 0xD). Emulator: girl 0's first voiced line requested the clip pair 0x180001C3 / 0x210001D6 with speaker 0; the CD level metering (clip playing) started 6 frames later; `voice_mode` was 2.

**Skip rule.** Codes 0, 1, 0xA, 0xB, 0xC and 0xD test whether the text ends in two ASCII `n` bytes (`nn`, no space). If it does and `choice != 0` the line is not drawn at all (code 1 also skips its voice request; the other codes do not test it); if `choice == 0` the two bytes are cut off and the line is shown. Reply codes 2 and 5 skip a variant that starts with `n`.

| code | name | speaker label | wait | notes |
|---|---|---|---|---|
| 0x00 | `SPEAK_PLAYER` | player family name | confirm | text ending in two ASCII `n` bytes (`nn`, no space) shows only if `choice == 0` (the two bytes are stripped) |
| 0x01 | `SPEAK_GIRL` | girl short name | confirm | voice; same `nn` rule (a hidden line also skips its voice request) |
| 0x02, 0x05 | `SPEAK_GIRL_REPLY`, `_B` | girl short name | confirm | one case in `talk_speak_part` (a difference between 2 and 5, if any, is outside this file); reply variants separated by `q`; variant number `choice` is shown (a variant starting with `n` is skipped); voice pair `choice` |
| 0x03 | `SPEAK_CHOICE_3` | - | menu | options split at `q` or space, shuffled (7.5) unless `choice == 0xFF`; drawn with quote brackets |
| 0x04, 0x06 | `SPEAK_PLAYER_PAREN`, `_B` | player family name | confirm | text in parentheses; one case in `talk_speak_part` (a difference, if any, is outside this file); HYPOTHESIS: the player's inner thought; emulator: the GEKO girl-0 scene opens with code 6 |
| 0x07 | `SPEAK_INLINE` | in the text | confirm | first byte `>` (4-byte label, quote), `-` (6-byte label, quote), `+` (2-byte label, quote), `<` (4-byte label, parenthesised); a short list of fixed sound-effect words and one linking phrase get their own position and colour; optional voice |
| 0x08 | `SPEAK_CHOICE_2` | - | menu | two options, fixed order; emulator: BUNKAKEN group 1 line 1 |
| 0x09 | `SPEAK_EFFECT` | none | none | sound-effect text at x -0x80; the thud word is drawn twice (colour 1, then the line colour) |
| 0x0A | `SPEAK_YOSHIO` | Yoshio | confirm | voice speaker 0xD |
| 0x0B | `SPEAK_SHIORI` | Shiori | confirm | voice speaker 0 |
| 0x0C | `SPEAK_GIRL_AUTO` | girl short name | auto | clears `talk_voice_cancelled` |
| 0x0D | `SPEAK_GIRL_CONTINUE` | blank | confirm | next rows; no voice request while cancelled |
| 0x0E | `SPEAK_PLAYER_PAREN_NOWAIT` | player family name | none | parentheses |
| 0x0F | `SPEAK_GIRL_CLEAR` | girl short name | text and voice end | `line_clear` |
| 0x10 | `SPEAK_PLAIN_AUTO` | none | auto | x -0x80; HYPOTHESIS: narration |
| 0x11 | `SPEAK_CONTINUE_AUTO` | none | auto | next rows |
| 0x12 | `SPEAK_GIRL_CONTINUE_AUTO` | blank | auto | voiced; no request while cancelled |
| 0x13 | `SPEAK_CHOICE_2_PAREN` | - | menu | parenthesis brackets, fixed order |
| 0xFF | `SPEAK_END` | - | - | `mode_step_next` |
| other | - | - | - | `game_mode_reset_to_title` |

Emulator command sequences (group.line = code): GEKO girl-0 scene 1: 0.0 = 6, 0.1 = 0, 0.2 = FF; 1.0..1.9 alternate 1 and 0 (girl, player), 1.10 = FF; 2.x mixes 1, 6, 0, group 3 starts with 6; BUNKAKEN: 0.0 = 0, 0.1 = FF, 1.0 = 0, 1.1 = 8 (two-way choice), 1.2 = FF, then groups 2, 3, 5, 9, 10, 12, 13 with codes 0, 1, FF. DATE opens at group 8 with code 0 lines and uses 7 (inline) and 8.

### 7.4 `talk_render` and the name codes

`talk_render(x, y, color, text, extra, suffix, prefix, is_choice)` copies the text (512-byte buffer), scans it two bytes at a time (single ASCII control letters advance by one), and splits it into rows at ASCII space or `q` (line buffer 64 bytes, rows 16 px apart). Control letters inside a row are replaced by name buffers:

| letter | `NameCode` | buffer |
|---|---|---|
| P | `NAME_CODE_PLAYER_FORM` | `name_buf_player_form`: how the scene girl addresses the player (`get_p_name`) |
| p | `NAME_CODE_PLAYER_FAMILY` | `name_buf_family` |
| m | `NAME_CODE_PLAYER_GIVEN` | `name_buf_given` |
| F | `NAME_CODE_PLAYER_FULL` | family + given name |
| G | `NAME_CODE_GIRL` | `girl_name_buf`: the scene girl's name with honorific (`get_g_name`) |
| Y | `NAME_CODE_YOSHIO` | Yoshio's fixed name |
| S | `NAME_CODE_S` | `name_buf_s`: school-trip destination (SHUGAKU) |
| O | `NAME_CODE_O` | `name_buf_o`: a label from a table (SHOUGATU) |
| W | `NAME_CODE_W` | `name_buf_w`: the entry picked by `TalkWork.choice` (SHOUGATU) |
| A | `NAME_CODE_A` | `name_buf_a`: a gift or technique name (33-byte table records) or club name |
| M | `NAME_CODE_M` | `name_buf_m`: a place or club name (DATE, KANGEI, GEKO) |

`talk_names_set(girl)` (`func_800847B8`, applied core name) fills `name_buf_family`, `name_buf_given` always, and for girl ids below 14 also `name_buf_player_form`, `name_buf_girl_short` (`girl_short_name_table[girl]`) and `girl_name_buf`. With `is_choice`, each row is drawn with the prefix string 14 px left and the suffix string appended. It sets `TalkWork.text_silent` (1 when the whole text is only ellipsis marks, punctuation and control letters), updates `talk_next_row_y = last y + 0x10`, and returns the text-list entry of the last row (choice menus start the typewriter there with `k_disp_start`).

### 7.5 Choices

`SPEAK_CHOICE_3`: unless `choice == 0xFF`, `junban_init` picks the order: first = `frame_count % 3`; the other two follow in an order chosen by `frame_count & 1` (all six permutations occur); `make_three_select` rewrites the text as `option[order[0]] q option[order[1]] q option[order[2]]`. `normal_date_three_select` registers menu 1 with three rows and waits for circle with the text complete; the result is `TalkWork.choice = talk_choice_order[row]`, the original option index. The two-way menu has fixed order and stores 0 or 1. The scene's next step branches on `TalkWork.choice` (BUNKAKEN stores it, the table reads it with `mode_step_set`). Emulator: no scene reached a real three-way line, so a BUNKAKEN two-way line's command byte (RAM 0x801E944D) was poked to 3; `make_three_select` then saw `talk_choice_order` = (1, 2, 0) at `frame_count` 4015 (4015 % 3 = 1, odd), as the rule predicts.

### 7.6 Window scroll and typewriter

`check_k_scroll` (each frame): the text list keeps up to three visible rows (y = 0x30..0x50). The entry after the current one is held hidden and sets `text_scroll_pending` while its y is 0x60 or more (a fourth row). When the typewriter reaches it (it has become the current entry, y still >= 0x60, and a scroll is pending) every earlier entry moves up 0x10 (an entry at y 0x30 is hidden instead) and the current entry is put on row 0x50 and shown. `k_disp_inc2` reveals one glyph per call when `text_delay` has run out, at the fastest speed while circle or cross is held on either pad (`pad_held & 0x600060`).

## 8. Heroine picker

`select_girl` / `select_girl2` (`select_girl_init(arg)`, `select_girl_main(arg)`): a menu of the 11 heroines (girl ids 0-10) shown in a 4 x 3 grid: item i at x = -0x80 + 0x40 * (i / 3), y = 0x30 + 0x10 * (i % 3), width = characters * 14, height 16, only for girls with `girl_menu_enabled[i] != 0` (the others get width 0 and are disabled). `select_girl` adds a back item (index 11) at (0x40, 0x50), width 0x2A; `select_girl2` has none (11 items). The pad cursor starts on the first listed girl. On circle (`pad_pressed & 0x200020`) the first listed item whose hover flag is set is chosen: `talk_choice = i` (11 = back), the cursor is hidden, `scene_menu_active = 0`, `k_reset(1)`, step. The calling scene fills `girl_menu_enabled` before the step: BUNKASAI, BUNKAKEN, MASTER and SHUGAKU copy the girl record flag bit 1 (`met`; SHUGAKU clears girl 10), SHOUGATU copies bit 8 of the flag word (byte 1 bit 0; core names no meaning for it) for slots 0-10. Used by BUNKAKEN stage 1 step 8 (the next step compares the pick with `D_80122EA4` and treats 11 as back), SHOUGATU and SHUGAKU. Emulator: in a new game only girl 0 is listed; the back item and one name were drawn; circle wrote `talk_choice` from `select_girl_main`.

## 9. Event still and event faces

`event_still_show` (`func_80084E90`, core) uploads six 64 x 128 tiles to VRAM and sets sprites 58-63 as a 3 x 2 grid (128 px cells from x -0xA0, y -0x78): a full-screen picture; `event_anim_show` (core) fills actors 96 and 97 as animated layers of it from frame, sequence and texture tables and a mode. `event_still_fade_in` / `event_still_fade_out` (`func_80085368`, `func_800853FC`) fade the picture with the background (+4 / -4 per frame), show and hide the layer with `event_still_layer_set`, set `scene_event_still_visible`, and hide or restore the window title. `event_face0..3` set sequence slots (96: 2, 3, 6, 7; 97: 0, 1, 4, 5), `event_face_kuchi0/1` slots 2/3 of actor 96, `event_kuchi_ani_yes/no` the mouth mode (1 / 0, below) and actor 96's `anim_flags` (0 / 4; bit 0 is "animation running" per the m-hud and m-weekday notes, the bit 2 of the 4 is not explained by this object), `ev_me_on`/`ev_me_on_continue` the eye hold flag and actor 97's `anim_flags` = 5. That 96 is the mouth layer and 97 the eye layer follows from the code, not only from the O.BIN words `kuchi` (mouth) and `ev_me` (eyes): `event_mouth_lipsync` sets the frame of actor 96 from the same voice-level thresholds as the portrait mouth, and `event_eyes_anim_start` restarts the blink of actor 97 every 240 frames (both m-hud functions, called from `portrait_wink_update`). Actors 96 and 97 are also the SD sprites of the week and holiday screens (section 11), so the pool's upper half is scene-owned.

**Mouth mode** (`scene_event_mouth_anim`, `D_80122D10`; the name follows the O.BIN `event_kuchi_ani_yes/no`). Three values, read from `portrait_wink_update` (`func_80083808`), `talk_run` and the writers:

| value | written by | behaviour while the event still is shown |
|---|---|---|
| 0 | `event_still_show`, `event_kuchi_ani_no`, SHUGAKU | with `voice_mode` on, `portrait_wink_update` calls `event_mouth_lipsync` (`func_80067C28`) every frame (while the still is shown and no portrait is): the mouth frame of actor 96 follows the voice level (0-3 thresholds of the current girl, see the m-hud spec). With `voice_mode` 0 that function does nothing; instead `talk_run` calls `event_mouth_anim_start` (`func_80067DFC`) while a girl line without voice is being typed (section 6) |
| 1 | `event_kuchi_ani_yes`, GEKO | `portrait_wink_update` calls `event_mouth_anim_start` once every 240 frames (a periodic mouth animation, independent of the voice) |
| 2 | DATE (two scene steps), GEKO (one) | the library drives neither (what animates the picture then is left to the scene's own steps; not traced) |

Emulator (GEKO girl-0 scene): `event_still_fade_in` ran 32 frames from frame 2370 (picture dim at frame 2380, empty text window, date HUD hidden), `event_still_layer_set` ran once to show it at brightness 4, `event_still_fade_out` ran 32 frames from frame 3572 and the layer was hidden when it ended; the picture is a full-screen portrait of the scene girl.

## 10. Calendar, limits, event log

**`day_plus`** (O.BIN name kept): `trip_day_state = 0`, remember the visibility of the date HUD (actor 16 `draw_flags` bit 7), `calendar_advance_day` (core 3.1: weekday, day, month and year roll over, `day_count`, spot clean-up in months 3, 6, 9, 12, clears `daily_event_bits`, re-sorts appointments), `hizuke_init`, restore the visibility, `hizuke_show`, step. Only SHUGAKU uses it (5 table entries): the school trip advances the calendar one day per call. Emulator (SHUGAKU, mode 0x51 started on 1995-04-04): `day_plus` and `calendar_advance_day` were hit together twice, the calendar going 4/4 -> 4/5 (weekday 1 -> 2, `day_count` 0 -> 1).

**`check_para_limit`** (O.BIN name kept; emulator-checked with out-of-range values): for girls 0-10, on the three 16.16 words `tokimeki` (+0), `affection` (+4) and `syoushin` (+8): if the integer half (signed) is negative the whole word becomes 0; if the integer half of tokimeki or affection is >= 0x80 it becomes 0x80 (the fraction half is kept); syoushin caps at 0xFF (fraction kept). For the nine parameters: negative integer half -> word 0; integer half >= 999 (0x3E7) -> word 0x03E70000 (999.0). Core 5.3 notes that the queue reader later clamps the girl counters to 0x80. Emulator: poked tokimeki 0x150 -> 0x80, affection -11 -> 0, syoushin 0x1F0 -> 0xFF, liberal arts -16 -> 0, stress 0x4000 -> 999.

**`change_girl` / `change_yoshio`**: `current_girl = date_girl` / `current_girl = 13`. Scenes that include Yoshio's lines switch to him (`change_yoshio`), then back (`change_girl`).

**`event_log_add(category, scene)`** (`func_80085B3C`, core): record at `event_log[event_log_count]`: girl = `current_girl`, scene, category, rank, year, month, day, weekday. Rank = `get_g_zyotai_h(current_girl)` (raw 0-4, not masked) unless `rank_override` is 1-6, in which case `rank_override - 1`; the override is cleared on every call; girl 12 always gets rank 0. The count is incremented and clamped to 0xFE, so a full log keeps its first 254 entries and keeps overwriting slot 254 with the newest.

## 11. SD sprites and the CD level meter

**SD (super-deformed) sprites.** The weekday and holiday screens show a small animated character acting out the chosen command (actors 96 and 97). `addr_init_weekly_sd(n)`, `addr_init_holiday_sd(n)`, `addr_init_club_sd(n)`, `addr_init_else_sd(n)` (n is `D_801230D0`, the SD index that the weekday handler computes from the day's command and a 0/1 variant through the tables `D_800B68C0` and `D_800B68D0` (800737A0.c), 0xFF = no SD sprite; the `else` set is called with the fixed 2) look up entry `n % 25`, `% 13`, `% 32`, `% 3` in `sd_table_weekday`, `sd_table_holiday`, `sd_table_club`, `sd_table_else` (three u16 per entry) and publish `sd_palette = 0x801E0000`, `sd_texture_blocks` (entry word 0), `sd_image_table` (word 1) and `sd_sequence_table` (word 2), each plus 0x801E0000. The weekday handler (`func_8007437C` in 800737A0.c) loads the palette with `load_palette(sd_palette, 0x10, 1, 1, 0)` and copies the three pointers into the actor fields of actors 96 and 97. **Data files**: the setup code reads file n of the matching disc folder whole to 0x801E0000 with `cd_read_sectors(0x29, 0x801E0000, sector_table[n])` (the sector tables `D_800B67AC` (`week_day_sd_set_sectors` in the m-weekday table), `D_800B6810` (`club_sd_set_sectors`, m-weekday) and `D_800B69E0` (`holiday_sd_set_sectors`, m-holiday draft) belong to the weekday and holiday code and are not renamed in this table; 0x29 = 41 sectors = 83,968 bytes, the size of every file of MAIN_SD (25 files), CLUB_SD (32) and HOLI_SD (12); the sector tables ascend by 41, so entry n is file n in name order, e.g. MAIN_SD: P00, P00W, P01, P01W ... P05W, P06, P07, P08, P09, P09W ... P0D, P0DW) and the sick set with `cd_read_sectors(9, 0x801E0000, 0x7D8A)` (9 sectors = 18,432 bytes, the size of the three ETC_SD files; HYPOTHESIS: the file of entry 2). Holiday entry 12 (sector 0xA79E) reuses weekday file 12. The `W` files (every second entry) are the paired variants noted above. `addr_init_else_sd(2)` is used while the player is sick on a school-day command. Emulator (school-day command screen, mode 0x40, `D_801230D0` = 1): `addr_init_weekly_sd(1)` ran every 83 frames and left the palette at 0x801E0000 and the pointers at 0x801E0480 (image table), 0x801E04B8 (sequence table) and 0x801E0478 (texture blocks), as the table predicts; the screen shows the player's small figure in the library scene. The offsets are in the header of each loaded file, so the tables are needed only when the original files are read; a port that converts the SD files needs the mapping from command number to file.

Table contents, hex offsets from 0x801E0000 written as image table / sequence table / texture blocks (the order in memory is texture blocks, image table, sequence table); entries repeat in pairs, which suggests two variants of one animation (HYPOTHESIS: the `W` files of the folders).

`sd_table_weekday` (D_800C9F60, 25 entries)

`0: 47C/4B8/474  1: 480/4B8/478  2: 3F8/428/3F0  3: 408/438/400  4: 3D0/3F8/3C8  5: 3D0/3F8/3C8  6: 3A0/3C8/398  7: 3A0/3C8/398  8: 40C/444/400  9: 418/450/40C  10: 458/49C/44C  11: 464/4A8/458  12: 40C/444/3FC  13: 478/4C0/464  14: 404/43C/3F4  15: 2D8/2E8/2D4  16: 2D8/2E8/2D4  17: 434/468/428  18: 448/47C/43C  19: 47C/4B8/470  20: 490/4CC/484  21: 45C/48C/450  22: 45C/48C/450  23: 484/4B4/478  24: 484/4B4/478`

`sd_table_holiday` (D_800C9FF8, 13 entries)

`0: 35C/37C/358  1: 36C/38C/368  2: 30C/324/308  3: 30C/324/308  4: 378/3A0/370  5: 384/3AC/37C  6: 2D8/2E8/2D4  7: 2D8/2E8/2D4  8: 39C/3C0/394  9: 3B0/3D4/3A8  10: 3C8/3E8/3C0  11: 3C8/3E8/3C0  12: 40C/444/3FC`

`sd_table_club` (D_800CA048, 32 entries)

`0: 3B4/3E0/3AC  1: 3B4/3E0/3AC  2: 3B4/3E0/3AC  3: 3B4/3E0/3AC  4: 3A4/3D4/398  5: 3A4/3D4/398  6: 3C0/3F4/3B4  7: 3C0/3F4/3B4  8: 41C/44C/410  9: 41C/44C/410  10: 414/444/408  11: 41C/44C/410  12: 430/468/424  13: 434/46C/428  14: 50C/550/500  15: 50C/550/500  16: 618/664/60C  17: 530/570/524  18: 420/454/414  19: 41C/450/410  20: 384/3AC/378  21: 384/3AC/378  22: 49C/4D8/490  23: 49C/4D8/490  24: 49C/4D8/490  25: 49C/4D8/490  26: 3B0/3E4/3A4  27: 3B0/3E4/3A4  28: 394/3C4/388  29: 394/3C4/388  30: 58C/5BC/580  31: 59C/5CC/590`

`sd_table_else` (D_800CA108, 3 entries)

`0: 3A8/3CC/3A4  1: 3A8/3CC/3A4  2: 32C/338/328`


**CD level meter.** The work area is `audio_cd_level` (`D_80125E60`, type `CdLevelMeter`; the sound-driver table of `m-dates-a` names it and its fields, and this unit uses those names). `SD_GetCDLevel(&audio_cd_level)` runs once per frame from the sound driver while an XA voice plays (`audio_xa_check_end`): it measures only while the top byte of `AudioState.request_flags` (`D_80125D14`, no bank transfer wanted or running) is 0, bit 0x4000 of `AudioState.state_flags` is clear and the SPU transfer calls report idle, and then reads two 0x200-byte SPU capture buffers (SPU RAM 0 and 0x400) into the sample work buffer, computes the peak (`options` method 1) or average (method 2) over `sample_count` evenly spaced sample pairs (doubled), optionally squares the result and applies one of three shaping curves, and stores left and right scaled by `precision_bits` (shift by 16 - `precision_bits`). The emulator showed `precision_bits` 8, `sample_count` 32 and `options` 0x212 (average, no square, no curve). `getCDlevel` returns the mean of left and right while `AudioState.state_flags` bit 0x400 is set, else 0; callers are NAME_ENT (9), GEKO (1), ENDING (1) and the HUD portrait and event mouth code (80062CD0.c, 800674B0.c). A port can replace it by the playing clip's amplitude.

## 12. Hardware layer: modules and entry points

| module | names here | what a port provides |
|---|---|---|
| CD audio level | `SD_GetCDLevel`, `SD_DetectCDPeak`, `SD_CalcCDAve`, `getCDlevel`, `audio_cd_level` | audio amplitude of the playing clip |
| primitives | `set_tarao_sprt`, `set_tarao_rect`, `_sprite_set_box_shade_tarao`, `set_tarao_bg`, `prim_line_gradient` calls | engine drawing for wipes (or engine transitions) |
| sprite tint | `sprite_brightness` (alias `func_80084C98`) | tint of a sprite quad |
| disc reads | `read_bustup*`, `bg_read_sub2`, `scene_wait_cd_read` | asset loading by name (section 6 and Appendix A map sector to file) |
| SD data | `addr_init_*_sd`, `sd_*` tables | SD sprite assets by command |

## 13. Names

The reconciled executable packet `config/renames/proposals/m-dates-b-apply.tsv` has 20 function rows, 29 standalone data rows, and 22 `TalkWork`/`TalkVoiceRef` field rows. It excludes the old proposal's unsupported file, comment and inline constant rows. `m-dates-b-essential.tsv` separately lists 25 essential parameters, 10 locals and 54 constant/value sites; sites that `rename.py const` cannot scope safely are marked unsupported. The 78 O.BIN function names stay unchanged; their 74 English definition comments from the original proposal are not represented by the supported symbol-only row kinds and remain an application follow-up. Names use this object's mapped `face_`, `sd_`, `talk_`, `name_code_`, `scene_` and `select_girl_` families, while existing applied `portrait_*` names remain authoritative. The file is `src/main/scene_library.c`; its declarations will eventually move from `include/main_api.h` to `include/main/scene.h`.

## 14. Notes for the port

- Model a scene as a list of steps (the overlay tables) executed by a small interpreter with the step, `step_timer` and substeps of [[spec/core]]; or translate each table to a coroutine: `load_background`, `wipe_in`, `load_portrait(girl, outfit)`, `fade_in_portrait`, `say(group)`, `choose(...)`, `fade_out`. Keep the set of primitives of section 2; scenes differ only by their tables.
- Keep the talk contract of section 7: three parallel script tables, the 20 speak codes, the `nn` and reply-variant rules, the name codes, line breaks at space and `q`, the voice-before-text rule, confirm and cancel handling, `talk_work.choice` as the link between a menu and the script.
- Portrait outfit rules (season and uniform months, the swimsuit year rule for girl 8), mood rank 0-4 (best to worst) from `get_g_zyotai_h`, and the expression index are game logic; the integrity checks are not.
- The wipe kind comes from the month (`month % 7`); durations are 16 frames (reveal), 18 (cover), about 32 (background fade), 1 + 16 + 1 (portrait fade, move place).
- `check_para_limit` and the event-log rank rule are exact and cheap to port; the parameter cap is 999 and the girl counters 0x80 (syoushin 0xFF here, 0x80 after the queue reader).
- The hardware paths (CD sync, MDEC decode, SPU capture, texture upload) are replaced by the engine.

## 15. Verification log (emulator, 2026-10-24)

All runs started from a boot-to-title save state with memory writes that set the main mode to the overlay loader (`mode = 2`, `overlay_target_mode`) and, for special events, `talk_work.scene` and `.girl`; the weekday screen was entered by writing mode 0x40 directly. The patched runner lives outside the repo (a scratch copy of `tools/emu` with a `--poke FRAME:ADDR:BYTES` option); no repo tool was changed, and no state file or screenshot is committed.

1. GEKO 0x5D, girl 0, scene 1: 22 functions of this object hit in 900 frames; step order, reveal wipe (kind 4 = April % 7, brightness 8..0x78, 16 calls), portrait file sector 0x544A, `check_para_limit` once, `scene_wait_cd_read` 110 calls.
2. Same scene, 5400 frames with confirm pressed every 70 frames: the command sequences of section 7.3, the voice request (clip pair 0x180001C3 / 0x210001D6, speaker 0), `voice_mode` 2, `audio_cd_level` = `precision_bits` 8, `sample_count` 32, `options` 0x212, `audio_state.state_flags` bit 0x400 set while the voice plays, the scene ends with `normal_date_bggirl_fadeout` (32 calls).
3. `check_para_limit` with poked values (section 10).
4. BUNKAKEN 0x71: speak codes 0, 8, FF; the two-way menu waits for circle (frames 373-440); jumping to stage 1 step 8 starts `select_girl_init(0)` with `girl_menu_enabled` = 1 for girl 0 only; circle writes `talk_choice` from `select_girl_main`. Later in the scene: `normal_date_bg_fadein` 96 calls in three uses and `normal_date_girl_in` 18 calls (1 + 16 + 1).
5. DATE 0x50 (girl 0, place 0): speak codes 0, 1, 7, 8, FF (choice 0xFF from `normal_date_speak_012`).
6. SHOUGATU 0x52: `normal_date_bg_out` 36 calls (two uses of 18), `normal_date_girl_in` and `_out` 18 calls each, reveal wipe 16 calls. SHUGAKU 0x51: reveal wipe 48 calls (three uses), cover wipe 36, `day_plus` twice.
7. School-day command screen (mode 0x40): `addr_init_weekly_sd`, section 11.
8. BUNKAKEN with the command byte of group 1 line 1 poked from 8 to 3: the shuffle of section 7.5.
9. Voice cancel: in the GEKO scene a cross press 50 frames after `snd_command(0x300)` (frame 780) sent `snd_command(0xB4)` in the same frame (830) and the line ended on its own 12 frames later (next line requested at frame 842).
10. Lip-sync source: the same scene with `voice_mode` 0 and 2 (section 6).
11. Event still: `event_still_fade_in` and `_fade_out` 32 calls each, `event_still_layer_set` show and hide (section 9).
12. Review round 1 fixes (code reading only, no new emulator run): the mouth mode of section 9 comes from the C of `portrait_wink_update`, `talk_run`, `func_80067C28` and `func_80067DFC` and from every writer of `D_80122D10` in `src/` (event_still_show, the two event_kuchi_ani functions, SHUGAKU, GEKO, DATE); the usage counts of sections 2 and 6 were recomputed from the data asm and the C sources (method in section 2).

Not run: the three-way menu itself (`normal_date_three_select`, `junban_init` rule only), a cover wipe of another kind than the month's (the kind follows the same month % 7), `select_girl2`, `portrait_show_checked_hidden`, the `check_k_scroll` scroll path (no run reached a fourth text row), the holiday, club and else SD loaders.

## 16. Open questions

1. `portrait_layout_set` and `portrait_quad_offsets` (core rows): the argument is an expression index (mood rank 0-4, 5 unused), not a girl id; the core note for `portrait_quad_offsets` says "per expression index". Suggest renaming `portrait_quad_offsets` to `portrait_expression_select` and keeping `portrait_layout_set` as an alias description. Proposed unchanged here (core names are used).
2. Core says `step_timer` is the frames in the current step; in fact only the setters clear it and step functions increment it themselves (section 1).
3. Seven comment rows duplicate core rows (`normal_date_speak`, `_bg_fadein/_fadeout`, `_bggirl_fadein/_fadeout`, `_girl_in`, `_girl_out`); mine are more exact, and the two `girl_in/out` rows correct "slide" to a tint fade.
4. `scene_event_eyes_hold`, `scene_mouth_lock`, `mouth_frames_reversed_saved` (the swap partner of the m-hud name `mouth_frames_reversed`), `trip_day_state` and `portrait_expression_*` are HYPOTHESIS names; the DATE, GEKO and SHOUGATU analysts can confirm them. The meaning of mouth mode 2 (DATE and GEKO event pictures that animate by their own steps) and of bit 2 in the value 4 that `event_kuchi_ani_no` writes to the `anim_flags` of actor 96 are not shown by this object.
5. TalkWork words +0x08, +0x10, +0x18, +0x44, +0x54, +0x64, +0x68, +0x70, +0x74 are scene-specific flags of the overlays and are named by their analysts.
6. `voice_speaker_by_girl`: girl 11 maps to speaker 9 (same as girl 9) and girl 15 to 0xF; meaning of the speaker ids above 10 is not settled (h-cdio notes speakers 0xE and 0xF in NAME_ENT).
7. `voice_state == 0xA` (DATE) and `D_800CA2C8` (40 bytes, only cleared by the runner) are unexplained.
8. The three-way menu was exercised only through a poked command byte (shuffle rule confirmed); the cover-wipe kinds other than the month's were read from code only.
9. Script data: the SERIFU files hold the text, command and voice tables with absolute pointers for their load address (section 7.1); the load address and the three table addresses are constants in each overlay. No extraction tool exists; the port needs one (a Docker tool that, per scene, reads the file, relocates by the load address and dumps `text[group][line]`, `command` and `voice`), and its output must not be committed.
10. SD tables: the file of each entry follows from the three sector tables (checked against the folder sizes); open are the meaning of the paired `W` files (entries repeat in pairs) and which ETC_SD file the sick set reads (sector 0x7D8A; entry 2 is a HYPOTHESIS).
11. `scene_bg_show`, `set_tarao_rect` and `event_face_kuchi0` have no caller; `event_kuchi_ani_yes` and `event_face_kuchi1` have one table entry each.
12. The current main prefix map assigns this object `face_`, `sd_`, `talk_`, `name_code_`, `scene_` and `select_girl_`. This spec and the reconciled table follow those owners; `portrait_*` names already present in the applied core ledger stay authoritative.
13. File name `scene_library.c` should be harmonised with the neighbouring units (`m-dates-a` covers 80079B10.c, which is the sound driver glue, not a scene file, despite the task text).
14. `D_80125C04` is the MDEC input buffer pointer named `bg_still_bitstream` by m-flow; it is set by `func_80053650` and read by `dec_init` and `func_800536AC`. `D_80125D14` is the address alias of `AudioState.request_flags` (the field at +0x04); it is not an independent data symbol. `SD_GetCDLevel` reads the request word top byte.
15. Overlap with the unmerged m-menutext draft: it names `NameCode` 0x50 `NAME_CODE_PLAYER` and 0x47 `NAME_CODE_GIRL` for a second implementation of the name codes (its own text routine); 0x47 agrees, 0x50 is `NAME_CODE_PLAYER_FORM` here because `p`, `m` and `F` are the other player name codes. One name has to be chosen at the merge.
16. Prefix policy follows the ADR map and applied ledger: this object owns the `face_`, `sd_`, `talk_`, `name_code_`, `scene_` and `select_girl_` families; shared `portrait_*` names are kept where the core ledger already established them. `talk_render`, `talk_speak_part`, `TalkWork`, `TalkTables` and `TalkVoiceRef` are the current names. The historical proposal table is not authoritative for these aliases.
17. Mood flag pair: `D_800B5A60` is named `mouth_frames_reversed` by m-hud (the ENDING lip sync inverts the mouth level while it is set); this table names only its swap partner `D_800CA360`. DATE sets the partner to 1 before an event picture; what the inverted mouth frames mean for the DATE pictures is not shown.

## Appendix A: portrait files by girl

Sector in hex in parentheses; casual spring, summer, autumn, winter; uniform summer, winter; swimsuit.

| id | girl | casual | uniform | swimsuit |
|---|---|---|---|---|
| 0 | Shiori | HU10 (55B2) HU11 (55DF) HU12 (560C) HU13 (5639) | HU01 (541D) HU02 (544A) | HU14 (5666) |
| 1 | Mio | KI10 (5D9B) KI11 (5DC8) KI12 (5DF5) KI13 (5E22) | KI01 (5D14) KI02 (5D41) | KI14 (5E4F) |
| 2 | Yuina | HI10 (5288) HI11 (52B5) HI12 (52E2) HI13 (530F) | HI01 (5201) HI02 (522E) | HI14 (533C) |
| 3 | Ayako | KA10 (5909) KA11 (5936) KA12 (5963) KA13 (5990) | KA01 (5882) KA02 (58AF) | KA14 (59BD) |
| 4 | Saki | NI10 (6719) NI11 (6746) NI12 (6773) NI13 (67A0) | NI01 (6692) NI02 (66BF) | NI14 (67CD) |
| 5 | Yukari | KO10 (6011) KO11 (603E) KO12 (606B) KO13 (6098) | KO01 (5F8A) KO02 (5FB7) | KO14 (60C5) |
| 6 | Nozomi | KY10 (6287) KY11 (62B4) KY12 (62E1) KY13 (630E) | KY01 (6200) KY02 (622D) | KY14 (633B) |
| 7 | Mira | KG10 (5B52) KG11 (5B7F) KG12 (5BAC) KG13 (5BD9) | KG01 (5AF8) KG02 (5B25) | KG14 (5C06) |
| 8 | Yuko | AS10 (503F) AS11 (506C) AS12 (5099) AS13 (50C6) | AS01 (4FE5) AS02 (5012) | AS14A (50F3) AS14B (5120) AS14C (514D) |
| 9 | Megumi | MI10 (64FD) MI11 (652A) MI12 (6557) MI13 (6584) | MI01 (64A3) MI02 (64D0) | MI14 (65B1) |
| 10 | Yumi | SA10 (698F) SA11 (69BC) SA12 (69E9) SA13 (6A16) | SA01 (6908) SA02 (6935) | SA14 (6A43) |
| 11 | Miharu | - | TA01 (6B51) TA02 (6B7E) | - |
| 12 | Rei | - | IJ01 (57FB) IJ02 (5828) | - |
| 13 | Yoshio | YO11 (6C8C) | YO01 (6C32) YO02 (6C5F) | - |
| 14 | Sotoi | - | SO16 (6B24) | - |

## Appendix B: function index

All 111 symbols of the object by address (the alias `func_80084C98` of `sprite_brightness` is not listed). "kept" = the existing name stays; rows from other proposal tables show the unit that names them.

| address | current name | proposed | role |
|---|---|---|---|
| 8007C030 | `SD_GetCDLevel` | kept (O.BIN) | Measure the CD-audio output level: reads 0x200-byte SPU capture buffers at SPU RAM 0 and 0x400, takes peak or average of N sample pairs,... |
| 8007C310 | `SD_DetectCDPeak` | kept (O.BIN) | Peak mode of the CD level meter: scans N evenly spaced samples of the left and right capture buffers and keeps the largest absolute... |
| 8007C414 | `SD_CalcCDAve` | kept (O.BIN) | Average mode of the CD level meter: mean absolute value of N evenly spaced samples of each capture buffer, doubled. |
| 8007C51C | `getCDlevel` | kept (O.BIN) | Return the mean of the left and right CD-audio level while metering is enabled (audio_state bit 0x400), else 0 |
| 8007C560 | `addr_init_weekly_sd` | kept (O.BIN) | Point the SD sprite palette, frame table, sequence table and texture-block pointers at entry n (mod 25) of the school-day SD table (n =... |
| 8007C5CC | `addr_init_holiday_sd` | kept (O.BIN) | Same for the holiday SD table (13 entries, n mod 13). |
| 8007C638 | `addr_init_club_sd` | kept (O.BIN) | Same for the club-activity SD table (32 entries, n mod 32). |
| 8007C6A8 | `addr_init_else_sd` | kept (O.BIN) | Same for the third table (3 entries, n mod 3) |
| 8007C740 | `addr_init_bustup` | kept (O.BIN) | Set the portrait sprite addresses for the current girl |
| 8007C784 | `func_8007C784` | `scene_wait_cd_read` | step: mode_step_next once cd_read_sync_status bit 0 is set |
| 8007C844 | `func_8007C844` | `scene_bg_show` | scene_bg_visible = 1, dec_bg_brightness = 0x80, mode_step_next |
| 8007C878 | `func_8007C878` | `scene_bg_hide` | scene_bg_visible = 0, dec_bg_brightness = 0, mode_step_next |
| 8007C8A4 | `func_8007C8A4` | `scene_bg_wipe_in` | step: calls scene_bg_wipe_in_step every frame and steps when it returns 1 |
| 8007C8D8 | `func_8007C8D8` | `scene_bg_wipe_in_step` | one frame of the reveal wipe: kind = month % 7 chosen when step_timer == 0, brightness +8 per frame up to 0x80, black prim_line_gradient... |
| 8007D384 | `set_tarao_bg` | kept (O.BIN) | Draw the mosaic wipe: 32 columns by 20 rows of strips from the shown background buffer, each n lines tall, even columns anchored at the... |
| 8007D680 | `set_tarao_sprt` | kept (O.BIN) | Queue one textured strip sprite (x, y, width, height, u, v, ordering slot, rgb) in the primitive arena of the active draw buffer. |
| 8007D788 | `set_tarao_rect` | kept (O.BIN) | Queue one flat rectangle (x, y, w, h, ordering slot, rgb) in the primitive arena of the active draw buffer. |
| 8007D878 | `normal_date_bg_out` | kept (O.BIN) | Scene step: run the cover wipe each frame (scene_bg_wipe_out_step) and step on when it finishes. |
| 8007D8AC | `func_8007D8AC` | `scene_bg_wipe_out_step` | one frame of the cover wipe called by normal_date_bg_out: same seven kinds drawn from the other end, brightness -8 per frame to 0,... |
| 8007E390 | `normal_date_girl_in` | kept (O.BIN) | Scene step: dispatch on step_timer to the portrait fade-in (init, then main), then draw the faded background layer. |
| 8007E3F8 | `normal_date_girl_in_init` | kept (O.BIN) | Portrait fade-in, first frame: zero the portrait brightness, the sprite tints and the fade amount, mark the portrait visible, switch the... |
| 8007E468 | `normal_date_girl_in_main` | kept (O.BIN) | Portrait fade-in, later frames: raise the fade amount by 8 per frame to 0x80 (16 frames) and copy it to the portrait tint |
| 8007E530 | `normal_date_girl_suddenin` | kept (O.BIN) | Scene step: show the portrait at full brightness at once. |
| 8007E5A0 | `normal_date_girl_out` | kept (O.BIN) | Scene step: dispatch on step_timer to the portrait fade-out (init, then main), then draw the faded background layer. |
| 8007E608 | `normal_date_girl_out_init` | kept (O.BIN) | Portrait fade-out, first frame: fade amount 0x80, plain background draw off. |
| 8007E634 | `normal_date_girl_out_main` | kept (O.BIN) | Portrait fade-out, later frames: lower the fade amount by 8 per frame (16 frames) and tint the portrait with it |
| 8007E75C | `normal_date_girl_suddenout` | kept (O.BIN) | Scene step: hide the portrait at once. |
| 8007E81C | `normal_date_bg_fadein` | kept (O.BIN) | Raise the background brightness by 4 per frame to 0x80 (about 32 frames), then step on |
| 8007E884 | `normal_date_bggirl_fadein` | kept (O.BIN) | Fade in background and portrait together: bg_fadein plus portrait actor and sprite tint following the brightness. |
| 8007E934 | `normal_date_bg_fadeout` | kept (O.BIN) | Lower the background brightness by 4 per frame to 0, then hide it and step on. |
| 8007E99C | `normal_date_bggirl_fadeout` | kept (O.BIN) | Fade out background and portrait together |
| 8007EAAC | `normal_date_move_place` | kept (O.BIN) | Scene step: change of location, dispatch on step_timer to init, then main. |
| 8007EB0C | `normal_date_move_place_init` | kept (O.BIN) | Move-place first frame: clear the text list, fade amount 0x80, plain background draw off. |
| 8007EB5C | `normal_date_move_place_main` | kept (O.BIN) | Move-place later frames: cross-fade from the old to the newly decoded background (fade amount -8 per frame, 16 frames), tint the... |
| 8007EC68 | `place_init` | kept (O.BIN) | Scene step: clear the text list and show scene_place_title in the message window title, then step on. |
| 8007EC9C | `place_init2` | kept (O.BIN) | Same as place_init with the second title buffer scene_place_title_2. |
| 8007ECD0 | `change_dec_bg` | kept (O.BIN) | Scene step: swap the shown buffer of background layer 1 so the freshly decoded background appears, then step on. |
| 8007ECF8 | `dec_init` | kept (O.BIN) | Scene step: check the MDEC header of the loaded background (first word: high half 0x3800; second word: top byte 0 and bit 16 set, the version bit) |
| 8007ED84 | `bg_read_sub2` | kept (O.BIN) | Background read helper: starts the read of the scene background |
| 8007EDF8 | `check_k_scroll` | kept (O.BIN) | Per-frame dialogue window scroll: when the next text row would fall below the third row (y >= 0x60) hold it, then move all earlier rows... |
| 8007EF04 | `wait_sub_sub` | kept (O.BIN) | Scene step with a parameter: wait n frames (step_timer counts up), then step on |
| 8007EF48 | `talk_speak_part` | kept (applied core name) | Speak one script line: copy the text, switch on the command byte (20 codes) to pick label, quote style, voice request and wait mode,... |
| 80081128 | `func_80081128` | `talk_voice_skip_check` | called while a line waits for its voice to start: once `talk_voice_wait_frames` > 0x200 a circle or cross press can cancel the wait |
| 80081190 | `talk_render` | kept (applied core name) | Split a script string into lines and queue them with name-code expansion |
| 80082764 | `func_80082764` | `talk_run` (applied core name) | 64 uses: the talk runner every script section goes through (reads group/line/command tables, waits for text end and voice, handles 20... |
| 80082D58 | `make_three_select` | kept (O.BIN) | Shuffle the three options of a choice line: split the text at q or space separators and rejoin it in the order `talk_choice_order`. |
| 80082EB4 | `junban_init` | kept (O.BIN) | Pick a random order of the three options from frame_count (first = frame_count % 3, the other two ordered by its parity) and store it in... |
| 80082F70 | `normal_date_three_select` | kept (O.BIN) | Scene step: three-option choice menu, dispatch on step_timer to init, then main. |
| 80082FD0 | `normal_date_three_select_init` | kept (O.BIN) | Register menu 1 with three 252x16 rows at x -0x80, y 0x30/0x40/0x50, cursor on row 0, scene_menu_active = 1. |
| 80083078 | `normal_date_three_select_main` | kept (O.BIN) | Run the menu |
| 80083174 | `normal_date_two_select` | kept (O.BIN) | Scene step: two-option choice menu, dispatch on step_timer to init, then main. |
| 800831D4 | `normal_date_two_select_init` | kept (O.BIN) | Register menu 1 with two 252x16 rows at x -0x80, y 0x30 and 0x40, cursor on row 0, scene_menu_active = 1. |
| 80083268 | `normal_date_two_select_main` | kept (O.BIN) | Run the menu |
| 80083338 | `func_80083338` | `face_expression_mood` | HYPOTHESIS: portrait layout from the current girl's mood rank, then step |
| 80083378 | `func_80083378` | `face_expression_0` | HYPOTHESIS: layout index 0, then step |
| 800833A0 | `func_800833A0` | `face_expression_1` | HYPOTHESIS: layout index 1, then step |
| 800833C8 | `func_800833C8` | `face_expression_2` | HYPOTHESIS: layout index 2, then step |
| 800833F0 | `func_800833F0` | `face_expression_3` | HYPOTHESIS: layout index 3, then step |
| 80083418 | `func_80083418` | `face_expression_4` | HYPOTHESIS: layout index 4, then step |
| 80083440 | `func_80083440` | `portrait_layout_set` (applied core name) | 178 uses: selects the portrait layout through per-girl quad offsets and actor setup |
| 80083474 | `func_80083474` | `portrait_show_checked` (applied core name) | 299 uses: re-verifies the loaded portrait and checksum, retries the step 3 back on mismatch, then builds the portrait |
| 80083628 | `func_80083628` | `face_show` | builds the loaded portrait using the current girl's mood rank, then advances |
| 800836A0 | `func_800836A0` | `face_show_checked_hidden` | checks image pointers/checksum, builds hidden at zero brightness, and retries on invalid data |
| 80083808 | `func_80083808` | `portrait_wink_update` (applied core name) | 45 uses: toggles portrait visibility bits and winks every 240 frames |
| 80083A10 | `func_80083A10` | `scene_frame_update` | per-frame helper every scene handler calls: dec_bg_draw(1) while scene_bg_visible, shows the pad cursor (actor 0) when a direction... |
| 80083AD4 | `hizuke_hide` | kept (O.BIN) | Scene step: hide the calendar date HUD (actors 16-23), then step on. |
| 80083AFC | `hizuke_appear` | kept (O.BIN) | Scene step: show the calendar date HUD, then step on. |
| 80083B24 | `read_bustup` | kept (O.BIN) | Start the CD read of the girl's casual-clothes portrait for the current season (months 3-5, 6-8, 9-11, else winter) into 0x80180000,... |
| 80084150 | `read_bustup_uniform` | kept (O.BIN) | Start the CD read of the girl's school-uniform portrait (months 6-9 summer uniform, else winter uniform), then step on. |
| 80084524 | `read_bustup_swimsuit` | kept (O.BIN) | Start the CD read of the girl's swimsuit portrait (girl 8 has one file per year), then step on. |
| 80084658 | `bustup_return` | kept (O.BIN) | Alias of read_bustup (the same code, reached through a second entry). |
| 80084678 | `return_step` | kept (O.BIN) | Restore the main mode, stage and step saved in saved_mode, saved_stage and saved_step by calling main_state_set, main_stage_set and... |
| 800846C0 | `k_disp_inc2` | kept (O.BIN) | Typewriter step with fast-forward: counts `talk_text_delay` down and reveals one glyph per call |
| 80084738 | `yosi_trans` | kept (O.BIN) | Scene step: restart sequence 0 of sound bank 0 (audio_bank_start(0, 0x200)), then step on |
| 80084764 | `change_yoshio` | kept (O.BIN) | Scene step: set current_girl to 13 (Yoshio) so portrait, name and voice code address him, then step on. |
| 8008478C | `change_girl` | kept (O.BIN) | Scene step: set current_girl back to date_girl (the girl of the scene), then step on. |
| 800847B8 | `func_800847B8` | `talk_names_set` (applied core name) | 246 uses: fills the name buffers for name codes: family and given name from GameState, the player-addressing form, the girl's short and full name |
| 8008484C | `select_girl` | kept (O.BIN) | Scene step: heroine picker with a back button, dispatch on step_timer to init(0), then main(0). |
| 800848AC | `select_girl2` | kept (O.BIN) | Same picker without the back button (menu of 11 items). |
| 8008490C | `select_girl_init` | kept (O.BIN) | Lay the heroine picker out: 11 names in a 4x3 grid (x -0x80 + 0x40 per column, y 0x30 + 0x10 per row, item width = characters * 14) for... |
| 80084B9C | `select_girl_main` | kept (O.BIN) | Run the picker |
| 80084C98 | `sprite_brightness` | kept (O.BIN) | Set the red, green and blue tint of sprite quad idx to val. |
| 80084CC8 | `day_plus` | kept (O.BIN) | Scene step: advance the calendar by one day (calendar_advance_day), rebuild the date HUD keeping its visibility, clear trip_day_state,... |
| 80084D3C | `check_para_limit` | kept (O.BIN) | Clamp girl tokimeki/affection (0..0x80), syoushin (0..0xFF) and the nine parameters (0..999) |
| 80084E4C | `func_80084E4C` | `text_marker_hide` (core) | 113 uses: text_marker_set(0) |
| 80084E6C | `func_80084E6C` | `scene_mouth_lock_clear` | HYPOTHESIS: scene_mouth_lock = 0 then mode_step_next |
| 80084E90 | `func_80084E90` | `scene_still_show` (applied core name) | uploads six 0x40 x 0x80 tiles to VRAM and sets SpriteQuads 58-63 as a 3 x 2 grid of one full-screen scene still |
| 800850D4 | `func_800850D4` | `scene_anim_show` (applied core name) | fills actors 96/97 as a full-screen animated picture from frame/sequence/tpage tables and a mode |
| 80085234 | `func_80085234` | `scene_still_layer_set` | shows or hides the still layer: actors 96/97 and sprites 58-63 |
| 80085368 | `func_80085368` | `scene_still_fade_in` | step_timer-driven still fade-in; shows the layer at first brightness |
| 800853FC | `func_800853FC` | `scene_still_fade_out` | fades the still out, hides its layer and restores the title |
| 80085498 | `event_face0` | kept (O.BIN) | Scene step: event picture face 0: sequence slot 2 on actor 96, slot 0 on actor 97. |
| 800854C8 | `event_face1` | kept (O.BIN) | Event face 1: slot 3 on actor 96, slot 1 on actor 97. |
| 800854FC | `event_face2` | kept (O.BIN) | Event face 2: slot 6 on actor 96, slot 4 on actor 97. |
| 80085530 | `event_face3` | kept (O.BIN) | Event face 3: slot 7 on actor 96, slot 5 on actor 97. |
| 80085564 | `event_face_kuchi0` | kept (O.BIN) | Event mouth 0: slot 2 on actor 96 only. |
| 8008558C | `event_face_kuchi1` | kept (O.BIN) | Event mouth 1: slot 3 on actor 96 only. |
| 800855B4 | `event_kuchi_ani_no` | kept (O.BIN) | Event mouth mode 0: scene_event_mouth_anim = 0 (mouth follows the voice level) and actor 96 anim_flags = 4. |
| 800855E4 | `event_kuchi_ani_yes` | kept (O.BIN) | Event mouth mode 1: scene_event_mouth_anim = 1 (periodic mouth animation every 240 frames) and actor 96 anim_flags = 0. |
| 80085614 | `ev_me_on` | kept (O.BIN) | Event eyes on: scene_event_eyes_hold = 0 (periodic blink allowed) and actor 97 anim_flags = 5. |
| 80085644 | `ev_me_on_continue` | kept (O.BIN) | Event eyes on and held: scene_event_eyes_hold = 1 (no periodic blink) and actor 97 anim_flags = 5. |
| 80085678 | `_sprite_set_box_shade_tarao` | kept (O.BIN) | Queue a four-corner gradient box (x, y, w, h, ordering slot, two packed rgb colours) in the primitive arena. |
| 8008585C | `func_8008585C` | `talk_clear` (applied core name) | 89 uses: clears TalkWork words 5..30, re-initialises and hides the cursor actor, empties the text list and blanks the window title |
| 800858E8 | `don_init2` | kept (O.BIN) | Scene step: start the screen shake (screen_shake_start), then step on. |
| 80085910 | `don_wait` | kept (O.BIN) | Scene step: wait 0x81 frames, clear the text list and step on. |
| 80085958 | `normal_date_speak_1line` | kept (O.BIN) | Scene step: wire the dialogue tables and run exactly one line of the script |
| 800859E8 | `normal_date_speak_012` | kept (O.BIN) | Scene step: run the dialogue with choice 0xFF, so a three-option line keeps the order 0, 1, 2. |
| 80085A60 | `normal_date_speak` | kept (O.BIN) | Scene step: wire the talk tables and run one frame of the talk runner with `talk_work.choice` as the reply index |
| 80085ADC | `vram_bustup_clear` | kept (O.BIN) | Scene step: reset actors 30 and 31 and free the five portrait texture slots 5-9, then step on. |
| 80085B3C | `func_80085B3C` | `event_log_add` (core) | appends (girl, scene, category, rank, year, month, day, weekday) to event_log |
| 80085CD4 | `func_80085CD4` | `portrait_quad_offsets` (core) | HYPOTHESIS: sets fields +0x0C/+0x0E of SpriteQuads 0 and 1 per expression index (texture offsets of the two portrait quads) |
