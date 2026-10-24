---
type: spec
updated: 2026-10-24
unit: BUNKAKEN
ticket: T-27026
sources: ["src/ovl/BUNKAKEN/", "include/ovl/BUNKAKEN.h", "config/renames/proposals/o-bunkaken.tsv", "src/main/8006CB30.c", "src/ovl/BUNKA_SD/80137890.c", "src/ovl/BUNKASAI/80142700.c"]
---

# Overlay BUNKAKEN: the culture festival, visitor variant (port spec)

Phase 2 spec for the Godot port ([[tickets/T-27026-p2-o-bunkaken]], parent [[tickets/T-27000-phase-2]]). Every statement is read from the matched C, the data tables of the overlay, the disc file names, or the emulator; "HYPOTHESIS" marks a reading of game meaning that the code does not prove. New names are in `config/renames/proposals/o-bunkaken.tsv` (457 rows). No game text is quoted: exhibits, speakers and scenes are described in English, and dialogue is only referred to by script file, group number and speak role.

Naming. Prefix: `festival_visit_` for overlay functions and data, `FestivalVisit` for types, `FV_` for constants. This is the English form of the overlay's job (the visitor variant of the culture festival); the audit's lower-case overlay name would be `bunkaken_`, but the abbreviation "bunkaken" is not explained by any string or file (HYPOTHESIS: "bunka" is "culture" and the rest is unknown), so the table does not use it. The sibling overlay BUNKASAI is the other variant of the same event. Girls are named by given name as in core's `GirlId` (`GIRL_SHIORI`, `GIRL_MIO`, ... `GIRL_YUMI`); the clubs are core's `Club` (`CLUB_LITERATURE` 0 .. `CLUB_BRASS_BAND` 5). `GameState` fields, main functions and pad bits use the names of `config/renames/proposals/core.tsv` (worktree `p2a-core`); section 9 maps the placeholder names of the matched C to them, so the spec still reads correctly after the core rename lands.

## 1. What it is

A walk through the school culture festival for a player who is **not** in a culture club. The festival day (one calendar day, three school years: 1995, 1996, 1997) is a scripted visit: the player arrives at the school gate, a girl may ask him to come along, he picks one of the six culture clubs (literature, drama, science, computer, art, brass band), and the overlay plays the exhibit of that club for the current year, alone or with a girl, sometimes handing over to a stage show (BUNKA_SD) or the TT shooter. At the end the girl who came along gets a small affection and tokimeki gain, a girl who was turned down loses affection, two birthday scenes can fire, and control returns to the day screen.

Everything the player sees is a scripted dialogue scene: the overlay holds no animation or rule engine of its own beyond the choice logic below. Scene text, voice references and speak codes live in disc script files loaded to 0x801E8000 (section 7); the overlay only decides **which group** of the loaded script to play and what to change in the game state.

Why two overlays (facts): the main queue reader starts mode 0x71 (this overlay) for a culture-festival day record (type 0x24) when the player's club id is 6 or more (the sports clubs and "no club"), and mode 0x70 (BUNKASAI) for club ids 0-5, the six culture clubs themselves. BUNKASAI is built around the player's own club: its point calculation indexes tables by the player's club and adds `clubs[club].exp >> 3` divided by the school year plus a parameter, and it sets the follow-up phone-call flags `festival_call_95/96` on girls. BUNKAKEN does neither: it has no club-dependent scoring, sets no call flag, and instead asks which club to visit. The scheduler treats the two groups differently around the event too: culture-club players get a preparation phone call on October 8, 6 and 5 of the three years (TEL stage 9, scheduler spec), sports-club players do not; the follow-up calls on October 15 and 13 come from the flags BUNKASAI sets. Both overlays end in the same way (`day_event_next`). HYPOTHESIS for the intent: a culture-club member presents his own club's work, everybody else tours the culture clubs; the code does not prove it.

Unit size: 18 C files, 201 functions (about 18,000 lines), load address 0x80132000, entry `festival_visit_entry` (0x80144F70). 142 of the functions are stage handlers in one table (section 5), 28 are 34-byte script setters, the rest are helpers.

## 2. Entry, trigger, exit

- Trigger: `day_event_dispatch` (main, `src/main/8006CB30.c`) reads the queue record of the day; type 0x24 (`DAY_EVENT_CULTURE_FESTIVAL`) clears overlay_scratch, runs `func_8006CF80` and calls `main_state_set(0x70)` when `player_flags.h >> 12` (the club id) is below 6, else `main_state_set(0x71)` (`MODE_CULTURE_FESTIVAL_B`). The test uses the club id (core `Club`; 11 is "no club"). The marker `MARKER_CULTURE_FESTIVAL` (core) creates the record.
- Entry: `festival_visit_entry` (mode 0x71) runs once per frame from the main mode dispatcher. First thing each frame: `bustup_wink_update` (portrait blinks); last: `Default_Disp` unless `festival_display_skip` is set.
- Exit: the finish phase (phase 8) ends with main `day_event_next(1)` (back to the day screen, mode 0x31), after zeroing the 0x100-byte `overlay_scratch`, clearing the dialogue and the background, `set_dec_bri(0)`.
- Sub-modes (section 5.6): `main_state_set(0x80)` BUNKA_SD stage show (returns by itself to mode 0x70 or 0x71 by the same club test, then restores stage and step from `festival_return_stage` and `festival_return_step`), and `main_state_set(0xD0)` TT (returns through `GameState.saved_mode/stage/step`).
- Mode byte, stage byte and step byte are `GameState.mode` (0x71), `stage` and `step` (core). In this overlay `stage` selects one of 142 scene handlers and `step` is the position inside the handler.

## 3. State

Shared festival words (main bss, inside the 0x100-byte `overlay_scratch` at 0x80122DD0..0x80122ED0; zeroed by the queue reader before the mode starts and again by the entry at the end of phase 8). BUNKASAI and BUNKA_SD use the same words; the table rows for them carry scope `main` and the note REQUEST (the main owner decides).

