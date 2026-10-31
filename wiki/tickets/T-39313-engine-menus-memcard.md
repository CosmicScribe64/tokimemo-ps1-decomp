---
id: T-39313
title: Port the engine menu, speaker and memory card files (WP-C)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[tickets/T-39007-player-save-relaunch-recovery]]"]
---

## Goal

Port `menu_speaker_girl.c`, `hud_screens.c` and `memory_card.c` onto the engine, following [[engine-porting-contract]].

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] The core suite, `run_engine_diff.sh --all` and the opening frame-timing gate pass.

## Notes

Per-package notes (WP-C), 2026-10-30.

**Ported:** `engine/memory_card.gd` 40/40 functions of `src/main/memory_card.c`, `engine/hud_screens.gd` 44/44 of `src/main/hud_screens.c`, `engine/menu_speaker_girl.gd` 35/35 of `src/main/menu_speaker_girl.c`. Hand checks: `tests/engine/{memory_card,hud_screens,menu_speaker_girl}_test.gd`. Specs: `config/port/engine-diff/{memory_card,hud_screens,menu_speaker_girl}.json`.

**Captures used** (game data, `.asset-cache/m2-engine/`, never committed; all from real save states, no pokes):
- `wpc-save`: notebook save from `endings-saves-golden-v1/states/select-0528.sstate` with the S1 inputs and the golden card (`MEMCARD_DIR`), 1800 frames; `wpc-load`: the S2 load from `s1-after-save.sstate`, cards `inputs-s2`; `wpc-create`: the NAME_ENT slot-create chain from `build/emu/states/name-slot-pointer-v1.sstate` with blank cards (file_create, file_write, scan polls).
- `wpc-hm-save`, `wpc-hm-load` (same states, hud/menu function lists), `wpc-hm-tel` (`dates-phone-golden-v1/states/tel-shiori-0x45.sstate`, CIRCLE mash), `wpc-hm-phone` (`route-select-008.sstate`, phone book), `wpc-hm-evt` (`evt-player-birthday-entry-v1.sstate`), `wpc-hm2-a/b` (route v6 `month-001/002.sstate`, CIRCLE/DOWN mash), `wpc-hm2-c` (`select-0528.sstate` mash), `wpc-hm2-d` (`restored-title.sstate`, START mash). All with `--ptr-bytes 20`.
- Tooling: `tools/emu/func_capture/run.sh` gained `MEMCARD_DIR` (start with copies of a card directory) and `CONTAINER_PREFIX` (container name prefix, here `wpc`); `tools/port/run_engine_diff.sh` gained `CONTAINER_PREFIX`.

**Decisions and limits:**
- SDK calls (`OpenEvent`, `TestEvent`, `_card_*`, `open/close/read/write/format/delete`, `rsin`, `abs`, `rand`, ...) are called by symbol name with `call_c`; nobody lists them, so they refuse until a backend exists (facade request below). Unnarrowed stub/SDK results are narrowed in the port where the C reads them as signed (`Math.s32`: `open`/`read`/`write`, `rsin`, `rand`, `abs`, `SquareRoot0`, `getCDlevel`).
- Blocking waits: `Hw_Test` and `memory_card_status_wait` spin on card hardware; they are bounded (0x4000 polls) and stop at a refusal; `memory_card_check_poll` step 0x20 calls `func_800AD950` (`VSync(0)`), which refuses by name.
- `hud_save_slot_info_show` calls `strncat(club_name_text, club_name)` with no count in the C (the count register is stale). The port passes 0x20, which equals the original whenever the stale count is at least the club name length; the three captured hits (system arm and card-image arm) match byte for byte.
- `hud_magazine_page_build`: `magazine_issue_index` is uninitialised in the C for years outside 0x5F..0x62 (the port uses 0; not reachable in play).
- `hud_appointment_add` with all eight entries active reads and writes entry [8] (`scene_flags` word 0) as the C does (tested).
- `menu_check` returns `menu_check_2`'s undefined `v0` in menu_mode 2 with L1 held, so its signature carries `ret_undefined` and `v0` is not compared for any `menu_check` hit (RAM still is); `menu_check_2`, `girl_points_others`, `girl_scores` likewise (no return in the C).
- `calendar_month_load`: libgpu queue words (0x800DD680.., 0x800E3450..0x800E3600) are interrupt-timing changes after the replayed `LoadSquare` calls; excluded per function with the reason in the spec.
- Unreached blocks: no `--cover` capture was run; the untested lists in the three specs name every function without a comparable hit and the hand check that covers it. Functions with hits cover the paths the sampled scenes take (for example `hud_cursor_update` 40 hits, `menu_cursor_update` 64 hits in modes 0 and 1; menu_mode 2/3 branches and the `get_near_menu` / `menu_cursor_to_edge` helpers only by hand checks).

