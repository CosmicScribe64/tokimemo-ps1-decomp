---
type: spec
updated: 2026-10-24
---

# Sports festival overlay (TAIIKU)

TAIIKU is the project's overlay name for `sports_festival` (`GLOSSARY.md`). The code contains scene scripts, race/controller animation systems, talk events, resource loaders, and shared actor/display routines. Labels “A”, “B”, and “C” below refer only to three source clusters; this pass did not establish their in-game titles. The proposal table is deliberately partial: names are made only where the source supports the described operation.

## Concrete scene flow observed

### Folk dance: `80132000.c`

The mode-step switch begins by loading the audio bank, resetting the scene, clearing talk/actors and seeding `rand()` from the game timer. After the bank is ready, it scans all 11 girl records for `flags.f.met == 1`. With fewer than three eligible girls it writes `D_80122ECC = 2`. Otherwise it schedules a background read, selects two nonzero distinct girls, then stores `[0, girlA, girlB]`; on the later path it uses entries 1 and 2 directly. It performs `rand() & 0x1F` swaps among the three entries, including the sentinel. Once the background read completes it initializes the actors/background, starts music and reads the selected girl's portrait. Later steps set talk tables and speaker groups; the group number depends on `girl_state_rank`: ranks 0–1 use `girl * 3 + 5`, ranks 2–3 use `girl * 3 + 6`, and rank 4 uses `girl * 3 + 7`. Exact terminal/success steps later in the function still need tracing.

### Runner source cluster A: `80133C80.c`

`func_80133C80` dispatches `D_8014933C[game_state.mode_stage]`. The setup function `func_80133CFC` resets scene/actors, seeds tpage slots, copies fixed lookup tables, requests sectors `0x1E` and `0x1F`, and waits for both fade completion and CD readiness before installing the next data set and advancing `mode_stage`.

The next observed routine, `func_801345A4`, waits for fade-in, then 0x5B VBLANK ticks, starts audio/bank loading, and enters its active step. The active step initializes runner records, updates runners and actors, and advances a 16.16 position/camera word by `0x18000` each tick. A threshold at high-half `0x140` advances stage. The following routine (`func_801346FC`) begins another phase: it advances the same position by `0x18000` per update, scrolls two display regions, and runs runner/actor updates. At high-half `0x280` it changes actors and prints two table-selected strings; then step 1 runs until `player.unk_01 != 0`; step 2 waits for `func_80137784(0)` to report completion, then applies table-selected X/Y deltas and advances stage. This is concrete phase/camera behavior, but the exact race objective and final winner formula are not proven by this slice.

`TaiRunnerA` is a 0x24-byte record. Header comments directly identify `unk_02` as display actor index, `unk_03` as animation frame, `unk_05` as start-sequence state, `unk_10` as speed, and `unk_20` as 16.16 position. Four runners are stored as one `player + cpu[3]` object. `func_80137784` transitions an actor to an interaction/label position and clears runner fields; it returns 1 when its counter reaches 0x80. `func_80137C44` updates runner display sequences and exposes nearby CPU actors based on position difference. Do not infer a winner from `func_80137EA8` without finishing its body analysis.

### Scene cluster B: `80138950.c`

`func_80138A34` dispatches `D_80149E08[game_state.mode_stage]`. `func_80138AA8` prepares tpage/palette arrays, reads sectors, fades out, installs resources and advances stage. `func_80138DF8` is a different scene: it loads 18 selectable actor tpages, a 0x40-by-0x80 image and five palette resources; after fade-in it initializes actors. During active step 2, `func_80139FE4` scrolls the eleven girl actors whose flags bit 1 is set. Holding `PAD_L1` moves them 4 pixels/frame, otherwise 1; actors that pass x < -0xC0 wrap behind the prior eligible actor at +0x78, clamped to x >= 0xA0. A pressed-button mask `0xF0` plus a hit test against actor 107's point selects an actor whose draw flag is nonzero, stores its index, and starts the selected event transition. The code establishes candidate selection and geometry; it does not establish a sports-scoring formula.

On the later result stage, after fade-out, if `D_801499D4[0] == 0`, all 13 girls gain 4 tokimeki, while the selected girl gains another 2 tokimeki and 10 affection. Else, if `D_801499D4[1] == 0`, all 13 gain 2 tokimeki and the selected girl gains another 1 tokimeki and 5 affection. The code does not explain what the two array conditions represent, so their outcomes stay unnamed.

`TaiikuBall` at `D_80149968` is a contiguous 0x20-byte block, and `func_8013D114` reads high halves from its +0x14 and +0x18 words. That proves fixed-point subfields but not that this is a “ball game” or what it scores. The names in the table avoid the earlier unsupported ball-game claim.

### Scene cluster C: `80142240.c` and `801452D0.c`

