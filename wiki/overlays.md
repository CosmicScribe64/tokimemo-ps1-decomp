---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md", "configure.py", "tools/gen_overlay_configs.py", "config/overlays.txt"]
---

# Overlays

26 headerless code overlays in `CDROM/EXEDIR/*.EXN`, each exactly 0x30000 bytes (96 sectors): BUNKAKEN, BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, MASTER, NAME_ENT, OLH, OMIMAI, OPTION, RENSYU, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TEL, TT, VALEN. All rebuild byte-identical ([[build-system]]), ticket [[tickets/T-0008-overlay-load-address-and-split]].

## How the main exe loads them
There are no file-name strings in the exe (no `EXEDIR`, no `.EXN`); overlays are read by CD sector number.
- `func_80078C48` picks a sector from the mode byte `D_800E738E` (jump table `jtbl_800B19E8`, modes 0x11-0x91, plus 0xC2-0xC4 and 0xD0), stores the sector count 0x60 in `D_800B6D38`, the destination in `D_800B6D3C` (always 0x80132000 there) and the sector in `D_800B6D40`, then calls the CD read `func_80046318(0x60, buffer, sector)`. `func_8007959C` does the same for TT (sector 0x9A6A).
- `func_80079070` sets the entry pointer `D_80123110` per mode (jump table `jtbl_800B1BEC`); the main loop `func_80042540` runs it with `jalr $t9` (`lw $t9, D_80123110`). `func_80041840` sets it to 0x80132000.
- `func_8004636C(sectors, buffer)` verifies the load: the 16-bit byte sum of the first `sectors * 0x800 - 4` bytes must equal the big-endian halfword at offset 0x2FFFC. The last 4 bytes of every file are `<sum hi> <sum lo> 0x12 0x10`; all 27 files pass this check (script run in Docker, T-0008), which also shows the files are exactly what the game reads.
- Sectors match the disc: BUNKAKEN 0x910A, then +0x60 per file in alphabetical order (BUNKASAI 0x916A ... VALEN 0x9ACA; O.BIN is 0x964A, between NAME_ENT and OLH). Found by searching `track1.bin` for each file's first sector; the literals in the exe agree for 24 of the 26 overlays.

## Load address
Evidence for 0x80132000, the base of the 0x30000-byte slot (ends 0x80162000):
- The exe stores 0x80132000 as the read buffer and as the default entry (`D_800B6D3C`, `D_80123110`), and the main exe's other overlay-space addresses (0x80132000-0x8014C08C) are the per-mode entries.
- In the 24 overlays at 0x80132000 (all but EVENT and GYOZI), `jal` targets inside the slot land on function starts (an `addiu $sp, $sp, -N` prologue, or the instruction after `jr $ra` plus delay slot) for 58-100% of targets (TT 755 of 814, TACO 1279 of 1432, RPG_BAT 3448 of 3512, OLH 70 of 74, OMIMAI the low end with 7 of 12). A wrong base gives about 3% (GYOZI tested at 0x80132000: 5 of 153). The misses are most likely data words that look like `jal`.
- Jump-table vote over all (target, prologue) pairs also puts TACO's best base at 0x80132000.

Overlays do not all share one address:
- EVENT.EXN: jal targets fit only at 0x800F6000 (329 of 400; 0 at 0x80132000).
- GYOZI.EXN: only at 0x80134000 (117 of 153; 5 at 0x80132000).
- Neither sector (0x940A, 0x94CA) appears as a literal in the exe or any overlay, so no loader was found; EVENT at 0x800F6000 would overlap main-exe bss such as `D_80123110`. Both addresses are inference only, see [[tickets/T-0200-event-gyozi-loader-and-address]]. A wrong address would only rename symbols; the rebuild stays byte-identical.

## Entry points
The entry is the address `D_80123110` receives; several overlays have more than one (one per mode). Offsets are from the load address.

