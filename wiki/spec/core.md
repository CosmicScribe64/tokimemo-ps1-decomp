---
type: spec
updated: 2026-10-24
sources: ["include/main_api.h", "include/game.h", "src/main/", "wiki/game-state.md", "wiki/data-types.md", "wiki/ref-english-patch.md", "config/renames/proposals/core.tsv"]
---

# Port spec: shared core (GameState, records, flow)

Unit `core` of phase 2 ([[tickets/T-27010-p2-core]], parent [[tickets/T-27000-phase-2]]). It describes the state and rules that every other unit reads, with the names proposed in `config/renames/proposals/core.tsv`. Old names are given in backticks where a reader needs the link to the current code. Facts are as the matched C implements them; a claim of game meaning that the code does not prove is marked HYPOTHESIS. No emulator run was used (the emulator image is not built in this tree); every statement comes from reading the matched C, the decoded data tables and the English-patch reference.

Scope (user decision of 2026-10-24): enough for the Godot port. Game logic, state and flow are described fully; the PS1 hardware layer (GPU, VRAM, sound driver, CD/XA, memory card, MDEC, movie) is named at module level with its entry points, section 9.

Table size: 742 rows (92 `func`, 39 `data`, 236 `field`, 75 `type`, 238 `const`, 60 `comment`, 2 `file`); 85 rows carry HYPOTHESIS. Reviewed once (4 blocking findings, fixed 2026-10-24); the cross-unit name list is in section 17.

How to read the table file: `kind scope old new note`. Apply `field` rows with the scope written as the type name *before* the `type` rows rename it (`Rec38`, `GsRec69C`, ...). `const` rows give `old` = literal, `new` = constant, `scope` = enum or mask group. `comment` rows carry an English one-liner for an O.BIN or patch name that stays.

## 1. The shape of the program

The main executable owns the frame loop, the shared state and a set of resident services. Every scene except a few resident ones lives in an overlay (26 files, one slot at 0x80132000, a few special cases) that main loads from disc on demand. All of them read and write one object, `game_state` (`D_800E6280`, 0x1A90 bytes, 12,306 uses in 295 files), and a second object right after it, `system_data` (`D_800E7D10`, 0xEE0 bytes).

Naming prefixes proposed for main (used by this table, to be followed by the module analysts): `main_state_/main_stage_/main_step_` for the machine, `mode_<name>` for resident mode handlers, `snd_` sound, `cd_` CD layer, `card_` memory card, `actor_/actors_` sprite actors, `sprite_`, `prim_` GPU primitives, `dec_bg_` decoded backgrounds (O.BIN), `text_`/`k_` text (O.BIN `k_*` kept), `message_window_`, `dialogue_`, `bustup_` portraits (O.BIN), `girl_`, `event_log_`, `day_event(s)_`, `calendar_`, `cursor_`. Overlay-own names take the overlay prefix in English form as the module analysts settle it.

## 2. Frame loop and the mode machine

`main_loop` (`func_80042134`): boot (`func_800420D0` clears 0x45498 bytes from 0x800E60A0, sets the stack, heap and gp), then init (`gpu_init`, CD ring reset, `game_state_init(1)`), then per frame:

1. `func_800422C8`: `draw_buffer` = the active buffer (0 or 1); point the draw environment and the primitive arena at that buffer (`SetWorkBase`); the shooter mode 0x90 uses other arenas.
2. If `paused` is 0, `main_state_dispatch()`, else the paused frame (`func_80043504`). Afterwards `mode_timer++`.
3. `frame_count++`.
4. Debug pad combinations (only while the debug flag `D_800B3220` is set): two pad bits held together enter `debug_vram_view` (mode 0xFE), another pair triggers `func_8004AD60(1)`.
5. If `restart_request` is 0: `frame_render(1)` (draw everything and present), `cd_cmd_pump()` and, while `sound_active` is set, `snd_command_pump()`.
6. When `restart_request` becomes nonzero the loop re-runs the boot-time initialisation (`gpu_init`, `game_state_init(1)`) and starts again.

`frame_render` also polls the input (section 6) and waits for vsync only while the elapsed horizontal-blank count is below `frame_hcount_limit` (0x106).

### 2.1 Four levels of state

| field | level | reset by |
|---|---|---|
| `mode` (`unk_1108`) | which scene runs | `main_state_set(m)`, `main_state_next()` |
| `stage` (`unk_1109`) | phase inside the mode | any mode change, `main_stage_set(s)`, `main_stage_next()` |
| `step` (`unk_110A`) | step inside the stage | any of the above, `main_step_set(n)`, `main_step_next()` |
| `substep` (`unk_110D`) | fourth level, private to a step function | any step change |

Counters that move with the levels: `mode_timer` (frames in the mode, reset by mode changes), `stage_timer` (reset by mode and stage changes), `step_timer` (reset by every change; overlays also add to it themselves, 582 uses), `mode_scratch` and `stage_scratch` (s8 scratch cleared with the mode or the stage).

Reset matrix (what each setter clears, besides writing its own value):

| call | stage | step | mode_timer | stage_timer | step_timer | mode_scratch | stage_scratch | substep |
|---|---|---|---|---|---|---|---|---|
| `main_state_set`, `main_state_next` | 0 | 0 | yes | yes | yes | yes | yes | yes |
| `main_stage_set`, `main_stage_next` | (written) | 0 | no | yes | yes | no | yes | yes |
| `main_step_set`, `main_step_next` | no | (written) | no | no | yes | no | no | yes |

`main_state_set` finishes with `main_state_request_overlay`: if the new mode is an overlay mode (the case list at 0x11, 0x13-0x15, 0x20-0x22, 0x35-0x37, 0x3A, 0x43, 0x45, 0x50-0x63, 0x70-0x76, 0x80-0x82, 0x90, 0x91, 0xC2-0xC4) the mode is stored in `overlay_target_mode` and `mode` becomes 2 (`MODE_OVERLAY_LOAD`). The loader state machine picks the disc position from the target mode (a switch with hard-coded sectors), reads 0x60 sectors to 0x80132000, verifies a 16-bit byte sum stored at the file end, sets `overlay_entry` (`D_80123110`) to the overlay entry for that mode and restores `mode = overlay_target_mode`. Resident modes never go through the loader. An overlay returns to the caller's context with the `return_mode/stage/step` triple (section 3.7). `game_reset_to_title()` is the universal error path (the `default:` of mode switches, 514 uses): reset loader, game state without clearing, sprite tables, sound, backgrounds and texture slots, then `main_state_set(0x12)`.

### 2.2 Mode table

`MainMode` (const rows). Resident handlers are named `mode_<name>` in the table; overlay modes list the overlay and entry. The queue type that leads to a mode is in section 3.5.

| mode | name | where | notes |
|---|---|---|---|
| 0x00 | `MODE_BOOT` | main `func_8005A06C` | loads the shared system textures (stage 0), then goes to the title |
| 0x01 | `MODE_BOOT_WAIT` | main | waits for `boot_hold`; HYPOTHESIS |
| 0x02 | `MODE_OVERLAY_LOAD` | main loader | section 2.1 |
| 0x03 | `MODE_CALL_ENTRY` | main | calls `overlay_entry` directly |
| 0x10 | `MODE_START` | main | initial mode; prepares the screen, then mode 0 |
| 0x11 | `MODE_PROLOGUE` | NAME_ENT 0x80144AD0 | voiced prologue; also started by the title idle timeout |
| 0x12 | `MODE_TITLE` | main `func_80062840` | four stages; stage 2 and later can time out to 0x11 |
| 0x13 | `MODE_NAME_ENTRY` | NAME_ENT 0x8014231C | new game: names, birthday, blood type |
| 0x14, 0x15 | `MODE_NAME_ENT_3/4` | NAME_ENT | roles not settled |
| 0x20-0x22 | `MODE_OPTION`, `_2`, `_3` | OPTION | 0x22 is also entered in-game when `D_800B3C6C` is set |
| 0x30 | `MODE_SCHEDULE` | main `schedule_init` | builds the queue for the coming week |
| 0x31 | `MODE_DAY_SCREEN` | main | shows the day, then `day_event_dispatch` |
| 0x32 | `MODE_WEEK_END` | main | weekly health check (section 5.2) |
| 0x34 | `MODE_COMMAND_SCREEN` | main `func_8005A0B0` | weekday command screen: the player picks the next week's command; reached when the queue reader hits the end marker 0xFF; stage 5 is club joining |
| 0x35-0x37 | `MODE_EXAM`, `MODE_CAREER_GUIDANCE`, `MODE_CAREER_CHOICE` | ETC | final exams and rank list; career talks; job or college choice |
| 0x38, 0x39 | `MODE_MONTH_START`, `MODE_DAY_NOTICE` | main | queue types 0xFE and 0xFD |
| 0x3A | `MODE_HELP` | OLH | on-line help |
| 0x40 | `MODE_SCHOOL_DAY` | main `week_day` | school-day command screen and the day's parameter changes |
| 0x41, 0x42 | `MODE_NEW_YEAR_PREP`, `MODE_CHRISTMAS_PREP` | main | pre_syogatu, pre_xmas |
| 0x43, 0x45 | `MODE_TELEPHONE`, `MODE_TELEPHONE_PROFILE` | TEL | calls; profile page (HYPOTHESIS for 0x45) |
| 0x44 | `MODE_RUMOR` | main `uwasa_main` | rumour report |
| 0x46 | `MODE_HOLIDAY` | main | holiday command screen, telephone, club joining, appointments |
| 0x50 | `MODE_DATE` | DATE | an appointment starts here; 0x5E hosts the girl-specific scenes |
| 0x51 | `MODE_SCHOOL_TRIP` | SHUGAKU | |
| 0x52, 0x53, 0x55 | `MODE_NEW_YEAR_A`, `_B`, `MODE_SEASONAL_PARTY` | SHOUGATU | New Year and party flows (HYPOTHESIS names: the English form of the overlay prefix); 0x5B `MODE_MEETING` (HYPOTHESIS name: SHOUGATU hosts generic scenes there) is also in SHOUGATU |
| 0x54 | `MODE_VALENTINE` | VALEN | |
| 0x56, 0x57 | `MODE_PLAYER_BIRTHDAY`, `MODE_GIRL_BIRTHDAY` | GEKO | |
| 0x58, 0x5C | `MODE_AMUSEMENT_PARK`, `_2` | DATE2 | |
| 0x59, 0x5A, 0x5D | `MODE_GIRL_ENCOUNTER`, `MODE_GIRL_INVITATION`, `MODE_SPECIAL_EVENT` | GEKO | |
| 0x5F, 0x60 | `MODE_CLUB_CAMP`, `_2` | KANGEI | HYPOTHESIS names: the overlay is rated low confidence; queue type 0x42 comes six in a row in the club season |
| 0x61, 0x62 | `MODE_SICK_VISIT`, `_2` | OMIMAI | |
| 0x63 | `MODE_ENDING` | ENDING | |
| 0x70, 0x71 | `MODE_CULTURE_FESTIVAL_A/B` | BUNKASAI / BUNKAKEN | chosen by the player's club: id < 6 gets A |
| 0x72 | `MODE_SPORTS_FESTIVAL` | TAIIKU | |
| 0x73 | `MODE_PRACTICE_MATCH` | RENSYU | |
| 0x74-0x76 | `MODE_CLUB_MASTER_A/B/C` | MASTER | HYPOTHESIS for the meaning |
| 0x80-0x82 | `MODE_CULTURE_STAGE`, `MODE_RPG_BATTLE`, `MODE_FESTIVAL_STALLS` | BUNKA_SD, RPG_BAT, EN_NICHI | |
| 0x90, 0x91 | `MODE_SHOOTER`, `MODE_ALBUM` | TACO, ETC | extras |
| 0xC2-0xC4 | `MODE_ETC_C2/C3/C4` | ETC | album/memory-card flows after the ending |
| 0xD0 | `MODE_TT_GAME` | TT | own loader |
| 0xFE, 0xFF | `MODE_DEBUG_VRAM_VIEW`, `MODE_EXIT` | main | |

EVENT and GYOZI are never loaded (no loader builds their sectors); they were linked against another main layout, so their calls to main addresses are stale. Their uses are excluded from every count in this spec.


### 2.3 GameState map (all 161 top-level members by offset)

Old name, new name, use count (direct `D_800E6280.` reads and writes in `src/`, EVENT and GYOZI excluded; accesses through a `GameState *` such as `g->` are not counted, so the counts undercount fields that overlays reach through a pointer: `current_girl` is named on about 1,650 lines in all), evidence. `kept` means no proven role: unused or unknown bytes. Sub-record fields are in the table file.

