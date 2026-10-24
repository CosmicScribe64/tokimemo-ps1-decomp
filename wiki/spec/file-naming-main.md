---
type: plan
updated: 2026-10-24
ticket: T-27059
sources: ["config/SLPM_86.053.yaml", "config/objects/main.txt", "config/renames/main-files.txt", "config/renames/proposals/layout-main-all-files.tsv"]
---

# Main C filenames

Implementation batch for [[tickets/T-27059-all-main-c-file-names]], following the user's requirement that game-code C files have understandable names. The main executable has **34 original C objects, now all moved to logical basenames**. The table below preserves the pre-application census and evidence. The exhaustive supported file-only table is `config/renames/proposals/layout-main-all-files.tsv`. These are concrete moves for application, not optional placeholder names. No object is split, merged or reordered.

## Census and precedence

The current main YAML C entries, 34 object rows in `config/objects/main.txt`, and all 34 filesystem `src/main/*.c` paths align exactly. The census uses the YAML main-segment ROM/VRAM mapping and the original object metadata. The applied `config/renames/main-files.txt` wins: `scheduler.c` (0x8006CB30) and `school_day.c` (0x800737A0) already exist and receive no rename row. No remaining source file is omitted. SDK assembly, overlays, and asset names are outside this main-only table.

The earlier seven reviewed file rows remain unchanged in their original tables; this batch reuses their exact destination stems. The 247 reviewed symbol rows remain frozen and are not bundled here. The module/type map in [[spec/header-split-plan]] and prior proposals/specs supplied candidates; actual matched C definitions decide their roles. In particular, 80079B10 is a sound driver, 80056300 reloads the executable, and 80059A20/80059B40 have proved debug-display/boot-assets roles.

## Exhaustive mapping

End addresses are exclusive. Definition counts from tools/srcscan.py are inventory aids, not matching proof. Applied means the current path and ledger already agree. Proposed in the historical table means a row in the new table; root has now applied all32 rows.

