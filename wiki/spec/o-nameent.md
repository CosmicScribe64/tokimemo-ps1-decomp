---
type: spec
updated: 2026-10-24
sources: ["src/ovl/NAME_ENT", "config/renames/proposals/o-nameent.tsv", "include/ovl/NAME_ENT.h"]
---

# Overlay NAME_ENT: file screens, name entry, prologue and attract opening (port spec)

Phase 2 spec for [[tickets/T-27030-p2-o-nameent]] ([[tickets/T-27000-phase-2]]). Facts as the matched C implements them, checked in the emulator where noted ("emu"). Names are the proposals in `config/renames/proposals/o-nameent.tsv`. Shared names (GameState fields, `SND_CMD_*`, `CardResult`, `SlotStatus`, `PadBits`, card layer functions) come from [[spec/core]] and [[spec/m-flow]]. No game text is quoted: strings are described by role.

## 1. What the overlay is

NAME_ENT.EXN (disc LBA 0x95EA, slot 0x80132000; the boot code also reads it once before the title) holds four main modes that belong to the game start:

| Mode | Entry (old address, new name) | Role | Entered from | Leaves to |
| --- | --- | --- | --- | --- |
| 0x11 `MODE_OPENING` | 0x80144AD0 `opening_entry` | attract opening: a narrated picture story (stage 0), then the opening movie with the theme lyrics (stage 1) | title idle timeout (emu: frame 3534 of a boot without input), title pad code (stage 1 directly), pad 2 soft reset | title (0x12) |
| 0x13 `MODE_FILE_SELECT` | 0x8014231C `file_select_entry` | the file screens behind the title item "game start": memory-card check, system file, 14 album slots, create, New, Copy, Clear, load | title menu item 0 | 0x14 (new game, replay, first-day or last-day save), 0x34 stage 0xFF (a normal save), 0x10 (internal error) |
| 0x14 `MODE_NAME_ENTRY` | 0x8013209C `name_entry_entry` | the player's name, nickname, birthday and blood type, then the heroine Shiori's birthday and blood type; builds the new game | mode 0x13 (11 sites); the in-game data screen after loading a first-day or last-day save | 0x15 |
| 0x15 `MODE_PROLOGUE` | 0x8014AF28 `prologue_entry` | the voiced prologue of the first school day: 22 stages, 43 text lines | mode 0x14 | 0x34 stage 0xFF (command screen) |

