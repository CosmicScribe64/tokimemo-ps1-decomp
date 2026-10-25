---
type: spec
updated: 2026-10-24
ticket: T-27032
status: analysis
---

# GEKO encounter rules

This page describes the code paths needed to port GEKO's rules. Proposed identifiers are in
`config/renames/proposals/o-geko.tsv`; the reconciled function/data-only application batch is
`config/renames/proposals/o-geko-apply.tsv` (T-27051). Neither table has been applied. Original identifiers below are evidence keys.
The code supports five entry points: walking-home encounters, the player's birthday, a girl's birthday,
queued girl scenes and invitations that create appointments. The overall `girl_encounters` folder and
`encounter_` prefix remain **HYPOTHESIS**: ADR-0001 explicitly requires emulator confirmation for GEKO,
and this analysis performed no emulator run. Technical callback numbers do not establish a scene's story.

Read with [[phase2]], [[families/geko-family-results]], [[game-state]], `GLOSSARY.md`,
`docs/adr/0001-phase-2-naming.md` and `CODING_STANDARDS.md` sections 4, 5, 8 and 10.
The shared core names in this page follow the current canonical topic owners in `config/header_owners.json`.
The `include/main_api.h` and `include/game.h` paths in earlier migration notes are historical; both umbrellas
have been retired.

## Entry and ownership contract

`src/main/800789E0.c:418-431` selects these entry addresses. The scheduler supplies the queued girl;
for special scenes it also stores the girl selector in `scene_work_selector` (address `D_80122CD4`) and the scene in O.BIN `talk_work`
(`src/main/8006CB30.c:1880-1918`). Those shared cells remain main-owned.

| Applied game mode | Entry | Proposed function | State owner |
|---|---|---|---|
| `GAME_MODE_GIRL_ENCOUNTER` (0x59; mode name itself is a shared-core hypothesis) | `func_80132244` | `encounter_walk_home_mode_handler` | stage 0 approach, stage 1 follow-up |
| `GAME_MODE_PLAYER_BIRTHDAY` (0x56) | `func_80134040` | `encounter_player_birthday_mode_handler` | stage 0, 29 steps |
| `GAME_MODE_GIRL_BIRTHDAY` (0x57) | `func_80134C90` | `encounter_girl_birthday_mode_handler` | stage 0, 73 steps |
| `GAME_MODE_SPECIAL_EVENT` (0x5D) | `func_8013882C` | `encounter_scene_mode_handler` | queued girl, `talk_work` scene, stage 0, per-scene steps |
| `GAME_MODE_GIRL_INVITATION` (0x5A) | `func_801426D0` | `encounter_invite_mode_handler` | stage 0, 32 steps |

Unexpected stages or scene selectors generally call `game_mode_reset_to_title`; callback indexing itself
has no bounds check. A port should model legal states explicitly. `mode_step_next` and `mode_stage_next`
are shared transition services; maintain their timer/substep resets when adapting callbacks to a new engine.
The standard completion operation is `day_event_next(1)`, after restoring HUD/audio state. Audio polling
uses a 0x400-tick retry: reset CD and bank state and retry the load/restore phase. Exact comparison and
postincrement behavior remains in each definition; this page does not equate every retry with a wall-clock time.

All five entries run shared text scrolling/typewriter and HUD/message/date rendering. Scene branches may
switch between decoded-background/parameter-HUD display and the scene frame using `D_80122D38`.
These presentation services can be replaced in Godot; the girl selection, score mutations, queue completion,
script cursor transitions and appointment creation must stay in the rules layer.

## Independent script state

Each mode has three script resource pointers (text, command and voice) and a group/line cursor pair.
The talk helpers install them into the shared runner and call `talk_run(choice, 1, 0)`; a choice prompt uses
0xFF instead. Special scenes use main-owned `talk_secondary_group` / `talk_secondary_line`
(`D_800CA150` / `D_800CA154`); phone presentation
uses runner mode 2. A resource pointer is not a line number, and a callback step is not a dialogue group.

| Proposed prefix | Text / command / voice cells | Group / line cells | Other local state |
|---|---|---|---|
| `encounter_walk_home_` | `D_80144C30/34/38` | `D_80144C3C/40` | topic `D_80144C50`, follow-up flag `D_80144C54`, high-syoushin snapshot `D_80144C58` |
| `encounter_player_birthday_` | `D_80144E80/84/88` | `D_80144E8C/90` | shared birthday done bits and preserved talk byte |
| `encounter_girl_birthday_` | `D_80145048/4C/50` | `D_80145060/64` | saved triple/group, school-year index, context/phone/choice flags |
| `encounter_scene_` | `D_80146274/78/7C` | main-owned `talk_secondary_group` / `talk_secondary_line` | saved queued girl and scene selector |
| `encounter_invite_` | `D_801476CC/D0/D4` | `D_801476D8/DC` | selected invitation topic `D_801476E0` |

Storage views matter to a matching application: for example `D_80144C40` is declared as a byte but is
initialized and read as a halfword in several functions. Preserve those views until the naming-tool proof
and clean build establish any declaration change. The Godot model can give each cursor a clear integer type.

The binders initialize parallel script-section rows: 12 entries per column for walking home, 13 each for
birthday, birthday-visit and invitation packs, and 79 for special scenes. Proposal names use column and
section index where narrative meaning is unproved. No script bytes, dialogue or translated game text are
included here. Existing O.BIN names are retained; the TSV has retention comments for the 52 names GEKO uses.

## Walking-home state machine and rules

`src/ovl/GEKO/801323E0.c`, `801338F0.c`; tables `D_80144C5C` (33 callbacks) and
`D_80144D40` (19 callbacks). The approach table initializes the queued girl, loads audio/background/scripts,
chooses the girl's script, adjusts the initial group by girl state, offers a choice, applies its consequences,
and checks whether to enter follow-up stage 1. Both tables finish through `func_801337EC`.

`func_801325C0` chooses the queued girl, clears local cursors, selects a future-calendar topic and records
whether her syoushin is at least 76. If no topic was chosen, Mira receives special topic 100 on a
`rand() & 3 == 0` result. This is a random outcome, not a guaranteed scene.

`func_80133024` considers offsets 1 through 19 days ahead, using this month's or next month's calendar
row. Year 0x60 patches February to 29. The marker helper `func_80132D04` offers topics in scan order:

| Calendar marker | Offered topic | Additional predicate |
|---|---:|---|
| 0xC | 2 | Shiori requires player club or her club below 6; other girls unconditional |
| 4 | 6 | none |
| 0xB | 7 or 8 | 7 when state <2 or Yumi; otherwise 8 |
| 6 / 0xE / 0x14 | 11 / 12 / 13 | none |
| 2 | 14 / 15 | year 0x60 / 0x61 respectively |
| 0xF | 9 | none |
| 5 | 1 | clears prior topic first, unless topic is already 16 |
| 0x15 | 16 | clears prior topic first |

The general offers are suppressed while topic is 1 or 16. Yumi also offers topic 8 before
14 September of year 0x61. Marker meanings beyond these numeric rules need calendar-owner evidence.
The replacement helper takes an empty topic immediately; replacing birthday topic 10 requires
`(frame_count & 7) >= threshold[current_girl]`, while replacing another nonzero topic requires odd frame parity.
This makes scan order and frame count part of selection.

After the scan, `func_80132B24` can select birthday topic 10 only when girl flag b6 is set. Its known-birthday interpretation is **HYPOTHESIS**. It considers the
birthday in the current or following month and a per-girl threshold. Preserve the source's unsigned
subtractions: the same-month condition is nonzero birthday-day minus today below 21; the following-month
expression uses `days[(month + 10) % 12] + birthday_day - today < 20`. That index is the source's expression,
although it resembles the previous month's length; do not repair it during a fidelity port.

Initial script groups add 6 at girl state 2, 12 at state 3, or 18 at state 4/high-syoushin. The last case
sets portrait 4 and disables follow-up. The choice prompt is skipped by five steps for state 4/high-syoushin
or Nozomi state below 2 (`func_80133250`).

`func_80133308` interprets choice zero as the follow-up-eligible arm. Nonzero choice raises the selected
girl's syoushin by `frame_count % (tokimeki / 20 + 1)` plus 10 for state below 2, otherwise 6, and adds 2 to
the talk group. Zero choice raises the other met girls' syoushin by a random tokimeki-band term plus 4. For Shiori,
Mira, Mio, Yuina, Yuko or Megumi at state at least 3, that base becomes 0 and follow-up is disabled.
Those same girls below state 3 keep base 4. `check_para_limit` clamps all results.