| Original text range | Current C basename | Final C basename | Definitions | State / body evidence |
|---|---|---|---:|---|
| `0x80041000-0x800420D0` | `80041000.c` | `game_init.c` | 15 | Proposed: Original object 0x80041000-0x800420D0: gpu_init/display environment setup, VRAM clears, then GameState/player/calendar/girl/appointment/event initializers and sprite/background resets. |
| `0x800420D0-0x80042540` | `800420D0.c` | `main_loop.c` | 9 | Proposed: Original object 0x800420D0-0x80042540: executable entry func_800420D0 and boot_loop perform callback/system init, repeated game startup, frame polling and per-frame dispatch. |
| `0x80042540-0x80042A00` | `80042540.c` | `game_mode.c` | 8 | Proposed: Original object 0x80042540-0x80042A00: game_mode_dispatch covers mode handlers; remaining functions advance or set mode/stage/step and request overlays. |
| `0x80042A00-0x80043510` | `80042A00.c` | `pad_input.c` | 10 | Proposed: Original object 0x80042A00-0x80043510: detects and initializes pad/mouse, polls controller state, maps buttons, handles turbo/pad code, pause and soft-reset sequences. |
| `0x80043510-0x800451D0` | `80043510.c` | `frame_render_audio.c` | 26 | Proposed: Original object 0x80043510-0x800451D0: draw_frame/draw2d3d render 2D/3D and present frames; same object queues palette/texture uploads and loads/polls/starts sound banks from CD. |
| `0x800451D0-0x80046500` | `800451D0.c` | `cd_queue.c` | 23 | Proposed: Original object 0x800451D0-0x80046500: CD command ring/pump, queued sector reads and checksum, read/audio state and XA clip requests. |
| `0x80046500-0x80047550` | `80046500.c` | `reset_iris_wipe.c` | 5 | Proposed: Original object 0x80046500-0x80047550: reset-to-title plus decimal-digit/packed-BCD helpers and 4-bit iris/radial wipe texture generation and draw setup. |
| `0x80047550-0x800490C0` | `80047550.c` | `actor_sprites.c` | 21 | Proposed: Original object 0x80047550-0x800490C0: screen shake, actor-pool sprite drawing/clearing, texture-page buffer allocation/cache, actor reset and screen-brightness update. |
| `0x800490C0-0x80049FF0` | `800490C0.c` | `gpu_primitives.c` | 9 | Proposed: Original object 0x800490C0-0x80049FF0: primitive work-area allocation, FT4 sprite and tpage/environment primitives, pulsing light triangle and Gouraud gradient-box drawing. |
| `0x80049FF0-0x8004AD60` | `80049FF0.c` | `gpu_effects.c` | 7 | Proposed: Original object 0x80049FF0-0x8004AD60: gradient-line drawing, background-layer setup, radial/disc effects, textured/tile circle effects and full-screen gradient fades. |
| `0x8004AD60-0x8004C3F0` | `8004AD60.c` | `debug_hud_palette.c` | 13 | Proposed: T-27059: object [0x8004AD60,0x8004C3F0); debug-number overlay and level control plus color-bar and sepia palette helpers; retain the mixed original object (prior h-graphics candidate) |
| `0x8004C3F0-0x8004E500` | `8004C3F0.c` | `screen_layers.c` | 11 | Proposed: T-27059: object [0x8004C3F0,0x8004E500); screen-page rendering, SRN layer VRAM setup, MOD tile-set upload and glyph fetch; preserve all functions and original object boundary (prior h-graphics candidate) |
| `0x8004E500-0x8004F6B0` | `8004E500.c` | `text_system.c` | 25 | Proposed: T-27059: object [0x8004E500,0x8004F6B0); text list/typewriter, glyph lookup and upload, text palette and per-frame glyph drawing; preserve complete object (reuse reviewed m-menutext filename) |
| `0x8004F6B0-0x8004F870` | `8004F6B0.c` | `vram_viewer.c` | 1 | Proposed: T-27059: object [0x8004F6B0,0x8004F870); one mode-0xFE developer viewer scrolls display origin by 16-pixel steps from pad 2 and restores the original video area on exit (reuse reviewed m-menutext filename) |
| `0x8004F870-0x80053650` | `8004F870.c` | `menu_speaker_girl.c` | 35 | Proposed: T-27059: object [0x8004F870,0x80053650); one mixed object for menu/choice navigation, speaker labels and talk-text/voice steps, girl name/score/state queries, birthday and club queries; do not split (reuse reviewed m-menutext filename) |
| `0x80053650-0x80056300` | `80053650.c` | `memory_card.c` | 40 | Proposed: T-27059: object [0x80053650,0x80056300); BIOS memory-card events, card polling/file jobs and system/game save images dominate; first two helpers set/decode MDEC still-image buffers, so keep the complete original object (reuse m-flow candidate) |
| `0x80056300-0x800563F0` | `80056300.c` | `reload_executable.c` | 1 | Proposed: T-27059: object [0x80056300,0x800563F0); its sole O.BIN function exit_function shuts down CD/SPU/timers/callbacks and calls LoadExec for PSX.EXE; preserve O.BIN function name. This is not STR playback; use full executable rather than prior h-cdio abbreviation reload_exe |
| `0x800563F0-0x80057390` | `800563F0.c` | `movie_player.c` | 11 | Proposed: T-27059: object [0x800563F0,0x80057390); STR movie setup/playback, double-buffered MDEC decode/sync, CD reads, frame loop and timed subtitle text; preserve complete object (reuse h-cdio candidate) |
| `0x80057390-0x80058D20` | `80057390.c` | `background_images.c` | 13 | Proposed: MDEC still-background image cache and decode/upload, brightness/draw selection and screen capture; current dec_bg_* helpers implement the picture pipeline. New filename uses English words rather than the unlisted dec abbreviation. |
| `0x80058D20-0x80059710` | `80058D20.c` | `model_3d.c` | 12 | Proposed: T-27059: object [0x80058D20,0x80059710); 3D view, light and coordinate setup; TMD model-data linking, model slots, display sizing and model updates (reuse h-cdio candidate) |
| `0x80059710-0x80059A20` | `80059710.c` | `mouse.c` | 7 | Proposed: T-27059: object [0x80059710,0x80059A20); mouse-port sample buffers, state/read, sensitivity and range setup, scaled cursor movement and bounds (reuse h-cdio candidate) |
| `0x80059A20-0x80059B40` | `80059A20.c` | `debug_display.c` | 2 | Proposed: Displays mode/stage/step and associated timers, timer_ticks, substep and GetSp with debug-number drawing; second helper prints diagnostic label D_800AFDF0 and marks a work-queue flag. |
| `0x80059B40-0x8005A0B0` | `80059B40.c` | `boot_assets.c` | 6 | Proposed: Boot-stage state machine reads common assets from CD, relocates texture pointers, loads tpage cache, initializes palettes/UI actors, and proceeds toward title setup; retains all six original definitions. |
| `0x8005A0B0-0x80061710` | `8005A0B0.c` | `home_screens.c` | 109 | Proposed: T-27059: original object [0x8005A0B0,0x80061710); command/calendar/card/settings screens plus club, rumor and Christmas/New Year prep; reuse prior reviewed m-sysmenu recommendation. |
| `0x80061710-0x80062CD0` | `80061710.c` | `title_screen.c` | 21 | Proposed: T-27059: original object [0x80061710,0x80062CD0); builds and links the 3D title models, loads the title scene and handles title selection/start flow. |
| `0x80062CD0-0x800674B0` | `80062CD0.c` | `hud_panels.c` | 37 | Proposed: T-27059: original object [0x80062CD0,0x800674B0); portrait/bustup, parameter digits, calendar date, message window and command-icon panel implementations. |
| `0x800674B0-0x8006CB30` | `800674B0.c` | `hud_screens.c` | 44 | Proposed: T-27059: original object [0x800674B0,0x8006CB30); calendar/month view, magazine, save/load panel, appointment and club screens, command roll and shared HUD helpers; reuse prior reviewed m-hud name. |
| `0x8006CB30-0x800737A0` | `scheduler.c` | `scheduler.c` | 56 | Applied: main-files ledger and current YAML/source path agree. |
| `0x800737A0-0x80075320` | `school_day.c` | `school_day.c` | 19 | Applied: main-files ledger and current YAML/source path agree. |
| `0x80075320-0x800789E0` | `80075320.c` | `holiday.c` | 41 | Proposed: T-27059: original object [0x80075320,0x800789E0); mode 0x46 holiday menu, activities, phone, magazine, appointment and club flows; reuse reviewed m-holiday name. |
| `0x800789E0-0x80079B10` | `800789E0.c` | `overlay_loader.c` | 12 | Proposed: T-27059: original object [0x800789E0,0x80079B10); dispatches/loads/verifies overlays from sector and entry tables and includes the TT overlay load path. Retains TT launch/setup and the memory-card/system-file polling, read/apply/write flow at 0x80079800; these helpers are mixed content rather than overlay verification. |
| `0x80079B10-0x8007C030` | `80079B10.c` | `sound_driver.c` | 54 | Proposed: T-27059: original object [0x80079B10,0x8007C030); per-frame audio command queue, bank transfer/loading, BGM, sound effects, XA voice and CD-level meter; actual body is sound driver, not date sequences. |
| `0x8007C030-0x80085E30` | `8007C030.c` | `scene_library.c` | 111 | Proposed: T-27059: original object [0x8007C030,0x80085E30); shared date/scene step primitives, portrait and face handling, dialogue and choices, heroine selection, SD sprite setup, day advance and parameter-limit helpers. Includes leading CD-level sampling, peak and average helpers as well as SD/portrait/face, normal-date transitions, girl selection, name-code and talk-script utilities. |
| `0x80085E30-0x80086810` | `80085E30.c` | `scene_display.c` | 16 | Proposed: T-27059: original object [0x80085E30,0x80086810); scene display reset, VBlank timer, texture scroll, 2D rotation, fades, finger cursor and display helpers, plus small portrait-read/state helpers; keep whole mixed object. Also retains best-girl selection at 0x8008647C and girl_state_rank; these are explicit non-display helpers in this mixed object. |