| Address (offset in overlay_scratch) | Proposed name | Meaning |
| --- | --- | --- |
| 0x80122E98 (+0xC8) | `festival_display_skip` | while nonzero the entry skips `Default_Disp`; 1 while a sub-mode is left and re-entered |
| 0x80122EA4 (+0xD4) | `festival_refused_girl` | girl the player turned down (0xFF none; set to 0xFF in phase 0) |
| 0x80122EA8 (+0xD8) | `festival_fortune_result` | fortune result 0-3 |
| 0x80122EAC (+0xDC) | `festival_show_variant` | variant 0-4 handed to BUNKA_SD |
| 0x80122EB0, EB4 (+0xE0, +0xE4) | `festival_return_stage`, `festival_return_step` | where to resume after a sub-mode |
| 0x80122EB8 (+0xE8) | `festival_club` | club id 0-5 being visited (first word of core's `CultureFestivalDispatch`, old `BunkaDispatch8`) |
| 0x80122EBC (+0xEC) | `festival_companion` | girl present: 0-10 heroine id, `FV_COMPANION_SHIORI_VISITOR` 0xB, `FV_COMPANION_NONE` 0xFF (second word of the dispatch pair) |
| 0x80122EC0 (+0xF0) | `festival_phase` | entry phase 0-10 (section 4) |

Other state used: `GameState.girls[i]` (`tokimeki`, `affection`, `syoushin`, `flags`: bit `met`, bit `acquainted` (core `CharFlags` bit 2), 4-bit `club`, `birthday`), `year` (0x5F, 0x60, 0x61 = 1995, 1996, 1997), `month`, `day`, `daily_event_bits`, `current_girl`, `player_flags` (club), the three-way and two-way `dialogue_choice`, the dialogue words (`dialogue_group`, `dialogue_line`, the three table pointers), `event_log`, `girl_menu_enabled`, the TT interface words: flag byte `D_800E7D4F` (bits `tt_unlocked`, `tt_direct`, field `tt_direct_stage`) and result byte `D_800E7D4E` (`tt_stages_cleared`).

Mood rank. `girl_state_rank(g)` (core, from `get_g_zyotai_s`) is 0 (best) to 4 (worst: weighted affection under 10, about "does not care"). Nearly every scene picks its script group from the rank: ranks 0 and 1 together, then 2, then 3 (three variants), and 4 means the girl is absent or refuses.

## 4. The entry phases (`festival_phase`)

One switch in `festival_visit_entry`; emulator order observed 0, 2, 3, 4, 6, 7 on a fresh start (phases 1 and 5 last one frame).

| Phase | Constant | Does |
| --- | --- | --- |
| 0 | `FV_PHASE_INIT` | `snd_command(SND_CMD_FADE_BOTH_FAST)` (0xBF), `snd_bank_load_start(1, ...)` for the festival sound set (SE bank SCLFESSE, sequence bank SCLFEST), `scene_screen_reset`, `girl_menu_enabled[i] = girls[i].flags.met` for i 0-10, `dialogue_clear`, `dec_bg_reset`, message window and title (festival banner, hidden), HUD parts off (mascot, date panel kept via `hizuke_init`, icons, parameters), refused girl and companion = 0xFF, display skip = 0, then the phase word is incremented through an aliasing store (marked FAKE in the C) |
| 1 | `FV_PHASE_FADE_RESET` | `set_dec_bri(0)`, next |
| 2 | `FV_PHASE_WAIT_SOUND` | when `snd_bank_load_poll() == 1`: `snd_command(0x200)` (BGM sequence 0 of the loaded set, the festival music), start the CD read of the gate background (BS file D_01_48) |
| 3 | `FV_PHASE_WAIT_BG` | when the read is done: decode and show the background, read the common script BUN_PC (5 sectors from 0x49BA to 0x801E8000), show the title |
| 4 | `FV_PHASE_FADE_IN` | `normal_date_bg_fadein`; `festival_visit_phase_next_on_step` moves on when a helper raised `step` |
| 5 | `FV_PHASE_WIRE_INTRO` | when the script is read: `text_marker_hide`, names for girl 0, `festival_visit_script_common` tables, group 0, line 0 |
| 6 | `FV_PHASE_INTRO_SPEAK` | `normal_date_speak` for group 0 (one narration line) then next phase |
| 7 | `FV_PHASE_STAGE` | `festival_visit_stage_run`: calls the handler of `GameState.stage` (starts at stage 0) |
| 8 | `FV_PHASE_FINISH` | see 4.1 |
| 9 | `FV_PHASE_SHIORI_BIRTHDAY` | `festival_visit_phase_shiori_birthday` (section 6.6) |
| 10 | `FV_PHASE_PLAYER_BIRTHDAY` | `festival_visit_phase_player_birthday` (section 6.6) |
| other | - | `game_reset_to_title` |

`festival_visit_finish_visit` (called by the last step of every exhibit) sets stage 0, step 0 and phase 8.

### 4.1 Finish phase

1. If girl 0 has her birthday today (`girls[0].birthday` month and day equal `month` and `day`), `daily_event_bits` has not got mask 1, and the companion is 0 or 0xB: phase 9, stage 0, step 0.
2. Else if the player has his birthday today (`player_birthday`), mask 4 of `daily_event_bits` is clear, the companion is 4-10 and her mood rank (`func_8008667C` = `girl_state_rank`) is below 2: phase 10, stage 0, step 0. (A second clause for companion 0xB is unreachable.)
3. Else if `stage == 0`: `snd_command(0xBF)`, main `day_bgm_load` (the day screen sound banks), `main_stage_next` (stage 1).
4. Else when `snd_bank_load_poll() == 1`: `festival_visit_apply_points` (6.2); if the companion is not 0xFF and her mood rank (read with the raw companion id) is below 4: `festival_visit_event_log_add` (6.7); zero the 64 words of `overlay_scratch`; `dialogue_clear`, `dec_bg_reset`, `set_dec_bri(0)`, `day_event_next(1)`.

Because the birthday phases run before steps 3 and 4 and then return to phase 8 through `festival_visit_finish_visit`, a birthday scene is followed by the normal finish. The emulator run with girl 0's birthday set to the calendar date confirmed the order: the birthday gains first, the visit gains after (6.6).

## 5. The stage machine

Flow in one picture:

```
phases 0-6 (arrive at the gate, intro line)
  -> phase 7, stage 0: a girl who likes the player enough invites him
       accept -> stage 3 (club menu), or her club (stage 4) for the four culture-club girls
       refuse, or nobody qualifies -> stage 1 (ask a girl; picker) -> stage 3 | stage 2 (Yoshio) -> stage 3
  stage 3 (pick 1 of 6 clubs) -> stage 4 (route by companion and club)
  -> exhibit stage 5-139 (one of 122 scenes, 13 more are routers) [-> stage show / TT / fortune telling and back]
  -> finish_visit -> phase 8 [-> phase 9 or 10: birthday scene] -> points, event log, day_event_next
```


`GameState.stage` indexes the 142-entry function table `festival_visit_stage_table` (`FV_STAGE_*`). Each handler is a `switch (GameState.step)` that does one frame of work and calls `main_step_next` (or `main_step_set`) when its step is done; steps that wait for a CD read test `cd_read_sync_status() & 1`. `main_stage_set(n)` changes stage and clears the step. All dialogue steps call `normal_date_speak`, which plays one group of the loaded script and ends the step at the group's end marker (main spec `m-dates-b`, section 7; this overlay reaches the script through `dialogue_group`, `dialogue_line` and the three table pointers).

### 5.1 Stage map

| Stages | Block (function prefix) | Content |
| --- | --- | --- |
| 0 | `stage_invitation` | the best-liked girl invites the player (5.2) |
| 1 | `stage_ask_companion` | no invitation or refused: ask a girl or not, pick her (5.3) |
| 2 | `stage_yoshio` | Yoshio (girl 13) greets the player (5.4) |
| 3 | `stage_club_menu` | choose one of six clubs (5.4) |
| 4 | `stage_route_visit` | route by companion and club (5.5) |
| 5-17 | `stage_solo_*` | exhibits visited alone: 5 route, 6-10, 12-17 exhibits, 11 fortune sub-stage |
| 18-31 | `stage_shiori_visitor_*` | companion id 0xB: Shiori whose club id is 6 or more; 18 route, 19-31 exhibits |
| 32-43 | `stage_shiori_club_*` | Shiori with a culture club, at her own club; 32 route, 33-43 |
| 44-47, 48-52, 53-57 | `stage_mio_club_*`, `stage_yuina_club_*`, `stage_ayako_club_*` | the other three club girls at their own clubs |
| 58-71, 72-84, 85-97, 98-110 | `stage_saki_*`, `yukari`, `nozomi`, `mira` | companion girls 4-7 |
| 111-121, 122-130, 131-139 | `stage_yuko_*`, `megumi`, `yumi` | companion girls 8-10 (Megumi and Yumi only in 1996 and 1997) |
| 140 | `stage_resume` | rebuild the system graphics after TT, then return |
| 141 | `stage_tt_narrator` | narrator scene, then the TT game |

Companion blocks are the same 13 exhibits (12 where the two brass variants merge); the exhibit of the six clubs by year:

| Club | 1995 | 1996 | 1997 |
| --- | --- | --- | --- |
| 0 literature | poem-collection exhibit | anthology exhibit | speech contest on the hall stage (stage show) |
| 1 drama | play on the hall stage (stage show), every year | | |
| 2 science | experiment show (stage show), every year | | |
| 3 computer | fortune telling by blood type (sub-stage 11) | game exhibit (TT) | experiment show (stage show) |
| 4 art | portrait drawing | open sketching | oil painting exhibit |
| 5 brass band | concert on the hall stage (stage show), every year | | |

The window title of each exhibit is set by `festival_visit_window_title_set` from the club and the year (the titles are the exhibit names above; the festival banner is the title in the phases).

Which stage a (block, club, year) pair gets is decided by the block's route function; the exceptions are the table below. "Same as" means the route function reuses a stage.

| Block | Route | Literature | Drama | Science | Computer 95 / 96 / 97 | Art 95 / 96 / 97 | Brass |
| --- | --- | --- | --- | --- | --- | --- | --- |
| solo | 5 | 6 (95, 96), 7 (97) | 8 | 9 | 10 / 12 / 13 | 14 / 15 / 16 | 17 |
| Shiori visitor | 18 | 19, 20, 21 | 22 | 23 | 24 / 25 / 26 | 27 / 28 / 29 | 30 (95, 96), 31 (97) |
| Shiori club (companion 0) | 32 | 33 (95, 96), 34 (97) | 35 | 36 | 37 / 38 / 39 | 40 / 41 / 42 | 43 |
| Mio club (1) | 44 | 45 (95, 96), 46 (97) | 47 | - | - | - | - |
| Yuina club (2) | 48 | - | - | 49 | 50 / 51 / 52 | - | - |
| Ayako club (3) | 53 | - | - | - | - | 54 / 55 / 56 | 57 |
| Saki (4) | 58 | 59, 60, 61 | 62 | 63 | 64 / 65 / 66 | 67 / 68 / 69 | 70 (95, 96), 71 (97) |
| Yukari (5), Nozomi (6), Mira (7) | 72, 85, 98 | +1, +2, +3 from the block base | +4 | +5 | +6 / +7 / +8 | +9 / +10 / +11 | +12 |
| Yuko (8) | 111 | 112 (95, 96), 113 (97) | 114 | 115 | 116 / 117 / 115 | 118 / 119 / 120 | 121 |
| Megumi (9) | 122 | - / 123 / 124 | 125 | 126 | - / 127 / 126 | - / 128 / 129 | 130 |
| Yumi (10) | 131 | - / 132 / 133 | 134 | 135 | - / 136 / 135 | - / 137 / 138 | 139 |

(Yukari: base 72, so literature 73-75; Nozomi base 85: 86-88; Mira base 98: 99-101; the same offsets give the rest.) In a few blocks the computer club's 1997 experiment show is the science stage itself (Yuko, Megumi, Yumi; the stage reads the lab background when `festival_club` is 2 and the computer-room one otherwise) because the two shows are the same show; in the other blocks it is a separate stage with the computer-room background. The Mio, Yuina and Ayako blocks exist only for the clubs the girl can be in (Mio: literature or drama; Yuina: science or computer; Ayako: art or brass band; core's new-game club assignment). Megumi and Yumi have no 1995 entries: Megumi's route function sends 1995 to `game_reset_to_title`, Yumi's treats every year but 1996 as 1997, and the 1995 points table has no rows for girls 9 and 10 (6.2); this suggests that the two girls cannot be companions in the first year (HYPOTHESIS: they are met only from 1996 on).