## Comments

- 2026-10-30 (worker): memory_card.c done. 40 functions; differential hits per function from the three `wpc-*` captures: Sw_Test 32, Sw_Clear 13, step_next 19, start 10, stop 10, event_handle 24, status_wait 48, check_poll 12, scan_poll 32, system_read_poll 8, system_write_poll 8, save_read_poll 4, save_write_poll 7, file_create 2, file_read 3, file_write 4, file_exists 30, block_clear 9, slot_summary_store 2, system_bank_build 2, system_bank_load 2, game_bank_build 2, bank_validate 4, game_bank_load 1, file_name_build 28, state_reset 2; all pass (`RESULT: PASS`). Untested with reasons in the spec: card_ev_set, Sw_Start, Hw_Start, Hw_Test, Hw_Clear, Hw_Stop, step_set, format, file_delete, file_write_sync, system_file_probe, save_files_probe, bg_still_buffers_set, bg_still_decode_to_vram (hand checks in `memory_card_test.gd`).
- 2026-10-30 (worker): hud_screens.c and menu_speaker_girl.c done (44 and 35 functions). hud_screens: 29 functions with passing hits, 15 untested with a hand check each; menu_speaker_girl: 27 with passing hits, 8 untested with hand checks. Counts per function are in the `run_engine_diff.sh --file <name>` output.
- 2026-10-30 (worker): **Facade requests.** (1) An owner module for the SDK memory-card calls (`OpenEvent EnableEvent TestEvent CloseEvent EnterCriticalSection ExitCriticalSection InitCARD StartCARD _bu_init _card_auto _card_info _card_load _card_clear _card_status open close read write format delete`) backed by `services/memory_card_image.gd` and `memory_card_service.gd` (the port's card image model), with `_card_*`/`TestEvent` as functions of `frame_count`; until then the card paths refuse at the first SDK call. (2) `abs` (and `rand`, `rsin`, `SquareRoot0` if the orchestrator prefers owning them in the facade) as engine functions: the menu and biorhythm code calls `abs` through `call_c`. (3) A signature note: unported or stubbed callees return unnarrowed values (`0xFFFFFFFF` for -1); modules narrow where the C compares as signed. No other facade change.
- 2026-10-30 (worker): **Missing data symbols.** `GameState +0x54C` (the eight appointment words `unk_54C`, `WordView[8]`, used by `hud_screens.c`, `memory_card.c` through `GameSaveImage.appointments`) has no field in `core/game_layout.gd`; the modules use `0x800E6280 + 0x54C` with this comment. `D_800B40F8` and `D_800B419C` (rodata of `menu_speaker_girl.c`, `sndisp`'s 5-byte zero template and a table) are not needed by the port. String literals are addressed by rodata address (listed per module): `0x800AFBF0..0x800AFC7C` (memory_card), `0x800B0F90..0x800B106C` (hud_screens), `0x800AFB70..0x800AFB8C` (menu_speaker_girl); the bytes are in the executable image.
