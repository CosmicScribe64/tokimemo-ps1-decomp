---
id: T-27020
title: Phase 2 naming proposals and port spec for the main graphics layer (h-graphics)
status: Done
assignee: claude
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/h-graphics]]"]
---

## Goal

Roadmap phase 2 (readable code), unit `h-graphics`: name the modules, entry points and shared types of the main-exe graphics layer (`src/main/80043510.c`, `80047550.c`, `800490C0.c`, `80049FF0.c`, `8004AD60.c`, `8004C3F0.c`, `80085E30.c`) and write the port spec. The layer is replaced in the Godot port, so names stay at module level; the game-visible data (actor records, sprite table, texture-block format, palette queue) is described in full.

## Acceptance criteria

- [x] `config/renames/proposals/h-graphics.tsv` has one row per rename (kinds func, data, field, type, file, const) and a comment row for each O.BIN-named function; no src/, include/ or symbol-file edits.
- [x] `wiki/spec/h-graphics.md` covers frame flow, actor pool, texture-block cache, VRAM queues, primitives, screen layers, palette tools, debug HUD and open questions; linked from `wiki/spec/index.md`.
- [x] Uncertain game-meaning claims are marked `HYPOTHESIS:` in the note column.
- [x] Review of the table and spec by the phase-2 reviewer (approved after three review rounds).

## Notes

- 263 rows: 53 func, 48 comment, 27 data, 13 type, 98 field, 7 file, 17 const; 24 rows start with `HYPOTHESIS:` (after review 2; 11 sound-loader data rows are left to the sound unit). "N call sites in src/" counts indented `name(` occurrences in `src/` (calls only, definitions and prototypes excluded).
- Evidence beyond the C: `tools/emu/run.sh trace` and `watch` from a save state (NAME_ENT, mode 0x11): `render_frame`, `screen_brightness_step`, `actor_pool_draw` and `disp_string` run every frame, `GetWorkBase` twice per frame; the work-area offset counter goes 0, 0xC, 0x18 for two 12-byte requests, which confirms the 4x address stride.
- Prefix choice: no overlay prefix for main-exe code; stems `render_`, `actor_`, `screen_`, `prim_`, `fx_`, `bg_slot_`, `debug_hud_`, `debug_print_`. The sound-bank loader at the end of `80043510.c` uses `sound_bank_` and `bgm_` provisionally and needs reconciling with the sound unit.
- `Rec24` is the PsyQ `GsSPRITE` layout (all 18 fields at the library offsets); `SpriteSlot24`, `SpriteMap10`, `SpriteCell8` match `GsBG`, `GsMAP`, `GsCELL`. The `DisplayActor44` fields were named only where `actor_pool_draw` reads them; other units may add more.
- Five helpers in `80085E30.c` are not graphics (`pick_top_girl`, `girl_state_code`, `asset_package_verify_step`, `sound_bank_wait_latch`, `actor_pair_swap_sequences`); named with their own notes.
- Two empty functions (`func_80044434`, `func_8004DAC4`) are left unnamed.
- Texture-block cache: an actor's claim on a slot lasts one frame (it is written to `tpage_frame_map` only; `search_tpage(0x7FFF)` frees every slot whose `tpage_resident` word is -1 at the next frame start), so the block is queued for upload again in each frame that uses it. Only slots with a resident word other than -1 persist (lock functions, `bg_slot_setup`, `_sys_default_tpage_set`, overlay stores). The first pass said the cache never evicts; that was wrong.
- Cross-unit: h-cdio (d11a7b8e7) adopted `draw_origin_x/y`; core (`p2a-core`, 7dc0b2252) differs from this table on 74 names (17 func, 3 data, 13 type, 41 field), all listed in the spec section Cross-unit names for the merge decision.

## Comments

### Review fixes (2026-10-24)

First review: not approved, five blocking findings. Fixed, each checked against the C:

- Field rows use the old type names as scope (`SrnRec28`, `ModRec14`), like the other rows and the h-cdio table.
- `model_objects` is 16 records of 16 bytes (the loop ends at `D_80122740`, 0x100 bytes on); the spec step 5 says so.
- `ACTOR_ANIM_END_BACK` clears `RUN` too: the animation plays once and holds its last frame.
- Frame limiter: the 2-vblank wait applies to every TACO stage from 10 up, 12 included; stage 12 differs only in still calling `DrawSync`.
- `pick_top_girl`: baseline girl 0, replacement rule with the state code test, and the larger record sum wins ties.

