---
type: spec
updated: 2026-10-24
sources: ["src/main/8007C030.c", "include/main_api.h", "wiki/spec/core.md", "wiki/emulator.md", "config/renames/proposals/m-dates-b.tsv"]
---

# Port spec: main scene library (m-dates-b)

Unit `m-dates-b` of phase 2 ([[tickets/T-27015-p2-m-dates-b]], parent [[tickets/T-27000-phase-2]]): the main-exe object `src/main/8007C030.c` (0x8007C030-0x80085E30, 111 symbols). The task text called it "girls, day advance and limits"; those are 6 of its functions. The object is the shared **scene step library**: the building blocks that every event scene (date, walk home, New Year, school trip, culture festival, ending ...) strings together, plus the dialogue engine they all use. Names come from `config/renames/proposals/m-dates-b.tsv` (258 rows); shared core names follow [[spec/core]], hardware names follow the `h-graphics` and `h-cdio` tables (the hardware table wins, see core section 17). Old names are given in backticks where a reader needs the link to the current code.

Scope (user decision of 2026-10-24): game logic, state and flow are described fully; the PS1 hardware layer (GPU primitives, sprite tables, VRAM, sound driver, CD/XA, MDEC) is named at module level (section 12). No game text is quoted: strings are described by role.

How facts were obtained: the matched C of this object, its callers and the step tables in the overlay asm data, and the emulator (section 15: about twenty short runs from a boot-to-title save state into the GEKO special-event scene, a BUNKAKEN culture-festival scene and a DATE scene, using memory writes to enter the scene). A claim of game meaning that neither the code nor a run proves is marked HYPOTHESIS.

## 1. What the library is

Every event scene overlay is a **table of function pointers indexed by `step`** (`GameState.step`, `unk_110A`). The overlay's mode handler picks the table (by girl and scene id) and calls `table[step](n)` once per frame; the call parameter `n` is one constant per table (0x20 in KANGEI, 0x80 in the GEKO girl-0 scene) and only `wait_sub_sub` reads it. A step function does its work for one frame and calls `main_step_next` when it is finished (a step that is not finished simply returns and is called again next frame), or branches with `main_step_set(k)`. Steps that must be repeated after a failed disc read step *back*: `main_step_next` followed by `step -= 3` is a net retry two steps earlier. This object holds the step functions that all tables share (the O.BIN names `normal_date_*`, `place_init`, `dec_init`, `hizuke_*`, `select_girl*`, ...) and the data they share. The overlay step tables reference them hundreds of times (for example the DATE tables hold `scene_wait_cd_read` 304 times, `normal_date_girl_out` 160, `dec_init` 129); BUNKASAI and BUNKAKEN call `normal_date_speak` 663 and 384 times from C.

The scene state is a per-scene work area (`dialogue_work`, section 3) and the shared `GameState` fields of [[spec/core]]. `step_timer` (`unk_1104`) is **not** advanced by the main loop: the setters clear it and the step functions that need a counter increment it themselves (wipes, `wait_sub_sub`, `scene_wait_cd_read`, `don_wait`). The two-phase steps (`normal_date_girl_in`, `normal_date_girl_out`, `normal_date_move_place`, `normal_date_two_select`, `normal_date_three_select`, `select_girl`, `select_girl2`) use it as the phase: 0 runs the init half, which sets it to 1, 1 runs the main half, anything else calls `game_reset_to_title` (the usual error path).

