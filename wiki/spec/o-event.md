---
type: spec
updated: 2026-10-24
unit: EVENT
ticket: T-27023
sources: ["src/ovl/EVENT/", "include/ovl/EVENT.h", "config/renames/proposals/o-event.tsv", "src/ovl/DATE/", "src/ovl/GEKO/", "src/main/800789E0.c", "wiki/overlays.md", "wiki/spec/core.md"]
---

# Overlay EVENT: port spec (a dead development copy of the girl-scene and date-place code)

Phase 2 analysis of `src/ovl/EVENT` ([[tickets/T-27023-p2-o-event]], phase [[tickets/T-27000-phase-2]]). Rename tables: `config/renames/proposals/o-event.tsv` (1126 rows, passes `rename.py plan` with no problem), `o-event-devmain.tsv` (121 stale-address aliases) and `o-event-deferred.tsv` (6 rows whose C declaration sits in `include/main_api.h`). Facts only, in my own words; no game text, assets or asm are quoted. "HYPOTHESIS" marks a reading of game meaning the code does not prove. The shared names come from `config/renames/proposals/core.tsv` (GameState fields, `GirlId`, `Place`, `MainMode`).

**Verdict for the port: do not port EVENT.** The overlay can never run: it was linked against a different build of the main program than the one on the disc. The live versions of everything in it are DATE (mode 0x50 place dates, mode 0x5E girl scenes), GEKO and DATE2 (the ride movies). This spec exists for three reasons: it proves the claim, it is the only place where the girl-scene and place-date machinery of that era is laid out in one piece (a cross-check for the DATE, GEKO and DATE2 specs), and it documents the dev-to-shipped address map that explains the odd calls and variables in the matched C.

## 1. What it is and why it is dead

EVENT.EXN is 0x30000 bytes. By the jump-target fit it loads at 0x800F6000 ([[overlays]]); the file then covers 0x800F6000-0x80126000, which is main-exe bss in the shipped game. It holds 804 C functions in 18 C files (code 0x800F6000-0x8011F530, a 4-word rodata stub, jump tables, `.data` 0x801202D0-0x801258DC and a trailer), 95 step tables, and the scenes of two systems: the 14 per-girl scene sets (73 scenes) and the 21 place dates with their arrival phase and the amusement-park ride movies.

Evidence that it is a stale development build, in order of strength:

