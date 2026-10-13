---
id: T-11090
title: Ingest the English patch notes as a reference
status: Done   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: r32 reference agent
created: 2026-10-13
updated: 2026-10-13
links: [[[ref-english-patch]], [[obin]], [[build-system]]]
---

## Goal

Record the facts (addresses, roles, file-format layouts) of the third-party English patch's internals notes in [[ref-english-patch]], and use them to give placeholder symbols in the 27 binaries descriptive names. Facts only: the source repo has no license, so no prose, code or translated text is copied.

## Acceptance criteria

- [x] `wiki/ref-english-patch.md` with a credit line and an address table grouped by area (about 150 addresses); linked from [[index]].
- [x] Confident placeholders renamed through the splat symbol files (main: `config/symbol_addrs.txt`; overlays: `config/symbols_<OVL>.txt`); TT untouched.
- [x] `tools/docker.sh ninja` 27/27 OK, `check_headers.py`, `check_symbols.py` and the globals check OK.
- [x] Review against `CODING_STANDARDS.md`, findings resolved.

## Notes

Source: `docs/game-internals.md` of https://github.com/gothamor/tokimeki-memorial-english-patch (same game version SLPM-86053). Only the facts are used; nothing else of that repo was fetched.

Renames (50, evidence = the role the notes give plus our own asm showing the same call/data shape; names are `snake_case`, uncollided in `src`, `include`, `config` and the link maps):

- Main exe, `config/symbol_addrs.txt` (30): `cd_read_sectors` 0x80046318, `cd_read_sync_status` 0x800460CC, `file_checksum_ok` 0x8004636C, `ovl_load_poll` 0x80078FE0, `ovl_load_verify` 0x80079014, `main_state_set` 0x80042878, `text_set_advance` 0x8004E750, `glyph_expand_1bpp` 0x8004F4F0, `dialogue_render` 0x80081190, `dialogue_speak_part` 0x8007EF48, `appointment_find_today` 0x8006AEC4, `appointment_launch` 0x80076DA0, `special_event_select` 0x8006FB4C, `system_settings_init` 0x8005DC4C, `club_list_menu` 0x8006B648; data `ovl_load_state/sectors/buf/lba` 0x800B6D34-0x800B6D40, `cd_read_status` 0x8012512A, `text_list` 0x800B3DC0, `text_list_count` 0x800B3F60, `text_glyph_count` 0x800B3F62, `text_glyph_advance` 0x800B3F6A, `font_glyphs` 0x800B6D50, `font_range_table` 0x800B402C, `sprite_frame_table` 0x800C975C, `girl_short_name_table` 0x800B35F4, `club_name_table` 0x800B3908, `girl_name_buf` 0x800CA190.
- DATE, `config/symbols_DATE.txt` (17): `date_event_trigger`, `date_event_choose`, `date_shiori_dispatch`, eleven `date_fill_sections_<girl>` (0x80141004 ... 0x8014420C), `date_place_names`, `date_gift_names`, `date_arrival`.
- NAME_ENT, `config/symbols_NAME_ENT.txt` (2): `prologue_line_start`, `prologue_line_run`. SHOUGATU, `config/symbols_SHOUGATU.txt` (1): `shougatu_stage_dispatch`.
- The `MAIN_API_OVERRIDE_func_80081190` macro became `MAIN_API_OVERRIDE_dialogue_render`. `tools/ido_eval_cases/*.c` keep the old names (standalone eval fixtures, not part of the build). TT and GameState-range globals (0x800E6280-0x800E7D10, T-5100) were not renamed.
- Build gotcha: after renaming, stale `asm/**/<old name>.s` files from an earlier split steal the `rodata_pieces.py` rodata (first function mentioning a symbol), giving "undefined reference D_..." at link time. Delete the old-name `.s` files and `build/split.stamp` / `build/ovl/*.stamp`, or start from a clean tree (CI does).

Result: `tools/docker.sh ninja` all 27 checksums OK, headers/symbols/globals OK.

## Comments

Review (inline, CODING_STANDARDS.md checklist): renames only through splat symbol files; main-exe symbols still declared once in `include/main_api.h` with unchanged types, override macro renamed with its symbol; overlay names in per-overlay `config/symbols_<OVL>.txt` registered in the overlay yaml, none in `config/symbol_addrs.txt` at overlay addresses; no copyrighted content (facts only, own words); no C changes beyond identifiers; wiki, kanban, index and log updated. No findings.