[[spec/core]] calls 0x11 `MODE_PROLOGUE`, 0x13 `MODE_NAME_ENTRY` and 0x14, 0x15 `MODE_NAME_ENT_3`, `MODE_NAME_ENT_4` and asked this unit to settle them. The code shows that the prologue scene is mode 0x15 (it plays the script, the player's name appears on the speaker plate) and that mode 0x11 is the attract opening. The table of constants in `o-nameent.tsv` renames all four (`MODE_OPENING`, `MODE_FILE_SELECT`, `MODE_NAME_ENTRY`, `MODE_PROLOGUE`). Emulator run: title, circle on item 0 gives mode 0x13 at frame 173; creating an album gives 0x14; the last confirm gives 0x15 at frame 711 of that run; the prologue ends in mode 0x34.

Prefixes. The naming ADR gives this overlay the prefixes `name_entry_` and `prologue_`. The overlay also holds two sub-systems the ADR does not name, so this proposal adds `file_select_` (mode 0x13, 104 functions) and `opening_` (mode 0x11) as sub-module prefixes; types `NameEntry*`, `FileSelect*`, `Opening*`, `Prologue*`; constants with the same words in upper case. The folder is `src/ovl/name_entry/`. See open question 1.

Frame rate 60; all timers are in frames. The screen is 320 x 240 with the origin at the centre (x -160..159, y -120..119).

## 2. Source files (proposed names)

| Old | New | Contents |
| --- | --- | --- |
| `src/ovl/NAME_ENT/80132000.c` | `src/ovl/name_entry/name_entry.c` | mode 0x14 (61 functions) |
| `src/ovl/NAME_ENT/80139740.c` | `src/ovl/name_entry/file_select.c` | mode 0x13 (104 functions) |
| `src/ovl/NAME_ENT/80142A80.c` | `src/ovl/name_entry/opening.c` | mode 0x11 story and movie (9) |
| `src/ovl/NAME_ENT/80144AD0.c` | `src/ovl/name_entry/prologue.c` | mode 0x11 entry and mode 0x15 (83) |
| `include/ovl/NAME_ENT.h` | `include/ovl/name_entry.h` | header |

The overlay's own read-only data starts at 0x8014B140 (kana rows, message strings, jump tables), its initialised data and state at 0x8014CC30, and the image ends with 82 KB of zero padding behind `prologue_line_clips_second`.

## 3. Mode 0x13: file select (`file_select_*`)

### 3.1 What the player sees

A screen with a slot list (14 boxes in two columns of 7, labelled with the slot numbers, a "system file" present or absent indicator), four function buttons (Copy, New, Clear, Load in a 2 x 2 block at the top left), an information panel (save date, name, nickname, club, number of cleared girls, three switchable pages) and a message window. A background iris circle grows and drifts behind the panels. The pointer or pad cursor selects boxes; circle or START confirms, cross cancels.

### 3.2 State it reads and writes

| Name | Meaning |
| --- | --- |
| `system_data.slot_status[0..14]` (`SlotStatus`) | 0 empty, 1 valid file, 0xFF broken (set by the scan, validation and loads); index 0 is the system file, 1-14 the albums |
| `slot_info_from_system_data` | 1 when the scan found the system file (file 0); the summaries are then read from `system_data` arrays; 0 means the summaries come from the album image in `card_block` (emu: a blank card gives 0) |
| `system_data.selected_slot` (`save_selected_slot`) | the slot last loaded or written; written back to the system file after a load |
| `GameState.save_slot` | the slot an operation works on |
| `GameState.album_slot` | which bank of the loaded card image is valid (index into `card_block`) |
| `system_data.year/month/day[16]`, `family_name`, `given_name`, `nickname`, `club`, `metrics[9][16]`, `slot_girl_marks[16][16]` | per slot summary (written by `card_slot_summary_store`, main) |
| `system_data.girl_clear_count[16]` (old `flags`) | per girl, how often her ending was reached (ETC adds 1, cap 99); also stored in each album image (`AlbumSlotRecord.girl_clear_count`) |
| `file_select_info_page` | page switch counter, page = value mod 3 |
| `file_select_menu_mode_lock` | 1 while the screens run: `file_select_entry` writes `menu_mode = 0` every frame |
| `file_select_saved_*` | snapshot of the option groups that changed (section 3.9) |

### 3.3 Stages

`file_select_entry` runs every frame: it hides the 24 girl icon actors (128-151), updates the page indicator, adds 1 to `stage_timer`, switches on the stage, then updates the iris background, the typewriter text, the pointer, the button highlight, the slot overlay and the fade-in flash. In stage 2 a step value that no case handles restores the settings and starts mode 0x10.

| Stage | Name | Role |
| ---: | --- | --- |
| 0 | `FILE_SELECT_STAGE_INIT` | 3 steps: settings snapshot and sound bank 0; fade in, BGM 0, assets; menus and actors |
| 1 | `FILE_SELECT_STAGE_CARD_CHECK` | 8 steps: check the card; no card; play without a card; ask to insert a card |
| 2 | `FILE_SELECT_STAGE_LIST` | the slot list and every album and system file operation, 47 step values (3.5) |
| 3 | `FILE_SELECT_STAGE_LOADED` | after a successful load: girl greeting, fade, leave |
| 4 | `FILE_SELECT_STAGE_NO_SPACE` | a file could not be created |
| 0xC0 | `FILE_SELECT_STAGE_FORMAT` | the card is unformatted |
| 0xD0 | `FILE_SELECT_STAGE_SYSTEM_FILE` | rebuild or create the system file |
| 0xF0 | `FILE_SELECT_STAGE_ALBUM_BROKEN` | the album failed validation or loading |
| 0xFF | `FILE_SELECT_STAGE_CARD_ERROR` | read timeout or card error |

### 3.4 Stage 0 and stage 1

Stage 0: step 0 snapshots the settings (3.9), turns the menu mode lock on, clears the retained profile state, seeds `rand` with `system_data.seed`, sets text speed 1, builds the iris palette and texture and loads sound bank 0 (waits in substep 1). Step 1 raises the screen brightness by 4 per frame to 0x80, plays BGM 0, reads the memory-card assets (5 sectors) and the file select assets (0x89 sectors) and uploads an icon rectangle and a palette. Step 2 builds two menus (menu 0: 14 slot boxes 32 x 16 at x = column * 32 - 144, y = row * 16 - 18; menu 1: four buttons 40 x 20 at x = (i & 1) * 44 - 144, y = (i & 2) * 8 - 96), the 45 panel actors and 11 star actors, locks four texture pages, loads palettes and sets two background slots; then stage 1. The background iris starts at radius 1, centre (-0x50, -0x50), velocity (8, 8). Each frame after stage 0 step 0, while the radius is 0x4000 or less: radius = (radius * 16 + 0x70) / 15, the centre moves by the velocity, and a velocity component is re-rolled to a random value 0-15 (negative at the right and bottom edge) when the centre leaves x -0xA0..0xA0 or y -0x78..0x78; above 0x4000 it freezes. `iris_wipe_draw` draws it every frame. After stage 0 a gray flash counts down from 0xFF (`scene_work`).

Stage 1 (card check):

| Step | Name | Behaviour |
| ---: | --- | --- |
| 0 | check begin | `card_state_reset`, message that the card is being checked, typewriter |
| 1 | check poll | `card_check_poll` each frame: result 1 (formatted card) -> stage 2; 0xF1 (unformatted) -> stage 0xC0; 0xFF (no card) -> step 2; no answer after 0x201 frames -> step 2 |
| 2 | no card message | two lines |
| 3 | no card wait | START, cross or circle (mask 0x860) |
| 4 | play without card prompt | asks whether to start a new game; yes and no lines with button glyphs |
| 5 | answer | START or circle: restore the settings and `main_state_set(0x14)` (a game without a save); cross -> step 6 |
| 6 | insert card message | insert a card in slot 1 and press a button |
| 7 | insert card wait | circle: `card_state_reset`, back to step 0 |

### 3.5 Stage 2: the list and the operations

Step 0 (`file_select_scan`): `card_start`, then `card_scan_poll` until result 2 (a 0x201-frame limit returns to stage 1). The scan fills `slot_status` and sets `slot_info_from_system_data`. Then: with the system file -> step 1; else, when any of `slot_status[0..13]` is non-zero (the loop stops one short of slot 14, so an album in slot 14 alone is not counted) -> step 0x20; else -> step 0x40.

Branch A, system file present (steps 1-3):

- Step 1 `file_select_system_file_read`: reads the system file (`card_system_read_poll` result 3; failure goes to stage 1, or after 0x201 frames or result 0xF0 to stage 1 with a reset). `system_bank_load` is tried; -1 means the file is broken: a message, a wait for confirm or cancel, a question whether to rebuild it; circle goes to stage 0xD0, cross deletes file 0 (up to four tries) and returns to stage 1. On success the first valid slot becomes `selected_slot`, the header text and info page are shown (page 1) and the step becomes 2.
- Step 2 `file_select_list_input`: pointer or cursor on slot boxes (`menu_check(0)`; the SELECT mode toggle bit is removed from the pad word). A circle or START press on a valid slot (status 1): when the slot is already the selected one, sound 0x501, `save_slot` = slot and step 3; otherwise the slot becomes the selected one and the header and cursor update. Empty and broken slots do nothing. The page widgets and function buttons run in the same step.
- Step 3 `file_select_album_load`: substep 1 reads the album (`card_save_read_poll` 4); 2 `card_bank_validate`; 3 `game_bank_load` (GameState replaced, menu mode lock off), SE 0x501, `card_slot_summary_store`, `selected_slot = save_slot`; 4 writes the system file (`card_system_write_poll` 0x13) and goes to stage 3. A failed validation or load marks the slot 0xFF and goes to stage 0xF0; a timeout (0x201) or result 0xD0 goes to stage 0xFF; result 0xF0 resets the card and goes to stage 1.

Branch B, no system file but albums (0x20-0x24): 0x20 reads the first valid album, validates it and shows its record from the image; 0x21 is the list (first press selects, second press on the same slot -> 0x24); 0x22 message; 0x23 reads and validates the chosen album and returns to 0x21; 0x24 `game_bank_load` then stage 3 (failure: stage 1).

Branch C, empty card (0x40-0x4C):

| Step | Behaviour |
| ---: | --- |
| 0x40, 0x41 | "no data, create a new album and play?" circle -> 0x44; cross -> 0x42 |
| 0x42, 0x43 | message that the game starts without saving; any of START, circle or cross restores settings and `main_state_set(0x14)` |
| 0x44, 0x45 | "which album number?"; any slot box with circle or START sets `save_slot` -> 0x46 |
| 0x46, 0x47 | explanation of the system file (it enables saving sub-game data); any button |
| 0x48, 0x49 | "create the system file?" circle or START -> 0x4C; cross -> 0x4A |
| 0x4A | create the album file only (`card_file_create`; an existing or broken slot is reused), `game_data_init`, `settings_defaults_set`, write the album (result 0x14), restore settings, mode 0x14; a failed create goes to stage 4 |
| 0x4C | create the album and the system file (failure deletes the album and goes to stage 4), clear all 15 statuses, write the system file (0x13) then the album, mark the slot used, restore settings, mode 0x14 |

The four function buttons (menu 1) run in every list step that shows them: Copy -> 0xC0, New -> 0xA0, Clear -> 0xE0, Load -> step 0 (rescan). The first three also hide the 13 girl icon sprites. A circle press on the system-file button (x -0x8F..-0x51, y -0x27..-0x18) plays SE 0x50F and goes to stage 0xD0.

New (0xA0-0xA9), "start over from the prologue, keeping the clear counts":

| Step | Behaviour |
| ---: | --- |
| 0xA0, 0xA1 | explanation; any button |
| 0xA2, 0xA3 | "which album?"; a used slot (status non-zero) -> 0xA4; an empty slot -> 0xA8 (system file) or 0xA9 |
| 0xA4, 0xA5 | "start the album over?" cross -> step 0; circle -> 0xA6 (system file) or 0xA7 |
| 0xA6, 0xA7 | identical bodies: read, validate and `game_bank_load` the album, `file_select_profile_retain`, `game_data_init`, `settings_defaults_set`, restore settings, mode 0x14. The album file itself is not rewritten and the clear counts stay |
| 0xA8, 0xA9 | create the album file in the empty slot, `game_data_init`, `settings_defaults_set`, write the album (0x14); 0xA8 also writes the slot summary and the system file (0x13); restore settings, mode 0x14 |

`file_select_profile_retain` copies the loaded names and the player's and Shiori's birthday and blood type into the retained buffers (selection indices: month - 1, day - 1, (blood + 3) and 3) and sets `name_entry_retained_state` to 1, so the name entry starts filled in.

Copy (0xC0-0xC8): 0xC0 explanation; 0xC1 choose a valid source (SE 0x501); 0xC2 read, validate and load the source (re-locks the menu mode), then "copy to which album?"; 0xC3 choose the destination: a used slot goes to the overwrite question 0xC4/0xC5 (cross returns to step 0 substep 2, circle goes on), an empty slot is created first (`card_file_create`, SE 0x501; failure goes to stage 4); 0xC6 routes to 0xC7 (system file: write the destination album, then the system file) or 0xC8 (write the destination only); both end at step 0 substep 2.

Clear (0xE0-0xE6): 0xE0 explanation; 0xE1 choose an album with status non-zero (SE 0x501); 0xE2/0xE3 "delete for sure?" cross returns to step 0; circle goes to 0xE4 (system file: `card_file_delete`, mark empty, rewrite the system file, then stage 1) or 0xE5 (delete only, step 0); 0xE6 returns to step 0.

### 3.6 Information panel

Header (every page): the slot number, the family name, the given name, the nickname and the club name (`club_name_table`) of the selected slot, taken from `system_data` when the system file is present, else from the album image (`AlbumSlotRecord`, club in the high nibble of a halfword). Next to it the save date (year, month, day as numbers, `file_select_number_draw`) and the clear count = the number of non-zero `girl_clear_count[0..12]`.

Pages (page = `file_select_info_page` mod 3, negative values wrap; starts at 1; triangle, or circle on the region x -0x3F..0x1F, y 0xD..0x1B adds 1; pressing a function button sets it back to 1):

| Page | Content |
| ---: | --- |
| 0 | the 13 clear counters as numbers with girl icons, laid out in three columns (girls 0-3 at x -4, girls 4-7 at x 0x3C, girl 12 then girls 8-11 at x 0x88); a counter of 0 shows the lock marker |
| 1 | the nine parameter values of the slot (`metrics[i].value`) as numbers, four at x 16 (y 30, 46, 62, 78: condition, liberal arts, science, art) and five at x 128 (y 14..78: athletics, trivia, looks, guts, stress), the `Parameter` order of core (emu: a fresh game shows 100, 40, 40, 40 and 40, 32, 60, 5, 0) |
| 2 | for heroines 0-10: a star icon (frame 0x58 + (value and 7)) when the girl's met flag is set and her saved byte is non-zero. The byte is `girls[15].event_counters[g]`, which the telephone evaluation screen (TEL) sets to stars + 1 for every met girl (stars from her rank value: below 6 gives 6, below 11 gives 5, below 21 gives 4, below 41 gives 3, below 61 gives 2, else 1; 0 when her points are 500 or more) |

The 13 girl icon sprites of page 0 are shown by `file_select_girl_icons_visibility_update`, which tests the album record at `card_block[D_800E8BEF]` (a main byte that nothing writes, 0 in the emulator) instead of the loaded slot, so the sprites are always decided by record 0. The highlighted slot box is marked with unk_43 = 0x11 on the actors 100-113; the empty slots are tinted gray by `file_select_slot_status_overlay_draw` and the broken ones (0xFF) with a second tint.

### 3.7 Stages 3, 4, 0xC0, 0xD0, 0xF0, 0xFF

- Stage 3 step 0 `file_select_loaded_greeting`: shows the loaded scene background and plays a girl's greeting: the girl is the one of the latest event log entry (`event_log_scan`); none for ids 13 and up; girl 11 uses clip 1 when event flag 0x4D is set; girl 12 (Rei): when her clear count or this save's cleared flag is set, clip 0 if the byte at +0xF of her record is 0x29 or more, else clip 1; when she was never cleared, clip 1 if that byte is 0x29 or more, else no voice; other girls use clip 1 when `get_g_zyotai_s` rank is 0, else clip 0 (rows of `girl_voice_clips_by_state`). The brightness is lowered by 2 per frame. The BGM fade commands 5 and 0xC1 are sent when the clip ends, except for saves dated 1995-04-04 and 1998-03-01. Step 1 waits 0x41 frames, restores the settings and chooses: a save dated 1998-03-01 or 1995-04-04 (the last day and the first day) runs `file_select_profile_retain`, `game_data_init`, `event_state_init`, clears the event log count and enters mode 0x14 (a new run with the old names as defaults); any other save loads the day BGM and enters mode 0x34 stage 0xFF.
- Stage 4: one message, then after a button: if any slot is valid go back to the list (step 1 with a system file, else 0x20), else a hint to use another card and back to stage 1 step 6.
- Stage 0xC0: message that the card is not initialised; "format?" yes: `card_format` (success -> stage 2, failure -> stage 1); no: ask for another card (circle -> stage 1).
- Stage 0xD0 (system file): step 0 routes to 1 (rebuild, when the system file exists) or 2 (create). Both ask for confirmation (circle yes, cross back to stage 1), warn that it takes about a minute, wait for a button, then scan slots 1-14: every slot with non-zero status is read, validated and `game_bank_load`ed, its summary stored, and for each girl i with `girl_cleared[i] != 0` a counter is incremented. Rebuild merges: `new = counter`, `old = previous girl_clear_count`; the result is `max(old, new)`, but forced to 1 when `new != 0` and `new != old`; create sets the counts to `new`. Then the system file is written (result 0x13), SE 0x50F, stage 2 step 0. The menu mode is forced to 0 during the stage.
- Stage 0xF0 / 0xFF: play one voice clip pair from a per-slot table of 14 pairs (0xF0: `file_select_broken_album_clips`, 0xFF: `file_select_card_error_clips`; speaker `slot % 13`; HYPOTHESIS: the lines tell that the album data is broken or unreadable), wait for it to start and end, then return to stage 2 step 2 (0xF0, system file present) or reset the card and go to stage 1.

### 3.8 Voices and sounds

`xa_clip_request_set` followed by `SND_CMD_VOICE_PLAY` (0x300) is the voice call. SE ids used: 0x501 (confirm), 0x50F (system file written or button). Sound commands 5 (fade via master), 0xB1/0xC1 (BGM fades) and 3 (forget voice) are sent when leaving.

### 3.9 Settings preservation

`options_dirty` (OPTION marks changed groups) is copied with the values at the start of the screens and then cleared: bit 0x80 text speed, 0x40 click mode, 0x20 double-click interval, 0x10 confirm button, 8 cursor speed (the icon, menu and voice modes are copied too). While `file_select_menu_mode_lock` is 1 the menu mode is forced to 0. A load (`game_bank_load`) overwrites the settings with the saved ones; when the screens end `file_select_settings_restore` re-applies the changed groups (text speed through `k_speed_set`, the others by assignment), rebuilds the icon bar (icon count 0xF in free icon mode, else 0xE) and validates the icon layout (free mode: icon positions outside x -0x86..0x7C or y -0x54..0x54, or two or more at (0, 0), reset to mode 0). The restores of icon mode, menu mode and voice mode only run when the saved value is 3 or more, 4 or more and 3 or more; valid values (0-2, 0-3, 0-2) never are, so those three never take effect (as implemented).

## 4. Mode 0x14: name entry (`name_entry_*`)

### 4.1 Flow

| Stage | Step | Function | What |
| ---: | ---: | --- | --- |
| 0 | 0 | `name_entry_load_init` | resets the screen; reads the background still (NAME_SAK.BS, 10 sectors) and decodes it to VRAM (0x140, 0x100); reads the UI pack (NAME_ENT.NBN, 54 sectors) and the kanji lists (NAME_KAN.NBN, 6 sectors) |
| 0 | 1 | `name_entry_load_fade_in` | relocates the 50 list pointers, builds the UI, brightness +4 per frame; at 0x80 captures the screen, builds the actors and enters stage 1 |
| 1 | 0 | `name_entry_step_init` | prefill from the retained profile (state 1) or blank everything; family-name prompt voice |
| 1 | 1 | `name_entry_family_name_input` | family name (3 characters) |
| 1 | 2, 3 | given name init and input | given name (3 characters) |
| 1 | 4, 5 | nickname init and input | nickname (5 characters) |
| 1 | 6, 7 | birthday init and input | month, day, blood type |
| 1 | 8, 9 | confirm | "is this right?" with the entered data; yes -> stage 2; no or cross -> step 0 |
| 2 | 0, 1 | Shiori birthday init and input | the heroine's birthday and blood type |
| 2 | 2, 3 | confirm | heroine data confirm; yes -> step 4; no or cross -> step 0 |
| 2 | 4 | `name_entry_commit` | builds the new game (4.7) |
| 2 | 5 | `name_entry_exit_to_prologue` | waits 0x41 frames, fades 4 per frame to 0, `main_state_set(0x15)`, text advance 0xE |

START at any input step finishes the current input as if confirmed (names: filled with a random preset when empty; birthday: random for unset values). Circle on the grid puts a character, circle on a tab switches page; cross moves the cursor back one slot in the name steps, back one field in the birthday step, and returns to step 0 in the confirm steps. The prompts and the confirm each start one voice clip (the clip pair tables in section 7).

### 4.2 Name buffers

Three buffers: family name `name_entry_family_name` (8 bytes, three 2-byte characters), given name (8 bytes, three characters), nickname (12 bytes, five characters). `name_entry_cursor` indexes the 11 slots (0-2 family, 3-5 given, 6-10 nickname); 0xC (`NAME_ENTRY_CURSOR_NONE`) means no active slot. A character is a 2-byte code; the blank is the full-width space (`fullwidth_space_text`, main). A circle press on a slot box while the matching name is active moves the cursor there. After the last character the cursor jumps to 0xC (family at 3, given at 6, nickname at 11).

### 4.3 Character grid and tabs

The grid is 6 rows of 16 x 16 cells with its top-left at (-0x78, -0x30): 17 columns in the two kana tabs, 16 in the symbol and kanji tabs (pointer test: x -0x77..0x97, y -0x2F..0x2F, cell = ((x + 0x78) / 16, (y + 0x30) / 16), column 16 rejected in tabs 2 and 3). Five tab buttons at x -0x9D..-0x80 and y -0x30, -0x10, 0x10, 0x30, 0x50 (16 high):

| Tab | Page | Content |
| ---: | --- | --- |
| 0 | hiragana | 17 row strings; the grid has 17 columns in three groups (0-4, 6-10, 12-16) with gap columns 5 and 11; string index = row * 3 + column / 6, character = column % 6; cells without a kana (the gap columns, the two empty cells of the three-kana rows, the last group of the last row) store the blank |
| 1 | katakana | the same layout with the katakana table |
| 2 | symbols | candidate list 38 (453 symbols), 16 per line, scrollable |
| 3 | kanji by reading | choose one of 45 readings in a 3 x 3 grid of kana-row labels at y 0x38-0x58 (reading = row * 15 + column / 6 * 5 + column % 6); the grid then shows, on row 0, the kana variants of the reading (hiragana, katakana and voiced forms) and below the candidate characters of that reading (16 per line, scrollable); reading 38 is the blank one and shows the symbol list |
| 4 | done | finishes the current name (random preset when it is empty, all blank, or its blank-byte count points at the terminator) and goes to the next step |

Scrolling (tabs 2 and 3): up and down arrow buttons at x 0x89..0x97 (y -0x2F..-0x21 up, 0x11..0x1F down) change `name_entry_page_scroll` by one line per circle press (not below 0; the page draw clamps it so that the list end stays visible). Characters outside the list length store the blank. The candidate file (NAME_KAN.NBN, 12288 bytes) holds `u16 counts[48]` at the start (45 used; reading 38 has 453 entries), the 2-byte character lists from +0x60 (each list ends with a short terminator) and a `u32 offsets[50]` table at +0x2B2C that `name_entry_load_fade_in` makes absolute by adding the load address 0x801E0000 (the last five offsets are 0).

### 4.4 Presets and chords

Fifteen preset triples (family 8 bytes, given 8 bytes, nickname 12 bytes; staff-style full names and nicknames; HYPOTHESIS: they are the developers' names) are copied by `name_entry_preset_apply(n % 15)`. They are used (a) as the random fill when a name is finished empty (`rand() % 15`) and (b) by the chord check: while SELECT is held, a new press of exactly the shoulder buttons below selects a preset:

| Pressed (L2 1, R2 2, L1 4, R1 8) | Preset |
| --- | ---: |
| L2 | 1 |
| R1 | 2 |
| R2 + R1 | 3 |
| L2 + L1 | 4 |
| L1 + R1 | 5 |
| L2 + R2 | 6 |
| L2 + R1 | 7 |
| R2 + L1 | 8 |
| R2 | 9 |
| L1 | 10 |
| L2 + L1 + R1 | 11 |
| L2 + R2 + L1 | 12 |
| R2 + L1 + R1 | 13 |
| L2 + R2 + R1 | 14 |
| all four | preset 0 and the birthday selections: the player's month index 11 (December), day index 9 (10), blood index 0 (A); Shiori's month index 1 (February), day index 13 (14), blood index 3 (O) |

### 4.5 Birthday screens

Both screens (player stage 1, Shiori stage 2) use the same three fields, `NAME_ENTRY_FIELD_MONTH`, `_DAY`, `_BLOOD`, in order. The selections are indices: month 0-11 (grid of 6 x 2 boxes, 32 x 16 at x = (i % 6) * 40 - 104, y = (i / 6) * 16 - 40), day 0-30 (11 per row, 24 px apart from x -104, rows at y 8, 24, 40), blood 0-3 in the order A, B, AB, O (four boxes 64 px apart from x -96, y 72-88). The pointer sets the selection while it is over a box; circle moves to the next field, cross to the previous one (at field 0 nothing), START finishes. 0xFF means not chosen (shown blank). Finalising (`name_entry_birthday_finalize`) replaces every unset selection by `rand() % 12`, `% 31` and `% 3` (so a random blood type is A, B or AB, never O) and clamps the day: February above the 28th becomes 28, April, June, September and November above the 30th become 30.

The numbers displayed are the selection + 1; the blood letter is A, B, AB or O. Stored values: `(blood selection + 1) and 3`, so O = 0, A = 1, B = 2, AB = 3 (`BloodType`). The heroine screen's prompt asks the player for Shiori's birthday and blood type; her confirm screen shows her fixed name.

### 4.6 What is stored

`name_entry_profile_store` writes into GameState: `player_family_name` (8-byte buffer), `player_given_name`, `player_nickname` (16 bytes), each cleared first and filled with the characters of the matching entry buffer without blanks and zero pairs (blanks inside a name are removed, so characters move left); `player_full_name` = family + given (16 bytes); `player_birthday` (`GsDateBits`: month = index + 1, day = index + 1, blood = (index + 1) and 3); `girls[0].birthday` from Shiori's selections the same way. The heroine's real birthday is therefore whatever the player enters; the in-game reference value is 14 February, blood O (the chord preset).

### 4.7 The new game (`name_entry_commit` -> `name_entry_new_game_setup`)

Order: sound commands 5, 0xC1 and 3; `game_data_init` (resets calendar, player, girls, events to the start values); `name_entry_profile_store`; then:

1. Shiori's club: `girls[0].club = (Shiori day * 3 + Shiori month) % 11` (day and month are the values 1-31 and 1-12; the result is a club 0-10, never "none").
2. Shiori's initial scores from the blood compatibility grade: `grade = name_entry_blood_compat_level[Shiori blood + player blood * 4]` (1-4) with the stored blood values; `idx = grade - 1`; affection (`girls[0].affection`) = 45, 50, 50, 55; tokimeki = 10, 20, 25, 25; syoushin = 10, 0, 0, 0 for idx 0-3. Table rows (player blood O, A, B, AB), columns (Shiori O, A, B, AB): `[1 4 3 2; 3 1 2 2; 2 3 3 4; 3 2 3 1]`. emu: player AB, Shiori born 26 January AB gave club 2 = (26 * 3 + 1) % 11 and 45, 10, 10.
3. Clubs of the other heroines: Mio is in club 1 when Shiori's club is 0 and in club 0 when Shiori's is 1, otherwise club 0 or 1 by `rand() & 1`; Yuina likewise between 2 and 3 (against Shiori's 2 and 3); Ayako between 4 and 5; Saki between 6 and 7; Yukari 8; Nozomi 9; Mira, Yuko, Megumi and Miharu 11 (no club); Yumi 10; Rei's club is not set here.
4. Fixed blood types (stored values) of the heroines 1-12: Mio 1, Yuina 1, Ayako 2, Saki 1, Yukari 2, Nozomi 1, Mira 0, Yuko 2, Megumi 1, Yumi 0, Miharu 1, Rei 3.
5. `name_entry_cheat_names_apply` (4.8).

Shiori (girl 0) birthday and blood and the player's blood therefore decide the start value of her affection, tokimeki and syoushin and her club, and her club decides the second member of the paired clubs.

### 4.8 Cheat names

The check runs after the setup, on the final names:

- Nickname equal to a fixed five-character string that spells the publisher's name with a "-man" suffix (HYPOTHESIS: a developer easter egg; the number 573 reads as the publisher's name in Japanese number wordplay): all nine parameter values become 0x23D (573).
- Family name and given name equal to the friendly pair ("everyone", "friends"): Shiori's tokimeki becomes 0x20 and affection 0x40; every heroine 1-8 whose club differs from Shiori's gets tokimeki 0x7F, affection 0x7F and the met flag and flag bit 3 set.
- The Konami code spelled with the kanji words for up, down, left, right in the three names (up up / down down / left right left right and a cross): all nine parameters become 0.
- The same code spelled with arrow glyphs: all nine parameters become 999 (0x3E7).

(HYPOTHESIS: the meaning of flag bit 3 on the heroine records is not known from this unit.)

## 5. Mode 0x15: the prologue (`prologue_*`)

### 5.1 The line player

43 text lines are shown one after another. Data: `prologue_line_texts[43]` (up to four short lines of text each), `prologue_line_speakers[44]`, `prologue_line_clips[43]` and `prologue_line_clips_second[43]` (voice clip pairs; both words 0 = no voice; the two tables differ only for line 19, which has two clips). `prologue_line` is the index (0xFF before the first).

`prologue_line_start`: cuts the text list back to the mark; speaker plate: low nibble 0xF is the player (the entered family name), 0x1E and 0x2E are two fixed labels for unnamed schoolgirls, otherwise the girl's short name from `girl_short_name_table[code]`; queues the text with `sndisp(text, 0, code)`; for a voiced speaker requests the clip (`xa_clip_request_set(start, end, code)` and `SND_CMD_VOICE_PLAY`); sets `current_girl` = code; clears `prologue_voice_started`; starts the typewriter.

`prologue_line_run` (each frame): advances the typewriter and the two-line text scroll (more than three display lines scroll up two pixels per frame); `prologue_voice_started` becomes 1 when the CD reports a clip playing (`cd_state_get() & 4`). A voiced line waits until its clip has started and ended (`cd_state_get() & 2`); at that moment, if the second clip table has a different clip for the line it is started (once), else when the typewriter has finished the step timer counts; an unvoiced line counts as soon as the typewriter has finished. A frame in which neither holds resets the counter. At 0x3D (61) the substep advances. Text speed is 5 (set in the first stage) and 3 at the end.

Speakers by line: 0-4 player, 5 Yoshio, 6 player, 7 Yoshio, 8 player, 9 schoolgirl A, 10 schoolgirl B, 11 Yoshio, 12-14 Rei, 15 Yoshio, 16 player, 17 player (thought), 18-19 Shiori, 20 Yoshio, 21 thought, 22 Yoshio, 23 player, 24-26 Yoshio, 27 player, 28 Yoshio, 29 player, 30 Yoshio, 31 player, 32 Yoshio, 33 player, 34 Yoshio, 35 player, 36 Yoshio, 37 player, 38 Yoshio, 39 player, 40 Yoshio, 41-42 player. Only the heroine and her friends are voiced (Yoshio, Rei, Shiori, the two girls).

Lip sync (HYPOTHESIS: the selected frames are mouth shapes): eight helper functions `prologue_lip_sync_*` average four frames of the CD level meter (`getCDlevel`, kept in `prologue_cd_level_history`), divide by 10 (max 3) and select a frame of an actor while the voice plays; short random animations start with `rand() & 0xFF == 0xFF` (HYPOTHESIS: eye blinks).

Two answers are synthesised from the entered data: the lines after line 24 (the friend asks Shiori's birthday) and line 25 (her blood type) show the player's answer, built from `girls[0].birthday`: number of the month, the month word, day number (tens digit only from 10), the day word; and the blood letter from `prologue_blood_texts` plus the type word, spoken under the player's name (`current_girl` = 0xF).

### 5.2 Stages

Every stage has one step except stage 0 (3 steps) and stage 1 (2 steps). Resources are disc files read with `cd_read_sectors` (sprite packs O_xx and CUT_xx under PROLODIR) or `dec_bg_cd_read` (background stills P_xx under BS and PROLODIR); the LBAs are in section 8.

| Stage | Function | Lines | Scene and loads |
| ---: | --- | --- | --- |
| 0 | `prologue_stage_arrival` | 0 | step 0 loads sound bank 0; step 1 title window, calendar panel, BGM 0, still P_01_01, pack O_01 (fade in 1 per frame); step 2 plays line 0 and holds 0x3D frames |
| 1 | `prologue_stage_board` | 1 | step 0 reads P_01_02 and sets the falling particles; step 1 scrolls the picture (counter +1 per two frames to 0x140), line 1, then reads P_01_03 |
| 2 | `prologue_stage_names` | 2 | line 2 over P_01_03; crossfade to the darker P_01_03B and back; then loads CUT_12, O_12 and P_01_04B |
| 3 | `prologue_stage_childhood_friend` | 3 | line 3, fade in; loads CUT_05, P_01_05 |
| 4 | `prologue_stage_friend_gap` | 4 | line 4; loads O_05, CUT_12, P_01_04B; random blink |
| 5 | `prologue_stage_classroom_transition` | none | a portrait slides out and in (SE 0x605), P_01_07 then P_01_04 (classroom), packs CUT_07, O_07 |
| 6 | `prologue_stage_yoshio_greeting` | 5-10 | Yoshio meets the player; SE 0x503 before line 9; loads P_01_06, CUT_08, O_08 |
| 7 | `prologue_stage_girls_notice` | 11 | the girls react; BGM change (commands 5, 0xB1, bank 1); loads P_01_07B, CUT_09, O_09 |
| 8 | `prologue_stage_rei_banner` | 12 | white flash up and down, Rei's banner fades in, line 12; loads CUT_10, O_10 |
| 9 | `prologue_stage_rei_speech` | 13-14 | Rei's speech with a flash and sparkles; loads CUT_08, O_11 |
| 10 | `prologue_stage_yoshio_reacts` | 15-16 | portraits drift left 0.5 px per frame; BGM commands 5, 0xC1 and bank 1; loads CUT_12, O_12, P_01_07 |
| 11 | `prologue_stage_shiori_next` | 17 | BGM 1, SE 0x604; loads P_01_08, O_13 |
| 12 | `prologue_stage_shiori_load` | none | loads CUT_13, O_14, P_01_09 |
| 13 | `prologue_stage_shiori_name` | 18 | Shiori speaks (voice); loads O_15 |
| 14 | `prologue_stage_shiori_hobby` | 19 | two clips; loads P_01_06, CUT_14, O_16 |
| 15 | `prologue_stage_yoshio_quiz` | 20-31 plus two answers | 35 substeps; the scene scrolls left 2 px per frame at the end; loads P_01_04, CUT_07, O_07 |
| 16 | `prologue_stage_yoshio_offer` | 32-36 | BGM 0 variant 1 at line 34; decodes P_01_10 |
| 17 | `prologue_stage_window_tree` | 37 | decodes P_01_12, reads O_13 |
| 18 | `prologue_stage_legend` | 38-41 | the tree legend; waits for the voice of line 38 |
| 19 | `prologue_stage_flash` | none | two white flashes with a portrait switch |
| 20 | `prologue_stage_closing` | 42 | hold 0x3D frames, commands 5 and 0xD1 (BGM fade) |
| 21 | `prologue_stage_finish` | none | resets CD queue and sound state, loads sound bank 0, text speed 3, `menu_mode` = 1, `main_state_set(0x34)` and `main_stage_set(0xFF)` |

START (0x800) at any time after the first step jumps to stage 21. The dispatcher also draws the title window ("prologue" banner), the screen frame (type 0 at stage 0, type 2 later) and the message window, and the calendar panel at stage 0 shows 1995-04-04, Tuesday (emu: frame 750).

## 6. Mode 0x11: the attract opening (`opening_*`)

Earlier notes call the stage 0 scene (old `func_80143580`) the "attract prologue"; the code shows it is a narrated picture story with its own voice clips, not the prologue lines of mode 0x15 (`GLOSSARY.md` still says the prologue plays in the attract run too).

Stage 0 (story): `opening_story_stage` adds 1 to `step_timer`; START, circle or cross (0x860) jumps to step 2. Step 0 (`opening_story_load`) stops the sound, loads sound bank 1, plays BGM 1 and reads seven pictures from FUKUTEST (O_31_05L, 07, 05R, 02, 04, 01, 03 in that order) into the two background slots with their palettes, then starts the first narration clip and four hidden text lines. Step 1 (`opening_story_scene`, 20 substeps) fades and pans the background slots (a slot's x decreases by 1 every fourth frame; the pictures are wider than the screen), starts a second narration clip as soon as the first has ended (`opening_second_narration_started`) and a third text block when slot 0 reaches x -0x118, builds three picture sprites that pan the same way, flashes to gray (sound 0xD1 fades the BGM), reads O_31_06 and the 0xAE-sector OPEN.NBN pack, converts the palettes to sepia (`col2sepia`), animates one character actor, plays a final voiced line, shows it for 0x41 frames and ends with a 255-step gray flash. While the triangle button is held the narration text is shown over a dark box; otherwise it stays hidden. The mouth actor follows the CD level; holding any button of pad 2 simulates a voice level (`stage_timer & 0x3F`) and also (at substep 16) plays an extra short voiced line and text; a pad 1 button press ends substep 15 early. Step 2 (`opening_story_end`) resets the CD queue, VRAM, sprites and actors, stops BGM and voice and goes to stage 1.

Stage 1 (`opening_movie`): sets the HUD level to 0, text speed 3, queues the 21 lyric lines, `set_movie_offset(0x18, 0x10)`, `movie_play(0, 0x33B, 0x820)` (the STR movie from sector 0x820 to frame 0x33B), sends the CD commands set mode 0x80, pause and init, re-initialises the MDEC buffers, clears VRAM, restores the HUD level, text speed and the pad code state (`pad_code_index`, `pad_code_entered`, `pad_code_timer`) and returns to the title (`main_state_set(0x12)`). When the stage is entered directly (pad code from the title) the story is skipped.

## 7. Data tables and the main data they use

| Name | Address | Content |
| --- | --- | --- |
| `name_entry_*_prompt_clip_start/end` (6 pairs) | 0x8014CC38-0x8014CC64 | voice clips for the family, given, nickname, birthday, Shiori and confirm prompts; (0x8014CC30, 0x8014CC34) and a pair at 0x8014CC68 are unused |
| `name_entry_hiragana_rows`, `name_entry_katakana_rows` | 0x8014CD28, 0x8014CD6C | 17 row pointers each |
| `name_entry_reading_labels` | 0x8014CDB0 | 45 readings (kana variants), the 38th blank |
| kana row and reading label strings | 0x8014B150-0x8014B4AC | the 17 hiragana rows, 17 katakana rows and 45 reading labels the three pointer tables point to; rodata strings with no name of their own (read only through the tables) |
| `name_entry_candidate_counts`, `_lists` | 0x8014CE64, 0x8014CE68 | pointers into the NAME_KAN.NBN image at 0x801E0000 |
| `name_entry_preset_family_names/given_names/nicknames` | 0x8014CE6C, 0x8014CEE4, 0x8014CF5C | 15 x 8, 15 x 8, 15 x 12 bytes |
| `name_entry_small_digit_uv`, `name_entry_wide_digit_uv`, `file_select_digit_uv` | 0x8014D014, 0x8014D028, 0x8014D0F4 | (u, v) of the digit sprites |
| `name_entry_blood_compat_level`, `name_entry_initial_stats` | 0x8014D03C, 0x8014D04C | 4 x 4 grade table; four (affection, tokimeki, syoushin) triples |
| `prologue_line_texts`, `_speakers`, `_clips`, `_clips_second` | 0x8014D958, 0x8014DA04, 0x8014DA30, 0x8014DB88 | the 43-line script |
| `prologue_blood_texts` | 0x8014D1A0 | four letters, indexed by the stored blood value |
| `file_select_broken_album_clips`, `file_select_card_error_clips` (main data) | 0x800B3A3C, 0x800B394C | 14 clip pairs each, indexed by slot |
| `fullwidth_space_text` (main data) | 0x800B3644 | the blank character |

Main data and functions the overlay uses and that belong to other specs: the card layer and `SystemData` ([[spec/m-flow]]), text and menu system, speaker plates (`gnsx`, `sndisp`), `girl_voice_clips_by_state`, `girl_short_name_table`, `club_name_table`, `number_strings` ([[spec/m-menutext]], [[spec/m-sysmenu]]), the actor pool, background slots, `iris_wipe_draw`, `screen_flash_gray` ([[spec/h-graphics]]), CD and MDEC ([[spec/h-cdio]]), sound ([[spec/m-dates-a]]).

Emulator evidence (tools/emu, states under build/emu/states, not committed): the title menu, circle on item 0 gives mode 0x13 at frame 173 and the card check; a blank card gives the no-data prompt (stage 2 step 0x41); creating an album and the system file reaches mode 0x14 stage 1 step 1 (name input); with START on every step the setup ends at `name_entry_cheat_names_apply` with the player AB, born 11 September, Shiori born 26 January AB: her club field was 2 = (26 * 3 + 1) % 11, affection 45, tokimeki 10, syoushin 10, birthday word 0x07A1 (month 1, day 26, blood 3); SELECT with L1 held at the family-name step loads preset 10 into the three name buffers; the prologue starts at frame 711 of that run with the calendar panel at 1995-04-04 (Tuesday) and runs for more than 10,000 frames; START ends it in mode 0x34.

## 8. Disc files read by the overlay

| LBA | File (under CDROM) | By |
| --- | --- | --- |
| 0x9CB5 (0x89 sectors) | FUKUTEST/FILESEL.NBN | file select assets |
| 0x9DBA (5) | FUKUTEST/MEMCARD.NBN | memory-card icons |
| 0x9DBF (6) | FUKUTEST/NAME_KAN.NBN | kanji candidate lists |
| 0xA9E2 (54) | NAME_ENT/NAME_ENT.NBN | name entry UI pack |
| 0xAA18 (10) | NAME_ENT/NAME_SAK.BS | name entry background still |
| 0x9DF6 (0xAE), 0x9EE1, 0x9F22, 0x9F63, 0x9FA4, 0x9FD5, 0x9FF6, 0xA017, 0xA048 | FUKUTEST/OPEN.NBN, O_31_01, O_31_02, O_31_03, O_31_04, O_31_05L, O_31_05R, O_31_06, O_31_07 | opening story |
| 0x494B, 0x4954, 0x495D, 0x4966, 0x496A, 0x4973 | BS/P_01_01, P_01_02, P_01_03, P_01_03B, P_01_10, P_01_12 | prologue stills |
| 0xACD4, 0xACDD, 0xACE6, 0xACEC, 0xACF6, 0xAD00, 0xAD05, 0xAD0B | PROLODIR/P_01_04, P_01_04B, P_01_05, P_01_06, P_01_07, P_01_07B, P_01_08, P_01_09 | prologue stills |
| 0xAA23, 0xAA54, 0xAA85, 0xAAB6, 0xAAE7, 0xAB18, 0xAB49, 0xAB7A | PROLODIR/CUT_05, CUT_07, CUT_08, CUT_09, CUT_10, CUT_12, CUT_13, CUT_14 (last two inferred from the 0x31-sector spacing) | prologue picture packs |
| 0xABAB, 0xABB8, 0xABCA, 0xABE3, 0xABFC, 0xAC05, 0xAC1E, 0xAC3F, 0xAC50, 0xAC91, 0xAC9A, 0xACBB | PROLODIR/O_01, O_05, O_07, O_08, O_09, O_10, O_11, O_12, O_13, O_14, O_15, O_16 | prologue sprite packs |

Sound banks are loaded with `sound_bank_load_start`; the LBA words in the calls are in the source.

## 9. How the unit connects to the rest of the game

- The title (`mode_title`) leaves to mode 0x13 for item 0 and to mode 0x11 on idle; the pad code enters 0x11 stage 1. ETC returns to 0x12 or 0x11 and OPTION to 0x12 (see [[spec/m-flow]]).
- The in-game data screen (`8005A0B0.c`) loads a save and enters mode 0x14 directly when the saved date is 1995-04-04 or 1998-03-01; there is no retained profile on that path (the overlay is loaded fresh), so the entry starts blank.
- The new game replaces calendar, player, girls and events (`game_data_init`, [[spec/m-flow]]); the profile fields it sets are `player_*` names, `player_birthday`, `girls[0].birthday`, club ids and blood types of the girls, and the start scores of Shiori.
- The prologue ends in the weekday command screen (mode 0x34, stage 0xFF) with menu mode 1 (menu A) and text speed 3, which starts the first week ([[spec/m-scheduler]]).
- `system_data` summary arrays and `girl_clear_count` feed the file screens; ETC writes the clear counts at the ending.

## 10. Hardware layer (module level)

Used only coarsely: sprite and primitive drawing (gouraud boxes, digit sprites, the iris wipe), background slots from decoded stills (`dec_bg_*`, MDEC), the actor pool, CD reads and XA clips, the STR movie, memory card calls (`card_*`), sound banks and BGM/SE commands, and the main text system (kanji strings and typewriter). A port replaces these with its own renderer, audio and save storage; the file screens' card state machine then reduces to the rules in 3.5 on a save directory.

## 11. Open questions

1. Prefixes: the ADR lists only `name_entry_` and `prologue_` for this overlay; the proposal adds `file_select_` and `opening_`. The ADR row (and the audit's `name_ent_`) should be updated to match, or the 104 `file_select_*` names and 10 `opening_*` names renamed.
2. The core table names the modes 0x11, 0x13, 0x14, 0x15 differently (section 1); the constants in `o-nameent.tsv` override them. The names `MODE_PROLOGUE` and `MODE_NAME_ENTRY` change value (0x11 to 0x15, 0x13 to 0x14), so the four rows must be applied together.
3. Who owns `Work800E7D10.flags` and `NameRecordF00.flags`? They are renamed `girl_clear_count` here; ETC, OPTION and EN_NICHI use the same field.
4. The main-side tables at 0x800B3A3C, 0x800B394C and the blank-character string at 0x800B3644 are named in this table with scope `main`; the main owners may take them over.
5. `D_800B3DB0` (written 1 and 0 by the prologue, read by the tile upload to add `move_255_line`) has no name; it belongs to the h-graphics unit.
6. What the three "retained" restores of icon, menu and voice mode were meant to do (section 3.9) is unknown.
7. Meaning of the heroine flag bit 3 set by the friendly-pair cheat, and of the `girls[12]` score byte at +0xF (threshold 0x29) in the greeting, is not settled.
8. Which clips the two per-slot clip tables play was not listened to; the names carry HYPOTHESIS.
9. `D_800E8BEF` (the album record index read by the page 0 icon test) is probably a leftover of `GameState.album_slot`; it is named `file_select_icon_record_index` with scope `main`.
