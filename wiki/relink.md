---
type: design
updated: 2026-11-01
sources: ["tools/english_link.py", "tools/raw_addrs.py", "tools/elf32.py", "tools/patch/variant_disc.py", "tools/emu/route.py", "config/english_overlay_entries.txt", "config/english_raw_addrs.txt", "configure.py"]
---

# Free relink

How the English build links the main executable and the overlays without pinning any address, which addresses the linker cannot see, and how the relink was proved on the emulator. Ticket: [[tickets/T-39505-english-memory-and-relink]]. Room it can use: [[ram-map]]. Text runtime on top of it: [[english-text-runtime]].

## Variants

| Target | Directory | What |
|---|---|---|
| `ninja english` | `build/en/` | flagged objects rebuilt with `-DTOKIMEMO_ENGLISH`, the English runtime (`src/english/`, the font) appended to the main executable's code and data, hooks of `config/english_hooks.txt` |
| `ninja relink` | `build/j0/` | the default objects relinked as they are; every one of the 27 binaries must equal the default one (`build/j0/identity.ok`, a `cmp` per binary) |
| `ninja relink` | `build/jr/` | the default objects with padding: 0x1110 bytes before the second main `.text` input and 0x2220 before the first main `.data` input (the bss moves up 0x3330), 0x50 and 0x70 bytes in every overlay. Nearly every address moves. |

`tools/patch/variant_disc.py DIR TRACK1 TRACK2` writes `DIR/disc/track1.bin` and `tokimemo.cue`: the executable moved to the end of track 1 (`tools/patch/disc.py`, relocate=True), every overlay in place (it fails if one would move: the loader reads overlays by fixed LBA). The output is game data; never commit it.

## How a unit is linked

`tools/english_link.py`, run by ninja:
1. `ld`: the default linker script with an anchor `__en_<unit>_<n> = ALIGN(., 2);` before every input section (n = its place in the script; inputs are 2-aligned by `SUBALIGN(2)`). Rebuilt objects are read from the variant directory. New main objects go after the last `.text`, `.rodata` and `.data` inputs; their `.bss` and `COMMON` go after the data as zeros in the file, so nothing has to clear them. No `. =` pins.
2. `aux`: the side scripts of the default link (`undefined_funcs_auto.txt`, `undefined_syms_auto.txt`, `*_data_syms.ld`, and `main_names.ld` for overlays) define about 106,000 names as numbers. Each number in the unit's own range becomes `anchor + offset` (or the global symbol at that address when the containing object is rebuilt; no symbol there fails the build). In an overlay, a number in the main executable becomes the address the first main link gives it. New main symbols (`english_*`) are added for overlay code.
3. Main executable, first link, against the default overlay entry addresses.
4. Overlays, each against the first main link.
5. `entries`: the 44 overlay entry symbols that `ovl_load_enter` and `game_mode_tt_game_mode_handler` name (`D_8013209C` ...) take their address from the overlay links; `config/english_overlay_entries.txt` says which overlays each one enters (from the mode -> LBA and mode -> entry switches). Every listed overlay must give the same address; `D_80132000` must stay the slot base.
6. Main executable, second link, with those entries. Every other symbol must keep its address from the first link (checked).
7. `exe` / `ovl`: anchors checked against the map, raw addresses patched (below), hooks, then size checks ([[ram-map]]), header `pc0` = `boot_start` and `t_size`, or the overlay checksum trailer.

EVENT and GYOZI are copied unchanged: the game never loads them.

The English build no longer has an island or a boot stub (`src/english/boot.s` is gone): the English code is part of the main executable, and the bss moves up behind it.

## Raw addresses

`tools/raw_addrs.py MAP` scans every object of a link for addresses the linker does not write: data words in the main range (0x80041000-0x8012B540) or inside the overlay slot with no `R_MIPS_32`, `lui` with no `R_MIPS_HI16` whose value (with the first low part) lies there, jumps with no `R_MIPS_26`, and relocations against names that a side script defines as a number. The slot base 0x80132000 and its end 0x80162000 (the still buffer) are fixed addresses and are not reported.

Results on the default build:

| Unit | Kind | Count | Decision |
|---|---|---|---|
| main | side-script numbers | 384 sites | translated by `aux` (step 2) |
| main | words in `D_800DC644` (libcd) | 40 | move: pointers to the CD command names in `.rodata` |
| main | words in `text_font_glyphs` | 34 | keep: font bitmap bytes |
| main | words in the PS-X EXE header | 4 | keep (`exe` writes `pc0` and `t_size`) |
| main | `lui/ori 0x800E60A0` in `boot_start` | 1 | move: start of the bss clear; the length 0x45498 stays because the bss block does not change |
| main | `lui/ori 0x80132000` (`game_init.c`, `overlay_loader.c` x2), `0x80162000` (`background_images.c` x3, `title_screen.c`) | 7 | fixed addresses |
| overlays | side-script numbers | 106,001 sites | translated by `aux` |
| 12 overlays | words that are main function starts | 2,348 | move, automatically: mode-step tables (`.word 0x8007C784` = `scene_wait_cd_read` 733 times). DATE 935, SHOUGATU 521, GEKO 413, SHUGAKU 193, KANGEI 100, DATE2 76, VALEN 38, ENDING 36, OMIMAI 34. |
| TT | words `tt_sys` and the 20 after it | 21 | move: TT's work areas inside the main bss (0x80110000-0x8011FD00) |
| TT | 3 words of tile data | 3 | keep |
| TACO | `D_8015F830` | 1 | move: pointer into its own data |
| TACO | `shooter_digit_uv` | 1 | keep: bytes |
| RPG_BAT | `lui/lw` of `rpg_player_action` written as `0x8015ED98` (a FAKE in the C) | 2 | move |
| RPG_BAT | `0x800F0000` | 1 | keep: a mask on `rpg_encounter` |
| 7 overlays | `lui/ori 0x80162000` | 33 | fixed address |
| EVENT, GYOZI | 1,913 words | | not relinked (never loaded) |
| all | jumps without a relocation | 0 | |

