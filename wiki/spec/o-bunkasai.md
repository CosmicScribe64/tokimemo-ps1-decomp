---
type: spec
updated: 2026-10-24
unit: BUNKASAI
ticket: T-27025
sources: ["src/ovl/BUNKASAI/", "include/ovl/BUNKASAI.h", "config/renames/proposals/o-bunkasai.tsv", "src/ovl/BUNKA_SD/80137890.c", "src/ovl/BUNKAKEN/", "wiki/spec/o-tt.md", "wiki/spec/core.md", "wiki/spec/m-scheduler.md", "wiki/spec/m-dates-b.md"]
---

# Overlay BUNKASAI: the culture festival, club-exhibit variant (port spec)

Phase 2 spec for the Godot port ([[tickets/T-27025-p2-o-bunkasai]], parent [[tickets/T-27000-phase-2]]). Everything below is read from the matched C of `src/ovl/BUNKASAI/` (207 functions, 20 files) and checked in the emulator where noted. "HYPOTHESIS" marks a reading of game meaning that the code does not prove. New names are in `config/renames/proposals/o-bunkasai.tsv` (360 rows). The overlay prefix is `culture_` (functions, globals, constants `CULTURE_`, types `Culture`): `docs/adr/0001-phase-2-naming.md` maps BUNKASAI to the folder `culture_festival` and the prefix `culture_`; the Module words table of `GLOSSARY.md` already lists it. Names of the main exe, `GameState` fields, girl ids and clubs are the ones of the core table (`GIRL_*`, `CLUB_*`, `girls[i].affection`, `dialogue_group`, ...). No game text is quoted: exhibits are described in plain words.

## 1. What it is

BUNKASAI plays the school culture festival for a player who belongs to one of the six culture clubs. The festival happens once in each of the three school years (1995, 1996, 1997). The player's club has an exhibit or performance, and the scene shows it with one heroine: either a heroine who is in the same club, or a heroine picked at random among those the player has met. She walks into the exhibit, talks according to her mood, and the festival adds tokimeki to every girl according to how well the club did.

Who gets this overlay, and who gets BUNKAKEN: the queue record of type `DAY_EVENT_CULTURE_FESTIVAL` (0x24) is turned into mode 0x70 (`MODE_CULTURE_FESTIVAL_A`, this overlay) when the player's club id is below 6 (`CLUB_LITERATURE` .. `CLUB_BRASS_BAND`), and into mode 0x71 (BUNKAKEN) for every other club id, see [[spec/m-scheduler]] and [[spec/core]]. So the nibble the knowledge notes asked about is the player's club: this overlay is the club-member festival; BUNKAKEN is the festival of a player whose club has no exhibit (the glossary calls it the tour; HYPOTHESIS that it tours the culture clubs).

Unit size: 207 functions in 20 C files; the data starts at 0x801603xx (the largest part is strings, which have no rows). The overlay is the usual 0x30000-byte slot at 0x80132000 (sector 0x916A, `overlays`), entry `culture_festival_entry` at 0x80142700. About 20 functions hold rules; the other 180 are scripted scene steps of one pattern (sections 5 and 6).

The overlay depends on three other overlays: BUNKA_SD (mode 0x80, the stage shows with SD sprites, section 7.2), TT (mode 0xD0, the TwinBee game exhibit, section 7.4) and the shared main-exe scene helpers (`normal_date_*`, `dialogue_run`, `dec_bg_*`, `bustup_*`).

## 2. Entry, exit and the overlay reload

- Entry: `main_state_dispatch` calls `culture_festival_entry` every frame while mode 0x70 runs. The first call (main mode 0x70 reached through the loader, mode 2) arrives with stage 0 and step 0; the scene's own phase byte `culture_phase` starts at 0.
- Exit: `culture_festival_entry` phase 8 (finish) ends with `day_event_next(1)`, which advances the day queue and returns to the day screen (mode 0x31).
- **The overlay is reloaded after every sub-scene.** BUNKA_SD, TT and the stage-163 path replace the code in the shared slot; when they finish, main mode 0x70 runs again and the loader reads BUNKASAI anew. All overlay-private state is lost (the script-pointer words, the fortune timers, the typed-line counter). Everything that must survive lives in main's bss, which is why the scene variables of section 3.1 sit at `D_80122E98..D_80122EC0` and `D_800E7D4E/F`. The scene continues at the saved stage and step; the call sites save "stage and step plus one" in `culture_return_stage/step` (shows, fortune, stage 163) or in `GameState.saved_mode/stage/step` (TT).
- Emulator check (title-menu state with mode 2, overlay target 0x70, club and calendar poked): phases 0..6 change at frames 60, 61, 62, 202, 222, 254, 255, phase 7 at frame 302; the stage sequence for a literature-club player in 1995 with only girl 0 met is 0, 14, 15, 16 (step 0..0x11), then phase 8 and the overlay loader again at frame 906 (the poked state was not a real game, so `day_event_next` took its alternative target, mode 0x22).

How the checks were made (for later probes): start from the title-menu save state of [[emulator]], poke `GameState` at frame 2 (main mode 0x1108 = 2, overlay target 0x110E = 0x70, the club halfword at +0xF4 = club << 12, the calendar bytes at +0x3E, per girl the flag word at +0x1BC + 0x38 g + 0xC: byte 2 = club << 4, byte 0 bit 1 = met, `input_device` +0xF74 = 1, `menu_mode` +0x1092 = 1), then press a face button every 14 frames (CIRCLE confirms menu rows) and log mode, stage, step and the scene variables whenever they change. The shipped `tools/emu` has no poke option; the scratch wrapper that added one lives in `build/emu_scratch/` of the worktree and is not committed.

## 3. State

### 3.1 Scene variables (main bss, shared with BUNKAKEN and BUNKA_SD)

