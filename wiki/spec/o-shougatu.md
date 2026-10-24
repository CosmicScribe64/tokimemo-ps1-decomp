---
type: spec
updated: 2026-10-24
sources: ["src/ovl/SHOUGATU", "src/ovl/GYOZI", "include/ovl/SHOUGATU.h", "include/ovl/GYOZI.h", "wiki/families/shougatu-family-results.md", "wiki/families/event-family-results.md", "config/renames/proposals/o-shougatu.tsv"]
---

# SHOUGATU and GYOZI seasonal overlays (port spec)

Phase 2 analysis for [[tickets/T-27033-p2-o-shougatu]]. The names in the proposal table are drafts. SHOUGATU's disc name maps to `new_year_christmas`, prefixes `new_year_` and `christmas_`; the map still says **confirm**, so the folder/name split is a code-based proposal pending runtime content confirmation. GYOZI maps to `annual_events_unused` / `annual_`, also pending a loader search. O.BIN symbol spellings remain unchanged; their C definitions need concise English comments.

## Evidence and limits

The matching C is the source for control flow and data layout. `wiki/families/shougatu-family-results.md` and the linked T-17500/T-240xx tickets record successful sibling ports, typed table evidence and source-only experiments. No emulator run is cited for the event choices in this spec. The seasonal names describe the overlay classification supported by the ADR and source clusters; exact story text, date gates, character-selection rules and scene order remain hypotheses until runtime or stronger data evidence establishes them. The old family notes retain historical identifiers; the shared core has applied names, so consult current `include/game.h` and `include/main_api.h` for shared fields and functions.

## Module boundary and shared-main contract

SHOUGATU's entry/handler code owns its overlay mode stages, scene actor records, dialogue setup and seasonal resource tables. It calls the main executable's talk system (`talk_run`), HUD/date drawing, portrait updates, input, graphics/display and game-mode reset helpers. The main talk contract is set up by assigning the overlay's talk table pointers and then calling `talk_run(script, mode, flags)`; those globals are shared global state, not private script data. `func_801322D4` performs overlay stage dispatch and per-frame presentation work, so the caller must first select/load this overlay and maintain the game's common mode/stage state.

The code initializes many scene/resource addresses in `D_80143A40..D_80143AF0`; treat them as address scalars, not one presumed contiguous record. T-17500 found that nearby address runs are read individually by matched GYOZI siblings, and that auto-recovered C-stride arrays often represent copied function-pointer step tables instead of records. Existing shared-core names are already applied; no stale names from core proposals should be used in new headers.

## State machine

SHOUGATU's main dispatcher reads `game_state.mode_stage` and switches values 0 through 6 to seven step-table handlers (`func_80132550`, `func_80132D00`, `func_801330C0`, `func_80133D10`, `func_80134120`, `func_80134930`, `func_80136078`). Invalid stages reset to title. Every frame it also updates portrait wink, scrolling, dialogue display, date display and HUD layer 2. Each handler indexes a stage-specific function-pointer table with `game_state.mode_step`; some receive `0x80` or `0x40` as an argument. Preserve that dispatch and presentation ordering. The code inspected does not prove the external scheduler/loader entry into this overlay, so its call-in contract remains open.

The source evidence gives several concrete stage transitions and gates:

