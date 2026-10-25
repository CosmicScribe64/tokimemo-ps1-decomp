---
type: spec
updated: 2026-10-24
sources: ["src/ovl/NAME_ENT", "config/renames/proposals/o-nameent.tsv", "include/ovl/NAME_ENT.h"]
---

# Overlay NAME_ENT: file screens, name entry, prologue and attract opening (port spec)

Phase 2 spec for [[tickets/T-27030-p2-o-nameent]] ([[tickets/T-27000-phase-2]]), reconciled under [[tickets/T-27051-parallel-overlay-reconciliation]]. The original mixed proposal `config/renames/proposals/o-nameent.tsv` and executable packet `config/renames/proposals/o-nameent-apply.tsv` are historical preparation inputs, not current application status. Current names follow applied ledgers and source; unapplied candidate rows remain proposals. Use current source and committed ledgers for shared identifiers. Earlier notes marked "emu" are historical reports, not newly reproduced runtime evidence in this reconciliation. No game text is quoted: strings are described by role.

## 1. What the overlay is

NAME_ENT.EXN (disc LBA 0x95EA, slot 0x80132000; the boot code also reads it once before the title) holds four main modes that belong to the game start:

| Mode | Entry (old address, new name) | Role | Entered from | Leaves to |
| --- | --- | --- | --- | --- |
| 0x11 `GAME_MODE_OPENING` | 0x80144AD0 `opening_mode_handler` | attract opening: a narrated picture story (stage 0), then the opening movie with the theme lyrics (stage 1) | title idle timeout (emu: frame 3534 of a boot without input), title pad code (stage 1 directly), pad 2 soft reset | title (0x12) |
| 0x13 `GAME_MODE_FILE_SELECT` | 0x8014231C `file_select_mode_handler` | the file screens behind the title item "game start": memory-card check, system file, 14 album slots, create, New, Copy, Clear, load | title menu item 0 | 0x14 (new game, replay, first-day or last-day save), 0x34 stage 0xFF (a normal save), 0x10 (internal error) |
| 0x14 `GAME_MODE_NAME_ENTRY` | 0x8013209C `name_entry_mode_handler` | the player's name, nickname, birthday and blood type, then the heroine Shiori's birthday and blood type; builds the new game | mode 0x13 (11 sites); the in-game data screen after loading a first-day or last-day save | 0x15 |
| 0x15 `GAME_MODE_PROLOGUE` | 0x8014AF28 `prologue_mode_handler` | the voiced prologue of the first school day: 22 stages, 43 text lines | mode 0x14 | 0x34 stage 0xFF (command screen) |

The numeric mode values and entry addresses are the contract. The integration owner has now applied `GameMode` and the corrected `GAME_MODE_OPENING` (0x11), `GAME_MODE_FILE_SELECT` (0x13), `GAME_MODE_NAME_ENTRY` (0x14) and `GAME_MODE_PROLOGUE` (0x15); see `config/renames/game-mode.txt` and `include/game_enums.h:175-185`. These supersede the original unit's `MODE_*` proposals. The executable NAME_ENT batch does not rename or override shared constants. The code shows that the prologue scene is mode 0x15 (it runs `prologue_line_start`/`prologue_line_run`) and mode 0x11 is the attract opening. Historical emulator report: title item 0 entered 0x13, album creation entered 0x14, final confirmation entered 0x15, and the prologue ended in 0x34; this reconciliation has not repeated that run.

Prefixes. The current naming ADR already assigns `name_entry_`, `file_select_`, `opening_` and `prologue_` to NAME_ENT. Proposed type families and constants use the corresponding PascalCase or upper-case words. Folder and header moves to `name_entry` remain separate work; no move is included in the symbol batch.

Frame rate 60; all timers are in frames. The screen is 320 x 240 with the origin at the centre (x -160..159, y -120..119).

## 2. Source files (proposed names)

| Old | New | Contents |
| --- | --- | --- |
| `src/ovl/NAME_ENT/80132000.c` | `src/ovl/name_entry/name_entry.c` | mode 0x14 (62 functions) |
| `src/ovl/NAME_ENT/80139740.c` | `src/ovl/name_entry/file_select.c` | mode 0x13 (104 functions) |
| `src/ovl/NAME_ENT/80142A80.c` | `src/ovl/name_entry/opening.c` | mode 0x11 story and movie (9) |
| `src/ovl/NAME_ENT/80144AD0.c` | `src/ovl/name_entry/prologue.c` | mode 0x11 entry and mode 0x15 (82 functions, two keep their names) |
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
| `system_data.girl_clear_count[16]` (current `flags`; proposed field name) | Persistent per-girl counters: ETC increments under 99. The album view is a different source, the serialized `GameState.girl_cleared` counter array, not a copy of this system array; see section 15 for writer/save/load evidence. Both are displayed as clear counts. |
| `file_select_information_page` | page switch counter, page = value mod 3 |
| `file_select_menu_mode_lock` | 1 while the screens run: `file_select_mode_handler` writes `menu_mode = 0` every frame |
| `file_select_saved_*` | snapshot of the option groups that changed (section 3.9) |

### 3.3 Stages

`file_select_mode_handler` runs every frame: it hides the 24 girl icon actors (128-151), updates the page indicator, adds 1 to `stage_timer`, switches on the stage, then updates the iris background, the typewriter text, the pointer, the button highlight, the slot overlay and the fade-in flash. In stage 2 a step value that no case handles restores the settings and starts mode 0x10.

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
| 5 | answer | START or circle: restore the settings and `game_mode_set(0x14)` (a game without a save); cross -> step 6 |
| 6 | insert card message | insert a card in slot 1 and press a button |
| 7 | insert card wait | circle: `card_state_reset`, back to step 0 |

### 3.5 Stage 2: the list and the operations

Step 0 (`file_select_scan`): `card_start`, then `card_scan_poll` until result 2 (a 0x201-frame limit returns to stage 1). The scan fills `slot_status` and sets `slot_info_from_system_data`. Then: with the system file -> step 1; else, when any of the 14 album slots `slot_status[1..14]` is non-zero (the C loops `i = 0..13` over the alias label `D_800E7D11 + 4 + i`, which is `slot_status[1 + i]`; the system file entry 0 is not counted, an album in slot 14 alone is) -> step 0x20; else -> step 0x40.

Branch A, system file present (steps 1-3):

- Step 1 `file_select_system_file_read`: reads the system file (`card_system_read_poll` result 3; failure goes to stage 1, or after 0x201 frames or result 0xF0 to stage 1 with a reset). `system_bank_load` is tried; -1 means the file is broken: a message, a wait for confirm or cancel, a question whether to rebuild it; circle goes to stage 0xD0, cross deletes file 0 (up to four tries) and returns to stage 1. On success the previously selected slot is kept when it is valid, else the first valid one of slots 1-13 (slot 14 is not searched) becomes `selected_slot`; the header text and info page are shown (page 1) and the step becomes 2.
- Step 2 `file_select_list_input`: pointer or cursor on slot boxes (`menu_check(0)`; the SELECT mode toggle bit is removed from the pad word). A circle or START press on a valid slot (status 1): when the slot is already the selected one, sound 0x501, `save_slot` = slot and step 3; otherwise the slot becomes the selected one and the header and cursor update. Empty and broken slots do nothing. The page widgets and function buttons run in the same step.
- Step 3 `file_select_album_load`: substep 1 reads the album (`card_save_read_poll` 4); 2 `card_bank_validate`; 3 `game_bank_load` (GameState replaced, menu mode lock off), SE 0x501, `card_slot_summary_store`, `selected_slot = save_slot`; 4 writes the system file (`card_system_write_poll` 0x13) and goes to stage 3. A failed validation or load marks the slot 0xFF and goes to stage 0xF0; a timeout (0x201) or result 0xD0 goes to stage 0xFF; result 0xF0 resets the card and goes to stage 1.

