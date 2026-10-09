---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md", "config/SLPM_86.053.yaml"]
---

# PsyQ SDK findings (T-0006)

## Version evidence
- RCS ids in the rodata/data: libgpu `sys.c,v 1.107 1995/10/18` (0x800B2E50), `intr.c,v 1.71 1995/08/29` (0x800B3170), `ut_f.c,v 1.2 1995/03/13` (0x800DCB50), `Copyright (C) by 1993, 1994 Sony Computer Entertainment Inc.` (0x800DCB85).
- No "PsyQ" or "Library Programs" version string. Late-1995 library snapshot, so PsyQ 3.6-4.0 era libs, probably linked with an early v4.x SDK. Not pinned to an exact release.

## Libraries present (anchors are code addresses that reference the library's strings)
| library | evidence | code anchor |
|---|---|---|
| libpress (MDEC) | `MDEC_rest:bad option`, `MDEC_in_sync`, `MDEC_vlec: invalid VLC ID` (rodata 0x800B23B0) | 0x80086AC4; API entry points 0x80086810-0x800869C8 are called from game code at 0x800536xx |
| libcd | `CdInit: Init failed`, `CdSearchFile: searching %s...`, `Cdl*` command names (0x800B2480-0x800B27E4) | 0x80087794 (CdInit area) |
| libsnd | `Can't Open Sequence data any more` (0x800B2930) | 0x800900C8 |
| libspu | `SPU:T/O [%s]` (0x800DCC24) | 0x800969DC |
| libgpu | `ResetGraph`, `DrawSync`, `LoadImage` (0x800B2E84-0x800B2F84), `VSync: timeout` (0x800B31E0) | 0x8009C240 (ResetGraph), 0x800ADAE4 (VSync) |
| libapi | `cdrom:PSX.EXE;1` (0x800AFDE0) and the BIOS call stubs | stubs 0x800ADFC0-0x800AF340 |

libapi stubs: 26 tiny functions of the form `addiu $t2,$zero,0xA0|0xB0|0xC0; jr $t2; addiu $t1,$zero,N`, plus `EnterCriticalSection` (`syscall` with a0=1, at 0x800AEFE0) and `ExitCriticalSection` (a0=2, at 0x800AF000).

## Link-order observation
Rodata order (game strings, MDEC, Cd, snd, gpu) matches the text order, so the libs sit at the end of `.text` in the order libpress, libcd, libsnd, libspu, libgpu, libapi. Switch tables and strings of each object are adjacent in rodata.

## Region used in the split
`sdk_libs` = 0x80086810-0x800ADFC0 and `libapi_stubs` = 0x800ADFC0-0x800AF340 are asm-only segments. The real start of libpress is probably earlier (the switch table at rodata 0x800B2398 is referenced from 0x80085CE4, directly before the MDEC strings; tables at 0x800B22F8-0x800B236C are referenced from 0x80083B40-0x80084540). Object-level boundaries and exact versions remain open: [[tickets/T-0010-sdk-lib-object-boundaries]].
