---
type: spec
unit: m-menutext
updated: 2026-10-24
sources: ["src/main/8004E500.c", "src/main/8004F6B0.c", "src/main/8004F870.c", "config/renames/proposals/m-menutext.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/spec/m-scheduler.md", "wiki/spec/h-cdio.md", "wiki/ref-english-patch.md", "tools/emu (probes from the title-menu save state)"]
---

# Port spec: text system, menus and girl queries (m-menutext)

Roadmap phase 2, unit `m-menutext` ([[tickets/T-27018-p2-m-menutext]], parent [[tickets/T-27000-phase-2]]). This is shared game logic of the main executable: the text list with its typewriter, the speaker label and dialogue-block helpers, the menu engine, the girl name builders, and the girl score and state queries that most scenes call. It covers all 61 functions of `src/main/8004E500.c` (25), `src/main/8004F6B0.c` (1) and `src/main/8004F870.c` (35). Facts come from the matched C, its callers, the decoded data tables and short emulator probes (section 15). Anything the code does not prove is marked HYPOTHESIS. No game text appears here; strings are described by their role.

Names are the proposals in `config/renames/proposals/m-menutext.tsv` (150 rows) and the core table (`p2a-core`). Where core already named an address (`text_system_reset`, `menu_cursor_update`, `text_wait_confirm`, `voice_play`, `dialogue_text_step`, the GameState fields, `TextEntry`, `TextState`, `MenuItem`, `MenuSelection`) this unit uses that name and adds no row. Suggested files: `src/main/text_system.c` (8004E500), `src/main/debug_vram_view.c` (8004F6B0), `src/main/menu_speaker_girl.c` (8004F870), headers `include/main/text.h` and `include/main/menu.h`. O.BIN names stay (45 of the 61 functions); 22 of them have a comment row in core, 23 in this table (section 16 lists the core comments that could be firmer).

## 1. What the unit does

Three systems that nearly every scene uses:

1. **Text system** (8004E500). A scene queues strings into a 52-entry list with `set_kanji_string(x, y, color, string, ot_slot)`. Each queued character claims one 16 x 16 cell of a glyph atlas in VRAM (the 12 x 12 bitmap is unpacked and uploaded at queue time). Once per frame `disp_string` draws the glyphs below the reveal position `revealed`, two sprites per glyph (text and a one-pixel shadow). The reveal position is advanced by the typewriter helpers (`k_disp_inc` and friends), one glyph every `speed + 1` frames. Scenes reset the list with `text_system_reset` and cut it back to a saved mark with `k_sub_reset`.
2. **Menus and dialogue** (8004F870, first half). `menu_set` registers up to 16 rectangles per menu (two menus, 0 and 1); `menu_check` hit-tests the pointer or the pad cursor; `menu_cursor_update` moves the pad cursor; `menu_bar_show` draws the highlight bar. `x_taku_*` build two- or three-item choice menus over the message rows. `gnsx`, `sndisp` and `sndi` put the speaker plate and the lines of a dialogue block on the three message rows, expanding the player and girl name codes; `xa_wait`, `text_wait_confirm` and `dialogue_text_step` run the typewriter, the line scroll, the voice wait and the confirm press.
3. **Girl queries** (8004F870, second half). Weighted scores per girl (`get_h_yuukou` = affection, `get_h_tokimeki`), the two rank functions `get_g_zyotai_s/h`, the name builders (`get_g_name`, `get_p_name`), the closeness and points computation `girl_scores` that the scheduler uses to pick encounters and ending candidates, and the birthday and club helpers.

Plus one stray function: `mode_debug_vram_view` (8004F6B0), the developer VRAM viewer of mode 0xFE.

## 2. Function map

Use counts are call-site lines outside the unit's three files (main files / overlay files, EVENT and GYOZI excluded); a dash means none. "core" marks addresses core already named.

| address | name | role | uses |
|---|---|---|---|
| 8004E500 | `text_glyph_pages_clear` | clear n glyph pages in VRAM | -/2 |
| 8004E58C | `text_system_reset` (core) | upload text palettes, clear two glyph pages and all 52 entries, zero the counters, keep a valid advance | 42/213 |
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
| 8004EBEC | `k_speed_set` | typewriter speed, values above 0x80 become 0x7F | 6/24 |
| 8004EC14 | `get_k_speed` | read it | 3/5 |
| 8004EC24 | `k_disp_inc` | one typewriter tick; 1 when everything is revealed | 31/17 |
| 8004ECB4 | `check_end_k` | 1 when everything is revealed | 9/36 |
| 8004ECE0 | `check_set_k` | entry count (low byte) | -/2 |
| 8004ECF0 | `get_now_k` | entry holding the last revealed glyph, 0xFF when none | 1/6 |
| 8004EE10 | `disp_string` | per-frame draw of the revealed glyphs | 2/- |
| 8004F268 | `load_string` | font lookup of every character, queue-time upload | - |
| 8004F364 | `vload_font` | upload one glyph to the next slot | - |
| 8004F4F0 | `text_glyph_expand` | 1-bit to 4-bit unpack | - |
| 8004F6B0 | `mode_debug_vram_view` | mode 0xFE viewer | 2/- |
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
| 80052060 | `voice_play` (core) | start a voice clip, store the sync state | 2/118 |
| 8005215C | `dialogue_text_step` (core) | text step with voice sync | 2/118 |
| 80052648 | `get_h_tokimeki` | weighted tokimeki plus parameter bonus | 3/25 |
| 80052C88 | `get_h_yuukou` | weighted affection | 3/22 |
| 80052D04 | `get_h_tokimeki_table` | 11 bytes | - |
| 80052D54 | `get_h_yuukou_table` | 11 bytes | 2/- |
| 80052DA4 | `birth_day_check` | birthday today | 1/- |
| 80052DD4 | `birth_day_check_days` | birthday on a given day | 6/- |
| 80052E60 | `girl_points_shiori` | girl 0 points | - |
| 80052F68 | `girl_points_heroines` | girls 1-10 points | - |
| 80053418 | `girl_scores` | closeness and points of girls 0-10 | 6/4 |
| 8005352C | `player_in_sports_club` | club 6-10 | 3/- |
| 80053564 | `girl_find_by_club` | first girl in a club | 4/- |