| Address | New name | Meaning |
|---|---|---|
| `D_80122EC0` | `culture_phase` | phase of the entry switch (section 4), `CulturePhase` |
| `D_80122EB8` | `culture_exhibit_club` | player's club id 0..5, set by `culture_stage_intro`; the first word of `CultureFestivalDispatch` (core type, fields renamed `club` and `girl` here) |
| `D_80122EBC` | `culture_visitor_girl` | heroine id 0..10 of the running scene, 0xFF = none (set to 0xFF in phase 0); second word of the dispatch pair. For the Shiori member block it is 0, for a club mate her own id, for a visitor the random pick |
| `D_80122EAC` | `culture_show_variant` | cast variant handed to BUNKA_SD; BUNKASAI writes 0, 2, 3 or 6 (section 7.2) |
| `D_80122EB0`, `D_80122EB4` | `culture_return_stage`, `culture_return_step` | return point after a sub-scene (byte values) |
| `D_80122EA8` | `culture_fortune_result` | fortune result 0..3, added to the dialogue group (section 7.3) |
| `D_80122E98` | `culture_default_disp_off` | nonzero skips `Default_Disp()` at the end of the entry; set to 1 around the stage-163 jump |
| `D_800E7D4E` | `tt_stages_cleared` | result of the last TT run (stages cleared, 0..4), read by the game scenes |
| `D_800E7D4F` | `tt_mode_flags` | `Flags7D4F` byte: bit 0 `tt_unlocked`, bit 1 `direct_mode`, bits 2-4 `direct_stage` |
| `D_80122EA4` | (not renamed) | set to 0xFF in phase 0, never read here; BUNKAKEN compares it with girl ids. Left for the BUNKAKEN unit |

Main fields used: `GameState.year` (95, 96, 97), `month`, `day`, `player_flags` bits 12-15 (club), `player_birthday`, `girls[i]` (`tokimeki`, `affection`, `syoushin`, `flags` with `met` bit 1, `acquainted` bit 2 (HYPOTHESIS name of m-scheduler), `festival_call_95/96` bits 12 and 13 (m-scheduler names), `club` bits 20-23, `birthday`), `clubs[c].exp` (u16), `param_liberal_arts`/`param_science`/`param_art` (`value` half), `daily_event_bits`, `event_log`, `saved_mode/stage/step`, `input_device`, `dialogue_group/line/*_table/choice`, `girl_menu_enabled`.

### 3.2 Overlay data (the tables the logic reads)

| Name | Address | Content |
|---|---|---|
| `culture_stage_table` | 0x80160810 | 165 function pointers indexed by `GameState.stage` (type `CultureStageTable`) |
| `culture_script_text_table`, `_command_table`, `_voice_table` | 0x801604F0/F4/F8 | the three pointers written by the 22 `culture_script_set_*` functions, copied into `dialogue_text_table`, `dialogue_command_table`, `dialogue_voice_table` |
| `culture_tokimeki_gain_95/96/97` | 0x80160534/584/5D4 | 13 girls x 6 clubs bytes, section 8.1 |
| `culture_club_param_index` | 0x8016052C | club 0..5 -> parameter block index 1, 1, 2, 2, 3, 3 |
| `culture_choice_names` | 0x801606DC | 3 years x 3 options x 33 bytes of option text (birthday scene) |
| `culture_fortune_compat_table` | 0x80160B50 | 4 x 4 words, section 7.3 |
| `culture_fortune_girl_names`, `culture_fortune_girl_blood_labels`, `culture_blood_type_labels` | 0x80160AE0 / B10 / B3C | 12, 11 and 4 text pointers for the fortune screen |
| `culture_birthday_*_table_00..12` | 0x80160640..0x801606D8 | 13 text, 13 command and 13 voice pointers of the extra file the birthday scene reads to 0x801B0000; only index 1 is used |
| `culture_unused_girl_bytes`, `culture_visitor_list_init`, `culture_backstage_title` | 0x80160520, 0x80160AA4, 0x8015D774 | dead 11-byte copy; template of the visitor list; the backstage banner string |

## 4. Phase machine (`culture_festival_entry`)

Every frame: `bustup_wink_update()`, then a switch on `culture_phase`, then `Default_Disp()` unless `culture_default_disp_off` is set. Constants `CULTURE_PHASE_*`.

| Phase | Name | What it does |
|---|---|---|
| 0 | `INIT` | sound command 0xBF, sound bank 1 start, scene reset, `girl_menu_enabled[i]` = the met bit of girls 0..10, background and text reset, banner title = festival name, HUD panels hidden, `culture_visitor_girl` = 0xFF, `culture_phase` += 1 |
| 1 | `FADE_RESET` | brightness 0, next |
| 2 | `GATE_LOAD` | when the bank wait is done (`snd_bank_load_poll() == 1`): sound command 0x200, read the festival background (sector 0x4029), next |
| 3 | `GATE_SHOW` | when the read is done: show the background in layer 1, read the intro script (5 sectors at 0x49BA to 0x801E8000), banner on, next |
| 4 | `GATE_FADE_IN` | `normal_date_bg_fadein`; `culture_phase_next_on_step` leaves when the fade moved the step |
| 5 | `INTRO_SETUP` | when the read is done: text marker hidden, speaker names for girl 0, script set `culture_script_set_intro`, `dialogue_group` 0, line 0, tables stored, next |
| 6 | `INTRO_TALK` | `normal_date_speak` (the festival intro lines), leaves like phase 4 |
| 7 | `STAGES` | `culture_stage_dispatch`: calls the stage function of `GameState.stage` (section 5) |
| 8 | `FINISH` | see below |
| 9 | `BIRTHDAY` | `culture_birthday_scene` (section 7.5) |
| other | | `game_reset_to_title()` |

Phase 8 (`FINISH`), reached when a scene calls `culture_scene_end` (stage 0, step 0, phase 8):

1. If `girls[0].birthday` month and day equal today and bit 0 of `daily_event_bits` is clear: stage and step to 0, phase 9 (the birthday scene, once; the scene sets bit 0 itself).
2. Else if stage is 0: sound command 0xBF, `day_bgm_load()`, `main_stage_next()` (stage 1).
3. Else when `snd_bank_load_poll() == 1`: `culture_result_apply` (section 8.1), `culture_event_log_add` (8.2), clear the 64-word scratch `overlay_scratch`, `dialogue_clear`, `dec_bg_reset`, brightness 0, `day_event_next(1)`.