- Stage 0 (`func_80132550`) loads the seasonal bank/resources, initializes the New Year scene and resets shared stage data. It snapshots met flags for girl indexes 0–10. A separate selector (`func_8013288C`) scans met girls, chooses the lowest rank (`get_g_zyotai_s(i) & 0x7F`), ties by the *larger* affection+tokimeki sum, and chooses that girl only when rank is below 2. Otherwise it moves to `mode_stage` 2. The selected girl's three dialogue resource pointers are selected separately; Nozomi rank below 2 changes her set.
- In stage 1, the outcome updater (`func_80132E8C`) uses `D_80122CDC` as a branch condition. Nonzero decrements the current girl's affection and tokimeki by one and adds 20 to syoushin; when zero, it increments tokimeki only if `current_girl` is Shiori. Source does not identify the flag's gameplay meaning. The current girl's portrait/resource selection is explicit by index.
- Stage 3 includes shrine-visit dialogue initialization (`func_80133D98`): selector state depends on `D_80143B20` and `D_80143B18`, with an extra Yukari variant. Seasonal scene code writes the shrine-visit title label (text omitted). The inspected code does not establish the scheduler date gate that launches the scene.
- A separate actor sequence in the same overlay performs omikuji: it displays the fortune-drawing prompt, moves the actor horizontally with acceleration bounded at 20, transitions through a result animation, and selects sequence slot based on year values 0x61 and 0x62. The table-to-result meaning is unknown.
- The Ijuin-mansion initialization checks whether Shiori's birthday or the player's birthday is Dec 24 and whether the corresponding done bit is clear. It sets the bit and switches to girl-birthday or player-birthday mode. This is a proven Dec 24 gate; the source does not show a Christmas-party rule in that function.

Nearby scene handlers around `0x8013Dxxx` share helpers such as `func_8013D2A8`, `func_8013D324`, `func_8013D3B4` and `func_8013D5D8`. Their exact transition predicates and return-value meanings need a focused line-by-line pass; no transition graph more specific than the dispatcher is asserted here. `GameState.mode_stage` and `mode_step` are reused with mode-specific meaning. Do not treat `stage` as an unqualified identifier stem; glossary names the field `mode_stage`.

## Shrine/Christmas selection and scoring

The source of `func_801350F8` identifies an omikuji fortune-drawing scene: it writes prompt/result dialogue, animates the fortune actor, and chooses a result animation sequence with a year-sensitive expression (`year == 0x61` or `0x62`). That is direct code evidence for the scene role. The exact year-specific meaning of the animation ids and the result text table is not established. Stress effects are documented below; any additional effects on other parameters or girl relationships remain unverified.

The inspected evidence does not establish the calendar gate that selects the New Year shrine visit or any Christmas-party eligibility/selection rule. The December 24 birthday dispatch is a different proven gate. The girl selector's rank threshold and tie-break are explicit as above, but the code inspected does not prove they are the calendar-scene eligibility rule or a general points formula. `func_801359FC` computes a selector from `frame_count & 7`, subtracting one when nonzero; Ayako's result 3 is remapped to 4. It copies that row from `D_80143E80`; selector 0 lowers stress by 5, selector 4 raises stress by 5, and `check_para_limit` follows. No runtime confirmation is available here. Keep other score rules opaque.

The per-girl affection, tokimeki, rank and points systems belong to shared core. This overlay may read those shared values through the currently applied GameState/API names, but this analysis found no evidence tying a numeric score calculation to shrine or Christmas selection. Port logic should keep any such gate opaque until the exact reads and comparisons are mapped.

## Data tables

- **Step dispatch tables:** dozens of `FnTblNN` tables in `include/ovl/SHOUGATU.h` hold function pointers copied/used by scene step handlers. T-17500 supports them as function-pointer tables and warns that their three-word copy loops mislead stride-based type recovery.
- **Text choice rows:** `D_80143E5C` and `D_80143E80` are two 7-by-5 byte string rows; `func_801359FC` passes selected five-byte rows to the text copy helper. Exact Japanese content stays in the binary/raw sources and is not reproduced here.
- **Scene parameters:** six seven-halfword tables (`ShoShorts7`) hold values including thresholds/timing-like magnitudes and selector values; source evidence proves layout and values, not all semantics.
- **Rendering tables:** `ShoColors6x2` color pairs and `ShoBytes3x12` byte rows are copied by `func_80140314`; the byte table is indexed by a shared selector. Treat semantic labels as open pending reader-by-reader analysis.
- **Particle/work arrays:** four 50-entry work arrays and separate byte counters support the `func_80141D20` helper family. T-24031 records a position reset involving Fix32 halves and real `Rec24` indexing; the production paths remain assembly-backed where matching is not proven.
- **Resource address scalars:** `D_801464B0..D_8014650C` and `D_80143A44..D_80143AFC` remain scalar addresses where consumers read individual words. Do not fold these into guessed structs.