`func_80133598` enters follow-up only when the local flag is set, a topic is available and the girl's
flag b0 does not already suppress that branch. Nozomi state below 2 has an additional skip. Otherwise a
low-state refusal script uses `min(syoushin / 20 + 1, 5)`. Follow-up marks b0, chooses a club-related topic
variant, and selects group `base + topic - 1`; topic 100 forces group 0x28. Saki has specific club-matching
group changes. `func_801337EC` gives affection +2 before queue completion, regardless of which table reaches it.

## Player birthday

`src/ovl/GEKO/80134040.c`; `D_80144E94` is the 29-entry step table. Initialization ORs done mask 0x04 (bit 2) and
skips the birthday flow if that mask was already set. The default visitor script is separate from the
selected-girl script. A calendar marker 5 can insert a 128-tick audio delay. Door-chime presentation is
suppressed by done mask 0x08 (bit 3); its exact narrative context remains unconfirmed.

`func_80134808` scans met girls 0..10. Lower `get_g_zyotai_s(id) & 0x7F` wins; ties use the greater sum
of `get_h_yuukou(id)` and `get_h_tokimeki(id)`, not the raw stored halfwords. The incumbent survives an exact sum tie. Done mask 0x80 excludes a candidate when
`id + 1` equals the preserved shared talk byte. A missing candidate, state at least 2, or done mask 0x08 jumps
to step 23; otherwise it installs the selected girl's script. The source calls the state helper even when
best remains 0xFF, before the later sentinel test; the continuation below proves the shared helpers'
modulo-13 handling of that positive sentinel.

Completion redirects to `GAME_MODE_NEW_YEAR_A` when done mask 0x40 is set; otherwise it restores HUD/audio and
advances the day event. These done bits are shared flow-control state, not independent permanent progression flags.

## Girl birthday and player-birthday interruption

`src/ovl/GEKO/80134D10.c:107-127` defines the 73 callback entries directly in C.
Important step boundaries are:

| Steps | Role |
|---|---|
| 0..5 | initialize queued girl, bank request/wait, script load, I/O wait, skip six entries |
| 11..21 | context branch, birthday choice, saved reply script, three-answer selection and restore |
| 22..28 | appointment check, known-phone gate, phone presentation and scene-display reset |
| 29..46 | choose location, load portrait/background, girl entry, talk, choice-score update |
| 47..51 | test player birthday, girl exit/talk, conditional background exit, finish |
| 52..60 | embedded player-birthday interruption using visit scripts, then finish |
| 61..66 | same-day-appointment phone branch, then finish |
| 67..71 | seasonal background reuse/decode/display, then return to step 1 |
| 72 | invalid-state/title callback |

Initialization uses the queued girl. Shiori's already-run guard is done mask 0x01 (bit 0), other girls' is mask 0x02 (bit 1);
others also require flag b6. Initialization sets the respective guard. Local context starts as holiday-like;
ordinary non-Sunday school-calendar days select school context, unless explicitly forced by done mask 0x10.
Here ordinary means marker is neither 0xA nor 0x10 and day-kind is none of 1, 2 or 3
(`src/ovl/GEKO/80134D10.c:792-798`). The mask-0x10 snapshot is named
`encounter_girl_birthday_background_variant` in the application batch; its night interpretation remains unproved.
December 24 bypasses those school-context tests. A same-club last-command-4 path can select club context
when no same-girl appointment exists today. Context labels are **HYPOTHESIS**; the predicates are source facts.
The source initializes phone ring count to `frame_count % 3 + 2` and school year to `year-0x5F` from April,
otherwise `year-0x60`.

`func_80135894` (`src/ovl/GEKO/80134D10.c:612-615`) reads the three-sector visit pack and
calls `func_80134F84`; it does not assign the script group or line. Its application name is
`encounter_girl_birthday_visit_scripts_load`, correcting the original proposal's group-reset claim.

The birthday menu first penalizes a nonzero choice by affection -1 and syoushin +5 and jumps to step 49.
Choice zero saves the current script triple/group, switches to a common reply script at
`girl * 3 + school_year`, and later restores it. Reply-name selection uses
`girl * 297 + school_year * 99 + choice * 33`.

| Reply choice | Tokimeki | Affection | Syoushin |
|---|---:|---:|---:|
| 0 | +3 | +2 | -15 |
| 1 | +2 | +1 | -10 |
| other | unchanged | +1 | +10 |

`func_80137694` applies the table and clamps. Girl states 2, 3 and 4 add group offsets 2, 4 and 6.
Girl flag b3 permits the phone branch; otherwise it closes at step 49. The known-phone interpretation of b3 is **HYPOTHESIS**. Phone presentation has a
90-tick prelude, eight number sounds spaced 8 ticks apart, and ring sounds every 64 ticks. Cross can skip
after tick 64. One branch stops by tick 320; the other uses the initialized ring limit times 64.
`D_80145078` is a byte counter incremented on each call to the talk helper, rather than a Boolean;
zero enables the prelude (`src/ovl/GEKO/80134D10.c:1230-1260`). Preserve that width and increment.

The location selector records log entries for corridor, club room, home front or park variants. Portraits
can be uniform, club-specific or casual; seasonal substitutions occur outside June through September.
A same-girl appointment today can jump to step 61. The explicit player-birthday test at step 47 uses the
same best-met-girl rule as visitor selection and requires the current girl to be that winner with state below 2;
it ORs done mask 0x44 and jumps to step 52. Birthday completion redirects to `GAME_MODE_NEW_YEAR_A` on mask 0x20.

## Invitation and appointment rules

`src/ovl/GEKO/80142760.c`; `D_801476E4` has 32 callbacks. The main scheduler skips this mode when the
shared appointment is already active. Steps 0..15 initialize/select a destination, load audio/scripts,
background and portrait; 16..19 present/apply the first choice; 20..25 present/apply the second; 26..31
exit, store the accepted appointment or refusal script, restore display/audio, and finish.

`func_80144198(selector)` computes the current week's Sunday: `day - weekday + 6`, carries past the
current month length and from December to the next year. Selector 0 returns year, 1 month, anything else day.
Sunday input can return today. Year 0x60 has February 29; no general leap-year algorithm is present.

The destination chooser uses 21 place ids, each with an eligibility flag. It first computes the current
week's Sunday and uses that **target year/month/day**, not today's date, for eligibility and weights.
The calendar day-kind uses row 1 when Sunday spills into the next month, otherwise row 0
(`src/ovl/GEKO/80142760.c:155-164`). It computes one of 12 season slots from that target date:
year 0x5F gives 0 for April/May, 1 for June..August, 2 for September..November, otherwise 3;
year 0x60 gives 3 for January/February, 4 for March..May, 5 for June..August, 6 for September..November,
otherwise 7; year 0x61 gives 7, 8, 9, 10, 11 for the corresponding five windows. Other years use 11
(`src/ovl/GEKO/80142760.c:175-211`). A place requires
`progress_state[place] == 1` and its minimum year/month. Further source gates are:

- Places 0, 1 and 18 are always excluded. Place 13 excludes February/December; place 14 excludes all months before June of year 0x5F, including January..March if such an input is supplied.
- Places 14, 15 and 7 respectively reject girl-scene masks 0x01, 0x02 and 0x04. Movie theater and concert hall also require a nonzero girl/season availability entry.
- Place 7 requires scene counter 12 ==0 for September..November 0x61, counter 13 ==0 for March..May 0x61, and counter 14 ==0 for September..November 0x5F. These are the selected girl's counters.
- Places 16/17 need calendar day-kind 2 or July; 19/20 need day-kind 2 or December.
- The previous-place exclusion table rejects its cell indexed by `girl_last_date_spot[current_girl]`. If shared `D_800E71DF` equals the appointment word's girl bits 25..28, the word's place bits 0..4 are also excluded; the predicate does not test the active bit here.

The final girl-specific exclusion clauses are exact below; they combine with all preceding gates
(`src/ovl/GEKO/80142760.c:235-241`). The stadium subset means target month 1, 3, 6 or 11.