| Evidence | Fact |
| --- | --- |
| No loader | The shipped loader (`func_80078C48`, src/main/800789E0.c) maps the modes to 24 distinct disc sectors; EVENT's sector 0x940A (and GYOZI's 0x94CA) is not one of them, and no entry address in the 0x800F6000 range is stored by any mode. |
| Emulator probe | From the saved title-menu state (NAME_ENT resident, mode 0x11) `tools/emu/run.sh boot --dump`: RAM at the dev `step_next` address 0x80011DE0-0x80011E1F is all zero (a call there runs no code), RAM at the shipped `main_step_next` 0x8004284C is code, RAM at EVENT's frame handler address 0x800F6E84 is zero, and the overlay entry pointer D_80123110 holds NAME_ENT's entry 0x80144AD0. The emulator cannot start EVENT itself. |
| Calls into the main exe hit nothing | The code calls 135 distinct main-side addresses (4626 call and table slots). None is the start of a function of the shipped exe. 53 lie below 0x80041000, where the shipped text begins (1,445 of the 4,626 sites), the other 82 fall inside shipped functions. |
| Main variables sit elsewhere | 175 distinct main-side data addresses are used; 51 lie in the shipped rodata (0x800AF340-0x800B3220). The step byte is `D_800B1AF6`, which is rodata in the shipped exe. |
| The address map is consistent | Aligning EVENT's near-clones in DATE and GEKO call by call gives a stable map dev address to shipped function (section 9): `func_80011DFC` is `main_step_next` in 338 of 363 aligned sites, `func_80015D28` is `cd_read_sectors` in 166 of 166, `func_8004C250` is `check_para_limit` in 87 of 87. The dev GameState has the same fields at other offsets. |
| Script sizes are almost the same | The per-girl script loader reads 3-7 sectors per girl; the shipped `SERIFU/EVENT_xx.BIN` files have the same sector count for 10 of the 12 files (Saki's and Mira's files grew by one sector). The date script loader matches 10 of 11 `SERIFU/DATE_xx.BIN` sizes (Yuko's grew by one). So the code belongs to the same game at an earlier state. |
| Twin code | 675 of the 804 C functions have a sibling of opcode-shape ratio 0.90 or more in another overlay (634 in DATE, 97 in GEKO as best match). Many scene steps and nearly all shared date steps have clones in DATE; the table lengths and the order of steps differ, so the dev build is an earlier state of the same machine. |

Core already treats EVENT and GYOZI as never loaded and linked against another main layout (`wiki/spec/core.md`, the `LegacyGirlRecord` row). This unit adds the proof above. The loader question of [[tickets/T-0200-event-gyozi-loader-and-address]] is answered for EVENT: there is none, and 0x800F6000 is the address the dev build linked it for (the only address where its jump targets land on function starts).

## 2. Names used here

- Prefix `spot_` for every EVENT symbol, as the ADR map has it (row EVENT.EXN: folder `date_spots_unused`, prefix `spot_`, "confirm unused"). The "unused" part is confirmed (section 1). The map's description ("an older set of date-place scenes") is too narrow: the overlay also holds the girl scenes and the ride movies. The prefix fits the place dates best. I kept it as instructed; a broader folder name is proposed under open questions.
- Girls are named by given name (`GirlId`): the family name Himoo is Yuina, Katagiri is Ayako, Kagami is Mira, Asahina is Yuko, Mikihara is Megumi, Nijino is Saki, Koshiki is Yukari, Kiyokawa is Nozomi, Saotome is Yumi, Tatebayashi is Miharu. Girl 12 is Rei and girl 13 Yoshio.
- `spot_<girl>_scene<k>_...` is scene k of that girl. A step function is named `spot_<owner>_<role>_<NN>` where NN is its position in the step table of its owner; a function used in several tables of different owners is `spot_shared_<role>_<n>`. Tables are `spot_<owner>_steps`. Pointer-set initialisers are `spot_<girl>_<kind>_ptrs_<n>_init` and their data `spot_<girl>_<kind>_ptrs_<n>`.
- `spot_devmain_*` names the stale main-side addresses (section 9). They are not symbols of the shipped exe, so `rename.py` refuses them as overlay rows; they are in the separate table `o-event-devmain.tsv` for an integrator to decide (an EVENT-only symbol file or documentation only).
- The six rows of `o-event-deferred.tsv` are EVENT-range data names whose only C declaration is an `extern` in `include/main_api.h`; the gcc proof of the tool fails until that header line is renamed too.
- Names that claim game meaning without proof are marked HYPOTHESIS in the note column (for example the variant names `SPOT_VARIANT_*`). The two dev mode numbers are facts of the code: mode 0x50 and 0x60.
- No O.BIN-named function is defined in this unit, so there are no `comment` rows. Section 9 lists the O.BIN-named shipped functions that the stale addresses stand for.
- Girl-record vocabulary follows core's `GirlRecord`: `tokimeki` (+2), `affection` (+6); the field at +0xA is called `counter` in this table because core's name `syoushin` is itself a HYPOTHESIS.

## 3. Memory model

| Area | Address | What |
| --- | --- | --- |
| Code | 0x800F6000-0x8011F52F | 18 objects (proposed file names): 800F6000 `girl_scene_entry`; 800F7220-8010B360 the 14 girl objects `<girl>_scenes` (one per girl, section 6); 8010BB90 `place_scene_entry` (date section fillers, place entry); 8010FE10 `place_scenes` (place scenes and the shared date steps); 8011C650 `ride_movies` |
| Rodata | 0x8011F530-0x801202D0 | the BIOS `srand` stub (`spot_bios_srand`), jump tables and debug strings; Shift-JIS labels are place and speaker names and debug format strings (not reproduced) |
| Data | 0x801202D0-0x801258DC | pointer tables (`spot_script_sections` 235 words, `spot_date_sections` 81 words, 63 pointer sets of which 54 are 11-word still sets), 95 step tables, small state cells and byte tables |
| Girl script area | 0x80198000 | the loaded `EVENT_xx` script file (text, command and voice tables; addresses in `spot_script_sections`) |
| Date script area | 0x80197000-0x801A4000 | common part (2 sectors at 0x80197000) and the girl's date script (19-24 sectors at 0x80198000) |
| Scene pack | 0x801B0000 | the scene still pack read by the `assets_load` steps (up to 0x3D sectors): a 0x2000 byte palette block, six 0x4000 byte tiles of one full-screen still (0x801B2000-0x801C6000), and the frame, sequence and tpage tables around 0x801CE0xx |
| Result tables | 0x801A4000 | 3 sectors of per-place result byte tables read by `spot_place_tables_load_step` |

State lives in the dev main (stale addresses, section 9) and in a few EVENT cells: `spot_place` (D_801243B0, place 0-20), `spot_place_variant` (D_8012531C, 0-2), `spot_arrival_mode` (D_801252E4, 0-2), `spot_ride` (D_80125320, 0-4), the reaction cells D_8012438C/94/9C and D_801243A4 (three halfwords each), `spot_dialogue_group_offset` (D_80124344) and `spot_mood_tier` (D_8012435C).

## 4. Entry points and the frame loop

The dev main would call one of two entry functions every frame, depending on its mode. Both end with the same tail: dev `check_k_scroll`, `k_disp_inc2`, the screen frame, the message window and title updates, `hizuke_show`, `scene_frame_update` (the names are the shipped twins, section 9).

**Girl scenes: `spot_girl_scene_frame` (0x800F6E84).** Dev mode 0x60 (the shipped counterpart is `MODE_DATE_SCENE` 0x5E). Inputs: girl `D_800EECB4` (dialogue_work word 1, 0-13) and scene `D_800EECB0` (word 0). Per frame: `bustup_wink_update`; switch on the girl to `spot_<girl>_scene_select`; that function switches on the scene to `spot_<girl>_scene<k>_run`; the runner prints stage and step (a debug print), and when the stage `D_800B1AF5` is 0 copies the scene's step table to the stack and calls `table[step](0x80)`. Any other stage or scene calls the bad-state handler (shipped `game_reset_to_title`). Some runners run extras after the step (face blinking, palette cycles, particles). Table 6.1 lists the selectors.

**Place dates: `spot_place_scene_frame` (0x8010F4B8).** Dev mode 0x50 (= shipped `MODE_DATE`). Stage 0 runs `spot_place_arrival_run` and stage 1 runs `spot_place_scene_run`; anything else is the bad-state handler.

- Stage 0 (`spot_place_arrival_run`, 0x8010F55C): copies the 61-step table `spot_place_arrival_steps` and runs the step. Step 9 and step 22 of that table hold the placeholder `spot_place_neighborhood_park_arrival`; when the table slot of the current step holds it, it is replaced for this frame by the arrival step of the place `spot_place` (cases 1-20, place 0 keeps the placeholder). The stage then continues through the 61 steps (section 7.1).
- Stage 1 (`spot_place_scene_run`, 0x8010F74C): copies the 21 place tables to the stack, picks the table by `spot_place`, runs the step, then three extras: `spot_particles_update` when `D_800EECF0` is set; for place 1 (Kirameki Central Park) the falling sprites `spot_place_falling_sprites_update` at steps 12-20 and `spot_place_falling_sprites_hide` at step 21 (hide).

The initialisation of a place date is step 0 of the arrival table, `spot_place_init_step` (0x8010F340): it resets the layers (dev `draw2d3d`, `actors_draw_enable`, `sprite_tables_clear`, `actor_clear_range`, `back_clear_switch`, `text_system_reset`, `k_reset`, `hizuke_init`, `message_window_init`), copies the appointment place `D_800B0F2C` to `spot_place` and the date girl `D_800B0F30` to the girl cell and the current girl, sets `spot_arrival_mode` to 1 (0 when `frame_count & 3` is 0 and the girl is not Yuina), clears the cells, runs `dialogue_names_set` and shows the portrait. The shipped twin is `func_80144720` in DATE.

## 5. The step-table interpreter

A scene or place is a table of function pointers indexed by the step byte (`GameState.step`, dev `D_800B1AF6`). The runner copies the table to its stack and calls the entry for the current step with the argument 0x80, which most steps ignore. A step does its work for one frame (or several, when it waits) and ends by calling `main_step_next` (dev `func_80011DFC`: step + 1, step timer and substep cleared), or jumps with `main_step_set` (`func_80011EC4`), or skips by adding a constant to the step byte. There is no other script language: the dialogue text, voices and commands are tables inside the loaded script file and are driven by the talk steps; the branching of a scene is in the small "branch" steps.

The step vocabulary (names are the proposals; counts are table slots over all 95 tables):

| Idiom | Steps | What it does |
| --- | --- | --- |
| Background change | `bg_load` (own function, 157 plain and a few with a label), then dev `scene_wait_cd_read` (383), `dec_init` (175), `change_dec_bg` (143) | starts the read of a background picture by a dev-disc id, waits for the CD, decodes it (MDEC) and shows it. 167 own `bg_load` functions; the ids are dev-disc values and are not portable. |
| Asset pack | `assets_load`: `cd_read_sectors(n, 0x801B0000, sector)` then the pointer-set initialiser | loads the scene still pack and wires the pointer set; the next steps are `still_show` ones. |
| Still and animation | `still_show`: palette load, `event_still_show` (six tiles), `event_anim_show`; the stat variants first apply the girl-record deltas and `check_para_limit` | one full-screen event picture with its animation. `still_fade_in` and `still_fade_out` fade it. |
| Talk | `spot_talk_step` (330 slots), `spot_result_talk_step`, `spot_mood_talk_step`, `spot_year_talk_step`, `spot_talk_until_done_step` and the variants | wires the dialogue cells to the current script tables and runs `dialogue_run` with the last choice `D_800EECBC` (shipped `dialogue_choice`). |
| Portrait | `normal_date_girl_in` (171), `girl_out` (176), the five `bustup_expression_*` steps, `bustup_expression_mood` (75), `hizuke_hide` and `hizuke_appear` (61 each) | slides the girl in or out, picks the expression (0 best mood ... 4 worst) or hides the date panel. |
| Sound | `bgm_start` / `se_play`: `snd_command(0x2xx)` starts BGM sequence xx of the loaded pack, `snd_command(0x5xx)` plays a sound effect; sound-bank steps load and poll the per-girl or per-place bank | starts music, plays effects and loads the sound banks of the scene. |
| Branch | `branch`, `special_case`, `branch_on_mood`, `speak_branch`: add to the dialogue cursor `D_80094714` (jump ahead in the script), to the dialogue group `D_80094730` or to the step byte | the conditional logic of a scene: girl mood rank, choice, flags, season, girl id. |
| End | `spot_scene_close_step` then a bad-state entry; the short scenes end with `spot_scene_exit_step` | closes the date outcome (7.6). |

The dev primitives that fill the tables are listed in section 9; the full step list of every table (the whole state machine of each scene and place) is the appendix [[spec/o-event-steps]]. A typical scene table is: `spot_script_load_step`, waits, `assets_load` (one or two), `spot_talk_reset_step`, `init`, a `still_show`, then alternating talk, expression and background-change blocks, `spot_birthday_check_step`, the result and mood talks, `girl_out`, `return_step`, `spot_scene_close_step`, bad state.

## 6. Girl scenes

### 6.1 Selectors

Each girl has a selector (`spot_<girl>_scene_select`, switch on `D_800EECB0`):

| Girl | Selector | Scenes |
| --- | --- | --- |
| shiori (0) | `spot_shiori_scene_select` | 1, 2, 3, 4, 5, 6, 8 |
| mio (1) | `spot_mio_scene_select` | 1, 2, 3, 4, 5 |
| yuina (2) | `spot_yuina_scene_select` | 1, 2, 3, 4, 5 |
| ayako (3) | `spot_ayako_scene_select` | 1, 2, 3, 4, 5 |
| saki (4) | `spot_saki_scene_select` | 1, 2, 3, 4, 5 |
| yukari (5) | `spot_yukari_scene_select` | 1, 2, 3, 4, 5, 6 |
| nozomi (6) | `spot_nozomi_scene_select` | 1, 2, 3, 4, 5 |
| mira (7) | `spot_mira_scene_select` | 1, 2, 3, 4, 5, 6, 8 |
| yuko (8) | `spot_yuko_scene_select` | 1, 2, 3, 4, 5, 6 |
| megumi (9) | `spot_megumi_scene_select` | 1, 2, 3, 4, 5, 6 |
| yumi (10) | `spot_yumi_scene_select` | 1, 2, 3, 4, 5, 6, 7 |
| miharu (11) | `spot_miharu_scene_select` | 1, 2, 3, 4, 5 |
| rei (12) | `spot_rei_scene_select` | 1, 2, 3 |
| yoshio (13) | `spot_yoshio_scene_select` | 1 |

Girl 13 (Yoshio) has no switch; `spot_yoshio_scene_select` calls his only scene. The scene numbers follow the DATE twin `date_shiori_dispatch`: DATE runs Shiori's scenes 3-6 only; scenes 1, 2 and 8 have no DATE scene (HYPOTHESIS: they went to GEKO, whose step functions have siblings of scene 1's steps).

