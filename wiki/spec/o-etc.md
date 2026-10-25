---
type: spec
updated: 2026-10-24
sources: ["src/ovl/ETC/", "include/ovl/ETC.h", "config/renames/proposals/o-etc.tsv", "config/renames/proposals/core.tsv", "wiki/spec/core.md", "wiki/spec/m-scheduler.md", "wiki/spec/m-sysmenu.md", "wiki/emulator.md"]
---

# Overlay ETC: port spec (exams, career talks, ending tail, album)

Phase 2 analysis of `src/ovl/ETC` ([[tickets/T-27029-p2-o-etc]], phase [[tickets/T-27000-phase-2]]). `config/renames/proposals/o-etc.tsv` is an untrusted mixed-kind draft, not the executable packet. For T-27065, its supported symbol rows were separated into `config/renames/proposals/o-etc-apply.tsv`; its 114 constant candidates and 13 file rows stay out of that five-column table. No applied `config/renames/ETC.txt` ledger was present when this reconciliation began, so existing source spellings remain authoritative until a reviewed table is applied. The corrected packet has 681 rows (394 func, 270 data, 11 overlay-owned fields, 6 types); its read-only Docker plan exits 0. The ETC prefix map is now closed by the root ADR update, and independent review approved the complete symbol packet after the 14 misc-prefix corrections. The separate constant-site and file candidates remain deferred to their own review and scope decisions. Source census found 393 ANSI-style definitions plus the one K&R definition `func_8013E1DC`, covering all 394 function rows. The old draft's constant and file rows remain leads only.

Sources: the matched C of all 13 files, `include/ovl/ETC.h`, the callers in `src/main`, `src/ovl/ENDING` and `src/ovl/OPTION`, the overlay data tables, the core, scheduler and sysmenu specs, and the emulator. Facts are as the matched C implements them; a claim of game meaning that the code does not prove is marked HYPOTHESIS. Scope (user decision 2026-10-24): game logic fully, hardware layer (GPU primitives, VRAM copies, CD reads, sound driver, 3D matrices) only at module level.

Emulator use: `tools/emu` with a local wrapper (build/emu/poke_run.py, not committed) that pokes the overlay target mode, calendar bytes, parameters and event log into the title-menu save state, so each ETC mode could be started directly. Checked this way: exam day score arithmetic (printed values), the results ranking (printed values and the screen), staff roll (both variants), last photo and the all-clear photo, save prompt, album pages, entrance exam result byte (pass, fail and job cases), remedial week flag and calendar marker. Screenshots stayed in `build/emu/` (ignored); nothing from them is quoted here.

## 1. What the overlay is

ETC is a grab bag of seven scene entries that share one overlay file (0x30000 bytes at 0x80132000) and have nothing else in common. The loader picks the entry from the mode byte (`func_80079070` in `src/main/800789E0.c`):

| mode | const | entry (address) | how it starts | what it does |
|---|---|---|---|---|
| 0x35 | `MODE_EXAM` | `exam_entry` (0x801465F8) | queue record type 0x40, once per school day of an exam week (Monday to Saturday) | one day of the final exam: a subject score per weekday, then the results ranking on Saturday |
| 0x36 | `MODE_CAREER_GUIDANCE` | `career_guidance_entry` (0x8014ABD0) | queue record type 0x43, year 97 | the teacher talk: self assessment, choice of university or job, looking up other girls' plans |
| 0x37 | `MODE_CAREER_CHOICE` | `career_choice_entry` (0x801492C0) | queue record type 0x4A, year 98 | the entrance exam (10 questions) for the chosen university or company tier, or the club dream path |
| 0xC2 | `MODE_STAFF_ROLL` | `staff_roll_entry` (0x80142974) | ENDING when the ending scenes are done (also when ENDING is replayed from OPTION) | credits: scrolling staff and cast lines over heroine drawings (flat or on a moving 3D plane) |
| 0xC3 | `MODE_LAST_PHOTO` | `last_photo_entry` (0x8014636C) | staff roll end | the girl's last photo, or the all-clear photo |
| 0xC4 | `MODE_CLEAR_SAVE` | `clear_save_entry` (0x80145A6C) | last photo, album, or any "no" exit | asks to save the finished play to the memory card, counts the clear |
| 0x91 | `MODE_ALBUM` | `album_entry` (0x801407F0) | OPTION album menu (girl from the system file), last photo (live event log) | photo album: pages of four dated photos of one girl's recorded events, voice, closing fade, album save |

The core placeholders `MODE_ETC_C2`, `_C3`, `_C4` are replaced by `MODE_STAFF_ROLL`, `MODE_LAST_PHOTO`, `MODE_CLEAR_SAVE` (const rows). Evidence for the names: disc directories `STAFF2` (per-girl 13-sector art files), `LASTPHOT` (13 files of 33 sectors plus one of 49 sectors, the sizes read by `last_photo_load_step`), the on-screen result in the emulator.

Naming reconciliation: the approved ETC prefixes are `exam_`, `career_`, `album_`, `system_file_`, `staff_roll_`, `last_photo_`, `clear_save_`, `confession_`, `epilogue_` and `misc_`. `confession_` and `epilogue_` are scoped to the unused developer stubs; `misc_` covers shared helpers and the unused calendar functions. Types follow the listed map as `Exam*` and `Album*`; the POLY_F4 layout candidate uses `AlbumPolyF4View` to retain the owning type stem. C-file renames are applied, while `src/ovl/misc_scenes/` remains a later, separate directory-only batch.

Dead code: 25 functions in `80132000.c` (three stub scenes), 2 in `calendar_skip_and_resume.c`, `func_8013F77C` and three animation table setters are never called (section 8).

## 2. How the scenes connect to the game

```
day queue (day_events) ---- type 0x40 ----> exam (0x35) ---- day_event_next ----> day screen (0x31)
                      ---- type 0x43 ----> career guidance (0x36) --- day_event_next(1) --->
                      ---- type 0x4A ----> career choice (0x37) ---- day_event_next(1) --->
ENDING (0x63) -> staff roll (0xC2) -> last photo (0xC3) -> clear save (0xC4) -> prologue (0x11)
                                            |                  ^
                                            +--> album (0x91) -+  (album save, then 0xC4)
OPTION (album item) -> album (0x91) -> album save (stage 0xFF) -> title (0x12) when entered from OPTION
```

- Inputs from main state: parameters, `girls[]`, `clubs[]`, `event_log`, `event_flags`, calendar bytes, `player_flags`, `player_flags_hi`, `current_girl`, `girl_cleared`, `system_data`.
- Outputs: exam scores (`exam_work` in `scene_work`, section 3), `girls[i].tokimeki`, `event_flags[0xC8 + i]`, `player_flags` bits 16 and 17, calendar marker 0x17, the career wish and the career result byte in `player_flags_hi` byte 1 (read by ENDING), the per-girl clear counters `girl_cleared[]` and `system_data.flags[]`, the album copy of the event log in `system_data`.
- `options_dirty` (core name of `GameState` +0x1124) is used by every ETC tail scene as "this session came through the OPTION screens, so there is no live play to save": nonzero skips the clear save and the album save and sends the player to the title (section 6.5, question 2).
- `current_girl` is the girl of the ending that just finished (staff roll, last photo, clear save) or the album girl. A value of 13 or more means "no girl"; `last_photo_leave_to_title` and `album_save_step_write` add 0x10 to mark the album as shown.