## Supported operation and verification

Every executable row is `file main OLD_STEM NEW_STEM NOTE`, with exactly five tab-separated fields. No path-shaped fake rows, header extraction, directory move, identifier change or literal replacement is mixed in. The tool updates existing C/YAML/rodata/data references and related asm include paths, and generates an original-range comment from object metadata. All 34 final main basenames are unique without regard to case. None is an address or a numeric fallback; model_3d uses the established technical abbreviation for three-dimensional models.

Three disjoint Luna passes inspected the 10 early, 13 middle and 9 late objects. Independent Sol 6.1 review passed the exact complete table against actual bodies and object metadata; no remaining naming findings. Final Docker plan result is recorded below. The preparation lane made no implementation edits; root has now applied the32source moves and associated ledger/YAML/path references.

The integration owner applied with rename.py, checked declaration views remapped to moved paths, and obtained clean27/27hashes plus unchanged function/byte progress. Each original object stays intact. Existing comments, overrides, K&R/implicit views and INCLUDE_RODATA content must remain equivalent. The parent owns commits and shared index/status/board/log updates. This new page requires an index entry from that owner.

## Remaining work

Preparation gates are complete: independent final review PASS and exhaustive Docker plan exit 0/no PROBLEM. Root application and matching proof are complete below. There is no semantic filename deferral or numeric fallback in the prepared 32-row table. The applied census contains all34logical names and zero address/number-only basenames.


