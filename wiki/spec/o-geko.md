---
type: spec
updated: 2026-10-24
ticket: T-27032
status: analysis
---

# GEKO encounter rules

This page describes the code paths needed to port GEKO's rules. Proposed identifiers are in
`config/renames/proposals/o-geko.tsv`; none are applied. Original identifiers below are evidence keys.
The code supports five entry points: walking-home encounters, the player's birthday, a girl's birthday,
queued girl scenes and invitations that create appointments. The overall `girl_encounters` folder and
`encounter_` prefix remain **HYPOTHESIS**: ADR-0001 explicitly requires emulator confirmation for GEKO,
and this analysis performed no emulator run. Technical callback numbers do not establish a scene's story.

Read with [[phase2]], [[families/geko-family-results]], [[game-state]], `GLOSSARY.md`,
`docs/adr/0001-phase-2-naming.md` and `CODING_STANDARDS.md` sections 4, 5, 8 and 10.
The shared core names in this page are the names currently applied in `include/game.h`,
`include/game_enums.h` and `include/main_api.h`.

## Entry and ownership contract

`src/main/800789E0.c:418-431` selects these entry addresses. The scheduler supplies the queued girl;
for special scenes it also stores the girl in `D_80122CD4` and the scene in O.BIN `talk_work`
(`src/main/8006CB30.c:1880-1918`). Those shared cells remain main-owned.

| Applied game mode | Entry | Proposed function | State owner |
|---|---|---|---|
| `MODE_GIRL_ENCOUNTER` (0x59; mode name itself is a shared-core hypothesis) | `func_80132244` | `encounter_walk_home_entry` | stage 0 approach, stage 1 follow-up |
| `MODE_PLAYER_BIRTHDAY` (0x56) | `func_80134040` | `encounter_player_birthday_entry` | stage 0, 29 steps |
| `MODE_GIRL_BIRTHDAY` (0x57) | `func_80134C90` | `encounter_girl_birthday_entry` | stage 0, 73 steps |
| `MODE_SPECIAL_EVENT` (0x5D) | `func_8013882C` | `encounter_scene_entry` | queued girl, `talk_work` scene, stage 0, per-scene steps |
| `MODE_GIRL_INVITATION` (0x5A) | `func_801426D0` | `encounter_invite_entry` | stage 0, 32 steps |

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
0xFF instead. Special scenes use the shared group/line cells `D_800CA150`/`D_800CA154`; phone presentation
uses runner mode 2. A resource pointer is not a line number, and a callback step is not a dialogue group.

| Proposed prefix | Text / command / voice cells | Group / line cells | Other local state |
|---|---|---|---|
| `encounter_walk_home_` | `D_80144C30/34/38` | `D_80144C3C/40` | topic `D_80144C50`, follow-up flag `D_80144C54`, high-syoushin snapshot `D_80144C58` |
| `encounter_player_birthday_` | `D_80144E80/84/88` | `D_80144E8C/90` | shared birthday done bits and preserved talk byte |
| `encounter_girl_birthday_` | `D_80145048/4C/50` | `D_80145060/64` | saved triple/group, school-year index, context/phone/choice flags |
| `encounter_scene_` | `D_80146274/78/7C` | shared `D_800CA150/154` | saved queued girl and scene selector |
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

`src/ovl/GEKO/80134040.c`; `D_80144E94` is the 29-entry step table. Initialization sets done bit 4 and
skips the birthday flow if that bit was already set. The default visitor script is separate from the
selected-girl script. A calendar marker 5 can insert a 128-tick audio delay. Door-chime presentation is
suppressed by done bit 8; its exact narrative context remains unconfirmed.

`func_80134808` scans met girls 0..10. Lower `get_g_zyotai_s(id) & 0x7F` wins; ties use the greater sum
of affection and tokimeki. The incumbent survives an exact sum tie. Done bit 0x80 excludes a candidate when
`id + 1` equals the preserved shared talk byte. A missing candidate, state at least 2, or done bit 8 jumps
to step 23; otherwise it installs the selected girl's script. The source calls the state helper even when
best remains 0xFF, before the later sentinel test: helper behavior for that sentinel requires main-owner
verification before copying it into a port.