## GYOZI relationship

GYOZI has the same seven-way dispatcher shape (`func_801342A4` uses `D_800F6479` for mode stage and `D_800F647A` for substep dispatch) and a parallel candidate selector. Its selector scans met girls and minimizes rank; on a rank tie it selects the **larger** sum of the two score fields. It only enters the selected-girl branch below rank 2. `func_80134938` selects three dialogue table addresses and changes girl index 6's table set below rank 2. Its outcome function decrements two target-girl fields and increments a third by 20 when the branch flag is nonzero; when the flag is zero it increments the second field only if the selected target index is 0. The legacy GYOZI record layout is not yet mapped well enough to assert those field names mean affection/tokimeki/syoushin.

GYOZI's omikuji twin (`func_801368B8`) has the same visible fortune prompt and actor pattern, but its result sequence uses `D_800F53DE` relative to 0x5F rather than SHOUGATU's explicit comparisons with 0x61 and 0x62. Do not copy those thresholds into GYOZI. Its Ijuin-mansion setup independently checks Dec 24 birthday gates for the target girl's stored birthday and the player birthday, sets separate done bits and switches to the birthday modes. Verified/documented sibling pairs include SHOUGATU `func_80132A34` / GYOZI `func_80134938`, `func_80133760` / `func_80135518`, `func_801350F8` / `func_801368B8`, and `func_80133208` / GYOZI `func_80135280`. A sibling is evidence for shared mechanics, not proof names/game meaning are identical. Use GYOZI's ADR prefix `annual_` and preserve its `_unused` folder hypothesis until loader inspection is repeated.

## Proposed files and headers

When file renames are applied in a later ordered batch, place content under `src/ovl/new_year_christmas/`, with files grouped by function: `mode_dispatch.c`, `new_year_scene.c`, `christmas_scene.c`, `scene_text.c`, `scene_actors.c`, `particles.c`, and `resources.c`. Split `include/ovl/SHOUGATU.h` into topic headers such as `include/new_year_christmas/scene.h`, `dialogue.h`, `actors.h`, and `resources.h`; avoid one monolithic overlay header. Exact split depends on full symbol ownership review. GYOZI later gets `src/ovl/annual_events_unused/` and `include/annual_events_unused/`, reusing sibling names under `annual_` only where its own evidence supports them.

## Open questions

1. Confirm the actual mode entry points and overlay loader at runtime; resolve the ADR's SHOUGATU `confirm` marker and GYOZI `confirm unused` marker.
2. Trace `mode_stage` transitions and `mode_step` substeps for each stage, then document entry/exit conditions and frame waits.
3. Identify calendar/date gates for shrine and Christmas scenes, including all eligibility and girl-selection rules; do not infer these from overlay names.
4. Trace the score/affection reads, if any, in each selection path; record the comparison sites and formula before naming a score rule.
5. Map data table consumers and exact timing/render meanings; preserve unknown tables until all readers are covered.
6. Reconcile each near-copy pair while keeping per-overlay names and documenting true behavioral differences.

## Coverage census (this pass)

The repository contains 400 SHOUGATU function definitions in 27 C files and 417 GYOZI definitions in 33 C files (root census). This pass read the two primary mode dispatchers, candidate selectors, relevant score branches, SHOUGATU shrine dialogue setup, SHOUGATU/GYOZI omikuji routines, and both Dec 24 birthday dispatches. The proposal table names 9 SHOUGATU definitions and 6 GYOZI definitions; this is deliberately a primary-flow slice, not full function coverage. The rest of both overlays—including several mode-step tables, event sub-scenes, resource readers, rendering helpers, and the exact external entry/loader rules—remains unreviewed. The tables section contains layout/use facts collected in the earlier family type pass, not a current exhaustive consumer audit. No emulator checks were run in this pass.
