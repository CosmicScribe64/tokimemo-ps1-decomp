---
id: T-27020
title: Phase 2 naming proposals and port spec for the main graphics layer (h-graphics)
status: In Review
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
- [ ] Review of the table and spec by the phase-2 reviewer (this ticket is In Review).

## Notes

- 274 rows: 53 func, 48 comment, 38 data, 13 type, 98 field, 7 file, 17 const; 25 rows start with `HYPOTHESIS:`.
- Evidence beyond the C: `tools/emu/run.sh trace` and `watch` from a save state (NAME_ENT, mode 0x11): `render_frame`, `screen_brightness_step`, `actor_pool_draw` and `disp_string` run every frame, `GetWorkBase` twice per frame; the work-area offset counter goes 0, 0xC, 0x18 for two 12-byte requests, which confirms the 4x address stride.
- Prefix choice: no overlay prefix for main-exe code; stems `render_`, `actor_`, `screen_`, `prim_`, `fx_`, `bg_slot_`, `debug_hud_`, `debug_print_`. The sound-bank loader at the end of `80043510.c` uses `sound_bank_` and `bgm_` provisionally and needs reconciling with the sound unit.
- `Rec24` is the PsyQ `GsSPRITE` layout (all 18 fields at the library offsets); `SpriteSlot24`, `SpriteMap10`, `SpriteCell8` match `GsBG`, `GsMAP`, `GsCELL`. The `DisplayActor44` fields were named only where `actor_pool_draw` reads them; other units may add more.
- Five helpers in `80085E30.c` are not graphics (`pick_top_girl`, `girl_state_code`, `overlay_load_verify_step`, `sound_bank_wait_latch`, `actor_pair_swap_sequences`); named with their own notes.
- Two empty functions (`func_80044434`, `func_8004DAC4`) are left unnamed.

## Comments
