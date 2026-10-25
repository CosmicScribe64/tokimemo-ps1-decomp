---
type: spec
unit: m-menutext
updated: 2026-10-24
sources: ["src/main/8004E500.c", "src/main/8004F6B0.c", "src/main/8004F870.c", "config/renames/proposals/m-menutext.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/spec/m-scheduler.md", "wiki/spec/h-cdio.md", "wiki/ref-english-patch.md", "tools/emu (probes from the title-menu save state)"]
---

# Port spec: text system, menus and girl queries (m-menutext)

Roadmap phase 2, unit `m-menutext` ([[tickets/T-27018-p2-m-menutext]], parent [[tickets/T-27000-phase-2]]). This is shared game logic of the main executable: the text list with its typewriter, the speaker label and dialogue-block helpers, the menu engine, the girl name builders, and the girl score and state queries that most scenes call. It covers all 61 functions of `src/main/8004E500.c` (25), `src/main/8004F6B0.c` (1) and `src/main/8004F870.c` (35). Facts come from the matched C, its callers, the decoded data tables and short emulator probes (section 15). Anything the code does not prove is marked HYPOTHESIS. No game text appears here; strings are described by their role.

The application candidate is `config/renames/proposals/m-menutext-apply.tsv`: **48 rows (10 function, 38 data)**, prepared under [[tickets/T-27050-parallel-main-reconciliation]]. None of these rows is applied by this lane. The earlier `m-menutext.tsv` is an analysis draft and must not be applied wholesale. Current definitions and `config/renames/core.txt` win over that draft: voice entry points are already `talk_voice_play` and `talk_text_step`; the text-entry field is `attribute`, the state field remains `pos`, its advance field remains `unk_6A`, and the local view is `MenuSelection` with `current` and `pad_02`. Role names used below (`revealed`, `glyph_advance`, `color`, `ot_slot`, `cursor`) describe those bytes; they do not imply an applied field rename. Proposed symbol names below are keyed by the function-map address or the data-map address.

The ten function rows cover all nine remaining `func_` definitions plus existing `glyph_expand_1bpp`; **no owned placeholder function is deferred**. Applied and O.BIN names remain unchanged. The developer viewer takes the owning `menu_` prefix and ends in `_mode_handler`; the player club query takes `girl_`, the prefix of its query object. No source, header, symbol file or ledger has been edited. The Docker command `docker run --rm --cpus=2 -v "$PWD":/work -w /work tokimemo-decomp-arm64 python3 tools/rename.py plan config/renames/proposals/m-menutext-apply.tsv --ledger m-menutext` exited 0 with 48 rows and no `PROBLEM` (2026-10-24). Application and the clean 27/27 build belong to the integration owner.

## 1. What the unit does

Three systems that nearly every scene uses:

1. **Text system** (8004E500). A scene queues strings into a 52-entry list with `set_kanji_string(x, y, color, string, ot_slot)`. Each queued character claims one 16 x 16 cell of a glyph atlas in VRAM (the 12 x 12 bitmap is unpacked and uploaded at queue time). Once per frame `disp_string` draws the glyphs below the reveal position `revealed`, two sprites per glyph (text and a one-pixel shadow). The reveal position is advanced by the typewriter helpers (`k_disp_inc` and friends), one glyph every `speed + 1` frames. Scenes reset the list with `text_system_reset` and cut it back to a saved mark with `k_sub_reset`.
2. **Menus and dialogue** (8004F870, first half). `menu_set` registers up to 16 rectangles per menu (two menus, 0 and 1); `menu_check` hit-tests the pointer or the pad cursor; `menu_cursor_update` moves the pad cursor; `menu_bar_show` draws the highlight bar. `x_taku_*` build two- or three-item choice menus over the message rows. `gnsx`, `sndisp` and `sndi` put the speaker plate and the lines of a dialogue block on the three message rows, expanding the player and girl name codes; `xa_wait`, `text_wait_confirm` and `talk_text_step` run the typewriter, the line scroll, the voice wait and the confirm press.
3. **Girl queries** (8004F870, second half). Weighted scores per girl (`get_h_yuukou` = affection, `get_h_tokimeki`), the two rank functions `get_g_zyotai_s/h`, the name builders (`get_g_name`, `get_p_name`), the closeness and points computation `girl_scores` that the scheduler uses to pick encounters and ending candidates, and the birthday and club helpers.

Plus one stray function: `menu_vram_view_mode_handler` (8004F6B0), the developer VRAM viewer of mode 0xFE.

## 2. Function map

Use counts are call-site lines outside the unit's three files (main files / overlay files, EVENT and GYOZI excluded); a dash means none. "core" marks addresses core already named.

| address | name | role | uses |
|---|---|---|---|
| 8004E500 | `text_glyph_pages_clear` | clear n glyph pages in VRAM | -/2 |
| 8004E58C | `text_system_reset` (core) | upload text palettes, clear two glyph pages and all 52 entries, zero count, total, revealed and mark (the speed is kept), keep a valid advance | 42/213 |
| 8004E750 | `text_set_advance` | glyph advance 0x10 or (anything else) 0xE | -/4 |
| 8004E780 | `text_unused_stub` | empty | - |
| 8004E788 | `set_kanji_string` | queue one string; returns the new entry count, -1 when full | 246/329 |
| 8004E884 | `k_disp_start` | reveal position = glyphs of the first n entries | 29/68 |
| 8004E93C | `k_disp_switch` | set an entry's active byte (1 when the argument is positive) | 9/61 |
| 8004E970 | `get_k_work` | pointer to entry n (the last entry for n >= 0x35) | 5/17 |
| 8004E99C | `get_ksys` | pointer to `text_state` | 1/1 |
| 8004E9A8 | `set_k_work` | copy an entry by value | - |
| 8004E9F4 | `k_reset` | cut the list to n entries, glyph slots and reveal position follow | 13/85 |
| 8004EA98 | `k_sub_reset_point_set` | mark = entry count | 22/18 |
| 8004EAAC | `k_sub_reset` | `k_reset(mark)` | 48/267 |
| 8004EAD4 | `k_sub_disp_start` | `k_disp_start(mark + n)` | 34/253 |
| 8004EAFC | `k_disp_goto_line_end` | reveal the rest of the entry being typed | - (internal) |
| 8004EBEC | `k_speed_set` | typewriter speed, arguments from 0x81 up become 0x7F (0x80 passes) | 6/24 |
| 8004EC14 | `get_k_speed` | read it | 3/5 |
| 8004EC24 | `k_disp_inc` | one typewriter tick; 1 when everything is revealed | 31/17 |
| 8004ECB4 | `check_end_k` | 1 when everything is revealed | 9/36 |
| 8004ECE0 | `check_set_k` | entry count (low byte) | -/2 |
| 8004ECF0 | `get_now_k` | entry holding the last revealed glyph, 0xFF when none | 1/6 |
| 8004EE10 | `disp_string` | per-frame draw of the revealed glyphs | 2/- |
| 8004F268 | `load_string` | font lookup of every character, queue-time upload | - |
| 8004F364 | `vload_font` | upload one glyph to the next slot | - |
| 8004F4F0 | `text_glyph_expand` | 1-bit to 4-bit unpack | - |
| 8004F6B0 | `menu_vram_view_mode_handler` | mode 0xFE viewer | 2/- |
| 8004F870 | `menu_set` | register a menu | 21/11 |
| 8004F984 | `menu_check` | per-frame hit test or cursor sync; returns the item | 23/39 |
| 8004FA88 | `menu_check_1` | pointer hit test | - |
| 8004FB6C | `menu_check_2` | cursor marking | - |
| 8004FC10 | `menu_cursor_update` (core) | pad cursor movement | 23/55 |
| 80050324 | `menu_cursor_to_edge` | first or last usable item | - |
| 80050408 | `get_near_menu` | nearest item in a direction | - |
| 80050B00 | `menu_bar_color` | set a bar style | 2/2 |
| 80050B54 | `menu_bar_show` | draw the bar on the cursor item | 21/43 |
| 80050C24 | `x_taku_menu_set` | register a 2- or 3-item choice menu | 3/1 |
| 80050D78 | `x_taku_string_set` | queue its labels | 3/2 |
| 80050DFC | `gnsx` | name plate | 21/239 |
| 80050E8C | `sndisp` | queue a dialogue block, start the scroll state | 34/184 |
| 80051010 | `sndi` | queue one dialogue line | 9/136 |
| 8005142C | `set_c_girl` | set `current_girl` | -/1 |
| 80051438 | `get_g_name` | the name text uses for a girl | 4/4 |
| 80051508 | `get_p_name` | how a girl addresses the player | 1/2 |
| 80051A68 | `get_g_zyotai_s` | rank with high-affection flag | 10/60 |
| 80051B48 | `get_g_zyotai_h` | rank (event log) | 9/17 |
| 80051BDC | `menu_girl_taku_set` | girl choice menu | -/1 |
| 80051DBC | `xa_wait` | text step without voice | 9/141 |
| 80052000 | `text_wait_confirm` (core) | simple text step | -/49 |
| 80052060 | `talk_voice_play` (core) | start a voice clip, store the sync state | 2/118 |
| 8005215C | `talk_text_step` (core) | text step with voice sync | 2/118 |
| 80052648 | `get_h_tokimeki` | weighted tokimeki plus parameter bonus | 3/25 |
| 80052C88 | `get_h_yuukou` | weighted affection | 3/22 |
| 80052D04 | `get_h_tokimeki_table` | 11 bytes | - |
| 80052D54 | `get_h_yuukou_table` | 11 bytes | 2/- |
| 80052DA4 | `birth_day_check` | birthday today | 1/- |
| 80052DD4 | `birth_day_check_days` | birthday on a given day | 6/- |
| 80052E60 | `girl_points_shiori` | girl 0 points | - |
| 80052F68 | `girl_points_others` | girls 1-10 points | - |
| 80053418 | `girl_scores` | closeness and points of girls 0-10 | 6/4 |
| 8005352C | `girl_player_sports_club_check` | club 6-10 | 3/- |
| 80053564 | `girl_find_by_club` | first girl in a club | 4/- |

## 3. State and data

### 3.1 Text state and list

| name | address | what |
|---|---|---|
| `text_list` | 800B3DC0 | 52 `TextEntry` records of 8 bytes: `x` (s16, relative to the screen center), `y` (s16), `attribute` (u8, the color index 0-15; proposed name `color`), `glyph_count` (u8), `extra` (u8, the ordering-table slot, 0 means 1; proposed name `ot_slot`), `active` (s8, 1 = drawn); the field names are current declarations and section 16 asks core to change the two weak ones; the rest of this spec already writes `color` and `ot_slot`. Last entry also labelled `text_list_last` |
| `text_state` (was `text_list_count`) | 800B3F60 | `TextState`, 12 bytes: `count` (s16, entries used), `total` (s16, glyph slots allocated; alias `text_glyph_count`), `mark` (s16, saved count), `speed` (u8), `revealed` (s16, glyphs revealed), `glyph_advance` (u8, alias `text_glyph_advance`) |
| `text_font_range_table`, `text_font_glyphs` | 800B402C, 800B6D50 | 46 range entries (first code, first glyph number); 3418 glyphs of 18 bytes |
| `text_palette_0/1`, `text_shadow_palette_0/1` | 800B3FAC, 800B3FCC, 800B3FEC, 800B400C | 8 two-color palettes each |
| `text_glyph_upload_ring` | 801255F0 | 16 slots of 0x60 bytes |
| `text_row_y` | 800B40F0 | 0x32, 0x42, 0x52, 0 |