| Girl | Additional excluded places |
|---|---|
| Mio | stadium in the stadium subset; pool, sea, ski resort always |
| Yuina | ski resort |
| Ayako | pool, sea |
| Saki | art museum |
| Yukari | stadium in the stadium subset |
| Megumi | stadium in the stadium subset; ski resort always |
| Yumi | art museum, karaoke |

`place_to_topic[place]` maps eligibility to 19 script/weight topics. For place 13 in January, March,
June or November the topic increments by one. Weights are 11 girls by 19 topics. Both the total and
cumulative sums are bytes. The code seeds `rand` from `frame_count`, draws `rand() % total`, then picks
the first eligible place with cumulative weight **greater than or equal to** the draw. It initializes the
fallback place to 2. Preserve this boundary: using the usual strictly-greater test would change outcomes.
No zero-total guard exists in the source; reachable table invariants have not been proved in this pass.

After random selection, `func_80144270` can override the topic and mark a place available for these source windows:

| Girl | Place | Window / additional gate |
|---|---:|---|
| Mio | 14 | year 0x60; December or January, or February with day <21; scene flag 12 ==0 |
| Ayako | 7 | year 0x61; September or October, or November with day <21; flag 24 ==0 |
| Saki | 13 | year >=0x60; September, or October with day <21; flag 30 ==0 |
| Mira | 2 | March or April, or May with day <21; flag 47 ==0 |
| Yumi | 13 | year >=0x60, October before day 21; flag 71 ==0 |
| Yuko | several | sequential checks over scene counters, scene flag 57 and computed Sunday; see definition |

Unlike the eligibility predicate, the overrides use the **stored current date**, including Mio's unusual
January/December same-year condition (`src/ovl/GEKO/80142760.c:538-559`). The `<21` cutoff applies only to the
last month in each listed window. Every matching branch also writes `progress_state[place] = 1`.

Yuko's checks execute in this order (`src/ovl/GEKO/80142760.c:562-591`):

| Order | Target place | Predicate |
|---|---:|---|
| 1 | 3 | scene counter 6 ==0; December 0x5F, January 0x60, or February 0x60 with day <21 |
| 2 | 5 | year 0x60, June or July; counter 9 ==0 |
| 3 | 10 | year 0x60; September or October, or November with day <21; counter 18 ==0 |
| 4 | 11 | year 0x61; March or April, or May with day <21; counter 19 ==0 |
| 5 | 15 | year 0x61; September or October, or November with day <21; scene flag 57 ==0 |
| 6 | 12 | year 0x61; December or January, or February with day <11 |
| 7 | 8 | computed Sunday equals December 10 0x5F, December 8 0x60, or December 7 0x61 |

Counters in this table belong to Yuko. Orders 1..6 use today's stored date; order 7 uses computed Sunday.
The last applicable topic wins, while **all** matching branches keep their availability writes. Preserve
sequential assignment even if replacing this code with a rules table.

First-choice nonzero: syoushin +15, group 23, step 24. Second-choice nonzero: syoushin +20, group 24 for
Yuina or 23 for the other girls, step 24. Zero on the second choice gives the other met girls a syoushin
increase of `rand() % (tokimeki/20 + 1)` plus 6 for girl state below 2, otherwise 4. The exit gives the
selected girl affection +1 for zero choice. Nonzero exit chooses one of two shared talk-exit helpers by girl.
Clamping occurs after the choice handlers; the final affection +1 helper itself does not call the clamp.

`func_80143F48` writes an appointment only when choice is zero. The word carries mapped place, computed
Sunday year/month/day, girl, active=1, bits 21..23=6, bit24=1 and bits30..31=0. The shared insertion helper
is `func_8006B5BC`; no main rename is proposed here. Places 14/15/7 OR girl-scene masks 0x01/0x02/0x04. Nonzero
choice switches to the refusal script, group `min(syoushin/20,4)`. The unknown packed-bit names remain
unknown because fixed stores do not establish their game meaning.

## Girl-specific queued scenes

The top-level saved-girl switch establishes identity. The scene selector is the queue record's numeric
scene, not a name derived from dialogue. All listed scene variants require mode_stage 0.

| Girl | Accepted `talk_work` | Callback counts |
|---|---|---|
| Yuko | 1 | 53 |
| Shiori | 1, 2, 8 | 39, 23, 55 |
| Yuina | 1, 7 | 42, 41 |
| Rei | 1, 2, 3 | 58, 39, 50 |
| Ayako | 1 | 32 |
| Mira | 1, 8 | 38, 22 |
| Mio | 1 | 39 |
| Yukari | 1, 2 | 32, 23 |
| Nozomi | 1 | 33 |
| Megumi | 1, 2 | 46, 22 |
| Saki | 1 | 35 |
| Yumi | 1, 2, 7 | 49, 23, 61 |
| Miharu | 1, 2, 4 | 27, 28, 64 |
| Yoshio | all selectors delegate to one flow | 43 |

Most local wrappers bind a still pack, load a background/palette, install a script triple or wait for shared
portrait/text transitions. The TSV gives those wrappers technical role names with ordinal suffixes when
the same file has multiple definitions. Those suffixes identify definitions; they do not claim narrative
scene numbers. Step dispatchers instead use the actual `talk_work` selector in their proposed names.

Port-visible special cases include Yuina's RPG transition (`func_8013B350` saves stage and next step before
`GAME_MODE_RPG_BATTLE`), Rei's choice outcome, Megumi's affection threshold, Saki's counter branches, Yumi's repeat
branches, Miharu's story counter and Yoshio's sixth-ranked girl selection. The following evidence table
records score and progress mutations without interpreting the scene's dialogue.

| Evidence definition | Rule effect |
|---|---|
| `801393C0.c:func_80139640` | Yuko affection +1, then clamp |
| `80139910.c:func_8013A6D0` | For scene 1, Shiori tokimeki +1 and affection +2, then clamp |
| `80139910.c:func_8013AA1C` | Stress -1, then clamp |
| `80139910.c:func_8013AB4C` | Shiori tokimeki +2, affection +2; birthday done mask OR 0x05 |
| `8013AF80.c:func_8013B510` | Yuina tokimeki +1, affection +2, then clamp |
| `8013AF80.c:func_8013B7D0` | Forces calendar to 1998-02-28, Saturday; sets scene log id 0x14 |
| `8013B930.c:func_8013C090` | For scene 1, stress +5; current/date girl both Rei |
| `8013B930.c:func_8013C014` | Nonzero choice sets scene flag 79 to 1 and skips 20 extra steps; zero choice sets it to 2 |
| `8013C4C0.c:func_8013C73C` | Ayako affection +2, then clamp |
| `8013C980.c:func_8013CD18` | Mira affection +1, then clamp |
| `8013D0F0.c:func_8013D2C8` | Mio tokimeki +1, affection +2, then clamp |
| `8013D4B0.c:func_8013D7D4` | Scene 1 gives Yukari affection +1 and syoushin -1, then clamp |
| `8013D4B0.c:func_8013D9C4` | Yukari tokimeki +1, then clamp |
| `8013DB80.c:func_8013DD58` | Nozomi tokimeki +1, affection +2, then clamp |
| `8013DF70.c:func_8013E1A0` | Megumi affection >=80: tokimeki +1, affection +2; otherwise flag b14 set and affection +1; clamp |
| `8013DF70.c:func_8013E5C8/8013E60C/8013E650` | Affection below 80 adds 2 talk groups or skips 14 extra steps; at least 80 skips 3 extra steps in the third helper |
| `8013EE40.c:func_8013F0BC` | Clamp scene counter 27 from >=3 to 2; if counter 28 is 0 and counter 27 is 1, Saki tokimeki +1; affection +1 and condition +10 always; then clamp |
| `8013F360.c:func_8014016C` | Scene 1 gives Yumi tokimeki +1, affection +2, condition -5 and guts +5; then clamp |
| `8013F360.c:func_801403A0` | Yumi state <2 gives tokimeki +1; affection +2 always; then clamp |
| `8013F360.c:func_801404FC` | Yumi affection +2, then clamp |
| `80140A00.c:func_80140FD0` | Miharu story_stage >=5 becomes 4; talk group adds twice the stage, then stage increments |
| `80140A00.c:func_801411DC` | Scene 1 reduces stress by 5 and all met girls 0..10 tokimeki by 1; then clamp |
| `80141AF0.c:func_80141D74` | Saves selected date girl in scene flag 82 and current girl, clears her met mask 0x02 (bit 1) |