### 5.2 Stage 0: the invitation

`festival_visit_invitation_pick_girl` decides who invites, at the start of the stage (step 0):

- Candidates: girl 0 (Shiori) and girls 1-10 with `met` set. A girl qualifies when her weighted tokimeki (`get_h_tokimeki`) is at least `FV_INVITE_TOKIMEKI_MIN` (50) and her weighted affection (`get_h_yuukou`) is at least `FV_INVITE_AFFECTION_MIN` (80). Girl 0 additionally has to pass the affection test just to take part in the comparison.
- Score: raw affection + raw tokimeki (`girls[i].affection + girls[i].tokimeki`); the highest wins, ties go to the lower id (strictly greater replaces).
- In 1996 (`year == 0x60`) girls whose club is 4 (art) or 1 (drama), girl 0 included, score 0 and so cannot win, and girl 0 is not invited by the final rule either (HYPOTHESIS for the reason: those clubs are busy with their own show; the code only encodes the exclusion).
- The winner is checked once more against the 50 and 80 thresholds; if girl 0 wins and her club id is 6 or more the companion becomes 0xB (`FV_COMPANION_SHIORI_VISITOR`). The id is stored in `festival_companion`. No qualifying girl: stage 1 (`main_stage_set(1)`).

Steps (for a winner; step 0 is the pick):

| Step | Does |
| --- | --- |
| 1-3 | read the portrait file of the companion (sector table `festival_visit_bustup_lba`), `current_girl` = id (0 for 0xB), show it, `normal_date_girl_in` |
| 4 | when the invitation script (table `festival_visit_invite_script_lba`/`_sectors`, 1-3 sectors to 0x801E8000) is read: names for her, setter `festival_visit_invite_script_setter[id]`, group 0, line 0 |
| 5 | speak group 0 (her invitation) |
| 6 | companion 0: step 17; companions 1-3: step 32; 4-10 and 0xB: step 7 |
| 7-16 | the generic path: two-way choice (option 0 accepts, option 1 refuses); a refusal shows the sad face (`bustup_layout_set(festival_visit_bustup_layout[id])`) and stores the girl in `festival_refused_girl` (0 for 0xB); speak her reply; accepted: for girls 8 and 9 one more line (group 8), then stage 3; refused: she leaves (`normal_date_girl_out`), a line, stage 1 |
| 17-31 | Shiori with a culture club: group (her club + 1), speak, `acquainted` set, group 7 asks, two-way choice, a refusal leaves through step 28-31 (group 0xF, refused girl 0, stage 1); accepting plays group (her club + 9), the girl leaves, `festival_club` = her club, companion 0xFF, stage 4 |
| 32-46 | girls 1-3: group 1 or 2 by whether her club is her first or second club (girl 1: literature then drama, girl 2: science then computer, girl 3: art then brass band), `acquainted` set, group 3, two-way choice; accept: group 5 or 6, she leaves, `festival_club` = her club, companion 0xFF, stage 4; refuse: group 7, refused girl = her, stage 1 |

Emulator: with girls 4 and 5 poked above both thresholds (sums 200 and 190) the companion became 4 at frame 310, accepting led to stage 3 and then to stage 59.

### 5.3 Stage 1: ask a companion (50 steps)

Reached from stage 0 on a refusal or when nobody qualifies. The common script BUN_PC is the source of the first lines.