## 5. Stage machine (`GameState.stage` while phase 7)

`culture_stage_dispatch` copies `culture_stage_table` to the stack and calls entry `[stage]`. Each stage function is itself a switch on `GameState.step`; stages move with `main_stage_set(n)` and steps with `main_step_next` and `main_step_set`. The 165 stages are 15 blocks plus a few single stages. `CultureStage` constants name the block entries.

| Stages | Block |
|---|---|
| 0 | `culture_stage_intro` |
| 1..13 | Shiori member block (entry 1, fortune stage 7) |
| 14 | `culture_visitor_pick` |
| 15..27, 28..41 | visit blocks of girls 0 and 1 (entries 15 and 28; fortune stages 21 and 36) |
| 42..45 | Mio member block |
| 46..58 | Yuina visit block (fortune 52) |
| 59..64 | Yuina member block (fortune 62) |
| 65..79 | Ayako visit block (fortune 71; 78 is an empty stub nothing selects; 79 the follow-up scene) |
| 80..84 | Ayako member block |
| 85..96, 97..108, 109..120, 121..132, 133..144 | visit blocks of girls 4 to 8 (entries 85, 97, 109, 121, 133; fortune stages 91, 103, 115, 127, 139) |
| 145..153, 154..162 | visit blocks of girls 9 and 10 (no fortune stage) |
| 163 | `culture_stage_game_resume` (section 7.4) |
| 164 | `culture_stage_game_launch` (section 7.4) |

### 5.1 Stage 0: `culture_stage_intro`

Step 0: `dialogue_group = club * 3 + (year - 95) + 18` (groups 18..35: one narration per club and year; `club` = `player_flags` bits 12-15), `main_step_next`. Step 1: `normal_date_speak` plays it. Step 2 stores the club in `culture_exhibit_club` and picks the first block (rules checked in this order, with c = the club):

| Player's club c | Next stage |
|---|---|
| any of 0..5 | if `girls[0].club == c`: stage 1 (Shiori member block). Else the club mate of c (clubs 0,1 -> girl 1; clubs 2,3 -> girl 2; clubs 4,5 -> girl 3): if `girls[mate].club == c` and she is met: stage 42 (Mio), 59 (Yuina) or 80 (Ayako). Else stage 14 (random visitor) |

`CLUB_NONE` and clubs above 5 hit `game_reset_to_title`. Emulator check: club 0 with girl 0 in club 0 goes 0 -> 1 -> 2; club 0 with girl 1 in club 0 and met goes 0 -> 0x2A; club 3 with no member goes 0 -> 0xE -> 0xF.

### 5.2 Stage 14: `culture_visitor_pick`

Seeds `srand` with `GameState.timer_ticks`, collects the ids of heroines 0..10 with the met bit (girl 0 always qualifies), picks `rand() % count`, stores her in `culture_visitor_girl`, and jumps to her visit block entry: girl 0 -> 15, 1 -> 28, 2 -> 46, 3 -> 65, 4 -> 85, 5 -> 97, 6 -> 109, 7 -> 121, 8 -> 133, 9 -> 145, 10 -> 154.

### 5.3 Block entries and the (club, year) -> exhibit table

