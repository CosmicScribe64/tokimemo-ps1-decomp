---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md", "config/SLPM_86.053.yaml", "config/symbol_addrs_sdk.txt", "tools/psyq_sigmatch.py"]
---

# PsyQ SDK findings (T-0006, T-0010)

## Verdict
- No single release could be pinned. Library by library the executable matches different PsyQ vintages (libgte 3.4 and libc 3.3 with high confidence, others weak); the game shipped Nov 1995 and the newest RCS id is 1995/10/18, which favours 3.x over 4.x (circumstantial). Evidence per library is in the table below; the open conflict is [[tickets/T-0302-sdk-version-conflict]].
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

## Per-library evidence and confidence
| library | evidence | candidate | confidence |
|---|---|---|---|
| libgte | `COR_00.OBJ` is 432 bytes at 0x8009F8AC (384 in 3.3, 460 in 3.5+); `MTX.OBJ` (1216) and `MSC01.OBJ` (288) exist only in 3.3/3.4 | 3.4 | high (sizes unique to 3.4) |
| libc | `SPRINTF.OBJ` 3456 bytes, `MEMMOVE.OBJ` 144 bytes; 3.4 has 2880/128, 3.5 2140/108 | 3.3 | high for these two objects |
| libcard | `CARD.OBJ` 48 bytes at 0x800AE030 | 3.3 | low (one tiny object) |
| libapi | `COUNTER.OBJ` 368 bytes (3.0, 3.3, 4.1+); `_96_remove` is the 16-byte form (<= 3.5) | <= 3.5 | medium |
| libgpu | `get_mode`/`get_ofs` chunk matches only in 3.3-3.61; `SYS.OBJ` begins with ResetGraph | <= 3.61 | low (two chunks) |
| libcd | `CdComstr`, `CdIntstr` match 3.5+, `CdDataCallback` 3.7+; `EVENT.OBJ` 256 bytes (4.1/4.2) vs 264 (<= 3.7) | >= 3.5 | low, contradicts libgte/libc |
| libsnd | `UT_VVOL` chunk matches 3.61-4.0; whole-object fuzzy scores for `UT_GVA`/`UT_GPA` only 0.91/0.87 against 4.1+ (a mismatch, not a match) | unknown | none |
| libpress | `DecDCTReset` instruction order equals 4.1+ sets; no whole `LIBPRESS`/`VLC`/`ENCSPU` object of any set matches | unknown | none |

Open conflict (not explained away): libgte (3.4) and libc/libcard (3.3) are older than libcd (>= 3.5, possibly 3.7+). A single release cannot be pinned from the signature sets. The dating context (Nov 1995 release, newest RCS id 1995/10/18) points to 3.x rather than 4.x; this is circumstantial, and the signature sets lack 3.1/3.2/3.6.0/3.8, so a missing set may also explain unmatched libs. Narrowed from the old "3.6-4.0" guess to "3.x, late 1995, mixed vintages". Follow-up: [[tickets/T-0302-sdk-version-conflict]].

## Game/SDK boundary
The first SDK function is `DecDCTReset` at 0x80086810 (first function of `LIBPRESS.OBJ` in every signature set). This is the former heuristic start and it is correct: libpress does not start lower. The switch table at 0x800B2398 belongs to the game function `func_80085CD4` (IDO style, writes game globals), and the first libpress string is at 0x800B23B0. The game code (`src/main/*.c`) keeps all its functions (the ~22 decompiled ones are all below 0x8006B640). A 48-byte signature hit at 0x80042C94 (libgpu `KanjiFntClose`) is a mid-function false positive inside game code.

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

`config/symbol_addrs_sdk.txt` carries 166 real names: 61 libapi stubs (`open`, `close`, `read`, `write`, `EnterCriticalSection`, `InitHeap`, `memcpy`, `strlen`, ...), libgte (including the `GsTMDfast*` routines), libetc, libc, libcard, libgs/libgpu leaves, and libpress/libcd/libsnd entry points. `GsDrawOt` and `GsDrawOtIO` are byte-identical at 0x8009AD00 and left as `func_8009AD00`.

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

## Engine SDK layer: identified symbols (T-39314)
The engine ([[engine-porting-contract]], ticket [[tickets/T-39314-engine-sdk-layer]]) provides the SDK calls the main executable makes. Symbols without a name in `config/symbol_addrs_sdk.txt` are identified here from the asm (`asm/libgpu.s` and neighbours) by the GP0 command code and header length each initialiser writes, the offsets a function reads from its argument, the data labels it uses and the driver table it calls through (`D_800DD580` points at the driver table `D_800DD540` of libgpu `sys.c`: +0x08 queue, +0x0C clear, +0x10 GP1/GP0 word write, +0x14 FIFO send, +0x18 DMA list send, +0x1C store, +0x20 load, +0x2C OT clear, +0x34 reset, +0x38 status, +0x3C DrawSync). The names are the PsyQ ones; the engine keeps the `func_` symbol the C calls.