Yoshio's ranking initializer (`80141AF0.c:func_801420E4`) scans met girls 9 down through 1, excluding
Shiori and girls 10 onward. It keeps six descending scores of `tokimeki + affection` and selects the sixth
slot as `date_girl`. Comparisons use >=, so a lower girl id inserted later wins a tie. It increments scene
flag 82 during initialization, then another callback can overwrite that byte with the chosen girl id.
It is therefore incorrect to treat flag 82 as a Boolean completion flag. Behavior with fewer than six met
girls follows the initial six-slot arrays; the scheduler's entry conditions need to remain part of the port contract.

`encounter_scene_finish` restores the saved queued girl before logging. It omits the scene log when id is
0xFF. Id 0x14 forces rank 4 on the relevant event-log record, then clears a shared scene flag and advances
the queue. This special case accompanies Yuina's forced final calendar date above.

## Tables, files and deferred work

The proposal includes role names for the important lookup data: month lengths, girl thresholds,
club-name/portrait tables, birthday replies, invitation eligibility/weights/maps, ranking seeds, callback
arrays and parallel script sections. Copied type names describe their role, not their byte count.
`GekoDateEntry` becomes the proposed `EncounterAppointmentEntry`; its unknown bits retain their names.
The current shared `AppointmentDate`/`AppointmentBits` definitions are the reference for a later type merge,
which requires a byte-match proof.

Twenty-eight local callback typedef declarations reuse names such as `FnTbl32` in different source
objects. Their file-specific proposed role names are comment rows, because a unit-wide type rename cannot
safely disambiguate those declarations. The shared header-owned callback types have active proposals.
The overlay's 391 function definitions all have proposed technical role names. This is an analysis
coverage count, not proof that every narrative rule or presentation effect is understood.

The file proposals keep all 22 object boundaries: walking-home entry/logic/follow-up; player birthday;
girl-birthday entry and the shared birthday/scene-script object; the 14 girl scene objects; invitation
entry/logic. Header proposal: `include/ovl/girl_encounters.h`, preserving the current main override views.
Do not split `80134D10.c` as part of a rename: it owns shared scene-script bindings as well as birthday code.
Disc filenames, asm roots, original assets and build outputs keep GEKO's original disc name.

Before applying names, separate identifier batches from file/dir moves, resolve the deferred local callback
types, perform the required independent standards/spec review and `rename.py plan`, then apply only through
`tools/rename.py`. A clean 27/27 SHA-1 build and prototype-view proof are application gates; none was claimed
or run for this documentation-only pass.

Outstanding evidence needed for a Godot implementation:

- Run GEKO through `tools/emu.py` using the documented [[emulator]] workflow and record the exact command/run to satisfy ADR confirmation. No screenshots or save states belong in the repository.
- Observe one example of each of the five entry modes and representative girl/choice branches; check that the provisional context labels match their visuals and behavior.
- Prove the invitation weight total is nonzero for reachable saves; preserve byte accumulation and the inclusive cumulative boundary until then.
- The birthday sentinel's source behavior is resolved by the continuation trace below; preserve its call order. The unusual next-month birthday index and sequential Yuko override priority still need representative runtime traces.
- Recover the meaning of fixed appointment bits and unproved girl flags from their main/TEL owners, rather than assigning a narrative label from one write.
- Verify the explicit invitation eligibility and sequential-override tables above against runtime traces; retain the source's target-Sunday/current-date distinction and persistent writes when porting them.
- Detailed animation internals, image-pointer globals and unnamed shared-main dependencies remain presentation work. Scene script group transitions remain encoded in the original script resources; a clean-room Godot script layer needs a separate content plan.

Original proposal validation on 2026-10-24: Docker using `tokimemo-decomp-arm64` checked that every active row has five tab
columns, all function/data/type targets exist in this overlay's source/header, and old/new keys are unique.
No source, header, symbol map, raw game data or build configuration was changed. The table is a reviewable
proposal, not an application ledger.

## T-27051 symbol-batch reconciliation and explicit deferrals

`config/renames/proposals/o-geko-apply.tsv` contains **1,061 active rows: 391 functions and 670 data
symbols**, each scoped `GEKO`, with exactly five TSV columns and a source-file/line citation. Every note
starts with `HYPOTHESIS:` so `rename.py` will retain the ADR-required provisional `encounter_` grouping.
No GEKO function or data rename was found in the applied ledgers at preparation time; main/shared
symbols and their applied spellings were retained. The table contains no jump tables, comments, constants,
locals, parameters, types, fields, files, directories or deferred rows.

The reconciliation checked all function-role claims against bodies/callers, every existing data row
against a body use, all girl-selector dispatch chains, and broad presentation wrappers across the 22
objects. Added resource names describe the actual palette, still-image or animation-input argument and
its source binder ordinal; neither ordinal nor technical argument number claims a story scene or resource
format. `minimum`, `movie_theater` and `concert_hall` replace unlisted or ambiguous target stems. The five
game-mode entry points end in `_mode_handler`. Syoushin remains an unresolved field meaning: its proposed
snapshot/helper names say `high_girl_counter`, and notes cite the existing `syoushin` field and threshold.

The source/header census has 391 function definitions and 676 distinct owned `D_` identifiers after
excluding comments. Of those data identifiers, 670 are proposed and the six below are explicitly deferred.
Generated assembly has 756 unique `dlabel D_` symbols: those same 676 plus 80 assembly-only labels.
The table below accounts for every remaining label, including the historical array-base alias; jump
tables remain outside this census. No unexplained game-logic function placeholder remains in this batch.

| Deferred source identifier | Evidence and remaining work |
|---|---|
| `D_801463B0` | `src/ovl/GEKO/80139910.c:90`, `src/ovl/GEKO/8013DF70.c:30`: extra image loaded by `LoadSquare`, rebound by both Shiori and Megumi packs; defer shared image/quad resource ownership and format to the presentation pass. |
| `D_80146600`, `D_80146604`, `D_80146608`, `D_8014660C` | `src/ovl/GEKO/8013AF80.c:13-16`, `:204-207`: four fixed pack values copied directly to shared actor cells; defer their animation-resource formats and field meanings to the actor/header pass. |
| `D_8014755C` | `src/ovl/GEKO/80141AF0.c:42-44`, `:241`: zeroed by init; nonzero condition calls `func_80063668`, but GEKO never sets it nonzero. Its intended producer and role remain unknown. |

The following 79 rodata labels are compiler-emitted literal/zero storage with no named C identifier.
They are explicitly deferred to the source-literal/presentation pass; the application table does not
invent a game meaning for empty or repeated labels. The final row is the assembly-only halfword-array
base alias, deferred pending aggregate/relocation review. Evidence names labels only, without game text.

