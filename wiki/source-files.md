---
type: concept
updated: 2026-10-24
sources: ["src/main/README.md", "config/SLPM_86.053.yaml", "config/objects/", "src/main/", "src/ovl/", "wiki/spec/file-naming-main.md", "wiki/spec/file-naming-overlays.md", "tools/srcscan.py", "tools/object_boundaries.py", "tools/cc.py"]
---

# Source files of the game code

The current source tree has **437 logical C files**: 34 in `src/main/` and 403 in the 26 disc-named folders under `src/ovl/`. The 34 main filenames and their original, half-open text ranges are listed below; the reviewed role descriptions live in [`src/main/README.md`](../src/main/README.md). Overlay filenames remain grouped by their current disc folder. The exhaustive 403-path overlay map and its review history are in [[spec/file-naming-overlays]].

The verified matching reference is **4dab92732: 27/27 units, 6,952/6,952 functions** (main 830; overlays 6,122). The in-progress small-constant wave changes literals only and does not change file or function counts; this page makes no newer matching claim. The configured original-object metadata has 34 main ranges and 409 overlay `object` records. The filesystem has 403 overlay C files; DATE and TACO each have three more object records than C files in these inventories. This page does not infer a one-to-one mapping for those units.

The source basenames are logical names now. Earlier boundary work used address basenames; its evidence and decisions are retained under [historical boundary analysis](#historical-main-boundary-analysis-address-basename-stage). The current main YAML entries agree with `src/main/README.md` and the main object ranges in `config/objects/main.txt`.

## Main executable C files

Ranges are original text ranges, start-inclusive and end-exclusive. Paths below are the current `config/SLPM_86.053.yaml` names and filesystem paths; the original boundaries are independently recorded in `config/objects/main.txt`.

| Original text range | Current C file |
|---|---|
| `0x80041000–0x800420D0` | `src/main/game_init.c` |
| `0x800420D0–0x80042540` | `src/main/main_loop.c` |
| `0x80042540–0x80042A00` | `src/main/game_mode.c` |
| `0x80042A00–0x80043510` | `src/main/pad_input.c` |
| `0x80043510–0x800451D0` | `src/main/frame_render_audio.c` |
| `0x800451D0–0x80046500` | `src/main/cd_queue.c` |
| `0x80046500–0x80047550` | `src/main/reset_iris_wipe.c` |
| `0x80047550–0x800490C0` | `src/main/actor_sprites.c` |
| `0x800490C0–0x80049FF0` | `src/main/gpu_primitives.c` |
| `0x80049FF0–0x8004AD60` | `src/main/gpu_effects.c` |
| `0x8004AD60–0x8004C3F0` | `src/main/debug_hud_palette.c` |
| `0x8004C3F0–0x8004E500` | `src/main/screen_layers.c` |
| `0x8004E500–0x8004F6B0` | `src/main/text_system.c` |
| `0x8004F6B0–0x8004F870` | `src/main/vram_viewer.c` |
| `0x8004F870–0x80053650` | `src/main/menu_speaker_girl.c` |
| `0x80053650–0x80056300` | `src/main/memory_card.c` |
| `0x80056300–0x800563F0` | `src/main/reload_executable.c` |
| `0x800563F0–0x80057390` | `src/main/movie_player.c` |
| `0x80057390–0x80058D20` | `src/main/background_images.c` |
| `0x80058D20–0x80059710` | `src/main/model_3d.c` |
| `0x80059710–0x80059A20` | `src/main/mouse.c` |
| `0x80059A20–0x80059B40` | `src/main/debug_display.c` |
| `0x80059B40–0x8005A0B0` | `src/main/boot_assets.c` |
| `0x8005A0B0–0x80061710` | `src/main/home_screens.c` |
| `0x80061710–0x80062CD0` | `src/main/title_screen.c` |
| `0x80062CD0–0x800674B0` | `src/main/hud_panels.c` |
| `0x800674B0–0x8006CB30` | `src/main/hud_screens.c` |
| `0x8006CB30–0x800737A0` | `src/main/scheduler.c` |
| `0x800737A0–0x80075320` | `src/main/school_day.c` |
| `0x80075320–0x800789E0` | `src/main/holiday.c` |
| `0x800789E0–0x80079B10` | `src/main/overlay_loader.c` |
| `0x80079B10–0x8007C030` | `src/main/sound_driver.c` |
| `0x8007C030–0x80085E30` | `src/main/scene_library.c` |
| `0x80085E30–0x80086810` | `src/main/scene_display.c` |

## Overlay C files by current disc folder

The C-file counts below are a directory census; each link opens the exhaustive filename map for that overlay. Original-object record counts come from `config/objects/<UNIT>.txt` and are shown separately because they differ from C-file counts for DATE and TACO.

| Current source folder | C files | Original object records | Filename map |
|---|---:|---:|---|
| `src/ovl/BUNKAKEN/` | 18 | 18 | [BUNKAKEN table](../config/renames/proposals/layout-BUNKAKEN-files.tsv) |
| `src/ovl/BUNKASAI/` | 20 | 20 | [BUNKASAI table](../config/renames/proposals/layout-BUNKASAI-files.tsv) |
| `src/ovl/BUNKA_SD/` | 11 | 11 | [BUNKA_SD table](../config/renames/proposals/layout-BUNKA_SD-files.tsv) |
| `src/ovl/DATE/` | 19 | 22 | [DATE table](../config/renames/proposals/layout-DATE-files.tsv) |
| `src/ovl/DATE2/` | 6 | 6 | [DATE2 table](../config/renames/proposals/layout-DATE2-files.tsv) |
| `src/ovl/ENDING/` | 8 | 8 | [ENDING table](../config/renames/proposals/layout-ENDING-files.tsv) |
| `src/ovl/EN_NICHI/` | 2 | 2 | [EN_NICHI table](../config/renames/proposals/layout-EN_NICHI-files.tsv) |
| `src/ovl/ETC/` | 13 | 13 | [ETC table](../config/renames/proposals/layout-ETC-files.tsv) |
| `src/ovl/EVENT/` | 18 | 18 | [EVENT table](../config/renames/proposals/layout-EVENT-files.tsv) |
| `src/ovl/GEKO/` | 22 | 22 | [GEKO table](../config/renames/proposals/layout-GEKO-files.tsv) |
| `src/ovl/GYOZI/` | 33 | 33 | [GYOZI table](../config/renames/proposals/layout-GYOZI-files.tsv) |
| `src/ovl/KANGEI/` | 8 | 8 | [KANGEI table](../config/renames/proposals/layout-KANGEI-files.tsv) |
| `src/ovl/MASTER/` | 9 | 9 | [MASTER table](../config/renames/proposals/layout-MASTER-files.tsv) |
| `src/ovl/NAME_ENT/` | 4 | 4 | [NAME_ENT table](../config/renames/proposals/layout-NAME_ENT-files.tsv) |
| `src/ovl/OLH/` | 8 | 8 | [OLH table](../config/renames/proposals/layout-OLH-files.tsv) |
| `src/ovl/OMIMAI/` | 4 | 4 | [OMIMAI table](../config/renames/proposals/layout-OMIMAI-files.tsv) |
| `src/ovl/OPTION/` | 3 | 3 | [OPTION table](../config/renames/proposals/layout-OPTION-files.tsv) |
| `src/ovl/RENSYU/` | 2 | 2 | [RENSYU table](../config/renames/proposals/layout-RENSYU-files.tsv) |
| `src/ovl/RPG_BAT/` | 61 | 61 | [RPG_BAT table](../config/renames/proposals/layout-RPG_BAT-files.tsv) |
| `src/ovl/SHOUGATU/` | 27 | 27 | [SHOUGATU table](../config/renames/proposals/layout-SHOUGATU-files.tsv) |
| `src/ovl/SHUGAKU/` | 11 | 11 | [SHUGAKU table](../config/renames/proposals/layout-SHUGAKU-files.tsv) |
| `src/ovl/TACO/` | 38 | 41 | [TACO table](../config/renames/proposals/layout-TACO-files.tsv) |
| `src/ovl/TAIIKU/` | 19 | 19 | [TAIIKU table](../config/renames/proposals/layout-TAIIKU-files.tsv) |
| `src/ovl/TEL/` | 3 | 3 | [TEL table](../config/renames/proposals/layout-TEL-files.tsv) |
| `src/ovl/TT/` | 31 | 31 | [TT table](../config/renames/proposals/layout-TT-files.tsv) |
| `src/ovl/VALEN/` | 5 | 5 | [VALEN table](../config/renames/proposals/layout-VALEN-files.tsv) |

The overlay folder spellings remain the disc names in the current tree. English folder/header moves are proposed separately in T-27074 and are not applied here.

## Count and boundary sources

- Current main names/ranges: `src/main/README.md`, the `main` C entries in `config/SLPM_86.053.yaml`, and text ranges in `config/objects/main.txt`.
- Overlay folders and C-file counts: `src/ovl/<DISC>/`; the exhaustive logical basename map is [[spec/file-naming-overlays]].
- Original object records and boundaries: `config/objects/<UNIT>.txt`, generated by `tools/object_boundaries.py`.
- Function census method: `tools/srcscan.py` over the configured C-source set. Current verified definition totals are 830 main and 6,122 overlays.

## Historical main boundary analysis (address-basename stage)

The following analysis records the address-basename boundary stage documented on **2026-10-09**. Its file names and census figures are historical; the current inventory and paths are listed above. The exact original boundaries remain recorded in the current table and `config/objects/`.

## Evidence

1. **Alignment padding (decisive, 27 boundaries).** The original linker aligned each object's `.text` to 16 bytes; functions inside an object follow each other without padding (804 of the 833 function ends touch the next function's start). A run of zero words after a function that ends at a non-16-aligned address and ends exactly at the next 16-byte boundary is therefore the padding in front of a new object. 27 gaps fit (3 to 12 bytes of nops, next function 16-aligned). Two gaps do not fit and are not used: `0x80067E24-0x80067E34` (16 bytes, next function at +4 mod 16) and `0x8006B8EC-0x8006B900` (20 bytes); they are runs of nops that may belong to either neighbouring object, so `80062CD0` stays large. An object whose size is a multiple of 16 leaves no padding at all: with 27 visible boundaries about a quarter of the true boundaries (roughly 9 of about 36) are invisible, so the large files are expected to hide further boundaries.
2. **Entry point (earlier rejected split).** `0x800420D0` is the PS-X EXE entry (header PC), 16-aligned, and the string at `0x800AF370` used by the code after it starts a new rodata object, while the previous string, `0x800AF35C`, is used by `func_800412E0`. At the address-basename stage a boundary was constrained to (`func_800412E0`, `func_80042134`], with candidates `80041840`, `800418B0`, `800420D0`; that pass did not split it. The later object inventory records `800420D0` as a boundary, and the current logical file is `src/main/main_loop.c`.
3. **Rodata object starts agree.** `.rodata` objects start 16-aligned (a zero run of 5 or more bytes ends 16-aligned in front of them). Of the 8 such starts that are referenced exactly and fall between two functions, 6 have a padding boundary inside the interval of code between the last function using lower rodata and the first function using the new object (`800AF390` -> `80042540`; `800AFB00` -> `8004AD60`/`8004C3F0`; `800AFB70` -> `8004F6B0`/`8004F870`; `800AFE10` -> `8005A0B0`; `800B1590` -> `80075320`; `800B2000` -> `800789E0`/`80079B10`). The other two need a boundary the padding did not reveal: `800AF370` (point 2) and `800AFE00` (used by `func_80059E00`; boundary in (`func_80059B04`, `func_80059E00`] inside `80059A20`, candidates `80059B40`, `80059BC0`, `80059E00`, not decidable). The earlier pass left both unsplit; the later object inventory records `80059B40` as a boundary after the `800AFE00` chunk. No rodata start contradicted a padding boundary.
4. **Data and bss follow file order.** Of 1714 distinct `.data`/`.bss` addresses referenced from game code, 1338 are used by one file only; 94% of address-adjacent pairs of those (1261 of 1337) are in file order, i.e. each file's private data sits in one contiguous block in link order. This is the property a later per-file data split relies on ([[tickets/T-0500-per-file-game-rodata-data-bss-split]]). It does not locate boundaries inside the big files.
5. **Call graph.** 881 of the 3680 calls between game functions stay inside a file (24%; a random split into files of these sizes would give about 7%), and 386 of 680 called functions are called from their own file only (static-looking). Both numbers rise when the boundaries are right; they are used as a sanity check, not to place boundaries (leaf-heavy files have zero crossing calls at many places).