| symbol | PsyQ function | evidence |
|---|---|---|
| `func_8009C210` | `ResetGraph(mode)` | first function of SYS.OBJ; prints `ResetGraph`, GPU probe, calls ResetCallback; `mode & 7` selects the reset form |
| `func_8009C5E0` | `SetDispMask(mask)` | GP1(03h) with bit 0 = !mask; mask 0 invalidates the stored DISPENV |
| `func_8009C674` | `DrawSync(mode)` | calls driver +0x3C |
| `func_8009C6D8` | rectangle check used by the transfer calls | prints through `printf` for an invalid RECT |
| `func_8009C7F8` / `C884` / `C8E0` / `C93C` | `ClearImage` / `LoadImage` / `StoreImage` / `MoveImage` | strings `ClearImage`, `LoadImage`, `StoreImage`, `MoveImage` at 0x800B2F78..; drivers +0x0C, +0x20, +0x1C, +0x18 |
| `func_8009C9F8` / `CAAC` | `ClearOTag` / `ClearOTagR` | forward chain ending in the node at `D_800DD644` (0x04FFFFFF); reverse table via the OTC driver +0x2C |
| `func_8009CB3C` / `CB9C` | `DrawPrim` / `DrawOTag` | DrawSync(0) then FIFO send +0x14; DMA list send +0x18 |
| `func_8009CC04` / `CCBC` / `CCF4` / `D10C` | `PutDrawEnv` / `GetDrawEnv` / `PutDispEnv` / `GetDispEnv` | 0x5C-byte DRAWENV copy at `D_800DD598`, 0x14-byte DISPENV copy at `D_800DD5F4`; `D2E8` builds the DR_ENV packet at env+0x1C |
| `func_8009D294` | `SetDrawMode(p, dfe, dtd, tpage, tw)` | header length 2, E1 word from `get_mode`, E2 word from `D7C4` |
| `func_8009ECB0` / `ED74` | `GetTPage(tp, abr, x, y)` / `GetClut(x, y)` | field layouts; GetTPage switches on `GetGraphType` (`D_800DD588`) |
| `AddPrim`, `AddPrims`, `SetSemiTrans`, `SetShadeTex` | same | named in `symbol_addrs_sdk.txt` |
| `func_8009EFDC` `EFF0` `F004` `F018` `F02C` `F040` `F054` `F068` | `SetPolyF3` `SetPolyFT3` `SetPolyG3` `SetPolyGT3` `SetPolyF4` `SetPolyFT4` `SetPolyG4` `SetPolyGT4` | header length 4/7/6/9/5/9/8/12 and codes 0x20/0x24/0x30/0x34/0x28/0x2C/0x38/0x3C |
| `func_8009F07C` `F090` `F0A4` | `SetSprt8` `SetSprt16` `SetSprt` | codes 0x74/0x7C/0x64 |
| `func_8009F0B8` `F0CC` `F0E0` `F0F4` | `SetTile1` `SetTile8` `SetTile16` `SetTile` | codes 0x68/0x70/0x78/0x60 |
| `func_8009F108` `F11C` `F130` `F150` `F170` `F190` | `SetLineF2` `SetLineG2` `SetLineF3` `SetLineG3` `SetLineF4` `SetLineG4` | codes 0x40/0x50/0x48/0x58/0x4C/0x5C, polylines write the 0x55555555 terminator |
| `func_8009AD00` / `func_8009824C` | libgs `GsDrawOt` / `GsSortClear` | `DrawOTag(ot->org)`; the per-buffer clear packet at 0x800E6100 linked into `ot->org` |
| `func_800AD950` | libetc `VSync(mode)` | RCS id `intr.c` 1995/08/29; `VSync: timeout` string |
| `func_800AD3F8` | libetc I_MASK write that returns the old value; the PsyQ name is inferred from behaviour (not matched to a published symbol) | used by the queue to save and restore the mask |
| `func_80098650` / `func_8009ABF0` | `GetRCnt(0xF2000001)` / `ResetRCnt(0xF2000001)` (root counter 2 = the horizontal-retrace counter), although splat files them in `libspu.s` / `libgs.s` | they only call the BIOS root-counter functions with that argument; the engine refuses both |
| `rsin` (0x800A0070), `func_800A0140` | libgte `rsin` / `rcos` | sine table at `D_800DD6C0` |
| `OpenEvent EnableEvent CloseEvent TestEvent EnterCriticalSection ExitCriticalSection InitCARD StartCARD _bu_init _card_auto _card_info _card_load _card_clear _card_status open close read write format delete rand srand abs printf` | the BIOS A/B/C-table stubs of libapi (`asm/libapi_*.s`, `libcard_card.s`) | `addiu $t2, $zero, 0xA0/0xB0/0xC0; jr $t2; addiu $t1, $zero, N` |

`rand` and `srand` are BIOS routines (A0[2f], A0[30]). The engine keeps the seed in the kernel-RAM word the captures use (OpenBIOS, `0x8000850C`); `rand` before any `srand` therefore starts from the RAM value (0 in a fresh memory), not from the retail BIOS power-on seed, which has not been observed. `printf` keeps `ret_undefined` (T-39314): the BIOS returns the character count and a restricted formatter cannot compute it for the debug formats (`%d` and others); every caller discards the value.