### 6.2 Script files

`spot_script_load_step` (clone of DATE `func_8014C7FC` and GEKO `func_80138F7C`) loads the girl's `EVENT_xx` script to 0x80198000 and refills the section table; girls 11, 12 and 13 share one file (the "other" file). Sector counts of the dev loader against the shipped file sizes:

| Girl | File | Dev sectors | Shipped sectors |
| --- | --- | --- | --- |
| shiori (0) | EVENT_FU | 6 | 6 |
| mio (1) | EVENT_KI | 3 | 3 |
| yuina (2) | EVENT_HI | 4 | 4 |
| ayako (3) | EVENT_KA | 5 | 5 |
| saki (4) | EVENT_NI | 6 | 7 (differs) |
| yukari (5) | EVENT_KO | 5 | 5 |
| nozomi (6) | EVENT_KY | 6 | 6 |
| mira (7) | EVENT_KG | 4 | 5 (differs) |
| yuko (8) | EVENT_AS | 4 | 4 |
| megumi (9) | EVENT_MI | 5 | 5 |
| yumi (10) | EVENT_SA | 6 | 6 |
| miharu (11) | EVENT_OT | 7 | 7 |
| rei (12) | EVENT_OT (shared) | 7 | 7 |
| yoshio (13) | EVENT_OT (shared) | 7 | 7 |

The date script loader `spot_date_script_load_step` reads `DATE_xx.BIN` (0x13-0x18 sectors) the same way and calls `spot_fill_sections_<girl>` to set the 81 section addresses.

### 6.3 Scene catalogue

