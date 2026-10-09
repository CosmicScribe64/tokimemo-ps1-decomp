---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md"]
---

# Disc layout

Tokimeki Memorial - Forever with You (Japan, PlayStation the Best). Volume ID `VX009J2`, publisher KONAMI_KCET, dated 1997-12-25. Findings ingested from `raw/disc-findings.md`.

## Images
- The game zip holds `(Track 1).bin` (MODE2/2352, 298500 sectors, data) and `(Track 2).bin` (CD-DA).
- Reproducible extraction: `tools/docker.sh python3 tools/extract_disc.py "<game>.zip" disc` writes `disc/track1.bin`, `disc/track2.bin`, `disc/game.cue` and every ISO9660 file to `disc/files/` (1355 files extracted; 17 skipped: the CD-DA reference and the XA/STR streams, whose Form2 sectors cannot be cut to the ISO size and are not needed for the decomp). Everything under `disc/` is gitignored.
- `dumpsxiso` (mkpsxiso 2.30) cannot extract the tree: the `CDROM` directory sector (LBA 14515) has no valid XA subheader. `tools/extract_disc.py` reads such a sector at +16 and otherwise follows the sector rules below.
- Sector rules (offset LBA*2352): 16-byte sync+header, 8-byte subheader; Form1 gives 2048 bytes at +24, Form2 (submode bit 0x20) gives 2324 bytes at +24.
- `CDROM/DADIR/TR02.DA` points past the end of Track 1 (a CD-DA reference, no data in the bin), so it is skipped.
- `SYSTEM.CNF`: `BOOT = cdrom:SLPM_86.053;1`, `TCB = 8`, `EVENT = 32`, `STACK = 801FFF00`.

## Files (1372 on disc)
| path | notes |
|---|---|
| `SLPM_86.053` | 667,648 bytes, the only PS-X EXE, see [[executable]] |
| `SYSTEM.CNF` | 67 bytes |
| `OPENXA.STR`, `KONAMIC3.STR` | XA/STR video |
| `CDROM/EXEDIR/*.EXN` (26) | headerless overlays, 0x30000 bytes each, see [[overlays]] |
| `CDROM/EXEDIR/O.BIN` | 0x30000 bytes, not code (starts `62 01 04 00`) |
| `CDROM/XA_G/*.XA` (14) | XA audio, about 492 MB |
| `CDROM/BS`, `BUSTUP`, `BATTLE`, `BUNKA`, `EVENT`, `SERIFU`, `END_OBJ`, ... | game data (graphics, scripts, per-event data) |
| `CDROM/TMD`, `CDROM/TACO` | PS1 models |
| `CDROM/VAB_SE`, `CDROM/VAB_SEQ` | sound banks and sequences |

31 subdirectories under `CDROM/`.