## 3. State and data

### 3.1 Text state and list

| name | address | what |
|---|---|---|
| `text_list` | 800B3DC0 | 52 `TextEntry` records of 8 bytes: `x` (s16, relative to the screen centre), `y` (s16), `attr` (u8, bits 0-3 colour 0-15), `glyph_count` (u8), `extra` (u8, ordering-table slot, 0 means 1), `active` (s8, 1 = drawn). Last entry also labelled `text_list_last` |
| `text_state` (was `text_list_count`) | 800B3F60 | `TextState`, 12 bytes: `count` (s16, entries used), `total` (s16, glyph slots allocated; alias `text_glyph_count`), `mark` (s16, saved count), `speed` (u8), `revealed` (s16, glyphs revealed), `glyph_advance` (u8, alias `text_glyph_advance`) |
| `text_font_range_table`, `text_font_glyphs` | 800B402C, 800B6D50 | 46 range entries (first code, first glyph number); 3418 glyphs of 18 bytes |
| `text_palette_0/1`, `text_shadow_palette_0/1` | 800B3FAC, 800B3FCC, 800B3FEC, 800B400C | 8 two-colour palettes each |
| `text_glyph_upload_ring` | 801255F0 | 16 slots of 0x60 bytes |
| `text_row_y` | 800B40F0 | 0x32, 0x42, 0x52, 0 |

GameState fields the unit uses (core names): `text_lines_total` (+0xF60), `text_first_line` (+0xF61), `text_scroll_state` (+0xF62), `current_girl`, `pad_held`, `pad_pressed`, `frame_count`, `screen_brightness`, `screen_center_x/y`, `voice_mode`, `substep`, the `menu_*` fields, the girl records and the nine parameters. Section 5.2 and section 16 say what the three text fields really hold.

### 3.2 Menu state

`menu_items` (+0xF90, core): 16 x 2 `MenuItem` records of 8 bytes, item i of menu m at `+0xF90 + i * 0x10 + m * 8`: `x` (s16), `y` (s16), `w` (u8), `h` (u8), `hover` (u8, 0 none, `MENU_HOVER_POINTER` 1, `MENU_HOVER_CURSOR` 10), `disabled` (u8, 1 when w * h is 0). Next to it (+0x1090): `menu_count_0/1`, `menu_mode`, `menu_cursor_0/1` (s8, the pad cursor, -1 none), `menu_hit[menu]` (s8 at +0x1095, the item to act on). `menu_bar_style[16]` (800E3680) holds the highlight bar style per menu. The pointer position is the actor-0 position `cursor_x`/`cursor_y` (core `D_8011ECF6`/`FA`, screen-centre relative).

### 3.3 Voice sync state

Four words in the EVENT range of main bss (a main-only symbol file): `voice_table_a` (80125BF4), `voice_table_b` (80125BF8), `voice_index` (80125BFC), `voice_state` (80125C00; 0 pending, 1 playing, 2 done).

### 3.4 Girl score tables

All in `.data` at 800B4100-800B437C, indexed by girl 0-12 unless noted (section 8 has the values).

| name | address | shape |
|---|---|---|
| `girl_tokimeki_weight` (core) | 800B4100 | u8[13], percent |
| `girl_bonus_param_mask` | 800B4110 | u8[13] |
| `girl_param_ladder_kind` | 800B4120 | u8[13], 1-6 |
| `girl_param_bonus` | 800B4130 | s8[13][8] |
| `param_ladder_base` | 800B4198 | u8[7] |
| `girl_affection_weight` (core) | 800B41A0 | u8[13], percent |
| `closeness_grid` | 800B41B0 | u8[20][20] |
| `shiori_param_minimum` | 800B4340 | u8[8], entries 0-6 used (label D_800B4341 sits on entry 1) |
| `girl_syoushin_weight` | 800B4348 | s8[12] |
| `girl_points_mask_strong`, `girl_points_mask_normal` | 800B4354, 800B4360 | u8[12] each |
| `points_param_curve`, `points_stress_curve` | 800B436C, 800B437C | s8[15] each |
| `girl_short_name_table` (kept) | 800B35F4 | 16 string pointers: family names of girls 0-12 and 14, slot 13 and 15 hold given names (section 6) |
| `name_suffix_kun`, `name_suffix_san` | 800B3634, 800B3638 | the two suffix strings |

## 4. Text system

### 4.1 Queueing

`set_kanji_string(x, y, color, string, ot_slot)`:

- Returns -1 and does nothing when the list already holds 52 entries.
- A null string, or one whose first character is not complete (byte 0 or byte 1 is 0), only sets `active = 0` on the next free entry; the entry is still consumed and its `glyph_count` keeps whatever the slot held before (quirk, section 14).
- Otherwise it stores `y`, `color`, `x`, `ot_slot`, sets `glyph_count = load_string(string)` and `active = 1`, and returns the new count (`++count`).

`load_string(string)` reads two bytes per character as a Shift-JIS code until either byte is 0. A code below 0x8140 prints a debug error line and ends the string (single-byte ASCII is not supported by this system; the name codes of section 5 are replaced before the call). Otherwise it finds the first range of `text_font_range_table` whose start is above the code, takes the range before it, and the glyph number is `code - range.first + range.glyph`. Codes at or beyond the last entry (0x9873) load nothing and do not count. The font covers 3418 glyphs: punctuation, full-width digits and Latin, kana, Greek, Cyrillic (0x8140-0x84BE, glyphs 0-452) and the JIS level-1 kanji (0x889F-0x9872, glyphs 453-3417), so every on-screen text is full-width. It returns the number of glyphs loaded.

