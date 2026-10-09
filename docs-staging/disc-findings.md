# Disc findings: Tokimeki Memorial - Forever with You (Japan, PlayStation the Best)

## Layout
- `disc/` (gitignored). Track 1 = `disc/track1.bin` (raw MODE2/2352, 298500 sectors), Track 2 = `disc/track2.bin` (CD-DA), cue edited to match.
- Volume ID `VX009J2`, publisher KONAMI_KCET, dated 1997-12-25.
- **dumpsxiso (mkpsxiso 2.30) does not extract the tree.** It writes only 4 root files plus 32 empty directories (it reports a bogus file count, and the `CDROM` directory sector has a non-standard layout). The ISO9660 tree is valid, though: 1368 files. A small Python parser plus extractor reads the directory records and sector subheaders and writes everything to `disc/files/` (~598 MB). The scripts are not committed; re-create them if needed. Rules:
  - The directory sector at LBA 14515 has no subheader, so read it at offset +16.
  - Form1 sectors give 2048 bytes at +24.
  - Form2 sectors give 2324 bytes at +24 (XA files).
- `CDROM/DADIR/TR02.DA` points at LBA 298650, past the end of Track 1. It is a CD-DA track reference with no data in the bin, and was skipped.
- SYSTEM.CNF: `BOOT = cdrom:SLPM_86.053;1`, `TCB = 8`, `EVENT = 32`, `STACK = 801FFF00`.

## Files
| path | size | type |
|---|---|---|
| SLPM_86.053 | 667,648 | PS-X EXE: the only real executable |
| SYSTEM.CNF | 67 | text |
| KONAMIC3.STR | 3,924,480 | XA/STR video (Konami logo) |
| OPENXA.STR | 29,048,160 | XA/STR video (opening) |
| CDROM/EXEDIR/*.EXN (26) + O.BIN | 196,608 (0x30000) each | **overlays**: headerless MIPS code, zero-padded to 0x30000. 26 files: BUNKAKEN, BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, MASTER, NAME_ENT, OLH, OMIMAI, OPTION, RENSYU, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TEL, TT, VALEN |
| CDROM/EXEDIR/O.BIN | 196,608 | different format (starts `62 01 04 00 ...`); looks like a data or table blob, not code |
| CDROM/XA_G/*.XA (14) | ~492 MB | XA audio (voice/BGM) |
| CDROM/BS (301), BUSTUP (164), BATTLE (34), BUNKA (46), EVENT (78), SERIFU (45), END_OBJ (34), TAIIKU, TARA_OBJ, SHIAI, MASTER, SPEAKTST, BUNKASD, EN_GAME (.BIN/.BS) | | game data (graphics, scripts, per-event data) |
| CDROM/*_SD, FUKUTEST, HOLI_SD, LASTPHOT, MAIN_SD, PROLODIR, STAFF2, TACO, NAME_ENT (.NBN/.BS/.TIM) | | scene data |
| CDROM/TMD (24), TACO (25 TMD) | | PS1 models |
| CDROM/VAB_SE (58 VH+VB pairs), VAB_SEQ (63 VH/VB/SEQ) | | sound banks / sequences |
| CDROM/TWINSGD/*.DAT | 340K | data |

31 subdirectories under `CDROM/`, 1372 files total on disc. Only SLPM_86.053 has a `PS-X EXE` header, so the overlays are the `.EXN` files (all exactly 0x30000 bytes).

Overlay load address is unverified. In TT.EXN and OPTION.EXN, `jal` targets cluster at 0x8013xxxx-0x8014xxxx and also hit the main exe at 0x8004xxxx. A reasonable guess is that overlays load at about 0x80130000 and the slot is 0x30000 wide. Confirm by reading the main exe's loader code, where the string `EXEDIR` appears.

## Boot exe: SLPM_86.053
- SHA1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26` (recorded in `config/SLPM_86.053.sha1`).
- Header, 0x800 bytes, then code:
  - entry PC `0x800420D0`
  - gp `0x800EB670`
  - text addr `0x80041000`
  - text size `0xA2800` (665,600), so it ends at `0x800E3800`
  - header 0x20/0x24 holds `0x800AF340` / `0x3EE0` (in the data-address slot; likely the bss area, inside text range); header 0x28/0x2C (bss) and 0x30/0x34 (sp) are zero, so the stack comes from SYSTEM.CNF (`801FFF00`)
  - region string "Sony Computer Entertainment Inc. for Japan area"

## SDK / compiler guesses
- Embedded RCS ids: libgpu `sys.c,v 1.107 1995/10/18`, `intr.c,v 1.71 1995/08/29`, `ut_f.c,v 1.2 1995/03/13`, plus `Copyright (C) by 1993, 1994 Sony Computer Entertainment Inc.`. Libcd (`CdSearchFile: searching`, `CD_newmedia`, `CdInit`), libpress (`MDEC_*`) and `cdrom:PSX.EXE;1` (libapi) strings are all present.
- There are no "PsyQ" or "Library Programs" version strings. The library dates point to a late-1995 library snapshot, so **PsyQ 3.6-4.0 era libs, likely linked with an early v4.x SDK**. Pin it down by matching libgpu/libcd functions.
- Compiler: almost certainly Sony's GCC 2.7.2-based CCPSX (`gcc-2.7.2-psx` in the image), with 2.6.3-psx and 2.8.x as fallbacks. Use the `.file 1 "stdin"` and `.rdata`/`$LC` patterns plus maspsx (`--aspsx-version`) to settle it per function.

## Toolchain (tools/Dockerfile)
- Image `tokimemo-decomp`, base ubuntu:22.04, **forced linux/amd64**. The old-gcc PsyQ builds are x86_64-only, so every tool runs under one arch. On Apple Silicon this runs under Rosetta/QEMU emulation.
- Pinned versions:
  - splat64 0.50.0
  - spimdisasm 1.42.4
  - rabbitizer 1.16.2
  - pycparser 3.11
  - colorama 0.4.6
  - m2c 0.1.0 (git HEAD at build time, not pinned)
  - binutils-mips-linux-gnu 2.38 (apt)
  - ninja 1.10.1, make 4.3, git 2.34.1
  - maspsx @ `e85ecb3`
  - objdiff-cli v3.8.2
  - mkpsxiso/dumpsxiso v2.30
  - decompals/old-gcc release 0.17, in `/opt/gcc/<ver>/` with `cc1`, `gcc`, `cpp`: 2.6.3-psx, 2.7.2-psx, 2.7.2-cdk, 2.8.0-psx, 2.8.1-psx, 2.91.66-psx, 2.95.2-psx
  - env: `MASPSX_DIR=/opt/maspsx`, `GCC_DIR=/opt/gcc`
- Usage: `tools/docker.sh <cmd>` builds the image if missing, then runs the command with the repo at `/work` as your uid/gid. With no args it opens a shell.
  - Example: `tools/docker.sh python3 -m splat split config/x.yaml`
  - Example: `tools/docker.sh /opt/gcc/2.7.2-psx/cc1 -O2 -quiet foo.c -o foo.s`
