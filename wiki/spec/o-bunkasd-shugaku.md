---
type: spec
updated: 2026-10-24
sources: ["src/ovl/BUNKA_SD/80132000.c", "src/ovl/BUNKA_SD/80133540.c", "src/ovl/BUNKA_SD/801369D0.c", "src/ovl/BUNKA_SD/80137B40.c", "src/ovl/SHUGAKU/80132000.c", "src/ovl/SHUGAKU/80133D60.c", "src/ovl/SHUGAKU/80134880.c", "src/ovl/SHUGAKU/80136380.c", "src/ovl/SHUGAKU/80138A60.c", "src/ovl/SHUGAKU/801378B0.c"]
---

# BUNKA_SD stage shows and SHUGAKU school trip

This is a body-backed partial port specification for these two overlays. Prefixes are confirmed by `docs/adr/0001-phase-2-naming.md`: BUNKA_SD `stage_show_`; SHUGAKU `trip_`. Names in the proposal TSV are proposals only. O.BIN names remain unchanged. Overlay-specific narrative and outcome semantics are hypotheses unless the C bodies directly establish them.

## BUNKA_SD: stage-show state flow

`func_801322EC` is the per-frame input wrapper. While `mode_step` is 5..14, held Circle triggers the scene handler and marks ten display actors (records 96–105) with state `0x82`; otherwise it writes state `1`. Held Cross in steps 5..13 issues audio commands and sets step 13. Then the wrapper calls `func_80132428` (source `80132000.c`). This proves input gating, but not the game's intent for Circle or Cross.

`func_80132428` dispatches mode steps 0–15 and increments `mode_step_timer` every frame. Step 0 starts scene initialization; 1 waits at least `0x1F` timer ticks; 2 waits for audio-bank and CD sector loading; 3 selects a script; 4 initializes actors and speed; 5/14 run fades; 6 and 12 perform frame-dependent handlers; 7/9/11 run specialized scene helpers; 8/10 dispatch helper durations 15/30; 13 resets actors and advances; 15 evaluates a completion predicate. Any other step resets to title. Source: `80132000.c`.

At step 3, `func_80132148` switches on `D_80122EAC` (0..4). Selector 0 chooses script pointer `D_8013B74C`; selectors 1–4 choose paired pointers and set `current_girl` respectively to SOTOI, SHIORI, MIO, MIHARU; those arms also configure audio requests. The function always advances the mode step. The selector's actual show/story identity is not established by the code alone.

Three scene wrappers have related structures: A `func_8013376C`, B `func_80136BFC`, C `func_80137D6C`. They route script/event execution through the corresponding interpreter `func_80133D54`, `func_801373D0`, `func_8013842C`; scene A includes mode steps 4–11, 15–18, 20–36. Script data is indexed from scene-specific event-list/text tables. The interpreter checks the current frame against each `BsdEvent.unk_00`; `0x0FFFFFFF` terminates a list. Command low byte `0xFE` calls the display operation with hide parameters; other values are image IDs. Bits 16–23 select the actor slot and bits 28–31 gate text startup. It advances event/frame cursors and can advance the outer step when the list completes. Sources: `80133540.c`, `801369D0.c`, `80137B40.c`; shared type evidence in `wiki/families/bunka-sd-family-results.md`.

The scene loaders initialize display actors 96 onward, load palettes and texture pages, and bind pointer tables for the scene pictures. The type-family page establishes these are picture-set pointers and BsdEvent lists. Exact image-to-actor meaning, script identities, scoring/reward effects, and any affection/tokimeki changes are not demonstrated in the inspected dispatcher/interpreter bodies. No BUNKA_SD relationship-stat mutation was found in these primary flow bodies; that is not a full-overlay negative census.

## SHUGAKU: school-trip state and outcome evidence

`func_80132274` copies callback table `D_8013B094` and invokes the callback selected by `mode_step`. The broader trip has multiple scene families: `func_80132364` sets one set of current pointers and runs `talk_run`; `func_801326F8` switches to another pointer triplet; `func_8013282C` selects text/table pointers by `current_girl`. The callback lists are shared types documented in `wiki/families/shugaku-family-results.md`.

`func_8013282C` maps girls to text/table pointer triplets, calls `talk_names_set(current_girl)`, and selects a year value using `year - 0x5F` when month >= 4, else `year - 0x60`. For NOZOMI only, status `(get_g_zyotai_s(NOZOMI) & 0x7F) < 2` substitutes another pointer triplet. This is body evidence for route-specific dialogue selection, not proof of the narrative content or route rules that produced the current girl/status.

`func_8013243C` gates a birthday scene: if the player's current month/day does not match `player_birthday`, it advances the step and returns. On the birthday, it scans ten girls whose `D_800CA2B0[i]` flag is nonzero, choosing the lowest low-seven-bit status; equal status breaks by greater sum of tokimeki and yuukou. If the chosen girl's status is below 2 and the girl index is 0, it selects mode step `0x32`; otherwise it advances normally. The status and array meaning are only described by direct uses here; exact event meaning remains unverified.

`func_80132C64` branches on `D_80122CDC`: nonzero subtracts one affection and adds five syoushin for `current_girl`, clamps parameters, and adds `0x1B` to mode_step; zero increments `D_8013B084` and advances the step. `func_80132D14` adds 2/4/6 to that dialogue/script index for current-girl status values 2/3/other when status >=2, otherwise no offset; it then advances. These are explicit state/flow effects, but the response/status meanings are not proven.

`func_801330E8` is a direct response-stat transition indexed by `D_80122CDC`: choice 0 adds tokimeki +3, affection +2, syoushin -15; choice 1 adds tokimeki +2, affection +1, syoushin -10; every other value adds affection +1 and syoushin +10. It calls `check_para_limit()` and advances the step. `func_80133348` has special saved-stage/current-girl/status branches into mode stages 8, 0xB, 0xA; exact story/result labels for those branches remain HYPOTHESIS. `func_80132BCC` restores saved stage/step unless saved mode stage equals 6, which instead dispatches through `func_8013ABAC`; `func_80132C1C` adds six to mode_step in that saved-stage-6 case.

## Tables and identifiers

SHUGAKU has three sets of 13-word location pointers and three callback families; the established family page notes that many apparent 12-byte arrays are unrelated scalars and should not be merged into guessed structs. BUNKA_SD uses three event-list pointer arrays of 29, 12, and 31 entries, plus per-script text data. Existing raw symbol spellings are preserved in this analysis. No game text is reproduced.

## Coverage and open work

Source-body census: BUNKA_SD has 98 defined functions and SHUGAKU has 182, for 280 total (root's Docker-verified body count; earlier grep included two prototypes). This pass inspected the entry/dispatch and principal scene, event, selector, and outcome bodies listed above; it did not audit all 280 functions or all globals/callback targets. Not covered: full BUNKA_SD A/B/C event tables and all helper semantics; complete SHUGAKU callback-table target census and every location scene; route-entry conditions and exact companion selection; the relation between stages and story outcomes; emulator validation of uncertain mode/status meanings. There is no body-backed evidence here for general score changes, a full school-trip companion selection algorithm, or BUNKA_SD affection/tokimeki consequences. Runtime work using existing save states/debugger traces is authorized by the project phase-2 scope and is a next step for those questions.
