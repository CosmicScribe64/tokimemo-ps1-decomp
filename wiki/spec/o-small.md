---
type: spec
updated: 2026-10-24
sources: ["wiki/spec/m-flow.md", "wiki/spec/o-date.md", "src/ovl/KANGEI/80132000.c", "src/ovl/KANGEI/80132090.c", "src/ovl/KANGEI/80132E40.c", "src/ovl/KANGEI/80135F10.c", "src/ovl/DATE2/80132000.c", "src/ovl/DATE2/80132DE0.c", "src/ovl/DATE2/80137D50.c", "src/ovl/EN_NICHI/80132000.c", "src/ovl/EN_NICHI/80136B10.c", "src/ovl/OLH/80132000.c", "src/ovl/OMIMAI/801320A0.c", "src/ovl/OMIMAI/80132AD0.c", "src/ovl/OMIMAI/801331B0.c", "src/ovl/RENSYU/80132040.c", "src/ovl/VALEN/80132000.c", "src/ovl/VALEN/80132760.c", "src/ovl/VALEN/80133C70.c"]
---

# Small overlay group: entry and primary rules

Partial, source-backed port specification for KANGEI, DATE2, EN_NICHI, OLH, OMIMAI, RENSYU and VALEN. Prefixes follow the current ADR: `camp_` for KANGEI (folder/meaning confirmation pending), `amusement_`, `stalls_`, `help_`, `sick_visit_`, `practice_`, and `valentine_` (Valentine's Day supported; White Day remains unconfirmed). No game text is reproduced. Proposed names are not applied.

## Loader entry map

The main overlay loader table in `wiki/spec/m-flow.md` supplies the mode, disc LBA and entry PC. The C body at an entry is called once per frame; a file's address 0x80132000 is not necessarily its only entry.

| Overlay | Mode -> entry PC | Per-frame entry behavior (source) |
|---|---|---|
| KANGEI | 0x5F -> 0x80132FF8; 0x60 -> 0x80132000 | 0x5F entry `func_80132FF8` switches on mode_stage (0..0x1F scene phases) and draws the HUD; it is not the 22-entry mode_step callback table. For 0x60, `func_80132000` calls the scene dispatcher at mode_stage 0 and resets to title otherwise; its scene callback dispatcher is `func_80132214` (22 entries indexed by mode_step). The loader table and ADR mark camp interpretation as HYPOTHESIS. |
| DATE2 | 0x58 -> 0x80132214; 0x5C -> 0x80137F64 | `func_80132214` dispatches mode_stage 0..4, then draws parameter/background for stage 0 steps <0x13, otherwise shared frame/HUD. The second entry has a karaoke-return flow in `80137D50.c`. |
| EN_NICHI | 0x82 -> 0x80132000 | Initializes stall-game setup; later steps use the common scene state. `func_80132278` restores caller mode/stage/step. |
| OLH | 0x3A -> 0x80132000 | `func_80132000` dispatches help page groups by mode_stage and checks Start to return to command screen. |
| OMIMAI | 0x61 -> 0x80133120; 0x62 -> 0x80132000 | 0x61's `func_80133120` dispatches mode_step through a 26-entry table only at stage 0, otherwise resets to title. 0x62's `func_80132000` dispatches stage 0 through a 25-entry mode_step table, stage 1 through the separate 12-entry table at `func_80132D44`, and resets to title for all other stages. |
| RENSYU | 0x73 -> 0x80132040 | Callback table selected by mode_stage; actual practice flow is in the callback bodies, with the intro sequence at `func_801320E0`. |
| VALEN | 0x54 -> 0x80132274 | Stage 0..3 dispatch; other values reset to title. The loader mode is Valentine's Day. ADR still requires confirmation before treating additional White Day content as part of this overlay. |

## Per-overlay state and outcome rules

### KANGEI (camp interpretation unconfirmed)

The 0x5F loader entry `func_80132FF8` in `80132E40.c` switches on `mode_stage` across day phases and conditionally suppresses the icon bar for selected stages, when `mode_step != 0` and `D_80139AC0` is zero (stage 0x1E has the separate zero-flag condition). It is not the mode_step callback dispatcher. For mode 0x60, the `func_80132000` -> stage-0 path uses `func_80132214`, which invokes the mode_step-indexed callback from `D_801399B0` (22 entries). The final callback invokes `day_event_next(1)` after asynchronous audio loading; helper entries set portrait layout from current girl's status and use normal date presentation functions. `func_80133A54` advances a day-event scene: when `D_80120652` bit 0 is clear, its first `D_80120658 == 0` pass chooses an audio command from `D_80120666` bit 0, reads the current day event's girl and scene, then adds 2 practice points to the club chosen by the high bits of `player_flags`. Club indices 0–1, 2–3, 4–5, and >=6 select different GameState words for a +0x18000 update; then `parameter_up_down(girl, 0, scene)` runs and parameter display is initialized. If `D_80120658++ >= 5`, it advances mode_stage. This is a precise transition/effect; its seasonal/camp narrative is not proven.

Other inspected helpers show thresholds based on club practice points (0x64, 0xC8, 0x164), and one branch in `80135F10.c` subtracts 10 tokimeki from `date_girl` when a table result is zero, non-FF and the girl is met; table result 1 instead adds 10 stress. These are conditional effects, not a complete KANGEI rule census. Keep `camp_` semantic names marked HYPOTHESIS until emulator confirmation.

### DATE2 (amusement/karaoke overlay)

`func_80132214` routes mode_stage 0..4 to five phase functions. `func_801381A4` sets mode to DATE, stage 1, step 0x11 to return to the caller's date path. It is a concrete interface contract; loader mode 0x5C's story label as karaoke is also supported by `wiki/spec/o-date.md`, which identifies the DATE caller as its karaoke-place third-visit branch.

At `func_80136BA4`, branch order matters: Shiori's affection (`girls[0]`) < 10 increments `D_8013A428` by 2 and selects SHIORI; else current-girl yuukou < 10 increments that selector by 2; else Shiori's syoushin (`girls[0]`) >= 0x33 selects SHIORI; else if current girl's syoushin < 0x33, Shiori's tokimeki >= 0x32 increments selector by 1 and selects SHIORI, or current-girl tokimeki >= 0x32 increments selector by 1; if neither tokimeki threshold is met it increments selector by 4 and calls `mode_step_next()` twice. A final common `mode_step_next()` follows. No `else` fallback occurs when current girl's syoushin >= 0x33. This is scene-selection logic, not a scoring result.

Another inspected result body (`func_80136860` in `80132DE0.c`) applies a response code: it counts bits 0..2 of `D_8013A4C4`; 0 adds Shiori (`girls[0]`) affection 12 and current date girl's syoushin 7; 1 adds Shiori affection 3 and girl's syoushin 2; 2 adds current date girl's affection 3 and Shiori syoushin 2; 3 adds current date girl's affection 12 and Shiori syoushin 7. Then it clamps parameters and advances the day event. Exact choice prompts remain in game data and are not repeated here.

### EN_NICHI (festival stalls)

Main mode 0x82 enters `func_80132000`; the caller in DATE is the shrine-festival path and saves mode/stage/resume step before launch (`wiki/spec/o-date.md`). `func_80132278` clears cursor/upper actors and restores saved mode, stage and step. Stall play writes a result into `D_80139C4C` and the return sequence resumes the caller. Existing `wiki/spec/o-date.md` section 7 records the caller-side result handling and exact reward deltas; this partial overlay review did not re-derive every game subtype from every function body. Local overlay evidence: `func_8013735C` determines result codes from input/table comparisons and restores saved game state; `80136B10.c` contains play/exit steps. The key interface contract is the saved-state restore, with caller-owned reward interpretation.

### OLH (help)

`func_80132000` switches mode_stage 0,1,2 and 0x10..0x70 to a title/setup, menu, and seven help-content handlers, then updates cursor. A pressed Start sets `MODE_COMMAND_SCREEN` directly. The menu handler `func_80132294` acts only on Circle with cursor != -1; selector 0..6 jumps to stage 0x10..0x70, selector 7 jumps to stage 2. No relationship or day-event mutation is present in these inspected dispatch/menu bodies. Page-specific render content and every handler are unreviewed.

### OMIMAI (sick visit)

The 0x61 and 0x62 entries have separate state paths. At 0x61, `func_80133120` dispatches stage 0 through 26 callbacks (`func_80133424`); other stages reset to title. At 0x62, `func_80132000` uses a 25-callback table (`func_801322E4`) at stage 0, a separate 12-callback table (`func_80132D44`) at stage 1, and resets to title for other stages. Initial callbacks load scene data and audio; audio/timeouts retry by moving the step back one. `func_80132398` (callback) advances mode_stage on the exact player birthday month/day match and otherwise advances mode_step. The terminal callbacks return through `day_event_next(1)` after audio completion. Search of OMIMAI C found no direct affection/tokimeki/stress/syoushin mutations. This pass therefore establishes event-flow contract only, not relationship rewards or the caller's eligibility rule.

### RENSYU (practice match)

`func_80132040` dispatches one of eight callbacks by mode_stage. `func_801320E0` step 0 sets up a match scene, title, audio load and deterministic seed from timer_ticks; step 1 waits for bank completion; step 2 reads sectors; step 3 waits for CD sync, loads talk tables, then chooses one of five talk groups from club ID 6..10. IDs outside 6..10 reset to title. Steps 4 and 6 speak, step 5 chooses a talk group from the selected club's level nibble +5, step 7 advances stage; stage 8 exits by advancing stage. These branch conditions are body-backed.

`func_80133CB8` is a result path after audio load completes. If `D_801342C4 >= 3`, it increments the selected club's count and adds tokimeki +2 to girls 0..12; otherwise adds guts +10 and stress +10. In either case, if `D_801342D0 != 0xFF`, that girl's affection increases by 2. Then it clears talk, resets brightness and advances the day event. The meaning of `D_801342C4` as a match grade is plausible but not proven by this code, so it is left as a raw threshold.

### VALEN (Valentine's Day; White Day unconfirmed)

`func_80132274` dispatches mode_stage 0..3 and draws the scene/HUD each frame. `func_80132760` is the 56-entry callback dispatcher indexed by mode_step. The ranking/candidate algorithm is `func_80132834` in the same `80132760.c`: if girl 9 is unmet but tokimeki >=20, flag `D_80134544` is set and her affection becomes 50. Top three ranking slots are chosen among met girls with `get_h_tokimeki(i) >= 0x46`, descending by affection+tokimeki with strict `>` comparisons (ties do not replace earlier slots). A second list includes each met girl or that special girl 9 if `get_h_yuukou(i) >= 0x32` (special girl bypasses the yuukou test); it marks each included girl's b8 flag. The list is rotated by frame_count modulo count and rearranged so girl 0 does not occupy an early position when the stated thresholds apply; girl 9 is moved to first when special flag is set. No unsupported tie-break is inferred.

`func_801342E8` restores saved mode_stage and mode_step; other VALEN paths route to player-birthday scenes only if exact birthday day/month, current girl meets tokimeki >=0x46 (with SHIORI using Shiori's `girls[0].tokimeki` instead), and current girl appears among the first three candidates. Valentine's parameter and scene branches are covered here only partially. The ADR's possible White Day content remains explicitly HYPOTHESIS and needs runtime/content confirmation.

## Tables and interfaces

- Main loader contracts are mode + entry address, not implied from folder names. For example DATE2 has two modes and two different entry PCs; OMIMAI has a scene entry plus a common entry; KANGEI has two modes and two PCs.
- KANGEI and RENSYU use callback tables. Their selectors (`mode_step` or `mode_stage`) are exact integer indices; invalid states may dispatch reset callbacks or reset to title, so a port should retain these guards.
- DATE2 returns to DATE using a specific mode/stage/step triple. EN_NICHI returns by restoring its saved caller triple. OLH returns to MODE_COMMAND_SCREEN. OMIMAI and RENSYU complete through `day_event_next(1)` on the observed terminal callbacks.
- Existing tables and global names stay unchanged in this draft; names are proposed separately. Do not commit copyrighted game data.

## Coverage limits

Inspected actual C bodies at loader entries, the central state dispatchers, key result/return functions, and selected state-changing helpers. Not completed: a whole-overlay census of all 7 units; every callback-table arm; every mode_stage and mode_step; all timing/input conditions for game subgames; complete EN_NICHI internal result calculation; all VALEN stat-writing callbacks; OMIMAI caller eligibility and unseen stat behavior outside this overlay; complete KANGEI scene table and meaning. KANGEI's `summer_camp` semantic interpretation and VALEN's White Day possibility remain pending confirmation. The proposal table is a selected evidence set only and must receive independent review before application.
