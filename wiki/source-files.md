---
type: concept
updated: 2026-10-09
sources: ["tools/game_boundaries.py", "tools/object_boundaries.py", "config/objects/", "config/SLPM_86.053.yaml", "src/main/", "tools/cc.py"]
---

# Source files of the game code

The game code (834 functions, 0x80041000-0x80086810) was one C file before [[tickets/T-0012-game-file-boundaries-and-shift-jis]]. It is now split into 28 files `src/main/<start address>.c`, one `c` subsegment each in `config/SLPM_86.053.yaml`; `configure.py` reads that list, so adding a boundary needs only a yaml line and moving the functions. Each file is a union of one or more original translation units: boundaries are only placed where the evidence is clear, so a file may still hold several originals ("fewer, larger files"). Names are neutral (address) because no file names are known; rename a file only through the yaml and `git mv`.

Re-run the evidence: `tools/docker.sh python3 tools/game_boundaries.py` (needs `asm/`; `--yaml` prints the segment list).

## Files
| file | size | functions |
|---|---|---|
| `src/main/80041000.c` | 0x1540 | 24 |
| `src/main/80042540.c` | 0x4C0 | 8 |
| `src/main/80042A00.c` | 0xB10 | 10 |
| `src/main/80043510.c` | 0x1CC0 | 26 |
| `src/main/800451D0.c` | 0x1330 | 23 |
| `src/main/80046500.c` | 0x1050 | 5 |
| `src/main/80047550.c` | 0x2AA0 | 30 |
| `src/main/80049FF0.c` | 0xD70 | 7 |
| `src/main/8004AD60.c` | 0x1690 | 13 |
| `src/main/8004C3F0.c` | 0x2110 | 11 |
| `src/main/8004E500.c` | 0x11B0 | 25 |
| `src/main/8004F6B0.c` | 0x1C0 | 1 |
| `src/main/8004F870.c` | 0x3DE0 | 35 |
| `src/main/80053650.c` | 0x2CB0 | 40 |
| `src/main/80056300.c` | 0xF0 | 1 |
| `src/main/800563F0.c` | 0xFA0 | 11 |
| `src/main/80057390.c` | 0x1990 | 13 |
| `src/main/80058D20.c` | 0x9F0 | 12 |
| `src/main/80059710.c` | 0x310 | 7 |
| `src/main/80059A20.c` | 0x690 | 8 |
| `src/main/8005A0B0.c` | 0x7660 | 109 |
| `src/main/80061710.c` | 0x15C0 | 21 |
| `src/main/80062CD0.c` | 0x9E60 | 85 |
| `src/main/8006CB30.c` | 0x87F0 | 75 |
| `src/main/80075320.c` | 0x36C0 | 41 |
| `src/main/800789E0.c` | 0x1130 | 12 |
| `src/main/80079B10.c` | 0xC320 | 165 |
| `src/main/80085E30.c` | 0x9E0 | 16 |

Sizes include the alignment padding that follows the last function. Largest: `80079B10` (165 functions), `8005A0B0` (109), `80062CD0` (85); the first file `80041000` (24 functions) also holds the entry point `0x800420D0`.

## Evidence

1. **Alignment padding (decisive, 27 boundaries).** The original linker aligned each object's `.text` to 16 bytes; functions inside an object follow each other without padding (804 of the 833 function ends touch the next function's start). A run of zero words after a function that ends at a non-16-aligned address and ends exactly at the next 16-byte boundary is therefore the padding in front of a new object. 27 gaps fit (3 to 12 bytes of nops, next function 16-aligned). Two gaps do not fit and are not used: `0x80067E24-0x80067E34` (16 bytes, next function at +4 mod 16) and `0x8006B8EC-0x8006B900` (20 bytes); they are runs of nops that may belong to either neighbouring object, so `80062CD0` stays large. An object whose size is a multiple of 16 leaves no padding at all: with 27 visible boundaries about a quarter of the true boundaries (roughly 9 of about 36) are invisible, so the large files are expected to hide further boundaries.
2. **Entry point (rejected split).** `0x800420D0` is the PS-X EXE entry (header PC), 16-aligned, and the string at `0x800AF370` used by the code after it starts a new rodata object, while the previous string, `0x800AF35C`, is used by `func_800412E0`. A boundary must therefore lie in (`func_800412E0`, `func_80042134`]; the 16-aligned candidates are `80041840`, `800418B0`, `800420D0`. The entry point is the natural choice, but no padding gap proves it, so under the rule "fewer, larger files when evidence is weak" it is not split: `80041000` keeps the entry stub and the game main. `INFERRED` in `tools/game_boundaries.py` is empty for this reason; add `0x800420D0` there (and a yaml line) to split it.
3. **Rodata object starts agree.** `.rodata` objects start 16-aligned (a zero run of 5 or more bytes ends 16-aligned in front of them). Of the 8 such starts that are referenced exactly and fall between two functions, 6 have a padding boundary inside the interval of code between the last function using lower rodata and the first function using the new object (`800AF390` -> `80042540`; `800AFB00` -> `8004AD60`/`8004C3F0`; `800AFB70` -> `8004F6B0`/`8004F870`; `800AFE10` -> `8005A0B0`; `800B1590` -> `80075320`; `800B2000` -> `800789E0`/`80079B10`). The other two need a boundary the padding did not reveal: `800AF370` (point 2) and `800AFE00` (used by `func_80059E00`; boundary in (`func_80059B04`, `func_80059E00`] inside `80059A20`, candidates `80059B40`, `80059BC0`, `80059E00`, not decidable). Both are left unsplit. No rodata start contradicts a padding boundary.
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

