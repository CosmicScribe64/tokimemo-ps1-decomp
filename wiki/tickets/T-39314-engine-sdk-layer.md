---
id: T-39314
title: Give the engine an SDK layer for the PsyQ library calls (M2)
status: Done
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[psyq-sdk]]", "[[ps1-renderer]]"]
---

## Goal

Own the PsyQ SDK calls the game makes, so ported engine code reaches the generic renderer, the vsync and pad model, the memory card service and libc. Today every SDK call refuses by name.

## Acceptance criteria

- [x] libgpu calls used by `src/main` are backed by the generic renderer: primitive setup (Set*/AddPrim and the sorters), GetTPage/GetClut, SetDrawMode, ordering tables (ClearOTag/ClearOTagR/DrawOTag), Load/Store/Move/ClearImage, PutDrawEnv/PutDispEnv and DrawSync.
- [x] VSync and the other blocking waits follow the contract's rule for blocking waits.
- [~] libapi events and the card calls (`_card_*`, `_bu_init`, InitCARD/StartCARD, open/read/write/close/format/delete) are backed by `services/memory_card_service.gd`.
- [x] libc gains `rand` (BIOS LCG as the port already models it), `abs` and the other calls the packages reported.
- [x] Each call has a function-differential test from captures, or is listed as untested with a reason.

## Comments

- 2026-10-30 (worker): SDK layer implemented on `port/m2-engine-sdk`. Modules `engine/_sdk_gpu.gd` (libgpu + two libgs leaves), `_sdk_etc.gd` (VSync, I_MASK), `_sdk_card.gd` (critical sections, BIOS events, card jobs), `_sdk_gte.gd` (rsin/rcos) and the additions to `_libc.gd` (rand, srand, abs, printf, sprintf), registered by the facade. The card file calls (`_card_status _card_clear open close read write format delete`) refuse by name (acceptance criterion 3 is therefore met for events and `_card_info`/`_card_load` only; see Limits). Review against `CODING_STANDARDS.md` is open.

## Notes

Per-package notes (SDK layer), 2026-10-30.