| offset | type | old | name | uses | note |
|---|---|---|---|---|---|
| 0x000 | u16 | `unk_000` | `video_width` | 13 | passed with unk_002/unk_004 to the display-mode call in func_8004F6B0; 0x140 at init, 0x100/0x200 set in func_80058D20 (3D init) |
| 0x002 | u16 | `unk_002` | `video_height` | 7 | 0xF0 at init and in func_80058D20; passed to the display-mode call |
| 0x004 | u16 | `unk_004` | `video_interlace` | 6 | HYPOTHESIS: third argument of the display-mode call, 0 everywhere it is set |
| 0x006 | u8[0xE] | `unk_006` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x014 | s16[2] | `unk_014` | `screen_center_x` | 114 | index = draw buffer (D_8011ECA0); 0xA0 = 160 at init, shaken by func_80047560; sprite and primitive x are written as... |
| 0x018 | s16[2] | `unk_018` | `screen_center_y` | 172 | index = draw buffer; 0x168 for buffer 0 and 0x78 for buffer 1 at init (VRAM y of the buffer centre), shaken by func_80047560 |
| 0x01C | GsRec01C[2] | `unk_01C` | `prim_arena` | 6 | SetWorkBase/GetWorkBase: per draw buffer a bump allocator for GPU primitives (base, limit 0x10C00, used) |
| 0x034 | u8 | `unk_034` | `sound_active` | 2 | main loop calls func_80079B34 (sound command pump) only while it is nonzero; 1 at init |
| 0x035 | u8 | `unk_035` | `card_busy` | 26 | set by func_80053CE0, cleared by func_80053D10; gates the memory-card step machine func_80053F04 |
| 0x036 | u8 | `unk_036` | `clear_color_r` | 66 | passed with unk_037/unk_038 as r,g,b to the draw-environment clear call (func_8009824C) when back_clear is on; 0xFF = white on... |
| 0x037 | u8 | `unk_037` | `clear_color_g` | 38 | see clear_color_r |
| 0x038 | u8 | `unk_038` | `clear_color_b` | 44 | see clear_color_r |
| 0x039 | u8 | `unk_039` | `back_clear` | 3 | set by back_clear_switch(arg): clear the draw buffer with clear_color each frame |
| 0x03A | u8 | `unk_03A` | `screen_brightness` | 158 | func_8004901C returns it; 0x80 = neutral (func_8004902C sets it); func_80049044 steps it by brightness_step per frame, clamped... |
| 0x03B | u8 | `unk_03B` | `brightness_step` | 7 | s8 fade speed per frame added to screen_brightness by func_80049044 (func_800490B4 sets it) |
| 0x03C | u8 | `unk_03C` | `voice_mode` | 52 | OPTION/main settings: 0 always off, 1 always on, 2 on but cancelled by the cross button; dialogue and voice code test it |
| 0x03D | u8 | `unk_03D` | `vacation` | 8 | 0 school term, 1 summer break, 2 winter break, 3 spring break; set by func_800726F0 on the day before a break; selects the... |
| 0x03E | u8 | `unk_03E` | `year` | 414 | 95/96/97 = 1995/1996/1997 school years, 98 = final months; starts at 0x5F; compared 0x5F/0x60/0x61/0x62 in the event builders |
| 0x03F | u8 | `unk_03F` | `month` | 370 | 1-12; starts at 4; December rolls to month 1 and year + 1 in func_80072338 |
| 0x040 | u8 | `unk_040` | `day` | 125 | day of month 1-31; starts at 4 |
| 0x041 | u8 | `unk_041` | `weekday` | 37 | 0 = Monday .. 6 = Sunday; starts at 1 (Tuesday 1995-04-04); calendar sprite frame is 0x52 + weekday |
| 0x042 | u16 | `unk_042` | `day_count` | 18 | u16 days elapsed since the start; ++ in the day advance; compared 0x2D6/0x2DA (730) and /365 for the years |
| 0x044 | GsRec044[2] | `unk_044` | `month_calendar` | 52 | two MonthCalendar records: this month and next month (day kinds and event markers per day) |
| 0x0C4 | u8[0x10] | `unk_0C4` | `player_full_name` | 7 | family + given name concatenated by NAME_ENT (func_80138C1C); initial text is a placeholder; saved in the 0x7C-byte profile block |
| 0x0D4 | u8[8] | `unk_0D4` | `player_family_name` | 190 | 8-byte buffer, up to 3 characters; copied to the {p} name buffer by func_800847B8; shown by gnsx |
| 0x0DC | u8[8] | `unk_0DC` | `player_given_name` | 21 | 8-byte buffer, up to 3 characters; {m} buffer |
| 0x0E4 | u8[0x10] | `unk_0E4` | `player_nickname` | 10 | 16-byte buffer, up to 5 characters; used by get_p_name for the nickname form |
| 0x0F4 | GsHalf | `unk_0F4` | `player_flags` | 232 | word view PlayerFlagBits (ScheduleFlags): bits 8-11 health state, 12-15 club, 16-20 flags, 21-23 recovery counter; OPTION... |
| 0x0F6 | GsHalf | `unk_0F6` | `player_flags_hi` | 76 | upper halfword of player_flags: byte 0 = bits 16-23, byte 1 = career bits written by ETC and read by ENDING |
| 0x0F8 | s32 | `unk_0F8` | `player_birthday` | 65 | GsDateBits word: month bits 0-3, day bits 4-8, blood type bits 9-10; initial 1/1; compared by birth_day_check(0xF) |
| 0x0FC | GsRec0FC | `unk_0FC` | `param_condition` | 21 | parameter 0 = condition; record {frac,value}; value capped 0..999 by check_para_limit |
| 0x100 | GsRec0FC | `unk_100` | `param_liberal_arts` | 22 | parameter 1; order = table at 0x800B32D0 (condition, liberal arts, science, art, athletics, trivia, looks, guts, stress) |
| 0x104 | GsRec0FC | `unk_104` | `param_science` | 17 | parameter 2 |
| 0x108 | GsRec0FC | `unk_108` | `param_art` | 20 | parameter 3 |
| 0x10C | GsRec0FC | `unk_10C` | `param_athletics` | 24 | parameter 4 |
| 0x110 | GsRec0FC | `unk_110` | `param_trivia` | 22 | parameter 5 |
| 0x114 | GsRec0FC | `unk_114` | `param_looks` | 20 | parameter 6 |
| 0x118 | GsRec0FC | `unk_118` | `param_guts` | 20 | parameter 7 |
| 0x11C | GsRec0FC | `unk_11C` | `param_stress` | 21 | parameter 8 |
| 0x120 | u8[0x9C] | `unk_120` | `progress_state` | 76 | HYPOTHESIS: 0x9C bytes = 21 one-shot flag bytes (+0..0x14, set by HUD code), 9 command counters (+0x15, ++ per chosen... |
| 0x1BC | Rec38[16] | `unk_1BC` | `girls` | 1128 | 16 GirlRecords indexed by girl id (GIRL_*); 0-10 heroines, 11 Miharu, 12 Rei, 13/14 unused here |
| 0x53C | u8[0x10] | `unk_53C` | `girl_cleared` | 53 | HYPOTHESIS: per-girl flag; the mascot menu lists girl i when this or the saved SystemData.flags[i] is set; ETC graduation... |
| 0x54C | GsSlot54C | `u54C` | `appointment_slots` | 0 | 8 appointment words (date promises), union AppointmentSlots; macros unk_54C -> appointments, unk_54C_0 -> next_appointment |
| 0x56C | u8[0xFC] | `unk_56C` | `event_flags` | 71 | one-shot event and progress bytes indexed by event number (set by DATE/GEKO/SHOUGATU/main); saved as a 0x100-byte block |
| 0x668 | u8[4] | `unk_668` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x66C | GsRec66C[12] | `unk_66C` | `clubs` | 71 | 12 ClubRecords indexed by club id (CLUB_*), including CLUB_NONE |
| 0x69C | GsRec69C[32] | `unk_69C` | `day_events` | 115 | 32 DayEvent records: the queue of events for the coming days, built by the weekly queue builder (func_8006D7EC) |
| 0x71C | u8 | `unk_71C` | `day_event_index` | 103 | read position in day_events; advanced by day_event_next |
| 0x71D | u8 | `unk_71D` | `events_today` | 6 | HYPOTHESIS: ++ each time a girl encounter record (types 0x44/0x46/0x47) is run; cleared by func_8007259C each day |
| 0x71E | u8 | `unk_71E` | `birthday_done_bits` | 70 | bit 1 = Shiori's birthday scene played, bit 2 = another girl's, bit 4 = the player's (set by the birthday overlays); the queue... |
| 0x71F | u8 | `unk_71F` | `daily_check_done` | 12 | HYPOTHESIS: set to 1 before the telephone/appointment prompt (mode 0x43) and by the day-start code; cleared when a new queue... |
| 0x720 | u8 | `unk_720` | `return_mode` | 38 | saved main mode to resume after a sub-scene (set from mode, restored with main_state_set) |
| 0x721 | u8 | `unk_721` | `return_stage` | 59 | saved stage, restored with main_stage_set |
| 0x722 | u8 | `unk_722` | `return_step` | 49 | saved step (often step + 1), restored with main_step_set |
| 0x723 | u8 | `unk_723` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x724 | u8 | `unk_724` | `appt_place_0` | 19 | HYPOTHESIS: recent appointment places; unk_725 is the newest (appointment_launch shifts the others), compared with unk_724 to... |
| 0x725 | u8 | `unk_725` | `appt_place_1` | 18 | see appt_place_0 |
| 0x726 | u8 | `unk_726` | `appt_place_2` | 12 | see appt_place_0 |
| 0x727 | u8 | `unk_727` | `appt_place_3` | 13 | see appt_place_0 |
| 0x728 | u8[4] | `unk_728` | `date_place_history` | 1 | HYPOTHESIS: 4 places of the latest dates; DATE compares [0]==[1]==chosen place to detect a repeated place (sets player_flags... |
| 0x72C | u8[0x10] | `unk_72C` | `girl_last_date_spot` | 5 | date spot id (`D_80122D08`, the value that also indexes `event_counters`; not the `Place` enum) of the last date with each girl (loops of 11); GEKO uses it as a ban-list index; rule in section 5.4 |
| 0x73C | u8[0x10] | `unk_73C` | `girl_last_date_spot_timer` | 2 | weeks left in which that spot counts as a repeat: set to 12 by DATE, lowered once per queue build (weekly) by `last_date_spot_timer_dec` |
| 0x74C | u8[0x10] | `unk_74C` | `girl_event_bits` | 7 | HYPOTHESIS: one byte per girl; GEKO tests/sets bits 1, 2, 4 for scenes 0xE, 0xF, 7 |
| 0x75C | u8 | `unk_75C` | `appointment_flag` | 4 | copy of appointment_slots[0] bit 24, stored by appointment_launch |
| 0x75D | u8 | `unk_75D` | `date_girl` | 90 | girl of the current date or event (copied from current_girl at launch); read by DATE, KANGEI, SHUGAKU, DATE2 |
| 0x75E | GsRec75E[256] | `unk_75E` | `event_log` | 85 | 256 EventLogEntry records (girl, scene, rank, date) appended by event_log_add |
| 0xF5E | u8 | `unk_F5E` | `event_log_count` | 126 | number of valid entries in event_log, capped at 0xFE |
| 0xF5F | u8 | `unk_F5F` | `current_girl` | 1219 | index of the girl record in use (1,219 uses); set by select_girl/change_girl and by the event launchers |
| 0xF60 | u8 | `unk_F60` | `text_lines_total` | 5 | HYPOTHESIS: count of dialogue text entries (check_set_k) used by the line-scroll step func_8005215C |
| 0xF61 | u8 | `unk_F61` | `text_first_line` | 30 | HYPOTHESIS: index of the first visible dialogue line in the text list |
| 0xF62 | u8 | `unk_F62` | `text_scroll_state` | 22 | HYPOTHESIS: bit 7 = scrolling active, low bits = phase of the two-line scroll |
| 0xF63 | s8 | `unk_F63` | - (kept) | 1 | unused or not proven (see open questions) |
| 0xF64 | u8[4] | `unk_F64` | `quarter_flags` | 31 | HYPOTHESIS: FlagsF64 bit view: set by func_8006A044 by year and month group; at months 3/6/9/12 bit 14 takes bit 13... |
| 0xF68 | GsWord | `unk_F68` | `last_command` | 51 | low nibble = the weekday command chosen on the schedule screen (icon index; 0-7 = row of command_deltas, 4 = club activity;... |
| 0xF6C | u8 | `unk_F6C` | `icon_mode` | 23 | settings: 0 = TYPE-A, 1 = TYPE-B, 2 = free layout (strings of the settings screen) |
| 0xF6D | u8 | `unk_F6D` | `icon_count` | 18 | number of icon actors shown (0xF in free mode, 0xE otherwise) |
| 0xF6E | u8 | `unk_F6E` | `sd_anim_fast` | 10 | toggled by pad 2 START (bit 27) in func_80042A00; the in-game hint calls it the SD character animation speed switch; nonzero sets `unk_06` to 0x82 on actors 96 and 97 of the week and holiday screens (HYPOTHESIS: the SD characters) and shortens the parameter-panel pop animation from 8 to 4 frames; saved with the game |
| 0xF6F | u8 | `unk_F6F` | `cursor_speed` | 21 | settings 1-5, default 3; step of the pad-driven cursor in func_8006BA40 |
| 0xF70 | u8 | `unk_F70` | `click_mode` | 10 | mouse setting: 0 single click, 1 double click |
| 0xF71 | s8 | `unk_F71` | - (kept) | 1 | unused or not proven (see open questions) |
| 0xF72 | u8 | `unk_F72` | - (kept) | 0 | unused or not proven (see open questions) |
| 0xF73 | u8 | `unk_F73` | `double_click_interval` | 18 | mouse setting in frames: default 0x20, clamped 0x10..0x40 |
| 0xF74 | u8 | `unk_F74` | `input_device` | 30 | 0 = mouse, 1 = pad, 0xFF = none (from the controller id byte in func_80042AC8) |
| 0xF75 | u8 | `unk_F75` | `confirm_button` | 19 | mouse setting: 1 = right button confirms, 0 = left button |
| 0xF76 | u8 | `unk_F76` | - (kept) | 0 | unused or not proven (see open questions) |
| 0xF77 | u8 | `unk_F77` | `mouse_buttons` | 4 | mouse button bits (4 = right, 8 = left) tested with confirm_button |
| 0xF78 | u8 | `unk_F78` | - (kept) | 0 | unused or not proven (see open questions) |
| 0xF79 | u8 | `unk_F79` | - (kept) | 0 | unused or not proven (see open questions) |
| 0xF7A | s16 | `unk_F7A` | `mouse_x` | 3 | s16, cleared when the device changes |
| 0xF7C | s16 | `unk_F7C` | `mouse_y` | 3 | s16 |
| 0xF7E | u8[2] | `unk_F7E` | - (kept) | 0 | unused or not proven (see open questions) |
| 0xF80 | s32 | `unk_F80` | `pad_held` | 165 | buttons held (pad 1 low half, pad 2 high half, PsyQ bit layout; mouse buttons are folded in) |
| 0xF84 | s32 | `unk_F84` | `pad_prev` | 6 | previous frame's pad_held |
| 0xF88 | s32 | `unk_F88` | `pad_pressed` | 355 | ~pad_prev & pad_held: buttons newly pressed this frame (355 uses) |
| 0xF8C | s32 | `unk_F8C` | `pad_released` | 1 | pad_prev & ~pad_held |
| 0xF90 | GsRecF90[32] | `unk_F90` | `menu_items` | 14 | 16 x 2 MenuItem rects (menu 0 and 1) set by menu_set, tested by menu_check |
| 0x1090 | u8 | `unk_1090` | `menu_count_0` | 4 | item count of menu 0 (MenuSelection view at +0x1090) |
| 0x1091 | u8 | `unk_1091` | `menu_count_1` | 0 | item count of menu 1 |
| 0x1092 | u8 | `unk_1092` | `menu_mode` | 22 | settings: 0 mouse-like, 1 menu A, 2 menu B, 3 mouse-like + A (low 2 bits) |
| 0x1093 | s8 | `unk_1093` | `menu_cursor_0` | 100 | pad cursor item of menu 0 (-1 none) |
| 0x1094 | s8 | `unk_1094` | `menu_cursor_1` | 118 | pad cursor item of menu 1; settings screens switch on it after confirm |
| 0x1095 | s8[0xD] | `unk_1095` | `menu_hit` | 10 | menu_hit[menu] = item under the pointer/cursor (-1 none); bytes [3..6] double as the s32 actor_draw_last |
| 0x10A2 | s8 | `unk_10A2` | `tile_layer_enabled` | 46 | func_80043510 calls srn_set(0) while nonzero; cleared by func_80085E30 |
| 0x10A3 | u8 | `unk_10A3` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x10A4 | u8 | `unk_10A4` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x10A5 | u8[0x43] | `unk_10A5` | `quad_layer` | 56 | 0x43 bytes, one per SpriteQuad: bit 7 picks the primitive variant, low bits (v-1) the ordering-table depth |
| 0x10E8 | s32 | `unk_10E8` | `quads_enabled` | 39 | func_80043510 draws the SpriteQuad table and BG layers while nonzero; set to 1 by scene setup |
| 0x10EC | u8[4] | `unk_10EC` | `bg_layer_order` | 4 | 4 bytes, one per BgLayer slot; 0x80 = hidden, else ordering depth + 1 |
| 0x10F0 | s32 | `unk_10F0` | `frame_hcount_limit` | 1 | HYPOTHESIS: 0x106 scanline count; the frame waits for vsync only while the elapsed count is below it |
| 0x10F4 | s32 | `unk_10F4` | `timer_ticks` | 19 | ++ in the root-counter event callback (func_800423D4); used as the srand seed and for movie timing |
| 0x10F8 | u32 | `unk_10F8` | `frame_count` | 189 | ++ once per main-loop iteration; used for blinking, random jitter and periodic checks (% 240) |
| 0x10FC | s32 | `unk_10FC` | `mode_timer` | 5 | ++ after each mode dispatch, reset by every mode change |
| 0x1100 | s32 | `unk_1100` | `stage_timer` | 74 | reset by mode and stage changes; overlays count frames in it |
| 0x1104 | GsWord | `unk_1104` | `step_timer` | 582 | frames in the current step; reset by every step, stage and mode change (582 uses) |
| 0x1108 | u8 | `unk_1108` | `mode` | 55 | main state; MainMode; set by main_state_set; dispatched by main_state_dispatch |
| 0x1109 | u8 | `unk_1109` | `stage` | 289 | stage inside the mode; main_stage_set / main_stage_next |
| 0x110A | u8 | `unk_110A` | `step` | 1152 | step inside the stage; main_step_set / main_step_next (1,152 uses) |
| 0x110B | s8 | `unk_110B` | `mode_scratch` | 38 | s8, cleared on mode change; BUNKA_SD uses it as a brightness value |
| 0x110C | s8 | `unk_110C` | `stage_scratch` | 29 | s8, cleared on mode and stage change; ETC/BUNKA_SD counters |
| 0x110D | u8 | `unk_110D` | `substep` | 1535 | cleared by step changes; the fourth level of the machine (1,535 uses; TEL, NAME_ENT, ETC, OLH switch on it) |
| 0x110E | u8 | `unk_110E` | `overlay_target_mode` | 4 | mode that asked for an overlay; main_state_request_overlay stores it and runs mode 2 (loader); the loader returns to it |
| 0x110F | u8 | `unk_110F` | `restart_request` | 3 | main loop leaves the frame loop and re-initialises when nonzero |
| 0x1110 | u8 | `unk_1110` | `paused` | 5 | frame handler runs func_80043504 instead of the mode dispatcher while 1 |
| 0x1111 | u8 | `unk_1111` | `pause_allowed` | 1 | pad polling toggles paused on a pad bit only while 1 |
| 0x1112 | u8 | `unk_1112` | `actors_draw_enabled` | 4 | func_80043510 calls actors_draw while it is 1 (set by actors_draw_enable) |
| 0x1113 | u8 | `unk_1113` | `draw_3d_enabled` | 3 | draw2d3d second argument; func_80043510 runs the GTE object pass while nonzero |
| 0x1114 | u8 | `unk_1114` | `draw_2d_enabled` | 2 | draw2d3d first argument; gates the sprite-quad pass |
| 0x1115 | u8 | `unk_1115` | `card_result` | 81 | memory-card event result (0x81..0x88 events, 0xF0/0xFF final codes) |
| 0x1116 | u8 | `unk_1116` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1117 | u8 | `unk_1117` | `album_slot` | 29 | index of the selected album/save slot (NameRecordF00 index) |
| 0x1118 | u8 | `unk_1118` | `save_slot` | 107 | memory-card slot number used by func_80054884 |
| 0x1119 | u8 | `unk_1119` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x111A | s8 | `unk_111A` | `boot_hold` | 2 | HYPOTHESIS: mode 1 waits until it is 0 and then starts mode 0 |
| 0x111B | u8 | `unk_111B` | `card_job` | 10 | 1 while an asynchronous memory-card save/load started from the menu is pending; the poller ends it on result 0x13 (done) or... |
| 0x111C | u8 | `unk_111C` | `card_step` | 8 | step of the memory-card state machine (func_80053F04) |
| 0x111D | u8 | `unk_111D` | `card_retry_count` | 5 | failed attempts; 3 or more aborts |
| 0x111E | u8[2] | `unk_111E` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1120 | s32 | `unk_1120` | `card_timer` | 45 | frames since the card operation began; > 0x200 = timeout |
| 0x1124 | u8 | `unk_1124` | `options_dirty` | 27 | bit mask of changed option groups (0x80/0x40/0x20/0x10/8); nonzero makes NAME_ENT/ETC save the settings |
| 0x1125 | u8[3] | `unk_1125` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1128 | s32[64] | `unk_1128` | `tpage_req` | 16 | 64 per-frame texture-page slot states: 0x7FFF free, 0x7FFE resident (copied from tpage_buf by search_tpage(0x7FFF)), else the... |
| 0x1228 | s32[64] | `unk_1228` | `tpage_buf` | 210 | 64 texture-page slot contents (resource ids; 0 reserved, -1 free); tpage_buf_clear |
| 0x1328 | GsRec1328[32] | `unk_1328` | `palette_queue` | 0 | 32 PaletteLoad records queued by load_palette and flushed by palette_load_vram |
| 0x1428 | u8 | `unk_1428` | `palette_queue_count` | 1 | fill count of palette_queue |
| 0x1429 | u8 | `unk_1429` | `csr_queue_count` | 2 | fill count of csr_queue |
| 0x142A | u8[2] | `unk_142A` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x142C | GsRec142C[33] | `unk_142C` | `csr_queue` | 0 | 33 CsrLoad records queued by load_csr_ab/load_csr_tp and flushed by csr_load_vram |
| 0x163C | s32[128] | `unk_163C` | `scratch_words` | 57 | HYPOTHESIS: 128-word scratch used by TACO and the VRAM loaders; its first 0x50 bytes also hold the saved free-icon layout... |
| 0x183C | u8[0x200] | `unk_183C` | - (kept) | 11 | unused or not proven (see open questions) |
| 0x1A3C | s32 | `unk_1A3C` | - (kept) | 12 | unused or not proven (see open questions) |
| 0x1A40 | u8[0xC] | `unk_1A40` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1A4C | s32 | `unk_1A4C` | - (kept) | 10 | unused or not proven (see open questions) |
| 0x1A50 | u8[0xC] | `unk_1A50` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1A5C | s32 | `unk_1A5C` | - (kept) | 10 | unused or not proven (see open questions) |
| 0x1A60 | u8[0xC] | `unk_1A60` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1A6C | s32 | `unk_1A6C` | - (kept) | 7 | unused or not proven (see open questions) |
| 0x1A70 | u8[0xC] | `unk_1A70` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1A7C | s32 | `unk_1A7C` | - (kept) | 2 | unused or not proven (see open questions) |
| 0x1A80 | u8[4] | `unk_1A80` | - (kept) | 0 | unused or not proven (see open questions) |
| 0x1A84 | s32 | `unk_1A84` | - (kept) | 2 | unused or not proven (see open questions) |
| 0x1A88 | u8[8] | `unk_1A88` | - (kept) | 0 | unused or not proven (see open questions) |

## 3. Calendar, day structure and the event queue

### 3.1 Date

`year/month/day/weekday` are bytes: year 95, 96, 97 (the three school years; the last months are year 98), month 1-12, day 1-31, weekday 0 (Monday) to 6 (Sunday). Start: 1995-04-04, a Tuesday (weekday 1). `calendar_advance_day` adds one day: weekday wraps at 7; month length 31 for months 1,3,5,7,8,10,12, 30 for 4,6,9,11, February 29 days in year 96 and 28 otherwise; after December the month is 1 and the year +1. `day_count` (u16) is incremented once per advance (days since the start; the code uses 730 and `/365` as year boundaries). In appointment and birthday checks month 13 means January of the following year.

From year 98, month 3 (day != 0) the queue reader stops dispatching and calls `main_step_next`; step 1 of the day screen then loads the ending sound bank and starts `MODE_ENDING` (0x63). The graduation-day scenes before that (exam rank list, career choice, confession counting) are ETC modes selected by queue types.

### 3.2 Month tables

`month_calendar[2]` holds this month and next month. Each record has `day_kind[0x20]` (`[0]` = days in the month, `[d]` = day kind) and `marker[0x20]` (`[0]` = weekday of the 1st, `[d]` = marker). At the end of a month the second record moves to the first and the new second record is loaded from `month_data_table` (`D_800B3748`, 36 month blobs indexed `(year - 95) * 12 + month - 4`; each blob is a 0x2000-byte calendar image followed by words: the day count, the first weekday, then n day kinds and n markers, of which only the low byte is kept). The initial table (April and May 1995) is written by `func_800418B0`. The blobs live in memory loaded from disc, not in the executable: **the port must read them from the disc data**. Mode 0x38 (`MODE_MONTH_START`) loads them at every month start: step 0 reads 0x95 sectors from LBA 0x9B94 to 0x801A0000, step 1 calls `func_80068938(year, month, 1)`, and the mode shows its screen only when the month is divisible by 3 (the new-term message in September, except on 1 September 1996, where it just continues). `func_80068938` loads blobs `idx` and `idx + 1` only while `idx < 35`.

Day kinds: 0 school day, 1 Sunday, 2 national holiday, 3 break day (HYPOTHESIS: events of the summer and winter breaks are gated on kind 3). Markers (HYPOTHESIS for names that are not tied to an event): 2/6/10/14/16/20 push a day-notice record, 4 sports festival, 5 exam period, 8 club season, 11 school trip, 12 culture festival, 13 Christmas preparations, 15 New Year preparations, 17 Valentine's day, 18 career choice (year 98), 19 seasonal party.

### 3.3 Vacation flag

`vacation` is 0 in term, 1 summer, 2 winter, 3 spring. `func_800726F0` (stage 0 of the notice mode 0x39) ends a running break with a new-term message, or starts the summer, winter or spring break when the month is 7, 12 or 3 (message that the break starts tomorrow); the value picks the break background and music. Affection decay and syoushin growth run only while it is 0 (`schdeule_girls_param`).

### 3.4 Commands and the weekly queue builder

The player picks a command for the coming week on the schedule screen: `last_command` low nibble is the weekday command (an icon index; commands 0-7 have a row in `command_deltas`, command 4 is club activity, 0xE means none chosen yet), bits 4-7 hold the last holiday command. `parameter_up_down(command, flag, half)` applies a command: for each of parameters 0-7 the delta comes from row `command` of `command_deltas` (signed bytes, 9 per row; the club command uses `club_activity_deltas[club]`): a positive delta is multiplied by `0x1000 * (4 if flag else 1) / (2 if half else 1) / div_a / div_b`, a delta of zero or less is `delta << 12` and is never scaled; stress (index 8) mirrors this: a negative delta (relief) gets the scale factor, a delta of zero or more is `delta << 12`, and one whole point is added to stress when `half` is set. `div_a` is 2 when the player is sick (health 3); `div_b` is 2 for injured or neurotic (health 1 or 2), except when the command id (the first argument, not the parameter index) is 5, 6 or 7. `parameter_change(i, delta)` adds the 16.16 delta to parameter i, floors it at 0 and caps the value at 999. (HYPOTHESIS: command 7 is rest; it selects a sick-bed picture when the player is sick.)

The command screen (mode 0x34) ends in `MODE_SCHEDULE` (`schedule_init`), which runs five steps once per queue build: 0 preparation (`func_8006CD14`), 1 `day_events_build_week`, 2 `schedule_girls_gakko` (resolve encounter placeholders), 3 `schdeule_girls_param` (the weekly girl decay of section 5.3), 4 `func_80070ECC` (`calendar_advance_day`, stop CD audio when it plays, mode 0x31). `day_events_build_week` (`func_8006D7EC`) fills `day_events` from tomorrow until Sunday. Order of its work, as coded:

1. Increment the command counter `progress_state[0x15 + command % 9]`, count down girl 11's `event_counters[0]`, count met girls 1-9, decrement the 3-bit `recovery_days`, clear the queue (`day_events_clear`: type 0xFF everywhere, index 0).
2. If the next day is the first of a month, put `DAY_EVENT_MONTH_START` first.
3. Marker 11 at tomorrow: a school-trip record followed by a holiday record, and the builder returns (the trip week replaces the rest of the queue).
4. Marker 8 while the command is club activity and the player has a club: six tournament records (0x25) when the player is in club 6 and the club level nibble is at least 8, otherwise six records of type 0x42 (`DAY_EVENT_CLUB_CAMP`, HYPOTHESIS name) with `unk_01 = 4`; either way the builder returns after a holiday record.
5. For each remaining day until Sunday: notice records for the notice markers; marker-driven festival records (sports 0x20, culture 0x24, Christmas prep 0x28, New Year prep 0x2C, Valentine 0x31, seasonal party 0x30); year-specific markers (year 96: meeting with girl 10, year 97: career guidance, year 98: career choice 0x4A); holidays and Sundays become holiday records (0x10) unless the late-game special event fires; the year-97 January 3 award checks produce 0x26 or 0x27; exam markers and exam weeks produce 0x40; ordinary school days produce a school-day record (type 0, girl field = command, scene field = result of the command check `func_8006BDA8`, extra = girl selection from `func_8006F524`).
6. On ordinary school days with no special event: random sick visits (0x47; girl 12 on weekdays, on Saturdays girl 9 or the selected girl), the summer amusement-park event (0x49) on a break Wednesday in August with probability 20%, the meeting selectors (`func_8006EB64`, `func_8006EEC4`, `func_8006F218`) producing 0x48 records before `day_count` 726 (0x2D6), and up to two random encounter placeholders per week (0x44; each school day with probability 1/4).
7. For each day: girl birthdays (0x32) for met girls with a set bit-6 flag, the player's birthday (0x33) unless ill, a week-end record (0xFC) after Saturday and a day-end record (0xFB) every day except Sunday.

The exact random thresholds are in the matched C (`func_8006D7EC`); they are not repeated here.

### 3.5 The queue reader

`day_event_dispatch` (`func_8007132C`, run by the day screen, stage 0 step 0) first applies fixed rules (from 1998-03 on it hands over to the ending, section 3.1; two fixed late-February specials; a forced encounter (mode 0x59) with a met girl whose syoushin is at least 0x4C and whose bit 16 is still clear (the reader sets bit 16 when it starts the encounter, and `girl_counters_clamp` clears it again while her syoushin is below 0x32, see below); skipping birthday records whose `birthday_done_bits` bit is set; appointment prompt: if the first appointment is today it opens the telephone mode with stage 2 when the girl's state rank is 4, 0 when the place history changed, 1 otherwise), then switches on `day_events[day_event_index].type`. Type 0xFA is skipped. Type 0xFB (end of a day) never reaches the switch: the reader checks for telephone calls (mode 0x43 with a stage code 3-9 chosen by date and girl state), for a rumour report (mode 0x44 when a met girl has `syoushin` of 0x3D or more and her flag bit 17 is clear; the reader sets bit 17 for every such girl in the pass, and `girl_counters_clamp` clears it while syoushin is below 0x32), then does `day_event_index++` and `calendar_advance_day()` and continues with the next record. The type 0xFF at the end of the queue also runs the Sunday telephone checks before it goes to the command screen.

Every pass of the reader starts with two housekeeping calls. `func_80071280` (`girl_flag4_may_clear`) clears flag bit 4 of girls 0-10 on May 6 (HYPOTHESIS: a once-a-year May flag; DATE sets it on a May 3-4 date and a date on May 4-5 gives a girl that has it 2 extra tokimeki). `func_80071038` (`girl_counters_clamp`) clamps tokimeki, affection and syoushin of girls 0-10 to 0..0x80 and clears flag bits 16 and 17 whenever the girl's syoushin is below 0x32. So the forced encounter and the rumour are not one-time events: both re-arm each time a girl's syoushin falls below 0x32 and later climbs again, and the effective syoushin cap seen by the rules is 0x80, not the 0xFF of `check_para_limit`.

| type | mode | note |
|---|---|---|
| 0x00 | 0x40 | before the switch, only the command icon of the record's command (girl field) is enabled (14 icons) |
| 0x10 | 0x46 | holiday |
| 0x20 | 0x72 | sports festival |
| 0x24 | 0x70 or 0x71 | by the player's club |
| 0x25-0x27 | 0x74-0x76 | MASTER |
| 0x28, 0x2C | 0x42, 0x41 | Christmas, New Year preparations |
| 0x30, 0x31 | 0x55, 0x54 | seasonal party, Valentine |
| 0x32, 0x33 | 0x57, 0x56 | girl birthday, player birthday |
| 0x40, 0x43, 0x4A | 0x35, 0x36, 0x37 | ETC |
| 0x41, 0x42 | 0x51, 0x5F | school trip, club camp (HYPOTHESIS name) |
| 0x44 | 0x59 | encounter; skipped when the player is sick, right after a Christmas-preparation or sick-visit record, or when the girl is not met |
| 0x45 | 0x5D | special event; scene globals `dialogue_work` word 0 = scene, word 1 = girl |
| 0x46 | 0x5A | invitation; skipped when an appointment is active |
| 0x47 | 0x61 | sick visit |
| 0x48 | 0x5B | meeting (HYPOTHESIS name; mode 0x5B is a generic scene host in SHOUGATU); sets the girl's affection to 0x32 and syoushin to 0 first |
| 0x49 | 0x58 | amusement park |
| 0xFF, unknown | 0x34 | end of the week's queue: command screen |
| 0xFC, 0xFD, 0xFE | 0x32, 0x39, 0x38 | week end, notice, month start |

`day_event_next` advances the index and returns to mode 0x31. Encounter placeholders are resolved when the week is built by `schedule_girls_gakko`: for each 0x44, if the next record is a birthday or sick visit, or the player is ill (health 3), it becomes a skip (0xFA); otherwise a special event (0x45) is chosen by `special_event_select`, else an invitation (0x46, at most one per week) by `date_sasoi_judge`, else the placeholder is kept for `geko_judge` to fill the girl, else skipped.

### 3.6 Appointments

`appointment_slots` are eight words (date promises). Bits 0-4 place + 1, 12-15 month, 16-20 day, 24 a flag, 25-28 girl, 29 active. `appointment_find_today(year, month, day)` returns the 1-based slot whose month and day match (it returns -1 for the last day of a school year and the days it excludes). `appointment_launch` pushes the place into `appt_place_*`, stores a `DAY_EVENT_APPOINTMENT` record (girl, place), copies the girl to `current_girl` and `date_girl`, the flag to `appointment_flag`, and enters `MODE_DATE`. The list is sorted by month and day with January treated as month 13 in December (`func_8006B0C8`).

### 3.7 Return context and the event log

A scene that must resume after a sub-scene saves `return_mode`, `return_stage` and `return_step` (usually the step + 1) and restores them with the three setters. `event_log_add(scene, flag)` appends an `EventLogEntry` at `event_log_count` (capped at 0xFE) with the current girl, the girl's state rank (or the override in `D_80122D48` minus one; rank 0 for girl 12) and the date; MASTER, DATE and others read the entries back (index `count - 1` is the latest).

## 4. Player state

### 4.1 Parameters

Nine records `param_*` in `PARAM_*` order: condition, liberal arts, science, art, athletics, trivia, looks, guts, stress. Each is a 16.16 fixed-point number (`frac` at +0, `value` at +2). `check_para_limit` clamps each value to 0..999 (the word is zeroed when negative). The panel (`parameter_show_init`) compares each value with the snapshot taken at the previous refresh to colour increases and decreases.

### 4.2 Health

`player_flags` bits 8-11 are the health state: 0 healthy, 1 injured, 2 neurotic, 3 sick (`HEALTH_*`). The weekly check (mode 0x32, stage 1: step 0 decides, steps 1-3 start an injury, neurosis or illness with its message, steps 5-7 recover from them) sets the state, and recovery clears bits 8-11 and sets the 3-bit `recovery_days` counter to 3 (grace). The screen frame animation, the parameter deltas (section 3.4), sick visits and several events read the state. The OPTION debug page lets a tester edit the state and the club.

### 4.3 Other player fields

`player_family_name` (6 bytes used), `player_given_name`, `player_nickname`, `player_full_name` (family + given), `player_birthday` (month, day, blood type), `player_flags` bits 12-15 club, 16-20 flags, `player_flags_hi` byte 1 career bits (written by ETC, read by ENDING). `get_p_name(buf, form)` builds how the current girl addresses the player: forms switch on the girl and on bit 7 of her state (given name, family name + a suffix, nickname, ...).

### 4.4 Settings kept in GameState

`voice_mode`, `icon_mode`, `icon_count`, `menu_mode`, `cursor_speed`, `click_mode`, `double_click_interval`, `confirm_button`, `sd_anim_fast`, plus the free icon layout in `scratch_words`. They are saved with the game and mirrored by NAME_ENT/OPTION; `options_dirty` marks which groups changed.

## 5. Girls

### 5.1 GirlRecord

`girls[16]`, index = `GIRL_*` (0 Shiori, 1 Mio, 2 Yuina, 3 Ayako, 4 Saki, 5 Yukari, 6 Nozomi, 7 Mira, 8 Yuko, 9 Megumi, 10 Yumi, 11 Miharu, 12 Rei, 13 Yoshio, 14 Sotoi). The 11 heroines 0-10 are walked by most loops; scoring uses `girl % 13`. Fields: `tokimeki`, `affection`, `syoushin` (16.16 words, value at +2/+6/+0xA), `flags` (`GirlFlags`), `birthday` (month, day, blood type), `event_counters[0x24]`.

`flags` bits (named): `met` bit 1 (set at start for girls 0 and 12), `club` bits 20-23 (club id, 11 = none; new-game setup in NAME_ENT: girl 0 gets `(day * 3 + month) % 11` from her birthday; girls 1-4 each have two possible clubs (literature or drama, science or computer, art or brass band, baseball or soccer) and take the one that differs from girl 0's club when girl 0 is in one of the two, else a random one; girl 5 tennis, girl 6 swimming, girl 10 basketball, girls 7-9 explicitly none; girls 11-14 keep the none value 11 that `func_80041C2C` gives all 16 records, because the club setup only writes girls 0-10), `story_stage` bits 9-11 (HYPOTHESIS). Bit 0 is cleared daily for girls 0-10 (`girl_met_today_clear`). Other bits (2, 3, 6, 12-14, 16-19) are tested by events; the bit 2 pairs with `met` in club-related checks (HYPOTHESIS: introduced).

### 5.2 Scores and state ranks

- `get_h_yuukou(g)` = clamp(`affection[g % 13] * D_800B41A0[g % 13] / 100`, 0, 100).
- `get_h_tokimeki(g)` = clamp(`tokimeki * D_800B4100[g] / 100` + the preference bonus, 0, 100). The preference bonus: let `mask = D_800B4110[g]`, `kind = D_800B4120[g]` (1..6; 0 for girls 11 and 12), thresholds `lv[0..6] = {128, 100, 90, 70, 50, 30, 10}` (`D_800B4198`). For each parameter index i in 1..6 with `mask` bit i set, find the first k in 0..6 such that `param_i.value > lv[k] + offset(kind, k)` and add `D_800B4130[g][k]` (signed bytes); if none matches add entry 7. Offsets per kind (k = 0..6): kind 1 `50,35,20,20,5,5,0`; kind 2 all 0; kind 3 `50,50,50,20,20,20,20`; kind 4 `10,10,10,10,0,0,0`; kind 5 `20,20,20,20,0,-5,0`; kind 6 `0,0,0,0,-5,-5,-5`. (Girls 11 and 12 use zero weights and masks.)
- `get_g_zyotai_h(g)` (rank 0 best, 4 worst): yuukou < 10 -> 4; < 50 -> 3; tokimeki < 50 -> 2; < 80 -> 1; else 0.
- `get_g_zyotai_s(g)`: yuukou < 10 -> 4; < 30 -> 3; < 50 -> (tokimeki < 50 ? 3 : 1); < 80 -> (tokimeki < 50 ? 2 : 1); else (tokimeki < 50 ? 0x82 : 0x80). Bit 7 means "affection at least 80". `girl_state_rank(g)` = this value & 0x7F.

### 5.3 Daily changes and caps

`check_para_limit` caps girls 0-10: tokimeki and affection at 0..0x80 (the word is zeroed when negative), syoushin at 0..0xFF; the queue reader's `girl_counters_clamp` (`func_80071038`) then clamps all three to 0..0x80, so 0x80 is the working cap of syoushin. `yuukou_down`: for every met girl, affection -1 (floor 0) when `day / 7 == 0` (days 1-6), and for girl 0 also when `day / 7 == 2` (days 14-20). `syoushin_up`: for every met girl with weighted affection < 10, syoushin +5. Both run once per queue build (weekly, mode 0x30 step 3) and only while `vacation` is 0. Effects of syoushin: >= 0x3C blocks encounters and phone calls; >= 0x4C allows a forced encounter (flag bit 16 is set when it fires and cleared again once syoushin is below 0x32); >= 0x3D at a day end queues a rumour report (flag bit 17, same re-arm). HYPOTHESIS: resentment counter; the name is kept from the developer's function.

### 5.4 Per-girl arrays

`girl_last_date_spot[16]`, `girl_last_date_spot_timer[16]` (decremented once per weekly queue build), `girl_event_bits[16]`, `girl_cleared[16]` (HYPOTHESIS), `date_place_history[4]`, `event_flags[0xFC]` (one-shot event ids), `clubs[12]` (exp, level, flags per club).

**Last date spot.** After a date, DATE stores the spot id `D_80122D08` in `girl_last_date_spot[girl]` and sets `girl_last_date_spot_timer[girl]` to 12. It does so only when the looks parameter is at least 35 (at least 30 for every girl except 7), and spots 8 and 6, the special variants of two places, are stored as 7 and 5. `last_date_spot_timer_dec` (called by `schedule_init` step 0) lowers every girl's timer by 1 once per queue build, that is once a week. When the player picks the same spot with a girl whose timer is nonzero (date mode, not an appointment date, place index neither 0 nor 0x12), the date shows the girl's repeat reaction and, at its first substep, her tokimeki drops by 1, her affection by 2 and her syoushin rises by 5. Spot ids 0x23 and 0x24 are forgotten by `func_8006D038`, which `calendar_advance_day` runs every day in months 3, 6, 9 and 12 (it also clears `girl_event_bits`). Value 0 means no spot.

## 6. Input, menus and text

Input is polled in `frame_render`: `input_device` is 0 for a mouse, 1 for a pad, 0xFF when nothing is attached. The pad word layout is the PsyQ one (`PadBits`); `pad_pressed = ~pad_prev & pad_held` and `pad_released = pad_prev & ~pad_held`; pad 2 is the upper 16 bits. The low byte of a pad word is L2, R2, L1, R1, triangle, circle, cross, square; the second byte is SELECT, L3, R3, START, up, right, down, left (the game never uses L3 and R3). START on either pad toggles `paused` while `pause_allowed` is 1, and START on pad 2 also toggles `sd_anim_fast`. Holding SELECT, START, L1 and R1 together on pad 1 for 0x81 frames runs a reset sequence back to mode 0 (on pad 2, to mode 0x11), except in modes 0x20, 0x90 and 0xD0. In mouse mode `mouse_x/mouse_y` and `mouse_buttons` are filled and the pointer is actor 0. Holding the four face buttons of pad 2 together enables a turbo that injects a confirm press every other frame (any other button cancels it). The settings screens (all in main `8005A0B0.c`) change `menu_mode`, `icon_mode`, `voice_mode`, `cursor_speed`, `click_mode`, `double_click_interval` and `confirm_button` as listed in section 4.4.

Menus: two menus (0 and 1) of up to 16 items. `menu_set(menu, count, x[], y[], w[], h[])` stores the rectangles in `menu_items` (an item with zero area is disabled). `menu_check(menu, x, y)` hit-tests the pointer (or follows the pad cursor, depending on `menu_mode` bits 0-1; SELECT adds 1 to the mode) and fills `menu_hit[menu]` and `menu_items[i][menu].hover`. `menu_cursor_update(menu)` moves the pad cursor (up/left previous, down/right next, wrapping, skipping disabled items). A screen reads `menu_cursor_1` or `menu_hit[1]` after a confirm press.

Text: `set_kanji_string(x, y, attr, string, extra)` appends a `TextEntry` to the 52-entry `text_list`; `k_*` functions run the typewriter (one glyph every `speed + 1` frames, `k_disp_inc`), `k_reset/k_sub_reset` cut the list, `check_end_k` reports the end. Strings are Shift-JIS pairs; code `0x20` ends a line, control letters expand name codes (section 7). Dialogue: `dialogue_run` reads the script tables through `dialogue_tables` (group, line, text, command and voice tables) and the 20 speak codes.

## 7. Dialogue names

`dialogue_names_set(girl)` fills the buffers for the dialogue codes: family name (`name_buf_family`), given name (`name_buf_given`), the form in which the girl addresses the player (`name_buf_player_form`), her short name (`name_buf_girl_short`) and her full name (`girl_name_buf`). `girl_full_name_table`, `girl_short_name_table` (16 entries) and the family-name list hold the names in `GIRL_*` order.

## 8. Persistent state (what the save keeps)

`GameSaveImage` (0xF00 bytes per bank) is filled by `func_8005557C` from: `vacation`, the four calendar bytes, `day_count`, `month_calendar[2]`, the 0x7C-byte profile block (names, flags, birthday, parameters and the first 0x20 bytes of `progress_state`), `girls[16]`, `girl_cleared`, `appointments[8]`, the 12 club words, `event_flags`, `date_place_history`, `girl_last_event`, `girl_cooldown`, `girl_event_bits`, `event_log[256]`, and the settings bytes (`icon_mode`, `icon_count`, `menu_mode`, `sd_anim_fast`, `voice_mode`, `last_command`, `cursor_speed`, `click_mode`, `double_click_interval`, `confirm_button`, `quarter_flags`). A checksum (byte sum of the data) heads each bank. Not saved: `day_events` and its index (rebuilt by `schedule_init`), the return triple, `current_girl`, the appointment place list, everything in the graphics and sound layers. `SystemData` (album slots, per-girl persistent flags, TACO settings) goes to the separate system file.

## 9. Hardware layer: modules and entry points

| layer | names (entry points) | state | what a port provides instead |
|---|---|---|---|
| frame and GPU | `main_loop`, `gpu_init`, `frame_render`, `draw_buffer`, `screen_center_x/y`, `prim_arena`, `prim_gradient_rect/line`, `back_clear`, `clear_color_*`, `screen_brightness` | arena per buffer, ordering tables | engine renderer; keep the 320x240 logical screen with origin at the centre |
| actors and sprites | `actor_pool`, `actors_draw`, `actor_clear*`, `sprite_quads`, `bg_layers`, `tile_layer_enabled` (`srn_*`, `mod_*`), `screen_frame_draw`, `light_effect_draw` | 160 actors of 0x44 bytes, frame and sequence tables (`sprite_frame_table`) | sprite nodes with frame tables; actor 0 is the cursor |
| VRAM management | `tpage_buf`, `tpage_req`, `palette_queue`, `csr_queue`, `load_palette`, `load_tpage_buf_lock`, `LoadSquare` | 64 texture-page slots, deferred uploads | texture atlas loading |
| backgrounds | `dec_bg_*`, `dec_bg_cd_read`, `dec_bg_decode_to_vram`, `dec_bg_draw`, MDEC helpers | two slots with an LBA cache | load images directly |
| text | `text_list`, `set_kanji_string`, `k_*`, `text_system_reset`, font tables | glyph cache in VRAM | engine text with the same typewriter timing |
| menus, cursor | `menu_*`, `cursor_*`, `menu_items` | | engine UI |
| sound | `snd_command`, `snd_command_pump`, `snd_bank_load_*`, `voice_play`, `audio_state` | command queue; sound banks loaded from disc | engine audio; command ids are the contract |
| CD and loading | `cd_cmd_*`, `cd_read_sectors`, `cd_audio_command`, overlay loader (`ovl_load_*`) | 48-entry ring | file access; overlays become code modules |
| memory card | `card_start/stop`, card step machine, `card_block` | busy flag, step, retry, result | save files |
| movie | `strInit`, `strNext`, ... (PsyQ movie sample) | `StrCtx` | video playback |
| input | pad/mouse polling, `pad_*` words | | engine input mapped to the PsyQ bit layout |

## 10. Shared records and types

All shared types and their proposed names (the 88 types of `include/game.h` and `include/main_api.h`; the rest keep their names). A size suffix is dropped once a role name exists.


| old | new | role |
|---|---|---|
| `WordBytes4` | `WordBytes` | word/bytes union view of D_801255D0 (CD state); size suffix dropped |
| `DialogueVoice8` | `DialogueVoiceRef` | leaf of the dialogue voice table (two words per line) |
| `DialogueBytes16` | `DialogueSelectors` | 16 byte selectors copied to the dialogue handler's stack |
| `Rec34Flags` | `LegacyGirlFlags` | flag word of the 0x34-byte girl record that the dead EVENT overlay was linked against |
| `Rec34` | `LegacyGirlRecord` | EVENT overlay's older GirlRecord layout (D_800B0A04 in its linked main layout; same s16 fields at +0..+0xA); EVENT is never loaded |
| `GsDateBits` | `BirthdayBits` | month bits 0-3, day bits 4-8, blood type bits 9-10 (player word and GirlRecord.birthday) |
| `GsWord` | `WordView` | word/halves/bytes/date union used for pad words, flag words and packed fields |
| `GsEntry54C` | `AppointmentEntry` | bit view of one appointment word (place, month, day, flag, girl, active) |
| `GsSlot54C` | `AppointmentSlots` | 8 appointment words plus the first-entry bit view; macros unk_54C/unk_54C_0 read through it |
| `GsHalf` | `HalfView` | halfword/bytes union (player_flags halves) |
| `NameMetric4` | `AlbumParam` | one saved parameter value of an album slot (metrics[9][16] of SystemData): s16 value at +2 |
| `WorkSetBits` | `TacoSettingsBits` | HYPOTHESIS: settings bits at SystemData +0x40 (saved flag, mode, selectors); read by TACO |
| `Work800E7D10` | `SystemData` | 0xEE0-byte area after GameState: random word, album slot table, per-girl persistent flags, TACO settings, names and parameters of 16 album slots,... |
| `CharFlags` | `GirlFlagBits` | bit-field view of GirlRecord.flags (met, club, story stage, event bits) |
| `FlagsF64` | `QuarterFlagBits` | bit-field view of GameState.quarter_flags |
| `Rec38Flags` | `GirlFlags` | union of GirlRecord.flags views (word, halves, bytes, GirlFlagBits) |
| `Rec38` | `GirlRecord` | 0x38-byte per-girl record in GameState.girls[16]: tokimeki, affection, syoushin counters, flags, birthday, event counters |
| `Rec24` | `SpriteQuad` | 0x24-byte textured quad drawn by sprite_set_gpu_poly_ft4: position, size, uv, tint, flags (bit 31 hidden, bit 30 semi-transparent); table D_801217D0 |
| `Work80125D10` | `AudioState` | 0x50-byte work area used by the sound driver (80079B10) and the CD audio code; unk_44/unk_48 are CD command buffers |
| `GsRec01C` | `PrimArena` | per draw buffer: size 0x10C00, used offset, base address (SetWorkBase/GetWorkBase) |
| `GsRec044` | `MonthCalendar` | day_kind[0x20] ([0] = days in month) and marker[0x20] ([0] = weekday of the 1st) of one month |
| `GsRec0FC` | `ParamValue` | one player parameter: 16.16 fixed point, frac at +0, value at +2 |
| `GsRec66C` | `ClubRecord` | per club: u16 exp at +0, level byte at +2, flag byte at +3 (ClubFlagBits view of the word) |
| `GsRec69C` | `DayEvent` | queue record: type, girl, scene, flags |
| `GsRec75E` | `EventLogEntry` | girl, flag, scene, girl rank, year, month, day, weekday |
| `GsRecF90` | `MenuItem` | menu item rectangle x, y, w, h, hover flag, disabled flag |
| `GsRec1328` | `PaletteLoad` | queued palette upload: source pointer and 4 parameter bytes |
| `GsRec142C` | `CsrLoad` | queued image upload: source pointer, tpage slot, rect (x, y, w, h) |
| `KWork` | `TextState` | text list state: entry count, total glyphs, mark, speed, shown glyph position |
| `Flags7D4F` | `ExtraModeFlags` | HYPOTHESIS: byte D_800E7D4F (TT unlock bit, festival return mode bits) in SystemData |
| `SpriteCell8` | `BgCell` | PsyQ GsCELL layout |
| `SpriteMap10` | `BgMap` | PsyQ GsMAP layout |
| `SpriteIndex80` | `BgMapIndex` | cell index array of one BG slot |
| `Ring18` | `CdCommand` | entry of the 48-slot CD command ring: command, argument, 16 data bytes |
| `StrDecEnv30` | `StrDecEnv` | PsyQ movie sample DECENV; size suffix dropped |
| `SaveSettings20` | `SaveConfigBlock` | HYPOTHESIS: 0x14-byte settings record copied by the save routines; protection flag in the last byte |
| `SaveSettings24` | `SaveTacoSettings` | HYPOTHESIS: 0x18-byte copy of SystemData +0x40.. (settings bits, three names, three words) |
| `SaveMetricWord` | `SaveAlbumParam` | word/AlbumParam union of the system-file image |
| `SaveImageView` | `SystemSaveImage` | 0xF00 byte image of SystemData as written to the memory card |
| `SaveWorkView` | `SystemDataSaveView` | 0x58-byte view of the SystemData prefix with the two settings records |
| `GsBlockC4` | `ProfileBlock` | GameState +0xC4..+0x140: names, flags, birthday and the nine parameters as saved (0x7C bytes) |
| `GsBlock56C` | `EventFlagBlock` | GameState +0x56C..+0x66C as saved (event_flags, 0x100 bytes) |
| `SaveGameView` | `GameSaveImage` | 0xF00 byte memory-card image of one game save; comments name the GameState source of each field |
| `Entry8` | `TextEntry` | 8-byte entry of text_list: x, y, attribute, glyph count, extra, visible |
| `FileReq` | `CardFileRequest` | stack request of the memory-card file functions: 32-byte name and retry counter |
| `MenuIconBits` | `LastCommandBits` | weekday command (bits 0-3) and holiday command (bits 4-7) nibbles of GameState.last_command |
| `DisplayActor44` | `Actor` | 0x44-byte display actor in actor_pool[160]: sprite frame tables, 16.16 position, scale, rotation, brightness; size suffix dropped |
| `DisplayFixActor44` | `ActorFixed` | Actor with +0x24/+0x28 as 16.16 words (units that move actors in fixed point) |
| `DisplayImagePart0E` | `SpritePart` | one part of a sprite frame: uv, size, offset, flags |
| `DisplayImage78` | `SpriteFrame` | sprite frame: part count, origin and up to 8 SpriteParts |
| `DisplayColorPair4` | `ColorPair555` | two packed 5:5:5 colours interpolated by screen_frame_draw |
| `BunkaDispatch8` | `CultureFestivalDispatch` | HYPOTHESIS: the word pair D_80122EB8 shared by BUNKASAI and BUNKAKEN |
| `SpriteSlot24` | `BgLayer` | 0x24-byte PsyQ GsBG-style layer at D_801220D0 (2 slots) |
| `GsByteNibbles` | `NibbleView` | low/high nibble byte view |
| `ScheduleFlags` | `PlayerFlagBits` | bit view of GameState.player_flags (health, club, flags, recovery counter) |
| `ScheduleClubFlags` | `ClubFlagBits` | bit view of a ClubRecord word |
| `ScheduleEntryDate` | `AppointmentDate` | month and day halves of an appointment word |
| `ScheduleEntryFlags` | `AppointmentBits` | bit view of an appointment word (same layout as AppointmentEntry) |
| `NameRecordF00` | `AlbumSlotRecord` | 0xF00-byte album slot record read by NAME_ENT and main |
| `ScheduleGirlFlags` | `GirlFlagBitsSched` | extra bit view of GirlRecord.flags used by the schedule builder |
| `CallPair8` | `AudioRequestPair` | two words passed to cd_audio_request_set (NAME_ENT table rows) |
| `SrnRec28` | `SrnLayer` | 0x28-byte record of the srn_* tile-layer routines (D_80121750) |
| `ModRec14` | `ModTileSet` | 0x14-byte tile-set record of the mod_* routines (D_801217A0) |
| `StateClearBytes100` | `GameStateClearBlock` | byte view used to clear GameState +0x56C..+0x66C (event_flags and clubs) in one block |
| `DrawEnv60` | `DrawEnvCopy` | stack copy of the PsyQ DRAWENV |
| `DispEnv14` | `DispEnvCopy` | stack copy of the PsyQ DISPENV |
| `SprEnt` | `SpriteQuadView` | second view of SpriteQuad (+0x0A is s16 here, u16 in SpriteQuad); merge candidate once the match allows |
| `GsF6Bits` | `PlayerFlagsHiBits` | bit view of the byte at GameState +0xF6 (player_flags_hi byte 0) |
| `F6Bits` | `RecoveryCounterBits` | bit view of the same byte with the 3-bit recovery counter in bits 5-7 |
| `CdRingView` | `CdRingState` | head, tail, status and phase bytes D_80125128..D_8012512B of the CD command ring |
| `Pix2In` | `PixelPairIn` | two 5:5:5 pixels read by the colour-bar conversion func_8004B590 |
| `Pix2Out` | `PixelPairOut` | two 5:5:5 pixels written by func_8004B590 |
| `ModelRec88` | `ModelRecord` | 0x88-byte model record of the 3D init code; size suffix dropped |
| `MouseOut` | `MouseSample` | three-word mouse state returned by func_80059860 |
| `MenuSelection5` | `MenuSelection` | count[2], mode and cursor[2] bytes at GameState +0x1090; the view the menu code casts to |


Kept as they are: `DialogueWork`, `DialogueTables`, `GameState`, `FontRange`, `SaveBank`, `StHEADER`, `StrCtx`, `CdLevelWork`, `Fix32` (descriptive or PsyQ-style names) and `TaiikuBig` (owned by the TAIIKU analyst). The two bit-field views of the byte at GameState +0xF6 (`GsF6Bits`, `F6Bits`), the two `SpriteQuad` views (`Rec24`, `SprEnt`) and `AppointmentEntry`/`AppointmentBits` are merge candidates for a later pass (the matched code needs the separate views today).

Field renames of the shared records (234 `field` rows, 133 in `GameState` including the two macro aliases) use the pre-rename type name as scope. Fields with no proven role keep `unk_XX` (30 of the 161 top-level `GameState` members: unused bytes, the unknown 0x200-byte area at +0x183C and the pointer words at +0x1A3C..+0x1A84).

## 11. Enums and masks (const rows)

Proposed for `include/game_enums.h` (238 rows): `GirlId` (16, `GIRL_*`), `Parameter` (9), `Month` (12), `Weekday` (7), `Club` (12 including `CLUB_NONE`), `Place` (21), `HealthState` (4), `Vacation` (4), `DayKind` (4), `CalendarMarker` (18), `DayEventType` (31), `MainMode` (68), `InputDevice`, `VoiceMode`, `MenuMode`, `IconMode`, `PadBits` (14, PsyQ layout, `PAD_` prefix), `MouseButtons`, `PlayerFlag`. Rules for applying them:

- Fields and variables keep their fixed-width types (an `enum` variable is 4 bytes and signed and would change code); the field gets a `/* GirlId */` comment.
- A literal is replaced only where the other operand is a named site: comparisons and assignments of the field, `case` labels of a `switch` on it, and arguments of the named functions (`main_state_set`, `main_stage_set`, `main_step_set`, `icon_can_use_set`, `get_g_zyotai_*`, `birth_day_check_days` for `GirlId`).
- `Place` values stored in appointment words are place + 1; do not replace the stored value.
- Placeholders until the overlay analysts settle them: `MODE_NAME_ENT_3/4`, `MODE_TELEPHONE_PROFILE`, `MODE_CLUB_MASTER_*`, `MODE_ETC_C2..C4`, `MARKER_NOTICE_*`, `DAY_EVENT_CLUB_MASTER_*`.

## 12. The 100 most used main functions

Counts are call sites plus declarations in `src/` and `include/`, without EVENT and GYOZI (31,188 in all; the top 100 hold 86%). Functions that already have a developer or patch name keep it; their English comment is a `comment` row.


| # | address | uses | name | role |
|---|---|---|---|---|
| 1 | 8004284C | 4121 | `main_step_next` (was `func_8004284C`) | step += 1, step_timer and substep cleared (4,121 uses, the top function); FAKE z-store idiom keeps the match |
| 2 | 80085A60 | 1436 | `normal_date_speak` (kept) | Wire the dialogue table pointers and run the talk runner for the current scene girl |
| 3 | 80044750 | 1180 | `snd_command` (was `func_80044750`) | 1,180 uses: queue one sound-driver command (func_80079B10 -> func_8007B5EC); id low byte = effect or sequence, bits 8-11 = class dispatched by... |
| 4 | 8004E788 | 1167 | `set_kanji_string` (kept) | Queue one string (x, y, attribute, text, extra) in the 52-entry text list for drawing |
| 5 | 8008667C | 1106 | `girl_state_rank` (was `func_8008667C`) | 1,106 uses: get_g_zyotai_s(girl) & 0x7F, the 0-4 mood rank scenes switch on (0 best) |
| 6 | 800460CC | 1086 | `cd_read_sync_status` (kept) | Return the CD read status byte (bit 0 = read finished) |
| 7 | 80046318 | 844 | `cd_read_sectors` (kept) | Read n sectors to a buffer from an LBA through the CD command ring and wait |
| 8 | 80042940 | 773 | `main_step_set` (was `func_80042940`) | step = arg; step_timer and substep cleared |
| 9 | 80042908 | 721 | `main_stage_set` (was `func_80042908`) | stage = arg; step, stage_timer, step_timer, stage_scratch, substep cleared |
| 10 | 80046500 | 514 | `game_reset_to_title` (was `func_80046500`) | default branch of mode switches (514 uses): resets loader, game state, sprites, sound, dec_bg and tpage tables, then main_state_set(0x12) (title) |
| 11 | 8005742C | 491 | `dec_bg_cd_read` (kept) | Start reading a compressed background file unless one of the two cached LBAs of the slot already holds it; returns 0/1 for a cache hit slot, -1... |
| 12 | 8007E934 | 435 | `normal_date_bg_fadeout` (kept) | Fade the scene background out |
| 13 | 8004EAAC | 414 | `k_sub_reset` (kept) | Cut the text list back to the saved mark |
| 14 | 80057418 | 387 | `dec_bg_show_set` (kept) | Show or hide background slot n |
| 15 | 8007ED84 | 387 | `bg_read_sub2` (kept) | Background read helper: starts the read of the scene background |
| 16 | 8005751C | 386 | `dec_bg_decode_to_vram` (was `func_8005751C`) | 386 uses: reinitialises the MDEC buffers and decodes the loaded background into the VRAM band of slot arg0 |
| 17 | 80062CD0 | 384 | `bustup_read_start` (was `func_80062CD0`) | 384 uses: starts the CD read of a girl portrait file (0x2D sectors to 0x80180000) and clears the portrait state |
| 18 | 80043914 | 373 | `load_palette` (kept) | Queue a palette (CLUT) upload for the next VRAM flush |
| 19 | 8007C740 | 351 | `addr_init_bustup` (kept) | Set the portrait sprite addresses for the current girl |
| 20 | 8004EAD4 | 324 | `k_sub_disp_start` (kept) | Start the typewriter at mark + n |
| 21 | 8007E5A0 | 324 | `normal_date_girl_out` (kept) | Slide the girl's portrait out |
| 22 | 80042878 | 308 | `main_state_set` (kept) | Switch to a mode and reset stage, step, timers, substep |
| 23 | 80083474 | 299 | `bustup_show_checked` (was `func_80083474`) | 299 uses: re-verifies the loaded portrait (pointer words and checksum, retries the step 3 back on mismatch) then builds it with the girl's mood... |
| 24 | 8007E99C | 293 | `normal_date_bggirl_fadeout` (kept) | Fade background and portrait out together |
| 25 | 8007E884 | 292 | `normal_date_bggirl_fadein` (kept) | Fade background and portrait in together |
| 26 | 80050DFC | 288 | `gnsx` (kept) | Draw a name string centred by its length (2, 4 or 6 bytes) |
| 27 | 8004E58C | 257 | `text_system_reset` (was `func_8004E58C`) | 257 uses: reloads six small text/cursor blocks to VRAM, calls func_8004E500(2) and clears the 52 text list entries |
| 28 | 80050E8C | 246 | `sndisp` (kept) | Wrap and queue a narration string at row y with colour col |
| 29 | 800847B8 | 246 | `dialogue_names_set` (was `func_800847B8`) | 246 uses: fills the name buffers for dialogue codes: family and given name from GameState, the player-addressing form (get_p_name), the girl's... |
| 30 | 80042808 | 237 | `main_stage_next` (was `func_80042808`) | stage += 1; step, stage_timer, step_timer, stage_scratch, substep cleared |
| 31 | 8007E81C | 231 | `normal_date_bg_fadein` (kept) | Fade the scene background in |
| 32 | 80048514 | 205 | `load_tpage_buf_lock` (kept) | Store a resource id in a tpage slot and queue its image upload |
| 33 | 8004E884 | 180 | `k_disp_start` (kept) | Start the typewriter at entry n (skips the glyphs of earlier entries) |
| 34 | 8004E9F4 | 179 | `k_reset` (kept) | Cut the text list to n entries and recompute the glyph totals |
| 35 | 80083440 | 178 | `bustup_layout_set` (was `func_80083440`) | 178 uses: bustup_layout_set(girl) = func_80085CD4 (per-girl quad offsets) + func_800634FC |
| 36 | 80044E8C | 176 | `snd_bank_load_poll` (was `func_80044E8C`) | 176 uses: waits for the queued sound-bank reads, then issues the sound commands 0x21/0x22/0x2F (bank 0) or 0x23/0x24/0x2E (bank 1) and returns 1 |
| 37 | 8007E390 | 175 | `normal_date_girl_in` (kept) | Slide the girl's portrait in |
| 38 | 800460EC | 174 | `cd_audio_state` (was `func_800460EC`) | 174 uses: D_801255B4, the CD audio state byte (bit 2 / value 4 = playing) tested before waiting for voice |
| 39 | 800866A0 | 174 | `bustup_read_step` (was `func_800866A0`) | step function: substep 0 starts bustup_read_start; later waits for the CD, checks the five image pointers and the file checksum, then main_step_next |
| 40 | 80051DBC | 165 | `xa_wait` (kept) | Typewriter plus voice wait step used by dialogue lines |
| 41 | 80048F64 | 164 | `actor_clear` (was `func_80048F64`) | 164 uses: resets display actor i (0-159) to defaults: hidden, frame table = sprite_frame_table, scale 0x1000, brightness 0x80 |
| 42 | 80051010 | 156 | `sndi` (kept) | Queue a narration string with the speaker name codes expanded |
| 43 | 800850D4 | 152 | `event_anim_show` (was `func_800850D4`) | fills actors 96/97 as a full-screen animated picture from frame/sequence/tpage tables and a mode; arg0 == 0 clears them (DATE event scenes) |
| 44 | 80051A68 | 150 | `get_g_zyotai_s` (kept) | Girl state rank 0-4 for scenes (4 worst); bit 7 set when affection is high |
| 45 | 80085B3C | 143 | `event_log_add` (was `func_80085B3C`) | appends (girl, flag, scene, girl rank, year, month, day, weekday) to event_log; count capped at 0xFE |
| 46 | 80044890 | 138 | `snd_bank_load_start` (was `func_80044890`) | 138 uses: bank 0/1 sound data load: skips files already loaded (cache of LBAs), otherwise queues up to four CD reads (header, body, sequence) to... |
| 47 | 80057390 | 137 | `set_dec_bri` (kept) | Set the brightness tint of the decoded backgrounds |
| 48 | 8006612C | 131 | `message_window_set_title` (was `func_8006612C`) | 131 uses: queues the title string at the window's top-left and builds the title decoration actors 3.. (one per two bytes + 2); the slide-in... |
| 49 | 800649D4 | 130 | `hizuke_init` (kept) | Create the calendar date actors (month, day, weekday) for the HUD |
| 50 | 80064E84 | 125 | `message_window_init` (kept) | Create the message window actor |
| 51 | 80053D10 | 124 | `card_stop` (was `func_80053D10`) | 124 uses: card_busy = 0 and card_timer = 0 |
| 52 | 800438F0 | 123 | `back_clear_switch` (kept) | Turn the per-frame screen clear on or off |
| 53 | 80052060 | 122 | `voice_play` (was `func_80052060`) | 133 uses: if voice_mode != 0 and the mode nibble is not 15, sets the CD audio request and sends sound command 0x300 |
| 54 | 8005215C | 121 | `dialogue_text_step` (was `func_8005215C`) | 121 uses: per-frame step of dialogue text: typewriter, line scroll (text_scroll_state) and voice-aware speed by voice_mode |
| 55 | 80084D3C | 121 | `check_para_limit` (kept) | Clamp girl tokimeki/affection (0..0x80), syoushin (0..0xFF) and the nine parameters (0..999); the queue reader later clamps the girl counters to 0x80 (`girl_counters_clamp`) |
| 56 | 800573AC | 119 | `dec_bg_reset` (kept) | Clear the background cache, show flags and brightness (0x80) |
| 57 | 80041584 | 114 | `sprite_tables_clear` (was `func_80041584`) | 114 uses: clears SpriteQuads 0-63 and BgLayers 0-1 |
| 58 | 80084E4C | 113 | `text_marker_hide` (was `func_80084E4C`) | 113 uses: text_marker_set(0) |
| 59 | 80048EB8 | 108 | `actor_clear_range` (was `func_80048EB8`) | 108 uses: clears actors 0..n-1 (all 160 when n is 0 or > 160) and keeps the cursor position words |
| 60 | 80048390 | 108 | `tpage_buf_clear` (kept) | Reset the 64 tpage slots (reserved and free marks) and set the default system pages |
| 61 | 80046290 | 107 | `cd_audio_request_set` (was `func_80046290`) | 107 uses: fills the 4-word CD audio request (index, table word, address, length) from a pair table and a mode nibble |
| 62 | 800438DC | 101 | `draw2d3d` (kept) | Switch the 2D sprite pass (arg 1) and the 3D pass (arg 2) on or off |
| 63 | 80049A40 | 101 | `prim_gradient_rect` (was `func_80049A40`) | 101 uses: queues a four-colour gradient rectangle (POLY_G4) from the primitive arena; flag bit 7 = semi-transparent |
| 64 | 8004E93C | 90 | `k_disp_switch` (kept) | Show or hide a text list entry |
| 65 | 8004500C | 89 | `snd_bank_start` (was `func_8004500C`) | 89 uses: checks the bank header (0x40) then queues commands 0x21/0x22/0x2F and starts sequence arg1 (0x200 / id) |
| 66 | 80048DAC | 89 | `actors_draw_enable` (was `func_80048DAC`) | 89 uses: actors_draw_enabled = (arg != 0) |
| 67 | 8008585C | 89 | `dialogue_clear` (was `func_8008585C`) | 89 uses: clears dialogue_work words 5..30, re-initialises and hides the cursor actor, empties the text list and blanks the window title |
| 68 | 8004F984 | 81 | `menu_check` (kept) | Hit-test the pointer or move the cursor of a menu by menu_mode; returns the hit index |
| 69 | 80084E90 | 81 | `event_still_show` (was `func_80084E90`) | uploads six 0x40 x 0x80 tiles to VRAM and sets SpriteQuads 58-63 as a 3 x 2 grid (128 px cells) = one full-screen event still; callers KANGEI,... |
| 70 | 80048E78 | 80 | `actor_clear_upper` (was `func_80048E78`) | 80 uses: clears actors 96..159 |
| 71 | 8004FC10 | 80 | `menu_cursor_update` (was `func_8004FC10`) | 80 uses: moves the menu cursor by pad (up/left previous, down/right next, skips disabled items, wraps), or follows the mouse in mode 0 |
| 72 | 80050B54 | 79 | `menu_bar_show` (kept) | Draw the highlight bar of the hovered menu item |
| 73 | 800578F4 | 77 | `dec_bg_draw` (was `func_800578F4`) | 77 uses: draws the decoded background(s) as sprites with dec_bg brightness; arg = the last screen mode |
| 74 | 800462C8 | 76 | `cd_read_sectors_nowait` (was `func_800462C8`) | 76 uses: same as cd_read_sectors but D_800B3D60 = 0 (caller polls cd_read_sync_status); queues command 6 |
| 75 | 80066C08 | 74 | `screen_frame_draw` (was `func_80066C08`) | HYPOTHESIS: 74 uses; draws a 10 x 8 grid of 32 px sprites with interpolated colour pairs (the screen frame); the scroll pattern follows the health... |
| 76 | 8004F870 | 73 | `menu_set` (kept) | Register a menu: item count and rectangles for menu 0 or 1 |
| 77 | 8004EA98 | 73 | `k_sub_reset_point_set` (kept) | Remember the current text list length as the mark |
| 78 | 80049FF0 | 73 | `prim_gradient_line` (was `func_80049FF0`) | 73 uses: queues a two-colour line (LINE_G2) from the primitive arena |
| 79 | 800494BC | 68 | `dtd_on_tpage` (kept) | Draw a sprite from a texture page slot |
| 80 | 80064F48 | 66 | `message_window_show` (kept) | Per-frame update of the message window |
| 81 | 8004482C | 66 | `snd_bank_state_reset` (was `func_8004482C`) | 66 uses: clears the bank load state variables |
| 82 | 80066334 | 64 | `message_window_title_update` (was `func_80066334`) | 64 uses: per-frame slide-in of the title actors |
| 83 | 800452C4 | 64 | `cd_cmd_reset` (was `func_800452C4`) | 64 uses: clears the CD command ring, status and counters |
| 84 | 80082764 | 64 | `dialogue_run` (was `func_80082764`) | 64 uses: the talk runner every script section goes through (reads group/line/command tables, waits for text end and voice, handles 20 speak codes) |
| 85 | 80053CE0 | 64 | `card_start` (was `func_80053CE0`) | 85 uses: card_busy = 1; clears card step, result, retry count and timer |
| 86 | 8004EC24 | 63 | `k_disp_inc` (kept) | Reveal one more glyph every speed + 1 frames; returns 1 when everything is shown |
| 87 | 8004AE28 | 63 | `debug_number_show` (was `func_8004AE28`) | 63 uses: shows a number at (x, y) only while the debug flag D_800B3220 is set |
| 88 | 8004ECB4 | 62 | `check_end_k` (kept) | Return nonzero when the typewriter has shown the whole text |
| 89 | 80065F34 | 60 | `message_window_title_show` (was `func_80065F34`) | 60 uses: shows or hides the title actors and the matching text-list entry |
| 90 | 80064DEC | 58 | `hizuke_show` (kept) | Per-frame update of the calendar date HUD |
| 91 | 8006BD6C | 57 | `cursor_show` (was `func_8006BD6C`) | 57 uses: sets or clears the visible bit of actor 0 |
| 92 | 8006B900 | 56 | `light_effect_draw` (was `func_8006B900`) | HYPOTHESIS: 56 uses; draws nine layered light-effect sprites with texture-page draws; called by DATE scenes and the loader screen |
| 93 | 80081190 | 55 | `dialogue_render` (kept) | Split a script string into lines and queue them with name code expansion |
| 94 | 800639D8 | 54 | `parameter_show_init` (kept) | Create the digit actors of the parameter panel and snapshot the profile |
| 95 | 80065048 | 53 | `icon_can_use_set` (kept) | Enable or disable one of the 14 command icons |
| 96 | 80072B5C | 51 | `day_event_next` (was `func_80072B5C`) | 51 uses: next queued event: day_event_index += 1, back to mode 0x31 (or mode 0x22 when D_800B3C6C is set); arg 1 first runs func_8004500C |
| 97 | 80052000 | 51 | `text_wait_confirm` (was `func_80052000`) | 51 uses: typewriter step; on confirm/cancel jumps to the line end and, when finished, substep += 1 |
| 98 | 80085E30 | 50 | `scene_screen_reset` (was `func_80085E30`) | 50 uses: resets draw switches, clear colour, brightness, text system and tpage tables for a new scene screen |
| 99 | 80064FA4 | 48 | `icon_disp_switch` (kept) | Show or hide the command icons |
| 100 | 80051B48 | 48 | `get_g_zyotai_h` (kept) | Girl state rank 0-4 from weighted affection and tokimeki (4 if affection < 10, 3 if < 50, 2 if tokimeki < 50, 1 if < 80, else 0) |


## 13. New game setup

`game_state_init(1)` clears GameState (0x1A8C bytes) and sets: display 320x240, mode 0x10, `screen_center` 0xA0 / 0x168 / 0x78, `frame_hcount_limit` 0x106, `actor_draw_last` 0x9F, `sound_active` 1, `input_device` 1, `cursor_speed` 3, `menu_mode` 1, `overlay_entry` 0x80132000. It then runs four initialisers:

- `func_800418B0`: `vacation` 0, calendar 1995-04-04 (year 0x5F, month 4, day 4, weekday 1), `day_count` 0, the April and May 1995 `month_calendar` records (30 and 31 days, first weekdays 5 and 0, Sundays and the holidays of April 29 and May 3-5 marked, one marker in April on day 4).
- `func_800419FC`: placeholder names, birthday 1/1 with blood type 1, health 0, club 11 (no club), the three flag bits of `player_flags_hi` cleared, the initial parameters (condition 100, liberal arts 40, science 40, art 40, athletics 40, trivia 32, looks 60, guts 5, stress 0), the 21 flag bytes of `progress_state` (pattern 1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,0,0) and the first snapshot.
- `func_80041C2C`: every girl record gets tokimeki 0, affection 0x32, syoushin 0, club 11, flags cleared; girls 0 and 12 are `met`; girl 0 also has bits 3, 6 and 7 set, girl 12 bit 3; birthdays (month/day): 0 2/14 (overwritten by the entry screen), 1 2/3, 2 7/7, 3 9/30, 4 1/13, 5 6/13, 6 12/3, 7 11/15, 8 10/17, 9 9/5, 10 5/16, 11 3/3, 12 8/23, 13 4/4.
- `func_80041F48`: clears appointments, clubs, the queue, `event_flags`, place lists, per-girl bytes and the event log.

NAME_ENT then collects the player's name, birthday and blood type and the birthday and blood type of girl 0, fills `player_full_name`, and sets the starting values of girl 0 (`affection`, `tokimeki`, `syoushin`) from a 4x4 lookup indexed by girl 0's blood-type code and the player's (`D_8014D03C` selects one of the three-value rows of `D_8014D04C`), the clubs (section 5.1) and the blood-type code of every girl (1 -> 1, 2 -> 1, 3 -> 2, 4 -> 1, 5 -> 2, 6 -> 1, 7 -> 0, 8 -> 2, 9 -> 1, 10 -> 0, 11 -> 1, 12 -> 3).

## 14. Fixed-date and gating rules found in the queue code

- 1998-02-15 (a Sunday or holiday in the queue): if at least two girls among 0-10 are met with 501 or more points (`func_80053418`), girl 11's `story_stage` is 4 or more, `event_flags[0x4C]` and `[0x4E]` are nonzero, appointment 0 is not active and `(rand() & 0xF) < 5`, the holiday record becomes special event girl 11 scene 4 and `event_flags[77]` is incremented.
- 1998-01-03: the award checks (types 0x26 / 0x27, section 3.4).
- 1998-02-26: if `event_flags[82]` is 0 and at least 6 of girls 1-9 are met, special event girl 13 scene 0; 1998-02-28: if `func_80071110` is true and `event_flags[20]` is 0, special event girl 2 scene 7.
- Forced encounter (section 3.5) needs: not December 24, `events_today` 0, not Sunday, day kind 0 in this month's table, current record type 0xFB, the first record of the queue not an exam, and the previous record of type 0.
- A birthday record exists for girl 0 always, for girls 1-10 only when they are `met` and have flag bit 6, and for the player unless the player is sick.

## 15. Notes for the port

- Re-implement `game_state`, `system_data` and the records as plain data classes with the field names above; keep fixed-point (16.16) for the parameters and the girl scores (their fractional halves are real state).
- The state machine is the whole game flow: model `mode/stage/step/substep` as explicit state ids (the mode table, section 2.2) or coroutines per mode; `main_state_set` semantics (what it resets) matter because overlays rely on the reset of `step_timer` and `substep`.
- The event queue is rebuilt from the calendar every week, so the save needs no queue; but the **month blobs** (36 calendar entries) and the per-mode scripts are disc data that the port must load.
- Randomness: the game uses `rand()`/`srand()` with the seed in `system_data.seed` and the timer ticks; a port can use any RNG but should keep the probabilities (1/4 encounters, 20% amusement park, 8% invitation roll in `date_sasoi_judge`, etc.).
- The hardware layer (section 9) is replaced, not ported. The contracts to keep are: 320x240 coordinates with the origin at the centre, actor frame/sequence semantics, the text typewriter timing, the sound command ids and the PsyQ pad bit layout (or a mapping).
- Everything that reads `unk_*` in the matched C maps to the names in the table; the apply tool renames fields by compiler error positions, not by text.

## 16. Open questions

1. `girl_cleared` (`unk_53C`): the mascot menu and ETC graduation count it with the saved per-girl flag. Is it an "ending seen" flag? The overlay analysts for ETC and OPTION can settle it.
2. `story_stage` (flag bits 9-11) and flag bit 2 of `GirlFlags` ("introduced"?): events set them in many overlays; needs the DATE/GEKO analysts.
3. `MODE_NAME_ENT_3/4`, `MODE_ETC_C2..C4`, `MODE_TELEPHONE_PROFILE`, `MODE_CLUB_MASTER_*`: roles unsettled.
4. Calendar marker values 2, 6, 10, 14, 16, 20 (notice records) and what the notice mode 0x39 shows.
5. `progress_state` flag bytes (21): which HUD facts they record (first-time hints?).
6. `unk_F6E` (`sd_anim_fast`): the in-game hint names it the SD character animation speed switch (pad 2 START); the animations it changes besides actors 96 and 97 and the parameter panel are not checked.
7. `unk_71F` (`daily_check_done`) and `unk_71D` (`events_today`): names are inferred from set/clear sites only.
8. `scratch_words`/`unk_183C`/`unk_1A3C..`: the shared scratch areas; TACO and NAME_ENT hand pointers through the latter.
9. `frame_hcount_limit`: needs a timing check in the emulator (the emulator image is not built in this tree).
10. Mostly answered, see section 3.2: the first `month_data_table` entry points at the 0x801A0000 read address and 0x95 sectors hold 36 blobs of about 0x2100 bytes, so the blobs come from that one read (the other table entries were not decoded). Still open: whether every blob's marker table is complete.
11. `D_800B3220` (debug flag), `D_800B3C6C` (switches the in-game option mode), `D_80122D48` (rank override of `event_log_add`) have no names yet.
12. Types shared with the dead EVENT overlay (`Rec34` family): EVENT was linked against an older main layout; rename them only as `LegacyGirl*` and do not use them for the port.
13. `unk_1095[0xD]` overlaps an `s32` (`actor_draw_last`, written as `*(s32 *)&unk_1095[3]`, 0x9F or 0xA0 in most scenes). The field rename gives `menu_hit`; splitting it into `menu_hit[3]` + `s32 actor_draw_last` + padding needs a header change that the apply step must check byte by byte.
14. `include/main_api.h` (3,227 lines) keeps its name; splitting it per main module (and moving the enums into `include/game_enums.h`) is a later step. `include/game.h` is proposed to become `include/game_types.h`.
15. File names: only the two headers have `file` rows. The `src/main/*.c` module files that hold the top-100 functions get no English name here; another unit (module level, or the hardware units for the CD, sound and GPU files) must own them, otherwise file rows are added later.

## 17. Reconciliation with the hardware units

The hardware units (`h-cdio`, `h-graphics`) own the entry points of the CD/XA layer, the sound driver and the GPU, sprite and background code. This spec names many of those addresses too, because the frame loop, the state map and the top-100 list use them, and the three proposal tables give 96 rows a different new name ([[data/p2-core-name-conflicts]], generated 2026-10-24). Rule: for every address, field or type in that list the hardware table wins, except the two rows marked `core` there (`girl_state_rank`, the score code, and `mode_boot`, the mode machine). The `core` names in this spec's prose stay as working names so that the text reads on its own; the apply step takes the hardware name and the prose is updated afterwards.

Sound prefix: the three tables disagree (`snd_` in `core`, `sound_` and `bgm_` in `h-graphics`). Proposed single choice: `snd_`, because the developer's own symbols are `Snd_*` and `_snd_*` and `core` already uses it for the hardware entry points it names. The hardware units follow or argue otherwise; the orchestrator decides once, before the apply step.