Totals: 440 objects (main 34, overlays 406), 357 rodata chunks, every chunk an island candidate (`island_check`: the order asm-processor reproduces equals the original, and every `INCLUDE_ASM` function is long enough for asm-processor to emit its tables). 3 orphan chunks stay asm: BUNKASAI `8015F730-8015FF70` and `8015FF70-80160380` (second and third chunk of the text object `801511E0`, so a text boundary there is hidden) and TACO `8015DA90-8015DAA0` (second chunk of `8013DB80`). Coverage: 880 of the 902 jump-table functions (782 KB of 810 KB) and 1495 of the 1518 functions that use game rodata lie in an island object.

Cross-checks that hold: rodata order follows text order in every unit (users of consecutive chunks never overlap except in the three orphans); each object's `.data` also follows (OMIMAI: the tables of the three switch objects at `80134A60`, `80134B60`, `80134C30`); a full trial migration of all 27 units ([[tickets/T-0500-per-file-game-rodata-data-bss-split]]) rebuilt 27 of 27 sha1 from a clean `asm/`, which checks every island's size and content (not the low-confidence text cuts, see above).

Problems the script reports (not used as evidence): 14 text gaps that are not object padding (as `0x80067E24-0x80067E34` in main), the three orphans, IDO tables that end on 16 bytes followed by 16 more zero bytes (TACO `8015E210`): the object ends at the table, the zeros belong to what follows (here `.data`); a boundary between two symbols gets a synthetic symbol `D_<address>` (splat names the first symbol of a subsegment so).

Main exe: the 28 files become 34 objects. Six new text cuts are `choice/N` (`800420D0` among 3 candidates, which is the entry point of point 2 above, `800490C0`, `800674B0`, `800737A0`, `80059B40`, `8007C030`); the rodata start `800AFE00` of point 3 now separates `80059A20` (chunk `800AFDF0`, an `owner` cut) from `80059B40` (`choice/3`). Only `src/main/80062CD0.c` has been split so far (into `80062CD0` and `800674B0`); the other files follow when [[tickets/T-3050-run-per-object-migration-after-wave-2]] runs.

## Data and bss ranges (T-9010)
`tools/object_boundaries.py --data --write` adds one `data` (main exe also `bss`) line per object it can place to `config/objects/<UNIT>.txt`. Regions: overlays from the end of the rodata to the end of the 0x30000 file (no `.bss`: every overlay variable is in the file); main exe `.data` 0x800B3220-0x800E3800 (game and SDK) and `.bss` 0x800E3800-0x8012B538.

Evidence per data symbol (splat item): **at** (weight 10) the symbol is stored through a `lui $at` that a function of the object shares with another store (IDO and the original share `$at` only for one variable defined in the same file); **ref** (weight 1) the functions of exactly one object name it, or its pointer words all point into one object. The heaviest chain of evidence whose objects never decrease in address order places the objects (the link put every object's data in text order). A neighbour pair without a 16-aligned item start between them drops the weaker item (a global of the other object that only one object uses; reported). Boundaries are 16-aligned item starts: `owner` (exactly one candidate), `owner-min/N` (N candidates, the tightest range for each side, the bytes between stay without owner), `start`/`end` (region edge). Measured over the overlays: 173 of 216 single-user transitions to a later object fall on a 16-aligned address, 28 go back to an earlier object (globals).

Result (2026-10-10): 248 ranges over the 27 units (main: 20 `.data`, 7 `.bss`); most are `owner-min` on one side. Low confidence where only `ref` evidence places an object (a global defined in another file but used by one object looks the same). Known gaps: TACO objects `801430B0`-`80149F90` and TT/TAIIKU objects whose data starts unaligned (no range), main `D_800E36C0` (shared `$at` in `InitMouse`, out of object order), RPG_BAT objects 2-45 (data is one pool of globals: no single-user evidence). `.bss` of the main exe: GameState `D_800E6280` falls to the first object, many single-user items are struct members (T-5100), so these ranges are weak.

## Not split: data, bss (before T-9010)

`.data` and `.bss` stay whole; rodata is split per object since T-0500 (above). Only about 80 game functions reference `.rodata` directly (most strings are reached through `.data` pointer tables), and per-file `.data`/`.bss` blocks interleave with shared globals; cutting them needs symbol-level ownership. Tracked in [[tickets/T-0500-per-file-game-rodata-data-bss-split]].

Exception (T-1340): a C file that contains a jump-table function gets a rodata *island*, the chunk of rodata of one original object, provided by the C object itself (`.rodata` subsegment named like the file; `src/main/80053650.c` and `src/main/80079B10.c` have one). It is cut from the splat output, not from a claim about file ownership: the chunk is the strings and tables of one object up to its zero padding, and the sha1 check confirms it. See [[build-system]] (section "Jump tables: rodata islands"). The zero padding after a jump table is also a new rodata boundary witness for T-0500: tables ending in zero words up to a 16-byte boundary mark the end of an object (70 cases in the overlays).