Candidates rejected: data-only separation (single-file symbols below vs above a point) gave only 4 points (`8004ECE0`..`8004EE10`, `8006BA40`), all dominated by shared structs, so no split.

## Alignment in the build
The linker script keeps `SUBALIGN(2)` (the SDK asm objects must be packed exactly), which would drop the original linker's 16-byte alignment of each C object. `tools/cc.py` therefore zero-pads every object's `.text` to a multiple of 16 after compiling (a uniform toolchain emulation of the original link, see CODING_STANDARDS 7a). Functions kept as `INCLUDE_ASM` already carry their trailing padding nops in their `.s`, so the padding is never doubled. Consequence: the last function of a file can now be decompiled; `func_80043504` (end of `80042A00`) and `func_8006CAE0` (end of `80062CD0`) were listed as unmatchable in [[matching-notes]] for this reason; `func_80043504` as `void func_80043504(void) {}` was checked to match with the padding, but was left as `INCLUDE_ASM` so that the split commit leaves the progress count unchanged (75/834).

## Original objects, text and rodata (T-0500)
`tools/docker.sh python3 tools/object_boundaries.py [--write] [-v] [UNIT...]` (tests `tools/test_objects.py`) derives the original objects (translation units) of the main game code and of all 26 overlays from the original bytes and splat's asm, and `--write` records them in `config/objects/<UNIT>.txt` (one `object` line per object: text range, rodata chunk, island yes/no, evidence for both starts; `rodata_end`; `orphan` chunks). `tools/split_objects.py` turns them into C files ([[build-system]], "Per-object C files").