`vload_font(glyph)` unpacks the 12 x 12 bitmap with `text_glyph_expand` into ring slot `total & 0xF`, uploads it as a 16 x 12 pixel block (4 halfwords x 12 rows) to the next free atlas cell and bumps `total`. The atlas has three pages of 128 cells (8 columns x 16 rows, 16 x 16 pixel cells) at VRAM x 0x320, 0x360 and 0x3A0, y 0x100-0x1FF; slot `t` is at column `t & 7`, row `(t >> 3) & 15`. A fourth page does not exist (slots 0x180 and up would overwrite page 3). `text_glyph_expand` reads the bitmap as a 144-bit stream, row by row, most significant bit first, and writes each pixel as a nibble (0 transparent, 1 text colour), two words per row.

So glyph slot numbers are consecutive in queue order: the j-th character of entry e has slot `sum(glyph_count of entries 0..e-1) + j`. Every character occurrence is uploaded again (no sharing). `k_reset(n)` rewinds the allocator to the end of entry n-1 (`total = revealed = sum of the first n glyph counts`, `count = n`), so later entries overwrite the same cells.

### 4.2 Typewriter

`revealed` is the number of glyphs drawn. One tick `k_disp_inc()`:

```
if (frame_count % (speed + 1) == 0) { if (revealed < total) { revealed++; return 0 } return 1 }
return (revealed == total) ? 1 : 0
```

It returns 1 whenever everything is shown, so callers also use it as the end test (`check_end_k` is the same test without a tick). Speed values: 0 one glyph per frame, 1 every 2 frames, 3 every 4 frames; the settings screen offers slow = 3, normal = 1, fast = 0; ENDING uses 8 and 16, BUNKA_SD 5. The speed is saved with the game (the memory-card code copies `get_k_speed()` into the save block and calls `k_speed_set` on load); a freshly reset state has speed 0.

- `k_disp_start(n)`: `revealed` = glyph count of the first n entries, so those entries appear at once and the typewriter starts at entry n (TEL calls it with the current count).
- `k_sub_reset_point_set()`: `mark = count`. `k_sub_reset()`: `k_reset(mark)`. `k_sub_disp_start(n)`: `k_disp_start(mark + n)`. A screen keeps a fixed header below the mark and replaces the message above it. The settings screens use exactly this: queue the items, set the mark, queue the message, `k_sub_disp_start(0)`; after a choice, `k_sub_reset()` and a new message.
- `k_disp_goto_line_end()`: scans the 52 entries for the first running glyph sum that reaches `revealed` and sets `revealed` to it (end of the entry being typed); returns it, -1 if none.
- `get_now_k()`: the first entry whose running sum reaches `revealed`, i.e. the entry holding the last revealed glyph; 0xFF while `revealed` is 0.
- `check_set_k()`: low byte of `count`, used as the index the next queued entry will get.
- `k_disp_switch(n, on)`: entry n `active` = 1 if `on > 0` else 0 (inactive entries keep their glyph slots and are skipped by the draw loop).

### 4.3 Drawing (`disp_string`, once per frame from `frame_render`)

For glyph stream index `i` below `min(total, revealed)` it finds the entry `j` that owns the glyph (`k` = index inside the entry) and, if `entries[j].active == 1`, emits two 16 x 16 sprites (code 0x7C) into ordering slot `ot_slot` (0 is treated as 1):

- coloured sprite at screen x = `entry.x + screen_center_x + k * glyph_advance`, y = `entry.y + screen_center_y`, tint = `screen_brightness`, palette (x = 0x100 + (color & 8) * 4, y = 0x1F8 + (color & 7));
- shadow sprite one pixel right and down, tint 0, palette 0x10 further right (the dark twin);
- texture coordinates `u = (i & 7) * 16 + 0x80`, `v = (i & 0x78) * 2`, texture page 0x300, 0x340 or 0x380 by `i < 0x80`, `< 0x100`, else; draw-mode primitives for the page are added after the sprites of a run so they take effect first.

`glyph_advance` is 0xE (14 px) after `text_system_reset`, set to 0x10 by `text_set_advance(0x10)` (NAME_ENT `func_80132198`) and back to 0xE by NAME_ENT and OPTION code. A glyph occupies 12 pixels of its 16-pixel cell, so neighbouring cells overlap in their transparent margins.

Palettes: 16 text colours, two entries each (transparent, colour), uploaded at VRAM (0x100, 0x1F8) for colours 0-7 and (0x120, 0x1F8) for 8-15, with the shadow twins at (0x110, 0x1F8) and (0x130, 0x1F8). Colour 0 is the pale default of the message window, 3 is used by the settings items, 0xF is near-black (NAME_ENT prompts). `text_system_reset` also uploads two blank strips of `text_blank_strip` at y 0x1FF under pages 0 and 1, clears pages 0 and 1 with `text_glyph_pages_clear(2)` (ENDING clears 3) and a 2 x 8 block at (0x102, 0x1F8) to grey.

### 4.4 Coordinates

All text coordinates are relative to the screen centre of the 320 x 240 frame (160, 120); x -128 is 32 px from the left edge. The message window has three rows: y 0x32, 0x42, 0x52 (170, 186, 202 px from the top, 16 apart): `TEXT_ROW_0_Y`..`TEXT_ROW_2_Y`.

## 5. Speaker labels and dialogue blocks

### 5.1 Name plate

`gnsx(string)` queues the string at row 0 with colour 0 and x by byte length: 2 bytes (1 character) -114, 4 bytes -121, 6 bytes or more -128. The three x values centre a 14 px advance string on x = -107, a plate that fits at most three characters. Callers pass a girl's short name (`girl_short_name_table[girl]`), the player's family name or a fixed label (NAME_ENT prologue, TEL, ETC).

### 5.2 Dialogue line (`sndi(string, row, style)`) and block (`sndisp(string, row, style)`)

`style` is a byte whose low nibble is the speaker (a girl id; 15 = the player/narration, core `GIRL_PLAYER`) and whose high nibble selects the layout. It is also what scenes pass to `voice_play`. Values seen at call sites (117 of 0xF, 89 of 0xD, 70 of 0x1F, 21 of 0x2F, 20 of 0xC, 4 of 0xFF, 4 of 0xA, the rest the speaker variable).

