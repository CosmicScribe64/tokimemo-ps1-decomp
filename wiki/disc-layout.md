---
type: concept
updated: 2026-11-01
sources: ["raw/disc-findings.md"]
---

# Disc layout

Tokimeki Memorial - Forever with You (Japan, PlayStation the Best). Volume ID `VX009J2`, publisher KONAMI_KCET, dated 1997-12-25. Findings ingested from `raw/disc-findings.md`.

## Images
- The game zip holds `(Track 1).bin` (MODE2/2352, 298500 sectors, data) and `(Track 2).bin` (CD-DA).
- Reproducible extraction: the build does it from `game/` (`tools/prepare_disc.py`, see [[build-system]]). By hand: `tools/docker.sh python3 tools/extract_disc.py "<game>.zip" disc` writes `disc/track1.bin`, `disc/track2.bin`, `disc/game.cue` and every ISO9660 file to `disc/files/` (1355 files extracted; 17 skipped: the CD-DA reference and the XA/STR streams, whose Form2 sectors cannot be cut to the ISO size and are not needed for the decomp). Everything under `disc/` is gitignored.
- `dumpsxiso` (mkpsxiso 2.30) cannot extract the tree: the `CDROM` directory sector (LBA 14515) has no valid XA subheader. `tools/extract_disc.py` reads such a sector at +16 and otherwise follows the sector rules below.
- Sector rules (offset LBA*2352): 16-byte sync+header, 8-byte subheader; Form1 gives 2048 bytes at +24, Form2 (submode bit 0x20) gives 2324 bytes at +24.
- `CDROM/DADIR/TR02.DA` points past the end of Track 1 (a CD-DA reference, no data in the bin), so it is skipped.
- Sector census of track 1 (T-39501): every sector is Mode 2 with a valid sync and header. Form 1 submodes 0x08 (file body), 0x89 (last sector of a file or directory), 0x42 (STR video), 0x09, 0x80, 0xC2 and 0x00 (empty filler: LBA 12-15 are Form 2 0x20, the gaps and the postgap are Form 1 0x00); Form 2 0x64 and 0xE4 (XA audio). No Form 2 sector carries an EDC (all zero). Every Form 1 EDC/ECC is valid, including LBA 14515. Exceptions: LBA 67338, an XA sector whose two subheader halves differ (`21 04 AB D3` / `21 04 AC 4A`); LBA 298499, the last sector, whose ECC was computed over the header too (the Mode 1 rule, not Mode 2).
- LBA 14515 in detail: the `/CDROM` directory data starts at byte 16 and the bytes at 18 and 22 (subheader submode positions) read 0x80 instead of the record's 0xB3 and 0x00, so the `.` record's extent reads 0x3880 / 0x800038B3 instead of 14515. EDC/ECC were made after the damage (mastering, not the rip).
- End of track 1: the last extent (`CDROM/XA_G/YUMI.XA`) ends at LBA 298350, followed by the 150-sector postgap of empty Form 1 sectors (298350-298499). The PVD volume space size is 317697 = 298500 + 19197, track 2 included; `CDROM/DADIR/TR02.DA` starts at 298650 (track 2 INDEX 01) with 39,008,256 bytes. Nothing in the main exe names `TR02` or `DADIR`. Moving files to the end (`tools/patch/disc.py`) inserts before the postgap and moves the postgap, track 2, the volume size and TR02.DA back.
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