A typical scene start, as observed in the emulator (GEKO special event, girl 0, scene 1; frame numbers are frames of the run, the scene's first frame is 64; the table calls `f[step](0x80)`):

| frame | step function | what it does |
|---|---|---|
| 64 | setup | `dialogue_names_set(girl)`, `dialogue_clear`, `addr_init_bustup`, `check_para_limit`, `text_marker_hide` |
| 208 | `scene_wait_cd_read` | waits for an earlier CD read |
| 253 | `event_still_show`, `event_anim_show` | set up the event picture layers |
| 272 | `bg_read_sub2(lba)` | start loading the background picture |
| 291, 293 | `dec_init`, `change_dec_bg` | check and decode the picture, show it |
| 294 | `read_bustup_uniform` | start loading the girl's portrait (winter uniform in April: sector 0x544A) |
| 327 | `bustup_show_checked` | verify the loaded file and build the portrait |
| 329-344 | `scene_bg_wipe_in` | reveal wipe, 16 frames, kind 4 (April % 7) |
| 345 | `place_init` | place title into the message window |
| 347 | `normal_date_speak` -> `dialogue_run` | the script starts |

The same order (read, wait, decode, show; portrait read, wait, build; wipe; title; speak) is the skeleton of every table.

## 2. Step reference

Names: proposed new name (current name). "Table refs" counts `.word` references in the overlay data asm; "C" counts calls from C in overlays.

### 2.1 Background

| step | what it does | duration | users |
|---|---|---|---|
| `bg_read_sub2(lba)` (helper, O.BIN name kept) | `cd_read_checked = 0`, then `dec_bg_cd_read(lba, layer 1)`. If the picture is already cached in the shown buffer it calls `main_step_next` three times (skips wait, decode and show); if cached in the other buffer, twice (skips wait and decode); otherwise a read was started and nothing is skipped. The calling step adds its own `main_step_next` | 1 call | C: DATE 99, GEKO 63, SHOUGATU 40, OMIMAI 16 |
| `scene_wait_cd_read` (`func_8007C784`) | waits until `cd_read_sync_status` bit 0 is set, then steps. After 0x402 calls in one step it resets the CD ring (`cd_cmd_reset`) and the background cache (`dec_bg_reset`, keeping `dec_bg_shown[1]`, `dec_bg_last_layer` low byte and `dec_bg_brightness`) and steps back one | until the disc is done | DATE 304, GEKO 144, SHOUGATU 129, KANGEI 61, SHUGAKU 63 |
| `dec_init` | checks the MDEC bitstream header of the read picture (first word `0x3800....`, second word version bits `0x01....`); on a bad header it forgets the cached LBAs of layer 1 and goes two steps back (re-read); else `dec_bg_decode_to_vram(1)` into the hidden buffer and steps | 1 call | DATE 129, SHOUGATU 51, GEKO 56 |
| `change_dec_bg` | `dec_bg_show_switch(1)`: the decoded picture becomes the shown buffer; steps | 1 call | DATE 94, GEKO 55, SHOUGATU 46 |
| `scene_bg_show` (`func_8007C844`) | `scene_bg_visible = 1`, brightness 0x80; **no caller** (dead code) | 1 | none |
| `scene_bg_hide` (`func_8007C878`) | `scene_bg_visible = 0`, brightness 0 | 1 | KANGEI, SHOUGATU, GEKO |
| `scene_bg_wipe_in` (`func_8007C8A4` + `func_8007C8D8`) | reveal wipe, section 5 | 16 frames | SHOUGATU 30, KANGEI 9 |
| `normal_date_bg_out` (+ `func_8007D8AC`) | cover wipe, section 5 | 18 frames | SHOUGATU 35, SHUGAKU 11, KANGEI 9 |
| `normal_date_bg_fadein`, `normal_date_bg_fadeout` | brightness +4 / -4 per frame (about 32 frames) | 32 | BUNKASAI 130 / 268 C calls, MASTER, DATE |
| `normal_date_bggirl_fadein`, `normal_date_bggirl_fadeout` | the same with the portrait tinted by the same brightness; fade-out hides the portrait when the brightness is below 8 | 32 | DATE 34 (out), SHOUGATU, GEKO |
| `normal_date_move_place` | change of location: cross-fade from the old to the newly decoded background, section 5 | 18 (1 + 16 + 1) | DATE2, VALEN, SHOUGATU, ENDING |
| `place_init`, `place_init2` | `k_reset(0)`, `message_window_set_title(scene_place_title)` (`_2`: `scene_place_title_2`), step | 1 | SHOUGATU 34, SHUGAKU 11 |

### 2.2 Portrait, HUD, girl

| step | what it does | users |
|---|---|---|
| `read_bustup`, `read_bustup_uniform`, `read_bustup_swimsuit` (`bustup_return` calls `read_bustup`) | start the CD read of the portrait file of `current_girl`, section 6 | SHOUGATU 27 (uniform), DATE (swimsuit 2) |
| `bustup_show_checked` (`func_80083474`), `bustup_show` (`func_80083628`), `bustup_show_checked_hidden` (`func_800836A0`) | verify (optional) and build the portrait from the loaded file, section 6 | SHOUGATU 56, GEKO 34, MASTER, BUNKASAI 182 C |
| `normal_date_girl_in`, `normal_date_girl_out`, `normal_date_girl_suddenin`, `normal_date_girl_suddenout` | portrait fade in and out (18 frames each: 1 init + 16 + 1; emulator: 18 calls), or at once, section 5 | DATE 160 (out), DATE2 12 (in), GEKO |
| `bustup_expression_mood`, `bustup_expression_0` .. `bustup_expression_4` (`func_80083338`, `func_80083378` .. `func_80083418`) | set the face cells of the portrait by mood rank (HYPOTHESIS: expression) | DATE 79 (mood), 14 (2), 4 (0) |
| `vram_bustup_clear` | `actor_clear` 31 and 30, free `tpage_buf[5..9]` | SHOUGATU, GEKO |
| `change_girl` | `current_girl = date_girl` (the girl of the scene) | SHOUGATU 3, GEKO 2 |
| `change_yoshio` | `current_girl = 13` (Yoshio): portrait, names and voice code now address him | SHOUGATU 5, GEKO 3 |
| `yosi_trans` | `bgm_play(0, 0x200)`: restart sequence 0 of sound bank 0; used around the Yoshio scenes | SHOUGATU 4, GEKO 2 |
| `hizuke_hide`, `hizuke_appear` | `hizuke_disp_switch(0 / 1)`: the calendar date HUD (actors 16-23) | DATE 45 each |
| `day_plus` | one day forward, section 10 | SHUGAKU 5 |
| `scene_mouth_lock_clear` (`func_80084E6C`) | `scene_mouth_lock = 0` | SHOUGATU |
| `don_init2`, `don_wait` | `screen_shake_start` (the bang effect); wait 0x81 frames, `k_reset(1)` | SHOUGATU, DATE, GEKO |
| `wait_sub_sub(n)` | `step_timer++`, steps when it reaches `n` (the table parameter) | SHOUGATU 4, GEKO 4 |

### 2.3 Dialogue and choices

| step | what it does |
|---|---|
| `normal_date_speak` | wires `dialogue_tables` (group, line, text, command, voice) from the scene's table pointers and runs `dialogue_run(dialogue_choice, 1, 0)` once per frame; the group ends with command 0xFF, which steps the scene (section 7) |
| `normal_date_speak_012` | same with choice 0xFF: a three-option line keeps the order 0, 1, 2 (DATE 6 refs, TAIIKU) |
| `normal_date_speak_1line` | runs `dialogue_run`; steps when it returns 1, so exactly one line per step (OMIMAI, SHOUGATU) |
| `normal_date_two_select`, `normal_date_three_select` | choice menus, section 8 |
| `select_girl`, `select_girl2` | heroine picker, section 8 |

Per-frame helpers that the overlay handlers call outside the step table: `bustup_wink_update` (`func_80083808`, section 6), `scene_frame_update` (`func_80083A10`), `check_k_scroll` (window scroll, section 7), `k_disp_inc2` (typewriter, section 7).

## 3. Scene state

`dialogue_work` (`D_80122CD0`, type `DialogueWork`, at least 0x7C bytes) is the scene's work area. The core table names words 0 and 1 (`scene`, `girl`); `dialogue_clear` zeroes words 5-30 (+0x14..+0x78) at every scene start, which includes `date_spot` and `rank_override`. The same words are also reachable through alias labels (`D_80122CDC` ...) that the C declares separately; both names are proposed.

| offset | field | alias label | meaning |
|---|---|---|---|
| +0x00 | `scene` | `D_80122CD0` | scene id of a special event (core) |
| +0x04 | `girl` | `D_80122CD4` | girl id of a special event (core) |
| +0x0C | `choice` | `dialogue_choice` | last menu pick: heroine index (11 = back), 0/1 of a two-way menu, original option index of a three-way menu; also the reply index the runner uses (311 uses) |
| +0x14 | `line_wait` | `dialogue_line_wait` | 1: a spoken line is on screen and waits for the confirm press; 2: its voice was cancelled |
| +0x1C | `choice_saved` | `dialogue_choice_saved` | copy of `choice` kept across a sub-scene (SHOUGATU, DATE) |
| +0x20 | `voice_state` | `dialogue_voice_state` | 1 once the line's voice clip was started; 0xA set by DATE for lines that need no voice wait |
| +0x24 | `girl_visible` | `scene_girl_visible` | the portrait is on screen |
| +0x28 | `bg_visible` | `scene_bg_visible` | nonzero: `scene_frame_update` draws background layer 1 |
| +0x2C | `text_delay` | `dialogue_text_delay` | frames before `k_disp_inc2` reveals the next glyph; above 0 it also blocks the speaking mouth |
| +0x30 | `line_auto` | `dialogue_line_auto` | 1: the line ends by itself (codes 0xC, 0x10, 0x11, 0x12); 2: cancelled |
| +0x34 | `event_still_visible` | `scene_event_still_visible` | the full-screen event still is shown (hides the pad cursor) |
| +0x38 | `date_spot` | `D_80122D08` | date spot id of the current date (core 5.4); 96 uses |
| +0x3C | `event_eyes_hold` | `scene_event_eyes_hold` | HYPOTHESIS: 1 stops the periodic eye blink of the event picture |
| +0x40 | `event_mouth_anim` | `scene_event_mouth_anim` | 1: event mouth animated every 240 frames |
| +0x48 | `line_clear` | - | 1: code 0xF line, cut when text and voice end |
| +0x4C | `text_silent` | `dialogue_text_silent` | the text is only ellipsis or punctuation: no mouth movement |
| +0x50 | `mouth_lock` | `scene_mouth_lock` | HYPOTHESIS: speech animation of the portrait is off |
| +0x58 | `row_offset` | - | HYPOTHESIS: text rows shift down by 16 per unit; never written except by `dialogue_clear`, so 0 |
| +0x5C | `cursor_hide` | `scene_cursor_hide` | DATE and DATE2 set it around their own menus |
| +0x60 | `menu_active` | `scene_menu_active` | a scene menu is up (pad cursor shown) |
| +0x6C | `wipe_kind` | `scene_wipe_kind` | wipe pattern 0-6 |
| +0x78 | `rank_override` | `event_log_rank_override` | see `event_log_add`, section 10 |

Words +0x08, +0x10, +0x18, +0x44, +0x54, +0x64, +0x68, +0x70, +0x74 are used only by overlays (scene-specific flags; for example +0x10, `D_80122CE0`, is the RPG battle launch kind, see the `RpgLaunch` rows of the o-rpgbat table) and keep `unk_XX`.

Other state owned by this object (all `main`):

| name (old) | meaning |
|---|---|
| `dialogue_tables` (`D_800CA134`) and aliases `dialogue_tables_line/_text/_command/_voice` (`D_800CA138-144`) | five pointers: group cursor, line cursor, text table, command table, voice table; rewritten by every `normal_date_speak*` from the scene's own cells (`dialogue_group` `D_800CA148`, `dialogue_line` `D_800CA14C`; GEKO uses a second pair `D_800CA150/154`; text/command/voice base pointers `D_800CA160/164/168`) |
| `name_buf_*`, `girl_name_buf` | buffers of the name codes, section 7.4 |
| `dialogue_choice_order` (`D_800CA2A4`) | order of the three options, initial 0, 1, 2 |
| `dialogue_voice_waiting` (`D_800CA2A8`), `dialogue_voice_wait_frames` (`D_800CA2AC`), `dialogue_voice_cancelled` (`D_800CA2C4`) | voice wait state, section 7.3 |
| `dialogue_next_row_y` (`D_800CA2C0`) | y of the row below the last line drawn (continuation lines start there) |
| `text_scroll_pending` (`D_800CA2BC`) | `check_k_scroll` holds a row below the window |
| `girl_menu_enabled` (`D_800CA2B0`), `girl_menu_x/y/w/h` (`D_800CA300/318/330/348`) | heroine picker data, section 8 |
| `scene_place_title`, `scene_place_title_2` (`D_800CA19C`, `D_800CA1DC`) | 0x40-byte place name buffers |
| `bustup_images/_frame_tables/_sequences/_palette` (`D_800CA120/124/128/130`) | pointers into the loaded portrait file, section 6 |
| `voice_speaker_by_girl` (`D_800CA36C`) | speaker id per girl id for the voice request: 0-10 identity, 11 -> 9, 12 and 13 -> 0xD, 14 -> 0xE, 15 -> 0xF |
| `event_anim_flag`, `event_anim_flag_saved`, `trip_day_state` | HYPOTHESIS names, section 9 and 10 |
| `sd_table_*`, `sd_palette/_image_table/_sequence_table/_texture_blocks` | SD sprite lookup, section 11 |

## 4. Background loading protocol

Backgrounds are MDEC-compressed pictures read from disc into a buffer and decoded into one of two VRAM buffers of layer 1 (a 320 x 160 picture area above the dialogue window; layer geometry is the hardware units' business). The scene's table does, for a new background: a scene function calls `bg_read_sub2(lba)` (the cache logic above), then `scene_wait_cd_read`, `dec_init`, `change_dec_bg`. A failed header check re-reads (there is no retry limit). `dec_bg_brightness` is the tint of the shown background and is what wipes and fades change; `scene_bg_visible` says whether `scene_frame_update` draws the plain background at all (fades and `girl_in/out` draw it themselves through `dec_bg_fade_draw` / `dec_bg_crossfade_draw`).

## 5. Wipes and fades

All counts are calls (frames) and were read from the code; the reveal wipe was also run in the emulator.

| effect | frames | per frame | notes |
|---|---|---|---|
| reveal wipe (`scene_bg_wipe_in`) | 16 | brightness +8 from the current value (0 after `scene_bg_hide`) to 0x80 | kind = month % 7 is stored in `scene_wipe_kind` when `step_timer` is 0; black lines are drawn with `prim_line_gradient` (ordering slot 0xD) and removed progressively; `scene_bg_visible` becomes 2 (kind 6: stays 0 until the end) |
| cover wipe (`normal_date_bg_out`) | 18 | brightness -8 to 0, clamped | the same seven kinds, lines added progressively (kinds 1 and 2 use ordering slot 6); kind 6 clears `scene_bg_visible` at once and the end clears it for every kind |
| background fade in / out | about 32 | brightness +4 / -4 | the step ends at brightness >= 0x7D / < 4 |
| portrait fade in (`normal_date_girl_in`) | 1 init + 16 + 1 finishing | fade amount `dec_bg_fade_amount` +8 from 0 to 0x80; portrait tint and actor 31 brightness follow it | the face sprites 0 and 1 are unhidden on the last call; `scene_bg_visible` = 1 afterwards |
| portrait fade out (`normal_date_girl_out`) | 1 init + 16 + 1 | fade amount -8 from 0x80 | on the last call actors 30 and 31 and sprites 0 and 1 are hidden and their tint restored to 0x80 |
| move place (`normal_date_move_place`) | 1 init + 16 + 1 | fade amount -8, `dec_bg_crossfade_draw(1)` each frame | at the end `dec_bg_show_switch(1)` makes the new picture the shown one, the portrait sprites are hidden |

Wipe kinds (`SceneWipe`, month % 7): 0 diagonal blocks (15 bands 32 px apart, 2 lines per band per frame), 1 centre lines (rows around y = -0x29, 5 lines per frame), 2 blinds (ten bands of horizontal lines 16 px apart, one per band per frame), 3 diagonal sweep (32 lines per frame: the reveal runs from the top-left to the bottom-right, the cover back), 4 hatch (kind 3 with four interleaved line sets, 64 per frame; the kind of April, seen in the emulator as black hatching retreating from the top-left), 5 iris (nested black rectangle outlines, 10 px inset per frame), 6 mosaic (a 32 x 20 grid of 10 x 8 cells from the shown background buffer: strip height `step_timer / 2` on the reveal, `8 - step_timer / 2` on the cover; even columns anchored at the top of the cell, odd columns at the bottom). The port may replace the wipes by engine transitions; the duration (16 and 18 frames at 60 Hz) and the month-keyed choice are the contract.

## 6. Portraits (bust-ups)

**File choice.** `read_bustup` (casual clothes), `read_bustup_uniform` and `read_bustup_swimsuit` start `bustup_read_start(lba)` for the file of `current_girl`: 0x2D sectors from the BUSTUP disc folder to 0x80180000. File k of the folder (164 files, name order) starts at sector `0x4FE5 + 0x2D * k`, so the table below also gives the file names (checked: every sector maps to a name with the girl's code).

- casual clothes: month 3-5 file x10, 6-8 x11, 9-11 x12, otherwise (12, 1, 2) x13; only girls 0-10 and Yoshio (13, one file).
- uniform: month 6-9 file x01 (summer uniform), otherwise x02; Sotoi (14) has one file.
- swimsuit: girls 0-10 one file x14 each; girl 8 has one per calendar year (file A for year 95, B for 96, C otherwise).



Which of the three functions a scene calls is fixed by its table or its C: the uniform function is referenced by the SHOUGATU (27), SHUGAKU (12), VALEN (2) and ENDING (1) tables and called from C in GEKO and OMIMAI; the casual function by one SHOUGATU table entry and C in DATE and GEKO; the swimsuit function by two DATE table entries. No rule about the place of the date is coded here. Girls 11 and 12 have no casual or swimsuit file in these functions.

**Build.** `bustup_show_checked` first checks the loaded file for integrity, but only while the main mode is in 0x50-0x6F (the girl scenes): the five image pointers at `bustup_images` must be 0x801820AC + 0x4000 * k (k = 0..4), `bustup_sequences` must read 0x8018005C and 0x80180070, `bustup_frame_tables[0]` must be 0x80180000 and the 0x2D-sector checksum must match (`file_checksum_ok`). Any failure makes the step go two steps back (re-read). Then `bustup_build(palette, images, frame tables, sequences, rank)` runs with `rank = get_g_zyotai_h(current_girl) & 0x7F` (0 for girl ids >= 12). The portrait is two actors (31 and 30, sequence slots 0 and 1) built from five image blocks in texture slots 5-9, plus two 16 x 16 face-cell sprites (sprite slots 0 and 1). `bustup_show` is the same without the check; `bustup_show_checked_hidden` (DATE only) checks pointers and checksum, goes four steps back on failure, and leaves the portrait hidden at brightness 0 for a later fade-in. The disc integrity retry is a hardware-layer concern and can be dropped in a port.

**Expression.** `bustup_expression_*` and `bustup_layout_set(arg)` call `bustup_quad_offsets(arg)`, which sets the texture page (7, 8 or 9) and the u offset (0x30 / 0x70 for sprite 0, 0 / 0x40 for sprite 1) of the two face sprites: arg 0 = page 7 low, 1 = page 7 high, 2 = page 8 low, 3 = page 8 high, 4 = page 9 low, 5 = page 9 high. The argument is the mood rank 0 (best) to 4 (`bustup_expression_mood` takes `get_g_zyotai_h & 0x7F`; the five fixed variants pass 0-4), not a girl id (the core note says "per girl id"; see open questions). HYPOTHESIS: the sprites are the eye and mouth cells of the face.

**Per-frame driver** (`bustup_wink_update`, `func_80083808`): while `scene_girl_visible` it shows actors 31 and 30 and the face sprites, runs the portrait animation (`func_80063668`) and winks every 240 frames (`frame_count % 240 == 0`, `bustup_wink`); `scene_mouth_lock` zeroes the v offset of sprite 0. While only `scene_event_still_visible` is set it hides the portrait and animates the event picture: mouth every 240 frames when `scene_event_mouth_anim` is 1 (or a static mouth when 0), eyes blink every 240 frames unless `scene_event_eyes_hold`. Otherwise it hides everything.

**Speaking mouth.** Voiced lines drive the mouth from the loudness of the CD audio (`getCDlevel`, section 11). While `voice_mode == 0` (no voices) `dialogue_run` animates the mouth instead from the typewriter: for girl lines (codes 1, 2, 5, 0xA, 0xB, 0xC, 0xD) while text is still being typed and `text_delay == 0`, it calls `bustup_speech` (portrait visible, `text_silent` 0, `mouth_lock` 0) or `func_80067DFC` (event still visible, `event_mouth_anim` 0, `text_silent` 0). The port can map both to "mouth moves while text types or audio is loud".

## 7. Dialogue engine

### 7.1 Data

The script of a scene is three parallel tables the overlay points `D_800CA160/164/168` at: `text[group][line]` (pointer to a Shift-JIS string), `command[group][line]` (one byte, the speak code) and `voice[group][line]` (pointer to a `DialogueVoiceRef`: two words `clip_start`, `clip_end`; both zero = no voice; code 2 and 5 lines hold one pair per reply, indexed `choice * 2`). A group is a run of lines closed by a line whose command is 0xFF; `dialogue_group` and `dialogue_line` are the cursor (`D_800CA148/14C`). Text bytes are Shift-JIS pairs with a few single ASCII bytes that act as control letters (7.4). **Where the script lives**: in a file of the SERIFU disc folder (45 files: DATE_<girl>, EVENT_<girl>, GEKO, G_APPEAR, G_BIRTH, G_INVITE, SHOGATU, SHUGAKU, XMAS, VALEN, WHITE, OMIMAI, ENDING, EPILOG and others), not in the overlay. The scene reads the file to a fixed RAM address (BUNKAKEN: 5 sectors from sector 0x49BA to 0x801E8000) and the file is linked for that address: Shift-JIS strings first, then the pointer arrays, command byte runs and voice entries, all with absolute pointers. The overlay only stores the three table addresses as constants (BUNKAKEN: text 0x801E92FC, command 0x801E9598, voice 0x801EA298, written to `D_800CA160/164/168` by `func_801324E0`; other scenes have one such setter per script section). Checked by parsing the disc data: groups 0-5 of that file have the command runs [0, FF], [0, 8, FF], [0, FF], [0, 0, FF], [0, FF], [6, FF], exactly the sequences the emulator showed; voice entries are pointers to eight-byte pairs, (0, 0) for no voice. A port needs a dumper for these files (open questions).

Text window geometry (screen centre origin): rows at y = 0x30, 0x40, 0x50 (16 apart; `row_offset` shifts all by 16 per unit); label at x = -0x68 - 4 * (label bytes), so a three-character label starts at -0x80 and ends at the opening quote at x = -0x56; text at x = -0x48; unlabelled narration at x = -0x80; continuation rows at x = -0x48 from `dialogue_next_row_y`. A line that starts with an opening parenthesis is drawn 14 px further left. Choice menu rows: x = -0x72 text, menu rectangles x = -0x80, width 0xFC, height 16, y = 0x30, 0x40 (0x50).

### 7.2 `dialogue_run(choice, advance, color)` (`func_80082764`)

One call per frame. `advance` is 1 for every scene step. It is a state machine over `line_auto`, `line_clear`, `line_wait`:

1. `line_auto != 0` (codes 0xC, 0x10, 0x11, 0x12): when the text is complete (`check_end_k`) and the voice is done, clear it and the wait counter. "Voice done" means `voice_state == 0`, or `voice_state == 1` with the CD not playing (`cd_state_get() != 4`), or `voice_mode == 0`. If `voice_mode == 2` and cross is pressed on either pad (`pad_pressed & 0x400040`), send sound command 0xB4 (stop the clip), set `line_auto = 2` and `dialogue_voice_cancelled`; at 2 the state clears as soon as the text is complete and the CD is quiet.
2. else `line_clear != 0` (code 0xF): when text and voice are done, `k_reset(1)` (cut the list) and clear it.
3. else `line_wait != 0` (the normal spoken line): cross with `voice_mode == 2` cancels the voice (`line_wait = 2`); then, with the text complete and the voice done, circle on either pad (`pad_pressed & 0x200020`) ends the line (`k_reset(1)`, hide the continue marker, clear `voice_state`, return 1). `voice_state == 0xA` also counts as done. After a cancel (`line_wait == 2`) the line ends by itself, without a confirm press, as soon as the text is complete and the CD is quiet. While text and voice are done and the player has not confirmed, the continue marker (actor 2, `text_marker_draw_flags` bit 7) is shown and animated (`func_8006C934`); otherwise it is hidden.
4. else (idle): `step_timer = 0`, call `dialogue_speak_part(text, command, choice, voice, group, line, color)` for the current line; unless it was waiting for a voice (`dialogue_voice_waiting`) or returned 0x64 (voice aborted), advance: `line + 1`, or for a command-0xFF line `group + 1`, `line = 0`.
5. After the state machine, when a line is active, text is still typing and `voice_mode == 0`: lip movement as in section 6.

`voice_mode` values (core): 0 off, 1 on, 2 on and cancelable (the new-game default, seen as 2 in the emulator).

### 7.3 `dialogue_speak_part` (`dialogue_speak_part`, 20 command codes)

It copies the line, then switches on the code. Quote and label styles are drawn with `set_kanji_string`; the line text goes through `dialogue_render`. **Voice**: for voiced codes (1, 2, 5, 7, 0xA, 0xB, 0xC, 0xD, 0xF, 0x12), when the clip pair is nonzero and `voice_mode != 0`: on the first call (`dialogue_voice_wait_frames == 0`, `line_wait == 0`) it sends `xa_clip_request_set(clip_start, clip_end, speaker)` and sound command 0x300; the line is **not shown** until the CD reports the clip playing (`cd_state_get() & 4`): each call increments the counter, sets `dialogue_voice_waiting` and returns 0 (the line stays current). After 0x200 calls `dialogue_voice_skip_check` lets circle or cross cancel the wait; at 0x400 the wait is aborted (the sound command queue is cleared, sound command 0x74 sent, the call returns 0x64 and the line is retried). Then `voice_state = 1` and the line is drawn. The speaker id is `voice_speaker_by_girl[current_girl]` (code 0xB uses 0, code 0xA uses 0xD). Emulator: girl 0's first voiced line requested the clip pair 0x180001C3 / 0x210001D6 with speaker 0; the CD level metering (clip playing) started 6 frames later; `voice_mode` was 2.

| code | name | speaker label | wait | notes |
|---|---|---|---|---|
| 0x00 | `SPEAK_PLAYER` | player family name | confirm | text ending in `n n` shows only if `choice == 0` (the marker is stripped) |
| 0x01 | `SPEAK_GIRL` | girl short name | confirm | voice; same `n n` rule |
| 0x02, 0x05 | `SPEAK_GIRL_REPLY`, `_B` | girl short name | confirm | reply variants separated by `q`; variant number `choice` is shown (a variant starting with `n` is skipped); voice pair `choice` |
| 0x03 | `SPEAK_CHOICE_3` | - | menu | options split at `q` or space, shuffled (7.5) unless `choice == 0xFF`; drawn with quote brackets |
| 0x04, 0x06 | `SPEAK_THOUGHT`, `_B` | player family name | confirm | in parentheses; emulator: the GEKO girl-0 scene opens with code 6 |
| 0x07 | `SPEAK_INLINE` | in the text | confirm | first byte `>` (4-byte label, quote), `-` (6-byte label, quote), `+` (2-byte label, quote), `<` (4-byte label, parenthesised); a short list of fixed sound-effect words and one linking phrase get their own position and colour; optional voice |
| 0x08 | `SPEAK_CHOICE_2` | - | menu | two options, fixed order; emulator: BUNKAKEN group 1 line 1 |
| 0x09 | `SPEAK_EFFECT` | none | none | sound-effect text at x -0x80; the thud word is drawn twice (colour 1, then the line colour) |
| 0x0A | `SPEAK_YOSHIO` | Yoshio | confirm | voice speaker 0xD |
| 0x0B | `SPEAK_SHIORI` | Shiori | confirm | voice speaker 0 |
| 0x0C | `SPEAK_GIRL_AUTO` | girl short name | auto | clears `dialogue_voice_cancelled` |
| 0x0D | `SPEAK_GIRL_CONTINUE` | blank | confirm | next rows; no voice request while cancelled |
| 0x0E | `SPEAK_THOUGHT_NOWAIT` | player family name | none | parentheses |
| 0x0F | `SPEAK_GIRL_CLEAR` | girl short name | text and voice end | `line_clear` |
| 0x10 | `SPEAK_NARRATION_AUTO` | none | auto | x -0x80 |
| 0x11 | `SPEAK_CONTINUE_AUTO` | none | auto | next rows |
| 0x12 | `SPEAK_GIRL_CONTINUE_AUTO` | blank | auto | voiced; no request while cancelled |
| 0x13 | `SPEAK_CHOICE_2_THOUGHT` | - | menu | parenthesis brackets |
| 0xFF | `SPEAK_END` | - | - | `main_step_next` |
| other | - | - | - | `game_reset_to_title` |

Emulator command sequences (group.line = code): GEKO girl-0 scene 1: 0.0 = 6, 0.1 = 0, 0.2 = FF; 1.0..1.9 alternate 1 and 0 (girl, player), 1.10 = FF; 2.x mixes 1, 6, 0, group 3 starts with 6; BUNKAKEN: 0.0 = 0, 0.1 = FF, 1.0 = 0, 1.1 = 8 (two-way choice), 1.2 = FF, then groups 2, 3, 5, 9, 10, 12, 13 with codes 0, 1, FF. DATE opens at group 8 with code 0 lines and uses 7 (inline) and 8.

### 7.4 `dialogue_render` and the name codes

`dialogue_render(x, y, color, text, extra, suffix, prefix, is_choice)` copies the text (512-byte buffer), scans it two bytes at a time (single ASCII control letters advance by one), and splits it into rows at ASCII space or `q` (line buffer 64 bytes, rows 16 px apart). Control letters inside a row are replaced by name buffers:

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
| W | `NAME_CODE_W` | `name_buf_w`: the entry picked by `dialogue_choice` (SHOUGATU) |
| A | `NAME_CODE_A` | `name_buf_a`: a gift or technique name (33-byte table records) or club name |
| M | `NAME_CODE_M` | `name_buf_m`: a place or club name (DATE, KANGEI, GEKO) |

`dialogue_names_set(girl)` (`func_800847B8`, core) fills `name_buf_family`, `name_buf_given` always, and for girl ids below 14 also `name_buf_player_form`, `name_buf_girl_short` (`girl_short_name_table[girl]`) and `girl_name_buf`. With `is_choice`, each row is drawn with the prefix string 14 px left and the suffix string appended. The function sets `dialogue_text_silent` (1 when the whole text is only ellipsis marks, punctuation and control letters), `dialogue_next_row_y = last y + 0x10`, and returns the text-list entry of the last row (choice menus start the typewriter there with `k_disp_start`).

### 7.5 Choices

`SPEAK_CHOICE_3`: unless `choice == 0xFF`, `junban_init` picks the order: first = `frame_count % 3`; the other two follow in an order chosen by `frame_count & 1` (all six permutations occur); `make_three_select` rewrites the text as `option[order[0]] q option[order[1]] q option[order[2]]`. `normal_date_three_select` registers menu 1 with three rows and waits for circle with the text complete; the result is `dialogue_choice = dialogue_choice_order[row]`, the original option index. The two-way menu has fixed order and stores 0 or 1. The scene's next step branches on `dialogue_choice` (BUNKAKEN stores it, the table reads it with `main_step_set`). Emulator: no scene reached a real three-way line, so a BUNKAKEN two-way line's command byte (RAM 0x801E944D) was poked to 3; `make_three_select` then saw `dialogue_choice_order` = (1, 2, 0) at `frame_count` 4015 (4015 % 3 = 1, odd), as the rule predicts.

### 7.6 Window scroll and typewriter

`check_k_scroll` (each frame): the text list keeps up to three visible rows (y = 0x30..0x50). The entry after the current one is held hidden and sets `text_scroll_pending` while its y is 0x60 or more (a fourth row). When the typewriter reaches it (it has become the current entry, y still >= 0x60, and a scroll is pending) every earlier entry moves up 0x10 (an entry at y 0x30 is hidden instead) and the current entry is put on row 0x50 and shown. `k_disp_inc2` reveals one glyph per call when `text_delay` has run out, at the fastest speed while circle or cross is held on either pad (`pad_held & 0x600060`).

## 8. Heroine picker

`select_girl` / `select_girl2` (`select_girl_init(arg)`, `select_girl_main(arg)`): a menu of the 11 heroines (girl ids 0-10) shown in a 4 x 3 grid: item i at x = -0x80 + 0x40 * (i / 3), y = 0x30 + 0x10 * (i % 3), width = characters * 14, height 16, only for girls with `girl_menu_enabled[i] != 0` (the others get width 0 and are disabled). `select_girl` adds a back item (index 11) at (0x40, 0x50), width 0x2A; `select_girl2` has none (11 items). The pad cursor starts on the first listed girl. On circle (`pad_pressed & 0x200020`) the first listed item whose hover flag is set is chosen: `dialogue_choice = i` (11 = back), the cursor is hidden, `scene_menu_active = 0`, `k_reset(1)`, step. The calling scene fills `girl_menu_enabled` before the step: BUNKASAI, BUNKAKEN, MASTER and SHUGAKU copy the girl record flag bit 1 (`met`; SHUGAKU clears girl 10), SHOUGATU copies bit 8 of the flag word (byte 1 bit 0; core names no meaning for it) for slots 0-10. Used by BUNKAKEN stage 1 step 8 (the next step compares the pick with `D_80122EA4` and treats 11 as back), SHOUGATU and SHUGAKU. Emulator: in a new game only girl 0 is listed; the back item and one name were drawn; circle wrote `dialogue_choice` from `select_girl_main`.

## 9. Event still and event faces

`event_still_show` (`func_80084E90`, core) uploads six 64 x 128 tiles to VRAM and sets sprites 58-63 as a 3 x 2 grid (128 px cells from x -0xA0, y -0x78): a full-screen picture; `event_anim_show` (core) fills actors 96 and 97 as animated layers of it from frame, sequence and texture tables and a mode. `event_still_fade_in` / `event_still_fade_out` (`func_80085368`, `func_800853FC`) fade the picture with the background (+4 / -4 per frame), show and hide the layer with `event_still_layer_set`, set `scene_event_still_visible`, and hide or restore the window title. `event_face0..3` set sequence slots (96: 2, 3, 6, 7; 97: 0, 1, 4, 5), `event_face_kuchi0/1` slots 2/3 of actor 96, `event_kuchi_ani_yes/no` the mouth animation flag and actor 96's `anim_flags`, `ev_me_on`/`ev_me_on_continue` the eye hold flag and actor 97's `anim_flags` = 5. The O.BIN words `kuchi` (mouth) and `ev_me` (eyes) support reading 96 as the mouth layer and 97 as the eye layer; HYPOTHESIS for the actor assignment. Actors 96 and 97 are also the SD sprites of the week and holiday screens (section 11), so the pool's upper half is scene-owned.

## 10. Calendar, limits, event log

**`day_plus`** (O.BIN name kept): `trip_day_state = 0`, remember the visibility of the date HUD (actor 16 `draw_flags` bit 7), `calendar_advance_day` (core 3.1: weekday, day, month and year roll over, `day_count`, spot clean-up in months 3, 6, 9, 12, clears `daily_event_bits`, re-sorts appointments), `hizuke_init`, restore the visibility, `hizuke_show`, step. Only SHUGAKU uses it (5 table entries): the school trip advances the calendar one day per call. Emulator (SHUGAKU, mode 0x51 started on 1995-04-04): `day_plus` and `calendar_advance_day` were hit together twice, the calendar going 4/4 -> 4/5 (weekday 1 -> 2, `day_count` 0 -> 1).

**`check_para_limit`** (O.BIN name kept; emulator-checked with out-of-range values): for girls 0-10, on the three 16.16 words `tokimeki` (+0), `affection` (+4) and `syoushin` (+8): if the integer half (signed) is negative the whole word becomes 0; if the integer half of tokimeki or affection is >= 0x80 it becomes 0x80 (the fraction half is kept); syoushin caps at 0xFF (fraction kept). For the nine parameters: negative integer half -> word 0; integer half >= 999 (0x3E7) -> word 0x03E70000 (999.0). Core 5.3 notes that the queue reader later clamps the girl counters to 0x80. Emulator: poked tokimeki 0x150 -> 0x80, affection -11 -> 0, syoushin 0x1F0 -> 0xFF, liberal arts -16 -> 0, stress 0x4000 -> 999.

**`change_girl` / `change_yoshio`**: `current_girl = date_girl` / `current_girl = 13`. Scenes that include Yoshio's lines switch to him (`change_yoshio`), then back (`change_girl`).

**`event_log_add(scene, flag)`** (`func_80085B3C`, core): record at `event_log[event_log_count]`: girl = `current_girl`, scene, flag, rank, year, month, day, weekday. Rank = `get_g_zyotai_h(current_girl)` (raw 0-4, not masked) unless `rank_override` is 1-6, in which case `rank_override - 1`; the override is cleared on every call; girl 12 always gets rank 0. The count is incremented and clamped to 0xFE, so a full log keeps its first 254 entries and keeps overwriting slot 254 with the newest.

## 11. SD sprites and the CD level meter

**SD (super-deformed) sprites.** The weekday and holiday screens show a small animated character acting out the chosen command (actors 96 and 97). `addr_init_weekly_sd(n)`, `addr_init_holiday_sd(n)`, `addr_init_club_sd(n)`, `addr_init_else_sd(n)` (n is `D_801230D0`, the SD index that the weekday handler computes from the day's command and a 0/1 variant through the tables `D_800B68C0` and `D_800B68D0` (800737A0.c), 0xFF = no SD sprite; the `else` set is called with the fixed 2) look up entry `n % 25`, `% 13`, `% 32`, `% 3` in `sd_table_weekday`, `sd_table_holiday`, `sd_table_club`, `sd_table_else` (three u16 per entry) and publish `sd_palette = 0x801E0000`, `sd_texture_blocks` (entry word 0), `sd_image_table` (word 1) and `sd_sequence_table` (word 2), each plus 0x801E0000. The weekday handler (`func_8007437C` in 800737A0.c) loads the palette with `load_palette(sd_palette, 0x10, 1, 1, 0)` and copies the three pointers into the actor fields of actors 96 and 97. **Data files**: the setup code reads file n of the matching disc folder whole to 0x801E0000 with `cd_read_sectors(0x29, 0x801E0000, sector_table[n])` (`sd_file_sector_weekday` `D_800B67AC`, `sd_file_sector_club` `D_800B6810`, `sd_file_sector_holiday` `D_800B69E0`; 0x29 = 41 sectors = 83,968 bytes, the size of every file of MAIN_SD (25 files), CLUB_SD (32) and HOLI_SD (12); the sector tables ascend by 41, so entry n is file n in name order, e.g. MAIN_SD: P00, P00W, P01, P01W ... P05W, P06, P07, P08, P09, P09W ... P0D, P0DW) and the sick set with `cd_read_sectors(9, 0x801E0000, 0x7D8A)` (9 sectors = 18,432 bytes, the size of the three ETC_SD files; HYPOTHESIS: the file of entry 2). Holiday entry 12 (sector 0xA79E) reuses weekday file 12. The `W` files (every second entry) are the paired variants noted above. `addr_init_else_sd(2)` is used while the player is sick on a school-day command. Emulator (school-day command screen, mode 0x40, `D_801230D0` = 1): `addr_init_weekly_sd(1)` ran every 83 frames and left the palette at 0x801E0000 and the pointers at 0x801E0480 (image table), 0x801E04B8 (sequence table) and 0x801E0478 (texture blocks), as the table predicts; the screen shows the player's small figure in the library scene. The offsets are in the header of each loaded file, so the tables are needed only when the original files are read; a port that converts the SD files needs the mapping from command number to file.

Table contents, hex offsets from 0x801E0000 written as image table / sequence table / texture blocks (the order in memory is texture blocks, image table, sequence table); entries repeat in pairs, which suggests two variants of one animation (HYPOTHESIS: the `W` files of the folders).

`sd_table_weekday` (D_800C9F60, 25 entries)

`0: 47C/4B8/474  1: 480/4B8/478  2: 3F8/428/3F0  3: 408/438/400  4: 3D0/3F8/3C8  5: 3D0/3F8/3C8  6: 3A0/3C8/398  7: 3A0/3C8/398  8: 40C/444/400  9: 418/450/40C  10: 458/49C/44C  11: 464/4A8/458  12: 40C/444/3FC  13: 478/4C0/464  14: 404/43C/3F4  15: 2D8/2E8/2D4  16: 2D8/2E8/2D4  17: 434/468/428  18: 448/47C/43C  19: 47C/4B8/470  20: 490/4CC/484  21: 45C/48C/450  22: 45C/48C/450  23: 484/4B4/478  24: 484/4B4/478`

`sd_table_holiday` (D_800C9FF8, 13 entries)

`0: 35C/37C/358  1: 36C/38C/368  2: 30C/324/308  3: 30C/324/308  4: 378/3A0/370  5: 384/3AC/37C  6: 2D8/2E8/2D4  7: 2D8/2E8/2D4  8: 39C/3C0/394  9: 3B0/3D4/3A8  10: 3C8/3E8/3C0  11: 3C8/3E8/3C0  12: 40C/444/3FC`

`sd_table_club` (D_800CA048, 32 entries)

`0: 3B4/3E0/3AC  1: 3B4/3E0/3AC  2: 3B4/3E0/3AC  3: 3B4/3E0/3AC  4: 3A4/3D4/398  5: 3A4/3D4/398  6: 3C0/3F4/3B4  7: 3C0/3F4/3B4  8: 41C/44C/410  9: 41C/44C/410  10: 414/444/408  11: 41C/44C/410  12: 430/468/424  13: 434/46C/428  14: 50C/550/500  15: 50C/550/500  16: 618/664/60C  17: 530/570/524  18: 420/454/414  19: 41C/450/410  20: 384/3AC/378  21: 384/3AC/378  22: 49C/4D8/490  23: 49C/4D8/490  24: 49C/4D8/490  25: 49C/4D8/490  26: 3B0/3E4/3A4  27: 3B0/3E4/3A4  28: 394/3C4/388  29: 394/3C4/388  30: 58C/5BC/580  31: 59C/5CC/590`

`sd_table_else` (D_800CA108, 3 entries)

`0: 3A8/3CC/3A4  1: 3A8/3CC/3A4  2: 32C/338/328`


**CD level meter.** `SD_GetCDLevel(&cd_level_work)` runs once per frame from the sound driver while an XA voice plays: it reads two 0x200-byte SPU capture buffers (SPU RAM 0 and 0x400) into `cd_level_samples`, computes the peak (mode 1) or average (mode 2) over `sample_count` evenly spaced sample pairs (doubled), optionally squares the result and applies one of three shaping curves, and stores left and right scaled by `precision` (shift by 16 - precision). The emulator showed precision 8, 32 samples and mode 0x212 (average, no square, no curve). `getCDlevel` returns the mean of left and right while `audio_state` bit 0x400 is set, else 0; callers are NAME_ENT (8), GEKO and two HUD functions (80062CD0.c, 800674B0.c: the portrait speech animation). A port can replace it by the playing clip's amplitude.

## 12. Hardware layer: modules and entry points

| module | names here | what a port provides |
|---|---|---|
| CD audio level | `SD_GetCDLevel`, `SD_DetectCDPeak`, `SD_CalcCDAve`, `getCDlevel`, `cd_level_samples`, `cd_level_work` | audio amplitude of the playing clip |
| primitives | `set_tarao_sprt`, `set_tarao_rect`, `_sprite_set_box_shade_tarao`, `set_tarao_bg`, `prim_line_gradient` calls | engine drawing for wipes (or engine transitions) |
| sprite tint | `sprite_brightness` (alias `func_80084C98`) | tint of a sprite quad |
| disc reads | `read_bustup*`, `bg_read_sub2`, `scene_wait_cd_read` | asset loading by name (section 6 and Appendix A map sector to file) |
| SD data | `addr_init_*_sd`, `sd_*` tables | SD sprite assets by command |

## 13. Names

258 rows in `config/renames/proposals/m-dates-b.tsv` (1 file, 20 func, 75 comment, 71 data, 27 field, 64 const), 25 of them HYPOTHESIS. Prefixes: new main functions in this unit use `scene_` (scene step library and per-frame helpers), `dialogue_`, `bustup_`, `event_still_`; scene-state globals use `scene_` or `dialogue_`; constants are `SPEAK_*`, `NAME_CODE_*`, `TEXT_MARK_*`, `SCENE_WIPE_*`, `ACTOR_*`, `SPRITE_*`. The 78 O.BIN function names of this object stay (comment rows give the English one-liner: 74 here, the other 4 already have core rows; seven of mine duplicate core rows and are more exact). The proposed file name is `src/main/scene_library.c`; its declarations should eventually leave `include/main_api.h` for a `include/scene.h` (not proposed as a row: it is a new file, not a rename).

## 14. Notes for the port

- Model a scene as a list of steps (the overlay tables) executed by a small interpreter with the step, `step_timer` and substeps of [[spec/core]]; or translate each table to a coroutine: `load_background`, `wipe_in`, `load_portrait(girl, outfit)`, `fade_in_portrait`, `say(group)`, `choose(...)`, `fade_out`. Keep the set of primitives of section 2; scenes differ only by their tables.
- Keep the dialogue contract of section 7: three parallel script tables, the 20 speak codes, the `n n` and reply-variant rules, the name codes, line breaks at space and `q`, the voice-before-text rule, confirm and cancel handling, `dialogue_choice` as the only link between a menu and the script.
- Portrait outfit rules (season and uniform months, the swimsuit year rule for girl 8), mood rank 0-4 (best to worst) from `get_g_zyotai_h`, and the expression index are game logic; the integrity checks are not.
- The wipe kind comes from the month (`month % 7`); durations are 16 frames (reveal), 18 (cover), about 32 (background fade), 1 + 16 + 1 (portrait fade, move place).
- `check_para_limit` and the event-log rank rule are exact and cheap to port; the parameter cap is 999 and the girl counters 0x80 (syoushin 0xFF here, 0x80 after the queue reader).
- The hardware paths (CD sync, MDEC decode, SPU capture, texture upload) are replaced by the engine.

## 15. Verification log (emulator, 2026-10-24)

All runs started from a boot-to-title save state with memory writes that set the main mode to the overlay loader (`mode = 2`, `overlay_target_mode`) and, for special events, `dialogue_work.scene` and `.girl`; the weekday screen was entered by writing mode 0x40 directly. The patched runner lives outside the repo (a scratch copy of `tools/emu` with a `--poke FRAME:ADDR:BYTES` option); no repo tool was changed, and no state file or screenshot is committed.

1. GEKO 0x5D, girl 0, scene 1: 22 functions of this object hit in 900 frames; step order, reveal wipe (kind 4 = April % 7, brightness 8..0x78, 16 calls), portrait file sector 0x544A, `check_para_limit` once, `scene_wait_cd_read` 110 calls.
2. Same scene, 5400 frames with confirm pressed every 70 frames: the command sequences of section 7.3, the voice request (clip pair 0x180001C3 / 0x210001D6, speaker 0), `voice_mode` 2, `cd_level_work` = precision 8, 32 samples, mode 0x212, `audio_state` bit 0x400 set while the voice plays, the scene ends with `normal_date_bggirl_fadeout` (32 calls).
3. `check_para_limit` with poked values (section 10).
4. BUNKAKEN 0x71: speak codes 0, 8, FF; the two-way menu waits for circle (frames 373-440); jumping to stage 1 step 8 starts `select_girl_init(0)` with `girl_menu_enabled` = 1 for girl 0 only; circle writes `dialogue_choice` from `select_girl_main`. Later in the scene: `normal_date_bg_fadein` 96 calls in three uses and `normal_date_girl_in` 18 calls (1 + 16 + 1).
5. DATE 0x50 (girl 0, place 0): speak codes 0, 1, 7, 8, FF (choice 0xFF from `normal_date_speak_012`).
6. SHOUGATU 0x52: `normal_date_bg_out` 36 calls (two uses of 18), `normal_date_girl_in` and `_out` 18 calls each, reveal wipe 16 calls. SHUGAKU 0x51: reveal wipe 48 calls (three uses), cover wipe 36, `day_plus` twice.
7. School-day command screen (mode 0x40): `addr_init_weekly_sd`, section 11.
8. BUNKAKEN with the command byte of group 1 line 1 poked from 8 to 3: the shuffle of section 7.5.
9. Voice cancel: in the GEKO scene a cross press 50 frames after `snd_command(0x300)` (frame 780) sent `snd_command(0xB4)` in the same frame (830) and the line ended on its own 12 frames later (next line requested at frame 842).

Not run: the three-way menu itself (`normal_date_three_select`, `junban_init` rule only), a cover wipe of another kind than the month's (the kind follows the same month % 7), `select_girl2`, `bustup_show_checked_hidden`, the `check_k_scroll` scroll path (no run reached a fourth text row), the holiday, club and else SD loaders.

## 16. Open questions

1. `bustup_layout_set` and `bustup_quad_offsets` (core rows): the argument is an expression index (mood rank 0-4, 5 unused), not a girl id; the core note for `bustup_quad_offsets` says "per girl id". Suggest renaming `bustup_quad_offsets` to `bustup_expression_select` and keeping `bustup_layout_set` as an alias description. Proposed unchanged here (core names are used).
2. Core says `step_timer` is the frames in the current step; in fact only the setters clear it and step functions increment it themselves (section 1).
3. Seven comment rows duplicate core rows (`normal_date_speak`, `_bg_fadein/_fadeout`, `_bggirl_fadein/_fadeout`, `_girl_in`, `_girl_out`); mine are more exact, and the two `girl_in/out` rows correct "slide" to a tint fade.
4. `scene_event_eyes_hold`, `scene_mouth_lock`, `event_anim_flag(_saved)`, `trip_day_state`, `event_mouth_*`, `event_eyes_*` and `ACTOR_EVENT_MOUTH/EYES` are HYPOTHESIS names; the DATE, GEKO and SHOUGATU analysts can confirm them.
5. DialogueWork words +0x08, +0x10, +0x18, +0x44, +0x54, +0x64, +0x68, +0x70, +0x74 are scene-specific flags of the overlays and are named by their analysts.
6. `voice_speaker_by_girl`: girl 11 maps to speaker 9 (same as girl 9) and girl 15 to 0xF; meaning of the speaker ids above 10 is not settled (h-cdio notes speakers 0xE and 0xF in NAME_ENT).
7. `voice_state == 0xA` (DATE) and `D_800CA2C8` (40 bytes, only cleared by the runner) are unexplained.
8. The three-way menu was exercised only through a poked command byte (shuffle rule confirmed); the cover-wipe kinds other than the month's were read from code only.
9. Script data: the SERIFU files hold the text, command and voice tables with absolute pointers for their load address (section 7.1); the load address and the three table addresses are constants in each overlay. No extraction tool exists; the port needs one (a Docker tool that, per scene, reads the file, relocates by the load address and dumps `text[group][line]`, `command` and `voice`), and its output must not be committed.
10. SD tables: the file of each entry follows from the three sector tables (checked against the folder sizes); open are the meaning of the paired `W` files (entries repeat in pairs) and which ETC_SD file the sick set reads (sector 0x7D8A; entry 2 is a HYPOTHESIS).
11. `scene_bg_show`, `set_tarao_rect` and `event_face_kuchi0` have no caller; `event_kuchi_ani_yes` and `event_face_kuchi1` have one table entry each.
12. The prefix `scene_` is new in the main naming (core lists `dialogue_`, `bustup_`, `message_window_` and others, not `scene_`); the other main units should not reuse it for a different meaning. The SD sector tables `D_800B67AC`, `D_800B6810`, `D_800B69E0` are proposed here but belong to the weekday and holiday code (800737A0.c, 80075320.c); if those units name them differently, theirs win.
13. File name `scene_library.c` should be harmonised with the neighbouring units (`m-dates-a` covers 80079B10.c, which is the sound driver glue, not a scene file, despite the task text).

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
| 8007C784 | `func_8007C784` | `scene_wait_cd_read` | step: main_step_next once cd_read_sync_status bit 0 is set |
| 8007C844 | `func_8007C844` | `scene_bg_show` | scene_bg_visible = 1, dec_bg_brightness = 0x80, main_step_next |
| 8007C878 | `func_8007C878` | `scene_bg_hide` | scene_bg_visible = 0, dec_bg_brightness = 0, main_step_next |
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
| 8007ECF8 | `dec_init` | kept (O.BIN) | Scene step: check the MDEC header of the loaded background (magic 0x3800 and version bits) |
| 8007ED84 | `bg_read_sub2` | kept (O.BIN) | Background read helper: starts the read of the scene background |
| 8007EDF8 | `check_k_scroll` | kept (O.BIN) | Per-frame dialogue window scroll: when the next text row would fall below the third row (y >= 0x60) hold it, then move all earlier rows... |
| 8007EF04 | `wait_sub_sub` | kept (O.BIN) | Scene step with a parameter: wait n frames (step_timer counts up), then step on |
| 8007EF48 | `dialogue_speak_part` | kept | Speak one script line: copy the text, switch on the command byte (20 codes) to pick label, quote style, voice request and wait mode,... |
| 80081128 | `func_80081128` | `dialogue_voice_skip_check` | called while a line waits for its voice to start: once dialogue_voice_wait_frames > 0x200 a circle or cross press (pad_pressed &... |
| 80081190 | `dialogue_render` | kept (O.BIN) | Split a script string into lines and queue them with name code expansion |
| 80082764 | `func_80082764` | `dialogue_run` (core) | 64 uses: the talk runner every script section goes through (reads group/line/command tables, waits for text end and voice, handles 20... |
| 80082D58 | `make_three_select` | kept (O.BIN) | Shuffle the three options of a choice line: split the text at q or space separators and rejoin it in the order dialogue_choice_order. |
| 80082EB4 | `junban_init` | kept (O.BIN) | Pick a random order of the three options from frame_count (first = frame_count % 3, the other two ordered by its parity) and store it in... |
| 80082F70 | `normal_date_three_select` | kept (O.BIN) | Scene step: three-option choice menu, dispatch on step_timer to init, then main. |
| 80082FD0 | `normal_date_three_select_init` | kept (O.BIN) | Register menu 1 with three 252x16 rows at x -0x80, y 0x30/0x40/0x50, cursor on row 0, scene_menu_active = 1. |
| 80083078 | `normal_date_three_select_main` | kept (O.BIN) | Run the menu |
| 80083174 | `normal_date_two_select` | kept (O.BIN) | Scene step: two-option choice menu, dispatch on step_timer to init, then main. |
| 800831D4 | `normal_date_two_select_init` | kept (O.BIN) | Register menu 1 with two 252x16 rows at x -0x80, y 0x30 and 0x40, cursor on row 0, scene_menu_active = 1. |
| 80083268 | `normal_date_two_select_main` | kept (O.BIN) | Run the menu |
| 80083338 | `func_80083338` | `bustup_expression_mood` | HYPOTHESIS: bustup_layout_set(get_g_zyotai_h(current_girl) & 0x7F) then main_step_next: face cells picked by the girl's mood rank |
| 80083378 | `func_80083378` | `bustup_expression_0` | HYPOTHESIS: bustup_layout_set(0) then main_step_next (expression index 0 = mood rank 0, the best mood) |
| 800833A0 | `func_800833A0` | `bustup_expression_1` | HYPOTHESIS: bustup_layout_set(1) then main_step_next |
| 800833C8 | `func_800833C8` | `bustup_expression_2` | HYPOTHESIS: bustup_layout_set(2) then main_step_next |
| 800833F0 | `func_800833F0` | `bustup_expression_3` | HYPOTHESIS: bustup_layout_set(3) then main_step_next |
| 80083418 | `func_80083418` | `bustup_expression_4` | HYPOTHESIS: bustup_layout_set(4) then main_step_next (rank 4 = the worst mood) |
| 80083440 | `func_80083440` | `bustup_layout_set` (core) | 178 uses: bustup_layout_set(girl) = func_80085CD4 (per-girl quad offsets) + func_800634FC |
| 80083474 | `func_80083474` | `bustup_show_checked` (core) | 299 uses: re-verifies the loaded portrait (pointer words and checksum, retries the step 3 back on mismatch) then builds it with the... |
| 80083628 | `func_80083628` | `bustup_show` | builds the portrait (bustup_build) from the file loaded at 0x80180000 without the integrity check |
| 800836A0 | `func_800836A0` | `bustup_show_checked_hidden` | like bustup_show_checked but retries four steps back unless the five image pointers and the file checksum are right, then builds the... |
| 80083808 | `func_80083808` | `bustup_wink_update` (core) | 45 uses: toggles portrait visibility bits and winks every 240 frames |
| 80083A10 | `func_80083A10` | `scene_frame_update` | per-frame helper every scene handler calls: dec_bg_draw(1) while scene_bg_visible, shows the pad cursor (actor 0) when a direction... |
| 80083AD4 | `hizuke_hide` | kept (O.BIN) | Scene step: hide the calendar date HUD (actors 16-23), then step on. |
| 80083AFC | `hizuke_appear` | kept (O.BIN) | Scene step: show the calendar date HUD, then step on. |
| 80083B24 | `read_bustup` | kept (O.BIN) | Start the CD read of the girl's casual-clothes portrait for the current season (months 3-5, 6-8, 9-11, else winter) into 0x80180000,... |
| 80084150 | `read_bustup_uniform` | kept (O.BIN) | Start the CD read of the girl's school-uniform portrait (months 6-9 summer uniform, else winter uniform), then step on. |
| 80084524 | `read_bustup_swimsuit` | kept (O.BIN) | Start the CD read of the girl's swimsuit portrait (girl 8 has one file per year), then step on. |
| 80084658 | `bustup_return` | kept (O.BIN) | Alias of read_bustup (the same code, reached through a second entry). |
| 80084678 | `return_step` | kept (O.BIN) | Restore the main mode, stage and step saved in saved_mode, saved_stage and saved_step by calling main_state_set, main_stage_set and... |
| 800846C0 | `k_disp_inc2` | kept (O.BIN) | Typewriter step with fast-forward: counts dialogue_text_delay down and reveals one glyph per call |
| 80084738 | `yosi_trans` | kept (O.BIN) | Scene step: restart sequence 0 of sound bank 0 (bgm_play(0, 0x200)), then step on |
| 80084764 | `change_yoshio` | kept (O.BIN) | Scene step: set current_girl to 13 (Yoshio) so portrait, name and voice code address him, then step on. |
| 8008478C | `change_girl` | kept (O.BIN) | Scene step: set current_girl back to date_girl (the girl of the scene), then step on. |
| 800847B8 | `func_800847B8` | `dialogue_names_set` (core) | 246 uses: fills the name buffers for dialogue codes: family and given name from GameState, the player-addressing form (get_p_name), the... |
| 8008484C | `select_girl` | kept (O.BIN) | Scene step: heroine picker with a back button, dispatch on step_timer to init(0), then main(0). |
| 800848AC | `select_girl2` | kept (O.BIN) | Same picker without the back button (menu of 11 items). |
| 8008490C | `select_girl_init` | kept (O.BIN) | Lay the heroine picker out: 11 names in a 4x3 grid (x -0x80 + 0x40 per column, y 0x30 + 0x10 per row, item width = characters * 14) for... |
| 80084B9C | `select_girl_main` | kept (O.BIN) | Run the picker |
| 80084C98 | `sprite_brightness` | kept (O.BIN) | Set the red, green and blue tint of sprite quad idx to val. |
| 80084CC8 | `day_plus` | kept (O.BIN) | Scene step: advance the calendar by one day (calendar_advance_day), rebuild the date HUD keeping its visibility, clear trip_day_state,... |
| 80084D3C | `check_para_limit` | kept (O.BIN) | Clamp girl tokimeki/affection (0..0x80), syoushin (0..0xFF) and the nine parameters (0..999) |
| 80084E4C | `func_80084E4C` | `text_marker_hide` (core) | 113 uses: text_marker_set(0) |
| 80084E6C | `func_80084E6C` | `scene_mouth_lock_clear` | HYPOTHESIS: scene_mouth_lock = 0 then main_step_next |
| 80084E90 | `func_80084E90` | `event_still_show` (core) | uploads six 0x40 x 0x80 tiles to VRAM and sets SpriteQuads 58-63 as a 3 x 2 grid (128 px cells) = one full-screen event still |
| 800850D4 | `func_800850D4` | `event_anim_show` (core) | fills actors 96/97 as a full-screen animated picture from frame/sequence/tpage tables and a mode |
| 80085234 | `func_80085234` | `event_still_layer_set` | shows (arg != 0) or hides the event still layer: actors 96 and 97 (draw_flags bit 7) and sprites 58-63 (hidden bit 31) |
| 80085368 | `func_80085368` | `event_still_fade_in` | step_timer-driven fade-in of the event still: normal_date_bg_fadein (+4 per frame), shows the layer when the brightness is 4, sets... |
| 800853FC | `func_800853FC` | `event_still_fade_out` | fade-out of the event still: normal_date_bg_fadeout (-4 per frame) |
| 80085498 | `event_face0` | kept (O.BIN) | Scene step: event picture face 0: sequence slot 2 on actor 96, slot 0 on actor 97. |
| 800854C8 | `event_face1` | kept (O.BIN) | Event face 1: slot 3 on actor 96, slot 1 on actor 97. |
| 800854FC | `event_face2` | kept (O.BIN) | Event face 2: slot 6 on actor 96, slot 4 on actor 97. |
| 80085530 | `event_face3` | kept (O.BIN) | Event face 3: slot 7 on actor 96, slot 5 on actor 97. |
| 80085564 | `event_face_kuchi0` | kept (O.BIN) | Event mouth 0: slot 2 on actor 96 only. |
| 8008558C | `event_face_kuchi1` | kept (O.BIN) | Event mouth 1: slot 3 on actor 96 only. |
| 800855B4 | `event_kuchi_ani_no` | kept (O.BIN) | Event mouth animation off: scene_event_mouth_anim = 0 and actor 96 anim_flags = 4. |
| 800855E4 | `event_kuchi_ani_yes` | kept (O.BIN) | Event mouth animation on: scene_event_mouth_anim = 1 and actor 96 anim_flags = 0. |
| 80085614 | `ev_me_on` | kept (O.BIN) | Event eyes on: scene_event_eyes_hold = 0 (periodic blink allowed) and actor 97 anim_flags = 5. |
| 80085644 | `ev_me_on_continue` | kept (O.BIN) | Event eyes on and held: scene_event_eyes_hold = 1 (no periodic blink) and actor 97 anim_flags = 5. |
| 80085678 | `_sprite_set_box_shade_tarao` | kept (O.BIN) | Queue a four-corner gradient box (x, y, w, h, ordering slot, two packed rgb colours) in the primitive arena. |
| 8008585C | `func_8008585C` | `dialogue_clear` (core) | 89 uses: clears dialogue_work words 5..30, re-initialises and hides the cursor actor, empties the text list and blanks the window title |
| 800858E8 | `don_init2` | kept (O.BIN) | Scene step: start the screen shake (screen_shake_start), then step on. |
| 80085910 | `don_wait` | kept (O.BIN) | Scene step: wait 0x81 frames, clear the text list and step on. |
| 80085958 | `normal_date_speak_1line` | kept (O.BIN) | Scene step: wire the dialogue tables and run exactly one line of the script |
| 800859E8 | `normal_date_speak_012` | kept (O.BIN) | Scene step: run the dialogue with choice 0xFF, so a three-option line keeps the order 0, 1, 2. |
| 80085A60 | `normal_date_speak` | kept (O.BIN) | Scene step: wire the dialogue tables and run one frame of the dialogue runner with dialogue_choice as the reply index |
| 80085ADC | `vram_bustup_clear` | kept (O.BIN) | Scene step: reset actors 30 and 31 and free the five portrait texture slots 5-9, then step on. |
| 80085B3C | `func_80085B3C` | `event_log_add` (core) | appends (girl, flag, scene, rank, year, month, day, weekday) to event_log |
| 80085CD4 | `func_80085CD4` | `bustup_quad_offsets` (core) | HYPOTHESIS: sets fields +0x0C/+0x0E of SpriteQuads 0 and 1 per girl id (texture offsets of the two portrait quads) |