## Final review and plan

2026-10-24: independent Sol 6.1 review **PASS** for the exact 32-row table and full 34-object mapping. Actual bodies support every name; mixed helper roles are documented. The independent read-only Docker census confirmed 34 aligned source/YAML/object entries, 32 exact keys with no extras/omissions, 34 case-insensitively unique final names, absent target paths, and preserved applied/previously reviewed names.

The existing `tokimemo-decomp-arm64` image with `--cpus=2` ran `python3 tools/rename.py plan config/renames/proposals/layout-main-all-files.tsv --ledger main-files`: **exit 0, 32 rows, 69 affected files, no PROBLEM**. Ignored log: `.claude/orchestrator/state/T-27059-main-plan.txt`. The body census is 830 definitions across all 34 main objects, of which 755 are in the 32 files to move; this does not replace the whole-project 6,952-function matching gate. That preparation run installed no dependencies and performed no apply/build/commit. Root application is recorded below.

This exhaustive table incorporates the seven reviewed subset mappings. Root has applied all32moves; the main folder has34logical C basenames and no address-only basename. Declaration-view preservation, clean27/27matching hashes, unchanged progress and post-application reviews now pass; root tracking records completion. There is no remaining naming blocker.

Table SHA-256: `ad5732c7550d82505e9873996f229622b08ab59d06d88129a98ca31a87dc6198`.

## Root integration

2026-10-24: root applied all32file-only rows through rename.py. The original437-source snapshot is preserved; a separate copy maps only the32intentional main source path keys. Every payload compares exactly:0benign/0check/0risky. Post-apply Standards review confirms all34names, range comments and unchanged object boundaries; its stale living-page status finding is corrected here. Clean matching proof and independent application Spec review are complete below.

2026-10-24 final integration proof: cleanDocker27/27,6952/6952functions,2276920/2276920bytes; independent rehash27/27. All437path-mapped views remain0/0/0. Independent post-apply Standards/Spec reviews pass with unchanged object bodies/boundaries. Main filename work is Done; overlay filenames and wider semantic/header work remain separate.
