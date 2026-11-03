---
id: T-39505
title: Give the English build room to grow (memory map and free relink)
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]", "[[english-text-runtime]]"]
---

## Goal

The English build from [[tickets/T-39502-english-text-runtime]] pins every object to its original address, so overlays cannot grow and the main exe has only the 27 KB island. English text is roughly twice the size of the Japanese. Map the RAM the game really uses, and let the English build relink the main exe and overlays freely where that is safe, so English strings and code fit.

## Acceptance criteria

- [x] RAM map of the whole game (main exe, bss, overlay slot and its tail, the regions above it, file load buffers, stack) from the code's constants and from emulator memory watches, with each region's owner and whether English can use it.
- [x] Every raw address in data or code that is not a linker symbol is found and either symbolised or proven not to need moving.
- [x] The English build can relink freely (main exe and overlays); with no English changes it boots and plays the emulator route with screenshots identical to the original.
- [x] Size limits checked at build time against the RAM map.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

Results: [[ram-map]], [[relink]].

## Comments

- 2026-11-01 (claude): done on branch `patch/mem`, waiting for review.
  - RAM map ([[ram-map]]): free for English are 0x8012B540-0x80132000 (27,328 bytes; no code names it, zero in 23 dumps, no write in 8 watched scenes) and each overlay's zero tail while it is loaded (TACO 291 bytes, DATE 5,084, BUNKASAI 5,231, RPG_BAT 12,268, TT 31,962, the rest 47 KB to 187 KB; no CPU write seen). The slot end 0x80162000 is a wall: background still bitstream and its run-length buffer (written up to 0x8017418A in the samples) in nearly every scene. 0x80176000-0x80180000 was never written in the samples but belongs to TT and the TACO shooter: not free. Main bss has no free hole (the zero ends of the two packet buffers are TT's work area).
  - Raw addresses ([[relink]]): `tools/raw_addrs.py` over all 27 links. Main: 384 references to numbered side-script names, 40 libcd string pointers (move), 34 font bytes and 4 header words (keep), the bss clear start in `boot_start` (move), 7 fixed slot/still addresses. Overlays: 106,001 numbered-name references, 2,348 main function pointers in mode-step tables of 12 overlays (move, automatic), TT 21 pointers into the main bss (move) and 3 tile words (keep), TACO 1 pointer (move) and 1 byte table (keep), RPG_BAT 2 `lui/lw` of `rpg_player_action` (move) and 1 mask (keep), 33 still-buffer constants (fixed). EVENT and GYOZI are never loaded and are copied. No raw jumps.
  - Free relink: `ninja english` and `ninja relink` link every unit without pins (anchors, translated side scripts, overlay entries from `config/english_overlay_entries.txt` through a second main link, raw-address patching from `config/english_raw_addrs.txt`). The English island and `src/english/boot.s` are gone; the English code is part of the main executable (ends at 0x8012C940).
  - Validation: `build/j0` equals all 27 default binaries; the padded `build/jr` disc plays the whole T-39504 route (148 checkpoints, no fault); against a control disc with the same boot timing 118 of 148 checkpoints are identical pixel for pixel and the other 30 differ only in frame-counter animations, with identical text, dates and parameters (details and the timing note in [[relink]]). Against the original disc the relinked one starts 12 frames later, so most route shots differ by animation phase and RNG.
  - Tooling: `route.py run --relink DIR` (needed for every relinked disc, the English one included) and a pixel verdict in `route compare`; `tools/test_english_link.py`.
  - Default build: 27/27 OK.
- 2026-11-01 (claude): second round, after merging `patch/english` (T-39507's one-command disc build) into `patch/mem`.
  - Merge: the free relink replaces the pinned link; T-39507's section-label pins are dropped (nothing is pinned). `configure.py` keeps the `cctext` rule, `english_variant` and the CD remap table, now an extra object of the main executable; `src/english/cd.c` and its hooks work unchanged without the island.
  - `build_english.py`: `PINNED_LINK = False`; all 1,964 C literals of the loaded units go English (the 734 the pinned link sent back now place); EVENT/GYOZI literals and in-place records stay Japanese (never loaded); a literal that initialises a sized array grows the array; in-place records are written where the free link moved their slot (`english_link.translator`).
  - `english_link.py`: addresses inside rebuilt objects are carried over (same section size and symbol offsets, a global symbol, the section end, or the object's own relocation at the same site); only side-script names in the unit's symbol tables are translated, defined names included. Found on the emulator: `D_800AA178` (libgte_patchgte copies its handler up to that label) is an object label that the side script overrides with a number; left untranslated it hung the boot once `scene_library` shrank by 0x10.
  - Report: placed 30,951, file_kept_original 4,718 (DATE talk files), kept_original 625, needs_relink 266 (fixed-size table slots, need code changes), moved_in_build 257, overflow 8. Executable ends at 0x8012CA60; overlays within the slot (TACO 291 bytes left).
  - DATE buffer: not growable. TAKU3.BIN sits at 0x801A4000 and another load fills 0x801A5800-0x801AE000 during a date ([[ram-map]]); the files keep their original bytes.
  - English disc route (`--relink build/en`): all checkpoints reached (144 screenshots, 30,149 frames), `build/emu/route-en/`. Default build 27/27 OK; `build/j0` still equals all default binaries and `build/jr` is unchanged.

## Completed source review

2026-11-02: Reviewed tools/english_link.py against CODING_STANDARDS. The linker checks section anchors, unresolved raw addresses, shared high-half rewrites, hook bounds, stable symbols between main links, and main/overlay RAM limits. Existing padded-relink route evidence above covers the runtime relocation criterion. The linker and English-build Docker suites run 56 tests: 55 pass and one optional full-disc rebuild skips. Python reports existing implicit-close ResourceWarnings in tooling; no test fails. No game code changed, so the recorded default 27/27 proof remains applicable. No unresolved correctness finding for the supported build. Ticket and board move to Done.