| Generated evidence file | Explicitly deferred labels |
|---|---|
| `asm/ovl/GEKO/data/GEKO/801323E0.rodata.s:10` | `D_80144710`, `D_80144718` |
| `asm/ovl/GEKO/data/GEKO/80134040.rodata.s:10` | `D_80144750`, `D_8014475C`, `D_80144764` |
| `asm/ovl/GEKO/data/GEKO/80134D10.rodata.s:10` | `D_801447A0`, `D_801447A8`, `D_801447B4`, `D_801447BC`, `D_801447C4`, `D_801447CC`, `D_801447D4`, `D_801447DC`, `D_801447E8`, `D_801447F0`, `D_801447F8`, `D_80144804`, `D_80144810`, `D_8014481C`, `D_80144824`, `D_8014482C`, `D_80144834`, `D_80144840`, `D_80144848`, `D_80144854`, `D_80144864` |
| `asm/ovl/GEKO/data/GEKO/801393C0.rodata.s:9` | `D_80144980`, `D_80144984`, `D_8014498C`, `D_80144994` |
| `asm/ovl/GEKO/data/GEKO/80139910.rodata.s:9` | `D_801449A0`, `D_801449A4`, `D_801449AC`, `D_801449B0`, `D_801449B8`, `D_801449C4` |
| `asm/ovl/GEKO/data/GEKO/8013AF80.rodata.s:10` | `D_801449D0`, `D_801449D8` |
| `asm/ovl/GEKO/data/GEKO/8013B930.rodata.s:9` | `D_801449F0`, `D_801449F4`, `D_801449FC`, `D_80144A08`, `D_80144A0C` |
| `asm/ovl/GEKO/data/GEKO/8013C4C0.rodata.s:9` | `D_80144A20`, `D_80144A24` |
| `asm/ovl/GEKO/data/GEKO/8013C980.rodata.s:10` | `D_80144A30`, `D_80144A38`, `D_80144A40`, `D_80144A44`, `D_80144A4C`, `D_80144A50` |
| `asm/ovl/GEKO/data/GEKO/8013D0F0.rodata.s:9` | `D_80144A60`, `D_80144A64` |
| `asm/ovl/GEKO/data/GEKO/8013D4B0.rodata.s:10` | `D_80144A70`, `D_80144A78`, `D_80144A7C` |
| `asm/ovl/GEKO/data/GEKO/8013DB80.rodata.s:10` | `D_80144A90` |
| `asm/ovl/GEKO/data/GEKO/8013DF70.rodata.s:9` | `D_80144AA0`, `D_80144AA4`, `D_80144AAC` |
| `asm/ovl/GEKO/data/GEKO/8013EE40.rodata.s:9` | `D_80144AC0`, `D_80144AC4` |
| `asm/ovl/GEKO/data/GEKO/8013F360.rodata.s:9` | `D_80144AD0`, `D_80144AD4`, `D_80144ADC`, `D_80144AE4`, `D_80144AE8`, `D_80144AF0` |
| `asm/ovl/GEKO/data/GEKO/80140A00.rodata.s:10` | `D_80144B00`, `D_80144B08`, `D_80144B10`, `D_80144B18`, `D_80144B1C`, `D_80144B24`, `D_80144B30`, `D_80144B44` |
| `asm/ovl/GEKO/data/GEKO/80141AF0.rodata.s:9` | `D_80144B50`, `D_80144B54` |
| `asm/ovl/GEKO/data/GEKO/80142760.rodata.s:10` | `D_80144B60` |
| `asm/ovl/GEKO/data/GEKO_data_80146270.data.s:321` | `D_801463F8` |

All type/field/local/parameter/constant/comment work remains in the original proposal or a future
reviewed batch. In particular, duplicated file-local callback typedefs need file-scoped resolution;
appointment bits 21..24/30..31 retain unknown meanings; overlay/main override views remain intact.
All 22 C file moves, the directory/header rename and topic header split remain separate work, with the
prototype-view and clean 27/27 build gates. No source application, emulator run or build is claimed here.

The initial live 1,061-row plan completed with zero problems and 26 files to change. A later run crossed
the concurrent shared `MODE_*` to `GAME_MODE_*` conversion and reported three new undeclared-enum errors;
that transient run is not an accepted application proof. To freeze the input, the final read-only Docker
command adds `--root .claude/orchestrator/state/T-27051-geko/snapshot` before `plan
config/renames/proposals/o-geko-apply.tsv --ledger o-geko`, using `tokimemo-decomp-arm64` with two CPUs.
The ignored snapshot contains only `src/`, `include/`, `config/` and `tools/`; no raw/disc/build data.

Snapshot SHA-256 (sorted relative path, NUL, per-file SHA-256, newline; 1,244 files):
`a3b804d606c4ebc986cf3bac029ccc02e95e0c469497c8b1554d3041c06e4ed1`.
Final batch SHA-256: `f9940ae538f31fb5952f46ab7c44014d5f01e512a945a7afafc84a6f37ba7d71`.
Its enum header defines `GAME_MODE_PLAYER_BIRTHDAY=0x56`, `GAME_MODE_GIRL_BIRTHDAY=0x57`,
`GAME_MODE_GIRL_ENCOUNTER=0x59`, `GAME_MODE_GIRL_INVITATION=0x5A`, `GAME_MODE_SPECIAL_EVENT=0x5D`,
`GAME_MODE_NEW_YEAR_A=0x52` and `GAME_MODE_RPG_BATTLE=0x81`; the copied GEKO callers agree.
The manifest and command log stay in the ignored scratch directory. Snapshot validation does not replace
the live plan immediately before application. Historical out-of-scope comment mentions are planner notes,
not name/ownership failures.

Final snapshot plan: **exit 0, 1,061 rows, 26 files to change, zero problems**, including GCC proof.
The independent review's final change corrected only `D_8014662C`'s citation from line 68 to line 69;
the validated target map is unchanged. Independent standards/spec review checked all 391 function
locators, all 670 data locators and 176 explicit argument-consumer claims, plus exact mutations, ranking,
Sunday rollover, overrides and broad asset samples. Three generic sound targets now say `audio_command`;
their music/effect class is not assumed. No findings remain in the symbol batch.

## Logical C filenames and topic-header breakdown

This separate file/header plan is not part of the application batch. All paths under `girl_encounters`
remain **HYPOTHESIS** pending ADR emulator confirmation. Preserve all 22 object boundaries recorded in
`config/objects/GEKO.txt`; especially keep the mixed birthday/scene-script object intact. Each later
file move is its own rename-tool commit with the standard matching proof. The exact proposed target
directory is `src/ovl/girl_encounters/`:

| Original C object | Logical C filename | Source evidence (line numbers in original object) |
|---|---|---|
| `src/ovl/GEKO/80132000.c` | `walk_home_entry.c` | 4,43,65: binder, mode handler and talk adapters |
| `src/ovl/GEKO/801323E0.c` | `walk_home.c` | 9,178,217,364,405: approach callbacks, topics and choices |
| `src/ovl/GEKO/801338F0.c` | `walk_home_followup.c` | 12,81,143: follow-up callbacks, club variants and groups |
| `src/ovl/GEKO/80134040.c` | `player_birthday.c` | 4,133,219: player-birthday mode, replay guard and visitor ranking |
| `src/ovl/GEKO/80134C90.c` | `girl_birthday_entry.c` | 4: girl-birthday mode handler and presentation services |
| `src/ovl/GEKO/80134D10.c` | `girl_birthday_and_scene_scripts.c` | 375,417,755,1347,1587: birthday logic, birthday/visit/scene binders and shared scene flow |
| `src/ovl/GEKO/801393C0.c` | `yuko_scenes.c` | 18,39,69: Yuko selector1, callbacks and initializer |
| `src/ovl/GEKO/80139910.c` | `shiori_scenes.c` | 48,73,218,256,446: Shiori selectors1/2/8 and birthday writes |
| `src/ovl/GEKO/8013AF80.c` | `yuina_scenes.c` | 26,56,102,111,222: Yuina selectors1/7, RPG and forced calendar |
| `src/ovl/GEKO/8013B930.c` | `rei_scenes.c` | 37,101,138,171,209: Rei selectors1/2/3 and choice result |
| `src/ovl/GEKO/8013C4C0.c` | `ayako_scenes.c` | 18,39,54,71: Ayako selector1, speakers and affection |
| `src/ovl/GEKO/8013C980.c` | `mira_scenes.c` | 31,53,99,108: Mira selectors1/8, speakers and affection |
| `src/ovl/GEKO/8013D0F0.c` | `mio_scenes.c` | 23,39,54: Mio selector1 and scores |
| `src/ovl/GEKO/8013D4B0.c` | `yukari_scenes.c` | 36,66,86,95,136: Yukari selectors1/2 and scores |
| `src/ovl/GEKO/8013DB80.c` | `nozomi_scenes.c` | 18,39,54: Nozomi selector1 and scores |
| `src/ovl/GEKO/8013DF70.c` | `megumi_scenes.c` | 33,63,73,134: Megumi selectors1/2 and affection threshold |
| `src/ovl/GEKO/8013EE40.c` | `saki_scenes.c` | 23,39,54,65: Saki selector1, counters and scores |
| `src/ovl/GEKO/8013F360.c` | `yumi_scenes.c` | 47,93,108,152,354: Yumi selectors1/2/7, scores and effects |
| `src/ovl/GEKO/80140A00.c` | `miharu_scenes.c` | 69,115,136,359,166: Miharu selectors1/2/4 and story counter |
| `src/ovl/GEKO/80141AF0.c` | `yoshio_scenes.c` | 18,35,67,142: Yoshio flow, ranking and result store |
| `src/ovl/GEKO/801426D0.c` | `invite_entry.c` | 4: invitation mode handler and presentation services |
| `src/ovl/GEKO/80142760.c` | `invite.c` | 9,117,391,452,492,533: invitation scripts, eligibility, choices, appointments, Sunday and overrides |