**Facade changes** (`engine/engine.gd`, backward compatible; the packages' modules and the harness run unchanged): the four SDK modules are registered next to `libc`; `engine.memory_card` slot (null until attached); `engine.renderer_ready()` (opens the renderer on first use, false without the GDExtension); `C_SIGNATURES` accepts `"variadic": N` (printf, sprintf): `call_c` pads missing extra arguments with 0. **Harness** (`tests/engine/engine_diff*.gd`, `tools/port/run_engine_diff.sh`): variadic arguments are passed from the capture; a `not_comparable` entry may say `"refuses": "<text>"` (a hit the original served through a path the model refuses by design); the runner attaches a `MemoryCardService` to every engine; `run_engine_diff.sh` mounts the renderer GDExtension (`RENDER_SO`, default the `build-v3` candidate under `.asset-cache/m2-renderer`) read-only at `res://bin/`. New: `tests/engine/sdk_render_check.gd` + `tools/port/run_sdk_render_check.sh`. **Contract**: layout line, the "SDK functions" bullet, the blocking-wait bullet, the services bullet and the spec bullets (`wiki/engine-porting-contract.md`, not its Status section); symbol table in `wiki/psyq-sdk.md`.

**Captures** (game data, `.asset-cache/m2-engine/`, all from real save states, no pokes; `tools/emu/func_capture`, `CONTAINER_PREFIX=sdk`): `gpu1` (evt-player-birthday-entry, 400 frames), `gpu-title` (restored-title, START/CIRCLE inputs, 1500), `gpu-prol` (restored-prologue-v2, 900), `gpu-name` (name-ready-v1, 600), `gpu-school` (original-school-firstline-v1, 900), `gpu-sel` (select-0528, 900), `gpu-tel` (tel-shiori-0x45, 900), `gpu-evt` (evt-player-birthday-mode56, 900), `gpu-sp1` (route month-001, 3000), `gpu-sp2` (original-school-firstline, 2400), `gpu-sp3` (seasonal june01-start, 3000, the one sprintf hit); cards: `card-save`, `card-load`, `card-create`, `card2-load`, `card2-save` (the endings-saves and name-slot states with the WP-C inputs, blank cards, TestEvent keyed on the event-1 status byte). Frame captures for the VRAM check: the T-39302 set `.asset-cache/m2-renderer/f-*`.

**Results** (`CONTAINER_PREFIX=sdk sh tools/port/run_engine_diff.sh --all`: `RESULT: PASS`; hits passed / total, bytes compared in the spec output):
- libgpu (`sdk_gpu`): AddPrim 144/144, SetSemiTrans 90/90, SetShadeTex 88/88, SetPolyFT4 (F040) 10/10, SetPolyG4 (F054) 53/53, SetPolyGT4 (F068) 5/5, SetSprt16 (F090) 51/51, SetSprt (F0A4) 46/46, SetTile1 (F0B8) 56/56, SetTile (F0F4) 5/5, SetLineG2 (F11C) 34/34, GetTPage 184/184, GetClut 176/176, SetDrawMode 112/112, ClearOTagR 68/68, DrawOTag 68/68, ClearImage 54/54 (aligned and unaligned lists), ResetGraph(1) 30/30, SetDispMask 40/40, PutDrawEnv 66/66, PutDispEnv 66/66, DrawSync 116 (106 pass, 10 refused as the spec expects: the capture's queue held a job), LoadImage 80 (66 pass, 6 not comparable and 8 expected refusals: the original queued the call behind a running DMA, hardware state not in RAM), MoveImage 18 (16 pass, 2 not comparable, same reason).
- libetc/libgte/libc: VSync 70 (69 pass: mode -1; 1 expected refusal: VSync(0)), rsin 42/42, rcos (func_800A0140) 6/6, rand 40/40, srand 1/1, abs 10/10, sprintf 1/1, printf 6 (5 pass, 1 not comparable: a VBlank interrupt ran inside the BIOS call).
- BIOS: TestEvent 615/615, EnterCriticalSection 2/2, ExitCriticalSection 2/2, `_card_info` 4/4, `_card_load` 4/4.
- `tools/port/run_sdk_render_check.sh`: DrawOTag through `engine.call_c` over the 26 T-39302 frames: 24 exact over the whole VRAM, the 2 phone frames differ in 1 pixel (the renderer's documented interpolation rounding). `RESULT: PASS`.
- Gates: core suite `PASS: 11 test suite(s) ran`; signature check `153 function signature(s) checked, 0 error(s)`; opening frame timing `opening frame timing: pad 2, 104 substeps, 6709 frames to 12/0/0, 0 mismatches`.

**Untested (reason in the specs, hand checks in `tests/engine/sdk_gpu_test.gd`, `sdk_misc_test.gd`):** AddPrims, the initialisers no scene reached (F3, FT3, G3, GT3, F4, Sprt8, Tile8, Tile16, LineF2, LineF3, LineG3, LineF4, LineG4), ClearOTag, StoreImage, DrawPrim, GsDrawOt, GsSortClear (no hit in the sampled states; all are straight reads of the asm). OpenEvent/EnableEvent/CloseEvent/InitCARD/StartCARD/_bu_init/_card_auto run once at boot, before every available save state (the event table layout is confirmed by the captured RAM). The refused card calls have no model to test.

**Decisions and limits**
- Timing model: the renderer draws before `DrawOTag`/`LoadImage`/... return, so the library's queue is always empty and the DMA idle; a captured hit where the original found the DMA busy (a job queued, hardware state not in RAM) is listed `not_comparable` or `refuses` with that reason. `DrawSync(0)` returns 0 after refreshing the timeout words. This is the contract's blocking-wait rule applied (the condition is true by construction), not a wait skipped.
- `VSync(-1)` reads the vblank counter word `0x800E35E0`; nothing in the port advances it (only the VBlank interrupt does), so the draw-timeout words derived from it stay as they are. `VSync(0)`, `VSync(n)`, `VSync(1)` refuse naming `func_800AD950`. `GetRCnt` (`func_80098650`) and `ResetRCnt` (`func_8009ABF0`) read and reset the hardware root counter: not a function of frame_count and memory, refused; `frame_render_audio` calls `func_80098650` at the start of every frame, so the render loop stops there until a timing model exists.
- I_MASK (saved by the library around queued work) is a hardware register: the model takes the first value from the library's own saved copies (0x800DD694/698/69C), so a captured RAM image carries it.
- ClearImage's unaligned branch restores the draw area from the library's own copy of the last DR_ENV (0x800DD5B8..), not from the renderer's environment (a fresh renderer has none), which is what the GPU info registers hold when the area was set by PutDrawEnv.
- `rand`/`srand` keep the seed in kernel RAM at the OpenBIOS address `0x8000850C` (every capture confirms the single changed word); the LCG is `core/native_rng.gd` used as a function (no second state). The retail BIOS keeps its seed elsewhere in kernel RAM (unobserved).
- `printf` returns 0 and writes nothing: the BIOS printf writes only kernel/console state (the captures show no game-memory change except interrupt-driven timers in one long call) and the return value (character count) is discarded by every caller (`ret_undefined`).
- Card: only `_card_info` (2 frames) and `_card_load` (72 frames) are modelled, as SwCARD IOE events delivered when a poll asks, with the numbers measured once each on OpenBIOS with a blank card (capture `card2-load`); a no-card slot refuses (unobserved). Everything else about the file system was observed only on blank cards (`open` fails for ~100 frames and then returns descriptor 2, `_card_status` answers 2 or 4 every frame, file jobs take hundreds of frames), so `_card_status _card_clear open close read write format delete` refuse by name. A capture with a formatted golden card and the success flows (save, load, delete, format) is needed to model them.
- Pad: none of the three packages calls a libetc/libapi pad function (`pad_update` is a game function in a file nobody has ported), so no pad model was needed or added.

**Left** (still refusing by symbol name): libgs object sorters and helpers `func_80098680 func_80098B80 func_80098D00 func_80099540 func_80099B10 func_80099ED8 func_80099F04 func_8009A200 func_8009A4D0` (GTE transforms), `GsSwapDispBuff` `func_8009AC20` (needs `func_80098380`, a GTE offset write), `func_80098650`/`func_8009ABF0` (root counter), libspu/libsnd `func_80094B80 func_80094C60 func_80094C90 SsVabTransCompleted`, libcd `func_8008B750`, libpress/MDEC `DecDCTReset func_80086870..func_800869C8`, and the card calls listed above. `debug_print_string` and `exit_function` are game functions, not SDK.