GameState fields the unit uses (core names): `text_lines_total` (+0xF60, really the index one past the last entry of the current dialogue block: read it as `text_block_end`), `text_first_line` (+0xF61, really the index of the block's first entry: `text_block_first`), `text_scroll_state` (+0xF62), `current_girl`, `pad_held`, `pad_pressed`, `frame_count`, `screen_brightness`, `screen_center_x/y`, `voice_mode`, `substep`, the `menu_*` fields, the girl records and the nine parameters. Section 5.2 and section 16 say what the three text fields really hold and ask core to rename the first two; the body of this spec writes `text_block_end` and `text_block_first` for them.

### 3.2 Menu state

`menu_items` (+0xF90, core): 16 x 2 `MenuItem` records of 8 bytes, item i of menu m at `+0xF90 + i * 0x10 + m * 8`: `x` (s16), `y` (s16), `w` (u8), `h` (u8), `hover` (u8, 0 none, `MENU_HOVER_POINTER` 1, `MENU_HOVER_CURSOR` 10), `disabled` (u8, 1 when w * h is 0). Next to it (+0x1090): `menu_count_0/1`, `menu_mode`, `menu_cursor_0/1` (s8, the pad cursor, -1 none), `menu_hit[menu]` (s8 at +0x1095, the item to act on). `menu_bar_style[16]` (800E3680) holds the highlight bar style per menu. The pointer position is the actor-0 position `cursor_x`/`cursor_y` (core `cursor_x`/`FA`, screen-center relative).

### 3.3 Voice sync state

Four words in the EVENT range of main bss (a main-only symbol file). The two sources hold immediate clip words for index -1 and clip-pair table pointers otherwise: `speaker_voice_source_a` (80125BF4), `speaker_voice_source_b` (80125BF8), `speaker_voice_index` (80125BFC), `speaker_voice_state` (80125C00; 0 pending, 1 playing, 2 done).

### 3.4 Girl score tables

All in `.data` at 800B4100-800B437C, indexed by girl 0-12 unless noted (section 8 has the values).

| name | address | shape |
|---|---|---|
| `girl_tokimeki_weight` (core) | 800B4100 | u8[13], percent |
| `girl_bonus_param_mask` | 800B4110 | u8[13] |
| `girl_param_ladder_kind` | 800B4120 | u8[13], 1-6 |
| `girl_param_bonus` | 800B4130 | s8[13][8] |
| `girl_param_ladder_base` | 800B4198 | u8[7] |
| `girl_affection_weight` (core) | 800B41A0 | u8[13], percent |
| `girl_closeness_grid` | 800B41B0 | u8[20][20] |
| `girl_shiori_param_minimum` | 800B4340 | u8[8], entries 0-6 used; the splat label D_800B4341 sits on entry 1 and becomes the alias `girl_shiori_param_minimum_tail` (the table start needs a new symbol entry) |
| `girl_syoushin_weight` | 800B4348 | s8[12] |
| `girl_points_mask_strong`, `girl_points_mask_normal` | 800B4354, 800B4360 | u8[12] each |
| `girl_points_param_curve`, `girl_points_stress_curve` | 800B436C, 800B437C | s8[15] each |
| `girl_short_name_table` (kept) | 800B35F4 | 16 string pointers: family names of girls 0-12 and 14, slot 13 and 15 hold given names (section 6) |
| `girl_name_suffix_familiar`, `girl_name_suffix_polite` | 800B3634, 800B3638 | the two suffix strings: the familiar one (kun) and the polite one (san); the glossary keeps -kun and -san as string pieces, not stems, so the names describe the form |

## 4. Text system

### 4.1 Queueing

`set_kanji_string(x, y, color, string, ot_slot)`:

- Returns -1 and does nothing when the list already holds 52 entries.
- A null string, or one whose first character is not complete (byte 0 or byte 1 is 0), only sets `active = 0` on the next free entry; the entry is still consumed and its `glyph_count` keeps whatever the slot held before (quirk, section 14).
- Otherwise it stores `y`, `color`, `x`, `ot_slot`, sets `glyph_count = load_string(string)` and `active = 1`, and returns the new count (`++count`).

`load_string(string)` reads two bytes per character as a Shift-JIS code until either byte is 0. A code below 0x8140 prints a debug error line and ends the string (single-byte ASCII is not supported by this system; the name codes of section 5 are replaced before the call). Otherwise it finds the first range of `text_font_range_table` whose start is above the code, takes the range before it, and the glyph number is `code - range.first + range.glyph`. Codes at or beyond the last entry (0x9873) load nothing and do not count. The font covers 3418 glyphs: punctuation, full-width digits and Latin, kana, Greek, Cyrillic (0x8140-0x84BE, glyphs 0-452) and the JIS level-1 kanji (0x889F-0x9872, glyphs 453-3417), so every on-screen text is full-width. It returns the number of glyphs loaded.

`vload_font(glyph)` unpacks the 12 x 12 bitmap with `text_glyph_expand` into ring slot `total & 0xF`, uploads it as a 16 x 12 pixel block (4 halfwords x 12 rows) to the next free atlas cell and bumps `total`. The atlas has three pages of 128 cells (8 columns x 16 rows, 16 x 16 pixel cells) at VRAM x 0x320, 0x360 and 0x3A0, y 0x100-0x1FF; slot `t` is at column `t & 7`, row `(t >> 3) & 15`. A fourth page does not exist: slots 0x180 and up still take the third branch of `vload_font`, so they land on the cells of slots 0x100 and up inside the third page (page 2, x 0x3A0). `text_glyph_expand` reads the bitmap as a 144-bit stream, row by row, most significant bit first, and writes each pixel as a nibble (0 transparent, 1 text color), two words per row.

So glyph slot numbers are consecutive in queue order: the j-th character of entry e has slot `sum(glyph_count of entries 0..e-1) + j`. Every character occurrence is uploaded again (no sharing). `k_reset(n)` rewinds the allocator to the end of entry n-1 (`total = revealed = sum of the first n glyph counts`, `count = n`), so later entries overwrite the same cells.

### 4.2 Typewriter

`revealed` is the number of glyphs drawn. One tick `k_disp_inc()`:

```
if (frame_count % (speed + 1) == 0) { if (revealed < total) { revealed++; return 0 } return 1 }
return (revealed == total) ? 1 : 0
```

It returns 1 whenever everything is shown, so callers also use it as the end test (`check_end_k` is the same test without a tick). Speed values: 0 one glyph per frame, 1 every 2 frames, 3 every 4 frames; the settings screen offers slow = 3, normal = 1, fast = 0; ENDING uses 8 and 16, BUNKA_SD 5. The speed is saved with the game (the memory-card code copies `get_k_speed()` into the save block and calls `k_speed_set` on load). `text_system_reset` never writes the speed: it zeroes the counters and the entries and keeps the setting, so only the boot-time zero initialization leaves speed 0 (probe 2 reads speed 1 right after a reset). A port that clears the speed in its reset would lose the saved setting. `k_speed_set` clamps only arguments from 0x81 up to 0x7F; 0x80 passes unchanged.

- `k_disp_start(n)`: `revealed` = glyph count of the first n entries, so those entries appear at once and the typewriter starts at entry n (TEL calls it with the current count).
- `k_sub_reset_point_set()`: `mark = count`. `k_sub_reset()`: `k_reset(mark)`. `k_sub_disp_start(n)`: `k_disp_start(mark + n)`. A screen keeps a fixed header below the mark and replaces the message above it. The settings screens use exactly this: queue the items, set the mark, queue the message, `k_sub_disp_start(0)`; after a choice, `k_sub_reset()` and a new message.
- `k_disp_goto_line_end()`: scans the 52 entries for the first running glyph sum that reaches `revealed` and sets `revealed` to it (end of the entry being typed); returns it, -1 if none.
- `get_now_k()`: the first entry whose running sum reaches `revealed`, i.e. the entry holding the last revealed glyph; 0xFF while `revealed` is 0.
- `check_set_k()`: low byte of `count`, used as the index the next queued entry will get.
- `k_disp_switch(n, on)`: entry n `active` = 1 if `on > 0` else 0 (inactive entries keep their glyph slots and are skipped by the draw loop).

### 4.3 Drawing (`disp_string`, once per frame from `draw_frame`)

For glyph stream index `i` below `min(total, revealed)` it finds the entry `j` that owns the glyph (`k` = index inside the entry) and, if `entries[j].active == 1`, emits two 16 x 16 sprites (code 0x7C) into ordering slot `ot_slot` (0 is treated as 1):

- colored sprite at screen x = `entry.x + screen_center_x + k * glyph_advance`, y = `entry.y + screen_center_y`, tint = `screen_brightness`, palette (x = 0x100 + (color & 8) * 4, y = 0x1F8 + (color & 7));
- shadow sprite one pixel right and down, tint 0, palette 0x10 further right (the dark twin);
- texture coordinates `u = (i & 7) * 16 + 0x80`, `v = (i & 0x78) * 2`, texture page 0x300, 0x340 or 0x380 by `i < 0x80`, `< 0x100`, else; draw-mode primitives for the page are added after the sprites of a run so they take effect first.

`text_system_reset` preserves a valid `glyph_advance` of 0xE (14 px) or 0x10 (16 px), defaulting to 0xE only when its value is invalid. It is set to 0x10 by `text_set_advance(0x10)` (NAME_ENT `func_80132198`) and back to 0xE by NAME_ENT and OPTION code. A glyph occupies 12 pixels of its 16-pixel cell, so neighbouring cells overlap in their transparent margins.

Palettes: 16 text colors, two entries each (transparent, color), uploaded at VRAM (0x100, 0x1F8) for colors 0-7 and (0x120, 0x1F8) for 8-15, with the shadow twins at (0x110, 0x1F8) and (0x130, 0x1F8). Color 0 is the pale default of the message window, 3 is used by the settings items, 0xF is near-black (NAME_ENT prompts). `text_system_reset` also uploads two blank strips of `text_blank_strip` at y 0x1FF under pages 0 and 1, clears pages 0 and 1 with `text_glyph_pages_clear(2)` (ENDING clears 3) and a 2 x 8 block at (0x102, 0x1F8) to gray.

### 4.4 Coordinates

All text coordinates are relative to the screen center of the 320 x 240 frame (160, 120); x -128 is 32 px from the left edge. The message window has three rows: y 0x32, 0x42, 0x52 (170, 186, 202 px from the top, 16 apart): `TEXT_ROW_0_Y`..`TEXT_ROW_2_Y`.

## 5. Speaker labels and dialogue blocks

### 5.1 Name plate

`gnsx(string)` queues the string at row 0 with color 0 and x by byte length: 2 bytes (1 character) -114, 4 bytes -121, 6 bytes or more -128. The three x values center a 14 px advance string on x = -107, a plate that fits at most three characters. Callers pass a girl's short name (`girl_short_name_table[girl]`), the player's family name or a fixed label (NAME_ENT prologue, TEL, ETC).

### 5.2 Dialogue line (`sndi(string, row, style)`) and block (`sndisp(string, row, style)`)

`style` is a byte whose low nibble is the speaker (a girl id; 15 = the player/narration, core `GIRL_PLAYER`) and whose high nibble selects the layout. It is also what scenes pass to `talk_voice_play`. Values seen at call sites (117 of 0xF, 89 of 0xD, 70 of 0x1F, 21 of 0x2F, 20 of 0xC, 4 of 0xFF, 4 of 0xA, the rest the speaker variable).

`sndi`:

1. Copies up to 64 bytes of the line and clears `text_scroll_state`.
2. Name codes (a single ASCII letter on an even byte offset, so it cannot be half of a two-byte character; the constants are m-dates-b's `NAME_CODE_PLAYER_FORM` for `P` and `NAME_CODE_GIRL` for `G`): if `style & 15 == 15`, the first `P` is replaced by `get_p_name(buf, 15)` (form 15: the player's full name, which is why m-dates-b names the code for the form and not for a fixed name) and then the first `G` by `get_g_name(buf, current_girl)`; otherwise only the first `P` is replaced, by `get_p_name(buf, current_girl)` (how the current girl addresses the player), whatever the speaker is. At most one replacement per code per line.
3. Opening mark on row 0: `style == 0xF` double corner bracket, `0x1F` opening parenthesis, `0x2F` none, anything else (including 0xFF) the ordinary corner bracket (`SPEAKER_STYLE_*`). The mark is a character of the queued line, so row 0 has one glyph more than its text (emulator probe 5).
4. Placement: rows 0, 1 and 2-and-up map to y 0x32, 0x42, 0x52; x is -0x56 (-86) for row 0 and -0x48 (-72) for later rows, or -0x80 for every row when `style == 0xFF` (no speaker plate). The first line is 14 px further left than the others so the opening mark hangs in the margin. Color 0, `ot_slot` 0.

`sndisp` splits the string at ASCII spaces on an even byte offset (5 splits at most, so 6 lines: the split positions go into a `u8 pos[5]` array without a bounds check, so five is the array capacity and a sixth space would write past it), replacing each space by 0, then calls `sndi` for the first piece at `row` and for the others at `row + 1 + n`. Around the calls it records `text_block_first = check_set_k()` before and `text_block_end = check_set_k()` after (core names `text_block_first` and `text_block_end`, +0xF61 and +0xF60), and sets `text_scroll_state = 0x80` (after the last `sndi`, which cleared it). So a block is a run of consecutive entries and a scene queues the plate first, then the block, as in this common step:

```
k_sub_reset(); gnsx(name); sndisp(text, 0, style); talk_voice_play(a, b, index, style); substep++
... next step each frame: talk_text_step()   (or xa_wait() / text_wait_confirm())
```

Text is never wrapped by the code: a line break exists only where the script string has a space.

### 5.3 Per-frame steps

All three steps end by bumping `substep` (the caller's step function switches on it).

`text_wait_confirm` (core `text_wait_confirm`, overlay prompts): `k_disp_inc()`; when `pad_pressed` has circle, cross or START (0x860): `k_disp_goto_line_end()` and, if everything is now shown, `substep++`. One press therefore finishes the current line and, on the last line, also advances.

`xa_wait` (O.BIN name; no voice code in this build) and `talk_text_step` (voice aware) share the scroll logic. A block shows three rows, `text_block_first` .. `text_block_first + 2` (call it F). With `text_scroll_state & 0x80`:

- while `get_now_k()` is at most F + 2: tick the typewriter; when the typing reaches F + 3 (a fourth line) and `text_block_end >= F + 3` that entry is hidden (`k_disp_switch(now, 0)`) so it does not overprint row 2;
- when `get_now_k()` is beyond F + 2 (the typewriter waits): phase 0 hides line F, then lines F + 1 and F + 2 move up 2 px per frame (phase 1); when line F + 1 reaches y 0x32 the two lines snap to y 0x32 and 0x42, F increases by 1, the hidden fourth line (now F + 2, at y 0x52 from `sndi`) is shown and typing resumes. A block of n lines thus scrolls n - 3 times, 8 frames each.

Without the 0x80 bit the step is a plain typewriter.

The continue marker: when the typing is finished (`r == 1`) the step shows the marker with `text_marker_set(1)` or, when the marker actor already has bit 7 set, runs its bobbing step (`func_8006C934`). A confirm press (`pad_pressed & 0x60`): when finished, hide the marker, `substep++`, `text_scroll_state = 0`; otherwise `k_disp_goto_line_end()` (completes the line being typed, one press per line).

`talk_text_step` adds the voice wait. `voice_mode` is 0 off, 1 on, 2 on and cancelable (core `VOICE_*`). `table_a` and `table_b` below denote the indexed-table interpretation of `speaker_voice_source_a` and `speaker_voice_source_b`; the sources hold immediate clip words instead when index is -1. The `cd_audio_state` tests are masks (the code tests `& 4` and `& 2`).

```
non-scroll typing: voice_mode 0            -> tick every frame
                   voice_mode 1 or 2       -> tick only while speaker_voice_state != 0
speaker_voice_state 0 -> 1 when cd_audio_state() & 4 (value 4, bit 2: clip playing)
speaker_voice_state 1, when cd_audio_state() & 2 (value 2, bit 1: clip done):
                 if speaker_voice_index != -1 and table_a[idx].start != table_b[idx].start and
                    table_a[idx].start == start word of the current request:
                      cd_audio_request_set(table_b[idx].start, .end, current_girl); audio_command(0x300);
                      speaker_voice_state = 0                      (clip B follows clip A)
                 else speaker_voice_state = 2
marker when typing is finished and speaker_voice_state == 2 (or voice_mode == 0)
cross press, typing finished, voice_mode 2: speaker_voice_state = 2, sound effect 0x74 and cd_audio_command(9)  (cut the clip)
confirm advances only if voice_mode is 0, or speaker_voice_state == 2
```

`talk_voice_play(a, b, index, style)` (core): when `voice_mode != 0` and the speaker nibble is not 15, issues the clip request with `cd_audio_request_set(start, end, style)` and `audio_command(0x300)` (clip words come from the pair tables at `index * 2`, or are `a` and `b` themselves when `index == -1`); then stores the tables and index and sets `speaker_voice_state` to 2 for speaker 15 (no voice), else 0. In the tables found (ETC, TEL) A and B hold the same pairs, so every observed line is one clip; B only matters when the tables differ (HYPOTHESIS: a second recording of a line).

Main uses `talk_voice_play` with immediate clip words (`index -1`, speaker 0xE) in the system menus (8005A0B0); overlays (TEL 116 times, ETC 2) pass tables.

## 6. Name builders

`get_g_name(buf, girl)` (name text uses for a girl; returns 1, or -1 when `girl` is outside 0-15, the buffer is still filled using `girl & 15`):

| girl | result |
|---|---|
| 0 (Shiori) | her given name, no suffix (a literal) |
| 10 (Yumi) | the given name in `girl_short_name_table[15]` + the familiar suffix `chan` (a literal; the data string `girl_name_suffix_familiar` is the other familiar suffix, kun) |
| 12 (Rei) | the family name in the table, no suffix |
| any other | the table entry (family name; for girl 13 the table holds his given name) + the polite suffix `san` (a literal) |

`get_p_name(buf, form)` builds how girl `form` addresses the player, from the player's family name (`player_family_name`), given name, nickname and the two suffixes. `high` means `get_g_zyotai_s(girl) & 0x80` (weighted affection at least 80). N nickname, G given name, F family name; `+kun` / `+san` append the strings `girl_name_suffix_familiar` (kun) and `girl_name_suffix_polite` (san).

| girl | high | otherwise |
|---|---|---|
| 0 | N | G+kun |
| 1 | F+kun | F+san |
| 2 | G+kun | F+kun |
| 3 | N | F+kun |
| 4 | G+kun | F+kun |
| 5 | G+san | F+san |
| 6 | N | F+kun |
| 7 | G+kun | F+kun |
| 8 | N | G+kun |
| 9 | F+kun | F+san |
| 10 | G+san | F+san |
| 11 | G+kun (always) | |
| 12 | F+kun (always) | |
| 13 | G alone (always) | |
| 14, and any id above 15 | F+kun | |
| 15 | F followed by the first 6 bytes (3 characters) of G, no suffix: the full name | |

The branches for girls 4 and 5 that test "rank is 4" have identical arms (dead code, no behaviour).

## 7. Menus

### 7.1 Registration

`menu_set(menu, count, x[], y[], w[], h[])` with s16 arrays: first clears `w`, `h` and `disabled` of all 16 slots of the menu; returns -1 for `count >= 17`; stores `menu_count[menu] = count` and `menu_hit[menu] = -1`; returns -1 for `menu >= 2`; then fills the first `count` items: x, y, w, h, `hover = 0`, `disabled = (w * h == 0)`. (The count and hit stores happen before the menu-index test, so menu 2 or more would write past the arrays; every caller passes 0 or 1. The return value is unspecified when it succeeds; nobody reads it.) Typical callers: HUD icon bars (menu 0), the system settings screens (the main settings menu is menu 0 with 8 items, its pages use menu 1 with 2 to 14 items), the title menu (menu 0, 2 items; emulator: items at x -48, y 66 and 82, 160 x 20), TEL (menu 0, 11 date spots), NAME_ENT, OPTION, ETC.

`x_taku_menu_set(menu, count)`: items of 240 x 16 at x -120, y from `text_row_y`; 3 items when `count == 3` else 2. `x_taku_string_set(a, b, c, count)` queues the labels at x -0x78 on rows 0x32, 0x42 and (only when `count == 3` and `c` is non-null) 0x52. `menu_girl_taku_set(menu, *out)` builds a 12-item menu, 4 columns x 3 rows (x = column * 64 - 128, y = 0x32 + 16 * row, 48 x 16 each, filled column by column): item i is girl i, except item 11 which is girl 12. A girl is shown only when her `met` flag is set; an unmet girl's item has width 0 (disabled, no label). `*out` receives the entry count after the last label. (The C calls `menu_set` after every item; only the last call matters.)

### 7.2 Hit test and cursor

`menu_check(menu, x, y)` returns -1 for `menu >= 2`. It dispatches on `menu_mode & 3`:

| menu_mode | name (core) | behaviour |
|---|---|---|
| 0 | mouse-like | `menu_check_1` (pointer) and returns the item under it |
| 1 | menu A | `menu_check_2` (cursor) and returns `menu_cursor[menu]` |
| 2 | mouse plus A | pointer unless L1 is held: L1 pressed this frame uses the pointer, L1 held uses the cursor, otherwise the pointer |
| 3 | menu B | as 1 |

`menu_check_1`: for each item sets `hover = 0`, then sets `hover = 1` and remembers it when `x` is strictly inside (x0, x0 + w) and `y` strictly inside (y0, y0 + h) (a zero-size item can never match); the last matching item wins. With no match the cursor is set to -1. When L1 is not held and an item matched, `menu_hit[menu]` = that item. Returns the item or -1. `menu_check_2`: `menu_hit[menu] = -1`; the item equal to `menu_cursor[menu]` gets `hover = 10`, the others 0; when L1 is not held and the cursor item exists, `menu_hit[menu]` = it.

`menu_cursor_update(menu)` (core name), called every frame after `menu_check` by screens that can use the pad:

- If SELECT was pressed this frame: `menu_mode` + 1 (the stored byte; only the low two bits matter, so SELECT cycles the mode and the new value is saved with the game), cursor = -1, return.
- Mode 0: runs the pointer mover `cursor_update` (core `cursor_update`, d-pad or mouse moves the actor-0 pointer by `cursor_speed`), then the cursor = the first item with `hover != 0`.
- Mode 1: unless one of L2, R2, L1, R1 is held, up or left on `pad_pressed` steps to the previous item, down or right to the next. Then the cursor wraps (>= count to 0, < 0 to count - 1) and skips disabled items in the direction of travel (at most 16 steps). The pointer is moved to the item: `cursor_x = item.x + item.w - 4`, `cursor_y = item.y + 8`.
- Mode 2: while L1 is held: on the frame L1 is pressed the cursor adopts `menu_hit[menu]`; later frames step like mode 1 and move the pointer to the item; without L1: `cursor_update` and the cursor becomes the last item with `hover != 0`.
- Mode 3: up/down call `get_near_menu(1, -1 or +1)` (vertical), left/right `get_near_menu(0, -1 or +1)` (horizontal); a result of -1 keeps the cursor; a cursor at or above `count` is set to `menu_cursor_to_edge(menu, 1)` (last usable item), below 0 to `menu_cursor_to_edge(menu, 0)` (first usable item); the pointer follows as in mode 1.
- Finally `cursor_x` is clamped to -150..140 and `cursor_y` to -100..100.

`get_near_menu(axis, dir, menu)`: uses at most 16 items. For axis 0 the signed distance of every item from the cursor item is its x difference, for axis 1 its y difference; items on the wrong side of `dir` get distance 0 (ignored; a distance of 0 is always ignored); disabled items are ignored. Candidates in the same row (axis 0, equal y) or column (axis 1, equal x) win over all others, the nearest of them first; if there is none the item with the smallest distance wins and a tie goes to the one closer on the other axis. Returns -1 when no candidate.

Screens read the result as `menu_cursor_1` (or `menu_hit[1]`) on a confirm press; the same code works for every mode because mode 0 sets the cursor from the hover.

### 7.3 Highlight bar

`menu_bar_show(menu)` draws a gouraud box over the item at `menu_cursor[menu]` when that is a valid index: position and size from the item, ordering slot = bits 24-27 of `menu_bar_style[menu]`, color = bits 0-23, box mode = bits 28-30, semi-transparent = bit 31 (passed as 0x80 in the mode argument); it then adds a dither-on draw-mode primitive to the same slot. `menu_bar_color(menu, rgb, slot, mode)` packs a style (`rgb + (slot & 15) << 24 + (mode & 15) << 28`); an index of 0x10 or more instead restores the two defaults 0x832F7FAF (menu 0, blue) and 0x822F7F2F (menu 1, green); boot and the settings screen do that. The emulator shows both defaults in the title-menu state.

## 8. Girl scores

`girl` below is the index modulo 13 (`GIRL_SCORE_MODULUS`), so girls 13 and 14 read the records of girls 0 and 1.

`get_h_yuukou(g)` = clamp(`affection * girl_affection_weight[g] / 100`, 0, 100) (integer division). `get_h_tokimeki(g)` = clamp(`tokimeki * girl_tokimeki_weight[g] / 100` + bonus, 0, 100). The bonus: for every parameter p in 1..6 (liberal arts, science, art, athletics, trivia, looks; guts never counts) whose bit is set in `girl_bonus_param_mask[g]`, find the first step k in 0..6 with `value(p) > T[kind][k]` and add `girl_param_bonus[g][k]`; add `girl_param_bonus[g][7]` when none matches. Bit 7 of the masks of girls 0, 4 and 6 is dead. Effective thresholds `T[kind][k]` = `girl_param_ladder_base[k]` + offset:

| kind | girls | thresholds k = 0..6 |
|---|---|---|
| 1 | 0 | 178, 135, 110, 90, 55, 35, 10 |
| 2 | 1, 4, 5, 6, 8 | 128, 100, 90, 70, 50, 30, 10 |
| 3 | 2, 7 | 178, 150, 140, 90, 70, 50, 30 |
| 4 | 3 | 138, 110, 100, 80, 50, 30, 10 |
| 5 | 9 | 148, 120, 110, 90, 50, 25, 10 |
| 6 | 10 | 128, 100, 90, 70, 45, 25, 5 |

(`girl_param_ladder_base` is 128, 100, 90, 70, 50, 30, 10.) Girls 11 and 12 have kind 0, an empty mask and zero bonuses.

Data (girls 0-12):

```
girl_tokimeki_weight   83 113 103 119 125 106 119 113 109 106 150 100 100
girl_affection_weight  83 113 150 119 131 106 138 100 125 150 138 100 100
girl_bonus_param_mask  0xFE 0x42 0x44 0x48 0xD0 0x50 0x90 0x68 0x60 0x6E 0x70 0 0
  parameters used      1-6; 1,6; 2,6; 3,6; 4,6; 4,6; 4; 3,5,6; 5,6; 1,2,3,5,6; 4,5,6
girl_param_ladder_kind 1 2 3 4 2 2 2 3 2 5 6 0 0
girl_param_bonus (steps 0..7)
  0:  4   3   2   2  1  0  -2  -4     1: 17   9   6  3  1  0  -5 -15
  2: 20  15   9   5  2  0 -10 -20     3: 14  11   8  5  2  0  -3  -6
  4: 10   6   4   1  1  0  -3  -6     5: 16  10   7  5  2  0  -2  -7
  6: 16  12   8   4  0 -4 -10 -16     7: 13  11   7  4  2  0  -6 -13
  8: 13  10   7   5  2  0  -6 -13     9:  9   8   6  4  2  0  -4  -8
 10:  9   7   5   3  1  0  -4  -8    11, 12: all 0
```

`get_g_zyotai_s(g)` (a = weighted affection, t = weighted tokimeki): a < 10 -> 4; a < 30 -> 3; a < 50 -> (t < 50 ? 3 : 1); a < 80 -> (t < 50 ? 2 : 1); else (t < 50 ? 0x82 : 0x80). Bit 7 = a >= 80; `& 0x7F` is the rank (`girl_state_rank`). `get_g_zyotai_h(g)`: a < 10 -> 4; a < 50 -> 3; t < 50 -> 2; t < 80 -> 1; else 0. The two disagree for the same girl and both are used (core section 5.2). The `GIRL_STATE_*` constants are ordering words (0 = highest scores) with the band definitions in their note column.

`get_h_tokimeki_table` and `get_h_yuukou_table` fill 11 bytes (girls 0-10) from the two functions.

## 9. Closeness and points (`girl_scores`)

`girl_scores(closeness[16], points[16])`: fills 11 bytes of scores through the two table functions, then for i in 0..15 (only 0-10 are meaningful: entries 11-15 of the closeness array come from uninitialized stack bytes) sets `points[i] = 0`, divides both scores by 5 (cap 19) and stores `closeness[i] = girl_closeness_grid[tokimeki / 5][affection / 5]`. Then `girl_points_shiori` and `girl_points_others` fill `points`. Both output arrays are 16 entries long (`u8` closeness, `s16` points) and every caller declares them that size.

Ten call sites (counted in the matched C and the asm, 2026-10-24):

| caller | what it does with the result |
|---|---|
| scheduler (`miharu_final_event_ready`, `command_girl_select`, `geko_judge`, `date_sasoi_judge`, `yuina_top_candidate_check`) | thresholds on closeness and points (m-scheduler) |
| holiday code `func_800754C0` (80075320.c) | calls it once and never reads the arrays afterwards |
| ENDING `func_801330E4` (80133030.c) | among the met girls 0-10, picks the one with the most points above 0x1F3 (500 or more) as the girl of the ending; with none it falls back to girl 13, or to girl 12 when girl 0 is under 500 points and two further story tests hold |
| ETC `func_8014B0D0` (8014ABD0.c) | among the met girls 1-10, returns the one with the highest closeness above 0x3C (0 when none) and prints both values with a debug format |
| TEL `func_8013CCD0` (8013CCD0.c) | profile page: per met girl, points of 0x1F4 or more select variant 0 of a sprite record (type 0x59 + variant), otherwise closeness below 6, 0xB, 0x15, 0x29 and 0x3D select variants 6 down to 2 and the rest variant 1 |
| TEL `func_80136724` (80132000.c, stage 6) | closeness of the current girl divided by 4 is a term of a random chance roll |

`girl_closeness_grid` is about `5 * min(a / 5, max(3, t / 5))` with a few entries one lower; rows are tokimeki buckets, columns affection buckets. Rows 0-3 are identical, so tokimeki below 20 gives at most about 15; the value climbs 5 per affection bucket up to the cap of the row. Full table (row = tokimeki / 5, 20 columns):

```
r0-3  0 5 10 15 15 15 14 14 15 14 15 14 15 15 14 15 15 15 15 15
r4    0 5 10 15 20 20 19 19 20 19 20 20 20 20 20 20 20 20 20 20
r5    0 5 10 15 20 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25
r6    0 5 10 15 20 25 30 30 30 29 30 29 29 29 29 30 30 30 29 30
r7    0 5 10 15 20 25 30 35 35 35 35 35 35 34 34 35 35 35 35 35
r8    0 5 10 15 20 25 30 35 40 40 40 40 39 40 39 40 40 40 39 40
r9    0 5 10 15 20 25 30 35 40 45 45 45 45 45 45 45 45 45 45 44
r10   0 5 10 15 20 25 30 35 40 45 50 50 50 50 50 49 50 50 50 50
r11   0 5 10 15 20 25 30 35 40 45 50 55 55 55 55 55 55 55 55 55
r12   0 5 10 15 20 25 30 35 40 45 50 55 60 60 60 60 60 60 59 60
r13   0 5 10 15 20 25 30 35 40 45 50 55 60 65 65 65 65 65 65 65
r14   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 70 70 70 70 70
r15   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 75 75 75 75
r16   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 80 80 80
r17   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 84 85
r18   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 90
r19   0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95
```

Closeness is used by the scheduler: encounters need at least 0x11, invitations 0x21 (m-scheduler constants), `command_girl_select` compares it with rank bars. The name `closeness` is a HYPOTHESIS (meaning word), the value only grows with both scores.

`girl_points_shiori(closeness, points)`: `points[0] = 0x311` (785) when all of: `closeness[0] >= 0x55`; the health bits of `player_flags` (bits 8-11) are clear; each of the parameters 1-7 is at least `girl_shiori_param_minimum` (130, 130, 130, 130, 120, 100, 100 for liberal arts, science, art, athletics, trivia, looks, guts); the sum of her `scene_counters[1..35]` is at least 8; her `syoushin` is below 0x33. Otherwise `points[0]` stays 0. (A fixed value, so only the comparison with the scheduler's 0x1F4/0x1F5 thresholds matters.)

`girl_points_others(closeness, points)`, for girls 1-10: `v = closeness * 4 + syoushin * girl_syoushin_weight[g]` (weights -4, -2, -4, -2, -2, -2, -2, -4, -4, -3 for girls 1-10); girl 2 adds `girl_points_stress_curve[min(stress, 140) / 10]`; for every parameter p in 1..7 (guts counts here) set in `girl_points_mask_strong[g]`, with `c = min(value(p) / 10, 14)`, add `curve[c] + curve[c] / 2` (x1.5, integer halves toward zero), and for every p set in `girl_points_mask_normal[g]` add `curve[c]` (`curve` = `girl_points_param_curve`); finally with `n` = sum of `scene_counters[1..35]`: `n >= 4` adds `n * 10`, otherwise the whole value is replaced by 0. Bit 0 of the masks is never tested (the loop body has a dead `m == 7` branch that divides by 20, which suggests it was meant to include parameter 0). Tables:

```
girl_points_param_curve   -100 -75 -50 -25 0 10 20 30 40 50 60 70 80 90 100        (index = value / 10, capped 14)
girl_points_stress_curve   100 85 65 50 37 25 17 0 -17 -34 -50 -63 -75 -87 -100    (index = stress / 10, capped 14)
girl_syoushin_weight  0 -4 -2 -4 -2 -2 -2 -2 -4 -4 -3 0
girl_points_mask_strong  1:0x04 2:0x02 3:0x01 4:0x01 5:0x08 6:0x08 7:0x02 8:0x04 9:0 10:0x04
  parameters             1: science; 2: liberal arts; 3, 4: none; 5, 6: art; 7: liberal arts; 8, 10: science
girl_points_mask_normal  1:0x0A 2:0x04 3:0x06 4:0x88 5:0x82 6:0x81 7:0x14 8:0x0A 9:0x76 10:0x05
  parameters             1: liberal arts, art; 2: science; 3: liberal arts, science; 4: art, guts;
                         5: liberal arts, guts; 6: guts; 7: science, athletics; 8: liberal arts, art;
                         9: liberal arts, science, athletics, trivia, looks; 10: science
```

The meaning of the points as an ending-candidate score is a HYPOTHESIS, now with direct evidence: the scheduler tests them against about 500 before the final scenes (m-scheduler section 16) and ENDING picks the girl of the ending as the met girl with the most points of 500 or more (table above). Nothing in the code names the concept, so `closeness` and `points` stay glossary words with a guessed meaning (GLOSSARY.md rows added by this unit).

## 10. Birthday and club helpers

`birth_day_check_days(who, month, day)`: a month of 13 counts as 1 (the December look-ahead of the scheduler). `who & 15 == 15` reads `player_birthday` (month in bits 0-3, day in bits 4-8), any other value the birthday word of `girls[who & 15]`; returns 1 when both match, else 0. `birth_day_check(who)` passes today's `month` and `day`. The C declares it `void` (a matching artifact); it returns the same value, and callers use it as a flag.

`girl_player_sports_club_check()`: 1 when the club in `player_flags` bits 12-15 is from `CLUB_BASEBALL` to `CLUB_BASKETBALL` (6-10).

`girl_find_by_club(club)`: the first girl from 0 to 10 whose `club` field (bits 20-23 of the girl flags) equals `club`, else -1.

`set_c_girl(girl)`: `current_girl = girl` (TEL).

## 11. Debug VRAM viewer (`menu_vram_view_mode_handler`, mode 0xFE)

Developer tool, not part of the game. Entered by the mode dispatcher on mode 0xFE and by the main loop when the debug flag (`D_800B3220`) is nonzero and pad 2 L1 and R1 are held together (0xC0000 in the pad word). It sets `pad_pressed` to 0x80000, zeroes the display start `(menu_vram_view_x, menu_vram_view_y)`, then loops once per iteration: pad 2 up/down change y by -16/+16, pad 2 right/left change x by +16/-16 (each clamped at 0), polls the pad, sets the display area to that origin and waits for a frame; it ends when pad 2 bit 0x08000000 (START) is pressed, restores the video mode from `video_width`, `video_height`, `video_interlace` and returns. A port drops it.

## 12. Connections

- Every scene overlay and most main modes call `text_system_reset` at init, queue text with `set_kanji_string`/`sndisp`/`gnsx`, and step it with `k_disp_inc`, `xa_wait`, `talk_text_step` or `text_wait_confirm`; `draw_frame` calls `disp_string` once per frame.
- The script dialogue runner (`talk_run`, 8007C030) builds its own lines with `set_kanji_string` and `talk_names_set` (core), which uses `get_p_name` and `get_g_name`; `sndi` and `sndisp` are used by scene code that writes fixed lines (rumors, health notices, telephone, TEL, ETC).
- Menus: HUD command screens, title (the pad branch of `func_800623E4` bypasses `menu_check`), settings, club joining, holiday phone and club screens, NAME_ENT, OPTION, TEL (date spots), ETC (`menu_girl_taku_set`).
- `get_g_zyotai_s` is the base of core `girl_state_rank` (1,106 uses); `get_g_zyotai_h` is stored by `event_log_add`; `get_h_tokimeki` and `get_h_yuukou` also drive `syoushin_up` (affection < 10), the sick-visit pick in the health check (tokimeki of 0x33 or more), `pick_top_girl` (80085E30, used by MASTER, RENSYU and TAIIKU: the girl with the best tokimeki + affection) and score tests in SHUGAKU, SHOUGATU, VALEN, KANGEI, GEKO, DATE2, BUNKAKEN and ENDING; RPG_BAT tests `get_g_zyotai_s`.
- `girl_scores`: ten call sites, listed in section 9 (the five scheduler functions, the holiday code, ENDING's choice of the ending girl, ETC, and two in TEL).
- `talk_voice_play` and `talk_text_step` depend on the CD layer (h-cdio): `cd_audio_request_set`, `cd_audio_state`, `cd_audio_command`, the sound command queue (`audio_command` 0x300 = voice class, 0x74).

## 13. Port notes

- Replace the glyph atlas, the palettes, `disp_string` and the VRAM upload by normal font rendering. Keep: the 52-entry limit (or an unbounded list with the same order), the screen-center-relative coordinates (a 320 x 240 design space), the 14 px advance (16 px on the NAME_ENT screen that asks for it), the one-pixel dark shadow, the color index (16 colors) and the ordering slot as a draw layer.
- Keep the typewriter exactly: ticks on `frame_count % (speed + 1) == 0` at 60 Hz, speed 0/1/3 from the settings, `k_disp_start` and the mark/reset scheme (screens depend on it), the "confirm finishes the current line" rule, the three-row window with the 8-frame scroll for blocks of four or more lines, the voice gating of `talk_text_step`, and the rule that text never wraps by itself.
- Keep the name-code and honorific rules (sections 5.2 and 6) and `get_g_zyotai_s/h`, `get_h_*` and `girl_scores` bit for bit (tables in sections 8 and 9; a Python model of sections 8 and 9 reproduced the original's closeness and points for 11 girls in two poked scenarios, section 15).
- Menus: keep the item model, the four menu modes and their navigation, the SELECT mode switch, and the pointer/cursor rules, or reduce to mouse plus pad focus with the same item rectangles; `menu_bar_style` is a highlight color per menu.

## 14. Quirks kept

- `set_kanji_string` with an empty string consumes an entry and leaves its `glyph_count` stale.
- `get_k_work(0x34)` returns the address of `text_state` (the bound is 0x35); callers pass small values.
- `disp_string` moves to the next entry once per glyph, so two empty entries in a row would misattribute glyphs; empty entries only occur with the stale count above.
- `load_string` drops codes at or beyond 0x9873 without a glyph and stops at codes below 0x8140 after a debug print.
- The third glyph page (page 2, x 0x3A0) is the last: slots 0x180 and up wrap inside it and overwrite the cells of slots 0x100 and up.
- `get_now_k` returns 0xFF while `revealed` is 0; `talk_text_step` and `xa_wait` would then treat the block as beyond its third row. In practice the name plate before the block gives `revealed` at least 1.
- `menu_set` writes `menu_count` and `menu_hit` before it rejects `menu >= 2`.
- `menu_cursor_update` does not skip disabled items when no direction was pressed and may leave the cursor on one.
- `get_h_tokimeki` ignores guts (the loop ends at parameter 6) and bit 7 of the masks; `girl_points_others` ignores bit 0.
- Girls 13 and 14 score as girls 0 and 1.
- `girl_scores` writes 16 closeness bytes from 11 inputs; entries 11-15 are garbage.
- `birth_day_check` has a `void` signature but returns the flag.

## 15. Evidence from the emulator

All runs start from a title-menu save state made in this worktree (`build/emu/states/title_menu.sstate`, not committed); several poke GameState bytes right after the state loads (a local copy of the probe runner with a `--poke` option, kept in `build/scratch`, not committed). Ten probes:

1. **Menu state at the title** (`func_800623E4` break): `menu_count_0` 2, `menu_count_1` 0, `menu_mode` 1, `menu_cursor_0` 0, `menu_hit_0` -1; items of menu 0 at x -48, y 66 and 82, w 160, h 20. This fixes the `MenuItem` layout (x s16, y s16, w, h, hover, disabled) and the interleaving item-major, menu-minor. The title uses the pad branch, so `menu_check` is not reached; `menu_bar_style` reads 0x832F7FAF and 0x822F7F2F.
2. **Text entries in NAME_ENT** (`set_kanji_string` breaks after pressing circle at the title): `text_state` after the first call of one screen: count 1, total 9, mark 0, speed 1, revealed 0, advance 0xE; entry 0: x -64, y 0x20, color 0xF, glyph_count 9, ot_slot 0, active 1. On a later screen three entries at y 0x20, 0x30 and 0x40 had glyph counts 15, 12 and 12 and `total` 0x0F, 0x1B, 0x27, the running sums.
3. **Dialogue block** (mode byte poked to 0x44, the rumor report): `sndisp` with style 0x1F, then two `sndi` calls: `set_kanji_string` at x -0x56 and y 0x32, then x -0x48 and y 0x42. After it: `text_block_first` 2, `text_block_end` 4, `text_scroll_state` 0x80; `text_state.revealed` advances by 1 per frame at speed 0 (3, 4, 5 on three consecutive frames) and `xa_wait` runs each frame.
4. **Score pipeline** (mode byte poked to 0x30, girl records and parameters poked, break at the instruction after `girl_scores` returns): the buffers of the original matched a Python model of sections 8 and 9 for all 11 closeness and 11 points values in two scenarios (the first with several girls below the 4-counter limit and Shiori failing her conditions, the second with Shiori at 785 and girl 1 at 0). This covers the weights, the six ladders, the bonus table, the grid, the point masks and curves.
5. **Scroll of a six-line block** (the rumor string rewritten in RAM as six two-character lines separated by spaces, mode byte 0x44): `sndisp` made six entries (the first one glyph longer than its text: the opening mark is part of the line), `text_block_first` 2, `text_block_end` 8, `text_scroll_state` 0x80. Lines 0-2 typed at one glyph per frame; when the typing reached the fourth line (revealed 10) that entry became inactive, phase 1 started (`text_scroll_state` 0x81), line 0 went inactive and lines 1 and 2 moved from y 66 and 82 up by 2 per frame (64, 62, ... 52); 8 frames after the start the lines snapped to y 50 and 66, `text_block_first` became 3, the hidden line became active at y 82 and the typing went on. This repeated for lines 4 and 5 (three scrolls for six lines, 10 frames apart: 8 for the slide, 2 for the new glyphs); at the end the window showed lines 3-5 with `revealed` equal to `total`.
6. **Menu navigation** (mode byte poked to 0x34, stage 4, step 0: the system settings screen): `menu_set(0, 8, ...)` registers 8 items at x -124, y -64 + 12 * i, 96 x 12; item 6 has width 0 (disabled). With `menu_mode` 1 pad presses (DOWN, DOWN, RIGHT, DOWN, UP) moved the cursor 0, 1, 2, 3, 4, 3 and the pointer to `x = item.x + w - 4 = -32`, `y = item.y + 8`; a further run showed DOWN skipping the disabled item (5 to 7), DOWN on the last item wrapping to 0 and UP on item 0 wrapping to 7. SELECT changed the stored `menu_mode` from 1 to 2 and set the cursor to -1 for one frame, after which the item under the pointer became the cursor again.
7. **Menu mode 3** (same screen, `menu_mode` poked to 3): DOWN stepped 0 to 5, skipped the disabled item to 7, stayed on 7 at the end (no wrap), UP went 7 to 5 to 4, RIGHT with no item to the right kept the cursor; the cursor item showed `hover` 10.
8. **Voice sync** (the club-joining message of the system menus: mode 0x34, stage 5, step 2, substep 3 poked, `player_flags` club set to 6 so the captain's voiced lines run, `voice_mode` poked): with mode 1 `speaker_voice_state` stayed 0 until `cd_audio_state` showed value 4, bit 2 (frame 7); the typewriter started at frame 9 (one glyph per frame) and finished at frame 37; a circle press at frame 100 did nothing; the CD state showed value 2, bit 1 at frame 222 (the clip lasted about 3.6 s) and `speaker_voice_state` became 2 at frame 223; a circle press at frame 240 advanced the substep. With mode 2 a cross press at frame 100 (text finished, clip still playing) advanced at once.
9. **`get_p_name` forms** (TEL's first call line, reached through the loader: mode byte 2 with target 0x45, stage 1, step 1, substep 2; player names poked to three known characters each; the style argument of the `sndisp` call patched in RAM to a non-narration value and `current_girl` and the girl's affection poked after the overlay load): for all 16 girl ids in both affection states (weighted affection 0 and at least 80, 32 runs) the string handed to `set_kanji_string` equalled the form of the table in section 6; with the unpatched style 0xF the code came out as the full name (family name plus the first six bytes of the given name), and the name plate (`gnsx`, six-byte family name) sat at x -128.
10. **`get_g_name`** (TEL step 3 substep 12, the line with a girl name code and style 0x1F, `current_girl` poked 0-15): every id gave the name of section 6 (id 0 the fixed given name, 10 the given name from slot 15 plus the familiar suffix, 12 the family name alone, all others including 13, 14 and 15 the table entry plus the polite suffix; id 15 is therefore girl 10's given name plus the polite suffix).

Not run: the second clip of a two-table line (every table found has equal halves), the pointer hit test (needs a mouse) and the `get_near_menu` row and column preference on a grid of items; these come from the code only.

## 16. Exact deferrals and shared requests

The executable subset contains functions and clearly declared data only. These parts of the original draft are **deferred**, not implicitly approved:

- **16 data rows omitted:** interior labels `D_800B3DC6`, `D_800B3F64`, `D_800B3F66`, `D_800B3F68`, `D_800B4030`, `D_800B40F8`, `D_800E3684`, `D_800B4199`, `D_800B419A`, `D_800B419B`, `D_800B419C`, `D_800B419D`, `D_800B419E`, `D_800B434A`, `D_800B434B`, and missing base `D_800B4340`. Most interior labels have no current declaration or symbol-file entry; their owning arrays/structs are already covered. No new base declaration is authorized here. `D_800B4341` is renamed `girl_shiori_param_minimum_tail`: its pointer is the second byte and the comparison reads `p[-1]`, so calling that label the table start would be wrong.
- **Four field rows:** reconcile against current `TextState.pos`, `TextState.unk_6A`, `MenuSelection.pad_02` and `MenuSelection.current` first. The old draft still names historical types; a distinct field batch needs the compiler-oracle proof and build.
- **All 54 constant rows:** the old table is descriptive, not the `rename.py const` site format. Literal sites and enum ownership need an executable batch. The text module owns speeds 0, 1 and 3; system-menu code only consumes them. `m-dates-b` owns name-code constants.
- **All 24 comment rows:** reconcile with existing comments and current applied names; do not append duplicate comments. In particular the Shiori helper has no specified success return, and the other-girl points helper writes through its output rather than returning a score.
- **Seven file/header rows:** three file moves and four topic-header splits are separate future changes, after symbol application. Splitting headers needs before/after declaration-view proof. This lane prepared no file move.

Requests for the integration owner, without a second rename row:

1. `GameState.text_lines_total` (+0xF60) is the index **one past the last entry** of the dialogue block, not a count independent of its start; +0xF61 `text_first_line` is its first currently visible entry. `sndisp` writes the before/after entry counts, and scroll code advances the first index. Suggested roles remain block end/first; core owns the field decision.
2. `TextEntry.attribute` supplies the color bits and `extra` is passed as the ordering-table slot (0 maps to 1); changes belong to a field batch. `MenuSelection.current` aliases GameState menu cursors.
3. Main-only bss routing applies to `text_glyph_upload_ring`, `menu_vram_view_x/y` and `speaker_voice_*` (addresses within EVENT range). `rename.py plan` accepted their current placement and proposed names; application must retain that routing.
4. The glossary gives `closeness` a guessed English meaning. Its grid row retains `HYPOTHESIS:`. Points are numeric outputs; an ending-candidate interpretation remains a hypothesis even though ENDING consumes the score.
5. Sections 8-9 describe valid score results for girls 0..10. `girl_scores` still writes 16 grid values and zeroes 16 point slots; weighted-score input bytes 11..15 were not initialized. A port must decide deliberately whether to preserve that undefined tail rather than reading it as meaningful scores.

## 17. Evidence and review status

Current-body reconciliation read `src/main/8004E500.c`, `src/main/8004F6B0.c` and relevant definitions of `src/main/8004F870.c`, the applied core ledger and current declarations. The executable notes cite exact operations; Shiori requires input >=85, no low-nibble health flag, parameter minima, scene-counter sum >=8 and syoushin <51, then **stores 785**. The other-girl helper adds signed table terms, includes stress only for girl 2, and replaces points by zero when scene-counter sum <4. The menu edge helper returns -1 without selecting a new cursor when every rectangle is empty.

Section 15 preserves the original analysis ticket's emulator observations as historical evidence; T-27050 did not rerun those probes and does not claim fresh runtime validation. The remaining second-clip, pointer and grid-navigation questions are still open there. Independent review and any corrections are recorded in T-27050 before handoff. A successful plan proves tool acceptance and collisions only; it does not replace review or the integration build.

T-27050 final verification (2026-10-24): independent Sol 6.1 review PASS for this executable table and reconciled spec. Docker `tokimemo-decomp-arm64 --cpus=2`, `python3 tools/rename.py plan config/renames/proposals/m-menutext-apply.tsv --ledger m-menutext` exited 0 with 48 rows, 18 affected files, no PROBLEM. Combined four-table validation found no duplicate source keys or target names. No application or build was performed by this lane; the integration owner must apply and obtain clean 27/27 hashes before source completion.

## 18. Second work package: filenames, topic headers and constants

This section answers the user's additional phase-2 priorities. It is a reviewed-plan candidate, separate from the frozen 48-row symbol subset. No file move, header split or literal replacement has been applied. Current core enums and applied ledgers take precedence over the historical 54 constant rows above. In particular, the current GameMode names are `GAME_MODE_OPENING` (0x11), `GAME_MODE_FILE_SELECT` (0x13), `GAME_MODE_NAME_ENTRY` (0x14), and `GAME_MODE_PROLOGUE` (0x15). This unit creates none of those IDs.

### 18.1 Object-preserving file moves

`config/renames/proposals/m-menutext-files.tsv` uses supported `file main OLD_STEM NEW_STEM NOTE` rows. It contains three independent moves:

| Current source | Recommended source | Body evidence and boundary |
|---|---|---|
| `src/main/8004E500.c` | `src/main/text_system.c` | Entire 0x8004E500-0x8004F6B0 object: 25 text-list/typewriter/font/draw definitions. |
| `src/main/8004F6B0.c` | `src/main/vram_viewer.c` | Entire 0x8004F6B0-0x8004F870 object: developer display-origin viewer only. |
| `src/main/8004F870.c` | `src/main/menu_speaker_girl.c` | Entire 0x8004F870-0x80053650 object: mixed menu, speaker/talk text and girl-query code. Its mixed name is deliberate; separate topic headers improve its interface without changing object membership. |

The range comment is generated by rename.py from the object configuration. The Docker plan with ledger `m-menutext-files` accepted all three rows, 8 affected files, zero PROBLEM. Actual application must make each move a separate commit after symbol application, as required by ADR-0001. Do not split C definitions into new objects in this naming batch.

### 18.2 Topic-header partition

The header partition below is the pre-T27061 design proposal; those proposed paths are not current-owner declarations. Topic ownership is now registered in `config/header_owners.json`, and the umbrella headers have been retired. The `TextSpeed` and `SpeakerVoiceState` definitions are in `include/main/text.h` and `include/main/speaker.h`; the approved settings families are in `include/main/settings.h`. Treat the remaining table rows as design notes only where their proposed path differs from a current registered owner. Preserve the listed override guards, old-style `()` prototypes, fixed-width parameters and type layouts.

| Header / include guard | Exact ownership to move | Dependencies and boundary |
|---|---|---|
| `include/game/text_types.h` / `GAME_TEXT_TYPES_H` | `TextEntry` (currently game.h), `TextState` and `FontRange` (currently main_api.h), with their existing offsets/sizes and no field renames. | Includes `common.h` only. The types are shared by main/overlay consumers; no extern symbols or GameState dependency. |
| `include/game/menu_types.h` / `GAME_MENU_TYPES_H` | `MenuItem`, used by GameState.menu_items and by menu APIs. | Includes `common.h` only. GameState's future home imports this type header, avoiding a game-state-to-main/menu.h cycle. Keep the implementation-only `MenuSelection` view local to the C file; do not create a second definition. |
| `include/main/text.h` / `MAIN_TEXT_H` | All 25 definitions in 8004E500; `text_list`, text state and its existing aliases, glyph/font/palette data and upload ring. Public TextSpeed and text limits/geometry belong here when implemented. | Includes `common.h` and `game/text_types.h`; no GameState include is needed by its current signatures. Keep `load_string()` and `vload_font()` as `()` where currently required. The VRAM implementation includes SDK headers directly. |
| `include/main/menu.h` / `MAIN_MENU_H` | `menu_set`, `menu_check`, `menu_check_1/2`, `menu_cursor_update`, proposed `menu_cursor_to_edge`, `get_near_menu`, `menu_bar_color/show`, `x_taku_menu_set/string_set`, `menu_girl_taku_set`, menu-bar data. | Includes `common.h` and `game/menu_types.h`; consumes existing MenuMode/PAD constants from game_enums.h. `menu_set`'s current wide K&R signature must remain unchanged. Does not redeclare HUD-owned cursor APIs. |
| `include/main/speaker.h` / `MAIN_SPEAKER_H` | `gnsx`, `sndisp`, `sndi`, `xa_wait`, `text_wait_confirm`, current `talk_voice_play` and `talk_text_step`, plus the four proposed speaker_voice source/index/state words and the split-position template if exposed. | Includes `common.h`; imports `main/text.h` only if its consumers need those APIs. Physically defined here even though two applied names use talk_: a future main/talk.h may include this header but must not duplicate its declarations. CD/HUD/girl dependencies belong in the implementation. |
| `include/main/girl.h` / `MAIN_GIRL_H` | `set_c_girl`, `get_g_name`, `get_p_name`, `get_g_zyotai_s/h`, `get_h_tokimeki/yuukou`, both weighted-score table builders, `birth_day_check/days`, the five proposed points/score/club helpers at 0x80052E60-0x80053564, plus owned score/name suffix tables. | Includes `common.h` and shared game_enums.h. It consumes GirlRecord/GameState in the implementation; it does not own their definitions. birth_day_check has a void definition but **no current shared prototype**; callers read its implicit-int result. Do not add a global void declaration in a pure split. Leave that view unchanged until a separate prototype audit supplies proven overrides or reconciles callers. Preserve all other existing matching views. Shared core name tables retain one owner; no duplicate externs in the new header. |
| `include/main/vram_viewer.h` / `MAIN_VRAM_VIEWER_H` | The one viewer mode-handler prototype. Its two coordinate words already have canonical externs; move those declarations here unchanged in a pure split. Any later privatization is separate work and must not introduce per-C extern declarations forbidden by Standards 8a. | Includes `common.h` only. Main dispatcher and debug main-loop path import this header; no GPU SDK types appear in the prototype. |

Migration order: snapshot declaration views; introduce pure type headers and replace their old definitions by includes; move one topic's declarations, including its existing MAIN_API_OVERRIDE guards, into its header; make main_api.h include that header; update direct consumers; compare views and run header checks plus clean 27/27 build. Retain game.h/main_api.h as umbrellas until all owners finish. No topic header includes either umbrella, and no SDK declaration is copied. File moves follow the topic work in their own commits. These are implementation recommendations, not a claim that the current rename tool can perform header extraction.

### 18.3 Proven constants inventory and exact site scope

This inventory spans the three whole objects and the rules their shared callers consume. It is **not** an executable const table. Before using rename.py const, identify each precise argument/member/local site: a global match on a generic `arg0`, `i`, `sum`, `col` or numeric value would mix unrelated meanings. Some site forms cannot express a casted aggregate access or a formula; use a reviewed tool enhancement or prior essential-local rename rather than widening the replacement. Values below retain their original signedness at each C site; enums do not replace fixed-width storage types.

| Family / intended names or existing owner | Proven values | Source contexts and replacement boundary |
|---|---|---|
| Existing shared IDs | GirlId, Parameter, Club, HealthState, VoiceMode, MenuMode, PAD_* | `get_g_name/get_p_name`, birthday and club helpers, weighted-score/rank code, menu_check/update and talk_text_step. Reuse enum members for identity comparisons, never for unrelated array lengths or equal-valued rank/selector fields. |
| Viewer input and game mode | `GAME_MODE_DEBUG_VRAM_VIEW` is external dispatcher context only (no mode-ID literal in the viewer body); second-pad masks from PAD_UP/DOWN/RIGHT/LEFT/START shifted by 16; `MENU_VRAM_VIEW_STEP = 16` | Only 8004F6B0's coordinate increments/decrements and high-half pad tests. Keep the pad word unsigned: form shifted masks with an unsigned operand, particularly LEFT at bit31. The initial 0x80000 is second-pad R1 and needs the same ownership check. |
| Text-list bounds | `TEXT_LIST_MAX = 52`, `TEXT_ENTRY_LOOKUP_LIMIT = 53`, `TEXT_LAST_ENTRY = 51` | Reset/queue/cut/goto-line scans in 8004E500; get_k_work uses **>=53**, so index52 still returns the state address. Do not silently replace 53 by52. 51 describes the fallback label's location; no new clamping behavior. |
| Glyph page/ring geometry | `TEXT_GLYPHS_PER_PAGE = 128`, `TEXT_GLYPH_PAGE_COUNT = 3`, `TEXT_UPLOAD_RING_MASK = 0xF`, `TEXT_UPLOAD_WORDS_PER_GLYPH = 24` | vload_font page selection, disp_string tpage selection, and upload-ring pointer stride. Three pages are a layout, not a hard384-glyph allocation limit: slots384+ wrap inside the third page. Keep s32-pointer word arithmetic, and do not turn the stride into a byte offset. |
| Bitmap conversion dimensions | `TEXT_GLYPH_BITMAP_BYTES = 18`, `TEXT_GLYPH_WIDTH = 12`, `TEXT_GLYPH_HEIGHT = 12` | glyph expansion reads18 input bytes and writes24 words; upload is4 halfwords by12 rows. Hardware packing loop counts/shifts remain separate; do not substitute12 for unrelated row/menu dimensions. |
| Font range count | `TEXT_FONT_RANGE_COUNT = 46` | load_string's range loop and current FontRange[46] declaration. Actual first-code thresholds come from the table, not duplicated constants. |
| Glyph advance | `TEXT_ADVANCE_NORMAL = 14`, `TEXT_ADVANCE_WIDE = 16` | text_set_advance argument comparisons/assignments, text_system_reset validation. A valid16 survives reset; invalid values default14. Preserve u8 storage and the reset's FAKE pointer-view reads. |
| Text speed | `TEXT_SPEED_FAST = 0`, `TEXT_SPEED_NORMAL = 1`, `TEXT_SPEED_SLOW = 3`, `TEXT_SPEED_CLAMP = 127`, `TEXT_SPEED_CLAMP_THRESHOLD = 129` | k_speed_set stores0x80 unchanged, clamps values>=0x81 to0x7F; k_disp_inc ticks when frame_count % (speed+1)==0. Settings shares0/1/3. BUNKA_SD/ENDING use other proven speeds owned by those units; do not change them here. |
| Text/speaker coordinates | `TEXT_ROW_0_Y = 50`, `TEXT_ROW_1_Y = 66`, `TEXT_ROW_2_Y = 82`; `TEXT_X_FULL_WIDTH = -128`, `TEXT_X_SPEECH_FIRST = -86`, `TEXT_X_SPEECH_NEXT = -72`, `TEXT_X_CHOICE = -120` | sndi, gnsx, x_taku_* and scroll snapping; keep s16 and screen-center-relative meaning. Same values in unrelated image or parameter coordinates are outside this family. |
| Choice layout | `MENU_CHOICE_WIDTH = 240`, `MENU_CHOICE_HEIGHT = 16`, two/three choice counts | x_taku_menu_set and x_taku_string_set only. Count3 selects three items; any other count ultimately registers2. Preserve the current initialization loop and four-element stack arrays. |
| Text color and draw layer | `TEXT_PALETTE_ROW_MASK = 7`, `TEXT_PALETTE_BANK_BIT = 8`, `TEXT_DEFAULT_OT_SLOT = 1` | disp_string reads attribute bits0..3 and maps extra0 to ordering slot1. Actual palette/tpage/VRAM constants are hardware layout and may remain in this port-first pass. Do not make attribute an enum type. |
| Menu capacity/sentinel | `MENU_COUNT = 2`, `MENU_ITEM_MAX = 16`, `MENU_CURSOR_NONE = -1` | menu_set rejects count>=17 and menu>=2 after some stores; menu_check/update/get_near_menu and MenuSelection.current. Keep -1 in s8/s32 paths, not unsigned255; preserve invalid-menu write order. |
| Menu hover state | `MENU_HOVER_NONE = 0`, `MENU_HOVER_POINTER = 1`, `MENU_HOVER_CURSOR = 10` | menu_check_1/2 assign the hover byte. Existing low menu_mode bits0..3 use MenuMode instead of these values. Disabled is a separate boolean byte. |
| Menu direction/axis | `MENU_AXIS_HORIZONTAL = 0`, `MENU_AXIS_VERTICAL = 1`; direction -1/+1 | get_near_menu and its menu_cursor_update call sites only. Direction is signed; zero means no direction in the cursor updater. Do not reinterpret stored mode values0/1 as axes. |
| Menu mode/input masks | `MENU_MODE_MASK = 3`; PAD_L2|PAD_R2|PAD_L1|PAD_R1; PAD_UP|PAD_RIGHT|PAD_DOWN|PAD_LEFT | menu_check/update switches, shoulder-block tests and direction switches. Existing PAD_* replaces direction bit literals; menu mode remains a u8 field incremented by SELECT with only low2 bits dispatched. |
| Menu cursor geometry/search | `MENU_CURSOR_X_MIN = -150`, `...X_MAX = 140`, `...Y_MIN = -100`, `...Y_MAX = 100`; search-distance limit320; focus offsets x-4/y+8 | menu_cursor_update clamp/focus stores; get_near_menu starts signed best_d=320 and uses -320 for same-row/column preference. These are navigation geometry, not the 320-pixel framebuffer width. Preserve tie order and strict comparisons. |
| Menu bar packing | style capacity `MENU_BAR_STYLE_COUNT = 16` (reset path for index>=16); slot mask0xF shift24; packing-mode mask0xF shift28, drawing-box-mode mask7 shift28; RGB mask0xFFFFFF; semi-transparency bit31 | menu_bar_color/show only. Use technical MENU_BAR_* masks/shifts; keep u32 packing and the original addition/FAKE nibble-mask operations. Default packed styles0x832F7FAF/0x822F7F2F are data defaults, not an enum. |
| Speaker formatting | `SPEAKER_STYLE_DOUBLE_BRACKET = 0x0F`, `...PAREN = 0x1F`, `...PLAIN = 0x2F`, `...FULL_WIDTH = 0xFF` | sndi's style argument and call sites. Low nibble15 also selects the narration/name-form branch, so compare/mask sites have different roles. Keep u8-related casts and full-width0xFF distinct from direct-voice index-1. |
| Text control/name codes | line delimiter0x20; 'P'=0x50 and 'G'=0x47 reuse the m-dates-b NameCode owner | sndisp/sndi scan only even byte positions. Delimiter byte0x20 is a text-control byte, not PAD_CIRCLE despite equal value. sndisp's split array has5 slots and no sixth-break check: do not add safety behavior by naming it. |
| Scroll state/window | `TEXT_SCROLL_ACTIVE = 0x80`, `TEXT_SCROLL_PHASE_MASK = 0x7F`, `TEXT_VISIBLE_ROWS = 3`, `TEXT_SCROLL_PIXELS_PER_FRAME = 2`, row spacing16 | sndisp sets active; xa_wait/talk_text_step test first+2 and next row first+3, shift two entries up2 until y<51 then snap50/66. Constants must preserve **<51**, not <=51. The marker actor's separate0x80 bit is a different field and owner. |
| Confirm/cancel combinations | PAD_CIRCLE|PAD_CROSS =0x60; text_wait_confirm mask0x860 also includes PAD_START | xa_wait/talk_text_step/text_wait_confirm only. Keep the voice-mode-specific distinction: CROSS may cancel a completed clip only for VoiceMode2; the combination is not a universal advance mask. |
| Voice state/index | `SPEAKER_VOICE_PENDING = 0`, `...PLAYING = 1`, `...DONE = 2`; `SPEAKER_VOICE_INDEX_DIRECT = -1` | talk_voice_play stores source words, and talk_text_step interprets tables only when index!=-1. State comparisons/assignments are an enum family; index remains s32. CD status masks4/2 and audio commands0x300/0x74 belong to CD/audio owners and must reuse their constants when available. |
| Girl score dimensions | `GIRL_SCORED_COUNT = 11`, `GIRL_RECORD_COUNT = 16`, `GIRL_SCORE_MODULUS = 13` | Weighted table builders write11 inputs; girl_scores loops16; get_h_* reduce girl index modulo13; name queries and girl choice mapping use GirlId instead of count constants. Do not extend initialized inputs to16. |
| Weighted-score arithmetic | `GIRL_SCORE_MAX = 100`; percentage divisor100 | get_h_* weighted product/division and clamp branches>=101. The same numerical100 is a percentage divisor and a clamp ceiling; >=101 remains equivalent only after proving the exact replacement. Keep signed sum/bonus handling and existing u32 return. |
| Rank thresholds/flag | affection boundaries10/30/50/80; tokimeki boundaries50/80; rank mask0x7F/high-affection flag0x80 | get_g_zyotai_s/h. New names should say which weighted score and branch boundary, e.g. GIRL_AFFECTION_HIGH_FLAG_THRESHOLD=80; the two rank algorithms differ and must not share an incorrect threshold policy. Use existing VOICE_ or menu constants nowhere in these score tests. |
| Closeness grid buckets | `GIRL_CLOSENESS_BUCKET_SIZE = 5`, maximum bucket19, clamp threshold20 | girl_scores only, each weighted score divided unsigned by5 and capped19. The word closeness remains a glossary HYPOTHESIS; dimensions/indexing are proved. Keep both inputs11..15 uninitialized as described in section9. |
| Shiori point gates | minimum closeness85U; scene sum>=8; syoushin<51; fixed output785; parameter indices1..7; scene-counter indices1..35 | 80052E60: proposed GIRL_POINTS_SHIORI_* constants with scene-count wording, not bare event. Preserve u32 minimum comparisons, unsigned85U and unchanged output on failure. Parameter minima are table bytes; do not duplicate seven literals or infer the unknown syoushin meaning. |
| Other-girl point terms | scene sum>=4, per-counter factor10, stress cap140 with clamp>=141, curve index cap14 with clamp>=15, parameter divisors10 (dead branch20), strong-term half divisor2 | 80052F68: only girls1..10, stress term only GirlId YUINA. Masks are per-girl table data; loop p1..7 omits bit0. Keep signed curve addition and integer half rounding toward zero. Do not remove the unreachable m==7 branch. |
| Birthday packing | month mask0xF, day extraction left23/right27; month13 normalized1 | birth_day_check_days, plus GIRL_PLAYER for selector15. Owner should provide shared BIRTHDAY_* masks/shifts, preserving the u32 shift and packed field; do not invent a Month enum. Month13 is a look-ahead normalization, not a thirteenth month. |
| Club query bounds | CLUB_BASEBALL6 through CLUB_BASKETBALL10 inclusive; first11 girl records | 8005352C/80053564. Reuse Club/GirlId members for identity tests; preserve upper exclusive11 where source uses it, and return signed-1 when no girl matches. The no-club ID11 is not GIRL_SCORED_COUNT merely because values coincide. |

Do not promote FAKE stack padding/buffer sizes into gameplay constants; do not replace literal bytes inside strings/assets, table contents, ABI offsets, GTE/GPU bit patterns or SDK protocol values by guessed game terms. The 18/24/46/128 conversion/layout families are technically proved but remain optional hardware-internal work. Core GameState fields, name-code IDs, audio commands and shared enum additions need their existing owner's batch; this inventory does not authorize duplicate definitions or edits to those owners.

## 19. T-27062 essential local/parameter and constant packets

The frozen 48-row symbol packet still plans successfully after the main file moves: exit 0, 48 rows, 18 affected files. The new `config/renames/proposals/m-menutext-essential.tsv` has **135 rows (66 parameter, 69 local)** for `text_system.c` and `menu_speaker_girl.c`. Its final Docker plan exited 0, 135 rows, 3 affected files, no PROBLEM. Some inverse checks are explicitly skipped because ordinary proposed names already occur as member names or comments; the compiler error-set proof passed. Application still needs the tool's view proof and the clean matching build.

These rows cover text-list/typewriter inputs, menu control and nearest-item selection, choice/speaker text and name expansion, three-row scrolling and voice state, girl identities and player-addressing form, weighted score/ladder roles, packed birthdays and club queries. `score_input` deliberately covers the stress value and later scene-counter sum reused by the other-girl helper. Source types, K&R definitions, the absent birthday-wrapper shared prototype, FAKE arrays and all partial-word behavior stay unchanged. Ordinary loop counters, renderer/font packing temporaries, viewer hardware internals and unused coordinates are not essential game-logic renames. The tool reserves IDO's `entry`, so the by-value text record argument is `entry_value`; distinct hit-test parameter names also avoid its conservative K&R file-scope-name check.

The **190-row** `m-menutext-constant-sites.tsv` is an exact-context manifest with function/expression/value/name/evidence columns, not an apply table. It separates existing-ID reuse, new proved values, mathematical identities and hardware-internal deferrals. It includes score denominators separately from ceilings, the two different rank algorithms, Shiori's unsigned minimum and unchanged-on-failure output, the closeness multiplier and signed scene/curve terms, all kind-specific ladder offsets, cursor sentinels, scroll/formatting state and packed birthday fields. Fields retain their declared widths. Shared field selectors, generic locals, return expressions and array/shift/arithmetic sites must be scoped and proved individually. Existing enum reuse has a newly supported exact nonnegative subset, described below; it is not a blanket replacement.

Two positive-only families have executable values files and successful current-tree dry runs:

| Family | Values file | Current site | Dry result |
|---|---|---|---|
| SpeakerVoiceState | `m-menutext-voice-state.values.tsv` | scalar `speaker_voice_state` | 3 values, 14 literals, 1 file, 0 unresolved |
| TextSpeed | `m-menutext-text-speed.values.tsv` | `main:k_speed_set(arg0)` | 8 values, 26 literals, 15 files, 0 unresolved |

Those `rename.py const --header include/main_api.h ...` commands were pre-topic-migration dry runs, not current visibility roots. `TextSpeed` now belongs to `include/main/text.h`; its applied constant declarations and the other T-27062 families are recorded in `config/renames/main-constants.txt`. The speed observations remain 0/1/3/5/8/14/16/30, with cadence speed+1 frames. This declaration/site history does not authorize replacing unrelated speed-return switches.

**Historical signed-literal limitation (T-27062, 2026-10-24):** the earlier implementation could replace only the positive leaf beneath unary minus, leaving the original minus outside the identifier. T-27070 repaired this through typed-AST signed-span handling in `tools/rename.py`; the negative-hex, mixed-type and automatic legacy-routing cases are covered by `tools/test_rename.py`. That tool support does not mean every older negative-constant proposal was applied: individual sites still require their reviewed values files, exact site plans and applied ledger entries. Binary subtraction of a positive ladder offset remains a separate expression and its operator must be preserved.

No source, header, frozen table, symbol file or ledger was changed, and no application/build was performed. T-27062 records final independent review and the integration gates.

T-27062 final review (2026-10-24): independent Sol review passed the 135-row essential packet and corrected 190-record constant manifest. Both positive constant-family plans above passed against the current logical filenames. Together with the other three units, the handoff contains 305 local/parameter rows and 508 exact-context constant records. Those records include reuse and explicit deferrals, not 508 executable replacements. Root owns application, header ownership, any required tool fixes and clean matching-build verification.

T-27062 post-application revalidation (2026-10-24): root applied the 247 main UI symbols through `config/renames/main-ui.txt`. The unchanged historical scopes in this 135-row table resolve through that ledger; a fresh Docker plan passed with exit 0, three affected files and no `PROBLEM`. Both positive constant plans were repeated using the current selectors above and retained their counts. Independent Sol follow-up review passed all 305 supplementary rows across the same 114 current definitions, including all seven constant selectors. Evidence: ignored `.claude/orchestrator/state/T-27062-menutext-post-main-ui-plan.txt` and `T-27062-menutext-{voice,speed}-post-main-ui-const-plan.txt`.

The current tool has added `const --reuse ENUM` for existing nonnegative int enumerators at exact `expr:UNIT:function[@N]:EXPRESSION` comparison-right-operand and `index:UNIT:function[@N]:BASE` subscript sites. It requires the actual owning enum header, current function names, direct literal enum definitions and unambiguous occurrence selection. The 190-record manifest is not yet an executable reuse command list. Unsupported forms and legacy negative-literal replacement remain deferred; this lane has not applied or claimed proof for new reuse commands.

T-27050 second-work-package verification (2026-10-24): independent Sol 6.1 review PASS for the filename/header recommendations and constants inventory, with all findings resolved. The separate m-menutext-files.tsv Docker plan exited0 with 3 rows/8 affected files and no PROBLEM. This review approves recommendations, not implemented headers/constants. Existing symbol batch remains frozen; no source move, header extraction, literal replacement, apply or build was performed.