The link put the objects in the same order in every section and started each object's section on 16 bytes; IDO writes an object's `.rodata` as [strings and constants][jump tables] and pads it to 16. Evidence, by kind:

| boundary | evidence | confidence | count (all units) |
|---|---|---|---|
| text | `pad`: zero words after a function up to the next 16-byte boundary (as in [[source-files#Evidence]] point 1) | high | 359 |
| text | `rodata`: two rodata objects are used by functions of one padding-delimited text range; exactly one 16-aligned function start (no gap before it) lies between the last user of the first and the first user of the second | high | 25 |
| text | `choice/N`: as `rodata` with N candidates; the one with the fewest calls crossing it is taken. The build cannot tell (every candidate links the same bytes; only functions between the users move) | low | 29 |
| rodata | `jtbl-pad`: a jump table followed by zero words up to the next 16 bytes (table words are never zero) | high | 196 |
| rodata | `jtbl-end`: a jump table followed by something that is not a table, its end already 16-aligned (tables end an object) | high | 36 |
| rodata | `str-pad`: a string followed by more zeros than its 4-byte alignment needs, up to 16 | medium (an unreferenced zero constant would look the same) | 59 |
| rodata | `owner`: symbols on both sides are used by different text objects (one 16-aligned symbol start in between) | high | 39 |
| rodata end (overlays) | `last-jtbl` (20), `monotone-strings` (6): the end of the last table's object, extended over following string chunks while their users stay in text order and nothing in them is written by code or holds a pointer | medium | 26 |

Historical totals from the **2026-10-09** boundary run: 440 objects (main 34, overlays 406) and 357 rodata chunks; every chunk was an island candidate (`island_check`: the order asm-processor reproduced equaled the original, and every `INCLUDE_ASM` function was long enough for asm-processor to emit its tables). That run recorded 3 orphan chunks staying asm: BUNKASAI `8015F730-8015FF70` and `8015FF70-80160380` (second and third chunk of text object `801511E0`, so a text boundary there is hidden) and TACO `8015DA90-8015DAA0` (second chunk of `8013DB80`). Coverage in that run: 880 of 902 jump-table functions (782 KB of 810 KB) and 1495 of 1518 functions that use game rodata lay in an island object. Current configured record counts are summarized above; these earlier coverage figures are retained as historical evidence.

Cross-checks that hold: rodata order follows text order in every unit (users of consecutive chunks never overlap except in the three orphans); each object's `.data` also follows (OMIMAI: the tables of the three switch objects at `80134A60`, `80134B60`, `80134C30`); a full trial migration of all 27 units ([[tickets/T-0500-per-file-game-rodata-data-bss-split]]) rebuilt 27 of 27 sha1 from a clean `asm/`, which checks every island's size and content (not the low-confidence text cuts, see above).

Problems the script reports (not used as evidence): 14 text gaps that are not object padding (as `0x80067E24-0x80067E34` in main), the three orphans, IDO tables that end on 16 bytes followed by 16 more zero bytes (TACO `8015E210`): the object ends at the table, the zeros belong to what follows (here `.data`); a boundary between two symbols gets a synthetic symbol `D_<address>` (splat names the first symbol of a subsegment so).

Main exe: the earlier 28 address-named source files resolved into 34 original text-object ranges. Six cuts were `choice/N` (`800420D0` among 3 candidates, `800490C0`, `800674B0`, `800737A0`, `80059B40`, `8007C030`); the rodata start `800AFE00` separates `80059A20` (chunk `800AFDF0`, an `owner` cut) from `80059B40` (`choice/3`). The current 34 logical filenames and ranges are listed above.

## Data and bss ranges (T-9010)
`tools/object_boundaries.py --data --write` adds one `data` (main exe also `bss`) line per object it can place to `config/objects/<UNIT>.txt`. Regions: overlays from the end of the rodata to the end of the 0x30000 file (no `.bss`: every overlay variable is in the file); main exe `.data` 0x800B3220-0x800E3800 (game and SDK) and `.bss` 0x800E3800-0x8012B538.

Evidence per data symbol (splat item): **at** (weight 10) the symbol is stored through a `lui $at` that a function of the object shares with another store (IDO and the original share `$at` only for one variable defined in the same file); **ref** (weight 1) the functions of exactly one object name it, or its pointer words all point into one object. The heaviest chain of evidence whose objects never decrease in address order places the objects (the link put every object's data in text order). A neighbour pair without a 16-aligned item start between them drops the weaker item (a global of the other object that only one object uses; reported). Boundaries are 16-aligned item starts: `owner` (exactly one candidate), `owner-min/N` (N candidates, the tightest range for each side, the bytes between stay without owner), `start`/`end` (region edge). Measured over the overlays: 173 of 216 single-user transitions to a later object fall on a 16-aligned address, 28 go back to an earlier object (globals).

Result (2026-10-10): 248 ranges over the 27 units (main: 20 `.data`, 7 `.bss`); most are `owner-min` on one side. Low confidence where only `ref` evidence places an object (a global defined in another file but used by one object looks the same). Known gaps: TACO objects `801430B0`-`80149F90` and TT/TAIIKU objects whose data starts unaligned (no range), main `D_800E36C0` (shared `$at` in `InitMouse`, out of object order), RPG_BAT objects 2-45 (data is one pool of globals: no single-user evidence). `.bss` of the main exe: GameState `D_800E6280` falls to the first object, many single-user items are struct members (T-5100), so these ranges are weak.

## Not split: data, bss (before T-9010)

`.data` and `.bss` stay whole; rodata is split per object since T-0500 (above). Only about 80 game functions reference `.rodata` directly (most strings are reached through `.data` pointer tables), and per-file `.data`/`.bss` blocks interleave with shared globals; cutting them needs symbol-level ownership. Tracked in [[tickets/T-0500-per-file-game-rodata-data-bss-split]].

Exception (T-1340): a C file that contains a jump-table function gets a rodata *island*, the chunk of rodata of one original object, provided by the C object itself (`.rodata` subsegment named like the file; current examples include `src/main/memory_card.c` and `src/main/sound_driver.c`). It is cut from the splat output, not from a claim about file ownership: the chunk is the strings and tables of one object up to its zero padding, and the sha1 check confirms it. See [[build-system]] (section "Jump tables: rodata islands"). The zero padding after a jump table is also a new rodata boundary witness for T-0500: tables ending in zero words up to a 16-byte boundary mark the end of an object (70 cases in the overlays).