Completion redirects to `MODE_NEW_YEAR_A` when done bit 0x40 is set; otherwise it restores HUD/audio and
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

Initialization uses the queued girl. Shiori's already-run guard is done bit 1, other girls' is bit 2;
others also require flag b6. Initialization sets the respective guard. Local context starts as holiday-like;
ordinary non-Sunday school-calendar days select school context, unless explicitly forced by done bit 0x10.
December 24 bypasses those school-context tests. A same-club last-command-4 path can select club context
when no same-girl appointment exists today. Context labels are **HYPOTHESIS**; the predicates are source facts.
The source initializes phone ring count to `frame_count % 3 + 2` and school year to `year-0x5F` from April,
otherwise `year-0x60`.

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
after tick 64. One branch stops by tick 320; the other uses the initialized ring count times 64.

The location selector records log entries for corridor, club room, home front or park variants. Portraits
can be uniform, club-specific or casual; seasonal substitutions occur outside June through September.
A same-girl appointment today can jump to step 61. The explicit player-birthday test at step 47 uses the
same best-met-girl rule as visitor selection and requires the current girl to be that winner with state below 2;
it sets done bits 0x44 and jumps to step 52. Birthday completion redirects to `MODE_NEW_YEAR_A` on bit 0x20.

## Invitation and appointment rules

`src/ovl/GEKO/80142760.c`; `D_801476E4` has 32 callbacks. The main scheduler skips this mode when the
shared appointment is already active. Steps 0..15 initialize/select a destination, load audio/scripts,
background and portrait; 16..19 present/apply the first choice; 20..25 present/apply the second; 26..31
exit, store the accepted appointment or refusal script, restore display/audio, and finish.

`func_80144198(selector)` computes the current week's Sunday: `day - weekday + 6`, carries past the
current month length and from December to the next year. Selector 0 returns year, 1 month, anything else day.
Sunday input can return today. Year 0x60 has February 29; no general leap-year algorithm is present.

The destination chooser uses 21 place ids, each with an eligibility flag. It computes one of 12 season
slots over years 0x5F..0x61; January/February share the preceding winter slot. A place requires
`progress_state[place] == 1` and its minimum year/month. Further source gates are:

- Places 0, 1 and 18 are always excluded. Place 13 excludes February/December; place 14 excludes April/May of year 0x5F.
- Places 14, 15 and 7 respectively reject girl scene bits 1, 2 and 4. Movie and concert also require a nonzero girl/season availability entry.
- Place 7 has three specific year/season scene-counter exclusions.
- Places 16/17 need calendar day-kind 2 or July; 19/20 need day-kind 2 or December.
- The previous-spot exclusion table rejects one place; a packed-appointment comparison can reject its existing place too.
- Mio, Yuina, Ayako, Saki, Yukari, Megumi and Yumi have further fixed place/month exclusions. Their exact conditions remain in `func_80142C9C`; port this predicate as a table before simplifying it.

`place_to_topic[place]` maps eligibility to 19 script/weight topics. For place 13 in January, March,
June or November the topic increments by one. Weights are 11 girls by 19 topics. Both the total and
cumulative sums are bytes. The code seeds `rand` from `frame_count`, draws `rand() % total`, then picks
the first eligible place with cumulative weight **greater than or equal to** the draw. It initializes the
fallback place to 2. Preserve this boundary: using the usual strictly-greater test would change outcomes.
No zero-total guard exists in the source; reachable table invariants have not been proved in this pass.

After random selection, `func_80144270` can override the topic and mark a place available for these source windows:

