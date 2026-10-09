---
type: concept
updated: 2026-10-09
sources: ["disc/files/CDROM/EXEDIR/O.BIN", "tools/obin_syms.py", "tools/obin_map.py", "config/symbol_addrs_obin.txt", "config/obin_renames.txt", "config/symbol_addrs_sdk.txt"]
---

# O.BIN (developer-build symbol table)

Ticket [[tickets/T-0201-obin-format-and-symbols]]. Context: [[overlays]], [[executable]], [[psyq-sdk]].

## What it is
`CDROM/EXEDIR/O.BIN` (196608 bytes, sector 0x964A, never read by the game) is a **little-endian MIPS ECOFF** executable (magic 0x0162, as written by the PsyQ/SN linker), zero-padded to 0x30000 with the usual checksum trailer. It is the link of a tiny **OLH overlay skeleton** at 0x80132000 against the symbol map of the **main program**: 14 symbols belong to the overlay itself, the other 2132 are absolute (`scAbs`) addresses of a developer build of the main exe. It holds no code or data of the main program.

Not a copy of the retail layout: its link timestamp is 1995-07-25 (UTC, `f_timdat` 0x30152746) and its main-program addresses differ from SLPM_86.053 (game code starts at 0x8004C000 instead of 0x80041000; debug code present that retail lacks). Whether it is earlier or later than the retail build is not provable from the file; the OLH skeleton being much smaller than the retail OLH suggests earlier.

## Format (all little-endian, offsets in the file)
| offset | content |
|---|---|
| 0x00 | ECOFF file header: magic 0x0162, 4 sections, timdat, `symptr` 0x5C0, `nsyms` 0x60 (size of the symbolic header), `opthdr` 0x38, flags 0x800F |
| 0x14 | a.out optional header (0x38 bytes): magic 0x0107, tsize 0x3A0, dsize 0x130, bsize 0, entry 0, text_start 0x80132000, data_start 0x80132000 |
| 0x4C | section table, 4 x 0x28 bytes: `.text` 0x80132000 size 0x3A0 at 0xF0; `.rdata` 0x801323A0 size 0x90 at 0x490; `.data` 0x80132430 size 0xA0 at 0x520; `.comment` (empty, flags 0x1002, header fields not meaningful) |
| 0xF0..0x5C0 | section contents (0x3A0 + 0x90 + 0xA0 bytes) |
| 0x5C0 | mdebug symbolic header (HDRR, 0x60 bytes, magic 0x7009, vstamp 0x312). Only these counts are non-zero: `ipdMax`=5 (procedure descriptors at 0x638), `issExtMax`=0x6F7C (external strings at 0x73C), `ifdMax`=1 (file descriptor at 0x76B8), `iextMax`=0x862=2146 (external symbols at 0x7700) |
| 0x638 | 5 procedure descriptors (52 bytes each): address, frame size (0x28, 0xC0, 0x28, 0x28, 0x30), register masks |
| 0x73C | external string table, NUL-separated names |
| 0x76B8 | one file descriptor, empty (no source name, no local symbols) |
| 0x7700 | 2146 external symbols x 16 bytes (`EXTR`: flags, ifd, then `SYMR` iss/value/bitfield; st = bits 0-5, sc = bits 6-10, index = bits 12-31), ends at 0xFD20 |
| 0xFD20 | zeros up to the trailer `2E DC 12 10` at 0x2FFFC |

Local symbols, line numbers, types, auxiliary and optimization tables are all absent (stripped), so there are no source file names, no struct or type information and no variable sizes. Symbol sizes are only inferable: `tools/obin_syms.py` prints the gap to the next higher address as an upper bound (`~N`).

Symbol kinds: 2132 Global/Abs (main program, 0x8004C000-0x80131xxx), 5 Proc/Text (`olh_main` 0x80132000, `olh_init` 0x80132070, `olh` 0x8013222C, `olh_exit` 0x8013231C, `move_menu_title` 0x8013233C), 9 Global/Data (`menu0`..`menu7` at 0x80132430 + 0x10 each, `olh_menu` 0x801324B0).

## Tool: tools/obin_syms.py
`tools/docker.sh python3 tools/obin_syms.py [--file PATH] [--sections]` reads `disc/files/CDROM/EXEDIR/O.BIN` at run time (nothing from the file is committed) and prints `addr name kind sc size`. Tests with synthetic ECOFF input (parser, alignment, `map_region`, output filter): `tools/docker.sh python3 tools/test_obin_tools.py`.

## Mapping onto SLPM_86.053 and the overlays
Code-byte matching is only possible for the 5 overlay procedures (the 0x3A0 `.text` bytes). The 2132 main-program names have addresses and names only, so the mapping is structural: `tools/obin_map.py` (needs a build for `build/SLPM_86.053.elf`) sorts both sides by address, takes the gap to the next symbol as O.BIN size and aligns the two size sequences (Needleman-Wunsch, +3 equal size, +0.3 size within 2x, -0.7 otherwise, -1 gap). The 154 PsyQ names already found by signature matching ([[psyq-sdk]], `config/symbol_addrs_sdk.txt`) that O.BIN also carries are hard anchors: 116 form an ordered chain that splits the address space into segments (the other 38 are lib blocks linked in a different order). Data and bss are aligned the same way against the `D_`/`jtbl_` symbols (no anchors).

