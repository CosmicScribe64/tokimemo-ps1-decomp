---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md", "config/SLPM_86.053.yaml"]
---

# Boot executable SLPM_86.053

PS-X EXE, 667,648 bytes (0xA3000), SHA1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26` (`config/SLPM_86.053.sha1`). Split with `config/SLPM_86.053.yaml`; see [[build-system]].

## Header (0x800 bytes)
| offset | value | meaning |
|---|---|---|
| 0x08 | 0x0006EB40 | file offset of the rodata/data group (non-standard field, matches the 0x20 value below) |
| 0x10 | 0x800420D0 | entry PC |
| 0x14 | 0x800EB670 | initial gp |
| 0x18/0x1C | 0x80041000 / 0xA2800 | text address / size (file offset 0x800) |
| 0x20/0x24 | 0x800AF340 / 0x3EE0 | start and size of `.rodata` (hypothesis, fits exactly: rodata ends at 0x800B3220) |
| 0x28..0x34 | 0 | no bss or sp set in the header; sp comes from the entry code (`SetSp(0x801FEFF0)`) |
| 0x4C | "Sony Computer Entertainment Inc. for Japan area" | region string |

file offset = vram - 0x80041000 + 0x800.

## Memory map (verified by a matching rebuild)
| vram | file offset | section | notes |
|---|---|---|---|
| 0x80041000-0x800AF340 | 0x800-0x6EB40 | .text | 831 game functions at 0x80041000-0x80086810, then the PsyQ libs (libpress ... libgte, libetc) and the libapi/libc objects from 0x800ADFC0 (see [[psyq-sdk]]) |
| 0x800AF340-0x800B3220 | 0x6EB40-0x72A20 | .rodata | game strings (Shift-JIS) and switch tables first, then lib strings/tables (MDEC, Cd, RCS ids) |
| 0x800B3220-0x800E3800 | 0x72A20-0xA3000 | .data | includes lib data (`ut_f.c` id at 0x800DCB50, "SPU:T/O" at 0x800DCC24); no code uses `$gp`, so no `.sdata` split is needed for matching |
| 0x800E3800-0x8012B538 | not in file | .bss | size 0x47D38 |

Entry code (0x800420D0): `SetSp(0x801FEFF0)`, `InitHeap`-style call with (0x801FF800, 0x7F0), `bzero(0x800E60A0, 0x45498)` (BIOS A0:28h), then `GetGp` and the game main at 0x80042134. The bzero range ends at 0x8012B538, which fixes the bss end. Code also references 0x800E3800-0x800E60A0 (about 35 distinct addresses), so the bss is taken to start at the end of the file image, 0x800E3800. The first 0x28A0 bytes are not cleared by the entry bzero (they may be initialised at run time).

gp 0x800EB670 minus 0x8000 is 0x800E3670, which would be the start of a small-data area at the end of `.data` (words `0x40, 1`, then zeros). Unverified; not needed for the sha1.

## Segment layout in the splat config
`header` (0x0), `main` code at 0x800/vram 0x80041000 with subsegments: `game` (c, 0x800), 65 asm subsegments from file offset 0x46010 (vram 0x80086810) to 0x800AF340, one per SDK library or object (`libpress_*`, `libcd_*`, `libsnd`, `libspu`, `libgs`, `libgpu`, `libgte_*`, `libetc`, `libapi_*`, `libcard_card`, `libc_*`), `rodata` (0x6EB40), `data` (0x72A20), `bss` (vram 0x800E3800). The game/SDK boundary 0x80086810 (`DecDCTReset`) was confirmed by signature matching; per-lib boundaries and evidence are in [[psyq-sdk]] ([[tickets/T-0010-sdk-lib-object-boundaries]]).

Strings are Shift-JIS; splat's ASCII mode leaves them as `.word` data so the assembler emits the original bytes. A UTF-8 conversion would change the bytes (found while bringing up the build).