The exact compatibility umbrella is `include/ovl/girl_encounters.h`. It initially includes the topic
headers below without duplicating declarations. Splitting is a later pure move:
`tools/sync_protos.py --snapshot` and `--compare` must prove identical declaration views for every C file.
Shared game types and canonical main declarations stay in their registered topic owners.

| Proposed topic header | Exact declaration ownership / consumers |
|---|---|
| `include/ovl/girl_encounters/main_views.h` | Existing GEKO `MAIN_API_OVERRIDE_*` macros and explicit views: narrow `D_801206DA`, unprototyped `talk_names_set`, and `func_80046094` return view (`include/ovl/GEKO.h:4-13`). Include before the first shared game/main header; preserve all current views. |
| `include/ovl/girl_encounters/walk_home.h` | Functions and local script sections/cursors/topic/flags, callback tables, thresholds, month lengths and club labels from all three walk-home objects. |
| `include/ovl/girl_encounters/birthday.h` | Both birthday handlers, birthday/visit binders and sections, cursors/context/timers, portrait/reply tables and both birthday callback tables. Covers the birthday portion of `80134D10.c`; Shiori scenes also consume visit-script sections. |
| `include/ovl/girl_encounters/scene_scripts.h` | `func_80137990`, three 79-section rows, and selected text/command/voice cells `D_80146274/78/7C`; evidence `80134D10.c:1347-1585`, `:1666-1691`. Common scene flow and all girl objects consume these; main group/line cells remain main-owned. |
| `include/ovl/girl_encounters/scene_flow.h` | Shared scene mode handler, talk adapters, bank/load/wait/finish functions and pack loader (`80134D10.c:1587-1820`); references the girl entry dispatchers. |
| `include/ovl/girl_encounters/scene_effects.h` | Light-effect initializer (`80134D10.c:1823`), color-quad helper (`8013F360.c:279`), and Shiori/Megumi shared phase/delay arrays (`include/ovl/GEKO.h:857-861`). Other image resources stay with their girl's topic. |
| `include/ovl/girl_encounters/invite.h` | All invitation functions, scripts/cursors/topic, callback/weight/availability/mapping/month-day tables and the overlay appointment view (`80142760.c:9-591`, `include/ovl/GEKO.h:842-855`). Do not duplicate main appointment types. |
| `include/ovl/girl_encounters/scenes/yuko.h` | Functions, callback tables and private image/state resources of `yuko_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/shiori.h` | Functions, callback tables and private image/state resources of `shiori_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/yuina.h` | Functions, callback tables and private image/state resources of `yuina_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/rei.h` | Functions, callback tables and private image/state resources of `rei_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/ayako.h` | Functions, callback tables and private image/state resources of `ayako_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/mira.h` | Functions, callback tables and private image/state resources of `mira_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/mio.h` | Functions, callback tables and private image/state resources of `mio_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/yukari.h` | Functions, callback tables and private image/state resources of `yukari_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/nozomi.h` | Functions, callback tables and private image/state resources of `nozomi_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/megumi.h` | Functions, callback tables and private image/state resources of `megumi_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/saki.h` | Functions, callback tables and private image/state resources of `saki_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/yumi.h` | Functions, callback tables and private image/state resources of `yumi_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/miharu.h` | Functions, callback tables and private image/state resources of `miharu_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |
| `include/ovl/girl_encounters/scenes/yoshio.h` | Functions, callback tables and private image/state resources of `yoshio_scenes.c`; exact source and selector evidence in the C-file table above and symbol batch. Shared effects and scripts remain in their own headers. |

These are 21 topic headers plus the compatibility umbrella. Local `FnTbl*` typedefs remain in C until
disambiguated: copying repeated spellings into an umbrella closure would conflict. Unshared callback
types may remain local. Header guards derive from these paths. No file/header move has been executed or
dry-run here; the symbol-only plan does not prove the file/header plan.

## Constants inventory for the next identifier batch

This is a source-backed inventory, separate from the 1,061 function/data rows. Proposed `ENCOUNTER_*`
names inherit the provisional prefix's **HYPOTHESIS**. Closed sets use enums; masks, dimensions, bounds
and thresholds use defines. Every field/local/parameter retains its fixed-width type and current casts,
signedness, subtraction and postincrement. An enum type must not replace byte/halfword storage.
Application must use `rename.py const` at explicitly named sites, then GCC/view and byte-match proofs;
an equal literal at another site is not authorization to replace it.

Reuse existing core definitions rather than inventing overlay copies. The final
`include/game_enums.h` and applied `config/renames/game-mode.txt` establish `GameMode` / `GAME_MODE_*`.
Existing core hypotheses remain hypotheses.

| Existing vocabulary | GEKO application contexts |
|---|---|
| `GameMode` / `GAME_MODE_*` | Five mode handlers in the entry table; New Year redirects `80134040.c:57`, `80134D10.c:675`; RPG transition `8013AF80.c:118`. Never revive `MainMode` / `MODE_*`. |
| `GirlId` / `GIRL_*` | Saved-girl switch `80134D10.c:1590-1631`, script selection `801323E0.c:96-153`, invitation exclusions `80142760.c:235-241`, every girl initializer. Literal girl IDs become existing identities; Yoshio/Rei/Sotoi do not inherit ordinary dating eligibility. |
| `Place` / `PLACE_*` | Eligibility `80142760.c:217-241`, weights `:245-284`, appointment stores `:468-487`, override destinations `:538-590`; retain IDs 0..20. |
| `Club` / `CLUB_*` | Follow-up club equality/partition `801338F0.c:96-138`; birthday portraits `80134D10.c:1086-1115`. `<6` is a club partition bound, not one club's identity. |
| `Weekday` / `WEEKDAY_SUNDAY`, `WEEKDAY_SATURDAY` | Birthday context `80134D10.c:793`, forced calendar `8013AF80.c:223-226`, Sunday arithmetic `80142760.c:513`; retain arithmetic/local widths. |
| `CalendarMarker` / `MARKER_*` | `801323E0.c:227-273`: existing culture-festival, sports-festival, school-trip, exam, New-Year-prep and notice A/B/D/F markers. Birthday context uses notice C/E (`80134D10.c:793`); player birthday tests exam (`80134040.c:85`). Marker 0x15 has no current core entry. |
| `DayKind` / `DAY_KIND_*` | Birthday exclusions `80134D10.c:793`; pool/sea/skating/ski predicate `80142760.c:231-232` uses specifically `DAY_KIND_HOLIDAY=2`, not any non-school day. |
| `DayEventType`, `DAY_EVENT_EXAM`; `VoiceMode`, `VOICE_OFF` | First-record birthday exam test `80134D10.c:705`; Yuina voice branch `8013AF80.c:68`; Rei completion `8013B930.c:183`. |
| `Parameter` and named parameter fields | Condition/guts/stress deltas `8013F360.c:384-385`, `8013B930.c:254`, `80140A00.c:245`; no overlay duplicate parameter IDs. |

New constant families and exact rule contexts follow. Template families describe the complete inventory
for the indicated sites; choosing each final spelling and writing const-tool site selectors is later work.

| Proposed constant/enum family | Values, rules and evidence |
|---|---|
| `EncounterWalkHomeModeStage` | `ENCOUNTER_WALK_HOME_MODE_STAGE_APPROACH=0`, `...FOLLOWUP=1`; `80132000.c:45-54`. Preserve default reset and the byte mode_stage. Other mode entries accept only stage 0. |
| Per-girl `Encounter...Scene` enums | Actual selector sets: Yuko1; Shiori1/2/8; Yuina1/7; Rei1/2/3; Ayako1; Mira1/8; Mio1; Yukari1/2; Nozomi1; Megumi1/2; Saki1; Yumi1/2/7; Miharu1/2/4. Names `ENCOUNTER_<GIRL>_SCENE_ONE/TWO/.../EIGHT` retain numeric variant meaning; use each selector definition in the C-file table. Yoshio checks no selector and gets no fabricated selector enum. Keep talk_work's declaration. |
| `EncounterSundayComponent` | `ENCOUNTER_SUNDAY_YEAR=0`, `...MONTH=1`, `...DAY=2`; `80142760.c:492-531`. Callers use 0/1/2; preserve the implementation's default returning day for every other selector. |
| `ENCOUNTER_TOPIC_*` | None0; exam-marker1; culture-festival-marker2; club variants3/4/5; sports-festival-marker6; school-trip variants7/8; New-Year-prep-marker9; birthday10; notice-B/D/F topics11/12/13; notice-A year variants14/15; priority-marker topic16; Mira random topic100. `801323E0.c:80-81`, `:195-207`, `:227-275`; `801338F0.c:96-138`. Names describe marker origins/technical variants, not dialogue. Marker0x15 behind topic16 is a separate main-owner deferral. |
| Birthday replay masks | `ENCOUNTER_SHIORI_BIRTHDAY_DONE_MASK=0x01`, `ENCOUNTER_OTHER_GIRL_BIRTHDAY_DONE_MASK=0x02`, `ENCOUNTER_PLAYER_BIRTHDAY_DONE_MASK=0x04`; replay tests/stores `80134D10.c:764-771`, `:812-816`, player flow `80134040.c:137-141`. Shiori OR0x05 at `80139910.c:462` can use OR0x01/0x04 without changing store width. |
| Birthday control masks | Visitor skip0x08; background variant0x10; girl-birthday return0x20; player-birthday return0x40; excluded-girl filter0x80. Suggested `ENCOUNTER_BIRTHDAY_VISITOR_SKIP_MASK`, `...BACKGROUND_VARIANT_MASK`, `ENCOUNTER_GIRL_BIRTHDAY_RETURN_MASK`, `ENCOUNTER_PLAYER_BIRTHDAY_RETURN_MASK`, `ENCOUNTER_BIRTHDAY_EXCLUDED_GIRL_MASK`. `80134040.c:55-57`, `:120`, `:238`, `:255`; `80134D10.c:491`, `:548-549`, `:649-655`, `:673-675`, `:772`. Mask0x44 is OR0x40/0x04, not a new Boolean. |
| Place appointment masks | `ENCOUNTER_MOVIE_THEATER_APPOINTMENT_MASK=0x01`, `...CONCERT_HALL...=0x02`, `...ART_MUSEUM...=0x04`; `80142760.c:220-222`, `:479-487`. Tested on eligibility and ORed when storing an appointment; do not label them seen/completed scenes. |
| Callback counts | `ENCOUNTER_<FLOW>_MODE_STEP_COUNT`: approach33, follow-up19, player birthday29, girl birthday73, invitation32; views `801323E0.c:4-15`, `801338F0.c:4-18`, `80134040.c:35-49`, initializer `80134D10.c:107-127`, `80142760.c:4-57`. Per-girl counts are listed above. Counts alone do not authorize adding runtime bounds checks. |
| Script dimensions | `ENCOUNTER_<FLOW>_SCRIPT_SECTION_COUNT`: walk-home12, birthday/visit/invite13 each, scene79; `ENCOUNTER_SCRIPT_COLUMN_COUNT=3`. Binders `80132000.c:4-41`, `80134D10.c:375-457`, `:1347-1585`, `80142760.c:9-49`. Constants do not authorize merging separate cells into arrays. |
| Selection dimensions | `ENCOUNTER_SCORE_GIRL_COUNT=11`, `ENCOUNTER_INVITE_PLACE_COUNT=21`, `ENCOUNTER_INVITE_TOPIC_COUNT=19`, `ENCOUNTER_INVITE_SEASON_COUNT=12`, `ENCOUNTER_YOSHIO_RANK_COUNT=6`. Loops `80134040.c:235-252`, `801323E0.c:392-400`, `80142760.c:175-284`, `:424-433`; rank arrays/slot5 `80141AF0.c:152-214`. Yoshio's candidate scan is separately girls9 down through1. |
| Topic/birthday window bounds | `ENCOUNTER_TOPIC_SCAN_DAY_END=20` (offsets1..19); same-month birthday difference end21; following-month end20. `801323E0.c:200-207`, `:296-306`. Preserve unsigned subtraction, nonzero same-month difference and `(month+10)%12`; unrelated 20/21 literals are separate sites. |
| Walk-home high-counter threshold | `ENCOUNTER_WALK_HOME_COUNTER_THRESHOLD=76`; snapshot `801323E0.c:83-87`, consumers `:317`, `:328`, `:347`, `:411`, `:424`. Syoushin meaning remains unknown. |
| Score-band/group limits | Tokimeki divisor20, syoushin divisor20, walk-home group maximum5, invitation maximum4. Suggested `ENCOUNTER_TOKIMEKI_BAND_WIDTH`, `ENCOUNTER_COUNTER_GROUP_WIDTH`, `ENCOUNTER_WALK_HOME_GROUP_MAXIMUM`, `ENCOUNTER_INVITE_GROUP_MAXIMUM`. `801323E0.c:374-396`, `:428-430`; `80142760.c:425-430`, `:463-466`. Preserve casts/byte masks before modulo. |
| Walk-home/invitation deltas | Walk-home nonzero selected counter base+10 for state<2 or+6 otherwise; zero other-girl base+4 or+0 under the six-girl/state>=3 predicate. Invitation first nonzero+15, second+20; zero others+6 for state<2 or+4 otherwise; zero exit affection+1. `801323E0.c:371-400`; `80142760.c:392-434`, `:448`. `ENCOUNTER_<FLOW>_<CHOICE_BRANCH>_<SCORE>_DELTA` applies only at these sites. Zero/nonzero tests do not prove shared-runner accept/reject labels. |
| Birthday deltas | Initial nonzero affection decrease1/counter increase5. Reply0: tokimeki+3/affection+2/counter decrease15; reply1:+2/+1/decrease10; other:affection+1/counter increase10. `80134D10.c:1263-1274`, `:1303-1318`. `ENCOUNTER_BIRTHDAY_REPLY_<FIRST/SECOND/OTHER>_<SCORE>_INCREASE/DECREASE`; preserve -= versus +=. Three reply slots do not prove arbitrary invalid-choice indexing is safe. |
| Megumi affection threshold | `ENCOUNTER_MEGUMI_AFFECTION_THRESHOLD=80`; `8013DF70.c:74-79`, `:153-171`: >=80 score/skip and <80 alternative score/group/skip. |
| Counter thresholds | Saki counter27 >=3 clamps to2 (`8013EE40.c:92-99`); Miharu story_stage >=5 clamps to4 then increments (`80140A00.c:170-178`); Yumi flag67 >=2 repeats (`8013F360.c:118-137`). Suggested `ENCOUNTER_SAKI_COUNTER_CLAMP_VALUE=2`, `ENCOUNTER_MIHARU_COUNTER_CLAMP_VALUE=4`, `ENCOUNTER_YUMI_REPEAT_THRESHOLD=2`; retain original comparison forms3/5 until an expression change is byte-proved. Shared indices are separate schema work. |
| Phone timing | Prelude90; dial interval8/duration64; ring interval64/fixed limit320; generated ring multiplier2..4. `ENCOUNTER_PHONE_PRELUDE_TICKS`, `...DIAL_INTERVAL`, `...DIAL_END`, `...RING_INTERVAL`, `...RING_END`, `...RING_MINIMUM`, `...RING_VARIANTS`. `80134D10.c:821-822`, `:1197-1260`. Preserve &7/&0x3F, postincrements, Cross>=64 skip and byte phase counter. |
| Retry/text timing | `ENCOUNTER_AUDIO_RETRY_TICKS=1024`; player-birthday marker delay128; phone-text hold64; Rei text hold17. Retry `801323E0.c:36`, `80134040.c:104`, `80134D10.c:719`, `80142760.c:94`; delay `80134040.c:85-91`; holds `80139910.c:285-290`, `80140A00.c:386-391`, `8013B930.c:154`. Preserve >= versus == and tick units. |
| Scene-log sentinel | `ENCOUNTER_SCENE_LOG_NONE=0xFF`; special log id0x14 forces rank4 (`80134D10.c:1758-1770`), produced by forced-calendar init (`8013AF80.c:222-250`). `ENCOUNTER_FORCED_CALENDAR_SCENE_LOG=0x14` is a technical cross-reference only. Log-kind IDs2/9/13 and rank names belong to the main log owner. |

Explicit constant deferrals: calendar marker0x15 and calendar month/year labels absent from core;
girl-state mask0x7F and ranks2/3/4 should come from the main helper/schema owner rather than an overlay
duplicate. Girl flags b0/b3/b6/b14 and shared scene-flag/counter indices need multi-owner interpretation.
Appointment bits21..23/24/30..31 have fixed writes6/1/0 but no proved meanings. Script group/line IDs,
background/portrait resource IDs, CD LBAs, absolute pack addresses and audio command IDs keep technical
evidence without inferred story labels; their resource/audio owners must name formats/classes first.
GPU coordinates, texture-page values and animation internals remain optional hardware work. No constant,
file or header has been applied, and no runtime confirmation is claimed.

## Essential semantic continuation (T-27051)

The frozen 1,061 function/data rows remain unchanged. Supplementary identifiers and exact literal
sites are being reconciled against all 391 definitions in the 22 original objects. Full declaration
classification is separate from narrative confirmation and from the primary's eventual application.

Supplementary locators use the captured original-object paths and function names. For the primary's
concurrent filename-only moves, use `config/renames/proposals/layout-GEKO-files.tsv`, the YAML or
the preserved original address range to locate the live file. Captured-tree proofs remain distinct
from a later live checkout.

### Frozen identifier supplements

`config/renames/proposals/o-geko-essential.tsv` has 199 five-column rows: 4 parameters, 169 locals,
14 types and 12 fields. SHA-256:
`cbae30197ef9d747ded90bbc24b0d7b0e1afd3b4e36a66949f2583a8535c7ade`.
Its full verification-enabled Docker plan passes: 199 rows, 20 changed paths, exit 0, zero problems
or advisories. The separate `o-geko-essential-callback-types.tsv` adds 15 uniquely selectable local
callback types and their 15 `f` -> `callbacks` fields: 30 rows, 12 changed paths, exit 0, zero
problems/advisories; SHA-256
`035935663ee25c94ba0d1c38534b667f3545c60d90fc3ad1273fc02358deb12c`.
The two packets have distinct old identifiers; separate plans preserve the first frozen proof.
Authoritative results are `.claude/orchestrator/state/T-27051-essential/geko/essential-plan.json`
and `callback-plan.json` with their logs. Independent packet preparation reviews pass; no application is
claimed.

All 391 definitions, including old-style parameters, participate in the AST census: 11 parameters
and 239 locals. The 250 declarations classify as 173 proposed gameplay roles, 11 existing roles,
23 loop counters, 36 optional hardware/presentation declarations and 7 unused/match/frame items.
The callback-type audit records all 28 file-local definitions: 15 unique declarations are prepared;
13 declarations repeating five type names need file-scoped type selection. Each repeated
declaration still gets a concrete source-backed proposed role in the audit, rather than being
mistaken for unknown meaning. The original blanket deferral of all local callback types was too
broad.

The retained-use recheck subsequently recovered 13 more roles: five documented FAKE pads, three
mode-step snapshots, four background-read results governing skips and one FAKE actor-scan value.
They form `o-geko-essential-retained-locals.tsv`, SHA-256
`913ee1831233f40999026ee3c35d8f384a56ed554bfd914f17e9c6efa3722b92`; full verification-enabled
plan: 13 rows, 7 changed paths, exit 0, zero problems/advisories. All 23 retained loop counters
were checked as actual loop-condition controllers. The 199/30 packets remain unchanged; combined
identifier preparation now has 242 rows. Remaining unused declarations without a proved matching
purpose are explicitly no-game-logic residue, not invented FAKE roles.

The first 1,344-row constant inventory was revised after independent Spec review. Its
structural census left 1,180 symbols unprepared, including source-proven timeouts and reward deltas,
and some generic blocker text misclassified hardware calls. These are accepted preparation
findings, not a claim of unsupported meaning. The 199/30 identifier packets and their proofs remain
frozen. The two safe constant dry runs prove 40 topic-cell/offer-argument hits and 15 Sunday-call
hits only; Sunday definition comparisons and array indices remain separately recorded. Final
semantic-symbol and exact-operand review was required before constant-inventory handoff.

The corrected `o-geko-essential-constants.tsv` now has **1,123 exact sites / 500 symbols** in 17
columns, SHA-256 `4ae1acf081652d0cfd668cfea1dc17c4d5e520105ac25c39045eee2a420b5b0c`.
Independent Standards and Spec rechecks accepted its corrected immediate operand domains: calendar
markers/day kinds are separate from months, masks/deltas/tests are separate from absolute step
destinations, row strides have dimensional roles, and neutral FAKE identities are excluded.
All 3,624 dispositions are accounted for: 3,581 body operands across 391 functions plus 43 actual
typed dimensions, with 2,501 neutral/presentation/ABI exclusions. Remaining unknown shared meanings
and unsupported selectors have explicit evidence rather than invented game interpretations.

The 55 selected positive edits remain exactly equal to the original 40-topic/15-Sunday proof,
SHA-256 `37b9d27d61017937672730f0949e5b02126733995e1c55931719e88c20d3ceb3`.
All 26 numeric callback-dispatch arguments were checked against actual pointer-table words and
45 shared main definitions: three dispatcher arguments reach `wait_sub_sub` and are wait limits;
the other 23 are ignored ABI payloads and excluded. The helper increments its timer before its
comparison. The prepared constants do not impose a delay on callbacks that ignore their argument.
Proof, corrected findings and source contracts are in `revised-producer-report.md`,
`revised-packet-status.json`, `preserved-proof-mappings.json` and `callback-argument-closures.json`
under `.claude/orchestrator/state/T-27051-essential/geko/`. All 242 identifiers and their plans
remain unchanged; primary live application/view/build proofs are still required.

### Shared fixed-choice-order sentinel

GEKO's `talk_run(255, ...)` calls use a shared contract more specific than a generic prompt.
Main `talk_run` forwards its first argument as `talk_speak_part`'s choice argument
(`src/main/scene_library.c:1970,2036`). For three-choice command 3, value 255 bypasses
`junban_init`/`make_three_select` and writes the choice order `[0, 1, 2]`
(`:1499-1509`). Thus that command preserves fixed option order instead of shuffling it. This
meaning is conditional on the script command; it is not an invented universal response choice.
Any canonical constant belongs to the shared talk owner, and the existing fixed-width argument
and command behavior must remain intact.

### Birthday sentinel and shared score contracts

The player/girl-birthday scans (`func_80134808`, `func_801358CC`) use the shared helper results
for tie-breaking. Main `get_h_yuukou` computes weighted affection divided by 100 and clamps to
0..100; `get_h_tokimeki` includes parameter-dependent bonuses and clamps to the same range
(`src/main/menu_speaker_girl.c:1310-1323`, `:1137-1306`). In contrast, Yoshio's `func_801420E4`
directly sums the raw stored affection and tokimeki fields. A port must keep those distinct
contracts instead of substituting one common raw-score sum.

`func_80134808`, `src/ovl/GEKO/80134040.c:232-255`, initializes `best` to positive 255, scans
candidates, and calls `get_g_zyotai_s(best)` before checking `best == 255`. Main's implementation
in `src/main/menu_speaker_girl.c:818-841` only passes that argument to `get_h_yuukou` and
`get_h_tokimeki`. Both use `arg0 % 13` before girl-array indexing (`:1310-1323`, `:1154-1156`).
Thus the no-candidate call reads girl 8 (Yuko), since 255 % 13 is 8, before the explicit sentinel
branch skips the birthday candidate. These bodies do not make an out-of-range girl access for that
positive sentinel. Preserve the call/order/modulo and later skip in a port; do not turn this source
finding into an invented fallback birthday choice or a claim of newly observed runtime behavior.

### Current executable constant packet v2 (2026-10-24)

Fresh Docker plans in `.claude/orchestrator/state/T-27051-constant-v2/` prove 55 GEKO replacements:
40 topic and 15 Sunday-component sites, with zero unnamed selections. The corrected selectors
are `encounter_walk_home_topic`, `GEKO:encounter_walk_home_topic_offer(arg0)` and
`GEKO:encounter_invite_sunday_component(arg0)`. Callee arguments are distinct from caller-local
`arg0` comparisons. Fresh token-diff tuples match the frozen proof by caller alias, mapped source
line, original operand and named value; multiple literals on one line are independently resolved.
The private GEKO header has 22 users, all GEKO sources.

All 55 use the authorized unsuffixed positive route. These const ledger rows are metadata-free and
provide no signed-site replay metadata. Topic/component hypothesis notes and fixed-width storage
are preserved. The combined DATE/NAME_ENT/GEKO preflight passed at 204 exact replacements with
89 pinned inputs. Primary owns live application, the topic-domain extern comment, ledger checks
and matching verification. No source/header/ledger application or full build ran in this lane;
the broader essential inventory remains separate.