Non-blocking findings taken: palette column math per `palette_mode`, `palette_info` note, `unk_10E8` renamed `display_pass_enabled` (set to 1 at boot), screen shake no longer lists the `mod_set_gpu` tile sprites, `scene_gfx_init` clears the sprite table, BG slots and text list, `fx_disc_tiles`, `debug_hud_level_step` and `load_csr_ab` notes, HYPOTHESIS marks moved to the start of the note, the GsSortSprite naming conflict with `src/main/8004C3F0.c:441` recorded, a cross-unit section for `draw_origin_x/y` against the h-cdio names.

Open for the orchestrator: this ticket has no card in `wiki/kanban.md`, and the parent `T-27000-phase-2` does not exist in this tree.

### Review fixes, round 2 (2026-10-24)

Second review: not approved, three blocking findings. Fixed, each checked against the C:

- Texture-block cache: claims last one frame; only held slots persist (see Notes). Spec paragraph rewritten; the `unk_1128` and `unk_1228` notes and the three `search_tpage*` comments say the same.
- `prim_box_gouraud` modes: multipliers 10, 8, 6, 4 for v0..v3 (top-left, top-right, bottom-left, bottom-right) stored in a byte; 4 black on top; 5 colour left, black right; 6 black left, colour right; no diagonal case; mode 3 reads green for blue.
- `func_800866A0`: compares eight words (five, one and two through three pointers), not four. Renamed `overlay_load_verify_step` to `asset_package_verify_step` (HYPOTHESIS, evidence in the note); the "belongs to the CD/overlay unit" line is gone; core's `bustup_read_step` is recorded as the alternative.

Non-blocking findings taken: `make_color_bar16` direction (entry 1 is near `end`, the last entry near `start`), cell size (`texture_flags` bit 3) and pixel depth (`sprite_attribute` bits 24-25) are separate settings, record 160 is one past the pool when `unk_1098` is 0xA0, the core comparison and the stale h-cdio note, HYPOTHESIS marks added to `sound_request`, `sound_bank_state_reset`, `sound_bank_wait_latch`, `sound_bank_wait_done` and `load_csr_ab` (the glossary seed disagrees on `csr_*`) and removed from `ACTOR_DRAW_FIRST_PARTS` (the behaviour is certain), `actor_pair_swap_sequences` and `actor_reset_range` notes, call-site counts recomputed with one method.

One figure differs from the review: record 160 is at 0x80121750 (0x8011ECD0 + 160 * 0x44), the start of the screen-layer records, not at `sprite_table[0]` (0x801217D0, 0x80 bytes later).

### Review approved (2026-10-24)

Third review: approved, no findings. All three blocking findings from round 2 are fixed, and no new clear errors turned up in the lines the fix touched. Each claim was checked against the C in the worktree:

- Texture-block cache: the spec section matches `search_tpage`, `search_tpage8` and `search_tpage_multi` in `src/main/80047550.c`. A step-3 claim writes only `unk_1128` (`tpage_frame_map`); the next `search_tpage(0x7FFF)` frees every slot whose `unk_1228` (`tpage_resident`) word is -1, so a used block is queued for upload again each frame. The writer list is complete: `load_tpage_buf_lock`, `search_load_tpage_buf_lock`, `bg_slot_setup`, `_sys_default_tpage_set`, overlay and `8007C030.c` stores (marks 1, 8, 0x80000100+n, -1), and the `80059B40.c` subtraction on slots 27-31 and 59-63. The `unk_1128`, `unk_1228` and `search_tpage*` rows agree with the ticket notes.
- `prim_box_gouraud`: the spec table and the TSV row match `func_80049A40` in `src/main/800490C0.c`. Mode 0 and 7 or more use multipliers 10, 8, 6, 4 for v0..v3 stored in a byte, so a channel above 25 overflows at v0; mode 2 puts the 80% on v3; mode 3 multiplies by 8 and reads green for blue; mode 4 is black on top; mode 5 is colour left to black right.
- Scope: the branch changes only `config/renames/proposals/h-graphics.tsv` and wiki files; nothing under `src/`, `include/` or `config/symbol*`.
- The old "never evicts" text survives only in the earlier `ingest` log entry (the log is append-only); the round 2 `ticket` entry corrects it.
