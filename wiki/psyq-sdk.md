---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md", "config/SLPM_86.053.yaml", "config/symbol_addrs.txt", "tools/psyq_sigmatch.py"]
---

# PsyQ SDK findings (T-0006, T-0010)

## Verdict
- Not one release. Library by library the executable matches different PsyQ vintages, all of the 3.x family (late 1995; the game shipped Nov 1995 and the newest RCS id is 1995/10/18). Per-library pins are in the table below; the leftover conflict is [[tickets/T-0302-sdk-version-conflict]].
- No "PsyQ" or "Library Programs" version string exists in the exe.
- Dated RCS ids: libgpu `sys.c,v 1.107 1995/10/18` (0x800B2E50), libetc `intr.c,v 1.71 1995/08/29` (0x800B3170), libsnd `ut_f.c,v 1.2 1995/03/13` (0x800DCB50), and `Copyright (C) by 1993, 1994 Sony Computer Entertainment Inc.` (0x800DCB85, libsnd/libspu side data).

## Method: signature matching
Source: [lab313ru/psx_psyq_signatures](https://github.com/lab313ru/psx_psyq_signatures): public JSON byte signatures (relocation bytes wildcarded), one file per library for PsyQ 2.6, 3.0, 3.3, 3.4, 3.5, 3.61, 3.7 and 4.0-4.7. It holds signatures only, no SDK binaries. Downloaded into the gitignored `tools/psyq_sigs/`:
`curl -sL https://github.com/lab313ru/psx_psyq_signatures/archive/refs/heads/main.tar.gz | tar xz`, then copy the numbered version dirs and `syscalls/*.txt` into `tools/psyq_sigs/`.
Tools (run through `tools/docker.sh`):
- `tools/psyq_sigmatch.py EXE tools/psyq_sigs [--funcs] [--min-len N]`: exact masked match of whole objects, or of each named function.
- `tools/psyq_sigfuzzy.py EXE tools/psyq_sigs OUT`: scores an object at anchored candidates (fraction of literal bytes that agree).
Real .LIB/.OBJ files (needed for decompals/psyq-obj-parser) only come from a user-owned SDK; none were used or downloaded.

Caveats: a short chunk can collide, so names were accepted only for unique hits of at least 48 bytes whose length equals the exe function (+-8 bytes), and for the 16-byte libapi stubs by byte identity. Where a stub has an old and a new spelling (`InitPAD`/`InitPAD2`, `delete`/`erase`) the old one is used because the exe's `_96_remove` is the pre-3.6 form.

## Per-library pins (exact size or byte evidence)
| library | evidence | version |
|---|---|---|
| libgte | `COR_00.OBJ` is 432 bytes at 0x8009F8AC (384 in 3.3, 460 in 3.5+); `MTX.OBJ` (1216 bytes) and `MSC01.OBJ` (288 bytes) exist only in 3.3/3.4 (3.5+ split them) | 3.4 |
| libc | `SPRINTF.OBJ` 3456 bytes (0x800AE130), `MEMMOVE.OBJ` 144 bytes (0x800AEF20); 3.4 has 2880/128, 3.5 2140/108 | 3.3 (sprintf also 3.0) |
| libcard | `CARD.OBJ` 48 bytes at 0x800AE030 | 3.3 |
| libapi | `COUNTER.OBJ` 368 bytes (0x800AF030; 3.0, 3.3, 4.1+); `_96_remove` is the 16-byte form (<= 3.5) | <= 3.5, 3.3 fits |
| libgpu | `SYS.OBJ` starts exactly at 0x8009C210 (`ResetGraph` is its offset 0); `get_mode`/`get_ofs` chunks match 3.3-3.61 only | <= 3.61 |
| libcd | `CdComstr`, `CdIntstr` (3.5+), `CdDataCallback` (3.7+); `EVENT.OBJ` 256 bytes (4.1/4.2) vs 264 (<= 3.7) | >= 3.5, conflicts |
| libsnd | `UT_VVOL` chunk matches 3.61-4.0; `UT_GVA`/`UT_GPA` score 0.91/0.87 against 4.1+ | 3.6x or newer |
| libpress | `DecDCTReset` instruction order equals 4.1+; no `LIBPRESS`/`VLC`/`ENCSPU` object of any set matches | unresolved |

Conflict: libgte 3.4 and libc/libcard 3.3 are older than libcd/libsnd/libpress. Either the game linked libs from several installs or patches, or some signatures coincide. The release date (Nov 1995) and RCS ids (<= 1995/10/18) make 4.x unlikely, so the SDK is narrowed from "3.6-4.0" to "3.3-3.6 plus a few newer-looking components". Follow-up: [[tickets/T-0302-sdk-version-conflict]].

## Game/SDK boundary
The first SDK function is `DecDCTReset` at 0x80086810 (first function of `LIBPRESS.OBJ` in every signature set). This is the former heuristic start and it is correct: libpress does not start lower. The switch table at 0x800B2398 belongs to the game function `func_80085CD4` (IDO style, writes game globals), and the first libpress string is at 0x800B23B0. `src/game.c` keeps all its functions (the ~22 decompiled ones are all below 0x8006B640). A 48-byte signature hit at 0x80042C94 (libgpu `KanjiFntClose`) is a mid-function false positive inside game code.

## Layout of the library region (link order, 0x80086810-0x800AF340)
Segments are asm-only in `config/SLPM_86.053.yaml`.

| vram | segment | confidence | evidence |
|---|---|---|---|
| 0x80086810 | `libpress_main` | high start | DecDCTReset; libpress rodata refs 0x800B23B0-0x800B2460 |
| 0x80086E50 | `libpress_vlc` | medium | `MDEC_vlc_brk` (VLC.OBJ, 3.3/3.4) |
| 0x800871C0 | `libpress_obj2` | low | VLC.OBJ size (868 bytes) ends here; object unidentified |
| 0x80087730 | `libcd_event` | high | `CdInit` (EVENT.OBJ), first ref to "CdInit: Init failed" |
| 0x80087830 | `libcd_rest` | high start | SYS.OBJ (CdStatus/CdMode) then BIOS, ISO9660, stream C_0xx objects; ends at 0x8008B7B0 |
| 0x8008B7B0 | `libsnd` | medium | `_SsInit` (calls ResetCallback and the SPU callbacks); libcd C_010/C_008/C_002 sizes precede it |
| 0x800949B0 | `libspu` | medium | SPU.OBJ-sized block, first libspu data ref 0x800DCBD8, libsnd UT_ objects end here |
| 0x80099E30 | `libgs` | medium | MATRIX objects, first libgs rodata ref 0x800B2C00 |
| 0x8009C210 | `libgpu` | high | SYS.OBJ starts with ResetGraph; EXT/E04 ends 0x8009F678 |
| 0x8009F680 | `libgte_*` (36 objects) | high | whole-object exact matches from the 3.4 set, sizes agree; unmatched spans are `libgte_<addr>` segments |
| 0x800AD280 | `libetc` | high | `INTR.OBJ` (ResetCallback, DMACallback, VSync...) directly after DIVGT4A, which ends exactly at 0x800AD280; runs to 0x800ADFC0 |
| 0x800ADFC0 | `libapi_0`-`libapi_4`, `libcard_card`, `libc_sprintf`, `libc_memmove`, `libapi_counter` | high | 16-byte BIOS stubs (one object each, A##/C##.OBJ), CARD.OBJ, SPRINTF.OBJ, MEMMOVE.OBJ, COUNTER.OBJ |

`config/symbol_addrs.txt` carries 166 real names: 61 libapi stubs (`open`, `close`, `read`, `write`, `EnterCriticalSection`, `InitHeap`, `memcpy`, `strlen`, ...), libgte (including the `GsTMDfast*` routines), libetc, libc, libcard, libgs/libgpu leaves, and libpress/libcd/libsnd entry points. `GsDrawOt` and `GsDrawOtIO` are byte-identical at 0x8009AD00 and left as `func_8009AD00`.

Not yet split: objects inside `libcd_rest`, `libsnd`, `libspu`, `libgs`, `libgpu` ([[tickets/T-0300-sdk-object-split-remaining-libs]]) and the lib rodata/data ([[tickets/T-0301-sdk-rodata-data-split]]).

## Library evidence (rodata/data anchors)
| library | evidence | code anchor |
|---|---|---|
| libpress (MDEC) | `MDEC_rest:bad option`, `MDEC_in_sync`, `MDEC_vlec: invalid VLC ID` (rodata 0x800B23B0) | 0x800869EC-0x800871C0 |
| libcd | `CdInit: Init failed`, `CdSearchFile: searching %s...`, `Cdl*` names (0x800B2480-0x800B27E4) | 0x80087730 |
| libsnd | `Can't Open Sequence data any more` (0x800B2930) | 0x800900A0 |
| libspu | `SPU:T/O [%s]` (0x800DCC24) | 0x8009694C |
| libgpu | `ResetGraph`, `DrawSync`, `LoadImage` (0x800B2E84-0x800B2F84) | 0x8009C210 |
| libetc | `VSync: timeout` (0x800B31E0), `intr.c` id | VSync at 0x800ADA94 |
| libapi | `cdrom:PSX.EXE;1` (0x800AFDE0), BIOS call stubs | 0x800ADFC0-0x800AF340 |

The old order guess (libpress, libcd, libsnd, libspu, libgpu, libapi) is refined above: libgs sits between libspu and libgpu, libgte after libgpu, then libetc, then libapi/libcard/libc objects interleaved.

Build side: [[build-system]] (per-object asm assembly without gas padding). Executable layout: [[executable]]. Ticket: [[tickets/T-0010-sdk-lib-object-boundaries]].