Each block entry function stores its girl id in `culture_visitor_girl` (the member block entries also set bit 2 of that girl's flags), then maps the player's club and the year to the stage of the exhibit. The exhibits (banner titles; the banner comes from `culture_exhibit_title_show` by club and year):

| Club | Year 95 | Year 96 | Year 97 |
|---|---|---|---|
| 0 literature | poem collection exhibit | anthology exhibit | speech contest |
| 1 drama | drama club play (all years) | | |
| 2 science | experiment show (all years) | | |
| 3 computer | computer fortune telling | game exhibit | experiment show |
| 4 art | portrait drawing | public sketching session | oil painting exhibit |
| 5 brass band | concert (all years) | | |

Stage numbers per block (column: exhibit; `-` = the block has no such stage; shared = the stage also serves the next column):

| Block (entry) | Literature 95/96 | Speech 97 | Drama | Science | Computer 95 | Computer 96 | Computer 97 | Art 95 | Art 96 | Art 97 | Brass | Fortune stage |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| shiori visit (15) | 16 | 17 | 18 | 19 | 20 | 22 | 23 | 24 | 25 | 26 | 27 | 21 |
| mio visit (28) | 29 | 30 | 31 | 32/33/34 by year | 35 | 37 | 34 (shared with science 97) | 38 | 39 | 40 | 41 | 36 |
| yuina visit (46) | 47 | 48 | 49 | 50 | 51 | 53 | 54 | 55 | 56 | 57 | 58 | 52 |
| ayako visit (65) | 66 | 67 | 68 | 69 | 70 | 72 | 73 | 74 | 75 | 76 | 77 | 71 |
| saki visit (85) | 86 | 87 | 88 | 89 | 90 | 92 | 89 (shared) | 93 | 94 | 95 | 96 | 91 |
| yukari visit (97) | 98 | 99 | 100 | 101 | 102 | 104 | 101 | 105 | 106 | 107 | 108 | 103 |
| nozomi visit (109) | 110 | 111 | 112 | 113 | 114 | 116 | 113 | 117 | 118 | 119 | 120 | 115 |
| mira visit (121) | 122 | 123 | 124 | 125 | 126 | 128 | 125 | 129 | 130 | 131 | 132 | 127 |
| yuko visit (133) | 134 | 135 | 136 | 137 | 138 | 140 | 137 | 141 | 142 | 143 | 144 | 139 |
| megumi visit (145) | 146 (96 only) | 147 (other years) | 148 | 149 (also computer in years other than 96) | - | 150 | 149 | - | 151 (96 only) | 152 (other years) | 153 | - |
| yumi visit (154) | 155 (96 only) | 156 (other years) | 157 | 158 (also computer other than 96) | - | 159 | 158 | - | 160 (96 only) | 161 (other years) | 162 | - |
| shiori member (1) | 2 | 3 | 4 | 5 | 6 | 8 | 9 | 10 | 11 | 12 | 13 | 7 |
| mio member (42) | 43 | 44 | 45 | - | - | - | - | - | - | - | - | - |
| yuina member (59) | - | - | - | 60 | 61 | 63 | 64 | - | - | - | - | 62 |
| ayako member (80) | - | - | - | - | - | - | - | 81 | 82 | 83 | 84 | - |

The member blocks exist only for the clubs the girl can be in (Mio: literature and drama; Yuina: science and computer; Ayako: art and brass band). A club-mate block is entered only from stage 0. The names in the table are `culture_<girl>_<visit|member>_<exhibit>`, where the exhibit words are `literature_exhibit`, `speech_contest`, `drama_play`, `experiment_show`, `computer_fortune`, `computer_game`, `computer_experiment_show`, `portrait_drawing`, `open_sketching`, `oil_painting_exhibit`, `concert`.

Megumi and Yumi have no 1995 scenes (their gain-table rows are zero for 95 and the entries test only `year == 96`, sending every other year to the later scene). HYPOTHESIS: they are not met before 1996.

## 6. The visit scene pattern (about 150 functions)

Every visit or member scene is one function with 15 to 30 steps. The steps are the same few operations in a fixed order (step numbers vary by 1 or 2 between exhibits):

1. Read the exhibit background (`dec_bg_cd_read(sector, 1)`; sector by exhibit: 0x40EA literature exhibit, 0x40F4 speech contest, drama play and concert, 0x40FC science, 0x4105 computer, 0x410F art 95 and 97, 0x4118 art 96); fade the previous picture out; when the read is done show the new background in layer 1.
2. Set the banner title (`culture_exhibit_title_show`, or `culture_backstage_title_show` after a show), start reading the girl's portrait (BUSTUP sector in the core table of `bustup_read_start`: 0x544A Shiori, 0x5D41 Mio, 0x522E Yuina, 0x58AF Ayako, 0x66BF Saki, 0x5FB7 Yukari, 0x622D Nozomi, 0x5B25 Mira, 0x5012 Yuko, 0x64D0 Megumi, 0x6935 Yumi), fade the background in, `addr_init_bustup`, set `GameState.current_girl` to the girl, show the portrait.
3. Opening narration (visit blocks only; the member blocks start with reaction groups): `dialogue_group` = one narration group by exhibit, then `normal_date_speak`. Groups: 0x24 (literature 95) or 0x25 (96), 0x26 speech contest, 0x27 (drama 95) or 0x28 (96, 97), 0x29 science (and computer 97), 0x2A computer 95 and 96, 0x2B, 0x2C, 0x2D art 95, 96, 97, 0x2E (brass 95) or 0x2F (96, 97).
4. Read the girl's festival script file into 0x801E8000 (sector counts and sectors in section 9.2), girl enters (`normal_date_girl_in`), choose the first reaction group from her mood and call the block's `culture_<girl>_script_start` function, which sets the speaker names, selects the script set with the matching `culture_script_set_*` setter and loads the three table pointers, line 0.
5. The reaction: `normal_date_speak`, then further groups chosen again by `girl_state_rank(girl)`: ranks 0 and 1 share the best group, rank 2 and 3 get the next ones, rank 4 (the lowest mood) usually gets a short variant and the girl does not enter or leaves early (the picture is faded with `normal_date_bg_fadein` instead of `normal_date_bggirl_fadein`). A club mate's scene adds a comparison of `clubs[c].exp` with a threshold (section 8.3).
6. Optional sub-scene: the stage show of BUNKA_SD (7.2), the fortune stage (7.3) or the TT game (7.4); each saves a return point and comes back by `main_stage_set` and `main_step_set`.
7. Closing: girl leaves (`normal_date_girl_out`), the picture fades out, a last group (1, 2 or 3 and 0x76 by rank, then group 0), and in the literature scenes the festival-call flags (8.4). The scene ends with `culture_scene_end`, which jumps to phase 8.

`dialogue_run` increments `dialogue_group` by one when a group ends (seen in the emulator trace: after the intro group 0 the value is 1), but every speak step sets its group explicitly first.

Worked example, `culture_shiori_visit_literature_exhibit` (stage 16, 18 steps; `rank` = `girl_state_rank(0)`):

| Step | Does |
|---|---|
| 0 | read background 0x40EA, next |
| 1 | fade the old picture out |
| 2 | when the read is done: banner title for the exhibit, show the background in layer 1, start reading portrait 0x544A, next |
| 3 | fade the background in |
| 4 | when the portrait is read: `addr_init_bustup`, `current_girl` = 0, show portrait; group 0x24 (year 95) or 0x25 |
| 5 | speak (narration) |
| 6 | read the girl script (0x11 sectors from 0x49BF), next |
| 7 | girl enters |
| 8 | when the read is done: group 0xA (rank 0 or 1), 0xB, 0xC, 0xD (rank 4), plus 4 in year 96; `culture_shiori_visit_script_start` |
| 9 | speak |
| 10 | rank 4: jump to step 0xC; else group 1 (rank 0 or 1), 2 or 3; next |
| 11 | speak |
| 12 | group 0x90; girl leaves |
| 13 | rank 4: speak; else next |
| 14 | fade the picture out |
| 15 | group 0, next |
| 16 | speak |
| 17 | if rank < 4: `festival_call_95` (year 95) or `festival_call_96` on girl 0; `culture_scene_end` |

Any step number not listed in a switch falls to `game_reset_to_title()`.

Differences by exhibit (all other steps follow the pattern):

- **Literature exhibit**: the example above. In the Shiori and Mio member blocks the closing group also depends on `clubs[0].exp` (section 8.3); Yuina has no member scene for it.
- **Stage-show scenes (speech contest, drama play, science show, computer experiment show, concert)**: the stage show of BUNKA_SD (section 7.2) is started in the middle of the scene: after the opening narration (speech contest, drama play, concert: the girl is not on screen yet) or after a first reaction round (science show, computer experiment show). The return point is stored first. When the overlay is back the scene shows the backstage banner, reads the portrait and the girl's script again (the overlay was replaced, so `culture_<girl>_script_start` sets the pointers again), fades the background and the girl in together and plays the post-show groups by rank (in the Shiori speech contest: 0x12..0x15, then 4, 5, 2, 3, 7 by rank 0..4); the Shiori concert uses 4 and 5 (0x85 and 0x86 in year 95) for rank 0 and 1, 2, 3 and 7.
- **Computer fortune telling (year 95)**: opening group 0x2A, the girl enters, a first reaction group by rank, then a hand-over to the shared fortune stage (`main_stage_next` with `culture_return_stage/step` set and sound command 0x202); on return the girl enters again (skipped for rank 4) and the reaction group is chosen by rank plus `culture_fortune_result`.
- **Game exhibit (year 96)**: opening group 0x2A, a reaction group by rank, the girl leaves (rank 4 is skipped), then the TT launch of section 7.4; after the return the scene reloads the background and portrait, the girl enters (the background alone for rank 4) and a rank group follows, increased by one when `tt_stages_cleared` is below the girl's threshold.
- **Art scenes (portrait drawing, sketching session, oil painting)**: no show and no sub-scene; two or three rounds of conversation with rank groups and the `clubs[4].exp` tests of section 8.3 (the Yumi sketching scene adds Yoshio as a second speaker).
- **Rank 4**: usually shortens the scene (the girl does not enter, or leaves at once, and the closing group differs) and suppresses the festival-call flags.

Two quirks the port must keep: the rank-4 variants skip portrait fades, and the Yumi sketching scene (stage 160) shows two girls (Yumi and girl 13, Yoshio) in turn by copying portrait data between VRAM-side buffers (`memcpy` to 0x80197000 and 0x801AE000) and switching `current_girl`; Yoshio's lines use `scene_mouth_lock`.

## 7. Special scenes

### 7.1 The Ayako follow-up (stage 79)

Reached from Ayako's speech contest scene (stage 67, step 13) and, in 1997, from her drama scene (stage 68, step 15, only when her rank is not 4). The scene picks the group from her rank (0/1, 2, 3 -> 0x50, 0x52, 0x54), adds 0x18 when the low two bits of her `syoushin` are clear, else +6 once (and sets her bit 2) and +0xC when her club is the brass band, then talks, and when the player's club is literature and her rank is not 4 sets her festival-call bit (b12 in year 95, b13 in year 96).

### 7.2 Stage shows (BUNKA_SD, mode 0x80)

Five exhibits call `main_state_set(0x80)` after storing `culture_return_stage/step`: speech contest (literature 97), drama play, science experiment show, computer experiment show (97) and the brass concert, in the visit blocks and in the Shiori, Mio, Yuina and Ayako member blocks (the `plays the stage show` rows of the table). BUNKA_SD reads `culture_exhibit_club` and the year to choose its stage: club 2 -> stages 0xF0/F1/F2 by year, club 3 -> 0xF3, club 5 -> 0xF4, club 0 -> 0xF5, club 1 -> 0xF6/F7/F8 by year; at its stage 0x10 it sets main mode 0x70 or 0x71 by the player's club and stage and step from `culture_return_stage/step`. The variant in `culture_show_variant` selects the cast: BUNKA_SD sets `current_girl` to 14 for variant 1, 0 for 2, 1 for 3 and 11 for 4 and above (6 included), and nothing for 0. BUNKASAI writes 0 (visit blocks: speech, science and computer 97 shows), 2 (visit-block concerts; Shiori member speech, science and computer 97 shows), 3 (Mio member speech; Yuina member science and computer 97 shows; Shiori member concert) or 6 (Ayako member concert). The drama scenes do not write the variant and keep the value left by the previous scene (0 after a fresh boot). Emulator check: Mio member, literature club, 1997: stage 0x2A -> 0x2C, variant 3, return point (0x2C, 9) stored, BUNKA_SD runs stages 0, 1, 0xF5, 0x10 and the loader returns to mode 0x70 stage 0x2C step 9.

### 7.3 Computer fortune telling (`culture_fortune_scene`, stages 7, 21, 36, 52, 62, 71, 91, 103, 115, 127, 139)

One shared function, entered from the year-95 computer club scenes of the same block by `main_stage_next` with the return point saved (the caller's stage and step + 1). Steps: 0 read one sector (0x4B80) of script; 1 set the fortune script set; a branch picks the target girl: in this overlay the target is always `culture_visitor_girl` (the branch with `select_girl2` is dead here: it needs `culture_visitor_girl` = 0xFF or a non-computer player club, and the fortune is only reached from the computer club; BUNKAKEN uses the same code); 5 speak; 6..19 draw a fake computer printout with typed strings (a header line, the girl's short name and her name in kanji from `culture_fortune_girl_names`, her blood type label from `culture_fortune_girl_blood_labels` or, for girl 0, `culture_blood_type_labels`, then the player's name and blood type) with sound command 0x50A before each of the two blocks and 0x15-frame waits (`culture_fortune_timer`); 20 confirm wait (a face button, after the typewriter finished); 21 speak; 22 sound command 0x508; 23 wait 0x3D frames, then result = `culture_fortune_lookup()`, `dialogue_group = result + 3`, `culture_fortune_result = result`, sound command 0x50C; 24 speak; 25 return to the saved stage and step.

`culture_fortune_lookup`: blood type ids are the 2-bit value of `BirthdayBits.blood_type` of `girls[target].birthday` and `player_birthday`; the table is read as `compat[girl blood][player blood]`: 2 for equal types, 0 for the pairs (0, 1) and (2, 3), 1 for every other pair; id order O, A, B, AB follows the label tables. Special case: when the target is girl 2 (Yuina), Yuina is in the computer club and the player's club is not 3, the result is 3. The caller adds `culture_fortune_result` to its reaction group. Emulator check (three blood-type pairs): result 2 (group 5), 0 (group 3) and 1 (group 4) as predicted.

### 7.4 The game exhibit: TT (computer club, 1996)

Thirteen scene functions are "computer game" scenes (one per block that has the 1996 computer exhibit: stages 8, 22, 37, 53, 63, 72, 92, 104, 116, 128, 140, 150, 159). In 4 of them (Yukari 104, Nozomi 116, Yuko 140, Yumi 159) the scene first starts TT directly: `tt_mode_flags` bit 1 (direct mode) set and a stage in bits 2-4 (Yukari 1, Nozomi 2, Yumi 3, Yuko 4), `saved_mode/stage/step` = current mode, stage, step + 1, `main_state_set(0xD0)`; TT plays that stage from its recorded input. After TT the overlay reloads at the saved step, which jumps (`culture_default_disp_off` = 1, return point stored) to stage 163 (`culture_stage_game_resume`): it clears the actors and the texture-page slots (`vram_slots_clear(0xFF)`, a main helper this table names: it clears the 64 VRAM blocks of 64 x 128 pixels), reloads the boot data and the HUD textures (`boot_data_load_step`, `boot_hud_textures_step`), the debug font, 0x95 sectors at 0x9B94 to 0x801A0000 and the month calendar images, re-creates the cursor, sets `season_palette_fade` to 0xFE, and jumps back with `main_stage_set(return stage)` and `main_step_set(return step)`. Every one of the 13 scenes also has the second launch: bit 1 set with stage field 0, saved point, `main_stage_set(164)` -> `culture_stage_game_launch`: a narrator line (the title "voice from above" and a blank speaker label on slot 8), then step 4 starts `main_state_set(0xD0)` only while `GameState.input_device` is 1 (pad); with the mouse (0) the step does not advance (HYPOTHESIS: the line asks the player to use a controller). After TT the scene reloads the same way and plays a reaction group chosen by rank and by `tt_stages_cleared`: the group is increased by 1 when the stages-cleared byte is below a threshold (Shiori member and visit 2, Ayako 2, Saki 2, Mira 2, Nozomi 2, Yumi 3, Yuko 4; Yukari, Mio, Yuina and Megumi do not test it). The scene ends by setting `tt_mode_flags` bit 0 (`tt_unlocked`), which makes the OPTION extras menu list the TT game ([[spec/o-tt]]). Emulator check (computer club, 1996, only girl 0 met, pad input, 30000 frames): stage 22 step 15 stores the saved point (mode 0x70, stage 0x16, step 0x10) and sets stage 164 (`tt_mode_flags` = 2: direct mode); the narrator stage runs its dialogue; TT (mode 0xD0) starts with `tt_mode_flags` = 3 (bit 0 set by the TT loader), runs the demo for about 1,800 frames and leaves through the loader; mode 0x70 comes back at stage 0x16 step 0x10, which jumps to stage 163 with return point (0x16, 0x11); the scene continues, `tt_stages_cleared` is still 0 and the reaction group is the base group plus 1, and phase 8 follows.

### 7.5 Girl 0's birthday scene (phase 9, `culture_birthday_scene`)

Phase 8 starts it when today is the birthday stored in `girls[0].birthday` (month bits 0-3, day bits 4-8) and `daily_event_bits` bit 0 is clear. The scene has 65 steps in two parallel tracks:

- Step 0: set bit 0 of `daily_event_bits`, sound command 0xCF, start the frame timer, read the birthday script (3 sectors, 0x49B7). Step 1 (after 0x15 frames and the read): sound bank, girl 0, script set `culture_script_set_birthday`, group 0. Step 2: if the player's birthday is also today, girl 0's rank is below 2 and `culture_visitor_girl` is 0: jump to step 0x24; else clear the background and go on.
- Main track: steps 3-4 a two-way question (on screen: give a present or not; emulator screenshot). Choosing the second option goes through step 5 to step 6: `girls[0].affection -= 1`, `syoushin += 5`, end. Choosing the first goes to step 7: wait for the bank, then log an event-log entry by Shiori's club and year (scene 0x14, flag 0x1B literature (95, 96) or 0x1C (97), 0x1D drama, 0x1E science, 0x1F computer, 0x20 art (95, 97) or 0x21 (96), 0x22 brass band) and read the matching background (0x40EA, 0x40F4, 0x40FC, 0x4105, 0x410F, 0x4118; 0x403B if she is in no culture club, without a log entry), group 2. Step 9 selects Shiori's portrait by her club (0x5477 drama, 0x54D1 science, 0x54A4 art, else 0x544A). Steps A-E: group `year - 92` (3, 4, 5), three-way choice (step 0xC), `culture_birthday_choice_name_set` copies the option text for (year, choice) into name code A, group 6. Steps 0x10-0x1B: groups 0xA/0xB (player's club equals Shiori's or not), 0xE/0xD, the reaction groups 0x12..0x15 and 0x17..0x1A by rank, and 0x16 only when the clubs are the same. Step 0x1C applies the reward by the choice index (the option index of the three-way menu, whatever its display order): option 0: group 0x1B, portrait layout 1, tokimeki +3, affection +2, `syoushin` -15; option 1: group 0x1D, layout 2, tokimeki +2, affection +1, `syoushin` -10; option 2: group 0x1F, layout 3, affection +1, `syoushin` +10. Steps 0x1D-0x23: speak, girl out, speak, fade out, group 0x24, speak, `culture_scene_end`. Emulator check: the path 0..5, 7..0x15, 0x17, 0x18, 0x1A..0x23 with the option picked (third option: affection +1, `syoushin` +10, then +2 affection from the result step).
- Second track, steps 0x24-0x40 (entered from step 2: the player's birthday is today too): `daily_event_bits |= 4` (the player's birthday record is marked done), background fade in, and if the clubs differ group 0x22 then step 0x3E (girl in, speak, group 0x26, step 0x2E), else group 0x2A, girl in, speak, then read the extra file (3 sectors at 0xAF43 to 0x801B0000), `culture_birthday_tables_init`, group 1 through the second entries of the 13-entry tables, speak, reload the birthday script, group 0x26, speak, group 0x29, speak; group `year - 92`, the three-way choice, group 0x23, speak, the same reward switch as step 0x1C (step 0x36), girl out, speak, fade out, group 0x24, `culture_scene_end`.

HYPOTHESIS: the three options are presents (the earlier two-way question asks whether to give one); the code only shows that the option text of the year is copied into a name code.

## 8. Rules

### 8.1 The festival result (`culture_result_apply`, phase 8)

Inputs: the player's club c (0..5), the year y (95..97), the visitor girl v = `culture_visitor_girl`.

```
bonus = (clubs[c].exp >> 3) / (y - 94)  +  param[culture_club_param_index[c]].value / 20      (unsigned, low byte)
bonus = min(bonus, 10)
for i in 0..12:                                   // all 13 girl records
    girls[i].tokimeki += bonus + (gain[y][i][c] >> 1)
girls[v].affection += 2
amount = gain[y][v][c];  if amount != 0: amount -= 1
girls[v].tokimeki += amount
```

`param[]` is the player parameter block read from `param_condition`: index 1 liberal arts (clubs 0, 1), 2 science (clubs 2, 3), 3 art (clubs 4, 5). The gain tables hold 13 rows (girl id) by 6 columns (club). Years: 95, 96 and 97 select the three tables; any other year leaves `amount` unset (the festival happens in these three years only). Emulator check (club 0, year 95, exp 16, liberal arts 40, all tokimeki 10): every girl gets +4, girls 1, 4 and 5 get +5, girl 0 (visitor) +4 and affection +2, girls 11 and 12 +4.

Gain tables (rows girls, columns clubs 0 literature, 1 drama, 2 science, 3 computer, 4 art, 5 brass band):

| Girl | 95 | 96 | 97 |
|---|---|---|---|
| 0 shiori | 1 2 2 2 2 2 | 1 1 3 0 0 1 | 2 3 2 2 3 3 |
| 1 mio | 3 2 1 2 0 2 | 3 2 2 0 0 1 | 3 3 1 1 1 3 |
| 2 yuina | 0 1 2 1 2 1 | 0 0 1 1 0 0 | 0 0 2 2 3 3 |
| 3 ayako | 0 2 1 1 3 2 | 1 1 1 1 2 3 | 2 2 3 3 3 3 |
| 4 saki | 2 1 1 1 1 1 | 2 1 1 1 0 1 | 2 1 3 3 2 1 |
| 5 yukari | 2 2 1 1 1 1 | 2 1 2 1 1 1 | 1 2 1 1 2 1 |
| 6 nozomi | 1 1 0 3 2 1 | 1 0 3 2 0 1 | 3 3 2 2 1 2 |
| 7 mira | 1 1 0 1 3 2 | 1 1 3 0 0 0 | 1 3 2 2 2 1 |
| 8 yuko | 0 3 2 1 2 2 | 0 0 1 2 0 2 | 0 1 3 3 1 0 |
| 9 megumi | 0 0 0 0 0 0 | 2 1 3 1 0 0 | 2 2 2 2 2 2 |
| 10 yumi | 0 0 0 0 0 0 | 0 3 2 3 0 2 | 2 0 3 3 0 0 |
| 11 miharu, 12 rei | all 0 | all 0 | all 0 |

### 8.2 The event log entry (`culture_event_log_add`)

Appends an `EventLogEntry` (girl = `culture_visitor_girl`, scene 20, flag, rank = `get_g_zyotai_h(girl)`, then year, month, day, weekday), advancing `event_log_count` and capping it at 0xFE. The flag is `4 * club + 1` for the club, plus 1 when girl 0 is in that club or the club mate (girl 1 for clubs 0 and 1, girl 2 for 2 and 3, girl 3 for 4 and 5) is in the club and met. Quirk: when the club is 4 (art), the visitor is girl 10, her `girl_state_rank` is 4 and neither condition holds, the function returns before writing the entry. The album and ending read this log (core).

### 8.3 Club-points thresholds in the member scenes

The scenes of a club mate (and Shiori's own) compare the club's `exp` word: literature (club 0) and drama (club 1) scenes: `exp >= (year - 94) * 100` picks the better of two closing groups (95: 100, 96: 200, 97: 300; stage 45 also tests 300 in 1997); brass band (club 5) scenes: `exp < (year - 94) * 100` adds 1; every portrait drawing scene (art 95, visit and member blocks) tests `clubs[4].exp < 100` (adds 1 to the group); the oil painting scenes of the Shiori and Ayako member blocks (art 97) test `clubs[4].exp < 300`; Shiori's science scene tests `clubs[2].exp < 100` together with `girl_state_rank(0) >= 2`. These tests pick between two dialogue groups; they change no stat.

### 8.4 Flags set by the scenes

- `girls[g].flags` bit 2 (`acquainted`, m-scheduler's HYPOTHESIS name) is set by the entry of a member block (girls 0, 1, 2, 3).
- Bit 12 (`festival_call_95`) in year 95 and bit 13 (`festival_call_96`) otherwise are set at the end of the literature exhibit scene of each visit block (and of the Ayako follow-up) when the girl's rank is not 4: girls 0, 2..9 and, in year 96 only, girl 10. Girl 1's scene and the member blocks do not set them. The scheduler turns them into the phone calls of the festival Sundays ([[spec/m-scheduler]]).
- `tt_mode_flags` bit 0 at the end of every game exhibit; `daily_event_bits` bit 0 and bit 2 in the birthday scene.
- `culture_visitor_girl`, `culture_exhibit_club`, `culture_show_variant`, `culture_return_*` as in section 3.1.

## 9. Data tables

### 9.1 Script sets (22 setters)

Each `culture_script_set_*` writes three RAM addresses (text, command and voice tables of a script file already read to 0x801E8000..) into `culture_script_*_table`; the callers copy them into the dialogue words. The sets: festival intro (`culture_script_set_intro`, text table at 0x801E92FC), narrator (0x801E81BC), fortune (0x801E81CC), birthday (0x801E8A10), and 18 girl scripts: Shiori visit and member, Mio visit (two files) and member, Yuina visit and member, Ayako visit (three files) and member, and one each for Saki, Yukari, Nozomi, Mira, Yuko, Megumi and Yumi. The `culture_<girl>_*_script_start` functions combine `dialogue_names_set(girl)`, the setter, line 0 and the tables, then `main_step_next`.

### 9.2 Per-girl assets (sector counts and disc sectors as the code passes them)

| Girl | Portrait sector(s) | Visit script (sectors @ sector) | Member script |
|---|---|---|---|
| 0 shiori | 0x544A; member block adds 0x5477 (drama), 0x54D1 (science), 0x54A4 (art) | 17 @ 0x49BF | 24 @ 0x49D0 |
| 1 mio | 0x5D41, 0x5D6E | 20 @ 0x4A83 (stages 29-34), 18 @ 0x4A97 (35-41) | 11 @ 0x4AA9 |
| 2 yuina | 0x522E | 26 @ 0x4A12 | 10 @ 0x4A2C, portrait 0x525B in stage 60 |
| 3 ayako | 0x58AF | 25 @ 0x4ABD (66-68), 23 @ 0x4AD6 (69-73), 15 @ 0x4AED (74-77) | 12 @ 0x4AFC, portrait 0x58DC |
| 4 saki | 0x66BF | 10 @ 0x4B47 | |
| 5 yukari | 0x5FB7 | 13 @ 0x4A65 | |
| 6 nozomi | 0x622D | 15 @ 0x4B13 | |
| 7 mira | 0x5B25 | 15 @ 0x4A41 | |
| 8 yuko | 0x5012 | 20 @ 0x4991 | |
| 9 megumi | 0x64D0 | 10 @ 0x4B33 | |
| 10 yumi | 0x6935 (and 0x6C5F, girl 13 Yoshio, in stage 160) | 19 @ 0x4B5E | |

Other disc reads: gate background 0x4029, intro script 5 @ 0x49BA, birthday script 3 @ 0x49B7, birthday extra file 3 @ 0xAF43 (to 0x801B0000), narrator script 1 @ 0x4B7F, fortune script 1 @ 0x4B80, stage 163 reload 0x95 @ 0x9B94 (to 0x801A0000).

## 10. Connections

- Scheduler: [[spec/m-scheduler]] queues type 0x24 on the day with calendar marker 12 (the call dates in its spec suggest October) and clears the girl bit 0 and `overlay_scratch` first; it also sets the festival-call phone stages from the flags of 8.4.
- Main scene helpers used by every scene: `normal_date_speak`, `normal_date_girl_in/out`, `normal_date_bg_fadein/out`, `normal_date_bggirl_fadein/out`, the two- and three-way selects, `dec_bg_cd_read`, `dec_bg_show_set`, `bustup_*` ([[spec/m-dates-b]]); `girl_state_rank`, `get_g_zyotai_h`, `event_log_add` fields ([[spec/core]]).
- BUNKA_SD (stage shows), TT ([[spec/o-tt]], game exhibit and `tt_stages_cleared`), BUNKAKEN (the same variables and, by sibling code, the same fortune stage logic).
- OPTION: lists the TT game after bit 0 of `tt_mode_flags` is set.

## 11. Open questions

1. `D_80122EA4` (left unnamed), `culture_show_variant` and the other shared scene variables need BUNKA_SD's and BUNKAKEN's units to confirm names (shared bss); BUNKAKEN writes 0..4; the value 6 of the Ayako member concert falls into BUNKA_SD's default branch (the cast of variant 4).
2. Why Megumi's and Yumi's entries only test year 96 (HYPOTHESIS: not met in 1995); how `girls[0].birthday` is set (the scene tests its month and day against today; NAME_ENT sets her club from it).
3. The festival date itself: the scheduler places it by calendar marker 12; the days are in `month_data_table` (not decoded here).
4. Whether the mouse-only softlock at stage 164 step 4 is intended (the code only advances for the pad).
5. The meaning of the dialogue group numbers: they index script sections of the girl files; the stage C is the only place that maps rank, year and club to them.
6. Name proposals for main-bss variables used by three overlays (section 3.1) must be reconciled with BUNKAKEN and BUNKA_SD.

## 12. Function index

The old addresses of the framework and rule functions (the stage functions follow the scheme `culture_<girl>_<visit|member>_<exhibit>` of section 5.3; every old address and its new name is a row of the table):

| Old | New name | Role |
|---|---|---|
| `func_80142700` | `culture_festival_entry` | mode 0x70 entry, phase machine (section 4) |
| `func_80142B98` | `culture_phase_next_on_step` | leave phases 4 and 6 once the step moved |
| `func_80142BC8` | `culture_scene_end` | end a scene: stage 0, step 0, phase 8 (134 callers) |
| `func_80142BE8` | `culture_result_apply` | festival result (8.1) |
| `func_801431D4` | `culture_exhibit_title_show` | banner title by club and year |
| `func_8014338C` | `culture_backstage_title_show` | backstage banner (39 callers) |
| `func_801433C0` | `culture_event_log_add` | event log entry (8.2) |
| `func_80150930` | `culture_stage_dispatch` | stage table call |
| `func_80150B7C` | `culture_stage_intro` | stage 0 (5.1) |
| `func_801509A4` | `culture_visitor_pick` | stage 14 (5.2) |
| `func_80150F20`, `func_80151064` | `culture_stage_game_resume`, `culture_stage_game_launch` | stages 163 and 164 (7.4) |
| `func_8015AF90`, `func_8015B828` | `culture_fortune_scene`, `culture_fortune_lookup` | shared fortune stage and its table lookup (7.3) |
| `func_80146FA0`, `func_80147214`, `func_80147F8C` | `culture_birthday_tables_init`, `culture_birthday_scene`, `culture_birthday_choice_name_set` | birthday scene (7.5) |
| `func_80132000` .. `func_80132444` (22) | `culture_script_set_*` | script pointer setters (9.1) |
| 15 block entries | `culture_<girl>_<visit|member>_entry` | (club, year) -> stage tables of 5.3 |
| 19 start functions | `culture_<girl>_<visit|member>_script_[a-c_]start` | name codes, script set, line 0, tables, next step |
| `func_8013FA20`, `func_8013FA28` | `culture_ayako_visit_unused_stage`, `culture_ayako_visit_followup` | stages 78 and 79 (7.1) |