`func_80142240` dispatches `D_8014A254[game_state.mode_stage]`. A preceding gate (`func_801422BC`) starts an event only when girl 1 has rank below 2, a specific bit of her flag word is set, and scene flag 13 is clear; it then sets that flag, starts audio, and moves stage. Otherwise it clears the event flag and jumps to stage 3.

The resource setup (`func_80142400`) seeds page tables, loads sectors `0x1E` and `0x4F`, waits on fade/CD readiness, installs decoded/tpage data and advances stage. In its active scene, `func_8014280C` waits for fade-in and accepts any `pad_pressed & 0xF0` as a skip/exit into step 5. Without that input, step 3 runs an audio sequence whose seven substeps alternate between CD audio states 4 and 2, then waits at least `0xB5` VBLANK ticks before restarting the audio cycle. A second button check exits the sequence. This confirms an interruptible timed scene sequence, not its game title.

The later game-C-like controller records at `D_8014A3E4` and `D_8014A400[3]` are distinct allocated objects. Four `TaiikuSlot1C` records at `D_8014A454` correspond to four display actors. In `func_80147A6C`, a zero slot mode, cooldown >= 6, and player gating permit positive/negative motion at `unk_08` to increment/decrement `unk_02`, resetting cooldown. `func_80147F58` compares actor animation IDs with bounded lookup tables and invokes left/right movement routines based on horizontal thresholds. Those routines use slot modes 1 and 2 and set fixed animation/position offsets; `func_801482BC` clears motion state when a nonzero mode's timer reaches 0x65. These are directly observed control/animation contracts; the win condition is not yet established.

## Tables, types and rename coverage

The source currently contains 213 function definitions across 19 C files (census supplied by the parent task). The proposal table retains 15 function rows, 8 type rows, 8 field rows and 7 data rows, plus the O.BIN comment row. The table describes the best-supported names from this bounded inspection; it does not claim complete unit coverage. Most scalar globals and 0x44 display actor fields remain unnamed because their role needs per-use tracing. Type names for fields whose role is structural only remain candidates, not proof of game semantics.

Important structural contracts already supported by `include/ovl/TAIIKU.h` and [[families/taiiku-family-results]]:

- Game-A runners: `TaiRunnersA` is `player + cpu[3]`, total 0x90; array view conversions change address/code generation.
- Four game-B layout records: `TaiRunnerB[4]` has stride 0x24; `unk_08` is speed and `unk_14` is 16.16 position per header comments.
- Game-C control storage: one `TaiikuController` and a separate 3-element array; do not merge them.
- Game-C display slots: `TaiikuSlot1C[4]`, each 0x1C; table boundary labels are used as loop bounds and must remain stable.
- `D_8014A4F8[12]` and `D_8014A528[9]` are texture-page buffer IDs, with the first loaded by `func_80145640`.
- Compact tables in the header have verified copy sizes and element widths, while many meanings are unknown. Keep them typed structurally until their consumers prove names.

## Port design from observed rules

Represent each confirmed scene as a frame-driven state machine. Model CD/fade/audio readiness gates separately from gameplay transitions; they determine when data is safe to use. Keep runner/slot records in the shapes proven above. The useful port contracts already recoverable are the runner scene's fixed position increments and stage thresholds, event candidate scrolling/hit testing, exact girl-stat changes under two result conditions, and the controller slot cooldown/movement transition. Do not encode broader score, placement, or medal rules until the remaining result-state functions are read.

## Coverage census and remaining work

The 213-definition count covers all 19 source files, but this draft has inspected gameplay in detail in `80132000.c`, key state sections of `80133C80.c`, `80138950.c`, `80142240.c`, and slot operations in `801452D0.c`. Other files and functions are not semantically named here. The retained 15 function rows are a partial draft, based on direct inspection of the listed bodies or the cited field-access code. In particular, finish the bodies and call chains that decide runner-scene completion, determine both `D_801499D4` result flags, describe all 0–8 stage handlers in `D_80149E08`, and recover the remaining controller-scene inputs/results. A source census or this spec is not a substitute for reviewing every proposed row.

No runtime/emulator evidence was collected for this draft. No user-facing game text is reproduced; Japanese text is described only by its scene role.

## Proposed source organization (last; do not apply until complete review)

- Folder/header candidates: `src/ovl/sports_festival/`, `include/ovl/sports_festival.h`.
- Candidate semantic object names after per-file classification: `folk_dance.c`, `runner_a.c`, `event_candidates.c`, `event_sequence.c`, `controller_scene.c`, and `actor_tables.c`. These are provisional; original object boundaries remain separate, and the current address files are mixed. The complete file list must be classified before choosing a final basename for each object.
- Keep the disc/overlay identity `TAIIKU` in config. Only the source folder and header use the English name.