| Girl | Place | Window / additional gate |
|---|---:|---|
| Mio | 14 | year 0x60, December/January/February before day 21; scene flag 12 ==0 |
| Ayako | 7 | year 0x61, September/October/November before day 21; flag 24 ==0 |
| Saki | 13 | year >=0x60, September/October before day 21; flag 30 ==0 |
| Mira | 2 | March/April/May before day 21; flag 47 ==0 |
| Yumi | 13 | year >=0x60, October before day 21; flag 71 ==0 |
| Yuko | several | sequential checks over scene counters, scene flag 57 and computed Sunday; see definition |

The tests use the literal stored year/month, including Mio's unusual January/December same-year condition.
Yuko's checks can overwrite each other; the last applicable test wins. Do not turn this into a first-match
list without proving equivalent priority.

First-choice nonzero: syoushin +15, group 23, step 24. Second-choice nonzero: syoushin +20, group 24 for
Yuina or 23 for the other girls, step 24. Zero on the second choice gives the other met girls a syoushin
increase of `rand() % (tokimeki/20 + 1)` plus 6 for girl state below 2, otherwise 4. The exit gives the
selected girl affection +1 for zero choice. Nonzero exit chooses one of two shared talk-exit helpers by girl.
Clamping occurs after the choice handlers; the final affection +1 helper itself does not call the clamp.

`func_80143F48` writes an appointment only when choice is zero. The word carries mapped place, computed
Sunday year/month/day, girl, active=1, bits 21..23=6, bit24=1 and bits30..31=0. The shared insertion helper
is `func_8006B5BC`; no main rename is proposed here. Places 14/15/7 set girl scene bits 1/2/4. Nonzero
choice switches to the refusal script, group `min(syoushin/20,4)`. The unknown packed-bit names remain
unknown because fixed stores do not establish their game meaning.

## Girl-specific queued scenes

The top-level saved-girl switch establishes identity. The scene selector is the queue record's numeric
scene, not a name derived from dialogue. All listed scene variants require mode_stage 0.

| Girl | Accepted `talk_work` | Callback counts |
|---|---|---|
| Yuko | 1 | 53 |
| Shiori | 1, 2, 8 | 39, 23, 55 |
| Yuina | 1, 2 | 42, 41 |
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
`MODE_RPG_BATTLE`), Rei's choice outcome, Megumi's affection threshold, Saki's counter branches, Yumi's repeat
branches, Miharu's story counter and Yoshio's sixth-ranked girl selection. The following evidence table
records score and progress mutations without interpreting the scene's dialogue.

| Evidence definition | Rule effect |
|---|---|
| `801393C0.c:func_80139640` | Yuko affection +1, then clamp |
| `80139910.c:func_8013A6D0` | For scene 1, Shiori tokimeki +1 and affection +2, then clamp |
| `80139910.c:func_8013AA1C` | Stress -1, then clamp |
| `80139910.c:func_8013AB4C` | Shiori tokimeki +2, affection +2; birthday done bits OR 5 |
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
| `80141AF0.c:func_80141D74` | Saves selected date girl in scene flag 82 and current girl, clears her met bit |

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
The source object's 391 function definitions all have proposed technical role names. This is an analysis
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
- Verify the birthday candidate helper's sentinel 0xFF behavior, the unusual next-month birthday index, and the sequential Yuko override priority against runtime traces.
- Recover the meaning of fixed appointment bits and unproved girl flags from their main/TEL owners, rather than assigning a narrative label from one write.
- Convert the long per-girl invitation exclusion predicate into a reviewed rules table. The exact source predicate is the current authority; this page lists its categories, not every clause.
- Detailed animation internals, image-pointer globals and unnamed shared-main dependencies remain presentation work. Scene script group transitions remain encoded in the original script resources; a clean-room Godot script layer needs a separate content plan.

Validation on 2026-10-24: Docker using `tokimemo-decomp-arm64` checked that every active row has five tab
columns, all function/data/type targets exist in this overlay's source/header, and old/new keys are unique.
No source, header, symbol map, raw game data or build configuration was changed. The table is a reviewable
proposal, not an application ledger.