Confidence classes (run = consecutive same-diagonal pairs of identical size):
- **sdk**: the name is the PsyQ name already in the symbol file; anchor, exact by construction.
- **high**: identical size, run >= 4 with >= 3 distinct sizes. Calibrated by hiding 90% of the PsyQ anchors and re-deriving them: 99 of 99 correct.
- **med**: identical size, run of 2-3. Same test: 236 of 280 (84%).
- **low**: aligned with a different size. Same test: 250 of 545 (46%; the PsyQ libs differ in size between builds, game functions edited between builds look better, see spot checks). Not used for output.
- Game-code spot check (24 random high-confidence game renames, address < 0x80086810): in 12 the callees (named by PsyQ or other high names) fit the name (e.g. `srn_tpage_show` calls `SetSemiTrans`/`SetShadeTex`/`AddPrim`; `day_plus` calls `hizuke_init`/`hizuke_show`; `week_day_main` calls `week_day_main0`; `pre_xmas_init` calls `message_window_init`/`icon_disp_switch`), 0 contradicted, 12 could not be checked (leaf functions or only unnamed callees). So 12 of 24 corroborated, 0 refuted; the PsyQ calibration above is for library code and is optimistic for game code. No class was dropped; treat all names as hypotheses. The 22 data names (libsnd bss such as `_snd_openflag`, `_svm_vab_used`, order as in libsnd) are uncalibrated.
- Independent spot checks (roles known from [[overlays]]): the main loop `func_80042540` -> `game_main`, the overlay loader `func_80078C48` -> `load_func_main`, the load checksum `func_8004636C` -> `check_sum_check`, the CD read `func_80046318` -> `cdreaddata2`, the TT loader `func_8007959C` -> `load_func_go` (all class low or med). The names fit the roles, so the alignment holds even where sizes changed; the unsized classes just cannot be certified.
- Exact address agreement: none (0 symbols have the same address and size in both); the layouts differ by 0xB000 or more.
- Overlay side (OLH, tentative, by order and shape, not in any generated file). **Manual and not reproducible from the committed tools** (one-off script comparing opcode shapes with immediates masked, difflib ratio; the 0.81/0.63 figures come from it): `olh_main` -> `func_80132000` (both entries at 0x80132000), `olh_init` -> `func_80132114` (opcode-shape similarity 0.81), `olh` -> `func_80132294` (0.63), `olh_exit` -> `func_801323AC` (identical shape, 0x20 bytes). The O.BIN `.text` is not byte-identical to OLH.EXN (230 of 232 words differ).

### Statistics (2146 names)
| group | count |
|---|---|
| main exe, PsyQ name already known (sdk) | 154 |
| main exe, code, high | 348 |
| main exe, data/bss, high | 22 |
| main exe, med (code 187, data 132) | 319 |
| main exe, low (code 602, data 413) | 1015 |
| overlays (OLH family, tentative only, 0 high) | 14 |
| unmapped (code with no counterpart, e.g. dev-only init and debug code; 6 data) | 274 |

Mapped to an overlay with high confidence: 0. High-confidence additions to the project: 370 renames (348 functions, 22 data), in `config/symbol_addrs_obin.txt` (splat `symbol_addrs` format, regenerate with `tools/docker.sh python3 tools/obin_map.py --write`) and `config/obin_renames.txt` (`old new`). They are **not applied** to `src/`, `include/` or the main symbol files; see [[tickets/T-0600-apply-obin-renames]].

## Useful metadata
- No source file names, no types, no line numbers (stripped). Module boundaries can only be guessed from name prefixes and address order of the mapped functions: `Snd_*`, `SD_*` (sound driver), `cd*`/`load_*` (CD and overlay loader), `date_*`, `event_*`, `hizuke_*`, `girl_*`, plus the PsyQ libs (`Gs*`, `Ss*`, `Spu*`, `Cd*`).
- Struct hints (from the 9 data symbols, content is Shift-JIS text and pointers): `menu0`..`menu7` are 8 consecutive 16-byte text buffers (`char menu[8][16]`, 15 characters plus NUL); `olh_menu` is a table of 8 pointers to them.
- Procedure descriptors give frame sizes for the 5 overlay procedures (0x28, 0xC0, 0x28, 0x28, 0x30).
- The libsnd bss variables (`_snd_openflag`, `_svm_vab_used`, ...) and other SDK data map with high confidence, useful for [[tickets/T-0301-sdk-rodata-data-split]].

## Status of the names: hypotheses (T-0600)
The 370 high-confidence names are applied to the build through `config/symbol_addrs_obin.txt` (listed in `symbol_addrs_path` of `config/SLPM_86.053.yaml` after the SDK file; main exe only, overlay configs would reject them). They are hypotheses, not facts: of the 24 game-code spot checks, 12 were corroborated by the callees or the role, 0 were refuted, and the other 12 were inconclusive. Convention (CODING_STANDARDS section 4): provisional names stay in that file, whose header says HYPOTHESES; a confirmed name moves to `config/symbol_addrs.txt`. The file is hand-maintained now: the generator only emits placeholders, so `tools/obin_map.py --write` on the renamed tree would produce an empty list (do not run it). Code lines have no `size:` attribute, because the O.BIN gap includes trailing alignment nops that splat would count into the function (+12 bytes in `ninja progress`). Splat's `asm/nonmatchings/main/<addr>/<name>.s` file names and the `INCLUDE_ASM` names in `src/main/*.c` follow the new names; old-to-new pairs stay in `config/obin_renames.txt` (this page and [[log]] keep the old names on purpose). Collision check on all 370: no duplicates, no C keywords, no invalid identifiers, no clash with `config/symbol_addrs_sdk.txt`, `include/` or `src/` identifiers or libc names; none had to be skipped. Many names are PsyQ-style (`SsUt*`, `SpuVm*`, `Cd*`) because the developer build linked the same libs; they sit in game-code addresses, so they stay hypotheses.

## Open
- [[tickets/T-0600-apply-obin-renames]] applied (see Status above).
- [[tickets/T-0601-obin-med-confidence-review]] confirm med/low mappings by call-graph or semantics.