`config/english_raw_addrs.txt` lists every decision that is not automatic (by default address, address range or symbol; a symbol only the English build has names relinked addresses). An unlisted raw address fails the link, and so does an entry that matches nothing. The patch step rewrites a word, or a `lui` and every low part that uses its register (it fails if they would need different upper halves).

Not addresses but also fixed: the LBAs of every data file and overlay are literals in the code (`ovl_load_start`, `boot_assets.c`, `title_screen.c` ...), so files must keep their sectors on the disc.

## Validation

1. **Identity.** `build/j0/` (relinked, no padding) reproduces all 27 default binaries byte for byte (`ninja relink`, `build/j0/identity.ok`).
2. **Cycle-exact boot.** From power-on, `game_mode_dispatch` runs at the same frame and the same CPU cycle count on the padded build and on the control disc below for the first 300 hits (to frame 1566). PCSX-Redux timing does not depend on where the code sits.
3. **Route, padded build against the original.** The T-39504 route (`tools/emu/run.sh route run --relink build/jr`) played the whole way on the `build/jr/` disc: title, options, file select, name entry, 48 prologue pages, week 1, phone call, save, the date, load (148 checkpoints, 29,028 frames, no fault). The relinked executable sits at the end of track 1, so the game starts 12 frames later than on the original disc (`build/emu/route-jp`); with fixed inputs shifted by those 12 frames, 41 consecutive frames of the attract sequence are identical. On the route only 7 of 126 shared checkpoints are identical, because of that offset: the top scanline of the menu screens and every frame-counter animation differ, and Shiori's answer to the first call (random, seeded from the frame count) differs, so the two runs take different paths.
4. **Route, padded build against a control.** To remove the offset, the control disc `build/jc/` holds the original executable padded with zeros to the padded build's size and moved to the same place, and the original overlays (made by hand for this check: `variant_disc.py` on a directory of the default binaries with the executable padded and `t_size` raised; not a build target). Padded build against control: **118 of 148 checkpoints identical pixel for pixel**, including every screen from the Konami logo to prologue page 8, the command screen, week 1, the phone call and save. The other 30 differ only in frame-counter animations: the scrolling checker border, the calendar curves, an eye blink, a mouth frame, the typewriter text box shade. Dates, parameters, text and choices are the same on every one. From prologue page 9 the two runs are 78 frames apart and later up to 276. The control is the odd one here: the original disc, the padded build and a second relink with 4 KB-aligned padding (`build/jt`, main only) all take 288 frames from page 8 to page 9, the control 210. The control differs from the original only by the zeros it loads into the bss that `boot_start` does not clear (0x800E3800-0x800E60A0), so the game probably reads some of that memory before it writes it.
5. Screenshots (game data, not committed): `build/emu/route-jp`, `route-jr`, `route-jc`, `route-jt`; `build/emu/compare-route-jc-route-jr/compare.html` (`route compare` now prints how many shared checkpoints are identical and marks each row).

The route reads GameState and the cursor at their default addresses. Any relinked disc (the English one too, whose bss moves behind the English code) needs `--relink DIR`, which translates those addresses through the variant's link. The `emu.py` run log (mode byte, resident overlay) still assumes default addresses.

## Limits and risks

- Room: [[ram-map]]. The main executable may grow by 27,328 bytes in total (English now uses 4,352 + 768); each overlay up to its zero tail (TACO 291 bytes, DATE 5 KB, BUNKASAI 5 KB, RPG_BAT 12 KB).
- Raw-address decisions are reviewed by hand; a byte table that happens to hold a main function address would be patched by the automatic rule. The 2,348 automatic words are all `.word` lines in mode-step tables.
- A rebuilt object loses the anchor translation for addresses inside it; such an address needs a global symbol, and the build says so.
- The bss part that `boot_start` does not clear (0x800E3800-0x800E60A0 in the default build) moves with the relink; its contents at boot come from whatever the BIOS left, and the control run above suggests the game reads some of it. Behaviour that depends on it may differ by a few frames, as it does between the original disc and the control.
- `pc0` now points at `boot_start`; the header's non-standard fields 0x08, 0x20 and 0x24 (rodata position) keep their default values: the BIOS does not read them.