| overlay | entries (vram) |
|---|---|
| TACO (default for 85 modes), EN_NICHI, OLH | 0x80132000 |
| BUNKAKEN | 0x80144F70 |
| BUNKASAI | 0x80142700 |
| BUNKA_SD | 0x80137890 |
| DATE | 0x801448D4, 0x8014C08C |
| DATE2 | 0x80132214, 0x80137F64 |
| ENDING | 0x80132334 |
| ETC | 0x801407F0, 0x801465F8, 0x801492C0, 0x8014ABD0 |
| GEKO | 0x80132244, 0x80134040, 0x80134C90, 0x8013882C, 0x801426D0 |
| KANGEI | 0x80132000, 0x80132FF8 |
| MASTER | 0x80132140, 0x80133A2C, 0x80139150 |
| NAME_ENT | 0x8013209C, 0x8014231C, 0x80144AD0, 0x8014AF28 |
| OMIMAI | 0x80132000, 0x80133120 |
| OPTION | 0x801335E8, 0x801389A0, 0x8013C780 |
| RENSYU | 0x80132040 |
| RPG_BAT | 0x8013B5B0 |
| SHOUGATU | 0x801322D4, 0x80137144, 0x8013B484, 0x8013D180 |
| SHUGAKU | 0x80137490 |
| TAIIKU | 0x80141280 |
| TEL | 0x80132000, 0x801397D0 |
| VALEN | 0x80132274 |
| TT | 0x80132000 (via `func_8007959C`) |

(Mode-to-entry pairing is in `func_80079070`; EVENT, GYOZI: unknown.) The table comes from decoding the two jump tables with a small script (loader sector -> overlay, entry per mode).

## O.BIN
Not loaded by the game (its sector 0x964A is not referenced anywhere) but it has the same 96-sector size and checksum trailer. It is a little-endian MIPS ECOFF link (1995-07-25) of an OLH overlay skeleton at 0x80132000 against the symbol map of a developer build of the main exe (2146 names: 14 overlay-side, 2132 main-program). It has no main-program code, no source file names and no types. Format, mapping onto SLPM_86.053 (370 new high-confidence names plus 154 PsyQ names already known; 0 overlay) and tools: [[obin]]. Not split.

## Splitting and build
`tools/gen_overlay_configs.py` writes `config/overlays/<NAME>.yaml` (splat, `ovl_<NAME>`), `<NAME>.sha1` (sha1 of the original file) and `config/overlays.txt` (name, load address, text size). Each overlay is generated as two parts: a C-capable code segment `src/ovl/<NAME>.c` (every function `INCLUDE_ASM`, built with IDO 5.3 like the main game code, [[toolchain]]) from offset 0 to the end of the last `jr $ra` rounded up to 16, and one `rodata` segment for the rest (strings, jump tables, data, trailer). `tools/split_objects.py` (T-0500) then turns an overlay into one C file per original object (`src/ovl/<NAME>/<addr>.c`), one rodata island per object and a `<NAME>_data` subsegment for the rest; the objects of all 26 overlays are in `config/objects/<NAME>.txt` ([[source-files]], [[build-system]]). Migrated so far: RENSYU, OMIMAI, TEL, OLH. bss is not in the file.
- Caution: `config/symbol_addrs.txt` applies by address to every overlay. Names for addresses in the shared slot (0x80132000 and up) must therefore go into a per-overlay symbol file such as `config/overlays/<NAME>_symbols.txt`, never into the shared file; only main-exe addresses (below 0x80132000, except inside EVENT's range) belong there.
- Symbols are shared with the main exe: every overlay config reads `config/symbol_addrs.txt` and `config/reloc_addrs.txt`, so a rename of a main-exe function or variable appears in all overlays' asm. Unresolved calls into the main exe (`jal 0x8004xxxx`) become `func_8004xxxx` in the per-overlay `build/ovl/<NAME>_undefined_funcs_auto.txt`, which the overlay link includes, so they resolve to the real main-exe addresses.
- The 131 addresses the main exe leaves undefined (`build/undefined_syms_auto.txt`) lie in the shared slot; each overlay maps different code there, so they cannot be resolved from one overlay. They stay auto-defined for the main link.
- DATE starts with a lone `nop` before its entry code; `config/overlays/DATE_symbols.txt` gives `func_80132000` its full size so asm-processor does not see a 4-byte function.

Open: [[tickets/T-0200-event-gyozi-loader-and-address]].