Branch B, no system file but albums (0x20-0x24): 0x20 reads the first valid album, validates it and shows its record from the image; 0x21 is the list (first press selects, second press on the same slot -> 0x24); 0x22 message; 0x23 reads and validates the chosen album and returns to 0x21; 0x24 `game_bank_load` then stage 3 (failure: stage 1).

Branch C, empty card (0x40-0x4C):

| Step | Behaviour |
| ---: | --- |
| 0x40, 0x41 | "no data, create a new album and play?" circle -> 0x44; cross -> 0x42 |
| 0x42, 0x43 | message that the game starts without saving; any of START, circle or cross restores settings and `game_mode_set(0x14)` |
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

`file_select_profile_retain` copies the loaded names and the player's and Shiori's birthday and blood type into the retained buffers (selection indices: month - 1, day - 1, (blood + 3) & 3) and sets `name_entry_retained_state` to 1, so the name entry starts filled in.

Copy (0xC0-0xC8): 0xC0 explanation; 0xC1 choose a valid source (SE 0x501); 0xC2 read, validate and load the source (re-locks the menu mode), then "copy to which album?"; 0xC3 choose the destination: a used slot goes to the overwrite question 0xC4/0xC5 (cross returns to step 0 substep 2, circle goes on), an empty slot is created first (`card_file_create`, SE 0x501; failure goes to stage 4); 0xC6 routes to 0xC7 (system file: write the destination album, then the system file) or 0xC8 (write the destination only); both end at step 0 substep 2.

Clear (0xE0-0xE6): 0xE0 explanation; 0xE1 choose an album with status non-zero (SE 0x501); 0xE2/0xE3 "delete for sure?" cross returns to step 0; circle goes to 0xE4 (system file: `card_file_delete`, mark empty, rewrite the system file, then stage 1) or 0xE5 (delete only, step 0); 0xE6 returns to step 0.

### 3.6 Information panel

Header (every page): the slot number, the family name, the given name, the nickname and the club name (`club_name_table`) of the selected slot, taken from `system_data` when the system file is present, else from the album image (`AlbumSlotRecord`, club in the high nibble of a halfword). Next to it the save date (year, month, day as numbers, `file_select_number_draw`) and the clear count = the number of non-zero `girl_clear_count[0..12]`.