Main services the unit calls (core, h-* and m-* names): `mode_step_next`, `mode_step_set`, `mode_stage_next`, `mode_stage_set`, `game_mode_set`, `day_event_next`, `message_window_set_title`, `text_system_reset`, `set_kanji_string`, `k_disp_start`, `gnsx`/`sndisp` (speaker plate and narration), `xa_wait`, `x_taku_menu_set` / `x_taku_string_set` / `menu_girl_taku_set` / `menu_check` / `menu_bar_show` (choice menus), `audio_command`, `audio_bank_load_start` / `audio_bank_load_poll` / `audio_bank_start`, `cd_read_sectors`, `cd_read_sync_status`, `xa_clip_request_set`, `dec_bg_cd_read` / `dec_bg_decode` / `dec_bg_draw`, `portrait_build`, `portrait_face_update`, `event_anim_show`, `biorhythm_value`, `girl_scores`, `calendar_advance_day`, `rand`, the card functions (`card_start`, `card_stop`, `func_80053F04`, `func_80054140` ...). Main functions used here that no proposal names yet: `func_80053F04`, `func_80054108`, `func_80054140`, `func_80054284`, `func_80054388`, `func_8005448C`, `func_80054590`, `func_80054694`, `func_80054704`, `func_80054E6C`, `func_800552DC`, `func_80055A38`, `func_800536AC` (memory card save/load steps), `func_80061710`, `func_80061790`, `func_800618B0`, `func_8004111C`, `func_8009C5E0` (display setup), `func_800A0140` (the cosine call: emu printed cos(1871 / 4096 turn) = -3946 of 4096; next to `rsin`, so it is `rcos`).

## 3. Final exams (mode 0x35)

### 3.1 When it runs

The queue builder (scheduler spec 3.4) puts a type-0x40 record for each school day of an exam week, Monday to Saturday; `GameState.weekday` is the selector (0 to 5). Mondays to Fridays are the five subjects, Saturday is the results announcement. The exam rounds known to the code: July and December of every year and February of years 96 to 98 (the correction table index, 3.3). `exam_entry` runs `stage` 0 (setup), 1 (the day), 2 (end), then per frame the HUD calls (window title, date, parameter panel, cursor, icon bar), the mascot update (hidden on Saturday) and six debug digits per column at x 0x40 (day scores) and x 0x50 (grades) that `debug_print_hex` shows only while the debug flag is set.

State is kept in `exam_work` (type `ExamWork`, view of main's 0x40-byte scratch area `scene_work` at 0x801230D0): `subject_grade[6]` (grade 0-4 of days 0-4; slot 5 counts red marks, the days with grade 0) and `subject_score[6]` (day score 0-100 of days 0-4; slot 5 the running total). They are zeroed on Monday only (`exam_setup_scores_reset`), so a week that starts on another weekday keeps the old values.

### 3.2 Flow

| stage | step | function | what |
|---|---|---|---|
| 0 | 0 | `exam_setup_init_screen` | clears actors, hides icons, shows the title, starts loading the exam sound bank pair on the first exam day (Monday, or the record two back in the queue is not an exam; `queue_back2` is the `ExamQueueView` view that starts two records before `day_events`) |
| 0 | 1-2 | `exam_setup_background_start`, `_wait` | waits for the bank and starts BGM 0x205 (later days: only if none plays), classroom background (resource 0x3FA4) from CD or cache, decode, brighten |
| 0 | 3 | `exam_setup_scores_reset` | Monday: zero `exam_work` arrays; `mode_stage_next` |
| 1 | by weekday 0-4 | `exam_day_step_language` .. `exam_day_step_art` | steps 0 SD pack, 1 start, 2 wait, 3 comment, 4 `mode_stage_next` |
| 1 | by weekday 5 | `exam_results_step` | steps 0 show, 1 react, 2 wait |
| 2 | 0 | `exam_end_init` | panel init, clears the upper actors |
| 2 | 1 | `exam_end_leave` | Saturday returns to the main sound bank (`audio_bank_start(0, 0)`); `day_event_next(0)` |

An exam day (weekday 0-4):
- step 0 `exam_day_sd_pack_load`: reads the 9-sector SD pack of the season (LBA 0x7D78 in months 6-9, else 0x7D81, both in `misc_sd_pack_lba_other` and the word before it) to 0x801E0000 and binds it (`addr_init_else_sd`).
- step 1 `exam_day_start_<subject>`: one narration line that names the subject (day 1 language, 2 math, 3 science, 4 social studies, 5 art), then `exam_score_day()`; a grade below 2 sets the player's face to the sad frame set (`exam_player_face_set(1)`), else the happy one.
- step 2 `exam_day_wait_anim`: at step_timer 0x20 forces the face animation flag, then plays SE 0x601 (happy) or 0x602 (sad) once and waits 0x11 frames.
- step 3 `exam_day_comment`: one of five narration lines chosen by the day's grade (hopeless, weak, fair, good, perfect), waits for the text.
- step 4: `mode_stage_next` (stage 2, which ends the scene with `day_event_next(0)` and returns to the queue reader).

### 3.3 The day score (`exam_score_day`)

Let `v(p)` be a parameter's `value` (0-999) and `bio(a, b, c)` = `biorhythm_value(a, b, c, 16)` with a = physical, b = emotional, c = intellectual wave flags (m-hud spec section 12; it uses the day counter and the player's birthday).

| weekday | subject | `main` | `sub` | `bio` |
|---|---|---|---|---|
| 0 | language | v(liberal arts) | 0 | bio(0,0,1) |
| 1 | math | v(science) | 0 | bio(0,0,1) |
| 2 | science | v(science) x 7 / 10 | min(v(trivia) x 3 / 10, 38) | bio(0,1,1) |
| 3 | social studies | v(trivia) x 7 / 10 | min(v(liberal arts) x 3 / 10, 38) | bio(0,0,1) |
| 4 | art | v(art) x 8 / 10 | min(v(trivia) x 2 / 10, 25) | bio(0,1,0) |

```
ret = main + sub + bio - 8
if (health state & 2)        /* neurosis or sick: bit 1 of player_flags byte 1 low nibble */
    ret = ret / 2
if (ret < 0) ret = 1
ret += exam_correction_bonus()
ret += (rand() % 10) * exam_event_log_bonus(0xF)     /* the second factor is 0 or 1 */
if (ret < 128) score = ret * 100 / 128 else score = 100 - (rand() & 3)
grade = score < 40 ? 0 : score < 60 ? 1 : score < 80 ? 2 : score < 95 ? 3 : 4
if (grade == 0) red_marks += 1
subject_score[weekday] = score; subject_score[5] += score; subject_grade[weekday] = grade
```

Emulator: weekday 1, July 1995, science 40 printed `main 40 sub 0 bio 6 ret 38`, correction 14 and day score 40 (52 x 100 / 128), total 40.