`sndi`:

1. Copies up to 64 bytes of the line and clears `text_scroll_state`.
2. Name codes (a single ASCII letter on an even byte offset, so it cannot be half of a two-byte character): if `style & 15 == 15`, the first `P` is replaced by `get_p_name(buf, 15)` (the player's full name) and then the first `G` by `get_g_name(buf, current_girl)`; otherwise only the first `P` is replaced, by `get_p_name(buf, current_girl)` (how the current girl addresses the player), whatever the speaker is. At most one replacement per code per line.
3. Opening mark on row 0: `style == 0xF` double corner bracket, `0x1F` opening parenthesis, `0x2F` none, anything else (including 0xFF) the ordinary corner bracket (`SPEECH_*`). The mark is a character of the queued line, so row 0 has one glyph more than its text (emulator probe 5).
4. Placement: rows 0, 1 and 2-and-up map to y 0x32, 0x42, 0x52; x is -0x56 (-86) for row 0 and -0x48 (-72) for later rows, or -0x80 for every row when `style == 0xFF` (no speaker plate). The first line is 14 px further left than the others so the opening mark hangs in the margin. Colour 0, `ot_slot` 0.

`sndisp` splits the string at ASCII spaces on an even byte offset (up to 5 splits, so 6 lines), replacing each space by 0, then calls `sndi` for the first piece at `row` and for the others at `row + 1 + n`. Around the calls it records `text_first_line = check_set_k()` before and `text_lines_total = check_set_k()` after, and sets `text_scroll_state = 0x80` (after the last `sndi`, which cleared it). So a block is a run of consecutive entries and a scene queues the plate first, then the block, as in this common step:

```
k_sub_reset(); gnsx(name); sndisp(text, 0, style); voice_play(a, b, index, style); substep++
... next step each frame: dialogue_text_step()   (or xa_wait() / text_wait_confirm())
```

Text is never wrapped by the code: a line break exists only where the script string has a space.

### 5.3 Per-frame steps

All three steps end by bumping `substep` (the caller's step function switches on it).

`text_wait_confirm` (core `func_80052000`, overlay prompts): `k_disp_inc()`; when `pad_pressed` has circle, cross or START (0x860): `k_disp_goto_line_end()` and, if everything is now shown, `substep++`. One press therefore finishes the current line and, on the last line, also advances.

`xa_wait` (O.BIN name; no voice code in this build) and `dialogue_text_step` (voice aware) share the scroll logic. A block shows three rows, `text_first_line` .. `text_first_line + 2` (call it F). With `text_scroll_state & 0x80`:

- while `get_now_k()` is at most F + 2: tick the typewriter; when the typing reaches F + 3 (a fourth line) and `text_lines_total >= F + 3` that entry is hidden (`k_disp_switch(now, 0)`) so it does not overprint row 2;
- when `get_now_k()` is beyond F + 2 (the typewriter waits): phase 0 hides line F, then lines F + 1 and F + 2 move up 2 px per frame (phase 1); when line F + 1 reaches y 0x32 the two lines snap to y 0x32 and 0x42, F increases by 1, the hidden fourth line (now F + 2, at y 0x52 from `sndi`) is shown and typing resumes. A block of n lines thus scrolls n - 3 times, 8 frames each.

Without the 0x80 bit the step is a plain typewriter.

The continue marker: when the typing is finished (`r == 1`) the step shows the marker with `text_marker_set(1)` or, when the marker actor already has bit 7 set, runs its bobbing step (`func_8006C934`). A confirm press (`pad_pressed & 0x60`): when finished, hide the marker, `substep++`, `text_scroll_state = 0`; otherwise `k_disp_goto_line_end()` (completes the line being typed, one press per line).

`dialogue_text_step` adds the voice wait. `voice_mode` is 0 off, 1 on, 2 on and cancelable (core `VOICE_*`).

```
non-scroll typing: voice_mode 0            -> tick every frame
                   voice_mode 1 or 2       -> tick only while voice_state != 0
voice_state 0 -> 1 when cd_audio_state() has bit 4 (clip playing)
voice_state 1, when cd_audio_state() has bit 2 (clip done):
                 if voice_index != -1 and table_a[idx].start != table_b[idx].start and
                    table_a[idx].start == start word of the current request:
                      cd_audio_request_set(table_b[idx].start, .end, current_girl); snd_command(0x300);
                      voice_state = 0                      (clip B follows clip A)
                 else voice_state = 2
marker when typing is finished and voice_state == 2 (or voice_mode == 0)
cross press, typing finished, voice_mode 2: voice_state = 2, sound effect 0x74 and cd_audio_command(9)  (cut the clip)
confirm advances only if voice_mode is 0, or voice_state == 2
```

`voice_play(a, b, index, style)` (core): when `voice_mode != 0` and the speaker nibble is not 15, issues the clip request with `cd_audio_request_set(start, end, style)` and `snd_command(0x300)` (clip words come from the pair tables at `index * 2`, or are `a` and `b` themselves when `index == -1`); then stores the tables and index and sets `voice_state` to 2 for speaker 15 (no voice), else 0. In the tables found (ETC, TEL) A and B hold the same pairs, so every observed line is one clip; B only matters when the tables differ (HYPOTHESIS: a second recording of a line).

Main uses `voice_play` with immediate clip words (`index -1`, speaker 0xE) in the system menus (8005A0B0); overlays (TEL 116 times, ETC 2) pass tables.

## 6. Name builders

`get_g_name(buf, girl)` (name text uses for a girl; returns 1, or -1 when `girl` is outside 0-15, the buffer is still filled using `girl & 15`):

| girl | result |
|---|---|
| 0 (Shiori) | her given name, no suffix (a literal) |
| 10 (Yumi) | the given name in `girl_short_name_table[15]` + the familiar suffix `chan` (a literal) |
| 12 (Rei) | the family name in the table, no suffix |
| any other | the table entry (family name; for girl 13 the table holds his given name) + the polite suffix `san` (a literal) |

`get_p_name(buf, form)` builds how girl `form` addresses the player, from the player's family name (`player_family_name`), given name, nickname and the two suffixes. `high` means `get_g_zyotai_s(girl) & 0x80` (weighted affection at least 80). N nickname, G given name, F family name; `+kun` / `+san` append `name_suffix_kun` / `name_suffix_san`.

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
- Mode 0: runs the pointer mover `cursor_update` (core `func_8006BA40`, d-pad or mouse moves the actor-0 pointer by `cursor_speed`), then the cursor = the first item with `hover != 0`.
- Mode 1: unless one of L2, R2, L1, R1 is held, up or left on `pad_pressed` steps to the previous item, down or right to the next. Then the cursor wraps (>= count to 0, < 0 to count - 1) and skips disabled items in the direction of travel (at most 16 steps). The pointer is moved to the item: `cursor_x = item.x + item.w - 4`, `cursor_y = item.y + 8`.
- Mode 2: while L1 is held: on the frame L1 is pressed the cursor adopts `menu_hit[menu]`; later frames step like mode 1 and move the pointer to the item; without L1: `cursor_update` and the cursor becomes the last item with `hover != 0`.
- Mode 3: up/down call `get_near_menu(1, -1 or +1)` (vertical), left/right `get_near_menu(0, -1 or +1)` (horizontal); a result of -1 keeps the cursor; a cursor at or above `count` is set to `menu_cursor_to_edge(menu, 1)` (last usable item), below 0 to `menu_cursor_to_edge(menu, 0)` (first usable item); the pointer follows as in mode 1.
- Finally `cursor_x` is clamped to -150..140 and `cursor_y` to -100..100.

`get_near_menu(axis, dir, menu)`: uses at most 16 items. For axis 0 the signed distance of every item from the cursor item is its x difference, for axis 1 its y difference; items on the wrong side of `dir` get distance 0 (ignored; a distance of 0 is always ignored); disabled items are ignored. Candidates in the same row (axis 0, equal y) or column (axis 1, equal x) win over all others, the nearest of them first; if there is none the item with the smallest distance wins and a tie goes to the one closer on the other axis. Returns -1 when no candidate.

Screens read the result as `menu_cursor_1` (or `menu_hit[1]`) on a confirm press; the same code works for every mode because mode 0 sets the cursor from the hover.

### 7.3 Highlight bar

`menu_bar_show(menu)` draws a gouraud box over the item at `menu_cursor[menu]` when that is a valid index: position and size from the item, ordering slot = bits 24-27 of `menu_bar_style[menu]`, colour = bits 0-23, box mode = bits 28-30, semi-transparent = bit 31 (passed as 0x80 in the mode argument); it then adds a dither-on draw-mode primitive to the same slot. `menu_bar_color(menu, rgb, slot, mode)` packs a style (`rgb + (slot & 15) << 24 + (mode & 15) << 28`); an index of 0x10 or more instead restores the two defaults 0x832F7FAF (menu 0, blue) and 0x822F7F2F (menu 1, green); boot and the settings screen do that. The emulator shows both defaults in the title-menu state.

## 8. Girl scores

`girl` below is the index modulo 13 (`GIRL_SCORE_MODULUS`), so girls 13 and 14 read the records of girls 0 and 1.

`get_h_yuukou(g)` = clamp(`affection * girl_affection_weight[g] / 100`, 0, 100) (integer division). `get_h_tokimeki(g)` = clamp(`tokimeki * girl_tokimeki_weight[g] / 100` + bonus, 0, 100). The bonus: for every parameter p in 1..6 (liberal arts, science, art, athletics, trivia, looks; guts never counts) whose bit is set in `girl_bonus_param_mask[g]`, find the first step k in 0..6 with `value(p) > T[kind][k]` and add `girl_param_bonus[g][k]`; add `girl_param_bonus[g][7]` when none matches. Bit 7 of the masks of girls 0, 4 and 6 is dead. Effective thresholds `T[kind][k]` = `param_ladder_base[k]` + offset:

| kind | girls | thresholds k = 0..6 |
|---|---|---|
| 1 | 0 | 178, 135, 110, 90, 55, 35, 10 |
| 2 | 1, 4, 5, 6, 8 | 128, 100, 90, 70, 50, 30, 10 |
| 3 | 2, 7 | 178, 150, 140, 90, 70, 50, 30 |
| 4 | 3 | 138, 110, 100, 80, 50, 30, 10 |
| 5 | 9 | 148, 120, 110, 90, 50, 25, 10 |
| 6 | 10 | 128, 100, 90, 70, 45, 25, 5 |

(`param_ladder_base` is 128, 100, 90, 70, 50, 30, 10.) Girls 11 and 12 have kind 0, an empty mask and zero bonuses.

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

`girl_scores(closeness[16], points[16])`: fills 11 bytes of scores through the two table functions, then for i in 0..15 (only 0-10 are meaningful: entries 11-15 of the closeness array come from uninitialised stack bytes) sets `points[i] = 0`, divides both scores by 5 (cap 19) and stores `closeness[i] = closeness_grid[tokimeki / 5][affection / 5]`. Then `girl_points_shiori` and `girl_points_heroines` fill `points`.

`closeness_grid` is about `5 * min(a / 5, max(3, t / 5))` with a few entries one lower; rows are tokimeki buckets, columns affection buckets. Rows 0-3 are identical, so tokimeki below 20 gives at most about 15; the value climbs 5 per affection bucket up to the cap of the row. Full table (row = tokimeki / 5, 20 columns):

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

`girl_points_shiori(closeness, points)`: `points[0] = 0x311` (785) when all of: `closeness[0] >= 0x55`; the health bits of `player_flags` (bits 8-11) are clear; each of the parameters 1-7 is at least `shiori_param_minimum` (130, 130, 130, 130, 120, 100, 100 for liberal arts, science, art, athletics, trivia, looks, guts); the sum of her `event_counters[1..35]` is at least 8; her `syoushin` is below 0x33. Otherwise `points[0]` stays 0. (A fixed value, so only the comparison with the scheduler's 0x1F4/0x1F5 thresholds matters.)

`girl_points_heroines(closeness, points)`, for girls 1-10: `v = closeness * 4 + syoushin * girl_syoushin_weight[g]` (weights -4, -2, -4, -2, -2, -2, -2, -4, -4, -3 for girls 1-10); girl 2 adds `points_stress_curve[min(stress, 140) / 10]`; for every parameter p in 1..7 (guts counts here) set in `girl_points_mask_strong[g]`, with `c = min(value(p) / 10, 14)`, add `curve[c] + curve[c] / 2` (x1.5, integer halves toward zero), and for every p set in `girl_points_mask_normal[g]` add `curve[c]` (`curve` = `points_param_curve`); finally with `n` = sum of `event_counters[1..35]`: `n >= 4` adds `n * 10`, otherwise the whole value is replaced by 0. Bit 0 of the masks is never tested (the loop body has a dead `m == 7` branch that divides by 20, which suggests it was meant to include parameter 0). Tables:

```
points_param_curve   -100 -75 -50 -25 0 10 20 30 40 50 60 70 80 90 100        (index = value / 10, capped 14)
points_stress_curve   100 85 65 50 37 25 17 0 -17 -34 -50 -63 -75 -87 -100    (index = stress / 10, capped 14)
girl_syoushin_weight  0 -4 -2 -4 -2 -2 -2 -2 -4 -4 -3 0
girl_points_mask_strong  1:0x04 2:0x02 3:0x01 4:0x01 5:0x08 6:0x08 7:0x02 8:0x04 9:0 10:0x04
  parameters             1: science; 2: liberal arts; 3, 4: none; 5, 6: art; 7: liberal arts; 8, 10: science
girl_points_mask_normal  1:0x0A 2:0x04 3:0x06 4:0x88 5:0x82 6:0x81 7:0x14 8:0x0A 9:0x76 10:0x05
  parameters             1: liberal arts, art; 2: science; 3: liberal arts, science; 4: art, guts;
                         5: liberal arts, guts; 6: guts; 7: science, athletics; 8: liberal arts, art;
                         9: liberal arts, science, athletics, trivia, looks; 10: science
```

The meaning of the points as an ending-candidate score is a HYPOTHESIS: the scheduler tests them against about 500 before the final scenes (m-scheduler section 16).

## 10. Birthday and club helpers

`birth_day_check_days(who, month, day)`: a month of 13 counts as 1 (the December look-ahead of the scheduler). `who & 15 == 15` reads `player_birthday` (month in bits 0-3, day in bits 4-8), any other value the birthday word of `girls[who & 15]`; returns 1 when both match, else 0. `birth_day_check(who)` passes today's `month` and `day`. The C declares it `void` (a matching artifact); it returns the same value, and callers use it as a flag.

`player_in_sports_club()`: 1 when the club in `player_flags` bits 12-15 is from `CLUB_BASEBALL` to `CLUB_BASKETBALL` (6-10).

`girl_find_by_club(club)`: the first girl from 0 to 10 whose `club` field (bits 20-23 of the girl flags) equals `club`, else -1.

`set_c_girl(girl)`: `current_girl = girl` (TEL).

## 11. Debug VRAM viewer (`mode_debug_vram_view`, mode 0xFE)

Developer tool, not part of the game. Entered by the mode dispatcher on mode 0xFE and by the main loop when the debug flag (`D_800B3220`) is nonzero and pad 2 L1 and R1 are held together (0xC0000 in the pad word). It sets `pad_pressed` to 0x80000, zeroes the display start `(debug_vram_view_x, debug_vram_view_y)`, then loops once per iteration: pad 2 up/down change y by -16/+16, pad 2 right/left change x by +16/-16 (each clamped at 0), polls the pad, sets the display area to that origin and waits for a frame; it ends when pad 2 bit 0x08000000 (START) is pressed, restores the video mode from `video_width`, `video_height`, `video_interlace` and returns. A port drops it.

## 12. Connections

- Every scene overlay and most main modes call `text_system_reset` at init, queue text with `set_kanji_string`/`sndisp`/`gnsx`, and step it with `k_disp_inc`, `xa_wait`, `dialogue_text_step` or `text_wait_confirm`; `frame_render` calls `disp_string` once per frame.
- The script dialogue runner (`dialogue_run`, 8007C030) builds its own lines with `set_kanji_string` and `dialogue_names_set` (core), which uses `get_p_name` and `get_g_name`; `sndi` and `sndisp` are used by scene code that writes fixed lines (rumors, health notices, telephone, TEL, ETC).
- Menus: HUD command screens, title (the pad branch of `func_800623E4` bypasses `menu_check`), settings, club joining, holiday phone and club screens, NAME_ENT, OPTION, TEL (date spots), ETC (`menu_girl_taku_set`).
- `get_g_zyotai_s` is the base of core `girl_state_rank` (1,106 uses); `get_g_zyotai_h` is stored by `event_log_add`; `get_h_tokimeki` and `get_h_yuukou` also drive `syoushin_up` (affection < 10), the sick-visit pick in the health check (tokimeki of 0x33 or more), `pick_top_girl` (80085E30, used by MASTER, RENSYU and TAIIKU: the girl with the best tokimeki + affection) and score tests in SHUGAKU, SHOUGATU, VALEN, KANGEI, GEKO, DATE2, BUNKAKEN and ENDING; RPG_BAT tests `get_g_zyotai_s`.
- `girl_scores`: `miharu_final_event_ready`, `command_girl_select`, `geko_judge`, `date_sasoi_judge`, `yuina_top_candidate_check` (m-scheduler).
- `voice_play` and `dialogue_text_step` depend on the CD layer (h-cdio): `cd_audio_request_set`, `cd_audio_state`, `cd_audio_command`, the sound command queue (`snd_command` 0x300 = voice class, 0x74).

## 13. Port notes

- Replace the glyph atlas, the palettes, `disp_string` and the VRAM upload by normal font rendering. Keep: the 52-entry limit (or an unbounded list with the same order), the screen-centre-relative coordinates (a 320 x 240 design space), the 14 px advance (16 px on the NAME_ENT screen that asks for it), the one-pixel dark shadow, the colour index (16 colours) and the ordering slot as a draw layer.
- Keep the typewriter exactly: ticks on `frame_count % (speed + 1) == 0` at 60 Hz, speed 0/1/3 from the settings, `k_disp_start` and the mark/reset scheme (screens depend on it), the "confirm finishes the current line" rule, the three-row window with the 8-frame scroll for blocks of four or more lines, the voice gating of `dialogue_text_step`, and the rule that text never wraps by itself.
- Keep the name-code and honorific rules (sections 5.2 and 6) and `get_g_zyotai_s/h`, `get_h_*` and `girl_scores` bit for bit (tables in sections 8 and 9; a Python model of sections 8 and 9 reproduced the original's closeness and points for 11 girls in two poked scenarios, section 15).
- Menus: keep the item model, the four menu modes and their navigation, the SELECT mode switch, and the pointer/cursor rules, or reduce to mouse plus pad focus with the same item rectangles; `menu_bar_style` is a highlight colour per menu.

## 14. Quirks kept

- `set_kanji_string` with an empty string consumes an entry and leaves its `glyph_count` stale.
- `get_k_work(0x34)` returns the address of `text_state` (the bound is 0x35); callers pass small values.
- `disp_string` moves to the next entry once per glyph, so two empty entries in a row would misattribute glyphs; empty entries only occur with the stale count above.
- `load_string` drops codes at or beyond 0x9873 without a glyph and stops at codes below 0x8140 after a debug print.
- A third glyph page is the last; slot 0x180 and up wrap into page 3.
- `get_now_k` returns 0xFF while `revealed` is 0; `dialogue_text_step` and `xa_wait` would then treat the block as beyond its third row. In practice the name plate before the block gives `revealed` at least 1.
- `menu_set` writes `menu_count` and `menu_hit` before it rejects `menu >= 2`.
- `menu_cursor_update` does not skip disabled items when no direction was pressed and may leave the cursor on one.
- `get_h_tokimeki` ignores guts (the loop ends at parameter 6) and bit 7 of the masks; `girl_points_heroines` ignores bit 0.
- Girls 13 and 14 score as girls 0 and 1.
- `girl_scores` writes 16 closeness bytes from 11 inputs; entries 11-15 are garbage.
- `birth_day_check` has a `void` signature but returns the flag.

## 15. Evidence from the emulator

All runs start from a title-menu save state made in this worktree (`build/emu/states/title_menu.sstate`, not committed); several poke GameState bytes right after the state loads (a local copy of the probe runner with a `--poke` option, kept in `build/scratch`, not committed). Ten probes:

1. **Menu state at the title** (`func_800623E4` break): `menu_count_0` 2, `menu_count_1` 0, `menu_mode` 1, `menu_cursor_0` 0, `menu_hit_0` -1; items of menu 0 at x -48, y 66 and 82, w 160, h 20. This fixes the `MenuItem` layout (x s16, y s16, w, h, hover, disabled) and the interleaving item-major, menu-minor. The title uses the pad branch, so `menu_check` is not reached; `menu_bar_style` reads 0x832F7FAF and 0x822F7F2F.
2. **Text entries in NAME_ENT** (`set_kanji_string` breaks after pressing circle at the title): `text_state` after the first call of one screen: count 1, total 9, mark 0, speed 1, revealed 0, advance 0xE; entry 0: x -64, y 0x20, color 0xF, glyph_count 9, ot_slot 0, active 1. On a later screen three entries at y 0x20, 0x30 and 0x40 had glyph counts 15, 12 and 12 and `total` 0x0F, 0x1B, 0x27, the running sums.
3. **Dialogue block** (mode byte poked to 0x44, the rumor report): `sndisp` with style 0x1F, then two `sndi` calls: `set_kanji_string` at x -0x56 and y 0x32, then x -0x48 and y 0x42. After it: `text_first_line` 2, `text_lines_total` 4, `text_scroll_state` 0x80; `text_state.revealed` advances by 1 per frame at speed 0 (3, 4, 5 on three consecutive frames) and `xa_wait` runs each frame.
4. **Score pipeline** (mode byte poked to 0x30, girl records and parameters poked, break at the instruction after `girl_scores` returns): the buffers of the original matched a Python model of sections 8 and 9 for all 11 closeness and 11 points values in two scenarios (the first with several girls below the 4-counter limit and Shiori failing her conditions, the second with Shiori at 785 and girl 1 at 0). This covers the weights, the six ladders, the bonus table, the grid, the point masks and curves.
5. **Scroll of a six-line block** (the rumor string rewritten in RAM as six two-character lines separated by spaces, mode byte 0x44): `sndisp` made six entries (the first one glyph longer than its text: the opening mark is part of the line), `text_first_line` 2, `text_lines_total` 8, `text_scroll_state` 0x80. Lines 0-2 typed at one glyph per frame; when the typing reached the fourth line (revealed 10) that entry became inactive, phase 1 started (`text_scroll_state` 0x81), line 0 went inactive and lines 1 and 2 moved from y 66 and 82 up by 2 per frame (64, 62, ... 52); 8 frames after the start the lines snapped to y 50 and 66, `text_first_line` became 3, the hidden line became active at y 82 and the typing went on. This repeated for lines 4 and 5 (three scrolls for six lines, 10 frames apart: 8 for the slide, 2 for the new glyphs); at the end the window showed lines 3-5 with `revealed` equal to `total`.
6. **Menu navigation** (mode byte poked to 0x34, stage 4, step 0: the system settings screen): `menu_set(0, 8, ...)` registers 8 items at x -124, y -64 + 12 * i, 96 x 12; item 6 has width 0 (disabled). With `menu_mode` 1 pad presses (DOWN, DOWN, RIGHT, DOWN, UP) moved the cursor 0, 1, 2, 3, 4, 3 and the pointer to `x = item.x + w - 4 = -32`, `y = item.y + 8`; a further run showed DOWN skipping the disabled item (5 to 7), DOWN on the last item wrapping to 0 and UP on item 0 wrapping to 7. SELECT changed the stored `menu_mode` from 1 to 2 and set the cursor to -1 for one frame, after which the item under the pointer became the cursor again.
7. **Menu mode 3** (same screen, `menu_mode` poked to 3): DOWN stepped 0 to 5, skipped the disabled item to 7, stayed on 7 at the end (no wrap), UP went 7 to 5 to 4, RIGHT with no item to the right kept the cursor; the cursor item showed `hover` 10.
8. **Voice sync** (the club-joining message of the system menus: mode 0x34, stage 5, step 2, substep 3 poked, `player_flags` club set to 6 so the captain's voiced lines run, `voice_mode` poked): with mode 1 `voice_state` stayed 0 until `cd_audio_state` showed bit 4 (frame 7); the typewriter started at frame 9 (one glyph per frame) and finished at frame 37; a circle press at frame 100 did nothing; the CD state showed bit 2 at frame 222 (the clip lasted about 3.6 s) and `voice_state` became 2 at frame 223; a circle press at frame 240 advanced the substep. With mode 2 a cross press at frame 100 (text finished, clip still playing) advanced at once.
9. **`get_p_name` forms** (TEL's first call line, reached through the loader: mode byte 2 with target 0x45, stage 1, step 1, substep 2; player names poked to three known characters each; the style argument of the `sndisp` call patched in RAM to a non-narration value and `current_girl` and the girl's affection poked after the overlay load): for all 16 girl ids in both affection states (weighted affection 0 and at least 80, 32 runs) the string handed to `set_kanji_string` equalled the form of the table in section 6; with the unpatched style 0xF the code came out as the full name (family name plus the first six bytes of the given name), and the name plate (`gnsx`, six-byte family name) sat at x -128.
10. **`get_g_name`** (TEL step 3 substep 12, the line with a girl name code and style 0x1F, `current_girl` poked 0-15): every id gave the name of section 6 (id 0 the fixed given name, 10 the given name from slot 15 plus the familiar suffix, 12 the family name alone, all others including 13, 14 and 15 the table entry plus the polite suffix; id 15 is therefore girl 10's given name plus the polite suffix).

Not run: the second clip of a two-table line (every table found has equal halves), the pointer hit test (needs a mouse) and the `get_near_menu` row and column preference on a grid of items; these come from the code only.

## 16. Open questions and disagreements with core

1. Core names `text_lines_total` (+0xF60) and `text_first_line` (+0xF61): the code shows +0xF61 is the index of the first entry of the dialogue block and +0xF60 the index one past its last entry (both are `check_set_k()` values, taken before and after `sndisp` queues the lines, emulator: 2 and 4 for a two-line block after two earlier entries). `text_block_first` and `text_block_end` would say that; this unit does not rename them.
2. Core's `TextEntry.extra` is the ordering-table slot (`disp_string` passes it as the slot of `AddPrim`; 0 means 1) and `attr` carries only the colour index (bits 0-3); `ot_slot` and `color` are suggested, and `attr` is not in the abbreviation table.
3. Core comment rows marked HYPOTHESIS that the code proves: `k_disp_switch` (sets the entry's `active` byte, tested by `disp_string`), `check_end_k` (`revealed == total`). The `menu_bar_show` comment says "hovered item"; the bar is drawn on the item at `menu_cursor[menu]`.
4. `GIRL_PLAYER` (15) in core: the spare name-table slot 15 is girl 10's given name (this unit's `girl_short_name_yumi_given` row); the full-name table (`girl_full_name_table`) slot 15 is the placeholder for the player's name before it is entered.
5. `girl_short_name_table` holds family names except slot 13 (given name of girl 13) and slot 15 (given name of girl 10); it is called "short" in the patch notes; no rename proposed.
6. Whether `voice_table_b` ever differs from `voice_table_a`: the two overlays that pass tables (TEL, ETC) pass equal ones, so the second clip may be dead; the other callers pass immediate words.
7. Meaning of `closeness` and of the points (ending candidates?) is inferred from the scheduler's thresholds only (HYPOTHESIS).
8. Why `get_h_tokimeki` stops at parameter 6 (guts never feeds tokimeki) while the points loop includes parameter 7: probably an original off-by-one, unconfirmed.
9. Purpose of the two blank strips at y 0x1FF (`text_blank_strip`): HYPOTHESIS: the last pixel row of the glyph cells must be transparent.
10. The settings screens' text speed items map slow, normal, fast to speeds 3, 1, 0; the speeds 5, 8 and 16 of BUNKA_SD and ENDING belong to those units.
11. EVENT-range symbols: `text_glyph_upload_ring`, `debug_vram_view_x/y` and the four `voice_*` words lie in 0x800F6000-0x80126000 and must go to the main-only symbol file.

## 17. Rename summary

150 rows: 10 func, 53 data (10 of them alias labels that splat made inside a table), 3 field, 5 file, 56 const, 23 comment (no type rows: core renames `KWork`, `Entry8`, `GsRecF90`, `MenuSelection5`). Notes starting with HYPOTHESIS: 1 (`closeness_grid`). Renames of existing non-O.BIN names that fail the module-prefix or abbreviation rules: `TextState.pos` -> `revealed` (pos is not in the abbreviation table), `glyph_expand_1bpp` -> `text_glyph_expand`, `font_glyphs` -> `text_font_glyphs`, `font_range_table` -> `text_font_range_table`; `text_list_count` -> `text_state` fixes a misleading name. Names suggested by the m-scheduler spec and adopted: `girl_scores`, `girl_points_shiori`, `girl_points_heroines`, `player_in_sports_club`.