Steps is the table length, Talk the number of talk-type steps, BG the own background loads, "Ending stats" the girl-record deltas that the scene applies (t tokimeki, a affection, c counter; the counter is core's HYPOTHESIS `syoushin`, so a negative counter is a reward), Labels the place or speaker labels the scene sets (my English glosses), Extras the per-frame effect functions. Scenes with no delta set (rows with a dash) carry their changes in shared steps or none.

| Girl | Scene | Steps | Talk | BG | Ending stats | Labels | Extras |
| --- | ---: | ---: | ---: | ---: | --- | --- | --- |
| shiori | 1 | 39 | 4 | 2 | +1t +2a | school gate | sprites_update |
| shiori | 2 | 22 | 3 | 1 | - | home front | - |
| shiori | 3 | 45 | 9 | 1 | +2t -28c | - | - |
| shiori | 4 | 44 | 7 | 2 | +3t +2a -20c | - | - |
| shiori | 5 | 58 | 8 | 4 | +1t +1a -20c | Ferris wheel | - |
| shiori | 6 | 45 | 5 | 2 | +1t +2a -20c | - | - |
| shiori | 8 | 47 | 7 | 3 | - | Shiori's room, home front | face_update |
| mio | 1 | 39 | 4 | 2 | +1t +2a | corridor | - |
| mio | 2 | 67 | 10 | 4 | -2t -1a +20c | scream ride | - |
| mio | 3 | 39 | 6 | 2 | +1t +2a -20c | - | bars_overlay_draw |
| mio | 4 | 46 | 6 | 3 | +2t +2a -20c | - | - |
| mio | 5 | 53 | 9 | 3 | +3t +2a -20c | - | - |
| yuina | 1 | 42 | 5 | 2 | +1t +2a | rooftop | - |
| yuina | 2 | 41 | 5 | 2 | +3t +2a -20c | - | - |
| yuina | 3 | 39 | 5 | 2 | +2t -10c | - | se_a, se_b |
| yuina | 4 | 39 | 6 | 2 | - | - | - |
| yuina | 5 | 38 | 6 | 2 | +3t +2a -20c | - | - |
| ayako | 1 | 32 | 4 | 2 | +2a | behind the school | - |
| ayako | 2 | 49 | 9 | 2 | +2t +1a -20c | - | sprites_update |
| ayako | 3 | 48 | 7 | 2 | -1t -1a +20c | - | - |
| ayako | 4 | 42 | 7 | 2 | +2t +1a -20c | - | - |
| ayako | 5 | 45 | 7 | 2 | +2t +2a -20c +3t +1a -20c | - | - |
| saki | 1 | 35 | 4 | 2 | +1t +1a | corridor | - |
| saki | 2 | 37 | 5 | 2 | +2t +1a -20c +3t +2a -20c | - | - |
| saki | 3 | 55 | 11 | 2 | +3t +2a -20c | - | - |
| saki | 4 | 40 | 6 | 2 | +2a -20c | - | - |
| saki | 5 | 51 | 10 | 2 | +1t +1a -20c | - | - |
| yukari | 1 | 32 | 3 | 2 | - | corridor | - |
| yukari | 2 | 19 | 3 | 1 | +1t | corridor | - |
| yukari | 3 | 38 | 5 | 2 | +2t +1a -10c | - | - |
| yukari | 4 | 44 | 8 | 2 | +2t +1a -20c | - | sprites_update |
| yukari | 5 | 40 | 6 | 2 | +3t +2a -20c | - | - |
| yukari | 6 | 56 | 11 | 3 | +1t +1a -20c | - | - |
| nozomi | 1 | 29 | 3 | 1 | +1t +2a | corridor | - |
| nozomi | 2 | 50 | 7 | 2 | +1t +1a -20c | - | - |
| nozomi | 3 | 55 | 8 | 2 | +3t +2a -20c | - | - |
| nozomi | 4 | 41 | 6 | 2 | +2t +2a -10c | - | - |
| nozomi | 5 | 40 | 5 | 2 | +3t +2a +1t +3a -20c | - | - |
| mira | 1 | 38 | 6 | 2 | - | corridor, boy 1, boy 2 | - |
| mira | 2 | 40 | 5 | 2 | - | - | - |
| mira | 3 | 38 | 4 | 2 | +1t +1a -20c | - | - |
| mira | 4 | 51 | 9 | 2 | -1a +10c | amusement park | - |
| mira | 5 | 42 | 5 | 2 | +3t +2a -20c | - | - |
| mira | 6 | 15 | 4 | 0 | +3t +2a | - | - |
| mira | 8 | 21 | 2 | 1 | - | corridor | - |
| yuko | 1 | 52 | 6 | 4 | +1a | teacher, corridor, staff room | - |
| yuko | 2 | 41 | 6 | 2 | +3a -10c | - | face_update |
| yuko | 3 | 35 | 4 | 2 | +3t +2a -20c | - | palette_cycle, gradient_sprites_draw, gradient_quad_draw |
| yuko | 4 | 47 | 7 | 3 | +1t +1a -20c | - | gradient_sprites_draw |
| yuko | 5 | 42 | 6 | 2 | +1t +1a -20c | - | - |
| yuko | 6 | 42 | 7 | 1 | +1t +1a -20c | - | - |
| megumi | 1 | 45 | 5 | 2 | +1t +1a | corridor | sprites_update |
| megumi | 2 | 20 | 2 | 1 | - | - | - |
| megumi | 3 | 47 | 9 | 2 | +1t +1a -20c | amusement park | - |
| megumi | 4 | 63 | 19 | 1 | +2t +1a -20c | - | - |
| megumi | 5 | 36 | 5 | 2 | +1t +1a -20c | - | - |
| megumi | 6 | 36 | 6 | 1 | +3t +2a -20c | - | - |
| yumi | 1 | 47 | 7 | 3 | +1t +2a | corridor | - |
| yumi | 2 | 21 | 2 | 1 | +1t +2a | - | - |
| yumi | 3 | 41 | 5 | 2 | +1t +1a -10c | - | - |
| yumi | 4 | 44 | 6 | 3 | +3t +2a -20c | pond | - |
| yumi | 5 | 44 | 7 | 2 | +2a -10c | - | - |
| yumi | 6 | 52 | 8 | 3 | +2t +1a -20c | - | - |
| yumi | 7 | 55 | 6 | 3 | - | entrance, home front | flash_draw, gradient_quad_draw, sprites_update |
| miharu | 1 | 22 | 3 | 1 | - | mystery woman, corridor | - |
| miharu | 2 | 25 | 3 | 2 | - | mystery woman, corridor | - |
| miharu | 3 | 14 | 2 | 0 | - | - | - |
| miharu | 4 | 56 | 9 | 4 | - | Tatebayashi, Tatebayashi (honorific), Kirameki Central Park, tree-lined path | - |
| miharu | 5 | 14 | 2 | 0 | - | - | - |
| rei | 1 | 51 | 9 | 2 | - | corridor | - |
| rei | 2 | 37 | 6 | 1 | - | corridor | - |
| rei | 3 | 41 | 6 | 1 | - | corridor | - |
| yoshio | 1 | 42 | 3 | 2 | - | classroom | - |

Reading the table: scene 1 of every girl is a short school or home meeting (corridor, rooftop, behind the school, classroom, school gate, staff room) with two or three stat points; scenes 2-6 are the date events that `spot_date_event_choose` (7.4) starts, one per season for most girls (the scene number follows the season for most girls but not for all: Shiori's spring event is scene 4), each with a fixed reward set (typically +1..+5 tokimeki, +1..+5 affection, -10..-40 counter); scenes 7 and 8 and a second scene 6 are extras for a few girls. Three scenes carry only a negative set (Mio's 2, Ayako's 3, Mira's 4): their steps apply `-t -a +c` instead (HYPOTHESIS: these are the unlucky events of those girls; the code shows the signs only).

### 6.4 Effects

11 scenes run extra per-frame code (the Extras column): Yuko 2 (face update: a 13-frame palette cycle and a flag toggle), Yuko 3 and 4 (gradient quads and sprite batches, a 10-frame palette ping-pong), Shiori 1 (15 sprite records 4-18 falling and respawning at random), Shiori 8 (face update), Ayako 2, Yukari 4 and Megumi 1 (random sprite drifts), Yumi 7 (a flash quad and ten delayed sprites) and Yuina 3 (two sound-effect triggers); the shared bar overlay `spot_bars_overlay_draw` is called by the Ayako, Mio and Miharu scenes. Gradient quads and triangles are drawn through `spot_gradient_quad_draw` and `spot_gradient_tri_draw`; the screen wipe is `spot_wipe_gradient_draw`. The hardware layer (primitives, ordering table) is described at module level only.

## 7. Place dates

### 7.1 Stage 0: the arrival table (61 steps)

| Step | Function | What it does |
| --- | --- | --- |
| 0 | `spot_place_init_step` | step 0 of the arrival table: resets graphics layers and flags, copies the appointment place (D_800B0F2C) to D_801243B0 and the dat |
| 1 | `spot_place_bank_load_step` | starts the sound bank load for the place: two fixed dev-disc sectors chosen by place group (places 1/3, 9/10, 12, 16/17, 19/20, 2, |
| 2 | `spot_place_bgm_start_step` | when the bank load has finished (poll dev func_8001496C returns 1) starts the place BGM snd_command(spot_place_bgm_ids[place]) and |
| 3 | `spot_common_script_load_step` | reads 2 sectors from dev-disc sector 0xA0E0 to 0x80197000 (the common part of the date script area) and fills the common pointer t |
| 4 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 5 | `spot_date_script_load_step` | date girl script loader: switch on the date girl D_800B0F30 (0-9, default 10): fills the section table with spot_fill_sections_<gi |
| 6 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 7 | `spot_place_tables_load_step` | reads 3 sectors from dev-disc sector 0xA143 to 0x801A4000 (per-place result byte tables) and stores their addresses with spot_plac |
| 8 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 9 | `spot_place_neighborhood_park_arrival` | placeholder: replaced for the frame by the arrival step of the current place (place 0 keeps this one: label, background read, dialogue source cells) |
| 10 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 11 | `spot_devmain_dec_init` | dec_init (shipped twin) |
| 12 | `spot_devmain_change_dec_bg` | change_dec_bg (shipped twin) |
| 13 | `spot_place_intro_step` | arrival step 13: computes the dialogue group offset D_80124344 from mood tier and arrival mode, clears the outcome cells and flags |
| 14 | `func_80045BEC` | dev-main step, not identified |
| 15 | `spot_devmain_place_init` | place_init (shipped twin) |
| 16 | `spot_meeting_names_step` | sets the two speaker labels from the place name table: the place of D_800B0F2D (or of D_800B0F2E when it equals the appointment pl |
| 17 | `spot_arrival_talk_step_common` | talk step with choice 0xFF and the cells D_80124384/88 with the common tables D_801241D4/E4/F4; used at five positions of the arri |
| 18 | `spot_meeting_choice_menu_step` | choice menu of the place proposal: normal_date_three_select when a third place D_800B0F2F exists (not 0xFF), else normal_date_two_ |
| 19 | `spot_meeting_decide_step` | place decision after the girl proposes places: when the first two appointment places agree and the choice is 0 it flags D_801243B4 |
| 20 | `spot_arrival_talk_step_common` | talk step with choice 0xFF and the cells D_80124384/88 with the common tables D_801241D4/E4/F4; used at five positions of the arri |
| 21 | `spot_devmain_normal_date_bg_fadeout` | normal_date_bg_fadeout (shipped twin) |
| 22 | `spot_place_neighborhood_park_arrival` | placeholder: replaced for the frame by the arrival step of the current place (place 0 keeps this one: label, background read, dialogue source cells) |
| 23 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 24 | `spot_devmain_dec_init` | dec_init (shipped twin) |
| 25 | `spot_devmain_change_dec_bg` | change_dec_bg (shipped twin) |
| 26 | `spot_place_bank_load_step` | starts the sound bank load for the place: two fixed dev-disc sectors chosen by place group (places 1/3, 9/10, 12, 16/17, 19/20, 2, |
| 27 | `spot_place_bgm_start_step` | when the bank load has finished (poll dev func_8001496C returns 1) starts the place BGM snd_command(spot_place_bgm_ids[place]) and |
| 28 | `spot_devmain_normal_date_bg_fadein` | normal_date_bg_fadein (shipped twin) |
| 29 | `spot_devmain_place_init` | place_init (shipped twin) |
| 30 | `spot_meeting_place_check_step` | if the first appointment place D_800B0F2D differs from the current place D_801243B0 sets the dialogue cell D_80124384 to 7 and ski |
| 31 | `spot_arrival_talk_step_common` | talk step with choice 0xFF and the cells D_80124384/88 with the common tables D_801241D4/E4/F4; used at five positions of the arri |
| 32 | `spot_mira_mood_jump_step` | if the girl is Mira (7), her mood rank is at least 3 and the arrival mode is 2 jumps to step 0x39 and moves the place D_801243B0 o |
| 33 | `spot_portrait_read_pair_step` | portrait reads of the current girl in two substeps (spot_portrait_read_a then spot_portrait_read_b) |
| 34 | `spot_devmain_read_bustup` | read_bustup (shipped twin) |
| 35 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (shipped twin) |
| 36 | `spot_bustup_addr_init_step` | addr_init_bustup (dev func_80045B14): sets the portrait sprite addresses of the current girl, then step_next |
| 37 | `func_8004AB74` | dev-main step, not identified |
| 38 | `spot_reaction_rows_build_step` | for the three reaction cells: row A = result cell (0 becomes 1, 4 becomes 3, 3 when the current place equals the girl favourite by |
| 39 | `spot_arrival_wait_step` | when the arrival mode is 0 and D_801243B4 is 0 calls the dev wait helper (dev func_80046B88 with 0x80) and stays; otherwise step_n |
| 40 | `spot_low_looks_penalty_step` | if the looks value (dev +0x106) is below 0x1E (below 0x23 for Mira) sets the dialogue group offset to 0xC, skips 0xF steps and low |
| 41 | `spot_date_event_choose` | chooses a special girl event during a date: by girl (0-11), place D_801243B0, month window (spring 3-5, summer 6-8, autumn 9-11, w |
| 42 | `spot_arrival_talk_gate_step` | when the arrival mode is not 0 or D_801243B4 is 1 steps twice and returns 0, otherwise runs spot_arrival_talk_step |
| 43 | `spot_devmain_wait_sub_sub` | wait_sub_sub (shipped twin) |
| 44 | `spot_devmain_normal_date_girl_in` | normal_date_girl_in (shipped twin) |
| 45 | `spot_arrival_talk_step` | talk step with the cells D_80124344 (group) and D_80124348 and the tables D_8012425C/C8/334; when arrival mode is 2, D_80124348 is |
| 46 | `spot_all_places_equal_step` | if the four newest appointment places D_800B0F31..34 are all equal and the year is at least 0x60 calls spot_place_empty_step, else |
| 47 | `spot_special_day_talk_step` | on the first frame chooses the greeting kind D_8012434C from the calendar day: March 21 kind 0, July 24 (1995) / 28 (1996) / 27 (1 |
| 48 | `spot_devmain_stage_next` | main_stage_next (shipped twin) |
| 49 | `spot_arrival_talk_step_common` | talk step with choice 0xFF and the cells D_80124384/88 with the common tables D_801241D4/E4/F4; used at five positions of the arri |
| 50 | `spot_cell_seven_step` | stores 7 in the dialogue cell D_80124384, then step_next |
| 51 | `spot_devmain_wait_sub_sub` | wait_sub_sub (shipped twin) |
| 52 | `spot_arrival_talk_step_common` | talk step with choice 0xFF and the cells D_80124384/88 with the common tables D_801241D4/E4/F4; used at five positions of the arri |
| 53 | `spot_devmain_return_step` | return_step (shipped twin) |
| 54 | `spot_scene_close_step` | last real step of date scenes: counts the outcome (girls[g].visit_counts[D_800EECE4]++ on the first frame of the step, step_timer  |
| 55 | `spot_arrival_talk_step` | talk step with the cells D_80124344 (group) and D_80124348 and the tables D_8012425C/C8/334; when arrival mode is 2, D_80124348 is |
| 56 | `spot_devmain_normal_date_girl_out` | normal_date_girl_out (shipped twin) |
| 57 | `spot_arrival_talk_step` | talk step with the cells D_80124344 (group) and D_80124348 and the tables D_8012425C/C8/334; when arrival mode is 2, D_80124348 is |
| 58 | `spot_devmain_return_step` | return_step (shipped twin) |
| 59 | `spot_scene_close_step` | last real step of date scenes: counts the outcome (girls[g].visit_counts[D_800EECE4]++ on the first frame of the step, step_timer  |
| 60 | `spot_devmain_bad_state` | game_reset_to_title (shipped twin) |

### 7.2 Arrival steps per place

`spot_place_<place>_arrival` (0x74-0x504 bytes each) does for its place: sets the dialogue source cells `D_8009473C/40/44` (dev) to the place's tables, sets the place label, picks the variant `spot_place_variant` (0 normal, 1 alternate, 2 special) from flags, season and girl, writes the outcome id `D_800EECE4` (index into the girl's visit counters), and loads the per-girl result rows `D_8012438C/94/9C/A4` from the per-place byte tables (stride 9, 15, 18 or 36 bytes per girl, three bytes per variant). Then it starts the background read. Example variants read from the code: Kirameki Central Park gives the special variant between March 26 and April 10 (blossoms) under flag and girl conditions, otherwise the path variant, or the pond variant when the girl's flag bit is set and it is not summer; the aquarium gives the special variant (dolphin show) from year 0x61 on, or in year 0x60 from March, when the girl's flag byte is clear.

### 7.3 Stage 1: place tables

| Id | Place | Table | Steps | Own functions | Arrival bytes | Labels |
| ---: | --- | --- | ---: | ---: | --- | --- |
| 0 | neighborhood_park | spot_place_neighborhood_park_steps | 18 | 10 | 0xa4 | neighborhood park |
| 1 | kirameki_central_park | spot_place_kirameki_central_park_steps | 35 | 14 | 0x308 | Kirameki park, cherry blossoms, tree-lined path, pond |
| 2 | shopping_street | spot_place_shopping_street_steps | 48 | 20 | 0x74 | shopping street, boutique, fancy shop, junk shop |
| 3 | aquarium | spot_place_aquarium_steps | 32 | 12 | 0x254 | aquarium, dolphin show, inside the building |
| 4 | zoo | spot_place_zoo_steps | 35 | 14 | 0x2f0 | zoo, koala, inside the park |
| 5 | botanical_garden | spot_place_botanical_garden_steps | 34 | 12 | 0x1e0 | botanical garden, inside the park |
| 6 | planetarium | spot_place_planetarium_steps | 36 | 13 | 0x2bc | planetarium, inside the building |
| 7 | art_museum | spot_place_art_museum_steps | 37 | 16 | 0x504 | art museum, exhibition A, sculpture exhibition, famous painters exhibition, inside the building |
| 8 | library | spot_place_library_steps | 36 | 14 | 0x210 | library, inside the library |
| 9 | game_center | spot_place_game_center_steps | 39 | 17 | 0x220 | game center, video game corner, medal game corner |
| 10 | bowling_alley | spot_place_bowling_alley_steps | 32 | 12 | 0x1dc | bowling alley |
| 11 | karaoke | spot_place_karaoke_steps | 32 | 12 | 0x1dc | karaoke, inside the box |
| 12 | amusement_park | spot_place_amusement_park_steps | 103 | 38 | 0xf4 | amusement park, inside the amusement park, hero show, night parade |
| 13 | stadium | spot_place_stadium_steps | 29 | 12 | 0x260 | stadium, inside the stadium |
| 14 | movie_theater | spot_place_movie_theater_steps | 45 | 15 | 0x334 | movie theater, inside the building |
| 15 | concert_hall | spot_place_concert_hall_steps | 41 | 15 | 0x314 | concert hall, inside the hall |
| 16 | pool | spot_place_pool_steps | 37 | 13 | 0x1c4 | pool, inside the pool |
| 17 | sea | spot_place_sea_steps | 47 | 17 | 0x208 | swimming at the sea, beach, deserted island |
| 18 | shrine | spot_place_shrine_steps | 61 | 22 | 0xd0 | festival stalls, fireworks display |
| 19 | skating_rink | spot_place_skating_rink_steps | 22 | 7 | 0x1c4 | skating place, skating rink |
| 20 | ski_resort | spot_place_ski_resort_steps | 31 | 12 | 0x1c4 | ski place, ski slope |

The shopping street (`spot_place_shopping_street_steps`, 48 steps) lets the choice (0 boutique, 1 fancy shop, 2 junk shop) pick the dialogue group and the result rows (`func_80114C5C`); the art museum chooses among three exhibits; the sea has three spots (swimming, beach, deserted island); the shrine (place 18, `PLACE_SHRINE`) hosts two scenes, the festival stalls and the fireworks display, in 61 steps with sprite and palette effects; the amusement park is the largest (103 steps, 38 own functions) and holds the ride choice and the five ride movies (7.7).

### 7.4 The special event chooser (`spot_date_event_choose`, 0x801103A0)

The chooser is a step of the place tables, at fixed positions: place 0, 1, 3, 5, 10, 11 at step 1 (place 1 also at 13); the shopping street at 15; zoo 9; planetarium 11; art museum 7; library 0, 2 and 11; game center 13; amusement park at 8, 35, 56 and 60; stadium 8; movie theater and concert hall 17; pool and sea 10; skating rink and ski resort 0 and 8; the shrine has none. It is also in the arrival table (step 41), where its tests on stage 1 fail. Per date girl 0-11 it checks up to four seasonal events, spring (months 3-5), summer (6-8), autumn (9-11) and winter (12, 1, 2). An event fires when the place equals the event's place, the month is in its season, the event's flag in `event_flags` is still 0, the stage is 1 and the step equals the position of the chooser in that place's table (so each event is checked at one step only), and an extra condition holds (a mood rank limit, the year or affection byte, the choice `D_800EECBC`, a ride or shop flag, ...). On a hit it stores the scene number (2-6) in `D_800EECB0`, sets the flag and enters dev mode 0x60. For girls 0-7 the code implements:

| Girl | Spring | Summer | Autumn | Winter |
| --- | --- | --- | --- | --- |
| Shiori (0) | central park: scene 4 | pool: 6 | neighborhood park: 3 | amusement park: 5 |
| Mio (1) | amusement park: 2 | sea: 3 | library: 4 | movie theater: 5 |
| Yuina (2) | zoo: 2 | bowling alley: 3 | game center: 4 | shopping street: 5 |
| Ayako (3) | karaoke: 2 | sea: 3 | art museum: 4 | planetarium: 5 |
| Saki (4) | central park: 2 | pool: 3 | stadium: 4 | shopping street: 5 |
| Yukari (5) | botanical garden: 3 | aquarium: 4 | central park: 5 | ski resort: 6 |
| Nozomi (6) | art museum: 2 | sea: 3 | botanical garden: 4 | bowling alley: 5 |
| Mira (7) | shopping street: 2 | sea: 3 | amusement park: 4 | ski resort: 5 and 6 |

The other girls follow the same four-season pattern with exceptions (extra windows for Yuko, Megumi and Yumi); I did not tabulate them because the live table is `date_event_choose` in DATE (ratio 0.68 to this function), which may differ in places. The port must take the DATE table. Scene 1 (the school meeting) is never chosen here; its caller would be the dev main's day-event reader (HYPOTHESIS).

### 7.5 Outcome scoring (`spot_outcome_apply_step`, `spot_outcome_factors`)

Each place step ends in a result: the three reaction cells `D_8012439C[choice]` are small integers 0-4 loaded from the per-place byte tables; the choice is `D_800EECBC` (0-2). `spot_outcome_factors(c, &a, &b)` computes the two multipliers:

- `a` from the player's trivia value `v` (dev `D_800B0962`; HYPOTHESIS: the field is the trivia parameter, by the +0x10 shift of the dev parameter block against the shipped one): for `c > 0`, `a = 5 - i` for the first `i` in 0..4 with `v >= 100 - 25 * i` (so 5, 4, 3, 2, 1) and `a = 0` when `v == 0`; for `c <= 0`, `a = i + 3` for the same test (3 to 7) and `a = 8` when `v == 0`.
- `b = 5 - affinity[girl][place]` for `c > 0`, and `affinity[girl][place] + 3` otherwise. The affinity table is an 11 x 21 byte table (heroine times place, values 0-4; `spot_place_affinity`, shape only; the values are in the binary).

`spot_outcome_apply_step` then, with `c = D_8012439C[choice]`, sets the portrait layout from `D_8012438C[choice]` and adds to the 16.16 fields of the girl record: field +0 (tokimeki) `((3 - c) * a * b << 16) / 16`, with `a, b = spot_outcome_factors(3 - c)`; field +4 (affection) `((2 - c) * a * b << 16) / 16` and `(spot_outcome_terms[c] * a * b << 16) / 16`, with `a, b = spot_outcome_factors(2 - c)`. `x << 16 / 16` is x/16 whole points in 16.16, so a lower reaction value is better and a value above 2 turns the first term negative. It then runs `check_para_limit` and steps. The faces for the result come from `spot_face_from_row_step`, and `spot_reaction_rows_build_step` turns the three raw cells into tier cells: row A (0 becomes 1, 4 becomes 3, and 3 when the place is the girl's favourite place byte), row B (cell minus 1, clamped to 0-2).

### 7.6 Closing a date (`spot_scene_close_step`, `spot_birthday_check_step`)

- The birthday check (a step of every girl and place table, 65 tables): if today is the girl's birthday (month and day of `girls[g].birthday`) the machine goes to dev mode 0x50 stage 2 (a birthday scene), if today is the player's birthday to stage 3, otherwise it steps on.
- The close step (68 table slots): on the first frame of the step, when the mode is not 0x60, it increments `girls[g].visit_counts[outcome id]`; it stores the outcome as the girl's last outcome (outcome 7 is recorded as 6, 5 as 4 and 0xB-0xD as 0xA); then, if the looks value is at least 0x23 (at least 0x1E for every girl but Mira) it subtracts 0x28 from the girl's counter, otherwise it adds +1 tokimeki at place 0x12 (the shrine, the festival) and +2 tokimeki on May 4 and 5 when a record flag is set; `check_para_limit`; the place is stored in a state byte (dev +0x6CC, the last place) and two dev-main calls whose roles I could not identify (`func_80014CA8(0, 0)` and `func_8003E638(1)`) end the step; presumably they return to the caller's mode (HYPOTHESIS).
- `spot_low_looks_penalty_step` applies the opposite rule before the talk: a looks value below 0x1E (0x23 for Mira) gives -1 tokimeki, -1 affection and +5 counter and a dialogue group offset of 0xC.

### 7.7 The amusement park rides

At the park the choice (0, 1, 2) selects a ride (`spot_amusement_ride_choose_step`): choice 0 gives the scream ride (ride 3) when `D_80125308` or `D_80125304` is set, otherwise the roller coaster (ride 0); choice 1 gives the virtual ship (4) when `D_80125308` is set, otherwise the ghost house (1); choice 2 gives the Ferris wheel (2). Choice 1 adds 5 to the dialogue group and choice 2 adds 0xA. `spot_ride_setup` writes the ride label, the outcome id (0x14 coaster, 0x15 ghost house, 0x16 Ferris wheel, 0x17 scream ride, 0x18 virtual ship), the variant from the girl's ride-seen bit and the two result rows from the ride tables (0x24 bytes per girl). The ride movie runs through `spot_ride_movie_run` (file `ride_movies.c`): roller coaster `spot_ride_coaster_movie` (a 51-case state machine on the step timer), ghost house `spot_ride_ghost_house_movie`, Ferris wheel `spot_ride_ferris_wheel_movie`, scream ride and virtual ship share `spot_ride_scream_virtual_movie`. The DATE2 overlay holds the live movies (EVENT's Ferris wheel movie and DATE2 `func_801348F0` are siblings, T-24028).

## 8. Data tables

| Name | Address | Shape | Used by |
| --- | --- | --- | --- |
| `spot_<girl>_scene<k>_steps` and `spot_place_<place>_steps` | 95 tables in 0x801207B8-0x8012514C | 14-103 function pointers | the scene and place runners |
| `spot_place_arrival_steps` | 0x801243B8 | 61 pointers | `spot_place_arrival_run` |
| `spot_script_sections` | 0x801202D0 | 235 addresses | talk steps via `D_80120678/7C/80` |
| `spot_date_sections`, `spot_date_common_ptrs` | 0x80124200, 0x801241D0 | 81 and 12 addresses | date talk steps |
| `spot_<girl>_still_ptrs_<n>` | 54 sets | 11 words: animation tables, mode, palette, six tiles | `still_show` steps |
| `spot_place_result_tables` | 0x801251D0 | 40 pointers to byte tables | place arrival steps |
| `spot_place_affinity` | 0x8012536C | 11 x 21 bytes | `spot_outcome_factors` |
| `spot_place_names` | 0x80125504 | 22 x 33 bytes | `spot_meeting_names_step` |
| `spot_place_bgm_ids`, `spot_place_bank_arg_d/e/f` | 0x801254D8, 0x80125480, 0x801254AC, 0x80125454 | 21 halfwords each | the place sound steps |
| `spot_outcome_terms` | 0x80125360 | 5 halfwords | `spot_outcome_apply_step` |

## 9. The dev-build address map

The tables below are the evidence behind the `spot_devmain_*` names. Function twins (shipped name and address, votes in the aligned clones):

| Dev address | Slots | Name | Shipped twin | Evidence |
| --- | ---: | --- | --- | --- |
| func_80011DFC | 613 | `spot_devmain_step_next` | main_step_next (0x8004284C) | 338 of 363 aligned call sites in DATE/GEKO near-clones; 606 call sites in EVENT |
| func_80045B58 | 383 | `spot_devmain_scene_wait_cd_read` | scene_wait_cd_read (0x8007C784) | 315 of 345 table-slot votes |
| func_800469F4 | 238 | `spot_devmain_bg_read_sub2` | bg_read_sub2 (0x8007ED84) | 27 of 49 aligned call sites, 23 of 24 in larger clones; 238 call sites in EVENT |
| func_80046204 | 176 | `spot_devmain_normal_date_girl_out` | normal_date_girl_out (0x8007E5A0) | 154 of 170 table-slot votes |
| func_8004695C | 175 | `spot_devmain_dec_init` | dec_init (0x8007ECF8) | 159 of 160 table-slot votes |
| func_80045FF4 | 172 | `spot_devmain_normal_date_girl_in` | normal_date_girl_in (0x8007E390) | 152 of 162 table-slot votes |
| func_80046934 | 143 | `spot_devmain_change_dec_bg` | change_dec_bg (0x8007ECD0) | 115 of 132 table-slot votes |
| func_80015FE0 | 136 | `spot_devmain_bad_state` | game_reset_to_title (0x80046500) | 20 of 27 aligned default branches of mode switches; 95 table slots |
| func_80078970 | 114 | `spot_devmain_strcpy` | strcpy (0x800AE0F0) | 47 of 50 aligned call sites |
| func_8004CF30 | 110 | `spot_devmain_normal_date_speak` | normal_date_speak (0x80085A60) | 84 of 87 table slots aligned, 7 of 7 larger call sites |
| func_80015D28 | 91 | `spot_devmain_cd_read_sectors` | cd_read_sectors (0x80046318) | 166 of 166 aligned call sites (sectors, buffer, dev-disc sector) |
| func_80012D64 | 89 | `spot_devmain_load_palette` | load_palette (0x80043914) | 101 of 102 aligned call sites |
| func_800468CC | 88 | `spot_devmain_place_init` | place_init (0x8007EC68) | 59 of 77 table-slot votes |
| func_80045DF8 | 84 | `spot_devmain_return_step` | return_step (0x80084678) | 4 of 8 table-slot votes (weak); 84 table slots, one before each scene close |
| func_80078950 | 81 | `spot_devmain_printf` | printf (0x800AE0B0) | debug prints of stage and step ("s%d ss%d" style); the shipped twins have no such calls; the shipped printf address is the SDK name, the pairing is by role |
| func_8004A6AC | 75 | `spot_devmain_bustup_expression_mood` | bustup_expression_mood (0x80083338) | 45 of 47 table-slot votes |
| func_80046710 | 74 | `spot_devmain_normal_date_move_place` | normal_date_move_place (0x8007EAAC) | 59 of 67 table-slot votes |
| func_8004C250 | 71 | `spot_devmain_check_para_limit` | check_para_limit (0x80084D3C) | 87 of 87 aligned call sites |
| func_800433D0 | 65 | `spot_devmain_snd_command` | snd_command (0x80044750) | 9 of 15 aligned call sites; arguments are sound ids (0x200 + n starts BGM sequence n) |
| func_8004C984 | 65 | `spot_devmain_event_still_fade_out` | event_still_fade_out (0x800853FC) | 52 of 58 table-slot votes |
| func_8004C46C | 64 | `spot_devmain_event_still_show` | event_still_show (0x80084E90) | 96 of 97 aligned call sites |
| func_8004A764 | 64 | `spot_devmain_bustup_expression_1` | bustup_expression_1 (0x800833A0) | 14 of 19 table-slot votes |
| func_8004C90C | 63 | `spot_devmain_event_still_fade_in` | event_still_fade_in (0x80085368) | 55 of 59 table-slot votes |
| func_80046480 | 63 | `spot_devmain_normal_date_bg_fadein` | normal_date_bg_fadein (0x8007E81C) | 45 of 55 table-slot votes |
| func_8004C6B0 | 62 | `spot_devmain_event_anim_show` | event_anim_show (0x800850D4) | 96 of 97 aligned call sites |
| func_8004AF68 | 61 | `spot_devmain_hizuke_hide` | hizuke_hide (0x80083AD4) | 48 of 58 table-slot votes |
| func_8004AF90 | 61 | `spot_devmain_hizuke_appear` | hizuke_appear (0x80083AFC) | 42 of 52 table-slot votes |
| func_80011E28 | 53 | `spot_devmain_state_set` | main_state_set (0x80042878) | func_80119370 and func_801103A0 call it where DATE func_8013D174 and date_event_trigger call main_state_set(0x50/0x5E); dev mode numbers: 0x50 date, 0x60 girl scene |
| func_800789A0 | 53 | `spot_devmain_rand` | rand (0x800AE0D0) | 17 of 17 aligned call sites |
| func_80046600 | 46 | `spot_devmain_normal_date_bggirl_fadeout` | normal_date_bggirl_fadeout (0x8007E99C) | 30 of 40 table-slot votes |
| func_8004A9CC | 43 | `spot_devmain_bustup_show_checked` | bustup_show_checked (0x80083474) | 12 of 32 table-slot votes (weak) |
| func_80046598 | 36 | `spot_devmain_normal_date_bg_fadeout` | normal_date_bg_fadeout (0x8007E934) | 18 of 33 table-slot votes (moderate) |
| func_800335E0 | 32 | `spot_devmain_bustup_read_start` | bustup_read_start (0x80062CD0) | 18 of 40 aligned call sites, the rest pair with other readers |
| func_80012D40 | 26 | `spot_devmain_back_clear_switch` | back_clear_switch (0x800438F0) | 27 of 27 aligned scene-init sequences |

24 dev primitives in the tables (145 slots) are not identified; the commonest are `func_80046900` (29 slots), `func_80045BEC` (26), `func_8004B5E4` (25), `func_8004A89C` (15, in the `bustup_expression_*` family) and `func_800464E8` (10). Their roles stay open; none carries a rule that the port needs.

Data twins:

| Dev address | Name | Shipped twin |
| --- | --- | --- |
| D_800B0860 | `spot_devmain_game_state` | game_state (D_800E6280) |
| D_800B0A04 | `spot_devmain_girls` | girls (shipped GameState +0x1BC, dev +0x1A4) |
| D_800B1746 | `spot_devmain_current_girl` | current_girl (shipped +0xF5F, dev +0xEE6) |
| D_800B1AF5 | `spot_devmain_stage` | stage (shipped +0x1109) |
| D_800B1AF6 | `spot_devmain_step` | step (shipped +0x110A) |
| D_800EECB0 | `spot_devmain_dialogue_work` | dialogue_work (D_80122CD0), word 0 = scene id |
| D_800EECB4 | `spot_devmain_dialogue_work_girl` | dialogue_work word 1 = girl id of the running event (D_80122CD4) |
| D_800EECBC | `spot_devmain_dialogue_choice` | dialogue_choice (D_80122CDC) |
| D_800E9620 | `spot_devmain_actor_pool` | actor_pool (D_8011ECD0) |
| D_800EC190 | `spot_devmain_sprite_quads` | sprite_quads (D_801217D0) |
| D_800F19AB | `spot_devmain_dec_bg_brightness` | dec_bg_brightness (D_800B593C) |
| D_80094714 | `spot_devmain_dialogue_cursor` | group cursor cell of the dialogue tables (shipped D_800CA150, the pair GEKO uses) |
| D_8009471C | `spot_devmain_dialogue_tables` | dialogue_tables (D_800CA134) |
| D_80094730 | `spot_devmain_dialogue_group` | dialogue_group (D_800CA148) |

The dev GameState (`D_800B0860`) is the shipped `GameState` with other offsets. Offsets of the fields that EVENT uses, dev to shipped (all from aligned references): month +0x36 to +0x3F, year +0x35 to +0x3E, day +0x37 to +0x40, weekday +0x38 to +0x41, parameter values +0xF6.. to +0x106.. (+0x10), girls +0x1A4 to +0x1BC (+0x18), event flags +0x5CA to +0x56C, current girl +0xEE6 to +0xF5F, frame count +0x1284 to +0x10F8, step timer +0x1290 to +0x1104, mode +0x1294 to +0x1108, stage +0x1295 to +0x1109, step +0x1296 to +0x110A, substep +0x1299 to +0x110D. The girl record in the dev build is 0x34 bytes (shipped `GirlRecord` is 0x38); the s16 fields at +0..+0xA, the flag word at +0xC and the birthday word at +0x10 coincide with core's layout; +0x14 holds 0x20 visit counters (core: 0x24 event counters). The shipped lists name the dev types `LegacyGirlRecord`.

Mode numbers differ too: dev 0x50 (date) equals `MODE_DATE`, dev 0x60 (girl scene) is `MODE_DATE_SCENE` 0x5E.

## 10. Relation to the live game

- DATE (mode 0x50 and 0x5E) is the live successor of everything in section 7 and of scenes 3-6 of section 6; DATE's place tables are the same machine with the shipped primitives. `date_event_trigger` / `date_event_choose` (DATE) correspond to `spot_date_event_choose`. DATE's `date_fill_sections_<girl>` (names from the English patch notes) are the shipped twins of the `spot_fill_sections_<girl>` fillers.
- GEKO holds the girl encounters and one-shot events that probably correspond to scenes 1, 2 and 8 of the girl sets (HYPOTHESIS, from the sibling counts); `func_80138F7C` is the twin of `spot_script_load_step`.
- DATE2 holds the amusement-park rides (the ride movies of section 7.7).
- DATE's entry handler has four stages where this build's place entry has two: the birthday scenes (stages 2 and 3, reached by `spot_birthday_check_step`) are missing here and fall into the bad-state handler, another sign that the dev build predates the live one. The stale addresses in this unit never need to be ported. The numbers that matter for the port are the formulas of 7.4-7.7 and the structure of 5 and 6, to be confirmed against DATE.

## 11. Open questions

1. Dev mode 0x60 is used both to start a girl scene (`spot_date_event_choose`) and as the last real step of the short scenes (`spot_scene_exit_step`). I could not tell what the dev main did with the second use.
2. The map description of EVENT.EXN ("an older set of date-place scenes") is too narrow; the folder name could be `date_scenes_unused` (a name that covers girl scenes, place dates and ride movies) while the prefix `spot_` stays; to be decided by the reviewer.
3. The dev-main aliases (121 rows) cannot be applied by `rename.py` as overlay rows. Either an EVENT-only symbol file is introduced, or the aliases stay as documentation (this spec).
4. Dev-disc ids (background ids, scene pack sectors, LBAs) are build-specific and are not reproduced in the table of data; they appear as note text only.
5. The dev main code that sets the girl `D_800EECB4` and the scene `D_800EECB0` for scene 1, 2 and 8 and starts mode 0x60 is not in this unit; only `spot_date_event_choose` does it for the date events (scenes 2-6).
6. The variant semantics (`spot_place_variant` 0/1/2 and the outcome ids) are readings; DATE's twin cells (`D_800CA2FC`, `date_arrival`) will confirm them.
7. The EVENT code reaches the girl records mostly through the shared main type `Rec34` (core: `LegacyGirlRecord`), for example `D_800B0A04[g].unk_06`; the rows `SpotLegacyGirlRecord.*` rename only EVENT's own view (the plan counts 54 to 124 member uses per field left in other types). The same member names (`tokimeki_frac`, `tokimeki`, `affection_frac`, `affection`, `counter_frac`, `counter`, `flags`, `birthday`, `visit_counts`) belong on `LegacyGirlRecord` in core's table; EVENT and GYOZI are its only users, so this changes no live code.