| Step | Does |
| --- | --- |
| 0-1 | when a refused girl is pending (companion not 0xFF) re-read BUN_PC and wire it; group 1, line 0 |
| 2-4 | speak group 1 (a narration line and a two-way question); option 1 goes to step 6, option 0 to step 7 |
| 6 | companion 0xFF, stage 2 |
| 7-8 | speak group 2, then `select_girl()` (the picker with a back item, listing the girls with `girl_menu_enabled`, that is the `met` ones) |
| 9 | the pick: the refused girl -> group 4 (step 10-11, then back to the question); back item (11) -> group 5, then stage 2; otherwise companion = pick (0 with a club id of 6 or more becomes 0xB), names, step 12 |
| 12-16 | speak group 3, read and show her portrait, `girl_in`, read her invitation file, setter `festival_visit_invite_script_setter[id]`; then step 20 for companion 0, 31 for 1-3, 42 for the others |
| 17-19 | companion leaves, a line, companion 0xFF, stage 2 (the refusal exit) |
| 20-30 | companion 0 (Shiori with a culture club): lines by mood rank (groups 0x10, 0x1B, 0x24, 0x2D for ranks 0-1, 2, 3, 4), then a second set by rank (0x11, 0x1C, 0x25, 0x2E) plus her club id; rank 4 goes to step 17 with group 0x34; otherwise group 0x17 or 0x18 by `acquainted` (then set), a third rank line (0x19, 0x22, 0x2B), she leaves, companion 0xFF, stage 2 |
| 31-41 | companions 1-3: lines by rank (8, 0xF, 0x15, 0x1B; then 9, 0x10, 0x16, 0x1C, +1 when it is the girl's first club), rank 4 goes to the refusal exit, else `acquainted` set and more lines, she leaves, companion 0xFF, stage 2 |
| 42-49 | companions 4-10 and 0xB: group by `get_g_zyotai_s` rank (3, 4, 5, 6; 8 when the girl is Saki and the player's club equals her club); if the raw `get_g_zyotai_s` equals 4 she refuses (step 17); Yuko with rank 2 or worse adds three more lines by year (groups 9-0xB, +3 for rank 3, then 0xF); otherwise stage 3 |

So girls 0-3, who belong to culture clubs, never join through the picker: they answer by mood and the flow goes to Yoshio (stage 2). They are met at their own clubs through the club menu (5.5).

### 5.4 Stages 2, 3 and 4

- Stage 2 (11 steps): reads Yoshio's portrait (BUSTUP/YO02), `current_girl` = 13, names for 13, speaks common group 6, then the year line group 7 + (year - 1995) (7, 8 or 9), the portrait leaves, common group 10, then stage 3.
- Stage 3: `festival_visit_club_menu_init` builds six menu items in two columns (labels "go to the X club" for literature, drama, science, computer, art, brass band); `festival_visit_club_menu_update` waits for circle on a hovered item; step 2 stores the index in `festival_club`; with no companion (0xFF) it plays common group 12 + club (a short remark) before stage 4, otherwise goes straight to stage 4.
- Stage 4 (`festival_visit_stage_route_visit`):
  - companion 0xFF: club 0-5: if girl 0's club equals the chosen club, stage 32 (Shiori club); else if girl 1's club equals it and she is `met` and the club is 0 or 1: stage 44; else girl 2 for clubs 2 and 3: stage 48; else girl 3 for clubs 4 and 5: stage 53; else the solo block (stage 5, or 9 for the science club, which is the same stage).
  - companion 0xB: stage 18; 4: 58; 5: 72; 6: 85; 7: 98; 8: 111; 9: 122; 10: 131.
  - any other value (0-3, 12-254): `game_reset_to_title`.

### 5.5 Exhibit templates (what every exhibit stage does)

All exhibit handlers (stages 5-139 without the routers) are one of three step patterns; only the group numbers, the backgrounds and the sub-mode launches differ (read them in the C; the table rows give stage, exhibit, background, script file and launches).

**Solo template** (stages 6-17, 11 stages): step 0 starts the CD read of the exhibit background (`dec_bg_cd_read`); 1 `normal_date_bg_fadeout`; 2 when read: `festival_visit_window_title_set`, decode and show the background, read BUN_PC; 3 fade in; 4 when read: pick the first group, `festival_visit_solo_script_begin`; then speak steps and group changes; optional launch of a stage show or TT; the last two steps set group 0x52 (`FV_COMMON_GROUP_WRAPUP`), speak, `festival_visit_finish_visit`. Typical length 10-23 steps.

**Companion template** (blocks of Shiori visitor, Saki, Yukari, Nozomi, Mira, Yuko, Megumi, Yumi, 13 stages each at most): the same sequence with `normal_date_bggirl_fadeout/in` (the background and the girl fade together, the girl's portrait was loaded in stage 0 or 1 and stays), the girl's own script file (disc BUNKA/XX_S6W.BIN, 9-19 sectors) read in step 2, and `festival_visit_<girl>_script_begin` in place of the solo helper. The reaction groups are selected by `girl_state_rank(girl)`: ranks 0 and 1 share a group, rank 2 and 3 differ in some stages. The last step sets group 0 (the girl's closing lines), speaks, finishes.

**Club-girl template** (Shiori club, Mio, Yuina, Ayako blocks, stages 33-57): the girl is already at her exhibit. Step 2 also starts reading her portrait for the club (Shiori has four costume files: normal, drama, science, art; Yuina a science one), her club script file (BUNKA/XX_S6O.BIN, 7-23 sectors) is read in step 4. The girl is **absent** when `girl_state_rank(girl) == 4` or she is the refused girl: then `normal_date_girl_in/out` and the `bggirl` fades are replaced by the background-only ones, her introduction group and her goodbye are skipped, and the scene uses the alternative first group (0xD in 1995, 0x16 later). When she is present: first meeting (her `acquainted` flag clear, set now) plays group 4; later visits play a rank-selected group (7/9/0xB in 1995, 0x10/0x12/0x14 later in the Mio block). A few brass-concert stages bring in girl 12 (Rei): her portrait is staged through memory copies (0x16800 bytes between 0x80180000, 0x80197000 and 0x801AE000). Route functions of these blocks send the visit to the solo block (`festival_companion` = 0xFF) when the girl's mood rank is 4 and she has not met the player at a club before (`acquainted` clear).

### 5.6 Sub-modes started from exhibit stages

- **Stage show (BUNKA_SD)**: used by the literature speech (1997), drama, science, computer 1997 experiment, brass concerts. The stage sets `festival_show_variant`, `festival_return_stage` = current stage, `festival_return_step` = step + 1, then `main_state_set(0x80)`. BUNKA_SD dispatches on `festival_club` and the year (its handler `func_80137890`), plays the show, then restores mode 0x70/0x71 by the club test and calls `main_stage_set(return_stage)` and `main_step_set(return_step)`. After the show the stage re-sets the title, starts `title_banner_timer` at 0xC0, rereads the script file and fades back in. Variants (the stage stores them just before the launch): 0 brass concerts of every block except 43 and 57, and the drama stages (which store nothing, the scratch word is still 0); 1 science, computer experiment and the speech stages of the companion blocks, and the generic case of the club-girl shows (set when the club girl is absent, else the girl's own variant); 2 the Shiori-club shows when she is present (speech 34 unconditionally); 3 the Yuina-club shows when she is present and the Mio-club speech (46); 4 the solo speech (7) and the Ayako-club brass concert (57). BUNKA_SD uses the variant as `2*variant + 2` and `+ 3` frame numbers of two display records and special-cases 3; HYPOTHESIS: it picks which performer set is drawn (a club girl's costume or the generic students).
- **TT game exhibit** (computer club, 1996: stages 12 solo, 25, 38, 51, 65, 79, 92, 105, 117, 127, 136): the girl talks, then `tt_direct` (`D_800E7D4F` bit 1) and `tt_direct_stage` (bits 2-4) are set, `saved_mode/stage/step` = mode, stage, step + 1, and TT starts. Two launch kinds: (a) stages 79 (Yukari, TT stage 1), 92 (Nozomi, 2), 117 (Yuko, 4) and 136 (Yumi, 3) first call `main_state_set(0xD0)` directly with the recorded run of that TT stage (TT's direct mode plays a demo without player input, o-tt spec); (b) every stage then (or alone, in the other seven) sets `tt_direct_stage` 0 and enters stage 141, the narrator scene, whose step 4 calls `main_state_set(0xD0)` only when `input_device` is the pad (with a mouse it waits forever); with stage 0 selected TT shows its own title and the player plays himself. TT returns to the step after the launch; that step stores `festival_return_stage/step` again, sets `festival_display_skip` and enters stage 140 (`festival_visit_stage_resume`), which reloads cursor, boot data, HUD textures, debug font, the 0x95-sector calendar data (disc sector 0x9B94) and the month calendar, then returns to the stored step; the following steps restart the sound set, background and portrait. Reaction: after the run `tt_stages_cleared` (`D_800E7D4E`) below a threshold adds 1 to the reaction group: threshold 2 in stages 25, 38, 51, 65, 92 and 105, 3 in stage 136, 4 in stage 117 (the thresholds of 92, 117 and 136 equal the demo stage of that block; HYPOTHESIS: the player is compared with the girl's recorded run); stages 12, 79 and 127 do not test it. At the end the exhibit sets `tt_unlocked` (bit 0 of `D_800E7D4F`).
- **Fortune telling** (computer club, 1995, `stage_fortune_telling` = stage 11): the exhibit stage stores its return stage and step, `snd_command(0x202)`, and `main_stage_set(0xB)`. Stage 11: step 0 reads URANAI (1 sector from 0x4B80) and refreshes the picker flags; step 1: if there is no companion, or a computer-club girl (girl 0 or 2 with club 3) is absent (rank 4 or refused), the player picks a girl (`select_girl2`, no back item) and `festival_companion` becomes the pick, else the present girl is used; the terminal screen lists the girl's name, "blood type" and her type, the player's name and type, each line after 0x15 frames, then a confirm press; then `festival_visit_fortune_result` (6.8) gives the result; step 23 waits 0x3D frames, stores `festival_fortune_result` and sets group (result + 3); step 25 re-sets the companion to girl 0 or 2 when that computer-club girl was absent, and returns through the saved stage and step. The calling stage adds `festival_fortune_result` to a group base to pick the reaction lines.

## 6. Rules

### 6.1 Group choice by mood

`girl_state_rank` is read again at almost every step that speaks: group = base(rank): ranks 0 and 1 share a base, rank 2 and rank 3 have their own, rank 4 has an "absent or refuses" base or none. The year (`year - 0x5F`) adds or selects groups in the club-girl and some companion stages. Parameter gates exist in five places: `param_liberal_arts` at least 120 (0x78) picks the first of two adjacent lines in the Shiori-visitor literature poem exhibit (stage 19) and below 120 adds 1 in the Nozomi poem exhibit (86); `param_art` at least 120 picks the first line in the Shiori-visitor oil painting exhibit (29) and year-97 brass concert (31), and below 120 adds 1 in Saki's year-97 brass concert (71) and in Nozomi's brass concert (97). Group numbers per step are in the stage handlers.

### 6.2 Points at the end of a visit (`festival_visit_apply_points`)

- Table choice by year: `sel = year - 0x5F` selects `festival_visit_points_y95`, `_y96` or `_y97`; cell = `[girl][club]` where the girl is the companion (0xB uses row 0) and the club is `festival_club`. Codes 0-3.
- Companion 0-10 or 0xB: affection +2, tokimeki + (code - 1 when the code is above 0). The 1995 table has rows for girls 0-8 only.
- Refused girl (not 0xFF): affection -10 and syoushin +10.
- Companion 0xFF: no gain.
- Birthday scenes are separate (6.6).

Decoded tables (rows girl 0-10, columns clubs 0-5):

| Girl | 1995 | 1996 | 1997 |
| --- | --- | --- | --- |
| 0 | 1 2 2 2 2 2 | 1 1 3 0 0 1 | 2 3 2 2 3 3 |
| 1 | 3 2 1 2 0 2 | 3 2 2 0 0 1 | 3 3 1 1 1 3 |
| 2 | 0 1 2 1 2 1 | 0 0 1 1 0 0 | 0 0 2 2 3 3 |
| 3 | 0 2 1 1 3 2 | 1 1 1 1 2 3 | 2 2 3 3 3 3 |
| 4 | 2 1 1 1 1 1 | 2 1 1 1 0 1 | 2 1 3 3 2 1 |
| 5 | 2 2 1 1 1 1 | 2 1 2 1 1 1 | 1 2 1 1 2 1 |
| 6 | 1 1 0 3 2 1 | 1 0 3 2 0 1 | 3 3 2 2 1 2 |
| 7 | 1 1 0 1 3 2 | 1 1 3 0 0 0 | 1 3 2 2 2 1 |
| 8 | 0 3 2 1 2 2 | 0 0 1 2 0 2 | 0 1 3 3 1 0 |
| 9 | - | 2 1 3 1 0 0 | 2 2 2 2 2 2 |
| 10 | - | 0 3 2 3 0 2 | 2 0 3 3 0 0 |

Tokimeki gain from the code: 0 and 1 give 0, 2 gives 1, 3 gives 2 (affection is +2 in every case). The tables are plain data and a port should copy them.

### 6.3 Refusal and the refused girl

Declining an invitation or a companion (stage 0 and stage 1) writes the girl into `festival_refused_girl`. Effects: she cannot be picked again in stage 1 (group 4 instead); at her own club she is absent (6.5); a computer-club girl who was refused no longer rigs the fortune telling (6.8); and at the end she loses 10 affection and gains 10 syoushin (the third counter of the girl record; core leaves its meaning open). The refusal also shows her sad face (`festival_visit_bustup_layout`).

### 6.4 `acquainted`

`girls[g].flags.f.acquainted` (core `CharFlags`, bit 2) is set the first time the player meets a club girl: in stage 0 (steps 17-19 and 32), in stage 1 (steps 24 and 35), and at the first step of a club-girl exhibit. The flag selects a longer first-meeting group once; the route functions of the club-girl blocks also use it (a girl whose mood rank is 4 and who is not yet acquainted is replaced by the solo block).

### 6.5 Absent girl

A club girl is absent when `girl_state_rank == 4` or she is the refused girl. In her exhibit stage the portrait fades and the girl scenes are skipped (see 5.5), the stage plays the absent-girl groups instead, and a show variant is the generic one (5.6).

### 6.6 Birthday phases

**Phase 9** (`festival_visit_phase_shiori_birthday`, 73 steps; condition in 4.1): sets mask 1 of `daily_event_bits`, starts reading BUN_FU (3 sectors from 0x49B7), waits 21 frames (`Vblnk_Timer`) and for the CD, loads the birthday sound set and wires BUN_FU for girl 0. Step 2 chooses one of three paths:

- Her club id is below 6 (step 0x2E, steps 0x2E-0x48): the scene is at her club exhibit: background by her club and the year (club 0: the hall in 1997, else the literature room; clubs 1 and 5: the backstage room; 2: lab; 3: computer room; 4: the sketching room in 1996, else the art room), her club costume portrait (clubs 1, 2, 4: the drama, science and art files, else the normal one). A two-way question follows: option 1 declines (she loses 1 affection and gains 5 syoushin, then the visit finishes), option 0 plays the year line (group year - 0x5C), a three-way choice, a reply that carries the chosen option's name in name buffer A, she enters, rank lines (groups 0x12-0x15 and 0x17-0x1A) and the gains below.
- The player's birthday is also today and her mood rank is below 2 (steps 0x16-0x2D): the combined scene: sets mask 4, park background (D_09_02), log flag 0x19, reads P_BIRTH (3 sectors from 0xAF43 to 0x801B0000), plays its section 1 through `festival_visit_birthday_script`, then BUN_FU groups 0x26-0x29, the three-way choice, the gains and group 0x23 (0x27 for option 0 at rank below 2).
- Otherwise (steps 3-0x15): intro group 0x25; the setting depends on her mood: rank below 2 gives the neighborhood park (D_09_02, log flag 0x19), worse gives the school corridor (D_01_50, log flag 0x1A); groups 0x28, the year line (group year - 0x5C), the three-way choice, her rank reply (0x17, 0x18, 0x19, 0x1A), the option reply (0x1B, 0x1D or 0x1F) with her face changing, group 0x27 when her rank is below 2, fade out, finish.

Gains by the original option index of the three-way choice (the menu shuffles the order but returns the original index): option 0 tokimeki +3, affection +2, syoushin -15; option 1 tokimeki +2, affection +1, syoushin -10; option 2 affection +1, syoushin +10. The reply line's name code A is filled by `festival_visit_birthday_option_name_set` from the 3 x 3 name table by year and option (HYPOTHESIS: the names are presents). Emulator: with girl 0's birthday poked to today and companion 0 (solo art exhibit) the standard path ran at frame 580, option 1 came out (+2, +1, -10), then the visit added +2 affection and +1 tokimeki, `daily_event_bits` held mask 1 and two log entries existed.

**Phase 10** (`festival_visit_phase_player_birthday`, 12 steps; the companion is girl 4-10 with mood rank below 2): sets mask 4, reads BUN_FU (group 0x25), waits like phase 9, names for the companion, reads the park background, shows the park (title, `bustup_layout_set(get_g_zyotai_h(companion) & 0x7F)`), reads P_BIRTH, plays its section for that girl (4 -> 10, 5 -> 7, 6 -> 8, 7 -> 5, 8 -> 0, 9 -> 9, 10 -> 12; id 11 -> 1 is dead), fades out and logs flag 0x19. No point change of its own.

### 6.7 Event log

`festival_visit_event_log_add` appends one record to `event_log` (cap 0xFE entries, same layout as core's `EventLogEntry`): girl (companion, 0 for 0xB), flag `4 * club + 3` when the companion is the girl who belongs to that club and stands at her own club, else `4 * club + 4`, scene `FV_LOG_SCENE` 0x14, rank `get_g_zyotai_h(girl)`, the date. Skipped when the girl is the refused girl or her `girl_state_rank` is 4.

### 6.8 Fortune result (`festival_visit_fortune_result`)

3 when the companion is Yuina (2) whose club is 3, her mood rank is below 4 and she is not the refused girl; else `festival_visit_fortune_grid[girl blood code][player blood code]` where the code is the 2-bit blood type of the girl's birthday word and of the player's (core `BirthdayBits`): 2 when the codes are equal, 0 for the pairs (0,1), (1,0), (2,3), (3,2), 1 otherwise. The terminal labels the codes A, B, O, AB. Group (result + 3) is spoken (4 outcomes). Emulator: girl code 2 and player code 1 gave result 1 and group 4.

### 6.9 Pointers to the script of a girl block

28 setters store three addresses (text, command, voice tables) of the file loaded at 0x801E8000 into `festival_visit_script_text/commands/voices`; the helpers `festival_visit_script_tables_apply` and the per-block `..._script_begin` copy them into `dialogue_text_table`, `dialogue_command_table` and `dialogue_voice_table`. The addresses depend on the file; they are not tables a port needs when it reads the files itself.

## 7. Data

Disc files (names only). Script files, read to 0x801E8000 (sector number, sector count):

| Use | File | LBA | Sectors |
| --- | --- | --- | --- |
| common | BUNKA/BUN_PC.BIN | 0x49BA | 5 |
| girl 0 invitation / at own club / sports-or-no-club invitation / visits | FU_S6I / FU_S6O / FU_S6S / FU_S6W | 0x49E8 / 0x49EB / 0x4A02 / 0x4A03 | 3 / 23 / 2 / 15 |
| girl 1: invitation, own club | KS_S6I, KS_S6O | 0x4AB4, 0x4AB6 | 2, 7 |
| girl 2: invitation, own club | HI_S6I, HI_S6O | 0x4A36, 0x4A38 | 2, 9 |
| girl 3: invitation, own club | KT_S6I, KT_S6O | 0x4B08, 0x4B0A | 2, 9 |
| girls 4-10: invitation (S), visits (W) | NI_S6S / NI_S6W, KO, KY, KG, AS, MI, SA | 0x4B51/0x4B52, 0x4A72/0x4A73, 0x4B22/0x4B23, 0x4A50/0x4A52, 0x49A5/0x49A7, 0x4B3D/0x4B3E, 0x4B71/0x4B72 | 1+12, 1+16, 1+16, 2+19, 2+16, 1+9, 1+13 |
| TT narrator | TWINBEE.BIN | 0x4B7F | 1 |
| fortune telling | URANAI.BIN | 0x4B80 | 1 |
| Shiori birthday common | BUN_FU.BIN | 0x49B7 | 3 |
| birthday park scenes | SERIFU/P_BIRTH.BIN | 0xAF43 (to 0x801B0000) | 3 |

(The first letters are the girl codes of the disc: FU girl 0, KS 1, HI 2, KT 3, NI 4, KO 5, KY 6, KG 7, AS 8, MI 9, SA 10. The letters I, S, O, W are file kinds: club-girls' invitation (I), other girls' invitation (S), own club (O), visit together (W); the meaning of the letters is HYPOTHESIS, the use is read from the code.)

Backgrounds (BS folder, sector): gate D_01_48 (0x4029), literature exhibit D_01_67 (0x40EA), backstage D_01_68 (0x40F4), science lab D_01_69 (0x40FC), computer room D_01_70 (0x4105), art room D_01_71 (0x410F) and the sketching variant D_01_72 (0x4118), the hall stage D_30_01 (0x467C), park D_09_02 (0x42D8), corridor D_01_50 (0x403B). The room names are my reading of which exhibit uses the file.

Portraits (BUSTUP folder, `*02.BIN` is the normal pose set): girl 0 HU02 (0x544A), HU03 (0x5477, drama), HU05 (0x54D1, science), HU04 (0x54A4, art); 1 KI02 (0x5D41); 2 HI02 (0x522E), HI05 (0x525B science); 3 KA02 (0x58AF), KA04 (0x58DC brass); 4 NI02; 5 KO02; 6 KY02; 7 KG02; 8 AS02; 9 MI02; 10 SA02; Yoshio YO02 (0x6C5F); Rei IJ02 (0x5828).

Overlay tables (`data` rows in the table): `festival_visit_stage_table` (142 pointers), `festival_visit_bustup_lba`, `festival_visit_invite_script_setter/_lba/_sectors`, `festival_visit_bustup_layout` (3 or 4 per companion id: the sad face of a refused girl), `festival_visit_club_menu_labels`, the three `festival_visit_points_*` tables (6.2), `festival_visit_fortune_grid` and its label tables, `festival_visit_birthday_script` (3 x 13 pointers into P_BIRTH), `festival_visit_birthday_option_names` (3 x 3 x 33 bytes). Unused data: `festival_visit_unused_table` (11 bytes, copied by the entry and never read), `festival_visit_scratch_bytes` (32 bytes, one store).

## 8. Connections

| Link | Facts |
| --- | --- |
| Main queue reader | `day_event_dispatch` starts the mode (section 2); phase 8 ends with `day_event_next(1)`; `overlay_scratch` is cleared by both |
| BUNKASAI (mode 0x70) | the other variant: same entry skeleton, shared words, scoring built from the player's own club, sets `festival_call_95/96` |
| BUNKA_SD (mode 0x80) | stage shows; contract in 5.6; returns by the club test of its own |
| TT (mode 0xD0) | game exhibit; contract in 5.6; `tt_stages_cleared` read back |
| Scheduler | the `MARKER_CULTURE_FESTIVAL` marker creates the day record; BUNKAKEN sets no flags for later phone calls |
| Event log | scene 0x14 entries (6.7); the log layout and its readers belong to core |
| Dialogue engine | main `normal_date_speak`, `normal_date_two_select`, `normal_date_three_select`, `select_girl`, `select_girl2` (spec `m-dates-b`) |
| Sound | phase 0 fades the day music out (`SND_CMD_FADE_BOTH_FAST`) and loads the festival set (SCLFESSE sound effects, SCLFEST sequence bank); BGM sequence 0 (0x200) plays from phase 2 and again after sub-modes; the fortune telling switches to sequence 2 (0x202) and uses effects 0x50A and 0x50C; the birthday phases fade out (`SND_CMD_FADE_BOTH_MEDIUM`) and load the DAYS set (the day-screen music); the finish phase calls `day_bgm_load` for the banks of the day screen |

## 9. Placeholder names in the matched C

| Placeholder | Core name |
| --- | --- |
| `D_800E6280` / `game_state` fields `unk_1109`, `unk_110A`, `unk_1108` | `stage`, `step`, `mode` |
| `unk_1BC[i].unk_02 / .unk_06 / .unk_0A / .unk_0C / .unk_10` | `girls[i].tokimeki`, `.affection`, `.syoushin`, `.flags`, `.birthday` |
| `unk_0F4.h >> 12` | the club field of `player_flags` |
| `unk_03E`, `unk_03F`, `unk_040` | `year`, `month`, `day` |
| `unk_71E` | `daily_event_bits` |
| `unk_F5F`, `unk_F74`, `unk_F88` | `current_girl`, `input_device`, `pad_pressed` |
| `unk_720..722` | `saved_mode`, `saved_stage`, `saved_step` |
| `func_8008667C` | `girl_state_rank` |
| `func_80042808`, `func_80042908`, `func_80042940`, `func_8004284C` | `main_stage_next`, `main_stage_set`, `main_step_set`, `main_step_next` |
| `func_80044750`, `func_80044890`, `func_80044E8C` | `snd_command`, `snd_bank_load_start`, `snd_bank_load_poll` |
| `func_80072B5C`, `func_800674B0` | `day_event_next`, `day_bgm_load` |
| `D_800CA148/14C/160/164/168` | `dialogue_group`, `dialogue_line`, `dialogue_text_table`, `dialogue_command_table`, `dialogue_voice_table` |
| `D_80122CDC` | `dialogue_choice` |
| `D_800CA2B0` | `girl_menu_enabled` |
| `D_80125CA0` | `title_banner_timer` |
| `D_80122DD0` | `overlay_scratch` |

## 10. File plan

`src/ovl/BUNKAKEN/` becomes `src/ovl/festival_visit/`; the 18 files are named by their main content (rows `file` in the table): `entry.c` (80144F70), `stage_table.c` (80145E40), `invitation.c` (8013B510), `companion_menu.c` (80143DD0), `solo_block.c` (80139E50), `script_setters_shiori_yukari_blocks.c` (80132000, 28 setters plus two exhibit blocks), `shiori_club_block.c`, `mio_club_block.c`, `yuina_club_block.c`, `ayako_club_block.c`, `saki_block.c`, `nozomi_block.c`, `mira_block.c`, `yuko_block.c`, `megumi_block.c`, `yumi_block.c`, `birthday.c`, `computer_fortune.c`; the header `include/ovl/festival_visit.h`.

## 11. Port notes

- Model the visit as data plus a small interpreter: the key is (companion, club, year); the value is an exhibit scene made of dialogue groups, a background, optional sub-scenes (stage show, TT, fortune telling) and the mood-rank group tables. The handlers are repetitive on purpose, so the group numbers can be dumped by walking the C of each handler once (the catalog in Appendix A lists the handler, background, script file, portraits and launches).
- Everything shown is script text from the BUNKA files; the port needs the dumper for those files (groups, speak codes, voice references) described in `m-dates-b`, plus the background, portrait and sound files named in section 7.
- Keep exactly: the invitation pick (5.2), the picker rules (5.3), the club-girl routing and absence rules (5.4, 5.5, 6.5), the three year tables (6.2), the refused-girl penalty (6.3), the birthday phases (6.6), the fortune grid and its rigging (6.8), the event log record (6.7), and the mood rank from core's `girl_state_rank` and `get_g_zyotai_h`.
- Replace: the CD reads, VRAM background decode, portrait staging copies, sound bank loads and the display-skip flag (all hardware plumbing), and the per-frame `step` machine (a port may run each exhibit as a coroutine). The two sub-modes become calls: the stage show (BUNKA_SD spec) and the TT game (o-tt spec) with their return values: show variant in, nothing out; TT direct stage in, `tt_stages_cleared` out.
- Order of effects at the end of a visit: birthday scene (if due), companion gain, refused-girl penalty (both in `festival_visit_apply_points`; the same girl can receive both), event log, clear festival state, day advance.

## 12. Quirks a port should know about

- The mood rank tests in the finish phase and in the event log use the raw companion id: for 0xB they read girl 11, not Shiori (the record written is for girl 0).
- The 1995 points table has 9 rows; a 1995 visit with girl 9 or 10 would read memory behind it. Megumi's route function refuses 1995; Yumi's maps it to the 1997 stage, so a port should treat rows 9 and 10 of 1995 as absent and keep both girls out of 1995.
- After the fortune telling the computer-club girl is set as the companion when she was absent, so she receives the visit gain although she was not there.
- Stage 141 waits forever with a mouse instead of a pad.
- The second clause of the player-birthday test (companion 0xB) can never be true, and phase 10's switch case for id 11 is dead.
- `festival_visit_script_ayako_club` stores the command-table pointer twice; the second value is the one that stays.
- In 1996 girls of the art and drama clubs cannot invite; girl 0 additionally can lose the invitation by the final rule.
- Entry-phase 0 increments the phase through an aliasing store; a port should just set phase 1.

## 13. Open questions

- What "bunkaken" abbreviates (prefix choice, section naming note).
- Core names: `MODE_CULTURE_FESTIVAL_A/B` are neutral and fine; if the orchestrator wants meaning in them, the code supports `_MEMBER` (club id below 6) and `_VISITOR` (6 or more), but no rename is proposed here. The type `CultureFestivalDispatch` (core, HYPOTHESIS) holds the club and the companion; this table names its fields `club` and `companion`.
- The festival words (section 3) and the TT flag bits are main-scope proposals; the BUNKASAI table will name the same addresses, so the two names must be reconciled before the rename tool runs.
- Which girl or cast the five BUNKA_SD `festival_show_variant` values stand for (BUNKA_SD spec).
- Meaning of `syoushin` (affection counter that falls with good choices and rises with refusals: HYPOTHESIS "doubt" or "disappointment").
- Whether the year 1996 invitation exclusion of the drama and art clubs is a design decision; no code states a reason.
- The file kind letters I, S, O, W (section 7).
- `festival_visit_unused_table` (D_80155F90) looks like a leftover per-girl table of values 0-2; unread.
- The names of the BS backgrounds are inferred from use.
- A port needs a dumper for the BUNKA script files (the groups and speak codes are in the common format of the other SERIFU scripts, see `m-dates-b`).

## 14. Emulator checks used

A local copy of the probe runner with `--poke` and `--sample` (kept in `build/scratch`, not committed) started from a title-menu save state, set the loader to overlay mode 0x71 and sampled the festival words every 10 frames. Observed: phases 0, 2, 3, 4, 6, 7 on a fresh start; with only Shiori met the picker listed her, her id became 0xB (club none) and the visit went stage 1 -> 3 -> 18 -> 19; two girls poked above the thresholds gave the invitation to the one with the larger sum, accepting led to stage 59, and the finish gave her +2 affection and +1 tokimeki; a refused girl poked into `festival_refused_girl` lost 10 affection and gained 10 syoushin at the end of the art exhibit (stage 14) while the empty companion got nothing; fortune telling with blood codes 2 and 1 stored result 1; with Shiori's club poked to literature the club menu pick 0 routed stage 4 -> 32 -> 33 with companion 0; the Shiori birthday phase ran before the finish and applied option 1's gains. Screenshots taken in these runs (not committed) showed: the opening scene at the school gate with one narration line; the invitation scene with the girl's portrait and the two answer lines (an accepting one and a declining one, the first being option 0); the club menu with six items in two columns in the order literature, drama / science, computer / art, brass band; and the picker with the first met girl listed at the top left and a cancel item at the bottom right. Not run: stage shows (they last about 3,000 frames), the TT launches, the 142 handlers one by one.

## Appendix A. Stage catalog (exhibit stages 6-139 without routers and stage 11)

One row per exhibit handler: the rows come from the matched C (backgrounds, script file, portrait files read by the handler, sub-mode launches, number of steps). `show vN` is a BUNKA_SD stage show with variant N (`show` alone: the variant is not stored by the stage), `TT demo N + play` the two TT launches of 5.6, `TT<N` the stage-cleared threshold, `gate` a parameter test of 6.1. Handler names are the table names without the prefix `festival_visit_stage_`. The room name of a background is in section 7.

| Stage | Handler (`festival_visit_stage_...`) | Exhibit | Background | Script | Portrait files | Launches | Steps |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 6 | `solo_lit_collection` | lit collection | D_01_67 | BUN_PC | - | - | 14 |
| 7 | `solo_lit_speech` | lit speech | D_30_01 | BUN_PC | - | show v4 | 16 |
| 8 | `solo_drama` | drama | D_30_01 | BUN_PC | - | show | 17 |
| 9 | `solo_science` | science | D_01_69 | BUN_PC | - | show v1 | 16 |
| 10 | `solo_comp_fortune` | comp fortune | D_01_70 | BUN_PC | - | fortune | 14 |
| 12 | `solo_comp_game` | comp game | D_01_70 | BUN_PC | - | TT play | 18 |
| 13 | `solo_comp_experiment` | comp experiment | D_01_70 | BUN_PC | - | show v1 | 16 |
| 14 | `solo_art_portrait` | art portrait | D_01_71 | BUN_PC | - | - | 10 |
| 15 | `solo_art_sketch` | art sketch | D_01_72 | BUN_PC | YO02 | - | 23 |
| 16 | `solo_art_oil` | art oil | D_01_71 | BUN_PC | - | - | 14 |
| 17 | `solo_brass` | brass | D_30_01 | BUN_PC | - | show v0 | 16 |
| 19 | `shiori_visitor_lit_poems` | lit poems | D_01_67 | FU_S6W | - | gate liberal_arts>=120 | 18 |
| 20 | `shiori_visitor_lit_anthology` | lit anthology | D_01_67 | FU_S6W | - | - | 17 |
| 21 | `shiori_visitor_lit_speech` | lit speech | D_30_01 | FU_S6W | HU02 | show v1 | 17 |
| 22 | `shiori_visitor_drama` | drama | D_30_01 | FU_S6W | HU02 | show | 19 |
| 23 | `shiori_visitor_science` | science | D_01_69 | FU_S6W | HU02 | show v1 | 17 |
| 24 | `shiori_visitor_comp_fortune` | comp fortune | D_01_70 | FU_S6W | - | fortune | 18 |
| 25 | `shiori_visitor_comp_game` | comp game | D_01_70 | FU_S6W | HU02 | TT play; TT<2 | 23 |
| 26 | `shiori_visitor_comp_experiment` | comp experiment | D_01_70 | FU_S6W | HU02 | show v1 | 17 |
| 27 | `shiori_visitor_art_portrait` | art portrait | D_01_71 | FU_S6W | - | - | 16 |
| 28 | `shiori_visitor_art_sketch` | art sketch | D_01_72 | FU_S6W | - | - | 12 |
| 29 | `shiori_visitor_art_oil` | art oil | D_01_71 | FU_S6W | - | gate art>=120 | 18 |
| 30 | `shiori_visitor_brass` | brass | D_30_01 | FU_S6W | HU02 | show v0 | 17 |
| 31 | `shiori_visitor_brass_y97` | brass y97 | D_30_01 | FU_S6W | HU02 | show v0; gate art>=120 | 21 |
| 33 | `shiori_club_lit_collection` | lit collection | D_01_67 | FU_S6O | HU02 | - | 21 |
| 34 | `shiori_club_lit_speech` | lit speech | D_01_68/D_30_01 | FU_S6O | HU02 | show v2 | 23 |
| 35 | `shiori_club_drama` | drama | D_01_68/D_30_01 | FU_S6O | HU03 | show | 23 |
| 36 | `shiori_club_science` | science | D_01_69 | FU_S6O | HU05 | show v1/2 | 24 |
| 37 | `shiori_club_comp_fortune` | comp fortune | D_01_70 | FU_S6O | HU02 | fortune | 27 |
| 38 | `shiori_club_comp_game` | comp game | D_01_70 | FU_S6O | HU02 | TT play; TT<2 | 31 |
| 39 | `shiori_club_comp_experiment` | comp experiment | D_01_70 | FU_S6O | HU02 | show v1/2 | 24 |
| 40 | `shiori_club_art_portrait` | art portrait | D_01_71 | FU_S6O | HU04/IJ02 | - | 36 |
| 41 | `shiori_club_art_sketch` | art sketch | D_01_72 | FU_S6O | HU04/YO02 | - | 28 |
| 42 | `shiori_club_art_oil` | art oil | D_01_71 | FU_S6O | HU04 | - | 21 |
| 43 | `shiori_club_brass` | brass | D_01_68/D_30_01 | FU_S6O | HU02 | show v1 | 23 |
| 45 | `mio_club_lit_collection` | lit collection | D_01_67 | KS_S6O | KI02 | - | 21 |
| 46 | `mio_club_lit_speech` | lit speech | D_01_68/D_30_01 | KS_S6O | KI02 | show v3 | 23 |
| 47 | `mio_club_drama` | drama | D_01_68/D_30_01 | KS_S6O | KI03 | show | 23 |
| 49 | `yuina_club_science` | science | D_01_69 | HI_S6O | HI05 | show v1/3 | 24 |
| 50 | `yuina_club_comp_fortune` | comp fortune | D_01_70 | HI_S6O | HI02 | fortune | 23 |
| 51 | `yuina_club_comp_game` | comp game | D_01_70 | HI_S6O | HI02 | TT play; TT<2 | 30 |
| 52 | `yuina_club_comp_experiment` | comp experiment | D_01_70 | HI_S6O | HI02 | show v1/3 | 24 |
| 54 | `ayako_club_art_portrait` | art portrait | D_01_71 | KT_S6O | IJ02/KA04 | - | 40 |
| 55 | `ayako_club_art_sketch` | art sketch | D_01_72 | KT_S6O | KA04/YO02 | - | 26 |
| 56 | `ayako_club_art_oil` | art oil | D_01_71 | KT_S6O | KA04 | - | 21 |
| 57 | `ayako_club_brass` | brass | D_01_68/D_30_01 | KT_S6O | KA02 | show v4 | 24 |
| 59 | `saki_lit_poems` | lit poems | D_01_67 | NI_S6W | - | - | 14 |
| 60 | `saki_lit_anthology` | lit anthology | D_01_67 | NI_S6W | - | - | 17 |
| 61 | `saki_lit_speech` | lit speech | D_30_01 | NI_S6W | NI02 | show v1 | 17 |
| 62 | `saki_drama` | drama | D_30_01 | NI_S6W | NI02 | show | 18 |
| 63 | `saki_science` | science | D_01_69 | NI_S6W | NI02 | show v1 | 21 |
| 64 | `saki_comp_fortune` | comp fortune | D_01_70 | NI_S6W | - | fortune | 18 |
| 65 | `saki_comp_game` | comp game | D_01_70 | NI_S6W | NI02 | TT play; TT<2 | 23 |
| 66 | `saki_comp_experiment` | comp experiment | D_01_70 | NI_S6W | NI02 | show v1 | 17 |
| 67 | `saki_art_portrait` | art portrait | D_01_71 | NI_S6W | - | - | 14 |
| 68 | `saki_art_sketch` | art sketch | D_01_72 | NI_S6W | - | - | 14 |
| 69 | `saki_art_oil` | art oil | D_01_71 | NI_S6W | - | - | 14 |
| 70 | `saki_brass` | brass | D_30_01 | NI_S6W | NI02 | show v0 | 17 |
| 71 | `saki_brass_y97` | brass y97 | D_30_01 | NI_S6W | NI02 | show v0; gate art<120 | 21 |
| 73 | `yukari_lit_poems` | lit poems | D_01_67 | KO_S6W | - | - | 14 |
| 74 | `yukari_lit_anthology` | lit anthology | D_01_67 | KO_S6W | - | - | 17 |
| 75 | `yukari_lit_speech` | lit speech | D_30_01 | KO_S6W | KO02 | show v1 | 17 |
| 76 | `yukari_drama` | drama | D_30_01 | KO_S6W | KO02 | show | 18 |
| 77 | `yukari_science` | science | D_01_69 | KO_S6W | KO02 | show v1 | 17 |
| 78 | `yukari_comp_fortune` | comp fortune | D_01_70 | KO_S6W | - | fortune | 18 |
| 79 | `yukari_comp_game` | comp game | D_01_70 | KO_S6W | KO02 | TT demo 1 + play | 34 |
| 80 | `yukari_comp_experiment` | comp experiment | D_01_70 | KO_S6W | KO02 | show v1 | 17 |
| 81 | `yukari_art_portrait` | art portrait | D_01_71 | KO_S6W | - | - | 14 |
| 82 | `yukari_art_sketch` | art sketch | D_01_72 | KO_S6W | - | - | 13 |
| 83 | `yukari_art_oil` | art oil | D_01_71 | KO_S6W | - | - | 14 |
| 84 | `yukari_brass` | brass | D_30_01 | KO_S6W | KO02 | show v0 | 17 |
| 86 | `nozomi_lit_poems` | lit poems | D_01_67 | KY_S6W | - | gate liberal_arts<120 | 18 |
| 87 | `nozomi_lit_anthology` | lit anthology | D_01_67 | KY_S6W | - | - | 18 |
| 88 | `nozomi_lit_speech` | lit speech | D_30_01 | KY_S6W | KY02 | show v1 | 17 |
| 89 | `nozomi_drama` | drama | D_30_01 | KY_S6W | KY02 | show | 18 |
| 90 | `nozomi_science` | science | D_01_69 | KY_S6W | KY02 | show v1 | 17 |
| 91 | `nozomi_comp_fortune` | comp fortune | D_01_70 | KY_S6W | - | fortune | 18 |
| 92 | `nozomi_comp_game` | comp game | D_01_70 | KY_S6W | KY02 | TT demo 2 + play; TT<2 | 36 |
| 93 | `nozomi_comp_experiment` | comp experiment | D_01_70 | KY_S6W | KY02 | show v1 | 20 |
| 94 | `nozomi_art_portrait` | art portrait | D_01_71 | KY_S6W | - | - | 14 |
| 95 | `nozomi_art_sketch` | art sketch | D_01_72 | KY_S6W | - | - | 17 |
| 96 | `nozomi_art_oil` | art oil | D_01_71 | KY_S6W | - | - | 14 |
| 97 | `nozomi_brass` | brass | D_30_01 | KY_S6W | KY02 | show v0; gate art<120 | 21 |
| 99 | `mira_lit_poems` | lit poems | D_01_67 | KG_S6W | - | - | 23 |
| 100 | `mira_lit_anthology` | lit anthology | D_01_67 | KG_S6W | - | - | 19 |
| 101 | `mira_lit_speech` | lit speech | D_30_01 | KG_S6W | KG02 | show v1 | 20 |
| 102 | `mira_drama` | drama | D_30_01 | KG_S6W | KG02 | show | 20 |
| 103 | `mira_science` | science | D_01_69 | KG_S6W | KG02 | show v1 | 26 |
| 104 | `mira_comp_fortune` | comp fortune | D_01_70 | KG_S6W | - | fortune | 20 |
| 105 | `mira_comp_game` | comp game | D_01_70 | KG_S6W | KG02 | TT play; TT<2 | 26 |
| 106 | `mira_comp_experiment` | comp experiment | D_01_70 | KG_S6W | KG02 | show v1 | 19 |
| 107 | `mira_art_portrait` | art portrait | D_01_71 | KG_S6W | - | - | 16 |
| 108 | `mira_art_sketch` | art sketch | D_01_72 | KG_S6W | - | - | 13 |
| 109 | `mira_art_oil` | art oil | D_01_71 | KG_S6W | - | - | 16 |
| 110 | `mira_brass` | brass | D_30_01 | KG_S6W | KG02 | show v0 | 19 |
| 112 | `yuko_lit_collection` | lit collection | D_01_67 | AS_S6W | - | - | 16 |
| 113 | `yuko_lit_speech` | lit speech | D_30_01 | AS_S6W | AS02 | show v1 | 18 |
| 114 | `yuko_drama` | drama | D_30_01 | AS_S6W | AS02 | show | 21 |
| 115 | `yuko_science` | science | D_01_69/D_01_70 | AS_S6W | AS02 | show v1 | 18 |
| 116 | `yuko_comp_fortune` | comp fortune | D_01_70 | AS_S6W | - | fortune | 19 |
| 117 | `yuko_comp_game` | comp game | D_01_70 | AS_S6W | AS02 | TT demo 4 + play; TT<4 | 36 |
| 118 | `yuko_art_portrait` | art portrait | D_01_71 | AS_S6W | - | - | 15 |
| 119 | `yuko_art_sketch` | art sketch | D_01_72 | AS_S6W | - | - | 12 |
| 120 | `yuko_art_oil` | art oil | D_01_71 | AS_S6W | - | - | 16 |
| 121 | `yuko_brass` | brass | D_30_01 | AS_S6W | AS02 | show v0 | 20 |
| 123 | `megumi_lit_anthology` | lit anthology | D_01_67 | MI_S6W | - | - | 17 |
| 124 | `megumi_lit_speech` | lit speech | D_30_01 | MI_S6W | MI02 | show v1 | 17 |
| 125 | `megumi_drama` | drama | D_30_01 | MI_S6W | MI02 | show | 18 |
| 126 | `megumi_science` | science | D_01_69/D_01_70 | MI_S6W | MI02 | show v1 | 17 |
| 127 | `megumi_comp_game` | comp game | D_01_70 | MI_S6W | MI02 | TT play | 21 |
| 128 | `megumi_art_sketch` | art sketch | D_01_72 | MI_S6W | - | - | 13 |
| 129 | `megumi_art_oil` | art oil | D_01_71 | MI_S6W | - | - | 14 |
| 130 | `megumi_brass` | brass | D_30_01 | MI_S6W | MI02 | show v0 | 17 |
| 132 | `yumi_lit_anthology` | lit anthology | D_01_67 | SA_S6W | - | - | 10 |
| 133 | `yumi_lit_speech` | lit speech | D_30_01 | SA_S6W | SA02 | show v1 | 17 |
| 134 | `yumi_drama` | drama | D_30_01 | SA_S6W | SA02 | show | 18 |
| 135 | `yumi_science` | science | D_01_69/D_01_70 | SA_S6W | SA02 | show v1 | 17 |
| 136 | `yumi_comp_game` | comp game | D_01_70 | SA_S6W | SA02 | TT demo 3 + play; TT<3 | 32 |
| 137 | `yumi_art_sketch` | art sketch | D_01_72 | SA_S6W | - | - | 19 |
| 138 | `yumi_art_oil` | art oil | D_01_71 | SA_S6W | - | - | 14 |
| 139 | `yumi_brass` | brass | D_30_01 | SA_S6W | SA02 | show v0 | 17 |