`exam_correction_bonus` returns `exam_correction_table[round]` when `(rand() & 7) + value - 8 > 0`, else 0. The round index is 3 for year 96 and 6 for year 97 or later, 0 for year 95, minus 1 in February, plus 1 in December (July and other months add nothing). Table values: 14, 12, 10, 8, 6, 4, 2, 0. So the July 1995 round always gets 14, December 1995 12, February 1996 10 (always), July 1996 8 (7 in 8), December 1996 6 (5 in 8), February 1997 and February 1998 4 (3 in 8), July 1997 2 (1 in 8) and December 1997 0. The bonus shrinks as the game goes on.

`exam_event_log_bonus(mode)` scans `event_log` (stops at the first entry with year 0) for entries dated the current year and month and returns 9 when `mode` is 8 and the entry is category 2 scene 0x3B, 1 when `mode` is 0xB and the entry is category 0xB scene 0xF, and 1 for a category 0 scene 0xF entry when `mode` is 0xF (any girl) or the girl id of the entry. Category 0 is a finished date (DATE logs category 1 at the start of a spot's scene and replaces that record with category 0 when the date ends) and scene 0xF is the library spot (the DATE branch that sets spot 0xF also sets the library place name). So the player's own bonus is a random 0-9 points when a date at the library was completed this month, and a girl's value gets +0..49 for the same; HYPOTHESIS: it is meant as studying together.

### 3.4 Results day (weekday 5)

`exam_results_show`: substep 0 shows a narration line and starts a 13-sector read of the results art (LBA 0x9CA8); substep 1 waits until step_timer passes 0x100; substep 2, once the read is done, builds the ranking with `exam_ranking_build`, draws the panels and sets up 14 row actors from `exam_ranking_order` (row 0 is the best). Row i belongs to person `exam_ranking_order[i]`:
- person 13 (the player): his full name;
- person 10: the name of table entry 13, the friend (emulator: the friend's name appears in the list; girl 10 herself never appears);
- person 11 (Miharu): her name only when her story stage (`GirlFlagBits.f9`) is nonzero, otherwise the row is blank;
- other persons: the name only when the girl is `met`.

`exam_ranking_build` (once per results day):
1. `srand(frame_count)`.
2. For persons 0-12: `value = ((exam_base_score[i] + a) * 5) + n` with `n` and `a` each `(rand() % exam_score_spread[i])` with a random sign. Base scores (u8[13]): 92, 80, 81, 65, 61, 63, 33, 35, 31, 68, 40, 80, 79. Spreads: 3, 7, 7, 7, 7, 7, 15, 15, 15, 7, 15, 7, 7. So girl 0 sits near 460 and the weakest near 150.
3. Bonus from `exam_event_log_bonus(i)`: 1 adds `rand() % 50`, 9 adds `rand() % 250`; the value is capped at 500.
4. Person 13 is the player's total (`subject_score[5]`, 0-500).
5. Ties are broken by decrementing the later value until all 14 differ.
6. `exam_ranking_order[j]` = person with the j-th highest value (selection sort).
7. Rank numbers: `angle = (value << 12) / 1000`, `rank = cos(angle) * 200 / 4096 + 200 + offset` with `offset = rand() & 3`: a high value gives a small number (emulator: value 457 gave 8, 399 gave 40, 165 gave 302). HYPOTHESIS: this is a class rank on a scale of about 1-400. Equal ranks are separated by +-1 by value. Then, with `min` the smallest rank number: when `min >= 9` every rank is lowered by `(min - 8) + rand() % (min - 8)`; when `min <= 0` every rank is raised by `1 - min`. When the best value is 466 or more, every rank is lowered by (rank of that person - 1) so he or she is exactly number 1. (With the base score table the best value is always near 460, so `min` stays in the range of about 5 to 13 and the shifts are small.)
8. If the player is first in the order and person 0 second, and person 0's number is smaller (better) than the player's, the two numbers are swapped; if the player is first and his number is 0 or less, every number is raised so his is 1.
9. Relationship effect: for every girl index 0-10 whose value is below the player's: the first time (`event_flags[0xC8 + i]` clear) `girls[i].tokimeki += 11` and the flag is set, else `+= 2`. If the player's rank number is 1, every girl 0-10 gets `+ 2` more. (Index 10 is the friend's value in the list but the bonus goes to the record of girl 10.)

The panel draws value and rank digits with `exam_number_draw` (8x12 digit sprites; rank in the left column, value with a unit mark in the right).

`exam_results_react` (step 1, after 0x79 frames): the first rule that holds wins.

| rule | message | BGM | flags |
|---|---|---|---|
| red marks 3 or more | forced remedial week | 0x203 | `player_flags` bit 16 and bit 17 are set; core exposes these positions as bits, not overlay-owned field names, `month_calendar[0].marker[day + 1] = 0x17` |
| `exam_rank_player` below 2 | top of the class | 0x204 | none |
| `exam_player_in_lower_half` (row 7 or lower) | exam finished | 0x200 | bit 17 set |
| else | exam finished | 0x201 | none |

Bit 17 is cleared every results frame before the test. Emulator: 3 red marks left `player_flags_hi` byte 0 = 0x03 and the marker 0x17 at tomorrow's calendar slot. The command screen reads bit 16 as the forced remedial week (m-sysmenu); `calendar_advance_day` clears it when the weekday becomes Sunday unless the first queued record is an exam (m-scheduler). No reader of bit 17 was found in the matched code (question 8).

`exam_results_wait` (step 2) waits for circle after 0x101 frames or cross, shows the cursor and leaves through stage 2.

### 3.5 Data

| symbol | size | content |
|---|---|---|
| `exam_correction_table` | s32[8] | 14, 12, 10, 8, 6, 4, 2, 0 |
| `exam_base_score`, `exam_score_spread` | u8[13] each | listed in 3.4 |
| `exam_ranking_value` | s16[14] | person values; aliases `exam_value_friend` [10], `exam_value_miharu` [11], `exam_value_player` [13] |
| `exam_ranking_order` | s32[14] | person id by position |
| `exam_ranking_rank` | s32[14] | displayed rank number; aliases `exam_rank_friend`, `exam_rank_miharu`, `exam_rank_player` |
| `exam_row_x`, `exam_row_y` | s16[14] | row positions; `exam_digit_u` u8[10] digit u offsets |
| `misc_sd_pack_lba_other` | u32[2] | LBAs 0x7D81 and 0x7D8A of two 9-sector SD packs (disc dir ETC_SD); the word before it, 0x7D78, is the summer pack (index -1) |

## 4. Career guidance (mode 0x36)

One conversation with the teacher at the start of year 97 (the first narration line says the third school year has begun). Stages: 0 setup (staff-room background 0x4055, title), 1 `career_guidance_stage_talk`, 2 `career_guidance_stage_choice`, 3 `career_guidance_stage_ask_others`, 4 university answer, 5 job answer, 6 leave (`day_event_next(1)`).

**Stage 1 step 0** `career_guidance_intro_line`: one narration line (the third year has started).

**Stage 1 step 1** `career_guidance_self_assessment` (substeps; the value of substep selects the branch):
- `career_guidance_assess_params` counts parameters 1-7 (liberal arts, science, art, athletics, trivia, looks, guts) whose value is at least 101 (`CAREER_PARAM_HIGH`) and fills four counters: `career_guidance_academic_deficit` (0-3: liberal arts, science and art below 101), `career_guidance_body_mind_deficit` (athletics below 101 adds 1, guts below 101 adds 2), `career_guidance_trivia_deficit`, `career_guidance_looks_deficit` (1 each).
- count 7: an optimistic line, then straight to stage 2. Count 0: a pessimistic line, then only the closing remarks. Otherwise a neutral line followed by one remark per deficit (academic 1/2/3 has three wordings, body and mind 1/2/3 three, trivia one, looks one).
- `career_guidance_closest_girl`: from `girl_scores` closeness it takes the met girl 1-10 with the highest closeness strictly above 60 (`CAREER_CLOSE_GIRL_MIN`); if one exists, `current_girl` is set to her and a remark names her (girl 0 is not considered).
- Last line: a resolve to try hard; `mode_stage_next`.

**Stage 2** `career_guidance_question` / `career_guidance_choice_menu`: two narration lines of the player (the consultation is today; what should he do) and then a 3-choice menu (university, job, look at others). Circle on choice 0 goes to stage 4, 1 to stage 5, 2 to stage 3.

**Stage 3** (look up others): `menu_girl_taku_set` shows the 12-item girl menu (item i is girl i, item 11 is girl 12; unmet girls are disabled); circle shows `career_guidance_girl_plan_line[cursor]`, a fixed one-line statement of that girl's plan (university or company tier, study abroad, vocational school, a sport) with the player's comment, waits for the text and returns to stage 2. The table has 16 entries: 0-9 the girls, 10 and 13 the friend's line, 11 and 12 the rival's line, 14 girl 10's own line (still a second-year), 15 girl 11's line (unknown to the player). The menu reaches entries 0-11 only, so cursor 10 (girl 10) shows the friend's line and cursor 11 (the rival) shows the rival's line; the lines for entries 14 and 15 cannot be reached (question 5).

**Stage 4** (university) and **5** (job): step 0 reads the teacher background (resource 0x408D). Step 1 sets the career wish in `player_flags_hi` byte 1 (`CAREER_WISH_UNIVERSITY` 0, `CAREER_WISH_JOB` 1), says the wish, then the teacher gives a verdict line with a voice clip, and the player reacts. The verdict (0 best, 3 worst; four lines each):
- university, `career_guidance_university_verdict`: `n` = number of liberal arts, science, art values of 121 or more, `sum` = the sum of (value - 120) over them. If `n == 3` and `sum >= 201` the verdict is 0; else if `n != 0` the verdict is `3 - (n - 1)`; `n == 0` gives 3. In words: first-class university, second-class, third-class, "you may become a ronin".
- job, `career_guidance_job_verdict`: `m = 6 - 2 * university_verdict`, +1 if trivia is 61 or more, +1 if looks is 61 or more; verdict 0 for `m >= 7`, 1 for `m >= 5`, 2 for `m >= 3`, else 3 (first, second, third-class company, none).
- the player's reaction line is one of four short words chosen by the same number.

Stage 6 leaves with `day_event_next(1)`. Emulator: all three academic parameters 120, trivia 70 printed `thl 0` (verdict 3) and the parameter list.

Output: `player_flags_hi` byte 1 = wish (0 university, 1 job). Mode 0x37 reads it.

## 5. Career choice and the entrance exam (mode 0x37)

The last big decision, in year 98. Stages: 0 prompt, 1 university exam, 2 job exam, 3 club dream, 4 leave (`day_event_next(1)`).

**Stage 0**: `career_choice_init_screen` resets the question counter, the correct counter and the tier and picks `career_choice_threshold_start = rand() % 12`. The prompt line depends on the wish (exam day of a university or of a company). A menu of two choices (the wish exam and the other exam) or three (plus the club dream when `career_choice_mastered_club` returns a club, see below). Choice 0 starts the wish exam, choice 1 the other one, choice 2 the club dream.

**Stage 1 / 2** (same shape, university or company wording):
1. step 0: sets the wish byte (stage 1: 0, stage 2: 1), asks the tier, 3 choices: first, second, third class.
2. step 1: stores `career_choice_tier` 0-2.
3. step 2: the player's line for the tier and a venue line.
4. step 3: exam-room background (resource 0x463B), the parameter panel is hidden.
5. step 4: loads the sound bank pair, starts BGM 0x205. Step 5: reads the SD pack (LBA 0x7D81) and builds the player's face actor.
6. step 6, 10 questions: each question `q` (0-9), subject `q % 3` (0 liberal arts, 1 science, 2 art):

```
s = v(subject param) + min(club_exp(subject club) >> 4, 20)   /* clubs: 0 literature, 2 science, 4 art */
if (player_flags health nibble != 0) s = s / 2                /* any health problem */
if (tier == 1) s += 0x26; if (tier == 2) s += 0x36
correct = s >= career_choice_threshold[(start + q + 1) % 12]
```
   Thresholds: 66, 66, 114, 82, 98, 82, 66, 98, 98, 82, 114, 114. After each question the face plays a right or wrong animation (SE 0x501 or 0x502), 0x3D frames.
7. grade from the number of right answers: 10 gives 0, 8-9 give 1, 7 gives 2, 5-6 give 3, 3-4 give 4, 0-2 give 5.
8. step 7: a reaction line and BGM by grade (0: 0x204, 1 and 2: 0x201, 3: 0x200, 4 and 5: 0x203), then the pass-announcement date line (March 1), then the result byte, then stage 4.

**Result byte** (`player_flags_hi` byte 1, read by ENDING, constants `CAREER_RESULT_*`):

| path | passes when | byte |
|---|---|---|
| university, tier 0 / 1 / 2 | grade 0 or 1 | 0x01 / 0x02 / 0x04 |
| company, tier 0 / 1 / 2 | grade 0 or 1, or grade 2 with trivia + looks above 256 (`career_choice_interview_ok`) | 0x08 / 0x10 / 0x20 |
| any of the above | otherwise | the same bit OR 0x80 (failed) |
| club dream | club exp 0x19B (411) or more | 0x40; otherwise 0xC0 |

Emulator: liberal arts, science, art 120, trivia and looks 70, tier 0 gave result 1.

**Club dream**: `career_choice_mastered_club` returns the first club 0-10 whose `ClubFlagBits.club_progress_grade` is 3 or more and whose separate `ClubFlagBits.b30` bit is set. `club_progress_grade` is the current canonical field name for the two-bit progression grade/count; interpreting `b30` as `mastered` remains a HYPOTHESIS. In the matched C the grade is raised only by MASTER (club 6), the queue builder (club 11) and DATE (club 11), so only club 6 can qualify, although the dream texts exist for all 12 (`career_club_dream_name`, `career_club_dream_line`; question 6). Choosing it prints the dream line and writes 0x40 or 0xC0 without an exam.

## 6. The ending tail (modes 0xC2, 0xC3, 0xC4)

### 6.1 Staff roll (0xC2)

Stage 0 loads, stage 1 runs. `staff_roll_load_step` (22 substeps) picks the variant: `staff_roll_variant` is 1 (3D variant) when exactly one of `misc_all_girls_cleared()` and "triangle held" is true, else 0 (picture slideshow). `misc_all_girls_cleared` is true when each of girls 0-12 has a nonzero ending counter in `girl_cleared` or in `system_data.flags`. The loader reads two art sets (the 3D variant uses LBAs near 0x7BD0, the picture variant LBAs near 0xB2CC; 13 sectors per file) and palettes. If `current_girl` is 13 or more at the start it jumps to mode 0xC4 (or 0x12 when `options_dirty`).

Common to both: the CD clip and BGM start (`staff_roll_music_start`), seven scrolling credit-line actors (`staff_roll_credit_lines_update`: one pixel up per frame, a line that left the top takes the next of the 0x11D table entries `staff_roll_credit_table` (frame index, x)), and the exit rule: after at least 0x259 frames and when the CD clip is done (`cd_state & 2`) the scene moves on (`staff_roll_models_leave`: fade, then mode 0x12 when `current_girl` is 13 or more, else 0xC3).

Variant 1 (3D): `staff_roll_models_step` shows the art of one girl per 701-frame segment in the order `staff_roll_girl_order` (14 girls: 0, 10, 13, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11), loading her textures at the start of each segment (`staff_roll_girl_textures_load`), and moves it along a sine/cosine path; the pad moves and rotates it (debug controls). Emulator: the "model" is a flat picture plane in 3D, the sepia drawing of the girl, skewed by perspective as it moves; the credit text scrolls over it exactly as in variant 0, and the two variants load different art sets. Variant 0: three picture quads with random y that fade in and out per segment. While the clip plays, pad 2 bit 0x200000 jumps to mode 0xC4 and bit 0x400000 to mode 0x91 (both to mode 0x11 when `options_dirty`).

### 6.2 Last photo (0xC3)

`last_photo_load_step` reads the photo of `current_girl`: LBA from `last_photo_lba[girl]` (33 sectors) unless the all-clear photo is chosen (`last_photo_all_clear_lba`, 49 sectors): when start is held, or when it is 1998-03-01, every girl but `current_girl` has a counter, and `current_girl` is the one missing (this clear completes the set of 13). Fade in over 0xFF frames, music request, then waits for circle, cross or start, or 0x709 frames. Stage 2 chooses the exit: `options_dirty` nonzero goes to the title (with `current_girl += 0x10`), a confirm button held on this frame goes to mode 0xC4, otherwise mode 0x91 (album).

### 6.3 Clear save (0xC4)

Stage 0: boot data and HUD textures, then "save this play?" with yes on circle and no on cross (waits 0x80 frames); yes starts the card check and enters stage 1, no enters stage 2. Stage 1 is the memory-card flow: step 0 card check and directory (result 1 ok, 0xF1 unformatted card goes to step 0xA0, 0xFF or a timeout of 0x200 frames goes to step 0xFF), step 1 builds the 14-slot menu (clone of the main save screen), step 2 selects a slot (circle on an empty slot tests that a file can be created; a new slot goes through step 0xC0), step 3 writes. `clear_save_write_step` first adds 1 (cap 99) to `girl_cleared[current_girl & 0xF]` and `system_data.flags[...]`, before writing the slot and, when needed, the system file. The counters are not rolled back if the write fails; they record an attempt by the save flow, not only a successful saved clear. On success stage 2 fades out and starts mode 0x11. Error steps: 0xA0 (unformatted, offer to format or use another card), 0xF0 (no free space, offer another card), 0xFF (no card recognised, press a button and retry).

`girl_cleared` (`GameState` +0x53C) and `system_data.flags` are per-girl ending counters (cap 99), incremented before the clear-save routine attempts the file write; the saved copy persists across new games through the system file. `girl_cleared[girl] & 1` also picks the album closing voice.

### 6.4 Memory-card prompt helpers

`misc_card_confirm_label` and `misc_card_cancel_label` return the button labels shown in the yes/no lines: pad gives circle and cross, mouse gives the right or left button by `confirm_button` (opposite for cancel). `misc_card_yes_text` and `misc_card_no_text` are the two label strings.

### 6.5 `options_dirty`

ENDING jumps to mode 0xC2 when it is nonzero, OPTION sets it to 1 on entry (`func_80132BA8` case 0) and ORs option groups into it, and NAME_ENT clears it for a new game. In ETC it always means "no live play": `album_save_step_begin` leaves to the title, `staff_roll_*` and `last_photo_*` skip the save, `album_event_record_load` reads the saved copy instead of the live event log, and `album_cleared_level` returns 0.

## 7. Album (mode 0x91)

### 7.1 What it shows

A photo album of the events of one girl. The source is a list of up to 256 records: `event_log` while a play is running (`options_dirty` is 0, `current_girl` = the finished ending's girl), or the copy stored in `system_data` +0x58 (0x800 bytes) when entered from OPTION (`current_girl` is read from the first record's girl byte). A record is an 8-byte `AlbumLogEntry`: `girl`, `scene`, `category`, `mood`, `year`, `month`, `day`, `weekday` (core `EventLogEntry` uses the same `scene` and `category` names for these bytes; its `rank` byte is `mood` here). The viewer skips records of other girls and empty records (year 0).

Per record `album_event_record_load` fills `album_event_*` and the season (`album_event_season`: months 3-5 spring, 6-8 summer, 9-11 autumn, 12-2 winter). `album_event_picture_select(category, scene, girl, season)` decides how it is shown:
- a portrait file (`album_event_is_portrait` = 1): `album_portrait_start_*` read an LBA from a per-girl table (seasonal tables, school-wear tables by month, ordinal tables) and start the portrait read; the portrait is built with the record's mood (`portrait_build`);
- an animation (category 2, or category 8 scenes 0xE and 0x46): `album_anim_<scene>_show` binds the pointer block read from `album_anim_lba[scene]` / `album_anim_sectors[scene]` and starts it with `event_anim_show`; the 72 setters have five assignment-count patterns: 59 write 11 fields, 5 write 13, 5 write 7, 2 write 18 and 1 writes 5. Three of the 13-assignment setters write contiguous Extended13 blocks; two write their own Standard11 block plus two fields in another scene block. The 7-assignment setters write a palette and six image pointers; the 5-assignment setter writes three tables, a count and a palette but no images; the 18-assignment seasonal setters write two palettes and two six-image sets. Each `_show` function calls its pack setter except `album_anim_0d_show` and `album_anim_2d_show`, which call the 0C and 2C setters, and `album_anim_4b_show`, which sets no tables and passes 0, 0, 0, 0 to `event_anim_show`;
- the background (`album_event_background_load`): a CD read of the picture behind the girl from `album_background_lba_table[category][scene]`, with a few swaps by year and season.

Where the records come from (`event_log_add(category, scene)` callers, by overlay): the album groups them by `category`, so the table below tells which scene produces which pictures.

| category | logged by (scene ids) |
|---|---|
| 0, 1 | DATE: the date spot id (`D_80122D08`) and a few fixed ids |
| 2 | animated events: SHUGAKU (0x13, 0x1A, 0x20, 0x27), TAIIKU (0xD, 0x2D), SHOUGATU (6, 0x34, 0x42), DATE and GEKO with a variable scene |
| 3 | SHUGAKU trip scenes |
| 4, 5, 7 | SHOUGATU (4: 0 and 1; 5: 0-5; 7: 0) |
| 6 | VALEN (0-5) |
| 8 | SHOUGATU (girl x 10 + n) |
| 9 | GEKO (0) |
| 0xA | DATE (spot id), GEKO (0x25-0x28, 0x2F), KANGEI (0x2D, 0x30, 0x31, 0x3E), SHOUGATU (0x2E, 0x32), SHUGAKU (0x29-0x2C) |
| 0xB | DATE (spot id) |
| 0xC | DATE2 (0) |
| 0xD | DATE (spot id), GEKO (0x25), KANGEI (0x2A), OMIMAI (0x25), SHOUGATU (0x2C, 0x2D), SHUGAKU (0x26-0x29) |
| 0xE | KANGEI (0xB, 0xC, 0xD and one variable scene) |
| 0xF | OMIMAI (0) |
| 0x10 | KANGEI (the player's club id) |
| 0x12 | RENSYU |
| 0x14 | BUNKAKEN (0x19, 0x1A), BUNKASAI (0x1B-0x22) |
| 0x15 | MASTER (6) |

### 7.2 State machine

| stage | step | function | what |
|---|---|---|---|
| 0 | 0 (14 substeps) | `album_load_step` | draw switches, banks, model and UI picture packs (0x69 sectors at LBA 0xB389), portrait strips, intro voice clip, fade in with the ALBUM title card, wait for circle, spin the model up to angle 0xC00, `mode_stage_next` |
| 1 | 0 | `album_playback_page_start` | skip to the first valid event of the page, request its resources |
| 1 | 1-4 | `album_playback_photo`, `album_playback_photo_last` | the loaded photo is shown big, waits for circle only in secret modes 2 and 3 (modes 0 and 1 go on by themselves), then zooms into its cell (top-left, top-right, bottom-left, bottom-right) over 0x28 frames while the loader fetches the next record |
| 1 | 5 | `album_playback_voice` | one voice clip of the girl chosen at random from her clip table (`album_voice_clips`, counts `album_voice_clip_count`: 21, 10, 9, 11, 14, 11, 8, 8, 8, 8, 10, 4, 8; never the previous clip, and a few clips excluded for girls 0, 2, 3, 4, 9); skipped for girls 11 and 12, who use `album_special_girl_voice` instead |
| 1 | 6 | `album_playback_end_check` | stage 2 when the record has no date or the index is 0xFF, else step 7 |
| 1 | 7 | `album_playback_ui_restore` | reload the UI picture set if a special animation replaced it, wait for circle, spin the model, back to step 0 for the next page |
| 2 | clock `stage_scratch` 0-5 | `album_stage_finale` | closing sequence: girl-specific voice for girls 0, 2, 4, 12 after 0x79 frames, the girl's closing clip `album_finale_clip`, then fade to white over 0x12D frames, reset and jump to stage 0xFF |
| 0xFF | 0-6, 0xA0, 0xF0-0xFF | `album_stage_save` | save the album to the system file (7.4) |

The start button anywhere before stage 0xFF requests the skip: it clears the buffer, sets the model angle 0x800 and jumps to stage 2. The date of every photo is drawn as year, month, day digits in its corner (`album_date_counters_draw`).

A page therefore holds four photos; the emulator showed them in a 2x2 grid with date stamps for a fabricated four-record log.

### 7.3 Secret mode

`album_secret_mode` (0-3, starts at 0) is changed in `album_secret_mode_update`: in the intro (`album_phase` 0), when `misc_all_girls_cleared()` is true and pad 2 select or start is held, it becomes 1 and SE 0x50E plays; after the intro it becomes 2 (3 if it was 1) when the secret-code latch `D_800B3CFC` is set. Effects found in the code: modes 0 and 1 advance each photo by themselves, modes 2 and 3 wait for circle; modes 0 and 2 skip records of other girls (`album_event_skip_other_girls`), modes 1 and 3 do not, and read the previous record for the special girl-11 and girl-12 voices. HYPOTHESIS: bit 0 is the "show every girl's records" extra album unlocked by clearing all 13 girls, bit 1 a manual mode (question 7). Three more checks run each frame whose results are not used (`album_cleared_level`, `album_event_flag_count_check`, `album_all_met_check`).

### 7.4 Album save (stage 0xFF)

Steps: 0 begin (to the title if `options_dirty`), 1 background (logo art, LBA 0xA1A2), 2 ask ("save this album mode data to the system file?"), 3 answer (circle go on, cross stop sound and go to mode 0xC4), 4 card check (as 6.3), 5 load the system file (`func_80054284` result 3; failure goes to step 0xFD), 6 write: `album_event_log_pack` clears the 0x800-byte album area, copies every `event_log` record whose girl equals `current_girl & 0xF` (no per-record date test: it only needs the calendar bytes nonzero, so for girl 0 empty records are copied too and the viewer skips them; the count can reach 256), stores the count (`system_data` +0x28) and the flag (+0x27 = 1), writes the file, plays the girl's closing clip (second clip table when her counter is odd), fades out, `current_girl += 0x10`, mode 0xC4. Error steps: 0xA0 unformatted card, 0xF0 no system file (offers to create it), 0xFD broken system file (offers to rebuild), 0xFE no free blocks, 0xFF no card.

## 8. Dead code

- `80132000.c`: three stub scenes with the developer titles of a confession judgement flow, a confession scene flow and an epilogue flow. Each stage only sets a window title, shows the text screen and waits for pad bit 0x40 (or frame counter ends in 0x2FF); the flows end with `game_mode_set(0x63)`, `(0xC1)` and `(0xC2)`. No entry or table references them (`func_80079070` has no such entry; mode 0xC1 has no handler).
- `misc_unused_skip_week_a/b` (`calendar_skip_and_resume.c`): advance the calendar six days and call `day_event_next(1)`.
- `album_debug_fill_miharu`: fills `girl_cleared` with 1 (girl 11 with 5), sets `current_girl` to 11 and writes five event records for girl 11: a fixture for testing the album.
- `album_anim_unused_a/b/c_tables_set`: pointer blocks of animation packs that no scene plays.

## 9. Files and header

The file-only renames have been applied. C files remain in `src/ovl/ETC/`; the ADR's destination folder `misc_scenes` and header rename are later, separate batches.

| current file | content |
|---|---|
| `career_guidance.c` | mode 0x36 |
| `career_choice.c` | mode 0x37 |
| `final_exam.c` | mode 0x35 |
| `staff_roll_and_clear_save.c` | staff roll stages, clear save and last photo |
| `staff_roll_asset_load.c` | staff roll loader |
| `album_save_flow.c` | album save flow, card helpers, and the mode 0xC2 entry |
| `album_mode_entry.c` | album entry and loader |
| `album_playback.c` | album stages 1 and 2 |
| `album_portrait_selection.c` | portrait resource choice |
| `album_model_setup.c` | model setup and number sprites |
| `album_effects_and_animations.c` | VRAM blur, fades, background loader, animation packs |
| `confession_and_epilogue_handlers.c` | unused developer stubs |
| `calendar_skip_and_resume.c` | unused six-day skip helpers |

`include/ovl/ETC.h` holds about 800 externs for animation pointer-block members. The 72 setters have five assignment counts (5, 7, 11, 13 and 18), but that does not imply five uniform contiguous block layouts: two setters also write fields belonging to other scene groups. A single `AlbumAnimPack packs[72]` is not supported by the observed writes. A future hardware-data cleanup could use layout-specific records or retain the member labels as listed hardware residue; it must preserve the per-scene group layout and cross-block writes.

## 10. Open questions

1. `girl_cleared` (`GameState` +0x53C) is settled here as a per-girl ending counter (6.3); a rename to `girl_clear_count` would say so better than the core name.
2. `options_dirty` (core name) behaves in ETC as "came through OPTION; no live play". The name may mislead; the byte is OR-ed with option group bits by OPTION.
3. The current `EventLogEntry` declaration in `include/game/day_event.h` names byte +1 `flag`, byte +2 `scene`, and byte +3 `rank`. `EtcConfig8` is an album-specific record view: its `category`/`scene` interpretation follows `event_log_add` callers, which pass `(category, scene)`. Its byte +3 view is `mood` because the album uses it modulo 6; the shared record still calls that byte `rank` (the value from `get_g_zyotai_h`). Keep the album-specific view difference explicit without treating its private labels as the shared ABI names.
4. Exam ranking person 10 is drawn with the friend's name; girl 10's own value does not appear, yet the tokimeki bonus loop treats index 10 as girl 10. Intended or a mapping slip is unknown.
5. `career_guidance_girl_plan_line` has 16 entries but the 12-item menu reaches 0-11: cursor 10 (girl 10) reads the friend's line, cursor 11 (rival) the rival's line; entries 14 and 15 are unreachable.
6. The club dream path (`career_choice_club_dream`) needs `club_progress_grade` 3 and the separate `b30` bit; in the matched C only club 6 can reach grade 3 (MASTER), plus the no-club record. The dream texts exist for all 12 clubs. The grade/count role is named; interpreting `b30` as `mastered` remains HYPOTHESIS.
7. `album_secret_mode` and the latch `D_800B3CFC`: what sets the latch, and what modes 1-3 show to the player, was not run in the emulator.
8. Bit 17 of `player_flags` is set by the exam results handler but has no reader in the matched code; keep it unexplained until the core owner proves its meaning.
9. That the exam bonus for a finished library date means "studied together" is a guess; the category and scene ids themselves are read from DATE (see 3.3). The girl 11 case (category 0xB scene 0xF, logged by a DATE routine with the current spot id) and the girl 8 case (category 2 scene 0x3B, an animated event) were not traced to their scenes.
10. The 72 animation setter groups were named by scene id; which event each pack shows was not checked (the emulator run used four fabricated records). Their assignment counts are 5, 7, 11, 13 and 18, and two 13-assignment setters include two writes into other scene groups. Do not flatten them into one uniform type. The pointer members are hardware-facing pack wiring. All 37 career text labels now have role-based rows in `o-etc-apply.tsv`.
11. `staff_roll_credit_table` pairs and the 701-frame segment timing depend on the CD clip length; the port may drive the scroll by time instead.
12. Not renamed (main scope): `D_800B3CFC` (secret-code latch), `D_8011F553` (parameter panel flag bit 7 in the sprite state), `D_800E8BEE` (core: `slot_info_from_system_data`), `D_800E7D68` (first album record girl in `system_data`, read by OPTION), and the memory-card functions listed in section 2.

### 10.1 Overlay data coverage and residue

The Docker reference census found 950 distinct ETC-owned `D_` labels referenced by the C bodies. The initial 233 data rows covered 198 referenced labels plus 35 table-only roots. This packet adds all 37 previously unnamed career text definitions, bringing game-side named labels to 270 rows (235 referenced labels plus the same 35 roots). The remainder is classified below; it is not silently treated as complete.

| residue | count | classification/evidence | handling |
|---|---:|---|---|
| animation pointer-block members | 714 | 72 setter groups already have named roots; members are consumed by `load_palette`, `scene_anim_show`, and image-buffer setup. The groups write 11 slots (59 groups), 13 (5), 7 (5), 18 (2), or 5 (1). | Hardware-facing resource wiring; list as module-level residue. Do not invent event names or force a uniform record type. |
| career text labels | 0 unresolved | 37 text definitions are now named in the symbol table by their exact consumer role; no game text is quoted. | Included in the symbol-only packet. |
| `D_80150188` | 1 | Fade quad parameter used by the album's hardware drawing path; no game-rule meaning is proved. | Hardware-layer internal; retain in the residue list. |

The 72 pointer-group roots, their five assignment-count patterns, and the two cross-group setters are described in §7.1 and §9. If a later hardware refactor groups them by layout, verify each structure and every consumer. The packet does not claim those 714 individual members are semantic game identifiers.

## 11. T-27065 reconciliation and essential-site inventory

The body-backed port rules above remain the source of meaning. The draft's 13 file moves require their own later file-only tables; the 114 constant candidates are not symbol rows. The essential game-rule literal sites are the functions and rules tabulated in §§3.3-3.4, 4-6 and 7.1-7.4; only values whose meaning changes game behavior belong in the later `rename.py const` batch. It must use exact selectors after the symbol/type batch establishes final names. Do not turn these sites into ordinary `func` or `data` rows. `config/renames/proposals/o-etc-essential.tsv` contains only exact, supported five-column `local` and `param` rows for essential locals and parameters.

Essential parameter behavior is covered at the owning functions: `exam_score_day`'s weekday and health inputs; `career_guidance_assess_params`' parameter values; `career_guidance_university_verdict`'s academic values; `career_guidance_job_verdict`'s trivia and looks; and `career_choice_exam_question`'s subject parameter, club experience and tier. `func_8013B1F0` receives category, scene, girl and season as `arg0` through `arg3`; the four parameter rows name these selectors for the album resource dispatch. In `func_80146D4C`, essential locals `main`, `bio`, `score`, `w` and `g` have exact five-column rows. `sub` is deliberately left unresolved because it first holds a secondary score then an event-log bonus factor; `ret` changes from raw score to grade; `zero` is an existing FAKE padding local and must retain its matching role. `func_80149F48` (`career_choice_exam_question`) has exact rows for `var_a1` (wrong-answer flag), `var_v0` (club bonus) and `var_v1` (adjusted subject score). The career-guidance girl-selection scan has three evidence-backed rows; two helper-array locals remain unresolved pending return-field meanings. The packet has 15 rows: 11 locals and four parameters; it is separate from the symbol-only packet. The inventory and this spec do not authorize source edits. Root owns application, proof and shared tracking.

The following numeric sites are game-rule literals (all other arithmetic constants are implementation scaling or hardware parameters). `rename.py const` accepts a variable, `Type.field` or a call-argument selector; entries whose literal is held only in a local are listed explicitly as requiring a typed site or a later scoped selector, rather than being disguised as ordinary symbol rows.

| proposed constant | value | exact owning C site | const-tool selector status |
|---|---:|---|---|
| `EXAM_GRADE_WEAK_FROM` | 0x28 (40) | `func_80146D4C`, comparison against the computed day score | exact selector `ret`; probe found 5 replacements, with 8 other literal hits unresolved |
| `EXAM_GRADE_FAIR_FROM` | 0x3C (60) | `func_80146D4C`, comparison against the computed day score | exact selector `ret`; probe found 5 replacements, with 8 other literal hits unresolved |
| `EXAM_GRADE_GOOD_FROM` | 0x50 (80) | `func_80146D4C`, comparison against the computed day score | exact selector `ret`; probe found 5 replacements, with 8 other literal hits unresolved |
| `EXAM_GRADE_PERFECT_FROM` | 0x5F (95) | `func_80146D4C`, comparison against the computed day score | exact selector `ret`; probe found 5 replacements, with 8 other literal hits unresolved |
| `EXAM_REMEDIAL_RED_MARKS` | 3 | exam results handler, red-mark count comparison | `D_801230F4`; confirm final data name after symbol batch |
| `EXAM_REMEDIAL_MARKER` | 0x17 | exam results handler, tomorrow's calendar marker write | `GameState.month_calendar` nested marker; needs exact nested type/field selector |
| `CAREER_PARAM_HIGH` | 101 | `career_guidance_assess_params`, seven parameter checks | parameter value reads; needs exact `Parameter.value` selector review |
| `CAREER_CLOSE_GIRL_MIN` | 60 | `career_guidance_closest_girl`, strict closeness threshold | local maximum; needs a typed closeness site |
| `CAREER_UNIVERSITY_PARAM_MIN` | 121 | `career_guidance_university_verdict`, three academic parameter checks | `Parameter.value`; needs exact selector review |
| `CAREER_UNIVERSITY_EXCESS_MIN` | 201 | `career_guidance_university_verdict`, sum of academic excess | local sum; needs a typed site |
| `CAREER_JOB_PARAM_MIN` | 61 | `career_guidance_job_verdict`, trivia and looks checks | `Parameter.value`; needs exact selector review |
| `CAREER_CHOICE_QUESTION_COUNT` | 10 | `func_80149F48` and its question-loop caller | `D_80150E94`; confirm final data name after symbol batch |
| `CAREER_CHOICE_CLUB_BONUS_SHIFT` | 4 | `func_80149F48`, practice-points right shift | `game_state.clubs[].practice_points`; call-site is local calculation, requires typed selector review |
| `CAREER_CHOICE_CLUB_BONUS_MAX` | 20 (0x14) | `func_80149F48`, cap after the `0x15` boundary check | local `var_v0`; needs a typed site |
| `CAREER_CHOICE_TIER_TWO_BONUS` | 0x26 | `func_80149F48`, first tier score offset | local `var_v1`; needs a typed site |
| `CAREER_CHOICE_TIER_THREE_BONUS` | 0x36 | `func_80149F48`, second tier score offset | local `var_v1`; needs a typed site |
| `CAREER_CHOICE_INTERVIEW_MIN` | 256 | `career_choice_interview_ok`, trivia-plus-looks check | local sum; needs a typed site |
| `CAREER_CLUB_DREAM_EXP` | 0x19B (411) | `func_8014AA74` (`career_choice_club_dream`), club experience gate | direct `ClubRecord.practice_points` member comparison; syntactically supported, but check member uniqueness and every same-field hit before using the selector |
| `ETC_CLEAR_COUNT_MAX` | 99 | clear-save writer, both per-girl counters | `GameState.girl_cleared` / system-data counter; exact array-field selectors require review |
| `ETC_STAFF_ROLL_MIN_FRAMES` | 0x259 | staff-roll exit predicate at `staff_roll_and_clear_save.c:272` | reads `game_state.mode_step_timer.w`; `Type.w` is ambiguous across visible types and `*.w` is too broad, so a safe selector remains unresolved |
| `ETC_STAFF_ROLL_SEGMENT_FRAMES` | 701 | staff-roll girl/model segment selection | local frame quotient; needs a typed site |
| `ALBUM_RECORD_CAPACITY` | 256 | album log packer and viewer index | album log count; exact array/count type selector requires review |

Exact `rename.py const --site` selectors probed read-only in Docker against current source:

| selector | values/site meaning | probe result / limit |
|---|---|---|
| `ret` | `128`, `40`, `60`, `80`, `95` in `func_80146D4C`: score scaling and grade thresholds | 5 replacements in one C file; 8 additional literal hits had no value mapping. `ret` occurs only in this function, but changes from raw score to grade. |
| `sub` | `0x27`, `0x26`, `0x1A`, `0x19` in `func_80146D4C`: two cap tests and their cap values | 6 replacements in one C file; one hit unresolved. `sub` is reused for the event-log bonus after the cap calculations. |
| `var_v0`, `var_v1` | `0x15` and `0x14` in `func_80149F48`: practice-point cap test/value | 6 replacements in one C file. After the local batch, use their new names as the selectors. |
| `D_80150E98` | values 0, 1, 2 in `func_80149F48`: career exam tier | 12 replacements in one C file; use the post-batch tier data name as the selector. |
| `D_801500D8` | values 0, 1, 2, 3 in album secret-mode logic | 22 replacements in two C files; two literal hits unresolved. Use the post-batch mode data name as the selector. |
| `D_80150040`, `D_80150044` | direct album category and scene selectors | Variable sites are supported; extract the switch values per owning handler into separate enum value files. |
| ETC dispatcher call arguments | scene/category values passed directly to `func_8013B1F0` and the album animation dispatchers | Use `ETC:<current-callee>(argN)` only for direct literal arguments; inspect each caller and keep different domains separate. |

The `const` selector probe confirms these are the exact supported selector shapes. The following rule literals cannot be safely fed to the current tool as scoped const sites: the career tier `+= 0x26/0x36` bonuses; arithmetic scaling, biorhythm coefficients and random bounds; career result bytes stored through `player_flags_upper.b[1]`; the `girl_cleared[index] < 99` array cap; the player-flag array mask; `trivia + looks > 256`; return literals in verdict helpers; and per-scene `mode_stage`/`mode_step` values (one member selector would conflate unrelated state machines). These require a future scoped-selector/tool change or must remain explicitly unresolved; do not use an unscoped variable guess. This body-site/selector inventory supersedes any broad prose-only interpretation of the old 114 constant rows.

The ETC proposal is not ready to apply until its new symbol-only table passes `rename.py plan` in Docker and an independent Standards/Spec review. In particular, the proposal rows still need exact applied-ledger lookup at each address immediately before application; a later applied ledger always wins over a draft alias. The unit's code, header and overlay YAML must remain untouched in this lane.