Pages (each has its own title bar in the picture: clear count, parameters, girls' evaluation; emu: pages 0, 2 and 1 seen; page = `file_select_information_page` mod 3, negative values wrap; starts at 1; triangle, or circle on the region x -0x3F..0x1F, y 0xD..0x1B adds 1; pressing a function button sets it back to 1):

| Page | Content |
| ---: | --- |
| 0 | the 13 clear counters as numbers with girl icons, laid out in three columns (girls 0-3 at x -4, girls 4-7 at x 0x3C, girl 12 then girls 8-11 at x 0x88); a counter of 0 shows the lock marker |
| 1 | the nine parameter values of the slot (`metrics[i].value`) as numbers, four at x 16 (y 30, 46, 62, 78: condition, liberal arts, science, art) and five at x 128 (y 14..78: athletics, trivia, looks, guts, stress), the `Parameter` order of core (emu: a fresh game shows 100, 40, 40, 40 and 40, 32, 60, 5, 0) |
| 2 | for heroines 0-10: a star icon (frame 0x58 + (value & 7)) when the girl's met flag is set and her saved byte is non-zero. The byte is `girls[15].scene_counters[g]`, which the telephone evaluation screen (TEL) sets to stars + 1 for every met girl (stars from her rank value: below 6 gives 6, below 11 gives 5, below 21 gives 4, below 41 gives 3, below 61 gives 2, else 1; 0 when her points are 500 or more) |

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

`options_dirty` (OPTION marks changed groups) is copied with the values at the start of the screens and then cleared: bit 0x80 text speed, 0x40 click mode, 0x20 double-click interval, 0x10 confirm button, 8 cursor speed (the icon, menu and voice modes are copied too). While `file_select_menu_mode_lock` is 1 the menu mode is forced to 0. A load (`game_bank_load`) overwrites the settings with the saved ones; when the screens end `file_select_settings_restore` re-applies the changed groups (text speed through `k_speed_set`, the others by assignment), rebuilds the icon bar (icon count 0xF in free icon mode, else 0xE) and validates the icon layout (`file_select_icon_layout_validate`; free mode only: it counts icon positions outside x -0x86..0x7C or y -0x54..0x54 as errors and positions at (0, 0) as zero entries, and resets the icon bar to mode 0 only when there are two or more zero entries and at least one error; either condition alone changes nothing). The restores of icon mode, menu mode and voice mode only run when the saved value is 3 or more, 4 or more and 3 or more; valid values (0-2, 0-3, 0-2) never are, so those three never take effect (as implemented).

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
| 2 | 5 | `name_entry_exit_to_prologue` | waits 0x41 frames, fades 4 per frame to 0, `game_mode_set(0x15)`, text advance 0xE |

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

`name_entry_profile_store` writes into GameState: `player_family_name` (8-byte buffer), `player_given_name`, `player_nickname` (16 bytes), each cleared first and filled with the characters of the matching entry buffer without blanks and zero pairs (blanks inside a name are removed, so characters move left); `player_full_name` = family + given (16 bytes); `player_birthday` (`BirthdayBits`: month = index + 1, day = index + 1, blood = (index + 1) & 3); `girls[0].birthday` from Shiori's selections the same way. The heroine's real birthday is therefore whatever the player enters; the in-game reference value is 14 February, blood O (the chord preset).

### 4.7 The new game (`name_entry_commit` -> `name_entry_new_game_setup`)

Order: sound commands 5, 0xC1 and 3; `game_data_init` (resets calendar, player, girls, events to the start values); `name_entry_profile_store`; then:

1. Shiori's club: `girls[0].club = (Shiori day * 3 + Shiori month) % 11` (day and month are the values 1-31 and 1-12; the result is a club 0-10, never "none").
2. Shiori's initial scores from the blood compatibility grade: `grade = name_entry_blood_compatibility_level[Shiori blood + player blood * 4]` (1-4) with the stored blood values; `idx = grade - 1`; affection (`girls[0].affection`) = 45, 50, 50, 55; tokimeki = 10, 20, 25, 25; syoushin = 10, 0, 0, 0 for idx 0-3. Table rows (player blood O, A, B, AB), columns (Shiori O, A, B, AB): `[1 4 3 2; 3 1 2 2; 2 3 3 4; 3 2 3 1]`. emu: player AB, Shiori born 26 January AB gave club 2 = (26 * 3 + 1) % 11 and 45, 10, 10.
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

The compared buffers are `name_entry_family_name` (0x8014CC74), `name_entry_given_name` (0x8014CC7C) and `name_entry_nickname` (0x8014CC84), compared with `strcmp` against Shift-JIS string literals in the overlay's rodata. The literals have no symbol of their own (the C holds them inline), so they are not in the table; a port extracts them from the NAME_ENT.EXN image at file offset = address - 0x80132000 (the image has no header):

| Check | Family name | Given name | Nickname |
| --- | --- | --- | --- |
| nickname cheat (5 characters, 10 bytes) | not tested | not tested | 0x8014B590 |
| friendly pair (3 characters, 6 bytes each) | 0x8014B59C | 0x8014B5A4 | not tested |
| code with kanji (2, 2 and 5 characters) | 0x8014B5AC | 0x8014B5B4 | 0x8014B5BC |
| code with arrow glyphs (2, 2 and 5 characters) | 0x8014B5C8 | 0x8014B5D0 | 0x8014B5D8 |

The checks run in the order of the list above.

(HYPOTHESIS: the meaning of flag bit 3 on the heroine records is not known from this unit.)

## 5. Mode 0x15: the prologue (`prologue_*`)

### 5.1 The line player

43 text lines are shown one after another. Data: `prologue_line_texts[43]` (up to four short lines of text each), `prologue_line_speakers[44]`, `prologue_line_clips[43]` and `prologue_line_clips_second[43]` (voice clip pairs; both words 0 = no voice; the two tables differ only for line 19, which has two clips). `prologue_line` is the index (0xFF before the first).

`prologue_line_start`: cuts the text list back to the mark; speaker plate: low nibble 0xF is the player (the entered family name), 0x1E and 0x2E are two fixed labels for unnamed schoolgirls, otherwise the girl's short name from `girl_short_name_table[code]`; queues the text with `sndisp(text, 0, code)`; for a voiced speaker requests the clip (`xa_clip_request_set(start, end, code)` and `SND_CMD_VOICE_PLAY`); sets `current_girl` = code; clears `prologue_voice_started`; starts the typewriter.

`prologue_line_run` advances the typewriter and the two-line scroll (more than three display lines scroll up two pixels per frame). The voice-started flag becomes 1 when `cd_audio_state() & 4`. For a voiced speaker, the timer is preserved before that flag is set. Once set, a clip-ended result (`cd_audio_state() & 2`) either requests the different second clip, preserving the timer, or increments the timer when the typewriter result is 1; an ended clip with another typewriter result preserves the timer. A started clip without the ended bit resets the timer. For an unvoiced speaker, typewriter result 1 increments the timer and any other result resets it. At timer >=0x3D the timer resets and the substep advances (`src/ovl/NAME_ENT/80144AD0.c:105-179`). Text speed is 5 initially and 3 at the end.

Speakers by line: 0-4 player, 5 Yoshio, 6 player, 7 Yoshio, 8 player, 9 schoolgirl A, 10 schoolgirl B, 11 Yoshio, 12-14 Rei, 15 Yoshio, 16 player, 17 player (thought), 18-19 Shiori, 20 Yoshio, 21 thought, 22 Yoshio, 23 player, 24-26 Yoshio, 27 player, 28 Yoshio, 29 player, 30 Yoshio, 31 player, 32 Yoshio, 33 player, 34 Yoshio, 35 player, 36 Yoshio, 37 player, 38 Yoshio, 39 player, 40 Yoshio, 41-42 player. Only the heroine and her friends are voiced (Yoshio, Rei, Shiori, the two girls).

Lip sync (HYPOTHESIS: the selected frames are mouth shapes): eight helper functions `prologue_lip_synchronization_*` average four frames of the CD level meter (`getCDlevel`, kept in `prologue_cd_level_history`), divide by 10 (max 3) and select a frame of an actor while the voice plays; short random animations start with `rand() & 0xFF == 0xFF` (HYPOTHESIS: eye blinks).

Two answers are synthesised from the entered data: the lines after line 24 (the friend asks Shiori's birthday) and line 25 (her blood type) show the player's answer, built from `girls[0].birthday`: number of the month, the month word, day number (tens digit only from 10), the day word; and the blood letter from `prologue_blood_texts` plus the type word, spoken under the player's name (`current_girl` = 0xF).

### 5.2 Stages

Every stage has one step except stage 0 (3 steps) and stage 1 (2 steps). Resources are disc files read with `cd_read_sectors` (sprite packs O_xx and CUT_xx under PROLODIR) or `dec_bg_cd_read` (background stills P_xx under BS and PROLODIR); the LBAs are in section 8.

| Stage | Function | Lines | Scene and loads |
| ---: | --- | --- | --- |
| 0 | `prologue_mode_stage_arrival` | 0 | step 0 loads sound bank 0; step 1 title window, calendar panel, BGM 0, still P_01_01, pack O_01 (fade in 1 per frame); step 2 plays line 0 and holds 0x3D frames |
| 1 | `prologue_mode_stage_board` | 1 | step 0 reads P_01_02 and sets the falling particles; step 1 scrolls the picture (counter +1 per two frames to 0x140), line 1, then reads P_01_03 |
| 2 | `prologue_mode_stage_names` | 2 | line 2 over P_01_03; crossfade to the darker P_01_03B and back; then loads CUT_12, O_12 and P_01_04B |
| 3 | `prologue_mode_stage_line_03` | 3 | line 3, fade in; loads CUT_05, P_01_05 |
| 4 | `prologue_mode_stage_line_04` | 4 | line 4; loads O_05, CUT_12, P_01_04B; random blink |
| 5 | `prologue_mode_stage_scene_05_transition` | none | actor 97 slides out and in (SE 0x605), P_01_07 then P_01_04, packs CUT_07, O_07 |
| 6 | `prologue_mode_stage_yoshio_greeting` | 5-10 | plays the six indexed lines; SE 0x503 before line 9; loads P_01_06, CUT_08, O_08 |
| 7 | `prologue_mode_stage_line_11` | 11 | Yoshio's indexed line; BGM change (commands 5, 0xB1, bank 1); loads P_01_07B, CUT_09, O_09 |
| 8 | `prologue_mode_stage_rei_banner` | 12 | white flash up and down, Rei's banner fades in, line 12; loads CUT_10, O_10 |
| 9 | `prologue_mode_stage_rei_lines_13_and_14` | 13-14 | Rei's indexed lines, palette fade and actor 99/100 updates; loads CUT_08, O_11 |
| 10 | `prologue_mode_stage_lines_15_and_16` | 15-16 | portraits drift left 0.5 px per frame; BGM commands 5, 0xC1 and bank 1; loads CUT_12, O_12, P_01_07 |
| 11 | `prologue_mode_stage_line_17` | 17 | BGM 1, SE 0x604; loads P_01_08, O_13 |
| 12 | `prologue_mode_stage_shiori_load` | none | loads CUT_13, O_14, P_01_09 |
| 13 | `prologue_mode_stage_shiori_line_18` | 18 | Shiori speaks (voice); loads O_15 |
| 14 | `prologue_mode_stage_shiori_line_19` | 19 | two clips; loads P_01_06, CUT_14, O_16 |
| 15 | `prologue_mode_stage_lines_20_to_31` | 20-31 plus two answers | 35 substeps; the scene scrolls left 2 px per frame at the end; loads P_01_04, CUT_07, O_07 |
| 16 | `prologue_mode_stage_lines_32_to_36` | 32-36 | BGM 0 variant 1 at line 34; decodes P_01_10 |
| 17 | `prologue_mode_stage_line_37` | 37 | decodes P_01_12, reads O_13 |
| 18 | `prologue_mode_stage_lines_38_to_41` | 38-41 | plays the four indexed lines; waits for the voice of line 38 |
| 19 | `prologue_mode_stage_flash` | none | two white flashes with a portrait switch |
| 20 | `prologue_mode_stage_closing` | 42 | hold 0x3D frames, commands 5 and 0xD1 (BGM fade) |
| 21 | `prologue_mode_stage_finish` | none | resets CD queue and sound state, loads sound bank 0, text speed 3, `menu_mode` = 1, `game_mode_set(0x34)` and `mode_stage_set(0xFF)` |

The speaker identities come from the line-speaker table and `prologue_line_start`, which uses the code to index `girl_short_name_table`. Numeric line and phase names establish code positions only; they do not classify the story or picture. Board, names, Rei's banner and petals remain explicit visual HYPOTHESES in the executable proposal; the observed operations are image loads, palette changes and actor updates. Older visual/emulator reports have no reproduced command in this reconciliation and do not remove those markers. The former childhood-friend, classroom, reaction, tree and legend classifications are not evidence for executable names. A port should preserve the verified order, indices, loads and timing while separately reviewing its presentation resources.

START (0x800) at any time after the first step jumps to stage 21. The dispatcher also draws the title window ("prologue" banner), the screen frame (type 0 at stage 0, type 2 later) and the message window, and the calendar panel at stage 0 shows 1995-04-04, Tuesday (emu: frame 750).

## 6. Mode 0x11: the attract opening (`opening_*`)

Earlier notes call the stage 0 scene (old `func_80143580`) the "attract prologue"; the code shows it is a narrated picture story with its own voice clips, not the prologue lines of mode 0x15 (the `GLOSSARY.md` row for `prologue` is corrected on this branch).

Stage 0 (story): `opening_story_mode_stage` adds 1 to `step_timer`; START, circle or cross (0x860) jumps to step 2. Step 0 (`opening_story_load`) stops the sound, loads sound bank 1, plays BGM 1 and reads seven pictures from FUKUTEST (O_31_05L, 07, 05R, 02, 04, 01, 03 in that order) into the two background slots with their palettes, then starts the first narration clip and four hidden text lines. Step 1 (`opening_story_scene`, 20 substeps) fades and pans the background slots (a slot's x decreases by 1 every fourth frame; the pictures are wider than the screen), starts a second narration clip as soon as the first has ended (`opening_second_narration_started`) and a third text block when slot 0 reaches x -0x118, builds three picture sprites that pan the same way, flashes to gray (sound 0xD1 fades the BGM), reads O_31_06 and the 0xAE-sector OPEN.NBN pack, converts the palettes to sepia (`col2sepia`), animates one character actor, plays a final voiced line, shows it for 0x41 frames and ends with a 255-step gray flash. While the triangle button is held the narration text is shown over a dark box; otherwise it stays hidden. The helper stores CD level and computes an unused four-frame average; voice-playing status alone starts or clears actor 98 (HYPOTHESIS: mouth animation). Holding any button of pad 2 supplies a simulated level (`mode_stage_timer & 0x3F`) for that unused average and also (at substep 16) plays an extra short voiced line and text; a pad 1 button press ends substep 15 early. Step 2 (`opening_story_end`) resets the CD queue, VRAM, sprites and actors, stops BGM and voice and goes to stage 1.

Stage 1 (`opening_movie`): sets the HUD level to 0, text speed 3, queues the 21 lyric lines, `set_movie_offset(0x18, 0x10)`, `movie_play(0, 0x33B, 0x820)` (the STR movie from sector 0x820 to frame 0x33B), sends the CD commands set mode 0x80, pause and init, re-initialises the MDEC buffers, clears VRAM, restores the HUD level, text speed and the pad code state (`pad_code_index`, `pad_code_entered`, `pad_code_timer`) and returns to the title (`game_mode_set(0x12)`). When the stage is entered directly (pad code from the title) the story is skipped.

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
| cheat name literals (no symbols) | 0x8014B590-0x8014B5DF | nine Shift-JIS strings compared by `name_entry_cheat_names_apply` (section 4.8 lists each address) |
| `name_entry_blood_compatibility_level`, `name_entry_initial_scores` | 0x8014D03C, 0x8014D04C | 4 x 4 grade table; four (affection, tokimeki, syoushin) triples |
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

## 11. What a port needs from this unit

1. A new-game function: inputs are the three name strings, the player's month, day and blood type, Shiori's month, day and blood type and a random source; output is the profile fields of section 4.6 plus the club, blood and start-score rules of section 4.7 and the cheat checks of section 4.8.
2. Name input rules: three, three and five characters from four pages (kana, symbols, kanji by reading), presets and chord shortcuts, blank handling (4.2 to 4.4); birthday rules including the day clamp and the random fill (4.5).
3. The file rules (3.5 to 3.9): 14 album slots plus a system file with per-girl clear counts and per-slot summaries; New keeps clear counts and starts at the prologue with the retained names; saves dated 1995-04-04 and 1998-03-01 start a new run with the old names as defaults; every other save resumes at the weekday command screen.
4. The prologue as data: 43 lines with speaker, text, voice clip and the stage in which each is shown (5.1 and 5.2), two answers built from Shiori's entered data, the 61-frame hold between lines.
5. The attract opening: the story pictures, narration clips and lyric lines (section 6), start and skip rules.

Everything else (card polling, CD sector reads, VRAM and palette handling, actor tables) is hardware layer.

## 12. Open questions

1. The current ADR already includes all four prefixes and GLOSSARY already includes `uv`; no glossary or prefix-map change is required by this batch. New identifiers expand unsupported abbreviations and use `mode_stage`/`memory_card` for the glossary's trap senses.
2. Resolved by the integration owner's `config/renames/game-mode.txt`: the four numeric mode meanings now have the correct `GAME_MODE_*` names (section 1). The old `const` rows are superseded and must not be applied as overrides or used to delete core ledger rows.
3. Shared clear-counter fields need coordinated ownership and review across ETC, OPTION and EN_NICHI. The old draft types `Work800E7D10` and `NameRecordF00` are already `SystemData` and `AlbumSlotRecord` under the core ledger. The continuation trace below establishes counter operations; no shared field row is included in the executable NAME_ENT batch.
4. Main-side tables at 0x800B3A3C, 0x800B394C and the blank-character data at 0x800B3644 are main-owner requests only; no main-scope row is included in the executable batch.
5. `D_800B3DB0` (written 1 and 0 by the prologue, read by the tile upload to add `move_255_line`) has no name; it belongs to the h-graphics unit.
6. What the three "retained" restores of icon, menu and voice mode were meant to do (section 3.9) is unknown.
7. Meaning of the heroine flag bit 3 set by the friendly-pair cheat, and of the `girls[12]` score byte at +0xF (threshold 0x29) in the greeting, is not settled.
8. Which clips the two per-slot clip tables play was not listened to; the names carry HYPOTHESIS.
9. `D_800E8BEF` is the album record index read by the page 0 icon test. Its connection to `GameState.album_slot` is a HYPOTHESIS. Any main-symbol rename remains a main-owner request; the original proposal's name is not an applied name.

## 13. Executable batch and explicit residue (T-27051)

`config/renames/proposals/o-nameent-apply.tsv` contains 478 symbol rows: 255 functions and
223 data, all scoped NAME_ENT. It excludes the original proposal's 4 main-owned data rows, one
unused data request (`D_8014CC34`, absent from current C/header), types, fields, constants and moves.
All 255 placeholder function definitions are represented. The two already named
`prologue_line_start` and `prologue_line_run` remain unchanged. Names describe source operations;
line and mode-stage numbers identify code positions, not inferred story scenes. Speaker identities
come from the line-speaker table and `prologue_line_start`; unsupported narrative/visual meanings
were removed from executable targets. Existing visual hypotheses remain marked.

The source/header data-token census is 304 distinct owned `D_` labels: 223 planned,
80 `INCLUDE_RODATA`-only labels, and one comment-only endpoint (`D_8014D168`, the end of
the 16-byte table at `D_8014D158`). The endpoint is not a distinct C variable and is deferred as such.
Each of the following 80 labels is deferred: its sole C occurrence is a preserved rodata
island in `src/ovl/NAME_ENT/80132000.c:469-627`, with no body-level semantic use to justify a role.
These data labels are not jump-table/function/game-state names to guess from their address order.

```text
D_8014B140 D_8014B150 D_8014B15C D_8014B168 D_8014B174 D_8014B180 D_8014B18C D_8014B198 D_8014B1A4
D_8014B1AC D_8014B1B8 D_8014B1C0 D_8014B1CC D_8014B1D8 D_8014B1E4 D_8014B1F0 D_8014B1FC D_8014B208
D_8014B214 D_8014B220 D_8014B22C D_8014B238 D_8014B244 D_8014B250 D_8014B25C D_8014B268 D_8014B270
D_8014B27C D_8014B284 D_8014B290 D_8014B29C D_8014B2A8 D_8014B2B4 D_8014B2C0 D_8014B2CC D_8014B2D8
D_8014B2E4 D_8014B2F0 D_8014B2FC D_8014B308 D_8014B314 D_8014B320 D_8014B32C D_8014B338 D_8014B344
D_8014B350 D_8014B35C D_8014B368 D_8014B374 D_8014B380 D_8014B38C D_8014B398 D_8014B3A4 D_8014B3B0
D_8014B3BC D_8014B3C8 D_8014B3D0 D_8014B3D8 D_8014B3E0 D_8014B3E8 D_8014B3F0 D_8014B400 D_8014B410
D_8014B420 D_8014B430 D_8014B440 D_8014B448 D_8014B450 D_8014B458 D_8014B460 D_8014B468 D_8014B470
D_8014B478 D_8014B480 D_8014B484 D_8014B48C D_8014B494 D_8014B49C D_8014B4A4 D_8014B4AC
```

There are another 176 generated-assembly-only data labels not mentioned by current C/header.
They denote literal/recovery positions and internal/adjacent data. Each listed label is explicitly
deferred until a consumer or aggregate boundary is established; naming it from neighboring text,
asset order or a nearby address would not establish a game-logic role. No data bytes are reproduced.

```text
D_8014B4B4 D_8014B4C0 D_8014B4DC D_8014B4E0 D_8014B4E4 D_8014B4E8 D_8014B4EC D_8014B4F0 D_8014B4F4
D_8014B4F8 D_8014B4FC D_8014B500 D_8014B504 D_8014B508 D_8014B50C D_8014B510 D_8014B520 D_8014B524
D_8014B528 D_8014B530 D_8014B534 D_8014B55C D_8014B56C D_8014B574 D_8014B57C D_8014B580 D_8014B584
D_8014B58C D_8014B590 D_8014B59C D_8014B5A4 D_8014B5AC D_8014B5B4 D_8014B5BC D_8014B5C8 D_8014B5D0
D_8014B5D8 D_8014B660 D_8014B664 D_8014B668 D_8014B67C D_8014B68C D_8014B69C D_8014B6AC D_8014B6C0
D_8014B6D0 D_8014B6E0 D_8014B6F4 D_8014B704 D_8014B710 D_8014B71C D_8014B72C D_8014B73C D_8014B74C
D_8014B75C D_8014B770 D_8014B780 D_8014B790 D_8014B7A4 D_8014B7E0 D_8014B7EC D_8014B804 D_8014B810
D_8014B848 D_8014B864 D_8014B878 D_8014B888 D_8014B8A4 D_8014B8C0 D_8014B8CC D_8014B8E0 D_8014B8F0
D_8014B904 D_8014B918 D_8014B928 D_8014B93C D_8014B94C D_8014B964 D_8014B974 D_8014B984 D_8014B9B4
D_8014B9D4 D_8014B9F0 D_8014BA0C D_8014BB00 D_8014BB20 D_8014BB40 D_8014BB54 D_8014BB68 D_8014BB7C
D_8014BC00 D_8014BC10 D_8014BC24 D_8014BC34 D_8014BC44 D_8014BC54 D_8014BC64 D_8014BC78 D_8014BC8C
D_8014BCA0 D_8014BCFC D_8014BD10 D_8014BD20 D_8014BD40 D_8014BD5C D_8014BD6C D_8014BD88 D_8014BD98
D_8014BDA8 D_8014BDB8 D_8014BDC8 D_8014BDD8 D_8014BE2C D_8014BE3C D_8014BE58 D_8014BE64 D_8014BE70
D_8014BE78 D_8014BE80 D_8014BE8C D_8014BE94 D_8014BE9C D_8014BEA4 D_8014BEAC D_8014BECC D_8014BEEC
D_8014BF0C D_8014BF28 D_8014BF48 D_8014BF58 D_8014BF78 D_8014BF90 D_8014BFA4 D_8014BFB8 D_8014BFD8
D_8014BFF0 D_8014C00C D_8014C01C D_8014C03C D_8014C054 D_8014C068 D_8014C07C D_8014C09C D_8014C0B4
D_8014C0D4 D_8014C0EC D_8014C104 D_8014C114 D_8014C130 D_8014C14C D_8014C150 D_8014C154 D_8014C158
D_8014C718 D_8014C73C D_8014C764 D_8014C77C D_8014C7A0 D_8014C7BC D_8014C7D4 D_8014C7F4 D_8014C810
D_8014C82C D_8014C84C D_8014C864 D_8014C874 D_8014C910 D_8014C918 D_8014C920 D_8014C92C D_8014C930
D_8014C938 D_8014CC7A D_8014CC7B D_8014CC82 D_8014CC83
```

Remaining separate work: original type/field tables need header-closure ownership and independent
review; literal constants require supported constant-site tables; file and header moves require
individual commits and prototype-view checks; gameplay locals remain a later essential-role pass.
Shared-core requests are described in section 12 and must be resolved by their owner. The original
mixed TSV is historical analysis and must not be passed directly to `rename.py apply`.

Historical validation (2026-10-24): full Docker `python3 tools/rename.py plan
config/renames/proposals/o-nameent-apply.tsv --ledger o-nameent`, in `tokimemo-decomp-arm64` with
`--cpus=2`, exited 0: 478 rows, 9 files to change, zero `PROBLEM` lines. Subsequent review changes
corrected notes and documentation without changing target names. Independent Sol 6.1 review passed
Standards and Spec with zero unresolved findings in the reviewed scope; it checked the full structural
census, suspicious semantic claims and a broad source sample, rather than claiming exhaustive
runtime semantics. A separate Docker check matched all 255 function citations to actual definitions.
No new emulator evidence is claimed by that historical plan. Application and current source status
follow the applied ledgers; the old plan is not a pending instruction. Shared topic-header ownership
now follows `config/header_owners.json`, with current overlay declarations in the accepted NAME_ENT
header path. The final matching proof passed; final documentation rereview remains in progress.

## 14. Historical logical-file and topic-header proposal (second pass)

The proposed file/header destinations below describe the former layout packet. The accepted logical
layout is applied; use `src/ovl/name_entry/`, `include/ovl/name_entry.h` and the current owner
registry for present paths. Preserve the original address ranges and declaration widths recorded in
this proposal; it does not authorize new moves.

The four C objects divide along gameplay responsibilities. Their original boundaries in
`config/objects/NAME_ENT.txt`; the logical destinations are the four paths in section 2. The fourth
object also owns the opening-mode dispatcher, so its accurate module description is prologue plus
opening dispatch. Do not move that dispatcher between objects as part of naming. The two named
line-player functions remain in the prologue object. The source layout and logical paths are applied;
the following file rows preserve the earlier reviewed mapping.

The supported rename-tool rows, in separate move-only tables/commits after symbols, are:

```text
file NAME_ENT 80132000 name_entry T-27051 name/profile input, birthday input and new-game rules
file NAME_ENT 80139740 file_select T-27051 album/system-file selection and operations
file NAME_ENT 80142A80 opening T-27051 attract picture sequence and opening movie
file NAME_ENT 80144AD0 prologue T-27051 indexed prologue plus opening-mode dispatcher
dir NAME_ENT NAME_ENT name_entry T-27051 existing ADR folder, preserve disc-named build roots
file include ovl/NAME_ENT.h ovl/name_entry.h T-27051 temporary overlay umbrella
```

The exact six rows were also saved as `config/renames/proposals/o-nameent-files.tsv`. The full Docker
`python3 tools/rename.py plan config/renames/proposals/o-nameent-files.tsv --ledger o-nameent-files`
exited 0: 6 rows, 11 files to change, zero problems. It reported one advisory: `tools/test_redraft.py`
named the old header path at that time. The path moves are now applied; these counts and advisory are
historical preparation evidence. Disc names in overlay targets, asm roots and checksums remain the
identities used by the build.

The following table records the older proposed split of the overlay umbrella into logical declaration owners:

| Topic header | Declaration owner and consumers | Source evidence |
|---|---|---|
| `include/ovl/name_entry/name_input.h` | name grid, selection/cursor state, birthday input, presets, commit/new-game entry; input object consumes it | `src/ovl/NAME_ENT/80132000.c:103-2595` |
| `include/ovl/name_entry/file_select.h` | file-screen callbacks, slot summaries, clear-counter rebuild scratch, display-page/iris state; file-select object consumes it | `src/ovl/NAME_ENT/80139740.c:1-3353` |
| `include/ovl/name_entry/opening.h` | opening callbacks, movie/lyric references and opening-mode declaration; opening object and the dispatcher in the prologue object consume it | `src/ovl/NAME_ENT/80142A80.c:277-704`, `src/ovl/NAME_ENT/80144AD0.c:71-81` |
| `include/ovl/name_entry/prologue.h` | line player, line/speaker/clip tables, prologue callbacks and frame-history state; prologue object consumes it | `src/ovl/NAME_ENT/80144AD0.c:83-2818` |
| `include/ovl/name_entry/profile.h` | overlay-owned birthday bit view and retained entry defaults shared by input and file-select objects; retain one declaration per symbol | `src/ovl/NAME_ENT/80132000.c:1227-1281`, `src/ovl/NAME_ENT/80139740.c:1753-1775` |

The proposed topic paths above are not current declaration providers. Current overlay declaration
ownership follows the accepted header path and `config/header_owners.json`; shared `GameState`, enums,
memory-card services and main BSS remain in their registered owners. The original narrow widths and
override views remain evidence constraints, not permission to add declarations.

The original TSV's 144 `const` rows are an inventory, not an executable constant plan. Four game-mode
rows are superseded by the applied core. The other groups need the following context-bound pass;
new names use `mode_stage`, `memory_card` and fully spelled words where the old draft used trap words
or unlisted abbreviations. An enum describes a closed set, while a mask/count/bound is a define.

| Proven family / values | Scope and candidate treatment | Definition/body evidence and constraints |
|---|---|---|
| Game modes 0x11/0x13/0x14/0x15 and command-screen 0x34 | reuse `GAME_MODE_OPENING`, `GAME_MODE_FILE_SELECT`, `GAME_MODE_NAME_ENTRY`, `GAME_MODE_PROLOGUE`, `GAME_MODE_COMMAND_SCREEN` where each source role matches | applied `include/game_enums.h:175-199`; entry dispatchers and exit calls; no duplicate overlay modes; `GAME_MODE_SCHOOL_DAY` is the distinct value 0x40 |
| Girl and club identities | reuse `GirlId` and `Club` constants for proven indexed girl/club roles | `src/ovl/NAME_ENT/80132000.c:2451-2530`; pair assignment preserves Shiori's club formula `(3*day+month)%11`; 11 is the number of joinable clubs here, not the stored no-club enum type |
| Controller single bits and combined masks | reuse `PAD_*`; express only proven combinations at their pad-word sites | `include/game_enums.h:276-289`, `src/ovl/NAME_ENT/80132000.c:1201-1225`; SELECT plus shoulder combinations; packed two-controller masks need their upper-half placement retained |
| Name-input mode stages 0..2; input steps 0..9; Shiori steps 0..5 | proposed `NameEntryModeStage`, `NameEntryInputStep`, `NameEntryShioriStep` closed sets | mode/stage/step dispatch in `80132000.c:112-141`, `:422-467`, `:2220-2254`; keep `u8` selectors rather than enum-typed storage |
| Tabs 0..4, birthday fields 0..2, name cursor sentinel 0xC | enums for tabs/fields; define for inactive cursor | `80132000.c:368-419`, `:770-870`, `:1744-1791`; cursor 0xC is not an address, and field/tab values are different domains despite overlaps |
| Presets 15; family/given/nickname slot counts 3/3/5; grid bytes 32 per line | `NAME_ENTRY_PRESET_COUNT`, named slot/grid counts | `80132000.c:103-110`, `:959-1135`, `:1137-1154`; two-byte encoding gives 16 characters per 32-byte row, never 32 characters |
| Unset birthday selection 0xFF; month/day selection counts 12/31; random blood bound 3 | defines at selection variables only | `80132000.c:1962-2021`; random blood index excludes the fourth entry intentionally; retain `%3`, do not replace by a four-value enum count |
| February/day limits and stored birthday bit packing | named month/valid-day counts only at profile sites; stored blood values O/A/B/AB=0/1/2/3 need shared-profile owner review | `80132000.c:1962-2021`, `:2385-2450`; selection values are zero-based, stored month/day add one, blood uses `(index+1)&3`; fixed-width bit view must stay intact |
| Compatibility grades 1..4, 4 blood columns, 3 score components; initial score triples | counts and component indices, with the values kept in named score data | `80132000.c:2466-2470`, `D_8014D03C`, `D_8014D04C`; these are girl scores, distinct from the nine player parameters |
| Nine player parameters; special-name score values 573, 0, 999, 32/64 and 127 | reuse parameter count if shared owner supplies it; named limits/cheat effects at actual writes | `80132000.c:2531-2573`; preserve four checks in source order, 16-bit parameter stores, and girl met/b3 masks without inventing b3's meaning |
| File-select mode stages 0,1,2,3,4,0xC0,0xD0,0xF0,0xFF; operation steps | proposed `FileSelectModeStage` plus operation-specific step enums | `80139740.c:1045-1120`, `:2800-3135`; same byte holds several modes' stages, so a global `GameState.mode_stage` replacement is unsafe |
| Slot count 14, system slot 0, valid/broken/empty slot statuses; information pages 0..2 and function buttons 0..3 | shared slot/result/status constants where already defined; local enums for page/button domains | `80139740.c:85-112`, `:1045-1083`, `:1086-1124`; the scan examines 14 album slots, while one first-valid-slot search stops at 13—preserve that source boundary |
| Memory-card timeout 0x201 and result values | reuse shared memory-card result constants when proven; name timeout as 513 ticks | `80139740.c:947`, `:1059`, `:1102`, `:1224-1261` and other operation handlers; retain each `>=` comparison and signed/unsigned cast |
| Iris growth guard 0x4001 | name as an update-stop threshold, not a saturation limit | `80139740.c:578-583`; recurrence can overshoot, so do not clamp or call it a maximum |
| Opening stages 0..1 and story steps 0..2 | context-local mode-stage/step enums | `80142A80.c:695-704`, `80144AD0.c:71-81`; source ownership crosses two C objects, so declarations belong to opening topic |
| Prologue mode stages 0..21 and 43 line indices; speakers player0xF, schoolgirl codes0x1E/0x2E, girl ids | `PrologueModeStage` with neutral source-role names, count defines and source speaker mapping | `80144AD0.c:83-103`, `:2718-2818`; actual speaker table establishes identities, not scene narrative |
| Prologue line hold 0x3D, opening hold 0x41, blink masks 0x7F/0xFF | separate timing/mask defines per observed role | `80144AD0.c:155-179`, `80142A80.c:669-691`; preserve timer-reset/preservation branches and random comparison outcomes; 61 and 65 are different holds |

The inventory is deliberately tied to all matching contexts in the relevant rule family. The next
pass should enumerate every site before using `rename.py const`. A broad site such as
`GameState.mode_stage`, `GameState.mode_step` or a shared setter argument is insufficient when values
mean different things in file selection, input, opening and prologue. Use reviewed function-context
sites (or add that tool support first); keep unsigned comparisons, postincrements, packed masks and
fixed-width storage unchanged. Presentation LBAs, image pointers, shader-like palette/actor values,
SDK commands without a verified mapping and unknown flag meanings remain deferred, with numeric
identities retained rather than given invented gameplay names. No constants have been applied here.

## 15. Essential semantic continuation (T-27051)

The 478 frozen function/data rows remain unchanged. The supplementary pass audits all 257
definitions in the four original objects for gameplay parameters/locals, owned types/fields and
literal sites. Rendering and match-only temporaries are distinguished from gameplay residue;
neither an earlier draft nor logical filenames prove semantic completion.

Supplementary locators refer to captured original objects. Resolve the concurrent filename moves
through `config/renames/proposals/layout-NAME_ENT-files.tsv`, the unit YAML and original
address-range comments. Shared ETC and DATE2/EN_NICHI locators similarly use their own layout maps;
they are evidence locations, not a claim that the old address-shaped path still exists live.

### Frozen identifier supplement

`config/renames/proposals/o-nameent-essential.tsv` has 121 five-column rows: 31 parameters,
85 locals, 3 types and 2 fields. SHA-256:
`3fecf486ad825a3dc270bd2b20aadfb5b2e1cc11354a4adece1741532d2ec5f1`.
The full verification-enabled Docker plan passes: 121 rows, 5 changed paths, exit 0, zero problems.
Captured-tree manifest: `5887d7ab9aac287808a7d8b1f66c1c0213118c4afc5bc33b1d7013a10709c613`.
Proof: `.claude/orchestrator/state/T-27051-essential/nameent/plan-result.json`, `plan.log` and
`snapshot-essential.tsv`. The K&R birthday parameters use distinct display/finalize role names to
avoid the current tool's false file-scope collision; no declaration type changes. Independent
packet preparation reviews pass; no application is claimed.

The full 257-function census contains 357 parameter/local declarations. A second exact-use audit recovered 17 more
declarations from false retained classifications, including two live heading parameters, buffers,
game/system pointers, slot indices, a numeric quotient and explicit uninitialized hit-test locals.
They form `o-nameent-essential-retained-locals.tsv`, SHA-256
`5bc8a0a35eb3f1220cb0b7594c180709fece73b72ece79bd0b5eb05bf663fdaf`.
Its full verification-enabled plan passes: 17 rows, 3 changed paths, exit 0, zero problems.
The combined supplements contain 138 identifier rows: 33 parameters, 100 locals, 3 types and 2
fields. The earlier 121 rows remain byte-for-byte frozen; independent preparation reviews pass.
Final declaration categories are 133 prepared roles, 92 already meaningful names, 38 actual loop
counters, 51 optional presentation/hardware declarations and 43 unused/match-only declarations.
Every census item has a category; these are classification counts, not proof of understood
gameplay meaning for every presentation or matching artifact.

The first 1,704-row literal inventory required correction following Standards review. That report is
historical. Numeric token validity and the 105 verified positive-site plans passed, but some candidate column lists joined
opposite-sign or different-role operands; some nested tab cases were mislabeled as name-part
selectors, and an opening timer label leaked into other flows. Those accepted findings required AST
operand/switch/flow reconciliation and are resolved. Current application is governed by applied
ledgers; the 1,617-row inventory is not evidence that every candidate site landed.

The corrected `o-nameent-essential-constants.tsv` has **1,617 exact operand rows / 781 symbols**,
SHA-256 `e7842e66e012b1208b9170e13da7acc2f3808135b36b375762678d9c548905cd`.
Every row identifies one signed operand at an original-source byte column. All 7,581 numeric code
tokens have explicit dispositions: 7,571 match the AST cache directly, and ten preprocessing-coordinate
exceptions are individually recorded (two negative gameplay comparisons and eight optional hardware
operands). The final four shared club-schema rows correct the previous false FAKE-layout exemption.

Six positive families have exact captured-tree dry-plan proof: tabs 63 hits, inactive cursor 3,
unset selections 12, name-part calls 12, birthday-profile calls 14 and iris stop 1, totaling 105.
The unchanged proof JSON SHA-256 is
`445aa462e11213f18efe409f96a468997b20d0f3a27bc8b93026168761903bff`.
Independent Standards and Spec checks accepted the corrected operands, domains, preserved edits
and explicit remaining blockers. This was preparation approval, not source application or matching
build proof at that time. T-27070 repaired unary-minus handling through typed signed operands in
`tools/rename.py`, with negative and mixed-type regression coverage. Tool support does not imply that
all 27 negative candidates were applied; consult each site's current ledger entry.

### Shared clear counters and current type names

Earlier `include/game.h` line citations in the following evidence tables describe the source snapshot
used for those layout deductions. Current declaration owners follow `config/header_owners.json`; the
separate 14-byte album view and 16-byte saved GameState array widths remain distinct.

The applied core ledger already renamed `Work800E7D10` to `SystemData` and `NameRecordF00` to
`AlbumSlotRecord` (`config/renames/core.txt:254,300`). Old proposal rows must not revive the old
type names. The NAME_ENT readers use `SystemData.flags` or an album record's `flags` to draw counts
and gate the corresponding icons (`func_80139F30`, `src/ovl/NAME_ENT/80139740.c:139-260`).

The writer is ETC's `func_80144F74`, `src/ovl/ETC/80143240.c:738-750`: it increments both
`game_state.girl_cleared[current_girl & 15]` and `system_data.flags[current_girl & 15]` while each
is below 99. Thus the former field is also used as a counter, despite the old core note calling it
a flag. Main memory-card code copies 16 bytes between the saved bank and `system_data.flags`
(`src/main/memory_card.c:725,777`). The album view has 14 bytes at offset 0x688
(`include/game.h:389`); it must not be widened to match that separate 16-byte system record.
The album view's provenance is now traced: main `src/main/memory_card.c:834` copies
`GameState.girl_cleared[16]` into `GameSaveImage.unk_488[16]`, and `:950` restores it. The save-image
array starts at `memory_card_block + 0x200`, so its offset 0x488 is exactly the album reader's
0x688. The original proposal's claim that album flags copy `SystemData.flags` is superseded.

These are proven shared-owner requests for counter names and layout reconciliation, not executable
NAME_ENT field rows. OPTION/ETC/EN_NICHI consumers and the save views must be reviewed together;
main owns any resulting field changes. The packed flags' other bits and unused array slots do not
gain narrative meanings from this counter evidence.

| Shared owner request | Evidence and boundary |
|---|---|
| `SystemData.flags` -> `girl_clear_count` | 16-byte array; ETC's capped increment and NAME_ENT count renderer above. Main owns the canonical declaration. |
| `SystemSaveImage.flags` -> `girl_clear_count` | Separate 16-byte save image, `include/game.h:44`; copied to/from the canonical array by main memory-card code. |
| `AlbumSlotRecord.flags` -> `girl_clear_count` | 14-byte reader view at 0x688, `include/game.h:389`, over the serialized GameState counter array; preserve its width. |
| `GameSaveImage.unk_488` -> `girl_clear_count` | Actual 16-byte serialized GameState counter array, `include/game.h:97`; main save/load loops above prove the mapping. |
| `GameState.girl_cleared` counter wording/name review | ETC increments it under 99 alongside the persistent system array; keep its separate storage/lifetime and do not infer that it is only a Boolean. |

### Prologue clip-pair interpretation

The prologue passes each two-word row to `cd_audio_request_set`. Main `src/main/cd_queue.c:475-481`
stores the words in request entries 2 and 3. The sound consumer
`src/main/sound_driver.c:385-402` splits their high bytes into filter cells, masks each low 24 bits
and scales those low parts into playback boundaries. `func_8007A868` (`:423-443`) promotes the
first to the current/start position and the second to the end boundary checked against CD ticks
at `:112`. Thus start/end roles are supported, but the original words are packed values, not plain
sector IDs. A pair-field name/evidence must preserve that distinction; the voices' narrative
identity remains a separate listening question.

### Literal-site boundaries

The final constants inventory records individual observed expressions and exact source contexts.
Current `rename.py const` has header-closure scope, no function-local or line selector, no arbitrary
arithmetic-operand support, and rejects reuse of already-declared enum names through its
fresh-declaration path. Shared stage/step setters and generic local names can hit unrelated values.
Such contexts remain explicit application/tooling work; no duplicate core enums or unsafe broad
substitution is proposed as a workaround.

### Matching artifact with an unproved input producer

`name_entry_birthday_buttons_update` (`func_80137608`, captured `80132000.c:1911-1935`) tests
two stack locals that it never assigns, as its existing FAKE comment states. The new names
`fake_hit_x` and `fake_hit_y` identify the horizontal/vertical tests only. Their producer is not
proved by this function, and no current-cursor source is invented. The separate field-cursor-3
confirmation path has an explicit source predicate. A port must resolve the first path's intended
input contract from caller/stack/runtime evidence rather than copy uninitialized C reads or assume
the speculative earlier-call explanation is established.

### Current executable constant packet v2 (2026-10-24)

The fresh ignored packet `.claude/orchestrator/state/T-27051-constant-v2/` passes current Docker
planning for 105 NAME_ENT replacements. The corrected name-part and birthday/profile selectors
are callee argument selectors, `NAME_ENT:callee(arg0)`, rather than caller-local parameter
selectors. Fresh token comparison proves 104 edits in `src/ovl/NAME_ENT/name_entry.c` and one
in `src/ovl/NAME_ENT/file_select.c`. All four private-header users belong to NAME_ENT.

The 47 active-tab sites use the positive route. Four strict one-caller requests name the 16
positive hover operands (1/5/5/5) while leaving the negative initializer at name_entry.c:899
outside their scopes. NameEntryTab, NameEntryNamePart and NameEntryBirthdayProfile are enums;
inactive cursor, unset profile selection and iris cutoff are defines. There are 89 legacy and
16 strict replacements. The exact 44 unnamed selections remain numeric: 19 cursor, 24 profile
and one iris operand; this includes the unchanged out-of-domain `3U` cursor comparisons.

The combined 204-edit plan passed exact source reconstruction and input-pin checks. Legacy rows
are metadata-free const records without signed-site replay; strict hover requests retain versioned
metadata and default inverse/GCC/prototype-view proofs. Primary owns application, the two tab-domain
extern comments, ledger checks and matching verification. The negative initializer and the wider
literal inventory remain explicit work; this is bounded preparation, not unit completion.
